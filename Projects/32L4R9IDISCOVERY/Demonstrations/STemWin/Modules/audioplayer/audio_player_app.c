/**
  ******************************************************************************
  * @file    audio_player_app.c
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
#include "stm32l4xx_hal.h"
#include "stm32l4r9i_discovery_audio.h"
#include "audio_player_app.h"

#include "ff_gen_drv.h"
#include "k_rtc.h"

/** @addtogroup AUDIO_PLAYER_MODULE
  * @{
  */

/** @defgroup AUDIO_APPLICATION
  * @brief audio application routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static FIL wav_file;

static osMessageQId   AudioEvent = 0;
static osThreadId     AudioThreadId = 0;
static uint8_t muteState = MUTE_OFF;
static uint32_t current_nbrChannels = 0;
static uint32_t current_frequency = 0;

#if !defined(__GNUC__) && defined( USE_SPIRIT_EQ )
static uint8_t spirit_eq_used = 0;
long  AUDIO_EqInstance[SPIRIT_EQ_PERSIST_SIZE_IN_BYTES/4]  ;
TSpiritEQ_Band AUDIO_EQ_Bands[SPIRIT_EQ_MAX_BANDS];
__IO int16_t AUDIO_loudness;
__IO uint32_t AUDIO_LdCtrlPersistance[SPIRIT_LDCTRL_PERSIST_SIZE_IN_BYTES/4];
__IO uint32_t AUDIO_LdCtrlScratch[SPIRIT_LDCTRL_SCRATCH_SIZE_IN_BYTES/4];
TSpiritLdCtrl_Prms AUDIO_LdCtrlInstanceParams;
TSpiritEQ_Band *tmpEqBand;

#define CALC_LDNS_DB(x)   (1 << (SPIRIT_LDCTRL_GAIN_Q_BITS - x ))  

#endif

//#define DEBUG_AUDIO_APP

#ifndef DEFAULT_AUDIO_OUT_FREQ
#define DEFAULT_AUDIO_OUT_FREQ               BSP_AUDIO_FREQUENCY_48K
#endif

/* External function prototypes -----------------------------------------------*/
extern void Error_Handler(void);

/* Private function prototypes -----------------------------------------------*/
static void Audio_Thread(void const * argument);
static void AUDIO_TransferComplete_CallBack(void);
static void AUDIO_HalfTransfer_CallBack(void);
static void AUDIO_Error_CallBack(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes audio
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Init(uint8_t volume, uint8_t use_spirit_eq)
{
#if !defined(__GNUC__) && defined( USE_SPIRIT_EQ )  
  uint32_t index = 0;
  __IO uint32_t ldness_value;

  if(use_spirit_eq)
  {
    return AUDIOPLAYER_ERROR_HW;
  }
#endif

  portENTER_CRITICAL();

  /* Try to Init Audio interface in different config in case of failure */
  if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, volume, DEFAULT_AUDIO_OUT_FREQ))
  {
    Error_Handler();
  }

  /* Initialize internal audio structure */
  haudio.out.state  = AUDIOPLAYER_STOP;
  haudio.out.mute   = MUTE_OFF;
  haudio.out.volume = volume;
  current_frequency = 0;
  
  /* Register audio BSP drivers callbacks */
  BSP_AUDIO_OUT_RegisterCallbacks(AUDIO_Error_CallBack, 
                                  AUDIO_HalfTransfer_CallBack, 
                                  AUDIO_TransferComplete_CallBack);
