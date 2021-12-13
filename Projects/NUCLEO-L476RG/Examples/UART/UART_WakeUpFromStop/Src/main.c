/**
  ******************************************************************************
  * @file    UART/UART_WakeUpFromStop/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use UART HAL API to wake up
  *          the MCU from STOP1 mode  
  *          Two boards are used, one which enters STOP1 mode and the second
  *          one which sends the wake-up stimuli.  
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup UART_WakeUpFromStop
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* to enable for the board entering STOP1 mode,
   to disable for the board sending wake-up stimuli */
#define BOARD_IN_STOP1_MODE

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;
UART_WakeUpTypeDef WakeUpSelection; 
__IO uint32_t UserButtonStatus = 0;  /* set to 1 after User Button interrupt  */

/* Buffer used for confirmation messages transmission */
uint8_t aTxBuffer1[] = "RXNE wake-up successful";
uint8_t aTxBuffer2[] = "Start bit detection wake-up successful";
uint8_t aTxBuffer3[] = "7-bit address match wake-up successful";
uint8_t aTxBuffer4[] = "4-bit address match wake-up successful";

uint8_t aTxBuffer[] = "Start bit detection wake-up successful";
uint8_t aWakeUpTrigger1[] = "R";
uint8_t aWakeUpTrigger2[] = "S";
uint8_t aWakeUpTrigger3[] = {0xA9};
uint8_t aWakeUpTrigger4[] = {0x82};

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);
#if defined(BOARD_IN_STOP1_MODE)
void SystemClock_Config_fromSTOP1(void);
#endif
static void Error_Handler(void);
#if !defined(BOARD_IN_STOP1_MODE)
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
#endif /* !defined(BOARD_IN_STOP1_MODE) */
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32L4xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 80 MHz */
  SystemClock_Config();
  
#ifdef BOARD_IN_STOP1_MODE
  /* Select HSI as wakeup clock from stop mode */
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_HSI);
#endif

  /* Configure LED2 */
  BSP_LED_Init(LED2);

#ifdef BOARD_IN_STOP1_MODE
  /* LED2 is on till stop mode */
  BSP_LED_On(LED2);
#endif
  
  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
   
  UartHandle.Instance        = USARTx;
  HAL_UART_DeInit(&UartHandle); 

  UartHandle.Init.BaudRate   = 9600;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
