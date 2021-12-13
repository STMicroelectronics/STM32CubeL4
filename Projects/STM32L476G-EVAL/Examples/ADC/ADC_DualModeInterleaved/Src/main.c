/**
  ******************************************************************************
  * @file    ADC/ADC_DualModeInterleaved/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the ADC
  *          peripheral to perform conversions in interleaved dual-mode.
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

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup ADC_DualModeInterleaved
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Application general parameters */
#define VDD_APPLI                      ((uint32_t) 3300)    /* Value of analog voltage supply Vdda (unit: mV) */
#define RANGE_8BITS                    ((uint32_t)  255)    /* Max digital value for a full range of 8 bits */
#define RANGE_12BITS                   ((uint32_t) 4095)    /* Max digital value for a full range of 12 bits */

/* Timer for ADC trigger parameters */
#define TIMER_FREQUENCY_HZ             ((uint32_t) 1000)   /* Timer frequency (unit: Hz). Range is min=1Hz, max=32kHz (with default parameters: timer 16 bits, time base freq min 1Hz). */
#define TIMER_FREQUENCY_RANGE_MIN_HZ   ((uint32_t)    1)   /* Time base range from minimum frequency possible (unit: Hz). With timer 16 bits, maximum frequency possible will be 32000 times this value. */

/* ADC parameters */
#define ADCCONVERTEDVALUES_BUFFER_SIZE ((uint32_t)  256)   /* Size of array of ADC converted values */


#if defined WAVEFORM_VOLTAGE_GENERATION_FOR_TEST
/* Timer for DAC trigger parameters */
#define TIMER_FOR_WAVEFORM_TEST_FREQUENCY_HZ             ((uint32_t)  500)   /* Timer frequency to trigger DAC to send each sample of the waveform (unit: Hz). Range is min=1Hz, max=32kHz (with default parameters: timer 16 bits, time base freq min 1Hz). */
#define TIMER_FOR_WAVEFORM_TEST_FREQUENCY_RANGE_MIN_HZ   ((uint32_t)    1)   /* Time base range from minimum frequency possible (unit: Hz). With timer 16 bits, maximum frequency possible will be 32000 times this value. */

/* Waveform voltage generation for test parameters */
#define WAVEFORM_TEST_SAMPLES_NUMBER                     ((uint32_t)    5)   /* Size of array of DAC waveform */
#define WAVEFORM_TEST_PERIOD_US                          ((WAVEFORM_TEST_SAMPLES_NUMBER * 1000000) / TIMER_FOR_WAVEFORM_TEST_FREQUENCY_HZ)   /* Waveform voltage generation for test period (unit: us) */
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */


/* Private macro -------------------------------------------------------------*/

/**
  * @brief  Computation of ADC master conversion result 
  *         from ADC dual mode conversion result (ADC master and ADC slave
  *         results concatenated on data register of ADC master).
  * @param  DATA: ADC dual mode conversion result
  * @retval None
  */
#define COMPUTATION_DUALMODEINTERLEAVED_ADCMASTER_RESULT(DATA)                 \
  ((DATA) & 0x0000FFFF)

/**
  * @brief  Computation of ADC slave conversion result 
  *         from ADC dual mode conversion result (ADC master and ADC slave
  *         results concatenated on data register of ADC master).
  * @param  DATA: ADC dual mode conversion result
  * @retval None
  */
#define COMPUTATION_DUALMODEINTERLEAVED_ADCSLAVE_RESULT(DATA)                  \
  ((DATA) >> 16)

#if defined WAVEFORM_VOLTAGE_GENERATION_FOR_TEST
/**
  * @brief  Computation of digital value on range 8 bits from voltage value
  *         (unit: mV).
  *         Calculation depends on settings: digital resolution and power
  *         supply of analog voltage Vdda.
  * @param DATA: Voltage value (unit: mV)
  * @retval None
  */
#define COMPUTATION_VOLTAGE_TO_DIGITAL_8BITS(DATA)                             \
  ((DATA) * RANGE_8BITS / VDD_APPLI)
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */


/* Private variables ---------------------------------------------------------*/
/* Peripherals handler declaration */
ADC_HandleTypeDef    AdcHandle_master;
ADC_HandleTypeDef    AdcHandle_slave;
TIM_HandleTypeDef    TimHandle;

