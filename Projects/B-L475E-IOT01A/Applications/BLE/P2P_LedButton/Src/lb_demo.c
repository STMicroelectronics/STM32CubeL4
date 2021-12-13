/**
 ******************************************************************************
 * @file    lb_demo.c
 * @author  MCD Application Team
 * @brief   BLE Application
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
#include "debug.h"

#include "ble_lib.h"
#include "blesvc.h"

#include "lb_demo.h"
#include "lb_server_app.h"

#include "scheduler.h"
#include "debug.h"
#include "tl_types.h"
#include "tl_ble_hci.h"
#include "lpm.h"
#include "tl_ble_reassembly.h"
#include "stm32l475e_iot01.h"

/* Private typedef -----------------------------------------------------------*/

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
} tSecurityParams;

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
    uint16_t connectionHandle[BLE_CFG_MAX_CONNECTION];

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
  BleGlobalContext_t LBRContext_legacy;
  LBR_Gap_Gatt_State Remote_Connection_Status[1];
  uint16_t connectionHandleRemote;
} LBRContext_t;

LBR_ConnHandle_Not_evt_t handleNotification;


#if(LB_CLIENT!=0)
typedef struct{
  /**
   * state of the Data Transfer
   * state machine
   */ 
  LBR_Gap_Gatt_State state;

  /**
   * connection handle 
   */ 
  uint16_t connHandle;

   /**
   * handle of the Led Button service
   */ 
  uint16_t LedButtonServiceHandle;

  /**
   * end handle of the Data Transfer service
   */ 
  uint16_t LedButtonServiceEndHandle;

  /**
   * handle of the Tx characteristic 
   * 
   */ 
  uint16_t LedButtonLedCharHdle;

  /**
   * handle of the client configuration
   * descriptor of Tx characteristic
   */ 
  uint16_t LedButtonLedDescHandle;

  /**
   * handle of the client configuration
   * descriptor of new alert characteristic
   */ 

  uint16_t  LedButtonClientCharHdle;                  /**< Characteristic handle */
  uint16_t  LedButtonClientDescHandle;                  /**< Characteristic handle */

}LBR_ClientContext_t;
#endif


#if (LB_SERVER == 1)  

static const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'L','B','_','P','2','P','_','S','E','R','V','E','R'};

#endif
 
#if (LB_CLIENT == 1)

/* static  char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'L','B','_','P','2','P','_','C','L','I','E','N','T'}; */
tBDAddr LB_SERVER_BDADDR = 
{
 (uint8_t)((REMOTE_BD_ADDRESS & 0xFF0000000000) >> 40),
 (uint8_t)((REMOTE_BD_ADDRESS & 0x00FF00000000) >> 32),
 (uint8_t)((REMOTE_BD_ADDRESS & 0x0000FF000000) >> 24),
 (uint8_t)((REMOTE_BD_ADDRESS & 0x000000FF0000) >> 16),
 (uint8_t)((REMOTE_BD_ADDRESS & 0x00000000FF00) >> 8),
 (uint8_t)((REMOTE_BD_ADDRESS & 0x0000000000FF))
};


#endif //LB_CLIENT


/* Private defines -----------------------------------------------------------*/

#define UNPACK_2_BYTE_PARAMETER(ptr)  \
        (uint16_t)((uint16_t)(*((uint8_t *)ptr))) |   \
        (uint16_t)((((uint16_t)(*((uint8_t *)ptr + 1))) << 8))

#define POOL_SIZE (CFG_TLBLE_EVT_QUEUE_LENGTH * ( sizeof(TL_PacketHeader_t) + TL_BLE_EVENT_FRAME_SIZE ))


/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
 * START of Section BLE_APP_CONTEXT
 */
static LBRContext_t LBRContext;

#if(LB_CLIENT!=0)
static LBR_ClientContext_t aLBRClientContext[BLE_CFG_CLT_MAX_NBR_CB];
#endif

/**
 * END of Section BLE_APP_CONTEXT
 */
uint16_t connection_handle;

/* Global variables ----------------------------------------------------------*/
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

/* Private function prototypes -----------------------------------------------*/
//static void Add_Advertisment_Service_UUID(uint16_t servUUID);
#if(LB_CLIENT!=0)
static SVCCTL_EvtAckStatus_t LBR_Client_Event_Handler(void *pckt);
#endif
/* Functions Definition ------------------------------------------------------*/
__weak void TL_Enable( void );

