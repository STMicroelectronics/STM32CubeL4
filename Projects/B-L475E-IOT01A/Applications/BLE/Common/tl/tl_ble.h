/**
 ******************************************************************************
 * @file    tl_ble.h
 * @author  MCD Application Team
 * @brief   Header for tl_xxx.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TL_BLE_H
#define __TL_BLE_H


/* Includes ------------------------------------------------------------------*/
#include "common.h"

#include "tl_types.h"


/* Exported types ------------------------------------------------------------*/
typedef enum
{
  TL_BLE_Init_Limited,
  TL_BLE_Init_Full,
} TL_BLE_InitMode_t;

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TL_BLE_Init( TL_BLE_InitMode_t initmode, TL_CmdPacket_t * cmdbuffer, uint8_t *evtpool, uint32_t evtpoolsize );
void TL_BLE_SendCmd( void );
void TL_BLE_EvtDone( TL_EvtPacket_t * hcievt );

void TL_BLE_EvtReceived( TL_EvtPacket_t * hcievt );

#endif /*__TL_BLE_H */

