/**
  ******************************************************************************
  * @file    stm32l475e_iot01_psensor.h
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the pressure sensor
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
#ifndef __STM32L475E_IOT01_PSENSOR_H
#define __STM32L475E_IOT01_PSENSOR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l475e_iot01.h"
#include "../Components/lps22hb/lps22hb.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32L475E_IOT01
  * @{
  */

/** @addtogroup STM32L475E_IOT01_PRESSURE 
  * @{
  */
   
/* Exported types ------------------------------------------------------------*/
/** @defgroup STM32L475E_IOT01_PRESSURE_Exported_Types PRESSURE Exported Types
  * @{
  */
   
/** 
  * @brief  PSENSOR Status  
  */ 
typedef enum
{
  PSENSOR_OK = 0,
  PSENSOR_ERROR
}PSENSOR_Status_TypDef;

/**
  * @}
  */

/** @defgroup STM32L475E_IOT01_PRESSURE_Exported_Functions PRESSURE Exported Functions
  * @{
  */
/* Sensor Configuration Functions */
uint32_t BSP_PSENSOR_Init(void);
uint8_t  BSP_PSENSOR_ReadID(void);
float    BSP_PSENSOR_ReadPressure(void);
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

#endif /* __STM32L475E_IOT01_PSENSOR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
