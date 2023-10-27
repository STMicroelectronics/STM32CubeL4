/**
  ******************************************************************************
  * @file    lib_NDEF_Text.h
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file help to manage Text NDEF file.
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
#ifndef __LIB_NDEF_TEXT_H
#define __LIB_NDEF_TEXT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF.h"

/** @brief NDEF Text buffer length. */
#define NDEF_TEXT_MAX_LENGTH 40  
/** @brief NDEF Language code buffer length. */
#define NDEF_TEXT_LANGUAGE_CODE_MAX_LENGTH 10  

/** @brief NDEF Text encoding possible values. */
typedef enum {
  NDEF_TEXT_UTF8  = 0,
  NDEF_TEXT_UTF16 = 1
} NDEF_Text_encoding_t;

/** @brief This structure is used to handle information from a NDEF Text record. */
#ifdef NDEF_DYN_ALLOC
  typedef struct {
    NDEF_Text_encoding_t  encoding;                                           /**< metadata: UTF-8 / UTF-16. */
    char                  language_code[NDEF_TEXT_LANGUAGE_CODE_MAX_LENGTH];  /**< metadata: Language code as specified by IANA. */
    char                  *text;                                              /**< The text itself. */
  } NDEF_Text_info_t;  
#else
  typedef struct {
    NDEF_Text_encoding_t  encoding;                                           /**< metadata: UTF-8 / UTF-16. */
    char                  language_code[NDEF_TEXT_LANGUAGE_CODE_MAX_LENGTH];  /**< metadata: Language code as specified by IANA. */
    char                  text[NDEF_TEXT_MAX_LENGTH];                         /**< The text itself. */
  } NDEF_Text_info_t;  
#endif

/** @brief This structure is used to parse the raw data from a text record and access its metadata. */
typedef struct {
  uint8_t  language_length:4; /**< Language code length. */
  uint8_t  rfu:3;             /**< reserved for futur usage */
  uint8_t  encoding:1;        /**< UTF-8 (0) or UTF-16 (1) encoding. */
  char     language[1];       /**< IANA language code. */
} NDEF_Text_metadata_t;  
 
uint16_t NDEF_WriteText( char *text );
uint16_t NDEF_ReadText( sRecordInfo_t *pRecordStruct, NDEF_Text_info_t *pText );

#ifdef __cplusplus
}
#endif

#endif /* __LIB_NDEF_TEXT_H */

