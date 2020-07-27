/**
  ******************************************************************************
  * @file    tagtype5_wrapper.c 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file provides an abstraction layer to the libNDEF for the NFC Forum Type5 Tag.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
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
#include "lib_NDEF_config.h"
#include "tagtype5_wrapper.h"

/** @addtogroup Tag_Type_5 Type 5 Tag library
 *  @ingroup libNFC_FORUM
 *  @{
 *  @brief    This module provides an abstraction layer to the NDEF library for the NFC Forum Type5 Tag.
 *  @details  These functions manage the specificities of the Type5 Tag accesses.\n
 *            The NFC Forum defines specific Capability Containers for the Type5 Tags.
 *            The Capability Container provides basic information on how a tag can be accessed.
 *            Depending on the memory size of the tag, 2 different CC will be used:
 *             1. 4-bytes Capability Container (Memory Size < 16kbits):
 *             |Byte0         |Byte1                        |Byte2        |Byte3           |
 *             |:------------:|:---------------------------:|:-----------:|:--------------:|
 *             | Magic Number | Version & Access Conditions | Memory Size | NFC Type 5 Tag |
 *             2. 8-bytes Capability Container (Memory Size > 16kbits):
 *             |Byte0         |Byte1                        |Byte2|Byte3           |Byte4|Byte5|Byte6      |Byte7      |
 *             |:------------:|:---------------------------:|:---:|:--------------:|:---:|:---:|:---------:|:---------:|
 *             | Magic Number | Version & Access Conditions | 00h | NFC Type 5 Tag |RFU  |RFU  |Memory Size|Memory Size|
 *            @section Type5_Tag_Usage How to write a Type5 Tag Capability Container
 *            1. Initialize the Capacity Container structure with the Tag compliant values:
                 - Example for a 4kbits memory tag:

                        CCFileStruct.MagicNumber  = NFCT5_MAGICNUMBER_E1_CCFILE;
                        CCFileStruct.Version      = NFCT5_VERSION_V1_0;
                        CCFileStruct.MemorySize   = 0x40;
                        CCFileStruct.TT5Tag       = 0x0;

                 - Example for a 64kbits memory tag:

                        CCFileStruct.MagicNumber   = NFCT5_MAGICNUMBER_E2_CCFILE;
                        CCFileStruct.Version       = NFCT5_VERSION_V1_0;
                        CCFileStruct.MemorySize    = NFCT5_EXTENDED_CCFILE;
                        CCFileStruct.ExtMemorySize = 0x400;
                        CCFileStruct.TT5Tag        = 0x1;

              2. Call the `NfcType5_TT5Init` function to write the Capability Container to the Tag:

                        NfcType5_TT5Init();

 */

/** @brief Capability Container structure instance (global). */
sCCFileInfo CCFileStruct;
uint16_t ccFileOffset = 0;

/**
  * @brief  This function reads the data stored in the NDEF message.
  * @param	pData Pointer on the buffer used to store the read data.
  * @retval NDEF_ERROR_MEMORY_INTERNAL  The buffer is too small for the NDEF message.
  * @retval NDEF_ERROR_NOT_FORMATED     No Capability Container detected.
  * @retval NDEF_ERROR                  Error when reading the NDEF message.
  * @retval NDEF_OK                     NDEF message successfully read.
  */
uint16_t NfcType5_ReadNDEF( uint8_t* pData )
{
  uint16_t status = NDEF_ERROR;
  TT5_TLV_t tlv;
  uint8_t tlv_size = 0;
  uint16_t DataLength;

  /* Detect NDEF message in memory */
  status = NfcType5_NDEFDetection( );
  if( status != NDEF_OK )
  {
    return status;
  }
  
  /* Read TL of Type 5 */
  status = NDEF_Wrapper_ReadData( (uint8_t*)&tlv, CCFileStruct.NDEF_offset, sizeof(TT5_TLV_t) );
  if( status != NDEF_OK )
  {
    return status;
  }
  
  /* Check if L is on 3 or 1 byte and update length in buffer */
  if( tlv.Length == NFCT5_3_BYTES_L_TLV )
  {
    tlv_size = 4;
    DataLength = ((tlv.Length16 >> 8)&0xff) | ((tlv.Length16&0xff)<<8);
  }
  else
  {
    tlv_size = 2;
    DataLength = tlv.Length;
  }
      /* If too many data to write return error */
  if( DataLength > NDEF_MAX_SIZE )
  {
    return NDEF_ERROR_MEMORY_INTERNAL;
  }
  
  /* Check CC file is in the correct mode to proceed */
  if( CCFileStruct.State ==  TT5_INITIALIZED )
  {
    return NDEF_OK;
  }

  if( DataLength > 0 )
  {
    /* Read NDEF */
    if( NDEF_Wrapper_ReadData( (pData), CCFileStruct.NDEF_offset + tlv_size, DataLength ) != NDEF_OK )
    {
      return NDEF_ERROR;
    }
  }
  
  return NDEF_OK;
}

