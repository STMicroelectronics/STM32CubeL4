/**
 ******************************************************************************
 * @file    lpm.c
 * @author  MCD Application Team
 * @brief   Low Power Manager
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

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "hw.h"

#include "lpm.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t LowPowerModeSel = 0;
static uint32_t SysClockReq = 0;
static LPM_Conf_t LowPowerModeConfiguration;

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


/* Functions Definition ------------------------------------------------------*/
void LPM_SetConf(LPM_Conf_t *p_conf)
{
  LowPowerModeConfiguration.Stop_Mode_Config = p_conf->Stop_Mode_Config;
  LowPowerModeConfiguration.OFF_Mode_Config = p_conf->OFF_Mode_Config;

  return;
}

void LPM_SetOffMode(uint32_t id, LPM_OffModeSel_t mode)
{
  uint32_t primask_bit;

  primask_bit = __get_PRIMASK();  /**< backup PRIMASK bit */
  __disable_irq();                /**< Disable all interrupts by setting PRIMASK bit on Cortex*/

  if(mode == LPM_OffMode_En)
  {
    LowPowerModeSel &= (~(1<<id));
  }
  else
  {
    LowPowerModeSel |= (1<<id);
  }

  __set_PRIMASK(primask_bit);     /**< Restore PRIMASK bit*/

  return;
}

void LPM_SetStopMode(uint32_t id, LPM_StopModeSel_t mode)
{
  uint32_t primask_bit;

  primask_bit = __get_PRIMASK();  /**< backup PRIMASK bit */
  __disable_irq();      /**< Disable all interrupts by setting PRIMASK bit on Cortex*/

  if(mode == LPM_StopMode_Dis)
  {
    SysClockReq &= (~(1<<id));
  }
  else
  {
    SysClockReq |= (1<<id);
  }

  __set_PRIMASK(primask_bit); /**< Restore PRIMASK bit*/

  return;
}

void LPM_EnterModeSelected(void)
{
  if(SysClockReq)
  {
    /**
     * SLEEP mode is required
     */
    LPM_EnterSleepMode();
    HW_SleepMode();
    LPM_ExitSleepMode();
  }
  else
  {
    if(LowPowerModeSel)
    {
      /**
       * STOP mode is required
       */
      LPM_EnterStopMode();
      HW_StopMode(LowPowerModeConfiguration.Stop_Mode_Config);
      LPM_ExitStopMode();
    }
    else
    {
      /**
       * OFF mode is required
       */
      LPM_EnterOffMode();
      HW_OffMode(LowPowerModeConfiguration.OFF_Mode_Config);
      LPM_ExitOffMode();
    }
  }

  return;
}

LPM_ModeSelected_t LPM_ReadModeSel(void)
{
  LPM_ModeSelected_t mode_selected;
  uint32_t primask_bit;

  primask_bit = __get_PRIMASK();  /**< backup PRIMASK bit */
  __disable_irq();                /**< Disable all interrupts by setting PRIMASK bit on Cortex*/

  if(SysClockReq)
  {
    mode_selected = LPM_SleepMode;
  }
  else
  {
    if(LowPowerModeSel)
    {
      mode_selected = LPM_StopMode;
    }
    else
    {
      mode_selected = LPM_OffMode;
    }
  }

  __set_PRIMASK(primask_bit); /**< Restore PRIMASK bit*/

  return mode_selected;
}


/* __WEAK functions: The application can overload this function  if needed ----*/
/* ----------------------------------------------------------------------------*/

__weak void LPM_EnterSleepMode(void)
{
}
__weak void LPM_ExitSleepMode(void)
{
}
__weak void LPM_EnterStopMode(void)
{
}
__weak void LPM_ExitStopMode(void)
{
}
__weak void LPM_EnterOffMode(void)
{
}
__weak void LPM_ExitOffMode(void)
{
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
