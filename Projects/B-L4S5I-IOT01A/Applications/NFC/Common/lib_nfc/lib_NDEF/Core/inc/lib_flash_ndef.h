/**
  ******************************************************************************
  * @file    lib_flash_ndef.h 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file provides set of functions to write or read the NDEF file into flash.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _LIB_FLASH_NDEF_H
#define _LIB_FLASH_NDEF_H

/* Includes */
#include "common.h"

/* Flag to disable FLASH storage */
//#define DISABLE_NDEF_FLASH

/* Addresses for the storage of the NDEF files */
/* /!\ FLASH IS ERASED USING PAGES OF 2048 (0x800) BYTES /!\ */
#define PICCNFCT4A_NDEF_ADDRESS   0x08070000
#define PICCNFCT4B_NDEF_ADDRESS   (PICCNFCT4A_NDEF_ADDRESS+NFCT4_MAX_NDEFMEMORY+FLASH_MAGIC_NUMBER_LENTH+(2048-(NFCT4_MAX_NDEFMEMORY+FLASH_MAGIC_NUMBER_LENTH)%2048))
#define PICCNFCT3_NDEF_ADDRESS    (PICCNFCT4B_NDEF_ADDRESS+NFCT4_MAX_NDEFMEMORY+FLASH_MAGIC_NUMBER_LENTH+(2048-(NFCT4_MAX_NDEFMEMORY+FLASH_MAGIC_NUMBER_LENTH)%2048))
#define PICCNFCT2_NDEF_ADDRESS    (PICCNFCT3_NDEF_ADDRESS+NFCT3_MAX_TAGMEMORY+FLASH_MAGIC_NUMBER_LENTH+(2048-(NFCT3_MAX_TAGMEMORY+FLASH_MAGIC_NUMBER_LENTH)%2048))

/* This number is used to know if the flash has been formated before or not */
#define FLASH_MAGIC_NUMBER_LENTH  4 /* Size in bytes */
#define FLASH_MAGIC_NUMBER        0x01234567

/* Error codes */
#define FLASH_OK                  0x00
#define FLASH_NO_NDEF             0x01
// Redifined #define FLASH_ERROR_SIZE 				0x02

/* Functions */
void initFlashNDEF( void );
void manageFlashNDEF( void );
uint8_t writeNDEFToFlash( uint8_t *CardNDEFfile, uint32_t size, uint32_t address );
uint8_t readNDEFFromFlash( uint8_t *CardNDEFfile, uint32_t size, uint32_t address );

#endif /* _LIB_FLASH_NDEF_H */

