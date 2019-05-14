/**
 ******************************************************************************
 * @file    debug.h
 * @author  MCD Application Team
 * @brief   debug macro Declarations
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
#ifndef _DEBUG_H_
#define _DEBUG_H_
/******************************************************************************
 * Includes
 *****************************************************************************/
/* Print the data travelling over the SPI in the .csv format for the GUI*/
#define PRINT_CSV_FORMAT                                                       0
#if (PRINT_CSV_FORMAT == 1)
#include <stdio.h>
#define PRINT_CSV(...)                                       printf(__VA_ARGS__)
#else
#define PRINT_CSV(...)
#endif

#if (CFG_DEBUG_TRACE == 1)
#define DEBUG_HCI                                                              1
#define DEBUG_TIMER                                                            1
#define DEBUG_PROFILE                                                          1
#define DEBUG_HRPROFILE                                                        1
#define DEBUG_PROXIMITY_REPORTER                                               1
#define DEBUG_PROXIMITY_MONITOR                                                1
#define DEBUG_CURRENT_TIME_SERVER                                              1
#define DEBUG_CURRENT_TIME_CLIENT                                              1
#define DEBUG_HID_DEVICE_SERVER                                                1
#define DEBUG_HID_DEVICE_CLIENT                                                1
#define DEBUG_FIND_ME                                                          1
#define DEBUG_APPLICATION                                                      1
#define DEBUG_PHONE_ALERT                                                      1
#define DEBUG_BLOOD_PRESSURE_SENSOR                                            1
#define DEBUG_HEALTH_THERMOMETER                                               1
#define DEBUG_ALERT_NOTIFICATION_CLIENT                                        1
#define DEBUG_ALERT_NOTIFICATION_SERVER                                        1
#define DEBUG_GLUCOSE_SENSOR                                                   1
#define DEBUG_SCAN_PARAMETERS                                                  1
#define DEBUG_ENVIRONMENTAL_SENSING                                            1
#define DEBUG_BATTERY                                                          1
#define DEBUG_DEVICE_INFORMATION                                               1
#define DEBUG_DATA_TRANSFER                                                    1
#define DEBUG_WEIGHT_SCALE                                                     1
#define DEBUG_LOCATION_NAVIGATION                                              1
#define DEBUG_IMMEDIATE_ALERT_SERVER                                           1
#define DEBUG_IMMEDIATE_ALERT_CLIENT                                           1
#define DEBUG_LINK_LOSS_SERVER                                                 1
#define DEBUG_LINK_LOSS_CLIENT                                                 1
#define DEBUG_TX_POWER_SERVER                                                  1
#define DEBUG_TX_POWER_CLIENT                                                  1
#define DEBUG_HTTP_PROXY                                                       1
#endif

/******************************************************************************
 * Macros
 *****************************************************************************/
/**
 * Debug Macros
 */
#if (CFG_PLATFORM_TYPE == CFG_PLATFORM_ARM)
#if (CFG_DEBUG_TRACE == 1)
#define DBG_PRINT(...)                                DBG_TRACE_DBG(__VA_ARGS__)
#define PRINT_LOG_MESG_DBG(...)                       DBG_TRACE_DBG(__VA_ARGS__)
#define PRINT_LOG_BUFF_DBG(...)                      DbgTraceBuffer(__VA_ARGS__)
#define PRINT_MESG_DBG(...)                           DBG_TRACE_DBG(__VA_ARGS__)
#define DBG_ASSERT(expression)                         DBG_TRACE_DBG(expression)
#define PRINT_MESG_DATA(file,dptr,dlen)                                        \
    do {                                                                       \
      int i=0;                                                                 \
      for (i=0;i<dlen;i++)                                                     \
      {                                                                        \
        if (i%16 == 0)                                                         \
        {                                                                      \
          DBG_TRACE_DBG ("\n[%s]:[%d]:[--DATA--]: ", __FILE__, __LINE__);      \
        }                                                                      \
        DBG_TRACE_DBG ("%02X ", dptr[i]);                                      \
      }                                                                        \
      printf ("\n ");                                                          \
    }while(0)

