/**
  ******************************************************************************
  * @file    audioplayer_app.c
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
 
/** @addtogroup AUDIO_PLAYER_MODULE
  * @{
  */

/** @defgroup AUDIO_APPLICATION
  * @brief audio application routines
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "audio_player_app.h"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/

/* Audio out parameters */
#define DEFAULT_AUDIO_OUT_FREQ              BSP_AUDIO_FREQUENCY_48K
#define DEFAULT_AUDIO_OUT_BIT_RESOLUTION    ((uint8_t)16)
#define DEFAULT_AUDIO_OUT_CHANNEL_NBR       ((uint8_t)2) /* Mono = 1, Stereo = 2 */
#define DEFAULT_AUDIO_OUT_VOLUME            ((uint16_t)80)
    
#define AUDIO_OUT_TASK_STACK_SIZE       512    

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static FIL wav_file;

static osMessageQId   AudioEvent = 0;
static osThreadId     AudioThreadId = 0;
static uint8_t muteState = MUTE_OFF;
static uint32_t current_nbrChannels = 0;
static uint32_t current_frequency = 0;
static uint8_t graphic_eq_used = 0;
static uint8_t loudness_used = 0;


#if defined( USE_GRAPHIC_EQ )
/* Graphic Equalizer */
void *pGreqPersistentMem = NULL;
void *pGreqScratchMem = NULL;
static  greq_static_param_t greq_static_param;
static  greq_dynamic_param_t greq_dynamic_param;
static  buffer_t BufferHandler;
static  buffer_t *pBufferHandler = &BufferHandler;

__IO int16_t AUDIO_loudness;

/* BiQuad Loudness Filter */
//static const int32_t tab_coeff_Loudness[1+NB_BIQUAD_SEC_1*6]={ 1,0,1201999107,424590732,3068307,344502132,-1644547};
static const int32_t tab_coeff_Loudness[1+NB_BIQUAD_SEC_5*6] = {NB_BIQUAD_SEC_5,
                                                                0,  1201999107,  424590732,      3068307,   344502132,   -1644547, \
                                                                1,  1073741824, -2075485822,  1010128930,  1905720500, -845892267, \
                                                                1,  1073741824, -1348856775,   812477686,  1121328227, -685022681, \
                                                                1,  1073741824, -1791523805,   845106364,  1415657206, -407278556, \
                                                                2,  1134670788,           0,           0,           0,          0};

void *pBIQPersistentMem = NULL;
void *pBIQScratchMem = NULL;

static  biquad_df1_cascade_static_param_t  biquad_df1_cascade_static_param;
static  biquad_df1_cascade_dynamic_param_t biquad_df1_cascade_dynamic_param;
#endif

//#define DEBUG_AUDIO_APP

/* External function prototypes -----------------------------------------------*/
extern void Error_Handler();

/* Private function prototypes -----------------------------------------------*/
static void Audio_Thread(void const * argument);
static void AUDIO_TransferComplete_CallBack(void);
static void AUDIO_HalfTransfer_CallBack(void);
static void AUDIO_Error_CallBack(void);

/* Private functions ---------------------------------------------------------*/
#if defined( USE_GRAPHIC_EQ )
static void AUDIO_FilterLoudnessConfig(biquad_df1_cascade_dynamic_param_t *biquad_df1_cascade_dyn, uint8_t BiqEn)
{
  uint8_t k =0;
  /* content: nb_sos, post-shift, b0, b1, a0, a1, a2*/
  int32_t *tab_coeff = (int32_t*)&tab_coeff_Loudness[1];

  /* Update some dynamic parameters values*/
  biquad_df1_cascade_dyn->enable = BiqEn;
  biquad_df1_cascade_dyn->nb_sos = tab_coeff_Loudness[0];   
  for (k=0 ; k<biquad_df1_cascade_dyn->nb_sos ; k++)
  {
    biquad_df1_cascade_dyn->biquad_coeff[k*6]   =*tab_coeff++; /*post-chift*/
    biquad_df1_cascade_dyn->biquad_coeff[k*6+1] =*tab_coeff++; /*b0*/
    biquad_df1_cascade_dyn->biquad_coeff[k*6+2] =*tab_coeff++; /*b1*/
    biquad_df1_cascade_dyn->biquad_coeff[k*6+3] =*tab_coeff++; /*a0*/
    biquad_df1_cascade_dyn->biquad_coeff[k*6+4] =*tab_coeff++; /*a1*/
    biquad_df1_cascade_dyn->biquad_coeff[k*6+5] =*tab_coeff++; /*a2*/
  }    
}
#endif /* USE_GRAPHIC_EQ */

