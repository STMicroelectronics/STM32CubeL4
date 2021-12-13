/**
  ******************************************************************************
  * @file    guitartuner.c
  * @author  MCD Application Team   
  * @brief   Guitar tuner functions
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
#include "guitartuner.h"

/* Private function prototypes -----------------------------------------------*/
static void GuitarTuner_Init(void);
static void GuitarTuner_DeInit(void);

/* Idd items functions */
static void GuitarTuner_Run(int16_t Index);
static void GuitarTunerStr1(void);
static void GuitarTunerStr2(void);
static void GuitarTunerStr3(void);
static void GuitarTunerStr4(void);
static void GuitarTunerStr5(void);
static void GuitarTunerStr6(void);

KMODULE_RETURN GuitarTunerPreExec(void);
KMODULE_RETURN GuitarTunerExec(void);
KMODULE_RETURN GuitarTunerPostExec(void);

static void GuitarRecord_TransferComplete_CallBack(void);
static void GuitarRecord_HalfTransfer_CallBack(void);
static void GuitarRecord_Error_Callback(void);

static void    DisplayResult(uint8_t  string);
static uint8_t GetStringFromMode(int16_t tuning_mode);

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef            GuitarTunerCrcHandle;
int16_t                      GuitarRecBuff[4096];
int16_t                      GuitarTunerBuff[2048];
__IO uint32_t                GuitarRecHalfBuffCplt  = 0;
__IO uint32_t                GuitarRecBuffCplt      = 0;
void                        *gt_static_mem_ptr;
void                        *gt_dynamic_mem_ptr;
GuitarTuner_static_param_t   gt_input_static_param_ptr;
GuitarTuner_dynamic_param_t  gt_input_dynamic_param_ptr;
buffer_t                    *gt_input_buffer;
uint32_t                     NoteFound = 0;

const tMenuItem GuitarTunerMenuItems[] =
{
    {" STR1 "             , 0, 0,   SEL_EXEC, 0, GuitarTunerStr1, NULL, NULL, 0},
    {" STR2 "             , 0, 0,   SEL_EXEC, 0, GuitarTunerStr2, NULL, NULL, 0},
    {" STR3 "             , 0, 0,   SEL_EXEC, 0, GuitarTunerStr3, NULL, NULL, 0},
    {" STR4 "             , 0, 0,   SEL_EXEC, 0, GuitarTunerStr4, NULL, NULL, 0},
    {" STR5 "             , 0, 0,   SEL_EXEC, 0, GuitarTunerStr5, NULL, NULL, 0},
    {" STR6 "             , 0, 0,   SEL_EXEC, 0, GuitarTunerStr6, NULL, NULL, 0},
    {""                   , 0, 0,   SEL_EXIT, 0, NULL, NULL, NULL, NULL}
};

const tMenu GuitarTunerMenu = {
  "", GuitarTunerMenuItems, countof(GuitarTunerMenuItems), TYPE_TEXT, 1, 0
  };

    

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleGuitarTuner =
{
  MODULE_GUITARTUNER,
  GuitarTunerPreExec,
  GuitarTunerExec,
  GuitarTunerPostExec,
  NULL
};  

/**
  * @brief  Run the GuitarTuner application 
  * @param  None.
  * @note   run and display GuitarTuner Menu.  
  * @retval None.
  */
KMODULE_RETURN GuitarTunerExec(void)
{
  kMenu_Execute(GuitarTunerMenu);
  return KMODULE_OK;
}

/**
  * @brief  Initialize GuitarTuner application 
  * @param  None.  
  * @retval None.
  */
