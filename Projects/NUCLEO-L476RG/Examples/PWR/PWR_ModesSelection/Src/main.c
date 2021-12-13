/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L4xx PWR HAL API to enter
  *          power modes.
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
#include <stdio.h>
#include "string.h"

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup PWR_ModesSelection
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#define RXCOMMANDSIZE   0x20
#define RXBUFFERSIZE   0x01
#define LR_ASCII_VALUE ((uint8_t)0x0A)
#define CR_ASCII_VALUE ((uint8_t)0x0D)

char RxCommand[RXCOMMANDSIZE];
uint8_t RxBuffer[RXBUFFERSIZE] = {0}; //transmitting byte per byte

char 	temp; //initialisation character
char * s;

RTC_HandleTypeDef RTCHandle;
UART_HandleTypeDef UARTHandle;

__IO uint8_t ReceiveStatus = 0;
__IO uint16_t RxCmdCounter = 0;
__IO uint8_t ReadyToReception = 0;
__IO uint8_t CmdEnteredOk  = 0;

/* Private function prototypes -----------------------------------------------*/

static void     LSE_OFF_Config(void);
static void     USART2_Config(void);
static void     DecodeReception(void);
static void     Error_Handler(void);
/* functions used in <*>_test.c */ 
void            RTC_Config(void);
void            GPIO_AnalogState_Config(void);

#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

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

  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Enable backup domain for LSE/RTC control */
  HAL_PWR_EnableBkUpAccess();
  
  /* Swith off LSE clock (to make sure RTC is off) */
  LSE_OFF_Config();
  
  /* Configure the main internal regulator output voltage (Range1 by default)*/ 
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Disable Power Clock */
  __HAL_RCC_PWR_CLK_DISABLE();

  /* Configure USART2 */
  USART2_Config(); 
  
  /* Insert 1 seconds delay */
  HAL_Delay(1000);
  
  /* Output a message on Hyperterminal using printf function */
  printf("\n\rREADY\n\r");
  printf("Please enter the test number according below options\n\r");
  printf("      0       (SHUTDOWN) \n\r"); 
  printf("      1       (STANDBY) \n\r");
  printf("      2       (STANDBY + RTC) \n\r");
  printf("      3       (STANDBY + RTC + SRAM2) \n\r");
  printf("      4       (STOP2) \n\r");
  printf("      5       (STOP2   + RTC) \n\r");
  printf("      6       (STOP1   + MR OFF) \n\r");
  printf("      7       (STOP1   + MR OFF + RTC) \n\r");
  printf("      8       (LPSLEEP 2MHz         - FLASH OFF) \n\r");
  printf("      9       (LPRUN   2MHz         - with FLASH ART ON) \n\r");
  printf("     10       (SLEEP Range 2, 24MHz - with FLASH ART ON) \n\r");
  printf("     11       (SLEEP Range 1, 80MHz - with FLASH ART ON) \n\r");
  printf("     12       (RUN   Range 2, 24MHz - with FLASH ART ON) \n\r");
  printf("     13       (RUN   Range 1, 80MHz - with FLASH ART ON) \n\r");
  printf(" --> \n\r");
  
  while (CmdEnteredOk != 0x1) {
    ReceiveStatus = 0;
    while (ReceiveStatus != 0x1){
      /* Start the USART2 Receive process to receive the RxBuffer */
      if(HAL_UART_Receive_IT(&UARTHandle, RxBuffer, RXBUFFERSIZE) != HAL_OK)
        {
          Error_Handler();
        }
      while (!ReadyToReception);
      ReadyToReception = 0;
      DecodeReception(); 
    }
      CmdEnteredOk = 1;

      s = RxCommand;
      
      if (strcmp(s, "0\r") == 0){test_shutdown();}
      else if (strcmp(s, "1\r") == 0) {test_standby();}
      else if (strcmp(s, "2\r") == 0) {test_standby_rtc();}
      else if (strcmp(s, "3\r") == 0) {test_standby_rtc_sram2();}
      else if (strcmp(s, "4\r") == 0) {test_stop2();}
      else if (strcmp(s, "5\r") == 0) {test_stop2_rtc();}
      else if (strcmp(s, "6\r") == 0) {test_stop1_mroff();}
      else if (strcmp(s, "7\r") == 0) {test_stop1_mroff_rtc();}
      else if (strcmp(s, "8\r") == 0) {test_lpsleep_2mhz();}
      else if (strcmp(s, "9\r") == 0) {test_lprun_2mhz();}
      else if (strcmp(s, "10\r") == 0) {test_sleep_range2_24mhz();}
      else if (strcmp(s, "11\r") == 0) {test_sleep_range1_80mhz();}
      else if (strcmp(s, "12\r") == 0) {test_run_range2_24mhz();}
      else if (strcmp(s, "13\r") == 0) {test_run_range1_80mhz();}
      else {
         printf("Invalid test number.. Please enter again\n\r");
         CmdEnteredOk = 0;
         /* Reset received test number array */
         memset(RxCommand, 0, RXCOMMANDSIZE);
       }
    }
  while (1);
}


