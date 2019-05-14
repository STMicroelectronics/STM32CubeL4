/**
 ******************************************************************************
 * @file    dis.c
 * @author  MCD Application Team
 * @brief   Device Information Service
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

/* Includes ------------------------------------------------------------------*/
#include <string.h>   

#include "config.h"   /* provided by the applic. see config/ble_config_templete.h */
#include "debug.h"    /* provided by the applic. see config/ble_debug_templete.h */

#include "svc_private.h"
#include "ble_lib.h"
#include "svc_ctl.h"
#include "dis.h"

/* Private typedef -----------------------------------------------------------*/
#if (BLE_CFG_MENU_DEVICE_INFORMATION == 1)
typedef struct
{
  uint16_t  DeviceInformationSvcHdle;       /**< Service handle */
  uint16_t  ManufacturerNameStringCharHdle; /**< Characteristic handle */
  uint16_t  ModelNumberStringCharHdle;      /**< Characteristic handle */
  uint16_t  SerialNumberStringCharHdle;     /**< Characteristic handle */
  uint16_t  HardwareRevisionStringCharHdle; /**< Characteristic handle */
  uint16_t  FirmwareRevisionStringCharHdle; /**< Characteristic handle */
  uint16_t  SoftwareRevisionStringCharHdle; /**< Characteristic handle */
  uint16_t  SystemIDCharHdle;               /**< Characteristic handle */
  uint16_t  IEEECertificationCharHdle;      /**< Characteristic handle */
  uint16_t  PNPIDCharHdle;                  /**< Characteristic handle */
}DIS_Context_t;
#else
typedef struct
{
  uint16_t  DeviceInformationSvcHdle;       /**< Service handle */
#if (BLE_CFG_DIS_MANUFACTURER_NAME_STRING != 0)
  uint16_t  ManufacturerNameStringCharHdle; /**< Characteristic handle */
#endif
#if (BLE_CFG_DIS_MODEL_NUMBER_STRING != 0)
  uint16_t  ModelNumberStringCharHdle;      /**< Characteristic handle */
#endif
#if (BLE_CFG_DIS_SERIAL_NUMBER_STRING != 0)
  uint16_t  SerialNumberStringCharHdle;     /**< Characteristic handle */
#endif
#if (BLE_CFG_DIS_HARDWARE_REVISION_STRING != 0)
  uint16_t  HardwareRevisionStringCharHdle; /**< Characteristic handle */
#endif
#if (BLE_CFG_DIS_FIRMWARE_REVISION_STRING != 0)
  uint16_t  FirmwareRevisionStringCharHdle; /**< Characteristic handle */
#endif
#if (BLE_CFG_DIS_SOFTWARE_REVISION_STRING != 0)
  uint16_t  SoftwareRevisionStringCharHdle; /**< Characteristic handle */
#endif
#if (BLE_CFG_DIS_SYSTEM_ID != 0)
  uint16_t  SystemIDCharHdle;               /**< Characteristic handle */
#endif
#if (BLE_CFG_DIS_IEEE_CERTIFICATION != 0)
  uint16_t  IEEECertificationCharHdle;      /**< Characteristic handle */
#endif
#if (BLE_CFG_DIS_PNP_ID != 0)
  uint16_t  PNPIDCharHdle;                  /**< Characteristic handle */
#endif
}DIS_Context_t;
#endif


/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
 * START of Section BLE_BLUENRG_DRIVER_CONTEXT
 */
static DIS_Context_t DIS_Context;
/**
 * END of Section BLE_BLUENRG_DRIVER_CONTEXT
 */


