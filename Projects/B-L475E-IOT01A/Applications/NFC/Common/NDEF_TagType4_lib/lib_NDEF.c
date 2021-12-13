/**
  ******************************************************************************
  * @file    lib_NDEF.c
  * @author  MMY Application Team
  * @brief   This file help to manage NDEF file, to parse and identify them.
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

/** @addtogroup NFC_libraries
 *  @{
 */
 
/** @addtogroup TAGTYPE4_NDEF_COMMON
 *  @{
 *  @brief  <b>This is the library used to manage the content of the TAG (data)
 *          But also the specific feature of the tag, for instance
 *          password, gpo... </b>
 */


/** @defgroup TAGTYPE4_NDEF_COMMON_Private_FunctionsPrototypes NDEF COMMON Private FunctionsPrototypes 
  * @{
  */
static uint16_t NDEF_IsNDEFPresent ( void );
static uint16_t NDEF_ParseRecordHeader ( sRecordInfo *pRecordStruct );
static void NDEF_ParseWellKnownType ( sRecordInfo *pRecordStruct );
static void NDEF_ParseMediaType ( sRecordInfo *pRecordStruct );
static void NDEF_ParseForumExternalType ( sRecordInfo *pRecordStruct );
static void NDEF_ParseURI(sRecordInfo *pRecordStruct);
static void NDEF_ParseSP(sRecordInfo *pRecordStruct);
static uint16_t NDEF_IdentifySPRecord ( sRecordInfo *pRecordStruct, uint8_t* pPayload );

/**
  * @}
  */

/** @defgroup TAGTYPE4_NDEF_COMMON_Private_Variables NDEF COMMON Private Variables 
  * @{
  */
  
/* In case of smart Poster composed with different record, 3 records supported so far */
sRecordInfo SPRecordStruct1, SPRecordStruct2, SPRecordStruct3;
void *SPRecordStructAdd[SP_MAX_RECORD] = { (void *)&SPRecordStruct1, (void *)&SPRecordStruct2, (void *)&SPRecordStruct3 };

/**
  * @}
  */

/** @defgroup TAGTYPE4_NDEF_COMMON_Private_Functions  NDEF COMMON Private Functions  
  * @{
  */
  
/**
  * @brief  This fonction check that the tag contain a NDEF message
  * @param  None :
  * @retval SUCCESS : There is a NDEF file stored in tag
  * @retval ERROR : No NDEF in the tag. 
  */
static uint16_t NDEF_IsNDEFPresent ( void )
{
  uint16_t FileSize;
  uint8_t uM24SR_NDEFHeader [0x2];
  
  /* Check NDEF existence */
  NFC_TT4_ReadData ( NDEF_SIZE_OFFSET , 2 , uM24SR_NDEFHeader);
  FileSize = (uint16_t) ((uM24SR_NDEFHeader[0x00]<<8) | uM24SR_NDEFHeader[0x01]);
  
  if( FileSize != 0)
    return NDEF_SUCCESS;
  else
    return NDEF_ERROR;  
}

/**
  * @brief  This fonction identify the type of record
  * @param  pRecordStruct : pointer on the record structure to fill
  * @param  pPayload : pointer on the payload
  * @retval Status : Status of the operation.
  */
