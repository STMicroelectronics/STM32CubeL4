/**
  ******************************************************************************
  * @file    lib_NDEF_Email.h
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file help to manage Email NDEF file.
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
#ifndef __LIB_NDEF_EMAIL_H
#define __LIB_NDEF_EMAIL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF.h"

/* "mailto:customer.service@st.com?subject=M24SR S/N 754FHFGJF46G329 WARRANTY&body=this is an auomatic warranty activation email" */

#ifdef NDEF_DYN_ALLOC
typedef struct
{
  char *EmailAdd;
  char *Subject;
  char *Message;
  char *Information;
}sEmailInfo;
#else
typedef struct
{
  char EmailAdd[64];
  char Subject[100];
  char Message[200];
  char Information[200];
}sEmailInfo;
#endif
uint16_t NDEF_ReadEmail( sRecordInfo_t *pRecordStruct, sEmailInfo *pEmailStruct );
uint16_t NDEF_WriteEmail( sEmailInfo *pEmailStruct );
void NDEF_PrepareEmailMessage( sEmailInfo *pEmailStruct, uint8_t *pNDEFMessage, uint16_t *size );
void NDEF_closeEmail( sEmailInfo *pEmailStruct);

#ifdef __cplusplus
}
#endif

#endif /* __LIB_NDEF_EMAIL_H */