/**
  * @brief  Configure USART2(in UART mode)
  * @param  None
  * @retval None
  */
  /*
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Tx and Rx enabled
 */
static void USART2_Config(void)
{
  UARTHandle.Instance = USART2;
  UARTHandle.Init.BaudRate = 9600;
  UARTHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UARTHandle.Init.StopBits = UART_STOPBITS_1;
  UARTHandle.Init.Parity = UART_PARITY_NONE;
  UARTHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UARTHandle.Init.Mode = UART_MODE_TX_RX;
  UARTHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  UARTHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;

  if(HAL_UART_Init(&UARTHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  husart: USART handle
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
     ReadyToReception = 1;
}

/**
  * @brief  Decode recpeption of RBuffer
  * @param  None
  * @retval None
  */
void DecodeReception(void)
{
  /* Read one byte from the receive data register */
  temp = (char) RxBuffer[0];
  RxCommand[RxCmdCounter++] = temp;

  /* Check if is an LR/CR character */
  if ((temp == CR_ASCII_VALUE) || (temp == LR_ASCII_VALUE))
  {
    /* echo entered command on terminal */
    printf("You entered below test number:\n\r");
    printf(RxCommand);
    printf("\n\r");

    RxCmdCounter = 0;
     
    ReceiveStatus = 1;
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
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
void SystemClock_80MHz(void)
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
    Error_Handler();
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
    Error_Handler();
  }
}

/**
  * @brief  System Clock Speed set at 24MHz
  *         MSI range is set to RCC_MSIRANGE_9  
  *         Flash Latency set to 3 WS
  * @param  None
  * @retval None
  */
void SystemClock_24MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, update MSI to 24Mhz (RCC_MSIRANGE_9) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_9;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI; 
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
}

/**
  * @brief  System Clock set at 2MHz
  *         MSI range is set to RCC_MSIRANGE_5   
  *         Flash Latency set to 0 WS
  * @param  None
  * @retval None
  */
void SystemClock_2MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, update MSI to 2Mhz (RCC_MSIRANGE_5) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI; 
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
}

/**
  * @brief  System Clock set at 100 KHz
  *         MSI range is set to RCC_MSIRANGE_0   
  *         Flash Latency set to 0 WS
  * @param  None
  * @retval None
  */
void SystemClock_100KHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, update MSI to 100Khz (RCC_MSIRANGE_0) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_0;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI; 
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
}

/**
  * @brief  Switch OFF LSE
  * @param  None
  * @retval None
  */
void LSE_OFF_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* set LSE OFF */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  RTC Configuration
  * @param  None
  * @retval None
  */
void RTC_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* set LSE ON */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* set low drive on LSE to reduce conso */
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  
  /* configure clock RTC as LSE */
  __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE); 
  
  /* Enable RTC in RCC */
  __HAL_RCC_RTC_ENABLE();
}

/**
* @brief GPIO config for LP
* @param None
* @retval None
*/
void GPIO_AnalogState_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Set all GPIO in analog state to reduce power consumption, */
        
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();    

    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Pin = GPIO_PIN_All;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);  

    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();
    __HAL_RCC_GPIOE_CLK_DISABLE();
    __HAL_RCC_GPIOF_CLK_DISABLE();
    __HAL_RCC_GPIOG_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();

}

/**
* @brief UART MSP Init
* @param huart: uart handle
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* USART2 Configuration */
  if(huart->Instance == USART2)
  {
    /* Enable UART2 GPIO TX/RX clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    /* Enable USART2 clock */
    __HAL_RCC_USART2_CLK_ENABLE();    
    
    /* Configure USART2 Rx and Tx as alternate function  */
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM; // GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* Enable and set USART2 Interrupt to the highest priority */
    HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  }  
}


/**
  * @brief  Retargets the C library printf function to the UART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UARTHandle, (uint8_t *)&ch, 1, 0xFFFF); 

  return ch;
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn on the LED2 */
  BSP_LED_Init(LED2);

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

