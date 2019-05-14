/**
 ******************************************************************************
 * @file    hrs.c
 * @author  MCD Application Team
 * @brief   Heart Rate Service
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

#include "config.h"   /* provided by the applic. see config/ble_config_templete.h */
#include "debug.h"    /* provided by the applic. see config/ble_debug_templete.h */

#include "svc_private.h"
#include "ble_lib.h"
#include "svc_ctl.h"
#include "hrs.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint16_t  HeartRateSvcHdle;        /**< Service handle */
  uint16_t  HeartRatemeasurementCharHdle;  /**< Characteristic handle */
#if (BLE_CFG_HRS_BODY_SENSOR_LOCATION_CHAR != 0)
  uint16_t  BodySensorLocationCharHdle;    /**< Characteristic handle */
#endif
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
  uint16_t  ControlPointCharHdle;      /**< Characteristic handle */
#endif
}HRS_Context_t;


/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define HRS_CNTL_POINT_RESET_ENERGY_EXPENDED      (0x01)
#define HRS_CNTL_POINT_VALUE_IS_SUPPORTED         (0x00)
#define HRS_CNTL_POINT_VALUE_NOT_SUPPORTED        (0x80)

#define HRS_MAX_NBR_RR_INTERVAL_VALUES            9


/* Private variables ---------------------------------------------------------*/

/**
 * START of Section BLE_BLUENRG_DRIVER_CONTEXT
 */
static HRS_Context_t HRS_Context;
/**
 * END of Section BLE_BLUENRG_DRIVER_CONTEXT
 */


/* Private function prototypes -----------------------------------------------*/
#if (BLE_CFG_HRS_BODY_SENSOR_LOCATION_CHAR != 0)
static tBleStatus Update_Char_BodySensorLocation( HRS_BodySensorLocation_t *pBodySensorLocationValue );
#endif
static tBleStatus Update_Char_Measurement(HRS_MeasVal_t *pMeasurement );
static SVCCTL_EvtAckStatus_t HearRate_Event_Handler(void *pckt);


/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/

/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t HearRate_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blue_aci *blue_evt;
  evt_gatt_attr_modified    * attribute_modified;

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  switch(event_pckt->evt)
  {
    case EVT_VENDOR:
    {
      blue_evt = (evt_blue_aci*)event_pckt->data;
      switch(blue_evt->ecode)
      {
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
        case EVT_BLUE_GATT_WRITE_PERMIT_REQ:
        {
          evt_gatt_write_permit_req * write_perm_req;

          HRPROFILE_MESG_DBG("HeartRate_Event_Handler: EVT_BLUE_GATT_WRITE_PERMIT_REQ\n");
          write_perm_req = (evt_gatt_write_permit_req*)blue_evt->data;

          if(write_perm_req->attr_handle == (HRS_Context.ControlPointCharHdle + 1))
          {
            return_value = SVCCTL_EvtAck;

            if (write_perm_req->data[0] == HRS_CNTL_POINT_RESET_ENERGY_EXPENDED)
            {
              /* received a correct value for HRM control point char */

              aci_gatt_write_response(write_perm_req->conn_handle,
                                      write_perm_req->attr_handle,
                                      0x00, /* write_status = 0 (no error))*/
                                      (uint8_t)HRS_CNTL_POINT_VALUE_IS_SUPPORTED, /* err_code */
                                      write_perm_req->data_length,
                                      (uint8_t *)&write_perm_req->data[0]);

              /**
               * Notify the application to Reset The Energy Expended Value
               */
              HRS_Notification(HRS_RESET_ENERGY_EXPENDED_EVT);
            }
            else
            {
              /* received value of HRM control point char is incorrect */
              aci_gatt_write_response(write_perm_req->conn_handle,
                                      write_perm_req->attr_handle,
                                      0x1, /* write_status = 1 (error))*/
                                      (uint8_t)HRS_CNTL_POINT_VALUE_NOT_SUPPORTED, /* err_code */
                                      write_perm_req->data_length,
                                      (uint8_t *)&write_perm_req->data[0]);
            }
          }
        }
        break;
#endif

        case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED:
        {
          attribute_modified = (evt_gatt_attr_modified*)blue_evt->data;
          if(attribute_modified->attr_handle == (HRS_Context.HeartRatemeasurementCharHdle + 2))
          {
            /**
             * Descriptor handle
             */

            /**
             * Notify the application to start measurement
             */
            if(attribute_modified->att_data[0] & COMSVC_Notification)
            {
              HRPROFILE_MESG_DBG("HeartRate_Event_Handler: EVT_BLUE_GATT_ATTRIBUTE_MODIFIED HRS_NOTIFICATION_ENABLED\n");
              HRS_Notification(HRS_NOTIFICATION_ENABLED);
            }
            else
            {
              HRPROFILE_MESG_DBG("HeartRate_Event_Handler: EVT_BLUE_GATT_ATTRIBUTE_MODIFIED HRS_NOTIFICATION_DISABLED\n");
              HRS_Notification(HRS_NOTIFICATION_DISABLED);
            }
          }
        }
        break;

        default:
          break;
      }
    }
    break; /* HCI_EVT_VENDOR_SPECIFIC */

    default:
      break;
  }

  return(return_value);
}/* end HearRate_Event_Handler */

