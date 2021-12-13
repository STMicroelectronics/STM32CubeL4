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
#ifndef WIN32
#include "stm32l4xx_hal.h"

/* EVAL includes component */
#include "stm32l4r9i_eval.h"
#include "stm32l4r9i_eval_io.h"
#include "stm32l4r9i_eval_ts.h"
#include "stm32l4r9i_eval_ospi_nor.h"
#include "stm32l4r9i_eval_sd.h"

#include "cmsis_os.h"

/* FatFs includes component */
#include "ff_gen_drv.h"

/* mSD includes component */
#include "sd_diskio.h"
#include "sd_diskio_dma.h"
#endif /* !WIN32 */

/* GUI includes components */
#include "GUI.h"
#include "DIALOG.h"
#include "LCDConf.h"
#ifndef WIN32
#include "ST_GUI_Addons.h"
#endif /* !WIN32 */

#ifndef WIN32
/* Kernel includes components */
#include "ts.h"
#include "mfx.h"
#include "storage.h"
#include "bsp.h"
#endif /* !WIN32 */

/* standard includes components */
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
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
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);
void BSP_ErrorHandler(void);

void TouchScreenEnable(void);
void TouchScreenDisable(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

