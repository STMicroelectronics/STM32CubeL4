/**
  ******************************************************************************
  * @file    CRYP/CRYP_DMA/Src/stm32l4xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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

/** @defgroup HAL_MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief CRYP MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - DMA configuration for transmission request by peripheral 
  *           - NVIC configuration for DMA interrupt request enable
  * @param hcryp: CRYP handle pointer
  * @retval None
  */
void HAL_CRYP_MspInit(CRYP_HandleTypeDef *hcryp)
{
  static  DMA_HandleTypeDef   hdmaIn;
  static  DMA_HandleTypeDef   hdmaOut;

  /* Enable CRYP clock */
  __HAL_RCC_AES_CLK_ENABLE();

  /* Force the CRYP Periheral Clock Reset */  
  __HAL_RCC_AES_FORCE_RESET(); 
  /* Release the CRYP Periheral Clock Reset */  
  __HAL_RCC_AES_RELEASE_RESET();

  
    /* Enable DMA2 clocks */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA2_FORCE_RESET();
  __HAL_RCC_DMA2_RELEASE_RESET();
  
  /***************** Configure common DMA In parameters ***********************/  
  hdmaIn.Instance                 = DMA2_Channel1;
  hdmaIn.Init.Request             = DMA_REQUEST_6;
  hdmaIn.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdmaIn.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdmaIn.Init.MemInc              = DMA_MINC_ENABLE;
  hdmaIn.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdmaIn.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdmaIn.Init.Mode                = DMA_NORMAL;
  hdmaIn.Init.Priority            = DMA_PRIORITY_MEDIUM;
  
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(hcryp, hdmain, hdmaIn);
  
  /* Deinitialize the Stream for new transfer */
  HAL_DMA_DeInit(hcryp->hdmain);
  
  /* Configure the DMA Stream */
  HAL_DMA_Init(hcryp->hdmain);      
  
  /* NVIC configuration for DMA Input data interrupt */
  HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel1_IRQn);
  
  /***************** Configure common DMA Out parameters **********************/
  hdmaOut.Instance                 = DMA2_Channel2;
  hdmaOut.Init.Request             = DMA_REQUEST_6;
  hdmaOut.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdmaOut.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdmaOut.Init.MemInc              = DMA_MINC_ENABLE;
  hdmaOut.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdmaOut.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdmaOut.Init.Mode                = DMA_NORMAL;
  hdmaOut.Init.Priority            = DMA_PRIORITY_MEDIUM;
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(hcryp, hdmaout, hdmaOut);
  
  /* Deinitialize the Stream for new processing */
  HAL_DMA_DeInit(&hdmaOut);
  
  /* Configure the DMA Stream */
  HAL_DMA_Init(&hdmaOut);
  
  /* NVIC configuration for DMA output data interrupt */
  /* Already configured */
  HAL_NVIC_SetPriority(DMA2_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel2_IRQn);
  
}


/**
  * @brief  DeInitializes the CRYP MSP.
  * @param  hcryp: CRYP handle pointer
  * @retval None
  */
void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef *hcryp)
{
  /* Disable CRYP clock */
  __HAL_RCC_AES_CLK_DISABLE();
  

}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

