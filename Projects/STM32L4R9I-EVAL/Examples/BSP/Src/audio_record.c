/**
  ******************************************************************************
  * @file    BSP/Src/audio_record.c
  * @author  MCD Application Team
  * @brief   This example describes how to use BSP AUDIO for record.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int16_t                      RecordBuff[4096];
int16_t                      PlaybackBuff[4096];
uint32_t                     RecHalfBuffCplt  = 0;
uint32_t                     RecBuffCplt      = 0;
uint32_t                     PlaybackStarted  = 0;

extern __IO FlagStatus  TamperItOccurred;

/* Private function prototypes -----------------------------------------------*/
static void AudioRecord_SetHint(void);
static void Record_Init(void);
static void Playback_Init(void);
void Audio_Record_RxHalfCpltCallback(void);
void Audio_Record_RxCpltCallback(void);
void Audio_Record_ErrorCallback(void);
void Audio_Playback_TxHalfCpltCallback(void);
void Audio_Playback_TxCpltCallback(void);
void Audio_Playback_ErrorCallback(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Test BSP AUDIO for record.
  * @param  None
  * @retval None
  */
void AudioRecord_demo(void)
{
  uint32_t i;
  
  /* Display test information */
  AudioRecord_SetHint();
  
  /* Initialize record */
  Record_Init();
  
  /* Initialize playback */
  Playback_Init();
  
  /* Start record */
  if(AUDIO_OK != BSP_AUDIO_IN_Record((uint16_t*) RecordBuff, 4096))
  {
    Error_Handler();
  }
  
  /* Start loopback */
  while(TamperItOccurred != SET)
  {
    if(RecHalfBuffCplt == 1)
    {
      /* Store values on Play buff */
      for(i = 0; i < 2048; i++)
      {
        PlaybackBuff[i] = RecordBuff[i];
      }
      if(PlaybackStarted == 0)
      {
        /* Insert a little delay before starting playback to be sure that data are available for playback */
        /* Without this delay, potential noise when optimization high is selected for compiler */
        HAL_Delay(10);
        /* Start the playback */
        if(AUDIO_OK != BSP_AUDIO_OUT_Play((uint16_t *)PlaybackBuff, 4096))
        {
          Error_Handler();
        }
        PlaybackStarted = 1;
      }      
      RecHalfBuffCplt = 0;
    }
    if(RecBuffCplt == 1)
    {
      /* Store values on Play buff */
      for(i = 2048; i < 4096; i++)
      {
        PlaybackBuff[i] = RecordBuff[i];
      }
      RecBuffCplt  = 0;
    }
  }
  /* Add delay to avoid rebound and reset it status */
  HAL_Delay(500);
  TamperItOccurred = RESET;
  
  /* Stop playback */
  if(BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW) != AUDIO_OK)
  {
    Error_Handler();
  }
  
  /* Stop record */
  if(BSP_AUDIO_IN_Stop() != AUDIO_OK)
  {
    Error_Handler();
  }
  
  /* De-initialize playback */
  if(BSP_AUDIO_OUT_DeInit() != AUDIO_OK)
  {
    Error_Handler();
  }
  
  /* De-initialize record */
  if(BSP_AUDIO_IN_DeInit() != AUDIO_OK)
  {
    Error_Handler();
  }
  
  /* Switch off LEDs */
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_BLUE);
  
  /* Reset global variables */
  RecHalfBuffCplt  = 0;
  RecBuffCplt      = 0;
  PlaybackStarted  = 0;
}

/**
  * @brief  Display audio record demo hint
  * @param  None
  * @retval None
  */
static void AudioRecord_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set Audio Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"Audio Record", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Press Wakeup push-button to stop record", CENTER_MODE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
}

/**
  * @brief  Record initialization
  * @param  None
  * @retval None
  */
static void Record_Init(void)
{
  if(AUDIO_OK != BSP_AUDIO_IN_Init(AUDIO_FREQUENCY_8K, 16, 2))
  {
    Error_Handler();
  }
  
  BSP_AUDIO_IN_RegisterCallbacks(Audio_Record_ErrorCallback,
                                 Audio_Record_RxHalfCpltCallback,
                                 Audio_Record_RxCpltCallback);
}

/**
  * @brief  Playback initialization
  * @param  None
  * @retval None
  */
static void Playback_Init(void)
{
  if(AUDIO_OK != BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, 60, AUDIO_FREQUENCY_8K))
  {
    Error_Handler();
  }
  
  BSP_AUDIO_OUT_RegisterCallbacks(Audio_Playback_ErrorCallback,
                                  Audio_Playback_TxHalfCpltCallback,
                                  Audio_Playback_TxCpltCallback);
}

/**
  * @brief  Half record complete callback. 
  * @param  None.
  * @retval None.
  */
void Audio_Record_RxHalfCpltCallback(void)
{
  BSP_LED_Toggle(LED_GREEN);
  RecHalfBuffCplt = 1;
}

/**
  * @brief  Record complete callback. 
  * @param  None.
  * @retval None.
  */
void Audio_Record_RxCpltCallback(void)
{
  BSP_LED_Toggle(LED_GREEN);
  RecBuffCplt = 1;
}

/**
  * @brief  Record error callback. 
  * @param  None.
  * @retval None.
  */
void Audio_Record_ErrorCallback(void)
{
  Error_Handler();
}

/**
  * @brief  Half playback complete callback. 
  * @param  None.
  * @retval None.
  */
void Audio_Playback_TxHalfCpltCallback(void)
{
  BSP_LED_Toggle(LED_BLUE);
}

/**
  * @brief  Playback complete callback. 
  * @param  None.
  * @retval None.
  */
void Audio_Playback_TxCpltCallback(void)
{
  BSP_LED_Toggle(LED_BLUE);
}

/**
  * @brief  Playback error callback. 
  * @param  None.
  * @retval None.
  */
void Audio_Playback_ErrorCallback(void)
{
  Error_Handler();
}

/**
  * @}
  */

/**
  * @}
  */

