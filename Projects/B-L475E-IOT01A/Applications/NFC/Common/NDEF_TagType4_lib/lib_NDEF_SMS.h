/**
  ******************************************************************************
  * @file    lib_NDEF_SMS.h
  * @author  MMY Application Team
  * @brief   This file help to manage SMS NDEF file.
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
#ifndef __LIB_NDEF_SMS_H
#define __LIB_NDEF_SMS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF.h"
   
/** @addtogroup NFC_libraries
  * @{
  */

/** @addtogroup TAGTYPE4_NDEF_SMS
  * @{
  */

  /**
 * @defgroup TAGTYPE4_NDEF_SMS_Exported_Types             TAGTYPE4_NDEF_SMS Exported Types
 * @{
 */

/**
  * @brief  SMS structure, to store Phone Number, Message and optional information
  */      
typedef struct 
{
  char PhoneNumber[16];
  char Message[400];
  char Information[400];
}sSMSInfo;
/**
  * @}
  */
    
/**
 * @defgroup TAGTYPE4_NDEF_SMS_Exported_FunctionPrototypes             TAGTYPE4_NDEF_SMS Exported FunctionPrototypes
 * @{
 */
uint16_t NDEF_ReadSMS(sRecordInfo *pRecordStruct, sSMSInfo *pSMSStruct);
uint16_t NDEF_WriteSMS( sSMSInfo *pSMSStruct );
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
   
#endif /* __LIB_NDEF_SMS_H */

