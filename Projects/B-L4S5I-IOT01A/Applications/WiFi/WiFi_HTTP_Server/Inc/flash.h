/**
  ******************************************************************************
  * @file    flash.h
  * @author  MCD Application Team
  * @brief   Management of the internal flash memory.
  *          Header for flash.c
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
#ifndef flash_H
#define flash_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

int FLASH_update(uint32_t dst_addr, const void *data, uint32_t size);
int FLASH_Write(uint32_t uDestination, uint32_t *pSource, uint32_t uLength);
int FLASH_Erase_Size(uint32_t uStart, uint32_t uLength);




#ifdef __cplusplus
}
#endif

#endif /* flash_H */



