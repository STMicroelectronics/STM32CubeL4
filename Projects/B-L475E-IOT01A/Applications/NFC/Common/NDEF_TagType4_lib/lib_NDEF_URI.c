/**
  ******************************************************************************
  * @file    lib_NDEF_URI.c
  * @author  MMY Application Team
  * @brief   This file help to manage NDEF file that represent URI.
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
#include "lib_NDEF_URI.h"

/** @addtogroup NFC_libraries
 *  @{
 */


/** @addtogroup TAGTYPE4_NDEF_URI
  * @{
  */

/**
 * @brief  This buffer contains the data send/received by TAG
 */
extern uint8_t NDEF_Buffer [NFC_TT4_NDEF_MAX_SIZE];

/** @defgroup TAGTYPE4_NDEF_URI_Private_Functions NDEF_URI Private Functions
  * @{
  */

static void NDEF_Parse_WellKnowType(sRecordInfo *pRecordStruct, sURI_Info* pURI );

/**
  * @brief  This fonction read the URI information and store data in a structure
  * @param  pRecordStruct : Pointer on the record structure
  * @param  pURI : pointer on the structure to fill
  * @retval NONE 
  */
static void NDEF_Parse_WellKnowType(sRecordInfo *pRecordStruct, sURI_Info* pURI )
{
  uint32_t PayloadSize;
  uint8_t Offset;
  uint8_t* pPayload;
  
  pPayload = (uint8_t*)(pRecordStruct->PayloadBufferAdd);
  
  switch( *pPayload)
  {
  case URI_ID_0x01:
    memcpy( pURI->protocol, URI_ID_0x01_STRING, strlen(URI_ID_0x01_STRING));
    Offset = strlen(URI_ID_0x01_STRING);
    break;
    
  case URI_ID_0x02:
    memcpy( pURI->protocol, URI_ID_0x02_STRING, strlen(URI_ID_0x02_STRING));
    Offset = strlen(URI_ID_0x02_STRING);
    break;
    
  case URI_ID_0x03:
    memcpy( pURI->protocol, URI_ID_0x03_STRING, strlen(URI_ID_0x03_STRING));
    Offset = strlen(URI_ID_0x03_STRING);
    break;
    
  case URI_ID_0x04:
    memcpy( pURI->protocol, URI_ID_0x04_STRING, strlen(URI_ID_0x04_STRING));
    Offset = strlen(URI_ID_0x04_STRING);
    break;
    
  case URI_ID_0x05:
    memcpy( pURI->protocol, URI_ID_0x05_STRING, strlen(URI_ID_0x05_STRING));
    Offset = strlen(URI_ID_0x05_STRING);
    break;
    
  case URI_ID_0x06:
    memcpy( pURI->protocol, URI_ID_0x06_STRING, strlen(URI_ID_0x06_STRING));
    Offset = strlen(URI_ID_0x06_STRING);
    break;
    
  case URI_ID_0x07:
    memcpy( pURI->protocol, URI_ID_0x07_STRING, strlen(URI_ID_0x07_STRING));
    Offset = strlen(URI_ID_0x07_STRING);
    break;
    
  case URI_ID_0x08:
    memcpy( pURI->protocol, URI_ID_0x08_STRING, strlen(URI_ID_0x08_STRING));
    Offset = strlen(URI_ID_0x08_STRING);
    break;
    
  case URI_ID_0x09:
    memcpy( pURI->protocol, URI_ID_0x09_STRING, strlen(URI_ID_0x09_STRING));
    Offset = strlen(URI_ID_0x09_STRING);
    break;
    
  case URI_ID_0x0A:
    memcpy( pURI->protocol, URI_ID_0x0A_STRING, strlen(URI_ID_0x0A_STRING));
    Offset = strlen(URI_ID_0x0A_STRING);
    break;
    
  case URI_ID_0x0B:
    memcpy( pURI->protocol, URI_ID_0x0B_STRING, strlen(URI_ID_0x0B_STRING));
    Offset = strlen(URI_ID_0x0B_STRING);
    break;
    
  case URI_ID_0x0C:
    memcpy( pURI->protocol, URI_ID_0x0C_STRING, strlen(URI_ID_0x0C_STRING));
    Offset = strlen(URI_ID_0x0C_STRING);
    break;
    
  case URI_ID_0x0D:
    memcpy( pURI->protocol, URI_ID_0x0D_STRING, strlen(URI_ID_0x0D_STRING));
    Offset = strlen(URI_ID_0x0D_STRING);
    break;
    
  case URI_ID_0x0E:
    memcpy( pURI->protocol, URI_ID_0x0E_STRING, strlen(URI_ID_0x0E_STRING));
    Offset = strlen(URI_ID_0x0E_STRING);
    break;
    
  case URI_ID_0x0F:
    memcpy( pURI->protocol, URI_ID_0x0F_STRING, strlen(URI_ID_0x0F_STRING));
    Offset = strlen(URI_ID_0x0F_STRING);
    break;
    
  case URI_ID_0x10:
    memcpy( pURI->protocol, URI_ID_0x10_STRING, strlen(URI_ID_0x10_STRING));
    Offset = strlen(URI_ID_0x10_STRING);
    break;
    
  case URI_ID_0x11:
    memcpy( pURI->protocol, URI_ID_0x11_STRING, strlen(URI_ID_0x11_STRING));
    Offset = strlen(URI_ID_0x11_STRING);
    break;
    
  case URI_ID_0x12:
    memcpy( pURI->protocol, URI_ID_0x12_STRING, strlen(URI_ID_0x12_STRING));
    Offset = strlen(URI_ID_0x12_STRING);
    break;
    
  case URI_ID_0x13:
    memcpy( pURI->protocol, URI_ID_0x13_STRING, strlen(URI_ID_0x13_STRING));
    Offset = strlen(URI_ID_0x13_STRING);
    break;
    
  case URI_ID_0x14:
    memcpy( pURI->protocol, URI_ID_0x14_STRING, strlen(URI_ID_0x14_STRING));
    Offset = strlen(URI_ID_0x14_STRING);
    break;
    
  case URI_ID_0x15:
    memcpy( pURI->protocol, URI_ID_0x15_STRING, strlen(URI_ID_0x15_STRING));
    Offset = strlen(URI_ID_0x15_STRING);
    break;
    
  case URI_ID_0x16:
    memcpy( pURI->protocol, URI_ID_0x16_STRING, strlen(URI_ID_0x16_STRING));
    Offset = strlen(URI_ID_0x16_STRING);
    break;
    
  case URI_ID_0x17:
    memcpy( pURI->protocol, URI_ID_0x17_STRING, strlen(URI_ID_0x17_STRING));
    Offset = strlen(URI_ID_0x17_STRING);
    break;
    
  case URI_ID_0x18:
    memcpy( pURI->protocol, URI_ID_0x18_STRING, strlen(URI_ID_0x18_STRING));
    Offset = strlen(URI_ID_0x18_STRING);
    break;
    
  case URI_ID_0x19:
    memcpy( pURI->protocol, URI_ID_0x19_STRING, strlen(URI_ID_0x19_STRING));
    Offset = strlen(URI_ID_0x19_STRING);
    break;
    
  case URI_ID_0x1A:
    memcpy( pURI->protocol, URI_ID_0x1A_STRING, strlen(URI_ID_0x1A_STRING));
    Offset = strlen(URI_ID_0x1A_STRING);
    break;
    
  case URI_ID_0x1B:
    memcpy( pURI->protocol, URI_ID_0x1B_STRING, strlen(URI_ID_0x1B_STRING));
    Offset = strlen(URI_ID_0x1B_STRING);
    break;
    
  case URI_ID_0x1C:
    memcpy( pURI->protocol, URI_ID_0x1C_STRING, strlen(URI_ID_0x1C_STRING));
    Offset = strlen(URI_ID_0x1C_STRING);
    break;
    
  case URI_ID_0x1D:
    memcpy( pURI->protocol, URI_ID_0x1D_STRING, strlen(URI_ID_0x1D_STRING));
    Offset = strlen(URI_ID_0x1D_STRING);
    break;
    
  case URI_ID_0x1E:
    memcpy( pURI->protocol, URI_ID_0x1E_STRING, strlen(URI_ID_0x1E_STRING));
    Offset = strlen(URI_ID_0x1E_STRING);
    break;
    
  case URI_ID_0x1F:
    memcpy( pURI->protocol, URI_ID_0x1F_STRING, strlen(URI_ID_0x1F_STRING));
    Offset = strlen(URI_ID_0x1F_STRING);
    break;
    
  case URI_ID_0x20:
    memcpy( pURI->protocol, URI_ID_0x20_STRING, strlen(URI_ID_0x20_STRING));
    Offset = strlen(URI_ID_0x20_STRING);
    break;
    
  case URI_ID_0x21:
    memcpy( pURI->protocol, URI_ID_0x21_STRING, strlen(URI_ID_0x21_STRING));
    Offset = strlen(URI_ID_0x21_STRING);
    break;
    
  case URI_ID_0x22:
    memcpy( pURI->protocol, URI_ID_0x22_STRING, strlen(URI_ID_0x22_STRING));
    Offset = strlen(URI_ID_0x22_STRING);
    break;
    
  case URI_ID_0x23:
    memcpy( pURI->protocol, URI_ID_0x23_STRING, strlen(URI_ID_0x23_STRING));
    Offset = strlen(URI_ID_0x23_STRING);
    break;
    
  default:
    Offset = 0;
    /* Should not happened */
    break;
  }
  /* add end of string charactere */
  pURI->protocol[Offset] = '\0';
  
  pPayload++; /* go after well know byte */
  
  PayloadSize = ((uint32_t)(pRecordStruct->PayloadLength3)<<24) | ((uint32_t)(pRecordStruct->PayloadLength2)<<16) |
    ((uint32_t)(pRecordStruct->PayloadLength1)<<8)  | pRecordStruct->PayloadLength0;
  
  PayloadSize = PayloadSize -1; /* remove well know byte */
  
  memcpy( pURI->URI_Message, pPayload, PayloadSize);
  /* add end of string charactere */
  pURI->URI_Message[PayloadSize] = '\0';
  
}

