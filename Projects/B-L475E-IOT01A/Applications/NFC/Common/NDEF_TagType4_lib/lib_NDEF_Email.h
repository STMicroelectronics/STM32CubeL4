/**
  ******************************************************************************
  * @file    lib_NDEF_Email.h
  * @author  MMY Application Team
  * @brief   This file help to manage Email NDEF file.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MMY-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
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

/******************* (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