KMODULE_RETURN GuitarTunerPreExec(void)
{
  /* Clear the LCD device */
  BSP_LCD_GLASS_Clear();
   
  /* Initialize audio record */
  if (AUDIO_OK != BSP_AUDIO_IN_Init(AUDIO_FREQUENCY_8K, 
                                    DEFAULT_AUDIO_IN_BIT_RESOLUTION,
                                    DEFAULT_AUDIO_IN_CHANNEL_NBR))
  {
    Error_Handler();
  }

  /* Set Callback function pointers */
  BSP_AUDIO_IN_RegisterCallbacks(GuitarRecord_Error_Callback,
                                 GuitarRecord_HalfTransfer_CallBack,
                                 GuitarRecord_TransferComplete_CallBack);

  /* Initialize GuitarTuner */
  GuitarTuner_Init();
  
  return KMODULE_OK;
}

/**
  * @brief  Uninitialize GuitarTuner application 
  * @param  None.  
  * @retval None.
  */
KMODULE_RETURN GuitarTunerPostExec(void)
{
  /* Un-initialize the GuitarTuner */
  GuitarTuner_DeInit();

  /* Un-initialize audio record */
  if (AUDIO_OK != BSP_AUDIO_IN_DeInit())
  {
    Error_Handler();
  } 

  return KMODULE_OK;
}

/**
  * @brief  Guitar tuner main function
  * @param  Tuning mode.
  * @note   None.  
  * @retval None.
  */
static void GuitarTuner_Run(int16_t tuning_mode)
{
  uint32_t i;
  uint32_t StartIndex = 0;
  uint8_t  String;
  
  /* Clear LCD */
  BSP_LCD_GLASS_Clear();

  /* Get corresponding string */
  String = GetStringFromMode(tuning_mode);
  
  /* Set dynamic parameters at first to disable process */
  gt_input_dynamic_param_ptr.enable = GT_DISABLE;
  if(GT_ERROR_NONE != GuitarTuner_setConfig(&gt_input_dynamic_param_ptr, gt_static_mem_ptr))
  {
    Error_Handler();
  }
  
  /* Set static parameters */
  gt_input_static_param_ptr.tuning_type = GT_TYPE_STANDARD;
  gt_input_static_param_ptr.tuning_mode = tuning_mode;
  if(GT_ERROR_NONE != GuitarTuner_setParam(&gt_input_static_param_ptr, gt_static_mem_ptr))
  {
    Error_Handler();
  }
  
  /* Enable processing */
  gt_input_dynamic_param_ptr.enable = GT_ENABLE;
  if(GT_ERROR_NONE != GuitarTuner_setConfig(&gt_input_dynamic_param_ptr, gt_static_mem_ptr))
  {
    Error_Handler();
  }

  /* Start audio record */
  if(AUDIO_OK != BSP_AUDIO_IN_Record((uint16_t*) GuitarRecBuff, 4096))
  {
    Error_Handler();
  }
  
  /* Loop to display note information */
  do {
    /* Start loopback */
    if((GuitarRecHalfBuffCplt == 1) || (GuitarRecBuffCplt == 1))
    {
      StartIndex = (GuitarRecBuffCplt == 1) ? 2048 : 0;
      /* Store values on guitar tuner buffer */
      for(i = 0; i < 2048; i++)
      {
        GuitarTunerBuff[i] = GuitarRecBuff[i + StartIndex];
      }
      /* Process data */
      if(GT_ERROR_NONE != GuitarTuner_process(gt_input_buffer, NULL, gt_static_mem_ptr))
      {
        Error_Handler();
      }
      /* Get result */
      if(GT_ERROR_NONE != GuitarTuner_getConfig(&gt_input_dynamic_param_ptr, gt_static_mem_ptr))
      {
        Error_Handler();
      }
      /* Display result */
      DisplayResult(String);
      if(GuitarRecHalfBuffCplt  == 1)
      {
        GuitarRecHalfBuffCplt  = 0;
      }
      else
      {
        GuitarRecBuffCplt = 0;
      }
    }      
  } while(kMenu_GetEvent(10) != JOY_LEFT);
   
  /* Stop audio record */
  if(AUDIO_OK != BSP_AUDIO_IN_Stop())
  {
    Error_Handler();
  }

  GuitarRecHalfBuffCplt = 0;
  GuitarRecBuffCplt     = 0;
}

