/**
  ******************************************************************************
  * @file    STemWin/STemWin_HelloWorld/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
