/**
  ******************************************************************************
  * @file    k_temperature.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel temperature functions 
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
#include "k_temperature.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_TEMPERATURE
  * @brief Kernel temperature routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DFSDM_Channel_HandleTypeDef  DfsdmVoltageChannelHandle;
DFSDM_Channel_HandleTypeDef  DfsdmCurrentChannelHandle;
DFSDM_Filter_HandleTypeDef   DfsdmFilterHandle;
TIM_HandleTypeDef            htim;
float                        voltageValue = 0;
float                        currentValue = 0;
uint32_t                     voltageAvailable = 0;
uint32_t                     currentAvailable = 0;
float                        temperature = 0;

/* Private function prototypes -----------------------------------------------*/
static void DFSDM_Init(void);
static void DFSDM_DeInit(void);
static void TIMER_Init(void);
static void TIMER_DeInit(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Temperature sensor initialization.
  * @param  None
  * @retval None
  */
void k_TemperatureInit(void)
{
  /* Initialize DFSDM channels and filter */
  DFSDM_Init();
  
  /* Initialize timer */
  TIMER_Init();

  /* Start DFSDM conversions */
  if(HAL_OK != HAL_DFSDM_FilterInjectedStart_IT(&DfsdmFilterHandle))
  {
  }
  
  /* Start timer */
  if(HAL_OK != HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_1))
  {
  }
}

/**
  * @brief  Temperature sensor Deinitialization.
  * @param  None
  * @retval None
  */
void k_TemperatureDeInit(void)
{
  /* DeInitialize DFSDM channels and filter */
  DFSDM_DeInit();
  
  /* Stop timer */
  if(HAL_OK != HAL_TIM_PWM_Stop(&htim, TIM_CHANNEL_1))
  {
  }

  /* DeInitialize timer */
  TIMER_DeInit();
}

/**
  * @brief  Get temperature. 
  * @param  Temperature: Pointer to Time structure
  * @retval None
  */
uint8_t k_GetTemperature(void)
{
  /* Waiting current and voltage values available */
  if((voltageAvailable == 1) && (currentAvailable == 1))
  {
    /* Compute temperature value */
    temperature = ((voltageValue/currentValue) - 100)/0.385f;
    voltageAvailable = 0;
    currentAvailable = 0;
  }
  return (uint8_t)temperature;
}

/**
  * @brief  DFSDM channels and filter Deinitialization
  * @param  None
  * @retval None
  */
static void DFSDM_DeInit(void)
{
  /* DeInitialize channel 3 (voltage channel)*/
  if(HAL_OK != HAL_DFSDM_ChannelDeInit(&DfsdmVoltageChannelHandle))
  {
  }

  /* DeInitialize channel 2 (current channel)*/
  if(HAL_OK != HAL_DFSDM_ChannelDeInit(&DfsdmCurrentChannelHandle))
  {
  }
  
  /* Initialize filter 0 (both channels) */
  if(HAL_OK != HAL_DFSDM_FilterDeInit(&DfsdmFilterHandle))
  {
  }
}

/**
  * @brief  DFSDM channels and filter initialization
  * @param  None
  * @retval None
  */
