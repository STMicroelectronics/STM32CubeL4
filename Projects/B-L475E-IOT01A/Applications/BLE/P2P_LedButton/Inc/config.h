/**
 ******************************************************************************
 * @file    config.h
 * @author  MCD Application Team
 * @brief   Application configuration file
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
#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hw.h"
#include "bluenrg_gap.h"

/**
 * This setting shall be set to 0 when BlueNRG is used as a coprocessor
 * It shall be set to 1 within a BLE SoC.
 */
#define CFG_BLE_HCI_STDBY   0

/******************************************************************************
 *
 * APP CONFIG
 *
 ******************************************************************************/

/******************************************************************************
 * LB CONFIGURATION //Routeur or End Dev 1 or End Dev 2 
 ******************************************************************************/

#define LB_CLIENT  0    /*1 = Device is GAP Central & GATT Client */
#define LB_SERVER  1    /*1 = Device is GAP Peripherique & GATT SERVER*/


#if (LB_CLIENT == 1)  

#define LEDBUTTON_CONN_ADV_INTERVAL_MIN  (0x80)  /**< 20ms */
#define LEDBUTTON_CONN_ADV_INTERVAL_MAX  (0xA0)  /**< 30ms */

#define CFG_ADV_BD_ADDRESS 0xEDDE11EDDE11

#define REMOTE_BD_ADDRESS 0xAA22334455AA   

#define CONN_L(x)      ((int)((x)/0.625f))
#define CONN_P(x)      ((int)((x)/1.25f))
#define SCAN_P         (0x20)
#define SCAN_L         (0x30)    
#define CONN_P1        (CONN_P(20)) 
#define CONN_P2        (CONN_P(50)) 
#define SUPERV_TIMEOUT (400)
#define CONN_L1        (CONN_L(10)) 
#define CONN_L2        (CONN_L(10)) 

   
//#define GATT_SERVER 1     /*1 = Device support GATT Server */
//#define GATT_CLIENT 0     /*1 = Device support GATT Client */  
/**
 * This setting shall be set to '1' if the device needs to support the Peripheral Role
 * In the MS configuration, both BLE_CFG_PERIPHERAL and BLE_CFG_CENTRAL shall be set to '1'
 */
#define BLE_CFG_PERIPHERAL                                                    0

/**
 * This setting shall be set to '1' if the device needs to support the Central Role
 * In the MS configuration, both BLE_CFG_PERIPHERAL and BLE_CFG_CENTRAL shall be set to '1'
 */
#define BLE_CFG_CENTRAL                                                       1

#endif

 
#if (LB_SERVER == 1)

#define LEDBUTTON_CONN_ADV_INTERVAL_MIN  (0x1F4)  /**< 312.5ms */
#define LEDBUTTON_CONN_ADV_INTERVAL_MAX  (0x640)  /**< 1000ms */


#define CFG_ADV_BD_ADDRESS 0xAA22334455AA 

/**
 * This setting shall be set to '1' if the device needs to support the Peripheral Role
 * In the MS configuration, both BLE_CFG_PERIPHERAL and CFG_CENTRAL may be set to '1'
 */
#define BLE_CFG_PERIPHERAL  1

/**
 * This setting shall be set to '1' if the device needs to support the Central Role
 * In the MS configuration, both BLE_CFG_PERIPHERAL and CFG_CENTRAL may be set to '1'
 */
#define BLE_CFG_CENTRAL     0


#endif //LB_CLIENT

#define BLE_CFG_DATA_ROLE_MODE          4
#define BLE_CFG_MAX_CONNECTION          8

/**< IO capabilities  */
#define CFG_IO_CAPABILITY_DISPLAY_ONLY       (0x00)
#define CFG_IO_CAPABILITY_DISPLAY_YES_NO     (0x01)
#define CFG_IO_CAPABILITY_KEYBOARD_ONLY      (0x02)
#define CFG_IO_CAPABILITY_NO_INPUT_NO_OUTPUT (0x03)
#define CFG_IO_CAPABILITY_KEYBOARD_DISPLAY   (0x04)

