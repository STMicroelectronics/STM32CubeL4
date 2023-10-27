/**
  ******************************************************************************
  * @file    DMA/DMAMUX_SYNC/Src/stm32h7xx_hal_msp.c
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

/** @defgroup DMAMUX_SYNC
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern DMA_HandleTypeDef DMA_Handle;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA & DMAMUX configurations are performed in the main.c module as the example first purpose is to show the DMA & DMAMUX
  *             configuration when using the DMAMUX synchronization block.
  *             In normal conditions DMA and DMAMUX configuration should be placed here.
  *           - NVIC configuration for DMA interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{

  GPIO_InitTypeDef  GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  LPUART_GPIO_CLK_ENABLE();

  /* Enable LPUART clock */
  LPUART_CLK_ENABLE();

  /* IOSV bit MUST be set to access GPIO port G[2:15] */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = LPUART_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStruct.Alternate = LPUART_TX_AF;

  HAL_GPIO_Init(LPUART_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = LPUART_RX_PIN;
  GPIO_InitStruct.Alternate = LPUART_RX_AF;

  HAL_GPIO_Init(LPUART_RX_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure The DMA ###################################################*/
  /* Associate the initialized DMA handle to the UART handle */
  __HAL_LINKDMA(huart, hdmatx, DMA_Handle);

  /*##-4- Configure the NVIC for DMA #########################################*/
  /* NVIC configuration for DMA transfer complete interrupt (LPUART_TX) */
  HAL_NVIC_SetPriority(LPUART_DMA_TX_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(LPUART_DMA_TX_IRQn);

  /* NVIC configuration for USART, to catch the TX complete */
  HAL_NVIC_SetPriority(LPUART_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(LPUART_IRQn);
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  LPUART_FORCE_RESET();
  LPUART_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-Initialize UART Tx as alternate function  */
  HAL_GPIO_DeInit(LPUART_TX_GPIO_PORT, LPUART_TX_PIN);
  /* De-Initialize UART Rx as alternate function  */
  HAL_GPIO_DeInit(LPUART_RX_GPIO_PORT, LPUART_RX_PIN);

  /*##-3- Disable the DMA Streams ############################################*/
  /* De-Initialize the DMA Stream associated to transmission process */
  HAL_DMA_DeInit(huart->hdmatx);

  /*##-4- Disable the NVIC for DMA ###########################################*/
  HAL_NVIC_DisableIRQ(LPUART_DMA_TX_IRQn);
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