/* Private functions ----------------------------------------------------------*/


/* Public functions ----------------------------------------------------------*/

void LBR_Init(LBR_InitMode_t InitMode)
{
  uint8_t index;

  if(InitMode != LBR_Limited )
  {
    /**
     * Initialization of all transport layer
     */
    TL_BLE_HCI_Init(TL_BLE_HCI_InitFull, &CmdBuffer, EvtPool, POOL_SIZE);
    TL_Enable();

    /**
     * Initialization of the BLE Controller
     */
     SVCCTL_Init();

    /**
     * Initialization of the BLE App Context
     */
   
    LBRContext.Remote_Connection_Status[0] = LBC_IDLE;
    
   

   
    
    /**
     * Set TX Power to -2dBm.
     * This avoids undesired disconnection due to instability on 32KHz
     * internal oscillator for high transmission power.
     */
    aci_hal_set_tx_power_level(1,0x18);

    /**
     * Initialize IO capability
     */
    LBRContext.LBRContext_legacy.bleSecurityParam.ioCapability = CFG_LBR_IO_CAPABILITY;
    aci_gap_set_io_capability(LBRContext.LBRContext_legacy.bleSecurityParam.ioCapability);

    /**
     * Initialize authentication
     */
    LBRContext.LBRContext_legacy.bleSecurityParam.mitm_mode = CFG_LBR_MITM_PROTECTION;
    LBRContext.LBRContext_legacy.bleSecurityParam.OOB_Data_Present = 0;
    LBRContext.LBRContext_legacy.bleSecurityParam.encryptionKeySizeMin = 8;
    LBRContext.LBRContext_legacy.bleSecurityParam.encryptionKeySizeMax = 16;
    LBRContext.LBRContext_legacy.bleSecurityParam.Use_Fixed_Pin = 0;
    LBRContext.LBRContext_legacy.bleSecurityParam.Fixed_Pin = 111111;
    LBRContext.LBRContext_legacy.bleSecurityParam.bonding_mode = 1;
    for (index=0; index<16 ;index++)
    {
      LBRContext.LBRContext_legacy.bleSecurityParam.OOB_Data[index] = (uint8_t)index;
    }

    aci_gap_set_auth_requirement(LBRContext.LBRContext_legacy.bleSecurityParam.mitm_mode,
                                 LBRContext.LBRContext_legacy.bleSecurityParam.OOB_Data_Present,
                                 LBRContext.LBRContext_legacy.bleSecurityParam.OOB_Data,
                                 LBRContext.LBRContext_legacy.bleSecurityParam.encryptionKeySizeMin,
                                 LBRContext.LBRContext_legacy.bleSecurityParam.encryptionKeySizeMax,
                                 LBRContext.LBRContext_legacy.bleSecurityParam.Use_Fixed_Pin,
                                 LBRContext.LBRContext_legacy.bleSecurityParam.Fixed_Pin,
                                 LBRContext.LBRContext_legacy.bleSecurityParam.bonding_mode);

    /**
     * Initialize whitelist
     */
    if(LBRContext.LBRContext_legacy.bleSecurityParam.bonding_mode)
    {
      aci_gap_configure_whitelist();
    }

    
    /**
     * Initialize LBS Applciation
     */
#if(LB_SERVER!=0)     
    LBSAPP_Init();
   // Start Advertise to be connected by Client
    SCH_SetTask(CFG_IdleTask_StartAdv); 
#endif    
#if(LB_CLIENT!=0)    
    LBSAPP_Init();
    LBR_Client_Init();
    SCH_SetTask(CFG_IdleTask_ConnDev1);
#endif
  
  }

  return;
}



/****************************************************************/
/*                                                              */
/*                     DEVICE IS LB CLIENT                     */
/****************************************************************/
#if(LB_CLIENT!=0) 


