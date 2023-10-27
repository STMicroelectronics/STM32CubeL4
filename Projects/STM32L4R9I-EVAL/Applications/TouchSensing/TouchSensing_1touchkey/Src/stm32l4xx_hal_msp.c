/**
  ******************************************************************************
  * @file    TouchSensing/TouchSensing_1touchkey/Src/stm32l4xx_hal_msp.c
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
  * @brief TSC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htsc: TSC handle pointer
  * @retval None
  */
void HAL_TSC_MspInit(TSC_HandleTypeDef *htsc)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /*##-1- Enable TSC and GPIO clocks #########################################*/
  TSCx_CLK_ENABLE();
  TSCx_TS1_IO_GPIO_CLK_ENABLE();
  TSCx_SHIELD_GPIO_CLK_ENABLE();

  /*##-2- Configure Sampling Capacitor IOs (Alternate-Function Open-Drain) ###*/
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;

  /* Shield Sampling Capacitor IO = TSC_GROUP4_IO2 */
  GPIO_InitStruct.Pin       = TSCx_SHIELD_SAMPLING_PIN;
  GPIO_InitStruct.Alternate = TSCx_SHIELD_SAMPLING_AF;
  HAL_GPIO_Init(TSCx_SHIELD_SAMPLING_GPIO_PORT, &GPIO_InitStruct);

  /* Channel TS1 Sampling Capacitor IO = TSC_GROUP1_IO2 */
  GPIO_InitStruct.Pin       = TSCx_TS1_SAMPLING_PIN;
  GPIO_InitStruct.Alternate = TSCx_TS1_SAMPLING_AF;
  HAL_GPIO_Init(TSCx_TS1_SAMPLING_GPIO_PORT, &GPIO_InitStruct);
  
  /*##-3- Configure Channel & Shield IOs (Alternate-Function Output PP) ######*/
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;

  /* Shield IO = TSC_GROUP4_IO1 */
  GPIO_InitStruct.Pin       = TSCx_SHIELD_IO_PIN;
  GPIO_InitStruct.Alternate = TSCx_SHIELD_IO_AF;
  HAL_GPIO_Init(TSCx_SHIELD_IO_GPIO_PORT, &GPIO_InitStruct);

  /* Channel IO (TS1) = TSC_GROUP1_IO1 */
  GPIO_InitStruct.Pin       = TSCx_TS1_IO_PIN;
  GPIO_InitStruct.Alternate = TSCx_TS1_IO_AF;
  HAL_GPIO_Init(TSCx_TS1_IO_GPIO_PORT, &GPIO_InitStruct);

  /*##-4- Configure the NVIC for TSC #########################################*/
  /* Not used in this example. Keep it for reference.
  HAL_NVIC_SetPriority(TSCx_EXTI_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(TSCx_EXTI_IRQn); */
}

/**
  * @brief TSC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param htsc: TSC handle pointer
  * @retval None
  */
void HAL_TSC_MspDeInit(TSC_HandleTypeDef *htsc)
{
  /*##-1- Reset peripherals ##################################################*/
  TSCx_FORCE_RESET();
  TSCx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the TSC GPIO pins */
  HAL_GPIO_DeInit(TSCx_TS1_IO_GPIO_PORT, TSCx_TS1_IO_PIN);
  HAL_GPIO_DeInit(TSCx_SHIELD_IO_GPIO_PORT, TSCx_SHIELD_IO_PIN);
  HAL_GPIO_DeInit(TSCx_TS1_SAMPLING_GPIO_PORT, TSCx_TS1_SAMPLING_PIN);
  HAL_GPIO_DeInit(TSCx_SHIELD_SAMPLING_GPIO_PORT, TSCx_SHIELD_SAMPLING_PIN);
  
  /*##-3- Disable the NVIC for TSC ###########################################*/
  /* Not used in this example. Keep it for reference.
  HAL_NVIC_DisableIRQ(TSCx_EXTI_IRQn); */
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

