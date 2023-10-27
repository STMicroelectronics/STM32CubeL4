/**
  ******************************************************************************
  * @file    lib_TT4_interface.h
  * @author  MMY Application Team
  * @brief   This file help to manage TagType4 NDEF library.
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
#ifndef __LIB_TT4_INTERFACE_H
#define __LIB_TT4_INTERFACE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF_URI.h"
#include "lib_NDEF_SMS.h"
#include "lib_NDEF_Email.h"
#include "lib_NDEF_Vcard.h"
#include "lib_NDEF_Geo.h"
#include "lib_NDEF_MyApp.h"   
#include "lib_NDEF_AAR.h"

/** @addtogroup NFC_libraries
  * @{
  */
  
/** @addtogroup TAGTYPE4_NDEF_INTERFACE
  * @{
  */

/**
 * @defgroup TAGTYPE4_NDEF_INTERFACE_Exported_Types             TAGTYPE4_NDEF_INTERFACE Exported Types
 * @{
 */

/**
  * @brief  CCfile structure
  */
typedef struct
{
  uint16_t NumberCCByte;
  uint8_t Version;
  uint16_t MaxReadByte;
  uint16_t MaxWriteByte;
  uint8_t TField;
  uint8_t LField;
  uint16_t FileID;
  uint16_t NDEFFileMaxSize;
  uint8_t ReadAccess;
  uint8_t WriteAccess;
}sCCFileInfo;

/**
  * @brief  Tag type supported
  */
typedef enum 
{
  UNKNOWN_TAG = 0,
  INTERFACE_TAG,
  URI_TAG,
  SMS_TAG,
  GEO_TAG,
  EMAIL_TAG,
  TEXT_TAG,
  BT_TAG,
  WIFI_TAG
} Tag_TypeDef;

/**
  * @}
  */
    
/**
 * @defgroup TAGTYPE4_NDEF_INTERFACE_Exported_FunctionPrototypes             TAGTYPE4_NDEF_INTERFACE Exported FunctionPrototypes
 * @{
 */

uint16_t TT4_Init( void );
uint16_t TT4_ReadNDEF(uint8_t *pNDEF);
uint16_t TT4_WriteNDEF(uint8_t *pNDEF);
uint16_t TT4_ReadURI(sURI_Info *pURI);
uint16_t TT4_WriteURI(sURI_Info *pURI);
uint16_t TT4_ReadSMS(sSMSInfo *pSMS);
uint16_t TT4_WriteSMS(sSMSInfo *pSMS);
uint16_t TT4_ReadEmail(sEmailInfo *pEmailStruct);
uint16_t TT4_WriteEmail(sEmailInfo *pEmailStruct);
uint16_t TT4_ReadVcard(sVcardInfo *pVcard);
uint16_t TT4_WriteVcard(sVcardInfo *pVcard);
uint16_t TT4_ReadGeo(sGeoInfo *pGeo);
uint16_t TT4_WriteGeo(sGeoInfo *pGeo);
uint16_t TT4_ReadMyApp(sMyAppInfo *pMyAppStruct);
uint16_t TT4_WriteMyApp(sMyAppInfo *pMyAppStruct);
uint16_t TT4_AddAAR(sAARInfo *pAAR);
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

#endif /* __LIB_TT4_INTERFACE_H */


