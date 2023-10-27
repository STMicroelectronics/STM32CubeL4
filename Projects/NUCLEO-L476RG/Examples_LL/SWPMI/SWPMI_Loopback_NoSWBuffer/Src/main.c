/**
  ******************************************************************************
  * @file    Examples_LL/SWPMI/SWPMI_Loopback_NoSWBuffer/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to transfer SWPMI data through
  *          the STM32L4xx SWPMI LL API.
  *          Peripheral initialization done using LL unitary services functions.
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

/** @addtogroup STM32L4xx_LL_Examples
  * @{
  */

/** @addtogroup SWPMI_Loopback_NoSWBuffer
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SWPMI_BITRATE                     (uint32_t)2000000 /* 2Mbit/s */

/* Size of SWPMI frame (in word) */
#define SWPMI_FRAME_SIZE                  (uint32_t)8
/* Number of frames to be transferred */
#define SWPMI_FRAME_NUMBER_TO_TRANSMIT    (uint32_t)1
/* Total size of buffer used for reception (in word)*/
#define SWPMI_RECEIVEBUFFER_SIZE          (uint32_t)(SWPMI_FRAME_NUMBER_TO_TRANSMIT * SWPMI_FRAME_SIZE)

/* Latest Frame length received at the end of the test */
#define SWPMI_LATEST_RFL                  (uint32_t)0x1E

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* SWPMI_RECEIVEBUFFER_SIZE Data Buffer for multi buffer Reception */
uint32_t pReceiveBuffer[SWPMI_RECEIVEBUFFER_SIZE];

/* Transmit Data Buffer for No buffer mode Transmission.
         (frame length = 0x1E} as configured in the first data byte)*/ 
uint32_t pTransmitBuffer[] = {0x1E115100 | SWPMI_LATEST_RFL, 0x21115354, 0x21115556, 0x22225758, 0x3322595A, 0x44225B5C, 0x55225D5E, 0xFFFF5F60};

/* SWPMI_RECEIVEBUFFER_SIZE Data Buffer for expected multi buffer Reception (If no error flags detected) */
uint32_t pExpectedReceiveBuffer[] =  
                             {0x541E1151, 0x56211153, 0x58211155, 0x5A222257, 0x5C332259, 0x5E44225B, 0x6055225D, 0x0000FF5F};

/* Variable used to save the number of data bytes in the payload */
__IO uint32_t ubLastestRFL = 0;

/* Variables used for communication */
__IO uint8_t  ubDataTransmitted   = 0;
__IO uint8_t  ubDataNbReceived    = 0;
__IO uint8_t  ubEndOfTransmission = 0;

__IO uint8_t ubButtonPress = 0;

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_SWPMI(void);
void     Activate_SWPMI_Communication(void);
void     WaitAndCheckEndOfTransfer(void);
uint32_t BufferCmp32(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);
void     LED_Init(void);
void     LED_On(void);
void     LED_Blinking(uint32_t Period);
void     UserButton_Init(void);
void     WaitForUserButtonPress(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 80 MHz */
  SystemClock_Config();

  /* Initialize LED2 */
  LED_Init();

  /* Initialize button in GPIO mode */
  UserButton_Init();
  
  /* Configure SWMPI */
  Configure_SWPMI();

  /* Wait Until USER BUTTON is pressed */
  /* LED blinking FAST during waiting time */
  WaitForUserButtonPress();

  /* Handle the SWPMI communication */
  Activate_SWPMI_Communication();

  /* Wait for the end of the transfer and check received data */
  /* LED blinking with period of 1 second if error else ON if no problem */
  WaitAndCheckEndOfTransfer();

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function configures SWPMI
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_SWPMI(void)
{
  /* Configure the SWPMI peripheral */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SWPMI1);
  
  /* By default, SWPMI is clocked with PCLK1 at 80 MHz */
  //LL_RCC_SetSWPMIClockSource(LL_RCC_SWPMI1_CLKSOURCE_PCLK1);
  
  /* NVIC configuration for SWPMI1 interrupt */
  NVIC_SetPriority(SWPMI1_IRQn, 0);   
  NVIC_EnableIRQ(SWPMI1_IRQn);    

  /* Enable loopback mode */
  LL_SWPMI_EnableLoopback(SWPMI1);

  /* SWPMI configured as follow:
      - SWP class selection: SWPMI_IO uses an internal voltage regulator to operate in class B.
      - BitRate: SWPMI_freq = SWPMI_clk / (((BitRate) + 1)  * 4)
      - Tx/Rx Buffering mode = Don't care */
  LL_SWPMI_SetVoltageClass(SWPMI1, LL_SWPMI_VOLTAGE_CLASS_B);
  LL_SWPMI_SetBitRatePrescaler(SWPMI1, __LL_SWPMI_CALC_BITRATE_PRESCALER(SWPMI_BITRATE, SystemCoreClock));
  
  /* To detect errors immediately, following IT can be enabled */
  /* Under run during payload transmission */
  LL_SWPMI_EnableIT_TXUNR(SWPMI1);
  /* Overrun during payload reception */
  LL_SWPMI_EnableIT_RXOVR(SWPMI1);
  /* CRC error during payload reception */
  LL_SWPMI_EnableIT_RXBER(SWPMI1);
}