#define CFG_LBR_IO_CAPABILITY                  CFG_IO_CAPABILITY_DISPLAY_ONLY

/**< MITM modes  */
#define CFG_MITM_PROTECTION_NOT_REQUIRED      (0x00)
#define CFG_MITM_PROTECTION_REQUIRED          (0x01)

#define CFG_LBR_MITM_PROTECTION                CFG_MITM_PROTECTION_NOT_REQUIRED


/******************************************************************************
 * Low Power
 ******************************************************************************/
/**
 *  When set to 1, the low power mode is enable
 *  When set to 0, the device stays in RUN mode
 */
#define CFG_LPM_SUPPORTED   1

/**
 * This setting shall be set to 0 when BlueNRG is used as a coprocessor
 * It shall be set to 1 within a BLE SoC.
 */
#define CFG_HCI_STDBY   0

/******************************************************************************
 * Timer Server
 ******************************************************************************/
/** tick timer value is us */
#define CFG_TS_TICK_VAL                                                63

/**
 * Calculated for RTCCLK coming from LSI (37Khz)
 * Define how much is divided the RTCCLK
 * Should be either 0,2,4,8,16
 *
 * Larger tick value is providing better power consumption
 */
#define CFG_RTCCLK_DIVIDER_CONF 0

/**
 * Timer tick is 54us
 * When the 1Hz calendar clock is not needed, CFG_RTC_SYNCH_PRESCALER should be set to 0x7FFF (MAX VALUE)
 * When the RTC digital calibration is not used, CFG_RTC_ASYNCH_PRESCALER should be set to 1
 */
#if (CFG_RTCCLK_DIVIDER_CONF == 2)
#define CFG_RTC_WUCKSEL_DIVIDER (3)
#define CFG_RTC_ASYNCH_PRESCALER (3)
#define CFG_RTC_SYNCH_PRESCALER (0x2422)
/**
 * Timer tick is 108us
 * When the 1Hz calendar clock is not needed, CFG_RTC_SYNCH_PRESCALER should be set to 0xFFFF
 */
#elif (CFG_RTCCLK_DIVIDER_CONF == 4)
#define CFG_RTC_WUCKSEL_DIVIDER (2)
#define CFG_RTC_ASYNCH_PRESCALER (3)
#define CFG_RTC_SYNCH_PRESCALER (0x2422)
/**
 * Timer tick is 216us
 * When the 1Hz calendar clock is not needed, CFG_RTC_SYNCH_PRESCALER should be set to 0xFFFF
 */
#elif (CFG_RTCCLK_DIVIDER_CONF == 8)
#define CFG_RTC_WUCKSEL_DIVIDER (1) /**< Divided by 8 */
#define CFG_RTC_ASYNCH_PRESCALER (7)
#define CFG_RTC_SYNCH_PRESCALER (0x1211)
/**
 * Timer tick is 432us
 * When the 1Hz calendar clock is not needed, CFG_RTC_SYNCH_PRESCALER should be set to 0xFFFF
 */
#elif (CFG_RTCCLK_DIVIDER_CONF == 16)
#define CFG_RTC_WUCKSEL_DIVIDER (0)
#define CFG_RTC_ASYNCH_PRESCALER (15)
#define CFG_RTC_SYNCH_PRESCALER (0x0908)
/**
 * Custom configuration
 */
#else
#define CFG_RTC_WUCKSEL_DIVIDER (3)
#define CFG_RTC_ASYNCH_PRESCALER (1)
#define CFG_RTC_SYNCH_PRESCALER (0x7FFF)
#endif

/**
 * Low Power Mode configuration
 * Only one the the three configuration shall be enabled
 *
 * Note1: The MSI configuration is not accurate enough to use the Debug Traces
 * over UART.
 *
 * Note2: For most application, the best power consumption is obtained when the SPI clock connected to the BlueNRG-MS
 * is set to its maximum speed 8Mhz.
 * The power consumption is better with MSI@16Mhz and SPI@8Mhz versus MSI@24Mhz and SPI@6Mhz.
 */
