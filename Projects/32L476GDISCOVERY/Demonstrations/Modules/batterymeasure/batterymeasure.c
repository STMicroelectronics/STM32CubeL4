/**
  ******************************************************************************
  * @file    batterymeasure.c
  * @author  MCD Application Team   
  * @brief   System information functions
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
#include "k_menu.h"
#include "k_module.h"
#include "batterymeasure.h"

/* ## Definition of ADC related resources ################################### */
// #define DISPLAY_VOLT_IN_V               1

/* Definition of ADCx clock resources */
#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC_CLK_ENABLE()
#define ADCx_CLK_DISABLE()              __HAL_RCC_ADC_CLK_DISABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

/* Definition of ADCx channels */
#define SAMPLINGTIME                    ADC_SAMPLETIME_640CYCLES_5

/* Private function prototypes -----------------------------------------------*/
/* Battery Measure Module functions */
KMODULE_RETURN BatteryMeasureExec(void);

/* Battery Measure items functions */
static void BatteryMeasure_Run(void);

/* Battery Measure private sub-functions */
static void ADC_Config(void);
static void Volt_Convert(uint32_t Value, uint16_t *DisplayString);

/* Private Variable ----------------------------------------------------------*/
const tMenuItem BatteryMeasureMenuItems[] =
{
    {""             , 0, 0,   TYPE_EXEC, MODULE_NONE, BatteryMeasure_Run, NULL, NULL, NULL}
};

const tMenu BatteryMeasureMenu = {
  "", BatteryMeasureMenuItems, countof(BatteryMeasureMenuItems), TYPE_EXEC, 2, 3
  };

ADC_ChannelConfTypeDef   sConfig;

/* Variables for ADC conversions results computation to physical values */
uint16_t   uhADCChannelToDAC_mVolt = 0;

  /* display variable */
uint16_t datatodisplay[LCD_DIGIT_MAX_NUMBER] = {0};

/* Private variables ---------------------------------------------------------*/
/* ADC handler declaration */
ADC_HandleTypeDef    AdcHandle;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define UNIT_CHAR_NB                              2

#define COMPENSATE_VBAT_DIVIDER                   3         /* the converted digital value is on third of the VBAT voltage */ 

/* 3000 = 3300 - 300 (diode)*/
#define VDD_APPLI                      ((uint32_t) 3000)    /* Value of analog voltage (unit: mV) */
#define RANGE_12BITS                   ((uint32_t) 4095)    /* Max value with a full range of 12 bits */

#define VDD_REFINT                     ((uint32_t) 1200)    /* Value of VrefInt            (unit: mV) */
#define RANGE_12BITS                   ((uint32_t) 4095)    /* Max value with a full range of 12 bits */

/* Private macro -------------------------------------------------------------*/
/**
  * @brief  Computation of voltage (unit: mV) from ADC measurement digital
  *         value on range 12 bits.
  *         Calculation validity conditioned to settings: 
  *          - ADC resolution 12 bits (need to scale value if using a different 
  *            resolution).
  *          - Power supply of analog voltage Vdda 3.3V (need to scale value 
  *            if using a different analog voltage supply value).
  * @param ADC_DATA: Digital value measured by ADC
  * @retval None
  */
#define COMPUTATION_DIGITAL_12BITS_TO_VOLTAGE(ADC_DATA)                        \
  ( ((ADC_DATA) * VDD_APPLI) / RANGE_12BITS)

/* Private macro -------------------------------------------------------------*/
/**
  * @brief  Computation of voltage (unit: mV) from ADC measurement digital
  *         value on range 12 bits.
  *         Calculation validity conditioned to settings: 
  *          - ADC resolution 12 bits (need to scale value if using a different 
  *            resolution).
  *          - Power supply of analog voltage Vdda 3.3V (need to scale value 
  *            if using a different analog voltage supply value).
  * @param ADC_DATA: Digital value measured by ADC (VrefInt)
  * @retval None
  */
#define COMPUTATION_VDD_USING_VREFINT_12BITS_TO_VOLTAGE(ADC_DATA)                        \
  ( ((VDD_REFINT) * RANGE_12BITS) / ADC_DATA)
    
    
    
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleBatteryMeasure =
{
  MODULE_BATTERYMEASURE,
  NULL,
  BatteryMeasureExec,
  NULL,
  NULL
};  

