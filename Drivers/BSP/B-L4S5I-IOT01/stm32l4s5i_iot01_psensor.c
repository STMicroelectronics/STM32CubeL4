/**
  ******************************************************************************
  * @file    stm32l4s5i_iot01_psensor.c
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

/* Includes ------------------------------------------------------------------*/
#include "stm32l4s5i_iot01_psensor.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32L4S5I_IOT01
  * @{
  */

/** @defgroup STM32L4S5I_IOT01_PRESSURE PRESSURE
  * @{
  */

/** @defgroup STM32L4S5I_IOT01_PRESSURE_Private_Variables PRESSURE Private Variables 
  * @{
  */ 
static PSENSOR_DrvTypeDef *Psensor_drv;  
/**
  * @}
  */

/** @defgroup STM32L4S5I_IOT01_PRESSURE_Private_Functions PRESSURE Private Functions
  * @{
  */ 

/**
  * @brief  Initializes peripherals used by the I2C Pressure Sensor driver.
  * @retval PSENSOR status
  */
uint32_t BSP_PSENSOR_Init(void)
{
  uint32_t ret;
   
  if(LPS22HB_P_Drv.ReadID(LPS22HB_I2C_ADDRESS) != LPS22HB_WHO_AM_I_VAL)
  {
    ret = PSENSOR_ERROR;
  }
  else
  {
     Psensor_drv = &LPS22HB_P_Drv;
     
    /* PSENSOR Init */   
    Psensor_drv->Init(LPS22HB_I2C_ADDRESS);
    ret = PSENSOR_OK;
  }
  
  return ret;
}

/**
  * @brief  Read ID of LPS22HB.
  * @retval LPS22HB ID value.
  */
uint8_t BSP_PSENSOR_ReadID(void)
{ 
  return Psensor_drv->ReadID(LPS22HB_I2C_ADDRESS);
}

/**
  * @brief  Read Pressure register of LPS22HB.
  * @retval LPS22HB measured pressure value.
  */
float BSP_PSENSOR_ReadPressure(void)
{ 
  return Psensor_drv->ReadPressure(LPS22HB_I2C_ADDRESS);
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
