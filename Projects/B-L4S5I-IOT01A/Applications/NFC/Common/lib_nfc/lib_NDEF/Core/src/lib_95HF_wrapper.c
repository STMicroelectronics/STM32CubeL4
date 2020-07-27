/**
  ******************************************************************************
  * @file    lib_95HF_wrapper.c 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   Interface for xx95HF in order to use NDEF lib
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
#include "lib_wrapper.h"
#include "lib_ndef.h"

#if defined (ST95HF) || defined (CR95HF)
#include "lib_nfctype1pcd.h"
#include "lib_nfctype2pcd.h"
#include "lib_nfctype3pcd.h"
#include "lib_nfctype4pcd.h"
#include "lib_nfctype5pcd.h"
#endif /* defined (ST95HF) || defined (CR95HF) */

 /* Variables for the different modes */
extern DeviceMode_t devicemode;
extern TagType_t nfc_tagtype;

bool updateFlash = false;

/* Tag type 1 */
extern uint8_t TT1Tag[];

/* Tag type 2 */
extern uint8_t TT2Tag[];

/* Tag type 3 */
extern uint8_t TT3Tag[];
extern uint8_t *TT3AttribInfo, *TT3NDEFfile;

/* Tag type 4 */
/* NDEF buffer */
extern uint8_t 	CardNDEFfileT4A [];
extern uint8_t 	CardNDEFfileT4B [];
/* CC buffer*/
extern uint8_t	CardCCfile [];

/* Tag type 5 */
#if defined (CR95HF) || defined (ST95HF)
extern uint8_t TT5Tag[];
#endif

/** @addtogroup NFC_libraries
 * 	@{
 *	@brief  <b>This is the library used to manage the content of the TAG (data)
 *          But also the specific feature of the tag, for instance
 *          password, gpo... </b>
 */


/** @addtogroup libNFC_FORUM
  * @{
	*	@brief  This part of the library manage data which follow NFC forum organisation.
  */


/** @defgroup lib_95HF_Private_Functions
  * @{
  */ 

/**
  * @}
  */ 
	
/** @defgroup lib_95HF_Public_Functions
  * @{
  */

/**
  * @brief  This fonction read the data stored in NDEF file at defined offset.
  * @param  Offset : Offset in the NDEF file.
  * @param  DataSize : Number of byte to read.
  * @param  pData : pointer on buffer to store read data.
  * @retval ACTION_COMPLETED : The operation is completed.
  */