#ifdef BOARD_IN_STOP1_MODE
  /* wait for two seconds before test start */
  HAL_Delay(2000);
  
  /* make sure that no UART transfer is on-going */ 
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_BUSY) == SET);
  /* make sure that UART is ready to receive
   * (test carried out again later in HAL_UARTEx_StopModeWakeUpSourceConfig) */
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_REACK) == RESET);

  /* set the wake-up event:
   * specify wake-up on RXNE flag */
  WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_READDATA_NONEMPTY;
  if (HAL_UARTEx_StopModeWakeUpSourceConfig(&UartHandle, WakeUpSelection)!= HAL_OK)
  {
    Error_Handler(); 
  }
 
  /* Enable the UART Wake UP from STOP1 mode Interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);
  
  /* about to enter STOP1 mode: switch off LED */
  BSP_LED_Off(LED2);
  /* enable MCU wake-up by UART */
  HAL_UARTEx_EnableStopMode(&UartHandle);
  /* enter STOP1 mode */
  HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);

  /* ... STOP1 mode ... */
  
  
  /* at that point, MCU has been awoken: the LED has been turned back on */
  SystemClock_Config_fromSTOP1();

  /* Wake Up based on RXNE flag successful */ 
  HAL_UARTEx_DisableStopMode(&UartHandle);

  /* wait for some delay */
  HAL_Delay(100);
  /* Inform other board that wake up is successful */
  if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer1, COUNTOF(aTxBuffer1)-1, 5000)!= HAL_OK)  
  {
    Error_Handler();
  }  
  
  /*##-2- Wake Up second step  ###############################################*/
  /* make sure that no UART transfer is on-going */ 
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_BUSY) == SET);
  /* make sure that UART is ready to receive 
   * (test carried out again later in HAL_UARTEx_StopModeWakeUpSourceConfig) */
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_REACK) == RESET);
  
  /* set the wake-up event:
   * specify wake-up on start-bit detection */
  WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_STARTBIT;
  if (HAL_UARTEx_StopModeWakeUpSourceConfig(&UartHandle, WakeUpSelection)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Enable the UART Wake UP from STOP1 mode Interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);
  
  /* about to enter STOP1 mode: switch off LED */
  BSP_LED_Off(LED2);
  /* enable MCU wake-up by UART */
  HAL_UARTEx_EnableStopMode(&UartHandle);
  /* enter STOP1 mode */
  HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);

  /* ... STOP1 mode ... */
  
  
  /* at that point, MCU has been awoken: the LED has been turned back on */
  SystemClock_Config_fromSTOP1();

  /* Wake Up on start bit detection successful */ 
  HAL_UARTEx_DisableStopMode(&UartHandle);
  /* wait for some delay */
  HAL_Delay(100);
  /* Inform other board that wake up is successful */
  if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer2, COUNTOF(aTxBuffer2)-1, 5000)!= HAL_OK)
  {
    Error_Handler();
  }
  
  
  /*##-3- Wake Up third step  ################################################*/
 /* make sure that no UART transfer is on-going */ 
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_BUSY) == SET);
  /* make sure that UART is ready to receive
   * (test carried out again later in HAL_UARTEx_StopModeWakeUpSourceConfig) */
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_REACK) == RESET);
     
  /* set the wake-up event:  
   * specify address-to-match type. 
   * The address is 0x29, meaning the character triggering the 
   * address match is 0xA9 */
  WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_ADDRESS;
  WakeUpSelection.AddressLength = UART_ADDRESS_DETECT_7B;
  WakeUpSelection.Address = 0x29;  
  if (HAL_UARTEx_StopModeWakeUpSourceConfig(&UartHandle, WakeUpSelection)!= HAL_OK)
  {
    Error_Handler(); 
  }

  /* Enable the UART Wake UP from stop mode Interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);
  
  /* about to enter stop mode: switch off LED */
  BSP_LED_Off(LED2);
  /* enable MCU wake-up by UART */
  HAL_UARTEx_EnableStopMode(&UartHandle);
  /* enter STOP1 mode */
  HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);

  /* ... STOP1 mode ... */
  
  
  /* at that point, MCU has been awoken: the LED has been turned back on */
  SystemClock_Config_fromSTOP1();

  /* Wake Up on 7-bit address detection successful */ 
  HAL_UARTEx_DisableStopMode(&UartHandle);
  /* wait for some delay */
  HAL_Delay(100);
  /* Inform other board that wake up is successful */
  if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer3, COUNTOF(aTxBuffer3)-1, 5000)!= HAL_OK)  
  {
    Error_Handler();
  } 
  

  /*##-4- Wake Up fourth step  ###############################################*/
 /* make sure that no UART transfer is on-going */ 
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_BUSY) == SET);
  /* make sure that UART is ready to receive
   * (test carried out again later in HAL_UARTEx_StopModeWakeUpSourceConfig) */
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_REACK) == RESET);
    
  /* set the wake-up event:  
   * specify address-to-match type. 
   * The address is 0x2, meaning the character triggering the 
   * address match is 0x82 */
  WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_ADDRESS;
  WakeUpSelection.AddressLength = UART_ADDRESS_DETECT_4B; 
  WakeUpSelection.Address = 0x2;  
  if (HAL_UARTEx_StopModeWakeUpSourceConfig(&UartHandle, WakeUpSelection)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Enable the UART Wake UP from STOP1 mode Interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);
  
  /* about to enter STOP1 mode: switch off LED */
  BSP_LED_Off(LED2);
  /* enable MCU wake-up by UART */
  HAL_UARTEx_EnableStopMode(&UartHandle); 
  /* enter STOP1 mode */
  HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);

  /* ... STOP1 mode ... */
  
  
  /* at that point, MCU has been awoken: the LED has been turned back on */
  SystemClock_Config_fromSTOP1();

  /* Wake Up on 4-bit address detection successful */ 
  /* wait for some delay */
  HAL_Delay(100);
  /* Inform other board that wake up is successful */
  if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer4, COUNTOF(aTxBuffer4)-1, 5000)!= HAL_OK)  
  {
    Error_Handler();
  }

  
#else

