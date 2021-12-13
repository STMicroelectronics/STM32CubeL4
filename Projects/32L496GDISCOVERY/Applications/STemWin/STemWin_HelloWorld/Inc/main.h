/**
  ******************************************************************************
  * @file    STemWin/STemWin_HelloWorld/Inc/main.h 
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
#include "stm32l4xx_hal.h"
#include "GUI.h"

/* Board includes component */
#include "stm32l496g_discovery.h"
#include "stm32l496g_discovery_lcd.h"
void LCD_LL_Init(void);
void Error_Handler(void);
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define LAYER_SIZE_X 			240
#define LAYER_SIZE_Y			160
#define LAYER_BYTE_PER_PIXEL	2 /* RGB565 format */

#endif /* __MAIN_H */

