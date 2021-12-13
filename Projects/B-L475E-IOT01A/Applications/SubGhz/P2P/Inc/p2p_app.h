/**
******************************************************************************
  * @file    p2p_app.h
  * @author  MCD Application Team
  * @brief   Header file of the spirit1 P2P application.
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
#ifndef __P2P_APP_H
#define __P2P_APP_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx.h"
#include "spsgrf.h"
#include "spsgrf_io.h"


/*******Platform definition : Uncomment the expansion board to be used*********/
#define SPSGRF_EU
//#define SPSGRF_US
/******************************************************************************/


/************************Set the Node address**********************************/ 
/*Source address of one node should be destination for other node & vice-versa*/  
#define MY_ADDRESS                  0x44  //0x44
#define DESTINATION_ADDRESS         0x44  //0x34

/*************************Uncomment the the protocol to be used****************/
/* One of the other of the two defines should be activated */
//#define USE_STack_PROTOCOL
#define USE_BASIC_PROTOCOL

/* to activate Basic protocol with Address field */
/* to be activated with USE_BASIC_PROTOCOL */
#define USE_BASIC_PROTOCOL_ADDRESS

#if defined(USE_STack_PROTOCOL)
  //#define USE_STack_LLP  /*Uncomment if LLP featured need to be used*/
#endif

#ifdef USE_BASIC_PROTOCOL
  #define CSMA_ENABLE  /* Comment this line to disable the CSMA */
#endif
/******************************************************************************/

#ifdef CSMA_ENABLE
//#include "SPIRIT_Csma.h"


/* CSMA configuration parameters */
  #define PERSISTENT_MODE_EN              S_DISABLE
  #define CS_PERIOD                       TBIT_TIME_64
  #define CS_TIMEOUT                      TCCA_TIME_3
  #define MAX_NB                          5
  #define BU_COUNTER_SEED                 0xFA21
  #define CU_PRESCALER                    32
  
extern CsmaInit xCsmaInit;

#endif

/*************** SPIRIT1 CSMA Settings end ************************************/

#if defined(SPSGRF_EU)
         #define USE_SPIRIT1_868MHz
#elif defined(SPSGRF_US)
         #define USE_SPIRIT1_915MHz
#else
#error SPIRIT1 Expansion Board is undefined or unsupported
#endif


/* Exported constants --------------------------------------------------------*/

/*  Radio configuration parameters  */
#define XTAL_OFFSET_PPM             0
#define INFINITE_TIMEOUT            0.0

#ifdef USE_SPIRIT1_433MHz
#define BASE_FREQUENCY              433.0e6
#endif

#ifdef USE_SPIRIT1_868MHz
#define BASE_FREQUENCY              868.0e6
#endif

#ifdef USE_SPIRIT1_915MHz
#define BASE_FREQUENCY              915.0e6
#endif

#define CHANNEL_SPACE               100e3
#define CHANNEL_NUMBER              0
#define MODULATION_SELECT           FSK
#define DATARATE                    38400
#define FREQ_DEVIATION              20e3
#define BANDWIDTH                   100E3

#define POWER_DBM                   11.6
#define POWER_INDEX                 7
#define RECEIVE_TIMEOUT             2000.0 /*change the value for required timeout period*/   

#define RSSI_THRESHOLD              -120  /* Default RSSI at reception, more 
                                                             than noise floor */
#define CSMA_RSSI_THRESHOLD         -90   /* Higher RSSI to Transmit. 
                              If it's lower, the Channel will be seen as busy */

/*  Packet configuration parameters  */
#define PREAMBLE_LENGTH             PKT_PREAMBLE_LENGTH_04BYTES
#define SYNC_LENGTH                 PKT_SYNC_LENGTH_4BYTES
#define SYNC_WORD                   0x88888888
#define LENGTH_TYPE                 PKT_LENGTH_VAR
#define LENGTH_WIDTH                7
#define CRC_MODE                    PKT_CRC_MODE_8BITS
#define CONTROL_LENGTH              PKT_CONTROL_LENGTH_0BYTES
#define EN_FEC                      S_DISABLE
#define EN_WHITENING                S_ENABLE

#ifndef USE_BASIC_PROTOCOL_ADDRESS
  /*  Addresses configuration parameters  */
  #define EN_ADDRESS                  S_DISABLE
  #define EN_FILT_MY_ADDRESS          S_DISABLE
  #define EN_FILT_MULTICAST_ADDRESS   S_DISABLE
  #define EN_FILT_BROADCAST_ADDRESS   S_DISABLE
  #define EN_FILT_SOURCE_ADDRESS      S_DISABLE//S_ENABLE
  #define SOURCE_ADDR_MASK            0xf0
  #define SOURCE_ADDR_REF             0x37
  #define MULTICAST_ADDRESS           0xEE
  #define BROADCAST_ADDRESS           0xFF 
    
#else

  #define EN_ADDRESS                  S_ENABLE
  #define EN_FILT_MY_ADDRESS          S_ENABLE
  #define EN_FILT_MULTICAST_ADDRESS   S_ENABLE
  #define EN_FILT_BROADCAST_ADDRESS   S_ENABLE
  #define EN_FILT_SOURCE_ADDRESS      S_ENABLE
  #define SOURCE_ADDR_MASK            0xf0
  #define SOURCE_ADDR_REF             0x37
  #define MULTICAST_ADDRESS           0xEE
  #define BROADCAST_ADDRESS           0xFF 