#if !defined(__GNUC__) && defined( USE_SPIRIT_EQ )  
  if(use_spirit_eq)
  {
    /* Enable CRC to Unlock Audio add-ons */
  __HAL_RCC_CRC_CLK_ENABLE();
  
  /* Retrieve stored settings*/
  AUDIO_EQ_Bands[0].gainDb = RTC_BkupRestoreParameter(AUDIOPLAYER_EQU1_BKP);
  AUDIO_EQ_Bands[1].gainDb = RTC_BkupRestoreParameter(AUDIOPLAYER_EQU2_BKP);    
  AUDIO_EQ_Bands[2].gainDb = RTC_BkupRestoreParameter(AUDIOPLAYER_EQU3_BKP);   
  AUDIO_EQ_Bands[3].gainDb = RTC_BkupRestoreParameter(AUDIOPLAYER_EQU4_BKP);  
  AUDIO_loudness = RTC_BkupRestoreParameter(AUDIOPLAYER_LOUD_BKP);
  
  /* Enable the Eq */
  SpiritEQ_Init(AUDIO_EqInstance, AUDIO_FREQUENCY_48K);
    
  /* set band params */
  SET_BAND_PRMS(&AUDIO_EQ_Bands[0], SPIRIT_EQ_FLT_TYPE_SHELVING_LOWPASS , 100, 100);
  SET_BAND_PRMS(&AUDIO_EQ_Bands[1], SPIRIT_EQ_FLT_TYPE_SHELVING_LOWPASS , 800, 200);
  SET_BAND_PRMS(&AUDIO_EQ_Bands[2], SPIRIT_EQ_FLT_TYPE_SHELVING_HIPASS , 8000, 2000);
  SET_BAND_PRMS(&AUDIO_EQ_Bands[3], SPIRIT_EQ_FLT_TYPE_SHELVING_HIPASS , 15000, 2000);

  for (index = 0; index < SPIRIT_EQ_MAX_BANDS ; index++)
  {
    if((AUDIO_EQ_Bands[index].gainDb < -12) && (AUDIO_EQ_Bands[index].gainDb > 12))
    {
      AUDIO_EQ_Bands[index].gainDb = 0;
    }
    tmpEqBand = &AUDIO_EQ_Bands[index];
    SpiritEQ_FltSet((TSpiritEq *)AUDIO_EqInstance, tmpEqBand, index);
  }
  
  /* Enable Loundness Control */
  if((AUDIO_loudness < -12) && (AUDIO_loudness > 12))
  {
    AUDIO_loudness= 0;
  }
  
  SpiritLdCtrl_Init((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance, AUDIO_FREQUENCY_48K);
  SpiritLdCtrl_GetPrms((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance, &AUDIO_LdCtrlInstanceParams);
  AUDIO_LdCtrlInstanceParams.gainQ8 = CALC_LDNS_DB(AUDIO_loudness);
  SpiritLdCtrl_SetPrms((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance, &AUDIO_LdCtrlInstanceParams);
  }
#endif
    
  /* Create Audio Queue */
  osMessageQDef(AUDIO_Queue, 1, uint16_t);
  AudioEvent = osMessageCreate (osMessageQ(AUDIO_Queue), NULL); 
  
  /* Create Audio task */
  osThreadDef(osAudio_Thread, Audio_Thread, osPriorityRealtime, 0, 512);
  AudioThreadId = osThreadCreate (osThread(osAudio_Thread), NULL);  

  portEXIT_CRITICAL();

#if !defined(__GNUC__) && defined( USE_SPIRIT_EQ )
  spirit_eq_used = use_spirit_eq;
#endif

  return AUDIOPLAYER_ERROR_NONE;
}

#if !defined(__GNUC__) && defined( USE_SPIRIT_EQ )
/**
  * @brief  This function Set the new gain of the equilizer
  * @param  BandNum : equilizer band index
  * @param  NewGainValue : the new band gain -12 dB to 12 dB.
  * @retval None.
*/
void AUDIOPLAYER_SetEqGain(uint8_t BandNum, int16_t NewGainValue)
{
  if(spirit_eq_used)
  {
  if((NewGainValue > -12) && (NewGainValue < 12)) 
  {
    SpiritEQ_FltGet((TSpiritEq *)AUDIO_EqInstance, &AUDIO_EQ_Bands[BandNum], BandNum );
    AUDIO_EQ_Bands[BandNum].gainDb = NewGainValue;
    SpiritEQ_FltSet((TSpiritEq *)AUDIO_EqInstance, &AUDIO_EQ_Bands[BandNum], BandNum);
  }
  }
}

/**
  * @brief  This function return the gain of the equilizer
  * @param  BandNum : equilizer band index
  * @retval band gain -12 dB to +12 dB.
*/
int16_t AUDIOPLAYER_GetEqGain(uint8_t BandNum)
{
  return (AUDIO_EQ_Bands[BandNum].gainDb);
}

/**
  * @brief  This function Set Loudness Control Gain Value.
  * @param  NewGainValue -12 dB to +12 dB.
  * @retval None.
  */
void AUDIOPLAYER_SetLoudnessGain(int16_t NewGainValue)
{
  if(spirit_eq_used)
  {
  if((NewGainValue > -12) && (NewGainValue < 12)) 
  {  
    SpiritLdCtrl_GetPrms((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance, &AUDIO_LdCtrlInstanceParams);
    AUDIO_LdCtrlInstanceParams.gainQ8 = CALC_LDNS_DB (NewGainValue);
    AUDIO_loudness = NewGainValue; 
    SpiritLdCtrl_Reset  ((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance);
    SpiritLdCtrl_SetPrms((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance, &AUDIO_LdCtrlInstanceParams);
  }
  }
}

/**
  * @brief  This function Set Loudness Control Gain Value.
  * @param  NewGainValue -12 dB to 12 dB.
  * @retval None.
  */
int16_t AUDIOPLAYER_GetLoudnessGain(void)
{
  return (AUDIO_loudness);
}
#endif

/**
  * @brief  Get audio state
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_StateTypdef  AUDIOPLAYER_GetState(void)
{
  return haudio.out.state;
}

/**
  * @brief  Get audio volume
  * @param  None.
  * @retval Audio volume.
  */
uint32_t  AUDIOPLAYER_GetVolume(void)
{
  return haudio.out.volume;
}

/**
  * @brief  Set audio volume
  * @param  Volume: Volume level to be set in percentage from 0% to 100% (0 for 
  *         Mute and 100 for Max volume level).
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SetVolume(uint32_t volume)
{
  if(BSP_AUDIO_OUT_SetVolume(volume) == AUDIO_OK)
  {
    haudio.out.volume = volume;
    return AUDIOPLAYER_ERROR_NONE;    
  }
  else
  {
    return AUDIOPLAYER_ERROR_HW;
  }
}

/**
  * @brief  Play audio stream
  * @param  frequency: Audio frequency and nb of channel used to play the audio stream.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Play(uint32_t frequency, uint32_t nbrChannels)
{
  uint32_t numOfReadBytes;
  uint32_t mode = BSP_AUDIO_OUT_STEREOMODE;
  haudio.out.state = AUDIOPLAYER_PLAY;
  
  /* Fill whole buffer @ first time */
  if(f_read(&wav_file, 
            &haudio.buff[0], 
            AUDIO_OUT_BUFFER_SIZE, 
            (void *)&numOfReadBytes) == FR_OK)
  { 
    if(numOfReadBytes != 0)
    {
      if(current_nbrChannels != nbrChannels)
      {
        current_nbrChannels = nbrChannels;
        if (nbrChannels == 1)
        {
          mode = BSP_AUDIO_OUT_MONOMODE;
        }
        else if (nbrChannels == 2)
        {
          mode = BSP_AUDIO_OUT_STEREOMODE;
        }
        else
        {
          Error_Handler();
        }

        BSP_AUDIO_OUT_ChangeAudioConfig(mode | BSP_AUDIO_OUT_CIRCULARMODE);
      }
      
      if(current_frequency != frequency)
      {
        current_frequency = frequency;
        if (BSP_AUDIO_OUT_SetFrequency(frequency))
        {
          Error_Handler();
        }
      }
      
      osThreadResume(AudioThreadId);

      if (BSP_AUDIO_OUT_Play((uint16_t*)&haudio.buff[0], AUDIO_OUT_BUFFER_SIZE_HALF_WORD))   
      {
        Error_Handler();
      }        
      
      AUDIOPLAYER_Mute(MUTE_OFF);

      return AUDIOPLAYER_ERROR_NONE;
    }
  }

  return AUDIOPLAYER_ERROR_IO;

}

/**
  * @brief  Audio player process
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Process(void)
{
  if((AudioEvent == 0) || (AudioThreadId == 0))
  {
    /* Not yet initialized or already stopped */
    return AUDIOPLAYER_ERROR_NONE;
  }

  switch(haudio.out.state)
  {
  case AUDIOPLAYER_START:
    AUDIOPLAYER_Mute(MUTE_OFF);
    haudio.out.state = AUDIOPLAYER_PLAY;
    break;    

  case AUDIOPLAYER_EOF:
     AUDIOPLAYER_NotifyEndOfFile();
    break;    
    
  case AUDIOPLAYER_ERROR:
     AUDIOPLAYER_Stop();
    break;
    
  case AUDIOPLAYER_STOP:
  case AUDIOPLAYER_PLAY:    
  default:
    break;
  }
  
  return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @brief  Audio player DeInit
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_DeInit(void)
{
  if((AudioEvent == 0) || (AudioThreadId == 0))
  {
    /* Not yet initialized or already stopped */
    return AUDIOPLAYER_ERROR_NONE;
  }

  if(haudio.out.state != AUDIOPLAYER_STOP)
  {
    haudio.out.state = AUDIOPLAYER_STOP; 
    if (BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW))
    {
      Error_Handler();
    }
    f_close(&wav_file); 
  }
  
  if (BSP_AUDIO_OUT_DeInit())
  {
    Error_Handler();
  }

  if(AudioEvent != 0)
  {
    vQueueDelete(AudioEvent); 
    AudioEvent = 0;
  }
  if(AudioThreadId != 0)
  {
    osThreadTerminate(AudioThreadId);
    AudioThreadId = 0;
  }
#if !defined(__GNUC__) && defined( USE_SPIRIT_EQ )   
  if(spirit_eq_used)
  {
  RTC_BkupSaveParameter(AUDIOPLAYER_EQU1_BKP, AUDIO_EQ_Bands[0].gainDb);
  RTC_BkupSaveParameter(AUDIOPLAYER_EQU2_BKP, AUDIO_EQ_Bands[1].gainDb);
  RTC_BkupSaveParameter(AUDIOPLAYER_EQU3_BKP, AUDIO_EQ_Bands[2].gainDb);
  RTC_BkupSaveParameter(AUDIOPLAYER_EQU4_BKP, AUDIO_EQ_Bands[3].gainDb);
  RTC_BkupSaveParameter(AUDIOPLAYER_LOUD_BKP, AUDIO_loudness);
  }
#endif
  
  return AUDIOPLAYER_ERROR_NONE;  
}

/**
  * @brief  Stop audio stream.
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Stop(void)
{
  AUDIOPLAYER_ErrorTypdef  ErrorCode = AUDIOPLAYER_ERROR_NONE;

  if(AudioThreadId != 0)
  {  
    osThreadSuspend(AudioThreadId); 
  }

  if(haudio.out.state != AUDIOPLAYER_STOP)
  {
    AUDIOPLAYER_Mute(MUTE_ON);

    haudio.out.state = AUDIOPLAYER_STOP;

    if (BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW))
    {
      ErrorCode = AUDIOPLAYER_ERROR_HW;
    }

    f_close(&wav_file);
  }

  return ErrorCode;
}


/**
  * @brief  Pause Audio stream
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Pause(void)
{
  AUDIOPLAYER_ErrorTypdef  ErrorCode = AUDIOPLAYER_ERROR_NONE;

  if(AudioThreadId != 0)
  {
    osThreadSuspend(AudioThreadId); 
  }

  if(haudio.out.state == AUDIOPLAYER_PLAY)
  {
    if (BSP_AUDIO_OUT_Pause())
    {
      ErrorCode = AUDIOPLAYER_ERROR_HW;
    }
    else
    {
      haudio.out.state = AUDIOPLAYER_PAUSE;
    }
  }

  return ErrorCode;
}


/**
  * @brief  Resume Audio stream
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Resume(void)
{
  AUDIOPLAYER_ErrorTypdef  ErrorCode = AUDIOPLAYER_ERROR_NONE;

  if(AudioThreadId != 0)
  {  
    osThreadResume(AudioThreadId);  
  }

  if(haudio.out.state == AUDIOPLAYER_PAUSE)
  {
    if (BSP_AUDIO_OUT_Resume())
    {
      ErrorCode = AUDIOPLAYER_ERROR_HW;
    }
    else
    {
      haudio.out.state = AUDIOPLAYER_PLAY; 
    }
  }

  return ErrorCode;
}
/**
  * @brief  Sets audio stream position
  * @param  position: stream position.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SetPosition(uint32_t position)
{
  long file_pos;
  
  file_pos = f_size(&wav_file) / AUDIO_OUT_BUFFER_SIZE / 100; 
  file_pos *= (position * AUDIO_OUT_BUFFER_SIZE);
  AUDIOPLAYER_Pause(); 
  f_lseek(&wav_file, file_pos);
  AUDIOPLAYER_Resume(); 
  
  return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @brief  Sets the volume at mute
  * @param  state: could be MUTE_ON to mute sound or MUTE_OFF to unmute 
  *                the codec and restore previous volume level.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Mute(uint8_t state)
{
  AUDIOPLAYER_ErrorTypdef  ErrorCode = AUDIOPLAYER_ERROR_NONE;

  if((AudioEvent == 0) || (AudioThreadId == 0))
  {
    /* Not yet initialized or already stopped */
    return AUDIOPLAYER_ERROR_NONE;
  }

  if(muteState != state)
  {
    if (BSP_AUDIO_OUT_SetMute(state))
    {
      ErrorCode = AUDIOPLAYER_ERROR_HW;
    }
    else
    {
      muteState = state;
    }
  }

   return ErrorCode;
}

