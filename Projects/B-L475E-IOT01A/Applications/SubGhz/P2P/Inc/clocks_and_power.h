/**
******************************************************************************
  * @file    clocks_and_power.h
  * @author  MCD Application Team
  * @brief   This file provides functions prototypesfor setting the clocks or  
             entering low power modes.
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
