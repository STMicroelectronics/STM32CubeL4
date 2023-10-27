/**
  ******************************************************************************
  * @file    BSP/Src/audio_record.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use AUDIO features for the record.
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

/** @addtogroup BSP_Examples
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef struct {
  int32_t offset;
  uint32_t fptr;
}Audio_BufferTypeDef;

typedef enum
{
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF,
  BUFFER_OFFSET_FULL,
}BUFFER_StateTypeDef;


/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Size of the recorder buffer */
#define REC_BUFFER_SIZE 4*INTERNAL_BUFF_SIZE
static uint16_t RecBuffer[REC_BUFFER_SIZE];

#define WR_BUFFER_SIZE  4*REC_BUFFER_SIZE
uint16_t WrBuffer[WR_BUFFER_SIZE];

#define NB_SAMPLES_RATE 7
static uint32_t SamplesRates[NB_SAMPLES_RATE] = {BSP_AUDIO_FREQUENCY_48K,
                                                 BSP_AUDIO_FREQUENCY_44K,
                                                 BSP_AUDIO_FREQUENCY_32K,
                                                 BSP_AUDIO_FREQUENCY_22K,
                                                 BSP_AUDIO_FREQUENCY_16K,
                                                 BSP_AUDIO_FREQUENCY_11K,
                                                 BSP_AUDIO_FREQUENCY_8K};
__IO uint8_t SampleRateCounter = 0;

__IO uint32_t ITCounter = 0;
Audio_BufferTypeDef  buffer_ctl;

/* Temporary data sample */
__IO uint32_t AUDIODataReady = 0, AUDIOBuffOffset = 0;

/* Variable used to replay audio sample (from play or record test)*/
extern uint32_t AudioTest;

/* Private function prototypes -----------------------------------------------*/
static void AudioRecord_SetHint(void);

extern __IO uint8_t NbLoop;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief Test Audio Hardware.
  *   The main objective of this test is to check the hardware connection of the 
  *   Audio peripheral.
  * @param  None
  * @retval None
  */

