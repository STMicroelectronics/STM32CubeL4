/**
  ******************************************************************************
  * @file    AudioRecorder.c
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
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "audiorecorder.h"
#include "stm32l476g_discovery_audio.h"

/** @addtogroup AUDIO_RECORDER_MODULE
  * @{
  */

/** @defgroup AUDIO_APPLICATION
  * @brief audio application routines
  * @{
  */

/* Private defines -----------------------------------------------------------*/
//#define RECORD_LOOPBACK 1
#define QSPI_STORAGE 1

/* Size (in bytes) of the buffer containing the recorded PCM samples */
#define RECORD_BUFFER_SIZE (uint32_t)1024

/* Audio record sample rate */
#define RECORD_SAMPLE_RATE BSP_AUDIO_FREQUENCY_16K

/* Size (in bytes) of a wav header */
#define WAV_HEADER_SIZE (uint32_t)44

#if defined(QSPI_STORAGE)
/* Size (in bytes) of the quad SPI flash memory (16 Mbytes)*/
#define QSPI_FLASH_SIZE (uint32_t)(0xFA0000)

/* Number of sectors of the quad SPI flah memory (256) */
#define QSPI_SECTOR_NB 256

/* Size (in bytes) of a  QSPI block (sub-sector): 4 KBytes */
#define QSPI_SUBSECTOR_SIZE 4096 

/* Offset of the 'File size' field in the wav header */
#define FILE_SIZE_OFFSET 4

/* Offset of the 'Data size' field in the wav header */
#define DATA_SIZE_OFFSET 40
#endif /* QSPI_STORAGE */

/* DBG */
#define QSPI_SECTOR_SIZE ((uint32_t)(64*1024))
/* DBG */

/* Private typedef -----------------------------------------------------------*/
/* Audio recorder services errors */
typedef enum
{
  AUDIORECORDER_ERROR_NONE = 0, 
  AUDIORECORDER_ERROR_IO,
  AUDIORECORDER_ERROR_HW, 
  AUDIORECORER_ERROR_INITIALIZATION,
} AudioRecorder_ErrorTypdef;

/* Audio recoder status */
typedef enum
{
  AUDIORECORDER_STOP = 0, 
  AUDIORECORDER_START,   
  AUDIORECORDER_EOF,   
  AUDIORECORDER_ERROR,  
} Audiorecorder_StateTypdef;

/* Recording buffer offset */ 
typedef enum
{
  RECORD_BUFFER_OFFSET_NONE = 0,  
  RECORD_BUFFER_OFFSET_HALF,  
  RECORD_BUFFER_OFFSET_FULL,     
} RecordBuffer_StateTypeDef;

/* Audio recorder context information */
typedef struct 
{
  osMessageQId              osMsgId;                             // Id of the message sent to the date streaming thread
  osThreadId                AudioRecorderThreadId;               // Data streaming thread
  uint16_t                  buffer[RECORD_BUFFER_SIZE];          // Record buffer 
  Audiorecorder_StateTypdef state;                               // Actual audio recorder status
#if defined(RECORD_LOOPBACK)
  uint16_t                  WrBuffer[RECORD_BUFFER_SIZE];        // Buffer used to transfer recorded data to the audio codec
  FlagStatus                AudioLoopbackInit;                   // Flag indicating whether or not audio loop back has been initialized
#elif defined(QSPI_STORAGE)
  FlagStatus                QUADSPI_full;                        // Flag indicating whether the QSPI flash memory is full
  uint32_t                  RecordTotalSize;                     // Cumulated number of recorded bytes */
  uint8_t                   QspiSubSector[QSPI_SUBSECTOR_SIZE];  // Buffer used to program a QSPI sub-sector */
  uint16_t                  NextSectorToErase;                   // Next sector to erase */
  FlagStatus                QUADSPI_full_notified;              // Flag indicating whether the QSPI flash memory full message has been displayed
  /* DBG */
  uint16_t                  SectorToProgram;
  /* DBG */
#endif /* RECORD_LOOPBACK */
} AudioRecorder_ContextTypeDef ;

