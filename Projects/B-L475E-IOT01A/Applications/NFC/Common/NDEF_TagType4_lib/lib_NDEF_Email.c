/**
  ******************************************************************************
  * @file    lib_NDEF_Email.c
  * @author  MMY Application Team
  * @brief   This file help to manage NDEF file that represent Email.
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

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF_private.h"
#include "lib_NDEF_Email.h"

/** @addtogroup NFC_libraries
 *  @{
 */

/** @addtogroup TAGTYPE4_NDEF_EMAIL
  * @{
  */

/**
 * @brief  This buffer contains the data send/received by TAG
 */
extern uint8_t NDEF_Buffer [NFC_TT4_NDEF_MAX_SIZE];

/** @defgroup TAGTYPE4_NDEF_EMAIL_Private_FunctionsPrototypes
  * @{
  */
static void NDEF_FillEmailStruct( uint8_t* pPayload, uint32_t PayloadSize, sEmailInfo *pEmailStruct);
static void NDEF_ReadURI_Email ( sRecordInfo *pRecordStruct, sEmailInfo *pEmailStruct );
/**
  * @}
  */ 
  
/** @defgroup TAGTYPE4_NDEF_EMAIL_Private_Functions
  * @{
  */
  
/**
  * @brief  This fonction fill Email structure with information of NDEF message
  * @param  pPayload : pointer on the payload data of the NDEF message
  * @param  PayloadSize : number of data in the payload
  * @param  pEmailStruct : pointer on the structure to fill
  * @retval NONE 
  */
static void NDEF_FillEmailStruct( uint8_t* pPayload, uint32_t PayloadSize, sEmailInfo *pEmailStruct)
{
  uint8_t* pLastByteAdd, *pLook4Word, *pEndString ;
  uint32_t SizeOfKeyWord;
  
  /* First charactere force to NULL in case not matching found */
  *pEmailStruct->EmailAdd = 0;
  *pEmailStruct->Subject = 0;
  *pEmailStruct->Message = 0;
  
  /* Interresting information are stored before picture if any */
  /* Moreover picture is not used in this demonstration SW */  
  pLastByteAdd = (uint8_t*)(pPayload + PayloadSize);
  
  /* first byte should be the "mailto:" well know URI type, skip it */
  pLook4Word = ++pPayload;
  
  pEndString = pLook4Word;
  
  /* Retrieve email add */
  if( pLook4Word != pLastByteAdd)
  {  
    while( memcmp( pEndString, URI_FIRST_DATA_END, URI_FIRST_DATA_END_LENGTH) && pEndString<pLastByteAdd )
    {
      pEndString++;
    }
    if( pEndString != pLastByteAdd)
    {
      memcpy( pEmailStruct->EmailAdd, pLook4Word, pEndString-pLook4Word);
      /* add end of string charactere */
      pEmailStruct->EmailAdd[pEndString-pLook4Word] = 0;  
    }
  }  
  
  pEndString += URI_FIRST_DATA_END_LENGTH;
  pLook4Word = pEndString;
  
  /* check if e-mail subject is present */
  if(!memcmp( pLook4Word, SUBJECT_BEGIN_STRING, SUBJECT_BEGIN_STRING_LENGTH))
  {    
    SizeOfKeyWord = SUBJECT_BEGIN_STRING_LENGTH;
    
    /* Retrieve subject */
    if( pLook4Word != pLastByteAdd)
    {
      pLook4Word += SizeOfKeyWord;
      pEndString = pLook4Word;
      while( memcmp( pEndString, URI_SECOND_DATA_END, URI_SECOND_DATA_END_LENGTH) && pEndString<pLastByteAdd )
      {
        pEndString++;
      }
      if( pEndString != pLastByteAdd)
      {
        memcpy( pEmailStruct->Subject, pLook4Word, pEndString-pLook4Word);
        /* add end of string charactere */
        pEmailStruct->Subject[pEndString-pLook4Word] = 0;  
      }
      pEndString += URI_SECOND_DATA_END_LENGTH;
    }    
  }
  
  pLook4Word = pEndString;  
  
  /* check if e-mail message is present */
  if(!memcmp( pLook4Word, MESSAGE_BEGIN_STRING, MESSAGE_BEGIN_STRING_LENGTH))
  {
    pEndString += MESSAGE_BEGIN_STRING_LENGTH;
    /* Retrieve message */
    memcpy( pEmailStruct->Message, pEndString, PayloadSize-(pEndString-pPayload+1));
    /* add end of string charactere */
    pEmailStruct->Message[PayloadSize-(pEndString-pPayload+1)] = 0;    
  }
}

