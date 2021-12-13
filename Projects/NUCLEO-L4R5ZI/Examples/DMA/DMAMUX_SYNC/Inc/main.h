/**
  ******************************************************************************
  * @file    DMA/DMAMUX_SYNC/Inc/main.h
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
#include "stm32l4xx_nucleo_144.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition for LPUART clock resources */
#define LPUART                           LPUART1
#define LPUART_CLK_ENABLE()              __HAL_RCC_LPUART1_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define DMAMUX1_CLK_ENABLE()             __HAL_RCC_DMAMUX1_CLK_ENABLE()
#define LPUART_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOG_CLK_ENABLE()

#define LPUART_FORCE_RESET()             __HAL_RCC_LPUART1_FORCE_RESET()
#define LPUART_RELEASE_RESET()           __HAL_RCC_LPUART1_RELEASE_RESET()

/* Definition for LPUART Pins */
#define LPUART_TX_PIN                    GPIO_PIN_7
#define LPUART_TX_GPIO_PORT              GPIOG
#define LPUART_TX_AF                     GPIO_AF8_LPUART1
#define LPUART_RX_PIN                    GPIO_PIN_8
#define LPUART_RX_GPIO_PORT              GPIOG
#define LPUART_RX_AF                     GPIO_AF8_LPUART1

/* Definition for LPUART's DMA */
#define LPUART_TX_DMA_INSTANCE           DMA1_Channel1

#define LPUART_TX_DMA_REQUEST            DMA_REQUEST_LPUART1_TX


/* Definition for LPUART's NVIC */
#define LPUART_DMA_TX_IRQn               DMA1_Channel1_IRQn
#define LPUART_DMA_TX_IRQHandler         DMA1_Channel1_IRQHandler

/* Definition for LPUART's NVIC */
#define LPUART_IRQn                      LPUART1_IRQn
#define LPUART_IRQHandler                LPUART1_IRQHandler

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

