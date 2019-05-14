/**
 ******************************************************************************
 * @file    hw_lpm.c
 * @author  MCD Application Team
 * @brief   hw to low power manager
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

void HW_SleepMode(void)
{
  LL_LPM_EnableSleep(); /**< Clear SLEEPDEEP bit of Cortex System Control Register */

  /**
   * This option is used to ensure that store operations are completed
   */
#if defined ( __CC_ARM)
  __force_stores();
#endif

  __WFI();

  return;
}

void HW_StopMode(uint32_t configuration)
{
  LL_PWR_SetPowerMode(configuration);

  LL_LPM_EnableDeepSleep(); /**< Set SLEEPDEEP bit of Cortex System Control Register */

  /**
   * This option is used to ensure that store operations are completed
   */
#if defined ( __CC_ARM)
  __force_stores();
#endif

  __WFI();

  return;
}

void HW_OffMode(uint32_t configuration)
{
  LL_PWR_SetPowerMode(configuration);

  LL_LPM_EnableDeepSleep(); /**< Set SLEEPDEEP bit of Cortex System Control Register */

  /**
   * This option is used to ensure that store operations are completed
   */
#if defined ( __CC_ARM)
  __force_stores();
#endif

  __WFI();

  return;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
