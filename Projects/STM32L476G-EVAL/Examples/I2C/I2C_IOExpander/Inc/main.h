/**
  ******************************************************************************
  * @file    I2C/I2C_IOExpander/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

