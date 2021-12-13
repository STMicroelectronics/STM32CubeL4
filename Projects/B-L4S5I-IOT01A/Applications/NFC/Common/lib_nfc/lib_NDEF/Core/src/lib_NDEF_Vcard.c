/**
  ******************************************************************************
  * @file    lib_NDEF_Vcard.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file help to manage NDEF file that represent Vcard.
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

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF_Vcard.h"


/** @addtogroup lib_NDEF_Vcard
  * @ingroup libNDEF
  * @{
  * @brief  This part of the library manage the data which follow NFC forum organisation.
  */

/*  This buffer contains the data sent/received by TAG */
extern uint8_t NDEF_Buffer [];

static void NDEF_FillVcardStruct( uint8_t* pPayload, uint32_t PayloadSize, char* pKeyWord, uint32_t SizeOfKeyWord, uint8_t* pString );
static void NDEF_ExtractVcard( sRecordInfo_t *pRecordStruct, sVcardInfo *pVcardStruct );

/**
  * @brief  This function extracts a Vcard particular property from a vCard.
  * @param  pPayload      Pointer on the vCard payload data of the vCard record.
  * @param  PayloadSize   Number of bytes in the vCard payload.
  * @param  pKeyWord      Pointer on the vCard property keyword to look for.
  * @param  SizeOfKeyWord Number of bytes of the vCard property keyword we are looking for.
  * @param  pString       Pointer on a string used to return the vCard property read.
  */
static void NDEF_FillVcardStruct( uint8_t* pPayload, uint32_t PayloadSize, char* pKeyWord, uint32_t SizeOfKeyWord, uint8_t* pString )
{
  uint8_t* pLastByteAdd, *pLook4Word, *pEndString;

  /* First character force to NULL in case not matching found */
  *pString = 0;
  
   pLastByteAdd = pPayload;
  while( memcmp( pLastByteAdd, JPEG, JPEG_STRING_SIZE ) && (pLastByteAdd < (pPayload + PayloadSize)) )
  {
    pLastByteAdd++;
  }

  pLook4Word = pPayload;
  while( memcmp( pLook4Word, pKeyWord, SizeOfKeyWord ) && (pLook4Word < pLastByteAdd) )
  {
    pLook4Word++;
  }

  /* Word found */
  if( pLook4Word != pLastByteAdd )
  {
    pLook4Word += SizeOfKeyWord;
    pEndString = pLook4Word;
    while( memcmp( pEndString, LIMIT, LIMIT_STRING_SIZE ) && (*pEndString!='\n') && (pEndString < pLastByteAdd) )
    {
      pEndString++;
    }
    if( pEndString != pLastByteAdd )
    {
      memcpy( pString, pLook4Word, pEndString-pLook4Word );
      /* add end of string character */
      pString += pEndString - pLook4Word;
      *pString = '\0';
    }
  }	
}

/**
  * @brief  This function reads a Vcard record and stores data in a `sVcardInfo` structure.
  * @param  pRecordStruct Pointer on the vCard record structure.
  * @param  pVcardStruct  Pointer on the `sCardInfo` structure to fill.
  */