void LBR_AppConnReq1(void)
{
  tBleStatus result;
   APPL_MESG_DBG("\r\n\r** CREATE CONNECTION TO SERVER **  \r\n\r");
  
  if(LBRContext.Remote_Connection_Status[0] != LBC_CONNECTED)
  {
    result = aci_gap_create_connection(SCAN_P, 
                                       SCAN_L, 
                                       PUBLIC_ADDR, 
                                       LB_SERVER_BDADDR, 
                                       PUBLIC_ADDR, 
                                       CONN_P1, 
                                       CONN_P2, 
                                       0, 
                                       SUPERV_TIMEOUT, 
                                       CONN_L1 , 
                                       CONN_L2);    
    
    if( result == BLE_STATUS_SUCCESS ) {
      LBRContext.Remote_Connection_Status[0]=LBC_CONNECTING;
     
    }
    else 
    {
      BSP_LED_On(LED2);
      LBRContext.Remote_Connection_Status[0]=LBC_IDLE;
    }
  }
  
  return;
}


void SVCCTL_App_Notification(void *pckt)
{
  hci_event_pckt *event_pckt;
  evt_le_meta_event *meta_evt;
  evt_le_connection_complete * connection_complete_event;
  event_pckt = (hci_event_pckt*)((hci_uart_pckt *)pckt)->data;
  
  uint8_t result;
  uint8_t role;
  evt_disconn_complete *cc = (void *)event_pckt->data;
  
  switch(event_pckt->evt)
  {
      
    case EVT_DISCONN_COMPLETE:
      
      
      if(cc->handle == LBRContext.connectionHandleRemote){
        APPL_MESG_DBG("\r\n\r** DISCONNECTION EVENT OF SERVER \n");
        LBRContext.Remote_Connection_Status[0] = LBC_IDLE;
        LBRContext.connectionHandleRemote = 0xFFFF;
        handleNotification.LBR_Evt_Opcode=LB_SERVER_DISCON_EVT;
        handleNotification.ConnectionHandle=connection_handle;
        LBR_Notification(&handleNotification);
      }
 
      break; /* EVT_DISCONN_COMPLETE */

        
    case EVT_LE_META_EVENT:
    
      meta_evt = (evt_le_meta_event*)event_pckt->data;

      switch(meta_evt->subevent)
      {
           
        case EVT_LE_CONN_COMPLETE:
          /**
           * The connection is done, there is no need anymore to schedule the LP ADV
           */
          connection_complete_event = (evt_le_connection_complete *)meta_evt->data;
         
          connection_handle = connection_complete_event->handle;
          role = connection_complete_event->role;
          if(role == 0x00){ //ROLE MASTER
            
            uint8_t dev1=1;
            
            for (int i=0 ; i <6 ; i++) {
              dev1 &= (LB_SERVER_BDADDR[i] == connection_complete_event->peer_bdaddr[i]);
              
            }
             
            if(dev1==1) {
             // Inform Application it is End Device 1
              APPL_MESG_DBG("-- CONNECTION SUCCESS WITH SERVER\n");
              LBRContext.Remote_Connection_Status[0]=LBC_CONNECTED;
              LBRContext.connectionHandleRemote = connection_handle;
              LBRContext.LBRContext_legacy.connectionHandle[0] = connection_handle;
              handleNotification.LBR_Evt_Opcode=LB_SERVER_CONN_HANDLE_EVT;
              handleNotification.ConnectionHandle=connection_handle;
              LBR_Notification(&handleNotification);
              
              result = aci_gatt_disc_all_prim_services(connection_handle);
              
              if( result == BLE_STATUS_SUCCESS )     {
                APPL_MESG_DBG("\r\n\r** GATT SERVICES & CHARACTERISTICS DISCOVERY  \n");
                APPL_MESG_DBG("* GATT :  Start Searching Primary Services \r\n\r");
              }            else             {
                APPL_MESG_DBG("BLE_CTRL_App_Notification(), All services discovery Failed \r\n\r");
              }
                            
            }

          } 
      
                    
         break; /* HCI_EVT_LE_CONN_COMPLETE */
         
       
            
      } //end switch meta_event ...
    
    break; /* HCI_EVT_LE_META_EVENT */
  
  }

  return;
}

