/**
  ******************************************************************************
  * @file    stm32l475e_iot01_tsensor.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the temperature sensor
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

/* Includes ------------------------------------------------------------------*/
#include "stm32l475e_iot01_tsensor.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32L475E_IOT01
  * @{
  */

/** @defgroup STM32L475E_IOT01_TEMPERATURE TEMPERATURE
  * @{
  */

/** @defgroup STM32L475E_IOT01_TEMPERATURE_Private_Variables TEMPERATURE Private Variables 
  * @{
  */ 
static TSENSOR_DrvTypeDef *tsensor_drv;  
/**
  * @}
  */

/** @defgroup STM32L475E_IOT01_TEMPERATURE_Private_Functions TEMPERATURE Private Functions
  * @{
  */

/**
  * @brief  Initializes peripherals used by the I2C Temperature Sensor driver.
  * @retval TSENSOR status
  */
uint32_t BSP_TSENSOR_Init(void)
{  
  uint8_t ret = TSENSOR_ERROR;

#ifdef USE_LPS22HB_TEMP
  tsensor_drv = &LPS22HB_T_Drv;
#else /* USE_HTS221_TEMP */
  tsensor_drv = &HTS221_T_Drv; 
#endif

  /* Low level init */
  SENSOR_IO_Init();

  /* TSENSOR Init */   
  tsensor_drv->Init(TSENSOR_I2C_ADDRESS, NULL);

  ret = TSENSOR_OK;
  
  return ret;
}

/**
  * @brief  Read Temperature register of TS751.
  * @retval STTS751 measured temperature value.
  */
float BSP_TSENSOR_ReadTemp(void)
{ 
  return tsensor_drv->ReadTemp(TSENSOR_I2C_ADDRESS);
}

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
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
