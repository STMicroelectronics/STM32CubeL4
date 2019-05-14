/**
 ******************************************************************************
 * @file    ble_status.h
 * @author  MCD Application Team
 * @brief   Header file with BLE Stack status codes.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license SLA0044,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        http://www.st.com/SLA0044
 *
 ******************************************************************************
 */

#ifndef __BLE_STATUS_H__
#define __BLE_STATUS_H__

#include <stdint.h>

/** @defgroup ble_status Bluetooth Status/Error Codes
 * @{
 */
 
typedef uint8_t tBleStatus; 

/* Error Codes as specified by the specification 
 * according to the spec the error codes range
 * from 0x00 to 0x3F 
 */
#define ERR_CMD_SUCCESS                             (0x00)
#define ERR_UNKNOWN_HCI_COMMAND                     (0x01)
#define ERR_UNKNOWN_CONN_IDENTIFIER                 (0x02)

#define ERR_AUTH_FAILURE                            (0x05)
#define ERR_PIN_OR_KEY_MISSING                      (0x06)
#define ERR_MEM_CAPACITY_EXCEEDED                   (0x07)
#define ERR_CONNECTION_TIMEOUT                      (0x08)

#define ERR_COMMAND_DISALLOWED                      (0x0C)

#define ERR_UNSUPPORTED_FEATURE                     (0x11)
#define ERR_INVALID_HCI_CMD_PARAMS                  (0x12)
#define ERR_RMT_USR_TERM_CONN                       (0x13)
#define ERR_RMT_DEV_TERM_CONN_LOW_RESRCES           (0x14)
#define ERR_RMT_DEV_TERM_CONN_POWER_OFF             (0x15)
#define ERR_LOCAL_HOST_TERM_CONN                    (0x16)

#define ERR_UNSUPP_RMT_FEATURE                      (0x1A)

#define ERR_INVALID_LMP_PARAM                       (0x1E)
#define ERR_UNSPECIFIED_ERROR                       (0x1F)

#define ERR_LL_RESP_TIMEOUT                         (0x22)
#define ERR_LMP_PDU_NOT_ALLOWED                     (0x24)

#define ERR_INSTANT_PASSED                          (0x28)

#define ERR_PAIR_UNIT_KEY_NOT_SUPP                  (0x29)
#define ERR_CONTROLLER_BUSY                         (0x3A)

#define ERR_DIRECTED_ADV_TIMEOUT                    (0x3C)

#define ERR_CONN_END_WITH_MIC_FAILURE               (0x3D)

#define ERR_CONN_FAILED_TO_ESTABLISH                (0x3E)


#define BLE_STATUS_SUCCESS                          (0x00)
#define BLE_STATUS_FAILED                           (0x41)
#define BLE_STATUS_INVALID_PARAMS                   (0x42)
#define BLE_STATUS_BUSY                             (0x43)
#define BLE_STATUS_INVALID_LEN_PDU                  (0x44)
#define BLE_STATUS_PENDING                          (0x45)
#define BLE_STATUS_NOT_ALLOWED                      (0x46)
#define BLE_STATUS_ERROR                            (0x47)
#define BLE_STATUS_ADDR_NOT_RESOLVED                (0x48)

#define FLASH_READ_FAILED                           (0x49)
#define FLASH_WRITE_FAILED                          (0x4A)
#define FLASH_ERASE_FAILED                          (0x4B)

#define BLE_STATUS_INVALID_CID                      (0x50)

#define TIMER_NOT_VALID_LAYER                       (0x54)
#define TIMER_INSUFFICIENT_RESOURCES                (0x55)
 
#define BLE_STATUS_CSRK_NOT_FOUND                   (0x5A)
#define BLE_STATUS_IRK_NOT_FOUND                    (0x5B)
#define BLE_STATUS_DEV_NOT_FOUND_IN_DB              (0x5C)
#define BLE_STATUS_SEC_DB_FULL                      (0x5D)
#define BLE_STATUS_DEV_NOT_BONDED                   (0x5E)
#define BLE_STATUS_DEV_IN_BLACKLIST                 (0x5F)
 
#define BLE_STATUS_INVALID_HANDLE                   (0x60)
#define BLE_STATUS_INVALID_PARAMETER                (0x61)
#define BLE_STATUS_OUT_OF_HANDLE                    (0x62)
#define BLE_STATUS_INVALID_OPERATION                (0x63)
#define BLE_STATUS_INSUFFICIENT_RESOURCES           (0x64)
#define BLE_INSUFFICIENT_ENC_KEYSIZE                (0x65)
#define BLE_STATUS_CHARAC_ALREADY_EXISTS            (0x66)

 /*
  * Library Error Codes
  */
#define BLE_STATUS_TIMEOUT                          (0xFF)
#define BLE_STATUS_PROFILE_ALREADY_INITIALIZED      (0xF0)
#define BLE_STATUS_NULL_PARAM                       (0xF1) 

/**
 * @}
 */
 

#endif /* __BLE_STATUS_H__ */
