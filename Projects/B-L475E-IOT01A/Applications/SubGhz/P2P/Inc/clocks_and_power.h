/**
******************************************************************************
  * @file    clocks_and_power.h
  * @author  MCD Application Team
  * @brief   This file provides functions prototypesfor setting the clocks or  
             entering low power modes.
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

#ifdef __cplusplus
 extern "C" {
#endif

#ifndef _CLOCKS_AND_POWER_H_
#define _CLOCKS_AND_POWER_H_

/* Includes ------------------------------------------------------------------*/
void SystemClock_Config(void);
void SystemClockConfig_ExitSTOPMode(void);
void SystemPower_Configuration(void);
void APPLI_EnterSTANDBYMode(void);
void APPLI_EnterSTOPMode(void);
void APPLI_EnterSLEEPMode(void);

#ifdef __cplusplus
}
#endif

#endif //_CLOCKS_AND_POWER_H_

