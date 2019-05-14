/**
  ******************************************************************************
  * @file    audio_recorder_res.h
  * @author  MCD Application Team
  * @brief   audio recorder ressources
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

#ifndef __AUDIO_RECORDER_RES_H
#define __AUDIO_RECORDER_RES_H

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

/*** Genereted bitmaps used for the audio recorder module ***/

/* Analog Clock 1 */
extern GUI_CONST_STORAGE GUI_BITMAP bmcancel_button_selected;
extern GUI_CONST_STORAGE GUI_BITMAP bmcancel_button_unselected;
extern GUI_CONST_STORAGE GUI_BITMAP bmmenu_button;
extern GUI_CONST_STORAGE GUI_BITMAP bmmicro_icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmpause_button_selected;
extern GUI_CONST_STORAGE GUI_BITMAP bmpause_button_unselected;
extern GUI_CONST_STORAGE GUI_BITMAP bmrecord_button_unselected;
extern GUI_CONST_STORAGE GUI_BITMAP bmrecord_time_shape;
extern GUI_CONST_STORAGE GUI_BITMAP bmrecording_cercle1;
extern GUI_CONST_STORAGE GUI_BITMAP bmrecording_cercle2;
extern GUI_CONST_STORAGE GUI_BITMAP bmrecording_cercle3;
extern GUI_CONST_STORAGE GUI_BITMAP bmrecording_cercle4;
extern GUI_CONST_STORAGE GUI_BITMAP bmsave_button_selected;
extern GUI_CONST_STORAGE GUI_BITMAP bmsave_button_unselected;
extern GUI_CONST_STORAGE GUI_BITMAP *bmanimation_graph[];
extern GUI_CONST_STORAGE GUI_BITMAP bmgraphright;
extern GUI_CONST_STORAGE GUI_BITMAP bmgraphleft;
extern GUI_CONST_STORAGE GUI_BITMAP bmrecord_info;

#endif  /* __AUDIO_RECORDER_RES_H */
/*************************** End of file ****************************/

