/**
  ******************************************************************************
  * @file    lib_NDEF_Bluetooth.c
  * @author  MMY Application Team
  * @version $Revision: 2702 $
  * @date    $Date: 2016-07-13 18:45:05 +0200 (Wed, 13 Jul 2016) $
  * @ingroup LibNDEF_Bluetooth
  * @brief   This file help to manage a NDEF file representing a Bluetooth pairing info.
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
#include "lib_NDEF_Bluetooth.h"



/** @addtogroup lib_NDEF_Bluetooth Bluetooth OOB library
  * @ingroup libNDEF
  * @{
  * @brief    This module is used to manage a Bluetooth Out-Of-Band NDEF message, to start a communication based on Bluetooth.
  * @details  The Bluetooth OOB format is described by the Bluetooth v4.0 core specification.
  *           It consists in a list of Extended Inquiry Responses formated as length-type-value.
  *           This module allows to build, write & read such data embedded in a NDEF message.
  * @section      Bluetooth_Library_Usage Bluetooth NDEF Library usage
  * @subsection   Bluetooth_Write_BrEdr How to write a Bluetooth Br/Edr OOB
  *               1. Instanciate & initialize a `Ndef_Bluetooth_OOB_t` structure, specifying:
  *                 - the `NDEF_BLUETOOTH_BREDR` type.
  *                 - the mandatory Device Address field.
  *                 - any other optional EIRs.
  *
  *                       Ndef_Bluetooth_OOB_t w_bredr_oob = {  .Type = NDEF_BLUETOOTH_BREDR,
  *                                                             .DeviceAddress = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06},
  *                                                             .OptionalMask = NDEF_BLUETOOTH_OPTION(BLUETOOTH_EIR_COMPLETE_LOCAL_NAME) |
  *                                                                             NDEF_BLUETOOTH_OPTION(BLUETOOTH_EIR_DEVICE_CLASS) |
  *                                                                             NDEF_BLUETOOTH_OPTION(BLUETOOTH_EIR_SERVICE_CLASS_UUID_COMPLETE_16),
  *                                                             .LocalName = "MyDevice",
  *                                                             .nbUUID16 = 2,
  *                                                             .ClassUUID16 = {0x111E,0x110B},
  *                                                             .DeviceClass = {0x04,0x04,0x20}
  *                                                           };
  *               2. Clear the NDEF message and call the `NDEF_AppendBluetoothOOB` function to write the OOB:
  * 
  *                      NDEF_ClearNDEF();
  *                      NDEF_AppendBluetoothOOB ( &w_bredr_oob, NULL );
  *                 @note Second parameter of `NDEF_AppendBluetoothOOB` can be used to specify an ID for the OOB record (useful for the NDEF Handover message, where specifying an ID is mandatory)
  * @subsection   Bluetooth_Write_Ble How to write a Bluetooth LE OOB
  *               1. Instanciate & initialize a `Ndef_Bluetooth_OOB_t` structure, specifying:
  *                 - the `NDEF_BLUETOOTH_BLE` type.
  *                 - the mandatory Device Address & LE Role fields.
  *                 - any other optional EIRs.
  *
  *                        Ndef_Bluetooth_OOB_t w_ble_oob = { .Type = NDEF_BLUETOOTH_BLE,
  *                                                           .DeviceAddress = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06},
  *                                                           .DeviceAddressType = NDEF_BLE_PUBLIC_ADDRESS_TYPE,
  *                                                           .Role = NDEF_BLE_ROLE_PERIPH_ONLY,
  *                                                           .OptionalMask = NDEF_BLUETOOTH_OPTION(BLUETOOTH_EIR_COMPLETE_LOCAL_NAME),
  *                                                           .LocalName = "MyDeviceName",                                
  *                                                         };
  *
  *               2. Clear the NDEF message and call the `NDEF_AppendBluetoothOOB` function to write the OOB:
  * 
  *                      NDEF_ClearNDEF();
  *                      NDEF_AppendBluetoothOOB ( &w_ble_oob, NULL );
  *                 @note Second parameter of `NDEF_AppendBluetoothOOB` can be used to specify an ID for the OOB record (useful for the NDEF Handover message, where specifying an ID is mandatory)
  *
  * @subsection   Bluetooth_Read How to read a Bluetooth OOB
  *               1. Read the 1st record of the NDEF message:
  *
  *                      sRecordInfo_t record;
  *                      NDEF_ReadNDEF(NDEF_Buffer);
  *                      NDEF_IdentifyBuffer(record,NDEF_Buffer);
  *               2. Decode the Bluetooth OOB:
  *
  *                       Ndef_Bluetooth_OOB_t bluetooth_oob;
  *                       NDEF_ReadBluetoothOOB(&record,&bluetooth_oob);
  *               3. Use the data from the `Ndef_Bluetooth_OOB_t` structure to start a Bluetooth connexion.
  *                       
  *                                              
  *
  */


