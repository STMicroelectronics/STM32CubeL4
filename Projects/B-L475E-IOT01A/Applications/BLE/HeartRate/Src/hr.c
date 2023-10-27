/**
 ******************************************************************************
 * @file    hr.c
 * @author  MCD Application Team
 * @brief   HeartRate Application
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "hw.h"

#include "ble_lib.h"
#include "blesvc.h"

#include "hr.h"
#include "dis_app.h"
#include "hrs_app.h"

#include "scheduler.h"
#include "debug.h"
#include "tl_types.h"
#include "tl_ble_hci.h"
#include "lpm.h"
#include "tl_ble_reassembly.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  HR_IDLE,
  HR_FAST_ADV,
  HR_LP_ADV,
  HR_LP_CONNECTING,
  HR_CONNECTED_SERVER,
  HR_CONNECTED_CLIENT
} HR_ConnStatus_t;


/**
 * security parameters structure
 */ 
typedef struct _tSecurityParams
{
  /**
  * IO capability of the device
  */ 
  uint8_t ioCapability;
  
  /**
  * Authentication requirement of the device
  * Man In the Middle protection required?
  */ 
  uint8_t mitm_mode;
  
  /**
  * bonding mode of the device
  */ 
  uint8_t bonding_mode;
  
  /**
  * Flag to tell whether OOB data has 
  * to be used during the pairing process
  */ 
  uint8_t OOB_Data_Present; 
  
  /**
  * OOB data to be used in the pairing process if 
  * OOB_Data_Present is set to TRUE
  */ 
  uint8_t OOB_Data[16]; 
  
  /**
  * this variable indicates whether to use a fixed pin 
  * during the pairing process or a passkey has to be 
  * requested to the application during the pairing process
  * 0 implies use fixed pin and 1 implies request for passkey
  */ 
  uint8_t Use_Fixed_Pin; 
  
  /**
  * minimum encryption key size requirement
  */ 
  uint8_t encryptionKeySizeMin;
  
  /**
  * maximum encryption key size requirement
  */ 
  uint8_t encryptionKeySizeMax;
  
  /**
  * fixed pin to be used in the pairing process if
  * Use_Fixed_Pin is set to 1
  */ 
  uint32_t Fixed_Pin;
  
  /**
  * this flag indicates whether the host has to initiate
  * the security, wait for pairing or does not have any security
  * requirements.\n
  * 0x00 : no security required
  * 0x01 : host should initiate security by sending the slave security
  *        request command
  * 0x02 : host need not send the clave security request but it
  * has to wait for paiirng to complete before doing any other
  * processing
  */ 
  uint8_t initiateSecurity;
}tSecurityParams;

/**
 * global context
 * contains the variables common to all 
 * services
 */ 
typedef struct _tBLEProfileGlobalContext
{
  
  /**
   * security requirements of the host
   */ 
  tSecurityParams bleSecurityParam;
  
  /**
   * gap service handle
   */
  uint16_t gapServiceHandle;
  
  /**
   * device name characteristic handle
   */ 
  uint16_t devNameCharHandle;
  
  /**
   * appearance characteristic handle
   */ 
  uint16_t appearanceCharHandle;
  
  /**
   * connection handle of the current active connection
   * When not in connection, the handle is set to 0xFFFF
   */ 
    uint16_t connectionHandle[CFG_MAX_CONNECTION];

  /**
   * length of the UUID list to be used while advertising
   */ 
    uint8_t advtServUUIDlen;
  
  /**
   * the UUID list to be used while advertising
   */ 
    uint8_t advtServUUID[100];

}BleGlobalContext_t;

typedef struct
{
  BleGlobalContext_t BleApplicationContext_legacy;
  HR_ConnStatus_t Device_Connection_Status[CFG_MAX_CONNECTION];
  uint8_t Connection_mgr_timer_Id;
} BleApplicationContext_t;


/* Private defines -----------------------------------------------------------*/
#define FAST_ADV_TIMEOUT            (30*1000*1000/CFG_TS_TICK_VAL) /**< 30s */

#define POOL_SIZE (CFG_TLBLE_EVT_QUEUE_LENGTH * ( sizeof(TL_PacketHeader_t) + TL_BLE_EVENT_FRAME_SIZE ))


/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
 * START of Section BLE_APP_CONTEXT
 */
