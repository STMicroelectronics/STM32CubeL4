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
#include "GUI.h"

/* Private defines -----------------------------------------------------------*/
/* Define the available number of bytes available for the GUI */
#define GUI_NUMBYTES  (80 * 1024)
U32 aMemory[GUI_NUMBYTES / 4];

/* Functions -----------------------------------------------------------------*/
/**
  * @brief  Memory config needed by STemWin
  * @param  None
  * @retval None
  */
void GUI_X_Config(void)
{
  /* Assign memory to emWin */
  GUI_ALLOC_AssignMemory(aMemory, GUI_NUMBYTES);
  /* Set default font */
  GUI_SetDefaultFont(GUI_FONT_6X8);
}

/*************************** End of file ****************************/
