/**
  ******************************************************************************
  * @file    audiorecorder_app.h
  * @author  MCD Application Team
  * @brief   header of audio recorder application file
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
#ifndef __AUDIORECORDER_APP_H
#define __AUDIORECORDER_APP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported constants --------------------------------------------------------*/
#define RECORD_ACTIVE                 0x00
#define PAUSE_ACTIVE                  0x01
   
#define RECORD_BUFFER_SIZE            (INTERNAL_BUFF_SIZE * 8)

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  AUDIORECORDER_STOP = 0, 
  AUDIORECORDER_START,   
  AUDIORECORDER_PAUSE, 
  AUDIORECORDER_ERROR,  
  
}AUDIORECORDER_StateTypdef;

typedef enum
{
  RECORDBUFFER_OFFSET_NONE = 0,  
  RECORDBUFFER_OFFSET_HALF,  
  RECORDBUFFER_OFFSET_FULL,     
}
RECORDBUFFER_StateTypeDef;

typedef enum
{
  AUDIORECORDER_ERROR_NONE = 0, 
  AUDIORECORDER_ERROR_IO,
  AUDIORECORDER_ERROR_HW, 
  AUDIORECORDER_ERROR_MEM, 
  AUDIORECORDER_ERROR_FORMAT_NOTSUPPORTED,   
  
}AUDIORECORDER_ErrorTypdef;

typedef struct 
{
  uint16_t                    buffer[RECORD_BUFFER_SIZE];
  uint32_t                    SampleRate;
  uint32_t                    RecordedSize;
  uint32_t                    NumChannels;
  uint32_t                    BitsPerSample;
  AUDIORECORDER_StateTypdef   state;

}AUDIORECORDER_ProcessTypdef ;

/* Audio file information structure */
typedef struct 
{
  uint32_t   ChunkID;       /* 0: Header "RIFF"                                            */
  uint32_t   FileSize;      /* 4: Total length of useful audio data (payload)              */
  uint32_t   FileFormatID;  /* 8: File Format "WAVE"                                       */
  uint32_t   SubChunk1ID;   /* 12: Format Block Identify "fmt"                             */
  uint32_t   SubChunk1Size; /* 16: The file header chunk size                              */
  uint16_t   AudioFormat;   /* 20: Audio file format: PCM = 1                              */ 
  uint16_t   NbrChannels;   /* 22: Number of channels: 1:Mono or 2:Stereo                  */
  uint32_t   SampleRate;    /* 24: Audio sampling frequency                                */
  
  uint32_t   ByteRate;      /* 28: Number of bytes per second  (sample rate * block align) */
  uint16_t   BlockAlign;    /* 32: channels * bits/sample / 8                              */
  uint16_t   BitPerSample;  /* 34: Number of bits per sample (16, 24 or 32)                */
  uint32_t   SubChunk2ID;   /* 36: Data Block Identify "data"                              */   
  uint32_t   SubChunk2Size; /* 40: Total length of useful audio data (payload)             */    
}WAVE_FormatTypeDef ;

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_Init(void);
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_Start(WAVE_FormatTypeDef FileInfo);
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_Stop(void);
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_Pause(void);
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_Resume(void);
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_Mute(uint8_t state);
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_SelectFile(char* file);
AUDIORECORDER_StateTypdef  AUDIORECORDER_GetState(void);
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_Process(void);
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_DeInit(void);
uint32_t                   AUDIORECORDER_GetDuration(void);
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_GetFileInfo(char* file, WAVE_FormatTypeDef* info);
uint32_t                   AUDIORECORDER_GetProgress (void);
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_SetVolume(uint32_t volume);
uint32_t                   AUDIORECORDER_GetVolume(void);
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_NotifyEndOfFile(void);
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_SetPosition(uint32_t position);

#ifdef __cplusplus
}
#endif

#endif /*__AUDIORECORDER_APP_H */