static void NDEF_ExtractVcard( sRecordInfo_t *pRecordStruct, sVcardInfo *pVcardStruct )
{
  uint32_t PayloadSize;
  uint8_t* pPayload;


  PayloadSize = pRecordStruct->PayloadLength;

  /* Read record header */
  pPayload = (uint8_t*)(pRecordStruct->PayloadBufferAdd);

  NDEF_FillVcardStruct( pPayload, PayloadSize, VERSION, VERSION_STRING_SIZE, (uint8_t*)(pVcardStruct->Version) );
  if( !memcmp( pVcardStruct->Version, VCARD_VERSION_2_1, VCARD_VERSION_2_1_SIZE ) )
  {
    NDEF_FillVcardStruct( pPayload, PayloadSize, FIRSTNAME, FIRSTNAME_STRING_SIZE, (uint8_t*)(pVcardStruct->FirstName) );
    NDEF_FillVcardStruct( pPayload, PayloadSize, TITLE, TITLE_STRING_SIZE, (uint8_t*)(pVcardStruct->Title) );
    NDEF_FillVcardStruct( pPayload, PayloadSize, ORG, ORG_STRING_SIZE, (uint8_t*)(pVcardStruct->Org) );
    NDEF_FillVcardStruct( pPayload, PayloadSize, HOME_ADDRESS, HOME_ADDRESS_STRING_SIZE, (uint8_t*)(pVcardStruct->HomeAddress) );
    NDEF_FillVcardStruct( pPayload, PayloadSize, GEN_ADDRESS, strlen(GEN_ADDRESS), (uint8_t*)(pVcardStruct->Address) );
    NDEF_FillVcardStruct( pPayload, PayloadSize, WORK_ADDRESS, WORK_ADDRESS_STRING_SIZE, (uint8_t*)(pVcardStruct->WorkAddress) );
    NDEF_FillVcardStruct( pPayload, PayloadSize, HOME_TEL, HOME_TEL_STRING_SIZE, (uint8_t*)(pVcardStruct->HomeTel) );
    NDEF_FillVcardStruct( pPayload, PayloadSize, WORK_TEL, WORK_TEL_STRING_SIZE, (uint8_t*)(pVcardStruct->WorkTel) );
    NDEF_FillVcardStruct( pPayload, PayloadSize, CELL_TEL, CELL_TEL_STRING_SIZE, (uint8_t*)(pVcardStruct->CellTel) );
    NDEF_FillVcardStruct( pPayload, PayloadSize, HOME_EMAIL, HOME_EMAIL_STRING_SIZE, (uint8_t*)(pVcardStruct->HomeEmail) );
    NDEF_FillVcardStruct( pPayload, PayloadSize, GEN_EMAIL, strlen(GEN_EMAIL), (uint8_t*)(pVcardStruct->Email) );
    NDEF_FillVcardStruct( pPayload, PayloadSize, WORK_EMAIL, WORK_EMAIL_STRING_SIZE, (uint8_t*)(pVcardStruct->WorkEmail) );
  }
  else if( !memcmp( pVcardStruct->Version, VCARD_VERSION_3_0, VCARD_VERSION_3_0_SIZE ) )
  {
    /* need to be implemented */
  }
  else
  {
    /* maybe new version but not supported in this sw */
  }

}

/**
  * @brief  This function reads a Vcard record and stores data in a `sVcardInfo` structure.
  * @param  pRecordStruct Pointer on the vCard record structure.
  * @param  pVcardStruct  Pointer on the `sCardInfo` structure to fill.
  * @retval NDEF_OK     The Vcard information has been retrieved.
  * @retval NDEF_ERROR  Not able to retrieve the Vcard information.
  */
uint16_t NDEF_ReadVcard( sRecordInfo_t *pRecordStruct, sVcardInfo *pVcardStruct )
{
  uint16_t status = NDEF_ERROR;

  if( pRecordStruct->NDEF_Type == VCARD_TYPE )
  {
    NDEF_ExtractVcard( pRecordStruct, pVcardStruct );
    status = NDEF_OK;
  }

  return status;
}

/**
  * @brief  This function writes a NDEF message to the NFC tag with the Vcard data given in a `sVcardInfo` structure.
  * @param  pVcardStruct Pointer on a `sVcardInfo` structure containing the Vcard information.
  * @retval NDEF_OK                     NDEF file data written in the tag.
  * @retval NDEF_ERROR                  Not able to store the NDEF in tag.
  * @retval NDEF_ERROR_MEMORY_INTERNAL  Cannot write to tag.
  * @retval NDEF_ERROR_NOT_FORMATED     CCFile data not supported or not present.
  * @retval NDEF_ERROR_MEMORY_TAG       Size not compatible with memory.
  * @retval NDEF_ERROR_LOCKED           Tag locked, cannot be write.
  */
uint16_t NDEF_WriteVcard( sVcardInfo *pVcardStruct )
{
  uint16_t status = NDEF_ERROR, Offset = 0;

  NDEF_PrepareVcardMessage( pVcardStruct, NDEF_Buffer, &Offset );

  status = NfcTag_WriteNDEF( Offset , NDEF_Buffer );

  return status;
}

