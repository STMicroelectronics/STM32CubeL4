/**
 ******************************************************************************
 * @file    tl_ble_hci.h
 * @author  MCD Application Team
 * @brief   Constants and functions for HCI layer. See Bluetooth Core
 *          v 4.0, Vol. 2, Part E.
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

#ifndef __TL_BLE_HCI_H_
#define __TL_BLE_HCI_H_

#include "tl_ble_io.h"

/* Exported Types -----------------------------------------------------------*/

typedef struct
{
  uint16_t ogf;
  uint16_t ocf;
  int32_t  event;
  void     *cparam;
  int32_t  clen;
  void     *rparam;
  int32_t  rlen;
} TL_BLE_HCI_Cmd_t;


/**
 * @brief  This API sends a HCI Command
 *
 * @param  p_cmd: Command definition
 * @retval int32_t: 0 when successful, -1 when the command times out
 */
int32_t TL_BLE_HCI_SendCmd( TL_BLE_HCI_Cmd_t *p_cmd );

/**
 * HCI LE Vendor Specific functions definition
 */

/**
 *********************************************************************************************************************
 * START OF SECTION - INTERFACES USED BY THE BLUENRG_BLE_DRIVER
 */

void TL_BLE_HCI_Init(TL_BLE_HCI_InitMode_t initmode, TL_CmdPacket_t * p_cmdbuffer, uint8_t *p_evtpool, uint32_t evtpoolsize);



/**
 * END OF SECTION - INTERFACES USED BY TBLUENRG_BLE_DRIVER
 *********************************************************************************************************************
 */

#endif /* __TL_BLE_HCI_H_ */
