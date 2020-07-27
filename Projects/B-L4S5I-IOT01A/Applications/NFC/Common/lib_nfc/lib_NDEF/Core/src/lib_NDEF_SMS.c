/**
  ******************************************************************************
  * @file    lib_NDEF_SMS.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file help to manage NDEF file that represent SMS.
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
#include "lib_NDEF_SMS.h"


/** @addtogroup NFC_libraries
  * @{
  * @brief  <b>This is the library used to manage the content of the TAG (data)
  *          But also the specific feature of the tag, for instance
  *          password, gpo... </b>
  */


/** @addtogroup libNFC_FORUM
  * @{
  *	@brief  This part of the library manage data which follow NFC forum organisation.
  */

/**
 * @brief  This buffer contains the data send/received by TAG
 */
extern uint8_t NDEF_Buffer [];

/** @defgroup libSMS_Private_Functions
  * @{
  */

static void NDEF_FillSMSStruct( uint8_t* pPayload, uint32_t PayloadSize, sSMSInfo *pSMSStruct );
static void NDEF_ReadURI_SMS( sRecordInfo_t *pRecordStruct, sSMSInfo *pSMSStruct );

/**
  * @brief  This function fill SMS structure with information of NDEF message.
  * @param  pPayload : pointer on the payload data of the NDEF message.
  * @param  PayloadSize : number of data in the payload.
  * @param  pSMSStruct : pointer on the structure to fill.
  */
static void NDEF_FillSMSStruct( uint8_t* pPayload, uint32_t PayloadSize, sSMSInfo *pSMSStruct )
{
  uint8_t* pLastByteAdd, *pLook4Word, *pEndString;
  char* pKeyWord;
  uint32_t SizeOfKeyWord;
  pEndString = 0;

  pKeyWord = SMS_TYPE_STRING;
  SizeOfKeyWord = SMS_TYPE_STRING_LENGTH;

  /* First character force to NULL in case not matching found */
  *pSMSStruct->PhoneNumber = 0;
  *pSMSStruct->Message = 0;

  /* Interesting information are stored before picture if any */
  /* Moreover picture is not used in this demonstration SW */
  pLastByteAdd = (uint8_t*)(pPayload + PayloadSize);

  pLook4Word = pPayload;
  while( memcmp( pLook4Word, pKeyWord, SizeOfKeyWord ) && (pLook4Word < pLastByteAdd) )
  {
    pLook4Word++;
  }

  /* Retrieve phone number */
  if( pLook4Word != pLastByteAdd )
  {
    pLook4Word += SizeOfKeyWord;
    pEndString = pLook4Word;
    while( memcmp( pEndString, URI_FIRST_DATA_END, URI_FIRST_DATA_END_LENGTH ) && (pEndString < pLastByteAdd) )
    {
      pEndString++;
    }
    if( pEndString != pLastByteAdd )
    {
      memcpy( pSMSStruct->PhoneNumber, pLook4Word, pEndString-pLook4Word );
      /* add end of string character */
      pSMSStruct->PhoneNumber[pEndString-pLook4Word] = 0;
    }
  }
  pEndString += URI_FIRST_DATA_END_LENGTH;
  pLook4Word = pEndString;

  /* check if e-mail subject is present */
  if( !memcmp( pLook4Word, MESSAGE_BEGIN_STRING, MESSAGE_BEGIN_STRING_LENGTH ) )
  {
    pEndString += MESSAGE_BEGIN_STRING_LENGTH;
    /* Retrieve message */
    memcpy( pSMSStruct->Message, pEndString, PayloadSize - (pEndString - pPayload) );
    /* add end of string character */
    pSMSStruct->Message[PayloadSize-(pEndString-pPayload)] = 0;
  }
}

/**
  * @brief  This function read the SMS and store data in a structure.
  * @param  pRecordStruct : Pointer on the record structure.
  * @param  pSMSStruct : pointer on the structure to fill.
  */
static void NDEF_ReadURI_SMS( sRecordInfo_t *pRecordStruct, sSMSInfo *pSMSStruct )
{
  uint8_t* pPayload;
  uint32_t PayloadSize;

  PayloadSize = pRecordStruct->PayloadLength;

  /* Read record header */
  pPayload = (uint8_t*)(pRecordStruct->PayloadBufferAdd);

  if( pRecordStruct->NDEF_Type == URI_SMS_TYPE )
    NDEF_FillSMSStruct( pPayload , PayloadSize, pSMSStruct );

}

/**
  * @}
  */

/** @defgroup libSMS_Public_Functions
  * @{
  * @brief  This file is used to manage SMS (stored or loaded in tag)
  */ 

/**
  * @brief  This function read NDEF and retrieve SMS information if any.
  * @param  pRecordStruct : Pointer on the record structure.
  * @param  pSMSStruct : pointer on the structure to fill.
  * @retval NDEF_OK : SMS information from NDEF have been retrieve.
  * @retval NDEF_ERROR : Not able to retrieve SMS information.
  */
