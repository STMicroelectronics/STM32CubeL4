/**
  ******************************************************************************
  * @file    ts.h
  * @author  MCD Application Team
  * @brief   Header for ts.c file
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
#ifndef __TOUCH_C_H
#define __TOUCH_C_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TouchInit(void);
void TouchDeInit(void);
void TouchDetectInt(void);

#ifdef __cplusplus
}
#endif

#endif /*__TOUCH_C_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