/* WAV header structure */
typedef struct
{
  uint32_t  SampleRate;        /* Audio sampling frequency */
  uint32_t  NumChannels;       /* Number of channels: 1:Mono or 2:Stereo */
  uint32_t  BitsPerSample;     /* Number of bits per sample (16, 24 or 32) */
  uint32_t  AudioLength;       /* Total length of useful audio data (payload) */
  uint32_t  AudioStartAddr;    /* Relative start address of audio payload */
  uint32_t  RIFFchunksize;     /* The file header chunk size */
  uint16_t  FormatTag;         /* Audio file format: PCM = 1 */
  uint32_t  ByteRate;          /* Number of bytes per second  (sample rate * block align)  */
  uint16_t  BlockAlign;        /* channels * bits/sample / 8 */
  uint32_t  DataSize;          /* Audio data size */
}WAV_HeaderTypeDef;   

/* Private function prototypes -----------------------------------------------*/
/* Audio recorder kModule related functions */
static KMODULE_RETURN AudioRecorderPreExec(void);
static KMODULE_RETURN AudioRecorderExec(void);
static KMODULE_RETURN AudioRecorderPostExec(void);
static void AudioRecorder_Run(void);

/* Audio recorder services */
static AudioRecorder_ErrorTypdef  AudioRecorder_Init(void);
static AudioRecorder_ErrorTypdef  AudioRecorder_DeInit(void);
static AudioRecorder_ErrorTypdef  AudioRecorder_Start(uint32_t frequency);
static AudioRecorder_ErrorTypdef  AudioRecorder_Stop(void);

/* Audio recorder thread */
static void AudioRecorder_Thread(void const * argument);

/* Audio recorder callback functions */
static void AudioRecorder_TransferComplete_CallBack(void);
static void AudioRecorder_HalfTransfer_CallBack(void);
static void AudioRecorder_Error_Callback(void);

/* Internal services */
#if defined(QSPI_STORAGE)
static void AudioRecorder_WavHeaderInit(uint8_t* WavHeaderBuffer,
                                        WAV_HeaderTypeDef* pWavHeader);
static uint8_t AudioRecorder_QspiProgram (uint8_t* pData,
                                          uint32_t WriteAddr,
                                          uint32_t NbBytesToWrite,
                                          uint32_t* NbBytesWritten);
#endif /* QSPI_STORAGE */

/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleAudioRecorder =
{
  MODULE_AUDIORECORDER,
  AudioRecorderPreExec,
  AudioRecorderExec,
  AudioRecorderPostExec,
  NULL
};  

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static AudioRecorder_ContextTypeDef hAudioRecorder;


const tMenuItem AudioRecorderMenuItems[] =
{
    {"", 0, 0, TYPE_EXEC, MODULE_NONE, AudioRecorder_Run, NULL, NULL, NULL}
};