/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void DIS_Init(uint16_t *options)
{
  uint16_t uuid;
  tBleStatus hciCmdResult;

  memset ( &DIS_Context, 0, sizeof(DIS_Context_t) );

  /**
   *  Register the event handler to the BLE controller
   *
   *  There is no need of an interrupt handler for this service
   */

#if (BLE_CFG_MENU_DEVICE_INFORMATION == 0)
  /**
   *  Add Device Information Service
   */
  uuid = DEVICE_INFORMATION_SERVICE_UUID;
  hciCmdResult = aci_gatt_add_serv(UUID_TYPE_16,
                                   (const uint8_t *) &uuid,
                                   PRIMARY_SERVICE,
                                   1
#if (BLE_CFG_DIS_MANUFACTURER_NAME_STRING != 0)
                                   +2
#endif
#if (BLE_CFG_DIS_MODEL_NUMBER_STRING != 0)
                                   +2
#endif
#if (BLE_CFG_DIS_SERIAL_NUMBER_STRING != 0)
                                   +2
#endif
#if (BLE_CFG_DIS_HARDWARE_REVISION_STRING != 0)
                                   +2
#endif
#if (BLE_CFG_DIS_FIRMWARE_REVISION_STRING != 0)
                                   +2
#endif
#if (BLE_CFG_DIS_SOFTWARE_REVISION_STRING != 0)
                                   +2
#endif
#if (BLE_CFG_DIS_SYSTEM_ID != 0)
                                   +2
#endif
#if (BLE_CFG_DIS_IEEE_CERTIFICATION != 0)
                                   +2
#endif
#if (BLE_CFG_DIS_PNP_ID != 0)
                                   +2
#endif
                                   +(2*BLE_CFG_DIS_NUMBER_OF_CUSTOM_CHAR),   /**< Custom Characteristic */
                                   &(DIS_Context.DeviceInformationSvcHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    DIS_DBG_MSG ("DIS_Init(), Device Information Service (DIS) is added Successfully %04X\n", 
                 DIS_Context.DeviceInformationSvcHdle);
  }
  else
  {
    DIS_DBG_MSG ("DIS_Init(), FAILED to add Device Information Service (DIS), Error: %02X !!\n", 
                 hciCmdResult);
  }

#if (BLE_CFG_DIS_MANUFACTURER_NAME_STRING != 0)
  /**
   *  Add Manufacturer Name String Characteristic
   */
  uuid = MANUFACTURER_NAME_UUID;
  hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                   UUID_TYPE_16,
                                   (const uint8_t *) &uuid ,
                                   BLE_CFG_DIS_MANUFACTURER_NAME_STRING_LEN_MAX,
                                   CHAR_PROP_READ,
                                   ATTR_PERMISSION_NONE,
                                   GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                                   &(DIS_Context.ManufacturerNameStringCharHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    DIS_DBG_MSG ("DIS_Init(), Manufacturer Name Characteristic Added Successfully  %04X \n", 
                 DIS_Context.ManufacturerNameStringCharHdle);
  }
  else
  {
    DIS_DBG_MSG ("DIS_Init(), FAILED to add Manufacturer Name Characteristic, Error: %02X !!\n", 
                hciCmdResult);
  }

#endif

#if (BLE_CFG_DIS_MODEL_NUMBER_STRING != 0)
  /**
   *  Add Model Number String Characteristic
   */
  uuid = MODEL_NUMBER_UUID;
  hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                   UUID_TYPE_16,
                                   (const uint8_t *) &uuid ,
                                   BLE_CFG_DIS_MODEL_NUMBER_STRING_LEN_MAX,
                                   CHAR_PROP_READ,
                                   ATTR_PERMISSION_NONE,
                                   GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                                   &(DIS_Context.ModelNumberStringCharHdle));
  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    DIS_DBG_MSG ("DIS_Init(), Model Number String Characteristic Added Successfully  %04X \n", 
                 DIS_Context.ModelNumberStringCharHdle);
  }
  else
  {
    DIS_DBG_MSG ("DIS_Init(), FAILED to add Model Number String Characteristic, Error: %02X !!\n", 
                hciCmdResult);
  }

#endif

