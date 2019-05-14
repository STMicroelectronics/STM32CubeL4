/**
  ******************************************************************************
  * @file    audio_player_res.h
  * @author  MCD Application Team
  * @brief   audio player resources
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

#ifndef GUI_AUDIO_PLAYER_RESOURCES_H__
#define GUI_AUDIO_PLAYER_RESOURCES_H__

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

/*** Genereted bitmaps used for the audio player and the equilizer modules ***/

extern GUI_CONST_STORAGE GUI_BITMAP bmaudio_player_splash;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_background;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_equalizer_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_equalizer_unpressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_minus_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_minus_unpressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_next_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_next_unpressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_pause_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_pause_unpressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_play_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_play_unpressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_plus_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_plus_unpressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_previous_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_previous_unpressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_repeat_all_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_repeat_all_unpressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_repeat_one_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_repeat_one_unpressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_sound_mute;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_speaker_transparent;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon_speaker_white;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_icon;

/* Volume and Playback Progress bar */
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_pb_playback_full;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_pb_playback_niddle;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_pb_volume_bkg;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_pb_volume_empty;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_pb_volume_full;

/* Equalizer */
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_eq_button_equalizer;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_eq_progress_bar_cercle;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_eq_progress_bar_niddle;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_eq_separation;
extern GUI_CONST_STORAGE GUI_BITMAP bmplayer_eq_shape;

#endif  /* Avoid multiple inclusion */
/*************************** End of file ****************************/