/**
  * @brief  Guitar tuner string 1
  * @param  String to tune.
  * @note   None.  
  * @retval None.
  */
static void GuitarTunerStr1(void)
{
  GuitarTuner_Run(GT_MODE_STRING_1);
}

/**
  * @brief  Guitar tuner string 2
  * @param  String to tune.
  * @note   None.  
  * @retval None.
  */
static void GuitarTunerStr2(void)
{
  GuitarTuner_Run(GT_MODE_STRING_2);
}

/**
  * @brief  Guitar tuner string 3
  * @param  String to tune.
  * @note   None.  
  * @retval None.
  */
static void GuitarTunerStr3(void)
{
  GuitarTuner_Run(GT_MODE_STRING_3);
}

/**
  * @brief  Guitar tuner string 4
  * @param  String to tune.
  * @note   None.  
  * @retval None.
  */
static void GuitarTunerStr4(void)
{
  GuitarTuner_Run(GT_MODE_STRING_4);
}

/**
  * @brief  Guitar tuner string 5
  * @param  String to tune.
  * @note   None.  
  * @retval None.
  */
static void GuitarTunerStr5(void)
{
  GuitarTuner_Run(GT_MODE_STRING_5);
}

/**
  * @brief  Guitar tuner string 6
  * @param  String to tune.
  * @note   None.  
  * @retval None.
  */
static void GuitarTunerStr6(void)
{
  GuitarTuner_Run(GT_MODE_STRING_6);
}

/**
  * @brief  GuitarTuner initialization
  * @param  None
  * @retval None
  */
static void GuitarTuner_Init(void)
{
  /* Initialize CRC used to protect GuitarTuner algorithm */
  GuitarTunerCrcHandle.Instance = CRC;
  __HAL_CRC_RESET_HANDLE_STATE(&GuitarTunerCrcHandle);
  
  GuitarTunerCrcHandle.InputDataFormat              = CRC_INPUTDATA_FORMAT_WORDS;
  GuitarTunerCrcHandle.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_ENABLE;
  GuitarTunerCrcHandle.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_ENABLE;
  GuitarTunerCrcHandle.Init.CRCLength               = CRC_POLYLENGTH_32B;
  GuitarTunerCrcHandle.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE;
  GuitarTunerCrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  if(HAL_OK != HAL_CRC_Init(&GuitarTunerCrcHandle))
  {
    Error_Handler();
  }
  
  /* Allocate static and dynamic memories */
  gt_static_mem_ptr  = (void *)k_malloc(GuitarTuner_static_mem_size);
  gt_dynamic_mem_ptr = (void *)k_malloc(GuitarTuner_dynamic_mem_size);
  if((gt_static_mem_ptr == NULL) || (gt_dynamic_mem_ptr == NULL))
  {
    Error_Handler();
  }
  
  /* Allocate and initialize buffers */
  gt_input_buffer  = (buffer_t *)k_malloc(sizeof(buffer_t));
  if(gt_input_buffer == NULL)
  {
    Error_Handler();
  }
  gt_input_buffer->buffer_size         = 2048;
  gt_input_buffer->nb_bytes_per_Sample = 2;
  gt_input_buffer->nb_channels         = 1;
  gt_input_buffer->mode                = INTERLEAVED;
  gt_input_buffer->data_ptr            = (void *) GuitarTunerBuff;

  /* Reset SoundMeter algorithm */
  if(GT_ERROR_NONE != GuitarTuner_reset(gt_static_mem_ptr, gt_dynamic_mem_ptr))
  {
    Error_Handler();
  }
}

/**
  * @brief  GuitarTuner de-initialization
  * @param  None
  * @retval None
  */
static void GuitarTuner_DeInit(void)
{
  /* De-Initialize CRC used to protect GuitarTuner algorithm */
  if(HAL_OK != HAL_CRC_DeInit(&GuitarTunerCrcHandle))
  {
    Error_Handler();
  }
  
  /* Free static and dynamic memories */
  k_free((void *)gt_static_mem_ptr);
  k_free((void *)gt_dynamic_mem_ptr);
  
  /* De-Allocate and initialize buffers */
  k_free((void *)gt_input_buffer);
}

