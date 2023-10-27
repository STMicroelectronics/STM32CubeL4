/**
  ******************************************************************************
  * @file    DMA2D/DMA2D_MemoryToMemory/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a description of how to configure
  *          DMA2D peripheral in Memory to Memory transfer mode
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

/** @addtogroup DMA2D_MemoryToMemory
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMA2D_HandleTypeDef Dma2dHandle;

__ALIGN_BEGIN uint32_t aBufferResult[(LAYER_SIZE_X * LAYER_SIZE_Y * LAYER_NB_BYTES_PER_PIXEL) / 4] __ALIGN_END;

/* ARGB4444 input buffer 16bpp */
__ALIGN_BEGIN uint32_t aBufferInput[(LAYER_SIZE_X * LAYER_SIZE_Y * LAYER_NB_BYTES_PER_PIXEL) / 4] __ALIGN_END =
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

/* Private function prototypes -----------------------------------------------*/
void TransferError(DMA2D_HandleTypeDef* dma2dHandle);
void TransferComplete(DMA2D_HandleTypeDef* dma2dHandle);
void SystemClock_Config(void);
void Error_Handler(void);
/* Private functions ---------------------------------------------------------*/
ErrorStatus Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	
  /* STM32L4xx HAL library initialization:
       - Configure the Flash prefetch, Instruction cache, Data cache
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 120 MHz */
  SystemClock_Config();

  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /*##-1- Configure the DMA2D Mode, Output Color Mode and output offset #############*/
  Dma2dHandle.Init.Mode         = DMA2D_M2M; /* Mode Memory To Memory */
  Dma2dHandle.Init.ColorMode    = DMA2D_OUTPUT_ARGB4444; /* Output color mode is ARGB4444 : 16 bpp */
  Dma2dHandle.Init.OutputOffset = 0x0; /* No offset on output */
  Dma2dHandle.Init.RedBlueSwap   = DMA2D_RB_REGULAR;      /* No R&B swap for the output image */
  Dma2dHandle.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;   /* No alpha inversion for the output image */
  Dma2dHandle.Init.BytesSwap     = DMA2D_BYTES_REGULAR;   /* No byte swap for the output image */
  Dma2dHandle.Init.LineOffsetMode = DMA2D_LOM_PIXELS;     /* Line offsets expressed in pixels */

  /*##-2- DMA2D Callbacks Configuration ######################################*/
  Dma2dHandle.XferCpltCallback  = TransferComplete;
  Dma2dHandle.XferErrorCallback = TransferError;
  
  /*##-3- Foreground Layer Configuration ###########################################*/
  Dma2dHandle.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA; /* Keep original Alpha from ARGB4444 input */
  Dma2dHandle.LayerCfg[1].InputAlpha = 0xFF; /* Fully opaque */
  Dma2dHandle.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB4444; /* Input color is ARGB4444 : 16 bpp */
  Dma2dHandle.LayerCfg[1].InputOffset = 0x0; /* No offset in input */
  Dma2dHandle.LayerCfg[1].RedBlueSwap   = DMA2D_RB_REGULAR;      /* No R&B swap for the input image */
  Dma2dHandle.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA;   /* No alpha inversion for the input image */

  Dma2dHandle.Instance = DMA2D;

  /*##-4- DMA2D Initialisation ###############################################*/
  if(HAL_DMA2D_Init(&Dma2dHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Configure Foreground layer (ie layer 1) */
  if(HAL_DMA2D_ConfigLayer(&Dma2dHandle, 1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-5- Start DMA2D transfer ###############################################*/
  
  if(HAL_DMA2D_Start_IT(&Dma2dHandle, 
                        (uint32_t)&aBufferInput,  /* Source memory buffer      */ 
						            (uint32_t)&aBufferResult, /* Destination memory buffer */
						            LAYER_SIZE_X,             /* Width of the 2D memory transfer in pixels */
						            LAYER_SIZE_Y)             /* Height of the 2D memory transfer in lines */
						            != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-6- Wait for the end of the transfer ###################################*/
  /*  Before starting a new transfer, you need to check the current state of
      the peripheral; if it’s busy you need to wait for the end of current
      transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the
      transfer, but application may perform other tasks while transfer operation
      is ongoing. */
  while (HAL_DMA2D_GetState(&Dma2dHandle) != HAL_DMA2D_STATE_READY)
  {
  }
 
  /*##-7- Check transfer correctness #########################################*/

  if(Buffercmp(aBufferInput, 
               aBufferResult, 
			         ((LAYER_SIZE_X * LAYER_SIZE_Y * LAYER_NB_BYTES_PER_PIXEL) / 4)) == ERROR)
  {
    /* KO */
    /* Turn on LED3 */
    BSP_LED_On(LED3);
  }
  else
  {
    /* OK */
    /* Turn on LED1 */
    BSP_LED_On(LED1);
  }

  while (1) { ; } 
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
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  /* Enable voltage range 1 boost mode for frequency above 80 Mhz */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
  __HAL_RCC_PWR_CLK_DISABLE();

  /* Enable MSI Oscillator and activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* To avoid undershoot due to maximum frequency, select PLL as system clock source */
  /* with AHB prescaler divider 2 as first step */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* AHB prescaler divider at 1 as second step */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn on LED3 */
  BSP_LED_On(LED3);
  while(1)
  {
  }
}


/**
  * @brief  DMA2D Transfer completed callback
  * @param  hdma2d: DMA2D handle.
  * @note   This example shows a simple way to report end of DMA2D transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void TransferComplete(DMA2D_HandleTypeDef *hdma2d)
{
  /* Turn LED1 on */
  BSP_LED_On(LED1);
}

/**
  * @brief  DMA2D error callbacks
  * @param  hdma2d: DMA2D handle
  * @note   This example shows a simple way to report DMA2D transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void TransferError(DMA2D_HandleTypeDef *hdma2d)
{
  /* Turn on LED3 */
  BSP_LED_On(LED3);
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length.
  * @retval  0: pBuffer1 identical to pBuffer2
  *          1: pBuffer1 differs from pBuffer2
  */
ErrorStatus Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return ERROR;
    }
    pBuffer1++;
    pBuffer2++;
  }
  return SUCCESS;
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

