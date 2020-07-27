/**
  ******************************************************************************
  * @file    lib_NDEF_Wifi.c
  * @author  Central LAB
  * @version $Revision:$
  * @date    $Date:$
  * @ingroup LibNDEF_Wifi
  * @brief   This file helps to manage a NDEF message representing a Wifi Token.
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
#include "lib_NDEF_Wifi.h"
#define ATTRIBUTE_ID_SSID_LSB                                               0X10
#define ATTRIBUTE_ID_SSID_MSB                                               0X45


#define ATTRIBUTE_ID_NETWORK_LSB                                            0X10
#define ATTRIBUTE_ID_NETWORK_MSB                                            0X27


/** @addtogroup lib_NDEF_Wifi Wifi OOB library (WPS)
  * @ingroup  libNDEF
  *	@brief    This module is used to manage a Wifi Out-Of-Band NDEF message, to start a communication based on Wifi.
  * @details  The Wifi OOB format is described by the Wifi Protected Setup specification.
  *           It consists in a list of data elements formated as type-length-value.
  *           This module allows to build, write & read such data embedded in a NDEF message.
  * @section  Wifi_Library_Usage Wifi NDEF library usage
  * @subsection  Write_Wifi_OOB How to write a Wifi OOB
  *           The Wifi OOB written in a NDEF record by this module has the following structure:
  *           - Version
  *           - Credential
                - Network index
                - SSID
                - Authentication Type
                - Encryption Type
                - Network Key
                - MAC Address
                - Vendor Extension
                  - Network Key Shareable
                - Vendor Extension
                  - Version2
                  
                To write a Wifi OOB, the user must:
                1. Instanciate & initialize a `sWifiTokenInfo` structure, such as:
                
                       sWifiTokenInfo wps_oob = {.NetworkSSID        = "MY_SSID",
                                                 .AuthenticationType = NDEF_WIFI_AUTHENTICATION_NONE,
                                                 .EncryptionType     = NDEF_WIFI_ENCRYPTION_NONE,
                                                 .NetworkKey         = "01234567"};
                  @note
                  1. `AuthenticationType` & `EncryptionType` fields must respectively be values from enums: `Ndef_Wifi_Authentication_t` & `Ndef_Wifi_Encryption_t`.
                  2. If the `Network key` is set to an empty string, the library sets it to `00000000`.\n
                   Even if 0-length Network Key is supposed to be supported, smartphones dont necessarily accept it.

                2. Call the `NDEF_WriteWifiToken` function on a pointer on the previous struct instance:
                
                       NDEF_WriteWifiToken(&wps_oob);
  * @subsection  Read_Wifi_OOB How to read a Wifi OOB
  *          1. Read the 1st record of the NDEF message:
  *
  *                 sRecordInfo_t rRecord;
  *                 NDEF_ReadNDEF(NDEF_Buffer);
  *                 NDEF_IdentifyBuffer(rRecord,NDEF_Buffer);
  *          2. Decode the Wifi OOB:
  *
  *                 sWifiTokenInfo wps;
  *                 NDEF_ReadWifiToken(&record, &wps);
  *             @note
  *               Only the `NetworkSSID` & `NetworkKey` are extracted from the Wifi OOB, other fields of the `sWifiTokenInfo` structure are left un-initialized.
  * @{
  */

/* This buffer contains the data send/received by the TAG */
extern uint8_t NDEF_Buffer [NDEF_MAX_SIZE];


static void NDEF_FillWifiTokenStruct( uint8_t* pPayload, uint32_t PayloadSize, sWifiTokenInfo *pWifiTokenStruct);
static void NDEF_Read_WifiToken ( struct sRecordInfo *pRecordStruct, sWifiTokenInfo *pWifiTokenStruct );

/**
  * @brief  This function fills a WifiToken structure with information from the NDEF message.
	* @param	pPayload Pointer on the payload data of the NDEF message.
	* @param	PayloadSize number of data in the payload.
	* @param	pWifiTokenStruct pointer on the structure to fill.
  * @retval NONE 
  */
static void NDEF_FillWifiTokenStruct( uint8_t* pPayload, uint32_t PayloadSize, sWifiTokenInfo *pWifiTokenStruct)
{
	uint8_t* pLastByteAdd,data1,data2,*temp,*temp_br ;
        uint16_t SSIDLen,NetWorkKeyLen;
        uint8_t *dbg,dbg1;
      
	
	pLastByteAdd = (uint8_t*)(pPayload + PayloadSize);
        pPayload--;     
        
        while(pPayload++ != pLastByteAdd)
        {
          uint8_t attribute = *pPayload;
          temp_br = pPayload;
          switch(attribute)
          {
            
          case ATTRIBUTE_ID_SSID_LSB:
            temp = pPayload;
            dbg = temp;
            dbg1 = *++dbg;
            if(dbg1 == ATTRIBUTE_ID_SSID_MSB )
            {
              data1 = *++dbg;
              data2 = *++dbg;
              SSIDLen = data1;
              SSIDLen = SSIDLen << 8;
              SSIDLen |= data2;
              pPayload += 4;
              memcpy( pWifiTokenStruct->NetworkSSID, pPayload, SSIDLen);
              /* add end of string charactere */
              pWifiTokenStruct->NetworkSSID[SSIDLen] = '\0';	
              pPayload += SSIDLen - 1;
            }
            else if(dbg1 == ATTRIBUTE_ID_NETWORK_MSB )
            {
              data1 = *++dbg;
              data2 = *++dbg;
              NetWorkKeyLen = data1;
              NetWorkKeyLen = NetWorkKeyLen << 8;
              NetWorkKeyLen |= data2;
              pPayload += 4;
              memcpy( pWifiTokenStruct->NetworkKey, pPayload, NetWorkKeyLen);
              /* add end of string charactere */
              pWifiTokenStruct->NetworkKey[NetWorkKeyLen] = '\0';
              pPayload += NetWorkKeyLen -1;
            }
            else
            {
              pPayload = temp_br;
            }
            
          break; 
           
          default :
            ;
          }
        }
        
}

