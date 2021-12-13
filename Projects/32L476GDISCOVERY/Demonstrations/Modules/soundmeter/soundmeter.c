/**
  ******************************************************************************
  * @file    soundmeter.c
  * @author  MCD Application Team   
  * @brief   Sound meter functions
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
#include "k_mem.h"
#include "soundmeter.h"
#include "../Components/cs43l22/cs43l22.h"

/* Private function prototypes -----------------------------------------------*/
static void SoundMeter_Init(void);
static void SoundMeter_DeInit(void);

KMODULE_RETURN SoundMeterExec(void);
static void SoundMeter_Run(void);

static void SoundRecord_TransferComplete_CallBack(void);
static void SoundRecord_HalfTransfer_CallBack(void);
static void SoundRecord_Error_Callback(void);
void SystemClock_BatterySupply_ClockDecrease(void);
void SystemClock_BatterySupply_ClockIncrease(void);

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DB_CALIBRATION      120
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef            SoundMeterCrcHandle;
int16_t                      RecBuff[960];        /* 20ms of data from DFSDM */
int16_t                      SoundMeterBuff[480]; /* 10ms of PCM data */
int16_t                      SoundMeterOutputBuff[480];
__IO uint32_t                DmaRecHalfBuffCplt  = 0;
__IO uint32_t                DmaRecBuffCplt      = 0;
void                        *static_mem_ptr;
void                        *dynamic_mem_ptr;
smr_static_param_t           input_static_param_ptr;
smr_dynamic_param_t          input_dynamic_param_ptr;
buffer_t                    *input_buffer;
buffer_t                    *output_buffer;

const tMenuItem SoundMeterMenuItems[] =
{
    {""             , 0, 0,   TYPE_EXEC, MODULE_NONE, SoundMeter_Run, NULL, NULL, NULL}
};

const tMenu SoundMeterMenu = {
  "", SoundMeterMenuItems, countof(SoundMeterMenuItems), TYPE_EXEC, 2, 3
  };


extern AUDIO_DrvTypeDef cs43l22_drv; 

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleSoundMeter =
{
  MODULE_SOUNDMETER,
  NULL,
  SoundMeterExec,
  NULL,
  NULL
};  

/**
  * @brief  Run the SoundMeter application 
  * @param  None.
  * @note   run and display SoundMeter Menu.  
  * @retval None.
  */
KMODULE_RETURN SoundMeterExec(void)
{
  kMenu_Execute(SoundMeterMenu);
  return KMODULE_OK;
}

/**
  * @brief  Measure in RUN mode 
  * @param  None.
  * @note   None.  
  * @retval None.
  */
