/**
  ******************************************************************************
  * @file    reset_res.h
  * @author  MCD Application Team
  * @brief   reset resources
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

#ifndef GUI_RESET_RESOURCES_H__
#define GUI_RESET_RESOURCES_H__

/*********************************************************************
*
*       Includes
*
**********************************************************************
*/
#include <stdlib.h>

#include "GUI.h"

/*********************************************************************
*
*       Exported constants
*
**********************************************************************
*/
#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

/*********************************************************************
*
*       Exported variables
*
**********************************************************************
*/

/*** Bitmaps ***/

/*** Genereted bitmaps used for the reset module ***/

extern GUI_CONST_STORAGE GUI_BITMAP bmreset_icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmreset_button;

#endif  /* Avoid multiple inclusion */