/**
  * @brief  This function writes data in the NFC Tag.
  * @param	Type    Type of the data: NDEF or Proprietary
  * @param	Length  Number of bytes to write.
  * @param	pData   Pointer on the buffer to copy.
  * @retval NDEF_ERROR_MEMORY_INTERNAL Memory size is too small for the data.
  * @retval NDEF_ERROR_NOT_FORMATED    No Capability Container detected.
  * @retval NDEF_ERROR                 Error when writing the Tag.
  * @retval NDEF_OK                    The data has been successfully written.
  */
uint16_t NfcType5_WriteData(uint8_t Type, uint16_t Length , uint8_t *pData )
{
  TT5_TLV_t tlv;
  uint8_t tlv_size;
  uint32_t offset;
  uint8_t NfcT5_Terminator = NFCT5_TERMINATOR_TLV;

  uint32_t max_length = NDEF_Wrapper_GetMemorySize()        /* Memory size */
                        - ((Length >= 0xFF) ? 4 : 2)    /* - TLV length */
                        - sizeof(NfcT5_Terminator)      /* - Terminator TLV */
                        - CCFileStruct.NDEF_offset;     /* - CCfile length */

  /* If too many data to write return error */
  if( Length > max_length )
  {
    return NDEF_ERROR_MEMORY_TAG;
  }
  
  /* Detect NDEF message in memory */
  if( NfcType5_NDEFDetection( ) != NDEF_OK )
  {
    return NDEF_ERROR;
  }
  
  /* Prepare TLV */
  tlv.Type = Type;
  if(Length >= 0xFF)
  {
    tlv.Length = NFCT5_3_BYTES_L_TLV;
    tlv.Length16 = ((Length&0xff)<<8) | ((Length>>8)&0xff) ;
    tlv_size = 4;
    
  } else {
    tlv.Length = Length;
    tlv_size = 2;
  }

  offset = CCFileStruct.NDEF_offset;
  /* Start write TLV to EEPROM */
  if(NDEF_Wrapper_WriteData( (uint8_t*)&tlv, offset, tlv_size )!= NDEF_OK)
    return NDEF_ERROR;
  offset += tlv_size;

  /* Continue write TLV data  to EEPROM */
  if(NDEF_Wrapper_WriteData( pData , offset, Length ) != NDEF_OK )
    return NDEF_ERROR;
  offset +=Length;
  
  /* Write Terminator TLV */
  if(NDEF_Wrapper_WriteData( &NfcT5_Terminator, offset, sizeof(NfcT5_Terminator) ) != NDEF_OK)
    return NDEF_ERROR;
  
  return NDEF_OK;

}

/**
  * @brief  This function writes a NDEF message in the NFC Tag.
  * @param	Length Number of bytes to write.
  * @param	pData  Pointer on the buffer to copy.
  * @retval NDEF_ERROR_MEMORY_INTERNAL Memory size is too small for the data.
  * @retval NDEF_ERROR_NOT_FORMATED    No Capability Container detected.
  * @retval NDEF_ERROR                 Error when writing the Tag.
  * @retval NDEF_OK                    The data has been successfully written.
  */
uint16_t NfcType5_WriteNDEF(uint16_t Length , uint8_t *pData )
{
  return NfcType5_WriteData(NFCT5_NDEF_MSG_TLV,Length,pData);
}

/**
  * @brief  This function writes a Proprietary message to the NFC Tag.
  * @param	Length Number of byte to write.
  * @param	pData  Pointer on the buffer to copy.
  * @retval NDEF_ERROR_MEMORY_INTERNAL Memory size is too small for the data.
  * @retval NDEF_ERROR_NOT_FORMATED    No Capability Container detected.
  * @retval NDEF_ERROR                 Error when writing the Tag.
  * @retval NDEF_OK                    The data has been successfully written.
  */
uint16_t NfcTag_WriteProprietary(uint16_t Length , uint8_t *pData )
{
  return NfcType5_WriteData(NFCT5_PROPRIETARY_TLV,Length,pData);
}


/**
  * @brief  This functions writes the Capability Container in the NFC Tag.
  * @param  pCCBuffer Pointer on the buffer containnig the Capability Container.
  * @retval NDEF_ERROR Error when writing the Tag.
  * @retval NDEF_OK    The CC has been successfully written.
  */