#if (BLE_CFG_HRS_BODY_SENSOR_LOCATION_CHAR != 0)
/**
 * @brief  Body Sensor Location Characteristic update
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 * @param  pBodySensorLocationValue: The address of the new value to be written
 * @retval None
 */
static tBleStatus Update_Char_BodySensorLocation(HRS_BodySensorLocation_t *pBodySensorLocationValue)
{
  tBleStatus return_value = BLE_STATUS_SUCCESS;

  return_value = aci_gatt_update_char_value(HRS_Context.HeartRateSvcHdle,
                                            HRS_Context.BodySensorLocationCharHdle,
                                            0, /* charValOffset */
                                            1, /* charValueLen */
                                            (const uint8_t *) pBodySensorLocationValue);
  return return_value;
}/* end Update_Char_BodySensorLocation() */
#endif

/**
 * @brief  Heart rate Measurement Characteristic update
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 * @param  pBodySensorLocationValue: The address of the new value to be written
 * @retval None
 */
static tBleStatus Update_Char_Measurement (HRS_MeasVal_t *pMeasurement )
{
  tBleStatus return_value;
  uint8_t ahrm_value[
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
                     2                                     /** Energy Expended Info */
#endif
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG != 0)
                     +(2*BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG)  /**< RR Interval */
#endif
                     +3
                     ];

  uint8_t hrm_char_length;

  /**
   * Flags update
   */
  ahrm_value[0] = (uint8_t)pMeasurement->Flags;
  hrm_char_length = 1;

  /**
   *  Heart Rate Measurement Value
   */
  if ( (pMeasurement->Flags) &  HRS_HRM_VALUE_FORMAT_UINT16 )
  {
    ahrm_value[hrm_char_length] = (uint8_t)(pMeasurement->MeasurementValue & 0xFF);
    hrm_char_length++;
    ahrm_value[hrm_char_length] = (uint8_t)(pMeasurement->MeasurementValue >> 8);
    hrm_char_length++;
  }
  else
  {
    ahrm_value[hrm_char_length] = (uint8_t)pMeasurement->MeasurementValue;
    hrm_char_length++;
  }

#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
  /**
   *  Energy Expended
   */
  if ((pMeasurement->Flags) &  HRS_HRM_ENERGY_EXPENDED_PRESENT)
  {
    ahrm_value[hrm_char_length] = (uint8_t)(pMeasurement->EnergyExpended & 0xFF);
    hrm_char_length++;
    ahrm_value[hrm_char_length] = (uint8_t)(pMeasurement->EnergyExpended >> 8);
    hrm_char_length++;
  }
#endif

