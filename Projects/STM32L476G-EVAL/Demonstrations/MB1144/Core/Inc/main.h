/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32l4xx_hal.h"
#include "stm32l4xx_it.h"

/* EVAL includes component */
#include "stm32l476g_eval.h"
#include "stm32l476g_eval_ts.h"
#include "stm32l476g_eval_io.h"
#include "stm32l476g_eval_sd.h"
#include "stm32l476g_eval_audio.h"
#include "stm32l476g_eval_sram.h"
#include "stm32l476g_eval_nor.h"
#include "stm32l476g_eval_idd.h"
   
/* CMSIS OS includes component */
#include "cmsis_os.h"   

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "k_sd_diskio.h"
#include "gui.h"

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern GUI_CONST_STORAGE GUI_BITMAP bmSTLogo40x20;
/* Exported constants --------------------------------------------------------*/
#define NUM_DISK_UNITS       1
#define MSD_DISK_UNIT        0
#define AUDIO_DEFAULT_VOLUME 70

#define CALIBRATION_BKP0                     RTC_BKP_DR0
#define CALIBRATION_BKP1                     RTC_BKP_DR1
#define CALIBRATION_BKP2                     RTC_BKP_DR2
#define CALIBRATION_BKP3                     RTC_BKP_DR3
#define CALIBRATION_IMAGE_SETTINGS_BKP       RTC_BKP_DR4
#define CALIBRATION_AUDIOPLAYER_SETTING_BKP  RTC_BKP_DR5
#define CALIBRATION_AUDIORECODER_SETTING_BKP RTC_BKP_DR6
#define CALIBRATION_IDDMEASURE_SETTING_BKP   RTC_BKP_DR7

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
extern void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

