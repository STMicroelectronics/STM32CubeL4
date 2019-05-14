/**
 ******************************************************************************
 * @file    svc_ctl.c
 * @author  MCD Application Team
 * @brief   BLE Controller
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
#include <stdint.h>

#include "config.h"   /* provided by the applic. see config/ble_config_templete.h */
#include "debug.h"    /* provided by the applic. see config/ble_debug_templete.h */

#include "svc_private.h"
#include "ble_lib.h"
#include "blesvc.h"


/* Private typedef -----------------------------------------------------------*/
typedef struct{
#if (BLE_CFG_SVC_MAX_NBR_CB > 0)
  SVC_CTL_p_EvtHandler_t SVCCTL__SvcHandlerTab[BLE_CFG_SVC_MAX_NBR_CB];
#endif
  uint8_t  NbreOfRegisteredHandler;
}SVCCTL_EvtHandler_t;

typedef struct{
#if (BLE_CFG_CLT_MAX_NBR_CB > 0)
  SVC_CTL_p_EvtHandler_t SVCCTL_CltHandlerTable[BLE_CFG_CLT_MAX_NBR_CB];
#endif
  uint8_t  NbreOfRegisteredHandler;
}SVCCTL_CltHandler_t;


/* Private defines -----------------------------------------------------------*/
#define SVCCTL_EGID_EVT_MASK   0xFF00
#define SVCCTL_GATT_EVT_TYPE   0x0C00
#define SVCCTL_GAP_DEVICE_NAME_LENGTH 7

#define BD_ADDR_SIZE_LOCAL        6

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const uint8_t M_bd_addr[BD_ADDR_SIZE_LOCAL] =
{
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0xFF0000000000) >> 40),
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0x00FF00000000) >> 32),
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0x0000FF000000) >> 24),
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0x000000FF0000) >> 16),
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0x00000000FF00) >> 8),
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0x0000000000FF))
};

/**
 * START of Section BLE_BLUENRG_DRIVER_CONTEXT
 */
SVCCTL_EvtHandler_t SVCCTL_EvtHandler;
SVCCTL_CltHandler_t  SVCCTL_CltHandler;
/**
 * END of Section BLE_BLUENRG_DRIVER_CONTEXT
 */


/* Fake __Weak functions ------------------------------------------------------*/
/* This is a dirty trick to avoid putting compilation flags to say which service are included */	
/* If the related service is added in the project space its Init function will be used */
/* Otherwise the dummy __Weak function allow to avoid the compilatio error */

__weak void DIS_Init(uint16_t *p_options) {}
__weak void HRS_Init(void) {}
__weak void BAS_Init(uint8_t * options, uint8_t battery_index) {}
__weak void ANS_Init(void) {}
__weak void HIDS_Init(void) {}
__weak void HPS_Init(void) {}
__weak void IPSS_Init(void) {}
__weak void LNS_Init(void) {}
__weak void SCPS_Init(void) {}
__weak void BLESVC_InitCustomSvc( void ) {}

/* Functions Definition ------------------------------------------------------*/

void SVCCTL_Init( void )
{
  uint16_t gap_service_handle, gap_dev_name_char_handle, gap_appearance_char_handle;
  uint8_t role;
  const uint8_t mode = BLE_CFG_DATA_ROLE_MODE;
  const uint8_t *bd_addr;

  /**
   * Initialize HCI layer
   */
  /**
   * Initialize the number of registered Handler
   */
  SVCCTL_EvtHandler.NbreOfRegisteredHandler = 0;
  SVCCTL_CltHandler.NbreOfRegisteredHandler = 0;

  /**
   * Initialize HCI layer
   */

  /**
   * Write the BD Address
   */
  bd_addr = SVCCTL_GetBdAddress();
  aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
                            CONFIG_DATA_PUBADDR_LEN,
                            bd_addr);

  /**
   * Write the BD Address
   */
  aci_hal_write_config_data(CONFIG_DATA_MODE_OFFSET,
                            CONFIG_DATA_MODE_LEN,
                            &mode);

  /**
   * Initialize GATT interface
   */
  aci_gatt_init();

  /**
   * Initialize GAP interface
   */
  role = 0;

