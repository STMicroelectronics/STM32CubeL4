/**
  ******************************************************************************
  * @file    UART/UART_LowPower_Hyperterminal_DMA/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4r9i_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           LPUART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_LPUART1_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMAMUX1_CLK_ENABLE(); \
  __HAL_RCC_DMA1_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOG_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOG_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_LPUART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_LPUART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_7
#define USARTx_TX_GPIO_PORT              GPIOG
#define USARTx_TX_AF                     GPIO_AF8_LPUART1
#define USARTx_RX_PIN                    GPIO_PIN_8
#define USARTx_RX_GPIO_PORT              GPIOG
#define USARTx_RX_AF                     GPIO_AF8_LPUART1

/* Definition for USARTx's DMA */

#define USARTx_TX_DMA_CHANNEL             DMA1_Channel2
#define USARTx_RX_DMA_CHANNEL             DMA1_Channel3



/* Definition for USARTx's DMA Request */
#define USARTx_TX_DMA_REQUEST             DMA_REQUEST_LPUART1_TX
#define USARTx_RX_DMA_REQUEST             DMA_REQUEST_LPUART1_RX

/* Definition for USARTx's NVIC */
#define USARTx_DMA_TX_IRQn                DMA1_Channel2_IRQn
#define USARTx_DMA_RX_IRQn                DMA1_Channel3_IRQn
#define USARTx_DMA_TX_IRQHandler          DMA1_Channel2_IRQHandler
#define USARTx_DMA_RX_IRQHandler          DMA1_Channel3_IRQHandler

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      LPUART1_IRQn
#define USARTx_IRQHandler                LPUART1_IRQHandler

/* Size of Transmission buffer */
#define TXSTARTMESSAGESIZE               (COUNTOF(aTxStartMessage) - 1)
#define TXENDMESSAGESIZE                 (COUNTOF(aTxEndMessage) - 1)

/* Size of Reception buffer */
#define RXBUFFERSIZE                      10

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

