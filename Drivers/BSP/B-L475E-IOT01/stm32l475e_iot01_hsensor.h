/**
  ******************************************************************************
  * @file    stm32l475e_iot01_hsensor.h
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the humidity sensor
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
#ifndef __STM32L475E_IOT01_HSENSOR_H
#define __STM32L475E_IOT01_HSENSOR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l475e_iot01.h"
#include "../Components/hts221/hts221.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32L475E_IOT01
  * @{
  */

/** @addtogroup STM32L475E_IOT01_HUMIDITY 
  * @{
  */
    
/* Exported types ------------------------------------------------------------*/
/** @defgroup STM32L475E_IOT01_HUMIDITY_Exported_Types HUMIDITY Exported Types
  * @{
  */
   
/** 
  * @brief  HSENSOR Status  
  */ 
typedef enum
{
  HSENSOR_OK = 0,
  HSENSOR_ERROR
}HSENSOR_Status_TypDef;

/**
  * @}
  */

/** @defgroup STM32L475E_IOT01_HUMIDITY_Exported_Functions HUMIDITY Exported Functions
  * @{
  */
/* Sensor Configuration Functions */
uint32_t BSP_HSENSOR_Init(void);
uint8_t  BSP_HSENSOR_ReadID(void);
float    BSP_HSENSOR_ReadHumidity(void);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

#endif /* __STM32L475E_IOT01_HSENSOR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
