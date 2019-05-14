/**
  ******************************************************************************
  * @file    stm32l4xx_os_timebase_lptim.h 
  * @author  MCD Application Team
  * @brief   RTOS and HAL time base based on LPTIM header file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        http://www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef __RTOS_LPTIM_BASE_TIME_H
#define __RTOS_LPTIM_BASE_TIME_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void InitTick (uint32_t TickPriority);
void LPTIM1_IRQHandler(void);
  
#ifdef __cplusplus
}
#endif

#endif /* */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
