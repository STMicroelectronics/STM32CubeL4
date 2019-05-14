/**
  ******************************************************************************
  * @file    I2C/I2C_IOExpander/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l476g_eval.h"
#include "stm32l476g_eval_ts.h"
#include "stm32l476g_eval_io.h"
#include "stm32l476g_eval_lcd.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define POLLING_MODE
/* #define INTERRUPT_MODE */

/* #define BUTTON_POLLING_MODE */
#define BUTTON_INTERRUPT_MODE

#ifdef BUTTON_POLLING_MODE
#define BUTTON_MODE  BUTTON_MODE_GPIO
#else
#define BUTTON_MODE  BUTTON_MODE_EXTI
#endif  /* BUTTON_POLLING_MODE */

#define MESSAGE1   "    STM32L476G-EVAL         "
#define MESSAGE2   "Example on how to use     "
#define MESSAGE3   " the TS & IO modules      "

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
