/**
  ******************************************************************************
  * @file    lib_NDEF_Wifi.h
  * @author  Central LAB
  * @version $Revision:$
  * @date    $Date:$
  * @brief   This file helps to manage the NDEF message representing a Wifi Token.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIB_NDEF_WIFIT_H
#define __LIB_NDEF_WIFIT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF.h"

/** @addtogroup lib_NDEF_Wifi Wifi OOB library (WPS)
  * @ingroup libNDEF
  * @{
  */

	
/* Exported types ------------------------------------------------------------*/
/** @brief This index points to the position of the credential length value in Token1.
  * Its value may require to be updated if the token1 is modified.
  */
#define CONFIG_TOKEN_1_CREDENTIAL_LENGTH_INDEX 7

/** @brief This index points to the position of the Authentication type value in Token3.
  *        Its value may require to be updated if the token3 is modified. 
  */
#define CONFIG_TOKEN_3_AUTHENTICATION_TYPE_INDEX  ((uint32_t)5)
/** @brief This index points to the position of the Encryption type value in Token3.
  *        Its value may require to be updated if the token3 is modified.
  */
#define CONFIG_TOKEN_3_ENCRYPTION_TYPE_INDEX      ((uint32_t)11)

/** @brief MIME type for the Wifi OOB token.
  */
#define WIFITOKEN_TYPE_STRING													"application/vnd.wfa.wsc"	
/** @brief Length of the MIME type for the Wifi OOB token.
  */
#define WIFITOKEN_TYPE_STRING_LENGTH	 								((uint32_t)23)

/**
  * @brief Network key to be used when the Authentication is set to None.
  *  Althought WPS defines a 0 length network key in such case, 
  *  with tested phones a 8 digit value is required.
  */
#define NDEF_WIFI_DEFAULT_NETWORK_KEY	 								      "00000000"

typedef enum {
  NDEF_WIFI_ENCRYPTION_NONE = 0x0000, /**< WPS No Encryption (set to 0 for Android native support / should be 1) */
  NDEF_WIFI_ENCRYPTION_WEP  = 0x0002, /**< WPS Encryption based on WEP  */
  NDEF_WIFI_ENCRYPTION_TKIP = 0x0003, /**< WPS Encryption based on TKIP  */
  NDEF_WIFI_ENCRYPTION_AES  = 0x0004 /**< WPS Encryption based on AES  */
} Ndef_Wifi_Encryption_t;

typedef enum {
  NDEF_WIFI_AUTHENTICATION_NONE     = 0x0000, /**< WPS No Authentication (set to 0 for Android native support / should be 1)  */
  NDEF_WIFI_AUTHENTICATION_WPAPSK   = 0x0002, /**< WPS Authentication based on WPAPSK  */
  NDEF_WIFI_AUTHENTICATION_SHARED   = 0x0003, /**< WPS Authentication based on ??  */
  NDEF_WIFI_AUTHENTICATION_WPA      = 0x0004, /**< WPS Authentication based on WPA  */
  NDEF_WIFI_AUTHENTICATION_WPA2     = 0x0005, /**< WPS Authentication based on WPA2  */
  NDEF_WIFI_AUTHENTICATION_WPA2PSK  = 0x0006 /**< WPS Authentication based on WPA2PSK  */
} Ndef_Wifi_Authentication_t;

/**
  * @brief  WifiToken structure, to store Network SSID, Authentication Type, Encryption Type and Network Key.
  */	 	 
typedef struct 
{
	char NetworkSSID[32];                           /**< Store the Network SSID. */
	Ndef_Wifi_Authentication_t AuthenticationType;  /**< Store the Authentication type to be used. */
	Ndef_Wifi_Encryption_t EncryptionType;          /**< Store the Encryption to be used. */
  char NetworkKey[32];                            /**< Store the Network Key. */
}sWifiTokenInfo;

	 	 
uint16_t NDEF_ReadWifiToken(struct sRecordInfo *pRecordStruct, sWifiTokenInfo *pWifiTokenStruct);
uint16_t NDEF_WriteWifiToken( sWifiTokenInfo *pWifiTokenStruct );

/**
  * @}
  */


#ifdef __cplusplus
}
#endif
	 
#endif /* __LIB_NDEF_WIFIT_H */

