/**
  ******************************************************************************
  * @file    OSPI/OSPI_RAM_ExecuteInPlace/Inc/main.h
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
#include "stm32l4r9i_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition for OSPI Pins */
#define OSPI_CS_PIN                 GPIO_PIN_5
#define OSPI_CS_GPIO_PORT           GPIOI
#define OSPI_CLK_PIN                GPIO_PIN_6
#define OSPI_CLK_GPIO_PORT          GPIOI
#define OSPI_NCLK_PIN               GPIO_PIN_7
#define OSPI_NCLK_GPIO_PORT         GPIOI
#define OSPI_D0_PIN                 GPIO_PIN_11
#define OSPI_D0_GPIO_PORT           GPIOI
#define OSPI_D1_PIN                 GPIO_PIN_10
#define OSPI_D1_GPIO_PORT           GPIOI
#define OSPI_D2_PIN                 GPIO_PIN_9
#define OSPI_D2_GPIO_PORT           GPIOI
#define OSPI_D3_PIN                 GPIO_PIN_8
#define OSPI_D3_GPIO_PORT           GPIOH
#define OSPI_D4_PIN                 GPIO_PIN_9
#define OSPI_D4_GPIO_PORT           GPIOH
#define OSPI_D5_PIN                 GPIO_PIN_10
#define OSPI_D5_GPIO_PORT           GPIOH
#define OSPI_D6_PIN                 GPIO_PIN_11
#define OSPI_D6_GPIO_PORT           GPIOH
#define OSPI_D7_PIN                 GPIO_PIN_12
#define OSPI_D7_GPIO_PORT           GPIOH
#define OSPI_DQS_PIN                GPIO_PIN_15
#define OSPI_DQS_GPIO_PORT          GPIOG

/* IS66WVH8M8BLL-100BLI ISSI memory */
/* Size of the HyperRAM */
#define OSPI_HYPERRAM_SIZE          23

/* Timing of the HyperRAM */
#define OSPI_HYPERRAM_RW_REC_TIME   3
#define OSPI_HYPERRAM_LATENCY       6

/* Exported macro ------------------------------------------------------------*/
/* Definition for OSPI clock resources */
#define OSPI_CLK_ENABLE()           __HAL_RCC_OSPI1_CLK_ENABLE()
#define OSPI_CLK_DISABLE()          __HAL_RCC_OSPI1_CLK_DISABLE()
#define OSPIM_CLK_ENABLE()          __HAL_RCC_OSPIM_CLK_ENABLE()
#define OSPIM_CLK_DISABLE()         __HAL_RCC_OSPIM_CLK_DISABLE()
#define OSPI_CS_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOI_CLK_ENABLE()
#define OSPI_CLK_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOI_CLK_ENABLE()
#define OSPI_NCLK_GPIO_CLK_ENABLE() __HAL_RCC_GPIOI_CLK_ENABLE()
#define OSPI_D0_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOI_CLK_ENABLE()
#define OSPI_D1_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOI_CLK_ENABLE()
#define OSPI_D2_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOI_CLK_ENABLE()
#define OSPI_D3_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOH_CLK_ENABLE()
#define OSPI_D4_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOH_CLK_ENABLE()
#define OSPI_D5_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOH_CLK_ENABLE()
#define OSPI_D6_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOH_CLK_ENABLE()
#define OSPI_D7_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOH_CLK_ENABLE()
#define OSPI_DQS_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOG_CLK_ENABLE()

#define OSPI_FORCE_RESET()          __HAL_RCC_OSPI1_FORCE_RESET()
#define OSPI_RELEASE_RESET()        __HAL_RCC_OSPI1_RELEASE_RESET()

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
