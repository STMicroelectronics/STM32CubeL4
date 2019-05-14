/**
 ******************************************************************************
 * @file    tl_ble_reassembly.c
 * @author  MCD Application Team
 * @brief   Transport layer for Serial interface
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
#include "tl_ble_reassembly.h"
#include "tl_ble.h"
#include "list.h"
#include "memory_manager.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  hci_packet_start,
  hci_packet_cont
}HciPacketStatus_t;

typedef enum
{
  HciAsynchEventLargeBuffer_Free,
  HciAsynchEventLargeBuffer_Busy
}HciAsynchEventLargeBufferStatus_t;

typedef struct
{
  tListNode         TlBufferQueue;
  HciPacketStatus_t HciPacketStatus;
  uint16_t           HciPacketLengthOfDataLeftToReceiveFromTl;
  uint16_t           HciLargeBufferOffset;
}TL_Queue_Context_t;

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static HciAsynchEventLargeBufferStatus_t HciAsynchEventLargeBufferStatus;
static uint8_t HciLargeAsynchBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];
static TL_Queue_Context_t TLQueueEvtContext[2];
static TL_CmdPacket_t *pCmdBuffer;

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Event_Queue_Process(TL_BLE_R_EvtPacketType_t tl_event_queue_type);

/* Public Functions Definition ------------------------------------------------------*/

void TL_BLE_R_EvtInput(TL_BLE_R_EvtPacketType_t hci_evt_packet_type, uint8_t *hci_evt_buffer)
{
  LST_insert_tail (&TLQueueEvtContext[hci_evt_packet_type].TlBufferQueue, (tListNode *)hci_evt_buffer);

  TL_BLE_R_EvtProcReq();

  return;
}

void TL_BLE_R_EvtProc(void)
{
  /**
   * Read CMD Event Queue
   */
  Event_Queue_Process(TL_BLE_R_HciCmdEvt);

  /**
   * Read Asynch Event Queue
   */
  Event_Queue_Process(TL_BLE_R_HciAsynchEvt);

  return;
}

void TL_BLE_R_Init(TL_CmdPacket_t *cmdbuffer)
{
  TLQueueEvtContext[TL_BLE_R_HciAsynchEvt].HciPacketStatus = hci_packet_start;
  TLQueueEvtContext[TL_BLE_R_HciCmdEvt].HciPacketStatus = hci_packet_start;
  TLQueueEvtContext[TL_BLE_R_HciAsynchEvt].HciLargeBufferOffset = 0;
  TLQueueEvtContext[TL_BLE_R_HciCmdEvt].HciLargeBufferOffset = 0;

  HciAsynchEventLargeBufferStatus = HciAsynchEventLargeBuffer_Free;

  LST_init_head (&TLQueueEvtContext[TL_BLE_R_HciAsynchEvt].TlBufferQueue);
  LST_init_head (&TLQueueEvtContext[TL_BLE_R_HciCmdEvt].TlBufferQueue);

  pCmdBuffer = cmdbuffer;

  return;
}

/* Private Functions Definition ------------------------------------------------------*/
static void Event_Queue_Process(TL_BLE_R_EvtPacketType_t tl_event_queue_type)
{
  TL_EvtPacket_t * ptl_evt_buffer;
  TL_EvtPacket_t *plarge_hci_buffer;

  if(tl_event_queue_type == TL_BLE_R_HciAsynchEvt)
  {
    plarge_hci_buffer = (TL_EvtPacket_t *)&HciLargeAsynchBuffer;
  }
  else
  {
    plarge_hci_buffer = (TL_EvtPacket_t*)pCmdBuffer;
  }

  while((LST_is_empty (&TLQueueEvtContext[tl_event_queue_type].TlBufferQueue) == FALSE) && ((HciAsynchEventLargeBufferStatus == HciAsynchEventLargeBuffer_Free) || (tl_event_queue_type == TL_BLE_R_HciCmdEvt)))
  {
    LST_remove_head (&TLQueueEvtContext[tl_event_queue_type].TlBufferQueue, (tListNode **)&ptl_evt_buffer);
    if(TLQueueEvtContext[tl_event_queue_type].HciPacketStatus == hci_packet_start)
    {
      TLQueueEvtContext[tl_event_queue_type].HciPacketLengthOfDataLeftToReceiveFromTl = ptl_evt_buffer->evtserial.evt.plen + TL_EVT_HDR_SIZE;
    }

    if(TLQueueEvtContext[tl_event_queue_type].HciPacketLengthOfDataLeftToReceiveFromTl > TL_BLE_EVENT_FRAME_SIZE)
    {
      /**
       * The packet is not complete
       */
      TLQueueEvtContext[tl_event_queue_type].HciPacketStatus = hci_packet_cont;
      TLQueueEvtContext[tl_event_queue_type].HciPacketLengthOfDataLeftToReceiveFromTl -= TL_BLE_EVENT_FRAME_SIZE;
      memcpy((uint8_t*)&plarge_hci_buffer->evtserial + TLQueueEvtContext[tl_event_queue_type].HciLargeBufferOffset, &ptl_evt_buffer->evtserial, TL_BLE_EVENT_FRAME_SIZE);
      MM_ReleaseBuffer((MM_pBufAdd_t)ptl_evt_buffer);
      TLQueueEvtContext[tl_event_queue_type].HciLargeBufferOffset += TL_BLE_EVENT_FRAME_SIZE;
    }
    else
    {
      /**
       * End of HCI packet
       */
      if(TLQueueEvtContext[tl_event_queue_type].HciPacketStatus == hci_packet_cont)
      {
        memcpy((uint8_t*)&plarge_hci_buffer->evtserial + TLQueueEvtContext[tl_event_queue_type].HciLargeBufferOffset, &ptl_evt_buffer->evtserial, TLQueueEvtContext[tl_event_queue_type].HciPacketLengthOfDataLeftToReceiveFromTl);
        MM_ReleaseBuffer((MM_pBufAdd_t)ptl_evt_buffer);
        if(tl_event_queue_type == TL_BLE_R_HciAsynchEvt)
        {
          /**
           * It shall be set to busy only for Asynchronous Events
           */
          HciAsynchEventLargeBufferStatus = HciAsynchEventLargeBuffer_Busy;
        }
        TL_BLE_EvtReceived( (TL_EvtPacket_t *)plarge_hci_buffer );
      }
      else
      {
        TL_BLE_EvtReceived( (TL_EvtPacket_t *)ptl_evt_buffer );
      }
      TLQueueEvtContext[tl_event_queue_type].HciPacketStatus = hci_packet_start;
      TLQueueEvtContext[tl_event_queue_type].HciLargeBufferOffset  = 0;
    }
  }

  return;
}

void TL_BLE_R_EvtDone(TL_EvtPacket_t * hcievt)
{
  if(hcievt == (TL_EvtPacket_t *)&HciLargeAsynchBuffer)
  {
    HciAsynchEventLargeBufferStatus = HciAsynchEventLargeBuffer_Free;

    /**
     * Check if there are waiting events to be copied in the large buffer
     */
    TL_BLE_R_EvtProcReq();
  }
  else if(hcievt != (TL_EvtPacket_t *)pCmdBuffer)
  {
    MM_ReleaseBuffer((MM_pBufAdd_t)hcievt);
  }

  return;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