#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG != 0)
  /**
   *  RR Interval Values
   */
  if ((pMeasurement->Flags) &  HRS_HRM_RR_INTERVAL_PRESENT)
  {
    uint8_t index;
    uint8_t rr_interval_number;

    if((pMeasurement->Flags) &  HRS_HRM_VALUE_FORMAT_UINT16)
    {
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
      if ((pMeasurement->Flags) &  HRS_HRM_ENERGY_EXPENDED_PRESENT)
      {
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG > (HRS_MAX_NBR_RR_INTERVAL_VALUES-2))
        /**
         * When the HRM is on 16bits and the Energy expended info is present
         * The maximum RR values is 7
         */
        if(pMeasurement->NbreOfValidRRIntervalValues > (HRS_MAX_NBR_RR_INTERVAL_VALUES-2))
        {
          rr_interval_number = (HRS_MAX_NBR_RR_INTERVAL_VALUES-2);
        }
        else
#endif
        {
          rr_interval_number = pMeasurement->NbreOfValidRRIntervalValues;
        }
      }
      else
      {
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
        /**
         * When the HRM is on 16bits and the Energy expended info is not present
         * The maximum RR values is 8
         */
        if(pMeasurement->NbreOfValidRRIntervalValues > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
        {
          rr_interval_number = (HRS_MAX_NBR_RR_INTERVAL_VALUES-1);
        }
        else
#endif
        {
          rr_interval_number = pMeasurement->NbreOfValidRRIntervalValues;
        }
      }
#else /**< (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0) */
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
      /**
       * When the HRM is on 16bits and the Energy expended info is not present
       * The maximum RR values is 8
       */
      if(pMeasurement->NbreOfValidRRIntervalValues > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
      {
        rr_interval_number = (HRS_MAX_NBR_RR_INTERVAL_VALUES-1);
      }
      else
#endif
      {
        rr_interval_number = pMeasurement->NbreOfValidRRIntervalValues;
      }
#endif
    }
    else
    {
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
      if ((pMeasurement->Flags) &  HRS_HRM_ENERGY_EXPENDED_PRESENT)
      {
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
        /**
         * When the HRM is on 8bits and the Energy expended info is present
         * The maximum RR values is 8
         */
        if(pMeasurement->NbreOfValidRRIntervalValues > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
        {
          rr_interval_number = (HRS_MAX_NBR_RR_INTERVAL_VALUES-1);
        }
        else
#endif
        {
          rr_interval_number = pMeasurement->NbreOfValidRRIntervalValues;
        }
      }
      else
      {
        rr_interval_number = pMeasurement->NbreOfValidRRIntervalValues;
      }
#else /**< (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0) */
      rr_interval_number = pMeasurement->NbreOfValidRRIntervalValues;
#endif
    }

    for ( index = 0 ; index < rr_interval_number ; index++ )
    {
      ahrm_value[hrm_char_length] = (uint8_t)(pMeasurement->aRRIntervalValues[index] & 0xFF);
      hrm_char_length++;
      ahrm_value[hrm_char_length] = (uint8_t)(pMeasurement->aRRIntervalValues[index] >> 8);
      hrm_char_length++;
    }
  }

#endif  /**< (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG != 0) */

  return_value = aci_gatt_update_char_value(HRS_Context.HeartRateSvcHdle,
                                            HRS_Context.HeartRatemeasurementCharHdle,
                                            0, /* charValOffset */
                                            hrm_char_length, /* charValueLen */
                                            (const uint8_t *) &ahrm_value[0]);

  return return_value;
}/* end Update_Char_Measurement() */


/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void HRS_Init(void)
{
  uint16_t uuid;
  tBleStatus hciCmdResult = BLE_STATUS_SUCCESS;

  /**
   *  Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(HearRate_Event_Handler);

  /**
   *  Add Heart Rate Service
   *
   * Max_Attribute_Records = 2*no_of_char + 1
   * service_max_attribute_record = 1 for heart rate service +
   *                                2 for hear rate measurement characteristic +
   *                                1 for client char configuration descriptor +
   *                                2 for body sensor location characteristic +
   *                                2 for control point characteristic
   */
  uuid = HEART_RATE_SERVICE_UUID;
  hciCmdResult = aci_gatt_add_serv(UUID_TYPE_16,
                                   (const uint8_t *) &uuid,
                                   PRIMARY_SERVICE,
                                   4
#if (BLE_CFG_HRS_BODY_SENSOR_LOCATION_CHAR != 0)
                                   +2
#endif
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
                                   +2
#endif
                                   +(2*BLE_CFG_HRS_NUMBER_OF_CUSTOM_CHAR),   /**< Custom Characteristic */
                                   &(HRS_Context.HeartRateSvcHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    HRPROFILE_MESG_DBG ("HRS_Init(), Heart Rate Service (HRS) is added Successfully %04X\n",
                        HRS_Context.HeartRateSvcHdle);
  }
  else
  {
    HRPROFILE_MESG_DBG ("HRS_Init(), FAILED to add Heart Rate Service (HRS), Error: %02X !!\n",
                        hciCmdResult);
  }

  /**
   *  Add Heart Rate Measurement Characteristic
   */
  uuid = HEART_RATE_MEASURMENT_UUID;
  hciCmdResult = aci_gatt_add_char(HRS_Context.HeartRateSvcHdle,
                                   UUID_TYPE_16,
                                   (const uint8_t *) &uuid ,
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
                                   2                                      /** Energy Expended Info */
#endif
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG != 0)
                                   +(2*BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG) /**< RR Interval */
#endif
                                   +1                                    /** Flags */
                                   +2,                                   /** Measure */
                                   CHAR_PROP_NOTIFY,
                                   ATTR_PERMISSION_NONE,
                                   GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   1, /* isVariable */
                                   &(HRS_Context.HeartRatemeasurementCharHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    HRPROFILE_MESG_DBG ("HRS_Init(), Heart Rate Measurement Characteristic Added Successfully  %04X \n",
                        HRS_Context.HeartRatemeasurementCharHdle);
  }
  else
  {
    HRPROFILE_MESG_DBG ("HRS_Init(), FAILED to add Heart Rate Measurement Characteristic, Error: %02X !!\n",
                        hciCmdResult);
  }

#if (BLE_CFG_HRS_BODY_SENSOR_LOCATION_CHAR != 0)
  /**
   *  Add Body Sensor Location Characteristic
   */
  uuid = SENSOR_LOCATION_UUID;
  hciCmdResult = aci_gatt_add_char(HRS_Context.HeartRateSvcHdle,
                                   UUID_TYPE_16,
                                   (const uint8_t *) &uuid ,
                                   1, /* bytes */
                                   CHAR_PROP_READ,
                                   ATTR_PERMISSION_NONE,
                                   GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   0, /* isVariable: 0 */
                                   &(HRS_Context.BodySensorLocationCharHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    HRPROFILE_MESG_DBG ("HRS_Init(), Sensor Location Characteristic Added Successfully  %04X \n",
                        HRS_Context.BodySensorLocationCharHdle);
  }
  else
  {
    HRPROFILE_MESG_DBG ("HRS_Init(), FAILED to add Sensor Location Characteristic, Error: %02X !!\n",
                        hciCmdResult);
  }

#endif

#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
  uuid = CONTROL_POINT_UUID;
  hciCmdResult = aci_gatt_add_char(HRS_Context.HeartRateSvcHdle,
                                   UUID_TYPE_16,
                                   (const uint8_t *) &uuid ,
                                   1, /* bytes */
                                   CHAR_PROP_WRITE,
                                   ATTR_PERMISSION_NONE,
                                   GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   0, /* isVariable: 0*/
                                   &(HRS_Context.ControlPointCharHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    HRPROFILE_MESG_DBG ("HRS_Init(), Control Point Characteristic Added Successfully  %04X \n",
                        HRS_Context.ControlPointCharHdle);
  }
  else
  {
    HRPROFILE_MESG_DBG ("HRS_Init(), FAILED to add Control Point Characteristic, Error: %02X !!\n",
                        hciCmdResult);
  }
#endif

#if (BLE_CFG_HRS_NUMBER_OF_CUSTOM_CHAR != 0)
  BLESVC_AddCustomChar((uint16)t)HEART_RATE_SERVICE_UUID, HRS_Context.HeartRateSvcHdle);
#endif

  return;
}

/**
 * @brief  Characteristic update
 * @param  UUID: UUID of the characteristic
 * @retval BodySensorLocationValue: The new value to be written
 */
tBleStatus HRS_UpdateChar(uint16_t UUID, uint8_t *pPayload)
{
  tBleStatus return_value;
  switch(UUID)
  {
#if (BLE_CFG_HRS_BODY_SENSOR_LOCATION_CHAR != 0)
    case SENSOR_LOCATION_UUID:
      return_value = Update_Char_BodySensorLocation((HRS_BodySensorLocation_t*)pPayload);
      break;
#endif
    case HEART_RATE_MEASURMENT_UUID:
      return_value = Update_Char_Measurement((HRS_MeasVal_t*)pPayload);
      break;

    default:
      return_value = BLE_STATUS_ERROR;
      break;
  }

  return return_value;
}/* end HRS_UpdateChar() */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
