/**
  ******************************************************************************
  * @file    BSP/Src/audio_play.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to play WAVE audio on the 
  *          STM32L476G-Discovery using the audio BSP.
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
/* WAV header structure */
typedef struct
{
  char       RIFF[4];          /* 0 */ 
  uint32_t   ChunkSize;        /* 4 */
  char       WAVE[4];          /* 8 */
  char       fmt[4];           /* 12 */
  uint32_t   SubChunk1Size;    /* 16*/  
  uint16_t   AudioFormat;      /* 20 */ 
  uint16_t   NbrChannels;      /* 22 */   
  uint32_t   SampleRate;       /* 24 */
  uint32_t   ByteRate;         /* 28 */
  uint16_t   BlockAlign;       /* 32 */  
  uint16_t   BitPerSample;     /* 34 */  
  char       SubChunk2ID[4];   /* 36 */   
  uint32_t   SubChunk2Size;    /* 40 */    
} WavHeaderTypeDef;

/* Private define ------------------------------------------------------------*/
/* Size (in bytes) of the audio file */
#define AUDIO_FILE_SIZE      (uint32_t)(448*1024)

/* Address of the first audio sample in FLASH memory*/ 
#define AUDIO_START_ADDRESS  (uint32_t)0x08020000  
  
/* Address of the last audio sample in FLASH memory*/ 
#define AUDIO_END_ADDRESS    (uint32_t)(AUDIO_START_ADDRESS + AUDIO_FILE_SIZE)
  
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Remainig number of audio samples to play */
static int32_t RemainingAudioSamplesNb;

/* Address (in Flash memory) of the first audio sample to play */
static uint16_t* pAudioSample;

/* Private function prototypes -----------------------------------------------*/
/* Audio player callback functions */
static void AudioPlay_TransferComplete_CallBack(void);
static void AudioPlay_Error_CallBack(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief Setup and start the WAVE audio playback
  * @param  None
  * @retval None
  */
void AudioPlay_demo(void)
{  
  WavHeaderTypeDef *WavHeader =  NULL;
  uint8_t strVolume[20] = {0};

  /* Retrieve Wave Sample rate*/
  WavHeader = (WavHeaderTypeDef*)AUDIO_START_ADDRESS;
  
  /* Check whether the WAV header is valid, if not a message is displayed on the
     LCD glass and the function is exited.
  */
  if (   (strncmp(WavHeader->RIFF, "RIFF", 4) != 0)
      || (strncmp(WavHeader->WAVE, "WAVE", 4) != 0)
      || (WavHeader->AudioFormat != 1))
  {
    /* Invalid WAV header: audio file not downloaded to Flash memory) */
    BSP_LCD_GLASS_Clear(); 
    BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      AUDIO FILE NOT FOUND AT 0x08020000",
                                 1,
                                 SCROLL_SPEED_MEDIUM);
  }
  else
  {
    /* Audio playback demo is running */
    AudioDemo = AUDIO_DEMO_PLAYBACK;
    
    /* Switch off the green led */
    BSP_LED_Off(LED5);
    
    /* Configuration of the EXTI for the joystick:
    - SEL  button for pause/resume
    - UP button to increase the volume level
    - DOWN button to decrease the volume level
    */
    BSP_JOY_Init(JOY_MODE_EXTI);
    
    /* Set the remaining number of data to be played */
    RemainingAudioSamplesNb = (uint32_t)(AUDIO_FILE_SIZE / AUDIODATA_SIZE);
    
    /* Set the pointer to the first audio sample to play */
    pAudioSample = (uint16_t*)AUDIO_START_ADDRESS;
    
    /* Initialize the audio device */
    if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE,
                          Volume,
                          WavHeader->SampleRate) != AUDIO_OK)
    {
      Error_Handler();
    }
    
    /* Set Callback function pointers */
    BSP_AUDIO_OUT_RegisterCallbacks(AudioPlay_Error_CallBack,
                                    NULL,
                                    AudioPlay_TransferComplete_CallBack);
    
    /* Set Volume */
    if(BSP_AUDIO_OUT_SetVolume(Volume) != 0)
    {
      Error_Handler();
    }
    
    /* Start the audio playback */
    if(BSP_AUDIO_OUT_Play(pAudioSample, RemainingAudioSamplesNb) != AUDIO_OK)
    {
      Error_Handler();
    }
    
    /* Display the audio playback status */
    BSP_LCD_GLASS_Clear(); 
    BSP_LCD_GLASS_DisplayString((uint8_t *)"PLAY");
    
    /* Audio playback event handler loop */
    while (!AudioPlaybackExit)
    { 
      if (PauseResumeStatus == PAUSE_STATUS)
      {
        /* Playback paused */
        if(BSP_AUDIO_OUT_Pause() != AUDIO_OK)
        {
          Error_Handler();
        }
        
        /* Display the audio playback status */
        BSP_LCD_GLASS_Clear(); 
        BSP_LCD_GLASS_DisplayString((uint8_t *)"PAUSED");
        
        /* Reset Pause/Resume flag */
        PauseResumeStatus = IDLE_STATUS;
        
        /* Turn on green LED: playback is paused */
        BSP_LED_On(LED5);
      }
      else if (PauseResumeStatus == RESUME_STATUS)
      {
        /* Playback resumed */
        if(BSP_AUDIO_OUT_Resume() != AUDIO_OK)
        {
          Error_Handler();
        }
        
        /* Display the audio playback status */
        BSP_LCD_GLASS_Clear(); 
        BSP_LCD_GLASS_DisplayString((uint8_t *)"PLAY");

        /* Reset Pause/Resume flag */
        PauseResumeStatus = IDLE_STATUS;
      
        /* Turn off green LED: playback is resumed */
        BSP_LED_Off(LED5);
      }
      
      if (VolumeChange != 0)
      {
        if(BSP_AUDIO_OUT_SetVolume(Volume) != AUDIO_OK)
        {
          Error_Handler();
        }
        
        /* Display the new volume level */
        BSP_LCD_GLASS_Clear(); 
        sprintf((char *) strVolume, "Vol%d", Volume);
        BSP_LCD_GLASS_DisplayString(strVolume);
        
        HAL_Delay(5);
        
        /* Reset volume change flag */
        VolumeChange = 0;
      }
    }
    
    /* Configure the joystick back to GPIO mode */
    BSP_JOY_Init(JOY_MODE_GPIO);
    
    /* Mute audio playback */
    BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_ON);
    HAL_Delay(20);
    
    /* Stop audio device */
    if (BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW) != AUDIO_OK)
    {
      /* Audio Stop error */
      Error_Handler();
    }
    
    /* De-initialize audio device */
    if(BSP_AUDIO_OUT_DeInit() != AUDIO_OK)
    {
      Error_Handler();
    }
    
    /* Turn OFF LED5 green: audio playback stopped */
    BSP_LED_Off(LED5);
    
    /* Reset AudioPlaybackExit flag */
    AudioPlaybackExit = 0;
  }
  
  /* No audio playback is running */
  AudioDemo = AUDIO_DEMO_NONE;
}

