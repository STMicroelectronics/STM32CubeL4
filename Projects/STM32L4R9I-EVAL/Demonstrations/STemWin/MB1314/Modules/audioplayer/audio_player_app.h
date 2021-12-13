/**
  ******************************************************************************
  * @file    audio_player_app.h
  * @author  MCD Application Team 
  * @brief   header of audio player application file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_PLAYER_APP_H
#define __AUDIO_PLAYER_APP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "../common/audio_if.h"   
   
/* Exported constants --------------------------------------------------------*/
#undef USE_SPIRIT_EQ
#if !defined(__GNUC__) && defined( USE_SPIRIT_EQ )
#include "Addons\SpiritDSP_Equalizer\spiritEQ.h"
#include "Addons\SpiritDSP_LoudnessControl\spiritLdCtrl.h"
#endif

#define MUTE_OFF                      0x00
#define MUTE_ON                       0x01
#define DEFAULT_AUDIO_VOLUME          50

#define AUDIOPLAYER_EQU1_BKP      RTC_BKP_DR0
#define AUDIOPLAYER_EQU2_BKP      RTC_BKP_DR1
#define AUDIOPLAYER_EQU3_BKP      RTC_BKP_DR2
#define AUDIOPLAYER_EQU4_BKP      RTC_BKP_DR3
#define AUDIOPLAYER_LOUD_BKP      RTC_BKP_DR4

#if !defined(__GNUC__) && defined( USE_SPIRIT_EQ )   
   
/* Equalizer Band Configuration */
#define SET_BAND_PRMS(band, _fltType, _centerHz, _widthHz) \
  (band)->fltType = _fltType; \
  (band)->centerHz = _centerHz; \
  (band)->widthHz = _widthHz;
#endif
typedef enum
{
  AUDIOPLAYER_ERROR_NONE = 0, 
  AUDIOPLAYER_ERROR_IO,
  AUDIOPLAYER_ERROR_HW, 
  AUDIOPLAYER_ERROR_MEM, 
  AUDIOPLAYER_ERROR_FORMAT_NOTSUPPORTED,   
  
}AUDIOPLAYER_ErrorTypdef;

#define AUDIOPLAYER_StateTypdef  OUT_StateTypdef
/* Exported types ------------------------------------------------------------*/


/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Init(uint8_t volume, uint8_t use_spirit_eq);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Play(uint32_t frequency, uint32_t nbrChannels);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Stop(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Pause(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Resume(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Mute(uint8_t state);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_GetFileInfo(char* file, WAV_InfoTypedef* info);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SelectFile(char* file);
AUDIOPLAYER_StateTypdef  AUDIOPLAYER_GetState(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Process(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_DeInit(void);
uint32_t                 AUDIOPLAYER_GetProgress (void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SetVolume(uint32_t volume);
uint32_t                 AUDIOPLAYER_GetVolume(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_NotifyEndOfFile(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_NotifyError(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SetPosition(uint32_t position);

#if !defined(__GNUC__) && defined( USE_SPIRIT_EQ )
void    AUDIOPLAYER_SetEqGain(uint8_t BandNum, int16_t NewGainValue);
void    AUDIOPLAYER_SetLoudnessGain(int16_t NewGainValue);
int16_t AUDIOPLAYER_GetLoudnessGain(void);
int16_t AUDIOPLAYER_GetEqGain(uint8_t BandNum);
#endif

#ifdef __cplusplus
}
#endif

#endif /*__AUDIO_PLAYER_APP_H */