#define PRINT_MESG_INFO(...)                          DBG_TRACE_DBG(__VA_ARGS__)
#define PRINT_MESG_ERR(...)                           DBG_TRACE_DBG(__VA_ARGS__)
#else
#define PRINT_LOG_MESG_DBG(...)
#define PRINT_LOG_BUFF_DBG(...)
#define DBG_PRINT(...)
#define PRINT_MESG_DBG(...)
#define DBG_ASSERT(expression)
#define PRINT_MESG_DBG(...)
#define PRINT_MESG_DATA(file,dptr,dlen)
#define PRINT_MESG_INFO(...)
#define PRINT_MESG_ERR(...)
#define PRINT_NO_MESG(...)
#endif
#else
#define PRINT_LOG_MESG_DBG(...)
#define PRINT_LOG_BUFF_DBG(...)
#define DBG_PRINT(...)
#define PRINT_MESG_DBG(...)
#define DBG_ASSERT(expression)
#define PRINT_MESG_DBG(...)
#define PRINT_MESG_DATA(file,dptr,dlen)
#define PRINT_MESG_INFO(...)
#define PRINT_MESG_ERR(...)
#define PRINT_NO_MESG(...)
#endif

/*
 * debug messages: XXXX_MESG_DBG()
 *                 print only if debug flag for corresponding module is set,
 *
 * Error messages: XXXX_MESG_ERR()
 *                 print always to log any type of error in execution,
 *                 these errors are error in control execution, but not the
 *                 s/w logic or implementation error.
 *
 * HEX Data dump: XXXX_MESG_DATA()
 *                print messages for hex data dump must be a debug messages,
 *                as data dump is a bulkey process and not required always for
 *                info or error, its required only for debugging purpose.
 */

#if (DEBUG_HCI != 0)
#define HCI_MESG_DBG                                              PRINT_MESG_DBG
#define HCI_MESG_DATA                                            PRINT_MESG_DATA
#else
#define HCI_MESG_DBG                                               PRINT_NO_MESG
#define HCI_MESG_DATA                                              PRINT_NO_MESG
#endif

#if (DEBUG_TIMER != 0)
#define TIMER_MESG_DBG                                           PRINT_MESG_DBG
#define TIMER_MESG_DATA                                          PRINT_MESG_DATA
#else
#define TIMER_MESG_DBG                                             PRINT_NO_MESG
#define TIMER_MESG_DATA                                            PRINT_NO_MESG
#endif

#if (DEBUG_PROFILE != 0)
#define PROFILE_MESG_DBG                                          PRINT_MESG_DBG
#else
#define PROFILE_MESG_DBG                                           PRINT_NO_MESG
#endif
#define PROFILE_MESG_ERR                                          PRINT_MESG_ERR

#if (DEBUG_HRPROFILE != 0)
#define HRPROFILE_MESG_DBG                                        PRINT_MESG_DBG
#define HRPROFILE_MESG_ERR                                        PRINT_MESG_ERR
#else
#define HRPROFILE_MESG_DBG                                         PRINT_NO_MESG
#define HRPROFILE_MESG_ERR                                         PRINT_NO_MESG
#endif

#if (DEBUG_PROXIMITY_REPORTER != 0)
#define PROXIMITY_REPORTER_MESG_DBG                               PRINT_MESG_DBG
#else
#define PROXIMITY_REPORTER_MESG_DBG                                PRINT_NO_MESG
#endif
#define PROXIMITY_REPORTER_MESG_ERR                               PRINT_MESG_ERR

#if (DEBUG_PROXIMITY_MONITOR != 0)
#define PROXIMITY_MONITOR_MESG_DBG                                PRINT_MESG_DBG
#define PROXIMITY_MONITOR_MESG_DATA                              PRINT_MESG_DATA
#else
#define PROXIMITY_MONITOR_MESG_DBG                                 PRINT_NO_MESG
#define PROXIMITY_MONITOR_MESG_DATA                                PRINT_NO_MESG
#endif
#define PROXIMITY_MONITOR_MESG_ERR                                PRINT_MESG_ERR

