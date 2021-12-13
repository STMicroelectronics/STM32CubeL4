/**
  ******************************************************************************
  * @file    audiorecorder_app.c
  * @author  MCD Application Team
  * @brief   Audio recorder application functions
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
#include "audiorecorder_app.h"
#include "stm32l476g_eval_audio.h"

/** @addtogroup AUDIO_RECORDER_MODULE
  * @{
  */

/** @defgroup AUDIO_APPLICATION
  * @brief audio application routines
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define WAV_HEADER_SIZE 44
extern FIL MyRecordFile;     /* File Record object */

#define countof(a) (sizeof(a) / sizeof(a[0]))

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static AUDIORECORDER_ProcessTypdef haudiorecord;
static osMessageQId   AudioRecordEvent;
static osThreadId     AudioRecordThreadId = 0;

/* Private function prototypes -----------------------------------------------*/
static uint32_t AudioRecord_WavHeaderInit(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct);
static void AudioRecord_Thread(void const * argument);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes audio
  * @param  None.
  * @retval Audio state.
  */
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_Init(void)
{
  /* Initialize BSP audio Record  */
  BSP_AUDIO_IN_Init(SAI_AUDIO_FREQUENCY_16K, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR);

  /* Initialize internal audio structure */
  haudiorecord.state  = AUDIORECORDER_STOP;
  /* Create Audio Queue */
  osMessageQDef(AUDIO_Queue, 1, uint16_t);
  AudioRecordEvent = osMessageCreate (osMessageQ(AUDIO_Queue), NULL); 
  
  /* Create Audio task */
  osThreadDef(osAudioRecord_Thread, AudioRecord_Thread, osPriorityHigh, 0, 4 * configMINIMAL_STACK_SIZE);
  AudioRecordThreadId = osThreadCreate (osThread(osAudioRecord_Thread), NULL);  

  return AUDIORECORDER_ERROR_NONE;
}
/**
  * @brief  Get audio state
  * @param  None.
  * @retval Audio state.
  */
AUDIORECORDER_StateTypdef  AUDIORECORDER_GetState(void)
{
  return haudiorecord.state;
}

/**
  * @brief  Start audio record
  * @param  frequency: Audio frequency used to record the audio stream.
  * @retval Audio state.
  */
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_Start(WAVE_FormatTypeDef FileInfo)
{
  haudiorecord.SampleRate = FileInfo.SampleRate;
  haudiorecord.NumChannels = FileInfo.NbrChannels;
  haudiorecord.BitsPerSample = FileInfo.BitPerSample;

  /* Buffer used to store the audio file header */
  uint8_t RecHeaderTmp[WAV_HEADER_SIZE];
  WAVE_FormatTypeDef EncWavFmtStruct;
  uint32_t byteswritten;
  
  /* Initialize the encoder structure */
  EncWavFmtStruct.SubChunk1Size  = WAV_HEADER_SIZE;                       /* The file header chunk size                               */
  EncWavFmtStruct.SampleRate     = haudiorecord.SampleRate;               /* Audio sampling frequency                                 */
  EncWavFmtStruct.NbrChannels    = haudiorecord.NumChannels;              /* Number of channels: 1:Mono or 2:Stereo                   */
  EncWavFmtStruct.BitPerSample   = haudiorecord.BitsPerSample;            /* Number of bits per sample (16, 24 or 32)                 */
  EncWavFmtStruct.FileSize       = 0;                                     /* Total length of useful audio data (payload)              */
  EncWavFmtStruct.AudioFormat    = 1;                                     /* Audio file format: PCM = 1                               */
  EncWavFmtStruct.ByteRate       = (EncWavFmtStruct.SampleRate * \
                                   (EncWavFmtStruct.BitPerSample/8) * \
                                    EncWavFmtStruct.NbrChannels);         /* Number of bytes per second  (sample rate * block align)  */
  EncWavFmtStruct.BlockAlign     = EncWavFmtStruct.NbrChannels * \
                                  (EncWavFmtStruct.BitPerSample/8);       /* channels * bits/sample / 8                               */
  EncWavFmtStruct.SubChunk2Size  = 0;                                     /* Total length of useful audio data (payload)              */

  /* Fill the wav file header */
  if (AudioRecord_WavHeaderInit(RecHeaderTmp, &EncWavFmtStruct))
  {
    return AUDIORECORDER_ERROR_IO;
  }
  
  if(f_write(&MyRecordFile,RecHeaderTmp, WAV_HEADER_SIZE,(void *)&byteswritten) != FR_OK)
  {
    return AUDIORECORDER_ERROR_IO;
  }
  
  haudiorecord.RecordedSize = byteswritten;

  BSP_AUDIO_IN_SetFrequency(haudiorecord.SampleRate);
  if (BSP_AUDIO_IN_Record((uint16_t*)&haudiorecord.buffer[0], countof(haudiorecord.buffer)) == AUDIO_OK)
  {
    haudiorecord.state = AUDIORECORDER_START;
    osThreadResume(AudioRecordThreadId);
    return AUDIORECORDER_ERROR_NONE;
  }

  return AUDIORECORDER_ERROR_IO;

}