static uint16_t NDEF_IdentifySPRecord ( sRecordInfo *pRecordStruct, uint8_t* pPayload )
{
  uint16_t status = NDEF_ERROR;
  uint16_t SizeOfRecordHeader, TypeNbByte, PayloadLengthField, IDLengthField, IDNbByte;
  
  /* Is ID length field present */
  if( (*pPayload)&IL_Mask)
  {
    IDLengthField = ID_LENGTH_FIELD;
  }
  else
  {
    IDLengthField = 0;
  }
  
  /* it's a SR */
  if( (*pPayload)&SR_Mask)
  {
    TypeNbByte = pPayload[1];
    PayloadLengthField = 1;
    if( IDLengthField == ID_LENGTH_FIELD)
      IDNbByte = pPayload[3];
    else
      IDNbByte = 0;
  }
  else
  {
    TypeNbByte = pPayload[1];
    PayloadLengthField = 4;
    if( IDLengthField == ID_LENGTH_FIELD)
      IDNbByte = pPayload[6];
    else
      IDNbByte = 0;
  }
  
  SizeOfRecordHeader = RECORD_FLAG_FIELD + TYPE_LENGTH_FIELD + PayloadLengthField + IDLengthField + TypeNbByte + IDNbByte;
  
  /* it's a SR */
  if( pPayload[0]&SR_Mask)
  {
    pRecordStruct->RecordFlags = pPayload[0];
    pRecordStruct->TypeLength = TypeNbByte;
    pRecordStruct->PayloadLength3 = 0;
    pRecordStruct->PayloadLength2 = 0;
    pRecordStruct->PayloadLength1 = 0;
    pRecordStruct->PayloadLength0 = pPayload[2];
    pRecordStruct->IDLength = IDNbByte;
    memcpy(pRecordStruct->Type, &pPayload[3+IDNbByte] , TypeNbByte);
    memcpy(pRecordStruct->ID, &pPayload[3+IDNbByte+TypeNbByte] , IDNbByte);
    pRecordStruct->PayloadOffset = SizeOfRecordHeader;
  }
  else
  {
    pRecordStruct->RecordFlags = pPayload[0];
    pRecordStruct->TypeLength = TypeNbByte;
    pRecordStruct->PayloadLength3 = pPayload[2];
    pRecordStruct->PayloadLength2 = pPayload[3];
    pRecordStruct->PayloadLength1 = pPayload[4];
    pRecordStruct->PayloadLength0 = pPayload[5];
    pRecordStruct->IDLength = IDNbByte;
    memcpy(pRecordStruct->Type, &pPayload[6+IDNbByte] , TypeNbByte);
    memcpy(pRecordStruct->ID, &pPayload[6+IDNbByte+TypeNbByte] , IDNbByte);
    pRecordStruct->PayloadOffset = SizeOfRecordHeader;
  }
  
  pRecordStruct->PayloadBufferAdd = (uint32_t)(pPayload+SizeOfRecordHeader);
  
  status = NDEF_ParseRecordHeader(pRecordStruct);
  
  return status;
}

/**
  * @brief  This fonction parse the record header and dispatch regarding TNF value
  * @param  pRecordStruct : pointer on the record structure to fill
  * @retval SUCCESS : record identified and structure filled
  * @retval ERROR : Not supported 
  */
static uint16_t NDEF_ParseRecordHeader ( sRecordInfo *pRecordStruct )
{ 
  uint16_t  status = NDEF_SUCCESS;
  
  switch( (pRecordStruct->RecordFlags&TNF_Mask))
  {
  case TNF_WellKnown:
    NDEF_ParseWellKnownType ( pRecordStruct );
    break;      
    
  case TNF_MediaType:
    NDEF_ParseMediaType ( pRecordStruct );
    break;
    
  case TNF_NFCForumExternal:
    NDEF_ParseForumExternalType ( pRecordStruct);
    break;
    
  default:
    /* currently not supported or unknown*/
    pRecordStruct->NDEF_Type = UNKNOWN_TYPE;
    status = NDEF_ERROR;
  }
  return status;
}  

/**
  * @brief  This fonction parse the Well Known type record
  * @param  pRecordStruct : pointer on the record structure to fill
  * @retval None 
  */