/**
  * @brief  Get the wav file information.
  * @param  file: wav file.
  * @param  info: pointer to wav file structure
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_GetFileInfo(char* file, WAV_InfoTypedef* info)
{
  uint32_t numOfReadBytes;
  AUDIOPLAYER_ErrorTypdef ret = AUDIOPLAYER_ERROR_IO;
  FIL fsfile;
  
  if( f_open(&fsfile, file, FA_OPEN_EXISTING | FA_READ) == FR_OK) 
  {
    /* Fill the buffer to Send */
    if(f_read(&fsfile, info, sizeof(WAV_InfoTypedef), (void *)&numOfReadBytes) == FR_OK)
    {
      if((info->ChunkID == 0x46464952) && (info->AudioFormat == 1))
      {
        ret = AUDIOPLAYER_ERROR_NONE;
      }
    }
    f_close(&fsfile);      
  }
  return ret;
}

/**
  * @brief  Select wav file.
  * @param  file: wav file.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SelectFile(char* file)
{
  AUDIOPLAYER_ErrorTypdef ret = AUDIOPLAYER_ERROR_IO;
  if( f_open(&wav_file, file, FA_OPEN_EXISTING | FA_READ) == FR_OK) 
  {
    f_lseek(&wav_file, sizeof(WAV_InfoTypedef));
    ret = AUDIOPLAYER_ERROR_NONE;
  }
  return ret;  
}

/**
  * @brief  Get wav file progress.
  * @param  None
  * @retval file progress.
  */
