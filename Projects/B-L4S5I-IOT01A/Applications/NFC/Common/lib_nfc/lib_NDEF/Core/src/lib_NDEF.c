/**
  ******************************************************************************
  * @file    lib_NDEF.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file help to manage NDEF file, to parse and identify them.
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
#include "lib_NDEF.h"
#include "lib_NDEF_Bluetooth.h"
#include "lib_NDEF_Handover.h"
#include "lib_NDEF_Wifi.h"

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

/** @defgroup libNDEF_Private_Functions
  * @{
  */


static uint16_t NDEF_IsNDEFPresent( void );
static uint16_t NDEF_ParseRecordHeader( sRecordInfo_t *pRecordStruct );
static void NDEF_ParseWellKnownType( sRecordInfo_t *pRecordStruct );
static void NDEF_ParseMediaType( sRecordInfo_t *pRecordStruct );
static void NDEF_ParseForumExternalType( sRecordInfo_t *pRecordStruct );
static void NDEF_ParseURI( sRecordInfo_t *pRecordStruct );
static void NDEF_ParseSP( sRecordInfo_t *pRecordStruct );
static uint16_t NDEF_IdentifySPRecord( sRecordInfo_t *pRecordStruct, uint8_t* pPayload );

/** @brief This buffer is used to store the data sent/received by the TAG. */
uint8_t NDEF_Buffer [NDEF_MAX_SIZE];
/** @brief Size of the buffer used to build the NDEF messages. */
uint32_t NDEF_Buffer_size = NDEF_MAX_SIZE;
/** @brief This buffer is used when it's required to prepare a record before adding it to the NDEF_Buffer. */
uint8_t NDEF_Record_Buffer [NDEF_RECORD_MAX_SIZE];
/** @brief Size of the buffer used when a record has to be prepared. */
uint32_t NDEF_Record_Buffer_size = NDEF_RECORD_MAX_SIZE;

/* In case of smart Poster composed with different record, 3 records supported so far */
sRecordInfo_t SPRecordStruct1, SPRecordStruct2, SPRecordStruct3, SPRecordStruct4;
sRecordInfo_t *SPRecordStructAdd[SP_MAX_RECORD] = { &SPRecordStruct1, &SPRecordStruct2, &SPRecordStruct3, &SPRecordStruct4 };

/**
  * @brief  This function checks that the tag contain a NDEF message.
  * @retval NDEF_OK : There is a NDEF file stored in tag.
  * @retval NDEF_ERROR : No NDEF in the tag.
  */
static uint16_t NDEF_IsNDEFPresent( void )
{
  uint16_t FileSize;

  /* Check NDEF existence */
  NfcTag_GetLength(&FileSize);
  
  if( FileSize != 0 )
    return NDEF_OK;
  else
    return NDEF_ERROR;
}

/**
  * @brief  This function identify the type of record.
  * @param  pRecordStruct : pointer on the record structure to fill.
  * @param  pPayload : pointer on the payload.
  * @retval Status : Status of the operation.
  */