const tMenu AudioRecorderMenu = {
  "", AudioRecorderMenuItems, countof(AudioRecorderMenuItems), TYPE_EXEC, 2, 3
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialize AudioRecorder application 
  * @param  None.  
  * @retval None.
  */
KMODULE_RETURN AudioRecorderPreExec(void)
{

  /* Initialize audio recorder context */
  hAudioRecorder.state  = AUDIORECORDER_STOP;
#if defined(RECORD_LOOPBACK)
  hAudioRecorder.AudioLoopbackInit = RESET;   
#elif defined(QSPI_STORAGE)
  hAudioRecorder.QUADSPI_full = RESET;
  hAudioRecorder.NextSectorToErase = 0;
  hAudioRecorder.RecordTotalSize = 0;
  hAudioRecorder.QUADSPI_full_notified = RESET;
  /* DEBUG */
  hAudioRecorder.SectorToProgram = 0;
  /* DEBUG */
#endif /* RECORD_LOOPBACK */
  

#if defined(QSPI_STORAGE)
  /* Initialize the quad SPI flash interface */
  if (BSP_QSPI_Init() != QSPI_OK)
  {
    Error_Handler();
  }
  
  /* Erase first sector of the quad SPI flash memory */
  if (BSP_QSPI_Erase_Sector(0) != QSPI_OK)
  {
    Error_Handler();
  }
  hAudioRecorder.NextSectorToErase++;
  
  /* Wait until the device is ready (sector erase operation completed)
     prior continuing the recording startup process.
  */
  while (BSP_QSPI_GetStatus() != QSPI_OK)
  {
    HAL_Delay(10);
  }  
#endif /* QSPI_STORAGE */

  /* Initialize audio recorder */
  if (AudioRecorder_Init() != AUDIORECORDER_ERROR_NONE)
  {
    Error_Handler();
  }

  /* initialize audio input */
  if (BSP_AUDIO_IN_Init(RECORD_SAMPLE_RATE, 
                        DEFAULT_AUDIO_IN_BIT_RESOLUTION,
                        DEFAULT_AUDIO_IN_CHANNEL_NBR) != AUDIO_OK)
  {
    Error_Handler();
  }
  
  /* Set Callback function pointers */
  BSP_AUDIO_IN_RegisterCallbacks(AudioRecorder_Error_Callback,
                                 AudioRecorder_HalfTransfer_CallBack,
                                 AudioRecorder_TransferComplete_CallBack);
  
  return KMODULE_OK;
}

/**
  * @brief  Execute the audio recorder application menu
  * @param  None.
  * @note   run and display audio player Menu.  
  * @retval None.
  */
KMODULE_RETURN AudioRecorderExec(void)
{
  kMenu_Execute(AudioRecorderMenu);
  return KMODULE_OK;
}

/**
  * @brief  Uninitialize AudioRecorder application 
  * @param  None.  
  * @retval None.
  */
KMODULE_RETURN AudioRecorderPostExec(void)
{
#if defined(QSPI_STORAGE)
  uint32_t* pData;

  /* Read QSPI first sub-sector */
  if (BSP_QSPI_Read(hAudioRecorder.QspiSubSector, 0, QSPI_SUBSECTOR_SIZE) != QSPI_OK)
  {
    Error_Handler();
  }
  
  /* Update WAV header */
  hAudioRecorder.RecordTotalSize -= 8;
  pData = (uint32_t*)&hAudioRecorder.QspiSubSector[FILE_SIZE_OFFSET];
  *pData = hAudioRecorder.RecordTotalSize;
  
  hAudioRecorder.RecordTotalSize -= 44;
  pData = (uint32_t*)&hAudioRecorder.QspiSubSector[DATA_SIZE_OFFSET];
  *pData = hAudioRecorder.RecordTotalSize;

  /* Erase QSPI first sub-sector */
  if (BSP_QSPI_Erase_Block(0) != QSPI_OK)
  {
    Error_Handler();
  }
  
  /* Program QSPI first sub-sector */
  if (BSP_QSPI_Write(hAudioRecorder.QspiSubSector, 0, QSPI_SUBSECTOR_SIZE) != QSPI_OK)
  {
    Error_Handler();
  }
#endif

  /* De-initialize audio recorder */
  if (AudioRecorder_DeInit() != AUDIORECORDER_ERROR_NONE)
  {
    Error_Handler();
  }
  
#if defined(QSPI_STORAGE)
  /* De-initialize the quad SPI flash interface */
  if (BSP_QSPI_DeInit() != QSPI_OK)
  {
    Error_Handler();
  }
#endif /* QSPI_STORAGE */
  
#if defined(RECORD_LOOPBACK)
  if (BSP_AUDIO_OUT_DeInit() != AUDIO_OK)
  {
    Error_Handler();
  }
#endif /* RECORD_LOOPBACK */
  
  if (BSP_AUDIO_IN_DeInit() != AUDIO_OK)
  {
    Error_Handler();
  } 
  
  return KMODULE_OK;
}

/**
  * @brief  Run the audio recorder application 
  * @param  None.
  * @note   None.  
  * @retval None.
  */
static void AudioRecorder_Run(void)
{
  JOYState_TypeDef JOYState = JOY_NONE;
  
  /* Start audio recorder */
  if (AudioRecorder_Start(RECORD_SAMPLE_RATE) != AUDIORECORDER_ERROR_NONE)
  {
    Error_Handler();
  }
  
  /* Audio recorder event loop */
  do 
  {
    JOYState = kMenu_GetEvent(osWaitForever);
  } while (JOYState != JOY_LEFT );
  
  /* Stop audio recorder */
  if (AudioRecorder_Stop() != AUDIORECORDER_ERROR_NONE)
  {
    Error_Handler();
  }
}

/**
  * @brief  Audio recorder initialization
  * @param  None.
  * @retval Audio state.
  */
AudioRecorder_ErrorTypdef  AudioRecorder_Init(void)
{
  /* Create Audio Queue */
  osMessageQDef(AUDIO_Queue, 1, uint16_t);
  hAudioRecorder.osMsgId = osMessageCreate (osMessageQ(AUDIO_Queue), NULL); 
  
  /* Create Audio task */
  osThreadDef(osAudioRecorder_Thread, AudioRecorder_Thread, osPriorityHigh, 0, 4 * configMINIMAL_STACK_SIZE);
  hAudioRecorder.AudioRecorderThreadId = osThreadCreate (osThread(osAudioRecorder_Thread), NULL);  
  
  return AUDIORECORDER_ERROR_NONE;
}

/**
  * @brief  Audio recorder de-initialization
  * @param  None.
  * @retval Audio state.
  */
AudioRecorder_ErrorTypdef  AudioRecorder_DeInit(void)
{
  /* Delete RTOS resources */
  vQueueDelete(hAudioRecorder.osMsgId);
  osThreadTerminate(hAudioRecorder.AudioRecorderThreadId);

  return AUDIORECORDER_ERROR_NONE;
}

/**
  * @brief  Start audio record
  * @param  frequency: Audio frequency used to record the audio stream.
  * @retval Audio state.
  */
AudioRecorder_ErrorTypdef  AudioRecorder_Start(uint32_t frequency)
{
#if defined(QSPI_STORAGE)
  uint8_t WavHeaderBuff[WAV_HEADER_SIZE];
  WAV_HeaderTypeDef WavHeader;
  uint32_t NbBytesWritten;

  hAudioRecorder.RecordTotalSize = 0;
  
  /* Initialize the WAV header */
  WavHeader.SampleRate     = RECORD_SAMPLE_RATE;
  WavHeader.NumChannels    = DEFAULT_AUDIO_IN_CHANNEL_NBR;
  WavHeader.BitsPerSample  = DEFAULT_AUDIO_IN_BIT_RESOLUTION;
  WavHeader.AudioLength    = 0;
  WavHeader.AudioStartAddr = WAV_HEADER_SIZE;
  WavHeader.RIFFchunksize  = WAV_HEADER_SIZE;
  WavHeader.FormatTag      = 1;
  WavHeader.ByteRate       = WavHeader.SampleRate * (WavHeader.BitsPerSample/8) * WavHeader.NumChannels;
  WavHeader.BlockAlign     = WavHeader.NumChannels * (WavHeader.BitsPerSample/8);
  WavHeader.DataSize       = 0;

  /* Fill the wav file header */
  AudioRecorder_WavHeaderInit(WavHeaderBuff, &WavHeader);
  
  /* Write the WAV header to the QSPI flash */
  if (AudioRecorder_QspiProgram ((uint8_t*)WavHeaderBuff,
                                 0,
                                 WAV_HEADER_SIZE,
                                 &NbBytesWritten) != 0)
  {
    return AUDIORECORDER_ERROR_IO;
  }
  hAudioRecorder.RecordTotalSize += NbBytesWritten;
#endif /* QSPI_STORAGE */

  hAudioRecorder.state = AUDIORECORDER_START;
  
  if (BSP_AUDIO_IN_Record((uint16_t*)&hAudioRecorder.buffer[0], RECORD_BUFFER_SIZE) == AUDIO_OK)
  {
    osThreadResume(hAudioRecorder.AudioRecorderThreadId);
    return AUDIORECORDER_ERROR_NONE;
  }

  /* Switch off the green led */
  BSP_LED_Off(LED_GREEN);
  
  return AUDIORECORDER_ERROR_IO;
}

/**
  * @brief  Stop audio record.
  * @param  None.
  * @retval Audio state.
  */
AudioRecorder_ErrorTypdef  AudioRecorder_Stop(void)
{
  hAudioRecorder.state = AUDIORECORDER_STOP;

#if defined(RECORD_LOOPBACK)
  /* Stop the audio playback */
  BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_ON);
  HAL_Delay(20);
  BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
  hAudioRecorder.AudioLoopbackInit = RESET; 
#endif /* RECORD_LOOPBACK */

  osThreadSuspend(hAudioRecorder.AudioRecorderThreadId);
  HAL_Delay(20);
  BSP_AUDIO_IN_Stop();
    
#if defined(QSPI_STORAGE)
  /* Wait until the device is ready (no program/erase operation in progress) */
  while (BSP_QSPI_GetStatus() != QSPI_OK)
  {
    HAL_Delay(10);
  };
#endif /* QSPI_STORAGE */  
 
  /* Switch off the green led */
  BSP_LED_Off(LED_GREEN);
  
  return AUDIORECORDER_ERROR_NONE;
}

