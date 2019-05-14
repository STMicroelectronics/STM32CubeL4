/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32l4r9i_eval.h"
#include "stm32l4r9i_eval_io.h"
#include "stm32l4r9i_eval_audio.h"
#include "stm32l4r9i_eval_eeprom.h"
#include "stm32l4r9i_eval_idd.h"
#include "stm32l4r9i_eval_lcd.h"
#include "stm32l4r9i_eval_nor.h"
#include "stm32l4r9i_eval_ospi_nor.h"
#include "stm32l4r9i_eval_ospi_ram.h"
#include "stm32l4r9i_eval_sd.h"
#include "stm32l4r9i_eval_sram.h"
#include "stm32l4r9i_eval_ts.h"

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

#if defined(LCD_DIMMING)
#define DIMMING_COUNTDOWN       7U     /* Countdown timer until screen is dimmed (in seconds) */
#define BRIGHTNESS_MAX          0xFFU  /* LCD brightness maximum value                        */
#define BRIGHTNESS_MIN          0x44U  /* LCD brightness minimum value (screen dimmed)        */
#if defined(LCD_OFF_AFTER_DIMMING)
#define LCD_OFF_COUNTDOWN       2U     /* Number of RTC wake-up timer counter expirations while
                                          the screen is dimmed before turning off the LCD     */
#endif /* defined(LCD_OFF_AFTER_DIMMING) */
#endif /* defined(LCD_DIMMING) */

/* Defines for the Audio playing process */
#define PAUSE_STATUS     ((uint32_t)0x00) /* Audio Player in Pause Status */
#define RESUME_STATUS    ((uint32_t)0x01) /* Audio Player in Resume Status */
#define IDLE_STATUS      ((uint32_t)0x02) /* Audio Player in Idle Status */

#define AUDIO_PLAY_SAMPLE        0
#define AUDIO_PLAY_RECORDED      1

/* Exported macro ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))

/* Exported functions ------------------------------------------------------- */
void Led_demo(void);
#if defined(USE_GVO_390x390)
void DSI_LCD_demo(void);
#endif
#if defined(USE_ROCKTECH_480x272)
void RGB_LCD_demo(void);
#endif
void EEPROM_demo(void);
void Idd_demo(void);
void Joystick_demo (void);
void AudioPlay_demo(void);
void AudioRecord_demo(void);
#if defined(USE_GVO_390x390)
void DSI_Touchscreen_demo(void);
#else
void Touchscreen_demo(void);
#endif
void NOR_demo(void);
void SRAM_demo(void);
void OSPI_NOR_demo(void);
void OSPI_RAM_demo(void);
void SD_demo(void);
void Log_demo(void);

void SystemClock_Config(void);
void SystemLowClock_Config(void);

void SystemHardwareInit(void);
void SystemHardwareDeInit(void);

void Mfx_Event(void);
void Toggle_Leds(void);
void Error_Handler(void);

#if defined(LCD_DIMMING)
void Dimming_Timer_Enable(RTC_HandleTypeDef * RTCHandle);
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
