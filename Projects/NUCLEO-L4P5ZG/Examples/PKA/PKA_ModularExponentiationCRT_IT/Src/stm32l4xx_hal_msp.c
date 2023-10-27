/**
  ******************************************************************************
  * @file    PKA/PKA_ModularExponentiationCRT_IT/Src/stm32l4xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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
  * @brief PKA MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param hpka: PKA handle pointer
  * @retval None
  */
void HAL_PKA_MspInit(PKA_HandleTypeDef *hpka)
{
  /* PKA Peripheral clock enable */
  __HAL_RCC_PKA_CLK_ENABLE();
  
  /* Enable the PKA interrupt */
  HAL_NVIC_SetPriority(PKA_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(PKA_IRQn);
}

/**
  * @brief PKA MSP De-Initialization
  *        This function freeze the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hpka: PKA handle pointer
  * @retval None
  */
void HAL_PKA_MspDeInit(PKA_HandleTypeDef *hpka)
{
  /* Enable PKA reset state */
  __HAL_RCC_PKA_FORCE_RESET();

  /* Release PKA from reset state */
  __HAL_RCC_PKA_RELEASE_RESET();
  
  /* PKA Peripheral clock disable */
  __HAL_RCC_PKA_CLK_DISABLE();
  
  /* Disable the PKA interrupt */
  HAL_NVIC_DisableIRQ(PKA_IRQn);
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

