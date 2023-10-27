/**
  ******************************************************************************
  * @file    PSSI/PSSI_Transmit/Src/stm32l4xx_hal_msp.c
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
  * @brief Initialize the PSSI MSP.
  * @param  hpssi Pointer to a PSSI_HandleTypeDef structure that contains
  *                the configuration information for the specified PSSI.
  * @retval None
  */
void HAL_PSSI_MspInit(PSSI_HandleTypeDef *hpssi)
{
  GPIO_InitTypeDef gpio_init_structure;

  /*** Enable peripherals and GPIO clocks ***/

  /* Enable DCMI clock */
  __HAL_RCC_PSSI_CLK_ENABLE();

  __HAL_RCC_PSSI_FORCE_RESET();
  __HAL_RCC_PSSI_RELEASE_RESET();

  /* Enable GPIO clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /* PSSI_D14, PSSI_PDCK */
  gpio_init_structure.Pin       = GPIO_PIN_5 | GPIO_PIN_6;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF4_PSSI;
  HAL_GPIO_Init(GPIOA, &gpio_init_structure);

  /* PSSI_DE */
  gpio_init_structure.Pin       = GPIO_PIN_4;
  gpio_init_structure.Alternate = GPIO_AF10_PSSI;
  HAL_GPIO_Init(GPIOA, &gpio_init_structure);

  /* PSSI_D2, PSSI_D3 */
  gpio_init_structure.Pin       = GPIO_PIN_9 | GPIO_PIN_11;
  gpio_init_structure.Alternate = GPIO_AF4_PSSI;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

  /* PSSI_D0 */
  gpio_init_structure.Pin       = GPIO_PIN_6;
  gpio_init_structure.Alternate = GPIO_AF10_PSSI;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

  /* PSSI_D4, PSSI_D6, PSSI_D7 */
  gpio_init_structure.Pin       = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);

  /* PSSI_D11, PSSI_D12 */
  gpio_init_structure.Pin       = GPIO_PIN_10 | GPIO_PIN_11;
  HAL_GPIO_Init(GPIOF, &gpio_init_structure);

  /* PSSI_D1, PSSI_D8, PSSI_D9 */
  gpio_init_structure.Pin       = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10;
  HAL_GPIO_Init(GPIOH, &gpio_init_structure);

  /* PSSI_D5, PSSI_D10, PSSI_D13, PSSI_D15, PSSI_RDY */
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11;
  HAL_GPIO_Init(GPIOI, &gpio_init_structure);


  /* NVIC configuration for DCMI transfer complete interrupt */
  HAL_NVIC_SetPriority(DCMI_PSSI_IRQn, 0x05, 0);
  HAL_NVIC_EnableIRQ(DCMI_PSSI_IRQn);

}

/**
  * @brief De-Initialize the PSSI MSP.
  * @param  hpssi Pointer to a PSSI_HandleTypeDef structure that contains
  *                the configuration information for the specified PSSI.
  * @retval None
  */
void HAL_PSSI_MspDeInit(PSSI_HandleTypeDef *hpssi)
{
  /* Disable PSSI clock */
  __HAL_RCC_PSSI_CLK_DISABLE();

  /* Force the PSSI Periheral Clock Reset */
  __HAL_RCC_PSSI_FORCE_RESET();

  /* Release the PSSI Periheral Clock Reset */
  __HAL_RCC_PSSI_RELEASE_RESET();
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

