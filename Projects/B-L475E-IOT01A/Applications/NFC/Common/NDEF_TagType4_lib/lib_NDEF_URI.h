/**
  ******************************************************************************
  * @file    lib_NDEF_URI.h
  * @author  MMY Application Team
  * @brief   This file help to manage URI NDEF file.
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

/******************* (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
