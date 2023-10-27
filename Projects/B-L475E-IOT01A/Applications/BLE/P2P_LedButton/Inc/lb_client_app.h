
/**
 ******************************************************************************
 * @file    lb_client_app.h
 * @author  MCD Application Team
 * @brief   Header for lb_client_app.c module
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
#ifndef __LB_CLIENT_APP_H
#define __LB_CLIENT_APP_H

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ------------------------------------------------------------------*/
  /* Exported types ------------------------------------------------------------*/
  /* Exported constants --------------------------------------------------------*/
  /* External variables --------------------------------------------------------*/
  /* Exported macros -----------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
  void BLE_App_dtc_Init(void);
  void BLE_App_dtc_Send_Data(void);
  void BLE_App_dtc_Profile_Update(void);
  void BLE_App_End_Device_Mgt_Connection_Update(void);

#ifdef __cplusplus
}
#endif

#endif /*__LB_CLIENT_APP_H */

