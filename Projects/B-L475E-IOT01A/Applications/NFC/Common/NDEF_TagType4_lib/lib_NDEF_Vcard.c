/**
  ******************************************************************************
  * @file    lib_NDEF_Vcard.c
  * @author  MMY Application Team
  * @brief   This file help to manage NDEF file that represent Vcard.
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
#include "lib_NDEF_Vcard.h"

/** @addtogroup NFC_libraries
 *   @{
 */


/** @addtogroup TAGTYPE4_NDEF_VCARD
  * @{
  */

/**
 * @brief  This buffer contains the data send/received by TAG
 */
extern uint8_t NDEF_Buffer [NFC_TT4_NDEF_MAX_SIZE];

/** @defgroup TAGTYPE4_NDEF_VCARD_Private_Functions    NDEF_VCARD Private Functions
  * @{
  */


static void NDEF_FillVcardStruct( uint8_t* pPayload, uint32_t PayloadSize, char* pKeyWord, uint32_t SizeOfKeyWord, uint8_t* pString);
static void NDEF_ExtractVcard ( sRecordInfo *pRecordStruct, sVcardInfo *pVcardStruct );

/**
  * @brief  This fonction fill Vcard structure with information of NDEF message
  * @param  pPayload : pointer on the payload data of the NDEF message
  * @param  PayloadSize : number of data in the payload
  * @param  pKeyWord : pointer on the keyword to look for.
  * @param  SizeOfKeyWord : number of byte of the keyword we are looking for
  * @param  pString : Pointer on the data string to fill
  * @retval NONE 
  */
static void NDEF_FillVcardStruct( uint8_t* pPayload, uint32_t PayloadSize, char* pKeyWord, uint32_t SizeOfKeyWord, uint8_t* pString)
{
  uint8_t* pLastByteAdd, *pLook4Word, *pEndString ;
  
  /* First charactere force to NULL in case not matching found */
  *pString = 0;
  
  /* Interresting information are stored before picture if any */
  /* Moreover picture is not used in this demonstration SW */  
  pLastByteAdd = pPayload;
  while( memcmp( pLastByteAdd, JPEG, JPEG_STRING_SIZE) && pLastByteAdd<(pPayload+PayloadSize) )
  {
    pLastByteAdd++;
  }
  
  pLook4Word = pPayload;
  while( memcmp( pLook4Word, pKeyWord, SizeOfKeyWord) && pLook4Word<pLastByteAdd )
  {
    pLook4Word++;
  }
  
  /* Word found */
  if( pLook4Word != pLastByteAdd)
  {  
    pLook4Word += SizeOfKeyWord;
    pEndString = pLook4Word;
    while( memcmp( pEndString, LIMIT, LIMIT_STRING_SIZE) && pEndString<pLastByteAdd )
    {
      pEndString++;
    }
    if( pEndString != pLastByteAdd)
    {
      memcpy( pString, pLook4Word, pEndString-pLook4Word);
      /* add end of string charactere */
      pString += pEndString-pLook4Word;
      *pString = '\0';    
    }
  }  
}

/**
  * @brief  This fonction read the Vcard and store data in a structure
  * @param  pRecordStruct : Pointer on the record structure
  * @param  pSMSStruct : pointer on the structure to fill
  * @retval NONE  
  */