#define HSI_WITH_PLL                                                           0
#define HSI_WITHOUT_PLL                                                        0
#define MSI_WITHOUT_PLL                                                        1


typedef enum
{
  CFG_TimProcID_isr,
} CFG_TimProcID_t;

/**
 * This the minimum of time before the next event of the timersver
 * required to enter OFF mode
 * The time is counted in ticks
 */
/**< 20ms */
#define TIMERSERVER_OFF_MODE_THRESHOLD                                (20000/CFG_TS_TICK_VAL)

/******************************************************************************
 * Debug
 ******************************************************************************/
/**
 * keep debugger enabled while in any low power mode when set to 1
 * should be set to 0 in production
 */
#define CFG_DEBUGGER_SUPPORTED    1

/* Define platform used: only ARM supported for TRACE trace */
#define CFG_PLATFORM_LINUX        (0x01)
#define CFG_PLATFORM_WINDOWS      (0x02)
#define CFG_PLATFORM_ARM          (0x03)

#define CFG_PLATFORM_TYPE         (CFG_PLATFORM_ARM)

/**
 * When set, this resets some hw resources to set the device in the same state than the power up
 * The FW resets only register that may prevent the FW to run properly
 *
 * This shall be set to 0 in a final product
 *
 */
#define CFG_HW_RESET_BY_FW         1

/******************************************************************************
 *
 * BLE SERVICES CONFIGURATION
 *
 ******************************************************************************/
/**
 * There is one handler per service enabled
 * Note: There is no handler for the Device Information Service
 *
 * This shall take into account all registered handlers
 * (from either the provided services or the custom services)
 */
#define BLE_CFG_SVC_MAX_NBR_CB      2

#define BLE_CFG_CLT_MAX_NBR_CB      2

/******************************************************************************
 * Device Information Service (DIS)
 ******************************************************************************/
/**< Number of characteristic added by the user */
#define BLE_CFG_DIS_NUMBER_OF_CUSTOM_CHAR                                     0

/**< Options: Supported(1) or Not Supported(0) */
#define BLE_CFG_DIS_MANUFACTURER_NAME_STRING                                  1
#define BLE_CFG_DIS_MODEL_NUMBER_STRING                                       0
#define BLE_CFG_DIS_SERIAL_NUMBER_STRING                                      0
#define BLE_CFG_DIS_HARDWARE_REVISION_STRING                                  0
#define BLE_CFG_DIS_FIRMWARE_REVISION_STRING                                  0
#define BLE_CFG_DIS_SOFTWARE_REVISION_STRING                                  0
#define BLE_CFG_DIS_SYSTEM_ID                                                 0
#define BLE_CFG_DIS_IEEE_CERTIFICATION                                        0
#define BLE_CFG_DIS_PNP_ID                                                    0

/**
 * device information service characteristic lengths
 */
#define BLE_CFG_DIS_SYSTEM_ID_LEN_MAX                                                   (8)
#define BLE_CFG_DIS_MODEL_NUMBER_STRING_LEN_MAX                                         (32)
#define BLE_CFG_DIS_SERIAL_NUMBER_STRING_LEN_MAX                                        (32)
#define BLE_CFG_DIS_FIRMWARE_REVISION_STRING_LEN_MAX                                    (32)
#define BLE_CFG_DIS_HARDWARE_REVISION_STRING_LEN_MAX                                    (32)
#define BLE_CFG_DIS_SOFTWARE_REVISION_STRING_LEN_MAX                                    (32)
#define BLE_CFG_DIS_MANUFACTURER_NAME_STRING_LEN_MAX                                    (32)
#define BLE_CFG_DIS_IEEE_CERTIFICATION_LEN_MAX                                          (32)
#define BLE_CFG_DIS_PNP_ID_LEN_MAX                                                      (7)

/******************************************************************************
 * Heart Rate Service (HRS)
 ******************************************************************************/