/**
  * @brief  This fonction reads the WifiToken and store data in a structure.
	* @param	pRecordStruct Pointer on the record structure.
	* @param	pWifiTokenStruct Pointer on the structure to fill.
  * @retval NONE 
  */
static void NDEF_Read_WifiToken ( struct sRecordInfo *pRecordStruct, sWifiTokenInfo *pWifiTokenStruct )
{
	uint8_t* pPayload;
	uint32_t PayloadSize;
	
	PayloadSize = pRecordStruct->PayloadLength;
	
	/* Read record header */
	pPayload = (uint8_t*)(pRecordStruct->PayloadBufferAdd);
	
	if( pRecordStruct->NDEF_Type == URI_WIFITOKEN_TYPE)
		NDEF_FillWifiTokenStruct(pPayload , PayloadSize, pWifiTokenStruct);
  
}


/**
  * @brief  This fonction reads a NDEF record and retrieves a WifiToken information if any.
  * @param	pRecordStruct Pointer on the record structure.
	* @param	pWifiTokenStruct Pointer on a `sWifiTokenInfo` structure to fill with the WifiToken information.
  * @retval NDEF_OK WifiToken information from NDEF have been retrieved.
	* @retval NDEF_ERROR Not able to retrieve the WifiToken information.
  */
uint16_t NDEF_ReadWifiToken(struct sRecordInfo *pRecordStruct, sWifiTokenInfo *pWifiTokenStruct)
{
	uint16_t status = NDEF_ERROR;
	  
	if( pRecordStruct->NDEF_Type == URI_WIFITOKEN_TYPE )
	{	
		NDEF_Read_WifiToken(pRecordStruct, pWifiTokenStruct );
		status = NDEF_OK;
	}
	
	return status;
}


/**
  * @brief  This fonction writes a NDEF message built with the WifiToken data given in the structure.
	* @param	pWifiTokenStruct Pointer on the structure containing the WifiToken information.
  * @retval NDEF_OK The NDEF message has been successfully written.
	* @retval NDEF_ERROR Not able to store the NDEF message inside the tag.
  */