/****************************************************************/
/*                                                              */
/*                      CLIENT                           */
/****************************************************************/
/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t LBR_Client_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blue_aci *blue_evt;
  LBR_Client_App_Notification_evt_t Notification;

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  switch(event_pckt->evt)
  {
    case EVT_VENDOR:
    {
      blue_evt = (evt_blue_aci*)event_pckt->data;
      switch(blue_evt->ecode)
      {
        case EVT_BLUE_ATT_READ_BY_GROUP_TYPE_RESP:
        {
          evt_att_read_by_group_resp *pr = (void*)blue_evt->data;
          /*APPL_MESG_DBG("EVT_BLUE_ATT_READ_BY_GROUP_TYPE_RESP - connection handle 0x%x !\n", pr->conn_handle);*/
          uint8_t numServ, i, idx;
          uint16_t uuid, handle;

          uint8_t index;
          handle = pr->conn_handle;
          index = 0;
          while((index < BLE_CFG_CLT_MAX_NBR_CB) &&
                    (aLBRClientContext[index].state != LBC_IDLE))
          {
             LBR_Gap_Gatt_State status;
                
             status = LBR_Get_Client_Connection_Status(aLBRClientContext[index].connHandle);
             if((aLBRClientContext[index].state == LBC_CONNECTED)&&
                 (status == LBC_IDLE))
                {
                  /* Handle deconnected */

                  aLBRClientContext[index].state = LBC_IDLE;
                  aLBRClientContext[index].connHandle = 0xFFFF;
                  break;
                }
            index++;
          }
          
          if(index < BLE_CFG_CLT_MAX_NBR_CB)
          {
            aLBRClientContext[index].connHandle= handle;

            numServ = (pr->event_data_length - 1) / pr->attribute_data_length;

            /* the event data will be
             * 2bytes start handle
             * 2bytes end handle
             * 2 or 16 bytes data
             * we are intersted only if the UUID is 16 bit.
             * So check if the data length is 6
             */
            if (pr->attribute_data_length == 6)
            {
              idx = 4;
              for (i=0; i<numServ; i++)
              {
                uuid = UNPACK_2_BYTE_PARAMETER(&pr->attribute_data_list[idx]);
                if(uuid == LED_BUTTON_SERVICE_UUID)
                {
                  APPL_MESG_DBG("-- GATT : LED_BUTTON_SERVICE_UUID FOUND - connection handle 0x%x \n", aLBRClientContext[index].connHandle);
                  aLBRClientContext[index].LedButtonServiceHandle = UNPACK_2_BYTE_PARAMETER(&pr->attribute_data_list[idx-4]);
                  aLBRClientContext[index].LedButtonServiceEndHandle = UNPACK_2_BYTE_PARAMETER (&pr->attribute_data_list[idx-2]);
                  aLBRClientContext[index].state = LBC_DISCOVER_CHARACS ;
                }
                idx += 6;
              }
            }
          }
        }
        break;

        
        case EVT_BLUE_ATT_READ_BY_TYPE_RESP:
        {
          
          evt_att_read_by_type_resp *pr = (void*)blue_evt->data;
          /* APPL_MESG_DBG("EVT_BLUE_ATT_READ_BY_TYPE_RESP - connection handle 0x%x !\n", pr->conn_handle); */
          uint8_t idx;
          uint16_t uuid, handle;

          /* the event data will be
           * 2 bytes start handle
           * 1 byte char properties
           * 2 bytes handle
           * 2 or 16 bytes data
           */
          
          uint8_t index;
              
          index = 0;
          while((index < BLE_CFG_CLT_MAX_NBR_CB) &&
                (aLBRClientContext[index].connHandle != pr->conn_handle))
          {
            index++;
          }

          if(index < BLE_CFG_CLT_MAX_NBR_CB)
          {
            /* we are interested in only 16 bit UUIDs */
            idx = 5;
            if (pr->handle_value_pair_length == 7)  
            {
              pr->event_data_length -= 1;
              while(pr->event_data_length > 0)
              {
                uuid = UNPACK_2_BYTE_PARAMETER(&pr->handle_value_pair[idx]);
                /* store the characteristic handle not the attribute handle */
                handle = UNPACK_2_BYTE_PARAMETER(&pr->handle_value_pair[idx-2]);
            
                if(uuid == LED_CHAR_UUID)
                {
                  APPL_MESG_DBG("-- GATT : LED_CHAR_UUID FOUND - connection handle 0x%x\n", aLBRClientContext[index].connHandle);
                  aLBRClientContext[index].state = LBC_DISCOVER_LED_CHAR_DESC;
                  aLBRClientContext[index].LedButtonLedCharHdle = handle;
                }
                else if(uuid == BUTTON_CHAR_UUID)
                {
                  APPL_MESG_DBG("-- GATT : BUTTON_CHAR_UUID FOUND  - connection handle 0x%x\n", aLBRClientContext[index].connHandle);
                  aLBRClientContext[index].state = LBC_DISCOVER_BUTTON_CHAR_DESC;
                  aLBRClientContext[index].LedButtonClientCharHdle = handle;
                }
                pr->event_data_length -= 7;//7;
                idx += 7;//7;
              }
            }
          }
        }
        break;
        
        
        case EVT_BLUE_ATT_FIND_INFORMATION_RESP:
        {
          
          evt_att_find_information_resp *pr = (void*)blue_evt->data;
          
          uint8_t numDesc, idx, i;
          uint8_t index;
          uint16_t uuid, handle;

          /*
           * event data will be of the format
           * 2 bytes handle
           * 2 bytes UUID
           */

          index = 0;
          while((index < BLE_CFG_CLT_MAX_NBR_CB) &&
                (aLBRClientContext[index].connHandle != pr->conn_handle))
          {      
            index++;
          }  

          if(index < BLE_CFG_CLT_MAX_NBR_CB)
          {
            numDesc = (pr->event_data_length) / 4;
            /* we are interested only in 16 bit UUIDs */
            idx = 0;
            if (pr->format == UUID_TYPE_16)
            {
              for (i=0; i<numDesc; i++)
              {
                handle = UNPACK_2_BYTE_PARAMETER(&pr->handle_uuid_pair[idx]);
                uuid = UNPACK_2_BYTE_PARAMETER(&pr->handle_uuid_pair[idx+2]);

                if(uuid == CLIENT_CHAR_CONFIG_DESCRIPTOR_UUID)
                {
                  APPL_MESG_DBG("-- GATT : CLIENT_CHAR_CONFIG_DESCRIPTOR_UUID- connection handle 0x%x\n", aLBRClientContext[index].connHandle);
                  if( aLBRClientContext[index].state == LBC_DISCOVER_BUTTON_CHAR_DESC)
                  {
                    aLBRClientContext[index].LedButtonClientDescHandle = handle;
                    aLBRClientContext[index].state = LBC_ENABLE_NOTIFICATION_BUTTON_DESC;
                  }
                }
                idx += 4;
              }
            }  
          }  
        }
        break; /*EVT_BLUE_ATT_FIND_INFORMATION_RESP*/

        
        case EVT_BLUE_GATT_NOTIFICATION:
        {
          evt_gatt_attr_notification *pr = (void*)blue_evt->data;
          uint8_t index;
              
          index = 0;
          while((index < BLE_CFG_CLT_MAX_NBR_CB) &&
                (aLBRClientContext[index].connHandle != pr->conn_handle))
          {
            index++;
          }

          if(index < BLE_CFG_CLT_MAX_NBR_CB)
          {
          
            if ( (pr->attr_handle == aLBRClientContext[index].LedButtonClientCharHdle) &&
                 (pr->event_data_length > (2)) )
            {
              APPL_MESG_DBG("-- GATT : BUTTON CHARACTERISTICS RECEIVED_EVT - connection handle 0x%x\n", aLBRClientContext[index].connHandle);
              Notification.LBR_Client_Evt_Opcode = LBC_BUTTON_INFO_RECEIVED_EVT;
              Notification.DataTransfered.Length = pr->event_data_length;
              Notification.DataTransfered.pPayload = &pr->attr_value[0];

              LBR_Client_App_Notification(&Notification); 
            
              /* INFORM APPLICATION BUTTON IS PUSHED BY END DEVICE */
            }
          }
        }
        break;/* end EVT_BLUE_GATT_NOTIFICATION */
        
        case EVT_BLUE_GATT_PROCEDURE_COMPLETE:
        {
          evt_gatt_procedure_complete *pr = (void*)blue_evt->data;
          APPL_MESG_DBG("-- GATT : EVT_BLUE_GATT_PROCEDURE_COMPLETE \n");
          APPL_MESG_DBG("\n");

          uint8_t index;
              
          index = 0;
          while((index < BLE_CFG_CLT_MAX_NBR_CB) &&
                (aLBRClientContext[index].connHandle != pr->conn_handle))
          {
            index++;
          }

          if(index < BLE_CFG_CLT_MAX_NBR_CB)
          {
            SCH_SetTask(CFG_IdleTask_SearchService);
          }
        }
        break; /*EVT_BLUE_GATT_PROCEDURE_COMPLETE*/
        default:
          break;
      }
    }

    break; /* HCI_EVT_VENDOR_SPECIFIC */

    default:
      break;
  }

  return(return_value);
}/* end BLE_CTRL_Event_Acknowledged_Status_t */



