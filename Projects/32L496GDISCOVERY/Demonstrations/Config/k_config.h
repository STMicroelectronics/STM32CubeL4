/**
  ******************************************************************************
  * @file    k_config.h
  * @author  MCD Application Team
  * @brief   Header for k_config.c file
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
#ifndef __K_CONFIG_H
#define __K_CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define DISCOVERY_BOARD   "      STM32L496G-DISCOVERY"
#define DEMO_VERSION      "      VERSION 0.0.1"
#define MFX_INFO          "      MFX VERSION x.y"
#define DEMO_DATE         "      DD-MONTH-2016"
#define DEMO_INFO1        "      STMICROELECTRONICS"
#define DEMO_INFO2        "      COPYRIGHT 2016"
   
typedef enum{
 MODULE_MAIN_APP,
 MODULE_IDDMEASURE,
 MODULE_AUDIOPLAYER,
 MODULE_AUDIORECORDER,
 MODULE_COMPASS,
 MODULE_SOUNDMETER,
 MODULE_BATTERYMEASURE,
 MODULE_GUITARTUNER,
 MODULE_LOG_DEMO,
 MODULE_SYSTEM_INFO,
 MODULE_MAX,
 MODULE_NONE = 0xFF
} MODULES_INFO;

#define countof(a) (sizeof(a) / sizeof(*(a)))
/* Exported functions ------------------------------------------------------- */ 

#ifdef __cplusplus
}
#endif

#endif /*__K_CONFIG_H */

