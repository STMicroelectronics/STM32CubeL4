/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team 
  * @brief   Header for main.c file
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

// #define USE_IMAGE_BROWSER_MODULE

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_it.h"

/* EVAL includes component */
#include "stm32l4r9i_eval.h"
#include "stm32l4r9i_eval_io.h"
#include "stm32l4r9i_eval_nor.h"
#include "stm32l4r9i_eval_ospi_nor.h"
#include "stm32l4r9i_eval_ts.h"
#include "stm32l4r9i_eval_sd.h"
#include "stm32l4r9i_eval_audio.h"
   
#include "cmsis_os.h"

/* FatFs includes component */
#include "ff_gen_drv.h"

/* USBD includes */
#include "usbd_msc.h"
#include "usbd_desc.h"

#if USE_USB_DISK == 1
/* USBH includes component */
#include "usbh_msc.h"
#include "usbh_diskio.h"
#endif /* USE_USB_DISK */

/* mSD includes component */
#include "sd_diskio.h"

#include "GUI.h"
#include "DIALOG.h"
#include "ST_GUI_Addons.h"

/* Kernel includes components */
#include "k_module.h"
#include "k_storage.h"
#include "k_rtc.h"    
#include "k_bsp.h"
#include "k_mfx.h"

/* utilities and standard includes components */   
#include "cpu_utils.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Enable one of the following defines to select which clock will be used for USB */
/* Use MSI clock automatically trimmed by LSE as USB clock */
/* #define USB_USE_LSE_MSI_CLOCK */
/* Use HSI48 clock */
#define USE_USB_CLKSOURCE_CRSHSI48

/* Exported types ------------------------------------------------------------*/
typedef union
{
  uint32_t d32;
  struct
  {
    uint32_t enable_sprite     : 1;
    uint32_t enable_background : 1;
    uint32_t use_120Mhz        : 1;
    uint32_t disable_flex_skin : 1;  
  }b;
}
SystemSettingsTypeDef;

/* Exported variables --------------------------------------------------------*/
extern GUI_CONST_STORAGE GUI_BITMAP bmSTLogo40x20;
/* Exported constants --------------------------------------------------------*/
typedef enum disk_units_e
{
  MSD_DISK_UNIT
, USB_DISK_UNIT
, NUM_DISK_UNITS
} disk_units_t;

#define AUDIO_DEFAULT_VOLUME 50

typedef enum STemWin_Modules_id_e
{
  MODULE_ID_MAIN
, MODULE_ID_AUDIO_PLAYER
, MODULE_ID_AUDIO_RECORDER
, MODULE_ID_HOME_ALARM
, MODULE_ID_GAME
, MODULE_ID_USBD
#if defined (USE_IMAGE_BROWSER_MODULE)
, MODULE_ID_FILE_BROWSER
, MODULE_ID_IMAGE_VIEWER
#else
, MODULE_ID_GARDENING
//, MODULE_ID_VIDEO_PLAYER
, MODULE_ID_BENCHMARK
#endif /* USE_IMAGE_BROWSER_MODULE */
, MODULE_ID_SETTINGS
, MODULE_ID_LAST
} STemWin_Modules_id_t;

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void Error_Handler(void);
void BSP_ErrorHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