/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void LBR_Client_Init(void)
{
  uint8_t index =0;
  for(index = 0; index < BLE_CFG_CLT_MAX_NBR_CB; index++)
    aLBRClientContext[index].state= LBC_IDLE;
  /**
   *  Register the event handler to the BLE controller
   */
  SVCCTL_RegisterCltHandler(LBR_Client_Event_Handler);

  APPL_MESG_DBG("-- LED BUTTON CLIENT INITIALIZED \n");
  
  return;
}

void LBR_Client_Update_Service()
{
  uint16_t enable = 0x0001;
  uint8_t index;
  index = 0;
  
  while((index < BLE_CFG_CLT_MAX_NBR_CB) &&
        (aLBRClientContext[index].state != LBC_IDLE))
  {
  
  
    switch(aLBRClientContext[index].state)
    {
      case LBC_DISCOVER_SERVICES:
        APPL_MESG_DBG("LBC_DISCOVER_SERVICES\n");
        break;
      case LBC_DISCOVER_CHARACS:
        APPL_MESG_DBG("* GATT : Discover Led Button  Characteristics\n");
        aci_gatt_disc_all_charac_of_serv(aLBRClientContext[index].connHandle,
                                                aLBRClientContext[index].LedButtonServiceHandle,
                                                aLBRClientContext[index].LedButtonServiceEndHandle);
        break;
      case LBC_DISCOVER_LED_CHAR_DESC: //Not Used - No decriptor
        APPL_MESG_DBG("* GATT : Discover Descriptor of Led Characteritic\n");
        aci_gatt_disc_all_charac_descriptors(aLBRClientContext[index].connHandle,
                                                    aLBRClientContext[index].LedButtonLedCharHdle,
                                                    aLBRClientContext[index].LedButtonLedCharHdle+2);
        break;  
      case LBC_DISCOVER_BUTTON_CHAR_DESC:
        APPL_MESG_DBG("* GATT : Discover Descriptor of Button Characteritic\n");
        aci_gatt_disc_all_charac_descriptors(aLBRClientContext[index].connHandle,
                                                    aLBRClientContext[index].LedButtonClientCharHdle,
                                                    aLBRClientContext[index].LedButtonClientCharHdle+2);
        break;  
      case LBC_ENABLE_NOTIFICATION_BUTTON_DESC:
        APPL_MESG_DBG("* GATT : Enable Button Notification\n");
        aci_gatt_write_charac_descriptor(aLBRClientContext[index].connHandle,
                                                aLBRClientContext[index].LedButtonClientDescHandle,
                                                2,
                                                (uint8_t *)&enable);
      
        aLBRClientContext[index].state = LBC_CONNECTED;
        break;
      case LBC_DISABLE_NOTIFICATION_TX_DESC :
        APPL_MESG_DBG("* GATT : Disable Button Notification\n");
        aci_gatt_write_charac_descriptor(aLBRClientContext[index].connHandle,
                                                aLBRClientContext[index].LedButtonClientDescHandle,
                                                2,
                                                (uint8_t *)&enable);
      
        aLBRClientContext[index].state = LBC_CONNECTED;
        break;
      default:
        break;
    }
    index++;
  }
  return;
}