static uint16_t NDEF_IdentifySPRecord( sRecordInfo_t *pRecordStruct, uint8_t* pPayload )
{
  uint16_t status = NDEF_ERROR;
  uint16_t SizeOfRecordHeader, TypeNbByte, PayloadLengthField, IDLengthField, IDNbByte;

  /* Is ID length field present */
  if( (*pPayload) & IL_Mask )
  {
    IDLengthField = ID_LENGTH_FIELD;
  }
  else
  {
    IDLengthField = 0;
  }

  /* it's a SR */
  if( (*pPayload) & SR_Mask )
  {
    TypeNbByte = pPayload[1];
    PayloadLengthField = 1;
    if( IDLengthField == ID_LENGTH_FIELD )
      IDNbByte = pPayload[3];
    else
      IDNbByte = 0;
  }
  else
  {
    TypeNbByte = pPayload[1];
    PayloadLengthField = 4;
    if( IDLengthField == ID_LENGTH_FIELD )
      IDNbByte = pPayload[6];
    else
      IDNbByte = 0;
  }

  SizeOfRecordHeader = RECORD_FLAG_FIELD + TYPE_LENGTH_FIELD + PayloadLengthField + IDLengthField + TypeNbByte + IDNbByte;

  /* it's a SR */
  if( pPayload[0] & SR_Mask )
  {
    pRecordStruct->RecordFlags = pPayload[0];
    pRecordStruct->TypeLength = TypeNbByte;
    pRecordStruct->PayloadLength = pPayload[2];
    pRecordStruct->IDLength = IDNbByte;
    memcpy( pRecordStruct->Type, &pPayload[3+IDLengthField], TypeNbByte );
    memcpy( pRecordStruct->ID, &pPayload[3+IDLengthField+TypeNbByte], IDNbByte );
    pRecordStruct->PayloadOffset = SizeOfRecordHeader;
  }
  else
  {
    pRecordStruct->RecordFlags = pPayload[0];
    pRecordStruct->TypeLength = TypeNbByte;
    pRecordStruct->PayloadLength = ( ((uint32_t)pPayload[2]) << 24 ) |
                                   ( ((uint32_t)pPayload[3]) << 16 ) |
                                   ( ((uint32_t)pPayload[4]) << 8 )
                                    | pPayload[5] ;
    pRecordStruct->IDLength = IDNbByte;
    memcpy( pRecordStruct->Type, &pPayload[6+IDLengthField], TypeNbByte );
    memcpy( pRecordStruct->ID, &pPayload[6+IDLengthField+TypeNbByte], IDNbByte );
    pRecordStruct->PayloadOffset = SizeOfRecordHeader;
  }

  pRecordStruct->PayloadBufferAdd =  pPayload + SizeOfRecordHeader ;

  status = NDEF_ParseRecordHeader( pRecordStruct );

  return status;
}

/**
  * @brief  This function parse the record header and dispatch regarding TNF value.
  * @param  pRecordStruct : pointer on the record structure to fill.
  * @retval NDEF_OK : record identified and structure filled.
  * @retval NDEF_ERROR : Not supported.
  */
static uint16_t NDEF_ParseRecordHeader( sRecordInfo_t *pRecordStruct )
{
  uint16_t status = NDEF_OK;

  switch( (pRecordStruct->RecordFlags & TNF_Mask) )
  {
    case TNF_WellKnown:
      NDEF_ParseWellKnownType( pRecordStruct );
      break;

    case TNF_MediaType:
      NDEF_ParseMediaType( pRecordStruct );
      break;

    case TNF_NFCForumExternal:
      NDEF_ParseForumExternalType( pRecordStruct);
      break;

  default:
      /* currently not supported or unknown*/
      pRecordStruct->NDEF_Type = UNKNOWN_TYPE;
      status = NDEF_ERROR;
  }
  return status;
}

/**
  * @brief  This function parse the Well Known type record.
  * @param  pRecordStruct : pointer on the record structure to fill.
  */
