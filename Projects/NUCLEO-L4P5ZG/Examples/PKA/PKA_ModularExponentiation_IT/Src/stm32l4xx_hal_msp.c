/**
  ******************************************************************************
  * @file    PKA/PKA_ModularExponentiation_IT/Src/stm32l4xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
