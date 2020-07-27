/**
  ******************************************************************************
  * @file    lib_NDEF_URI.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file help to manage NDEF file that represent URI.
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
#include "lib_NDEF_URI.h"

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

/** @defgroup libURI_Private_Functions
  * @{
  */

static void NDEF_Parse_WellKnowType( sRecordInfo_t *pRecordStruct, sURI_Info* pURI );

/**
  * @brief  This function read the URI information and store data in a structure.
  * @param  pRecordStruct : Pointer on the record structure.
  * @param  pURI : pointer on the structure to fill.
  */
static void NDEF_Parse_WellKnowType( sRecordInfo_t *pRecordStruct, sURI_Info* pURI )
{
  uint32_t PayloadSize;
  uint8_t Offset;
  uint8_t* pPayload;

  pPayload = (uint8_t*)(pRecordStruct->PayloadBufferAdd);

  switch( *pPayload )
  {
    case URI_ID_0x01:
      memcpy( pURI->protocol, URI_ID_0x01_STRING, strlen(URI_ID_0x01_STRING) );
      Offset = strlen( URI_ID_0x01_STRING );
      break;

    case URI_ID_0x02:
      memcpy( pURI->protocol, URI_ID_0x02_STRING, strlen(URI_ID_0x02_STRING) );
      Offset = strlen( URI_ID_0x02_STRING );
      break;

    case URI_ID_0x03:
      memcpy( pURI->protocol, URI_ID_0x03_STRING, strlen(URI_ID_0x03_STRING) );
      Offset = strlen( URI_ID_0x03_STRING );
      break;

    case URI_ID_0x04:
      memcpy( pURI->protocol, URI_ID_0x04_STRING, strlen(URI_ID_0x04_STRING) );
      Offset = strlen( URI_ID_0x04_STRING );
      break;

    case URI_ID_0x05:
      memcpy( pURI->protocol, URI_ID_0x05_STRING, strlen(URI_ID_0x05_STRING) );
      Offset = strlen( URI_ID_0x05_STRING );
      break;

    case URI_ID_0x06:
      memcpy( pURI->protocol, URI_ID_0x06_STRING, strlen(URI_ID_0x06_STRING) );
      Offset = strlen( URI_ID_0x06_STRING );
      break;

    case URI_ID_0x07:
      memcpy( pURI->protocol, URI_ID_0x07_STRING, strlen(URI_ID_0x07_STRING) );
      Offset = strlen( URI_ID_0x07_STRING );
      break;

    case URI_ID_0x08:
      memcpy( pURI->protocol, URI_ID_0x08_STRING, strlen(URI_ID_0x08_STRING) );
      Offset = strlen( URI_ID_0x08_STRING );
      break;

    case URI_ID_0x09:
      memcpy( pURI->protocol, URI_ID_0x09_STRING, strlen(URI_ID_0x09_STRING) );
      Offset = strlen( URI_ID_0x09_STRING );
      break;

    case URI_ID_0x0A:
      memcpy( pURI->protocol, URI_ID_0x0A_STRING, strlen(URI_ID_0x0A_STRING) );
      Offset = strlen( URI_ID_0x0A_STRING );
      break;

    case URI_ID_0x0B:
      memcpy( pURI->protocol, URI_ID_0x0B_STRING, strlen(URI_ID_0x0B_STRING) );
      Offset = strlen( URI_ID_0x0B_STRING );
      break;

    case URI_ID_0x0C:
      memcpy( pURI->protocol, URI_ID_0x0C_STRING, strlen(URI_ID_0x0C_STRING) );
      Offset = strlen( URI_ID_0x0C_STRING );
      break;

    case URI_ID_0x0D:
      memcpy( pURI->protocol, URI_ID_0x0D_STRING, strlen(URI_ID_0x0D_STRING) );
      Offset = strlen( URI_ID_0x0D_STRING );
      break;

    case URI_ID_0x0E:
      memcpy( pURI->protocol, URI_ID_0x0E_STRING, strlen(URI_ID_0x0E_STRING) );
      Offset = strlen( URI_ID_0x0E_STRING );
      break;

    case URI_ID_0x0F:
      memcpy( pURI->protocol, URI_ID_0x0F_STRING, strlen(URI_ID_0x0F_STRING) );
      Offset = strlen( URI_ID_0x0F_STRING );
      break;

    case URI_ID_0x10:
      memcpy( pURI->protocol, URI_ID_0x10_STRING, strlen(URI_ID_0x10_STRING) );
      Offset = strlen( URI_ID_0x10_STRING );
      break;

    case URI_ID_0x11:
      memcpy( pURI->protocol, URI_ID_0x11_STRING, strlen(URI_ID_0x11_STRING) );
      Offset = strlen( URI_ID_0x11_STRING );
      break;

    case URI_ID_0x12:
      memcpy( pURI->protocol, URI_ID_0x12_STRING, strlen(URI_ID_0x12_STRING) );
      Offset = strlen( URI_ID_0x12_STRING );
      break;

    case URI_ID_0x13:
      memcpy( pURI->protocol, URI_ID_0x13_STRING, strlen(URI_ID_0x13_STRING) );
      Offset = strlen( URI_ID_0x13_STRING );
      break;

    case URI_ID_0x14:
      memcpy( pURI->protocol, URI_ID_0x14_STRING, strlen(URI_ID_0x14_STRING) );
      Offset = strlen( URI_ID_0x14_STRING );
      break;

    case URI_ID_0x15:
      memcpy( pURI->protocol, URI_ID_0x15_STRING, strlen(URI_ID_0x15_STRING) );
      Offset = strlen( URI_ID_0x15_STRING );
      break;

    case URI_ID_0x16:
      memcpy( pURI->protocol, URI_ID_0x16_STRING, strlen(URI_ID_0x16_STRING) );
      Offset = strlen( URI_ID_0x16_STRING );
      break;

    case URI_ID_0x17:
      memcpy( pURI->protocol, URI_ID_0x17_STRING, strlen(URI_ID_0x17_STRING) );
      Offset = strlen( URI_ID_0x17_STRING );
      break;

    case URI_ID_0x18:
      memcpy( pURI->protocol, URI_ID_0x18_STRING, strlen(URI_ID_0x18_STRING) );
      Offset = strlen( URI_ID_0x18_STRING );
      break;

    case URI_ID_0x19:
      memcpy( pURI->protocol, URI_ID_0x19_STRING, strlen(URI_ID_0x19_STRING) );
      Offset = strlen( URI_ID_0x19_STRING );
      break;

    case URI_ID_0x1A:
      memcpy( pURI->protocol, URI_ID_0x1A_STRING, strlen(URI_ID_0x1A_STRING) );
      Offset = strlen( URI_ID_0x1A_STRING );
      break;

    case URI_ID_0x1B:
      memcpy( pURI->protocol, URI_ID_0x1B_STRING, strlen(URI_ID_0x1B_STRING) );
      Offset = strlen( URI_ID_0x1B_STRING );
      break;

    case URI_ID_0x1C:
      memcpy( pURI->protocol, URI_ID_0x1C_STRING, strlen(URI_ID_0x1C_STRING) );
      Offset = strlen( URI_ID_0x1C_STRING );
      break;

    case URI_ID_0x1D:
      memcpy( pURI->protocol, URI_ID_0x1D_STRING, strlen(URI_ID_0x1D_STRING) );
      Offset = strlen( URI_ID_0x1D_STRING );
      break;

    case URI_ID_0x1E:
      memcpy( pURI->protocol, URI_ID_0x1E_STRING, strlen(URI_ID_0x1E_STRING) );
      Offset = strlen( URI_ID_0x1E_STRING );
      break;

    case URI_ID_0x1F:
      memcpy( pURI->protocol, URI_ID_0x1F_STRING, strlen(URI_ID_0x1F_STRING) );
      Offset = strlen( URI_ID_0x1F_STRING );
      break;

    case URI_ID_0x20:
      memcpy( pURI->protocol, URI_ID_0x20_STRING, strlen(URI_ID_0x20_STRING) );
      Offset = strlen( URI_ID_0x20_STRING );
      break;

    case URI_ID_0x21:
      memcpy( pURI->protocol, URI_ID_0x21_STRING, strlen(URI_ID_0x21_STRING) );
      Offset = strlen( URI_ID_0x21_STRING );
      break;

    case URI_ID_0x22:
      memcpy( pURI->protocol, URI_ID_0x22_STRING, strlen(URI_ID_0x22_STRING) );
      Offset = strlen( URI_ID_0x22_STRING );
      break;

    case URI_ID_0x23:
      memcpy( pURI->protocol, URI_ID_0x23_STRING, strlen(URI_ID_0x23_STRING) );
      Offset = strlen( URI_ID_0x23_STRING );
      break;

    default:
      Offset = 0;
      /* Should not happened */
      break;
  }
  /* add end of string character */
  pURI->protocol[Offset] = '\0';

  pPayload++; /* go after well know byte */

  PayloadSize = pRecordStruct->PayloadLength;

  PayloadSize = PayloadSize - 1; /* remove well know byte */

  memcpy( pURI->URI_Message, pPayload, PayloadSize );
  /* add end of string character */
  pURI->URI_Message[PayloadSize] = '\0';

}

