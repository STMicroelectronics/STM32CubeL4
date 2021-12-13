/**
  ******************************************************************************
  * @file    stm32l4xx_os_timebase_lptim.h 
  * @author  MCD Application Team
  * @brief   RTOS and HAL time base based on LPTIM header file.
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

