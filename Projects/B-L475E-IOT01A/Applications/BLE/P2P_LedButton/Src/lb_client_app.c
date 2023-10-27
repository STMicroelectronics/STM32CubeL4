/**
 ******************************************************************************
 * @file    lb_client_app.c
 * @author  MCD Application Team
 * @brief   Led Button Demo - Routeur Application
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

#include "lb_client_app.h"
#include "lb_demo.h"

#include "ble_lib.h"
#include "blesvc.h"
#include "scheduler.h"
#include "stm32l475e_iot01.h"

#if (LB_CLIENT != 0) /**< LED BOUTTON ROUTER APPLICATION  */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint8_t                Notification_Button_Status; /* used to check if smart phone can receive push button information */
  LBR_LedCharValue_t     LedControlRemoteServer;
  uint16_t               connHandleWithserver; 
  uint8_t                connStatusWithServer; 
} LB_Client_Context_t;


/* Private defines -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
 * START of Section BLE_APP_CONTEXT
 */
static LB_Client_Context_t LB_Client_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/


void LBR_Notification(LBR_ConnHandle_Not_evt_t *pNotification)
{
  switch(pNotification->LBR_Evt_Opcode)
  {
    case LB_SERVER_CONN_HANDLE_EVT:
       LB_Client_Context.connHandleWithserver = pNotification->ConnectionHandle; 
       LB_Client_Context.connStatusWithServer = 0x01; //connected
       
    break;
      
    case LB_SERVER_DISCON_EVT :
      LB_Client_Context.connHandleWithserver = 0x00;
      LB_Client_Context.connStatusWithServer = 0x00; //Not connected
      
      //restart Create Connection
      SCH_SetTask(CFG_IdleTask_ConnDev1);
    break;
  
    default:
      break;
  }

  return;
}

void LBSAPP_Init(void)
{
  /**
   * Initialize LedButton Service
   */
  LB_Client_Context.Notification_Button_Status=0; 
  
  LB_Client_Context.connHandleWithserver=0x00;
  LB_Client_Context.connStatusWithServer=0x00;
  
  LB_Client_Context.LedControlRemoteServer.Device_Led_Selection=0x01;
  LB_Client_Context.LedControlRemoteServer.Led1Control=0x01;
  
  return;
}

void LB_App_Button_Trigger_Received(void)
{

  if(LB_Client_Context.connStatusWithServer == 0x01  ){
    if( LB_Client_Context.LedControlRemoteServer.Led1Control==0x00){
      LB_Client_Context.LedControlRemoteServer.Led1Control=0x01;
    } else {
      LB_Client_Context.LedControlRemoteServer.Led1Control=0x00;
    }
               
        APPL_MESG_DBG("-- APPLICATION CLIENT : WRITE LED CONFIG TO SERVER\n\r");
        LBR_Client_Update_Char( LED_CHAR_UUID, 0, (uint8_t *)&LB_Client_Context.LedControlRemoteServer); 
   
  }
  
}



/* CLIENT LED Button*/
/*-------------------*/
void LBR_Client_App_Notification(LBR_Client_App_Notification_evt_t *pNotification)
{
  switch(pNotification->LBR_Client_Evt_Opcode)
  {
    case LBC_BUTTON_INFO_RECEIVED_EVT:
      {
        switch(pNotification->DataTransfered.pPayload[1]) {
        case 0x00 : 
          BSP_LED_Off(LED2);
          break;
        case 0x01 : 
          BSP_LED_On(LED2);
          break;
        
        default : break;
        }
       
      }
      break;
   
    default:
      break;
 
  }
  return;
}


#endif //LB_CLIENT