/**
  * @brief  This function copies an array, changing its endianness, usefull to convert data to BLE endianess.
  * @param  dst Pointer on 1st element of the destination array.
  * @param  src pointer on 1st element of the source array .
  * @param  length Number of element to copy.
  * @return Pointer to the destination array.
  */
uint8_t* NDEF_BluetoothCopy(uint8_t* dst, uint8_t* src, uint32_t length)
{
  uint32_t index;
  for(index = 0 ; index < length; index++)
  {
    dst[index] = src[length - index - 1];
  }  
  return dst;
}

/**
  * @brief  This function reads a NDEF record and retrieves Bluetooth (BR/EDR or BLE) OOB information if present.
  * @param  pRecord       Pointer on the record structure.
  * @param  pBluetooth    Pointer on the structure to fill .
  * @retval NDEF_OK       OOB information has been retrieved from the NDEF record.
  * @retval NDEF_ERROR    OOB information cannot be retrieved.
  */
uint16_t NDEF_ReadBluetoothOOB( sRecordInfo_t *pRecord, Ndef_Bluetooth_OOB_t *pBluetooth )
{
  uint8_t* pData = pRecord->PayloadBufferAdd;
  uint8_t* OOBEnd = pRecord->PayloadBufferAdd + pRecord->PayloadLength;

  pBluetooth->OptionalMask = 0;
  pBluetooth->nbServiceData = 0;
  pBluetooth->Role = NDEF_BLE_ROLE_UNDEF;
  pBluetooth->DeviceAddressType = NDEF_BLE_UNDEF_ADDRESS_TYPE;
  pBluetooth->nbUUID16 = 0;
  pBluetooth->nbUUID32 = 0;
  pBluetooth->nbUUID128 = 0;
  pBluetooth->nbServiceSolicitation16 = 0;
  pBluetooth->nbServiceSolicitation128 = 0;
  
  if((pRecord->TypeLength == strlen(NDEF_BLUETOOTH_BREDR_MIME_TYPE)) &&
    !memcmp(pRecord->Type,NDEF_BLUETOOTH_BREDR_MIME_TYPE,strlen(NDEF_BLUETOOTH_BREDR_MIME_TYPE)))
  {
    pBluetooth->Type = NDEF_BLUETOOTH_BREDR;
    /* Retrieve mandatory OOB data:                   */
    /* 2 bytes for length and 6 bytes for device addr */
    NDEF_BluetoothCopy((uint8_t*)pBluetooth->DeviceAddress,&pData[2],sizeof(pBluetooth->DeviceAddress));
    pData += 8;
  } else if ((pRecord->TypeLength == strlen(NDEF_BLUETOOTH_BLE_MIME_TYPE)) &&
    !memcmp(pRecord->Type,NDEF_BLUETOOTH_BLE_MIME_TYPE,strlen(NDEF_BLUETOOTH_BLE_MIME_TYPE)))
  {
    pBluetooth->Type = NDEF_BLUETOOTH_BLE;
    /* for BLE, mandatory fields are in EIR */
  } else {
    /* This is an unknown MIME type */
    return NDEF_ERROR;
  }

  /* EIR format: 1 byte for length, 1 byte for type, n bytes for data */  
  while (pData < OOBEnd)
  {
    NDEF_EIR_t* rEIR = (NDEF_EIR_t*)pData;
    /* +1 for EIR length byte */
    pData += rEIR->length + 1;

    /* keep track of all EIR found */
    if(rEIR->type < 0x20)
      NDEF_BLUETOOTH_SET_OPTIONAL_MASK(pBluetooth,rEIR->type);

    switch (rEIR->type)
    {
      case BLUETOOTH_EIR_FLAGS:
        pBluetooth->Flags = *rEIR->value;
      break;
      
      case BLUETOOTH_EIR_SERVICE_CLASS_UUID_PARTIAL_16:
      case BLUETOOTH_EIR_SERVICE_CLASS_UUID_COMPLETE_16:
        pBluetooth->nbUUID16 = (rEIR->length - 1) / 2 ;
        NDEF_BluetoothCopy((uint8_t*)pBluetooth->ClassUUID16,rEIR->value,rEIR->length-1); 
      break;
      
      case BLUETOOTH_EIR_SERVICE_CLASS_UUID_PARTIAL_32:
      case BLUETOOTH_EIR_SERVICE_CLASS_UUID_COMPLETE_32:
        pBluetooth->nbUUID32 = (rEIR->length - 1) / 4 ;
        NDEF_BluetoothCopy((uint8_t*)pBluetooth->ClassUUID32,rEIR->value,rEIR->length-1);
      break;
      
      case BLUETOOTH_EIR_SERVICE_CLASS_UUID_PARTIAL_128:
      case BLUETOOTH_EIR_SERVICE_CLASS_UUID_COMPLETE_128:
        pBluetooth->nbUUID128 = (rEIR->length - 1) / 16 ;
        NDEF_BluetoothCopy((uint8_t*)pBluetooth->ClassUUID128,rEIR->value,rEIR->length-1);
      break;
      
      case BLUETOOTH_EIR_SHORT_LOCAL_NAME:
      case BLUETOOTH_EIR_COMPLETE_LOCAL_NAME:
        /* No worry about name length as max EIR length is 0xff using 1 byte for the type metadata  */
        memcpy(pBluetooth->LocalName,rEIR->value,rEIR->length-1);
        pBluetooth->LocalName[rEIR->length-1] = '\0';
      break;
      
      case BLUETOOTH_EIR_TX_POWER_LEVEL:
        pBluetooth->TxPowerLevel = *rEIR->value;
      break;
            
      case BLUETOOTH_EIR_DEVICE_CLASS:
        NDEF_BluetoothCopy((uint8_t*)pBluetooth->DeviceClass,rEIR->value,sizeof(pBluetooth->DeviceClass));
      break;
      
      case BLUETOOTH_EIR_SIMPLE_PAIRING_HASH:
        NDEF_BluetoothCopy((uint8_t*)pBluetooth->SimplePairingHash,rEIR->value,sizeof(pBluetooth->SimplePairingHash));
     break;
      
      case BLUETOOTH_EIR_SIMPLE_PAIRING_RANDOMIZER:
        NDEF_BluetoothCopy((uint8_t*)pBluetooth->SimplePairingRandomizer,rEIR->value,sizeof(pBluetooth->SimplePairingRandomizer));
      break;

      case BLUETOOTH_EIR_SECURITY_MANAGER_TK_VALUE:
        NDEF_BluetoothCopy((uint8_t*)pBluetooth->SecureManagerTK,rEIR->value,sizeof(pBluetooth->SecureManagerTK));       
      break;
      
     case BLUETOOTH_EIR_SECURITY_MANAGER_FLAGS:
        pBluetooth->SMFlags = *rEIR->value;
      break;
        
     case BLUETOOTH_EIR_SLAVE_CONNECTION_INTERVAL_RANGE:
        NDEF_BluetoothCopy((uint8_t*)pBluetooth->SlaveConnIntervalRange,rEIR->value,sizeof(pBluetooth->SlaveConnIntervalRange));
      break;
                
      case BLUETOOTH_EIR_SERVICE_SOLICITATION_16:
        pBluetooth->nbServiceSolicitation16 = (rEIR->length - 1) / 2 ;
        NDEF_BluetoothCopy((uint8_t*)pBluetooth->ServiceSolicitation16,rEIR->value,rEIR->length-1);
      break;
                
      case BLUETOOTH_EIR_SERVICE_SOLICITATION_128:
        pBluetooth->nbServiceSolicitation128 = (rEIR->length - 1) / 16 ;
        NDEF_BluetoothCopy((uint8_t*)pBluetooth->ServiceSolicitation128,rEIR->value,rEIR->length-1);
      break;
      
      case BLUETOOTH_EIR_SERVICE_DATA:
        /* a specific function should be used for this EIR */
        pBluetooth->nbServiceData++;
      break;
                
      case BLUETOOTH_EIR_MANUFACTURER_DATA:
        /* a specific function should be used for this EIR */
        pBluetooth->nbManufacturerData++;
      break;
                
     case BLUETOOTH_EIR_APPEARANCE:
        pBluetooth->Appearance = *(uint8_t *)rEIR->value;
      break;
      
      case BLUETOOTH_EIR_BLE_DEVICE_ADDRESS:
        NDEF_BluetoothCopy((uint8_t*)pBluetooth->DeviceAddress,rEIR->value,sizeof(pBluetooth->DeviceAddress));
        pBluetooth->DeviceAddressType = (Ndef_BLE_Address_Type_t)rEIR->value[sizeof(pBluetooth->DeviceAddress)];
      break;
      
      case BLUETOOTH_EIR_BLE_ROLE:
        pBluetooth->Role = (Ndef_BLE_Role_t) *rEIR->value;
      break;
      
      default:
        pBluetooth->nbUnknown++;
      break;
    } /* switch rEIR->type */
  } /* while (pData < OOBEnd) */
    /* Check that BLE mandatory fields are there */
  if((pBluetooth->Type == NDEF_BLUETOOTH_BLE) &&
    (!NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_BLE_DEVICE_ADDRESS) ||
      !NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_BLE_ROLE)))
      return NDEF_ERROR;
    
  return NDEF_OK;
}

