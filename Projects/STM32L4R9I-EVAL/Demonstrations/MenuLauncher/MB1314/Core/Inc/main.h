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
#ifndef GUI_SIMULATOR
#include "stm32l4xx_hal.h"

/* EVAL includes component */
#include "stm32l4r9i_eval.h"
#include "stm32l4r9i_eval_io.h"
#include "stm32l4r9i_eval_lcd.h"
#include "stm32l4r9i_eval_ts.h"
#include "stm32l4r9i_eval_sram.h"
#include "stm32l4r9i_eval_ospi_nor.h"
#include "stm32l4r9i_eval_idd.h"
#include "stm32l4r9i_eval_sd.h"
#include "stm32l4r9i_eval_audio.h"

/* CMSIS OS includes component */
#include "cmsis_os.h"

/* FatFs includes components */
#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio_dma.h"
#endif /* !GUI_SIMULATOR */

/* GUI includes components */
#include "GUI.h"
#include "DIALOG.h"
#include "LCDConf.h"

#ifndef GUI_SIMULATOR
/* Kernel includes components */
#include "ts.h"
#include "mfx.h"
#include "storage.h"
#include "bsp.h"
#endif /* !GUI_SIMULATOR */

/* standard includes components */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint32_t       demo_id;
  unsigned char  demo_descr[50];
} Demo_Header_t;

typedef enum
{
  DEMO_ID_0,
  DEMO_ID_1,
  DEMO_ID_2,
  DEMO_ID_NB,
} Demo_Id_t;

/* Exported variables --------------------------------------------------------*/
/* Flag to indicate whether or not software is coming back from a software reset */
extern uint32_t software_reset_flag;

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

