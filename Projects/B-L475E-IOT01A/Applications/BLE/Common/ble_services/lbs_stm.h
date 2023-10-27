
/**
  ******************************************************************************
  * @file    lbs_stm.h
  * @author  MCD Application Team
  * @brief   Header for led button module
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
#ifndef __STM32XX_LED_BUTTON_H
#define __STM32XX_LED_BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

 
  
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  BUTTON_NOTIFY_ENABLED_EVT,
  BUTTON_NOTIFY_DISABLED_EVT,
  ST_SVC_LED_READ_EVT,
  ST_SVC_LED_WRITE_EVT,
} LBS_Opcode_evt_t;

typedef struct
{
  uint8_t * pPayload;
  uint8_t     Length;
}LBS_Data_t;  

typedef struct
{
  LBS_Opcode_evt_t                              LBS_Evt_Opcode;
  LBS_Data_t                                    DataTransfered;
  uint16_t                                      ConnectionHandle;
  uint8_t                                       ServiceInstance;
}LBS_App_Notification_evt_t;


/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LBS_STM_Init(void);
void LBS_App_Notification(LBS_App_Notification_evt_t *pNotification);
tBleStatus BLE_SVC_LedButton_Update_Char(uint16_t UUID,  uint8_t *pPayload);


#ifdef __cplusplus
}
#endif

#endif /*__STM32XX_LBS_H */