static void DFSDM_Init(void)
{
  /* Initialize channel 3 (voltage channel)*/
  __HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(&DfsdmVoltageChannelHandle);
  DfsdmVoltageChannelHandle.Instance                      = DFSDM1_Channel3;
  DfsdmVoltageChannelHandle.Init.OutputClock.Activation   = ENABLE;
  DfsdmVoltageChannelHandle.Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_SYSTEM;
  DfsdmVoltageChannelHandle.Init.OutputClock.Divider      = 80; /* 80 MHz/80 = 1MHz */
  DfsdmVoltageChannelHandle.Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  DfsdmVoltageChannelHandle.Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE; /* N.U. */
  DfsdmVoltageChannelHandle.Init.Input.Pins               = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
  DfsdmVoltageChannelHandle.Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_FALLING;
  DfsdmVoltageChannelHandle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  DfsdmVoltageChannelHandle.Init.Awd.FilterOrder          = DFSDM_CHANNEL_FASTSINC_ORDER; /* N.U. */
  DfsdmVoltageChannelHandle.Init.Awd.Oversampling         = 10; /* N.U. */
  DfsdmVoltageChannelHandle.Init.Offset                   = 0;
  DfsdmVoltageChannelHandle.Init.RightBitShift            = 0;
  if(HAL_OK != HAL_DFSDM_ChannelInit(&DfsdmVoltageChannelHandle))
  {
  }

  /* Initialize channel 2 (current channel)*/
  __HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(&DfsdmCurrentChannelHandle);
  DfsdmCurrentChannelHandle.Instance                      = DFSDM1_Channel2;
  DfsdmCurrentChannelHandle.Init.OutputClock.Activation   = ENABLE;
  DfsdmCurrentChannelHandle.Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_SYSTEM;
  DfsdmCurrentChannelHandle.Init.OutputClock.Divider      = 80; /* 80 MHz/80 = 1MHz */
  DfsdmCurrentChannelHandle.Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  DfsdmCurrentChannelHandle.Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE; /* N.U. */
  DfsdmCurrentChannelHandle.Init.Input.Pins               = DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS;
  DfsdmCurrentChannelHandle.Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_RISING;
  DfsdmCurrentChannelHandle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  DfsdmCurrentChannelHandle.Init.Awd.FilterOrder          = DFSDM_CHANNEL_FASTSINC_ORDER; /* N.U. */
  DfsdmCurrentChannelHandle.Init.Awd.Oversampling         = 10; /* N.U. */
  DfsdmCurrentChannelHandle.Init.Offset                   = 0;
  DfsdmCurrentChannelHandle.Init.RightBitShift            = 0;
  if(HAL_OK != HAL_DFSDM_ChannelInit(&DfsdmCurrentChannelHandle))
  {
  }
  
  /* Initialize filter 0 (both channels) */
  __HAL_DFSDM_FILTER_RESET_HANDLE_STATE(&DfsdmFilterHandle);
  DfsdmFilterHandle.Instance                          = DFSDM1_Filter0;
  DfsdmFilterHandle.Init.RegularParam.Trigger         = DFSDM_FILTER_SW_TRIGGER; /* N.U. */
  DfsdmFilterHandle.Init.RegularParam.FastMode        = DISABLE; /* N.U. */
  DfsdmFilterHandle.Init.RegularParam.DmaMode         = DISABLE; /* N.U. */
  DfsdmFilterHandle.Init.InjectedParam.Trigger        = DFSDM_FILTER_EXT_TRIGGER;
  DfsdmFilterHandle.Init.InjectedParam.ScanMode       = ENABLE;
  DfsdmFilterHandle.Init.InjectedParam.DmaMode        = DISABLE;
  DfsdmFilterHandle.Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM1_TRGO;
  DfsdmFilterHandle.Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_RISING_EDGE;
  DfsdmFilterHandle.Init.FilterParam.SincOrder        = DFSDM_FILTER_SINC3_ORDER;
  DfsdmFilterHandle.Init.FilterParam.Oversampling     = 64;
  DfsdmFilterHandle.Init.FilterParam.IntOversampling  = 64;
  if(HAL_OK != HAL_DFSDM_FilterInit(&DfsdmFilterHandle))
  {
  }
  
  /* Configure injected channels for filter 0 (both channels) */
  if(HAL_OK != HAL_DFSDM_FilterConfigInjChannel(&DfsdmFilterHandle, (DFSDM_CHANNEL_2 | DFSDM_CHANNEL_3)))
  {
  }
}

/**
  * @brief  TIMER initialization
  * @param  None
  * @retval None
  */
static void TIMER_Init(void)
{
  TIM_OC_InitTypeDef      sConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim.Init.Prescaler         = 7999;  /* To have timer clock at 10kHz */
  htim.Init.Period            = 10000; /* To have PWM at 1Hz */
  htim.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim.Init.ClockDivision     = 0;
  htim.Init.RepetitionCounter = 0;
  htim.Instance = TIM1;
  if(HAL_OK != HAL_TIM_PWM_Init(&htim))
  {
  }
  
  sConfig.OCMode       = TIM_OCMODE_PWM1;
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
  sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  sConfig.Pulse        = (htim.Init.Period / 2); /* To have duty cycle of 50% */
  if(HAL_OK != HAL_TIM_PWM_ConfigChannel(&htim, &sConfig, TIM_CHANNEL_1))
  {
  }
  
  sMasterConfig.MasterSlaveMode      = TIM_MASTERSLAVEMODE_ENABLE;
  sMasterConfig.MasterOutputTrigger  = TIM_TRGO_UPDATE;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  if(HAL_OK != HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig))
  {
  }
}

