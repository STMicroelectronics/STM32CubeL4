/**
  ******************************************************************************
  * @file    iddmeasure.h
  * @author  MCD Application Team   
  * @brief   Idd Measurement functions
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

#undef GLOBAL
#ifdef __LOWPOWER_APP_C
#define GLOBAL
#else
#define GLOBAL extern
#endif

/** @addtogroup  IDD_MEASURE_MODULE
  * @{
  */

/** @defgroup  IDD_MEASURE

  * @brief Idd Measure routines 
  * @{
  */

/* Exported variables --------------------------------------------------------*/
GLOBAL const K_ModuleItem_Typedef ModuleIddMeasure;
GLOBAL uint32_t MfxFwVersion;

/* Exported typedef -----------------------------------------------------------*/
typedef enum
{
  IDD_RUN       = 0x00,
  IDD_SLEEP     = 0x01,
  IDD_LPR_2MHZ  = 0x02,
  IDD_LPR_SLEEP = 0x03,
  IDD_STOP2     = 0x04,
  IDD_STANDBY   = 0x05,
  IDD_SHUTDOWN  = 0x06,
  IDD_TEST_NB   = 0x07,
} Idd_StateTypeDef;

/* Exported defines -----------------------------------------------------------*/
/* Exported macros ------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
KMODULE_RETURN IddMeasureInit(void);
IDD_StatusTypeDef IddMeasureGetAndDisplayValue(void);

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

