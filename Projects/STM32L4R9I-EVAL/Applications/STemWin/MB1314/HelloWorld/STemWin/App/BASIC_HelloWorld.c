/**
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

/*************************** End of file ****************************/