#if (BLE_CFG_PERIPHERAL == 1)
  role |= GAP_PERIPHERAL_ROLE;
#endif

#if (BLE_CFG_CENTRAL == 1)
  role |= GAP_CENTRAL_ROLE;
#endif

  if(role > 0)
  {
    const char *name = "BlueNRG";

    aci_gap_init(role,
                 0,
                 SVCCTL_GAP_DEVICE_NAME_LENGTH,
                 &gap_service_handle,
                 &gap_dev_name_char_handle,
                 &gap_appearance_char_handle);

    if(aci_gatt_update_char_value(gap_service_handle,
                                  gap_dev_name_char_handle,
                                  0,
                                  strlen(name),
                                  (uint8_t *)name))
    {
      PRINT_MESG_DBG("Device Name aci_gatt_update_char_value failed.\n");
    }
  }

  /**
   * Add and Initialize requested services
   */
  HRS_Init();
#if (BLE_CFG_MENU_DEVICE_INFORMATION == 1)
  {
    uint16_t options;

    DIS_Menu(&options);
    DIS_Init(&options);
  }
#else
  DIS_Init(NULL);
#endif

#if (BLE_SVC_HEALTH_THERMOMETER != 0)
  BLE_SVC_HealthThermometer_Init();
#endif

#if (BLE_SVC_SCANPARAMETERS != 0)
  BLE_SVC_ScanParameters_Init();
#endif

#if (BLE_SVC_ALERT_NOTIFICATION != 0)
  BLE_SVC_AlertNotification_Init();
#endif  

#if (BLE_CLI_ALERT_NOTIFICATION != 0)
  BLE_SVC_AlertNotification_Client_Init();
#endif

#if (BLE_SVC_WEIGHT_SCALE != 0)
  BLE_SVC_WeightScale_Init();
#endif   

#if (BLE_SVC_CURRENT_TIME != 0)
  BLE_SVC_CurrentTime_Init();
#endif

#if (BLE_SVC_REFERENCE_TIME_UPDATE != 0)
  BLE_SVC_ReferenceTimeUpdate_Init();
#endif

#if (BLE_SVC_BLOOD_PRESSURE != 0)
  BLE_SVC_BloodPressure_Init();
#endif

#if (BLE_SVC_NEXT_DST != 0)
  BLE_SVC_NextDst_Init();
#endif

  SCPS_Init();

  ANS_Init();

#if ( ERIC_MECHIN == 1)
#if (MENU_BATTERY == 1)
  {
    uint8_t * options;
    uint8_t service_instance;

    for(service_instance = 0; service_instance < CFG_BAS_NUMBER ; service_instance++)
    {
      BAS_Menu(options);
      BAS_Init(options, service_instance);
    }
  }
#else
{
  uint8_t service_instance;

  for(service_instance = 0; service_instance < CFG_BAS_NUMBER ; service_instance++)
    BAS_Init(NULL, service_instance);
}
#endif
#endif

