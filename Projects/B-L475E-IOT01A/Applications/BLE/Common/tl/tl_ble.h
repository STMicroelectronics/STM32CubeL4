/**
 ******************************************************************************
 * @file    tl_ble.h
 * @author  MCD Application Team
 * @brief   Header for tl_xxx.c module
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
