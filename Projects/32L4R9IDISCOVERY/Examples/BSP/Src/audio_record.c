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

extern __IO JOYState_TypeDef JoyState;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int16_t                      RecordBuff[4096];
int16_t                      PlaybackBuff[4096];
uint32_t                     RecHalfBuffCplt  = 0;
uint32_t                     RecBuffCplt      = 0;
uint32_t                     PlaybackStarted  = 0;
uint16_t                     CurrentInputDevice = INPUT_DEVICE_DIGITAL_MIC;

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
  
  /* Reset JoyState */
  JoyState = JOY_NONE;

  while(JoyState != JOY_RIGHT)
  {
    /* Display current microphone */
    while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
    if(CurrentInputDevice == INPUT_DEVICE_DIGITAL_MIC)
    {
      BSP_LCD_DisplayStringAt(0, 150, (uint8_t *)"Current mic is digital", CENTER_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(0, 150, (uint8_t *)"Current mic is headset", CENTER_MODE);
    }
    BSP_LCD_Refresh();

    /* Initialize record */
    Record_Init();
    
    /* Initialize playback */
    Playback_Init();
    
    /* Start record */
    if(AUDIO_OK != BSP_AUDIO_IN_Record((uint16_t*) RecordBuff, 4096))
    {
      Error_Handler();
    }
    
    /* Reset JoyState */
    JoyState = JOY_NONE;
    
    /* Start loopback */
    while((JoyState != JOY_RIGHT) && (JoyState != JOY_SEL))
    { 
      if(MfxItOccurred == SET)
      {
        Mfx_Event();
      }
      
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
    
    /* Restore GPIOH clock used for LED */
    __HAL_RCC_GPIOH_CLK_ENABLE();
    
    /* Reset global variables */
    RecHalfBuffCplt  = 0;
    RecBuffCplt      = 0;
    PlaybackStarted  = 0;
    
    /* Switch input device if JOY_SEL */
    if(JoyState == JOY_SEL)
    {
      CurrentInputDevice = (CurrentInputDevice == INPUT_DEVICE_DIGITAL_MIC) ? INPUT_DEVICE_ANALOG_MIC : INPUT_DEVICE_DIGITAL_MIC;
    }
  }
  /* Reset JoyState */
  JoyState = JOY_NONE;
}

/**
  * @brief  Display audio record demo hint
  * @param  None
  * @retval None
  */
static void AudioRecord_SetHint(void)
{
  while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set Audio Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 130);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 40, (uint8_t *)"Audio Record", CENTER_MODE);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 80, (uint8_t *)"Press RIGHT to stop record", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 95, (uint8_t *)"Press SEL to change micro", CENTER_MODE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(40, 140, BSP_LCD_GetXSize() - 80, BSP_LCD_GetYSize()- 240);
  BSP_LCD_DrawRect(41, 141, BSP_LCD_GetXSize() - 82, BSP_LCD_GetYSize()- 242);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
  BSP_LCD_Refresh();
}

/**
  * @brief  Record initialization
  * @param  None
  * @retval None
  */
static void Record_Init(void)
{
  if(CurrentInputDevice == INPUT_DEVICE_DIGITAL_MIC)
  {
    if(AUDIO_OK != BSP_AUDIO_IN_InitEx(INPUT_DEVICE_DIGITAL_MIC, AUDIO_FREQUENCY_22K, 16, 2))
    {
      Error_Handler();
    }
  }
  else
  {
    if(AUDIO_OK != BSP_AUDIO_IN_InitEx(INPUT_DEVICE_ANALOG_MIC, AUDIO_FREQUENCY_22K, 16, 1))
    {
      Error_Handler();
    }
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
  if(CurrentInputDevice == INPUT_DEVICE_ANALOG_MIC)
  {
    if(AUDIO_OK != BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, 90, AUDIO_FREQUENCY_22K))
    {
      Error_Handler();
    }
    
    BSP_AUDIO_OUT_ChangeAudioConfig(BSP_AUDIO_OUT_CIRCULARMODE | BSP_AUDIO_OUT_MONOMODE);
  }
  else
  {
    if(AUDIO_OK != BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, 80, AUDIO_FREQUENCY_22K))
    {
      Error_Handler();
    }
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
  RecHalfBuffCplt = 1;
}

/**
  * @brief  Record complete callback. 
  * @param  None.
  * @retval None.
  */
void Audio_Record_RxCpltCallback(void)
{
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
}

/**
  * @brief  Playback complete callback. 
  * @param  None.
  * @retval None.
  */
void Audio_Playback_TxCpltCallback(void)
{
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