#if (BLE_SVC_ENV_SENSING != 0)
#if (MENU_ENV_SENSING == 1)
{
  uint8_t service_instance;
  ESS_Options_t ESS_Options;

  memset ( &ESS_Options, 0, sizeof(ESS_Options_t) );

  for(service_instance = 0; service_instance < BLE_SVC_ENV_SENSING ; service_instance++)
  {
    ESS_Menu(&ESS_Options);
    BLE_SVC_EnvSensing_Init(&ESS_Options);
  }
}
#else
{
  uint8_t service_instance;
  ESS_Options_t ESS_Options;

  memset ( &ESS_Options, 0, sizeof(ESS_Options_t) );
  ESS_Options.ES_char[0] = 2;
  ESS_Options.ES_char[ES_NB_CHAR] = 1;
  ESS_Options.ES_option_setting[0] = 0x7F;
  ESS_Options.ES_option_setting[1] = 0x7F;
  ESS_Options.ES_trigger_option[0] = 0x07;
  ESS_Options.ES_trigger_option[1] = 0x07;
  ESS_Options.ES_Nb_trigger[0] = 0x03;
  ESS_Options.ES_Nb_trigger[1] = 0x03;
  for(service_instance = 0; service_instance < BLE_SVC_ENV_SENSING ; service_instance++)
    BLE_SVC_EnvSensing_Init(&ESS_Options);
}
#endif
#endif

#if (BLE_SVC_GLUCOSE != 0)
#if (MENU_GLUCOSE == 1)
{
  uint8_t service_instance;
  GS_Options_t GS_Options;

  memset ( &GS_Options, 0, sizeof(GS_Options_t) );
  for(service_instance = 0; service_instance < BLE_SVC_GLUCOSE ; service_instance++)
  {
    GS_Menu(&GS_Options);
    BLE_SVC_Glucose_Init(&GS_Options);
  }
}
#else
{
  uint8_t service_instance;
  GS_Options_t GS_Options;

  memset ( &GS_Options, 0, sizeof(GS_Options_t) );
  GS_Options.GS_measure = 0x1F;
  GS_Options.GS_measure_context = 0xFF;

  for(service_instance = 0; service_instance < BLE_SVC_GLUCOSE ; service_instance++)
    BLE_SVC_Glucose_Init(&GS_Options);
}
#endif
#endif

#if (BLE_SVC_PHONE_ALERT_STATUS != 0)
BLE_SVC_PhoneAlertStatus_Init();
#endif   

#if (BLE_CLI_PHONE_ALERT_STATUS != 0)
BLE_SVC_PhoneAlertStatus_Client_Init();
#endif

#if (BLE_SVC_DATA_TRANSFER != 0)
BLE_SVC_DataTransfer_Init();
#endif   

#if (BLE_CLI_DATA_TRANSFER != 0)
BLE_SVC_DataTransfer_Client_Init();
#endif   

#if (BLE_SVC_HID != 0)
BLE_SVC_Hid_Init();
#endif

#if (BLE_CLI_HID != 0)
BLE_SVC_Hid_Client_Init();
#endif

#if (BLE_SVC_LOCATION_NAVIGATION != 0)
BLE_SVC_LocNav_Init();
#endif

#if (BLE_SVC_IMMEDIATE_ALERT != 0)
BLE_SVC_ImmediateAlert_Init();
#endif

#if (BLE_CLI_PXP != 0)
BLE_SVC_ImmediateAlert_Client_Init();
BLE_SVC_LinkLoss_Client_Init();
BLE_SVC_TxPower_Client_Init();
#endif

#if (BLE_SVC_LINK_LOSS != 0)
BLE_SVC_LinkLoss_Init();
#endif

#if (BLE_SVC_TX_POWER != 0)
BLE_SVC_TxPower_Init();
#endif

#if (BLE_SVC_IP_SUPPORT != 0)
BLE_SVC_IPSupport_Init();
#endif

#if (BLE_SVC_HTTP_PROXY != 0)
BLE_SVC_Http_Init();
#endif

HIDS_Init();

LNS_Init();

IPSS_Init();

HPS_Init();

BLESVC_InitCustomSvc();

return;
}

/**
 * @brief  BLE Controller initialization
 * @param  None
 * @retval None
 */
void SVCCTL_RegisterSvcHandler(SVC_CTL_p_EvtHandler_t pfBLE_SVC_Service_Event_Handler)
{
#if (BLE_CFG_SVC_MAX_NBR_CB > 0)
  SVCCTL_EvtHandler.SVCCTL__SvcHandlerTab[SVCCTL_EvtHandler.NbreOfRegisteredHandler] = pfBLE_SVC_Service_Event_Handler;
#endif
  SVCCTL_EvtHandler.NbreOfRegisteredHandler++;

  return;
}

