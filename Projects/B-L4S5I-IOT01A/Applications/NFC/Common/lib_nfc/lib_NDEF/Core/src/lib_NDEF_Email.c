/**
  ******************************************************************************
  * @file    lib_NDEF_Email.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file help to manage NDEF file that represent Email.
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
#include "lib_NDEF_Email.h"
#include "stdlib.h"

/** @addtogroup NFC_libraries
  * @{
  * @brief  <b>This is the library used to manage the content of the TAG (data)
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
extern uint8_t NDEF_Buffer [];


/** @defgroup libEmail_Private_Functions
  * @{
  */


static void NDEF_FillEmailStruct( uint8_t* pPayload, uint32_t PayloadSize, sEmailInfo *pEmailStruct );
static void NDEF_ReadURI_Email( sRecordInfo_t *pRecordStruct, sEmailInfo *pEmailStruct );

/**
  * @brief  This function fill Email structure with information of NDEF message
  * @param  pPayload : pointer on the payload data of the NDEF message
  * @param  PayloadSize : number of data in the payload
  * @param  pEmailStruct : pointer on the structure to fill
  * @retval NONE 
  */
static void NDEF_FillEmailStruct( uint8_t* pPayload, uint32_t PayloadSize, sEmailInfo *pEmailStruct )
{
  uint8_t* pLastByteAdd, *pLook4Word, *pEndString;
  uint32_t SizeOfKeyWord = 0;

  pEndString = 0;

  /* First character force to NULL in case not matching found */
#ifdef NDEF_DYN_ALLOC
  pEmailStruct->EmailAdd = NULL;
  pEmailStruct->Subject = NULL;
  pEmailStruct->Message = NULL;
  pEmailStruct->Information = NULL;
#else
  *pEmailStruct->EmailAdd = 0;
  *pEmailStruct->Subject = 0;
  *pEmailStruct->Message = 0;
#endif
  
  /* Interesting information are stored before picture if any */
  /* Moreover picture is not used in this demonstration SW */
  pLastByteAdd = (uint8_t*)(pPayload + PayloadSize);

  /* first byte should be the "mailto:" well know URI type, skip it */
  pLook4Word = ++pPayload;

  /* Retrieve email add */
  if( pLook4Word != pLastByteAdd )
  {
    pEndString = pLook4Word;
    while( memcmp( pEndString, URI_FIRST_DATA_END, URI_FIRST_DATA_END_LENGTH ) && (pEndString < pLastByteAdd) )
    {
      pEndString++;
    }
    if( ( !memcmp( pEndString, URI_FIRST_DATA_END, URI_FIRST_DATA_END_LENGTH ) ) || (pEndString == pLastByteAdd) )
    {
#ifdef NDEF_DYN_ALLOC
      pEmailStruct->EmailAdd = malloc(pEndString-pLook4Word + 1);
      if(pEmailStruct->EmailAdd != NULL) 
#endif
      {
        memcpy( pEmailStruct->EmailAdd, pLook4Word, pEndString-pLook4Word);
        /* add end of string character */
        pEmailStruct->EmailAdd[pEndString-pLook4Word] = 0;
      }
    }
  }

  pEndString += URI_FIRST_DATA_END_LENGTH;
  pLook4Word = pEndString;

  /* check if e-mail subject is present */
  if( !memcmp( pLook4Word, SUBJECT_BEGIN_STRING, SUBJECT_BEGIN_STRING_LENGTH ) )
  {
    SizeOfKeyWord = SUBJECT_BEGIN_STRING_LENGTH;

    /* Retrieve subject */
    if( pLook4Word != pLastByteAdd )
    {
      pLook4Word += SizeOfKeyWord;
      pEndString = pLook4Word;
      while( memcmp( pEndString, URI_SECOND_DATA_END, URI_SECOND_DATA_END_LENGTH ) && (pEndString < pLastByteAdd) )
      {
        pEndString++;
      }
      if( ( !memcmp( pEndString, URI_SECOND_DATA_END, URI_SECOND_DATA_END_LENGTH ) ) || (pEndString == pLastByteAdd) )
      {
#ifdef NDEF_DYN_ALLOC
       pEmailStruct->Subject = malloc(pEndString-pLook4Word + 1);
       if(pEmailStruct->Subject != NULL) 
#endif
       {
          memcpy( pEmailStruct->Subject, pLook4Word, pEndString-pLook4Word );
          /* add end of string character */
          pEmailStruct->Subject[pEndString-pLook4Word] = 0;	
       }
      }
      pEndString += URI_SECOND_DATA_END_LENGTH;
    }
  }

  pLook4Word = pEndString;

  /* check if e-mail message is present */
  if( !memcmp( pLook4Word, MESSAGE_BEGIN_STRING, MESSAGE_BEGIN_STRING_LENGTH ) )
  {
    pEndString += MESSAGE_BEGIN_STRING_LENGTH;
    /* Retrieve message */
#ifdef NDEF_DYN_ALLOC
      pEmailStruct->Message = malloc(PayloadSize - (pEndString - pPayload + 1) + 1);
      if(pEmailStruct->Message != NULL) 
#endif
      {
        memcpy( pEmailStruct->Message, pEndString, PayloadSize - (pEndString - pPayload + 1) );
        /* add end of string character */
        pEmailStruct->Message[PayloadSize-(pEndString-pPayload+1)] = 0;
      }
  }
}

