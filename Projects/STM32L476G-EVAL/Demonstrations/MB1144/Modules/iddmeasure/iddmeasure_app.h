/**
  ******************************************************************************
  * @file    iddmeasure_app.h
  * @author  MCD Application Team
  * @brief   Header for iddmeasure_app.c file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IDDMEASURE_APP_H
#define __IDDMEASURE_APP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "k_bsp.h"

   
/* Exported constants --------------------------------------------------------*/   
/* Exported types ------------------------------------------------------------*/

typedef enum
{
  IDD_ERROR_NONE = 0, 
  IDD_ERROR_IO,
  IDD_ERROR_HW, 
  IDD_ERROR_UNKNOWN, 
  
}IDD_ErrorTypdef;

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint32_t IDDMEASURE_GetCalibrationOffset(void);
uint32_t IDDMEASURE_GetStopConsumption(uint32_t Calibration);
uint32_t IDDMEASURE_GetSleepConsumption(uint32_t Calibration);
uint32_t IDDMEASURE_GetRunConsumption(uint32_t Calibration);
void     IDDMEASURE_EnterStandbyMode(void);
void     IDDMEASURE_StopModeForJumperSwitching(void);

#ifdef __cplusplus
}
#endif

#endif /*__USBD_APP_H */