#if defined WAVEFORM_VOLTAGE_GENERATION_FOR_TEST
DAC_HandleTypeDef    DacForWaveformTestHandle;  /* DAC used for waveform voltage generation for test */
TIM_HandleTypeDef    TimForWaveformTestHandle;  /* TIM used for waveform voltage generation for test */
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */

/* Variable containing ADC conversions results */
__IO uint32_t   aADCDualConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE];         /* ADC dual mode interleaved conversion results (ADC master and ADC slave results concatenated on data register 32 bits of ADC master). */
__IO uint16_t   aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE];    /* For the purpose of this example, dispatch dual conversion values into arrays corresponding to each ADC conversion values. */
__IO uint16_t   aADCyConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE];    /* For the purpose of this example, dispatch dual conversion values into arrays corresponding to each ADC conversion values. */
uint8_t         ubDCDualConversionComplete = RESET;                              /* Set into ADC conversion complete callback */

#if defined WAVEFORM_VOLTAGE_GENERATION_FOR_TEST
/* Waveform sent by DAC channel. With timer frequency 1kHz and size of 5 samples: waveform 200Hz */
const uint8_t Waveform_8bits[WAVEFORM_TEST_SAMPLES_NUMBER] = 
  {COMPUTATION_VOLTAGE_TO_DIGITAL_8BITS(             0),    /* Expected voltage: 0V,          corresponding digital values: to   0 on 8 bits and    0 and 12 bits */
   COMPUTATION_VOLTAGE_TO_DIGITAL_8BITS(VDD_APPLI *1/4),    /* Expected voltage: 1/4 of Vdda, corresponding digital values: to  63 on 8 bits and 1023 and 12 bits */
   COMPUTATION_VOLTAGE_TO_DIGITAL_8BITS(VDD_APPLI *2/4),    /* Expected voltage: 1/2 of Vdda, corresponding digital values: to 127 on 8 bits and 2047 and 12 bits */
   COMPUTATION_VOLTAGE_TO_DIGITAL_8BITS(VDD_APPLI *3/4),    /* Expected voltage: 3/4 of Vdda, corresponding digital values: to 191 on 8 bits and 3071 and 12 bits */
   COMPUTATION_VOLTAGE_TO_DIGITAL_8BITS(VDD_APPLI    )};    /* Expected voltage: Vdda,        corresponding digital values: to 255 on 8 bits and 4095 and 12 bits */
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);
static void ADC_Config(void);
#if defined ADC_TRIGGER_WITH_TIMER
static void TIM_Config(void);
#endif
#if defined WAVEFORM_VOLTAGE_GENERATION_FOR_TEST
static void WaveformVoltageGenerationForTest(void);
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32L4xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 80 MHz */
  SystemClock_Config();
  
  /*## Configure peripherals #################################################*/
  
  /* Initialize LEDs on board */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED1);


  /* Configure the ADCx (Master) and ADCy (Slave) peripherals */
  ADC_Config();
  
  /* Run the ADC calibration in single-ended mode */  
  if (HAL_ADCEx_Calibration_Start(&AdcHandle_master, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }

  if (HAL_ADCEx_Calibration_Start(&AdcHandle_slave, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }

#if defined ADC_TRIGGER_WITH_TIMER
  /* Configure the TIM peripheral */
  TIM_Config();
#endif

  /*## Enable peripherals ####################################################*/

#if defined ADC_TRIGGER_WITH_TIMER
  /* Timer enable */
  if (HAL_TIM_Base_Start(&TimHandle) != HAL_OK)
  {
    /* Counter Enable Error */
    Error_Handler();
  }
#endif

#if defined WAVEFORM_VOLTAGE_GENERATION_FOR_TEST
  /* Generate a periodic signal on a spare DAC channel */
  WaveformVoltageGenerationForTest();
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */
  

  /*## Start ADC conversions #################################################*/
  
  /* Start ADCx and ADCy multimode conversion on regular group with transfer by DMA */
  if (HAL_ADCEx_MultiModeStart_DMA(&AdcHandle_master,
                                   (uint32_t *)aADCDualConvertedValues,
                                    ADCCONVERTEDVALUES_BUFFER_SIZE
                                  ) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

  /* Array "aADCDualConvertedValues" contains both ADC results on 16 bits:    */
  /*  - ADC master results in the 8 LSB [7:0]                                 */
  /*  - ADC slave results in the 8 MSB [15:8]                                 */

  /* Infinite loop */
  while (1)
  {
  
    /* Turn-on/off LED1 in function of ADC conversion result */
    /*  - Turn-off if ADC conversions buffer is not complete */
    /*  - Turn-on if ADC conversions buffer is complete */

    /* ADC conversion buffer complete variable is updated into ADC conversions*/
    /* complete callback.                                                     */
    if (ubDCDualConversionComplete == RESET)
    {
      BSP_LED_Off(LED1);
    }
    else
    {
      BSP_LED_On(LED1);
    }
  
    /* For information: ADC conversion results are stored into array          */
    /* "aADCDualConvertedValues" (for debug: check into watch window)         */
    
    /* For the purpose of this example, dual conversion values are            */
    /* dispatched into 2 arrays corresponding to each ADC conversion values.  */
    /* (aADCxConvertedValues, aADCyConvertedValues)                           */
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 1
  *            PLL_N                          = 20
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable HSE Oscillator and activate PLL with HSE as source   */
  /* (Default MSI Oscillator enabled at system reset remains ON) */ 
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}


/**
  * @brief  ADC configuration
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_ChannelConfTypeDef   sConfig;
  ADC_MultiModeTypeDef     MultiModeInit;

  /* Configuration of ADC (master) init structure: ADC parameters and regular group */
  AdcHandle_master.Instance = ADCx;

   if (HAL_ADC_DeInit(&AdcHandle_master) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }
  
   AdcHandle_slave.Instance = ADCy;
   if (HAL_ADC_DeInit(&AdcHandle_slave) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }

  AdcHandle_master.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;          /* Asynchronous clock mode, input ADC clock not divided */
  AdcHandle_master.Init.Resolution            = ADC_RESOLUTION_12B;             /* 12-bit resolution for converted data */
  AdcHandle_master.Init.DataAlign             = ADC_DATAALIGN_RIGHT;           /* Right-alignment for converted data */
  AdcHandle_master.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle_master.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;           /* EOC flag picked-up to indicate conversion end */
  AdcHandle_master.Init.LowPowerAutoWait      = DISABLE;                       /* Auto-delayed conversion feature disabled */
#if defined ADC_TRIGGER_WITH_TIMER
  AdcHandle_master.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
#else
  AdcHandle_master.Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode to have maximum conversion speed (no delay between conversions) */
#endif
  AdcHandle_master.Init.NbrOfConversion       = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle_master.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle_master.Init.NbrOfDiscConversion   = 1;                             /* Parameter discarded because sequencer is disabled */
#if defined ADC_TRIGGER_WITH_TIMER
  AdcHandle_master.Init.ExternalTrigConv      = ADC_EXTERNALTRIG_T3_TRGO;       /* Timer 3 external event triggering the conversion */
  AdcHandle_master.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
#else
  AdcHandle_master.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trigger the 1st conversion manually, without external event */
  AdcHandle_master.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because trigger of conversion by software start (no external event) */
#endif
  AdcHandle_master.Init.DMAContinuousRequests = ENABLE;                        /* DMA circular mode selected */
  AdcHandle_master.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;      /* DR register is overwritten with the last conversion result in case of overrun */
  AdcHandle_master.Init.OversamplingMode      = DISABLE;                       /* No oversampling */


  if (HAL_ADC_Init(&AdcHandle_master) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }

  /* Configuration of ADC (slave) init structure: ADC parameters and regular group */
  AdcHandle_slave.Instance = ADCy;

  /* Same configuration as ADC master, with continuous mode and external      */
  /* trigger disabled since ADC master is triggering the ADC slave            */
  /* conversions                                                              */
  AdcHandle_slave.Init = AdcHandle_master.Init;
  AdcHandle_slave.Init.ContinuousConvMode    = DISABLE;
  AdcHandle_slave.Init.ExternalTrigConv      = ADC_SOFTWARE_START;

  if (HAL_ADC_Init(&AdcHandle_slave) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }
  
 
  /* Configuration of channel on ADC (master) regular group on sequencer rank 1 */
  /* Note: Considering IT occurring after each number of                      */
  /*       "ADCCONVERTEDVALUES_BUFFER_SIZE"  ADC conversions (IT by DMA end   */
  /*       of transfer), select sampling time and ADC clock with sufficient   */
  /*       duration to not create an overhead situation in IRQHandler.        */
  sConfig.Channel      = ADCx_CHANNELa;                /* Sampled channel number */
  sConfig.Rank         = ADC_REGULAR_RANK_1;          /* Rank of sampled channel number ADCx_CHANNEL */
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;    /* Minimum sampling time */
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;            /* Single-ended input channel */
  sConfig.OffsetNumber = ADC_OFFSET_NONE;             /* No offset subtraction */ 
  sConfig.Offset = 0;                                 /* Parameter discarded because offset correction is disabled */

  if (HAL_ADC_ConfigChannel(&AdcHandle_master, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }

  /* Configuration of channel on ADC (slave) regular group on sequencer rank 1 */
  /* Same channel as ADCx for dual mode interleaved: both ADC are converting  */
  /* the same channel.                                                        */
  sConfig.Channel = ADCx_CHANNELa;
  
  if (HAL_ADC_ConfigChannel(&AdcHandle_slave, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }

  /* Configuration of multimode */
  /* Multimode parameters settings and set ADCy (slave) under control of      */
  /* ADCx (master).                                                           */
  MultiModeInit.Mode = ADC_DUALMODE_INTERL;
  MultiModeInit.DMAAccessMode = ADC_DMAACCESSMODE_12_10_BITS;  /* ADC and DMA configured in resolution 12 bits to match with both ADC master and slave resolution */
  MultiModeInit.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
  
  if (HAL_ADCEx_MultiModeConfigChannel(&AdcHandle_master, &MultiModeInit) != HAL_OK)
  {
    /* Multimode Configuration Error */
    Error_Handler();
  }
}

#if defined ADC_TRIGGER_WITH_TIMER
/**
  * @brief  TIM configuration
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  TIM_MasterConfigTypeDef sMasterConfig;
  RCC_ClkInitTypeDef clkinitstruct = {0};         /* Temporary variable to retrieve RCC clock configuration */
  uint32_t tmpdummy;                              /* Temporary variable to retrieve RCC clock configuration */
  
  uint32_t timer_clock_source_freq = 0;           /* Timer clock source frequency */
  uint32_t timer_prescaler_timebase_freq_min = 0; /* Time base prescaler to have timebase aligned on minimum frequency possible */
  
  /* Configuration of timer as time base */ 
  /* Time base frequency is defined by literals:                              */
  /* - TIMER_FREQUENCY_HZ: time base frequency (unit: Hz).                    */
  /* - TIMER_FREQUENCY_RANGE_MIN_HZ: time base minimum frequency possible     */
  /*   (unit: Hz)                                                             */
  /* Note: Refer to comments at these literals definition for more details.   */
  
  /* Retrieve timer clock source frequency */
  HAL_RCC_GetClockConfig(&clkinitstruct, &tmpdummy);
  /* If APB1 prescaler is different of 1, timers have a factor x2 on their    */
  /* clock source.                                                            */
  if (clkinitstruct.APB1CLKDivider == RCC_HCLK_DIV1)
  {
    timer_clock_source_freq = HAL_RCC_GetPCLK1Freq();
  }
  else
  {
    timer_clock_source_freq = HAL_RCC_GetPCLK1Freq() *2;
  }
  
  /* Computation of timer prescaler */
  /* (computation for timer 16 bits, additional +1 to round the prescaler up) */
  timer_prescaler_timebase_freq_min = (timer_clock_source_freq / ( (0xFFFF-1) * TIMER_FREQUENCY_RANGE_MIN_HZ)) +1;
  
  /* Set timer instance */
  TimHandle.Instance = TIMx;
  
  /* Configure timer parameters */
  TimHandle.Init.Period            = ((timer_clock_source_freq / (timer_prescaler_timebase_freq_min * TIMER_FREQUENCY_HZ)) - 1);
  TimHandle.Init.Prescaler         = (timer_prescaler_timebase_freq_min - 1);
  TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0x0;
  
  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    /* Timer initialization Error */
    Error_Handler();
  }

  /* Timer TRGO selection */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if (HAL_TIMEx_MasterConfigSynchronization(&TimHandle, &sMasterConfig) != HAL_OK)
  {
    /* Timer TRGO selection Error */
    Error_Handler();
  }
  
}
#endif

#if defined WAVEFORM_VOLTAGE_GENERATION_FOR_TEST
/**
  * @brief  For this example, generate a periodic signal on a spare DAC
  *         channel, so user has just to connect a wire between DAC channel 
  *         (pin PA.00) and ADC channel (pin PA.00) to run this example.
  *         (this prevents the user from resorting to an external signal generator)
  * @param  None
  * @retval None
  */
static void WaveformVoltageGenerationForTest(void)
{
  DAC_ChannelConfTypeDef sConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  RCC_ClkInitTypeDef clkinitstruct = {0};         /* Temporary variable to retrieve RCC clock configuration */
  uint32_t tmpdummy;                              /* Temporary variable to retrieve RCC clock configuration */
  
  uint32_t timer_clock_source_freq = 0;           /* Timer clock source frequency */
  uint32_t timer_prescaler_timebase_freq_min = 0; /* Time base prescaler to have timebase aligned on minimum frequency possible */
  
  /* Configuration of timer as time base */
  /* Time base frequency is defined by literals:                              */
  /* - TIMER_FOR_WAVEFORM_TEST_FREQUENCY_HZ: time base frequency (unit: Hz).  */
  /* - TIMER_FOR_WAVEFORM_TEST_FREQUENCY_RANGE_MIN_HZ: time base minimum      */
  /*   frequency possible (unit: Hz).                                         */
  /* Note: Refer to comments at these literals definition for more details.   */
  
  /* Retrieve timer clock source frequency */
  HAL_RCC_GetClockConfig(&clkinitstruct, &tmpdummy);
  /* If APB1 prescaler is different of 1, timers have a factor x2 on their    */
  /* clock source.                                                            */
  if (clkinitstruct.APB1CLKDivider == RCC_HCLK_DIV1)
  {
    timer_clock_source_freq = HAL_RCC_GetPCLK1Freq();
  }
  else
  {
    timer_clock_source_freq = HAL_RCC_GetPCLK1Freq() *2;
  }
  
  /* Computation of timer prescaler */
  /* (computation for timer 16 bits, additional +1 to round the prescaler up) */
  timer_prescaler_timebase_freq_min = (timer_clock_source_freq / ( (0xFFFF-1) * TIMER_FOR_WAVEFORM_TEST_FREQUENCY_RANGE_MIN_HZ)) +1;
  
  /* Set timer instance */
  TimForWaveformTestHandle.Instance = TIMy;
  
  /* Configure timer parameters */
  TimForWaveformTestHandle.Init.Period            = ((timer_clock_source_freq / (timer_prescaler_timebase_freq_min * TIMER_FOR_WAVEFORM_TEST_FREQUENCY_HZ)) - 1);
  TimForWaveformTestHandle.Init.Prescaler         = (timer_prescaler_timebase_freq_min - 1);
  TimForWaveformTestHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  TimForWaveformTestHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimForWaveformTestHandle.Init.RepetitionCounter = 0x0;
  
  if (HAL_TIM_Base_Init(&TimForWaveformTestHandle) != HAL_OK)
  {
    /* Timer initialization Error */
    Error_Handler();
  }

  /* Timer TRGO selection */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if (HAL_TIMEx_MasterConfigSynchronization(&TimForWaveformTestHandle, &sMasterConfig) != HAL_OK)
  {
    /* Timer TRGO selection Error */
    Error_Handler();
  }


  
  
  /* Configuration of DACx peripheral */
  DacForWaveformTestHandle.Instance = DACx;

  /* DeInitialize the DAC peripheral */
  if (HAL_DAC_DeInit(&DacForWaveformTestHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Initialize the DAC peripheral */
  if (HAL_DAC_Init(&DacForWaveformTestHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Configuration of DAC channel */
  sConfig.DAC_Trigger = DACx_TRIGGER_Tx_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;

  if (HAL_DAC_ConfigChannel(&DacForWaveformTestHandle, &sConfig, DACx_CHANNELa) != HAL_OK)
  {
    /* Channel configuration error */
    Error_Handler();
  }
  
  
  /*## Enable peripherals ####################################################*/
  
  /* Timer counter enable */
  if (HAL_TIM_Base_Start(&TimForWaveformTestHandle) != HAL_OK)
  {
    /* Counter Enable Error */
    Error_Handler();
  }
  
  /* Enable DAC Channel1 and associated DMA */
  if (HAL_DAC_Start_DMA(&DacForWaveformTestHandle, DACx_CHANNELa, (uint32_t *)Waveform_8bits, WAVEFORM_TEST_SAMPLES_NUMBER, DAC_ALIGN_8B_R) != HAL_OK)
  {
    /* Start DMA Error */
    Error_Handler();
  }
  
}
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */

/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : AdcHandle handle
  * @note   This example shows a simple way to report end of conversion
  *         and get conversion result. You can add your own implementation.
  * @note   When ADC_TRIGGER_WITH_TIMER is disabled, conversions are software-triggered
  *         and are too fast for DMA post-processing. Therefore, to reduce the computational 
  *         load, the output buffer filled up by the DMA is post-processed only when 
  *         ADC_TRIGGER_WITH_TIMER is enabled.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{
#if defined ADC_TRIGGER_WITH_TIMER    
  uint32_t tmp_index = 0;
  
  /* For the purpose of this example, dispatch dual conversion values         */
  /* into 2 arrays corresponding to each ADC conversion values.               */
  for (tmp_index = (ADCCONVERTEDVALUES_BUFFER_SIZE/2); tmp_index < ADCCONVERTEDVALUES_BUFFER_SIZE; tmp_index++)
  {
    aADCxConvertedValues[tmp_index] = (uint16_t) COMPUTATION_DUALMODEINTERLEAVED_ADCMASTER_RESULT(aADCDualConvertedValues[tmp_index]);
    aADCyConvertedValues[tmp_index] = (uint16_t) COMPUTATION_DUALMODEINTERLEAVED_ADCSLAVE_RESULT(aADCDualConvertedValues[tmp_index]);
  }
#endif  
  
  /* Set variable to report DMA transfer status to main program */
  ubDCDualConversionComplete = SET;
}

/**
  * @brief  Conversion DMA half-transfer callback in non blocking mode 
  * @param  hadc: ADC handle
  * @note   When ADC_TRIGGER_WITH_TIMER is disabled, conversions are software-triggered
  *         and are too fast for DMA post-processing. Therefore, to reduce the computational 
  *         load, the output buffer filled up by the DMA is post-processed only when 
  *         ADC_TRIGGER_WITH_TIMER is enabled.
  * @retval None
  */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
#if defined ADC_TRIGGER_WITH_TIMER
  uint32_t tmp_index = 0;
  
  /* For the purpose of this example, dispatch dual conversion values         */
  /* into 2 arrays corresponding to each ADC conversion values.               */
  for (tmp_index = 0; tmp_index < (ADCCONVERTEDVALUES_BUFFER_SIZE/2); tmp_index++)
  {
    aADCxConvertedValues[tmp_index] = (uint16_t) COMPUTATION_DUALMODEINTERLEAVED_ADCMASTER_RESULT(aADCDualConvertedValues[tmp_index]);
    aADCyConvertedValues[tmp_index] = (uint16_t) COMPUTATION_DUALMODEINTERLEAVED_ADCSLAVE_RESULT(aADCDualConvertedValues[tmp_index]);
  }
#endif

  /* Reset variable to report DMA transfer status to main program */
  ubDCDualConversionComplete = RESET;
}

/**
  * @brief  ADC error callback in non blocking mode
  *        (ADC conversion with interruption or transfer by DMA)
  * @param  hadc: ADC handle
  * @note   When ADC_TRIGGER_WITH_TIMER is disabled, conversions are software-triggered
  *         and are too fast for DMA post-processing. Overrun issues are observed and to
  *         avoid ending up in the infinite loop of Error_Handler(), no call to this
  *         latter is done in case of HAL_ADC_ERROR_OVR error.
  * @retval None
  */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
#if !defined ADC_TRIGGER_WITH_TIMER
  /* In case of ADC error, call main error handler */
  if (HAL_IS_BIT_CLR(hadc->ErrorCode, HAL_ADC_ERROR_OVR)) 
  {
#endif    
  Error_Handler();
#if !defined ADC_TRIGGER_WITH_TIMER            
  }
#endif 
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with a potential error */
  
  /* In case of error, LED3 is toggling at a frequency of 1Hz */
  while(1)
  {
    /* Toggle LED3 */
    BSP_LED_Toggle(LED3);
    HAL_Delay(500);
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