/**
  * @brief  This function read the Email and store data in a structure.
  * @param  pRecordStruct : Pointer on the record structure.
  * @param  pEmailStruct : pointer on the structure to fill.
  */
static void NDEF_ReadURI_Email( sRecordInfo_t *pRecordStruct, sEmailInfo *pEmailStruct )
{
  uint8_t* pPayload;
  uint32_t PayloadSize;

  PayloadSize = pRecordStruct->PayloadLength;

  /* Read record header */
  pPayload = (uint8_t*)(pRecordStruct->PayloadBufferAdd);

  if( pRecordStruct->NDEF_Type == URI_EMAIL_TYPE )
    NDEF_FillEmailStruct( pPayload , PayloadSize, pEmailStruct );

}

/**
  * @}
  */

/** @defgroup libEmail_Public_Functions
  * @{
  * @brief  This file is used to manage Email (stored or loaded in tag)
  */ 

/**
  * @brief  This function read NDEF and retrieve Eamil information if any.
  * @param  pRecordStruct : Pointer on the record structure.
  * @param  pEmailStruct : pointer on the structure to fill .
  * @retval NDEF_OK : Email information from NDEF have been retrieved.
  * @retval NDEF_ERROR : not able to read NDEF in tag.
  */
uint16_t NDEF_ReadEmail( sRecordInfo_t *pRecordStruct, sEmailInfo *pEmailStruct )
{
  uint16_t status = NDEF_ERROR;
  sRecordInfo_t *pSPRecordStruct;
  uint32_t PayloadSize, RecordPosition;
  uint8_t* pData;


  if( pRecordStruct->NDEF_Type == URI_EMAIL_TYPE )
  {
    NDEF_ReadURI_Email( pRecordStruct, pEmailStruct );
    status = NDEF_OK;
  }
  else if( pRecordStruct->NDEF_Type == SMARTPOSTER_TYPE )
  {
    for( RecordPosition = 0; RecordPosition < pRecordStruct->NbOfRecordInSPPayload; RecordPosition++ )
    {
      pSPRecordStruct = pRecordStruct->SPRecordStructAdd[RecordPosition];
      if( pSPRecordStruct->NDEF_Type == URI_EMAIL_TYPE )
      {
        NDEF_ReadURI_Email( pSPRecordStruct, pEmailStruct );
        status = NDEF_OK;
      }
      if( pSPRecordStruct->NDEF_Type == TEXT_TYPE )
      {
        PayloadSize = pSPRecordStruct->PayloadLength;

        /* The instruction content the UTF-8 language code that is not used here */
        pData = (uint8_t*)pSPRecordStruct->PayloadBufferAdd;
        PayloadSize -= *pData + 1; /* remove not usefull data */
        pData += *pData + 1;

        memcpy( pEmailStruct->Information, pData, PayloadSize );
      }
    }
  }

  return status;
}

/**
  * @brief  This function write the NDEF file with the Email data given in the structure.
  * @param  pEmailStruct : pointer on structure that contain the Email information.
  * @retval NDEF_OK : the function is successful.
  * @retval NDEF_ERROR : not able to store NDEF in tag.
  * @retval NDEF_ERROR_MEMORY_INTERNAL : Cannot write to tag.
  * @retval NDEF_ERROR_NOT_FORMATED : CCFile data not supported or not present.
  * @retval NDEF_ERROR_MEMORY_TAG : Size not compatible with memory.
  * @retval NDEF_ERROR_LOCKED : Tag locked, cannot be write.
  */
uint16_t NDEF_WriteEmail( sEmailInfo *pEmailStruct )
{
  uint16_t status = NDEF_ERROR, Offset = 0;

  NDEF_PrepareEmailMessage( pEmailStruct, NDEF_Buffer, &Offset );

  status = NfcTag_WriteNDEF( Offset, NDEF_Buffer );

  return status;
}

/**
  * @brief  This function write the NDEF file with the Email data given in the structure.
  * @param  pEmailStruct : pointer on structure that contain the Email information.
  * @param  pNDEFMessage : pointer on the NDEF message.
  * @param  size : to store the size of the NDEF message generated.
  */