/**
  * @brief  Run the Idd Measurement application 
  * @param  None.
  * @note   run and display Idd Menu.  
  * @retval None.
  */
KMODULE_RETURN BatteryMeasureExec(void)
{
  kMenu_Execute(BatteryMeasureMenu);
  return KMODULE_OK;
}

/**
  * @brief  Measure in RUN mode 
  * @param  None.
  * @note   None.  
  * @retval None.
  */
static void BatteryMeasure_Run(void)
{
  uint32_t VoltReadValue = 0;
  
   /* Clear the LCD device */
   BSP_LCD_GLASS_Clear();

/* -------------------------------------------------------------------------- */   
  /* Initialize the batterymeasure */
  /* Configure the ADC peripheral */
  ADC_Config();
  
  /* Display batterymeasure information */
  
  /* Retrieve ADC conversion */ 
  VoltReadValue = HAL_ADC_GetValue(&AdcHandle);
    
  /* Convert Voltage value in order to display it on LCD glass */
  Volt_Convert(VoltReadValue, datatodisplay);

  /* display measure */
  BSP_LCD_GLASS_DisplayStrDeci(datatodisplay);

  /* Display for 2,5sec */ 
  HAL_Delay(2500);
   
  if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }
  
}

/**
  * @brief  ADC configuration
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  /* ADC handlers */
  ADC_HandleTypeDef adchandle = {0};

  /* Configuration of ADCx init structure: ADC parameters and regular group */
  AdcHandle.Instance = ADCx;
    
  if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }

  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;          /* Asynchronous clock mode, input ADC clock not divided */
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;             /* 12-bit resolution for converted data */
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;           /* Right-alignment for converted data */
  AdcHandle.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;           /* EOC flag picked-up to indicate conversion end */
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;                       /* Auto-delayed conversion feature disabled */
  AdcHandle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
  AdcHandle.Init.NbrOfConversion       = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.NbrOfDiscConversion   = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */
  AdcHandle.Init.DMAContinuousRequests = DISABLE;                       /* DMA one-shot mode selected (not applied to this example) */
  AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;      /* DR register is overwritten with the last conversion result in case of overrun */
  AdcHandle.Init.OversamplingMode      = DISABLE;                       /* No oversampling */
  
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization Error */
    Error_Handler();
  }

  /*##-2- Configure ADC regular channel ######################################*/
  sConfig.Channel      = ADC_CHANNEL_VREFINT;         /* Sampled channel number */
  // sConfig.Channel      = ADC_CHANNEL_VBAT;         /* Sampled channel number */
  sConfig.Rank         = ADC_REGULAR_RANK_1;          /* Rank of sampled channel number ADCx_CHANNEL */
  sConfig.SamplingTime = SAMPLINGTIME;                /* Sampling time (number of clock cycles unit) */
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;            /* Single-ended input channel */
  sConfig.OffsetNumber = ADC_OFFSET_NONE;             /* No offset subtraction */ 
  sConfig.Offset = 0;                                 /* Parameter discarded because offset correction is disabled */

  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }

  /* Start ADC2 & ADC 3 */
  /* Cut 1.0/1.1 had known bug (see Errata).*/ 
  /* Reason: */
  /* otherwise ADC2/ADC3 will short the VREFINT node to GND */ 
  /*   When ADC1 select channel 0 (VREFINT), ADC2 and ADC3 need to be enabled */
  adchandle.Instance = ADC2;
  ADC_Enable(&adchandle);
  
  adchandle.Instance = ADC3;
  ADC_Enable(&adchandle);

  /*##-3- Calibrate ADC then Start the conversion process ####################*/  
  if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED) !=  HAL_OK)
  {
    /* ADC Calibration Error */
    Error_Handler();
  }
  
  if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler();
  }

  /*##-4- Wait for the end of conversion #####################################*/
  /*  For simplicity reasons, this example is just waiting till the end of the
      conversion, but application may perform other tasks while conversion
      operation is ongoing. */
  if (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK)
  {
    /* End Of Conversion flag not set on time */
    Error_Handler();
  }

}

/**
  * @brief  Convert value to display correct volt unit
  * @param  None
  * @retval None
  */