/**
  * @brief  TIMER de-initialization
  * @param  None
  * @retval None
  */
static void TIMER_DeInit(void)
{
  htim.Instance = TIM1;
  if(HAL_OK != HAL_TIM_PWM_DeInit(&htim))
  {
  }
}

/**
  * @brief  Initializes the DFSDM channel MSP.
  * @param  hdfsdm_channel : DFSDM channel handle.
  * @retval None
  */
void HAL_DFSDM_ChannelMspInit(DFSDM_Channel_HandleTypeDef *hdfsdm_channel)
{
  /* Init of clock and gpio only needed one time */
  if(hdfsdm_channel == &DfsdmVoltageChannelHandle)
  {
    GPIO_InitTypeDef GPIO_Init;
    
    /* Enable DFSDM clock */
    __HAL_RCC_DFSDM1_CLK_ENABLE();
    
    /* Configure PC2 for DFSDM_CKOUT and PC7 for DFSDM_DATIN3 */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_Init.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init.Pull      = GPIO_PULLDOWN;
    GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Init.Alternate = GPIO_AF6_DFSDM1;
    GPIO_Init.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOC, &GPIO_Init);
    GPIO_Init.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOC, &GPIO_Init);
  }
}

/**
  * @brief  DeInitializes the DFSDM channel MSP.
  * @param  hdfsdm_channel : DFSDM channel handle.
  * @retval None
  */
void HAL_DFSDM_ChannelMspDeInit(DFSDM_Channel_HandleTypeDef *hdfsdm_channel)
{
  /* Init of clock and gpio only needed one time */
  if(hdfsdm_channel == &DfsdmVoltageChannelHandle)
  {
    GPIO_InitTypeDef GPIO_Init;
    
    /* DeConfigure PC2 for DFSDM_CKOUT and PC7 for DFSDM_DATIN3 */
    GPIO_Init.Pin = GPIO_PIN_2;
    HAL_GPIO_DeInit(GPIOC, GPIO_Init.Pin);
    GPIO_Init.Pin = GPIO_PIN_7;
    HAL_GPIO_DeInit(GPIOC, GPIO_Init.Pin);
  }
}

/**
  * @brief  Initializes the DFSDM filter MSP.
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterMspInit(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  /* initialize interrupt */
  HAL_NVIC_SetPriority(DFSDM1_FLT0_IRQn, 0x01, 0);
  HAL_NVIC_EnableIRQ(DFSDM1_FLT0_IRQn);
}

/**
  * @brief  DeInitializes the DFSDM filter MSP.
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterMspDeInit(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  /* Deinitialize interrupt */
  HAL_NVIC_DisableIRQ(DFSDM1_FLT0_IRQn);
}

/**
  * @brief  Initializes the TIM PWM MSP.
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  /* Enable TIM1 clock */
  __HAL_RCC_TIM1_CLK_ENABLE();
}

/**
  * @brief  DeInitializes the TIM PWM MSP.
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef *htim)
{
  /* Disable TIM1 clock */
  __HAL_RCC_TIM1_CLK_DISABLE();
}

/**
  * @brief  Injected conversion complete callback. 
  * @note   In interrupt mode, user has to read conversion value in this function
            using HAL_DFSDM_FilterGetInjectedValue.
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterInjConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  int32_t  convValue;
  uint32_t channel;
  
  /* read conversion value and corresponding channel */
  convValue = HAL_DFSDM_FilterGetInjectedValue(hdfsdm_filter, &channel);
  
  if(channel == 2)
  {
    /* current channel */
    currentValue = (float) convValue/(100*64);
    currentAvailable = 1;
  }
  else if(channel == 3)
  {
    /* voltage channel */
    voltageValue = convValue/64;
    voltageAvailable = 1;
  }
}

/**
  * @}
  */

/**
  * @}
  */

