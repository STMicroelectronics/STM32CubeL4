/**
  ******************************************************************************
  * @file    audioplayer.c
  * @author  MCD Application Team   
  * @brief   Audio player application functions
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
#include <string.h>
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "audioplayer.h"
#include "stm32l476g_discovery_audio.h"
#include <stdio.h>

/** @addtogroup AUDIO_PLAYER_MODULE
  * @{
  */

/** @defgroup AUDIO_APPLICATION
  * @brief audio application routines
  * @{
  */

/* Private defines -----------------------------------------------------------*/
/* Size (in bytes) of the buffer containing the PCM samples */
#define AUDIO_BUFFER_SIZE          (1024 * 4)
   
/* Audio file address in FLASH memory*/ 
#define AUDIO_FILE_ADDRESS         (uint32_t)0x08020000  

/* Size (in bytes) of a WAV header */
#define WAV_HEADER_SIZE 44

/* Audio volume level range (min..max) */
#define AUDIO_VOLUME_MAX 100
#define AUDIO_VOLUME_MIN 50

/* Private typedef -----------------------------------------------------------*/
/* Audio player services errors */
typedef enum
{
  AUDIOPLAYER_ERROR_NONE = 0,
  AUDIOPLAYER_ERROR_IO,
  AUDIOPLAYER_ERROR_HW,
  AUDIOPLAYER_ERROR_MEM,
  AUDIOPLAYER_ERROR_FORMAT_NOTSUPPORTED,
  AUDIOPLAYER_ERROR_INITIALIZATION,
  AUDIOPLAYER_ERROR_FILE_NOT_FOUND,
  AUDIOPLAYER_ERROR_OS
}AudioPlayer_ErrorTypeDef;

/* Audio player status */
typedef enum
{
  AUDIOPLAYER_STOPPED = 0, 
  AUDIOPLAYER_STARTED,   
  AUDIOPLAYER_PAUSED, 
  AUDIOPLAYER_EOF,   
  AUDIOPLAYER_ERROR,  
}AudioPlayer_StateTypeDef;

/* Playback buffer offset */
typedef enum
{
  PLAYER_BUFFER_OFFSET_NONE = 0,  
  PLAYER_BUFFER_OFFSET_HALF,  
  PLAYER_BUFFER_OFFSET_FULL,     
} BUFFER_StateTypeDef;

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
}WAV_HeaderTypeDef;

/* Audio source (FLASH v.s. Quad SPI flash) */
typedef enum {
  AUDIO_SOURCE_FLASH = 0,
  AUDIO_SOURCE_QSPI,
  /* Latest entry shall be AUDIO_SOURCE_NONE and AUDIO_SOURCES_NB */
  AUDIO_SOURCE_NONE,
  AUDIO_SOURCES_NB
} AudioSource_TypeDef;

/* Audio player context information */
typedef struct {
  uint16_t                  buffer[AUDIO_BUFFER_SIZE/2];  // Buffer containing the PCM samples to play  
  uint32_t                  volume;                       // Actual volume level
  AudioPlayer_StateTypeDef  state;                        // Actual audio player status
  uint32_t                  NbAudioSources;               // Number of audio sources
  AudioSource_TypeDef       AudioSource;                  // Actual audio source
  WAV_HeaderTypeDef         WavHeader[AUDIO_SOURCES_NB];  // Wav headers (onbe per audio source)
  uint32_t                  start;                        // Offset of the first audio sample
  uint32_t                  end;                          // Offset of the last audio sample
  uint32_t                  read;                         // Actual read offset
} AudioPlayer_ContextTypeDef;

/* Private function prototypes -----------------------------------------------*/
/* Audio player kModule related functions */
static KMODULE_RETURN AudioPlayerPreExec(void);
static KMODULE_RETURN AudioPlayer_Exec(void);
static KMODULE_RETURN AudioPlayerPostExec(void);
static void AudioPlayer_Run(void);