/**
  * @}
  */

/** @defgroup libURI_Public_Functions
  * @{
  * @brief  This file is used to manage URI (stored or loaded in tag)
  */ 

/**
  * @brief  This function read NDEF and retrieve URI information if any.
  * @param  pRecordStruct : Pointer on the record structure.
  * @param  pURI : pointer on the structure to fill.
  * @retval NDEF_OK : URI information from NDEF have been retrieved.
  * @retval NDEF_ERROR : Not able to retrieve URI information.
  */
uint16_t NDEF_ReadURI( sRecordInfo_t *pRecordStruct, sURI_Info *pURI )
{
  uint16_t status = NDEF_ERROR;
  sRecordInfo_t *pSPRecordStruct;
  uint32_t PayloadSize, RecordPosition;
  uint8_t* pData;

  if( pRecordStruct->NDEF_Type == WELL_KNOWN_ABRIDGED_URI_TYPE )
  {
    NDEF_Parse_WellKnowType( pRecordStruct, pURI );
    status = NDEF_OK;
  }
  else if( pRecordStruct->NDEF_Type == SMARTPOSTER_TYPE )
  {
    for( RecordPosition = 0; RecordPosition < pRecordStruct->NbOfRecordInSPPayload; RecordPosition++ )
    {
      pSPRecordStruct = pRecordStruct->SPRecordStructAdd[RecordPosition];
      if( pSPRecordStruct->NDEF_Type == WELL_KNOWN_ABRIDGED_URI_TYPE )
      {
        NDEF_Parse_WellKnowType( pSPRecordStruct, pURI );
        status = NDEF_OK;
      }
      if( pSPRecordStruct->NDEF_Type == TEXT_TYPE )
      {
        PayloadSize = pSPRecordStruct->PayloadLength;

        /* The instruction content the UTF-8 language code that is not used here */
        pData = (uint8_t*)pSPRecordStruct->PayloadBufferAdd;
        PayloadSize -= *pData + 1; /* remove not usefull data */
        pData += *pData + 1;

        memcpy( pURI->Information, pData, PayloadSize );
      }
    }
  }
  else 
  {
    status = NDEF_ERROR;
  }

  return status;
}

