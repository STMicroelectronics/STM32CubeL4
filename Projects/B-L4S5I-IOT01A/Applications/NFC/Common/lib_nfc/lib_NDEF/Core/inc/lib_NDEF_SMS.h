/**
  ******************************************************************************
  * @file    lib_NDEF_SMS.h
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file help to manage SMS NDEF file.
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
#ifndef __LIB_NDEF_SMS_H
#define __LIB_NDEF_SMS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF.h"


typedef struct
{
  char PhoneNumber[16];
  char Message[400];
  char Information[400];
}sSMSInfo;

uint16_t NDEF_ReadSMS( sRecordInfo_t *pRecordStruct, sSMSInfo *pSMSStruct );
uint16_t NDEF_WriteSMS( sSMSInfo *pSMSStruct );
void NDEF_PrepareSMSMessage( sSMSInfo *pSMSStruct, uint8_t *pNDEFMessage, uint16_t *size );

#ifdef __cplusplus
}
#endif

#endif /* __LIB_NDEF_SMS_H */

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