/**
  * @}
  */

/** @defgroup TAGTYPE4_NDEF_URI_Public_Functions   NDEF_URI Public Functions
  * @{
  *  @brief  This file is used to manage URI (stored or loaded in tag)
  */ 

/**
  * @brief  This fonction read NDEF and retrieve URI information if any
  * @param  pRecordStruct : Pointer on the record structure
  * @param  pURI : pointer on the structure to fill 
  * @retval SUCCESS : URI information from NDEF have been retrieved
  * @retval ERROR : Not able to retrieve URI information
  */
uint16_t NDEF_ReadURI(sRecordInfo *pRecordStruct, sURI_Info *pURI)
{
  uint16_t status = NDEF_ERROR;
  sRecordInfo *pSPRecordStruct;  
  uint32_t PayloadSize, RecordPosition;
  uint8_t* pData;
  
  if( pRecordStruct->NDEF_Type == WELL_KNOWN_ABRIDGED_URI_TYPE )
  {    
    NDEF_Parse_WellKnowType(pRecordStruct, pURI );
    status = NDEF_SUCCESS;
  }
  else if( pRecordStruct->NDEF_Type == SMARTPOSTER_TYPE)
  {
    for (RecordPosition = 0; RecordPosition<pRecordStruct->NbOfRecordInSPPayload; RecordPosition++)
    {
      pSPRecordStruct = (sRecordInfo *)(pRecordStruct->SPRecordStructAdd[RecordPosition]);
      if(pSPRecordStruct->NDEF_Type == WELL_KNOWN_ABRIDGED_URI_TYPE )
      {
        NDEF_Parse_WellKnowType(pSPRecordStruct, pURI );
        status = NDEF_SUCCESS;
      }
      if(pSPRecordStruct->NDEF_Type == TEXT_TYPE )
      {
        PayloadSize = ((uint32_t)(pSPRecordStruct->PayloadLength3)<<24) | ((uint32_t)(pSPRecordStruct->PayloadLength2)<<16) |
          ((uint32_t)(pSPRecordStruct->PayloadLength1)<<8)  | pSPRecordStruct->PayloadLength0;
        
        /* The instruction content the UTF-8 language code that is not used here */
        pData = (uint8_t*)pSPRecordStruct->PayloadBufferAdd;
        pData += *pData+1;
        
        memcpy(pURI->Information, pData, PayloadSize);
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
  * @brief  This fonction write the NDEF file with the URI data given in the structure
  * @param  pURI : pointer on structure that contain the URI information
  * @retval SUCCESS : the function is succesful
  * @retval ERROR : Not able to store NDEF file inside tag.
  */
uint16_t NDEF_WriteURI ( sURI_Info *pURI)
{
  uint16_t status = NDEF_ERROR;
  uint32_t uriSize, infoSize, totalSize, Offset = 0;
  char type;
  
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
  
  /* We need to know the URI type in order to define if an abreviation is available */
  type = getUriType(pURI->protocol);
  
  /* URI : 1+URI for abreviate protocol*/
  if (type != URI_ID_0x00)
    uriSize = 1+strlen(pURI->URI_Message);
  else /*: 1+protocol+URI else*/
    uriSize = 1+strlen(pURI->protocol)+strlen(pURI->URI_Message);
  
  /* Check if a Smart poster is needed */
  if (pURI->Information[0] != '\0')
  {
    /* Info : 1+2+info */
    infoSize = 1+ISO_ENGLISH_CODE_STRING_LENGTH+strlen(pURI->Information);
    /* Total */
    totalSize = 4+uriSize+4+infoSize;
    if (uriSize > 255) totalSize+=3; /* Normal URI size */
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
  
  /* URI header */
  NDEF_Buffer[Offset] = 0x81;
  if (uriSize < 256) NDEF_Buffer[Offset] |= 0x10;                      /* Set the SR bit */
  if (pURI->Information[0] == '\0') NDEF_Buffer[Offset] |= 0x40;       /* Set the ME bit */
  Offset++;
  
  NDEF_Buffer[Offset++] = URI_TYPE_STRING_LENGTH;
  if (uriSize > 255)
  {
    NDEF_Buffer[Offset++] = (uriSize & 0xFF000000)>>24;
    NDEF_Buffer[Offset++] = (uriSize & 0x00FF0000)>>16;
    NDEF_Buffer[Offset++] = (uriSize & 0x0000FF00)>>8;
    NDEF_Buffer[Offset++] = (uriSize & 0x000000FF);
  }
  else
  {
    NDEF_Buffer[Offset++] = (uint8_t)uriSize;
  }
  memcpy(&NDEF_Buffer[Offset], URI_TYPE_STRING, URI_TYPE_STRING_LENGTH);
  Offset+=URI_TYPE_STRING_LENGTH;
  
  NDEF_Buffer[Offset++]=type;
  if (type == URI_ID_0x00) /* No abreviation */
  {
    memcpy(&NDEF_Buffer[Offset], pURI->protocol, strlen(pURI->protocol));
    Offset+=strlen(pURI->protocol);
  }
  
  memcpy(&NDEF_Buffer[Offset], pURI->URI_Message, strlen(pURI->URI_Message));
  Offset+=strlen(pURI->URI_Message);
  
  /* Information header */
  if (pURI->Information[0] != '\0')
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
    memcpy( &NDEF_Buffer[Offset], pURI->Information,strlen(pURI->Information));
    Offset += strlen(pURI->Information);
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

char getUriType(char *protocol)
{
  if(!memcmp( protocol, URI_ID_0x01_STRING, strlen(URI_ID_0x01_STRING))) return URI_ID_0x01;
  else if(!memcmp( protocol, URI_ID_0x02_STRING, strlen(URI_ID_0x02_STRING))) return URI_ID_0x02;
  else if(!memcmp( protocol, URI_ID_0x03_STRING, strlen(URI_ID_0x03_STRING))) return URI_ID_0x03;
  else if(!memcmp( protocol, URI_ID_0x04_STRING, strlen(URI_ID_0x04_STRING))) return URI_ID_0x04;
  else if(!memcmp( protocol, URI_ID_0x05_STRING, strlen(URI_ID_0x05_STRING))) return URI_ID_0x05;
  else if(!memcmp( protocol, URI_ID_0x06_STRING, strlen(URI_ID_0x06_STRING)))  return URI_ID_0x06;
  else if(!memcmp( protocol, URI_ID_0x07_STRING, strlen(URI_ID_0x07_STRING)))  return URI_ID_0x07;
  else if(!memcmp( protocol, URI_ID_0x08_STRING, strlen(URI_ID_0x08_STRING)))  return URI_ID_0x08;
  else if(!memcmp( protocol, URI_ID_0x09_STRING, strlen(URI_ID_0x09_STRING)))  return URI_ID_0x09;
  else if(!memcmp( protocol, URI_ID_0x0A_STRING, strlen(URI_ID_0x0A_STRING)))  return URI_ID_0x0A;  
  else if(!memcmp( protocol, URI_ID_0x0B_STRING, strlen(URI_ID_0x0B_STRING)))  return URI_ID_0x0B;
  else if(!memcmp( protocol, URI_ID_0x0C_STRING, strlen(URI_ID_0x0C_STRING)))  return URI_ID_0x0C;
  else if(!memcmp( protocol, URI_ID_0x0D_STRING, strlen(URI_ID_0x0D_STRING)))  return URI_ID_0x0D;
  else if(!memcmp( protocol, URI_ID_0x0E_STRING, strlen(URI_ID_0x0E_STRING)))  return URI_ID_0x0E;
  else if(!memcmp( protocol, URI_ID_0x0F_STRING, strlen(URI_ID_0x0F_STRING)))  return URI_ID_0x0F;
  else if(!memcmp( protocol, URI_ID_0x10_STRING, strlen(URI_ID_0x10_STRING)))  return URI_ID_0x10;
  else if(!memcmp( protocol, URI_ID_0x11_STRING, strlen(URI_ID_0x11_STRING)))  return URI_ID_0x11;
  else if(!memcmp( protocol, URI_ID_0x12_STRING, strlen(URI_ID_0x12_STRING)))  return URI_ID_0x12;
  else if(!memcmp( protocol, URI_ID_0x13_STRING, strlen(URI_ID_0x13_STRING)))  return URI_ID_0x13;
  else if(!memcmp( protocol, URI_ID_0x14_STRING, strlen(URI_ID_0x14_STRING)))  return URI_ID_0x14;
  else if(!memcmp( protocol, URI_ID_0x15_STRING, strlen(URI_ID_0x15_STRING)))  return URI_ID_0x15;
  else if(!memcmp( protocol, URI_ID_0x16_STRING, strlen(URI_ID_0x16_STRING)))  return URI_ID_0x16;
  else if(!memcmp( protocol, URI_ID_0x17_STRING, strlen(URI_ID_0x17_STRING)))  return URI_ID_0x17;
  else if(!memcmp( protocol, URI_ID_0x18_STRING, strlen(URI_ID_0x18_STRING)))  return URI_ID_0x18;
  else if(!memcmp( protocol, URI_ID_0x19_STRING, strlen(URI_ID_0x19_STRING)))  return URI_ID_0x19;
  else if(!memcmp( protocol, URI_ID_0x1A_STRING, strlen(URI_ID_0x1A_STRING)))  return URI_ID_0x1A;
  else if(!memcmp( protocol, URI_ID_0x1B_STRING, strlen(URI_ID_0x1B_STRING)))  return URI_ID_0x1B;
  else if(!memcmp( protocol, URI_ID_0x1C_STRING, strlen(URI_ID_0x1C_STRING)))  return URI_ID_0x1C;
  else if(!memcmp( protocol, URI_ID_0x1D_STRING, strlen(URI_ID_0x1D_STRING)))  return URI_ID_0x1D;
  else if(!memcmp( protocol, URI_ID_0x1E_STRING, strlen(URI_ID_0x1E_STRING)))  return URI_ID_0x1E;
  else if(!memcmp( protocol, URI_ID_0x1F_STRING, strlen(URI_ID_0x1F_STRING)))  return URI_ID_0x1F;
  else if(!memcmp( protocol, URI_ID_0x20_STRING, strlen(URI_ID_0x20_STRING)))  return URI_ID_0x20;
  else if(!memcmp( protocol, URI_ID_0x21_STRING, strlen(URI_ID_0x21_STRING)))  return URI_ID_0x21;
  else if(!memcmp( protocol, URI_ID_0x22_STRING, strlen(URI_ID_0x22_STRING)))  return URI_ID_0x22;
  else if(!memcmp( protocol, URI_ID_0x23_STRING, strlen(URI_ID_0x23_STRING)))  return URI_ID_0x23;  
  else return URI_ID_0x00; /* No abreviation for this protocol  */
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





