/**
  ******************************************************************************
  * @file    HASH/HASH_SHA1SHA256_IT_DMA_Suspension/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4p5g_discovery.h"
#include "string.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define DMA_REQUEST                           DMA_REQUEST_HASH_IN
#define DMA_CHANNEL                           DMA2_Channel7
#define DMA_CLK_ENABLE()                      __HAL_RCC_DMA2_CLK_ENABLE()
#define DMAMUX_CLK_ENABLE()                   __HAL_RCC_DMAMUX1_CLK_ENABLE()
#define HASH_DMA_IRQn                         DMA2_Channel7_IRQn
#define HASH_IRQn                             HASH_CRS_IRQn
/* Exported functions ------------------------------------------------------- */
#define HASH_DMA_IRQHandler                   DMA2_Channel7_IRQHandler
#define HASH_IRQHandler                       HASH_CRS_IRQHandler

#endif /* __MAIN_H */

