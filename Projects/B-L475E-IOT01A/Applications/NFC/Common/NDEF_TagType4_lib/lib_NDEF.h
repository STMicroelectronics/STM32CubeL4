/**
  ******************************************************************************
  * @file    lib_NDEF.h
  * @author  MMY Application Team
  * @brief   This file help to manage NDEF file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIB_NDEF_H
#define __LIB_NDEF_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup NFC_libraries
  * @{
  */

/** @addtogroup TAGTYPE4_NDEF_COMMON
 *  @{
 */

/**
 * @defgroup TAGTYPE4_NDEF_COMMON_Exported_Types             TAGTYPE4_NDEF_COMMON Exported Types
 * @{
 */
 
/**
  * @brief  NDEF type supported
  */
typedef enum 
{
  UNKNOWN_TYPE = 0,
  VCARD_TYPE,
  WELL_KNOWN_ABRIDGED_URI_TYPE,
  URI_SMS_TYPE,
  URI_GEO_TYPE,
  URI_EMAIL_TYPE,
  SMARTPOSTER_TYPE,
  URL_TYPE,
  TEXT_TYPE,
  BT_TYPE,
  /* list of "external type" known by this demo, other external type will be addressed as UNKNWON_TYPE */
  M24SR_DISCOVERY_APP_TYPE 
} NDEF_TypeDef;


#define SP_MAX_RECORD              3
/**
  * @brief  structure to store record information
  */
typedef struct
{
  uint8_t RecordFlags;
  uint8_t TypeLength;
  uint8_t PayloadLength3;
  uint8_t PayloadLength2;
  uint8_t PayloadLength1;
  uint8_t PayloadLength0;
  uint8_t IDLength;
  uint8_t Type[0xFF];
  uint8_t ID[0xFF];
  uint16_t PayloadOffset;
  uint32_t PayloadBufferAdd;    /* add where payload content has been stored */
  NDEF_TypeDef NDEF_Type;       /* to store identification ID for application */
  void *SPRecordStructAdd[SP_MAX_RECORD]; /*in case of smart poster array to store add of other sRecordInfo struct */
  uint8_t NbOfRecordInSPPayload;
}sRecordInfo;     

/**
  * @}
  */


/** @defgroup TAGTYPE4_NDEF_COMMON_URI_ID_Constants   TAGTYPE4_NDEF_COMMON URI_ID Constants
  * @{
  */
#define URI_ID_0x00                0x00
#define URI_ID_0x01                0x01
#define URI_ID_0x02                0x02
#define URI_ID_0x03                0x03
#define URI_ID_0x04                0x04
#define URI_ID_0x05                0x05
#define URI_ID_0x06                0x06
#define URI_ID_0x07                0x07
#define URI_ID_0x08                0x08
#define URI_ID_0x09                0x09
#define URI_ID_0x0A                0x0A
#define URI_ID_0x0B                0x0B
#define URI_ID_0x0C                0x0C
#define URI_ID_0x0D                0x0D
#define URI_ID_0x0E                0x0E
#define URI_ID_0x0F                0x0F
#define URI_ID_0x10                0x10
#define URI_ID_0x11                0x11
#define URI_ID_0x12                0x12
#define URI_ID_0x13                0x13
#define URI_ID_0x14                0x14
#define URI_ID_0x15                0x15
#define URI_ID_0x16                0x16
#define URI_ID_0x17                0x17
#define URI_ID_0x18                0x18
#define URI_ID_0x19                0x19
#define URI_ID_0x1A                0x1A
#define URI_ID_0x1B                0x1B
#define URI_ID_0x1C                0x1C
#define URI_ID_0x1D                0x1D
#define URI_ID_0x1E                0x1E
#define URI_ID_0x1F                0x1F
#define URI_ID_0x20                0x20
#define URI_ID_0x21                0x21
#define URI_ID_0x22                0x22
#define URI_ID_0x23                0x23
#define URI_RFU                    0x24
/**
  * @}
  */

/** @defgroup TAGTYPE4_NDEF_COMMON_URI_ID_Strings  TAGTYPE4_NDEF_COMMON URI_ID Strings
  * @{
  */
#define URI_ID_0x01_STRING                "http://www.\0"
#define URI_ID_0x02_STRING                "https://www.\0"
#define URI_ID_0x03_STRING                "http://\0"
#define URI_ID_0x04_STRING                "https://\0"
#define URI_ID_0x05_STRING                "tel:\0"
#define URI_ID_0x06_STRING                "mailto:\0"
#define URI_ID_0x07_STRING                "ftp://anonymous:anonymous@\0"
#define URI_ID_0x08_STRING                "ftp://ftp.\0"
#define URI_ID_0x09_STRING                "ftps://\0"
#define URI_ID_0x0A_STRING                "sftp://\0"
#define URI_ID_0x0B_STRING                "smb://\0"
#define URI_ID_0x0C_STRING                "nfs://\0"
#define URI_ID_0x0D_STRING                "ftp://\0"
#define URI_ID_0x0E_STRING                "dav://\0"
#define URI_ID_0x0F_STRING                "news:\0"
#define URI_ID_0x10_STRING                "telnet://\0"
#define URI_ID_0x11_STRING                "imap:\0"
#define URI_ID_0x12_STRING                "rtsp://\0"
#define URI_ID_0x13_STRING                "urn:\0"
#define URI_ID_0x14_STRING                "pop:\0"
#define URI_ID_0x15_STRING                "sip:\0"
#define URI_ID_0x16_STRING                "sips:\0"
#define URI_ID_0x17_STRING                "tftp:\0"
#define URI_ID_0x18_STRING                "btspp://\0"
#define URI_ID_0x19_STRING                "btl2cap://\0"
#define URI_ID_0x1A_STRING                "btgoep://\0"
#define URI_ID_0x1B_STRING                "tcpobex://\0"
#define URI_ID_0x1C_STRING                "irdaobex://\0"
#define URI_ID_0x1D_STRING                "file://\0"
#define URI_ID_0x1E_STRING                "urn:epc:id:\0"
#define URI_ID_0x1F_STRING                "urn:epc:tag\0"
#define URI_ID_0x20_STRING                "urn:epc:pat:\0"
#define URI_ID_0x21_STRING                "urn:epc:raw:\0"
#define URI_ID_0x22_STRING                "urn:epc:\0"
#define URI_ID_0x23_STRING                "urn:nfc:\0"
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif
#endif /* __LIB_NDEF_H */


