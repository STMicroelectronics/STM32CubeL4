/**
  ******************************************************************************
  * @file    USB_Device/MSC_Standalone/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/

#include "stm32l4p5g_discovery.h"
#include "stm32l4p5g_discovery_mmc.h"
#ifdef EXT_FLASH_ACCESS
#include "stm32l4p5g_discovery_ospi_nor.h"
#endif

/* USB */
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_storage.h"

/* FATFS */
#include "ff.h"
#include "ff_gen_drv.h"
#include "mmc_diskio.h"

/* PNG */
#include "png.h"


/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Enable one of the following defines to select which clock will be used for USB */
/* Use MSI clock automatically trimmed by LSE as USB clock */
#define USB_USE_LSE_MSI_CLOCK
/* Use HSI48 clock */
/* #define USE_USB_CLKSOURCE_CRSHSI48 */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);
void Warning_Handler(void);
void Warning_Clear(void);
#endif /* __MAIN_H */

