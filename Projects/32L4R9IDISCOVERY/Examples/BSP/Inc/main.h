/**
  ******************************************************************************
  * @file    main.h
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
#include "stm32l4r9i_discovery.h"
#include "stm32l4r9i_discovery_io.h"
#include "stm32l4r9i_discovery_audio.h"
#include "stm32l4r9i_discovery_lcd.h"
#include "stm32l4r9i_discovery_ts.h"
#include "stm32l4r9i_discovery_sd.h"
#include "stm32l4r9i_discovery_camera.h"
#include "stm32l4r9i_discovery_ospi_nor.h"
#include "stm32l4r9i_discovery_idd.h"
#include "stm32l4r9i_discovery_psram.h"

/* Macros --------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[50];
  uint32_t DemoIndex;
}BSP_DemoTypedef;

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

/* Exported variables --------------------------------------------------------*/
extern const unsigned char stlogo[];
extern __IO FlagStatus MfxItOccurred;

/* Exported constants --------------------------------------------------------*/

/* Defines for the Audio playing process */
#define PAUSE_STATUS     ((uint32_t)0x00) /* Audio Player in Pause Status */
#define RESUME_STATUS    ((uint32_t)0x01) /* Audio Player in Resume Status */
#define IDLE_STATUS      ((uint32_t)0x02) /* Audio Player in Idle Status */

#define AUDIO_PLAY_SAMPLE        0
#define AUDIO_PLAY_RECORDED      1

#if defined(LCD_DIMMING)
#define DIMMING_COUNTDOWN       7U     /* Countdown timer until screen is dimmed (in seconds) */
#define BRIGHTNESS_MAX          0xFFU  /* LCD brightness maximum value                        */
#define BRIGHTNESS_MIN          0x44U  /* LCD brightness minimum value (screen dimmed)        */
#if defined(LCD_OFF_AFTER_DIMMING)
#define LCD_OFF_COUNTDOWN       2U     /* Number of RTC wake-up timer counter expirations while
                                          the screen is dimmed before turning off the LCD     */
#endif /* defined(LCD_OFF_AFTER_DIMMING) */
#endif /* defined(LCD_DIMMING) */

/* Exported macro ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))

/* Exported functions ------------------------------------------------------- */
void Led_demo(void);
void LCD_demo(void);
void Joystick_demo (void);
void AudioPlay_demo(void);
void AudioRecord_demo(void);
void Touchscreen_demo(void);
void SD_demo(void);
void Camera_demo(void);
void OSPI_NOR_demo(void);
void Idd_demo(void);
void PSRAM_demo (void);

void SystemClock_Config(void);
void SystemLowClock_Config(void);
void SystemClock_ConfigFromLowPower(void);

void SystemHardwareInit(void);
void SystemHardwareDeInit(void);
uint32_t SystemRtcBackupRead(void);
void SystemRtcBackupWrite(uint32_t SaveIndex);

void Convert_IntegerIntoChar(uint32_t number, uint16_t *p_tab);

void Mfx_Event(void);
void Toggle_Leds(void);
void Error_Handler(void);

#if defined(LCD_DIMMING)
void Dimming_Timer_Enable(RTC_HandleTypeDef * RTCHandle);
#endif

#endif /* __MAIN_H */