static void SoundMeter_Run(void)
{
  uint32_t i;
  uint32_t StartIndex = 0;
  float    mean_level;
  uint32_t ProcessCounter = 0;
  uint8_t  LCDstr[7] = {0};
  
  /* Clear the LCD device */
  BSP_LCD_GLASS_Clear();
  
  if( PowerSupplyMode == SUPPLY_MODE_BATTERY)
  { 
    SystemClock_BatterySupply_ClockIncrease();
  }
  
  /* Initialize audio record */
  /* Initialize audio input */
  if (AUDIO_OK != BSP_AUDIO_IN_Init(AUDIO_FREQUENCY_48K, 
                                    DEFAULT_AUDIO_IN_BIT_RESOLUTION,
                                    DEFAULT_AUDIO_IN_CHANNEL_NBR))
  {
    Error_Handler();
  }
  BSP_AUDIO_IN_RegisterCallbacks(SoundRecord_Error_Callback,
                                 SoundRecord_HalfTransfer_CallBack,
                                 SoundRecord_TransferComplete_CallBack);

  /* Initialize SoundMeter */
  SoundMeter_Init();
  
  /* Start audio record */
  if(AUDIO_OK != BSP_AUDIO_IN_Record((uint16_t*) RecBuff, 960))
  {
    Error_Handler();
  }
  
  /* Loop to display sounmeter information */
  do {
    /* Start loopback */
    if((DmaRecHalfBuffCplt == 1) || (DmaRecBuffCplt == 1))
    {
      StartIndex = (DmaRecBuffCplt == 1) ? 480 : 0;
      /* Store values on SoundMeterBuff */
      for(i = 0; i < 480; i++)
      {
        SoundMeterBuff[i] = RecBuff[i + StartIndex];
      }
      /* Process data */
      if(SMR_ERROR_NONE != smr_process(input_buffer, output_buffer, static_mem_ptr))
      {
        Error_Handler();
      }
      /* Update global variables */
      if(DmaRecHalfBuffCplt  == 1)
      {
        DmaRecHalfBuffCplt  = 0;
      }
      else
      {
        DmaRecBuffCplt = 0;
      }
      ProcessCounter++;
      /* Display result each 0.25s */
      if(ProcessCounter == 25)
      {
        /* Get average level, transform it in Q13.2 and then in float and store it */
        if(SMR_ERROR_NONE != smr_getConfig(&input_dynamic_param_ptr, static_mem_ptr))
        {
          Error_Handler();
        }
        mean_level = (float) (input_dynamic_param_ptr.mean_level_left >> 14);
        mean_level  /= 4;
        /* Display result on LCD Glass and apply calibration to have dBSPL value */
        sprintf((char*)LCDstr, "%lu dB ", (int32_t) mean_level + DB_CALIBRATION);
        BSP_LCD_GLASS_DisplayString(LCDstr);
        ProcessCounter = 0;
      }
    }      
  } while(kMenu_GetEvent(1) != JOY_LEFT);

  /* Stop audio record */
  if(AUDIO_OK != BSP_AUDIO_IN_Stop())
  {
    Error_Handler();
  }

  if (AUDIO_OK != BSP_AUDIO_IN_DeInit())
  {
    Error_Handler();
  } 

  /* Deinitialize the soundmeter */
  SoundMeter_DeInit();

  if( PowerSupplyMode == SUPPLY_MODE_BATTERY)
  { 
    SystemClock_BatterySupply_ClockDecrease();
  }
}

/**
  * @brief  SoundMeter initialization
  * @param  None
  * @retval None
  */