static void NDEF_ExtractVcard ( sRecordInfo *pRecordStruct, sVcardInfo *pVcardStruct )
{
  uint32_t PayloadSize;
  uint8_t* pPayload;
  
  
  PayloadSize = ((uint32_t)(pRecordStruct->PayloadLength3)<<24) | ((uint32_t)(pRecordStruct->PayloadLength2)<<16) |
    ((uint32_t)(pRecordStruct->PayloadLength1)<<8)  | pRecordStruct->PayloadLength0;
  
  /* Read record header */
  pPayload = (uint8_t*)(pRecordStruct->PayloadBufferAdd);
  
  NDEF_FillVcardStruct(pPayload , PayloadSize,VERSION, VERSION_STRING_SIZE, (uint8_t*)(pVcardStruct->Version));
  if( !memcmp(pVcardStruct->Version, VCARD_VERSION_2_1, VCARD_VERSION_2_1_SIZE))
  {
    NDEF_FillVcardStruct(pPayload , PayloadSize,FIRSTNAME, FIRSTNAME_STRING_SIZE, (uint8_t*)(pVcardStruct->FirstName));
    NDEF_FillVcardStruct(pPayload , PayloadSize,TITLE, TITLE_STRING_SIZE, (uint8_t*)(pVcardStruct->Title));
    NDEF_FillVcardStruct(pPayload , PayloadSize,ORG, ORG_STRING_SIZE, (uint8_t*)(pVcardStruct->Org));
    NDEF_FillVcardStruct(pPayload , PayloadSize,HOME_ADDRESS, HOME_ADDRESS_STRING_SIZE, (uint8_t*)(pVcardStruct->HomeAddress));
    NDEF_FillVcardStruct(pPayload , PayloadSize,WORK_ADDRESS, WORK_ADDRESS_STRING_SIZE, (uint8_t*)(pVcardStruct->WorkAddress));    
    NDEF_FillVcardStruct(pPayload , PayloadSize,HOME_TEL, HOME_TEL_STRING_SIZE, (uint8_t*)(pVcardStruct->HomeTel));
    NDEF_FillVcardStruct(pPayload , PayloadSize,WORK_TEL, WORK_TEL_STRING_SIZE, (uint8_t*)(pVcardStruct->WorkTel));    
    NDEF_FillVcardStruct(pPayload , PayloadSize,CELL_TEL, CELL_TEL_STRING_SIZE, (uint8_t*)(pVcardStruct->CellTel));
    NDEF_FillVcardStruct(pPayload , PayloadSize,HOME_EMAIL, HOME_EMAIL_STRING_SIZE, (uint8_t*)(pVcardStruct->HomeEmail));  
    NDEF_FillVcardStruct(pPayload , PayloadSize,WORK_EMAIL, WORK_EMAIL_STRING_SIZE, (uint8_t*)(pVcardStruct->WorkEmail));    
  }
  else if( !memcmp(pVcardStruct->Version, VCARD_VERSION_3_0, VCARD_VERSION_3_0_SIZE))
  {
    /* need to be implemented */
  }
  else
  {
    /* maybe new version but not supported in this sw */
  }
  
}

/**
  * @}
  */

/** @defgroup TAGTYPE4_NDEF_VCARD_Public_Functions    NDEF_VCARD Public Functions
  * @{
  *  @brief  This file is used to manage Vcard (stored or loaded in tag)
  */ 

/**
  * @brief  This fonction read NDEF and retrieve Vcard information if any
  * @param  pRecordStruct : Pointer on the record structure
  * @param  pVcardStruct : pointer on the structure to fill 
  * @retval SUCCESS : Vcard information from NDEF have been retrieved
  * @retval ERROR : Not able to retrieve Vcard information
  */
uint16_t NDEF_ReadVcard ( sRecordInfo *pRecordStruct, sVcardInfo *pVcardStruct )
{
  uint16_t status = NDEF_ERROR;
  
  if( pRecordStruct->NDEF_Type == VCARD_TYPE)
  {  
    NDEF_ExtractVcard(pRecordStruct, pVcardStruct );
    status = NDEF_SUCCESS;
  }
  
  return status;
}

/**
  * @brief  This fonction write the NDEF file with the Vcard data given in the structure
  * @param  pVcardStruct : pointer on structure that contain the Vcard information
  * @retval SUCCESS : the function is succesful
  * @retval ERROR : Not able to store NDEF file inside tag.
  */
