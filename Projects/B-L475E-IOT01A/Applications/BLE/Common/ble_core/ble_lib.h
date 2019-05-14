/**
 ******************************************************************************
 * @file    ble_lib.h
 * @author  MCD Application Team
 * @brief   BLE interface
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
#ifndef __BLE_H
#define __BLE_H

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ------------------------------------------------------------------*/
#include "uuid.h"
#include "ble_status.h"
#include "link_layer.h"
#include "hci_const.h"
#include "bluenrg_aci_const.h"

#include "bluenrg_gap_aci.h"
#include "bluenrg_gap.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_hal_aci.h"
#include "bluenrg_l2cap_aci.h"
#include "bluenrg_gatt_server.h"
#include "hci_le.h"

  /* Exported types ------------------------------------------------------------*/
  /* Exported constants --------------------------------------------------------*/
  /* External variables --------------------------------------------------------*/
  /* Exported macros -----------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /*__BLE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
