/**
  ******************************************************************************
  * @file    SWPMI_Session/Src/stm32l4xx_hal_msp.c
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

/** @addtogroup SWPMI_Session
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
  * @brief SWPMI MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - NVIC configuration
  * @param hswpmi: SWPMI handle pointer
  * @retval None
  */
void HAL_SWPMI_MspInit(SWPMI_HandleTypeDef *hswpmi)
{
  RCC_PeriphCLKInitTypeDef clkinit = {0};
  GPIO_InitTypeDef         gpioinit = {0};
  
  /*##-1- Configure the SWPMI1 clock source. The clock is derived from the SYSCLK #*/
  clkinit.PeriphClockSelection = RCC_PERIPHCLK_SWPMI1;
  clkinit.Swpmi1ClockSelection = RCC_SWPMI1CLKSOURCE_PCLK1;
  if(HAL_RCCEx_PeriphCLKConfig(&clkinit) != HAL_OK)
  {
    Error_Handler();
  }

  /*##-2- Enable peripherals and GPIO Clocks #################################*/
  /* Enable SWPMI clock */
  __HAL_RCC_SWPMI1_CLK_ENABLE();  
  /* Enable GPIOB clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*##-3- Configure peripheral GPIO ##########################################*/  
  /* SWPMI IO as alternate function  */
  gpioinit.Pin = GPIO_PIN_12;
  gpioinit.Mode = GPIO_MODE_AF_PP;
  gpioinit.Pull = GPIO_PULLDOWN;
  gpioinit.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpioinit.Alternate = GPIO_AF12_SWPMI1;
  HAL_GPIO_Init(GPIOB, &gpioinit);

  /*##-4- Configure the NVIC for SWPMI1 ######################################*/   
  /* NVIC for I2Cx */
  HAL_NVIC_SetPriority(SWPMI1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(SWPMI1_IRQn);

  HAL_Delay(1);
}

/**
  * @brief SWPMI MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hswpmi: SWPMI handle pointer
  * @retval None
  */
void HAL_SWPMI_MspDeInit(SWPMI_HandleTypeDef *hswpmi)
{
  /* Disable SWPMI clock */
  __HAL_RCC_SWPMI1_CLK_DISABLE();
  
  /* DeInit PB.12 SWPMI IO configuration */
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12);
  
  /* Disable GPIOB clock */
  __HAL_RCC_GPIOB_CLK_DISABLE();
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