#if (DEBUG_CURRENT_TIME_SERVER != 0)
#define CURRENT_TIME_SERVER_MESG_DBG                              PRINT_MESG_DBG
#define CURRENT_TIME_SERVER_MESG_DATA                            PRINT_MESG_DATA
#else
#define CURRENT_TIME_SERVER_MESG_DBG                               PRINT_NO_MESG
#define CURRENT_TIME_SERVER_MESG_DATA                              PRINT_NO_MESG
#endif
#define CURRENT_TIME_SERVER_MESG_ERR                              PRINT_MESG_ERR

#if (DEBUG_CURRENT_TIME_CLIENT != 0)
#define CURRENT_TIME_CLIENT_MESG_DBG                              PRINT_MESG_DBG
#define CURRENT_TIME_CLIENT_MESG_DATA                            PRINT_MESG_DATA
#else
#define CURRENT_TIME_CLIENT_MESG_DBG                               PRINT_NO_MESG
#define CURRENT_TIME_CLIENT_MESG_DATA                              PRINT_NO_MESG
#endif
#define CURRENT_TIME_CLIENT_MESG_ERR                              PRINT_MESG_ERR

#if (DEBUG_HID_DEVICE_SERVER != 0)
#define HIDS_DEVICE_MESG_DBG                                       PRINT_MESG_DBG
#define HIDS_DEVICE_MESG_DATA                                     PRINT_MESG_DATA
#else
#define HIDS_DEVICE_MESG_DBG                                        PRINT_NO_MESG
#define HIDS_DEVICE_MESG_DATA                                       PRINT_NO_MESG
#endif
#define HIDS_DEVICE_MESG_ERR                                       PRINT_MESG_ERR

#if (DEBUG_HID_DEVICE_CLIENT != 0)
#define HIDC_DBG_MSG                                              PRINT_MESG_DBG
#else
#define HIDC_DBG_MSG                                               PRINT_NO_MESG
#endif

#if (DEBUG_FIND_ME != 0)
#define FMT_DBG_MSG                                               PRINT_MESG_DBG
#else
#define FMT_DBG_MSG                                                PRINT_NO_MESG
#endif

#if (DEBUG_PHONE_ALERT != 0)
#define PAC_DBG_MSG                                               PRINT_MESG_DBG
#else
#define PAC_DBG_MSG                                                PRINT_NO_MESG
#endif

#if (DEBUG_BLOOD_PRESSURE_SENSOR != 0)
#define BPS_DBG_MSG                                               PRINT_MESG_DBG
#else
#define BPS_DBG_MSG                                                PRINT_NO_MESG
#endif

#if (DEBUG_HEALTH_THERMOMETER != 0)
#define HT_DBG_MSG                                                PRINT_MESG_DBG
#else
#define HT_DBG_MSG                                                 PRINT_NO_MESG
#endif

#if (DEBUG_ALERT_NOTIFICATION_CLIENT != 0)
#define ANC_DBG_MSG                                               PRINT_MESG_DBG
#define ANC_DBG_DATA                                             PRINT_MESG_DATA
#else
#define ANC_DBG_MSG                                                PRINT_NO_MESG
#define ANC_DBG_DATA                                               PRINT_NO_MESG
#endif

#if (DEBUG_ALERT_NOTIFICATION_SERVER != 0)
#define ANS_DBG_MSG                                               PRINT_MESG_DBG
#define ANS_DBG_DATA                                             PRINT_MESG_DATA
#else
#define ANS_DBG_MSG                                                PRINT_NO_MESG
#define ANS_DBG_DATA                                               PRINT_NO_MESG
#endif

#if (DEBUG_GLUCOSE_SENSOR != 0)
#define GL_DBG_MSG                                                PRINT_MESG_DBG
#else
#define GL_DBG_MSG                                                 PRINT_NO_MESG
#endif

