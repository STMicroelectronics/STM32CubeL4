/**
  ******************************************************************************
  * @file    BSP/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "string.h"
#include "stm32l4xx_hal.h"
#include "stm32l476g_discovery.h"
#include "stm32l476g_discovery_gyroscope.h"
#include "stm32l476g_discovery_idd.h"
#include "stm32l476g_discovery_glass_lcd.h"
#include "stm32l476g_discovery_audio.h"
#include "stm32l476g_discovery_qspi.h"

/* Exported constants --------------------------------------------------------*/
#define DEMO_NAME_CHAR_NB         20

/* Defines for the Audio playing process */
#define PAUSE_STATUS              ((uint32_t)0x00) /* Audio Player in Pause Status */
#define RESUME_STATUS             ((uint32_t)0x01) /* Audio Player in Resume Status */
#define IDLE_STATUS               ((uint32_t)0x02) /* Audio Player in Idle Status */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  IDD_RUN       = 0x00,
  IDD_SLEEP     = 0x01,
  IDD_LPR_2MHZ  = 0x02,
  IDD_LPR_SLEEP = 0x03,
  IDD_STOP2     = 0x04,
  IDD_STANDBY   = 0x05,
  IDD_SHUTDOWN  = 0x06,
  IDD_TEST_NB   = 0x07,
} Idd_StateTypeDef;

typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[DEMO_NAME_CHAR_NB];
  uint32_t DemoIndex;
}BSP_DemoTypedef;

typedef enum {
  AUDIO_DEMO_NONE = 0,
  AUDIO_DEMO_PLAYBACK,
  AUDIO_DEMO_RECORD
} Audio_DemoTypeDef;

/* Exported variables --------------------------------------------------------*/
/* Variable indicating which audio demo is currently running (playback v.s. record) */
extern Audio_DemoTypeDef AudioDemo;

/* Flag indicating that audio playback must be paused or resumed */
extern __IO uint32_t PauseResumeStatus;

/* Flag indicating that audio playback must be exited */
extern __IO uint8_t  AudioPlaybackExit;

/* Flag indicating that audio playback volume level must be changed */
extern __IO uint8_t  VolumeChange;

/* Actual audio playback volume level */
extern __IO uint8_t  Volume;

/* Flag indicating that audio record must be exited */
extern __IO uint8_t  AudioRecordExit;

/* Flag indicating that record sample rate has been selected */
extern __IO uint8_t  AudioRecordSampleRateSelected;  

/* Flag indicating that record sample rate must be changed changed */
extern __IO uint8_t AudioRecordSampleRateChange;

/* Index within SamplesRates[] */
extern __IO uint8_t SampleRateIndex;

/* Exported macro ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)       (sizeof(x)/sizeof(BSP_DemoTypedef))
#define ABS(x)                    (x < 0) ? (-x) : x

/* Exported functions ------------------------------------------------------- */
void AudioPlay_demo(void);
void AudioRecord_demo(void);
void Idd_demo(void);
void Gyro_demo(void);
void Joystick_demo (void);
void LCDGlass_demo(void);
void Led_demo(void);
void QSPI_demo(void);
uint8_t CheckForUserInput(void);
void      SystemClock_Config(void);
void      SystemLowClock_Config(void);
void      SystemHardwareInit(void);
void      SystemHardwareDeInit(void);
uint32_t  SystemRtcBackupRead(void);
void      SystemRtcBackupWrite(uint32_t SaveIndex);
void      fibonacci_flash(int n);

void Error_Handler(void);
void Convert_IntegerIntoChar(uint32_t number, uint16_t *p_tab);

#endif /* __MAIN_H */

