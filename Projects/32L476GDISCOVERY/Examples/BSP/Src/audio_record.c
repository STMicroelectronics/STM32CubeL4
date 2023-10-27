/**
  ******************************************************************************
  * @file    BSP/Src/audio_record.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to record PCM audio samples on the 
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
typedef enum
{
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF,
  BUFFER_OFFSET_FULL,
 } RecordBufferOffset_Typedef;


/* Private define ------------------------------------------------------------*/
/* Size (in bytes) of the buffer containing the recorded PCM samples */
#define RECORD_BUFFER_SIZE (uint32_t)2048

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Buffer containing the PCM samples coming from the microphone */
static uint16_t RecordBuffer[RECORD_BUFFER_SIZE];

/* Buffer used to stream the recorded PCM samples towards the audio codec. */
static uint16_t PlaybackBuffer[RECORD_BUFFER_SIZE*2];


#define NB_SAMPLES_RATE 7
static uint32_t SamplesRates[NB_SAMPLES_RATE] = {BSP_AUDIO_FREQUENCY_48K,
                                                 BSP_AUDIO_FREQUENCY_44K,
                                                 BSP_AUDIO_FREQUENCY_32K,
                                                 BSP_AUDIO_FREQUENCY_22K,
                                                 BSP_AUDIO_FREQUENCY_16K,
                                                 BSP_AUDIO_FREQUENCY_11K,
                                                 BSP_AUDIO_FREQUENCY_8K};

/* Information indicating which part of the recorded buffer is ready for audio loopback  */
static RecordBufferOffset_Typedef  RecordBufferOffset = BUFFER_OFFSET_NONE;

/* Private function prototypes -----------------------------------------------*/
/* Display actuyel record sample rate */
static void SampleRateDisplay(uint8_t SampleRate);

/* Record sample rate selection */
static void SampleRateSelection(void);

/* Audio recorder callback functions */
static void AudioRecord_TransferComplete_CallBack(void);
static void AudioRecord_HalfTransfer_CallBack(void);
static void AudioRecord_Error_CallBack(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief Setup and start audio recording in loopback mode
  * @param  None
  * @retval None
  */
void AudioRecord_demo(void)
{
  uint32_t i;
  /* Flag indicating whether the audio codec has already been initialized */
  uint32_t audio_loop_back_init = RESET ; 
  
  /* Audio record demo is running */
  AudioDemo = AUDIO_DEMO_RECORD;
  
  /* Turn off green LED5 */
  BSP_LED_Off(LED5);
  
  /* Initialize record buffer offset */
  RecordBufferOffset = BUFFER_OFFSET_NONE;

  /* Select the audio record sample rate */
  SampleRateSelection();

  /* Initialize the audio device*/ 
  if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, 
                         80, 
                         SamplesRates[SampleRateIndex]) != AUDIO_OK)
  {
    Error_Handler();
  }
  
  /* Initialize audio OUT transfer in DMA circular mode and in stereo mode */ 
  BSP_AUDIO_OUT_ChangeAudioConfig(BSP_AUDIO_OUT_CIRCULARMODE|BSP_AUDIO_OUT_STEREOMODE);
  
  /* Initialize audio input */
  if (BSP_AUDIO_IN_Init(SamplesRates[SampleRateIndex],
                        DEFAULT_AUDIO_IN_BIT_RESOLUTION,
                        DEFAULT_AUDIO_IN_CHANNEL_NBR) != AUDIO_OK)
  {
    /* Record Error */
    Error_Handler();
  } 
  
  /* Set Callback function pointers */
  BSP_AUDIO_IN_RegisterCallbacks(AudioRecord_Error_CallBack,
                                 AudioRecord_HalfTransfer_CallBack,
                                 AudioRecord_TransferComplete_CallBack);
  
  /* Start the audio record */
  if (BSP_AUDIO_IN_Record((uint16_t*)&RecordBuffer[0],
                          RECORD_BUFFER_SIZE) != AUDIO_OK)
  {
    Error_Handler();
  }
  
  /* PCM samples recording loop */
  while (AudioRecordExit != SET)
  {
    /* 1st or 2nd half of the record buffer ready for being copied 
       to the playback buffer */
    if(RecordBufferOffset != BUFFER_OFFSET_NONE)
    {
      /* Copy half of the record buffer to the playback buffer */
      if(RecordBufferOffset == BUFFER_OFFSET_HALF)
      {
        for(i = 0; i < (RECORD_BUFFER_SIZE/2); i++)
        {
          PlaybackBuffer[2*i]     = RecordBuffer[i];
          PlaybackBuffer[(2*i)+1] = PlaybackBuffer[2*i];
        }        
        
        if (audio_loop_back_init == RESET)
        {
          /* Play the recorded buffer */
          if (BSP_AUDIO_OUT_Play(PlaybackBuffer, RECORD_BUFFER_SIZE*2) != AUDIO_OK)
          {
            Error_Handler();
          }
          
          /* Audio device is initialized only once */
          audio_loop_back_init = SET; 
        }
      }
      else /* if(RecordBufferOffset == BUFFER_OFFSET_FULL)*/
      {
        for(i = (RECORD_BUFFER_SIZE/2); i < RECORD_BUFFER_SIZE; i++)
        {
          PlaybackBuffer[2*i]     = RecordBuffer[i];
          PlaybackBuffer[(2*i)+1] = PlaybackBuffer[2*i];
        }        
      }
      
      /* Wait for next data */
      RecordBufferOffset = BUFFER_OFFSET_NONE;  
    }
  }
  
  /* Mute audio device */
  BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_ON);
  HAL_Delay(20);
  
  /* Stop audio device */
  if (BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW) != AUDIO_OK)
  {
    Error_Handler();
  }
  
  /* De-initialize audio device */
  if(BSP_AUDIO_OUT_DeInit() != AUDIO_OK)
  {
    Error_Handler();
  }
  
  /* Stop audio input */
  if (BSP_AUDIO_IN_Stop() != AUDIO_OK)
  {
    Error_Handler();
  }
  
  /* De-initialize audio input */
  if (BSP_AUDIO_IN_DeInit() != AUDIO_OK)
  {
    Error_Handler();
  } 
  
  /* Turn OFF LED5: stop record */
  BSP_LED_Off(LED5);
  
  
  /* Reset AudioRecordExit flag */
  AudioRecordExit = 0;
  
    /* Turn off green led */
  AudioDemo = AUDIO_DEMO_NONE;
}

