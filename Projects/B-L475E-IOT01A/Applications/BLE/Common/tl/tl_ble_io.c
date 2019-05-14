/**
 ******************************************************************************
 * @file    tl_io.c
 * @author  MCD Application Team
 * @brief   buttom wrapper to abstract TL from HW and SCHeduler
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
#include "common.h"
#include "tl_types.h"
#include "tl_ble_io.h"
#include "hw.h"
#include "scheduler.h"
#include "svc_ctl.h"


/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Pubblic Functions Definition ----------------------------------------------*/



void TL_BLE_R_EvtProcReq(void)
{
  SCH_SetTask(CFG_IdleTask_TlEvt);

  return;
}

void TL_BLE_HCI_RxCmdResp()
{
  SCH_SetEvt(CFG_IdleEvt_HciCmdEvtResp);

  return;
}

void TL_BLE_HCI_ToNot(void)
{
  return;
}

void TL_BLE_HCI_WaitCmdResp(void)
{
  SCH_WaitEvt(CFG_IdleEvt_HciCmdEvtResp);

  return;
}

void TL_BLE_HCI_TimStop(uint8_t timerID)
{
  HW_TS_Delete(timerID);

  return;
}

void TL_BLE_HCI_TimStart(uint32_t expiryTime, void(*timercb)(void), uint8_t *timerID)
{
  HW_TS_Create(CFG_TimProcID_isr, timerID, hw_ts_SingleShot, (HW_TS_pTimerCb_t) timercb);
  HW_TS_Start(*timerID, expiryTime*1000/CFG_TS_TICK_VAL);

  return;
}


/**
 * @brief  This process shall be called by the scheduler each time it is requested with TL_BLE_HCI_UserEvtProcReq()
 *         This process may send an ACI/HCI command when the svc_ctl.c module is used
 *
 * @param  None
 * @retval None
 */
void TL_BLE_HCI_UserEvtProcReq( TL_PacketHeader_t *p_userevt_queue )
{
  SCH_SetTask(CFG_IdleTask_HciAsynchEvt);

  return;
}

/**
 * @brief This callback is triggered when an user event is received from the BlueNRG device. The event is returned in
 *        a HCI packet. The full HCI packet is stored in a single buffer and is available when this callback is triggered.
 *        However, an ACI event may be longer than a HCI packet and could be fragmented over several HCI packets.
 *        The HCI layer only handles HCI packets so when an ACI packet is split over several HCI packets, this callback
 *        is triggered for each HCI fragment. It is the responsibility of the application to reassemble the ACI event.
 *        This callback is triggered in the TL_BLE_HCI_UserEvtProc() context
 *
 * @param  pckt: The user event received from the BlueNRG device
 * @retval None
 */
void TL_BLE_HCI_UserEvtRx(void *pckt)
{
  SVCCTL_HCI_UserEvtRx(pckt);
}


/* __WEAK functions: The application can overload this function  if needed ----*/
/* ----------------------------------------------------------------------------*/

/**
 * @brief Informs to the application about the status of the TL (busy/available).
 *        The applicaiton shall not send a new command when the status is set to TL_BLE_HCI_CmdBusy.
 *        At least, TL_BLE_HCI_UserEvtProc shall not be executed while the TL remains busy
 *        This __WEAK function has to be OVERLOADED by the application, because only the application
 *        knows which TASKs are currently running and should be paused/resumed 
 * @param status
 * @retval None
 */
__weak void TL_BLE_HCI_StatusNot(TL_BLE_HCI_CmdStatus_t status)
{
  switch(status)
  {
    case TL_BLE_HCI_CmdBusy: 
      /* 
      Only the application knows which TASKs are currently running and should be paused   
      SCH_PauseTask( CFG_IdleTask_XXXX );
      SCH_PauseTask( CFG_IdleTask_YYYY );
      */
      SCH_PauseTask( CFG_IdleTask_HciAsynchEvt );
      break;

    case TL_BLE_HCI_CmdAvailable:
      /* 
      Only the application knows which TASKs are currently running and should be resumed   
      SCH_ResumeTask( CFG_IdleTask_XXXX );
      SCH_ResumeTask( CFG_IdleTask_YYYY );
      */
      SCH_ResumeTask( CFG_IdleTask_HciAsynchEvt );
      break;

    default:
      break;
  }
  return;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