uint16_t ReadData( uint16_t Offset , uint16_t DataSize , uint8_t* pData )
{
  uint16_t i;
  switch( nfc_tagtype )
  {
#if defined (ST95HF) || defined (CR95HF)		
    case TT1:
      if( DataSize > NFCT1_MAX_TAGMEMORY )
        return NDEF_ERROR_MEMORY_INTERNAL;
      for( i = Offset; i < (DataSize+Offset); i++ )
      {
        if( i == 0 ) // We need the size
        {
          /* 2 bytes length */
          if( TT1Tag[17] == 0xFF )
          {
            pData[0] = TT1Tag[18];
            pData[1] = TT1Tag[19];
          }
          /* 1 bytes length */
          else
          {
            pData[0] = 0x00;
            pData[1] = TT1Tag[17];
          }
          i++;
        }
        else // We need the NDEF message
        {
          /* 2 bytes length */
          if( TT1Tag[17] == 0xFF )
            pData[i-Offset] = TT1Tag[18+i];
          /* 1 bytes length */
          else
            pData[i-Offset] = TT1Tag[16+i];
        }
      }
      break;
#endif /* (ST95HF) || (CR95HF) */

    case TT2:
      if( DataSize > NFCT2_MAX_TAGMEMORY )
        return NDEF_ERROR_MEMORY_INTERNAL;
      for( i = Offset; i < (DataSize+Offset); i++ )
      {
        if( i == 0 ) // We need the size
        {
          /* 2 bytes length */
          if( TT2Tag[NFCT2_MAX_CONFIG+NFCT2_MAX_CC_SIZE+1] == 0xFF )
          {
            pData[0] = TT2Tag[NFCT2_MAX_CONFIG+NFCT2_MAX_CC_SIZE+2];
            pData[1] = TT2Tag[NFCT2_MAX_CONFIG+NFCT2_MAX_CC_SIZE+3];
          }
          /* 1 bytes length */
          else
          {
            pData[0] = 0x00;
            pData[1] = TT2Tag[NFCT2_MAX_CONFIG+NFCT2_MAX_CC_SIZE+1];
          }
          i++;
        }
        else // We need the NDEF message
        {
          /* 2 bytes length */
          if( TT2Tag[NFCT2_MAX_CONFIG+NFCT2_MAX_CC_SIZE+1] == 0xFF )
            pData[i-Offset] = TT2Tag[NFCT2_MAX_CONFIG+NFCT2_MAX_CC_SIZE+2+i];
          /* 1 bytes length */
          else
            pData[i-Offset] = TT2Tag[NFCT2_MAX_CONFIG+NFCT2_MAX_CC_SIZE+i];
        }
      }
      break;

    case TT3:
      if( DataSize > NFCT3_MAX_TAGMEMORY )
        return NDEF_ERROR_MEMORY_INTERNAL;
      for( i = Offset; i < (DataSize+Offset); i++ )
      {
        if( i == 0 ) // We need the size
        {
          pData[0] = TT3AttribInfo[12];
          pData[1] = TT3AttribInfo[13];
          i++;
        }
        else
        {
          pData[i-Offset] = *(TT3NDEFfile + i - 2);
        }
      }
      break;

    case TT4A:
      if( DataSize > NFCT4A_MAX_NDEFMEMORY )
        return NDEF_ERROR_MEMORY_INTERNAL;
      memcpy( pData, &CardNDEFfileT4A[Offset], DataSize );
      break;

    case TT4B:
      if( DataSize > NFCT4B_MAX_NDEFMEMORY )
        return NDEF_ERROR_MEMORY_INTERNAL;
      memcpy( pData, &CardNDEFfileT4B[Offset], DataSize );
      break;

#if defined (CR95HF) || defined (ST95HF)
    case TT5:
      if( DataSize > NFCT5_MAX_TAGMEMORY )
        return NDEF_ERROR_MEMORY_INTERNAL;
      for( i = Offset; i < (DataSize+Offset); i++ )
      {
        if( i == 0 ) // We need the size
        {
          /* 2 bytes length */
          if( TT5Tag[5] == 0xFF )
          {
            pData[0] = TT5Tag[6];
            pData[1] = TT5Tag[7];
          }
          /* 1 bytes length */
          else
          {
            pData[0] = 0x00;
            pData[1] = TT5Tag[5];
          }
          i++;
        }
        else // We need the NDEF message
        {
          /* 2 bytes length */
          if( TT5Tag[5] == 0xFF )
            pData[i-Offset] = TT5Tag[6+i];
          /* 1 bytes length */
          else
            pData[i-Offset] = TT5Tag[4+i];
        }
      }
      break;
#endif 

    default:
      break;
  }
  return NDEF_OK;
}

/**
  * @brief  This fonction write data in NDEF file at defined offset.
  * @param  Offset : Offset in the NDEF file.
  * @param  DataSize : Number of byte to write.
  * @param  pData : pointer on buffer to copy.
  * @retval ACTION_COMPLETED : The operation is completed.
  */