/**
  * @brief  Display actual audio record sample rate on LCD 
  * @param  None
  * @retval None
  */
static void SampleRateDisplay(uint8_t SampleRate)
{
  /* Clear the LCD glass */
  BSP_LCD_GLASS_Clear(); 
  
  /* Display actual audio record sample rate */
   switch (SamplesRates[SampleRate])
   {
   case BSP_AUDIO_FREQUENCY_48K:
     BSP_LCD_GLASS_DisplayString((uint8_t *)"48000");
     break;
   case BSP_AUDIO_FREQUENCY_44K:
     BSP_LCD_GLASS_DisplayString((uint8_t *)"44000");
     break;
   case BSP_AUDIO_FREQUENCY_32K:
     BSP_LCD_GLASS_DisplayString((uint8_t *)"32000");
     break;
   case BSP_AUDIO_FREQUENCY_22K:
     BSP_LCD_GLASS_DisplayString((uint8_t *)"22000");
     break;
   case BSP_AUDIO_FREQUENCY_16K:
     BSP_LCD_GLASS_DisplayString((uint8_t *)"16000");
     break;
   case BSP_AUDIO_FREQUENCY_11K:
     BSP_LCD_GLASS_DisplayString((uint8_t *)"11000");
     break;
   case BSP_AUDIO_FREQUENCY_8K:
     BSP_LCD_GLASS_DisplayString((uint8_t *)"8000");
     break;
   default:
     break;
   }
}

/**
  * @brief  Audio record sample rate selection 
  * @param  None
  * @retval None
  */
void SampleRateSelection(void)
{
  /* Display actual audio record sample rate */
   SampleRateDisplay(SampleRateIndex);   
   
  /* Select audio record sample rate */
  while (!AudioRecordSampleRateSelected)
  {
    if (AudioRecordSampleRateChange != 0)
    {
      SampleRateDisplay(SampleRateIndex);   
      HAL_Delay(5);
      AudioRecordSampleRateChange = 0;
    }
  }
  
  AudioRecordSampleRateSelected = 0;
}

/**
  * @brief Callback function invoked when half of the PCM samples have been 
  *        DMA transferred from the DFSDM channel.
  * @param  None
  * @retval None
  */
void AudioRecord_TransferComplete_CallBack(void)
{
  /* Toggle green LED */
  BSP_LED_Toggle(LED5);
  
  RecordBufferOffset = BUFFER_OFFSET_FULL;
}

/**
  * @brief Callback function invoked when all the PCM samples have been 
  *        DMA transferred from the DFSDM channel.
  * @param  None
  * @retval None
  */
void AudioRecord_HalfTransfer_CallBack(void)
{
  RecordBufferOffset = BUFFER_OFFSET_HALF;
}

/**
  * @brief Callback function invoked when an error occurred durint he DMA 
  *        transfer of the PCM samples from the DFSDM channel.
  * @param  None
  * @retval None
  */
void AudioRecord_Error_CallBack(void)
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
