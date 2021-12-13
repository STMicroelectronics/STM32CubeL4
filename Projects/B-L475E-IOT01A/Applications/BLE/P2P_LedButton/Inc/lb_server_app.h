/**
 ******************************************************************************
 * @file    lb_server_app.h
 * @author  MCD Application Team
 * @brief   Header for lb_server_app.c module
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
#ifndef __LB_SERVER_APP_H
#define __LB_SERVER_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
  
void LB_App_Button_Trigger_Received(void);
void BLE_App_LedButton_EndDevice_Init(void);

#ifdef __cplusplus
}
#endif

#endif /*__LB_SERVER_APP_H */