/**
  * @brief  This function prepare the NDEF message with the URI data given in the structure.
  * @param  pURI : pointer on structure that contain the URI information.
  * @param  pNDEFMessage : pointer on the NDEF message.
  * @param  size : to store the size of the NDEF message generated.
  */
void NDEF_PrepareURIMessage( sURI_Info *pURI, uint8_t *pNDEFMessage, uint16_t *size )
{
  uint32_t uriSize, totalSize, Offset = 0;
  uint32_t infoSize = 0;
  char type;

  /* An URI can be included in a smart poster to add text to give instruction to user for instance */

  /* URI (smart poster) Record Header */
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

  /* We need to know the URI type in order to define if an abreviation is available */
  type = getUriType( pURI->protocol );

  /* URI : 1+URI for abreviate protocol*/
  if( type != URI_ID_0x00 )
    uriSize = 1 + strlen(pURI->URI_Message);
  else /*: 1+protocol+URI else*/
    uriSize = 1 + strlen(pURI->protocol) + strlen(pURI->URI_Message);

  /* Check if a Smart poster is needed */
  if( pURI->Information[0] != '\0' )
  {
    /* Info : 1+2+info */
    infoSize = 1 + ISO_ENGLISH_CODE_STRING_LENGTH + strlen(pURI->Information);
    /* Total */
    totalSize = 4 + uriSize + 4 + infoSize;
    if( uriSize > 255 ) totalSize += 3;   /* Normal URI size */
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

  /* URI header */
  pNDEFMessage[Offset] = 0x81;
  if( uriSize < 256 ) pNDEFMessage[Offset] |= 0x10;                      // Set the SR bit
  if( pURI->Information[0] == '\0' ) pNDEFMessage[Offset] |= 0x40;       // Set the ME bit
  Offset++;

  pNDEFMessage[Offset++] = URI_TYPE_STRING_LENGTH;
  if( uriSize > 255 )
  {
    pNDEFMessage[Offset++] = (uriSize & 0xFF000000) >> 24;
    pNDEFMessage[Offset++] = (uriSize & 0x00FF0000) >> 16;
    pNDEFMessage[Offset++] = (uriSize & 0x0000FF00) >> 8;
    pNDEFMessage[Offset++] = uriSize & 0x000000FF;
  }
  else
  {
    pNDEFMessage[Offset++] = (uint8_t)uriSize;
  }
  memcpy( &pNDEFMessage[Offset], URI_TYPE_STRING, URI_TYPE_STRING_LENGTH );
  Offset += URI_TYPE_STRING_LENGTH;

  pNDEFMessage[Offset++] = type;
  if( type == URI_ID_0x00 ) // No abreviation
  {
    memcpy( &pNDEFMessage[Offset], pURI->protocol, strlen(pURI->protocol) );
    Offset += strlen(pURI->protocol);
  }

  memcpy( &pNDEFMessage[Offset], pURI->URI_Message, strlen(pURI->URI_Message) );
  Offset += strlen(pURI->URI_Message);

  /* Information header */
  if( pURI->Information[0] != '\0' )
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
    Offset+=TEXT_TYPE_STRING_LENGTH;
    pNDEFMessage[Offset++] = ISO_ENGLISH_CODE_STRING_LENGTH; /* UTF-8 with x byte language code */
    memcpy( &pNDEFMessage[Offset], ISO_ENGLISH_CODE_STRING, ISO_ENGLISH_CODE_STRING_LENGTH );
    Offset += ISO_ENGLISH_CODE_STRING_LENGTH;

    /* Information payload */
    memcpy( &pNDEFMessage[Offset], pURI->Information, strlen(pURI->Information) );
    Offset += strlen(pURI->Information);
  }

  *size = Offset;

}
/**
  * @brief  This function write the NDEF file with the URI data given in the structure.
  * @param  pURI : pointer on structure that contain the URI information.
  * @retval NDEF_OK : NDEF file data written in the tag.
  * @retval NDEF_ERROR : not able to store NDEF in tag.
  * @retval NDEF_ERROR_MEMORY_INTERNAL : Cannot write to tag.
  * @retval NDEF_ERROR_NOT_FORMATED : CCFile data not supported or not present.
  * @retval NDEF_ERROR_MEMORY_TAG : Size not compatible with memory.
  * @retval NDEF_ERROR_LOCKED : Tag locked, cannot be write.
  */