uint16_t NDEF_WriteWifiToken ( sWifiTokenInfo *pWifiTokenStruct )
{
	uint16_t status = NDEF_ERROR;
        uint8_t* pPayload,initStage = 0;
        uint16_t DataSize;
        uint32_t PayloadSize,SSIDSize,SSIDKeySize;

  if(pWifiTokenStruct->NetworkKey[0] == '\0')
  {
    /* Empty network key is not supported by Phones */
    strcpy(pWifiTokenStruct->NetworkKey, NDEF_WIFI_DEFAULT_NETWORK_KEY);
  }
  
          uint8_t configToken1[] = {0x10,0x4A, /* Attribute ID : Version*/
                                    0x00,0x01, /* Attribute ID Length*/
                                    0x10,    /* Version 1.0*/
                                    0x10,0x0E,  /* Attribute ID Credential*/  
                                    0x00,0x48, /* Attribute ID Length*/
                                    0x10,0x26, /* Attribute ID : Network Index*/
                                    0x00,0x01, /* Attribute Length*/
                                    0x01,  /* Index */
                                    0x10,0x45, /* Attribute ID :SSID*/
                            
        };
        
      /* Fill SSID length + SSID between configToken1 and configToken3*/ 

          uint8_t configToken3[] = {0x10,0x03, /* Attribute ID :Authentication Type*/
                                    0x00,0x02, /* Attribute Length*/
                                    0x00,0x01,  /* Attribute Type : Open*/
                                    0x10,0x0F,  /* Attribute ID  : Encryption Type*/
                                    0x00,0x02,  /* Attribute Length*/
                                    0x00,0x01, /* Encryption Type : None*/
                                    0x10,0x27};  /* Attribute ID  : Network Key */
        
              
     /*Fill SSID KEY Length and SSID Key between configToken3 and configToken5*/

          uint8_t configToken5[] = {0x10,0x20, /* Attribute ID  : MAC Address */
                                    0x00,0x06, /* Attribute Length*/
                                    0, /*MAC-ADDRESS*/
                                    0, /*MAC-ADDRESS*/
                                    0, /*MAC-ADDRESS*/
                                    0, /*MAC-ADDRESS*/
                                    0, /*MAC-ADDRESS*/
                                    0, /*MAC-ADDRESS*/
                                    0x10,0x49, /* Attribute ID  : Vendor Extension */
                                    0x00,0x06, /* Attribute Length*/
                                    0x00,0x37,0x2A, /* Vendor ID:WFA*/
                                    0x02, /* Subelement ID:Network Key Shareable*/
                                    0x01, /* Subelement Length*/
                                    0x01, /*Network Key Shareable : TRUE*/
                                    0x10,0x49, /* Attribute ID  : Vendor Extension */
                                    0x00,0x06,/* Attribute Length*/
                                    0x00,0x37,0x2A,/* Vendor ID:WFA*/
                                    0x00, /* Subelement ID:Version2*/
                                    0x01, /* Subelement Length:1*/
                                    0x20 /* Version2*/
        };
               

  /* Set size of the tokens */
  const uint32_t CONFIG_TOKEN_1 = sizeof(configToken1);
  const uint32_t CONFIG_TOKEN_3 = sizeof(configToken3);
  const uint32_t CONFIG_TOKEN_5 = sizeof(configToken5);

  /* Update Token3 for Autentication & Encryption Types, their default value is coded in token3 */
  configToken3[CONFIG_TOKEN_3_AUTHENTICATION_TYPE_INDEX] = pWifiTokenStruct->AuthenticationType & 0xFF;
  configToken3[CONFIG_TOKEN_3_ENCRYPTION_TYPE_INDEX] = pWifiTokenStruct->EncryptionType & 0xFF;

	/* fill Wifi record header */
	NDEF_Buffer[FIRST_RECORD_OFFSET] = 0xD2;   /* Record Flag */
	NDEF_Buffer[FIRST_RECORD_OFFSET+1] = WIFITOKEN_TYPE_STRING_LENGTH;
	NDEF_Buffer[FIRST_RECORD_OFFSET+2] = 76; /* needs to be autocalculated - done at the end */
  
	memcpy(&NDEF_Buffer[FIRST_RECORD_OFFSET+3], WIFITOKEN_TYPE_STRING, WIFITOKEN_TYPE_STRING_LENGTH);
	
	pPayload = &NDEF_Buffer[FIRST_RECORD_OFFSET+3+WIFITOKEN_TYPE_STRING_LENGTH];
	PayloadSize = 0;
	
  /* Compute credential length */
        
  uint16_t credential_length =  5 +                                     // Network index
                                4 +                                     // SSID type + length
                                strlen(pWifiTokenStruct->NetworkSSID) + // SSID
                                CONFIG_TOKEN_3 +
                                2 +                                     // Network key length
                                strlen(pWifiTokenStruct->NetworkKey) +  // Network KEY
                                CONFIG_TOKEN_5;
	
  /* update credential length */
  configToken1[CONFIG_TOKEN_1_CREDENTIAL_LENGTH_INDEX + 1] = credential_length & 0xff;
  configToken1[CONFIG_TOKEN_1_CREDENTIAL_LENGTH_INDEX] = credential_length>>8;
  
  
        for(initStage=0;initStage<CONFIG_TOKEN_1;initStage++)
        {
          *pPayload =configToken1[initStage];
          pPayload++;
        }
        
        /*Fill SSID length and SSID value*/
        SSIDSize = strlen(pWifiTokenStruct->NetworkSSID);
        *pPayload = 0x00; pPayload++;
        *pPayload = SSIDSize & 0x000000FF; pPayload++;
        
        strcpy((char*)pPayload,pWifiTokenStruct->NetworkSSID);
        pPayload = pPayload + strlen(pWifiTokenStruct->NetworkSSID);
 
        for(initStage=0;initStage<CONFIG_TOKEN_3;initStage++)
        {
          *pPayload =configToken3[initStage];
          pPayload++;
        }
        
      /* Fill the SSIDKey length and SSIDKey value */
        SSIDKeySize = strlen(pWifiTokenStruct->NetworkKey);
        *pPayload = 0x00; pPayload++;
        *pPayload = SSIDKeySize & 0x000000FF; pPayload++;
        
        memcpy((char*)pPayload,pWifiTokenStruct->NetworkKey,SSIDKeySize);
        pPayload = pPayload + SSIDKeySize;

        for(initStage=0;initStage<CONFIG_TOKEN_5;initStage++)
        {
          *pPayload =configToken5[initStage];
          pPayload++;
        }

	PayloadSize += CONFIG_TOKEN_1 + CONFIG_TOKEN_3 + CONFIG_TOKEN_5 + SSIDSize + SSIDKeySize + 4 ; // +4 is for SSID & Key length fields
		
	NDEF_Buffer[FIRST_RECORD_OFFSET+2] = (PayloadSize & 0x000000FF);
	
	DataSize = PayloadSize + 3 + WIFITOKEN_TYPE_STRING_LENGTH;
	
	/* Write NDEF */
	status = NDEF_WriteNDEF(DataSize , NDEF_Buffer);

  return status;
}


/**
  * @}
  */



/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/


