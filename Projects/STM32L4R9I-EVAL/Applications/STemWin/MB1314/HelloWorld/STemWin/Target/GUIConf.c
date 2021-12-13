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

