/**
  ******************************************************************************
  * @file    power_measurement_res.h
  * @author  MCD Application Team
  * @brief   power measurement resources
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

#ifndef GUI_POWER_MEASUREMENT_RESOURCES_H__
#define GUI_POWER_MEASUREMENT_RESOURCES_H__

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

/*** Genereted bitmaps used for the power measurement module ***/

/* Genereted bitmaps used for the power_measurement module */

/* Background */
extern GUI_CONST_STORAGE GUI_BITMAP bmpower_measurement_background;
extern GUI_CONST_STORAGE GUI_BITMAP bmpower_icon;

/* Buttons */
extern GUI_CONST_STORAGE GUI_BITMAP bmbutton_run_selected;
extern GUI_CONST_STORAGE GUI_BITMAP bmbutton_run_unselected;
extern GUI_CONST_STORAGE GUI_BITMAP bmbutton_run_cercle;
extern GUI_CONST_STORAGE GUI_BITMAP bmarrow_left;
extern GUI_CONST_STORAGE GUI_BITMAP bmarrow_left_gray;
extern GUI_CONST_STORAGE GUI_BITMAP bmarrow_right;
extern GUI_CONST_STORAGE GUI_BITMAP bmarrow_right_gray;

/* Modes */
extern GUI_CONST_STORAGE GUI_BITMAP bmlow_power_mode;
extern GUI_CONST_STORAGE GUI_BITMAP bmlow_power_run_mode_text;
extern GUI_CONST_STORAGE GUI_BITMAP bmlow_power_sleep_mode;
extern GUI_CONST_STORAGE GUI_BITMAP bmlow_power_sleep_mode_text;
extern GUI_CONST_STORAGE GUI_BITMAP bmrun_mode;
extern GUI_CONST_STORAGE GUI_BITMAP bmrun_mode_text;
extern GUI_CONST_STORAGE GUI_BITMAP bmshutdown;
extern GUI_CONST_STORAGE GUI_BITMAP bmshutdown_mode_text;
extern GUI_CONST_STORAGE GUI_BITMAP bmshutdown_mode_rtc_text;
extern GUI_CONST_STORAGE GUI_BITMAP bmsleep_mode;
extern GUI_CONST_STORAGE GUI_BITMAP bmsleep_mode_text;
extern GUI_CONST_STORAGE GUI_BITMAP bmstandby_mode;
extern GUI_CONST_STORAGE GUI_BITMAP bmstandby_mode_text;
extern GUI_CONST_STORAGE GUI_BITMAP bmstandby_mode_rtc_text;
extern GUI_CONST_STORAGE GUI_BITMAP bmstop_mode;
extern GUI_CONST_STORAGE GUI_BITMAP bmstop_2_mode_text;

/* Animations */
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle1_opacity10;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle1_opacity20;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle1_opacity30;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle1_opacity40;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle1_opacity50;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle1_opacity60;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle1_opacity70;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle1_opacity80;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle1_opacity90;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle1_opacity100;

extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle2_opacity10;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle2_opacity20;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle2_opacity30;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle2_opacity40;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle2_opacity50;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle2_opacity60;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle2_opacity70;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle2_opacity80;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle2_opacity90;
extern GUI_CONST_STORAGE GUI_BITMAP bmanimation_circle2_opacity100;

#endif  /* Avoid multiple inclusion */