uint16_t NDEF_ReadSMS( sRecordInfo_t *pRecordStruct, sSMSInfo *pSMSStruct )
{
  uint16_t status = NDEF_ERROR;
  sRecordInfo_t *pSPRecordStruct;
  uint32_t PayloadSize, RecordPosition;
  uint8_t* pData;

  if( pRecordStruct->NDEF_Type == URI_SMS_TYPE )
  {
    NDEF_ReadURI_SMS( pRecordStruct, pSMSStruct );
    status = NDEF_OK;
  }
  else if( pRecordStruct->NDEF_Type == SMARTPOSTER_TYPE )
  {
    for( RecordPosition = 0; RecordPosition < pRecordStruct->NbOfRecordInSPPayload; RecordPosition++ )
    {
      pSPRecordStruct = pRecordStruct->SPRecordStructAdd[RecordPosition];
      if( pSPRecordStruct->NDEF_Type == URI_SMS_TYPE )
      {
        NDEF_ReadURI_SMS( pSPRecordStruct, pSMSStruct );
        status = NDEF_OK;
      }
      if( pSPRecordStruct->NDEF_Type == TEXT_TYPE )
      {
        PayloadSize = pSPRecordStruct->PayloadLength;

        /* The instruction content the UTF-8 language code that is not used here */
        pData = (uint8_t*)pSPRecordStruct->PayloadBufferAdd;
        PayloadSize -= *pData + 1; /* remove not usefull data */
        pData += *pData + 1; /* set pointer on usefull data */

        memcpy( pSMSStruct->Information, pData, PayloadSize );
        /* add end of string character */
        pSMSStruct->Information[PayloadSize] = 0;
      }
    }
  }

  return status;
}

/**
  * @brief  This function write the NDEF file with the SMS data given in the structure.
  * @param  pSMSStruct : pointer on structure that contain the SMS information.
  * @retval NDEF_OK : NDEF file data written in the tag.
  * @retval NDEF_ERROR : not able to store NDEF in tag.
  * @retval NDEF_ERROR_MEMORY_INTERNAL : Cannot write to tag.
  * @retval NDEF_ERROR_NOT_FORMATED : CCFile data not supported or not present.
  * @retval NDEF_ERROR_MEMORY_TAG : Size not compatible with memory.
  * @retval NDEF_ERROR_LOCKED : Tag locked, cannot be write.
  */
uint16_t NDEF_WriteSMS( sSMSInfo *pSMSStruct )
{
  uint16_t status = NDEF_ERROR, Offset = 0;

  NDEF_PrepareSMSMessage( pSMSStruct, NDEF_Buffer, &Offset );

  status = NfcTag_WriteNDEF( Offset , NDEF_Buffer );

  return status;
}

/**
  * @brief  This function write the NDEF file with the SMS data given in the structure.
  * @param  pSMSStruct : pointer on structure that contain the SMS information.
  * @param  pNDEFMessage : pointer on the NDEF message.
  * @param  size : to store the size of the NDEF message generated.
  */