#define BLE_CFG_HRS_NUMBER_OF_CUSTOM_CHAR    0 /**< Number of characteristic added by the user */
#define BLE_CFG_HRS_BODY_SENSOR_LOCATION_CHAR           1 /**< BODY SENSOR LOCATION CHARACTERISTIC */
#define BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG           1 /**< ENERGY EXTENDED INFO FLAG */
#define BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG             1 /**< Max number of RR interval values - Shall not be greater than 9 */


/******************************************************************************
 *
 * UTILITIES CONFIGURATION
 *
 ******************************************************************************/

/******************************************************************************
 * Scheduler
 ******************************************************************************/

/**
 * This is a bit mapping over 32bits listing all tasks id supported in the application
 */
typedef enum
{
  CFG_IdleTask_TlEvt,
  CFG_IdleTask_StartAdv,                                            //!< CFG_IdleTask_StartAdv
  CFG_IdleTask_ConnDev1,                                            //!< CFG_IdleTask_ConnDev1
  CFG_IdleTask_SearchService,                                       //!< CFG_IdleTask_SearchService
  CFG_IdleTask_Button,                                            //!< CFG_IdleTask_Button
  CFG_IdleTask_HciAsynchEvt,                                       //!< CFG_IdleTask_HciAsynchEvt
  CFG_TASK_NBR,                   /**< Shall be last in the list *///!< CFG_TASK_NBR
} CFG_IdleTask_Id_t;

#define CFG_SCH_TASK_NBR  CFG_TASK_NBR


/**
 * This is a bit mapping over 32bits listing all events id supported in the application
 */
typedef enum
{
  CFG_IdleEvt_HciCmdEvtResp,
} CFG_IdleEvt_Id_t;


/******************************************************************************
 * LOW POWER MANAGER
 *****************************************************************************/
/**
 * Supported requester to the MCU Low Power Manager - can be increased up  to 32
 * It lits a bit mapping of all user of the Low Power Manager
 */
typedef enum
{
  LPM_SPI_TX_Id,
  LPM_SPI_RX_Id,
  CFG_LPM_HCI_CmdEvt,
  CFG_LPM_HCI_AsynchEvt,
  CFG_LPM_App,
  CFG_LPM_TS,
} CFG_LPM_Id_t;

/**
 * Stop Mode configuration
 */
typedef enum
{
  CFG_StopMode1_MR = LL_PWR_MODE_STOP0,
  CFG_StopMode1_LPR = LL_PWR_MODE_STOP1,
  CFG_StopMode2 = LL_PWR_MODE_STOP2
} CFG_StopModeConf_t;

/**
 * OFF Mode configuration
 */
typedef enum
{
  CFG_Standby = LL_PWR_MODE_STANDBY,
  CFG_Shutdown = LL_PWR_MODE_SHUTDOWN
} CFG_OffModeConf_t;

/******************************************************************************
 *
 * TLBLE CONFIGURATION
 *
 ******************************************************************************/
/**
 * Queue length of BLE Event
 * This parameter defines the number of asynchronous events that can be stored in the HCI layer before
 * being reported to the application. When a command is sent to the BlueNRG coprocessor, the HCI layer
 * is waiting for the event with the Num_HCI_Command_Packets set to 1. The receive queue shall be large
 * enough to store all asynchronous events received in between.
 * When CFG_TLBLE_MOST_EVENT_PAYLOAD_SIZE is set to 27, this allow to store three 255 bytes long asynchronous events
 * between the HCI command and its event.
 * This parameter depends on the value given to CFG_TLBLE_MOST_EVENT_PAYLOAD_SIZE. When the queue size is to small,
 * the system may hang if the queue is full with asynchronous events and the HCI layer is still waiting
 * for a CC/CS event, In that case, the notification TLHCI_ToNot() is called to indicate
 * to the application a HCI command did not receive its command event within 30s (Default HCI Timeout).
 */
#define CFG_TLBLE_EVT_QUEUE_LENGTH 5