#if (BLE_CFG_DIS_SERIAL_NUMBER_STRING != 0)
  /**
   *  Add Serial Number String Characteristic
   */
  uuid = SERIAL_NUMBER_UUID;
  hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                   UUID_TYPE_16,
                                   (const uint8_t *) &uuid ,
                                   BLE_CFG_DIS_SERIAL_NUMBER_STRING_LEN_MAX,
                                   CHAR_PROP_READ,
                                   ATTR_PERMISSION_NONE,
                                   GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                                   &(DIS_Context.SerialNumberStringCharHdle));
  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    DIS_DBG_MSG ("DIS_Init(), Serial Number String Characteristic Added Successfully  %04X \n", 
                 DIS_Context.SerialNumberStringCharHdle);
  }
  else
  {
    DIS_DBG_MSG ("DIS_Init(), FAILED to add Serial Number String Characteristic, Error: %02X !!\n", 
                hciCmdResult);
  }

#endif

#if (BLE_CFG_DIS_HARDWARE_REVISION_STRING != 0)
  /**
   *  Add Hardware Revision String Characteristic
   */
  uuid = HARDWARE_REVISION_UUID;
  hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                   UUID_TYPE_16,
                                   (const uint8_t *) &uuid ,
                                   BLE_CFG_DIS_HARDWARE_REVISION_STRING_LEN_MAX,
                                   CHAR_PROP_READ,
                                   ATTR_PERMISSION_NONE,
                                   GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                                   &(DIS_Context.HardwareRevisionStringCharHdle));
  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    DIS_DBG_MSG ("DIS_Init(), Hardware Revision String Characteristic Added Successfully  %04X \n", 
                 DIS_Context.HardwareRevisionStringCharHdle);
  }
  else
  {
    DIS_DBG_MSG ("DIS_Init(), FAILED to add Hardware Revision String Characteristic, Error: %02X !!\n", 
                hciCmdResult);
  }

#endif

#if (BLE_CFG_DIS_FIRMWARE_REVISION_STRING != 0)
  /**
   *  Add Firmware Revision String Characteristic
   */
  uuid = FIRMWARE_REVISION_UUID;
  hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                   UUID_TYPE_16,
                                   (const uint8_t *) &uuid ,
                                   BLE_CFG_DIS_FIRMWARE_REVISION_STRING_LEN_MAX,
                                   CHAR_PROP_READ,
                                   ATTR_PERMISSION_NONE,
                                   GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                                   &(DIS_Context.FirmwareRevisionStringCharHdle));
  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    DIS_DBG_MSG ("DIS_Init(), Firmware Revision String Characteristic Added Successfully  %04X \n", 
                 DIS_Context.FirmwareRevisionStringCharHdle);
  }
  else
  {
    DIS_DBG_MSG ("DIS_Init(), FAILED to add Firmware Revision String Characteristic, Error: %02X !!\n", 
                hciCmdResult);
  }
#endif

#if (BLE_CFG_DIS_SOFTWARE_REVISION_STRING != 0)
  /**
   *  Add Software Revision String Characteristic
   */
  uuid = SOFTWARE_REVISION_UUID;
  hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                   UUID_TYPE_16,
                                   (const uint8_t *) &uuid ,
                                   BLE_CFG_DIS_SOFTWARE_REVISION_STRING_LEN_MAX,
                                   CHAR_PROP_READ,
                                   ATTR_PERMISSION_NONE,
                                   GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                                   &(DIS_Context.SoftwareRevisionStringCharHdle));
  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    DIS_DBG_MSG ("DIS_Init(), Software Revision String Characteristic Added Successfully  %04X \n", 
                 DIS_Context.SoftwareRevisionStringCharHdle);
  }
  else
  {
    DIS_DBG_MSG ("DIS_Init(), FAILED to add Software Revision String Characteristic, Error: %02X !!\n", 
                hciCmdResult);
  }
#endif

#if (BLE_CFG_DIS_SYSTEM_ID != 0)
  /**
   *  Add System ID Characteristic
   */
  uuid = SYSTEM_ID_UUID;
  hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                   UUID_TYPE_16,
                                   (const uint8_t *) &uuid ,
                                   BLE_CFG_DIS_SYSTEM_ID_LEN_MAX,
                                   CHAR_PROP_READ,
                                   ATTR_PERMISSION_NONE,
                                   GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   CHAR_VALUE_LEN_CONSTANT, /* isVariable */
                                   &(DIS_Context.SystemIDCharHdle));
  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    DIS_DBG_MSG ("DIS_Init(), System ID Characteristic Added Successfully  %04X \n", 
                 DIS_Context.SystemIDCharHdle);
  }
  else
  {
    DIS_DBG_MSG ("DIS_Init(), FAILED to add System ID Characteristic, Error: %02X !!\n", 
                hciCmdResult);
  }