/**
  * @brief  Audio recorder process
  * @param  None.
  * @retval Audio state.
  */
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_Process(void)
{
  switch(haudiorecord.state)
  {
  case AUDIORECORDER_START:
    haudiorecord.state = AUDIORECORDER_START;
    break;    

  case AUDIORECORDER_ERROR:
     AUDIORECORDER_Stop();
    break;
    
  case AUDIORECORDER_STOP:
  default:
    break;
  }
  
  return AUDIORECORDER_ERROR_NONE;
}
/**
  * @brief  Audio recorder DeInit
  * @param  None.
  * @retval Audio state.
  */
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_DeInit(void)
{
  uint32_t byteswritten;

  vQueueDelete(AudioRecordEvent);
  osThreadTerminate(AudioRecordThreadId);
  
  if(haudiorecord.state != AUDIORECORDER_STOP)
  {
    HAL_Delay(20);
    haudiorecord.state = AUDIORECORDER_STOP;
    BSP_AUDIO_IN_Stop();
  
    haudiorecord.RecordedSize -= 8;

    /* Wav BlocSize */
    f_lseek(&MyRecordFile, 4);
    f_write(&MyRecordFile, (uint8_t*)&haudiorecord.RecordedSize, 1, (void *)&byteswritten);
    f_write(&MyRecordFile, (uint8_t*)((uint32_t)&haudiorecord.RecordedSize + 1), 1, (void *)&byteswritten);
    f_write(&MyRecordFile, (uint8_t*)((uint32_t)&haudiorecord.RecordedSize + 2), 1, (void *)&byteswritten);
    f_write(&MyRecordFile, (uint8_t*)((uint32_t)&haudiorecord.RecordedSize + 3), 1, (void *)&byteswritten);
    /* Wav DataSize */
    f_lseek(&MyRecordFile, 40);
    haudiorecord.RecordedSize -= 36;
    f_write(&MyRecordFile, (uint8_t*)&haudiorecord.RecordedSize, 1, (void *)&byteswritten);
    f_write(&MyRecordFile, (uint8_t*)((uint32_t)&haudiorecord.RecordedSize + 1), 1, (void *)&byteswritten);
    f_write(&MyRecordFile, (uint8_t*)((uint32_t)&haudiorecord.RecordedSize + 2), 1, (void *)&byteswritten);
    f_write(&MyRecordFile, (uint8_t*)((uint32_t)&haudiorecord.RecordedSize + 3), 1, (void *)&byteswritten);

    f_close(&MyRecordFile);
  }
  
  /* DeInitialize BSP audio Record  */
  BSP_AUDIO_IN_DeInit();

  return AUDIORECORDER_ERROR_NONE;
}