/**
 * This parameter should be set to fit most events received by the HCI layer. It defines the buffer size of each element
 * allocated in the queue of received events and can be used to optimize the amount of RAM allocated by the Memory Manager.
 * It should not exceed 255 which is the maximum HCI packet payload size (a greater value is a lost of memory as it will
 * never be used)
 * It shall be at least 4 to receive the command status event in one frame.
 * The default value is set to 27 to allow receiving an event of MTU size in a single buffer. This value maybe reduced
 * further depending on the application.
 *
 */
#define CFG_TLBLE_MOST_EVENT_PAYLOAD_SIZE 255   /**< Set to 255 with the memory manager and the mailbox */

/******************************************************************************
 *
 * HARDWARE CONFIGURATION
 *
 ******************************************************************************/

/******************************************************************************
 * TIMER SERVER
 *****************************************************************************/
/**
 * The user may define the maximum number of virtual timers supported.
 * It shall not exceed 255
 */
#define CFG_HW_TS_MAX_NBR_CONCURRENT_TIMER  6

/**
 * The user may define the priority in the NVIC of the RTC_WKUP interrupt handler that is used to manage the
 * wakeup timer.
 * This setting is the preemptpriority part of the NVIC.
 */
#define CFG_HW_TS_NVIC_RTC_WAKEUP_IT_PREEMPTPRIO   3

/**
 * The user may define the priority in the NVIC of the RTC_WKUP interrupt handler that is used to manage the
 * wakeup timer.
 * This setting is the subpriority part of the NVIC. It does not exist on all processors. When it is not supported
 * on the CPU, the setting is ignored
 */
#define CFG_HW_TS_NVIC_RTC_WAKEUP_IT_SUBPRIO   0

/**
 *  Define a critical section in the Timer server
 *  The Timer server does not support the API to be nested
 *  The  Application shall either:
 *    a) Ensure this will never happen
 *    b) Define the critical section
 *  The default implementations is masking all interrupts using the PRIMASK bit
 *  The TimerServer driver uses critical sections to avoid context corruption. This is achieved with the macro
 *  TIMER_ENTER_CRITICAL_SECTION and TIMER_EXIT_CRITICAL_SECTION. When CFG_HW_TS_USE_PRIMASK_AS_CRITICAL_SECTION is set
 *  to 1, all STM32 interrupts are masked with the PRIMASK bit of the CortexM CPU. It is possible to use the BASEPRI
 *  register of the CortexM CPU to keep allowed some interrupts with high priority. In that case, the user shall
 *  re-implement TIMER_ENTER_CRITICAL_SECTION and TIMER_EXIT_CRITICAL_SECTION and shall make sure that no TimerServer
 *  API are called when the TIMER critical section is entered
 */
#define CFG_HW_TS_USE_PRIMASK_AS_CRITICAL_SECTION 1

/**
   * This value shall reflect the maximum delay there could be in the application between the time the RTC interrupt
   * is generated by the Hardware and the time when the  RTC interrupt handler is called. This time is measured in
   * number of CFG_RTC_SYNCH_PRESCALER ticks.
   * A relax timing would be 1ms
   * When the value is too short, the timerserver will not be able to count properly and all timeout may be random.
   * When the value is too long, the device may wake up more often than the most optimal configuration. However, the
   * impact on power consumption would be marginal (unless the value selected is extremely too long). It is strongly
   * recommended to select a value large enough to make sure it is not too short to ensure reliability of the system
   * as this will have marginal impact on low power mode
   * The value below are calculated for RTCCLK coming from LSI (32Khz)
   */
#define CFG_HW_TS_RTC_HANDLER_MAX_DELAY 16  /**< 1ms x 32Khz / (CFG_RTC_ASYNCH_PRESCALER+1) */

  /**
   * Interrupt ID in the NVIC of the RTC Wakeup interrupt handler
   * It shall be type of IRQn_Type
   */
#define CFG_HW_TS_RTC_WAKEUP_HANDLER_ID  RTC_WKUP_IRQn

  /**
   * Calculated for RTCCLK coming from LSI (37Khz)
   * Define how much is divided the RTCCLK
   * Should be either 0,2,4,8,16
   *
   * Larger tick value is providing better power consumption
   */