static BleApplicationContext_t BleApplicationContext;
static uint16_t AdvIntervalMin, AdvIntervalMax;

/**
 * END of Section BLE_APP_CONTEXT
 */

/*******************************************************************************
 * START of Section BLE_STDBY_MEM
 */
static TL_CmdPacket_t CmdBuffer;


/*******************************************************************************
 * END of Section BLE_STDBY_MEM
 */


/*******************************************************************************
 * START of Section BLE_SHARED_NORET_MEM
 */
static uint8_t EvtPool[POOL_SIZE];

/*******************************************************************************
 * END of Section BLE_SHARED_NORET_MEM
 */

uint8_t Test_Status =0x00;

static const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'H','R','_','L','4','7','5','_','I','o','T'};

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Add_Advertisment_Service_UUID(uint16_t servUUID);
static void Adv_Request(HR_ConnStatus_t New_Status);
static void ConnMgr( void );

/* Functions Definition ------------------------------------------------------*/
void HR_Init(HR_InitMode_t InitMode)
{
  uint8_t index;

  if(InitMode != HR_Limited )
  {
    /**
     * Initialization of all transport layer
     */
    TL_BLE_HCI_Init(TL_BLE_HCI_InitFull, &CmdBuffer, EvtPool, POOL_SIZE);

    /**
     * Initialization of the BLE Controller
     */
    SVCCTL_Init();

    /**
     * Initialization of the BLE App Context
     */
    for(index = 0; index < CFG_MAX_CONNECTION; index++)
    {
      BleApplicationContext.Device_Connection_Status[index] = HR_IDLE;
      BleApplicationContext.BleApplicationContext_legacy.connectionHandle[index] = 0xFFFF;
    }

    /**
     * Set TX Power to -2dBm.
     * This avoids undesired disconnection due to instability on 32KHz
     * internal oscillator for high transmission power.
     */
    aci_hal_set_tx_power_level(1,4);

    /**
     * Initialize IO capability
     */
    BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.ioCapability = CFG_HR_IO_CAPABILITY;
    aci_gap_set_io_capability(BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.ioCapability);

    /**
     * Initialize authentication
     */
    BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.mitm_mode = CFG_HR_MITM_PROTECTION;
    BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.OOB_Data_Present = 0;
    BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMin = 8;
    BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMax = 16;
    BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Use_Fixed_Pin = 0;
    BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Fixed_Pin = 111111;
    BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode = 1;
    for (index=0; index<16 ;index++)
    {
      BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.OOB_Data[index] = (uint8_t)index;
    }

    aci_gap_set_auth_requirement(BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.mitm_mode,
                                 BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.OOB_Data_Present,
                                 BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.OOB_Data,
                                 BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMin,
                                 BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMax,
                                 BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Use_Fixed_Pin,
                                 BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Fixed_Pin,
                                 BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode);

    /**
     * Initialize whitelist
     */
    if(BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode)
    {
      aci_gap_configure_whitelist();
    }

    /**
     * Initialize DIS Applciation
     */
    DISAPP_Init();
    
    /**
     * Initialize HRS Application
     */
    HRSAPP_Init();
  
    /**
     * Create timer to handle the connection state machine
     */
    HW_TS_Create(CFG_TimProcID_isr, &(BleApplicationContext.Connection_mgr_timer_Id), hw_ts_SingleShot, ConnMgr);

    /**
     * Make device discoverable
     */
    BleApplicationContext.BleApplicationContext_legacy.advtServUUID[0] = AD_TYPE_16_BIT_SERV_UUID;
    BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen = 1;

    Add_Advertisment_Service_UUID(HEART_RATE_SERVICE_UUID);

    /* Initialize intervals for reconnexion without intervals update */
    AdvIntervalMin = CFG_HR_FAST_CONN_ADV_INTERVAL_MIN;
    AdvIntervalMax = CFG_HR_FAST_CONN_ADV_INTERVAL_MAX;
    
    Adv_Request(HR_FAST_ADV);
  }
  else
  {
    /**
     * Initialization of all transport layer
     */
    TL_BLE_HCI_Init(TL_BLE_HCI_InitLimited, &CmdBuffer, EvtPool, POOL_SIZE);
  }

  return;
}

void HR_AdvUpdate ( void )
{
  Adv_Request(HR_LP_ADV);

  return;
}

