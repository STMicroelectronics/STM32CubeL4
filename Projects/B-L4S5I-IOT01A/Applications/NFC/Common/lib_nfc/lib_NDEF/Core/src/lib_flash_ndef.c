/**
  ******************************************************************************
  * @file    lib_flash_ndef.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file provides set of functions to write or read the NDEF file into flash. 
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

#include "lib_flash_ndef.h"

/* Variables for the different modes */
extern DeviceMode_t devicemode;
extern TagType_t nfc_tagtype;

/* Variables for buffers */
/**** TT2 ****/
extern uint8_t TT2Tag[];
#define TT2_DEFAULT_SIZE 30
static uc8 TT2Default[TT2_DEFAULT_SIZE] ={
//0x02,0x80,0x74,0x7E,0x4A,0xEF,0x22,0x80,0x07,0x00,0x00,0x00,      /* UID  */
0x02,0x80,0x74,0xDA,0x4A,0xEF,0x22,0x80,0xF1,0x48,0x00,0x00,      /* UID  */
0xE1,0x10,(NFCT2_MAX_NDEFFILE/8),0x00,                            /* CC   */
0x03,0x0B,0xD1,0x01,0x07,0x55,0x01,'s','t','.','c','o','m',0xFE}; /* NDEF */

/**** TT3 ****/
extern uint8_t TT3Tag[];
#define TT3_DEFAULT_SIZE 32
static uc8 TT3Default[TT3_DEFAULT_SIZE] = {
0x10,0x08,0x08,NFCT3_NB_BLOC_MSB,NFCT3_NB_BLOC_LSB,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x0B,0x00,0x2C+NFCT3_NB_BLOC_LSB, /* AttribInfo */
0xD1,0x01,0x07,0x55,0x01, 's', 't', '.', 'c', 'o', 'm',0x00,0x00,0x00,0x00,0x00};/* NDEF       */

/**** TT4 ****/
extern uint8_t CardNDEFfileT4A [];
extern uint8_t CardNDEFfileT4B [];
#define TT4_DEFAULT_SIZE 13
static uc8 TT4Default[TT4_DEFAULT_SIZE] = {
0x00,0x0B,0xD1,0x01,0x07,0x55,0x01,'s','t','.','c','o','m'}; /* NDEF */

/* Flags used by the library */
extern bool updateFlash;

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


/** @defgroup lib_flash_ndef_Private_Functions
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup lib_flash_ndef_Public_Functions
  * @{
  */

/**
  * @brief  This function initialize the RAM buffer with FLASH content if available
  */
void initFlashNDEF(void)
{
  if( readNDEFFromFlash( TT2Tag, NFCT2_MAX_TAGMEMORY, PICCNFCT2_NDEF_ADDRESS ) == FLASH_NO_NDEF )
  {
    memcpy( TT2Tag, TT2Default, TT2_DEFAULT_SIZE );
    writeNDEFToFlash( TT2Tag, NFCT2_MAX_TAGMEMORY, PICCNFCT2_NDEF_ADDRESS );
  }
  if( readNDEFFromFlash( TT3Tag, NFCT3_MAX_TAGMEMORY, PICCNFCT3_NDEF_ADDRESS ) == FLASH_NO_NDEF )
  {
    memcpy( TT3Tag, TT3Default, TT3_DEFAULT_SIZE );
    writeNDEFToFlash( TT3Tag, NFCT3_MAX_TAGMEMORY, PICCNFCT3_NDEF_ADDRESS );
  }
  if( readNDEFFromFlash( CardNDEFfileT4A, NFCT4A_MAX_NDEFMEMORY, PICCNFCT4A_NDEF_ADDRESS ) == FLASH_NO_NDEF )
  {
    memcpy( CardNDEFfileT4A, TT4Default, TT4_DEFAULT_SIZE );
    writeNDEFToFlash( CardNDEFfileT4A, NFCT4A_MAX_NDEFMEMORY, PICCNFCT4A_NDEF_ADDRESS );
  }
  if( readNDEFFromFlash( CardNDEFfileT4B, NFCT4B_MAX_NDEFMEMORY, PICCNFCT4B_NDEF_ADDRESS ) == FLASH_NO_NDEF )
  {
    memcpy( CardNDEFfileT4B, TT4Default, TT4_DEFAULT_SIZE );
    writeNDEFToFlash( CardNDEFfileT4B, NFCT4B_MAX_NDEFMEMORY, PICCNFCT4B_NDEF_ADDRESS );
  }
}