#define CFG_RTCCLK_DIVIDER_CONF 0

  /**
   * Timer tick is 54us
   * When the 1Hz calendar clock is not needed, CFG_RTC_SYNCH_PRESCALER should be set to 0x7FFF (MAX VALUE)
   * When the RTC digital calibration is not used, CFG_RTC_ASYNCH_PRESCALER should be set to 1
   */
#if (CFG_RTCCLK_DIVIDER_CONF == 2)
#define CFG_RTC_WUCKSEL_DIVIDER (3)
#define CFG_RTC_ASYNCH_PRESCALER (3)
#define CFG_RTC_SYNCH_PRESCALER (0x2422)
  /**
   * Timer tick is 108us
   * When the 1Hz calendar clock is not needed, CFG_RTC_SYNCH_PRESCALER should be set to 0xFFFF
   */
#elif (CFG_RTCCLK_DIVIDER_CONF == 4)
#define CFG_RTC_WUCKSEL_DIVIDER (2)
#define CFG_RTC_ASYNCH_PRESCALER (3)
#define CFG_RTC_SYNCH_PRESCALER (0x2422)
  /**
   * Timer tick is 216us
   * When the 1Hz calendar clock is not needed, CFG_RTC_SYNCH_PRESCALER should be set to 0xFFFF
   */
#elif (CFG_RTCCLK_DIVIDER_CONF == 8)
#define CFG_RTC_WUCKSEL_DIVIDER (1) /**< Divided by 8 */
#define CFG_RTC_ASYNCH_PRESCALER (7)
#define CFG_RTC_SYNCH_PRESCALER (0x1211)
  /**
   * Timer tick is 432us
   * When the 1Hz calendar clock is not needed, CFG_RTC_SYNCH_PRESCALER should be set to 0xFFFF
   */
#elif (CFG_RTCCLK_DIVIDER_CONF == 16)
#define CFG_RTC_WUCKSEL_DIVIDER (0)
#define CFG_RTC_ASYNCH_PRESCALER (15)
#define CFG_RTC_SYNCH_PRESCALER (0x0908)
  /**
   * Custom configuration
   */
#else
#define CFG_RTC_WUCKSEL_DIVIDER (3)
#define CFG_RTC_ASYNCH_PRESCALER (1)
#define CFG_RTC_SYNCH_PRESCALER (0x7FFF)
#endif


/******************************************************************************
 * SPI and other timing defines
 *****************************************************************************/
 
  /**
   * When the STM32xx runs too fast, there could be a false detection on the SPI_IRQ after a packet has been received.
   * To prevent this, a timeout is added to wait after receiving a packet from the BlueNRG before checking the SPI_IRQ.
   * When the fix is not implemented, the system is still working fine but it would result in some additional
   * communication over SPI which may increase the power consumption.
   */
#define SPI_END_RECEIVE_FIX             1

  /**
   * All timeout below are given in term of Ticks of the TimerServer.
   * The tick of the Timer Server depends on the LPO clock used as input and the
   * prescaler of the wakeup timer.
   * In order to get the most efficient implementation, the wakeuptimer prescaler should divide
   * the input clock by 2
   *
   * The values below are based on the LSI clock @37Kz.
   * For this, the tick is 54us
   */

  /**
   * It takes some time for the BlueNRG to wakeup upon request from the application before sending data.
   * To optimize the power consumption, a timeout is implemented to wait for the BlueNRG to wakeup.
   * The timeout prevents the application to send packet continuously to the BlueNRG as all these packets will be
   * rejected until it wakes up. The BlueNRG wakeup time depends on its activity. The timeout is calculated in number
   * of Timer Server tick. The default configuration is suitable to cover all use cases.
   */
#define SPI_TX_TIMEOUT                  6

  /**
   * This is the timeout related to the SPI_END_RECEIVE_FIX definition. The timeout is calculated in number of the
   * Timer Server tick. The worst case should be taken to cover all STM32 MCU families as it does not hurt if the timeout
   * is slightly longer than the minimum required. There should be no need to change the default value.
   */
