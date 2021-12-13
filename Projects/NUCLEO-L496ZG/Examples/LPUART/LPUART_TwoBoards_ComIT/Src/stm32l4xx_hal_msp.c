/**
  ******************************************************************************
  * @file    UART/LPUART_TwoBoards_ComIT/Src/stm32l4xx_hal_msp.c
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

/** @defgroup LPUART_TwoBoards_ComIT
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
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for UART interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  LPUARTx_TX_GPIO_CLK_ENABLE();
  LPUARTx_RX_GPIO_CLK_ENABLE();
  /* Enable LPUART clock */
  LPUARTx_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/
  /* LPUART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = LPUARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = LPUARTx_TX_AF;
  
  HAL_GPIO_Init(LPUARTx_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* LPUART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = LPUARTx_RX_PIN;
  GPIO_InitStruct.Alternate = LPUARTx_RX_AF;
  
  HAL_GPIO_Init(LPUARTx_RX_GPIO_PORT, &GPIO_InitStruct);
  
  /*##-3- Configure the NVIC for LPUART ######################################*/
  /* NVIC for LPUART */
  HAL_NVIC_SetPriority(LPUARTx_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(LPUARTx_IRQn);
}

/**
  * @brief UART MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  LPUARTx_FORCE_RESET();
  LPUARTx_RELEASE_RESET();
  
  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  HAL_GPIO_DeInit(LPUARTx_TX_GPIO_PORT, LPUARTx_TX_PIN);
  HAL_GPIO_DeInit(LPUARTx_RX_GPIO_PORT, LPUARTx_RX_PIN);
  
  /*##-3- Disable the NVIC for LPUART ########################################*/
  HAL_NVIC_DisableIRQ(LPUARTx_IRQn);
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

