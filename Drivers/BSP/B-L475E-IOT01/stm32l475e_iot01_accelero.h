/**
  ******************************************************************************
  * @file    stm32l475e_iot01_accelero.h
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the accelerometer sensor
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
#ifndef __STM32L475E_IOT01_ACCELERO_H
#define __STM32L475E_IOT01_ACCELERO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l475e_iot01.h"
/* Include Accelero component driver */
#include "../Components/lsm6dsl/lsm6dsl.h"
   
/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L475E_IOT01
  * @{
  */
      
/** @addtogroup STM32L475E_IOT01_ACCELERO
  * @{
  */ 
   
/** @defgroup STM32L475_DISCOVERY_ACCELERO_Exported_Types ACCELERO Exported Types
  * @{
  */
typedef enum 
{
  ACCELERO_OK = 0,
  ACCELERO_ERROR = 1,
  ACCELERO_TIMEOUT = 2
} 
ACCELERO_StatusTypeDef;

/**
  * @}
  */

/** @defgroup STM32L475E_IOT01_ACCELERO_Exported_Functions ACCELERO Exported Functions
  * @{
  */
/* Sensor Configuration Functions */
ACCELERO_StatusTypeDef BSP_ACCELERO_Init(void);
void BSP_ACCELERO_DeInit(void);
void BSP_ACCELERO_LowPower(uint16_t status); /* 0 Means Disable Low Power Mode, otherwise Low Power Mode is enabled */
void BSP_ACCELERO_AccGetXYZ(int16_t *pDataXYZ);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __STM32L475E_IOT01_ACCELERO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
