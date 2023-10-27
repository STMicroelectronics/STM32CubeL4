/**
  ******************************************************************************
  * @file    wifi.h
  * @author  MCD Application Team
  * @brief   This file contains the different WiFi core resources definitions.
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
#ifndef WIFI_H
#define WIFI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "es_wifi.h"
#include "es_wifi_io.h"

/* Exported constants --------------------------------------------------------*/
#define WIFI_MAX_SSID_NAME            100
#define WIFI_MAX_PSWD_NAME            100
#define WIFI_MAX_APS                  20
#define WIFI_MAX_CONNECTIONS          4
#define WIFI_MAX_MODULE_NAME          100
#define WIFI_MAX_CONNECTED_STATIONS   2
#define WIFI_MSG_JOINED               1
#define WIFI_MSG_ASSIGNED             2


/* Exported types ------------------------------------------------------------*/
typedef enum {
  WIFI_ECN_OPEN = 0x00,
  WIFI_ECN_WEP = 0x01,
  WIFI_ECN_WPA_PSK = 0x02,
  WIFI_ECN_WPA2_PSK = 0x03,
  WIFI_ECN_WPA_WPA2_PSK = 0x04
}WIFI_Ecn_t;

typedef enum {
  WIFI_TCP_PROTOCOL = 0,
  WIFI_UDP_PROTOCOL = 1,
}WIFI_Protocol_t;

typedef enum {
  WIFI_SERVER = 0,
  WIFI_CLIENT = 1,
}WIFI_Type_t;

typedef enum {
  WIFI_STATUS_OK             = 0,
  WIFI_STATUS_ERROR          = 1,
  WIFI_STATUS_NOT_SUPPORTED  = 2,
  WIFI_STATUS_JOINED         = 3,
  WIFI_STATUS_ASSIGNED       = 4,
  WIFI_STATUS_TIMEOUT        = 5
} WIFI_Status_t;

typedef struct {
  WIFI_Ecn_t Ecn;                    /*!< Security of Wi-Fi spot. This parameter has a value of \ref WIFI_Ecn_t enumeration */
  char SSID[WIFI_MAX_SSID_NAME + 1]; /*!< Service Set Identifier value. Wi-Fi spot name */
  int16_t RSSI;                      /*!< Signal strength of Wi-Fi spot */
  uint8_t MAC[6];                    /*!< MAC address of spot */
  uint8_t Channel;                   /*!< Wi-Fi channel */
  uint8_t Offset;                    /*!< Frequency offset from base 2.4GHz in kHz */
  uint8_t Calibration;               /*!< Frequency offset calibration */
} WIFI_AP_t;

typedef struct {
  WIFI_AP_t    ap[WIFI_MAX_APS];
  uint8_t      count;
} WIFI_APs_t;


typedef struct {
  uint8_t Number;                    /*!< Connection number */
  uint16_t RemotePort;               /*!< Remote PORT number */
  uint16_t LocalPort;
  uint8_t RemoteIP[4];               /*!< IP address of device */
  WIFI_Protocol_t Protocol;          /*!< Connection type. Parameter is valid only if connection is made as client */
  uint32_t TotalBytesReceived;       /*!< Number of bytes received in entire connection lifecycle */
  uint32_t TotalBytesSent;           /*!< Number of bytes sent in entire connection lifecycle */
  uint8_t Active;                    /*!< Status if connection is active */
  uint8_t Client;                    /*!< Set to 1 if connection was made as client */
} WIFI_Socket_t;


typedef struct {
  uint8_t          SSID[WIFI_MAX_SSID_NAME + 1];
  uint8_t          PSWD[WIFI_MAX_PSWD_NAME + 1];
  uint8_t          channel;
  WIFI_Ecn_t       Ecn;
} WIFI_APConfig_t;

typedef struct {
  uint8_t SSID[WIFI_MAX_SSID_NAME + 1]; /*!< Network public name for ESP AP mode */
  uint8_t IP_Addr[4];                   /*!< IP Address */
  uint8_t MAC_Addr[6];                  /*!< MAC address */
} WIFI_APSettings_t;

