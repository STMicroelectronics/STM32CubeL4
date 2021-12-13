/**
  ******************************************************************************
  * @file    Examples_LL/SWPMI/SWPMI_Loopback_MultiSWBuffer_Init/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to transfer SWPMI data through
  *          the STM32L4xx SWPMI LL API.
  *          Peripheral initialization done using LL initialization function.
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

/** @addtogroup SWPMI_Loopback_MultiSWBuffer_Init
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SWPMI_BITRATE                     (uint32_t)2000000 /* 2Mbit/s */

/* Size of SWPMI frame (in word) */
#define SWPMI_FRAME_SIZE                  (uint32_t)8
/* Number of frames to be transferred */
#define SWPMI_FRAME_NUMBER_TO_TRANSMIT    (uint32_t)9
/* Number of frames in the Multi buffer used by DMA */
#define SWPMI_FRAME_NUMBER_IN_MULTIBUFFER (uint32_t)6
/* Number of frames to copy in the multi buffer */
#define SWPMI_NB_FRAMES_TO_COPY            (SWPMI_FRAME_NUMBER_TO_TRANSMIT - SWPMI_FRAME_NUMBER_IN_MULTIBUFFER)
/* Total size of Multi buffer (in word) */
#define SWPMI_MULTIBUFFER_SIZE            (uint32_t)(SWPMI_FRAME_NUMBER_IN_MULTIBUFFER * SWPMI_FRAME_SIZE)
/* Total size of buffer used for reception (in word)*/
#define SWPMI_RECEIVEBUFFER_SIZE          (uint32_t)(SWPMI_FRAME_NUMBER_TO_TRANSMIT * SWPMI_FRAME_SIZE)

/* Latest Frame length received at the end of the test */
#define SWPMI_LATEST_RFL                  (uint32_t)0x07

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* SWPMI_RECEIVEBUFFER_SIZE Data Buffer for multi buffer Reception */
uint32_t pReceiveBuffer[SWPMI_RECEIVEBUFFER_SIZE];

/* SWPMI_MULTIBUFFER_SIZE Data Buffer for multi buffer mode Transmission (SWPMI_FRAME_NUMBER_IN_MULTIBUFFER frames).
         (variable frame lengths = {0x1E, 0x03, 0x04, 0x1E, 0x0E, 0x15} as configured in the first data byte)*/ 
uint32_t pMultiBuffer[] =  
                               {0x1E11511E, 0x21115354, 0x21115556, 0x22225758, 0x3322595A, 0x44225B5C, 0x55225D5E, 0xFFFF5F60,
                                0x03030303, 0x00000000, 0x000FF000, 0x000FF000, 0x000000FF, 0xFF000000, 0x000000FF, 0x00000FF0,
                                0x04040404, 0x00000004, 0x000FF000, 0x000FF000, 0x000000FF, 0xFF000000, 0x000000FF, 0x00000FF0,
                                0x1E11511E, 0x21115354, 0x21115556, 0x22225758, 0x3322595A, 0x44225B5C, 0x55225D5E, 0xFFFF5F60,
                                0x1414140E, 0x14141414, 0x14141414, 0x00141414, 0x000000FF, 0x000000FF, 0xFF000000, 0xFF000000,
                                0x25252515, 0x25252525, 0x25252525, 0x25252525, 0x25252525, 0x25252525, 0x25252525, 0xFFFF2525};
/* Data Buffer used to transmit latest frames (SWPMI_NB_FRAMES_TO_COPY frames).
         (variable frame lengths = {0x05, 0x06, 0x07} as configured in the first data byte)*/ 
uint32_t pLastFramesBuffer[] =  
                               {0xCCBBAA05, 0x0000EEDD, 0x000FF000, 0x000FF000, 0x000000FF, 0xFF000000, 0x000000FF, 0x00000FF0,
                                0x06060606, 0x00060606, 0x000FF000, 0x000FF000, 0x000000FF, 0xFF000000, 0x000000FF, 0x00000FF0,
                                0x07070700 | SWPMI_LATEST_RFL, 0x07070707, 0x000FF000, 0x000FF000, 0x000000FF, 0xFF000000, 0x000000FF, 0x00000FF0};

/* SWPMI_RECEIVEBUFFER_SIZE Data Buffer for expected multi buffer Reception (If no error flags detected) */
uint32_t pExpectedReceiveBuffer[] =  
                               {0x541E1151, 0x56211153, 0x58211155, 0x5A222257, 0x5C332259, 0x5E44225B, 0x6055225D, 0x041EFF5F,
                                0x00030303, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x04030000,
                                0x04040404, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x04040000,
                                0x541E1151, 0x56211153, 0x58211155, 0x5A222257, 0x5C332259, 0x5E44225B, 0x6055225D, 0x041EFF5F,
                                0x14141414, 0x14141414, 0x14141414, 0x00001414, 0x00000000, 0x00000000, 0x00000000, 0x040E0000,
                                0x25252525, 0x25252525, 0x25252525, 0x25252525, 0x25252525, 0x00000025, 0x00000000, 0x04150000, 
                                0xDDCCBBAA, 0x000000EE, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x04050000,
                                0x06060606, 0x00000606, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x04060000,
                                0x07070707, 0x00070707, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x04000000 | SWPMI_LATEST_RFL << 16};