/**
  * @brief  Stop audio record.
  * @param  None.
  * @retval Audio state.
  */
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_Stop(void)
{
  uint32_t byteswritten;

  BSP_AUDIO_IN_Stop();
  HAL_Delay(20);
  
  osThreadSuspend(AudioRecordThreadId);
  haudiorecord.state = AUDIORECORDER_STOP;

  haudiorecord.RecordedSize -= 8;
  f_lseek(&MyRecordFile, 4);
  f_write(&MyRecordFile, (uint8_t*)&haudiorecord.RecordedSize, 1, (void *)&byteswritten);
  f_write(&MyRecordFile, (uint8_t*)((uint32_t)&haudiorecord.RecordedSize + 1), 1, (void *)&byteswritten);
  f_write(&MyRecordFile, (uint8_t*)((uint32_t)&haudiorecord.RecordedSize + 2), 1, (void *)&byteswritten);
  f_write(&MyRecordFile, (uint8_t*)((uint32_t)&haudiorecord.RecordedSize + 3), 1, (void *)&byteswritten);
  f_lseek(&MyRecordFile, 40);
  haudiorecord.RecordedSize -= 36;
  f_write(&MyRecordFile, (uint8_t*)&haudiorecord.RecordedSize, 1, (void *)&byteswritten);
  f_write(&MyRecordFile, (uint8_t*)((uint32_t)&haudiorecord.RecordedSize + 1), 1, (void *)&byteswritten);
  f_write(&MyRecordFile, (uint8_t*)((uint32_t)&haudiorecord.RecordedSize + 2), 1, (void *)&byteswritten);
  f_write(&MyRecordFile, (uint8_t*)((uint32_t)&haudiorecord.RecordedSize + 3), 1, (void *)&byteswritten);
  f_close(&MyRecordFile);
    
  return AUDIORECORDER_ERROR_NONE;
}


/**
  * @brief  Pause Audio record
  * @param  None.
  * @retval Audio state.
  */
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_Pause(void)
{
  osThreadSuspend(AudioRecordThreadId);  
  return AUDIORECORDER_ERROR_NONE;
}


/**
  * @brief  Resume Audio record
  * @param  None.
  * @retval Audio state.
  */
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_Resume(void)
{
  osThreadResume(AudioRecordThreadId);  
  return AUDIORECORDER_ERROR_NONE;
}

/**
  * @brief  Get rec file duration.
  * @param  None
  * @retval file duration.
  */
uint32_t AUDIORECORDER_GetDuration(void)
{
  return (haudiorecord.RecordedSize/(haudiorecord.SampleRate *
                                     haudiorecord.NumChannels *
                                     (haudiorecord.BitsPerSample/8)));
}

/**
  * @brief  Get the wav file information.
  * @param  file: wav file.
  * @param  info: pointer to wav file structure
  * @retval Audio state.
  */
AUDIORECORDER_ErrorTypdef  AUDIORECORDER_GetFileInfo(char* file, WAVE_FormatTypeDef* info)
{
  uint32_t numOfReadBytes;
  AUDIORECORDER_ErrorTypdef ret = AUDIORECORDER_ERROR_IO;
  FIL fsfile;
  
  if( f_open(&fsfile, file, FA_OPEN_EXISTING | FA_READ) == FR_OK) 
  {
    /* Fill the buffer to Send */
    if(f_read(&fsfile, info, sizeof(WAVE_FormatTypeDef), (void *)&numOfReadBytes) == FR_OK)
    {
      if((info->ChunkID == 0x46464952) && (info->AudioFormat == 1))
      {
        ret = AUDIORECORDER_ERROR_NONE;
      }
    }
    f_close(&fsfile);      
  }
  return ret;
}

/**
  * @brief  Initialize the wave header file
  * @param  pHeader: Header Buffer to be filled
  * @param  pWaveFormatStruct: Pointer to the wave structure to be filled.
  * @retval 0 if passed, !0 if failed.
  */
