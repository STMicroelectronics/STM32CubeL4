/**
  ******************************************************************************
  * @file    stm32l475e_iot01_gyro.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for the stm32l475e_iot01_gyro.c
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
#ifndef __STM32L475E_IOT01_GYRO_H
#define __STM32L475E_IOT01_GYRO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l475e_iot01.h"
/* Include Gyro component driver */
#include "../Components/lsm6dsl/lsm6dsl.h"   

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L475E_IOT01
  * @{
  */
      
/** @addtogroup STM32L475E_IOT01_GYROSCOPE 
  * @{
  */ 
   
/** @defgroup STM32L475_IOT01_GYROSCOPE_Exported_Constants GYROSCOPE Exported Constants
  * @{
  */
typedef enum 
{
  GYRO_OK = 0,
  GYRO_ERROR = 1,
  GYRO_TIMEOUT = 2
} 
GYRO_StatusTypeDef;

/**
  * @}
  */

/** @defgroup STM32L475E_IOT01_GYROSCOPE_Exported_Functions GYROSCOPE Exported Functions
  * @{
  */  
uint8_t BSP_GYRO_Init(void); 
void BSP_GYRO_DeInit(void);
void BSP_GYRO_LowPower(uint16_t status);   /* 0 Means Disable Low Power Mode, otherwise Low Power Mode is enabled */
void BSP_GYRO_GetXYZ(float* pfData);
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

#endif /* __STM32L475E_IOT01_GYRO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