#if (DEBUG_ENVIRONMENTAL_SENSING != 0)
#define ES_DBG_MSG                                                PRINT_MESG_DBG
#else
#define ES_DBG_MSG                                                 PRINT_NO_MESG
#endif

#if (DEBUG_BATTERY != 0)
#define BS_DBG_MSG                                                PRINT_MESG_DBG
#else
#define BS_DBG_MSG                                                 PRINT_NO_MESG
#endif

#if (DEBUG_APPLICATION != 0)
#define APPL_MESG_DBG                                             PRINT_MESG_DBG
#define APPL_MESG_DATA                                           PRINT_MESG_DATA
#else
#define APPL_MESG_DBG                                              PRINT_NO_MESG
#define APPL_MESG_DATA                                             PRINT_NO_MESG
#endif

#if (DEBUG_SCAN_PARAMETERS != 0)
#define SPPROFILE_MESG_DBG                                        PRINT_MESG_DBG
#define SPPROFILE_MESG_ERR                                        PRINT_MESG_ERR
#else
#define SPPROFILE_MESG_DBG                                         PRINT_NO_MESG
#define SPPROFILE_MESG_ERR                                         PRINT_NO_MESG
#endif

#if (DEBUG_DEVICE_INFORMATION != 0)
#define DIS_DBG_MSG                                               PRINT_MESG_DBG
#else
#define DIS_DBG_MSG                                                PRINT_NO_MESG
#endif

#if (DEBUG_DATA_TRANSFER != 0)
#define DTS_DBG_MSG                                               PRINT_MESG_DBG
#define DTS_LOG_MSG                                           PRINT_LOG_MESG_DBG
#define DTS_LOG_BUFFER                                        PRINT_LOG_BUFF_DBG
#else
#define DTS_DBG_MSG                                                PRINT_NO_MESG
#define DTS_LOG_MSG                                                PRINT_NO_MESG
#define DTS_LOG_BUFFER                                             PRINT_NO_MESG
#endif

#if (DEBUG_WEIGHT_SCALE != 0)
#define WSS_DBG_MSG                                               PRINT_MESG_DBG
#else
#define WSS_DBG_MSG                                                PRINT_NO_MESG
#endif

#if (DEBUG_LOCATION_NAVIGATION != 0)
#define LNS_DBG_MSG                                               PRINT_MESG_DBG
#else
#define LNS_DBG_MSG                                                PRINT_NO_MESG
#endif

#if (DEBUG_IMMEDIATE_ALERT_SERVER != 0)
#define IAS_DBG_MSG                                               PRINT_MESG_DBG
#else
#define IAS_DBG_MSG                                                PRINT_NO_MESG
#endif

#if (DEBUG_IMMEDIATE_ALERT_CLIENT != 0)
#define IAC_DBG_MSG                                               PRINT_MESG_DBG
#else
#define IAC_DBG_MSG                                                PRINT_NO_MESG
#endif

#if (DEBUG_LINK_LOSS_SERVER != 0)
#define LLS_DBG_MSG                                               PRINT_MESG_DBG
#else
#define LLS_DBG_MSG                                                PRINT_NO_MESG
#endif

#if (DEBUG_LINK_LOSS_CLIENT != 0)
#define LLC_DBG_MSG                                               PRINT_MESG_DBG
#else
#define LLC_DBG_MSG                                                PRINT_NO_MESG
#endif

#if (DEBUG_TX_POWER_SERVER != 0)
#define TPS_DBG_MSG                                               PRINT_MESG_DBG
#else
#define TPS_DBG_MSG                                                PRINT_NO_MESG
#endif

#if (DEBUG_TX_POWER_CLIENT != 0)
#define TPC_DBG_MSG                                               PRINT_MESG_DBG
#else
#define TPC_DBG_MSG                                                PRINT_NO_MESG
#endif

#if (DEBUG_HTTP_PROXY != 0)
#define HTTPS_DBG_MSG                                               PRINT_MESG_DBG
#else
#define HTTPS_DBG_MSG                                                PRINT_NO_MESG
#endif

#endif /* _DEBUG_H_ */