/**
  * @brief  Callback invoked when second half of the record buffer has been 
  *         filled in.
  * @param  None
  * @retval None
  */
static void AudioRecorder_TransferComplete_CallBack(void)
{
  if(hAudioRecorder.state == AUDIORECORDER_START)
  {
    if (osMessagePut(hAudioRecorder.osMsgId, RECORD_BUFFER_OFFSET_FULL, 0) != osOK)
    {
      Error_Handler();
    }
  }
}

/**
  * @brief  Callback invoked when first half of the record buffer has been 
  *         filled in.
  * @param  None
  * @retval None
  */
static void AudioRecorder_HalfTransfer_CallBack(void)
{ 
  if(hAudioRecorder.state == AUDIORECORDER_START)
  {
    if (osMessagePut(hAudioRecorder.osMsgId, RECORD_BUFFER_OFFSET_HALF, 0) != osOK)
    {
      Error_Handler();
    }
  }
}

/**
  * @brief  Callback invoked when a DMA transfer fails.
  * @param  None
  * @retval None
  */
static void AudioRecorder_Error_Callback(void)
{
  hAudioRecorder.state = AUDIORECORDER_ERROR;
}

/**
  * @brief  Audio Record task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void AudioRecorder_Thread(void const * argument)
{
  osEvent event;  
  
#if defined(QSPI_STORAGE)
  uint32_t NbBytesWritten = 0;  
#endif /* RECORD_LOOPBACK */
  
  for(;;)
  {
    event = osMessageGet(hAudioRecorder.osMsgId, 100);
    
    if( event.status == osEventMessage )
    {
#if defined(RECORD_LOOPBACK)
      if (hAudioRecorder.state == AUDIORECORDER_START)
#elif defined(QSPI_STORAGE)
      if (   (hAudioRecorder.state == AUDIORECORDER_START)
          && (hAudioRecorder.QUADSPI_full == RESET))
#endif /* RECORD_LOOPBACK */
      {
        switch(event.value.v)
        {
        case RECORD_BUFFER_OFFSET_HALF:
#if defined(RECORD_LOOPBACK)
          memcpy(hAudioRecorder.WrBuffer, 
                 hAudioRecorder.buffer, 
                 RECORD_BUFFER_SIZE);
          
          if (hAudioRecorder.AudioLoopbackInit == RESET)
          {
            /* Play in the loop the recorded sample */
            /* Initialize audio OUT at REC_FREQ*/ 
            BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, 70, RECORD_SAMPLE_RATE);
            
            /* Initialize audio OUT transfer in DMA circular mode and in mono mode */ 
            BSP_AUDIO_OUT_ChangeAudioConfig(BSP_AUDIO_OUT_CIRCULARMODE|BSP_AUDIO_OUT_MONOMODE);
            
            /* Play the recorded buffer */
            BSP_AUDIO_OUT_Play((uint16_t*)hAudioRecorder.WrBuffer, RECORD_BUFFER_SIZE);
            
            /* AudioPlay init done only at 1st time */
            hAudioRecorder.AudioLoopbackInit = SET; 
          }

#elif defined(QSPI_STORAGE)
          if (AudioRecorder_QspiProgram ((uint8_t*)hAudioRecorder.buffer,
                                         hAudioRecorder.RecordTotalSize,
                                         RECORD_BUFFER_SIZE,
                                         &NbBytesWritten) != 0)
          {
            Error_Handler();
          }
          hAudioRecorder.RecordTotalSize += NbBytesWritten;
#endif /* RECORD_LOOPBACK */
          break;  
          
        case RECORD_BUFFER_OFFSET_FULL:
#if defined(RECORD_LOOPBACK)
          memcpy(&hAudioRecorder.WrBuffer[RECORD_BUFFER_SIZE/2], 
                 &hAudioRecorder.buffer[RECORD_BUFFER_SIZE/2],
                 RECORD_BUFFER_SIZE);
          
#elif defined(QSPI_STORAGE)
          if (AudioRecorder_QspiProgram ((uint8_t*)&hAudioRecorder.buffer[RECORD_BUFFER_SIZE/2],
                                         hAudioRecorder.RecordTotalSize,
                                         RECORD_BUFFER_SIZE,
                                         &NbBytesWritten) != 0)
          {
            Error_Handler();
          }          
          hAudioRecorder.RecordTotalSize += NbBytesWritten;
#endif /* RECORD_LOOPBACK */
          
          /* Toggle the green led */
          BSP_LED_Toggle(LED_GREEN);
          break;   
          
        default:
          break;
        }
        
#if defined(QSPI_STORAGE)
        /* If the device is ready (no erase operation in progress)
        start erasing the next QSPI sector.
        */
        if (   (BSP_QSPI_GetStatus() == QSPI_OK)
            && (hAudioRecorder.NextSectorToErase < QSPI_SECTOR_NB))
        {
          if (BSP_QSPI_Erase_Sector(hAudioRecorder.NextSectorToErase) != 0)
          {
            Error_Handler();
          }
          hAudioRecorder.NextSectorToErase++;
        }
#endif  /* QSPI_STORAGE */
        
      }
      
#if defined(QSPI_STORAGE)
      if ((hAudioRecorder.QUADSPI_full == SET) &&
          (hAudioRecorder.QUADSPI_full_notified == RESET))
        
      {
        BSP_LCD_GLASS_Clear(); 
        BSP_LCD_GLASS_DisplayString((uint8_t*)" FULL ");
        hAudioRecorder.QUADSPI_full_notified = SET;
      }
#endif  /* QSPI_STORAGE */
      
    }
  }
}