#endif

#if (BLE_CFG_DIS_IEEE_CERTIFICATION != 0)
  /**
   *  Add IEEE CertificationCharacteristic
   */
  uuid = IEEE_CERTIFICATION_UUID;
  hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                   UUID_TYPE_16,
                                   (const uint8_t *) &uuid ,
                                   BLE_CFG_DIS_IEEE_CERTIFICATION_LEN_MAX,
                                   CHAR_PROP_READ,
                                   ATTR_PERMISSION_NONE,
                                   GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                                   &(DIS_Context.IEEECertificationCharHdle));
  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    DIS_DBG_MSG ("DIS_Init(), IEEE Certification Characteristic Added Successfully  %04X \n", 
                 DIS_Context.IEEECertificationCharHdle);
  }
  else
  {
    DIS_DBG_MSG ("DIS_Init(), FAILED to add IEEE Certification Characteristic, Error: %02X !!\n", 
                hciCmdResult);
  }
#endif

#if (BLE_CFG_DIS_PNP_ID != 0)
  /**
   *  Add PNP ID Characteristic
   */
  uuid = PNP_ID_UUID;
  hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                   UUID_TYPE_16,
                                   (const uint8_t *) &uuid ,
                                   BLE_CFG_DIS_PNP_ID_LEN_MAX,
                                   CHAR_PROP_READ,
                                   ATTR_PERMISSION_NONE,
                                   GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   CHAR_VALUE_LEN_CONSTANT, /* isVariable */
                                   &(DIS_Context.PNPIDCharHdle));
  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    DIS_DBG_MSG ("DIS_Init(), PNP ID Characteristic Added Successfully  %04X \n", 
                 DIS_Context.PNPIDCharHdle);
  }
  else
  {
    DIS_DBG_MSG ("DIS_Init(), FAILED to add PNP ID Characteristic, Error: %02X !!\n", 
                hciCmdResult);
  }