static void NDEF_ParseWellKnownType ( sRecordInfo *pRecordStruct )
{
  uint8_t* pPayload;
  
  pPayload = (uint8_t*)(pRecordStruct->PayloadBufferAdd);
  
  if( !memcmp( &(pRecordStruct->Type), SMART_POSTER_TYPE_STRING, pRecordStruct->TypeLength))
  { 
    /* special case where  we have to parse others records */
    pRecordStruct->NDEF_Type = SMARTPOSTER_TYPE;
    NDEF_ParseSP(pRecordStruct);
  }
  
  else if( !memcmp( &(pRecordStruct->Type), URI_TYPE_STRING, pRecordStruct->TypeLength))
  { 
    /* it's an URI Type check if it's an URL or SMS or ... */
    /* check identifier */
    if( *pPayload == URI_ID_0x00) 
    {  
      NDEF_ParseURI(pRecordStruct);
    }
    else if ( *pPayload > URI_ID_0x00 && *pPayload < URI_RFU ) 
    {
      /* email special case  */
      if ( *pPayload == (uint8_t) URI_ID_0x06)
      {
        pRecordStruct->NDEF_Type = URI_EMAIL_TYPE;
      }
      else
      {
        pRecordStruct->NDEF_Type = WELL_KNOWN_ABRIDGED_URI_TYPE;
      }
    }
    else
    {  
      pRecordStruct->NDEF_Type = UNKNOWN_TYPE;
    }
  }    
  
  else if( !memcmp( &(pRecordStruct->Type), TEXT_TYPE_STRING, pRecordStruct->TypeLength))
  {
    pRecordStruct->NDEF_Type = TEXT_TYPE;
  }    
  else
    pRecordStruct->NDEF_Type = UNKNOWN_TYPE;
  
}

/**
  * @brief  This fonction parse the Media type record
  * @param  pRecordStruct : pointer on the record structure to fill
  * @retval None 
  */
static void NDEF_ParseMediaType ( sRecordInfo *pRecordStruct )
{  
  if( !memcmp( &(pRecordStruct->Type), VCARD_TYPE_STRING, pRecordStruct->TypeLength))
    pRecordStruct->NDEF_Type = VCARD_TYPE;
  else if( !memcmp( &(pRecordStruct->Type), XVCARD_TYPE_STRING, pRecordStruct->TypeLength))
    pRecordStruct->NDEF_Type = VCARD_TYPE;
  else
    pRecordStruct->NDEF_Type = UNKNOWN_TYPE;
}

/**
  * @brief  This fonction parse the Forum External type record
  * @param  pRecordStruct : pointer on the record structure to fill
  * @retval None 
  */
static void NDEF_ParseForumExternalType ( sRecordInfo *pRecordStruct )
{
  if( !memcmp( &(pRecordStruct->Type), M24SR_DISCOVERY_APP_STRING, pRecordStruct->TypeLength))
    pRecordStruct->NDEF_Type = M24SR_DISCOVERY_APP_TYPE;
  else
    pRecordStruct->NDEF_Type = UNKNOWN_TYPE;
}

/**
  * @brief  This fonction parse the URI type record
  * @param  pRecordStruct : pointer on the record structure to fill
  * @retval None 
  */
static void NDEF_ParseURI(sRecordInfo *pRecordStruct)
{
  uint8_t* pPayload;
  
  pPayload = (uint8_t*)(pRecordStruct->PayloadBufferAdd);
  pPayload ++; /* to skip URI identifier first URI payload byte */
  
  if( !memcmp( pPayload, SMS_TYPE_STRING, strlen(SMS_TYPE_STRING)) )
  { 
    pRecordStruct->NDEF_Type = URI_SMS_TYPE;
  }
  else if( !memcmp( pPayload, GEO_TYPE_STRING, strlen(GEO_TYPE_STRING)) )
  { 
    pRecordStruct->NDEF_Type = URI_GEO_TYPE;
  }
  else
    pRecordStruct->NDEF_Type = UNKNOWN_TYPE;
  
}

/**
  * @brief  This fonction parse the Smart Poster 
  * @param  pRecordStruct : pointer on the record structure to fill
  * @retval None 
  */
