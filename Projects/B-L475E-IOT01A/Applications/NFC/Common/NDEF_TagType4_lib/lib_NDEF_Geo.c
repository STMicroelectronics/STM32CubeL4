/**
  ******************************************************************************
  * @file    lib_NDEF_Geo.c
  * @author  MMY Application Team
  * @brief   This file help to manage NDEF file that represent geolocation.
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
#include "lib_NDEF_Geo.h"

/** @addtogroup NFC_libraries
 *  @{
 */


/** @addtogroup TAGTYPE4_NDEF_GEO
  * @{
  */

/**
 * @brief  This buffer contains the data send/received by TAG
 */
extern uint8_t NDEF_Buffer [NFC_TT4_NDEF_MAX_SIZE];


/** @defgroup TAGTYPE4_NDEF_GEO_Private_FunctionsPrototypes NDEF GEO Private FunctionsPrototypes
  * @{
  */
static void NDEF_FillGeoStruct( uint8_t* pPayload, uint32_t PayloadSize, sGeoInfo *pGeoStruct);
static void NDEF_ReadURI_Geo ( sRecordInfo *pRecordStruct, sGeoInfo *pGeoStruct );
/**
  * @}
  */ 
  
/** @defgroup TAGTYPE4_NDEF_GEO_Private_Functions NDEF GEO Private Functions
  * @{
  */
  
/**
  * @brief  This fonction fill Geo structure with information of NDEF message
  * @param  pPayload : pointer on the payload data of the NDEF message
  * @param  PayloadSize : number of data in the payload
  * @param  pGeoStruct : pointer on the structure to fill
  * @retval NONE 
  */
static void NDEF_FillGeoStruct( uint8_t* pPayload, uint32_t PayloadSize, sGeoInfo *pGeoStruct)
{
  uint8_t* pLastByteAdd, *pLook4Word, *pEndString ;
  char* pKeyWord;
  uint32_t SizeOfKeyWord;
  
  pKeyWord = GEO_TYPE_STRING;
  SizeOfKeyWord = GEO_TYPE_STRING_LENGTH;
  
  /* First charactere force to NULL in case not matching found */
  *pGeoStruct->Latitude = 0;
  *pGeoStruct->Longitude = 0;
  
  /* Interresting information are stored before picture if any */
  /* Moreover picture is not used in this demonstration SW */  
  pLastByteAdd = (uint8_t*)(pPayload + PayloadSize);
  
  pLook4Word = pPayload;
  while( memcmp( pLook4Word, pKeyWord, SizeOfKeyWord) && pLook4Word<pLastByteAdd )
  {
    pLook4Word++;
  }
  
  pEndString = pLook4Word;
  
  /* Retrieve phone number */
  if( pLook4Word != pLastByteAdd)
  {  
    pLook4Word += SizeOfKeyWord;
    while( memcmp( pEndString, URI_LATITUDE_END, URI_LATITUDE_END_LENGTH) && pEndString<pLastByteAdd )
    {
      pEndString++;
    }
    if( pEndString != pLastByteAdd)
    {
      memcpy( pGeoStruct->Latitude, pLook4Word, pEndString-pLook4Word);
      /* add end of string charactere */
      pGeoStruct->Latitude[pEndString-pLook4Word] = 0;  
    }
  }  
  pEndString += URI_LATITUDE_END_LENGTH;
  pLook4Word = pEndString;
  
  memcpy( pGeoStruct->Longitude, pEndString, PayloadSize-(pEndString-pPayload));
  /* add end of string charactere */
  pGeoStruct->Longitude[PayloadSize-(pEndString-pPayload)] = 0;    
  
}

/**
  * @brief  This fonction read the geoloccation information and store data in a structure
  * @param  pRecordStruct : Pointer on the record structure
  * @param  pGeoStruct : pointer on the structure to fill
  * @retval NONE 
  */
static void NDEF_ReadURI_Geo ( sRecordInfo *pRecordStruct, sGeoInfo *pGeoStruct )
{
  uint8_t* pPayload;
  uint32_t PayloadSize;
  
  PayloadSize = ((uint32_t)(pRecordStruct->PayloadLength3)<<24) | ((uint32_t)(pRecordStruct->PayloadLength2)<<16) |
    ((uint32_t)(pRecordStruct->PayloadLength1)<<8)  | pRecordStruct->PayloadLength0;
  
  /* Read record header */
  pPayload = (uint8_t*)(pRecordStruct->PayloadBufferAdd);
  
  if( pRecordStruct->NDEF_Type == URI_GEO_TYPE)
    NDEF_FillGeoStruct(pPayload , PayloadSize, pGeoStruct);
  
}

/**
  * @}
  */

/** @defgroup TAGTYPE4_NDEF_GEO_Public_Functions NDEF GEO Private Functions
  * @{
  *  @brief  This file is used to manage geolocation (stored or loaded in tag)
  */ 

/**
  * @brief  This fonction read NDEF and retrieve Geo information if any
  * @param  pRecordStruct : Pointer on the record structure
  * @param  pGeoStruct : pointer on the structure to fill 
  * @retval SUCCESS : Geolocation information from NDEF have been retrieved
  * @retval ERROR : Not able to retrieve geolocation information
  */