#endif
#else
  uint8_t nb_attrib = 1, i;
  
  for(i = 0; i < NB_DIS_OPTIONS; i++)
    if(*options & (1 << i))
      nb_attrib += 2;

  nb_attrib += (2*BLE_CFG_DIS_NUMBER_OF_CUSTOM_CHAR);
  
  /**
   *  Add Device Information Service
   */
  uuid = DEVICE_INFORMATION_SERVICE_UUID;
  hciCmdResult = aci_gatt_add_serv(UUID_TYPE_16,
                                   (const uint8_t *) &uuid,
                                   PRIMARY_SERVICE,
                                   nb_attrib,
                                   &(DIS_Context.DeviceInformationSvcHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    DIS_DBG_MSG ("DIS_Init(), Device Information Service (DIS) is added Successfully %04X\n", 
                 DIS_Context.DeviceInformationSvcHdle);
  }
  else
  {
    DIS_DBG_MSG ("DIS_Init(), FAILED to add Device Information Service (DIS), Error: %02X !!\n", 
                 hciCmdResult);
  }

  i = 0;
  if(*options & (1 << i))
  {
    /**
     *  Add Manufacturer Name String Characteristic
     */
    uuid = MANUFACTURER_NAME_UUID;
    hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                     UUID_TYPE_16,
                                     (const uint8_t *) &uuid ,
                                     BLE_CFG_DIS_MANUFACTURER_NAME_STRING_LEN_MAX,
                                     CHAR_PROP_READ,
                                     ATTR_PERMISSION_NONE,
                                     GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                     10, /* encryKeySize */
                                     CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                                     &(DIS_Context.ManufacturerNameStringCharHdle));

    if (hciCmdResult == BLE_STATUS_SUCCESS)
    {
      DIS_DBG_MSG ("DIS_Init(), Manufacturer Name Characteristic Added Successfully  %04X \n", 
                   DIS_Context.ManufacturerNameStringCharHdle);
    }
    else
    {
      DIS_DBG_MSG ("DIS_Init(), FAILED to add Manufacturer Name Characteristic, Error: %02X !!\n", 
                  hciCmdResult);
    }
  }

  i++;
  if(*options & (1 << i))
  {
    /**
     *  Add Model Number String Characteristic
     */
    uuid = MODEL_NUMBER_UUID;
  hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                     UUID_TYPE_16,
                                     (const uint8_t *) &uuid ,
                                     BLE_CFG_DIS_MODEL_NUMBER_STRING_LEN_MAX,
                                     CHAR_PROP_READ,
                                     ATTR_PERMISSION_NONE,
                                     GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                     10, /* encryKeySize */
                                     CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                                     &(DIS_Context.ModelNumberStringCharHdle));
    if (hciCmdResult == BLE_STATUS_SUCCESS)
    {
      DIS_DBG_MSG ("DIS_Init(), Model Number String Characteristic Added Successfully  %04X \n", 
                   DIS_Context.ModelNumberStringCharHdle);
    }
    else
    {
      DIS_DBG_MSG ("DIS_Init(), FAILED to add Model Number String Characteristic, Error: %02X !!\n", 
                  hciCmdResult);
    }
  }

  i++;
  if(*options & (1 << i))
  {
    /**
     *  Add Serial Number String Characteristic
     */
    uuid = SERIAL_NUMBER_UUID;
    hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                     UUID_TYPE_16,
                                     (const uint8_t *) &uuid ,
                                     BLE_CFG_DIS_SERIAL_NUMBER_STRING_LEN_MAX,
                                     CHAR_PROP_READ,
                                     ATTR_PERMISSION_NONE,
                                     GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                     10, /* encryKeySize */
                                     CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                                     &(DIS_Context.SerialNumberStringCharHdle));
    if (hciCmdResult == BLE_STATUS_SUCCESS)
    {
      DIS_DBG_MSG ("DIS_Init(), Serial Number String Characteristic Added Successfully  %04X \n", 
                   DIS_Context.SerialNumberStringCharHdle);
    }
    else
    {
      DIS_DBG_MSG ("DIS_Init(), FAILED to add Serial Number String Characteristic, Error: %02X !!\n", 
                  hciCmdResult);
    }
  }

  i++;
  if(*options & (1 << i))
  {
    /**
     *  Add Serial Number String Characteristic
     */
    uuid = HARDWARE_REVISION_UUID;
    hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                     UUID_TYPE_16,
                                     (const uint8_t *) &uuid ,
                                     BLE_CFG_DIS_HARDWARE_REVISION_STRING_LEN_MAX,
                                     CHAR_PROP_READ,
                                     ATTR_PERMISSION_NONE,
                                     GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                     10, /* encryKeySize */
                                     CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                                     &(DIS_Context.HardwareRevisionStringCharHdle));
    if (hciCmdResult == BLE_STATUS_SUCCESS)
    {
      DIS_DBG_MSG ("DIS_Init(), Hardware Revision String Characteristic Added Successfully  %04X \n", 
                   DIS_Context.HardwareRevisionStringCharHdle);
    }
    else
    {
      DIS_DBG_MSG ("DIS_Init(), FAILED to add Hardware Revision String Characteristic, Error: %02X !!\n", 
                  hciCmdResult);
    }
  }

  i++;
  if(*options & (1 << i))
  {
    /**
     *  Add Firmware Revision String Characteristic
     */
    uuid = FIRMWARE_REVISION_UUID;
    hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                     UUID_TYPE_16,
                                     (const uint8_t *) &uuid ,
                                     BLE_CFG_DIS_FIRMWARE_REVISION_STRING_LEN_MAX,
                                     CHAR_PROP_READ,
                                     ATTR_PERMISSION_NONE,
                                     GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                     10, /* encryKeySize */
                                     CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                                     &(DIS_Context.FirmwareRevisionStringCharHdle));
    if (hciCmdResult == BLE_STATUS_SUCCESS)
    {
      DIS_DBG_MSG ("DIS_Init(), Firmware Revision String Characteristic Added Successfully  %04X \n", 
                   DIS_Context.FirmwareRevisionStringCharHdle);
    }
    else
    {
      DIS_DBG_MSG ("DIS_Init(), FAILED to add Firmware Revision String Characteristic, Error: %02X !!\n", 
                  hciCmdResult);
    }
  }

  i++;
  if(*options & (1 << i))
  {
    /**
     *  Add Software Revision String Characteristic
     */
    uuid = SOFTWARE_REVISION_UUID;
    hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                     UUID_TYPE_16,
                                     (const uint8_t *) &uuid ,
                                     BLE_CFG_DIS_SOFTWARE_REVISION_STRING_LEN_MAX,
                                     CHAR_PROP_READ,
                                     ATTR_PERMISSION_NONE,
                                     GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                     10, /* encryKeySize */
                                     CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                                     &(DIS_Context.SoftwareRevisionStringCharHdle));
    if (hciCmdResult == BLE_STATUS_SUCCESS)
    {
      DIS_DBG_MSG ("DIS_Init(), Software Revision String Characteristic Added Successfully  %04X \n", 
                   DIS_Context.SoftwareRevisionStringCharHdle);
    }
    else
    {
      DIS_DBG_MSG ("DIS_Init(), FAILED to add Software Revision String Characteristic, Error: %02X !!\n", 
                  hciCmdResult);
    }
  }

  i++;
  if(*options & (1 << i))
  {
    /**
     *  Add System ID Characteristic
     */
    uuid = SYSTEM_ID_UUID;
    hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                     UUID_TYPE_16,
                                     (const uint8_t *) &uuid ,
                                     BLE_CFG_DIS_SYSTEM_ID_LEN_MAX,
                                     CHAR_PROP_READ,
                                     ATTR_PERMISSION_NONE,
                                     GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                     10, /* encryKeySize */
                                     CHAR_VALUE_LEN_CONSTANT, /* isVariable */
                                     &(DIS_Context.SystemIDCharHdle));
    if (hciCmdResult == BLE_STATUS_SUCCESS)
    {
      DIS_DBG_MSG ("DIS_Init(), System ID Characteristic Added Successfully  %04X \n", 
                   DIS_Context.SystemIDCharHdle);
    }
    else
    {
      DIS_DBG_MSG ("DIS_Init(), FAILED to add System ID Characteristic, Error: %02X !!\n", 
                  hciCmdResult);
    }
  }

  i++;
  if(*options & (1 << i))
  {
    /**
     *  Add IEEE CertificationCharacteristic
     */
    uuid = IEEE_CERTIFICATION_UUID;
    hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                     UUID_TYPE_16,
                                     (const uint8_t *) &uuid ,
                                     BLE_CFG_DIS_IEEE_CERTIFICATION_LEN_MAX,
                                     CHAR_PROP_READ,
                                     ATTR_PERMISSION_NONE,
                                     GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                     10, /* encryKeySize */
                                     CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                                     &(DIS_Context.IEEECertificationCharHdle));
    if (hciCmdResult == BLE_STATUS_SUCCESS)
    {
      DIS_DBG_MSG ("DIS_Init(), IEEE Certification Characteristic Added Successfully  %04X \n", 
                   DIS_Context.IEEECertificationCharHdle);
    }
    else
    {
      DIS_DBG_MSG ("DIS_Init(), FAILED to add IEEE Certification Characteristic, Error: %02X !!\n", 
                  hciCmdResult);
    }
  }

  i++;
  if(*options & (1 << i))
  {
    /**
     *  Add PNP ID Characteristic
     */
    uuid = PNP_ID_UUID;
    hciCmdResult = aci_gatt_add_char(DIS_Context.DeviceInformationSvcHdle,
                                     UUID_TYPE_16,
                                     (const uint8_t *) &uuid ,
                                     BLE_CFG_DIS_PNP_ID_LEN_MAX,
                                     CHAR_PROP_READ,
                                     ATTR_PERMISSION_NONE,
                                     GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                     10, /* encryKeySize */
                                     CHAR_VALUE_LEN_CONSTANT, /* isVariable */
                                     &(DIS_Context.PNPIDCharHdle));
    if (hciCmdResult == BLE_STATUS_SUCCESS)
    {
      DIS_DBG_MSG ("DIS_Init(), PNP ID Characteristic Added Successfully  %04X \n", 
                   DIS_Context.PNPIDCharHdle);
    }
    else
    {
      DIS_DBG_MSG ("DIS_Init(), FAILED to add PNP ID Characteristic, Error: %02X !!\n", 
                  hciCmdResult);
    }
  }