/**
  * @brief  This function handles the SWPMI communication in loopback mode
  * @param  None
  * @retval None
  */
void Activate_SWPMI_Communication(void)
{
  /*  Activate Single wire protocol bus (SUSPEND or ACTIVATED state) */
  LL_SWPMI_Activate(SWPMI1);
  
  /* Enable RXBF Interrupt */
  LL_SWPMI_EnableIT_RXBF(SWPMI1);
  /* Enable data reception interrupt*/
  LL_SWPMI_EnableIT_RX(SWPMI1);
  
  /* Enable TXBE interrupt */
  /* Automatically, SWPMI_TransmitBufferEmpty_Callback() is called and then start 
     SWPMI transfer */
  LL_SWPMI_EnableIT_TXBE(SWPMI1);
}

/**
  * @brief  Wait end of transfer and check if received Data are correct.
  * @param  None 
  * @retval None
  */
void WaitAndCheckEndOfTransfer(void)
{
  /* Wait for end of transfer */
  /* End is detected when transfer complete flag is set */
  while (ubEndOfTransmission == 0)
  {
  } 
  
  /* SWPMI DeInit */    
  LL_SWPMI_Deactivate(SWPMI1);
  
  /* Disable loopback mode */
  LL_SWPMI_DisableLoopback(SWPMI1);
  
  /* Check that latest receive frame length */
  if (ubLastestRFL != SWPMI_LATEST_RFL)
  {
      LED_Blinking(LED_BLINK_ERROR);
  }

  /* Compare the transmit and receive data */
  if (BufferCmp32(pReceiveBuffer, pExpectedReceiveBuffer, SWPMI_RECEIVEBUFFER_SIZE))
  {
      LED_Blinking(LED_BLINK_ERROR);
  }
  else
  {
    /* No error detected during communication */
    LED_On();
  }
}

/**
  * @brief  Compares two 32-bit buffers and returns the comparison result
  * @param  pBuffer1: pointer to the source buffer to be compared to.
  * @param  pBuffer2: pointer to the second source buffer to be compared to the first.
  * @retval
  *    - 0: Comparison is OK (the two Buffers are identical)
  *    - Value different from 0: Comparison is NOK (Buffers are different)
  */
uint32_t BufferCmp32(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  Initialize LED2.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED2 Clock */
  LED2_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED2 */
  LL_GPIO_SetPinMode(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Turn-on LED2.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED2 on */
  LL_GPIO_SetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

/**
  * @brief  Set LED2 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);  
    LL_mDelay(Period);
  }
}

/**
  * @brief  Configures User push-button in GPIO Mode.
  * @param  None  
  * @retval None
  */
void UserButton_Init(void)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();
  
  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO);

  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn, 0x03);  
  
  /* Connect External Line to the GPIO*/
  USER_BUTTON_SYSCFG_SET_EXTI();
  
  /* Enable a rising trigger EXTI line 13 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();
}

/**
  * @brief  Wait for user push button press to start transfer.
  * @param  None 
  * @retval None
  */