__IO uint8_t  ubFramesNbTransmitted = 0;
__IO uint8_t  ubFramesNbReceived = 0;

__IO uint8_t ubButtonPress = 0;

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_DMA(void);
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
  
  /* Configure DMA channels for TX/RX SWPMI  */
  Configure_DMA();

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
  * @brief  This function configures the DMA2 Channels to copy data from 
  *         Flash memory(pMultiBuffer) to SWPMI1(TDR) 
  *         and then SWPMI1(RDR) to Flash memory(pReceiveBuffer).
  * @note  This function is used to :
  *        -1- Enable DMA2 clock
  *        -2- Configure the DMA functional parameters
  *        -3- Configure NVIC for DMA transfer complete/error interrupts
  * @param   None
  * @retval  None
  */
void Configure_DMA(void)
{  
  /* (1) Enable the clock of DMA2 */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);

  /* (2) Configure the DMA functional parameters for transmission */
  LL_DMA_ConfigTransfer(DMA2, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_MEMORY_TO_PERIPH | 
                                                LL_DMA_PRIORITY_HIGH              | 
                                                LL_DMA_MODE_CIRCULAR              | 
                                                LL_DMA_PERIPH_NOINCREMENT         | 
                                                LL_DMA_MEMORY_INCREMENT           | 
                                                LL_DMA_PDATAALIGN_WORD            | 
                                                LL_DMA_MDATAALIGN_WORD);
  LL_DMA_ConfigAddresses(DMA2, LL_DMA_CHANNEL_2, 
                         (uint32_t)pMultiBuffer, 
                         LL_SWPMI_DMA_GetRegAddr(SWPMI1, LL_SWPMI_DMA_REG_DATA_TRANSMIT), 
                         LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_CHANNEL_2));
  LL_DMA_SetDataLength(DMA2, LL_DMA_CHANNEL_2, SWPMI_MULTIBUFFER_SIZE);
  LL_DMA_SetPeriphRequest(DMA2, LL_DMA_CHANNEL_2, LL_DMA_REQUEST_4);
  
  /* (3) Configure the DMA functional parameters for reception */
  LL_DMA_ConfigTransfer(DMA2, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY | 
                                                LL_DMA_PRIORITY_HIGH              | 
                                                LL_DMA_MODE_CIRCULAR              | 
                                                LL_DMA_PERIPH_NOINCREMENT         | 
                                                LL_DMA_MEMORY_INCREMENT           | 
                                                LL_DMA_PDATAALIGN_WORD            | 
                                                LL_DMA_MDATAALIGN_WORD);
  LL_DMA_ConfigAddresses(DMA2, LL_DMA_CHANNEL_1, 
                         LL_SWPMI_DMA_GetRegAddr(SWPMI1, LL_SWPMI_DMA_REG_DATA_RECEIVE), 
                         (uint32_t)pReceiveBuffer, 
                         LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_CHANNEL_1));
  LL_DMA_SetDataLength(DMA2, LL_DMA_CHANNEL_1, SWPMI_RECEIVEBUFFER_SIZE);
  LL_DMA_SetPeriphRequest(DMA2, LL_DMA_CHANNEL_1, LL_DMA_REQUEST_4);
}

/**
  * @brief  This function configures SWPMI
  * @retval None
  */