uint32_t AUDIOPLAYER_GetProgress(void)
{
  return (wav_file.fptr);
}

/**
  * @brief  Manages the DMA Transfer complete interrupt.
  * @param  None
  * @retval None
  */

static void AUDIO_TransferComplete_CallBack(void)
{
  if(haudio.out.state == AUDIOPLAYER_PLAY)
  {
#ifdef DEBUG_AUDIO_APP
    if (osMessagePut ( AudioEvent, PLAY_BUFFER_OFFSET_FULL, 0) != osOK)
    {
      /* Fifo underflow !!! */
      Error_Handler();
    }
#else
    osMessagePut ( AudioEvent, PLAY_BUFFER_OFFSET_FULL, 0);
#endif
  }
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
static void AUDIO_HalfTransfer_CallBack(void)
{ 
  if(haudio.out.state == AUDIOPLAYER_PLAY)
  {
#ifdef DEBUG_AUDIO_APP
    if (osMessagePut ( AudioEvent, PLAY_BUFFER_OFFSET_HALF, 0) != osOK)
    {
      /* Fifo underflow !!! */
      Error_Handler();
    }
#else
    osMessagePut ( AudioEvent, PLAY_BUFFER_OFFSET_HALF, 0);
#endif
  }
}

/**
  * @brief  Manages the DMA FIFO error interrupt.
  * @param  None
  * @retval None
  */
static void AUDIO_Error_CallBack(void)
{
  haudio.out.state = AUDIOPLAYER_ERROR;
  AUDIOPLAYER_Mute(MUTE_ON);
}

/**
  * @brief  Audio task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void Audio_Thread(void const * argument)
{
  uint32_t numOfReadBytes;    
  osEvent event;  
  for(;;)
  {
    event = osMessageGet(AudioEvent, osWaitForever );

    if( event.status == osEventMessage )
    {
      if(haudio.out.state == AUDIOPLAYER_PLAY)
      {
        switch(event.value.v)
        {
        case PLAY_BUFFER_OFFSET_HALF:
          if(f_read(&wav_file, 
                    &haudio.buff[0], 
                    AUDIO_OUT_BUFFER_SIZE/2, 
                    (void *)&numOfReadBytes) == FR_OK)
          {
            if(numOfReadBytes == 0)
            {  
              haudio.out.state = AUDIOPLAYER_EOF;
              AUDIOPLAYER_Mute(MUTE_ON);
            }
#if !defined(__GNUC__) && defined( USE_SPIRIT_EQ )
            else if(spirit_eq_used)
            {
              SpiritEQ_Apply((void *)AUDIO_EqInstance,
                             /* NB_Channel */2, 
                             (int16_t *)&haudio.buff[0], 
                             numOfReadBytes  / 4);
              
              /* Apply Loudness settings */
              SpiritLdCtrl_Apply((TSpiritLdCtrl *)AUDIO_LdCtrlPersistance,
                                 /* NB_Channel */ 2, 
                                 (int16_t *)&haudio.buff[0], 
                                 numOfReadBytes / 4, 
                                 (void *)AUDIO_LdCtrlScratch);
            }
