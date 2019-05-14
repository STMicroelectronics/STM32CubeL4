
/**
 ******************************************************************************
 * @file    lb_client_app.h
 * @author  MCD Application Team
 * @brief   Header for lb_client_app.c module
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
