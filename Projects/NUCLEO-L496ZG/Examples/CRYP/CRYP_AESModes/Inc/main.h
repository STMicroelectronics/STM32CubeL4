/**
  ******************************************************************************
  * @file    CRYP/CRYP_AESModes/Inc/main.h
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
#include "stdio.h"
#include "string.h"


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define  TIMEOUT_VALUE 0xFF 

/* ## Definition of UARTx related resources ################################### */
/* Definition of UARTx */
#define UARTx                            LPUART1

/* Definition of UARTx clock resources */
#define UARTx_CLK_ENABLE()               __HAL_RCC_LPUART1_CLK_ENABLE()
#define UARTx_TX_PIN_CLK_ENABLE()        __HAL_RCC_GPIOG_CLK_ENABLE()
#define UARTx_RX_PIN_CLK_ENABLE()        __HAL_RCC_GPIOG_CLK_ENABLE()

/* Definition of UARTx channels pins */
#define UARTx_TX_PIN                     GPIO_PIN_7
#define UARTx_TX_GPIO_PORT               GPIOG
#define UARTx_TX_AF                      GPIO_AF8_LPUART1

#define UARTx_RX_PIN                     GPIO_PIN_8
#define UARTx_RX_GPIO_PORT               GPIOG
#define UARTx_RX_AF                      GPIO_AF8_LPUART1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

