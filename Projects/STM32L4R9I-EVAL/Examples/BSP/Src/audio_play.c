/**
  ******************************************************************************
  * @file    BSP/Src/audio_play.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use AUDIO features for the play.
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
typedef struct
{
  uint32_t   ChunkID;       /* 0 */ 
  uint32_t   FileSize;      /* 4 */
  uint32_t   FileFormat;    /* 8 */
  uint32_t   SubChunk1ID;   /* 12 */
  uint32_t   SubChunk1Size; /* 16*/  
  uint16_t   AudioFormat;   /* 20 */ 
  uint16_t   NbrChannels;   /* 22 */   
  uint32_t   SampleRate;    /* 24 */
  
  uint32_t   ByteRate;      /* 28 */
  uint16_t   BlockAlign;    /* 32 */  
  uint16_t   BitPerSample;  /* 34 */  
  uint32_t   SubChunk2ID;   /* 36 */   
  uint32_t   SubChunk2Size; /* 40 */    

}WAVE_FormatTypeDef;

/* Private define ------------------------------------------------------------*/
/* Audio file size and start address are defined here since the Audio file is 
   stored in Flash memory as a constant table of 16-bit data */
#define AUDIO_FILE_ADDRESS    0x08100000   /* Audio file address */  
#define AUDIO_FILE_SIZE      (180*1024)
#define PLAY_HEADER          0x2C
#define PLAY_BUFF_SIZE       4096
  
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t AudioPlayback = 0;

extern __IO uint32_t PauseResumeStatus;
extern __IO FlagStatus  TamperItOccurred;

/* Variable used to replay audio sample (from play or record test)*/
__IO uint32_t AudioTest = AUDIO_PLAY_SAMPLE;
  
int16_t       PlayBuff[PLAY_BUFF_SIZE];
__IO uint32_t PlaybackPosition;

extern __IO uint8_t Volume;
extern __IO uint8_t VolumeChange;

/* Private function prototypes -----------------------------------------------*/
static void AudioPlay_SetHint(void);
static void AudioPlay_DisplayInfos(WAVE_FormatTypeDef * format);

static void AUDIO_TransferComplete_CallBack(void);
static void AUDIO_HalfTransfer_CallBack(void);
static void AUDIO_Error_CallBack(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief Test Audio Hardware.
  *   The main objective of this test is to check the hardware connection of the 
  *   Audio peripheral.
  * @param  None
  * @retval None
  */
void AudioPlay_demo(void)
{  
  WAVE_FormatTypeDef *waveformat =  NULL;
  uint8_t Volume_string[20] = {0};
  uint8_t status = AUDIO_OK;

  PlaybackPosition   = (2*PLAY_BUFF_SIZE) + PLAY_HEADER;
  
  AudioPlay_SetHint();

  /* Check if the audio buffer has been loaded in flash */
  if(*((uint64_t *)AUDIO_FILE_ADDRESS) != 0x017EFE2446464952 )
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
    BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"Initialization problem", CENTER_MODE); 
    BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"No Audio data in Flash", CENTER_MODE); 
    status = AUDIO_ERROR;
  }
  else
  {
    /* Retrieve Wave Sample rate*/
    waveformat = (WAVE_FormatTypeDef*) AUDIO_FILE_ADDRESS;
    AudioPlay_DisplayInfos(waveformat);

    AudioTest = AUDIO_PLAY_SAMPLE;
    /* Initialize Audio Device */
    if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, Volume, waveformat->SampleRate) != AUDIO_OK)
    {
      BSP_LCD_SetTextColor(LCD_COLOR_RED);    
      BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"Initialization problem", CENTER_MODE); 
      BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"Audio Codec not detected", CENTER_MODE);
      status = AUDIO_ERROR;
    }
    else
    {
      /* Register audio BSP drivers callbacks */
      BSP_AUDIO_OUT_RegisterCallbacks(AUDIO_Error_CallBack, 
                                      AUDIO_HalfTransfer_CallBack, 
                                     AUDIO_TransferComplete_CallBack);

      /* Initialize Volume */
      if(BSP_AUDIO_OUT_SetVolume(Volume) == AUDIO_OK)
      {
        /* Initialize the data buffer */
        for(int i=0; i < PLAY_BUFF_SIZE; i++)
        {
          PlayBuff[i]=*((__IO uint16_t *)(AUDIO_FILE_ADDRESS + PLAY_HEADER + (2*i)));
        }

        /* Start the audio player */
        if(BSP_AUDIO_OUT_Play((uint16_t *)PlayBuff, PLAY_BUFF_SIZE) == AUDIO_OK)
        {
          /* Turn ON LED green: start of Audio file play */
          BSP_LED_On(LED_GREEN);

          BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, (uint8_t *)"Playback on-going", LEFT_MODE);
          sprintf((char *) Volume_string, " Volume : %d%% ", Volume);
          BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, Volume_string, RIGHT_MODE);
  
          /* Set audio playback variable */
          AudioPlayback = 1;
        }
      }
      else
      {
        status = AUDIO_ERROR;
      }
    }
  }
  
  /* Infinite loop */
  while(TamperItOccurred != SET)
  { 
    if(MfxItOccurred == SET)
    {
      Mfx_Event();
    }

    if(status == AUDIO_OK)
    {
      if (PauseResumeStatus == PAUSE_STATUS)
      {
        /* Turn ON LED orange: Audio play in pause */
        BSP_LED_On(LED_ORANGE);
      
        /* Pause playing */
        if(BSP_AUDIO_OUT_Pause() != AUDIO_OK)
        {
          Error_Handler();
        }
        BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, (uint8_t *)"Playback paused  ", LEFT_MODE);
        PauseResumeStatus = IDLE_STATUS;
      }
      else if (PauseResumeStatus == RESUME_STATUS)
      {
        /* Turn OFF LED orange: Audio play running */
        BSP_LED_Off(LED_ORANGE);
      
        /* Resume playing */
        if(BSP_AUDIO_OUT_Resume() != AUDIO_OK)
        {
          Error_Handler();
        }
        BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, (uint8_t *)"Playback on-going", LEFT_MODE);
        PauseResumeStatus = IDLE_STATUS;
      }
  
      if (VolumeChange != 0)
      {
        VolumeChange = 0;
        if(BSP_AUDIO_OUT_SetVolume(Volume) != AUDIO_OK)
        {
          Error_Handler();
        }
        sprintf((char *) Volume_string, " Volume : %d%% ", Volume);
        BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, Volume_string, RIGHT_MODE);
      }
    }
  }
  /* Add delay to avoid rebound and reset it status */
  HAL_Delay(500);
  TamperItOccurred = RESET;
  
  /* Reset audio playback variable */
  AudioPlayback = 0;
  
  /* Stop Player before close Test */
  if(status == AUDIO_OK)
  {
    if (BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW) != AUDIO_OK)
    {
      /* Audio Stop error */
      Error_Handler();
    }

    /* De-initialize playback */
    if(BSP_AUDIO_OUT_DeInit() != AUDIO_OK)
    {
      Error_Handler();
    }
  }

  /* Turn OFF LED green: stop of Audio file play */
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_ORANGE); 
}

