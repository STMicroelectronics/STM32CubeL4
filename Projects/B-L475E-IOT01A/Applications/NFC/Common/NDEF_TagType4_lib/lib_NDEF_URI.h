/**
  ******************************************************************************
  * @file    lib_NDEF_URI.h
  * @author  MMY Application Team
  * @brief   This file help to manage URI NDEF file.
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
#ifndef __LIB_NDEF_URI_H
#define __LIB_NDEF_URI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF.h"

/** @addtogroup NFC_libraries
  * @{
  */


/** @addtogroup TAGTYPE4_NDEF_URI
  * @{
  */

/**
 * @defgroup TAGTYPE4_NDEF_URI_Exported_Types             TAGTYPE4_NDEF_URI Exported Types
 * @{
 */

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  URI structure, to store Protocol, Message and optional information
  */      
typedef struct 
{
  char protocol[80];
  char URI_Message[200];
  char Information[200];
}sURI_Info;
/**
  * @}
  */
    
/**
 * @defgroup TAGTYPE4_NDEF_URI_Exported_FunctionPrototypes             TAGTYPE4_NDEF_URI Exported FunctionPrototypes
 * @{
 */
uint16_t NDEF_ReadURI(sRecordInfo *pRecordStruct, sURI_Info *pURI);
uint16_t NDEF_WriteURI(sURI_Info *pURI);
char getUriType(char *protocol);
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
   
#endif /* __LIB_NDEF_URI_H */

