/**
  ******************************************************************************
  * @file    lib_NDEF_MyApp.h
  * @author  MMY Application Team
  * @brief   This file illustrate how to use M24SR with a proprietary protocol.
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
#ifndef __LIB_NDEF_MYAPP_H
#define __LIB_NDEF_MYAPP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF.h"

/** @addtogroup NFC_libraries
  * @{
  */

/** @addtogroup TAGTYPE4_NDEF_MYAPP
  * @{
  */

/**
 * @defgroup TAGTYPE4_NDEF_MYAPP_Exported_Constants             TAGTYPE4_NDEF_MYAPP Exported Constants
 * @{
 */
#define BLINK_CONFIG                    "<BLINK_CONFIG>"
#define BLINK_CONFIG_STRING_SIZE        14  
#define BLINK_CONFIG_END                "<\\BLINK_CONFIG>"
#define BLINK_CONFIG_END_STRING_SIZE    15
   
#define LINE_DESCR                      "<LINE_DESCR>"
#define LINE_DESCR_STRING_SIZE          12
#define LINE_DESCR_END                  "<\\LINE_DESCR>"
#define LINE_DESCR_END_STRING_SIZE      13
   
#define BLINKING_NONE                    0x00
#define BLINKING_SLOW                    0x01
#define BLINKING_MEDIUM                  0x02
#define BLINKING_FAST                    0x03
/**
  * @}
  */
  
/**
 * @defgroup TAGTYPE4_NDEF_MYAPP_Exported_Types             TAGTYPE4_NDEF_MYAPP Exported Types
 * @{
 */
typedef struct 
{
  uint8_t LedConf[4];
  uint8_t Speed;
}sLedBlinkConfig;

typedef struct 
{
  uint8_t LineNb;
  uint16_t BackGroundColor;
  uint16_t FontColor;
  char String[20];
}sLineConfig;

typedef struct 
{
  sLedBlinkConfig LedBlinkConf;
  sLineConfig LineX[8];
}sMyAppInfo;
/**
  * @}
  */
    
/**
 * @defgroup TAGTYPE4_NDEF_MYAPP_Exported_FunctionPrototypes             TAGTYPE4_NDEF_MYAPP Exported FunctionPrototypes
 * @{
 */
      
uint16_t NDEF_ReadMyApp(sRecordInfo *pRecordStruct, sMyAppInfo *pMyAppStruct);
uint16_t NDEF_WriteMyApp(sMyAppInfo *pMyAppStruct);
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
   
#endif /* __LIB_NDEF_MYAPP_H */

