/**
  ******************************************************************************
  * @file    stm32l475e_iot01_magneto.h
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the magnetometer sensor
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
#ifndef __STM32L475E_IOT01_MAGNETO_H
#define __STM32L475E_IOT01_MAGNETO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l475e_iot01.h"   
/* Include Magnetometer component driver */
#include "../Components/lis3mdl/lis3mdl.h"  
   
/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L475E_IOT01
  * @{
  */
      
/** @addtogroup STM32L475E_IOT01_MAGNETO
  * @{
  */ 

/** @defgroup STM32L475_IOT01_MAGNETO_Exported_Types MAGNETO Exported Types
  * @{
  */   
/* Exported types ------------------------------------------------------------*/
typedef enum 
{
  MAGNETO_OK = 0,
  MAGNETO_ERROR = 1,
  MAGNETO_TIMEOUT = 2
} 
MAGNETO_StatusTypeDef;
/**
  * @}
  */

/** @defgroup STM32L475E_IOT01_MAGNETO_Exported_Functions MAGNETO Exported Functions
  * @{
  */ 
MAGNETO_StatusTypeDef BSP_MAGNETO_Init(void);
void BSP_MAGNETO_DeInit(void);
void BSP_MAGNETO_LowPower(uint16_t status); /* 0 Means Disable Low Power Mode, otherwise Low Power Mode is enabled */
void BSP_MAGNETO_GetXYZ(int16_t *pDataXYZ);
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

#endif /* __STM32L475E_IOT01_MAGNETO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