/* Audio player services */
static AudioPlayer_ErrorTypeDef  AudioPlayer_Init(void);
static AudioPlayer_ErrorTypeDef  AudioPlayer_DeInit(void);
static AudioPlayer_ErrorTypeDef  AudioPlayer_Start(uint32_t frequency);
static AudioPlayer_ErrorTypeDef  AudioPlayer_Stop(void);
static AudioPlayer_ErrorTypeDef  AudioPlayer_Pause(void);
static AudioPlayer_ErrorTypeDef  AudioPlayer_Resume(void);
static AudioPlayer_ErrorTypeDef  AudioPlayer_SetVolume(uint32_t volume);
static uint32_t                  AudioPlayer_GetVolume(void);

/* Audio player thread */
static void AudioPlayer_Thread(void const * argument);

/* Audio player callback functions */
static void AudioPlayer_TransferComplete_CallBack(void);
static void AudioPlayer_HalfTransfer_CallBack(void);
static void AudioPlayer_Error_CallBack(void);

/* Internal services */
static uint8_t IsValidWAVHeader(WAV_HeaderTypeDef* WAV_header);
static uint8_t ReadAudio( uint16_t* buff,
                          uint32_t btr,
                          uint32_t* br);
static uint8_t PlaylistInit(void);
static AudioSource_TypeDef AudioSourceSelect(void);


/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleAudioPlayer =
{
  MODULE_AUDIOPLAYER,
  AudioPlayerPreExec,
  AudioPlayer_Exec,
  AudioPlayerPostExec,
  NULL
};  

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static AudioPlayer_ContextTypeDef hAudioPlayer;

static osMessageQId   AudioEvent;
static osThreadId     AudioThreadId = 0;

const tMenuItem AudioPlayerMenuItems[] =
{
    {"", 0, 0, SEL_EXEC, MODULE_NONE, AudioPlayer_Run, NULL, NULL, NULL},
};

const tMenu AudioPlayerMenu = {
  NULL, AudioPlayerMenuItems, countof(AudioPlayerMenuItems), TYPE_EXEC, 2, 3
  };

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize AudioPlayer application 
  * @param  None.  
  * @retval None.
  */
KMODULE_RETURN AudioPlayerPreExec(void)
{
  /* Initialize internal audio structure */
  memset(&hAudioPlayer, 0, sizeof(AudioPlayer_ContextTypeDef));
  hAudioPlayer.state  = AUDIOPLAYER_STOPPED;
  hAudioPlayer.volume = AUDIO_DEFAULT_VOLUME;  

  /* Initialize the quad SPI flash interface */
  if (BSP_QSPI_Init() != QSPI_OK)
  {
    Error_Handler();
  }
  
  /* Initialize the playlist */
  if (PlaylistInit() != 0)
  {
    Error_Handler();
  }
  
  if(DemoStarted !=  RESET)
  {  
    hAudioPlayer.AudioSource = AudioSourceSelect();
  }
  else
  {
    hAudioPlayer.AudioSource = AUDIO_SOURCE_NONE;
  }
  
  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, 
                        hAudioPlayer.volume, 
                        hAudioPlayer.WavHeader[hAudioPlayer.AudioSource].SampleRate) != AUDIO_OK)
  {
    Error_Handler();
  }

  /* Configure DMA circular mode and channel number */
  if (hAudioPlayer.WavHeader[hAudioPlayer.AudioSource].NbrChannels == 1)
  {
    BSP_AUDIO_OUT_ChangeAudioConfig(BSP_AUDIO_OUT_CIRCULARMODE | BSP_AUDIO_OUT_MONOMODE);
  }
  else
  {
    BSP_AUDIO_OUT_ChangeAudioConfig(BSP_AUDIO_OUT_CIRCULARMODE | BSP_AUDIO_OUT_STEREOMODE);
  }

  /* Set Callback function pointers */
  BSP_AUDIO_OUT_RegisterCallbacks(AudioPlayer_Error_CallBack,
                                  AudioPlayer_HalfTransfer_CallBack,
                                  AudioPlayer_TransferComplete_CallBack);

  /* Switch off the green led */
  BSP_LED_Off(LED_GREEN);
  
  return KMODULE_OK;
}