#endif
          }
          else
          {
              haudio.out.state = AUDIOPLAYER_ERROR;
          }
          break;  
          
        case PLAY_BUFFER_OFFSET_FULL:
          if(f_read(&wav_file, 
                    &haudio.buff[AUDIO_OUT_BUFFER_SIZE/2], 
                    AUDIO_OUT_BUFFER_SIZE/2, 
                    (void *)&numOfReadBytes) == FR_OK)
          { 
            if(numOfReadBytes == 0)
            { 
              haudio.out.state = AUDIOPLAYER_EOF;
              AUDIOPLAYER_Mute(MUTE_ON);
            }  
#if !defined(__GNUC__) && defined( USE_SPIRIT_EQ )
            else if(spirit_eq_used)
            {
              SpiritEQ_Apply((void *)AUDIO_EqInstance,
                             /* NB_Channel */2, 
                             (int16_t *)&haudio.buff[AUDIO_OUT_BUFFER_SIZE /2], 
                             numOfReadBytes  / 4);
              
              
              /* Apply Loudness settings */
              SpiritLdCtrl_Apply((TSpiritLdCtrl *)AUDIO_LdCtrlPersistance,
                                 /* NB_Channel */ 2, 
                                 (int16_t *)&haudio.buff[AUDIO_OUT_BUFFER_SIZE /2], 
                                 numOfReadBytes / 4, 
                                 (void *)AUDIO_LdCtrlScratch);
            }
#endif
          }
          else
          {
              haudio.out.state = AUDIOPLAYER_ERROR;
          }
          break;   
          
        default:
          break;
        }
      }
    }
  }
}

/**
  * @brief  Notify end of playing.
  * @param  None
  * @retval None
  */
__weak AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_NotifyEndOfFile(void)
{
  return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @brief  Notify audio error.
  * @param  None
  * @retval None
  */
__weak AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_NotifyError(void)
{
  return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @}
  */

/**
  * @}
  */

