/**
  ******************************************************************************
  * @file    lib_NDEF_Bluetooth.h
  * @author  MMY Application Team
  * @version $Revision:$
  * @date    $Date:$
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIB_NDEF_BLUETOOTH_H
#define __LIB_NDEF_BLUETOOTH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF.h"

/** @addtogroup lib_NDEF_Bluetooth
  * @{
  */
   
/** @brief MIME type for the Bluetooth BR/EDR Out-Of-Band data. */
#define NDEF_BLUETOOTH_BREDR_MIME_TYPE "application/vnd.bluetooth.ep.oob"
/** @brief MIME type for the Bluetooth Low Energy Out-Of-Band data. */
#define NDEF_BLUETOOTH_BLE_MIME_TYPE "application/vnd.bluetooth.le.oob"

   
/** @brief Enumerates the Extended Inquiry Responses, as defined in the Bluetooth v4.0 core specification. */
typedef enum {
  BLUETOOTH_EIR_FLAGS                           = 0x01, /**<  Bluetooth flags:\n
                                                              b0: LE limited Discoverable Mode,\n
                                                              b1: LE general Discoverable Mode,\n
                                                              b2: BR/EDR not supported,\n
                                                              b3: Simultaneous LE & BR/EDR Conroller,\n
                                                              b4: Simultaneous LE & BR/EDR Host
                                                          */
  BLUETOOTH_EIR_SERVICE_CLASS_UUID_PARTIAL_16   = 0x02, /**< Bluetooth service UUID on 16-bits (partial list) */
  BLUETOOTH_EIR_SERVICE_CLASS_UUID_COMPLETE_16  = 0x03, /**< Bluetooth service UUID on 16-bits (complete list) */
  BLUETOOTH_EIR_SERVICE_CLASS_UUID_PARTIAL_32   = 0x04, /**< Bluetooth service UUID on 32-bits (partial list) */
  BLUETOOTH_EIR_SERVICE_CLASS_UUID_COMPLETE_32  = 0x05, /**< Bluetooth service UUID on 32-bits (complete list) */
  BLUETOOTH_EIR_SERVICE_CLASS_UUID_PARTIAL_128  = 0x06, /**< Bluetooth service UUID on 128-bits (partial list) */
  BLUETOOTH_EIR_SERVICE_CLASS_UUID_COMPLETE_128 = 0x07, /**< Bluetooth service UUID on 128-bits (complete list) */
  BLUETOOTH_EIR_SHORT_LOCAL_NAME                = 0x08, /**< Shortened local name */
  BLUETOOTH_EIR_COMPLETE_LOCAL_NAME             = 0x09, /**< Complete local name */
  BLUETOOTH_EIR_TX_POWER_LEVEL                  = 0x0A, /**< TX Power Level (1 byte): 0xXX:-127 to +127dBm */
  BLUETOOTH_EIR_DEVICE_CLASS                    = 0x0D, /**< Class of device, Format defined in Assigned Numbers */
  BLUETOOTH_EIR_SIMPLE_PAIRING_HASH             = 0x0E, /**< Simple Pairing Hash C (16 octets), Format defined in [Vol. 2], Part H Section 7.2.2*/
  BLUETOOTH_EIR_SIMPLE_PAIRING_RANDOMIZER       = 0x0F, /**< Simple Pairing Randomizer R (16 octets), Format defined in[Vol. 2], Part H Section 7.2.2 */
  BLUETOOTH_EIR_SECURITY_MANAGER_TK_VALUE       = 0x10, /**< TK Value: Value as used in pairing over LE Physical channel. Format defined in [Vol. 3], Part H Section 2.3*/
  BLUETOOTH_EIR_SECURITY_MANAGER_FLAGS          = 0x11, /**< Flags (1 octet):\n
                                                             b0: OOB Flags Field (0 = OOB data not present, 1 = OOB data present),\n
                                                             b1: LE supported (Host) (i.e. bit 65 of LMP Extended Feature bits Page 1),\n
                                                             b2: Simultaneous LE and BR/EDR to Same Device Capable (Host) (i.e. bit 66 of LMP Extended Feature bits Page 1)\n
                                                             b3: Address type (0 = Public Address, 1 = Random Address)
                                                           */
  BLUETOOTH_EIR_SLAVE_CONNECTION_INTERVAL_RANGE = 0x12, /**< Slave Connection Interval Range: The first 2 octets defines the minimum value for the connection interval, The second 2 octets defines the maximum value for the connection interval */
  BLUETOOTH_EIR_SERVICE_SOLICITATION_16         = 0x14, /**< Service UUIDs: List of 16 bit Service UUIDs*/
  BLUETOOTH_EIR_SERVICE_SOLICITATION_128        = 0x15, /**< Service UUIDs: List of 128 bit Service UUID*/
  BLUETOOTH_EIR_SERVICE_DATA                    = 0x16, /**< Service Data (2 or more octets): The first 2 octets contain the 16 bit Service UUID followed by additional service data */
  BLUETOOTH_EIR_APPEARANCE                      = 0x19, /**< UUID for `Appearance`: The Appearance characteristic value shall be the enumerated value as defined by Bluetooth Assigned Numbers document. */
  BLUETOOTH_EIR_BLE_DEVICE_ADDRESS              = 0x1B, /**< 6 LSB bytes: Device address, 7th byte: Address type (Public/Random) */
  BLUETOOTH_EIR_BLE_ROLE                        = 0x1C, /**< Device Role: Periph only, Central only, Periph prefered, Central prefered */
  BLUETOOTH_EIR_MANUFACTURER_DATA               = 0xFF  /**< Manufacturer Specific Data (2 or more octets): The first 2 octets contain the Company Identifier Code followed by additional manufacturer specific data */
} Ndef_Bluetooth_Eir_Types_t;
   
   
/** @brief Bluetooth Flag field: LE limited discovery. */ 
#define NDEF_BLE_LIMITED_DISCOVERY_FLAG     (0x1)
/** @brief Bluetooth Flag field: LE general discovery. */ 
#define NDEF_BLE_GENERAL_DISCOVERY_FLAG     (0x2)
/** @brief Bluetooth Flag field: BR/EDR compliant. */ 
#define NDEF_BLE_NOT_BREDR_COMPLIANT_FLAG   (0x4)
/** @brief Bluetooth Flag field: LE & BR/EDR simultaneous support. */ 
#define NDEF_BLE_LE_BREDR_SIMULTANEOUS_FLAG (0x8)