static uint32_t AudioRecord_WavHeaderInit(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct)
{
  /* write chunkID, must be 'RIFF'  ----------------------------------------------*/
  pHeader[0] = 'R';
  pHeader[1] = 'I';
  pHeader[2] = 'F';
  pHeader[3] = 'F';

  /* Write the file length ----------------------------------------------------*/
  /* The sampling time: this value will be be written back at the end of the 
   recording opearation.  Example: 661500 Bytes = 0x000A17FC, byte[7]=0x00, byte[4]=0xFC */
  pHeader[4] = 0xFC; //pWaveFormatStruct->AudioLength
  pHeader[5] = 0x17;
  pHeader[6] = 0x0A;
  pHeader[7] = 0x00;

  /* Write the file format, must be 'WAVE' ------------------------------------*/
  pHeader[8]  = 'W';
  pHeader[9]  = 'A';
  pHeader[10] = 'V';
  pHeader[11] = 'E';

  /* Write the format chunk, must be'fmt ' --------------------------------------*/
  pHeader[12]  = 'f';
  pHeader[13]  = 'm';
  pHeader[14]  = 't';
  pHeader[15]  = ' ';

  /* Write the length of the 'fmt' data, must be 0x10 -------------------------*/
  pHeader[16]  = 0x10;
  pHeader[17]  = 0x00;
  pHeader[18]  = 0x00;
  pHeader[19]  = 0x00;

  /* Write the audio format, must be 0x01 (PCM) -------------------------------*/
  pHeader[20]  = 0x01;
  pHeader[21]  = 0x00;

  /* Write the number of channels, ie. 0x01 (Mono) ----------------------------*/
  pHeader[22]  = pWaveFormatStruct->NbrChannels;
  pHeader[23]  = 0x00;

  /* Write the Sample Rate in Hz ----------------------------------------------*/
  /* Write Little Endian ie. 8000 = 0x00001F40 => byte[24]=0x40, byte[27]=0x00*/
  pHeader[24]  = (uint8_t)((pWaveFormatStruct->SampleRate & 0xFF));//0x40;
  pHeader[25]  = (uint8_t)((pWaveFormatStruct->SampleRate >> 8) & 0xFF);//0x1F;
  pHeader[26]  = (uint8_t)((pWaveFormatStruct->SampleRate >> 16) & 0xFF);//0x00;
  pHeader[27]  = (uint8_t)((pWaveFormatStruct->SampleRate >> 24) & 0xFF);//0x00;

  /* Write the Byte Rate ------------------------------------------------------*/
  pHeader[28]  = (uint8_t)((pWaveFormatStruct->ByteRate & 0xFF));
  pHeader[29]  = (uint8_t)((pWaveFormatStruct->ByteRate >> 8) & 0xFF);
  pHeader[30]  = (uint8_t)((pWaveFormatStruct->ByteRate >> 16) & 0xFF);
  pHeader[31]  = (uint8_t)((pWaveFormatStruct->ByteRate >> 24) & 0xFF);

  /* Write the block alignment ------------------------------------------------*/
  pHeader[32]  = pWaveFormatStruct->BlockAlign; 
  pHeader[33]  = 0x00;

  /* Write the number of bits per sample --------------------------------------*/
  pHeader[34]  = pWaveFormatStruct->BitPerSample;
  pHeader[35]  = 0x00;

  /* Write the Data chunk, must be 'data' ---------------------------------------*/
  pHeader[36]  = 'd';
  pHeader[37]  = 'a';
  pHeader[38]  = 't';
  pHeader[39]  = 'a';

  /* Write the number of sample data ------------------------------------------*/
  /* This variable will be written back at the end of the recording operation */
  pHeader[40]  = 0xFC;
  pHeader[41]  = 0x17;
  pHeader[42]  = 0x0A;
  pHeader[43]  = 0x00;
  
  /* Return 0 if all operations are OK */
  return 0;
}

/**
  * @brief  User callback when record buffer is filled.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  if(haudiorecord.state == AUDIORECORDER_START)
  {
    osMessagePut ( AudioRecordEvent, RECORDBUFFER_OFFSET_FULL, 0);
  }
}

/**
  * @brief  Manages the DMA Half Transfer complete event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{ 
  if(haudiorecord.state == AUDIORECORDER_START)
  {
    osMessagePut ( AudioRecordEvent, RECORDBUFFER_OFFSET_HALF, 0);
  }
}

/**
  * @brief  Audio IN Error callback function.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_Error_Callback(void)
{
  haudiorecord.state = AUDIORECORDER_ERROR;
}

/**
  * @brief  Audio Record task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void AudioRecord_Thread(void const * argument)
{
  uint32_t byteswritten = 0;

  osEvent event;  
  for(;;)
  {
    event = osMessageGet(AudioRecordEvent, 100 );
    
    if( event.status == osEventMessage )
    {
      if(haudiorecord.state == AUDIORECORDER_START)
      {
        switch(event.value.v)
        {
        case RECORDBUFFER_OFFSET_HALF:
          f_write(&MyRecordFile, &(haudiorecord.buffer[0]), sizeof(haudiorecord.buffer)/2, (void *)&byteswritten);
          haudiorecord.RecordedSize += byteswritten;
          break;  
          
        case RECORDBUFFER_OFFSET_FULL:
          f_write(&MyRecordFile, &(haudiorecord.buffer[countof(haudiorecord.buffer)/2]),sizeof(haudiorecord.buffer)/2, (void *)&byteswritten);
          haudiorecord.RecordedSize += byteswritten;
          break;   
          
        default:
          break;
        }
      }
    }
  }
}
/**
  * @}
  */

/**
  * @}
  */