typedef struct {
  uint8_t          IsConnected;
  uint8_t          IP_Addr[4];
  uint8_t          IP_Mask[4];
  uint8_t          Gateway_Addr[4];
} WIFI_Conn_t;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
WIFI_Status_t WIFI_Init(void);
WIFI_Status_t WIFI_ListAccessPoints(WIFI_APs_t *APs, uint8_t AP_MaxNbr);
WIFI_Status_t WIFI_Connect(const char *SSID, const char *Password, WIFI_Ecn_t ecn);
WIFI_Status_t WIFI_GetIP_Address(uint8_t *ipaddr, uint8_t IpAddrLength);
WIFI_Status_t WIFI_GetMAC_Address(uint8_t *mac, uint8_t MacLength);

WIFI_Status_t WIFI_Disconnect(void);
WIFI_Status_t WIFI_ConfigureAP(const uint8_t *ssid, const uint8_t *pass, WIFI_Ecn_t ecn,
                               uint8_t channel, uint8_t max_conn);

WIFI_Status_t WIFI_HandleAPEvents(WIFI_APSettings_t *setting);
WIFI_Status_t WIFI_Ping(const uint8_t *ipaddr, uint16_t count, uint16_t interval_ms,int32_t result[]);
WIFI_Status_t WIFI_GetHostAddress(const char *location, uint8_t *ipaddr, uint8_t IpAddrLength);

WIFI_Status_t WIFI_OpenClientConnection(uint32_t socket, WIFI_Protocol_t type, const char *name,
                                        const uint8_t *ipaddr, uint16_t port, uint16_t local_port);
WIFI_Status_t WIFI_CloseClientConnection(uint32_t socket);

WIFI_Status_t WIFI_StartServer(uint32_t socket, WIFI_Protocol_t type, uint16_t backlog, const char *name,
                               uint16_t port);
WIFI_Status_t WIFI_WaitServerConnection(uint32_t socket, uint32_t Timeout,
                                        uint8_t *remoteipaddr, uint8_t RemoteIpAddrLength, uint16_t *remoteport);

WIFI_Status_t WIFI_CloseServerConnection(uint32_t socket);
WIFI_Status_t WIFI_StopServer(uint32_t socket);

WIFI_Status_t WIFI_SendData(uint32_t socket, const uint8_t *pdata, uint16_t Reqlen, uint16_t *SentDatalen,
                            uint32_t Timeout);
WIFI_Status_t WIFI_SendDataTo(uint32_t socket, const uint8_t *pdata, uint16_t Reqlen, uint16_t *SentDatalen,
                              uint32_t Timeout,
                              const uint8_t *ipaddr, uint16_t port);
WIFI_Status_t WIFI_ReceiveData(uint32_t socket, uint8_t *pdata, uint16_t Reqlen, uint16_t *RcvDatalen,
                               uint32_t Timeout);
WIFI_Status_t WIFI_ReceiveDataFrom(uint32_t socket, uint8_t *pdata, uint16_t Reqlen, uint16_t *RcvDatalen,
                                   uint32_t Timeout, uint8_t *ipaddr, uint8_t IpAddrLength, uint16_t *port);
WIFI_Status_t WIFI_StartClient(void);
WIFI_Status_t WIFI_StopClient(void);

WIFI_Status_t WIFI_SetOEMProperties(const char *name, const uint8_t *Mac);
WIFI_Status_t WIFI_ResetModule(void);
WIFI_Status_t WIFI_SetModuleDefault(void);
WIFI_Status_t WIFI_ModuleFirmwareUpdate(const char *url);
WIFI_Status_t WIFI_GetModuleID(char *Id, uint8_t IdLength);
WIFI_Status_t WIFI_GetModuleFwRevision(char *rev, uint8_t RevLength);
WIFI_Status_t WIFI_GetModuleName(char *ModuleName, uint8_t ModuleNameLength);
#ifdef __cplusplus
}
#endif

#endif /* WIFI_H */