uint16_t WriteData( uint16_t Offset, uint32_t DataSize, uint8_t* pData )
{
  uint16_t index = 0, i, sum = 0;
  switch( nfc_tagtype )
  {
#if defined (ST95HF) || defined (CR95HF)
    case TT1:
      if( DataSize > NFCT1_MAX_TAGMEMORY )
        return NDEF_ERROR_MEMORY_INTERNAL;
      index = Offset + 14;
      DataSize -= 2;
      /* Write the NDEF TLV [0x03 length message] */
      TT1Tag[index++] = 0x03;
      /* Size can be stored in one byte or 3 bytes depending on the length */
      if( DataSize < 255 )
      {
        TT1Tag[index++] = DataSize;
      }
      else
      {
        TT1Tag[index++] = 0xFF;
        TT1Tag[index++] = ( DataSize&0x0000FF00 ) >> 8;
        TT1Tag[index++] = DataSize & 0x000000FF;
      }
      memcpy( &(TT1Tag[index]), pData+2, DataSize );
      /* Write the terminating TLV [0xFE] */
      TT1Tag[index+DataSize] = 0xFE;
      /* Update the phisical tag if needed */
      #if defined(ST95HF) || defined(CR95HF)
      if( devicemode == PCD )
        return PCDNFCT1_WriteNDEF( );
      #endif
      break;
#endif /* (ST95HF) || (CR95HF) */

    case TT2:
      if( DataSize > NFCT2_MAX_TAGMEMORY )
        return NDEF_ERROR_MEMORY_INTERNAL;
      index = Offset + NFCT2_MAX_CONFIG + NFCT2_MAX_CC_SIZE;
      DataSize -= 2;
      /* Write the NDEF TLV [0x03 length message] */
      TT2Tag[index++] = 0x03;
      /* Size can be stored in one byte or 3 bytes depending on the length */
      if( DataSize < 255 )
      {
        TT2Tag[index++] = DataSize;
      }
      else
      {
        TT2Tag[index++] = 0xFF;
        TT2Tag[index++] = ( DataSize&0x0000FF00 ) >> 8;
        TT2Tag[index++] = DataSize & 0x000000FF;
      }
      memcpy( &(TT2Tag[index]), pData+2, DataSize );
      /* Write the terminating TLV [0xFE] */
      TT2Tag[index+DataSize] = 0xFE;
      /* Update the phisical tag if needed */
      #if defined(ST95HF) || defined(CR95HF)
      if( devicemode == PCD )
        return PCDNFCT2_WriteNDEF( );
      else
      #endif
        updateFlash = true;
      break;

    case TT3:
      if( DataSize > NFCT3_MAX_TAGMEMORY )
        return NDEF_ERROR_MEMORY_INTERNAL;
      DataSize -= 2;
      /* Write the ndef message first */
      memcpy( TT3NDEFfile, pData+2, DataSize );
      /* Update AttribInfo */
      TT3AttribInfo[11] = ( DataSize&0x00FF0000 ) >> 16;
      TT3AttribInfo[12] = ( DataSize&0x0000FF00 ) >> 8;
      TT3AttribInfo[13] = DataSize & 0x000000FF;
      /* Update checksum */
      for( i = 0; i < 14; i++ )
        sum += TT3AttribInfo[i];
      TT3AttribInfo[14] = ( sum&0xFF00 ) >> 8;
      TT3AttribInfo[15] = sum & 0x00FF;
      /* Update the phisical tag if needed */
      #if defined(ST95HF) || defined(CR95HF)
      if( devicemode == PCD )
        return PCDNFCT3_WriteNDEF( );
      else
      #endif
        updateFlash = true;
      break;

    case TT4A:
      if( DataSize > NFCT4A_MAX_NDEFMEMORY )
        return NDEF_ERROR_MEMORY_INTERNAL;
      memcpy( &(CardNDEFfileT4A[Offset]), pData, DataSize );
      /* Update the phisical tag if needed */
      #if defined(ST95HF) || defined(CR95HF)
      if( devicemode == PCD )
        return PCDNFCT4_WriteNDEF( );
      else
      #endif
        updateFlash = true;
      break;

    case TT4B:
      if( DataSize > NFCT4_MAX_NDEFMEMORY )
        return NDEF_ERROR_MEMORY_INTERNAL;
      memcpy( &(CardNDEFfileT4B[Offset]), pData, DataSize );
      /* Update the phisical tag if needed */
      #if defined(ST95HF) || defined(CR95HF)
      if( devicemode == PCD )
        return PCDNFCT4_WriteNDEF( );
      else
      #endif
        updateFlash = true;
      break;

#if defined (CR95HF) || defined (ST95HF)
    case TT5:
      if( DataSize > NFCT5_MAX_TAGMEMORY )
        return NDEF_ERROR_MEMORY_INTERNAL;
      index = 4;
      DataSize -= 2;
      /* Write the NDEF TLV [0x03 length message] */
      TT5Tag[index++] = 0x03;
      /* Size can be stored in one byte or 3 bytes depending on the length */
      if( DataSize < 255 )
      {
        TT5Tag[index++] = DataSize;
      }
      else
      {
        TT5Tag[index++] = 0xFF;
        TT5Tag[index++] = ( DataSize&0x0000FF00 ) >> 8;
        TT5Tag[index++] = DataSize & 0x000000FF;
      }
      memcpy( &(TT5Tag[index]), pData + 2, DataSize );
      /* Write the terminating TLV [0xFE] */
      TT5Tag[index+DataSize] = 0xFE;
      /* Update the phisical tag if needed */
      #if defined(ST95HF) || defined(CR95HF)
      if( devicemode == PCD )
        return PCDNFCT5_WriteNDEF( );
      #endif
      break;
      #endif

    default:
      break;
  }
  return NDEF_OK;
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
