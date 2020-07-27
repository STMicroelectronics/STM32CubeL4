/**
  ******************************************************************************
  * @file    lib_NDEF_AAR.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file help to add AAR to NDEF file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under ST MYLIBERTY SOFTWARE LICENSE AGREEMENT (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/myliberty  
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF_AAR.h"

/** @addtogroup NFC_libraries
 *  @{
 *  @brief  <b>This is the library used to manage the content of the TAG (data)
 *          But also the specific feature of the tag, for instance
 *          password, gpo... </b>
 */


/** @addtogroup libNFC_FORUM
  * @{
  * @brief  This part of the library manage data which follow NFC forum organisation.
  */

/**
 * @brief  This buffer contains the data send/received by TAG
 */
/* extern uint8_t NDEF_Buffer []; */

/** @defgroup libAAR_Private_Functions
  * @{
  */

/**
  * @}
  */

/** @defgroup libAAR_Public_Functions
  * @{
  * @brief  This file is used to manage AAR (stored or loaded in tag)
  */ 


/**
  * @brief  This function add AAR (Android Application Record) in the tag.
  * @param  pAARStruct : pointer on structure that contain AAR information.
  * @retval NDEF_OK : AAR added.
  * @retval NDEF_ERROR : not able to store NDEF in tag.
  * @retval NDEF_ERROR_MEMORY_INTERNAL : Cannot write to tag.
  * @retval NDEF_ERROR_NOT_FORMATED : CCFile data not supported or not present.
  * @retval NDEF_ERROR_MEMORY_TAG : Size not compatible with memory.
  * @retval NDEF_ERROR_LOCKED : Tag locked, cannot be write.
  */
uint16_t NDEF_AddAAR( const sAARInfo *pAARStruct )
{
  uint16_t status = NDEF_ERROR;

/* AAR: External Type Record Header */
/************************************/
/*  7 |  6 |  5 |  4 |  3 | 2  1  0 */
/*----------------------------------*/
/* MB   ME   CF   SR   IL    TNF    */  /* <---- IL=0, CF=0 and SR=1 TNF=4 NFC Forum external type*/
/*----------------------------------*/
/*          TYPE LENGTH             */
/*----------------------------------*/
/*        PAYLOAD LENGTH 3          */  /* <---- Not Used  */
/*----------------------------------*/
/*        PAYLOAD LENGTH 2          */  /* <---- Not Used  */
/*----------------------------------*/
/*        PAYLOAD LENGTH 1          */  /* <---- Not Used  */
/*----------------------------------*/
/*        PAYLOAD LENGTH 0          */
/*----------------------------------*/
/*           ID LENGTH              */  /* <---- Not Used  */
/*----------------------------------*/
/*             TYPE                 */  /* android.com:pkg */
/*----------------------------------*/
/*              ID                  */  /* <---- Not Used  */
/************************************/

  /* Do we have to add AAR to an existing NDEF message */
  /* retrieve current NDEF size and current record flag*/
  sRecordInfo_t AARrecord;

  /* fill AAR record header */
  AARrecord.RecordFlags = SR_Mask | TNF_NFCForumExternal;
  AARrecord.TypeLength = AAR_TYPE_STRING_LENGTH;

  memcpy( AARrecord.Type, AAR_TYPE_STRING, AAR_TYPE_STRING_LENGTH );

  /* fill AAR payload */
  AARrecord.PayloadBufferAdd = (uint8_t*)pAARStruct->PackageName;
  AARrecord.PayloadLength= strlen(pAARStruct->PackageName) ;

  status = NDEF_AppendRecord(&AARrecord);

   return status;
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