/**
  * @brief  This fonction read the Email and store data in a structure
  * @param  pRecordStruct : Pointer on the record structure
  * @param  pEmailStruct : pointer on the structure to fill
  * @retval NONE 
  */
static void NDEF_ReadURI_Email ( sRecordInfo *pRecordStruct, sEmailInfo *pEmailStruct )
{
  uint8_t* pPayload;
  uint32_t PayloadSize;
  
  PayloadSize = ((uint32_t)(pRecordStruct->PayloadLength3)<<24) | ((uint32_t)(pRecordStruct->PayloadLength2)<<16) |
    ((uint32_t)(pRecordStruct->PayloadLength1)<<8)  | pRecordStruct->PayloadLength0;
  
  /* Read record header */
  pPayload = (uint8_t*)(pRecordStruct->PayloadBufferAdd);
  
  if( pRecordStruct->NDEF_Type == URI_EMAIL_TYPE)
    NDEF_FillEmailStruct(pPayload , PayloadSize, pEmailStruct);
  
}

/**
  * @}
  */

/** @defgroup TAGTYPE4_NDEF_EMAIL_Public_Functions
  * @{
  *  @brief  This file is used to manage Email (stored or loaded in tag)
  */ 

/**
  * @brief  This fonction read NDEF and retrieve Eamil information if any
  * @param  pRecordStruct : Pointer on the record structure
  * @param  pEmailStruct : pointer on the structure to fill 
  * @retval SUCCESS : Email information from NDEF have been retrieved
  * @retval ERROR : Not able to retrieve Email information
  */
uint16_t NDEF_ReadEmail(sRecordInfo *pRecordStruct, sEmailInfo *pEmailStruct)
{
  uint16_t status = NDEF_ERROR;
  sRecordInfo *pSPRecordStruct;  
  uint32_t PayloadSize, RecordPosition;
  uint8_t* pData;
  
  
  if( pRecordStruct->NDEF_Type == URI_EMAIL_TYPE )
  {  
    NDEF_ReadURI_Email(pRecordStruct, pEmailStruct );
    status = NDEF_SUCCESS;
  }
  else if( pRecordStruct->NDEF_Type == SMARTPOSTER_TYPE)
  {
    for (RecordPosition = 0; RecordPosition<pRecordStruct->NbOfRecordInSPPayload; RecordPosition++)
    {
      pSPRecordStruct = (sRecordInfo *)(pRecordStruct->SPRecordStructAdd[RecordPosition]);
      if(pSPRecordStruct->NDEF_Type == URI_EMAIL_TYPE )
      {
        NDEF_ReadURI_Email(pSPRecordStruct, pEmailStruct );
        status = NDEF_SUCCESS;
      }
      if(pSPRecordStruct->NDEF_Type == TEXT_TYPE )
      {
        PayloadSize = ((uint32_t)(pSPRecordStruct->PayloadLength3)<<24) | ((uint32_t)(pSPRecordStruct->PayloadLength2)<<16) |
          ((uint32_t)(pSPRecordStruct->PayloadLength1)<<8)  | pSPRecordStruct->PayloadLength0;
        
        /* The instruction content the UTF-8 language code that is not used here */
        pData = (uint8_t*)pSPRecordStruct->PayloadBufferAdd;
        pData += *pData+1;
        
        memcpy(pEmailStruct->Information, pData, PayloadSize);
      }
    }
  }
  
  return status;
}

