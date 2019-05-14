/**
  ******************************************************************************
  * @file    CRYP/CRYP_AESModes/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