#endif
    
#define EN_AUTOACK                      S_DISABLE
#define EN_PIGGYBACKING                 S_DISABLE
#define MAX_RETRANSMISSIONS             PKT_DISABLE_RETX


#define PAYLOAD_LEN                     25 /*20 bytes data+tag+cmd_type+cmd+cmdlen+datalen*/
#define APPLI_CMD                       0x11
#define NWK_CMD                         0x22
#define LED_TOGGLE                      0xff
#define ACK_OK                          0x01
#define MAX_BUFFER_LEN                  96
#define TIME_TO_EXIT_RX                 3000
#define DELAY_RX_LED_TOGGLE             100   
#define DELAY_TX_LED_GLOW               200 
#define LPM_WAKEUP_TIME                 100
#define DATA_SEND_TIME                  50//30

/* Exported types ------------------------------------------------------------*/
extern volatile FlagStatus xRxDoneFlag, xTxDoneFlag;
extern volatile FlagStatus PushButtonStatusWakeup; 
extern uint16_t wakeupCounter;
extern uint16_t dataSendCounter ;
extern volatile FlagStatus PushButtonStatusData, datasendFlag;


/**
* @brief  WMBus Link Layer State Enum.
*/
typedef enum {
   SM_STATE_START_RX=0,
   SM_STATE_WAIT_FOR_RX_DONE,
   SM_STATE_DATA_RECEIVED,
   SM_STATE_SEND_DATA,
   SM_STATE_WAIT_FOR_TX_DONE,
   SM_STATE_ACK_RECEIVED,
   SM_STATE_SEND_ACK,
   SM_STATE_TOGGLE_LED,
   SM_STATE_IDLE=0xFF
} SM_State_t;

typedef struct sRadioDriver
{
    void ( *Init )( void );
    void ( *GpioIrq )( SGpioInit *pGpioIRQ );
    void ( *RadioInit )( SRadioInit *pRadioInit );
    void ( *SetRadioPower )( uint8_t cIndex, float fPowerdBm );
    void ( *PacketConfig )( void );
    void ( *SetPayloadLen )( uint8_t length);
    void ( *SetDestinationAddress )( uint8_t address);
    void ( *EnableTxIrq )( void );
    void ( *EnableRxIrq )( void );
    void ( *DisableIrq )(void);
    void ( *SetRxTimeout )( float cRxTimeout );
    void ( *EnableSQI )(void);
    void ( *SetRssiThreshold)(int dbmValue);
    void ( *ClearIrqStatus )(void);
    void ( *StartRx )( void );
    void ( *StartTx )( uint8_t *buffer, uint8_t size ); 
    void ( *GetRxPacket )( uint8_t *buffer, uint8_t *size );
}RadioDriver_t;   

typedef struct sMCULowPowerMode
{
    void ( *McuStopMode )( void );
    void ( *McuStandbyMode )( void );
    void ( *McuSleepMode )( void );      
}MCULowPowerMode_t;

typedef struct sRadioLowPowerMode
{
    void ( *RadioShutDown )( void );
    void ( *RadioStandBy )( void );
    void ( *RadioSleep ) ( void );
    void ( *RadioPowerON )( void );
}RadioLowPowerMode_t; 

typedef struct
{
  uint8_t Cmdtag;
  uint8_t CmdType;
  uint8_t CmdLen;
  uint8_t Cmd;
  uint8_t DataLen;
  uint8_t* DataBuff;
}AppliFrame_t;

#ifdef CSMA_ENABLE
typedef struct
{
  int32_t RxRSSIThreshold;
  uint8_t CsmaEnabled;
  int32_t CsmaRSSIThreshold;/*!<RSSI Threshold for CSMA*/
  uint8_t CsmaOverrideFail;
  uint8_t CsmaCcaPeriod; /*!< RSSI meas cycle = (CsmaCcaPeriod+1)*64*TBIT*/
  uint8_t CsmaCcaLength; /*!<times of CsmaCcaPeriod for ch assesment */
  uint8_t CsmaMaxNb; /*!<Max N backoff */
  uint16_t CsmaBuSeed; /*!<seed for rand in (2^rand)* presc */
  uint8_t CsmaBuPrescaler; /*!<presc for (2^rand)*presc */
}CSMA_Attr_typedef;
#endif


/* Exported functions ------------------------------------------------------- */
void HAL_Spirit1_Init(void);
void P2P_Process(uint8_t *pTxBuff, uint8_t cTxlen, uint8_t* pRxBuff, uint8_t cRxlen);
void Enter_LP_mode(void);
void Exit_LP_mode(void);
void MCU_Enter_StopMode(void);
void MCU_Enter_StandbyMode(void);
void MCU_Enter_SleepMode(void);
void RadioPowerON(void);
void RadioPowerOFF(void);
void RadioStandBy(void);
void RadioSleep(void);
void AppliSendBuff(AppliFrame_t *xTxFrame, uint8_t cTxlen);
void AppliReceiveBuff(uint8_t *RxFrameBuff, uint8_t cRxlen);
void P2P_Init(void);
void STackProtocolInit(void);
void BasicProtocolInit(void);
void P2PInterruptHandler(void);
void Set_KeyStatus(FlagStatus val);


#endif /* __P2P_APP_H */

