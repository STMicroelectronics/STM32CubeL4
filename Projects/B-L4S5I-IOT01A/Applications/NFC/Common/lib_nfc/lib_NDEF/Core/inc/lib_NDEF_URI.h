/**
  ******************************************************************************
  * @file    lib_NDEF_URI.h
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file help to manage URI NDEF file.
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
#ifndef __LIB_NDEF_URI_H
#define __LIB_NDEF_URI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF.h"

typedef struct 
{
  char protocol[80];
  char URI_Message[400];
  char Information[400];
}sURI_Info;

uint16_t NDEF_ReadURI(sRecordInfo_t *pRecordStruct, sURI_Info *pURI);
uint16_t NDEF_WriteURI(sURI_Info *pURI);

void NDEF_PrepareURIMessage( sURI_Info *pURI, uint8_t *pNDEFMessage, uint16_t *size );
char getUriType( char *protocol );

#ifdef __cplusplus
}
#endif

#endif /* __LIB_NDEF_URI_H */

