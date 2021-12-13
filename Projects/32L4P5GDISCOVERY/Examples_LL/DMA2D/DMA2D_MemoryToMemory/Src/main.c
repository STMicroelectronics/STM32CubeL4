/**
  ******************************************************************************
  * @file    Examples_LL/DMA2D/DMA2D_MemoryToMemory/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a description of how to configure
  *          DMA2D peripheral in Memory to Memory transfer mode
  *          using the STM32L4xx DMA2D LL API.
  *          Peripheral initialization done using LL unitary services functions.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
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

/** @addtogroup DMA2D_MemoryToMemory
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
uint32_t aBufferResult[(LAYER_SIZE_X * LAYER_SIZE_Y * LAYER_NB_BYTES_PER_PIXEL) / 4];

/* ARGB4444 input buffer 16bpp */
uint32_t aBufferInput[(LAYER_SIZE_X * LAYER_SIZE_Y * LAYER_NB_BYTES_PER_PIXEL) / 4] =
      {0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/



/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_DMA2D(void);
void     CheckDMA2DTransfer(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength);
void     LED_Init(void);
void     LED_On(void);
void     LED_Blinking(uint32_t Period);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 120 MHz */
  SystemClock_Config();

  /* Initialize LED1 */
  LED_Init();

  /* Configure DMA2D (DMA2D Mode, Output Color Mode and output offset) */
  Configure_DMA2D();

  /* Set input (foreground) memory address */
  LL_DMA2D_FGND_SetMemAddr(DMA2D, (uint32_t)&aBufferInput);

  /* Set output memory address */
  LL_DMA2D_SetOutputMemAddr(DMA2D, (uint32_t)&aBufferResult);

  /* Set number of lines and number of pixels per line */
  LL_DMA2D_SetNbrOfLines(DMA2D, LAYER_SIZE_Y);
  LL_DMA2D_SetNbrOfPixelsPerLines(DMA2D, LAYER_SIZE_X);

  /* Start transfer */
  LL_DMA2D_Start(DMA2D);

  /* Wait until transfer is over */
  while (LL_DMA2D_IsTransferOngoing(DMA2D));

  /* Check whether or not DMA2D transfer is successful */
  CheckDMA2DTransfer(aBufferInput,
                     aBufferResult,
                    ((LAYER_SIZE_X * LAYER_SIZE_Y * LAYER_NB_BYTES_PER_PIXEL) / 4));

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function configures DMA2D Instance.
  * @note   This function is used to :
  *         -1- Enable peripheral clock for DMA2D.
  *         -2- Configure DMA2D functional parameters.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_DMA2D(void)
{

  /* (1) Enable peripheral clock for DMA2D                *********************/
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2D);

  /* (2) Configure DMA2D output parameters  ***********************************/

  /* Configure DMA2D output color mode */
  LL_DMA2D_SetOutputColorMode(DMA2D, LL_DMA2D_OUTPUT_MODE_ARGB4444);

  /* Initialize default DMA2D mode */
  /* Reset value is LL_DMA2D_MODE_M2M */
  // LL_DMA2D_SetMode(DMA2D, LL_DMA2D_MODE_M2M);

  /* Initialize default Red and Blue swap */
  /* Reset value is LL_DMA2D_RB_MODE_REGULAR */
  // LL_DMA2D_SetOutputRBSwapMode(DMA2D, LL_DMA2D_RB_MODE_REGULAR);

  /* Initialize default alpha inversion */
  /* Reset value is LL_DMA2D_ALPHA_REGULAR */
  // LL_DMA2D_SetOutputAlphaInvMode(DMA2D, LL_DMA2D_ALPHA_REGULAR);

  /* Initialize default output line offset */
  /* Reset value is 0x0 */
  // LL_DMA2D_SetLineOffset(DMA2D, 0x0);


  /* (3) Configure DMA2D input (foreground layer) parameters  *****************/

  /* Configure Foreground layer input color mode and alpha value */
  LL_DMA2D_FGND_SetColorMode(DMA2D, LL_DMA2D_INPUT_MODE_ARGB4444);
  LL_DMA2D_FGND_SetAlpha(DMA2D, 0xFF); /* Fully opaque */

  /* Initialize default DMA2D input alpha mode */
  /* Reset value is LL_DMA2D_ALPHA_MODE_NO_MODIF */
  // LL_DMA2D_FGND_SetAlphaMode(DMA2D, LL_DMA2D_ALPHA_MODE_NO_MODIF);

  /* Initialize default input Red and Blue swap */
  /* Reset value is LL_DMA2D_RB_MODE_REGULAR */
  // LL_DMA2D_FGND_SetRBSwapMode(DMA2D, LL_DMA2D_RB_MODE_REGULAR);

  /* Initialize default alpha inversion */
  /* Reset value is LL_DMA2D_ALPHA_REGULAR */
  // LL_DMA2D_FGND_SetAlphaInvMode(DMA2D, LL_DMA2D_ALPHA_REGULAR);

  /* Initialize default input line offset */
  /* Reset value is 0x0 */
  // LL_DMA2D_FGND_SetLineOffset(DMA2D, 0x0);
}

/**
  * @brief  Compares input and output buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length.
  * @retval None
  */
void CheckDMA2DTransfer(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      /* Transfer error */
      LED_Blinking(LED_BLINK_ERROR);
    }
    pBuffer1++;
    pBuffer2++;
  }

  /* No error detected, turn on LED */
  LED_On();
  return;
}


/**
  * @brief  Initialize LED1 .
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* For GPIO port G, need to enable VddIO2 supply */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_EnableVddIO2();

  /* Enable the LED1 Clock */
  LED1_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED1 */
  LL_GPIO_SetPinMode(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_PULL_NO);
  /* Turn LED1 off by default */
  LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

/**
  * @brief  Turn-on LED1 .
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED1 on */
  LL_GPIO_ResetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

/**
  * @brief  Set LED1 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
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
    LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
    LL_mDelay(Period);
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 120000000
  *            HCLK(Hz)                       = 120000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 60
  *            PLL_R                          = 2
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Enable voltage range 1 boost mode for frequency above 80 Mhz */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_EnableRange1BoostMode();
  LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);

  /* MSI already enabled at reset */

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 60, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  /* Intermediate AHB prescaler 2 when target frequency clock is higher than 80 MHz */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Insure 1µs transition state at intermediate medium speed clock based on DWT*/
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  DWT->CYCCNT = 0;
  while(DWT->CYCCNT < 100);

  /* AHB prescaler 1 */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 120MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ(MSI_VALUE,
                                  LL_RCC_PLLM_DIV_1, 60, LL_RCC_PLLR_DIV_2)*/
  LL_Init1msTick(120000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(120000000);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
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

