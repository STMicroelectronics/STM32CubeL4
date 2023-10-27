/**
  ******************************************************************************
  * @file    lib_NDEF_Vcard.h
  * @author  MMY Application Team
  * @brief   This file help to manage Vcard NDEF file.
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
#ifndef __LIB_NDEF_VCARD_H
#define __LIB_NDEF_VCARD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF.h"

/** @addtogroup NFC_libraries
  * @{
  */


/** @addtogroup TAGTYPE4_NDEF_VCARD
  * @{
  */

/**
 * @defgroup TAGTYPE4_NDEF_VCARD_Exported_Types             TAGTYPE4_NDEF_VCARD Exported Types
 * @{
 */
 
/**
  * @brief  Vcard structure, to store all vcard information
  */
typedef struct 
{
  char Version [10];
  char FirstName[80];
  char Title[80];
  char Org[80];
  char HomeAddress[80];
  char WorkAddress[80];
  char HomeTel[40];
  char WorkTel[40];
  char CellTel[40];
  char HomeEmail[80];
  char WorkEmail[80];
}sVcardInfo;   

/**
  * @}
  */
    
/**
 * @defgroup TAGTYPE4_NDEF_VCARD_Exported_Defines             TAGTYPE4_NDEF_VCARD Exported Defines
 * @{
 */
 
/** 
  *@brief Vcard version supported 2.1, not updated to 3.0 so far
  */
#define VCARD_VERSION_2_1             "2.1"  
#define VCARD_VERSION_2_1_SIZE        3 

#define VCARD_VERSION_3_0             "3.0"  
#define VCARD_VERSION_3_0_SIZE        3   
 
/** 
  *@brief Vcard keyword to parse message in v2.1
  */
#define BEGIN                         "BEGIN:"  
#define VCARD                         "VCARD"   
#define VERSION                       "VERSION:"
#define FIRSTNAME                     "FN:"
#define HOME_TEL                      "TEL;HOME:"   
#define WORK_TEL                      "TEL;WORK:"   
#define CELL_TEL                      "TEL;CELL:"   
#define HOME_EMAIL                    "EMAIL;HOME:"
#define WORK_EMAIL                    "EMAIL;WORK:"
#define HOME_ADDRESS                  "ADR;HOME:"
#define WORK_ADDRESS                  "ADR;WORK:"
#define TITLE                         "TITLE:"
#define ORG                           "ORG:"
#define END                           "END:"
#define JPEG                          "JPEG"  
   
#define LIMIT                         "\r\n"

#define BEGIN_STRING_SIZE             6   
#define VCARD_STRING_SIZE             5
#define VERSION_STRING_SIZE           8
#define FIRSTNAME_STRING_SIZE         3
#define HOME_TEL_STRING_SIZE          9   
#define WORK_TEL_STRING_SIZE          9   
#define CELL_TEL_STRING_SIZE          9   
#define HOME_EMAIL_STRING_SIZE        11
#define WORK_EMAIL_STRING_SIZE        11
#define HOME_ADDRESS_STRING_SIZE      9
#define WORK_ADDRESS_STRING_SIZE      9
#define TITLE_STRING_SIZE             6
#define ORG_STRING_SIZE               4
#define END_STRING_SIZE               4
#define JPEG_STRING_SIZE              4  
   
#define LIMIT_STRING_SIZE             2

/**
  * @}
  */
    
/**
 * @defgroup TAGTYPE4_NDEF_VCARD_Exported_FunctionPrototypes             TAGTYPE4_NDEF_VCARD Exported FunctionPrototypes
 * @{
 */
      
uint16_t NDEF_ReadVcard ( sRecordInfo *pRecordStruct, sVcardInfo *pVcardStruct );
uint16_t NDEF_WriteVcard ( sVcardInfo *pVcardStruct );
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
   
#endif /* __LIB_NDEF_VCARD_H */


