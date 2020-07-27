/**
  ******************************************************************************
  * @file    tagtype5_wrapper.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TAGTYPE5_WRAPPER_H
#define __TAGTYPE5_WRAPPER_H

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF.h"

/** @addtogroup Tag_Type_5
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/**
  * @brief  Tag Type 5 State enumeration definition.
  */
typedef enum
{
  TT5_NO_NDEF = 0,  /**< No data detected in the tag. */
  TT5_INITIALIZED,  /**< Capability container detected. */
  TT5_READ_WRITE,   /**< Read-Write data detected. */
  TT5_READ          /**< Read-Only data message detected. */
} TT5_State;

/** @brief Type5 Tag Capability Container Magic numbers as defined by the NFC Forum. */
typedef enum {
  NFCT5_MAGICNUMBER_E1_CCFILE = 0xE1, /**<  Complete data area can be read by 1-byte block adrdess commands. */
  NFCT5_MAGICNUMBER_E2_CCFILE = 0xE2  /**<  Last part of the data area can be only read by 2-bytes block address commands.\n
                                            The first 256 blocks can be read by 1-byte block address commands. */
} TT5_MagicNumber_t;

/**
  * @brief  Type5 Tag Capability Container structure.
  */
typedef struct
{
  TT5_MagicNumber_t MagicNumber;  /**< CCfile[0]: Magic Number should be E1h or E2h (for extended API) */
  uint8_t Version;                /**< CCfile[1]: Capability container version (b7-b4) and access conditions (b3-b0) */
  uint8_t MemorySize;             /**< CCfile[2]: Memory size, expressed in 8 bytes blocks, set to 0 if tag size is greater than 16kbits. */
  uint8_t TT5Tag;                 /**< CCfile[3]: Additionnal information on the Type5 Tag:\n
                                                  b0: supports `read multiple block` commands\n
                                                  b1: RFU\n
                                                  b2: RFU\n
                                                  b3: supports `lock block` commands\n
                                                  b4: requires the `special frame` format
                                    */
  uint8_t rsved1;                 /**< RFU */
  uint8_t rsved2;                 /**< RFU */
  uint16_t ExtMemorySize;         /**< CCfile[6],CCfile[7]: Memory size, expressed in 8 bytes blocks, when tag size is greater than 16kbits. */
  TT5_State State;                /**< Indicates if a NDEF message is present. */
  uint32_t NDEF_offset;           /**< Indicates the address of a NDEF message in the tag. */
}sCCFileInfo;

/** @brief Type5 Tag Type-Length-Value structure as defined by the NFC Forum */
typedef struct
{
  uint8_t   Type;     /**< NFC Forum message Type */
  uint8_t   Length;   /**< Message length if lesser than 255 bytes */
  uint16_t  Length16; /**< Message length if greater than or equal to 255 bytes */
} TT5_TLV_t;


/* Exported constants --------------------------------------------------------*/
/** @brief Memory size value indicating that this is a 8-bytes Capability Container */
#define NFCT5_EXTENDED_CCFILE             0x00
/** @brief Capability container version 1.0 */
#define NFCT5_VERSION_V1_0                0x40
/** @brief Read access condition mask for the Capability Container byte1 */
#define NFCT5_READ_ACCESS                 0x0C
/** @brief Write access condition mask for the Capability Container byte1 */
#define NFCT5_WRITE_ACCESS                0x03

/** @brief Type5 Tag NDEF message TLV-Type. */
#define NFCT1_LOCK_CONTROL_TLV            ((uint8_t) 0x01)
#define NFCT1_MEMORY_CONTROL_TLV          ((uint8_t) 0x02)
#define NFCT5_NDEF_MSG_TLV                ((uint8_t) 0x03)
/** @brief Type5 Tag Proprietary message TLV-Type. */
#define NFCT5_PROPRIETARY_TLV             ((uint8_t) 0xFD)
/** @brief Type5 Tag Terminator TLV-Type. */
#define NFCT5_TERMINATOR_TLV              ((uint8_t) 0xFE)
/** @brief TLV-Length indicating a 4-bytes TLV (Length coded on 2 bytes). */
#define NFCT5_3_BYTES_L_TLV               ((uint8_t) 0xFF)

/* Exported macro ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
extern sCCFileInfo CCFileStruct;
extern uint16_t ccFileOffset;

/* Exported functions ------------------------------------------------------- */
uint16_t NfcType5_WriteCCFile( const uint8_t * const pCCBuffer );
uint16_t NfcType5_ReadCCFile( uint8_t * const pCCBuffer );
uint16_t NfcType5_TT5Init( void );
uint16_t NfcType5_NDEFDetection( void );
uint16_t NfcType5_ReadNDEF( uint8_t* pData );
uint16_t NfcType5_WriteNDEF(uint16_t Length, uint8_t* pData );
uint16_t NfcType5_GetLength(uint16_t* Length);

#endif /* __TAGTYPE5_WRAPPER_H */

/* @}
 */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