void NDEF_PrepareSMSMessage( sSMSInfo *pSMSStruct, uint8_t *pNDEFMessage, uint16_t *size )
{
  uint16_t Offset = 0;
  uint32_t smsSize = 0;
  uint32_t infoSize= 0;
  uint32_t totalSize = 0;

  /* SMS is an URI but can be included in a smart poster to add text to give instruction to user for instance */

  /* SMS (smart poster) Record Header */
/************************************/
/*  7 |  6 |  5 |  4 |  3 | 2  1  0 */
/*----------------------------------*/
/* MB   ME   CF   SR   IL    TNF    */  /* <---- CF=0, IL=0 and SR=1 TNF=1 NFC Forum Well-known type*/
/*----------------------------------*/
/*          TYPE LENGTH             */
/*----------------------------------*/
/*        PAYLOAD LENGTH 3          */  /* <---- Used only if SR=0 */
/*----------------------------------*/
/*        PAYLOAD LENGTH 2          */  /* <---- Used only if SR=0 */
/*----------------------------------*/
/*        PAYLOAD LENGTH 1          */  /* <---- Used only if SR=0 */
/*----------------------------------*/
/*        PAYLOAD LENGTH 0          */
/*----------------------------------*/
/*          ID LENGTH               */  /* <---- Not Used  */
/*----------------------------------*/
/*              TYPE                */
/*----------------------------------*/
/*               ID                 */  /* <---- Not Used  */
/************************************/

  /* SMS : 1+sms:+tel+1+body=+message */
  smsSize = 1 + SMS_TYPE_STRING_LENGTH + strlen(pSMSStruct->PhoneNumber) + URI_FIRST_DATA_END_LENGTH + 
            MESSAGE_BEGIN_STRING_LENGTH + strlen(pSMSStruct->Message);

  /* Check if a Smart poster is needed */
  if( pSMSStruct->Information[0] != '\0' )
  {
    /* Info : 1+2+info */
    infoSize = 1 + ISO_ENGLISH_CODE_STRING_LENGTH + strlen(pSMSStruct->Information);
    /* Total */
    totalSize = 4 + smsSize + 4 + infoSize;
    if( smsSize > 255 ) totalSize += 3; /* Normal Email size */
    if( infoSize > 255 ) totalSize += 3;  /* Normal Info size */

    /* SmartPoster header */
    if( totalSize > 255 )
    {
      pNDEFMessage[Offset++] = 0xC1;
      pNDEFMessage[Offset++] = SMART_POSTER_TYPE_STRING_LENGTH;
      pNDEFMessage[Offset++] = (totalSize & 0xFF000000) >> 24;
      pNDEFMessage[Offset++] = (totalSize & 0x00FF0000) >> 16;
      pNDEFMessage[Offset++] = (totalSize & 0x0000FF00) >> 8;
      pNDEFMessage[Offset++] = totalSize & 0x000000FF;
    }
    else
    {
      pNDEFMessage[Offset++] = 0xD1;
      pNDEFMessage[Offset++] = SMART_POSTER_TYPE_STRING_LENGTH;
      pNDEFMessage[Offset++] = (uint8_t)totalSize;
    }
    memcpy( &pNDEFMessage[Offset], SMART_POSTER_TYPE_STRING, SMART_POSTER_TYPE_STRING_LENGTH );
    Offset += SMART_POSTER_TYPE_STRING_LENGTH;
  }

  /* SMS header */
  pNDEFMessage[Offset] = 0x81;
  if( smsSize < 256 ) pNDEFMessage[Offset] |= 0x10;                      // Set the SR bit
  if( pSMSStruct->Information[0] == '\0' ) pNDEFMessage[Offset] |= 0x40; // Set the ME bit
  Offset++;

  pNDEFMessage[Offset++] = URI_TYPE_STRING_LENGTH;
  if( smsSize > 255 )
  {
    pNDEFMessage[Offset++] = (smsSize & 0xFF000000) >> 24;
    pNDEFMessage[Offset++] = (smsSize & 0x00FF0000) >> 16;
    pNDEFMessage[Offset++] = (smsSize & 0x0000FF00) >> 8;
    pNDEFMessage[Offset++] = smsSize & 0x000000FF;
  }
  else
  {
    pNDEFMessage[Offset++] = (uint8_t)smsSize;
  }
  memcpy( &pNDEFMessage[Offset], URI_TYPE_STRING, URI_TYPE_STRING_LENGTH );
  Offset += URI_TYPE_STRING_LENGTH;

  /* SMS payload */
  pNDEFMessage[Offset++] = URI_ID_0x00;
  memcpy( &pNDEFMessage[Offset], SMS_TYPE_STRING, SMS_TYPE_STRING_LENGTH );
  Offset += SMS_TYPE_STRING_LENGTH;
  memcpy( &pNDEFMessage[Offset], pSMSStruct->PhoneNumber, strlen(pSMSStruct->PhoneNumber) );
  Offset += strlen( pSMSStruct->PhoneNumber );
  memcpy( &pNDEFMessage[Offset], URI_FIRST_DATA_END, URI_FIRST_DATA_END_LENGTH );
  Offset += URI_FIRST_DATA_END_LENGTH;

  memcpy( &pNDEFMessage[Offset], MESSAGE_BEGIN_STRING, MESSAGE_BEGIN_STRING_LENGTH );
  Offset += MESSAGE_BEGIN_STRING_LENGTH;
  memcpy( &pNDEFMessage[Offset], pSMSStruct->Message, strlen(pSMSStruct->Message) );
  Offset += strlen( pSMSStruct->Message );

  /* Information header */
  if( pSMSStruct->Information[0] != '\0' )
  {
    if( infoSize > 255 )
    {
      pNDEFMessage[Offset++] = 0x41;
      pNDEFMessage[Offset++] = TEXT_TYPE_STRING_LENGTH;
      pNDEFMessage[Offset++] = (infoSize & 0xFF000000) >> 24;
      pNDEFMessage[Offset++] = (infoSize & 0x00FF0000) >> 16;
      pNDEFMessage[Offset++] = (infoSize & 0x0000FF00) >> 8;
      pNDEFMessage[Offset++] = infoSize & 0x000000FF;
    }
    else
    {
      pNDEFMessage[Offset++] = 0x51;
      pNDEFMessage[Offset++] = TEXT_TYPE_STRING_LENGTH;
      pNDEFMessage[Offset++] = (uint8_t)infoSize;
    }

    memcpy( &pNDEFMessage[Offset], TEXT_TYPE_STRING, TEXT_TYPE_STRING_LENGTH );
    Offset += TEXT_TYPE_STRING_LENGTH;
    pNDEFMessage[Offset++] = ISO_ENGLISH_CODE_STRING_LENGTH; /* UTF-8 with x byte language code */
    memcpy( &pNDEFMessage[Offset], ISO_ENGLISH_CODE_STRING, ISO_ENGLISH_CODE_STRING_LENGTH );
    Offset += ISO_ENGLISH_CODE_STRING_LENGTH;

    /* Information payload */
    memcpy( &pNDEFMessage[Offset], pSMSStruct->Information, strlen(pSMSStruct->Information) );
    Offset += strlen( pSMSStruct->Information );
  }

  *size = (uint16_t)(Offset);
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
