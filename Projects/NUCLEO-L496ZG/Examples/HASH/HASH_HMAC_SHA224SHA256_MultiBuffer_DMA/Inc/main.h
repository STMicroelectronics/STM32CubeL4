/**
  ******************************************************************************
  * @file    HASH/HASH_HMAC_SHA224SHA256_MultiBuffer_DMA/Inc/main.h
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
#include "string.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define DMA_REQUEST                           DMA_REQUEST_6
#define DMA_CHANNEL                           DMA2_Channel7
#define DMA_CLK_ENABLE()                      __HAL_RCC_DMA2_CLK_ENABLE()
#define HASH_DMA_IRQn                         DMA2_Channel7_IRQn
/* Exported functions ------------------------------------------------------- */
#define HASH_DMA_IRQHandler                   DMA2_Channel7_IRQHandler

#endif /* __MAIN_H */

