/**
 ******************************************************************************
 * @file    tl_ble_reassembly.h
 * @author  MCD Application Team
 * @brief   Header for tl_serial.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TL_BLE_REASSEMBLY_H
#define __TL_BLE_REASSEMBLY_H


/* Includes ------------------------------------------------------------------*/
#include "tl_types.h"

/* Exported defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  TL_BLE_R_HciAsynchEvt,
  TL_BLE_R_HciCmdEvt
}TL_BLE_R_EvtPacketType_t;

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/**
 *********************************************************************************************************************
 * START OF SECTION - PROCESS TO BE CALLED BY THE SCHEDULER
 */

/**
 * @brief  This process shall be called by the scheduler each time it is requested with TL_BLE_R_EvtProcReq()
 *         This process does not send any ACI/HCI command so it can be executed without checking the availability of
 *         the HCI Layer
 *
 * @param  None
 * @retval None
 */
void TL_BLE_R_EvtProc(void);

/**
 * END OF SECTION - PROCESS TO BE CALLED BY THE SCHEDULER
 *********************************************************************************************************************
 */

/**
 * @brief  This callback is triggered when an event is received from the BlueNRG. It requests the process TL_BLE_HCI_UserEvtProc()
 *         to be executed.
 *         It is called from either:
 *         HW_BNRG_DMARxCb - RX DMA interrupt handler
 *         TimerTxRxCallback - RTC interrupt handler
 *         TL_BLE_R_EvtProc() context.
 *
 * @param  None
 * @retval None
 */
void TL_BLE_R_EvtProcReq(void);

void TL_BLE_R_EvtInput(TL_BLE_R_EvtPacketType_t hci_evt_packet_type, uint8_t *hci_evt_buffer);
void TL_BLE_R_Init(TL_CmdPacket_t * cmdbuffer);
void TL_BLE_R_EvtDone(TL_EvtPacket_t * hcievt);

#endif /*__TL_BLE_REASSEMBLY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
