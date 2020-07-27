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
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under ST MYLIBERTY SOFTWARE LICENSE AGREEMENT (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/myliberty  
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
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

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