/**
  * @brief  This fonction write the NDEF file with the Email data given in the structure
  * @param  pEmailStruct : pointer on structure that contain the Email information
  * @retval SUCCESS : the function is succesful
  * @retval ERROR : Not able to store NDEF file inside tag.
  */
uint16_t NDEF_WriteEmail ( sEmailInfo *pEmailStruct )
{
  uint16_t status = NDEF_ERROR;
  uint32_t Offset = 0;
  uint32_t emailSize, infoSize, totalSize;
  
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
  /*          ID LENGTH               */  /* <---- Not Used  */
  /*----------------------------------*/
  /*              TYPE                */
  /*----------------------------------*/
  /*               ID                 */  /* <---- Not Used  */ 
  /************************************/
  /* Leave the size of the file empty */
  NDEF_Buffer[0] = 0;
  NDEF_Buffer[1] = 0;
  Offset = FIRST_RECORD_OFFSET;
  infoSize = 0;
  
  /* Email : 1+@+1+subject+1+message */
  emailSize = 1+strlen(pEmailStruct->EmailAdd)+URI_FIRST_DATA_END_LENGTH+SUBJECT_BEGIN_STRING_LENGTH+strlen(pEmailStruct->Subject)+URI_SECOND_DATA_END_LENGTH+MESSAGE_BEGIN_STRING_LENGTH+strlen(pEmailStruct->Message);
  
  /* Check if a Smart poster is needed */
  if (pEmailStruct->Information[0] != '\0')
  {
    /* Info : 1+2+info */
    infoSize = 1+ISO_ENGLISH_CODE_STRING_LENGTH+strlen(pEmailStruct->Information);
    /* Total */
    totalSize = 4+emailSize+4+infoSize;
    if (emailSize > 255) totalSize+=3; /* Normal Email size */
    if (infoSize > 255) totalSize+=3;  /* Normal Info size */
    
    /* SmartPoster header */
    if (totalSize > 255) 
    {
      NDEF_Buffer[Offset++] = 0xC1;
      NDEF_Buffer[Offset++] = SMART_POSTER_TYPE_STRING_LENGTH;
      NDEF_Buffer[Offset++] = (totalSize & 0xFF000000)>>24;
      NDEF_Buffer[Offset++] = (totalSize & 0x00FF0000)>>16;
      NDEF_Buffer[Offset++] = (totalSize & 0x0000FF00)>>8;
      NDEF_Buffer[Offset++] = (totalSize & 0x000000FF);
    }
    else
    {
      NDEF_Buffer[Offset++] = 0xD1;
      NDEF_Buffer[Offset++] = SMART_POSTER_TYPE_STRING_LENGTH;
      NDEF_Buffer[Offset++] = (uint8_t)totalSize;
    }
    memcpy(&NDEF_Buffer[Offset], SMART_POSTER_TYPE_STRING, SMART_POSTER_TYPE_STRING_LENGTH);
    Offset+=SMART_POSTER_TYPE_STRING_LENGTH;
  }
  
  /* Email header */
  NDEF_Buffer[Offset] = 0x81;
  if (emailSize < 256) NDEF_Buffer[Offset] |= 0x10;                      /* Set the SR bit */
  if (pEmailStruct->Information[0] == '\0') NDEF_Buffer[Offset] |= 0x40; /* Set the ME bit */
  Offset++;
  
  NDEF_Buffer[Offset++] = URI_TYPE_STRING_LENGTH;
  if (emailSize > 255)
  {
    NDEF_Buffer[Offset++] = (emailSize & 0xFF000000)>>24;
    NDEF_Buffer[Offset++] = (emailSize & 0x00FF0000)>>16;
    NDEF_Buffer[Offset++] = (emailSize & 0x0000FF00)>>8;
    NDEF_Buffer[Offset++] = (emailSize & 0x000000FF);
  }
  else
  {
    NDEF_Buffer[Offset++] = (uint8_t)emailSize;
  }
  memcpy(&NDEF_Buffer[Offset], URI_TYPE_STRING, URI_TYPE_STRING_LENGTH);
  Offset+=URI_TYPE_STRING_LENGTH;
  
  /* Email payload */
  NDEF_Buffer[Offset++] = URI_ID_0x06;
  memcpy( &NDEF_Buffer[Offset], pEmailStruct->EmailAdd,strlen(pEmailStruct->EmailAdd));
  Offset += strlen(pEmailStruct->EmailAdd);  
  memcpy( &NDEF_Buffer[Offset], URI_FIRST_DATA_END,URI_FIRST_DATA_END_LENGTH);
  Offset += URI_FIRST_DATA_END_LENGTH;
  
  memcpy(&NDEF_Buffer[Offset], SUBJECT_BEGIN_STRING, SUBJECT_BEGIN_STRING_LENGTH);
  Offset += SUBJECT_BEGIN_STRING_LENGTH;
  memcpy( &NDEF_Buffer[Offset], pEmailStruct->Subject,strlen(pEmailStruct->Subject));
  Offset += strlen(pEmailStruct->Subject);
  memcpy( &NDEF_Buffer[Offset], URI_SECOND_DATA_END,URI_SECOND_DATA_END_LENGTH);
  Offset += URI_SECOND_DATA_END_LENGTH;
  
  memcpy( &NDEF_Buffer[Offset], MESSAGE_BEGIN_STRING, MESSAGE_BEGIN_STRING_LENGTH);
  Offset += MESSAGE_BEGIN_STRING_LENGTH;
  memcpy( &NDEF_Buffer[Offset], pEmailStruct->Message, strlen(pEmailStruct->Message));
  Offset += strlen(pEmailStruct->Message);
  
  /* Information header */
  if (pEmailStruct->Information[0] != '\0')
  {
    if (infoSize > 255)
    {
      NDEF_Buffer[Offset++] = 0x41;
      NDEF_Buffer[Offset++] = TEXT_TYPE_STRING_LENGTH;
      NDEF_Buffer[Offset++] = (infoSize & 0xFF000000)>>24;
      NDEF_Buffer[Offset++] = (infoSize & 0x00FF0000)>>16;
      NDEF_Buffer[Offset++] = (infoSize & 0x0000FF00)>>8;
      NDEF_Buffer[Offset++] = (infoSize & 0x000000FF);
    }
    else
    {
      NDEF_Buffer[Offset++] = 0x51;
      NDEF_Buffer[Offset++] = TEXT_TYPE_STRING_LENGTH;
      NDEF_Buffer[Offset++] = (uint8_t)infoSize;
    }
    
    memcpy(&NDEF_Buffer[Offset], TEXT_TYPE_STRING, TEXT_TYPE_STRING_LENGTH);
    Offset+=TEXT_TYPE_STRING_LENGTH;
    NDEF_Buffer[Offset++] = ISO_ENGLISH_CODE_STRING_LENGTH; /* UTF-8 with x byte language code */
    memcpy(&NDEF_Buffer[Offset], ISO_ENGLISH_CODE_STRING, ISO_ENGLISH_CODE_STRING_LENGTH);
    Offset+=ISO_ENGLISH_CODE_STRING_LENGTH;
    
    /* Information payload */
    memcpy( &NDEF_Buffer[Offset], pEmailStruct->Information,strlen(pEmailStruct->Information));
    Offset += strlen(pEmailStruct->Information);
  }
  
  /* Write NDEF */
  status = NFC_TT4_WriteData ( 0x00 , Offset , NDEF_Buffer);
  
  /* Write NDEF size to complete*/
  if( status == NFC_TT4_ACTION_COMPLETED)
  {
    Offset -= 2; /* Must not count the 2 byte that represent the NDEF size */
    NDEF_Buffer[0] = (Offset & 0xFF00)>>8;
    NDEF_Buffer[1] = (Offset & 0x00FF);
    
    status = NFC_TT4_WriteData ( 0x00 , 2 , NDEF_Buffer);
  }
  
  if( status == NFC_TT4_ACTION_COMPLETED)
    return NDEF_SUCCESS;
  else
    return NDEF_ERROR;
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



