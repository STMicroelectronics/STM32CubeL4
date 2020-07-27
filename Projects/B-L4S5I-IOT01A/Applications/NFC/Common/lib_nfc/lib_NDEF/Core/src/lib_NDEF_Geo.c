/**
  ******************************************************************************
  * @file    lib_NDEF_Geo.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file help to manage NDEF file that represent geolocation.
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
#include "lib_NDEF_Geo.h"

/** @addtogroup NFC_libraries
  * @{
  * @brief  <b>This is the library used to manage the content of the TAG (data)
  *         But also the specific feature of the tag, for instance
  *         password, gpo... </b>
  */


/** @addtogroup libNFC_FORUM
  * @{
  * @brief  This part of the library manage data which follow NFC forum organisation.
  */

/**
  * @brief  This buffer contains the data send/received by TAG
  */
extern uint8_t NDEF_Buffer [];

/** @defgroup libGeo_Private_Functions
  * @{
  */

static void NDEF_FillGeoStruct( uint8_t* pPayload, uint32_t PayloadSize, sGeoInfo *pGeoStruct );
static void NDEF_ReadURI_Geo( sRecordInfo_t *pRecordStruct, sGeoInfo *pGeoStruct );

/**
  * @brief  This function fill Geo structure with information of NDEF message.
  * @param  pPayload : pointer on the payload data of the NDEF message.
  * @param  PayloadSize : number of data in the payload.
  * @param  pGeoStruct : pointer on the structure to fill.
  */
static void NDEF_FillGeoStruct( uint8_t* pPayload, uint32_t PayloadSize, sGeoInfo *pGeoStruct )
{
  uint8_t* pLastByteAdd, *pLook4Word, *pEndString;
  char* pKeyWord;
  uint32_t SizeOfKeyWord;
  pEndString = 0;

  pKeyWord = GEO_TYPE_STRING;
  SizeOfKeyWord = GEO_TYPE_STRING_LENGTH;

  /* First character force to NULL in case not matching found */
  *pGeoStruct->Latitude = 0;
  *pGeoStruct->Longitude = 0;

  /* Interresting information are stored before picture if any */
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
    while( memcmp( pEndString, URI_LATITUDE_END, URI_LATITUDE_END_LENGTH ) && (pEndString < pLastByteAdd) )
    {
      pEndString++;
    }
    if( pEndString != pLastByteAdd )
    {
      memcpy( pGeoStruct->Latitude, pLook4Word, pEndString-pLook4Word );
      /* add end of string character */
      pGeoStruct->Latitude[pEndString-pLook4Word] = 0;
    }
  }
  pEndString += URI_LATITUDE_END_LENGTH;
  pLook4Word = pEndString;

  memcpy( pGeoStruct->Longitude, pEndString, PayloadSize - (pEndString - pPayload) );
  /* add end of string character */
  pGeoStruct->Longitude[PayloadSize-(pEndString-pPayload)] = 0;

}

/**
  * @brief  This function read the geoloccation information and store data in a structure.
  * @param  pRecordStruct : Pointer on the record structure.
  * @param  pGeoStruct : pointer on the structure to fill.
  */
static void NDEF_ReadURI_Geo( sRecordInfo_t *pRecordStruct, sGeoInfo *pGeoStruct )
{
  uint8_t* pPayload;
  uint32_t PayloadSize;

  PayloadSize = pRecordStruct->PayloadLength;

  /* Read record header */
  pPayload = (uint8_t*)(pRecordStruct->PayloadBufferAdd);

  if( pRecordStruct->NDEF_Type == URI_GEO_TYPE )
    NDEF_FillGeoStruct( pPayload , PayloadSize, pGeoStruct );

}

/**
  * @}
  */

/** @defgroup libGeo_Public_Functions
  * @{
  * @brief  This file is used to manage geolocation (stored or loaded in tag)
  */ 

/**
  * @brief  This function read NDEF and retrieve Geo information if any.
  * @param  pRecordStruct : Pointer on the record structure.
  * @param  pGeoStruct : pointer on the structure to fill .
  * @retval NDEF_OK : Geolocation information from NDEF have been retrieved.
  * @retval NDEF_ERROR : not able to read NDEF from tag.
  */
uint16_t NDEF_ReadGeo( sRecordInfo_t *pRecordStruct, sGeoInfo *pGeoStruct )
{
  uint16_t status = NDEF_ERROR;
  sRecordInfo_t *pSPRecordStruct;
  uint32_t PayloadSize, RecordPosition;
  uint8_t* pData;

  if( pRecordStruct->NDEF_Type == URI_GEO_TYPE )
  {
    NDEF_ReadURI_Geo( pRecordStruct, pGeoStruct );
    status = NDEF_OK;
  }
  else if( pRecordStruct->NDEF_Type == SMARTPOSTER_TYPE )
  {
    for( RecordPosition = 0; RecordPosition < pRecordStruct->NbOfRecordInSPPayload; RecordPosition++ )
    {
      pSPRecordStruct = pRecordStruct->SPRecordStructAdd[RecordPosition];
      if( pSPRecordStruct->NDEF_Type == URI_GEO_TYPE )
      {
        NDEF_ReadURI_Geo( pSPRecordStruct, pGeoStruct );
        status = NDEF_OK;
      }
      if( pSPRecordStruct->NDEF_Type == TEXT_TYPE )
      {
        PayloadSize = pSPRecordStruct->PayloadLength;

        /* The instruction content the UTF-8 language code that is not used here */
        pData = (uint8_t*)pSPRecordStruct->PayloadBufferAdd;
        PayloadSize -= *pData + 1; /* remove not usefull data */
        pData += *pData + 1; /* set pointer on usefull data */

        memcpy( pGeoStruct->Information, pData, PayloadSize );
        /* add end of string character */
        pGeoStruct->Information[PayloadSize] = 0;
      }
    }
  }

  return status;
}