/**
  * @brief  Execute the audio player application menu
  * @param  None.
  * @note   run and display audio player Menu.  
  * @retval None.
  */
static KMODULE_RETURN AudioPlayer_Exec(void)
{
  kMenu_Execute(AudioPlayerMenu);
  return KMODULE_OK;
}

/**
  * @brief  Uninitialize AudioPlayer application 
  * @param  None.  
  * @retval None.
  */
KMODULE_RETURN AudioPlayerPostExec(void)
{
  if (BSP_AUDIO_OUT_DeInit() != AUDIO_OK)
  {
    Error_Handler();
  }

  /* De-initialize the quad SPI flash interface */
  if (BSP_QSPI_DeInit() != QSPI_OK)
  {
    Error_Handler();
  }

  /* Switch off the green led */
  BSP_LED_Off(LED_GREEN);
  
  return KMODULE_OK;
}

/**
  * @brief  Run the audio player application 
  * @param  None.
  * @note   None.  
  * @retval None.
  */
void AudioPlayer_Run(void)
{
  uint32_t volume;
  JOYState_TypeDef JOYState = JOY_NONE;
  
  if (hAudioPlayer.AudioSource == AUDIO_SOURCE_NONE)
  {
    BSP_LCD_GLASS_ScrollSentence((uint8_t*)"      NO AUDIO DATA FOUND IN BOTH FLASH AND QSPI FLASH MEMORIES", 1, SCROLL_SPEED_HIGH);
  }
  else
  {
    /* Initialize audio player */
    if (AudioPlayer_Init() != AUDIOPLAYER_ERROR_NONE)
    {
      Error_Handler();
    }
    else
    {
      /* Start audio player */
      if (AudioPlayer_Start(hAudioPlayer.WavHeader[hAudioPlayer.AudioSource].SampleRate) != AUDIOPLAYER_ERROR_NONE)
      {
        Error_Handler();
      }
    
      /* Audio player event loop */
      do 
      {
        JOYState = kMenu_GetEvent(osWaitForever);
        switch(JOYState)
        {
        /* Pause/Resume handling */
        case JOY_SEL:
          if (hAudioPlayer.state == AUDIOPLAYER_STARTED)
          {
            if (AudioPlayer_Pause() != AUDIOPLAYER_ERROR_NONE)
            {
              Error_Handler();
            }
          }
          else if (hAudioPlayer.state == AUDIOPLAYER_PAUSED)
          {
            if (AudioPlayer_Resume() != AUDIOPLAYER_ERROR_NONE)
            {
              Error_Handler();
            }         
          }
          break;
        
        /* Vol + handling */
        case JOY_UP:
          volume = AudioPlayer_GetVolume();
          if (volume < AUDIO_VOLUME_MAX)
          {
            volume++;
            if (AudioPlayer_SetVolume(volume) != AUDIOPLAYER_ERROR_NONE)
            {
              Error_Handler();
            }
          }
          break;
        
        /* Vol - handling */
        case JOY_DOWN:
          volume = AudioPlayer_GetVolume();
          if (volume > AUDIO_VOLUME_MIN)
          {
            volume--;
            if (AudioPlayer_SetVolume(volume) != AUDIOPLAYER_ERROR_NONE)
            {
              Error_Handler();
            }      
          }
          break;
        
        default:
          break;
        }
      } while(JOYState != JOY_LEFT);
    
      /* Stop audio player before close application */
      if (AudioPlayer_Stop() != AUDIOPLAYER_ERROR_NONE)
      {
        Error_Handler();
      }
    
      /* De-initialize audio player */
      if (AudioPlayer_DeInit() != AUDIOPLAYER_ERROR_NONE)
      {
        Error_Handler();
      }
    }
  }
}

/**
  * @brief  Initializes audio
  * @param  None.
  * @retval Audio state.
  */