uint16_t NfcType5_WriteCCFile( const uint8_t * const pCCBuffer )
{
  int32_t ret_value;
  
  /* Write first block of CCFile */
  ret_value = NDEF_Wrapper_WriteData( pCCBuffer, 0x00, 0x4 );
 
  /* If extended memory writes the next 4 bytes */
  if( (pCCBuffer[2] == 0x00) && (ret_value == NDEF_OK) )
  {
    ret_value = NDEF_Wrapper_WriteData( pCCBuffer + 4, 0x04, 4 );
  }

  if( ret_value != NDEF_OK )
  {
    return NDEF_ERROR;
  }
  
	return NDEF_OK;
}

/**
  * @brief  This functions reads the Capability Container from the NFC Tag.
  * @param  pCCBuffer Pointer on the buffer used to store the CC.
  * @retval NDEF_ERROR Error when reading the Tag.
  * @retval NDEF_OK    The CC has been successfully read.
  */
uint16_t NfcType5_ReadCCFile( uint8_t * const pCCBuffer )
{
  int32_t ret_value;
  
  /* Read 4 bytes of CC File */
  ret_value = NDEF_Wrapper_ReadData( pCCBuffer, ccFileOffset, 4 );

  /* If extended memory reads the next 4 bytes */
  if( (pCCBuffer[2] == 0x00) && (ret_value == NDEF_OK) )
  {
    ret_value = NDEF_Wrapper_ReadData( pCCBuffer + 4, ccFileOffset + 0x04, 4 );
  }
  
  if( ret_value != NDEF_OK )
  {
    return NDEF_ERROR;
  }
  
	return NDEF_OK;
}

/**
  * @brief  This function initializes the Capability Container and an empty NDEF message in a NFC Tag.
  * @details The Capability Container content is defined by the global variable `CCFileStruct`.
  * @retval NDEF_ERROR The Tag has not been initialized.
  * @retval NDEF_OK    The Tag has been successfully initialized.
  */
uint16_t NfcType5_TT5Init( void )
{
  int32_t ret_value = NDEF_OK;
  uint16_t status;
  uint8_t accbuffer[8];
  uint8_t cdata;

  /* Prepare buffer to update CCFile */
  accbuffer[0] = CCFileStruct.MagicNumber;
  accbuffer[1] = CCFileStruct.Version;
  accbuffer[2] = CCFileStruct.MemorySize;
  accbuffer[3] = CCFileStruct.TT5Tag;
  CCFileStruct.NDEF_offset = ccFileOffset + 0x04;
  
  /* If extended memory prepare the length bytes */
  if( CCFileStruct.MemorySize == NFCT5_EXTENDED_CCFILE )
  {
    accbuffer[6] = (uint8_t)(CCFileStruct.ExtMemorySize >> 8);
    accbuffer[7] = (uint8_t)(CCFileStruct.ExtMemorySize & 0xFF);
    CCFileStruct.NDEF_offset = ccFileOffset + 0x08;
  }
  
  /* Update CCFile */
  status = NfcType5_WriteCCFile( accbuffer );
  if( status != NDEF_OK )
  {
    return status;
  }
  
  /* Update NDEF TLV for INITIALIZED state */
  /* Update T */
  cdata = NFCT5_NDEF_MSG_TLV;
  ret_value = NDEF_Wrapper_WriteData( &cdata, CCFileStruct.NDEF_offset, 1 );
  if( ret_value != NDEF_OK )
  {
    return NDEF_ERROR;
  }

  /* Update L */
  cdata = 0x00;
  ret_value = NDEF_Wrapper_WriteData( &cdata, (CCFileStruct.NDEF_offset + 1), 1 );
  if( ret_value != NDEF_OK )
  {
    return NDEF_ERROR;
  }
  
  return NDEF_OK;
}

/**
  * @brief    This function detects a NDEF message in a Type 5 Tag.
  * @details  It first detects the Capability Container and then look for the NDEF TLV.
  *           The `CCfileStruct` global variable is updated accordingly with what is detected.
  * @retval NDEF_OK                 NDEF message Tag Type 5 detected.
  * @retval NDEF_ERROR_NOT_FORMATED Device is not a NFC Tag Type 5 Tag.
  */
