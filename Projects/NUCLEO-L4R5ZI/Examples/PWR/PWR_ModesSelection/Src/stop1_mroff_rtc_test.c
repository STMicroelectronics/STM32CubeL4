/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Src/stop1_mroff_rtc_test.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
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
#include "main.h"
#include <stdio.h>

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup PWR_ModesSelection
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void test_stop1_mroff_rtc(void)
{
  printf("\n\r Executing test (STOP1 + MR OFF + RTC) \n\r");
  printf(" Please measure current then use Reset button to select another test \n\r");

  /* set RTC clock */
  /*     we do not set the interrupt on RTC clock since we do not want to wakeup from STOP*/
  /*     we want to stay in STOP 1 mode with RTC clock activated for power conso */
  RTC_Config();
  
  /* Set all GPIO in analog state to reduce power consumption */
  GPIO_AnalogState_Config();
  
  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Enter STOP 1 mode (Main Regulator OFF and Low Power Regulator On) */
  HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);
  
}


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