AudioPlayer_ErrorTypeDef  AudioPlayer_Init(void)
{
  AudioPlayer_ErrorTypeDef status = AUDIOPLAYER_ERROR_NONE;
  
  /* Create audio player Queue */
  osMessageQDef(AUDIO_Queue, 1, uint16_t);
  AudioEvent = osMessageCreate (osMessageQ(AUDIO_Queue), NULL);
  if(AudioEvent == NULL)
  {
    status = AUDIOPLAYER_ERROR_OS;
  }
  else
  {
    /* Create audio player task */
    osThreadDef(osAudioPlayer_Thread, AudioPlayer_Thread, osPriorityHigh, 0, 4 * configMINIMAL_STACK_SIZE);
    AudioThreadId = osThreadCreate (osThread(osAudioPlayer_Thread), NULL);
    if(AudioThreadId == NULL)
    {
      status = AUDIOPLAYER_ERROR_OS;
    }
  }
  return status;
}

/**
  * @brief  Audio player DeInit
  * @param  None.
  * @retval Audio state.
  */
AudioPlayer_ErrorTypeDef  AudioPlayer_DeInit(void)
{
  AudioPlayer_ErrorTypeDef status = AUDIOPLAYER_ERROR_NONE;

  vQueueDelete(AudioEvent);
  if(osThreadTerminate(AudioThreadId) != osOK)
  {
    status = AUDIOPLAYER_ERROR_OS;
  }
  return status;
}

/**
  * @brief  Play audio stream
  * @param  frequency: Audio frequency used to play the audio stream.
  * @retval Audio state.
  */
AudioPlayer_ErrorTypeDef  AudioPlayer_Start(uint32_t frequency)
{
  AudioPlayer_ErrorTypeDef status = AUDIOPLAYER_ERROR_NONE;
  uint32_t numOfReadBytes;
  
  /* Initialize audio descriptor */
  if (hAudioPlayer.AudioSource == AUDIO_SOURCE_FLASH)
  {
    hAudioPlayer.start = AUDIO_FILE_ADDRESS + WAV_HEADER_SIZE;
    hAudioPlayer.end   = hAudioPlayer.start + hAudioPlayer.WavHeader[AUDIO_SOURCE_FLASH].SubChunk2Size;
    hAudioPlayer.read  = hAudioPlayer.start;
  }
  else
  {
    hAudioPlayer.start = WAV_HEADER_SIZE;
    hAudioPlayer.end   = hAudioPlayer.WavHeader[AUDIO_SOURCE_QSPI].SubChunk2Size;
    hAudioPlayer.read  = hAudioPlayer.start;
  }
  
  /* Fill whole buffer @ first time */
  if (ReadAudio(hAudioPlayer.buffer, 
                AUDIO_BUFFER_SIZE, 
                &numOfReadBytes) == 0)
  { 
    if(numOfReadBytes != 0)
    {
      if (BSP_AUDIO_OUT_SetFrequency(frequency) != AUDIO_OK)
      {
        status = AUDIOPLAYER_ERROR_HW;
      }
      else
      {            
        if(osThreadResume(AudioThreadId) != osOK)
        {
          status = AUDIOPLAYER_ERROR_OS;
        }
        else
        {
          if(BSP_AUDIO_OUT_Play(hAudioPlayer.buffer, AUDIO_BUFFER_SIZE/2) != AUDIO_OK)
          {
            status = AUDIOPLAYER_ERROR_HW;
          }
          else
          {
            hAudioPlayer.state = AUDIOPLAYER_STARTED;
            BSP_LCD_GLASS_Clear(); 
            BSP_LCD_GLASS_DisplayString((uint8_t*)"PLAY");
            /* Switch off green led */
            BSP_LED_Off(LED_GREEN);
          }
        }
      }
    }
    else
    {
      status = AUDIOPLAYER_ERROR_MEM;
    }
  }
  else
  {
    status = AUDIOPLAYER_ERROR_MEM;
  }
  return status;
}

/**
  * @brief  Stop audio stream.
  * @param  None.
  * @retval Audio state.
  */