/**
 * @brief  BLE Controller initialization
 * @param  None
 * @retval None
 */
void SVCCTL_RegisterCltHandler(SVC_CTL_p_EvtHandler_t pfBLE_SVC_Client_Event_Handler)
{
#if (BLE_CFG_CLT_MAX_NBR_CB > 0)
  SVCCTL_CltHandler.SVCCTL_CltHandlerTable[SVCCTL_CltHandler.NbreOfRegisteredHandler] = pfBLE_SVC_Client_Event_Handler;
#endif
  SVCCTL_CltHandler.NbreOfRegisteredHandler++;

  return;
}

void SVCCTL_HCI_UserEvtRx(void *pckt)
{
  hci_event_pckt *event_pckt;
  evt_blue_aci *blue_evt;
  SVCCTL_EvtAckStatus_t event_notification_status;
  uint8_t index;

  event_pckt = (hci_event_pckt*)((hci_uart_pckt *)pckt)->data;
  event_notification_status = SVCCTL_EvtNotAck;

  switch(event_pckt->evt)
  {
    case EVT_VENDOR:
    {
      blue_evt = (evt_blue_aci*)event_pckt->data;

      switch ((blue_evt->ecode) & SVCCTL_EGID_EVT_MASK)
      {
        case SVCCTL_GATT_EVT_TYPE:
#if (BLE_CFG_SVC_MAX_NBR_CB > 0)
          /* For Service event handler */
          for(index = 0; index <SVCCTL_EvtHandler.NbreOfRegisteredHandler ; index++)
          {
            event_notification_status = SVCCTL_EvtHandler.SVCCTL__SvcHandlerTab[index](pckt);
            /**
             * When a GATT event has been acknowledged by a Service, there is no need to call the other registered handlers
             * a GATT event is relevant for only one Service
             */
            if (event_notification_status != SVCCTL_EvtNotAck)
            {
              /**
               *  The event has been managed. The Event processing should be stopped
               */
              break;
            }
          }
#endif
#if (BLE_CFG_CLT_MAX_NBR_CB > 0)
          /* For Client event handler */
          event_notification_status = SVCCTL_EvtNotAck;
          for(index = 0; index <SVCCTL_CltHandler.NbreOfRegisteredHandler ; index++)
          {
            event_notification_status = SVCCTL_CltHandler.SVCCTL_CltHandlerTable[index](pckt);
            /**
             * When a GATT event has been acknowledged by a Client, there is no need to call the other registered handlers
             * a GATT event is relevant for only one Client
             */
            if (event_notification_status != SVCCTL_EvtNotAck)
            {
              /**
               *  The event has been managed. The Event processing should be stopped
               */
              break;
            }
          }
#endif
          break;

        default:
          break;
      }
    }
    break; /* HCI_EVT_VENDOR_SPECIFIC */

    default:
      break;
  }

  /**
   * When no registered handlers (either Service or Client) has acknowledged the GATT event, it is reported to the application
   * a GAP event is always reported to the applicaiton.
   */
  if(event_notification_status == SVCCTL_EvtNotAck)
  {
    /**
     *  The event has NOT been managed.
     *  It shall be passed to the application for processing
     */
    SVCCTL_App_Notification(pckt);

  }

  return;
}


/* __WEAK functions: The application can overload this function  if needed ----*/
/* ----------------------------------------------------------------------------*/

/**
  * @note  __WEAK function: The application can overload this function  
           for example in case it needs to retrieve the value from OTP 
           instead of using the harcoded value in the config.h
  */
__weak const uint8_t* SVCCTL_GetBdAddress( void )
{
  return M_bd_addr;
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
