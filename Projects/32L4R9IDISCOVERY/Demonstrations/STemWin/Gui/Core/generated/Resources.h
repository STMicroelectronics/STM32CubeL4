/**
  ******************************************************************************
  * @file    Resources.h
  * @author  MCD Application Team
  * @brief   common resources
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

#ifndef GUI_COMMON_RESOURCES_H__
#define GUI_COMMON_RESOURCES_H__

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

/*** Genereted bitmaps used by all modules ***/

extern GUI_CONST_STORAGE GUI_BITMAP bmbackground;
extern GUI_CONST_STORAGE GUI_BITMAP bmround_background;
extern GUI_CONST_STORAGE GUI_BITMAP bmreflection_cover;
extern GUI_CONST_STORAGE GUI_BITMAP bmbutton_back;
extern GUI_CONST_STORAGE GUI_BITMAP bmback_button_bkg;
extern GUI_CONST_STORAGE GUI_BITMAP bmmodule_entry_icon;

/*** Genereted Fonts used by all modules ***/
extern GUI_CONST_STORAGE GUI_FONT GUI_FontLucidaSansUnicode25;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontRoboto_C_Regular35;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontRoboto_C_regular100;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontRoboto_Light31;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontRoboto_Light100;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontRobotoC_bold22;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontRobotoC_regular22;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontRobotoCn_bold27;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontRobotoCondensed_light25;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontRobotoRegular50;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontRobotoCnBold48;

#endif  /* Avoid multiple inclusion */
