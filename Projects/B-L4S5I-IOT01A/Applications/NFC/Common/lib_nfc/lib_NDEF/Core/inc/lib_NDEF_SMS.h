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

