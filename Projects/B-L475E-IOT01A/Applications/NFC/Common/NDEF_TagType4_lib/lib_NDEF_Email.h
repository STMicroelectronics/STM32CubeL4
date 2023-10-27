/**
  ******************************************************************************
  * @file    lib_NDEF_Email.h
  * @author  MMY Application Team
  * @brief   This file help to manage Email NDEF file.
   ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014 STMicroelectronics.
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

/** @addtogroup NFC_libraries
  * @{
  */

/** @addtogroup TAGTYPE4_NDEF_EMAIL
  * @{
  */

  /**
 * @defgroup TAGTYPE4_NDEF_EMAIL_Exported_Types             TAGTYPE4_NDEF_EMAIL Exported Types
 * @{
 */

/**
  * @brief  Email structure, to store email add, subject, Message and optional information
  */   
typedef struct
{
  char EmailAdd[64];
  char Subject[100];
  char Message[400];
  char Information[400];
}sEmailInfo;

/**
  * @}
  */
    
/**
 * @defgroup TAGTYPE4_NDEF_EMAIL_Exported_FunctionPrototypes             TAGTYPE4_NDEF_EMAIL Exported FunctionPrototypes
 * @{
 */

uint16_t NDEF_ReadEmail(sRecordInfo *pRecordStruct, sEmailInfo *pEmailStruct);
uint16_t NDEF_WriteEmail( sEmailInfo *pEmailStruct );

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif
   
#endif /* __LIB_NDEF_EMAIL_H */