static void * AUDIO_malloc(size_t size)
{
#ifdef AUDIO_USE_RTOS
  return pvPortMalloc(size);
#else /* !AUDIO_USE_RTOS */
  return malloc(size);
#endif
}

static void AUDIO_free( void *pMemToFree ) 
{
#ifdef AUDIO_USE_RTOS
  vPortFree(pMemToFree);
#else /* !AUDIO_USE_RTOS */
  free(pMemToFree);
#endif
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Initializes audio
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Init(uint8_t volume, uint8_t use_graphic_eq, uint8_t use_loudness)
{
#if defined( USE_GRAPHIC_EQ )
  int32_t error = GREQ_ERROR_NONE;
  int32_t *tab_coeff;
#endif

#if !defined(USE_GRAPHIC_EQ)
  if((use_graphic_eq) || (use_loudness))
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
#if defined( USE_GRAPHIC_EQ )

  if((use_graphic_eq) || (use_loudness))
  {
    /* Enables and resets CRC-32 to Unlock Audio Add-ons */
    __HAL_RCC_CRC_CLK_ENABLE();
    CRC->CR = CRC_CR_RESET;
  }

  if(use_graphic_eq)
  {
    /* Effect Initialization and configuration */
    /*-----------------------------------------*/
    /* Allocat mem for GrEq */
    pGreqPersistentMem = AUDIO_malloc(greq_persistent_mem_size); /* greq_persistent_mem_size  0x224 */
    pGreqScratchMem = AUDIO_malloc(greq_scratch_mem_size);       /* greq_scratch_mem_size  0xF00 */

    /* GREQ effect reset */
    error = greq_reset(pGreqPersistentMem, pGreqScratchMem);
    if (error != GREQ_ERROR_NONE)
    {
      return (AUDIO_ERROR_GREQ);
    }

    /* GREQ effect static parameters setting */
#ifdef EQ_HIGH_QUALITY
    greq_static_param.nb_bands = GREQ_NB_BANDS_10;  /* 10-bands equalizer */
#else
    greq_static_param.nb_bands = GREQ_NB_BANDS_5;  /* 5-bands equalizer */
#endif
    error = greq_setParam(&greq_static_param, pGreqPersistentMem);
    if (error != GREQ_ERROR_NONE)
    {
      return (AUDIO_ERROR_GREQ);
    }

    /* GREQ dynamic parameters that can be updated here every frame if required */
    greq_dynamic_param.enable = 0;            /* Enables the effect */
    greq_dynamic_param.user_gain_per_band_dB[0] = k_BkupRestoreParameter(AUDIOPLAYER_EQU1_BKP);  /* Gain for band 1  */
    greq_dynamic_param.user_gain_per_band_dB[1] = k_BkupRestoreParameter(AUDIOPLAYER_EQU2_BKP);  /* Gain for band 2  */
    greq_dynamic_param.user_gain_per_band_dB[2] = k_BkupRestoreParameter(AUDIOPLAYER_EQU3_BKP);  /* Gain for band 3  */
    greq_dynamic_param.user_gain_per_band_dB[3] = k_BkupRestoreParameter(AUDIOPLAYER_EQU4_BKP);  /* Gain for band 4  */
    greq_dynamic_param.user_gain_per_band_dB[4] = k_BkupRestoreParameter(AUDIOPLAYER_EQU5_BKP);  /* Gain for band 5  */
#ifdef EQ_HIGH_QUALITY
    greq_dynamic_param.user_gain_per_band_dB[5] = k_BkupRestoreParameter(AUDIOPLAYER_EQU6_BKP);  /* Gain for band 6  */
    greq_dynamic_param.user_gain_per_band_dB[6] = k_BkupRestoreParameter(AUDIOPLAYER_EQU7_BKP);  /* Gain for band 7  */
    greq_dynamic_param.user_gain_per_band_dB[7] = k_BkupRestoreParameter(AUDIOPLAYER_EQU8_BKP);  /* Gain for band 8  */
    greq_dynamic_param.user_gain_per_band_dB[8] = k_BkupRestoreParameter(AUDIOPLAYER_EQU9_BKP);  /* Gain for band 9  */
    greq_dynamic_param.user_gain_per_band_dB[9] = k_BkupRestoreParameter(AUDIOPLAYER_EQU10_BKP); /* Gain for band 10 */
#endif

    greq_dynamic_param.gain_preset_idx = GREQ_NO_PRESET;         /* User preset defined above */
    error = greq_setConfig(&greq_dynamic_param, pGreqPersistentMem);
    if (error != GREQ_ERROR_NONE)
    {
        return (AUDIO_ERROR_GREQ);
    }
  }

  if(use_loudness)
  {
    int32_t k=0;

    /* Allocat mem for Biq */
    pBIQPersistentMem = AUDIO_malloc(biquad_df1_cascade_persistent_mem_size); /* biquad_df1_cascade_persistent_mem_size  0x025C */ 
    pBIQScratchMem = AUDIO_malloc(biquad_df1_cascade_scratch_mem_size);       /* biquad_df1_cascade_scratch_mem_size  0x04 */

    /* BIQ effect reset */
    error = biquad_df1_cascade_reset(pBIQPersistentMem, pBIQScratchMem);
    if (error != BIQ_ERROR_NONE)
    {
      return (AUDIO_ERROR_BIQ);
    }
    
    AUDIO_loudness = k_BkupRestoreParameter(AUDIOPLAYER_LOUD_BKP);

    /* BIQ effect static parameters setting */
    biquad_df1_cascade_static_param.nb_sos =tab_coeff_Loudness[0]; /* by default set to:NB_BIQUAD_SEC_5 [5-second order sections] */
    tab_coeff =	(int32_t*)&tab_coeff_Loudness[1];
    for (k=0 ; k<biquad_df1_cascade_static_param.nb_sos ; k++)
    {
      biquad_df1_cascade_static_param.biquad_coeff[k*6]   =*tab_coeff++;
      biquad_df1_cascade_static_param.biquad_coeff[k*6+1] =*tab_coeff++;
      biquad_df1_cascade_static_param.biquad_coeff[k*6+2] =*tab_coeff++;
      biquad_df1_cascade_static_param.biquad_coeff[k*6+3] =*tab_coeff++;
      biquad_df1_cascade_static_param.biquad_coeff[k*6+4] =*tab_coeff++;
      biquad_df1_cascade_static_param.biquad_coeff[k*6+5] =*tab_coeff++;				
    }
    error = biquad_df1_cascade_setParam(&biquad_df1_cascade_static_param, pBIQPersistentMem);
    if (error != BIQ_ERROR_NONE)
    {
      return (AUDIO_ERROR_BIQ);
    }

    /* BIQ dynamic parameters that can be updated here every frame if required */
    AUDIO_FilterLoudnessConfig(&biquad_df1_cascade_dynamic_param, 1); /* Enable the effect */
    error = biquad_df1_cascade_setConfig(&biquad_df1_cascade_dynamic_param, pBIQPersistentMem);
    if (error != BIQ_ERROR_NONE)
    {
      return (AUDIO_ERROR_BIQ);
    }
  }
#endif

  /* Create Audio Queue */
  osMessageQDef(AUDIO_Queue, 1, uint16_t);
  AudioEvent = osMessageCreate (osMessageQ(AUDIO_Queue), NULL); 
  
  /* Create Audio task */
  osThreadDef(osAudio_Thread, Audio_Thread, osPriorityRealtime, 0, AUDIO_OUT_TASK_STACK_SIZE);
  AudioThreadId = osThreadCreate (osThread(osAudio_Thread), NULL);  

  portEXIT_CRITICAL();

  graphic_eq_used = use_graphic_eq;
  loudness_used   = use_loudness;

  return AUDIOPLAYER_ERROR_NONE;
}

#if defined( USE_GRAPHIC_EQ )

/**
  * @brief  This function Set the new gain of the equilizer
  * @param  BandNum : equilizer band index
  * @param  NewGainValue : the new band gain -12 dB to 12 dB.
  * @retval None.
*/
void AUDIOPLAYER_SetEqGain(uint8_t BandNum, int16_t NewGainValue)
{
  if(graphic_eq_used)
  {
    if((NewGainValue > -12) && (NewGainValue < 12)) 
    {
      greq_dynamic_param.user_gain_per_band_dB[BandNum] = NewGainValue;
      if (greq_setConfig(&greq_dynamic_param, pGreqPersistentMem) != GREQ_ERROR_NONE)
      {
        Error_Handler();
      }
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
  return (greq_dynamic_param.user_gain_per_band_dB[BandNum]);
}

/**
  * @brief  This function Set Loudness Control Gain Value.
  * @param  NewGainValue -12 dB to +12 dB.
  * @retval None.
  */
void AUDIOPLAYER_SetLoudness(void)
{
  if(graphic_eq_used)
  {      
      /* Update BIQ dynamic parameters for the next frame */
      AUDIO_FilterLoudnessConfig(&biquad_df1_cascade_dynamic_param, 1); /* Enable the effect */

      if (biquad_df1_cascade_setConfig(&biquad_df1_cascade_dynamic_param, pBIQPersistentMem) != BIQ_ERROR_NONE)
      {
        Error_Handler();
      }   
    
  }
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
      if (BSP_AUDIO_OUT_Pause())
      {
        Error_Handler();
      }

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

      if (BSP_AUDIO_OUT_Resume())
      {
        Error_Handler();
      }

      if (BSP_AUDIO_OUT_Play((uint16_t*)&haudio.buff[0], AUDIO_OUT_BUFFER_SIZE_HALF_WORD))   
      {
        Error_Handler();
      }

      if(muteState == MUTE_OFF)
      {
        if (BSP_AUDIO_OUT_SetMute(MUTE_OFF))
        {
          Error_Handler();
        }
      }

#if defined( USE_GRAPHIC_EQ ) 
      if(graphic_eq_used)
      {
        /* Enable The Graphic Equalizer */
        greq_getConfig(&greq_dynamic_param, pGreqPersistentMem);  
        greq_dynamic_param.enable = 1;                          /* Enables the effect */
        greq_dynamic_param.gain_preset_idx = GREQ_NO_PRESET;    /* No User preset */
        if (greq_setConfig(&greq_dynamic_param, pGreqPersistentMem) != GREQ_ERROR_NONE)
        {
          return AUDIO_ERROR_GREQ;
        }
      }

      if(loudness_used)
      {
        /* BIQ dynamic parameters that can be updated here every frame if required */
        biquad_df1_cascade_getConfig(&biquad_df1_cascade_dynamic_param, pBIQPersistentMem);
                    
        biquad_df1_cascade_dynamic_param.enable = 1;
        if (biquad_df1_cascade_setConfig(&biquad_df1_cascade_dynamic_param, pBIQPersistentMem) != BIQ_ERROR_NONE)
        {
          return (AUDIO_ERROR_BIQ);
        }
      }
#endif /* USE_GRAPHIC_EQ */

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
    BSP_AUDIO_OUT_SetMute(MUTE_OFF);
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
#if defined( USE_GRAPHIC_EQ ) 
  if(graphic_eq_used)
  {
    k_BkupSaveParameter(AUDIOPLAYER_EQU1_BKP,  greq_dynamic_param.user_gain_per_band_dB[0]);
    k_BkupSaveParameter(AUDIOPLAYER_EQU2_BKP,  greq_dynamic_param.user_gain_per_band_dB[1]);
    k_BkupSaveParameter(AUDIOPLAYER_EQU3_BKP,  greq_dynamic_param.user_gain_per_band_dB[2]);
    k_BkupSaveParameter(AUDIOPLAYER_EQU4_BKP,  greq_dynamic_param.user_gain_per_band_dB[3]);
    k_BkupSaveParameter(AUDIOPLAYER_EQU5_BKP,  greq_dynamic_param.user_gain_per_band_dB[4]);
#ifdef EQ_HIGH_QUALITY
    k_BkupSaveParameter(AUDIOPLAYER_EQU6_BKP,  greq_dynamic_param.user_gain_per_band_dB[5]);
    k_BkupSaveParameter(AUDIOPLAYER_EQU7_BKP,  greq_dynamic_param.user_gain_per_band_dB[6]);
    k_BkupSaveParameter(AUDIOPLAYER_EQU8_BKP,  greq_dynamic_param.user_gain_per_band_dB[7]);
    k_BkupSaveParameter(AUDIOPLAYER_EQU9_BKP,  greq_dynamic_param.user_gain_per_band_dB[8]);
    k_BkupSaveParameter(AUDIOPLAYER_EQU10_BKP, greq_dynamic_param.user_gain_per_band_dB[9]);
#endif

    /* Enable The Graphic Equalizer */
    greq_getConfig(&greq_dynamic_param, pGreqPersistentMem);
    greq_dynamic_param.enable = 0;            /* Disable the effect */
    if (greq_setConfig(&greq_dynamic_param, pGreqPersistentMem) != GREQ_ERROR_NONE)
    {
      return AUDIO_ERROR_GREQ;
    }

    /* Free mem used by GrEq */
    if(pGreqPersistentMem)
    {
      AUDIO_free(pGreqPersistentMem);
    }

    if(pGreqScratchMem)
    {
      AUDIO_free(pGreqScratchMem);
    }
  }

  if(loudness_used)
  {
    k_BkupSaveParameter(AUDIOPLAYER_LOUD_BKP, AUDIO_loudness);

    /* Disable Loudness filtering */
    biquad_df1_cascade_dynamic_param.enable = 0;            /* Disable the effect */
    if (biquad_df1_cascade_setConfig(&biquad_df1_cascade_dynamic_param, pBIQPersistentMem) != BIQ_ERROR_NONE)
    {
        return AUDIO_ERROR_BIQ;
    }

    /* Free mem used by Biq */
    if(pBIQPersistentMem)
    {
      AUDIO_free(pBIQPersistentMem);
    }

    if(pBIQScratchMem)
    {
      AUDIO_free(pBIQScratchMem);
    }
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
  if(AudioThreadId != 0)
  {  
    osThreadSuspend(AudioThreadId); 
  }

  if(haudio.out.state != AUDIOPLAYER_STOP)
  {
    if(muteState == MUTE_OFF)
    {
      if (BSP_AUDIO_OUT_SetMute(MUTE_ON))
      {
        Error_Handler();
      }
    }
    haudio.out.state = AUDIOPLAYER_STOP; 
    if (BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW))
    {
      Error_Handler();
    }

    f_close(&wav_file);
  }

  return AUDIOPLAYER_ERROR_NONE;
}


/**
  * @brief  Pause Audio stream
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Pause(void)
{
  if(AudioThreadId != 0)
  {
    osThreadSuspend(AudioThreadId); 
  }

  if(haudio.out.state == AUDIOPLAYER_PLAY)
  {
    haudio.out.state = AUDIOPLAYER_PAUSE; 
    if (BSP_AUDIO_OUT_Pause())
    {
      Error_Handler();
    }
  }
  return AUDIOPLAYER_ERROR_NONE;
}


/**
  * @brief  Resume Audio stream
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Resume(void)
{
  if(AudioThreadId != 0)
  {  
    osThreadResume(AudioThreadId);  
  }

  if(haudio.out.state == AUDIOPLAYER_PAUSE)
  {
    haudio.out.state = AUDIOPLAYER_PLAY; 
    if (BSP_AUDIO_OUT_Resume())
    {
      Error_Handler();
    }
  }
  return AUDIOPLAYER_ERROR_NONE;
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
  if((AudioEvent == 0) || (AudioThreadId == 0))
  {
    /* Not yet initialized or already stopped */
    return AUDIOPLAYER_ERROR_NONE;
  }

  if(muteState != state)
  {
    if (BSP_AUDIO_OUT_SetMute(state))
    {
      Error_Handler();
    }
    muteState = state;
  }

   return AUDIOPLAYER_ERROR_NONE;
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
#if defined( USE_GRAPHIC_EQ ) 
        if(graphic_eq_used)
        {
          /* I/O buffers settings */
          BufferHandler.nb_bytes_per_Sample = info->BitPerSample/8; /* 8 bits in 0ne byte */
          BufferHandler.nb_channels = info->NbrChannels; /* stereo or mono */
          BufferHandler.buffer_size = AUDIO_OUT_BUFFER_SIZE/(2*4)/*(2*4)*/; /* just half buffer is process (size per channel) */
          BufferHandler.mode = INTERLEAVED;
        }
#endif /* USE_GRAPHIC_EQ */
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
  if (BSP_AUDIO_OUT_SetMute(MUTE_ON))
  {
    Error_Handler();
  }
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
              BSP_AUDIO_OUT_SetMute(MUTE_ON);
            }
#if defined( USE_GRAPHIC_EQ )
            else
            {
              if((graphic_eq_used) || (loudness_used))
              {
                pBufferHandler->data_ptr = &haudio.buff[0];
              }
              
              if(graphic_eq_used)
              {
                /* Apply GrEq */
                if (greq_process(pBufferHandler, pBufferHandler, pGreqPersistentMem) != GREQ_ERROR_NONE)
                {
                  haudio.out.state = AUDIOPLAYER_ERROR;
                }
              }

              if(loudness_used)
              {
                /* Apply Loudness settings */
                if (biquad_df1_cascade_process(pBufferHandler, pBufferHandler, pBIQPersistentMem) != BIQ_ERROR_NONE)
                {
                  haudio.out.state = AUDIOPLAYER_ERROR;
                }
              }
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
              BSP_AUDIO_OUT_SetMute(MUTE_ON);
            }  
#if defined( USE_GRAPHIC_EQ )
            else
            {
              if((graphic_eq_used) || (loudness_used))
              {
                pBufferHandler->data_ptr = &haudio.buff[AUDIO_OUT_BUFFER_SIZE /2];
              }

              if(graphic_eq_used)
              {
                /* Apply GrEq */
                if (greq_process(pBufferHandler, pBufferHandler, pGreqPersistentMem) != GREQ_ERROR_NONE)
                {
                  haudio.out.state = AUDIOPLAYER_ERROR;
                }
              }

              if(loudness_used)
              {
                /* Apply Loudness settings */
                if (biquad_df1_cascade_process(pBufferHandler, pBufferHandler, pBIQPersistentMem) != BIQ_ERROR_NONE)
                {
                  haudio.out.state = AUDIOPLAYER_ERROR;
                }
              }
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

