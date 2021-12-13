/**
  ******************************************************************************
  * @file    audioplayer_app.h
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
/** @addtogroup AUDIO_PLAYER_MODULE
  * @{
  */

/** @defgroup AUDIO_APPLICATION
  * @brief audio application routines
  * @{
  */

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "..\Common\audio_if.h"

/* Exported constants --------------------------------------------------------*/
#define USE_GRAPHIC_EQ
#if defined( USE_GRAPHIC_EQ )
#include "audio_fw_glo.h"
#include "greq_glo.h"
#include "biq_glo.h"
#endif

   
#define MUTE_OFF                      0x00
#define MUTE_ON                       0x01
#define DEFAULT_AUDIO_VOLUME          50

#if defined( USE_GRAPHIC_EQ )   
   
/* Equalizer Band Configuration */
#define AUDIOPLAYER_LOUD_BKP      RTC_BKP_DR0
#define AUDIOPLAYER_EQU1_BKP      RTC_BKP_DR1
#define AUDIOPLAYER_EQU2_BKP      RTC_BKP_DR2
#define AUDIOPLAYER_EQU3_BKP      RTC_BKP_DR3
#define AUDIOPLAYER_EQU4_BKP      RTC_BKP_DR4
#define AUDIOPLAYER_EQU5_BKP      RTC_BKP_DR5
#define AUDIOPLAYER_EQU6_BKP      RTC_BKP_DR6
#define AUDIOPLAYER_EQU7_BKP      RTC_BKP_DR7
#define AUDIOPLAYER_EQU8_BKP      RTC_BKP_DR8
#define AUDIOPLAYER_EQU9_BKP      RTC_BKP_DR9
#define AUDIOPLAYER_EQU10_BKP     RTC_BKP_DR10
   

#define CALIBRATION_AUDIOPLAYER_SETTING_BKP   RTC_BKP_DR11
#define CALIBRATION_AUDIOPLAYER_EQU1_BKP      RTC_BKP_DR12
#define CALIBRATION_AUDIOPLAYER_EQU2_BKP      RTC_BKP_DR13
#define CALIBRATION_AUDIOPLAYER_EQU3_BKP      RTC_BKP_DR14
#define CALIBRATION_AUDIOPLAYER_EQU4_BKP      RTC_BKP_DR15
#define CALIBRATION_AUDIOPLAYER_EQU5_BKP      RTC_BKP_DR16
#define CALIBRATION_AUDIOPLAYER_LOUD_BKP      RTC_BKP_DR17
   
   
#define NB_BIQUAD_SEC_1   1
#define NB_BIQUAD_SEC_2   2
#define NB_BIQUAD_SEC_3   3
#define NB_BIQUAD_SEC_5   5
#define NB_BIQUAD_SEC_9   9

#define BIQ_FILT_LOUDNESS  2
   
#define EQ_MAX_GAIN_DB                 20   
#endif

typedef enum
{
  AUDIOPLAYER_ERROR_NONE = 0, 
  AUDIOPLAYER_ERROR_IO,
  AUDIOPLAYER_ERROR_HW, 
  AUDIOPLAYER_ERROR_MEM,
#if defined( USE_GRAPHIC_EQ )
  AUDIO_ERROR_GREQ,
  AUDIO_ERROR_BIQ,
#endif
  AUDIOPLAYER_ERROR_FORMAT_NOTSUPPORTED
} AUDIOPLAYER_ErrorTypdef;

#define AUDIOPLAYER_StateTypdef  OUT_StateTypdef
/* Exported types ------------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

#define EQUI_DB_TO_PERC(x)       (((int16_t)(x + EQ_MAX_GAIN_DB) * 100)/40)

/* Exported functions ------------------------------------------------------- */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Init(uint8_t volume, uint8_t use_graphic_eq, uint8_t use_loudness);
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

#if defined( USE_GRAPHIC_EQ )
void    AUDIOPLAYER_SetEqGain(uint8_t BandNum, int16_t NewGainValue);
void    AUDIOPLAYER_SetLoudness(void);
int16_t AUDIOPLAYER_GetEqGain(uint8_t BandNum);
#endif

#ifdef __cplusplus
}
#endif

/**
  * @}
  */

/**
  * @}
  */

#endif /*__AUDIO_PLAYER_APP_H */

