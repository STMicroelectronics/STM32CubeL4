/**
  ******************************************************************************
  * @file    DFSDM/DFSDM_Thermometer/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use DFSDM HAL API to realize
  *          temperature measurements.
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

/** @addtogroup DFSDM_Thermometer
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DFSDM_Channel_HandleTypeDef  DfsdmVoltageChannelHandle;
DFSDM_Channel_HandleTypeDef  DfsdmCurrentChannelHandle;
DFSDM_Filter_HandleTypeDef   DfsdmFilterHandle;
TIM_HandleTypeDef            htim;
float                        voltageValue = 0;
float                        currentValue = 0;
__IO uint32_t                     voltageAvailable = 0;
__IO uint32_t                     currentAvailable = 0;
float                        temperature = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void DFSDM_Init(void);
static void TIMER_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
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

  /* Configure the system clock to have a frequency of 80 MHz */
  SystemClock_Config();

  /* Configure LED1 */
  BSP_LED_Init(LED1);

  /* Initialize LCD */
  if(LCD_OK != BSP_LCD_Init())
  {
    Error_Handler();
  }
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);  
  BSP_LCD_DisplayStringAtLine(0, (uint8_t *)"TEMPERATURE :");
  BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
  
  /* Initialize DFSDM channels and filter */
  DFSDM_Init();
  
  /* Initialize timer */
  TIMER_Init();

  /* Start DFSDM conversions */
  if(HAL_OK != HAL_DFSDM_FilterInjectedStart_IT(&DfsdmFilterHandle))
  {
    Error_Handler();
  }
  
  /* Start timer */
  if(HAL_OK != HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_1))
  {
    Error_Handler();
  }   

  /* Start loopback */
  while(1)
  {
    /* Waiting current and voltage values available */
    if((voltageAvailable == 1) && (currentAvailable == 1))
    {
      uint8_t LCDstr[10] = {0};
      
      /* Compute and display temperature value */
      temperature = ((voltageValue/currentValue) - 100)/0.385f;
      sprintf((char*)LCDstr, "%3.1f oC", temperature);
      BSP_LCD_DisplayStringAtLine(1, (uint8_t *) LCDstr);      
      
      voltageAvailable = 0;
      currentAvailable = 0;
    }
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
    Error_Handler();
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
    Error_Handler();
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
    Error_Handler();
  }
  
  /* Configure injected channels for filter 0 (both channels) */
  if(HAL_OK != HAL_DFSDM_FilterConfigInjChannel(&DfsdmFilterHandle, (DFSDM_CHANNEL_2 | DFSDM_CHANNEL_3)))
  {
    Error_Handler();
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
    Error_Handler();
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
    Error_Handler();
  }
  
  sMasterConfig.MasterSlaveMode      = TIM_MASTERSLAVEMODE_ENABLE;
  sMasterConfig.MasterOutputTrigger  = TIM_TRGO_UPDATE;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  if(HAL_OK != HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig))
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  while (1)
  {
    /* Toggle LED1 with a period of one second */
    BSP_LED_Toggle(LED1);
    HAL_Delay(1000);
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