/**
 * @brief  Feature Characteristic update
 * @param  pFeatureValue: The address of the new value to be written
 * @retval None
 */
tBleStatus LBR_Client_Update_Char(uint16_t UUID, uint8_t Service_Instance, uint8_t *pPayload)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  uint8_t index;
  
  index = 0;
  while((index < BLE_CFG_CLT_MAX_NBR_CB) &&
        (aLBRClientContext[index].state != LBC_IDLE))
  {
    switch(UUID)
    {
      case LED_CHAR_UUID: // SERVER RX -- so CLIENT TX
      ret =aci_gatt_write_without_response(aLBRClientContext[index].connHandle,
                                        aLBRClientContext[index].LedButtonLedCharHdle,
                                        2, /* charValueLen */
                                         (const uint8_t *)  pPayload);
      
      break;
   
      default:
        break;
    }
    index++;
  }

  return ret;
}/* end LBR_Client_Update_Char() */


void LBR_App_Key_Button_Action(void)
{
  SCH_SetTask(CFG_IdleTask_Button);
}

LBR_Gap_Gatt_State LBR_Get_Client_Connection_Status(uint16_t Connection_Handle)
{
  if(LBRContext.connectionHandleRemote == Connection_Handle) 
  {
    return LBRContext.Remote_Connection_Status[0];
  }  
  else 
  {
    return LBC_IDLE;
  }
}