/**
  * @brief  Display audio play demo hint
  * @param  None
  * @retval None
  */
static void AudioPlay_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set Audio Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"Audio Play", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Press Wakeup push-button to stop play", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"Press SEL to pause/resume playback", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"Press UP/DOWN to change Volume", CENTER_MODE);   
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
}

/**
  * @brief  Display audio play demo hint
  * @param  format : structure containing information of the file
  * @retval None
  */
static void AudioPlay_DisplayInfos(WAVE_FormatTypeDef * format)
{
  uint8_t string[50] = {0};

  sprintf((char *) string, "Sampling frequency : %ld Hz", format->SampleRate);
  BSP_LCD_DisplayStringAt(20, 100, string, CENTER_MODE);

  if (format->NbrChannels == 2)
  {
    sprintf((char *) string, "Format : %d bits stereo", format->BitPerSample);
    BSP_LCD_DisplayStringAt(20, 115, string, CENTER_MODE);
  }
  else if (format->NbrChannels == 1)
  {
    sprintf((char *) string, "Format : %d bits mono", format->BitPerSample);
    BSP_LCD_DisplayStringAt(20, 115, string, CENTER_MODE);
  }
}

/*--------------------------------
Callbacks implementation:
The callbacks prototypes are defined in the stm32l476g_eval_audio.h file
and their implementation should be done in the user code if they are needed.
Below some examples of callback implementations.
--------------------------------------------------------*/
/**
* @brief  AUDIO transfer complete function.
* @param  None
* @retval None
*/
static void AUDIO_TransferComplete_CallBack(void)
{
  /* Update the first or the second part of the buffer */
  for(int i = 0; i < PLAY_BUFF_SIZE/2; i++)
  {
    PlayBuff[i+ (PLAY_BUFF_SIZE/2)] = *(uint16_t *)(AUDIO_FILE_ADDRESS + PlaybackPosition);
    PlaybackPosition+=2; 
  }

  /* check the end of the file */
  if((PlaybackPosition+PLAY_BUFF_SIZE/2) > AUDIO_FILE_SIZE)
  {
    PlaybackPosition = PLAY_HEADER;
  }
}

/**
  * @brief  AUDIO half transfer complete function.
  * @param  None
  * @retval None
  */
static void AUDIO_HalfTransfer_CallBack(void)
{ 
  /* Update the first or the second part of the buffer */
  for(int i = 0; i < PLAY_BUFF_SIZE/2; i++)
  {
    PlayBuff[i] = *(uint16_t *)(AUDIO_FILE_ADDRESS + PlaybackPosition);
    PlaybackPosition+=2; 
  }

  /* check the end of the file */
  if((PlaybackPosition+PLAY_BUFF_SIZE/2) > AUDIO_FILE_SIZE)
  {
    PlaybackPosition = PLAY_HEADER;
  }
}

/**
* @brief  AUDIO error callback.
* @param  None
* @retval None
*/
static void AUDIO_Error_CallBack(void)
{
  /* Stop the program with an infinite loop */
  Error_Handler();
}

/**
  * @}
  */ 


/**
  * @}
  */ 
