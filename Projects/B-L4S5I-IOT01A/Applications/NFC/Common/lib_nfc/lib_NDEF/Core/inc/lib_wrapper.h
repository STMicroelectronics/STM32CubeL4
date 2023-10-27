/**
  ******************************************************************************
  * @file    lib_wrapper.h
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file help to have upper layer independent from HW
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
#ifndef __LIB_WRAPPER_H
#define __LIB_WRAPPER_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"


typedef enum {
  NFCTAG_TYPE5 = 0,
  NFCTAG_TYPE4,
  NFCTAG_TYPE3,
  NFCTAG_TYPE2,
  NFCTAG_TYPE1,
  NFCTAG_NOTSET
} NFCTAG_Protocol_Id_t;


uint16_t NfcTag_SelectProtocol(NFCTAG_Protocol_Id_t protocol);
uint16_t NfcTag_ReadNDEF( uint8_t* pData );
uint16_t NfcTag_WriteNDEF(uint16_t Length, uint8_t* pData );
uint16_t NfcTag_WriteProprietary(uint16_t Length, uint8_t* pData );
uint16_t NfcTag_GetLength(uint16_t* Length);

#endif /* __LIB_WRAPPER_H */