void AudioRecord_demo(void)
{
  /* Pointer to 1st or 2nd half of "internalBuffer[]"  */
  uint16_t* tmp_internalBuffer_half;      
  uint8_t RateBuffer[27];

  /* Enable audio play loop back*/
  uint32_t audio_loop_back = 0 ; 
  /* Variable to initialize only 1 time audio record */
  uint32_t audio_loop_back_init = RESET ; 
  uint32_t index = 0;

  AudioRecord_SetHint();

  buffer_ctl.offset = BUFFER_OFFSET_NONE;
  SampleRateCounter = ((NbLoop-1)%(NB_SAMPLES_RATE));
  
  /* Initialise Audio In at first BSP examples loop */
  if(NbLoop == 1)
  {
    if(BSP_AUDIO_IN_Init(SamplesRates[SampleRateCounter], DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR) != AUDIO_OK)
    {
      /* Record Error */
      Error_Handler();
    }
  }
  else
  {
    /* Update only the audio frequency with default DEFAULT_AUDIO_IN_FREQ*/
    BSP_AUDIO_IN_SetFrequency(SamplesRates[SampleRateCounter]);
  }

  /* Turn ON LED3: start record */
  BSP_LED_On(LED3);
  
  /* Start the record */
  if (BSP_AUDIO_IN_Record((uint16_t*)&RecBuffer[0], REC_BUFFER_SIZE) != AUDIO_OK)
  {
    /* Record Error */
    Error_Handler();
  }
  
  buffer_ctl.fptr = 0;
  
  AUDIODataReady = 0; 
   
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(0, 100, (uint8_t*)"Make sure that JP14 and JP16", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"set to 2-3 position", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"RECORD ONGOING", CENTER_MODE);
  snprintf((char*)RateBuffer, 27, "at Sample rate %ld Khz", (SamplesRates[SampleRateCounter])/1000);
  BSP_LCD_DisplayStringAt(0, 160, RateBuffer, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 190, (uint8_t *)"Press Wkup/Tamper push-button", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 205, (uint8_t *)"to stop recording", CENTER_MODE);
    
  /* Wait for the data to be ready with PCM form */
  while ((AUDIODataReady != 2) || (audio_loop_back == 1))
  {
    /* If a half-buffer is filled */
    if(buffer_ctl.offset != BUFFER_OFFSET_NONE)
    {
      /* Set pointer to the corresponding half of internal buffer */
      if(buffer_ctl.offset == BUFFER_OFFSET_HALF)
      {
        tmp_internalBuffer_half = (uint16_t*)RecBuffer;
      }
      else /* if(buffer_ctl.offset == BUFFER_OFFSET_FULL)*/
      {
        tmp_internalBuffer_half = ((uint16_t*)RecBuffer + (REC_BUFFER_SIZE/2));
      }
      
      buffer_ctl.offset = BUFFER_OFFSET_NONE;
      
      /* Copy the completed half of internal buffer to main bufer */
      for (index = 0; index < REC_BUFFER_SIZE/2; index++)
      {
        WrBuffer[(ITCounter * (REC_BUFFER_SIZE/2))+index] = (tmp_internalBuffer_half[index]);
      }
      
      /* Check if previous processing is performed before next half-buffer fill */
      if (buffer_ctl.offset != BUFFER_OFFSET_NONE)
      {
        /* Record Error */
        Error_Handler();
      }
      
      /* WR_BUFFER not full: remains size for 1 half of RecBuffer */
      if(ITCounter < ((WR_BUFFER_SIZE / (REC_BUFFER_SIZE/2)) -1))
      {
        /* WR_BUFFER half */
        if((ITCounter >= (((WR_BUFFER_SIZE/2) / (REC_BUFFER_SIZE/2)) -1)) && (AUDIODataReady != 1))
        {
          AUDIODataReady = 1;
          audio_loop_back = 1;
          AUDIOBuffOffset = WR_BUFFER_SIZE/2;
        }
        ITCounter++;
      }
      /* WR_BUFFER full */
      else
      {
        AUDIODataReady = 2;
        AUDIOBuffOffset = 0;
        ITCounter = 0;
      }
    }
    
    if (audio_loop_back == 1)
    {
     if (audio_loop_back_init == RESET)
     {
        /* Turn ON LED1: play recorded sample */
        BSP_LED_On(LED1);
        
        /* Play in the loop the recorded sample */

        /* Set variable to indicate play from record buffer */ 
        AudioTest = AUDIO_PLAY_RECORDED;

        /* Initialize audio OUT at REC_FREQ*/ 
        BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, 60, SamplesRates[SampleRateCounter]);

        /* Set Audio_out1, CN20 connector at Frame slots 0_2 by default */
        BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);

        /* Initialize audio OUT transfer in DMA circular mode and in stereo mode */ 
        BSP_AUDIO_OUT_ChangeAudioConfig(BSP_AUDIO_OUT_CIRCULARMODE|BSP_AUDIO_OUT_STEREOMODE);
        
        /* Play the recorded buffer*/
        BSP_AUDIO_OUT_Play((uint16_t*)WrBuffer, WR_BUFFER_SIZE);
        
        /* AudioPlay init done only at 1st time*/
        audio_loop_back_init = SET; 
     }
   }

    /* Exit recording when user press on Wkup/Tamper push-button */
    if((CheckForUserInput() == 1) && (AUDIODataReady > 0))
    { 
      /* Stop Player before close Test */
      if (BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW) != AUDIO_OK)
      {
        /* Audio Play Stop error */
        Error_Handler();
      }

      /* Turn OFF LED1: stop play */
      BSP_LED_Off(LED1);
  
      /* Stop Recorder before close Test */
      if (BSP_AUDIO_IN_Stop() != AUDIO_OK)
      {
        /* Audio record Stop error */
        Error_Handler();
      }

      /* Turn OFF LED3: stop record */
      BSP_LED_Off(LED3);
  
      break;
    }
  }
}

/**
  * @brief  Display Audio Play demo hint
  * @param  None
  * @retval None
  */
static void AudioRecord_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"AUDIO RECORD", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example is recording", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"audio samples on Micros", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"and play directly on CN20", CENTER_MODE); 

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
 }

/**
  * @brief Calculates the remaining file size and new position of the pointer.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
    buffer_ctl.offset = BUFFER_OFFSET_FULL;
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
    buffer_ctl.offset = BUFFER_OFFSET_HALF;
}

/**
  * @brief  Audio IN Error callback function
  * @param  pData
  * @retval None
  */
void BSP_AUDIO_IN_Error_Callback(void)
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