/** @brief Enumerates the Bluetooth LE address types. */
typedef enum {
  NDEF_BLE_PUBLIC_ADDRESS_TYPE  = 0x0, /**< Public Device Address. */
  NDEF_BLE_RANDOM_ADDRESS_TYPE  = 0x1, /**< Random Device Address. */
  NDEF_BLE_UNDEF_ADDRESS_TYPE   = 0xff /**< Device Address is undefined. */
} Ndef_BLE_Address_Type_t;

/** @brief Enumerates Bluetooth LE Roles */
typedef enum {
  NDEF_BLE_ROLE_PERIPH_ONLY       = 0x0, /**< Only Peripheral Role supported. */
  NDEF_BLE_ROLE_CENTRAL_ONLY      = 0x1, /**< Only Central Role supported. */
  NDEF_BLE_ROLE_PERIPH_PREFERRED  = 0x2, /**< Peripheral and Central Role supported, Peripheral Role preferred for connection establishment. */
  NDEF_BLE_ROLE_CENTRAL_PREFERRED = 0x3, /**< Peripheral and Central Role supported, Central Role preferred for connection establishment. */
  NDEF_BLE_ROLE_UNDEF             = 0xff /**< LE Role is undefined. */
} Ndef_BLE_Role_t;

/** @brief Enumerates Bluetooth protocols */
typedef enum {
  NDEF_BLUETOOTH_BREDR, /**< Standard bluetooth */
  NDEF_BLUETOOTH_BLE    /**< Bluetooth Low Energy */
} Ndef_Bluetooth_type_t;