/**
  * @brief  This function computes the size of a Vcard record from the data given in the `sVcardInfo` structure.
  * @param  pVcardStruct Pointer on a `sVcardInfo` structure containing the Vcard information.
  * @return vCard record data size in bytes (excluding record metadata).
  */
uint32_t NDEF_GetVcardLength( sVcardInfo *pVcardStruct)
{

  uint32_t PayloadSize = 0;

  /* "BEGIN:VCARD\r\n" */
  PayloadSize += BEGIN_STRING_SIZE;
  PayloadSize += VCARD_STRING_SIZE;
  PayloadSize += LIMIT_STRING_SIZE;

  /* "VERSION:2.1\r\n" */
  PayloadSize += VERSION_STRING_SIZE;
  PayloadSize += VCARD_VERSION_2_1_SIZE;
  PayloadSize += LIMIT_STRING_SIZE;

  /* "N:\r\n" */
  if(*pVcardStruct->Name != '\0')
  {
  PayloadSize += VCARD_NAME_STRING_SIZE;
  PayloadSize += strlen( pVcardStruct->Name );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  /* "FN:\r\n" */
  if(*pVcardStruct->FirstName != '\0')
  {
    PayloadSize += FIRSTNAME_STRING_SIZE;
    PayloadSize += strlen( pVcardStruct->FirstName );
    PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->Title != '\0')
  {
  /* "TITLE:\r\n" */
  PayloadSize += TITLE_STRING_SIZE;
  PayloadSize += strlen( pVcardStruct->Title );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->Org != '\0')
  {
  /* "ORG:\r\n" */
  PayloadSize += ORG_STRING_SIZE;
  PayloadSize += strlen( pVcardStruct->Org );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->Url != '\0')
  {
  /* "URL:\r\n" */
  PayloadSize += URL_STRING_SIZE;
  PayloadSize += strlen( pVcardStruct->Url );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->HomeAddress != '\0')
  {
  /* "ADR;HOME:\r\n" */
  PayloadSize += HOME_ADDRESS_STRING_SIZE;
  PayloadSize += strlen( pVcardStruct->HomeAddress );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->WorkAddress != '\0')
  {
  /* "ADR;WORK:\r\n" */
  PayloadSize += WORK_ADDRESS_STRING_SIZE;
  PayloadSize += strlen( pVcardStruct->WorkAddress );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->HomeTel != '\0')
  {
  /* "TEL;HOME:\r\n" */
  PayloadSize += HOME_TEL_STRING_SIZE;
  PayloadSize += strlen( pVcardStruct->HomeTel );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->WorkTel != '\0')
  {
  /* "TEL;WORK:\r\n" */
  PayloadSize += WORK_TEL_STRING_SIZE;
  PayloadSize += strlen( pVcardStruct->WorkTel );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->CellTel != '\0')
  {
  /* "TEL;CELL:\r\n" */
  PayloadSize += CELL_TEL_STRING_SIZE;
  PayloadSize += strlen( pVcardStruct->CellTel );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->HomeEmail != '\0')
  {
  /* "EMAIL;HOME:\r\n" */
  PayloadSize += HOME_EMAIL_STRING_SIZE;
  PayloadSize += strlen( pVcardStruct->HomeEmail );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->WorkEmail != '\0')
  {
  /* "EMAIL;WORK:\r\n" */
  PayloadSize += WORK_EMAIL_STRING_SIZE;
  PayloadSize += strlen( pVcardStruct->WorkEmail );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if((pVcardStruct->Picture != NULL) && (pVcardStruct->PictureSize != 0))
  {
    PayloadSize += strlen(PICTURE_PHOTO);
    PayloadSize += strlen(JPEG);
    PayloadSize++;
    PayloadSize += strlen(PICTURE_BASE64);
    PayloadSize++;
    PayloadSize += pVcardStruct->PictureSize;
  }

  /* "END:VCARD\r\n" */
  PayloadSize += END_STRING_SIZE;
  PayloadSize += VCARD_STRING_SIZE;
 /* PayloadSize += LIMIT_STRING_SIZE;*/

  return PayloadSize;
  
}

/**
  * @brief  This function write the NDEF file with the Vcard data given in the structure.
  * @param  pVcardStruct : pointer on structure that contain the Vcard information.
  * @param  pNDEFMessage : pointer on the NDEF message.
  * @param  size : to store the size of the NDEF message generated.
  */
void NDEF_PrepareVcardMessage( sVcardInfo *pVcardStruct, uint8_t *pNDEFMessage, uint16_t *size )
{

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
/*           ID LENGTH              */  /* <---- Not Used  */
/*----------------------------------*/
/*              TYPE                */
/*----------------------------------*/
/*               ID                 */  /* <---- Not Used  */
/************************************/

  /* As we don't have embedded a jpeg encoder/decoder in this firmware */
  /* We have made the choice to manage only string content of the vCard */
  /* For demonstration purpose in order to fill the 8kB of the M24SR */
  /* We have embedded a NDEF vCard in the STM32 to be able to fill M24SR */

  /* fill record header */
  uint32_t length = NDEF_GetVcardLength(pVcardStruct);
  if(length >= 0xFF)
  {
  pNDEFMessage[0] = 0xC2;   /* Record Flag */
  pNDEFMessage[1] = XVCARD_TYPE_STRING_LENGTH;
  pNDEFMessage[5] = length & 0xff; 
  pNDEFMessage[4] = (length>>8) & 0xff;
  pNDEFMessage[3] = (length>>16) & 0xff;
  pNDEFMessage[2] = length>>24;	
  memcpy( &pNDEFMessage[6], XVCARD_TYPE_STRING, XVCARD_TYPE_STRING_LENGTH );

  /* Payload is positionned in the NDEF after record header */
  PayloadSize = 6 + XVCARD_TYPE_STRING_LENGTH;
  } else {
  pNDEFMessage[0] = 0xD2;   /* Record Flag */
  pNDEFMessage[1] = XVCARD_TYPE_STRING_LENGTH;
  pNDEFMessage[2] =  length;
  memcpy( &pNDEFMessage[3], XVCARD_TYPE_STRING, XVCARD_TYPE_STRING_LENGTH );

  /* Payload is positionned in the NDEF after record header */
  PayloadSize = 3 + XVCARD_TYPE_STRING_LENGTH;
  }

  /* "BEGIN:VCARD\r\n" */
  memcpy( &pNDEFMessage[PayloadSize], BEGIN, BEGIN_STRING_SIZE );
  PayloadSize += BEGIN_STRING_SIZE;
  memcpy( &pNDEFMessage[PayloadSize], VCARD,VCARD_STRING_SIZE );
  PayloadSize += VCARD_STRING_SIZE;
  memcpy( &pNDEFMessage[PayloadSize], LIMIT, LIMIT_STRING_SIZE );
  PayloadSize += LIMIT_STRING_SIZE;

  /* "VERSION:2.1\r\n" */
  memcpy( &pNDEFMessage[PayloadSize], VERSION, VERSION_STRING_SIZE );
  PayloadSize += VERSION_STRING_SIZE;
  memcpy( &pNDEFMessage[PayloadSize], VCARD_VERSION_2_1, VCARD_VERSION_2_1_SIZE );
  PayloadSize += VCARD_VERSION_2_1_SIZE;
  memcpy( &pNDEFMessage[PayloadSize], LIMIT, LIMIT_STRING_SIZE );
  PayloadSize += LIMIT_STRING_SIZE;

  /* "N:\r\n" */
  if(*pVcardStruct->Name != '\0')
  {
  memcpy( &pNDEFMessage[PayloadSize], VCARD_NAME, VCARD_NAME_STRING_SIZE );
  PayloadSize += VCARD_NAME_STRING_SIZE;
  memcpy( &pNDEFMessage[PayloadSize], pVcardStruct->Name, strlen(pVcardStruct->Name) );
  PayloadSize += strlen( pVcardStruct->Name );
  memcpy( &pNDEFMessage[PayloadSize], LIMIT, LIMIT_STRING_SIZE );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  /* "FN:\r\n" */
  if(*pVcardStruct->FirstName != '\0')
  {
  memcpy( &pNDEFMessage[PayloadSize], FIRSTNAME, FIRSTNAME_STRING_SIZE );
  PayloadSize += FIRSTNAME_STRING_SIZE;
  memcpy( &pNDEFMessage[PayloadSize], pVcardStruct->FirstName, strlen(pVcardStruct->FirstName) );
  PayloadSize += strlen( pVcardStruct->FirstName );
  memcpy( &pNDEFMessage[PayloadSize], LIMIT, LIMIT_STRING_SIZE );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->HomeTel != '\0')
  {

  /* "TEL;HOME:\r\n" */
  memcpy( &pNDEFMessage[PayloadSize], HOME_TEL, HOME_TEL_STRING_SIZE );
  PayloadSize += HOME_TEL_STRING_SIZE;
  memcpy( &pNDEFMessage[PayloadSize], pVcardStruct->HomeTel, strlen(pVcardStruct->HomeTel) );
  PayloadSize += strlen( pVcardStruct->HomeTel );
  memcpy( &pNDEFMessage[PayloadSize], LIMIT, LIMIT_STRING_SIZE );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->WorkTel != '\0')
  {

  /* "TEL;WORK:\r\n" */
  memcpy( &pNDEFMessage[PayloadSize], WORK_TEL, WORK_TEL_STRING_SIZE );
  PayloadSize += WORK_TEL_STRING_SIZE;
  memcpy( &pNDEFMessage[PayloadSize], pVcardStruct->WorkTel, strlen(pVcardStruct->WorkTel) );
  PayloadSize += strlen( pVcardStruct->WorkTel );
  memcpy( &pNDEFMessage[PayloadSize], LIMIT, LIMIT_STRING_SIZE );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->CellTel != '\0')
  {

  /* "TEL;CELL:\r\n" */
  memcpy( &pNDEFMessage[PayloadSize], CELL_TEL, CELL_TEL_STRING_SIZE );
  PayloadSize += CELL_TEL_STRING_SIZE;
  memcpy( &pNDEFMessage[PayloadSize], pVcardStruct->CellTel, strlen(pVcardStruct->CellTel) );
  PayloadSize += strlen( pVcardStruct->CellTel );
  memcpy( &pNDEFMessage[PayloadSize], LIMIT, LIMIT_STRING_SIZE );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->HomeEmail != '\0')
  {

  /* "EMAIL;HOME:\r\n" */
  memcpy( &pNDEFMessage[PayloadSize], HOME_EMAIL, HOME_EMAIL_STRING_SIZE );
  PayloadSize += HOME_EMAIL_STRING_SIZE;
  memcpy( &pNDEFMessage[PayloadSize], pVcardStruct->HomeEmail, strlen(pVcardStruct->HomeEmail) );
  PayloadSize += strlen( pVcardStruct->HomeEmail );
  memcpy( &pNDEFMessage[PayloadSize], LIMIT, LIMIT_STRING_SIZE );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->WorkEmail != '\0')
  {

  /* "EMAIL;WORK:\r\n" */
  memcpy( &pNDEFMessage[PayloadSize], WORK_EMAIL, WORK_EMAIL_STRING_SIZE );
  PayloadSize += WORK_EMAIL_STRING_SIZE;
  memcpy( &pNDEFMessage[PayloadSize], pVcardStruct->WorkEmail, strlen(pVcardStruct->WorkEmail) );
  PayloadSize += strlen( pVcardStruct->WorkEmail );
  memcpy( &pNDEFMessage[PayloadSize], LIMIT, LIMIT_STRING_SIZE );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->HomeAddress != '\0')
  {

  /* "ADR;HOME:\r\n" */
  memcpy( &pNDEFMessage[PayloadSize], HOME_ADDRESS, HOME_ADDRESS_STRING_SIZE );
  PayloadSize += HOME_ADDRESS_STRING_SIZE;
  memcpy( &pNDEFMessage[PayloadSize], pVcardStruct->HomeAddress, strlen(pVcardStruct->HomeAddress) );
  PayloadSize += strlen( pVcardStruct->HomeAddress );
  memcpy( &pNDEFMessage[PayloadSize], LIMIT, LIMIT_STRING_SIZE );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->WorkAddress != '\0')
  {

  /* "ADR;WORK:\r\n" */
  memcpy( &pNDEFMessage[PayloadSize], WORK_ADDRESS, WORK_ADDRESS_STRING_SIZE );
  PayloadSize += WORK_ADDRESS_STRING_SIZE;
  memcpy( &pNDEFMessage[PayloadSize], pVcardStruct->WorkAddress, strlen(pVcardStruct->WorkAddress) );
  PayloadSize += strlen( pVcardStruct->WorkAddress );
  memcpy( &pNDEFMessage[PayloadSize], LIMIT, LIMIT_STRING_SIZE );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->Org != '\0')
  {

  /* "ORG:\r\n" */
  memcpy( &pNDEFMessage[PayloadSize], ORG, ORG_STRING_SIZE );
  PayloadSize += ORG_STRING_SIZE;
  memcpy( &pNDEFMessage[PayloadSize], pVcardStruct->Org, strlen(pVcardStruct->Org) );
  PayloadSize += strlen( pVcardStruct->Org );
  memcpy( &pNDEFMessage[PayloadSize], LIMIT, LIMIT_STRING_SIZE );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->Title != '\0')
  {
  /* "TITLE:\r\n" */
  memcpy( &pNDEFMessage[PayloadSize], TITLE, TITLE_STRING_SIZE );
  PayloadSize += TITLE_STRING_SIZE;
  memcpy( &pNDEFMessage[PayloadSize], pVcardStruct->Title, strlen(pVcardStruct->Title) );
  PayloadSize += strlen( pVcardStruct->Title );
  memcpy( &pNDEFMessage[PayloadSize], LIMIT, LIMIT_STRING_SIZE );
  PayloadSize += LIMIT_STRING_SIZE;
  }
  if(*pVcardStruct->Url != '\0')
  {
  /* "URL:\r\n" */
  memcpy( &pNDEFMessage[PayloadSize], URL, URL_STRING_SIZE );
  PayloadSize += URL_STRING_SIZE;
  memcpy( &pNDEFMessage[PayloadSize], pVcardStruct->Url, strlen(pVcardStruct->Url) );
  PayloadSize += strlen( pVcardStruct->Url );
  memcpy( &pNDEFMessage[PayloadSize], LIMIT, LIMIT_STRING_SIZE );
  PayloadSize += LIMIT_STRING_SIZE;
  }

  if((pVcardStruct->Picture != NULL) && (pVcardStruct->PictureSize != 0))
  {
    memcpy(&pNDEFMessage[PayloadSize],PICTURE_PHOTO,strlen(PICTURE_PHOTO));
    PayloadSize += strlen(PICTURE_PHOTO);
    memcpy(&pNDEFMessage[PayloadSize],JPEG,strlen(JPEG));
    PayloadSize += strlen(JPEG);
    pNDEFMessage[PayloadSize] = ';';
    PayloadSize++;
    memcpy(&pNDEFMessage[PayloadSize],PICTURE_BASE64,strlen(PICTURE_BASE64));
    PayloadSize += strlen(PICTURE_BASE64);
    pNDEFMessage[PayloadSize] = ':';
    PayloadSize++;
    memcpy(&pNDEFMessage[PayloadSize],pVcardStruct->Picture,pVcardStruct->PictureSize);
    PayloadSize += pVcardStruct->PictureSize;
  }

  /* "END:VCARD\r\n" */
  memcpy( &pNDEFMessage[PayloadSize], END, END_STRING_SIZE );
  PayloadSize += END_STRING_SIZE;
  memcpy( &pNDEFMessage[PayloadSize], VCARD, VCARD_STRING_SIZE );
  PayloadSize += VCARD_STRING_SIZE;
//  memcpy( &pNDEFMessage[PayloadSize], LIMIT, LIMIT_STRING_SIZE );
//  PayloadSize += LIMIT_STRING_SIZE;

  *size = (uint16_t)(PayloadSize); /* Must not count the 2 byte that represent the NDEF size */
/*
  PayloadSize -= 6 + VCARD_TYPE_STRING_LENGTH;

  pNDEFMessage[2] = (PayloadSize & 0xFF000000) >> 24;
  pNDEFMessage[3] = (PayloadSize & 0x00FF0000) >> 16;
  pNDEFMessage[4] = (PayloadSize & 0x0000FF00) >> 8;
  pNDEFMessage[5] = PayloadSize & 0x000000FF;
  */
}


/**
  * @brief  This function extracts binary data from a base64 representation.
  * @param  input64 : pointer on the input data in base64.
  * @param  binary : returned binary data.
  * @return Updated pointer on the input64 buffer (normal increment is 4 base64 char for 3 bytes of binary data, but special char are ignored)
*/
static uint8_t* from_base64(uint8_t *input64, uint32_t* binary)
{
  /* Read 4 base64 character & translate them into 3 bytes binary */ 
  *binary = 0;
  uint8_t base64_6bits;
  int i = 0;
  while(i<4)
  {

    if((*input64 >= 'A') && (*input64 <= 'Z'))
    {
      base64_6bits = *(input64++) - 'A';
    } else if ((*input64 >= 'a') && (*input64 <= 'z'))
    {
      base64_6bits = *(input64++) - 'a' + 26;
    } else if ((*input64 >= '0') && (*input64 <= '9')) {
      base64_6bits = *(input64++) - '0' + 52;
    } else if (*input64 == '+')
    {
      base64_6bits = 62;
      input64++;
    } else if (*input64 == '/')
    {
      base64_6bits = 63;  
      input64++;
    } else if (*input64 == '=') {
      base64_6bits = 0;
      input64++;
    } else {
      input64++;
      continue;
    }
    
    *binary |= base64_6bits << (6*(3-i));
    i++;
  }
  *binary = ((*binary & 0xFF0000)>>16) | ((*binary & 0xFF)<<16) | (*binary & 0xFF00);
  return input64;
}


/**
  * @brief  This function return the picture stored in a VCARD.
  * @param  pPayload : pointer on the payload data of the NDEF message.
  * @param  PayloadSize : number of data in the payload.
  * @param  pPict : Pointer on the dpicture buffer (must be big enough to contain the picture - warning: no check is done in the function).
 */
int NDEF_getVcardPicture( uint8_t* pPayload, uint32_t PayloadSize,  uint8_t* pPict )
{
  uint8_t* pSrcPict;

  
  /* Let's find the picture */
  pSrcPict = pPayload;
  while( memcmp( pSrcPict, JPEG, JPEG_STRING_SIZE ) && (pSrcPict < (pPayload + PayloadSize)) )
  {
    pSrcPict++;
  }
  while( (*pSrcPict != ':') && (pSrcPict < (pPayload + PayloadSize)) )
  {
    pSrcPict++;
  }
  /* Picture start at next char */
  pSrcPict++;

  
  /* Word found */
  while( memcmp( pSrcPict, LIMIT, LIMIT_STRING_SIZE ) && memcmp( pSrcPict, END, END_STRING_SIZE ) && (*pSrcPict!='\n') && (pSrcPict < (pPayload + PayloadSize) ) )
    {
      uint32_t binary24;
      pSrcPict = from_base64(pSrcPict, &binary24);
      /* copy the 3 bytes read from the base64 data */
      memcpy( pPict, &binary24, 3);
      pPict += 3;
      /* Ignore any line breaks */
      while (*pSrcPict == 0x0A)
      {
        pSrcPict++;
      }
    }
    
    if(pSrcPict >= (pPayload + PayloadSize))
    {
      /* problem when parsing the picture */
      return 1;
    } else {
      /* picture found */
      return 0;
    }
}

/**
  * @}
  */