static void NDEF_ParseSP(sRecordInfo *pRecordStruct)
{
  uint8_t* pPayload;
  uint32_t PayloadSize = 0;
  uint32_t SPPayloadSize = 0;
  uint32_t OffsetInSPPayload =0;
  uint32_t RecordPosition = 0;
  sRecordInfo *pSPRecordStruct;
  
  /* initialize variable with size of the payload and poiter on data */
  PayloadSize = ((uint32_t)(pRecordStruct->PayloadLength3)<<24) | ((uint32_t)(pRecordStruct->PayloadLength2)<<16) |
    ((uint32_t)(pRecordStruct->PayloadLength1)<<8)  | pRecordStruct->PayloadLength0;
  
  pPayload = (uint8_t*)(pRecordStruct->PayloadBufferAdd);
  
  pSPRecordStruct = (sRecordInfo *)SPRecordStructAdd[0];
  
  /* Initailize the number of record find in the SP payload */
  pRecordStruct->NbOfRecordInSPPayload = 0;
  
  do
  {
    pSPRecordStruct = (sRecordInfo *)SPRecordStructAdd[RecordPosition];
    /* identify the 1st record in the SP payload */
    if( NDEF_IdentifySPRecord ( pSPRecordStruct, pPayload ))
    {
      /* store add of structure that will contain the other record information */
      pRecordStruct->NbOfRecordInSPPayload ++;
      pRecordStruct->SPRecordStructAdd[RecordPosition] = (void *) pSPRecordStruct;
      
      /* After SPRecord + First Record check if we are at the end of NDEF file */
      SPPayloadSize = ((uint32_t)(pSPRecordStruct->PayloadLength3)<<24) | ((uint32_t)(pSPRecordStruct->PayloadLength2)<<16) |
        ((uint32_t)(pSPRecordStruct->PayloadLength1)<<8)  | pSPRecordStruct->PayloadLength0;
      
      OffsetInSPPayload += pSPRecordStruct->PayloadOffset + SPPayloadSize;
      pPayload += OffsetInSPPayload;
      RecordPosition++;    
    }
  }
  while(  OffsetInSPPayload < PayloadSize && RecordPosition<SP_MAX_RECORD); /* there is another record */
  
}

/**
  * @}
  */

/** @defgroup libNDEF_Public_Functions
  * @{
  */ 


/**
  * @brief  This fonction identify the NDEF message stored in tag
  * @param  pRecordStruct : Structure to fill with record information
  * @param  pNDEF : pointer on the NDEF message data
  * @retval SUCCESS : record struct filled
  * @retval ERROR : record struct not updated
  */