/* initialize the User push-button in Interrupt mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  
  /* Wait for User push-button press before starting the test.
     In the meantime, LED2 is blinking */
  while(UserButtonStatus == 0)
  {
      /* Toggle LED2 */
      BSP_LED_Toggle(LED2);
      HAL_Delay(100);
  }
  
  
  /*##-2- Send the wake-up from stop mode first trigger ######################*/
  /*      (RXNE flag setting)                                                 */
  BSP_LED_On(LED2);
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aWakeUpTrigger1, COUNTOF(aWakeUpTrigger1)-1, 5000)!= HAL_OK)
  {
    Error_Handler();
  }
  
  /* Put UART peripheral in reception process to wait for other board
     wake up confirmation */  
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, COUNTOF(aTxBuffer1)-1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  BSP_LED_Off(LED2);
   
  /* Compare the expected and received buffers */
  if(Buffercmp((uint8_t*)aTxBuffer1,(uint8_t*)aRxBuffer,COUNTOF(aTxBuffer1)-1))
  {
    Error_Handler();
  }

  /* wait for two seconds before test second step */
  HAL_Delay(2000);
  
  /*##-3- Send the wake-up from stop mode second trigger #####################*/
  /*      (start Bit detection)                                               */
  BSP_LED_On(LED2);  
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aWakeUpTrigger2, COUNTOF(aWakeUpTrigger2)-1, 5000)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Put UART peripheral in reception process to wait for other board
     wake up confirmation */  
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, COUNTOF(aTxBuffer2)-1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  BSP_LED_Off(LED2);
   
  /* Compare the expected and received buffers */
  if(Buffercmp((uint8_t*)aTxBuffer2,(uint8_t*)aRxBuffer,COUNTOF(aTxBuffer2)-1))
  {
    Error_Handler();
  }

  /* wait for two seconds before test third step */
  HAL_Delay(2000);


  /*##-4- Send the wake-up from stop mode third trigger ######################*/
  /*      (7-bit address match)                                               */ 
  BSP_LED_On(LED2);
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aWakeUpTrigger3, 1, 5000)!= HAL_OK)
  {
    Error_Handler();
  }
 
  /* Put UART peripheral in reception process to wait for other board
     wake up confirmation */  
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, COUNTOF(aTxBuffer3)-1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  BSP_LED_Off(LED2);
   
  /* Compare the expected and received buffers */
  if(Buffercmp((uint8_t*)aTxBuffer3,(uint8_t*)aRxBuffer,COUNTOF(aTxBuffer3)-1))
  {
    Error_Handler();
  }

  /* wait for two seconds before test fourth and last step */
  HAL_Delay(2000);


  /*##-5- Send the wake-up from stop mode fourth trigger #####################*/
  /*      (4-bit address match)                                               */  
  BSP_LED_On(LED2);
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aWakeUpTrigger4, 1, 5000)!= HAL_OK)
  {
    Error_Handler();
  }
  
  
  /* Put UART peripheral in reception process to wait for other board
     wake up confirmation */  
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, COUNTOF(aTxBuffer4)-1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  BSP_LED_Off(LED2);
   
  /* Compare the expected and received buffers */
  if(Buffercmp((uint8_t*)aTxBuffer4,(uint8_t*)aRxBuffer,COUNTOF(aTxBuffer4)-1))
  {
    Error_Handler();
  }

  HAL_Delay(2000);

#endif /* BOARD_IN_STOP1_MODE */



  /* Turn on LED2 if test passes then enter infinite loop */
  BSP_LED_On(LED2);
  while (1)
  {
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}


#if defined(BOARD_IN_STOP1_MODE)
/**
  * @brief  System Clock Configuration restored after STOP1 mode
  * @param  None
  * @retval None
  */
void SystemClock_Config_fromSTOP1(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  uint32_t pFLatency = 0;

  /* Get the Oscillators & PLL configuration according to the internal RCC registers */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

  /* Wake up on HSI, re-enable MSI and PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Get the Clocks configuration according to the internal RCC registers */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);

  /* Select PLL as system clock source */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK)
  {
    Error_Handler();
  }
}
#endif

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  Error_Handler();
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == USER_BUTTON_PIN)
  {  
    UserButtonStatus = 1;
  }
}


#if !defined(BOARD_IN_STOP1_MODE)
/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}
#endif /* !defined(BOARD_IN_STOP1_MODE) */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while(1)
  {
    /* In case of error, LED2 transmits a sequence of three dots, three dashes, three dots */
    BSP_LED_On(LED2);
    HAL_Delay(300);
    BSP_LED_Off(LED2);
    HAL_Delay(300);
    BSP_LED_On(LED2);
    HAL_Delay(300);
    BSP_LED_Off(LED2);
    HAL_Delay(300);
    BSP_LED_On(LED2);
    HAL_Delay(300);
    BSP_LED_Off(LED2);
    HAL_Delay(300);
    BSP_LED_On(LED2);
    HAL_Delay(700);
    BSP_LED_Off(LED2);
    HAL_Delay(700);
    BSP_LED_On(LED2);
    HAL_Delay(700);
    BSP_LED_Off(LED2);
    HAL_Delay(700);
    BSP_LED_On(LED2);
    HAL_Delay(700);
    BSP_LED_Off(LED2);
    HAL_Delay(700);
    BSP_LED_On(LED2);
    HAL_Delay(300);
    BSP_LED_Off(LED2);
    HAL_Delay(300);
    BSP_LED_On(LED2);
    HAL_Delay(300);
    BSP_LED_Off(LED2);
    HAL_Delay(300);
    BSP_LED_On(LED2);
    HAL_Delay(300);
    BSP_LED_Off(LED2);
    HAL_Delay(800);
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