uint16_t NfcType5_NDEFDetection( void )
{
  uint8_t acc_buffer[8];
  TT5_TLV_t tlv_detect;
  uint16_t status;
  uint32_t memory_size;
  
  CCFileStruct.State = TT5_NO_NDEF;
  
  /* Read CCFile */
  status = NfcType5_ReadCCFile( acc_buffer );
  if( status != NDEF_OK )
  {
    return status;
  }
  
  /* Check Byte 0 is equal to magic number */
  if( ( acc_buffer[0] != NFCT5_MAGICNUMBER_E1_CCFILE ) && ( acc_buffer[0] != NFCT5_MAGICNUMBER_E2_CCFILE ) )
  {
    return NDEF_ERROR_NOT_FORMATED;
  }
  /* Check Version number */
  else if( ( (acc_buffer[1]&0xFC) != 0x40 ) && ((acc_buffer[1]&0xFC) != 0x10 ) )
  {
    return NDEF_ERROR_NOT_FORMATED;
  }
  
  /* Check if CCFile is on 4 Bytes or 8 Bytes */
  if( acc_buffer[2] == 0x00 )
  {
    /* Update CCFIle structure */
    CCFileStruct.MemorySize = 0x0;
    CCFileStruct.ExtMemorySize = (uint16_t)acc_buffer[6];
    CCFileStruct.ExtMemorySize = ( CCFileStruct.ExtMemorySize << 8 ) |  acc_buffer[7];
    memory_size = CCFileStruct.ExtMemorySize;
    CCFileStruct.NDEF_offset = ccFileOffset + 8;
  }
  else
  {
    /* Update CCFIle structure */
    CCFileStruct.MemorySize = acc_buffer[2];
    CCFileStruct.ExtMemorySize = 0x0;
    memory_size = CCFileStruct.MemorySize;
    CCFileStruct.NDEF_offset = ccFileOffset + 4;
  }
  
  /* Update CCFIle structure */
  CCFileStruct.MagicNumber = (TT5_MagicNumber_t)acc_buffer[0];
  CCFileStruct.Version = acc_buffer[1];
  CCFileStruct.TT5Tag = acc_buffer[3];
  
  /* Search for position of NDEF TLV in memory and tag status */
  while( ( NDEF_Wrapper_ReadData( (uint8_t *)&tlv_detect, CCFileStruct.NDEF_offset, sizeof(TT5_TLV_t) ) == NDEF_OK ) && ( CCFileStruct.NDEF_offset < memory_size ) )
  {
    /* Detect first NDEF Message in memory */
    if( tlv_detect.Type == NFCT5_NDEF_MSG_TLV )
    {
      if( tlv_detect.Length == 0x00 )
      {
        CCFileStruct.State = TT5_INITIALIZED;
      }
      else
      {
        if( CCFileStruct.Version & 0x3 )
        {
          CCFileStruct.State = TT5_READ;
        }
        else
        {
          CCFileStruct.State = TT5_READ_WRITE;
        }
      }
      return NDEF_OK;
    }
    /* If Proprietary NDEF jump to end of proprietary message */
    else if(( tlv_detect.Type == NFCT5_PROPRIETARY_TLV ) || 
            ( tlv_detect.Type == NFCT1_LOCK_CONTROL_TLV ) ||
            ( tlv_detect.Type == NFCT1_MEMORY_CONTROL_TLV ))
    {
      if( tlv_detect.Length == NFCT5_3_BYTES_L_TLV )
      {
        CCFileStruct.NDEF_offset = CCFileStruct.NDEF_offset + tlv_detect.Length16 + 4;
        continue;
      }
      else
      {
        CCFileStruct.NDEF_offset = CCFileStruct.NDEF_offset + tlv_detect.Length + 2;
        continue;
      }
    }
    /* if Terminator no NDEF detected */
    else if( tlv_detect.Type == NFCT5_TERMINATOR_TLV )
    {
      return NDEF_ERROR_NOT_FORMATED;
    }
      
    CCFileStruct.NDEF_offset++;
  }
  
  return NDEF_ERROR_NOT_FORMATED;
}



/**
  * @brief  This function reads and return the size of the NDEF message in the NFC tag.
  * @param	Length Pointer on the NDEF size to be returned.
  * @retval NDEF_ERROR_NOT_FORMATED Device is not a NFC Tag Type 5 Tag.
  * @retval NDEF_ERROR              The NDEF message size has not been read.
  * @retval NDEF_OK                 The NDEF message size has been retrieved.
  */
uint16_t NfcType5_GetLength(uint16_t* Length)
{
  
  uint16_t status = NDEF_ERROR;
  TT5_TLV_t tlv;
  
  /* Detect NDEF message in memory */
  status = NfcType5_NDEFDetection( );
  if( status != NDEF_OK )
  {
    return status;
  }
  
  /* Read TL of Type 5 */
  status = NDEF_Wrapper_ReadData( (uint8_t*)&tlv, CCFileStruct.NDEF_offset, sizeof(TT5_TLV_t) );
  if( status != NDEF_OK )
  {
    return NDEF_ERROR;
  }
  
  if(tlv.Length != NFCT5_3_BYTES_L_TLV)
  {
    *Length = tlv.Length;
  } else {
    *Length = ((tlv.Length16 >> 8)&0xff) | ((tlv.Length16 & 0xff) << 8);
  }
  
  return NDEF_OK;
  
}


/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
