/**
 ******************************************************************************
 * @file    tl_types.h
 * @author  MCD Application Team
 * @brief   Header for tl module
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
#ifndef __TL_TYPES_H
#define __TL_TYPES_H


/* Includes ------------------------------------------------------------------*/
/* Exported defines -----------------------------------------------------------*/
#define TL_BLECMD_PKT_TYPE  ( 0x01 )
#define TL_SYSCMD_PKT_TYPE  ( 0x10 )
#define TL_EVT_PKT_TYPE     ( 0x04 )

#define TL_EVT_HDR_SIZE     (3)
#define TL_EVT_CC_HDR_SIZE  (3)

#define TL_EVT_CC_OPCODE    (0x0E)
#define TL_EVT_CS_OPCODE    (0x0F)

#define TL_BLE_EVENT_FRAME_SIZE ( TL_EVT_HDR_SIZE + CFG_TLBLE_MOST_EVENT_PAYLOAD_SIZE )


/* Exported types ------------------------------------------------------------*/
#ifdef __GNUC__
typedef struct __packed {
#else
typedef __packed struct{
#endif
  uint32_t *next;
  uint32_t *prev;
} TL_PacketHeader_t;


/*******************************************************************************
 * Event type
 */

/**
 * This the the payload of TL_Evt_t
 */
#ifdef __GNUC__
typedef struct __packed {
#else
typedef __packed struct{
#endif
  uint8_t   status;
  uint8_t   numcmd;
  uint16_t  cmdcode;
} TL_CsEvt_t;

/**
 * This the the payload of TL_Evt_t
 */
#ifdef __GNUC__
typedef struct __packed {
#else
typedef __packed struct{
#endif
  uint8_t   numcmd;
  uint16_t  cmdcode;
  uint8_t   payload[1];
} TL_CcEvt_t;

#ifdef __GNUC__
typedef struct __packed {
#else
typedef __packed struct{
#endif
  uint8_t   evtcode;
  uint8_t   plen;
  uint8_t   payload[1];
} TL_Evt_t;

#ifdef __GNUC__
typedef struct __packed {
#else
typedef __packed struct{
#endif
  uint8_t   type;
  TL_Evt_t  evt;
} TL_EvtSerial_t;

#ifdef __GNUC__
typedef struct __packed {
#else
typedef __packed struct{
#endif
  TL_PacketHeader_t  header;
  TL_EvtSerial_t     evtserial;
} TL_EvtPacket_t;

/*****************************************************************************************
 * Command type
 */

#ifdef __GNUC__
typedef struct __packed {
#else
typedef __packed struct{
#endif
  uint16_t   cmdcode;
  uint8_t   plen;
  uint8_t   payload[255];
} TL_Cmd_t;

#ifdef __GNUC__
typedef struct __packed {
#else
typedef __packed struct{
#endif
  uint8_t   type;
  TL_Cmd_t  cmd;
} TL_CmdSerial_t;

#ifdef __GNUC__
typedef struct __packed {
#else
typedef __packed struct{
#endif
  TL_PacketHeader_t  header;
  TL_CmdSerial_t     cmdserial;
} TL_CmdPacket_t;


#endif /*__TL_TYPES_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