#endif
#if (BLE_CFG_DIS_NUMBER_OF_CUSTOM_CHAR != 0)
  BLESVC_AddCustomChar((uint16)t)DEVICE_INFORMATION_SERVICE_UUID, DIS_Context.DeviceInformationSvcHdle);
#endif

  return;
}

/**
 * @brief  Characteristic update
 * @param  UUID: UUID of the characteristic
 * @retval None
 */
tBleStatus DIS_UpdateChar(uint16_t UUID, DIS_Data_t *pPData)
{
  tBleStatus return_value;

  switch(UUID)
  {
#if (BLE_CFG_DIS_MANUFACTURER_NAME_STRING != 0)
    case MANUFACTURER_NAME_UUID:
      return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                DIS_Context.ManufacturerNameStringCharHdle,
                                                0,
                                                pPData->Length,
                                                (const uint8_t *)pPData->pPayload);
      break;
#else
#if(BLE_CFG_MENU_DEVICE_INFORMATION == 1)
    case MANUFACTURER_NAME_UUID:
      if(DIS_Context.ManufacturerNameStringCharHdle != 0)
        return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                  DIS_Context.ManufacturerNameStringCharHdle,
                                                  0,
                                                  pPData->Length,
                                                  (const uint8_t *)pPData->pPayload);
      break;