static void NDEF_ParseWellKnownType( sRecordInfo_t *pRecordStruct )
{
  uint8_t* pPayload;

  pPayload = (uint8_t*)( pRecordStruct->PayloadBufferAdd );

  if( !memcmp( &(pRecordStruct->Type), SMART_POSTER_TYPE_STRING, pRecordStruct->TypeLength ) )
  { 
    /* special case where	we have to parse others records */
    pRecordStruct->NDEF_Type = SMARTPOSTER_TYPE;
    NDEF_ParseSP( pRecordStruct );
  }

  else if( !memcmp( &(pRecordStruct->Type), URI_TYPE_STRING, pRecordStruct->TypeLength ) )
  { 
    /* it's an URI Type check if it's an URL or SMS or ... */
    /* check identifier */
    if( *pPayload == URI_ID_0x00 ) 
    {
      NDEF_ParseURI( pRecordStruct );
    }
    else if( (*pPayload > URI_ID_0x00) && (*pPayload < URI_RFU) ) 
    {
      /* email special case  */
      if( *pPayload == (uint8_t) URI_ID_0x06 )
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

  else if( !memcmp( &(pRecordStruct->Type), TEXT_TYPE_STRING, pRecordStruct->TypeLength ) )
  {
    pRecordStruct->NDEF_Type = TEXT_TYPE;
  } else if ((!memcmp( &(pRecordStruct->Type), NDEF_HANDOVER_SELECT_TYPE_STR, pRecordStruct->TypeLength )) ||
            (!memcmp( &(pRecordStruct->Type), NDEF_HANDOVER_REQUEST_TYPE_STR, pRecordStruct->TypeLength ) ))
  {
     pRecordStruct->NDEF_Type = HANDOVER_TYPE;
  }
  else
    pRecordStruct->NDEF_Type = UNKNOWN_TYPE;
}

/**
  * @brief  This function parse the Media type record.
  * @param  pRecordStruct : pointer on the record structure to fill.
  */
static void NDEF_ParseMediaType( sRecordInfo_t *pRecordStruct )
{
  if( !memcmp( &(pRecordStruct->Type), VCARD_TYPE_STRING, pRecordStruct->TypeLength ) )
    pRecordStruct->NDEF_Type = VCARD_TYPE;
  else if( !memcmp( &(pRecordStruct->Type), XVCARD_TYPE_STRING, pRecordStruct->TypeLength ) )
    pRecordStruct->NDEF_Type = VCARD_TYPE;
  else if( !memcmp( &(pRecordStruct->Type), XVCARD2_TYPE_STRING, pRecordStruct->TypeLength ) )
    pRecordStruct->NDEF_Type = VCARD_TYPE;
  else if (!memcmp(&pRecordStruct->Type, NDEF_BLUETOOTH_BREDR_MIME_TYPE,  pRecordStruct->TypeLength))
    pRecordStruct->NDEF_Type = BT_TYPE;
  else if (!memcmp(&pRecordStruct->Type, NDEF_BLUETOOTH_BLE_MIME_TYPE,  pRecordStruct->TypeLength))
    pRecordStruct->NDEF_Type = BLE_TYPE;
  else if (!memcmp(&pRecordStruct->Type, WIFITOKEN_TYPE_STRING,  pRecordStruct->TypeLength))
    pRecordStruct->NDEF_Type = URI_WIFITOKEN_TYPE;
  else
    pRecordStruct->NDEF_Type = UNKNOWN_TYPE;
}

/**
  * @brief  This function parse the Forum External type record.
  * @param  pRecordStruct : pointer on the record structure to fill.
  */
static void NDEF_ParseForumExternalType( sRecordInfo_t *pRecordStruct )
{
  if( !memcmp( &(pRecordStruct->Type), M24SR_DISCOVERY_APP_STRING, pRecordStruct->TypeLength ) )
    pRecordStruct->NDEF_Type = M24SR_DISCOVERY_APP_TYPE;
  else
    pRecordStruct->NDEF_Type = UNKNOWN_TYPE;
}

/**
  * @brief  This function parse the URI type record.
  * @param  pRecordStruct : pointer on the record structure to fill.
  */
static void NDEF_ParseURI( sRecordInfo_t *pRecordStruct )
{
  uint8_t* pPayload;

  pPayload = (uint8_t*)( pRecordStruct->PayloadBufferAdd );
  pPayload++; /* to skip URI identifier first URI payload byte */

  if( !memcmp( pPayload, SMS_TYPE_STRING, strlen(SMS_TYPE_STRING) ) )
  {
    pRecordStruct->NDEF_Type = URI_SMS_TYPE;
  }
  else if( !memcmp( pPayload, GEO_TYPE_STRING, strlen(GEO_TYPE_STRING) ) )
  {
    pRecordStruct->NDEF_Type = URI_GEO_TYPE;
  }
  else
    pRecordStruct->NDEF_Type = UNKNOWN_TYPE;
}

/**
  * @brief  This function parse the Smart Poster.
  * @param  pRecordStruct : pointer on the record structure to fill.
  */
static void NDEF_ParseSP( sRecordInfo_t *pRecordStruct )
{
  uint8_t* pPayload;
  uint32_t PayloadSize = 0;
  uint32_t SPPayloadSize = 0;
  uint32_t OffsetInSPPayload = 0;
  uint32_t RecordPosition = 0;
  sRecordInfo_t *pSPRecordStruct;

  /* initialize variable with size of the payload and poiter on data */
  PayloadSize = pRecordStruct->PayloadLength;

  pPayload = (uint8_t*)( pRecordStruct->PayloadBufferAdd );

  pSPRecordStruct = SPRecordStructAdd[0];

  /* Initailize the number of record find in the SP payload */
  pRecordStruct->NbOfRecordInSPPayload = 0;

  do
  {
    pSPRecordStruct = SPRecordStructAdd[RecordPosition];
    /* identify the record in the SP payload */
    if( NDEF_IdentifySPRecord( pSPRecordStruct, pPayload ) == NDEF_OK )
    {
      /* store add of structure that will contain the other record information */
      pRecordStruct->NbOfRecordInSPPayload++;
      pRecordStruct->SPRecordStructAdd[RecordPosition] = pSPRecordStruct;

      /* After SPRecord + First Record check if we are at the end of NDEF file */
      SPPayloadSize = pSPRecordStruct->PayloadLength;

      OffsetInSPPayload += pSPRecordStruct->PayloadOffset + SPPayloadSize;
      pPayload += OffsetInSPPayload;
    }
    else /* Recommended Action Record for example */
    {
      SPPayloadSize = 0;
    }
    RecordPosition++;
  }
  while( (OffsetInSPPayload < PayloadSize) && RecordPosition<SP_MAX_RECORD); /* there is another record */
}

/**
  * @}
  */

/** @defgroup libNDEF_Public_Functions
  * @{
  */


/**
  * @brief  This function identify the NDEF message stored in tag.
  * @param  pRecordStruct : Structure to fill with record information.
  * @param  pNDEF : pointer on the NDEF message data.
  * @retval NDEF_OK : record struct filled.
  * @retval NDEF_ERROR : record struct not updated.
  */
uint16_t NDEF_IdentifyNDEF( sRecordInfo_t *pRecordStruct, uint8_t* pNDEF )
{
  uint16_t SizeOfRecordHeader, TypeNbByte, PayloadLengthField, IDLengthField, IDNbByte;

  /* check NDEF present */
  if( NDEF_IsNDEFPresent() != NDEF_OK )
  {
    return NDEF_ERROR;
  }

  /* Read the NDEF file */
  NfcTag_ReadNDEF( pNDEF );

  /* Is ID length field present */
  if( (*pNDEF) & IL_Mask )
  {
    IDLengthField = ID_LENGTH_FIELD;
  }
  else
  {
    IDLengthField = 0;
  }

  /* it's a SR */
  if( (*pNDEF) & SR_Mask )
  {
    /* Analyse short record layout */
    TypeNbByte = pNDEF[1];
    PayloadLengthField = 1;
    if( IDLengthField == ID_LENGTH_FIELD )
      IDNbByte = pNDEF[3];
    else
      IDNbByte = 0;
  }
  else
  {
    /* Analyse normal record layout */
    TypeNbByte = pNDEF[1];
    PayloadLengthField = 4;
    if( IDLengthField == ID_LENGTH_FIELD )
      IDNbByte = pNDEF[6];
    else
      IDNbByte = 0;
  }

  SizeOfRecordHeader = RECORD_FLAG_FIELD + TYPE_LENGTH_FIELD + PayloadLengthField + IDLengthField + TypeNbByte + IDNbByte;

  /* Read record header */
  /* it's a SR */
  if( pNDEF[0] & SR_Mask )
  {
    pRecordStruct->RecordFlags = pNDEF[0];
    pRecordStruct->TypeLength = TypeNbByte;
    pRecordStruct->PayloadLength = pNDEF[2];
    pRecordStruct->IDLength = IDNbByte;
    memcpy( pRecordStruct->Type, &pNDEF[3+IDLengthField], TypeNbByte );
    memcpy( pRecordStruct->ID, &pNDEF[3+IDLengthField+TypeNbByte], IDNbByte );
    pRecordStruct->PayloadOffset = SizeOfRecordHeader;
  }
  else
  {
    pRecordStruct->RecordFlags = pNDEF[0];
    pRecordStruct->TypeLength = TypeNbByte;
    pRecordStruct->PayloadLength = ( ((uint32_t)pNDEF[2]) << 24 ) |
                                   ( ((uint32_t)pNDEF[3]) << 16 ) |
                                   ( ((uint32_t)pNDEF[4]) << 8 )
                                    | pNDEF[5] ;
    pRecordStruct->IDLength = IDNbByte;
    memcpy( pRecordStruct->Type, &pNDEF[6+IDLengthField], TypeNbByte );
    memcpy( pRecordStruct->ID, &pNDEF[6+IDLengthField+TypeNbByte], IDNbByte );
    pRecordStruct->PayloadOffset = SizeOfRecordHeader;
  }

  pRecordStruct->PayloadBufferAdd = pNDEF + SizeOfRecordHeader;
  NDEF_ParseRecordHeader( pRecordStruct );

  return NDEF_OK;
}

/**
  * @brief  This function read the NDEF content of the TAG.
  * @param  pNDEF : pointer on the buffer to store NDEF data.
  * @retval NDEF_OK : NDEF file data retrieve and store in the buffer.
  * @retval NDEF_ERROR : not able to read NDEF from tag.
  * @retval NDEF_ERROR_MEMORY_INTERNAL : Cannot read tag.
  * @retval NDEF_ERROR_NOT_FORMATED : CCFile data not supported or not present.
  * @retval NDEF_ERROR_MEMORY_TAG : Size not compatible with memory.
  * @retval NDEF_ERROR_LOCKED : Tag locked, cannot be read.
  */
uint16_t NDEF_ReadNDEF( uint8_t* pNDEF )
{
   return NfcTag_ReadNDEF( pNDEF );
}



/**
  * @brief  This function read the NDEF size of the TAG.
  * @param  Size : pointer to a 16-bit Size (in bytes) to be returned.
  * @retval NDEF_OK : NDEF file data retrieve and store in the buffer.
  * @retval NDEF_ERROR : not able to read NDEF from tag.
  * @retval NDEF_ERROR_MEMORY_INTERNAL : Cannot read tag.
  * @retval NDEF_ERROR_NOT_FORMATED : CCFile data not supported or not present.
  * @retval NDEF_ERROR_MEMORY_TAG : Size not compatible with memory.
  * @retval NDEF_ERROR_LOCKED : Tag locked, cannot be read.
  */
uint16_t NDEF_getNDEFSize(uint16_t* Size)
{
  return NfcTag_GetLength(Size);
}
/**
  * @brief  This function write the NDEF in the TAG.
  * @param  pNDEF : pointer on the buffer containing the NDEF data.
  * @retval NDEF_OK : NDEF file data written in the tag.
  * @retval NDEF_ERROR : not able to store NDEF in tag.
  * @retval NDEF_ERROR_MEMORY_INTERNAL : Cannot write to tag.
  * @retval NDEF_ERROR_NOT_FORMATED : CCFile data not supported or not present.
  * @retval NDEF_ERROR_MEMORY_TAG : Size not compatible with memory.
  * @retval NDEF_ERROR_LOCKED : Tag locked, cannot be write.
  */
uint16_t NDEF_WriteNDEF( uint16_t NDEF_Size, uint8_t *pNDEF )
{
  return NfcTag_WriteNDEF( NDEF_Size, pNDEF );

}

/**
  * @brief  This function append the record to an the NDEF in the TAG.
  * @param  pRecord : pointer on the record with data to be written.
  * @retval NDEF_OK : NDEF file data written in the tag.
  * @retval NDEF_ERROR : not able to store NDEF in tag.
  * @retval NDEF_ERROR_MEMORY_INTERNAL : Cannot write to tag.
  * @retval NDEF_ERROR_NOT_FORMATED : CCFile data not supported or not present.
  * @retval NDEF_ERROR_MEMORY_TAG : Size not compatible with memory.
  * @retval NDEF_ERROR_LOCKED : Tag locked, cannot be write.
  */
uint16_t NDEF_AppendRecord(sRecordInfo_t  *Record )
{
  uint16_t status;
  uint16_t NDEF_Size = 0;
  uint8_t* pData = NDEF_Buffer;

  status = NDEF_getNDEFSize(&NDEF_Size);
  if(status != NDEF_OK)  return status ;

  if(NDEF_Size != 0)
  {
    // There are already records in the NDEF
    Record->RecordFlags &= ~MB_Mask;

    status = NfcTag_ReadNDEF(pData);
    if(status != NDEF_OK)  return status ;
  
    uint8_t* pNdefRecord;
    sRecordInfo_t   LastRecord;
    do {
      pNdefRecord = pData;
      NDEF_IdentifyBuffer(&LastRecord,pData);
      pData  += LastRecord.PayloadOffset + LastRecord.PayloadLength;
   // TO DO: add a security condition to avoid infinite loop if NDEF file is corrupted
    } while(!(LastRecord.RecordFlags & ME_Mask));
    LastRecord.RecordFlags &= ~ME_Mask;
    *pNdefRecord = LastRecord.RecordFlags;
  } else {
    // This will be the first message in memory
    Record->RecordFlags |= MB_Mask;
  }
  Record->RecordFlags |= ME_Mask;
  uint32_t RecordLength = NDEF_WriteRecord(Record,pData);
  

  return NfcTag_WriteNDEF( NDEF_Size + RecordLength, NDEF_Buffer );

}



/**
  * @brief  This function identify the NDEF message stored in tag.
  * @param  pRecordStruct : Structure to fill with record information.
  * @param  pNDEF : pointer on the NDEF message data.
  * @retval NDEF_OK : record struct filled.
  * @retval NDEF_ERROR : record struct not updated.
  */
uint16_t NDEF_IdentifyBuffer( sRecordInfo_t *pRecordStruct, uint8_t* pNDEF )
{
  uint16_t SizeOfRecordHeader, TypeNbByte, PayloadLengthField, IDLengthField, IDNbByte;

  /* Is ID length field present */
  if( (*pNDEF) & IL_Mask )
  {
    IDLengthField = ID_LENGTH_FIELD;
  }
  else
  {
    IDLengthField = 0;
  }

  /* it's a SR */
  if( (*pNDEF) & SR_Mask )
  {
    /* Analyse short record layout */
    TypeNbByte = pNDEF[1];
    PayloadLengthField = 1;
    if( IDLengthField == ID_LENGTH_FIELD )
      IDNbByte = pNDEF[3];
    else
      IDNbByte = 0;
  }
  else
  {
    /* Analyse normal record layout */
    TypeNbByte = pNDEF[1];
    PayloadLengthField = 4;
    if( IDLengthField == ID_LENGTH_FIELD )
      IDNbByte = pNDEF[6];
    else
      IDNbByte = 0;
  }

  SizeOfRecordHeader = RECORD_FLAG_FIELD + TYPE_LENGTH_FIELD + PayloadLengthField + IDLengthField + TypeNbByte + IDNbByte;

  /* it's a SR */
  if( pNDEF[0] & SR_Mask )
  {
    pRecordStruct->RecordFlags = pNDEF[0];
    pRecordStruct->TypeLength = TypeNbByte;
    pRecordStruct->PayloadLength = pNDEF[2];
    pRecordStruct->IDLength = IDNbByte;
    memcpy( pRecordStruct->Type, &pNDEF[3+IDLengthField], TypeNbByte );
    memcpy( pRecordStruct->ID, &pNDEF[3+IDLengthField+TypeNbByte], IDNbByte );
    pRecordStruct->PayloadOffset = SizeOfRecordHeader;
  }
  else
  {
    pRecordStruct->RecordFlags = pNDEF[0];
    pRecordStruct->TypeLength = TypeNbByte;
    pRecordStruct->PayloadLength = ( ((uint32_t)pNDEF[2]) << 24 ) |
                                   ( ((uint32_t)pNDEF[3]) << 16 ) |
                                   ( ((uint32_t)pNDEF[4]) << 8 )
                                    | pNDEF[5] ;
    pRecordStruct->IDLength = IDNbByte;
    memcpy( pRecordStruct->Type, &pNDEF[6+IDNbByte], TypeNbByte );
    memcpy( pRecordStruct->ID, &pNDEF[6+IDNbByte+TypeNbByte], IDNbByte );
    pRecordStruct->PayloadOffset = SizeOfRecordHeader;
  }

  pRecordStruct->PayloadBufferAdd = &pNDEF[pRecordStruct->PayloadOffset];

  NDEF_ParseRecordHeader( pRecordStruct );

  return NDEF_OK;
}

/**
  * @brief  This function writes a record into a NDEF buffer. (update SR flag if required)
  * @param  pRecord : Structure with record information to be written.
  * @param  pNDEF : pointer on the NDEF buffer.
  * @retval Length : Length of the written data (in bytes)
  */
uint32_t NDEF_WriteRecord( sRecordInfo_t *pRecord, uint8_t* pNDEF )
{
  /************************************/
/*  7 |  6 |  5 |  4 |  3 | 2  1  0 */
/*----------------------------------*/
/* MB   ME   CF   SR   IL    TNF    */  
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
/*           ID LENGTH              */  
/*----------------------------------*/
/*             TYPE                 */  
/*----------------------------------*/
/*              ID                  */  
/*----------------------------------*/
/*           PAYLOAD                */  
/************************************/
  uint8_t * start = pNDEF;

  // start by considering payload length
  if(pRecord->PayloadLength <= 0xFF)
    pRecord->RecordFlags |= SR_Mask;
  else
    pRecord->RecordFlags &= ~SR_Mask;
  
  // Then start writing!
  *pNDEF++ = pRecord->RecordFlags;

  *pNDEF++ = pRecord->TypeLength;

  if (!(pRecord->RecordFlags & SR_Mask))
  {
    *pNDEF++ = (pRecord->PayloadLength >> 24) & 0xFF;
    *pNDEF++ = (pRecord->PayloadLength >> 16) & 0xFF ;
    *pNDEF++ = (pRecord->PayloadLength >> 8) & 0xFF;
  }
  *pNDEF++ = (pRecord->PayloadLength) & 0xFF;

  if(pRecord->RecordFlags & IL_Mask)
    *pNDEF++ = (pRecord->IDLength);
  
  memcpy(pNDEF,pRecord->Type,pRecord->TypeLength);
  pNDEF += pRecord->TypeLength;

  if(pRecord->RecordFlags & IL_Mask)
  {
    memcpy(pNDEF,pRecord->ID,pRecord->IDLength);
    pNDEF += pRecord->IDLength;
  }

  memcpy(pNDEF,pRecord->PayloadBufferAdd,pRecord->PayloadLength);
  pNDEF += pRecord->PayloadLength;

  return (pNDEF - start);
}

/**
  * @brief  This function return the length a record data (update SR flag if required)
  * @param  pRecord : Structure with record information
  * @retval Length : Length of the data (in bytes)
  */
uint32_t NDEF_GetRecordLength( sRecordInfo_t *pRecord )
{
  // start by considering payload length
  if(pRecord->PayloadLength <= 0xFF)
    pRecord->RecordFlags |= SR_Mask;
  else
    pRecord->RecordFlags &= ~SR_Mask;
  
  // Then compute the length
  uint32_t length = 1 +                                                         // Flags
                    1 +                                                         // Type length
                    ((pRecord->RecordFlags & SR_Mask)? 1 : 4) +                 // Payload length
                    ((pRecord->RecordFlags & IL_Mask)? 1 : 0) +                 // ID length
                    pRecord->TypeLength +                                       // Type
                    ((pRecord->RecordFlags & IL_Mask)? pRecord->IDLength : 0) + // ID 
                    pRecord->PayloadLength;                                     // Payload;
  
  return length;
}

/**
  * @brief  This function clears the NDEF file
  * @retval NDEF Status
  */
uint16_t NDEF_ClearNDEF(void)
{
  return NDEF_WriteNDEF(0 , NULL);
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