uint16_t NDEF_WriteURI( sURI_Info *pURI )
{
  uint16_t status = NDEF_ERROR, Offset = 0;

  NDEF_PrepareURIMessage( pURI, NDEF_Buffer, &Offset );

  status = NfcTag_WriteNDEF( Offset , NDEF_Buffer );

  return status;
}

char getUriType( char *protocol )
{
  if( !memcmp( protocol, URI_ID_0x01_STRING, strlen(URI_ID_0x01_STRING) ) ) return URI_ID_0x01;
  else if( !memcmp( protocol, URI_ID_0x02_STRING, strlen(URI_ID_0x02_STRING) ) ) return URI_ID_0x02;
  else if( !memcmp( protocol, URI_ID_0x03_STRING, strlen(URI_ID_0x03_STRING) ) ) return URI_ID_0x03;
  else if( !memcmp( protocol, URI_ID_0x04_STRING, strlen(URI_ID_0x04_STRING) ) ) return URI_ID_0x04;
  else if( !memcmp( protocol, URI_ID_0x05_STRING, strlen(URI_ID_0x05_STRING) ) ) return URI_ID_0x05;
  else if( !memcmp( protocol, URI_ID_0x06_STRING, strlen(URI_ID_0x06_STRING) ) ) return URI_ID_0x06;
  else if( !memcmp( protocol, URI_ID_0x07_STRING, strlen(URI_ID_0x07_STRING) ) ) return URI_ID_0x07;
  else if( !memcmp( protocol, URI_ID_0x08_STRING, strlen(URI_ID_0x08_STRING) ) ) return URI_ID_0x08;
  else if( !memcmp( protocol, URI_ID_0x09_STRING, strlen(URI_ID_0x09_STRING) ) ) return URI_ID_0x09;
  else if( !memcmp( protocol, URI_ID_0x0A_STRING, strlen(URI_ID_0x0A_STRING) ) ) return URI_ID_0x0A;
  else if( !memcmp( protocol, URI_ID_0x0B_STRING, strlen(URI_ID_0x0B_STRING) ) ) return URI_ID_0x0B;
  else if( !memcmp( protocol, URI_ID_0x0C_STRING, strlen(URI_ID_0x0C_STRING) ) ) return URI_ID_0x0C;
  else if( !memcmp( protocol, URI_ID_0x0D_STRING, strlen(URI_ID_0x0D_STRING) ) ) return URI_ID_0x0D;
  else if( !memcmp( protocol, URI_ID_0x0E_STRING, strlen(URI_ID_0x0E_STRING) ) ) return URI_ID_0x0E;
  else if( !memcmp( protocol, URI_ID_0x0F_STRING, strlen(URI_ID_0x0F_STRING) ) ) return URI_ID_0x0F;
  else if( !memcmp( protocol, URI_ID_0x10_STRING, strlen(URI_ID_0x10_STRING) ) ) return URI_ID_0x10;
  else if( !memcmp( protocol, URI_ID_0x11_STRING, strlen(URI_ID_0x11_STRING) ) ) return URI_ID_0x11;
  else if( !memcmp( protocol, URI_ID_0x12_STRING, strlen(URI_ID_0x12_STRING) ) ) return URI_ID_0x12;
  else if( !memcmp( protocol, URI_ID_0x13_STRING, strlen(URI_ID_0x13_STRING) ) ) return URI_ID_0x13;
  else if( !memcmp( protocol, URI_ID_0x14_STRING, strlen(URI_ID_0x14_STRING) ) ) return URI_ID_0x14;
  else if( !memcmp( protocol, URI_ID_0x15_STRING, strlen(URI_ID_0x15_STRING) ) ) return URI_ID_0x15;
  else if( !memcmp( protocol, URI_ID_0x16_STRING, strlen(URI_ID_0x16_STRING) ) ) return URI_ID_0x16;
  else if( !memcmp( protocol, URI_ID_0x17_STRING, strlen(URI_ID_0x17_STRING) ) ) return URI_ID_0x17;
  else if( !memcmp( protocol, URI_ID_0x18_STRING, strlen(URI_ID_0x18_STRING) ) ) return URI_ID_0x18;
  else if( !memcmp( protocol, URI_ID_0x19_STRING, strlen(URI_ID_0x19_STRING) ) ) return URI_ID_0x19;
  else if( !memcmp( protocol, URI_ID_0x1A_STRING, strlen(URI_ID_0x1A_STRING) ) ) return URI_ID_0x1A;
  else if( !memcmp( protocol, URI_ID_0x1B_STRING, strlen(URI_ID_0x1B_STRING) ) ) return URI_ID_0x1B;
  else if( !memcmp( protocol, URI_ID_0x1C_STRING, strlen(URI_ID_0x1C_STRING) ) ) return URI_ID_0x1C;
  else if( !memcmp( protocol, URI_ID_0x1D_STRING, strlen(URI_ID_0x1D_STRING) ) ) return URI_ID_0x1D;
  else if( !memcmp( protocol, URI_ID_0x1E_STRING, strlen(URI_ID_0x1E_STRING) ) ) return URI_ID_0x1E;
  else if( !memcmp( protocol, URI_ID_0x1F_STRING, strlen(URI_ID_0x1F_STRING) ) ) return URI_ID_0x1F;
  else if( !memcmp( protocol, URI_ID_0x20_STRING, strlen(URI_ID_0x20_STRING) ) ) return URI_ID_0x20;
  else if( !memcmp( protocol, URI_ID_0x21_STRING, strlen(URI_ID_0x21_STRING) ) ) return URI_ID_0x21;
  else if( !memcmp( protocol, URI_ID_0x22_STRING, strlen(URI_ID_0x22_STRING) ) ) return URI_ID_0x22;
  else if( !memcmp( protocol, URI_ID_0x23_STRING, strlen(URI_ID_0x23_STRING) ) ) return URI_ID_0x23;
  else return URI_ID_0x00; // No abreviation for this protocol	
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