AudioPlayer_ErrorTypeDef  AudioPlayer_Stop(void)
{
  AudioPlayer_ErrorTypeDef status = AUDIOPLAYER_ERROR_NONE;

  if(osThreadSuspend(AudioThreadId) != osOK)
  {
    status = AUDIOPLAYER_ERROR_OS;
  }
  else
  {
    hAudioPlayer.state = AUDIOPLAYER_STOPPED;
    if(BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW) != AUDIO_OK)
    {
      status = AUDIOPLAYER_ERROR_HW;
    }
    else
    {
      hAudioPlayer.state = AUDIOPLAYER_STOPPED;
      /* Switch off green led */
      BSP_LED_Off(LED_GREEN);
    }
  }
  return status;
}

/**
  * @brief  Pause Audio stream
  * @param  None.
  * @retval Audio state.
  */
AudioPlayer_ErrorTypeDef  AudioPlayer_Pause(void)
{
  AudioPlayer_ErrorTypeDef status = AUDIOPLAYER_ERROR_NONE;
  
  if(osThreadSuspend(AudioThreadId) != osOK)
  {
    status = AUDIOPLAYER_ERROR_OS;
  }
  else
  {
    if(BSP_AUDIO_OUT_Pause() != AUDIO_OK)
    {
      status = AUDIOPLAYER_ERROR_HW;
    }
    else
    {
      hAudioPlayer.state = AUDIOPLAYER_PAUSED;
      BSP_LCD_GLASS_Clear();
      BSP_LCD_GLASS_DisplayString((uint8_t*)"PAUSED");
      
      /* Switch on the green led */
      BSP_LED_On(LED_GREEN);
    }
  }
  return status;
}


/**
  * @brief  Resume Audio stream
  * @param  None.
  * @retval Audio state.
  */
AudioPlayer_ErrorTypeDef  AudioPlayer_Resume(void)
{
  AudioPlayer_ErrorTypeDef status = AUDIOPLAYER_ERROR_NONE;
  
  if(osThreadResume(AudioThreadId) != osOK)
  {
    status = AUDIOPLAYER_ERROR_OS;
  }
  else
  {
    if(BSP_AUDIO_OUT_Resume() != AUDIO_OK)
    {
      status = AUDIOPLAYER_ERROR_HW;
    }
    else
    {
      hAudioPlayer.state = AUDIOPLAYER_STARTED;
      BSP_LCD_GLASS_Clear(); 
      BSP_LCD_GLASS_DisplayString((uint8_t*)"PLAY");
    }
  }
  return status;
}

/**
  * @brief  Get audio volume
  * @param  None.
  * @retval Audio volume.
  */
uint32_t  AudioPlayer_GetVolume(void)
{
  return hAudioPlayer.volume;
}

/**
  * @brief  Set audio volume
  * @param  Volume: Volume level to be set in percentage from 0% to 100% (0 for 
  *         Mute and 100 for Max volume level).
  * @retval Audio state.
  */
AudioPlayer_ErrorTypeDef  AudioPlayer_SetVolume(uint32_t volume)
{
  AudioPlayer_ErrorTypeDef status = AUDIOPLAYER_ERROR_NONE;
  char VolumeString[14];
  
  if(BSP_AUDIO_OUT_SetVolume(volume) == AUDIO_OK)
  {
    hAudioPlayer.volume = volume;
    sprintf(VolumeString, "VOL%lu", hAudioPlayer.volume);
    BSP_LCD_GLASS_Clear();
    BSP_LCD_GLASS_DisplayString((uint8_t*)VolumeString);
  }
  else
  {
    status = AUDIOPLAYER_ERROR_HW;
  }
  return status;
}

/**
  * @brief  Manages the DMA Transfer complete interrupt.
  * @param  None
  * @retval None
  */