/**
  * @brief  This function appends a Bluetooth OOB record to the NDEF message, using the OOB data given in the input structure.
  * @param  pBluetooth  Pointer on a `Ndef_Bluetooth_OOB_t` structure containing the OOB information.
  * @param  RecordID    ID to be used for this record (required for Handover case, can be set to NULL in other cases).
  * @retval NDEF_OK                     The Bluetooth OOB record has been appended.
  * @retval NDEF_ERROR_MEMORY_INTERNAL  The Bluetooth OOB record cannot be appended due to memory size limitation.
  * @retval NDEF_ERROR                  The Bluetooth OOB record cannot be appended.
  */
uint16_t NDEF_AppendBluetoothOOB( Ndef_Bluetooth_OOB_t *pBluetooth, char* RecordID )
{
  sRecordInfo_t Record;
  uint16_t status;
  
  Record.RecordFlags = TNF_MediaType;
  Record.RecordFlags |= (RecordID != NULL) ? IL_Mask : 0;
  Record.IDLength = strlen(RecordID);
  memcpy(Record.ID,RecordID,Record.IDLength);
  
  if(pBluetooth->Type == NDEF_BLUETOOTH_BREDR)
  {
    Record.TypeLength = strlen(NDEF_BLUETOOTH_BREDR_MIME_TYPE);
    memcpy(Record.Type, NDEF_BLUETOOTH_BREDR_MIME_TYPE, Record.TypeLength); 
  }
  else if (pBluetooth->Type == NDEF_BLUETOOTH_BLE)
  {
    Record.TypeLength = strlen(NDEF_BLUETOOTH_BLE_MIME_TYPE);
    memcpy(Record.Type, NDEF_BLUETOOTH_BLE_MIME_TYPE, Record.TypeLength); 
  }
  else
  {
    return NDEF_ERROR;
  }
  
  /* Generate OOB payload */
  Record.PayloadLength = NDEF_GetBluetoothOOBLength(pBluetooth);
  Record.PayloadBufferAdd = NDEF_Record_Buffer;
  if(Record.PayloadLength > NDEF_RECORD_MAX_SIZE)
    return NDEF_ERROR_MEMORY_INTERNAL;

  /* pData: pointer to ease increment of record buffer address (byte granularity) */
  uint8_t* pData = Record.PayloadBufferAdd;  
  
  /* for BR-EDR Device address & length are managed outside EIR */
  if(pBluetooth->Type == NDEF_BLUETOOTH_BREDR)
  {
    *pData = Record.PayloadLength;
    pData += 2;
    NDEF_BluetoothCopy(pData,(uint8_t*)pBluetooth->DeviceAddress,sizeof(pBluetooth->DeviceAddress));
    pData+=sizeof(pBluetooth->DeviceAddress);
  }
  
  /* wEIR: pointer to ease write to the buffer.
   * length always set with an additional +1 corresponding to the EIR type byte.
   * pData increment is always done with an additional +1 corresponding to the EIR length byte.
   */
  NDEF_EIR_t* wEIR;

  
  if(pBluetooth->Type == NDEF_BLUETOOTH_BLE)
  {
    /* following EIR are mandatory for BLE */
    wEIR = (NDEF_EIR_t*)pData;
    wEIR->length = sizeof(pBluetooth->DeviceAddress) + sizeof(pBluetooth->DeviceAddressType) + 1;
      wEIR->type = BLUETOOTH_EIR_BLE_DEVICE_ADDRESS;
    NDEF_BluetoothCopy(wEIR->value, (uint8_t*)pBluetooth->DeviceAddress,sizeof(pBluetooth->DeviceAddress));
    wEIR->value[sizeof(pBluetooth->DeviceAddress)] = pBluetooth->DeviceAddressType;
    pData += wEIR->length + 1;
    
    wEIR = (NDEF_EIR_t*)pData;
    wEIR->length = sizeof(pBluetooth->Role) + 1;
      wEIR->type = BLUETOOTH_EIR_BLE_ROLE;
    wEIR->value[0] = pBluetooth->Role;
    pData += wEIR->length + 1;
    
  }

  /* Rely on the optional mask to know if a EIR is required or not */
  if(NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_FLAGS))
  {
    wEIR = (NDEF_EIR_t*)pData;
    wEIR->length = sizeof(pBluetooth->Flags) + 1;
    wEIR->type = BLUETOOTH_EIR_FLAGS;
    wEIR->value[0] = pBluetooth->Flags;
    pData += wEIR->length + 1;
  }    
  
  if(pBluetooth->nbUUID16 > 0)
  {
    wEIR = (NDEF_EIR_t*)pData;
    wEIR->length = 2 * pBluetooth->nbUUID16 + 1;
    if(NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SERVICE_CLASS_UUID_COMPLETE_16))
      wEIR->type = BLUETOOTH_EIR_SERVICE_CLASS_UUID_COMPLETE_16;
    else
      wEIR->type = BLUETOOTH_EIR_SERVICE_CLASS_UUID_PARTIAL_16;
    NDEF_BluetoothCopy(wEIR->value, (uint8_t*) pBluetooth->ClassUUID16, 2 * pBluetooth->nbUUID16);
    pData += wEIR->length + 1;
    
  }

  if(pBluetooth->nbUUID32 > 0)
  {
    wEIR = (NDEF_EIR_t*)pData;
    wEIR->length = 4 * pBluetooth->nbUUID32 + 1;
    if(NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SERVICE_CLASS_UUID_COMPLETE_32))
      wEIR->type = BLUETOOTH_EIR_SERVICE_CLASS_UUID_COMPLETE_32;
    else
      wEIR->type = BLUETOOTH_EIR_SERVICE_CLASS_UUID_PARTIAL_32;
    NDEF_BluetoothCopy(wEIR->value, (uint8_t*)pBluetooth->ClassUUID32, 4 * pBluetooth->nbUUID32);
    pData += wEIR->length + 1;
    
  }

  if(pBluetooth->nbUUID128 > 0)
  {
    wEIR = (NDEF_EIR_t*)pData;
    wEIR->length = 16 * pBluetooth->nbUUID128 + 1;
    if(NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SERVICE_CLASS_UUID_COMPLETE_128))
      wEIR->type = BLUETOOTH_EIR_SERVICE_CLASS_UUID_COMPLETE_128;
    else
      wEIR->type = BLUETOOTH_EIR_SERVICE_CLASS_UUID_PARTIAL_128;
    NDEF_BluetoothCopy(wEIR->value, (uint8_t*) pBluetooth->ClassUUID128, 16 * pBluetooth->nbUUID128);
    pData += wEIR->length + 1;
    
  }

  if(NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SHORT_LOCAL_NAME) ||
    NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_COMPLETE_LOCAL_NAME))
  {
    wEIR = (NDEF_EIR_t*)pData;
    wEIR->length = strlen(pBluetooth->LocalName) + 1;
    if(NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SHORT_LOCAL_NAME))
      wEIR->type = BLUETOOTH_EIR_SHORT_LOCAL_NAME;
    else
      wEIR->type = BLUETOOTH_EIR_COMPLETE_LOCAL_NAME;
    memcpy(wEIR->value, pBluetooth->LocalName,strlen(pBluetooth->LocalName));
    pData += wEIR->length + 1;
    
  }

  if(NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_TX_POWER_LEVEL))
  {
    wEIR = (NDEF_EIR_t*)pData;
    wEIR->length = sizeof(pBluetooth->TxPowerLevel) + 1;
      wEIR->type = BLUETOOTH_EIR_TX_POWER_LEVEL;
    wEIR->value[0] = pBluetooth->TxPowerLevel;
    pData += wEIR->length + 1;
  }

  if(NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_DEVICE_CLASS))
  {
    wEIR = (NDEF_EIR_t*)pData;
    wEIR->length = sizeof(pBluetooth->DeviceClass) + 1;
      wEIR->type = BLUETOOTH_EIR_DEVICE_CLASS;
    NDEF_BluetoothCopy(wEIR->value, pBluetooth->DeviceClass,sizeof(pBluetooth->DeviceClass));
    pData += wEIR->length + 1;
    
  }

  if(NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SIMPLE_PAIRING_HASH))
  {
    wEIR = (NDEF_EIR_t*)pData;
    wEIR->length = sizeof(pBluetooth->SimplePairingHash) + 1;
      wEIR->type = BLUETOOTH_EIR_SIMPLE_PAIRING_HASH;
    NDEF_BluetoothCopy(wEIR->value, pBluetooth->SimplePairingHash,sizeof(pBluetooth->SimplePairingHash));
    pData += wEIR->length + 1;
    
  }
  
  if(NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SIMPLE_PAIRING_RANDOMIZER))
  {
    wEIR = (NDEF_EIR_t*)pData;
    wEIR->length = sizeof(pBluetooth->SimplePairingRandomizer) + 1;
      wEIR->type = BLUETOOTH_EIR_SIMPLE_PAIRING_RANDOMIZER;
    NDEF_BluetoothCopy(wEIR->value, pBluetooth->SimplePairingRandomizer,sizeof(pBluetooth->SimplePairingRandomizer));
    pData += wEIR->length + 1;
    
  }
  
  if(NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SECURITY_MANAGER_TK_VALUE))
  {
    wEIR = (NDEF_EIR_t*)pData;
    wEIR->length = sizeof(pBluetooth->SecureManagerTK) + 1;
      wEIR->type = BLUETOOTH_EIR_SECURITY_MANAGER_TK_VALUE;
    NDEF_BluetoothCopy(wEIR->value, pBluetooth->SecureManagerTK,sizeof(pBluetooth->SecureManagerTK));
    pData += wEIR->length + 1;
    
  }
  
  if(NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SECURITY_MANAGER_FLAGS))
  {
    wEIR = (NDEF_EIR_t*)pData;
    wEIR->length = sizeof(pBluetooth->SMFlags) + 1;
      wEIR->type = BLUETOOTH_EIR_SECURITY_MANAGER_FLAGS;
    wEIR->value[0] = pBluetooth->SMFlags;
    pData += wEIR->length + 1;
    
  }
  
  if(NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SLAVE_CONNECTION_INTERVAL_RANGE))
  {
    wEIR = (NDEF_EIR_t*)pData;
    wEIR->length = sizeof(pBluetooth->SlaveConnIntervalRange) + 1;
      wEIR->type = BLUETOOTH_EIR_SLAVE_CONNECTION_INTERVAL_RANGE;
    NDEF_BluetoothCopy(wEIR->value, (uint8_t*)pBluetooth->SlaveConnIntervalRange,sizeof(pBluetooth->SlaveConnIntervalRange));
    pData += wEIR->length + 1;
    
  }
  
  if(pBluetooth->nbServiceSolicitation16 > 0)
  {
    wEIR = (NDEF_EIR_t*)pData;
    wEIR->length = 2 * pBluetooth->nbServiceSolicitation16 + 1;
      wEIR->type = BLUETOOTH_EIR_SERVICE_SOLICITATION_16;
    NDEF_BluetoothCopy(wEIR->value, (uint8_t*)pBluetooth->ServiceSolicitation16, 16 * pBluetooth->nbServiceSolicitation16);
    pData += wEIR->length + 1;
    
  }
 
  if(pBluetooth->nbServiceSolicitation128 > 0)
  {
    wEIR = (NDEF_EIR_t*)pData;
    wEIR->length = 16 * pBluetooth->nbServiceSolicitation128 + 1;
      wEIR->type = BLUETOOTH_EIR_SERVICE_SOLICITATION_128;
    NDEF_BluetoothCopy(wEIR->value, (uint8_t*) pBluetooth->ServiceSolicitation128, 16 * pBluetooth->nbServiceSolicitation128);
    pData += wEIR->length + 1;
    
  }
 
  if(NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_APPEARANCE))
  {
    wEIR = (NDEF_EIR_t*)pData;
    wEIR->length = sizeof(pBluetooth->Appearance) + 1;
      wEIR->type = BLUETOOTH_EIR_APPEARANCE;
    wEIR->value[0] = pBluetooth->Appearance;
    pData += wEIR->length + 1;
    
  }


  status = NDEF_AppendRecord(&Record);
  if(status != NDEF_OK) return status;
  
  return NDEF_OK;
}