/**
  * @brief  User callback when record buffer is filled.
  * @param  None
  * @retval None
  */
static void GuitarRecord_TransferComplete_CallBack(void)
{
  GuitarRecBuffCplt = 1;
}

/**
  * @brief  Manages the DMA Half Transfer complete event.
  * @param  None
  * @retval None
  */
static void GuitarRecord_HalfTransfer_CallBack(void)
{ 
  GuitarRecHalfBuffCplt = 1;
}

/**
  * @brief  Audio IN Error callback function.
  * @param  None
  * @retval None
  */
static void GuitarRecord_Error_Callback(void)
{
  Error_Handler();
}

/**
  * @brief  Display result on LCD GLASS
  * @param  None
  * @retval None
  */
static void DisplayResult(uint8_t string)
{
  /* Display result */
  switch(gt_input_dynamic_param_ptr.output_estimated[string])
  {
  case GT_OUTPUT_STRING_WELL_TUNED:
  case GT_OUTPUT_STRING_TUNING_NEARLY_COMPLETED:
    {
      BSP_LCD_GLASS_Clear();
      BSP_LCD_GLASS_DisplayString((uint8_t*)"  OK");
    }
    break;
  case GT_OUTPUT_STRING_NOT_STRAINED_ENOUGH:
    {
      BSP_LCD_GLASS_Clear();
      BSP_LCD_GLASS_DisplayString((uint8_t*)"  ++");
    }
    break;
  case GT_OUTPUT_STRING_NOT_STRAINED_ENOUGH_CLOSE_TO_FINE_TUNE:
    {
      BSP_LCD_GLASS_Clear();
      BSP_LCD_GLASS_DisplayString((uint8_t*)"  +");
    }
    break;
  case GT_OUTPUT_STRING_TOO_STRAINED:
    {
      BSP_LCD_GLASS_Clear();
      BSP_LCD_GLASS_DisplayString((uint8_t*)"  --");
    }
    break;
  case GT_OUTPUT_STRING_TOO_STRAINED_CLOSE_TO_FINE_TUNE:
    {
      BSP_LCD_GLASS_Clear();
      BSP_LCD_GLASS_DisplayString((uint8_t*)"  -");
    }
    break;
  default :
    {
      //BSP_LCD_GLASS_Clear();
    }
    break;
  }
}

/**
  * @brief  Get string from tuning mode
  * @param  Tuning mode
  * @retval String
  */
static uint8_t GetStringFromMode(int16_t tuning_mode)
{
  uint8_t string = 0;
  
  switch(tuning_mode)
  {
  case GT_MODE_STRING_1:
    {
      string = 0;
    }
    break;
  case GT_MODE_STRING_2:
    {
      string = 1;
    }
    break;
  case GT_MODE_STRING_3:
    {
      string = 2;
    }
    break;
  case GT_MODE_STRING_4:
    {
      string = 3;
    }
    break;
  case GT_MODE_STRING_5:
    {
      string = 4;
    }
    break;
  case GT_MODE_STRING_6:
    {
      string = 5;
    }
    break;
  default :
    {
      Error_Handler();
    }
    break;
  }
  return string;
}

/**
  * @brief  Initializes the CRC MSP.
  * @param  hcrc: CRC handle
  * @retval None
  */
/* void HAL_CRC_MspInit(CRC_HandleTypeDef *hcrc) */
/* Already defined in soundmeter.c */

/**
  * @brief  De-Initializes the CRC MSP.
  * @param  hcrc: CRC handle
  * @retval None
  */
/* void HAL_CRC_MspDeInit(CRC_HandleTypeDef *hcrc) */
/* Already defined in soundmeter.c */

/**
  * @}
  */

/**
  * @}
  */