static void AudioPlayer_TransferComplete_CallBack(void)
{
  if(hAudioPlayer.state == AUDIOPLAYER_STARTED)
  {
    if(osMessagePut(AudioEvent, PLAYER_BUFFER_OFFSET_FULL, 0) != osOK)
    {
      Error_Handler();
    }
  }
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
static void AudioPlayer_HalfTransfer_CallBack(void)
{ 
  if(hAudioPlayer.state == AUDIOPLAYER_STARTED)
  {
    if(osMessagePut(AudioEvent, PLAYER_BUFFER_OFFSET_HALF, 0) != osOK)
    {
      Error_Handler();
    }
  }
}

/**
  * @brief  Manages the DMA FIFO error interrupt.
  * @param  None
  * @retval None
  */
static void AudioPlayer_Error_CallBack(void)
{
  hAudioPlayer.state = AUDIOPLAYER_ERROR;
}

/**
  * @brief  Initializes the playlist
  * @param  None
  * @retval 0: playlist initialized, 1: playlist initialization failed
  */
static uint8_t PlaylistInit(void)
{
  uint8_t status = 0;
  
  /* Retrieve WAV header from Flash memory */
  if(memcpy(&hAudioPlayer.WavHeader[AUDIO_SOURCE_FLASH],
            (void*)AUDIO_FILE_ADDRESS,
            WAV_HEADER_SIZE) == NULL)
  {
    status = 1;
  }
  else
  {
    if(IsValidWAVHeader(&hAudioPlayer.WavHeader[AUDIO_SOURCE_FLASH]))
    {
      hAudioPlayer.NbAudioSources++;
    }
    
    /* Retrieve WAV header from quad SPI flash */
    if(BSP_QSPI_Read((uint8_t*)&hAudioPlayer.WavHeader[AUDIO_SOURCE_QSPI],
                     0,
                     WAV_HEADER_SIZE) != QSPI_OK)
    {
      status = 1;
    }
    else
    {
      if(IsValidWAVHeader(&hAudioPlayer.WavHeader[AUDIO_SOURCE_QSPI]))
      {
        hAudioPlayer.NbAudioSources++;
      }
    }
  }
  return status;
}

/**
  * @brief  Selects the audio source (FLASH v.s. QSPI)
  * @param  None
  * @retval Selected audio source (AUDIO_SOURCE_FLASH or AUDIO_SOURCE_QSPI)
  */
static AudioSource_TypeDef AudioSourceSelect(void)
{
  AudioSource_TypeDef audio_source;
  JOYState_TypeDef JOYState = JOY_NONE;
  
  if (hAudioPlayer.NbAudioSources == 1)
  {
    if (IsValidWAVHeader(&hAudioPlayer.WavHeader[AUDIO_SOURCE_FLASH]))
    {
      audio_source = AUDIO_SOURCE_FLASH;
    }
    else
    {
      audio_source = AUDIO_SOURCE_QSPI;
    }
  }
  else if (hAudioPlayer.NbAudioSources == 2)
  {
    audio_source = AUDIO_SOURCE_FLASH;
    BSP_LCD_GLASS_Clear(); 
    BSP_LCD_GLASS_DisplayString((uint8_t*)"FLASH");
    do 
    {
      JOYState = kMenu_GetEvent(osWaitForever);
      switch(JOYState)
      {
      /* Vol + handling */
      case JOY_UP:
      case JOY_DOWN:
        BSP_LCD_GLASS_Clear(); 
        if (audio_source == AUDIO_SOURCE_FLASH)
        {
          audio_source = AUDIO_SOURCE_QSPI;
          BSP_LCD_GLASS_DisplayString((uint8_t*)"QSPI");
        }
        else
        {
          audio_source = AUDIO_SOURCE_FLASH;
          BSP_LCD_GLASS_DisplayString((uint8_t*)"FLASH");
        }
        break;
      /* Exit subchoice list */
      case JOY_LEFT:
        audio_source = AUDIO_SOURCE_NONE;
        break;
      default:
        break;
      }
    } while((JOYState != JOY_RIGHT) && (JOYState != JOY_SEL) && (audio_source != AUDIO_SOURCE_NONE));
  }
  else
  {
    audio_source = AUDIO_SOURCE_NONE; /* init case */
  }
  return audio_source;
}

/**
  * @brief  Audio task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void AudioPlayer_Thread(void const * argument)
{
  uint32_t numOfReadBytes;
  osEvent event;
  for(;;)
  {
    event = osMessageGet(AudioEvent, 100);
    
    if(event.status == osEventMessage)
    {
      if(hAudioPlayer.state == AUDIOPLAYER_STARTED)
      {
        switch(event.value.v)
        {
        case PLAYER_BUFFER_OFFSET_HALF:
          if (ReadAudio(&hAudioPlayer.buffer[0],
                        AUDIO_BUFFER_SIZE/2,
                        &numOfReadBytes) == 0)
          {
            if (numOfReadBytes != AUDIO_BUFFER_SIZE/2)
            {
              hAudioPlayer.state = AUDIOPLAYER_EOF;
            }
          }
          else
          {
            hAudioPlayer.state = AUDIOPLAYER_ERROR;
          }
          break;
          
        case PLAYER_BUFFER_OFFSET_FULL:
          if (ReadAudio(&hAudioPlayer.buffer[AUDIO_BUFFER_SIZE/4],
                        AUDIO_BUFFER_SIZE/2,
                        &numOfReadBytes) == 0)
          {
            if (numOfReadBytes != AUDIO_BUFFER_SIZE/2)
            {
              hAudioPlayer.state = AUDIOPLAYER_EOF;
            }
            
            /* Toggle the green led */
            BSP_LED_Toggle(LED_GREEN);
          }
          else
          {
            hAudioPlayer.state = AUDIOPLAYER_ERROR;
          }
          break;
          
        default:
          break;
        }
      }
      if (hAudioPlayer.state == AUDIOPLAYER_EOF)
      {
        /* Stop playback */
        if(BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW) != AUDIO_OK)
        {
          Error_Handler();
        }
        hAudioPlayer.state = AUDIOPLAYER_STOPPED;
        /* Restart playback */
        if(AudioPlayer_Start(hAudioPlayer.WavHeader[hAudioPlayer.AudioSource].SampleRate) != AUDIOPLAYER_ERROR_NONE)
        {
          Error_Handler();
        }        
      }
    }
  }
}