#endif // LB CLIENT


/****************************************************************/
/*                                                              */
/*                     DEVICE IS SERVER                         */
/****************************************************************/

#if(LB_SERVER!=0) 
void LBR_Adv_Request(void)
{
  if(LBRContext.Remote_Connection_Status [0]!= LBC_CONNECTED )
  {
    tBleStatus result=0x00;
    
    result = aci_gap_set_discoverable(ADV_IND,
                             LEDBUTTON_CONN_ADV_INTERVAL_MIN,
                             LEDBUTTON_CONN_ADV_INTERVAL_MAX,
                             PUBLIC_ADDR,
                             NO_WHITE_LIST_USE, /* use white list */
                             sizeof(local_name),
                             local_name,
                             0,
                             NULL,
                             0,
                             0);
    /* Send Advertising data */
   
    if( result == BLE_STATUS_SUCCESS )  {
      APPL_MESG_DBG("  \r\n\r");
      APPL_MESG_DBG("** START ADVERTISING **  \r\n\r");
    }  
    else   
    {
      APPL_MESG_DBG("** START ADVERTISING **  Failed \r\n\r");
    }
  }
  return;
}

void SVCCTL_App_Notification(void *pckt)
{
  hci_event_pckt *event_pckt;
  evt_le_meta_event *meta_evt;
  evt_le_connection_complete * connection_complete_event;

  event_pckt = (hci_event_pckt*)((hci_uart_pckt *)pckt)->data;
  evt_disconn_complete *cc = (void *)event_pckt->data;
  switch(event_pckt->evt)
  {
    case EVT_DISCONN_COMPLETE:
      if(cc->handle == LBRContext.connectionHandleRemote)
      {
        LBRContext.connectionHandleRemote = LBC_IDLE;
        APPL_MESG_DBG("\r\n\r** DISCONNECTION EVENT WITH CLIENT \n");
        handleNotification.LBR_Evt_Opcode=LB_CLIENT_DISCON_EVT_EVT;
        handleNotification.ConnectionHandle=connection_handle;
        LBR_Notification(&handleNotification);
      
      }
      break; /* EVT_DISCONN_COMPLETE */

    case EVT_LE_META_EVENT:
      meta_evt = (evt_le_meta_event*)event_pckt->data;
      switch(meta_evt->subevent)
      {
        case EVT_LE_CONN_COMPLETE:
          /**
           * The connection is done, there is no need anymore to schedule the LP ADV
           */
          connection_complete_event = (evt_le_connection_complete *)meta_evt->data;
         
          connection_handle = connection_complete_event->handle;
      
          //CONNECTION WITH LB ROUTEUR 
           APPL_MESG_DBG("\r\n\r** CONNECTION EVENT WITH CLIENT \n");
          LBRContext.connectionHandleRemote = connection_handle;
          handleNotification.LBR_Evt_Opcode=LB_CLIENT_CONN_HANDLE_EVT;
          handleNotification.ConnectionHandle=connection_handle;
          LBR_Notification(&handleNotification);
         
          
         break; /* HCI_EVT_LE_CONN_COMPLETE */

        default:
          break;
      }
      break; /* HCI_EVT_LE_META_EVENT */

    default:
      break;
  }
  return;
}

void LBR_App_Key_Button_Action(void)
{
  SCH_SetTask(CFG_IdleTask_Button);
}

#endif 

void BLESVC_InitCustomSvc(void)
{
#if(LB_SERVER!=0) 
  LBS_STM_Init();
#endif    
  return;
}

