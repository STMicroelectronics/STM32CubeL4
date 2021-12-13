/**
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

/* Includes ------------------------------------------------------------------*/
#include "GUI.h"

/* Functions ------------------------------------------------------------------*/
void MainTask(void) 
{
  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  GUI_SetFont(GUI_FONT_24B_1);
  GUI_DispStringHCenterAt("Hello world!", LCD_GetXSize()/2, (LCD_GetYSize()-16)/2);  
}