/**
  * @brief  This function computeS the payload size for the OOB, using the data given in the input `Ndef_Bluetooth_OOB_t` structure.
  * @param  pBluetooth Pointer on a `Ndef_Bluetooth_OOB_t` structure containing the OOB information.
  * @return Computed length in bytes.
  */
uint32_t NDEF_GetBluetoothOOBLength( Ndef_Bluetooth_OOB_t *pBluetooth )
{
  uint32_t length = (pBluetooth->Type == NDEF_BLUETOOTH_BREDR) ? sizeof(pBluetooth->DeviceAddress) + 2 : 0; // +2 is for BR/EDR mandatory length
  length += NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_FLAGS)? sizeof(pBluetooth->Flags) + 2    : 0 ;
  length += pBluetooth->nbUUID16 ? pBluetooth->nbUUID16 * 2 + 2 : 0;
  length += pBluetooth->nbUUID32 ? pBluetooth->nbUUID32 * 4 + 2 : 0;
  length += pBluetooth->nbUUID128 ? pBluetooth->nbUUID128 * 16 + 2 : 0;
  length += NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SHORT_LOCAL_NAME)? strlen(pBluetooth->LocalName) + 2 : 0 ;
  length += NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_COMPLETE_LOCAL_NAME)? strlen(pBluetooth->LocalName) + 2 : 0 ;
  length += NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_TX_POWER_LEVEL)? sizeof(pBluetooth->TxPowerLevel + 2) : 0 ;   
  length += NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_DEVICE_CLASS)? sizeof(pBluetooth->DeviceClass) + 2 : 0 ;   
  length += NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SIMPLE_PAIRING_HASH)? sizeof(pBluetooth->SimplePairingHash) + 2 : 0 ;   
  length += NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SIMPLE_PAIRING_RANDOMIZER)? sizeof(pBluetooth->SimplePairingRandomizer) + 2 : 0 ;  
  length += NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SECURITY_MANAGER_TK_VALUE)? sizeof(pBluetooth->SecureManagerTK) + 2 : 0 ;
  length += NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SECURITY_MANAGER_FLAGS)? sizeof(pBluetooth->SMFlags) + 2 : 0 ;
  length += NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SLAVE_CONNECTION_INTERVAL_RANGE)? sizeof(pBluetooth->SlaveConnIntervalRange) + 2 : 0 ;   
  length += NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SERVICE_SOLICITATION_16)? pBluetooth->nbServiceSolicitation16 * 2 + 2 : 0 ;
  length += NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_SERVICE_SOLICITATION_128)? pBluetooth->nbServiceSolicitation128 * 16 + 2 : 0 ;   
  length += NDEF_BLUETOOTH_GET_OPTIONAL_MASK(pBluetooth,BLUETOOTH_EIR_APPEARANCE)? sizeof(pBluetooth->Appearance) + 2 : 0 ;
  length += (pBluetooth->Type == NDEF_BLUETOOTH_BLE)? sizeof(pBluetooth->DeviceAddress) + sizeof(pBluetooth->DeviceAddressType) + 2 : 0 ;   
  length += (pBluetooth->Type == NDEF_BLUETOOTH_BLE)? sizeof(pBluetooth->Role) + 2 : 0; 

  return length;
}
/**
  * @}
  */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