/** @brief Helper macro to set a particular EIR in the EIR mask of a `Ndef_Bluetooth_OOB_t` instance.
  * @param bt     Pointer to a `Ndef_Bluetooth_OOB_t` struct
  * @param option Index of the option based on the `Ndef_Bluetooth_Eir_Types_t` */
#define NDEF_BLUETOOTH_SET_OPTIONAL_MASK(bt,option)   do { bt->OptionalMask |= (1<<(option)); } while (0)  

/** @brief Helper macro to unset a particular EIR in the EIR mask of a `Ndef_Bluetooth_OOB_t` instance.
  * @param bt     Pointer to a `Ndef_Bluetooth_OOB_t` struct
  * @param option Index of the option based on the `Ndef_Bluetooth_Eir_Types_t` */
#define NDEF_BLUETOOTH_UNSET_OPTIONAL_MASK(bt,option) do {bt->OptionalMask &= ~(1<<(option)); } while (0)  

/** @brief Helper macro to know if a particular EIR is present in the EIR mask of a `Ndef_Bluetooth_OOB_t` instance.
  * @param bt     Pointer to a `Ndef_Bluetooth_OOB_t` struct
  * @param option Index of the option based on the `Ndef_Bluetooth_Eir_Types_t` */
#define NDEF_BLUETOOTH_GET_OPTIONAL_MASK(bt,option) ((bt->OptionalMask & (1<<(option))) >>  (option))

/** @brief Helper macro to create a mask for a particular EIR.
  * @param option Index of the option based on the `Ndef_Bluetooth_Eir_Types_t` */
#define NDEF_BLUETOOTH_OPTION(option)               (1<<(option))

/** @brief    Bluetooth Out-Of-Band data structure.
  * @details  Some fields are shared by both Br/Edr & LE Bluetooth, some are specific...
  *           No union used here to comply with the Cube coding guidelines, despite the fact that it could have save memory space.
  */
