/**
  ******************************************************************************
  * @file    iddmeasure_app.c
  * @author  MCD Application Team
  * @brief   This file provides the IDD Measure application method.
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
  
/* Includes ------------------------------------------------------------------*/
#include "iddmeasure_app.h"

/** @addtogroup IDD_MEASURE_MODULE
  * @{
  */

/** @defgroup IDD_MEASURE_APPLICATION
  * @brief IDD Measure application routines 
  * @{
  */

/* External variables --------------------------------------------------------*/
extern void SystemClock_Config(void);

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static uint32_t IDDMEASURE_GetIDDMeasured(IDD_ModeTypdef LowPowerMode, uint32_t Calibration);

/**
  * @brief  Get Calibration Offset
  * @param  None.
  * @retval Calibration offset measured.
  */
uint32_t IDDMEASURE_GetCalibrationOffset(void)
{
  uint32_t calibration = 0;
  
  /* DeInitialize Demonstration Context for low power*/
  k_BspDeInitForLowPower();

  /* Suspend RTOS Systick */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
  
  /* Run Stop Mode */
  BSP_IDD_EnterStopMode2();
  
  /* Restore after Stop Mode */
  BSP_IDD_ExitStopMode();

  /* Restore clock to PLL 80MHz */
  SystemClock_Config();

  /* Get Current consumption of IDD components */
  calibration = IDDMEASURE_GetIDDMeasured(BSP_IDD_MODE_CALIBRATION, NULL);
  
  /* Resume RTOS Systick */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  /* Restore Demonstration Context */
  k_BspInitAfterLowPower();
  
  /* Return the Calibration Offset */
  return calibration;
}

/**
  * @brief  Get Stop Mode Measure Current Consumption
  * @param  Calibration: Offset Calibration.
  * @retval IDD value measured.
  */
uint32_t IDDMEASURE_GetStopConsumption(uint32_t Calibration)
{
  uint32_t iddmeasured = 0;

  /* DeInitialize Demonstration Context for low power*/
  k_BspDeInitForLowPower();

  /* Suspend RTOS Systick */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
  
  /* Run Stop Mode 2 */
  BSP_IDD_EnterStopMode2();
  
  /* Restore after Stop Mode */
  BSP_IDD_ExitStopMode();

  /* Restore clock to PLL 80MHz */
  SystemClock_Config();

  /* Get Current Consumption For Stop Mode 2 */
  iddmeasured = IDDMEASURE_GetIDDMeasured(BSP_IDD_MODE_STOP, Calibration);
  
  /* Resume RTOS Systick */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  /* Restore Demonstration Context */
  k_BspInitAfterLowPower();
  
  /* Return the Current Consumption */
  return iddmeasured;
}

/**
  * @brief  Get Sleep Mode Measure Current Consumption
  * @param  Calibration: Offset Calibration.
  * @retval IDD value measured.
  */
uint32_t IDDMEASURE_GetSleepConsumption(uint32_t Calibration)
{
  uint32_t iddmeasured = 0;

  /* DeInitialize Demonstration Context for low power*/
  k_BspDeInitForLowPower();

  /* Suspend RTOS Systick */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
  
  /* Run Sleep Mode */
  BSP_IDD_EnterSleepMode();

  /* Restore after Sleep Mode */
  BSP_IDD_ExitSleepMode();

  /* Get Current Consumption For Sleep Mode */
  iddmeasured = IDDMEASURE_GetIDDMeasured(BSP_IDD_MODE_SLEEP, Calibration);
  
  /* Resume RTOS Systick */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  /* Restore Demonstration Context */
  k_BspInitAfterLowPower();
  
  /* Return the Current Consumption */
  return iddmeasured;
}

/**
  * @brief  Get Run Mode Measure Current Consumption
  * @param  Calibration: Offset Calibration.
  * @retval IDD value measured.
  */
uint32_t IDDMEASURE_GetRunConsumption(uint32_t Calibration)
{
  uint32_t iddmeasured = 0;

  /* DeInitialize Demonstration Context for low power*/
  k_BspDeInitForLowPower();

  /* Start IDD calculation for RUN Mode */
  BSP_IDD_EnterRunMode();

  /* Get Current Consumption For Run Mode */
  iddmeasured = IDDMEASURE_GetIDDMeasured(BSP_IDD_MODE_RUN, Calibration);
  
  /* Restore Demonstration Context */
  k_BspInitAfterLowPower();
  
  /* Return the Current Consumption */
  return iddmeasured;
}

/**
  * @brief  Enter Standby Mode
  * @param  Calibration: Offset Calibration.
  * @retval IDD value measured.
  */
void IDDMEASURE_EnterStandbyMode(void)
{
  /* DeInitialize Demonstration Context for low power*/
  k_BspDeInitForLowPower();

  /* Suspend RTOS Systick */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
  
  /* Run Standby Mode */
  BSP_IDD_EnterStandbyMode();
}

/**
  * @brief  Launch Stop Mode to manage Calibration
  * @param  None.
  * @retval None.
  */
void IDDMEASURE_StopModeForJumperSwitching(void)
{
  /* Initialize Tamper Button for WakeUp */
  BSP_PB_Init(BUTTON_TAMPER,  BUTTON_MODE_EXTI);

  /* Run Stop Mode 1 */
  BSP_IDD_EnterStopMode1();
  
  /* Restore after Stop Mode */
  BSP_IDD_ExitStopMode();

  /* Restore clock to PLL 80MHz */
  SystemClock_Config();
}

/**
  * @brief  Get IDD measured
  * @param  LowPowerMode: Low Power Mode (STOP, SLEEP, RUN, STANDBY).
  * @param  Calibration: Offset Calibration..
  * @retval IDD value measured.
  */
static uint32_t IDDMEASURE_GetIDDMeasured(IDD_ModeTypdef LowPowerMode, uint32_t OffsetCalibration)
{
  uint32_t iddmeasured = 0;
  
  BSP_IDD_GetValue(LowPowerMode, OffsetCalibration, &iddmeasured);
  return iddmeasured;
}

/**
  * @}
  */

/**
  * @}
  */