#define SPI_END_RECEIVE_FIX_TIMEOUT     2

  /**
   * This setting shall be set to the size of the SPI FIFO RX. When there is no FIFO (STM32L0, …), it should be set to 1.
   * It is recommended to set it to the longest value implemented across STM32 MCU family to avoid error when porting
   * the FW across MCUs. A longer value than the minimum required has no impact on the system
   * Default configuration is 4 (Longest FIFO size known at this time – STM32L4).
   *
   */
#define SPI_FIFO_RX_DEPTH               4

   /**
   * This setting shall be set to the requirement of the BlueNRG device which is 1.5ms The timeout is calculated in
   * number of Timer Server tick. The worst case should be taken to cover all STM32 MCU families as it does not hurt
   * if the timeout is slightly longer than the minimum required. There should be no need to change the default value.
   */
#define BLUENRG_HOLD_TIME_IN_RESET      28

  /**
   * The CS shall be at least 625ns long. This setting shall be set to the number of cycles required to reach that value.
   * As there is almost no impact on the system, the highest value should be selected to cover all CPU speed used in
   * the application.
   * Default configuration is 52 (CPU @ 84Mhz – Max speed on STM32L4). *
   */
#define CS_PULSE_625NS_NBR_CYCLES_REQ   52



/******************************************************************************
 * HARDWARE CONFIGURATION
 ******************************************************************************/

/******************************************************************************
 * UART
 *****************************************************************************/
#define CFG_HW_UART1_SUPPORTED      1
#define CFG_HW_UART2      0

/**
 * UART1
 */

#define CFG_HW_UART1_PREEMPTPRIORITY      0x0F
#define CFG_HW_UART1_SUBPRIORITY          0

#define CFG_HW_UART1_BAUDRATE             115200
#define CFG_HW_UART1_WORDLENGTH           UART_WORDLENGTH_8B
#define CFG_HW_UART1_STOPBITS             UART_STOPBITS_1
#define CFG_HW_UART1_PARITY               UART_PARITY_NONE
#define CFG_HW_UART1_HWFLOWCTL            UART_HWCONTROL_NONE
#define CFG_HW_UART1_MODE                 UART_MODE_TX_RX
#define CFG_HW_UART1_ADVFEATUREINIT       UART_ADVFEATURE_NO_INIT

#define CFG_HW_UART1_TX_PORT_CLK_ENABLE   __HAL_RCC_GPIOB_CLK_ENABLE
#define CFG_HW_UART1_TX_PORT              GPIOB
#define CFG_HW_UART1_TX_PIN               GPIO_PIN_6
#define CFG_HW_UART1_TX_MODE              GPIO_MODE_AF_PP
#define CFG_HW_UART1_TX_PULL              GPIO_NOPULL
#define CFG_HW_UART1_TX_SPEED             GPIO_SPEED_HIGH
#define CFG_HW_UART1_TX_ALTERNATE         GPIO_AF7_USART1_TX

#define CFG_HW_UART1_RX_PORT_CLK_ENABLE   __HAL_RCC_GPIOA_CLK_ENABLE
#define CFG_HW_UART1_RX_PORT              GPIOA
#define CFG_HW_UART1_RX_PIN               GPIO_PIN_10
#define CFG_HW_UART1_RX_MODE              GPIO_MODE_AF_PP
#define CFG_HW_UART1_RX_PULL              GPIO_NOPULL
#define CFG_HW_UART1_RX_SPEED             GPIO_SPEED_HIGH
#define CFG_HW_UART1_RX_ALTERNATE         GPIO_AF7_USART1_RX


/* Exported macros -----------------------------------------------------------*/
/* Compiler specific definition ----------------------------------------------*/
/*
 * Make sure the function (or variable) marked is part of the object file
 */
#if defined ( __CC_ARM )
#define USED                                               __attribute__((used))
#elif defined ( __ICCARM__ )
#define USED                                                              __root
#elif defined ( __GNUC__ )
#define USED                                               __attribute__((used))
#endif

#ifdef __cplusplus
}
#endif

#endif /*__CONFIG_H */