void Adv_Request(HR_ConnStatus_t New_Status)
{
  uint8_t index;
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  uint16_t Min_Inter, Max_Inter;
  
  index = 0;        
  if(New_Status == HR_FAST_ADV)
  {
    Min_Inter = AdvIntervalMin;
    Max_Inter = AdvIntervalMax;
    
    /* Find first index of connection in ADVERTISE or IDLE mode */
    while((index < CFG_MAX_CONNECTION) &&
          (BleApplicationContext.Device_Connection_Status[index] != HR_IDLE))
    {
      index++;
    }
  }
  else
  {
    Min_Inter = CFG_HR_LP_CONN_ADV_INTERVAL_MIN;
    Max_Inter = CFG_HR_LP_CONN_ADV_INTERVAL_MAX;
    
    /* Find first index of connection in Fast ADVERTISE mode */
    while((index < CFG_MAX_CONNECTION) &&
          (BleApplicationContext.Device_Connection_Status[index] != HR_FAST_ADV))
    {
      index++;
    }
  }
  
  if(index < CFG_MAX_CONNECTION)
  {
    /**
     * Stop the timer, it will be restarted for a new shot
     * It does not hurt if the timer was not running
     */
    HW_TS_Stop(BleApplicationContext.Connection_mgr_timer_Id);

    APPL_MESG_DBG("Adv_Request(), first index in %d state: %d\n",
                  BleApplicationContext.Device_Connection_Status[index],
                  index);

    if((New_Status == HR_LP_ADV) &&
       ((BleApplicationContext.Device_Connection_Status[index] == HR_FAST_ADV) ||
        (BleApplicationContext.Device_Connection_Status[index] == HR_LP_ADV)))
    {
      /* Connection in ADVERTISE mode have to stop the current advertising */
      ret = aci_gap_set_non_discoverable();
      if( ret == BLE_STATUS_SUCCESS )
      {
        APPL_MESG_DBG("Adv_Request(), Successfully Stopped Advertising at index: %d\n", index);
      }
      else
      {
        APPL_MESG_DBG("Adv_Request(), Stop Advertising Failed at index: %d, result: %d \n", index, ret);
      }
    }

    BleApplicationContext.Device_Connection_Status[index] = New_Status;
    /* Start Fast or Low Power Advertising */
    ret = aci_gap_set_discoverable(ADV_IND,
                                   Min_Inter,
                                   Max_Inter,
                                   PUBLIC_ADDR,
                                   NO_WHITE_LIST_USE, /* use white list */
                                   sizeof(local_name),
                                   local_name,
                                   BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen,
                                   BleApplicationContext.BleApplicationContext_legacy.advtServUUID,
                                   0,
                                   0);
    if( ret == BLE_STATUS_SUCCESS )
    {
      if(New_Status == HR_FAST_ADV)
      {
        APPL_MESG_DBG("Adv_Request(), Successfully Start Fast Advertising at index: %d\n", index);
      }
      else
      {
        APPL_MESG_DBG("Adv_Request(), Successfully Start Low Power Advertising at index: %d\n", index);
      }
    }
    else
    {
      if(New_Status == HR_FAST_ADV)
      {
        APPL_MESG_DBG("Adv_Request(), Start Fast Advertising Failed at index: %d, result: %d \n", index, ret);
      }
      else
      {
        APPL_MESG_DBG("Adv_Request(), Start Low Power Advertising Failed at index: %d, result: %d \n", index, ret);
      }
    }
    if(New_Status == HR_FAST_ADV)
    {
      /* Fast advertising during FAST_ADV_TIMEOUT */
      HW_TS_Start(BleApplicationContext.Connection_mgr_timer_Id, FAST_ADV_TIMEOUT);
    }
  }
  else
    APPL_MESG_DBG("Adv_Request(), no index in HR_IDLE state !\n");

  return;
}