#endif
#endif

#if (BLE_CFG_DIS_MODEL_NUMBER_STRING != 0)
    case MODEL_NUMBER_UUID:
      return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                DIS_Context.ModelNumberStringCharHdle,
                                                0,
                                                pPData->Length,
                                                (const uint8_t *)pPData->pPayload);
      break;
#else
#if(BLE_CFG_MENU_DEVICE_INFORMATION == 1)
    case MODEL_NUMBER_UUID:
      if(DIS_Context.ModelNumberStringCharHdle != 0)
        return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                  DIS_Context.ModelNumberStringCharHdle,
                                                  0,
                                                  pPData->Length,
                                                  (const uint8_t *)pPData->pPayload);
      break;
#endif
#endif

#if (BLE_CFG_DIS_SERIAL_NUMBER_STRING != 0)
    case SERIAL_NUMBER_UUID:
      return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                DIS_Context.SerialNumberStringCharHdle,
                                                0,
                                                pPData->Length,
                                                (const uint8_t *)pPData->pPayload);
      break;
#else
#if(BLE_CFG_MENU_DEVICE_INFORMATION == 1)
    case SERIAL_NUMBER_UUID:
      if(DIS_Context.SerialNumberStringCharHdle != 0)
        return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                  DIS_Context.SerialNumberStringCharHdle,
                                                  0,
                                                  pPData->Length,
                                                  (const uint8_t *)pPData->pPayload);
      break;
#endif
#endif

#if (BLE_CFG_DIS_HARDWARE_REVISION_STRING != 0)
    case HARDWARE_REVISION_UUID:
      return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                DIS_Context.HardwareRevisionStringCharHdle,
                                                0,
                                                pPData->Length,
                                                (const uint8_t *)pPData->pPayload);
      break;