uint16_t NDEF_ReadGeo(sRecordInfo *pRecordStruct, sGeoInfo *pGeoStruct)
{
  uint16_t status = NDEF_ERROR;
  uint16_t FileId=0;
  sRecordInfo *pSPRecordStruct;  
  uint32_t PayloadSize, RecordPosition;
  uint8_t* pData;
  
  if( pRecordStruct->NDEF_Type == URI_GEO_TYPE )
  {  
    NDEF_ReadURI_Geo(pRecordStruct, pGeoStruct );
    status = NDEF_SUCCESS;
  }
  else if( pRecordStruct->NDEF_Type == SMARTPOSTER_TYPE)
  {
    for (RecordPosition = 0; RecordPosition<pRecordStruct->NbOfRecordInSPPayload; RecordPosition++)
    {
      pSPRecordStruct = (sRecordInfo *)(pRecordStruct->SPRecordStructAdd[RecordPosition]);
      if(pSPRecordStruct->NDEF_Type == URI_GEO_TYPE )
      {
        NDEF_ReadURI_Geo(pSPRecordStruct, pGeoStruct );
        status = NDEF_SUCCESS;
      }
      if(pSPRecordStruct->NDEF_Type == TEXT_TYPE )
      {
        PayloadSize = ((uint32_t)(pSPRecordStruct->PayloadLength3)<<24) | ((uint32_t)(pSPRecordStruct->PayloadLength2)<<16) |
          ((uint32_t)(pSPRecordStruct->PayloadLength1)<<8)  | pSPRecordStruct->PayloadLength0;
        
        /* The instruction content the UTF-8 language code that is not used here */
        pData = (uint8_t*)pSPRecordStruct->PayloadBufferAdd;
        PayloadSize -= *pData+1; /* remove not usefull data */
        pData += *pData+1; /* set pointer on usefull data */
        
        memcpy(pGeoStruct->Information, pData, PayloadSize);
        /* add end of string charactere */
        pGeoStruct->Information[PayloadSize] = 0;    
      }
    }
  }
  
  NFC_TT4_CloseNDEFSession(FileId);
  
  return status;
}

/**
  * @brief  This fonction write the NDEF file with the geolocation data given in the structure
  * @param  pGeoStruct : pointer on structure that contain the geolocation information
  * @retval SUCCESS : the function is succesful
  * @retval ERROR : Not able to store NDEF file inside tag.
  */
uint16_t NDEF_WriteGeo ( sGeoInfo *pGeoStruct )
{
  uint16_t status = NDEF_ERROR;
  uint32_t geoSize, infoSize, totalSize, Offset = 0;
  
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
  
  NDEF_Buffer[0] = 0;
  NDEF_Buffer[1] = 0;
  Offset = FIRST_RECORD_OFFSET;
  infoSize = 0;
  
  /* GEO : 1+geo:+latitude+1+longitude */
  geoSize = 1+GEO_TYPE_STRING_LENGTH+strlen(pGeoStruct->Latitude)+URI_LATITUDE_END_LENGTH+strlen(pGeoStruct->Longitude);
  
  /* Check if a Smart poster is needed */
  if (pGeoStruct->Information[0] != '\0')
  {
    /* Info : 1+2+info */
    infoSize = 1+ISO_ENGLISH_CODE_STRING_LENGTH+strlen(pGeoStruct->Information);
    /* Total */
    totalSize = 4+geoSize+4+infoSize;
    if (geoSize > 255) totalSize+=3; /* Normal Geo size */
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
  
  /* GEO header */
  NDEF_Buffer[Offset] = 0x81;
  if (geoSize < 256) NDEF_Buffer[Offset] |= 0x10;                      /* Set the SR bit */
  if (pGeoStruct->Information[0] == '\0') NDEF_Buffer[Offset] |= 0x40; /* Set the ME bit */
  Offset++;
  
  NDEF_Buffer[Offset++] = URI_TYPE_STRING_LENGTH;
  if (geoSize > 255)
  {
    NDEF_Buffer[Offset++] = (geoSize & 0xFF000000)>>24;
    NDEF_Buffer[Offset++] = (geoSize & 0x00FF0000)>>16;
    NDEF_Buffer[Offset++] = (geoSize & 0x0000FF00)>>8;
    NDEF_Buffer[Offset++] = (geoSize & 0x000000FF);
  }
  else
  {
    NDEF_Buffer[Offset++] = (uint8_t)geoSize;
  }
  memcpy(&NDEF_Buffer[Offset], URI_TYPE_STRING, URI_TYPE_STRING_LENGTH);
  Offset+=URI_TYPE_STRING_LENGTH;
  
  /* GEO payload */
  NDEF_Buffer[Offset++] = URI_ID_0x00; /* URI identifier no abbreviation */
  memcpy(&NDEF_Buffer[Offset], GEO_TYPE_STRING, GEO_TYPE_STRING_LENGTH);
  Offset += GEO_TYPE_STRING_LENGTH;
  memcpy( &NDEF_Buffer[Offset], pGeoStruct->Latitude,strlen(pGeoStruct->Latitude));
  Offset += strlen(pGeoStruct->Latitude);
  memcpy( &NDEF_Buffer[Offset], URI_LATITUDE_END,URI_LATITUDE_END_LENGTH);
  Offset += URI_LATITUDE_END_LENGTH;
  memcpy( &NDEF_Buffer[Offset], pGeoStruct->Longitude, strlen(pGeoStruct->Longitude));
  Offset += strlen(pGeoStruct->Longitude);
  
  /* Information header */
  if (pGeoStruct->Information[0] != '\0')
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
    memcpy( &NDEF_Buffer[Offset], pGeoStruct->Information,strlen(pGeoStruct->Information));
    Offset += strlen(pGeoStruct->Information);
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