typedef struct
{
  /* Mandatory fields */
  Ndef_Bluetooth_type_t   Type;                               /**< *Mandatory*: Bluetooth type (Br/Edr or LE). */
  char                    DeviceAddress[6];                   /**< *Mandatory*: Device address. For LE: 6 LSB of Device Address EIR. */
  /* Optionnal common fields */
  uint32_t                OptionalMask;                       /**< This Mask is used to keep track of EIRs present in the OOB: each bit index corresponds to a `Ndef_Bluetooth_Eir_Types_t` value. */
  uint8_t                 Flags;                              /**< Flags from BLUETOOTH_EIR_FLAGS. */

  uint16_t                ClassUUID16[0xff/2];                /**< UUIDs from BLUETOOTH_EIR_SERVICE_CLASS_UUID_PARTIAL_16 or BLUETOOTH_EIR_SERVICE_CLASS_UUID_COMPLETE_16. */
  uint32_t                ClassUUID32[0xff/4];                /**< UUIDs from BLUETOOTH_EIR_SERVICE_CLASS_UUID_PARTIAL_32 or BLUETOOTH_EIR_SERVICE_CLASS_UUID_COMPLETE_32. */
  uint32_t                ClassUUID128[0xff/16][4];           /**< UUIDs from BLUETOOTH_EIR_SERVICE_CLASS_UUID_PARTIAL_128 or BLUETOOTH_EIR_SERVICE_CLASS_UUID_COMPLETE_128. */
  uint8_t                 nbUUID16;                           /**< Number of UUIDs speficied in ClassUUID16. */
  uint8_t                 nbUUID32;                           /**< Number of UUIDs speficied in ClassUUID32. */
  uint8_t                 nbUUID128;                          /**< Number of UUIDs speficied in ClassUUID128. */
  
  char                    LocalName[0xff];                    /**< Device local name from BLUETOOTH_EIR_SHORT_LOCAL_NAME or BLUETOOTH_EIR_COMPLETE_LOCAL_NAME. */
  uint8_t                 DeviceClass[3];                     /**< Class of device from BLUETOOTH_EIR_DEVICE_CLASS. */
  // if no TPL, TPL is unknown
  uint8_t                 TxPowerLevel;                       /**< TX power Level from BLUETOOTH_EIR_TX_POWER_LEVEL. */
  
  uint8_t                 SMFlags;                            /**< Security Manager Flags from BLUETOOTH_EIR_SECURITY_MANAGER_FLAGS. */
  uint16_t                SlaveConnIntervalRange[2];          /**< Slave Connection Interval Range from BLUETOOTH_EIR_SLAVE_CONNECTION_INTERVAL_RANGE. */
  uint16_t                ServiceSolicitation16[0xff/2];      /**< Service Solicitation from BLUETOOTH_EIR_SERVICE_SOLICITATION_16. */
  uint32_t                ServiceSolicitation128[0xff/16][4]; /**< Service Solicitation from BLUETOOTH_EIR_SERVICE_SOLICITATION_128. */
  uint8_t                 nbServiceSolicitation16;            /**< Number of UUIDs in ServiceSolicitation16. */
  uint8_t                 nbServiceSolicitation128;           /**< Number of UUIDs in ServiceSolicitation128. */
  /* for br/edr only */
    uint8_t                 SimplePairingHash[16];            /**< *For Br/Edr only*: Simple Pairing Hash from BLUETOOTH_EIR_SIMPLE_PAIRING_HASH. */
    uint8_t                 SimplePairingRandomizer[16];      /**< *For Br/Edr only*: Simple Pairing Randomizer from BLUETOOTH_EIR_SIMPLE_PAIRING_RANDOMIZER. */
  /* BLE mandatory fields */
    Ndef_BLE_Address_Type_t DeviceAddressType;                /**< *For LE only*, *Mandatory*: Address Type, MSB of the Device Address EIR (Public or Random) */
    Ndef_BLE_Role_t         Role;                             /**< *For LE only*, *Mandatory*: LE Role from BLUETOOTH_EIR_BLE_ROLE and defined in `Ndef_BLE_Role_t`. */
  /* BLE optional fields */
    uint8_t                 SecureManagerTK[16];              /**< *For LE only*: Security Manager TK from BLUETOOTH_EIR_SECURITY_MANAGER_TK_VALUE. */
    uint16_t                Appearance;                       /**< *For LE only*: Appearance from BLUETOOTH_EIR_APPEARANCE. */
  /* track number of unparsed EIR */
  uint8_t                 nbUnknown;                          /**< Number of EIRs not parsed because of an unknown EIR type. */
  uint8_t                 nbServiceData;                      /**< Number of EIRs not parsed because being of type BLUETOOTH_EIR_SERVICE_DATA. */
  uint8_t                 nbManufacturerData;                 /**< Number of EIRs not parsed because being of type BLUETOOTH_EIR_MANUFACTURER_DATA. */
} Ndef_Bluetooth_OOB_t ;

/** @brief    Extended Inquiry Response format.
  * @details  This structure is used to parse/build EIRs in the Bluetooth OOB.
  */
typedef  struct {
  uint8_t length;   /**< Length of the EIRs, including the type field (1 byte). */
  uint8_t type;     /**< EIR type as defined in `Ndef_Bluetooth_Eir_Types_t`. */
  uint8_t value[1]; /**< single-element array, to keep track of the address of the EIR data */
}  NDEF_EIR_t ;

uint16_t NDEF_ReadBluetoothOOB( sRecordInfo_t *pRecordStruct, Ndef_Bluetooth_OOB_t *pBluetooth );
uint16_t NDEF_AppendBluetoothOOB( Ndef_Bluetooth_OOB_t *pBluetooth, char* RecordID );
void NDEF_PrepareBluetoothMessage(Ndef_Bluetooth_OOB_t  *pBluetooth, uint8_t *pNDEFMessage, uint16_t *size );
uint32_t NDEF_GetBluetoothOOBLength( Ndef_Bluetooth_OOB_t *pBluetooth );

#ifdef __cplusplus
}
#endif

/** @}
  */

#endif /* __LIB_NDEF_BLUETOOTH_H */

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