#if defined(QSPI_STORAGE)
/**
  * @brief  Write to the quad spi flash memory
  * @param  pData: Pointer to data to be written
  * @param  WriteAddr: Write start address
  * @param  NbBytesToWrite: Number of bytes to write
  * @param  NbBytesWritten: Number of bytes written
  * @retval None
  */
static uint8_t AudioRecorder_QspiProgram (uint8_t* pData,
                                          uint32_t WriteAddr,
                                          uint32_t NbBytesToWrite,
                                          uint32_t* NbBytesWritten)
{
  uint32_t Size = 0;
  
  /* DBG */
  hAudioRecorder.SectorToProgram = (WriteAddr/QSPI_SECTOR_SIZE);
  /* DBG */
  
  if (WriteAddr + NbBytesToWrite < QSPI_FLASH_SIZE)
  {
    Size = NbBytesToWrite;
  }
  else
  {
    Size = QSPI_FLASH_SIZE - WriteAddr;
    
    /* Set the QUADSPI_full flag */
    hAudioRecorder.QUADSPI_full = SET;
  }
  
  /* If an erase sector operation is in progress it will be suspended */
  if (BSP_QSPI_SuspendErase() != QSPI_OK)
  {
    return 1;
  } 

  /* Add a little delay to prevent latency issue on QPSI memory after suspend erase */
  HAL_Delay(1);
  
  /* Program the QSPI */
  if (BSP_QSPI_Write(pData, WriteAddr, NbBytesToWrite) != QSPI_OK)
  {
    return 1;
  } 
  
  /* If an erase sector operation was suspended it will be resumed */
  if (BSP_QSPI_ResumeErase() != QSPI_OK)
  {
    return 1;
  } 

  *NbBytesWritten = Size;
  
  return 0;
}