uint16_t NDEF_IdentifyNDEF ( sRecordInfo *pRecordStruct, uint8_t* pNDEF )
{
  uint16_t status = NDEF_ERROR;
  uint16_t SizeOfRecordHeader, TypeNbByte, PayloadLengthField, IDLengthField, IDNbByte;
  uint32_t PayloadSize;
  
  /* check NDEF present */
  if(NDEF_IsNDEFPresent() != NDEF_SUCCESS)
  {
    return NDEF_ERROR;
  }
  
  /* Analyse record layout */
  NFC_TT4_ReadData ( FIRST_RECORD_OFFSET , 1 , pNDEF);
  
  /* Is ID length field present */
  if( (*pNDEF)&IL_Mask)
  {
    IDLengthField = ID_LENGTH_FIELD;
  }
  else
  {
    IDLengthField = 0;
  }
  
  /* it's a SR */
  if( (*pNDEF)&SR_Mask)
  {
    /* Analyse short record layout */
    NFC_TT4_ReadData ( FIRST_RECORD_OFFSET , 4 , pNDEF);
    TypeNbByte = pNDEF[1];
    PayloadLengthField = 1;
    if( IDLengthField == ID_LENGTH_FIELD)
      IDNbByte = pNDEF[3];
    else
      IDNbByte = 0;
  }
  else
  {
    /* Analyse normal record layout */
    NFC_TT4_ReadData ( FIRST_RECORD_OFFSET , 7 , pNDEF);
    TypeNbByte = pNDEF[1];
    PayloadLengthField = 4;
    if( IDLengthField == ID_LENGTH_FIELD)
      IDNbByte = pNDEF[6];
    else
      IDNbByte = 0;
  }
  
  SizeOfRecordHeader = RECORD_FLAG_FIELD + TYPE_LENGTH_FIELD + PayloadLengthField + IDLengthField + TypeNbByte + IDNbByte;
  
  /* Read record header */
  NFC_TT4_ReadData ( FIRST_RECORD_OFFSET , SizeOfRecordHeader , pNDEF);
  /* it's a SR */
  if( pNDEF[0]&SR_Mask)
  {
    pRecordStruct->RecordFlags = pNDEF[0];
    pRecordStruct->TypeLength = TypeNbByte;
    pRecordStruct->PayloadLength3 = 0;
    pRecordStruct->PayloadLength2 = 0;
    pRecordStruct->PayloadLength1 = 0;
    pRecordStruct->PayloadLength0 = pNDEF[2];
    pRecordStruct->IDLength = IDNbByte;
    memcpy(pRecordStruct->Type, &pNDEF[3+IDNbByte] , TypeNbByte);
    memcpy(pRecordStruct->ID, &pNDEF[3+IDNbByte+TypeNbByte] , IDNbByte);
    pRecordStruct->PayloadOffset = SizeOfRecordHeader;
  }
  else
  {
    pRecordStruct->RecordFlags = pNDEF[0];
    pRecordStruct->TypeLength = TypeNbByte;
    pRecordStruct->PayloadLength3 = pNDEF[2];
    pRecordStruct->PayloadLength2 = pNDEF[3];
    pRecordStruct->PayloadLength1 = pNDEF[4];
    pRecordStruct->PayloadLength0 = pNDEF[5];
    pRecordStruct->IDLength = IDNbByte;
    memcpy(pRecordStruct->Type, &pNDEF[6+IDNbByte] , TypeNbByte);
    memcpy(pRecordStruct->ID, &pNDEF[6+IDNbByte+TypeNbByte] , IDNbByte);
    pRecordStruct->PayloadOffset = SizeOfRecordHeader;
  }
  
  PayloadSize = ((uint32_t)(pRecordStruct->PayloadLength3)<<24) | ((uint32_t)(pRecordStruct->PayloadLength2)<<16) |
    ((uint32_t)(pRecordStruct->PayloadLength1)<<8)  | pRecordStruct->PayloadLength0;
  
  /* read Payload */
  status = NFC_TT4_ReadData ( (uint16_t)((FIRST_RECORD_OFFSET) + pRecordStruct->PayloadOffset) , PayloadSize , pNDEF);
  
  if( status != NFC_TT4_ACTION_COMPLETED)
    return status;
  else
    pRecordStruct->PayloadBufferAdd = (uint32_t)(pNDEF);
  
  NDEF_ParseRecordHeader(pRecordStruct);
  
  return NDEF_SUCCESS;
}  

/**
  * @brief  This fonction read the NDEF content of the TAG
  * @param  pNDEF : pointer on the buffer to store NDEF data
  * @retval SUCCESS : NDEF file data retrieve and store in the buffer
  * @retval ERROR : not able to read NDEF
  */
uint16_t NDEF_ReadNDEF( uint8_t* pNDEF)
{
  uint16_t status = NDEF_ERROR;
  uint16_t NDEF_Size = 0;
  
  status = NFC_TT4_ReadData( 0 , 2 , pNDEF);
  
  if( status == NFC_TT4_ACTION_COMPLETED)
  {
    NDEF_Size = (uint16_t) (*pNDEF << 8);
    NDEF_Size = NDEF_Size | (uint16_t) (*++pNDEF );
    
    status = NFC_TT4_ReadData( 0 , NDEF_Size+2 , --pNDEF);
  }
  
  if( status == NFC_TT4_ACTION_COMPLETED)
    return NDEF_SUCCESS;
  else
    return NDEF_ERROR;
  
}

/**
  * @brief  This fonction write the NDEF in the TAG
  * @param  pNDEF : pointer on the buffer containing the NDEF data
  * @retval SUCCESS : NDEF file data written in the tag
  * @retval ERROR : not able to store NDEF in tag
  */
uint16_t NDEF_WriteNDEF( uint8_t *pNDEF)
{
  uint16_t status = NDEF_ERROR;
  uint16_t NDEF_Size = 0;
  
  NDEF_Size = (uint16_t) (*pNDEF << 8);
  NDEF_Size = NDEF_Size | (uint16_t) (*++pNDEF );
  
  status = NFC_TT4_WriteData( 0 , NDEF_Size+2 , --pNDEF);
  
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