void WaitForUserButtonPress(void)
{
  while (ubButtonPress == 0)
  {
    /* Fast LED Toggle in waiting user action */
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);  
    LL_mDelay(LED_BLINK_FAST);
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
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* MSI configuration and activation */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
  LL_RCC_MSI_Enable();
  while(LL_RCC_MSI_IsReady() != 1) 
  {
  };
  
  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 40, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1) 
  {
  };
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) 
  {
  };
  
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 80MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ(__LL_RCC_CALC_MSI_FREQ(LL_RCC_MSIRANGESEL_RUN, LL_RCC_MSIRANGE_6), 
                                  LL_RCC_PLLM_DIV_1, 40, LL_RCC_PLLR_DIV_2)*/
  LL_Init1msTick(80000000);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(80000000);
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Function to manage Button push
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  ubButtonPress = 1;  
}

/**
  * @brief  Transmit buffer empty callback
  * @note   This function is executed when the Transmit buffer empty interrupt
  *         is generated
  * @retval None
  */
void SWPMI_TransmitBufferEmpty_Callback(void)
{
  if (ubDataTransmitted == 0)
  {
    LL_SWPMI_TransmitData32(SWPMI1, pTransmitBuffer[0]);
    /* Enable data Transmit interrupt */
    LL_SWPMI_EnableIT_TX(SWPMI1);
    /* Enable Transfer complete interrupt */
    LL_SWPMI_ClearFlag_TC(SWPMI1);
    LL_SWPMI_EnableIT_TC(SWPMI1);

    ubDataTransmitted++;
  }
  else
  {
    /* No more frames to transmit, can disable interrupts */
    /* Disable Transmit interrupt */
    LL_SWPMI_DisableIT_TX(SWPMI1);
    /* Disable Transfer complete interrupt */
    LL_SWPMI_DisableIT_TXBE(SWPMI1);
  }
}

/**
  * @brief  Callback to manage sent of new payload data
  * @note   This function is executed when the Transmit data register empty interrupt
  *         is generated
  * @retval None
  */
void SWPMI_TransmitData_Callback(void)
{
  LL_SWPMI_TransmitData32(SWPMI1, pTransmitBuffer[ubDataTransmitted]);
  ubDataTransmitted++;
}

/**
  * @brief  Receive buffer full callback
  * @note   This function is executed when Receive buffer full interrupt
  *         is generated
  * @retval None
  */
void SWPMI_ReceiveBufferFull_Callback(void)
{
  /* Once the complete frame has been received, including the CRC and the End of frame
     (EOF), both RXNE and RXBFF flags are set in the SWPMI_ISR register */
  
  /* Read the last byte(s) of the payload */
  pReceiveBuffer[ubDataNbReceived] = LL_SWPMI_ReceiveData32(SWPMI1);
  
  /* Clear the SWPMI Transmit buffer empty Flag */
  LL_SWPMI_ClearFlag_RXBF(SWPMI1);
  
  /* Save number of data bytes in the payload */
  ubLastestRFL = LL_SWPMI_GetReceiveFrameLength(SWPMI1);
  
  /* Increment the number of data received */
  ubDataNbReceived++;
  
  /* Disable reception interrupts */
  LL_SWPMI_DisableIT_RXBF(SWPMI1);
  LL_SWPMI_DisableIT_RX(SWPMI1);
}

/**
  * @brief  Receive data callback
  * @note   This function is executed when Receive data register not empty interrupt
  *         is generated
  * @retval None
  */
void SWPMI_ReceiveData_Callback(void)
{
  pReceiveBuffer[ubDataNbReceived] = LL_SWPMI_ReceiveData32(SWPMI1);
  /* Increment the number of data received */
  ubDataNbReceived++;
}

/**
  * @brief  End of transfer callback
  * @note   This function is executed when Transfer complete interrupt
  *         is generated
  * @retval None
  */
void SWPMI_EndTransfer_Callback(void)
{
  /* Increment the number of frames received */
  ubEndOfTransmission = 1;
}

/**
  * @brief  Transfer error callback
  * @note   This function is executed when error interrupt
  *         is generated (TX UND, RX OVR, RX CRC)
  * @retval None
  */
void SWPMI_TransferError_Callback(void)
{
  /* LED Blinking with 1sec period */
  LED_Blinking(LED_BLINK_ERROR);
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

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