static void Volt_Convert(uint32_t Value, uint16_t *DisplayString)
{
  uint32_t valuetoconvert = 0;
  
  /* VREFINT = 1.2 V*/
  if (sConfig.Channel == ADC_CHANNEL_VREFINT)
  {

   /* VFInt is 1.2 V */      
   /* Value is what is measured on ADC */
   /* VDD is then derived as full ADC scale */

    uhADCChannelToDAC_mVolt = COMPUTATION_VDD_USING_VREFINT_12BITS_TO_VOLTAGE((uint16_t)Value);
    
  }
  /* VBAT = 3.3 V */
  else if (sConfig.Channel == ADC_CHANNEL_VBAT)
  {
    /* Gain is not correct to be updated */
    uhADCChannelToDAC_mVolt = COMPENSATE_VBAT_DIVIDER * COMPUTATION_VDD_USING_VREFINT_12BITS_TO_VOLTAGE((uint16_t)Value);
  }
  /* error */
  else 
  { 
    while(1); 
  }
  
  /* DisplayString:           */
  /*                0   ' '  */
  /*                1   ' '  */
  /*                2   unit */
  /*                3   0.1  */
  /*                4   ' '  */
  /*                5   'V'  */
  
#ifdef DISPLAY_VOLT_IN_V
  uint16_t unit = 0;
  
  if(uhADCChannelToDAC_mVolt < 1000)
  {
    /* lower than 1V */
  
    *(DisplayString + 2) ='0';
    
    /* measured value is in V & we want 1 digit before coma */
    valuetoconvert = uhADCChannelToDAC_mVolt;

    Convert_IntegerIntoChar(valuetoconvert, DisplayString);
  }
  else
  {
    /* measured value is in V */

    unit = (uhADCChannelToDAC_mVolt / 1000); 
    valuetoconvert =  uhADCChannelToDAC_mVolt - (uhADCChannelToDAC_mVolt / 1000) * 1000;
    
    Convert_IntegerIntoChar(valuetoconvert, DisplayString);
  /* DisplayString */ 
  /* Currently                */
  /*                0         */
  /*                1         */
  /*                2         */
  /*     -->        3   0.1   */
  /*                4   0.01  */
  /*                5   0.001 */
  }

  *(DisplayString + 4) = *(DisplayString + 3);
  
  *(DisplayString + 0) =' ';
  
  *(DisplayString + 1) =' ';
  
  *(DisplayString + 2) = unit + '0';

  /* Add '.' */
  *(DisplayString + 3) = '.';
 
  /* Add SI preficx & volt information */
  *(DisplayString + 5) = (uint16_t) 'V';

  /* BAR Display Add SI preficx & volt information */
  BSP_LCD_GLASS_DisplayBar (LCD_BAR_0);
  BSP_LCD_GLASS_DisplayBar (LCD_BAR_1);
  BSP_LCD_GLASS_DisplayBar (LCD_BAR_2);
  BSP_LCD_GLASS_DisplayBar (LCD_BAR_3);  
  
  
#else
  
  /* measured value is in mV */
  valuetoconvert = uhADCChannelToDAC_mVolt;
  Convert_IntegerIntoChar(valuetoconvert, DisplayString);    

  /* Shift 4 digits left to the display */
  *(DisplayString + 0) = *(DisplayString + 2);
  *(DisplayString + 1) = *(DisplayString + 3);
  *(DisplayString + 2) = *(DisplayString + 4);
  *(DisplayString + 3) = *(DisplayString + 5);
   
  /* Add SI preficx & volt information */
  *(DisplayString + 4) = (uint16_t) 'm';

  /* Add SI preficx & volt information */
  *(DisplayString + 5) = (uint16_t) 'V';
  
#endif
}

/**
  * @}
  */

/**
  * @brief ADC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  if(hadc->Instance == ADCx)
  {
    /* ADC Periph clock enable */
    ADCx_CLK_ENABLE();

    /* ADC Periph interface clock configuration */
    __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_SYSCLK);
  }
}

/**
  * @brief ADC MSP De-Initialization
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
  /* ADC handlers */
  ADC_HandleTypeDef adchandle = {0};
  
  if(hadc->Instance == ADCx)
  {
    /* Disable first ADC2 and 3 */
    /* Cut 1.0/1.1 had known bug (see Errata).*/ 
    adchandle.Instance = ADC2;
    ADC_Disable(&adchandle);

    adchandle.Instance = ADC3;
    ADC_Disable(&adchandle);
    
    /* ADC Periph clock disable */
    ADCx_CLK_DISABLE();
  }
}
/**
  * @}
  */

/**
  * @}
  */