/**
  * @brief  Initialize the wave header file
  * @param  WavHeaderBuffer: Header Buffer to be filled
  * @param  pWavHeader: Pointer to the wave structure to be filled.
  * @retval None
  */
static void AudioRecorder_WavHeaderInit(uint8_t* WavHeaderBuffer, 
                                        WAV_HeaderTypeDef* pWavHeader)
{
  /* write chunkID, must be 'RIFF'  ----------------------------------------------*/
  WavHeaderBuffer[0] = 'R';
  WavHeaderBuffer[1] = 'I';
  WavHeaderBuffer[2] = 'F';
  WavHeaderBuffer[3] = 'F';

  /* Write the file length ----------------------------------------------------*/
  /* The sampling time: this value will be be written back at the end of the 
   recording opearation.  Example: 661500 Bytes = 0x000A17FC, byte[7]=0x00, byte[4]=0xFC */
  WavHeaderBuffer[4] = 0xFC; //pWavHeader->AudioLength
  WavHeaderBuffer[5] = 0x17;
  WavHeaderBuffer[6] = 0x0A;
  WavHeaderBuffer[7] = 0x00;

  /* Write the file format, must be 'WAVE' ------------------------------------*/
  WavHeaderBuffer[8]  = 'W';
  WavHeaderBuffer[9]  = 'A';
  WavHeaderBuffer[10] = 'V';
  WavHeaderBuffer[11] = 'E';

  /* Write the format chunk, must be'fmt ' --------------------------------------*/
  WavHeaderBuffer[12]  = 'f';
  WavHeaderBuffer[13]  = 'm';
  WavHeaderBuffer[14]  = 't';
  WavHeaderBuffer[15]  = ' ';

  /* Write the length of the 'fmt' data, must be 0x10 -------------------------*/
  WavHeaderBuffer[16]  = 0x10;
  WavHeaderBuffer[17]  = 0x00;
  WavHeaderBuffer[18]  = 0x00;
  WavHeaderBuffer[19]  = 0x00;

  /* Write the audio format, must be 0x01 (PCM) -------------------------------*/
  WavHeaderBuffer[20]  = 0x01;
  WavHeaderBuffer[21]  = 0x00;

  /* Write the number of channels, ie. 0x01 (Mono) ----------------------------*/
  WavHeaderBuffer[22]  = pWavHeader->NumChannels;
  WavHeaderBuffer[23]  = 0x00;

  /* Write the Sample Rate in Hz ----------------------------------------------*/
  /* Write Little Endian ie. 8000 = 0x00001F40 => byte[24]=0x40, byte[27]=0x00*/
  WavHeaderBuffer[24]  = (uint8_t)((pWavHeader->SampleRate & 0xFF));//0x40;
  WavHeaderBuffer[25]  = (uint8_t)((pWavHeader->SampleRate >> 8) & 0xFF);//0x1F;
  WavHeaderBuffer[26]  = (uint8_t)((pWavHeader->SampleRate >> 16) & 0xFF);//0x00;
  WavHeaderBuffer[27]  = (uint8_t)((pWavHeader->SampleRate >> 24) & 0xFF);//0x00;

  /* Write the Byte Rate ------------------------------------------------------*/
  WavHeaderBuffer[28]  = (uint8_t)((pWavHeader->ByteRate & 0xFF));
  WavHeaderBuffer[29]  = (uint8_t)((pWavHeader->ByteRate >> 8) & 0xFF);
  WavHeaderBuffer[30]  = (uint8_t)((pWavHeader->ByteRate >> 16) & 0xFF);
  WavHeaderBuffer[31]  = (uint8_t)((pWavHeader->ByteRate >> 24) & 0xFF);

  /* Write the block alignment ------------------------------------------------*/
  WavHeaderBuffer[32]  = pWavHeader->BlockAlign; 
  WavHeaderBuffer[33]  = 0x00;

  /* Write the number of bits per sample --------------------------------------*/
  WavHeaderBuffer[34]  = pWavHeader->BitsPerSample;
  WavHeaderBuffer[35]  = 0x00;

  /* Write the Data chunk, must be 'data' ---------------------------------------*/
  WavHeaderBuffer[36]  = 'd';
  WavHeaderBuffer[37]  = 'a';
  WavHeaderBuffer[38]  = 't';
  WavHeaderBuffer[39]  = 'a';

  /* Write the number of sample data ------------------------------------------*/
  /* This field will be overwritten at the end of the recording operation */
  WavHeaderBuffer[40]  = 0xFC;
  WavHeaderBuffer[41]  = 0x17;
  WavHeaderBuffer[42]  = 0x0A;
  WavHeaderBuffer[43]  = 0x00;
}
#endif /* QSPI_STORAGE */

/* End of private functions --------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */

