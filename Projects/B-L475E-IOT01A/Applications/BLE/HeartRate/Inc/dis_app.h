
/**
 ******************************************************************************
 * @file    dis_app.h
 * @author  MCD Application Team
 * @brief   Header for dis_app.c module
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
#ifndef __DIS_APP_H
#define __DIS_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define DISAPP_MANUFACTURER_NAME              "STM"
#define DISAPP_MODEL_NUMBER                   "4502-1.0"
#define DISAPP_SERIAL_NUMBER                  "1.0"
#define DISAPP_HARDWARE_REVISION_NUMBER       "1.0"
#define DISAPP_FIRMWARE_REVISION_NUMBER       "1.0"
#define DISAPP_SOFTWARE_REVISION_NUMBER       "1.0"
#define DISAPP_OUI                            0x123456
#define DISAPP_MANUFACTURER_ID                0x9ABCDE


/* Exported functions ------------------------------------------------------- */
void DISAPP_Init(void);


#ifdef __cplusplus
}
#endif

#endif /*__DIS_APP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