/**
  * @brief  This function write the NDEF file with the geolocation data given in the structure.
  * @param  pGeoStruct : pointer on structure that contain the geolocation information.
  * @retval NDEF_OK : the function is succesful.
  * @retval NDEF_ERROR_MEMORY_INTERNAL : Cannot write to tag.
  * @retval NDEF_ERROR_NOT_FORMATED : CCFile data not supported or not present.
  * @retval NDEF_ERROR_MEMORY_TAG : Size not compatible with memory.
  * @retval NDEF_ERROR_LOCKED : Tag locked, cannot be write.
  */
uint16_t NDEF_WriteGeo( sGeoInfo *pGeoStruct )
{
  uint16_t status = NDEF_ERROR,Offset = 0;

  NDEF_PrepareGeoMessage( pGeoStruct, NDEF_Buffer, &Offset );

  status = NfcTag_WriteNDEF( Offset, NDEF_Buffer );

  return status;
}

/**
  * @brief  This function write the NDEF file with the geolocation data given in the structure.
  * @param  pGeoStruct : pointer on structure that contain the geolocation information.
  * @param  pNDEFMessage : pointer on the NDEF message.
  * @param  size : to store the size of the NDEF message generated.
  */
void NDEF_PrepareGeoMessage( sGeoInfo *pGeoStruct, uint8_t *pNDEFMessage, uint16_t *size )
{
  uint16_t Offset = 0;
  uint32_t geoSize = 0;
  uint32_t infoSize = 0;
  uint32_t totalSize = 0;

  /* GEO is an URI but can be included in a smart poster to add text to give instruction to user for instance */

  /* GEO (smart poster) Record Header */
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

  /* GEO : 1+geo:+latitude+1+longitude */
  geoSize = 1 + GEO_TYPE_STRING_LENGTH + strlen(pGeoStruct->Latitude) + URI_LATITUDE_END_LENGTH + strlen(pGeoStruct->Longitude);

  /* Check if a Smart poster is needed */
  if( pGeoStruct->Information[0] != '\0' )
  {
    /* Info : 1+2+info */
    infoSize = 1 + ISO_ENGLISH_CODE_STRING_LENGTH + strlen(pGeoStruct->Information);
    /* Total */
    totalSize = 4 + geoSize + 4 + infoSize;
    if( geoSize > 255 ) totalSize += 3; /* Normal Geo size */
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

  /* GEO header */
  pNDEFMessage[Offset] = 0x81;
  if( geoSize < 256 ) pNDEFMessage[Offset] |= 0x10;                      // Set the SR bit
  if( pGeoStruct->Information[0] == '\0' ) pNDEFMessage[Offset] |= 0x40; // Set the ME bit
  Offset++;

  pNDEFMessage[Offset++] = URI_TYPE_STRING_LENGTH;
  if( geoSize > 255 )
  {
    pNDEFMessage[Offset++] = (geoSize & 0xFF000000) >> 24;
    pNDEFMessage[Offset++] = (geoSize & 0x00FF0000) >> 16;
    pNDEFMessage[Offset++] = (geoSize & 0x0000FF00) >> 8;
    pNDEFMessage[Offset++] = geoSize & 0x000000FF;
  }
  else
  {
    pNDEFMessage[Offset++] = (uint8_t)geoSize;
  }
  memcpy( &pNDEFMessage[Offset], URI_TYPE_STRING, URI_TYPE_STRING_LENGTH );
  Offset += URI_TYPE_STRING_LENGTH;

  /* GEO payload */
  pNDEFMessage[Offset++] = URI_ID_0x00; /* URI identifier no abbreviation */
  memcpy( &pNDEFMessage[Offset], GEO_TYPE_STRING, GEO_TYPE_STRING_LENGTH );
  Offset += GEO_TYPE_STRING_LENGTH;
  memcpy( &pNDEFMessage[Offset], pGeoStruct->Latitude, strlen(pGeoStruct->Latitude) );
  Offset += strlen( pGeoStruct->Latitude );
  memcpy( &pNDEFMessage[Offset], URI_LATITUDE_END, URI_LATITUDE_END_LENGTH );
  Offset += URI_LATITUDE_END_LENGTH;
  memcpy( &pNDEFMessage[Offset], pGeoStruct->Longitude, strlen(pGeoStruct->Longitude) );
  Offset += strlen( pGeoStruct->Longitude );

  /* Information header */
  if( pGeoStruct->Information[0] != '\0' )
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
    memcpy( &pNDEFMessage[Offset], pGeoStruct->Information, strlen(pGeoStruct->Information) );
    Offset += strlen( pGeoStruct->Information );
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
