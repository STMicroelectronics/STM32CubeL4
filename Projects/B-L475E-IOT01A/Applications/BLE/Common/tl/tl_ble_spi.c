/**
 ******************************************************************************
 * @file    tl_ble_spi.c
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
#include "hw.h"

#include "tl_ble.h"
#include "memory_manager.h"
#include "lpm.h"
#include "tl_ble_reassembly.h"


/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  tl_packet_start,
  tl_packet_cont
}TlPacketStatus_t;

typedef struct
{
  TlPacketStatus_t TlPacketStatus;
  TL_BLE_R_EvtPacketType_t TlEventQueueType;
  uint16_t HciPacketLengthOfDataLeftToReceiveFromSpi;
  uint16_t TlSizeLeftInBufferFrame;
}TL_SPI_Isr_Context_t;

/* Private defines -----------------------------------------------------------*/
#define ELT_SIZE (sizeof(TL_PacketHeader_t) + TL_BLE_EVENT_FRAME_SIZE)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static TL_EvtPacket_t * pHciEventBuffer;
static TL_SPI_Isr_Context_t TlSpiIsrContext;
static TL_CmdPacket_t *pCmdBuffer;

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Allocate_New_Buffer_To_TL(void);

/* Public Functions Definition ------------------------------------------------------*/

/**
 * HCI wrapper
 */

void TL_BLE_Init(TL_BLE_InitMode_t initmode, TL_CmdPacket_t * cmdbuffer, uint8_t *evtpool, uint32_t evtpoolsize)
{
  TlSpiIsrContext.TlPacketStatus = tl_packet_start;

  MM_Init(evtpool, evtpoolsize, ELT_SIZE);

  pCmdBuffer = cmdbuffer;

  TL_BLE_R_Init(cmdbuffer);

  if( initmode == TL_BLE_Init_Limited)
  {
    HW_BNRG_Init(HW_BNRG_Limited);
  }
  else
  {
    /**
     * Initialize the BlueNRG SPI driver
     */
    HW_BNRG_Init(HW_BNRG_Full);

    /**
     * Wait for the Reset Event from BlueNRG
     */
    while(HAL_GPIO_ReadPin(BNRG_SPI_IRQ_PORT, BNRG_SPI_IRQ_PIN) == GPIO_PIN_RESET);
  }

  /**
   * Provide event buffer to TL layer
   */
  Allocate_New_Buffer_To_TL();

  return;
}

void TL_BLE_SendCmd(void)
{
  pCmdBuffer->cmdserial.type = TL_BLECMD_PKT_TYPE;

  HW_BNRG_Send( (uint8_t *)&pCmdBuffer->cmdserial, 4 + pCmdBuffer->cmdserial.cmd.plen );

  return;
}

/**
 * SPI wrapper
 */

void HW_BNRG_Isr(uint16_t event_payload_len)
{
  if(TlSpiIsrContext.TlPacketStatus == tl_packet_cont)
  {
    /**
     * The packet HCI has been splitted over several SPI frame
     */
    TlSpiIsrContext.HciPacketLengthOfDataLeftToReceiveFromSpi -= event_payload_len;
  }
  else
  {
    /**
     * This is the start of a HCI packet
     */
    TlSpiIsrContext.HciPacketLengthOfDataLeftToReceiveFromSpi = TL_EVT_HDR_SIZE + pHciEventBuffer->evtserial.evt.plen - event_payload_len;

    if((pHciEventBuffer->evtserial.evt.evtcode != TL_EVT_CC_OPCODE) && (pHciEventBuffer->evtserial.evt.evtcode != TL_EVT_CS_OPCODE))
    {
      /**
       * Asynchronous Event
       */
      TlSpiIsrContext.TlEventQueueType = TL_BLE_R_HciAsynchEvt;
      LPM_SetOffMode(CFG_LPM_HCI_AsynchEvt, LPM_OffMode_Dis);
    }
    else
    {
      /**
       * Cmd Event
       */
      TlSpiIsrContext.TlEventQueueType = TL_BLE_R_HciCmdEvt;
    }
  }

  if(TlSpiIsrContext.HciPacketLengthOfDataLeftToReceiveFromSpi != 0)
  {
    /**
     * The HCI packet is not complete
     */
    TlSpiIsrContext.TlPacketStatus = tl_packet_cont;
    TlSpiIsrContext.TlSizeLeftInBufferFrame -= event_payload_len;
    if(TlSpiIsrContext.TlSizeLeftInBufferFrame != 0)
    {
      /**
       * There are still some bytes left to receive more data
       */
      HW_BNRG_EvtReq((uint8_t*)&pHciEventBuffer->evtserial + (TL_BLE_EVENT_FRAME_SIZE - TlSpiIsrContext.TlSizeLeftInBufferFrame), TlSpiIsrContext.TlSizeLeftInBufferFrame);
    }
    else
    {
      /**
       * Store the received packet in the relevant queue
       */
      TL_BLE_R_EvtInput(TlSpiIsrContext.TlEventQueueType, (uint8_t*)pHciEventBuffer);

      /**
       * a new buffer is required to receive missing data
       */
      Allocate_New_Buffer_To_TL();
    }
  }
  else
  {
    /**
     * Store the received packet in the relevant queue
     */
    TL_BLE_R_EvtInput(TlSpiIsrContext.TlEventQueueType, (uint8_t*)pHciEventBuffer);

    /**
     * The HCI packet has been fully received
     */
    TlSpiIsrContext.TlPacketStatus = tl_packet_start;
    Allocate_New_Buffer_To_TL();

    LPM_SetOffMode(CFG_LPM_HCI_AsynchEvt, LPM_OffMode_En);
  }

  return;
}

void TL_BLE_EvtDone(TL_EvtPacket_t * hcievt)
{
  TL_BLE_R_EvtDone(hcievt);

  return;
}


/* Private Functions Definition ------------------------------------------------------*/
static void Allocate_New_Buffer_To_TL(void)
{
  pHciEventBuffer =  (TL_EvtPacket_t*) MM_GetBuffer(sizeof(TL_PacketHeader_t) + TL_BLE_EVENT_FRAME_SIZE, (MM_pCb_t) Allocate_New_Buffer_To_TL);
  if(pHciEventBuffer)
  {
    TlSpiIsrContext.TlSizeLeftInBufferFrame = TL_BLE_EVENT_FRAME_SIZE;
    HW_BNRG_EvtReq((uint8_t *)&(pHciEventBuffer->evtserial), TL_BLE_EVENT_FRAME_SIZE);
  }

  return;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