static void SoundMeter_Init(void)
{
  /* Initialize CRC used to protect SoundMeter algorithm */
  SoundMeterCrcHandle.Instance = CRC;
  __HAL_CRC_RESET_HANDLE_STATE(&SoundMeterCrcHandle);
  
  SoundMeterCrcHandle.InputDataFormat              = CRC_INPUTDATA_FORMAT_WORDS;
  SoundMeterCrcHandle.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_ENABLE;
  SoundMeterCrcHandle.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_ENABLE;
  SoundMeterCrcHandle.Init.CRCLength               = CRC_POLYLENGTH_32B;
  SoundMeterCrcHandle.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE;
  SoundMeterCrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  if(HAL_OK != HAL_CRC_Init(&SoundMeterCrcHandle))
  {
    Error_Handler();
  }
  
  /* Allocate static and dynamic memories */
  static_mem_ptr  = (void *)k_malloc(smr_static_mem_size);
  dynamic_mem_ptr = (void *)k_malloc(smr_dynamic_mem_size);
  if((static_mem_ptr == NULL) || (dynamic_mem_ptr == NULL))
  {
    Error_Handler();
  }
  
  /* Allocate and initialize buffers */
  input_buffer  = (buffer_t *)k_malloc(sizeof(buffer_t));
  output_buffer = (buffer_t *)k_malloc(sizeof(buffer_t));
  if((input_buffer == NULL) || (output_buffer == NULL))
  {
    Error_Handler();
  }
  input_buffer->buffer_size         = 480;
  input_buffer->nb_bytes_per_Sample = 2;
  input_buffer->nb_channels         = 1;
  input_buffer->mode                = INTERLEAVED;
  input_buffer->data_ptr            = (void *) SoundMeterBuff;
  output_buffer->buffer_size         = 480;
  output_buffer->nb_bytes_per_Sample = 2;
  output_buffer->nb_channels         = 1;
  output_buffer->mode                = INTERLEAVED;
  output_buffer->data_ptr            = (void *) SoundMeterOutputBuff;
  
  /* Reset SoundMeter algorithm */
  if(SMR_ERROR_NONE != smr_reset(static_mem_ptr, dynamic_mem_ptr))
  {
    Error_Handler();
  }
  
  /* Set dynamic parameters at first to disable process */
  input_dynamic_param_ptr.enable = 0;
  input_dynamic_param_ptr.filter_type = SMR_PREFILTER_AWEIGHTING;
  input_dynamic_param_ptr.averaging_time = 100;
  input_dynamic_param_ptr.mean_level_left = -45;
  input_dynamic_param_ptr.mean_level_right = -45;
  if(SMR_ERROR_NONE != smr_setConfig(&input_dynamic_param_ptr, static_mem_ptr))
  {
    Error_Handler();
  }
  
  /* Set static parameters */
  input_static_param_ptr.sampling_rate = 48000;
  if(SMR_ERROR_NONE != smr_setParam(&input_static_param_ptr, static_mem_ptr))
  {
    Error_Handler();
  }
  
  /* Enable processing */
  input_dynamic_param_ptr.enable = 1;
  input_dynamic_param_ptr.filter_type = SMR_PREFILTER_AWEIGHTING;
  input_dynamic_param_ptr.averaging_time = 100;
  input_dynamic_param_ptr.mean_level_left = -45;
  input_dynamic_param_ptr.mean_level_right = -45;
  if(SMR_ERROR_NONE != smr_setConfig(&input_dynamic_param_ptr, static_mem_ptr))
  {
    Error_Handler();
  }
}

/**
  * @brief  SoundMeter de-initialization
  * @param  None
  * @retval None
  */
static void SoundMeter_DeInit(void)
{
  
  /* De-Initialize CRC used to protect SoundMeter algorithm */
  if(HAL_OK != HAL_CRC_DeInit(&SoundMeterCrcHandle))
  {
    Error_Handler();
  }
  
  /* Free static and dynamic memories */
  k_free((void *)static_mem_ptr);
  k_free((void *)dynamic_mem_ptr);
  
  /* De-Allocate and initialize buffers */
  k_free((void *)input_buffer);
  k_free((void *)output_buffer);
  
}

/**
  * @brief  User callback when record buffer is filled.
  * @param  None
  * @retval None
  */
static void SoundRecord_TransferComplete_CallBack(void)
{
  DmaRecBuffCplt = 1;
}

/**
  * @brief  Manages the DMA Half Transfer complete event.
  * @param  None
  * @retval None
  */
static void SoundRecord_HalfTransfer_CallBack(void)
{ 
  DmaRecHalfBuffCplt = 1;
}

/**
  * @brief  Audio IN Error callback function.
  * @param  None
  * @retval None
  */
static void SoundRecord_Error_Callback(void)
{
  Error_Handler();
}


/**
  * @brief  Initializes the CRC MSP.
  * @param  hcrc: CRC handle
  * @retval None
  */
void HAL_CRC_MspInit(CRC_HandleTypeDef *hcrc)
{
  __HAL_RCC_CRC_CLK_ENABLE();
}

/**
  * @brief  De-Initializes the CRC MSP.
  * @param  hcrc: CRC handle
  * @retval None
  */
void HAL_CRC_MspDeInit(CRC_HandleTypeDef *hcrc)
{
  __HAL_RCC_CRC_FORCE_RESET();
  __HAL_RCC_CRC_RELEASE_RESET();

  __HAL_RCC_CRC_CLK_DISABLE();
}

/**
  * @}
  */

/**
  * @}
  */


