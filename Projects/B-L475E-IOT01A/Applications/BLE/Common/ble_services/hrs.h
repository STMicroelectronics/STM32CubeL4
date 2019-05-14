/**
  ******************************************************************************
  * @file    hrs.h
  * @author  MCD Application Team
  * @brief   Header for stm32xx_heartrate.c module
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
#ifndef __HRS_H
#define __HRS_H

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ------------------------------------------------------------------*/
  /* Exported types ------------------------------------------------------------*/
  typedef enum
  {
    HRS_HRM_VALUE_FORMAT_UINT16 = 1,
    HRS_HRM_SENSOR_CONTACT_PRESENT = 2,
    HRS_HRM_SENSOR_CONTACT_SUPPORTED = 4,
    HRS_HRM_ENERGY_EXPENDED_PRESENT = 8,
    HRS_HRM_RR_INTERVAL_PRESENT = 0x10
  } HRS_HrmFlags_t;

  typedef enum
  {
    HRS_BODY_SENSOR_LOCATION_OTHER = 0,
    HRS_BODY_SENSOR_LOCATION_CHEST = 1,
    HRS_BODY_SENSOR_LOCATION_WRIST = 2,
    HRS_BODY_SENSOR_LOCATION_FINGER = 3,
    HRS_BODY_SENSOR_LOCATION_HAND = 4,
    HRS_BODY_SENSOR_LOCATION_EAR_LOBE = 5,
    HRS_BODY_SENSOR_LOCATION_FOOT = 6
  } HRS_BodySensorLocation_t;

  typedef enum
  {
    HRS_RESET_ENERGY_EXPENDED_EVT,
    HRS_NOTIFICATION_ENABLED,
    HRS_NOTIFICATION_DISABLED,
  } HRS_NotCode_t;

  typedef struct{
    uint16_t    MeasurementValue;
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG == 1)
    uint16_t    EnergyExpended;
#endif
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG != 0)
    uint16_t    aRRIntervalValues[BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG + BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG];
    uint8_t     NbreOfValidRRIntervalValues;
#endif
    uint8_t     Flags;
  }HRS_MeasVal_t;

  /* Exported constants --------------------------------------------------------*/
  /* External variables --------------------------------------------------------*/
  /* Exported macros -----------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
  void HRS_Init(void);
  tBleStatus HRS_UpdateChar(uint16_t uuid, uint8_t *p_payload);
  void HRS_Notification(HRS_NotCode_t notification);

#ifdef __cplusplus
}
#endif

#endif /*__HRS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