void NDEF_PrepareEmailMessage( sEmailInfo *pEmailStruct, uint8_t *pNDEFMessage, uint16_t *size )
{
  uint16_t Offset = 0;
  uint32_t emailSize = 0;
  uint32_t infoSize = 0;
  uint32_t totalSize = 0;

  /* Email is an URI but can be included in a smart poster to add text to give instruction to user for instance */

  /* Email (smart poster) Record Header */
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
/*           ID LENGTH              */  /* <---- Not Used  */
/*----------------------------------*/
/*             TYPE                 */
/*----------------------------------*/
/*              ID                  */  /* <---- Not Used  */ 
/************************************/

  /* Email : 1+@+1+subject+1+message */
  emailSize = 1 + strlen( pEmailStruct->EmailAdd ) + URI_FIRST_DATA_END_LENGTH + SUBJECT_BEGIN_STRING_LENGTH +
              strlen( pEmailStruct->Subject ) + URI_SECOND_DATA_END_LENGTH + MESSAGE_BEGIN_STRING_LENGTH + strlen( pEmailStruct->Message );

  /* Check if a Smart poster is needed */
  if( pEmailStruct->Information[0] != '\0' )
  {
    /* Info : 1+2+info */
    infoSize = 1 + ISO_ENGLISH_CODE_STRING_LENGTH + strlen( pEmailStruct->Information );
    /* Total */
    totalSize = 4 + emailSize + 4 + infoSize;
    if( emailSize > 255 ) totalSize += 3; /* Normal Email size */
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

  /* Email header */
  pNDEFMessage[Offset] = 0x81;
  if( emailSize < 256 ) pNDEFMessage[Offset] |= 0x10;                      // Set the SR bit
  if( pEmailStruct->Information[0] == '\0' ) pNDEFMessage[Offset] |= 0x40; // Set the ME bit
  Offset++;

  pNDEFMessage[Offset++] = URI_TYPE_STRING_LENGTH;
  if( emailSize > 255 )
  {
    pNDEFMessage[Offset++] = (emailSize & 0xFF000000) >> 24;
    pNDEFMessage[Offset++] = (emailSize & 0x00FF0000) >> 16;
    pNDEFMessage[Offset++] = (emailSize & 0x0000FF00) >> 8;
    pNDEFMessage[Offset++] = emailSize & 0x000000FF;
  }
  else
  {
    pNDEFMessage[Offset++] = (uint8_t)emailSize;
  }
  memcpy( &pNDEFMessage[Offset], URI_TYPE_STRING, URI_TYPE_STRING_LENGTH );
  Offset += URI_TYPE_STRING_LENGTH;

  /* Email pay load */
  pNDEFMessage[Offset++] = URI_ID_0x06;
  memcpy( &pNDEFMessage[Offset], pEmailStruct->EmailAdd, strlen(pEmailStruct->EmailAdd) );
  Offset += strlen( pEmailStruct->EmailAdd );
  memcpy( &pNDEFMessage[Offset], URI_FIRST_DATA_END, URI_FIRST_DATA_END_LENGTH );
  Offset += URI_FIRST_DATA_END_LENGTH;

  memcpy( &pNDEFMessage[Offset], SUBJECT_BEGIN_STRING, SUBJECT_BEGIN_STRING_LENGTH );
  Offset += SUBJECT_BEGIN_STRING_LENGTH;
  memcpy( &pNDEFMessage[Offset], pEmailStruct->Subject, strlen(pEmailStruct->Subject) );
  Offset += strlen( pEmailStruct->Subject );
  memcpy( &pNDEFMessage[Offset], URI_SECOND_DATA_END, URI_SECOND_DATA_END_LENGTH );
  Offset += URI_SECOND_DATA_END_LENGTH;

  memcpy( &pNDEFMessage[Offset], MESSAGE_BEGIN_STRING, MESSAGE_BEGIN_STRING_LENGTH );
  Offset += MESSAGE_BEGIN_STRING_LENGTH;
  memcpy( &pNDEFMessage[Offset], pEmailStruct->Message, strlen(pEmailStruct->Message) );
  Offset += strlen( pEmailStruct->Message );

  /* Information header */
  if( pEmailStruct->Information[0] != '\0' )
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
    memcpy( &pNDEFMessage[Offset], pEmailStruct->Information, strlen(pEmailStruct->Information) );
    Offset += strlen( pEmailStruct->Information );

  }

  *size = (uint16_t)(Offset);

}

/**
  * @brief  This function close a NDEF Email buffer freeing associated memory.
  * @param  pEmailStruct : pointer on structure that contain the Email information.
  */
void NDEF_closeEmail( sEmailInfo *pEmailStruct)
{
#ifdef NDEF_DYN_ALLOC
  if(pEmailStruct->EmailAdd != NULL) free(pEmailStruct->EmailAdd);
  if(pEmailStruct->Information != NULL) free(pEmailStruct->Information);
  if(pEmailStruct->Message != NULL) free(pEmailStruct->Message);
  if(pEmailStruct->Subject != NULL) free(pEmailStruct->Subject);
#endif
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