uint16_t NDEF_WriteVcard ( sVcardInfo *pVcardStruct )
{
  uint16_t status = NDEF_ERROR;
  uint16_t DataSize;
  uint32_t PayloadSize = 0;
  
  /* Vcard Record Header */
  /************************************/  
  /*  7 |  6 |  5 |  4 |  3 | 2  1  0 */
  /*----------------------------------*/  
  /* MB   ME   CF   SR   IL    TNF    */  /* <---- CF=0, IL=0 and SR=0 TNF=2 NFC Forum Media type*/
  /*----------------------------------*/  
  /*          TYPE LENGTH             */
  /*----------------------------------*/
  /*        PAYLOAD LENGTH 3          */
  /*----------------------------------*/
  /*        PAYLOAD LENGTH 2          */
  /*----------------------------------*/
  /*        PAYLOAD LENGTH 1          */
  /*----------------------------------*/  
  /*        PAYLOAD LENGTH 0          */
  /*----------------------------------*/
  /*          ID LENGTH               */  /* <---- Not Used  */
  /*----------------------------------*/
  /*              TYPE                */
  /*----------------------------------*/
  /*               ID                 */  /* <---- Not Used  */ 
  /************************************/
  
  /* As we don't have embedded a jpeg encoder/decoder in this firmware */
  /* We have made the choice to manage only string content of the vCard */
  /* For demonstration purpose in order to fill the 8kB of the M24SR */
  /* We have embedded a NDEF vCard in the STM32 to be able to fill M24SR */
  
  
  /* NDEF file must be written in 2 phases, first phase NDEF size is Null */
  NDEF_Buffer[NDEF_SIZE_OFFSET] = 0x00;
  NDEF_Buffer[NDEF_SIZE_OFFSET+1] = 0x00;
  
  /* fill record header */
  NDEF_Buffer[FIRST_RECORD_OFFSET] = 0xC2;   /* Record Flag */
  NDEF_Buffer[FIRST_RECORD_OFFSET+1] = VCARD_TYPE_STRING_LENGTH;
  NDEF_Buffer[FIRST_RECORD_OFFSET+2] = 0x00; /* Will be filled at the end when payload size is known */
  NDEF_Buffer[FIRST_RECORD_OFFSET+3] = 0x00;
  NDEF_Buffer[FIRST_RECORD_OFFSET+4] = 0x00;
  NDEF_Buffer[FIRST_RECORD_OFFSET+5] = 0x00;  
  memcpy(&NDEF_Buffer[FIRST_RECORD_OFFSET+6], VCARD_TYPE_STRING, VCARD_TYPE_STRING_LENGTH);
  
  /* Payload is positionned in the NDEF after record header */
  PayloadSize = FIRST_RECORD_OFFSET+6+VCARD_TYPE_STRING_LENGTH;
  
  /* "BEGIN:VCARD\r\n" */
  memcpy(&NDEF_Buffer[PayloadSize], BEGIN, BEGIN_STRING_SIZE);
  PayloadSize += BEGIN_STRING_SIZE;
  memcpy( &NDEF_Buffer[PayloadSize], VCARD,VCARD_STRING_SIZE);
  PayloadSize += VCARD_STRING_SIZE;
  memcpy( &NDEF_Buffer[PayloadSize], LIMIT, LIMIT_STRING_SIZE);
  PayloadSize += LIMIT_STRING_SIZE;
  
  /* "VERSION:2.1\r\n" */
  memcpy(&NDEF_Buffer[PayloadSize], VERSION, VERSION_STRING_SIZE);
  PayloadSize += VERSION_STRING_SIZE;
  memcpy( &NDEF_Buffer[PayloadSize], VCARD_VERSION_2_1,VCARD_VERSION_2_1_SIZE);
  PayloadSize += VCARD_VERSION_2_1_SIZE;
  memcpy( &NDEF_Buffer[PayloadSize], LIMIT, LIMIT_STRING_SIZE);
  PayloadSize += LIMIT_STRING_SIZE;
  
  /* "FN:\r\n" */
  memcpy(&NDEF_Buffer[PayloadSize], FIRSTNAME, FIRSTNAME_STRING_SIZE);
  PayloadSize += FIRSTNAME_STRING_SIZE;
  memcpy( &NDEF_Buffer[PayloadSize], pVcardStruct->FirstName,strlen(pVcardStruct->FirstName));
  PayloadSize += strlen(pVcardStruct->FirstName);
  memcpy( &NDEF_Buffer[PayloadSize], LIMIT, LIMIT_STRING_SIZE);
  PayloadSize += LIMIT_STRING_SIZE;
  
  /* "TITLE:\r\n" */
  memcpy(&NDEF_Buffer[PayloadSize], TITLE,TITLE_STRING_SIZE);
  PayloadSize += TITLE_STRING_SIZE;
  memcpy( &NDEF_Buffer[PayloadSize], pVcardStruct->Title,strlen(pVcardStruct->Title));
  PayloadSize += strlen(pVcardStruct->Title);
  memcpy( &NDEF_Buffer[PayloadSize], LIMIT, LIMIT_STRING_SIZE);
  PayloadSize += LIMIT_STRING_SIZE;
  
  /* "ORG:\r\n" */
  memcpy(&NDEF_Buffer[PayloadSize], ORG,ORG_STRING_SIZE);
  PayloadSize += ORG_STRING_SIZE;
  memcpy( &NDEF_Buffer[PayloadSize], pVcardStruct->Org,strlen(pVcardStruct->Org));
  PayloadSize += strlen(pVcardStruct->Org);
  memcpy( &NDEF_Buffer[PayloadSize], LIMIT, LIMIT_STRING_SIZE);
  PayloadSize += LIMIT_STRING_SIZE;
  
  /* "ADR;HOME:\r\n" */
  memcpy(&NDEF_Buffer[PayloadSize], HOME_ADDRESS,HOME_ADDRESS_STRING_SIZE);
  PayloadSize += HOME_ADDRESS_STRING_SIZE;
  memcpy( &NDEF_Buffer[PayloadSize], pVcardStruct->HomeAddress,strlen(pVcardStruct->HomeAddress));
  PayloadSize += strlen(pVcardStruct->HomeAddress);
  memcpy( &NDEF_Buffer[PayloadSize], LIMIT, LIMIT_STRING_SIZE);
  PayloadSize += LIMIT_STRING_SIZE;
  
  /* "ADR;WORK:\r\n" */
  memcpy(&NDEF_Buffer[PayloadSize], WORK_ADDRESS,WORK_ADDRESS_STRING_SIZE);
  PayloadSize += WORK_ADDRESS_STRING_SIZE;
  memcpy( &NDEF_Buffer[PayloadSize], pVcardStruct->WorkAddress,strlen(pVcardStruct->WorkAddress));
  PayloadSize += strlen(pVcardStruct->WorkAddress);
  memcpy( &NDEF_Buffer[PayloadSize], LIMIT, LIMIT_STRING_SIZE);
  PayloadSize += LIMIT_STRING_SIZE;
  
  /* "TEL;HOME:\r\n" */
  memcpy(&NDEF_Buffer[PayloadSize], HOME_TEL, HOME_TEL_STRING_SIZE);
  PayloadSize += HOME_TEL_STRING_SIZE;
  memcpy( &NDEF_Buffer[PayloadSize], pVcardStruct->HomeTel,strlen(pVcardStruct->HomeTel));
  PayloadSize += strlen(pVcardStruct->HomeTel);
  memcpy( &NDEF_Buffer[PayloadSize], LIMIT, LIMIT_STRING_SIZE);
  PayloadSize += LIMIT_STRING_SIZE;
  
  /* "TEL;WORK:\r\n" */
  memcpy(&NDEF_Buffer[PayloadSize], WORK_TEL, WORK_TEL_STRING_SIZE);
  PayloadSize += WORK_TEL_STRING_SIZE;
  memcpy( &NDEF_Buffer[PayloadSize], pVcardStruct->WorkTel,strlen(pVcardStruct->WorkTel));
  PayloadSize += strlen(pVcardStruct->WorkTel);
  memcpy( &NDEF_Buffer[PayloadSize], LIMIT, LIMIT_STRING_SIZE);
  PayloadSize += LIMIT_STRING_SIZE;
  
  /* "TEL;CELL:\r\n" */
  memcpy(&NDEF_Buffer[PayloadSize], CELL_TEL, CELL_TEL_STRING_SIZE);
  PayloadSize += CELL_TEL_STRING_SIZE;
  memcpy( &NDEF_Buffer[PayloadSize], pVcardStruct->CellTel,strlen(pVcardStruct->CellTel));
  PayloadSize += strlen(pVcardStruct->CellTel);
  memcpy( &NDEF_Buffer[PayloadSize], LIMIT, LIMIT_STRING_SIZE);
  PayloadSize += LIMIT_STRING_SIZE;
  
  /* "EMAIL;HOME:\r\n" */
  memcpy(&NDEF_Buffer[PayloadSize], HOME_EMAIL, HOME_EMAIL_STRING_SIZE);
  PayloadSize += HOME_EMAIL_STRING_SIZE;
  memcpy( &NDEF_Buffer[PayloadSize], pVcardStruct->HomeEmail,strlen(pVcardStruct->HomeEmail));
  PayloadSize += strlen(pVcardStruct->HomeEmail);
  memcpy( &NDEF_Buffer[PayloadSize], LIMIT, LIMIT_STRING_SIZE);
  PayloadSize += LIMIT_STRING_SIZE;
  
  /* "EMAIL;WORK:\r\n" */
  memcpy(&NDEF_Buffer[PayloadSize], WORK_EMAIL, WORK_EMAIL_STRING_SIZE);
  PayloadSize += WORK_EMAIL_STRING_SIZE;
  memcpy( &NDEF_Buffer[PayloadSize], pVcardStruct->WorkEmail,strlen(pVcardStruct->WorkEmail));
  PayloadSize += strlen(pVcardStruct->WorkEmail);
  memcpy( &NDEF_Buffer[PayloadSize], LIMIT, LIMIT_STRING_SIZE);
  PayloadSize += LIMIT_STRING_SIZE;
  
  /* "END:VCARD\r\n" */
  memcpy(&NDEF_Buffer[PayloadSize], END,END_STRING_SIZE);
  PayloadSize += END_STRING_SIZE;
  memcpy( &NDEF_Buffer[PayloadSize], VCARD,VCARD_STRING_SIZE);
  PayloadSize += VCARD_STRING_SIZE;
  memcpy( &NDEF_Buffer[PayloadSize], LIMIT, LIMIT_STRING_SIZE);
  PayloadSize += LIMIT_STRING_SIZE;
  
  DataSize = (uint16_t)(PayloadSize); /* Must not count the 2 byte that represent the NDEF size */
  PayloadSize -= FIRST_RECORD_OFFSET+6+VCARD_TYPE_STRING_LENGTH;  
  
  NDEF_Buffer[FIRST_RECORD_OFFSET+2] = (PayloadSize & 0xFF000000)>>24;
  NDEF_Buffer[FIRST_RECORD_OFFSET+3] = (PayloadSize & 0x00FF0000)>>16;
  NDEF_Buffer[FIRST_RECORD_OFFSET+4] = (PayloadSize & 0x0000FF00)>>8;
  NDEF_Buffer[FIRST_RECORD_OFFSET+5] = (PayloadSize & 0x000000FF);
  
  
  /* Write NDEF */
  status = NFC_TT4_WriteData ( 0x00 , DataSize , NDEF_Buffer);
  
  /* Write NDEF size to complete*/
  if( status == NFC_TT4_ACTION_COMPLETED)
  {
    DataSize -= 2; /* Must not count the 2 byte that represent the NDEF size */
    NDEF_Buffer[0] = (DataSize & 0xFF00)>>8;
    NDEF_Buffer[1] = (DataSize & 0x00FF);
    
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