/**
  * @brief  This function will write the correct buffer to the flash if needed.
  */
void manageFlashNDEF( void )
{
  #ifndef DISABLE_NDEF_FLASH
  if( updateFlash )
  {
    switch( nfc_tagtype )
    {
      case TT2:
        writeNDEFToFlash( TT2Tag, NFCT2_MAX_TAGMEMORY, PICCNFCT2_NDEF_ADDRESS );
        break;
      case TT3:
        writeNDEFToFlash( TT3Tag, NFCT3_MAX_TAGMEMORY, PICCNFCT3_NDEF_ADDRESS );
        break;
      case TT4A:
        writeNDEFToFlash( CardNDEFfileT4A, NFCT4A_MAX_NDEFMEMORY, PICCNFCT4A_NDEF_ADDRESS );
        break;
      case TT4B:
        writeNDEFToFlash( CardNDEFfileT4B, NFCT4B_MAX_NDEFMEMORY, PICCNFCT4B_NDEF_ADDRESS );
        break;
      default:
        break;
    }
    updateFlash = false;
  }	
  #endif
}

/**
  * @brief  This function writes the NDEF file from the RAM to the FLASH.
  * @param  dataToWrite : The buffer which contains the memory to be written.
  * @param  size : The buffer size.
  * @param  address : The destination address inside the FLASH.
  * @retval 	FLASH_OK : The file is written successfully.
  * @retval 	FLASH_ERROR_SIZE : The NDEF size is higher than the FLASH available.
  */
uint8_t writeNDEFToFlash( uint8_t *dataToWrite, uint32_t size, uint32_t address )
{
  #ifndef DISABLE_NDEF_FLASH
  int32_t j;
  uint32_t data=0;
  uint32_t *pdata;
  uint32_t EraseCounter = 0x0;
  uint32_t NbrOfPage = 0;
  FLASH_Status FLASHStatus = FLASH_COMPLETE;

  /* Test the size of the image to be sent */
  /* Image size is greater than Flash size */
  if( !IS_FLASH_ADDRESS( size + FLASH_MAGIC_NUMBER_LENTH + address ) )
  {
    return FLASH_ERROR_SIZE;
  }

  /* Unlock the FLASH */
  FLASH_Unlock( );

  /* Clear All pending flags */
  FLASH_ClearFlag( FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR );

  /* Erase the needed pages where the user application will be loaded */
  /* Define the number of page to be erased */
  NbrOfPage = FLASH_PagesMask( size + FLASH_MAGIC_NUMBER_LENTH );

  /* Erase the FLASH pages */
  for( EraseCounter = 0; ( EraseCounter < NbrOfPage ) && ( FLASHStatus == FLASH_COMPLETE ); EraseCounter++ )
  {
    FLASHStatus = FLASH_ErasePage( address + (PAGE_SIZE * EraseCounter) );
  }
  pdata = (uint32_t*)(dataToWrite);

  /* Write the magic number */
  FLASH_ProgramWord( address,FLASH_MAGIC_NUMBER );
  address += FLASH_MAGIC_NUMBER_LENTH;

  for( j = 0; j < size; j += 4 )
  {
    data = *pdata;
    FLASH_ProgramWord( address,data );
    pdata++;
    address += 4;
  }
  /* Lock the FLASH */
  FLASH_Lock( );
  #endif
  return FLASH_OK;
}

/**
  * @brief  This function reads the NDEF file from the FLASH to the RAM.
  * @param  dataToRead : The buffer which will contains the memory read.
  * @param  size : The buffer size.
  * @param  address : The destination address inside the FLASH.
  * @retval 	FLASH_OK : The file is read successfully.
  * @retval 	FLASH_NO_NDEF : The NDEF file is not present on the FLASH.
  */
uint8_t readNDEFFromFlash( uint8_t *dataToRead, uint32_t size, uint32_t address )
{
  #ifndef DISABLE_NDEF_FLASH
  /* If the flash does not contain any NDEF file */
  if( *((uint32_t*)(address)) != FLASH_MAGIC_NUMBER )
  {
    return FLASH_NO_NDEF;
  }

  /* Read size of the NDEF file */
  if( size == 0 )
    return FLASH_NO_NDEF;

  address += FLASH_MAGIC_NUMBER_LENTH;
  /* Read from flash the NDEF file */
  memcpy( dataToRead, (uint32_t*)(address), size );

  return FLASH_OK;
  #else
  return FLASH_NO_NDEF;
  #endif
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
