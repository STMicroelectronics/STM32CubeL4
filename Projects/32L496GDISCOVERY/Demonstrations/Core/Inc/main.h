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
   
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_it.h"

#include "stm32l496g_discovery.h"
#include "stm32l496g_discovery_lcd.h"
#include "stm32l496g_discovery_ts.h"
   
/* DISCO includes component */
#include "stm32l496g_discovery_audio.h"
#include "stm32l496g_discovery_sd.h"
#include "stm32l496g_discovery_qspi.h"
   
/* CMSIS OS includes component */
#include "cmsis_os.h"   

/* FatFs includes components */
#include "ff_gen_drv.h"
#include "sd_diskio.h"

#include "usbd_core.h"
   
/* GUI includes components */
#include "GUI.h"
#include "DIALOG.h"
#include "ST_GUI_Addons.h"
 
/* Kernel includes components */
#include "k_module.h"
#include "k_storage.h"
#include "k_rtc.h"    
#include "k_bsp.h"   
#include "k_calibration.h"
#include "k_mfx.h"
   
/* utilities and standard includes components */   
#include "cpu_utils.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


   
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  ULP_STOP2    = 0x00,
  ULP_STANDBY  = 0x01,
  ULP_OFF      = 0x02
} ULPMode;

typedef enum
{
  HWINIT_ERROR    = 0x00,
  HWINIT_STARTUP  = 0x01,
  HWINIT_IDD      = 0x02,
  HWINIT_STOP2    = 0x03,
  HWINIT_STANDBY  = 0x04
} HwInitMode;


typedef enum
{
  BACKUP_IDD        = 0x00,  /* IDD application backup data     */
  BACKUP_SETTINGS   = 0x01   /* Kernel settings backup data     */
} DemoBackupId;

typedef struct
{
  uint32_t mode;  
} IddBackupData_TypeDef;


typedef struct
{
  uint32_t lpmode;
} SettingsBackupData_TypeDef;

typedef struct
{
  /* IDD application */
  IddBackupData_TypeDef      idd;
  /* Global settings */
  SettingsBackupData_TypeDef settings;
} DemoBackupData_TypeDef;

typedef struct
{
  uint32_t       demo_id;
    unsigned char  demo_descr[50];
} Demo_Header_t;

/* Exported variables --------------------------------------------------------*/
extern FlagStatus DemoDebugMode;
extern FlagStatus WakeUpFromStandby;
extern FlagStatus WakeUpFromShutdown;
/* Exported constants --------------------------------------------------------*/
#define CALIBRATION_GENERAL_SETTINGS_BKP    RTC_BKP_DR2
#define DEMO_HOME_ADDRESS           0x08000000   

   
   
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void      SystemClock_Config(void);
void      SystemLowClock_Config(void);
void      SystemHardwareInit(HwInitMode Mode);
void      SystemHardwareDeInit(HwInitMode Mode);
void      SystemPeripheralClockEnable(void);
void      SystemPeripheralClockDisable(void);
void      EnterStop2Mode(void);
void      ExitStop2Mode(void);
void      Error_Handler(void);



#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