void Configure_SWPMI(void)
{
  LL_SWPMI_InitTypeDef swpmi_initstruct;

  /* Configure the SWPMI peripheral */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SWPMI1);
  
  /* By default, SWPMI is clocked with PCLK1 at 80 MHz */
  //LL_RCC_SetSWPMIClockSource(LL_RCC_SWPMI1_CLKSOURCE_PCLK1);
  
  /* NVIC configuration for SWPMI1 interrupt */
  NVIC_SetPriority(SWPMI1_IRQn, 0);   
  NVIC_EnableIRQ(SWPMI1_IRQn);    

  /* SWPMI configured as follow:
      - SWP class selection: SWPMI_IO uses an internal voltage regulator to operate in class B.
      - BitRate: SWPMI_freq = SWPMI_clk / (((BitRate) + 1)  * 4)
      - Tx/Rx Buffering mode = Multi */
  swpmi_initstruct.VoltageClass     = LL_SWPMI_VOLTAGE_CLASS_B;
  swpmi_initstruct.BitRatePrescaler = __LL_SWPMI_CALC_BITRATE_PRESCALER(SWPMI_BITRATE, SystemCoreClock);
  swpmi_initstruct.TxBufferingMode  = LL_SWPMI_SW_BUFFER_TX_MULTI;
  swpmi_initstruct.RxBufferingMode  = LL_SWPMI_SW_BUFFER_RX_MULTI;
  if (LL_SWPMI_Init(SWPMI1, &swpmi_initstruct) != SUCCESS)
  {
      LED_Blinking(LED_BLINK_ERROR);
  }
  
  /* Enable loopback mode */
  LL_SWPMI_EnableLoopback(SWPMI1);

  /* Start the tx/rx process in DMA */
  LL_SWPMI_EnableDMAReq_RX(SWPMI1);
  LL_SWPMI_EnableDMAReq_TX(SWPMI1);
  
  /* To detect errors immediately, following IT can be enabled */
  /* Under run during payload transmission */
  LL_SWPMI_EnableIT_TXUNR(SWPMI1);
  /* Overrun during payload reception */
  LL_SWPMI_EnableIT_RXOVR(SWPMI1);
  /* CRC error during payload reception */
  LL_SWPMI_EnableIT_RXBER(SWPMI1);
  
  /* Set TXBEIE in the SWPMI_IER register 
     Clear TXBEF flag to avoid getting 1st interruption */
  LL_SWPMI_ClearFlag_TXBE(SWPMI1);
  LL_SWPMI_EnableIT_TXBE(SWPMI1);
  
  /* Set RXBFIE in the SWPMI_IER register */
  LL_SWPMI_EnableIT_RXBF(SWPMI1);
}

/**
  * @brief  This function handles the SWPMI communication in loopback mode
  * @retval None
  */
void Activate_SWPMI_Communication(void)
{
  /*  Activate Single wire protocol bus (SUSPEND or ACTIVATED state) */
  LL_SWPMI_Activate(SWPMI1);
  
  /* Start SWPMI reception process through DMA */
  LL_DMA_EnableChannel(DMA2, LL_DMA_CHANNEL_1);
  /* Start SWPMI transmission process through DMA */
  LL_DMA_EnableChannel(DMA2, LL_DMA_CHANNEL_2);
}

/**
  * @brief  Wait end of transfer and check if received Data are correct.
  * @param  None 
  * @retval None
  */
void WaitAndCheckEndOfTransfer(void)
{
  /* Wait for end of transmission */
  /* In transmission callback, on the latest frame,  payload size 
     is given as 0x00 which clears automatically TXDMA */
  while (LL_SWPMI_IsEnabledDMAReq_TX(SWPMI1) == 1)
  {
  } 
  
  /* Wait for end of reception */
  while (ubFramesNbReceived < SWPMI_FRAME_NUMBER_TO_TRANSMIT)
  {
  } 
  
  /* SWPMI DeInit */ 
  LL_DMA_DisableChannel(DMA2, LL_DMA_CHANNEL_1);
  LL_DMA_DisableChannel(DMA2, LL_DMA_CHANNEL_2);
  LL_SWPMI_Deactivate(SWPMI1);
  
  /* Disable loopback mode */
  LL_SWPMI_DisableLoopback(SWPMI1);
  
  /* Check that latest receive frame length */
  if (LL_SWPMI_GetReceiveFrameLength(SWPMI1) != SWPMI_LATEST_RFL)
  {
      LED_Blinking(LED_BLINK_ERROR);
  }
 
  /* Compare the transmit and received data */
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
  /* Increment frame number in transmission (or already transmitted) */
  ubFramesNbTransmitted++;
  
  /* Check if number of transmitted frames is not higher than Multi buffer size */
  if (ubFramesNbTransmitted == (SWPMI_NB_FRAMES_TO_COPY))
  {
    /* Prepare data which will transmitted from Multi buffer 
       when DMA will restart from buffer beginning (circular mode) */
    memcpy(pMultiBuffer, 
           pLastFramesBuffer, 
           (SWPMI_NB_FRAMES_TO_COPY * 4 * SWPMI_FRAME_SIZE));
  }
  /* Check if reach number of frame to transmit*/
  else if (ubFramesNbTransmitted == SWPMI_FRAME_NUMBER_TO_TRANSMIT)
  {
    /* Transmission needs to stop before end of Multi buffer,  
      set the low significant byte of the first word to 0 will stop automatically TX DMA */
    pMultiBuffer[(SWPMI_NB_FRAMES_TO_COPY + 1) * SWPMI_FRAME_SIZE] = 0;
  }
}

/**
  * @brief  Receive buffer full callback
  * @note   This function is executed when Receive buffer full interrupt
  *         is generated
  * @retval None
  */
void SWPMI_ReceiveBufferFull_Callback(void)
{
  /* Increment the number of frames received */
  ubFramesNbReceived++;
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

