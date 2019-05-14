/**
  ******************************************************************************
  * @file    stm32l4r9i_eval_rgb_ts.h
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32l4r9i_eval_rgb_ts.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L4R9I_EVAL_RGB_TS_H
#define __STM32L4R9I_EVAL_RGB_TS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4r9i_eval.h"
/* Include touch screen FT5336 component Driver */
#include "../Components/ft5336/ft5336.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L4R9I_EVAL
  * @{
  */

/** @defgroup STM32L4R9I_EVAL_TS STM32L4R9I_EVAL TS
  * @{
  */

/** @defgroup STM32L4R9I_EVAL_RGB_TS STM32L4R9I_EVAL RGB TS
  * @{
  */

/** @defgroup STM32L4R9I_EVAL_RGB_TS_Imported_Variables Imported Variables
  * @{
  */
/**
 *  @brief Table for touchscreen event information display on LCD :
 *  table indexed on enum @ref TS_TouchEventTypeDef information
 */
extern char * ts_event_string_tab[TOUCH_EVENT_NB_MAX];

/**
 *  @brief Table for touchscreen gesture Id information display on LCD : table indexed
 *  on enum @ref TS_GestureIdTypeDef information
 */
extern char * ts_gesture_id_string_tab[GEST_ID_NB_MAX];
/**
  * @}
  */

/** @addtogroup STM32L4R9I_EVAL_RGB_TS_Exported_Functions
  * @{
  */
uint8_t BSP_RGB_TS_Init(uint16_t ts_SizeX, uint16_t ts_SizeY);
uint8_t BSP_RGB_TS_DeInit(void);
uint8_t BSP_RGB_TS_GetState(TS_StateTypeDef *TS_State);

#if (TS_MULTI_TOUCH_SUPPORTED == 1)
uint8_t BSP_RGB_TS_Get_GestureId(TS_StateTypeDef *TS_State);
#endif /* TS_MULTI_TOUCH_SUPPORTED == 1 */

uint8_t BSP_RGB_TS_ITConfig(void);
uint8_t BSP_RGB_TS_ITGetStatus(void);
void    BSP_RGB_TS_ITClear(void);
uint8_t BSP_RGB_TS_ResetTouchData(TS_StateTypeDef *TS_State);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32L4R9I_EVAL_RGB_TS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