/**
* @brief  Callback invoked the DMA transfer of te audio samples towards the
*         audio codec is completed.
* @param  None
* @retval None
* @note This callback function has been registered during the audio playback
*       setup phase by calling the BSP_AUDIO_OUT_RegisterCallbacks function.
*/
void AudioPlay_TransferComplete_CallBack()
{
  uint32_t replay = 0;
  
  if (AudioDemo == AUDIO_DEMO_PLAYBACK)
  {
    /* Toggle on green LED: playback is going-on */
    BSP_LED_Toggle(LED5);
    
    /* Update the current pointer position */
    pAudioSample += DMA_MAX(RemainingAudioSamplesNb);        

    /* Update the remaining number of data to be played */
    RemainingAudioSamplesNb = (AUDIO_END_ADDRESS - (uint32_t)pAudioSample)/2; 
    
    if (RemainingAudioSamplesNb > 0)
    {
      /* Replay from the current position */
      if (BSP_AUDIO_OUT_ChangeBuffer(pAudioSample,
                                     DMA_MAX(RemainingAudioSamplesNb)) != 0) 
      {
        Error_Handler();
      }
    }
    else
    {
      /* Request to replay audio file from beginning */
      replay = 1;
    }
    
    /* Audio sample used for play*/
    if (replay == 1)
    {
      /* Set the remaining number of data to be played */
      RemainingAudioSamplesNb = (uint32_t)(AUDIO_FILE_SIZE / AUDIODATA_SIZE);
      
      /* Set the pointer to the first audio sample to play */
      pAudioSample = (uint16_t*)AUDIO_START_ADDRESS;
      
      /* Replay from the beginning */
      if (BSP_AUDIO_OUT_Play((uint16_t*)pAudioSample, 
                             RemainingAudioSamplesNb) != AUDIO_OK)
      {
        Error_Handler();
      }
    }
  }
}

/**
* @brief  Callback invoked the DMA transfer of te audio samples towards the
*         audio codec has failed.
* @param  None
* @retval None
* @note This callback function has been registered during the audio playback
*       setup phase by calling the BSP_AUDIO_OUT_RegisterCallbacks function.
*/
void AudioPlay_Error_CallBack(void)
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