/**
  * @brief  Indicates whether or not the wav header of the audio file is valid
  * @param  WAV_header: WAV header
  * @retval 1 if the wav header is valid, 0 otherwise
  */
static uint8_t IsValidWAVHeader(WAV_HeaderTypeDef* WAV_header)
{
  uint8_t status = 1;

  if (   (strncmp(WAV_header->RIFF, "RIFF", 4) != 0)
      || (strncmp(WAV_header->WAVE, "WAVE", 4) != 0)
      || (WAV_header->AudioFormat != 1))
  {
    status = 0;
  }
  return status;
}

/**
  * @brief  Read a number of audio samples from QSPI flash 
  * @param  buff: Pointer to data buffer
  * @param  btr: Number of bytes to read
  * @param  br: Pointer to number of bytes read
  * @retval 0 if the read operation is successful, 1 otherwise
  */
static uint8_t ReadAudio(uint16_t* buff, 
                         uint32_t btr, 
                         uint32_t* br)
{
  uint8_t  status = 0;
  uint32_t nb_bytes_to_read = 0;
  
  if (hAudioPlayer.read + btr <= hAudioPlayer.end)
  {
    nb_bytes_to_read = btr;
  }
  else
  {
    nb_bytes_to_read = hAudioPlayer.end - hAudioPlayer.read;
  }
  
  if (hAudioPlayer.AudioSource == AUDIO_SOURCE_FLASH)
  {
    if(memcpy((void*) buff, (void*) hAudioPlayer.read, nb_bytes_to_read) == NULL)
    {
      status = 1;
    }
  }
  else
  {
    if (BSP_QSPI_Read((uint8_t*) buff, hAudioPlayer.read, nb_bytes_to_read) != QSPI_OK)
    {
      status = 1;
    }
  }
  
  hAudioPlayer.read += nb_bytes_to_read;
  *br = nb_bytes_to_read;
  
  return status;
}
/* End of private functions --------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */

