/**
  ******************************************************************************
  * @file    USB_Device/DFU_Standalone/Inc/usbd_dfu_flash.h
  * @author  MCD Application Team
  * @brief   Header for usbd_dfu_flash.c file.
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

#ifndef __USBD_DFU_FLASH_H_
#define __USBD_DFU_FLASH_H_

/* Includes ------------------------------------------------------------------*/
#include "usbd_dfu.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/ 
/* Exported macro ------------------------------------------------------------*/
/* Base address of the Flash sectors */
#define ADDR_FLASH_PAGE_0     ((uint32_t)0x08000000) /* Base @ of Page 0, 4 Kbytes */
#define ADDR_FLASH_PAGE_1     ((uint32_t)0x08001000) /* Base @ of Page 2, 4 Kbytes */
#define ADDR_FLASH_PAGE_2     ((uint32_t)0x08002000) /* Base @ of Page 4, 4 Kbytes */
#define ADDR_FLASH_PAGE_3     ((uint32_t)0x08003000) /* Base @ of Page 6, 4 Kbytes */
#define ADDR_FLASH_PAGE_4     ((uint32_t)0x08004000) /* Base @ of Page 8, 4 Kbytes */
#define ADDR_FLASH_PAGE_12    ((uint32_t)0x0800C000) /* Base @ of Page 24, 4 Kbytes */

extern USBD_DFU_MediaTypeDef  USBD_DFU_Flash_fops;

/* Exported functions ------------------------------------------------------- */

#endif /* __USBD_DFU_FLASH_H_ */