#else
#if(BLE_CFG_MENU_DEVICE_INFORMATION == 1)
    case HARDWARE_REVISION_UUID:
      if(DIS_Context.HardwareRevisionStringCharHdle != 0)
        return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                  DIS_Context.HardwareRevisionStringCharHdle,
                                                  0,
                                                  pPData->Length,
                                                  (const uint8_t *)pPData->pPayload);
      break;
#endif
#endif

#if (BLE_CFG_DIS_FIRMWARE_REVISION_STRING != 0)
    case FIRMWARE_REVISION_UUID:
      return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                DIS_Context.FirmwareRevisionStringCharHdle,
                                                0,
                                                pPData->Length,
                                                (const uint8_t *)pPData->pPayload);
      break;
#else
#if(BLE_CFG_MENU_DEVICE_INFORMATION == 1)
    case FIRMWARE_REVISION_UUID:
      if(DIS_Context.FirmwareRevisionStringCharHdle != 0)
        return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                  DIS_Context.FirmwareRevisionStringCharHdle,
                                                  0,
                                                  pPData->Length,
                                                  (const uint8_t *)pPData->pPayload);
      break;
#endif
#endif

#if (BLE_CFG_DIS_SOFTWARE_REVISION_STRING != 0)
    case SOFTWARE_REVISION_UUID:
      return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                DIS_Context.SoftwareRevisionStringCharHdle,
                                                0,
                                                pPData->Length,
                                                (const uint8_t *)pPData->pPayload);
      break;
#else
#if(BLE_CFG_MENU_DEVICE_INFORMATION == 1)
    case SOFTWARE_REVISION_UUID:
      if(DIS_Context.SoftwareRevisionStringCharHdle != 0)
        return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                  DIS_Context.SoftwareRevisionStringCharHdle,
                                                  0,
                                                  pPData->Length,
                                                  (const uint8_t *)pPData->pPayload);
      break;
#endif
#endif

#if (BLE_CFG_DIS_SYSTEM_ID != 0)
    case SYSTEM_ID_UUID:
      return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                DIS_Context.SystemIDCharHdle,
                                                0,
                                                pPData->Length,
                                                (const uint8_t *)pPData->pPayload);
      break;
#else
#if(BLE_CFG_MENU_DEVICE_INFORMATION == 1)
    case SYSTEM_ID_UUID:
      if(DIS_Context.SystemIDCharHdle != 0)
        return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                  DIS_Context.SystemIDCharHdle,
                                                  0,
                                                  pPData->Length,
                                                  (const uint8_t *)pPData->pPayload);
      break;
#endif
#endif

#if (BLE_CFG_DIS_IEEE_CERTIFICATION != 0)
    case IEEE_CERTIFICATION_UUID:
      return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                DIS_Context.IEEECertificationCharHdle,
                                                0,
                                                pPData->Length,
                                                (const uint8_t *)pPData->pPayload);
      break;
#else
#if(BLE_CFG_MENU_DEVICE_INFORMATION == 1)
    case IEEE_CERTIFICATION_UUID:
      if(DIS_Context.IEEECertificationCharHdle != 0)
        return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                  DIS_Context.IEEECertificationCharHdle,
                                                  0,
                                                  pPData->Length,
                                                  (const uint8_t *)pPData->pPayload);
      break;
#endif
#endif

#if (BLE_CFG_DIS_PNP_ID != 0)
    case PNP_ID_UUID:
      return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                DIS_Context.PNPIDCharHdle,
                                                0,
                                                pPData->Length,
                                                (const uint8_t *)pPData->pPayload);
      break;
#else
#if(BLE_CFG_MENU_DEVICE_INFORMATION == 1)
    case PNP_ID_UUID:
      if(DIS_Context.SystemIDCharHdle != 0)
        return_value = aci_gatt_update_char_value(DIS_Context.DeviceInformationSvcHdle,
                                                  DIS_Context.PNPIDCharHdle,
                                                  0,
                                                  pPData->Length,
                                                  (const uint8_t *)pPData->pPayload);
      break;
#endif
#endif

    default:
      return_value = BLE_STATUS_ERROR;
      break;
  }

  return return_value;
}/* end DIS_UpdateChar() */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