void SVCCTL_App_Notification(void *pckt)
{
  hci_event_pckt *event_pckt;
  evt_le_meta_event *meta_evt;
  uint8_t index;

  event_pckt = (hci_event_pckt*)((hci_uart_pckt *)pckt)->data;

  switch(event_pckt->evt)
  {
    case EVT_DISCONN_COMPLETE:
      {
        evt_disconn_complete * disconnection_complete_event;
        
        disconnection_complete_event = (evt_disconn_complete *)event_pckt->data;
          
        APPL_MESG_DBG("SVCCTL_App_Notification: EVT_DISCONN_COMPLETE for connection handle 0x%x\n", 
                      disconnection_complete_event->handle);
        /* Find index of the handle deconnected */
        index = 0;        
        while((index < CFG_MAX_CONNECTION) &&
              (BleApplicationContext.BleApplicationContext_legacy.connectionHandle[index] != disconnection_complete_event->handle))
        {
          index++;
        }
        
        if(index < CFG_MAX_CONNECTION)
        {
          APPL_MESG_DBG("SVCCTL_App_Notification: index of the handle deconnected: %d\n", index);
          BleApplicationContext.Device_Connection_Status[index] = HR_IDLE;
          BleApplicationContext.BleApplicationContext_legacy.connectionHandle[index] = 
            0xFFFF;
        }
        else
          APPL_MESG_DBG("SVCCTL_App_Notification: no index found for the handle discconnected !\n");

        /* restart advertising */
        Adv_Request(HR_FAST_ADV);
      }
      break; /* EVT_DISCONN_COMPLETE */

    case EVT_LE_META_EVENT:
      {
        meta_evt = (evt_le_meta_event*)event_pckt->data;

        switch(meta_evt->subevent)
        {
          case EVT_LE_CONN_COMPLETE:
            {
              evt_le_connection_complete * connection_complete_event;
              
              /**
               * The connection is done, there is no need anymore to schedule the LP ADV
               */
              connection_complete_event = (evt_le_connection_complete *)meta_evt->data;
              HW_TS_Stop(BleApplicationContext.Connection_mgr_timer_Id);

              APPL_MESG_DBG("SVCCTL_App_Notification: EVT_LE_CONN_COMPLETE for connection handle 0x%x\n",
                            connection_complete_event->handle);

              /* Find index of a connection not in HR_IDLE, HR_CONNECTED_SERVER or HR_CONNECTED_CLIENT state */
              index = 0;
              while((index < CFG_MAX_CONNECTION) &&
                    ((BleApplicationContext.Device_Connection_Status[index] == HR_IDLE) ||
                     (BleApplicationContext.Device_Connection_Status[index] == HR_CONNECTED_SERVER) ||
                     (BleApplicationContext.Device_Connection_Status[index] == HR_CONNECTED_CLIENT)))
              {
                index++;
              }
        
              if(index < CFG_MAX_CONNECTION)
              {
                APPL_MESG_DBG("SVCCTL_App_Notification(), first index in state %d: %d\n", 
                              BleApplicationContext.Device_Connection_Status[index],
                              index);
                if(BleApplicationContext.Device_Connection_Status[index] == HR_LP_CONNECTING)
                  /* Connection as client */
                  BleApplicationContext.Device_Connection_Status[index] = HR_CONNECTED_CLIENT;
                else
                  /* Connection as server */
                  BleApplicationContext.Device_Connection_Status[index] = HR_CONNECTED_SERVER;
                BleApplicationContext.BleApplicationContext_legacy.connectionHandle[index] = connection_complete_event->handle;
              }
              else
                APPL_MESG_DBG("SVCCTL_App_Notification(), no stored connection in state different than HR_IDLE, HR_CONNECTED_CLIENT and HR_CONNECTED_SERVER!\n");             
            }
            break; /* HCI_EVT_LE_CONN_COMPLETE */

          default:
            break;
        }
      }
      break; /* HCI_EVT_LE_META_EVENT */

    default:
      break;
  }

  return;
}

static void Add_Advertisment_Service_UUID(uint16_t servUUID)
{
  BleApplicationContext.BleApplicationContext_legacy.advtServUUID[BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen] = (uint8_t)(servUUID & 0xFF);
  BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen++;
  BleApplicationContext.BleApplicationContext_legacy.advtServUUID[BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen] = (uint8_t)(servUUID >> 8) & 0xFF;
  BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen++;

  return;
}

static void ConnMgr( void )
{
  /**
   * The code shall be executed in the background as an aci command may be sent
   * The background is the only place where the application can make sure a new aci command
   * is not sent if there is a pending one
   */
  SCH_SetTask(CFG_IdleTask_ConnMgr);

  return;
}

