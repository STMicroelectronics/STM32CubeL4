/**
  ******************************************************************************
  * @file    k_temperature.h
  * @author  MCD Application Team
  * @brief   Header for k_temperature.c file
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
#ifndef __K_TEMPERATURE_H
#define __K_TEMPERATURE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dialog.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void k_TemperatureInit(void);
void k_TemperatureDeInit(void);

uint8_t k_GetTemperature(void);

#ifdef __cplusplus
}
#endif

#endif /*__K_TEMPERATURE_H */

