/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the STM32L4R9I_EVAL BSP Drivers
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
#include "stlogo.h"

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LTDC_ACTIVE_LAYER     LTDC_DEFAULT_ACTIVE_LAYER   
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t DemoIndex = 0;
__IO uint8_t NbLoop = 1;

#if defined(LCD_DIMMING)
RTC_HandleTypeDef RTCHandle;
__IO uint32_t display_dimmed     = 0;     /* LCD dimming status */
__IO uint32_t maintain_display   = 0;     /* Flag to prevent LCD dimming or turning off */
#if defined(LCD_OFF_AFTER_DIMMING)
__IO uint32_t display_turned_off = 0;     /* LCD turn off status */
#endif /* defined(LCD_OFF_AFTER_DIMMING) */
TIM_HandleTypeDef    TimHandle;
static void Timer_Init(TIM_HandleTypeDef * TimHandle);
uint32_t brightness = 0;
extern DSI_HandleTypeDef    hdsi_eval;
#endif /* defined(LCD_DIMMING) */

/* Components Initialization Status */
FlagStatus ButtonInitialized = RESET;
FlagStatus JoyInitialized = RESET;
FlagStatus IddInitialized = RESET;
FlagStatus LcdInitialized = RESET;
FlagStatus LedInitialized = RESET;
FlagStatus TsInitialized  = RESET;

/* Player Pause/Resume Status */
__IO uint32_t PauseResumeStatus = IDLE_STATUS;   

/* Counter for Sel Joystick pressed */
__IO uint32_t PressCount = 0;

/* Joystick Status */
__IO JOYState_TypeDef JoyState = JOY_NONE;

__IO FlagStatus TamperItOccurred = RESET;

__IO FlagStatus TouchItOccurred = RESET;

/* MFX Interrupt Status */
__IO FlagStatus MfxItOccurred = RESET;

/* Volume of the audio playback */
/* Initial volume level (from 0% (Mute) to 100% (Max)) */
__IO uint8_t Volume = 60;
__IO uint8_t VolumeChange = 0;

extern FlagStatus IddItOccurred;
extern FlagStatus ReturnFromStandbyShutdown;

extern uint32_t AudioPlayback;

/* Private function prototypes -----------------------------------------------*/
static void Display_DemoDescription(void);

BSP_DemoTypedef  BSP_examples[]=
{
#if defined(USE_GVO_390x390)
  /* Following tests are run with USE_GVO_390x390 DSI round LCD */
  {DSI_LCD_demo, "LCD", 0},
  {DSI_Touchscreen_demo, "TOUCHSCREEN", 0},
#else
  /* Following tests are run with USE_ROCKTECH_480x272 RGB LCD */
  {Joystick_demo, "JOYSTICK", 0},
  {Led_demo, "LED", 0},
  {RGB_LCD_demo, "LCD", 0},
  {Touchscreen_demo, "TOUCHSCREEN", 0},  /* RGB LCD */
  {AudioPlay_demo, "AUDIO PLAY", 0},
  {AudioRecord_demo, "AUDIO RECORD", 0},
  {OSPI_NOR_demo, "OSPI NOR", 0},
  {SD_demo, "mSD", 0},
  {EEPROM_demo, "EEPROM", 0}, 
  {NOR_demo, "NOR", 0},
  {SRAM_demo, "SRAM", 0},
  {Log_demo, "LCD LOG", 0}
#endif
};
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
  
  /* Configure the system clock */
  SystemClock_Config(); 
  
  /* System common Hardware components initialization (Leds, button, joystick and LCD) */
  SystemHardwareInit();

  /* Other System Hardware components initialization if necessary */
  BSP_EEPROM_Init();
  
#if defined(USE_GVO_390x390)
  while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
#endif  
  Display_DemoDescription();
#if defined(USE_GVO_390x390)  
  BSP_LCD_Refresh();
#endif  
  
#if defined(LCD_DIMMING)
  Dimming_Timer_Enable(&RTCHandle);
#endif
  
  /* Wait For User inputs */
  while (1)
  {
    if(TamperItOccurred == SET)
    {
      /* Add delay to avoid rebound and reset it status */
      HAL_Delay(500);
      TamperItOccurred = RESET;
      
      BSP_examples[DemoIndex++].DemoFunc();
      
      if(DemoIndex >= COUNT_OF_EXAMPLE(BSP_examples))
      {
        /* Increment number of loops which be used by EEPROM example */
        NbLoop++;
        DemoIndex = 0;
      }
#if defined(USE_GVO_390x390)
      while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
#endif  
      Display_DemoDescription();
#if defined(USE_GVO_390x390)  
      BSP_LCD_Refresh();
#endif  
    }
    
    if(MfxItOccurred == SET)
    {
      Mfx_Event();
    }
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 120000000
  *            HCLK(Hz)                       = 120000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 60
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable voltage range 1 normal mode for frequency not above 80 Mhz (reset value) */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  __HAL_RCC_PWR_CLK_DISABLE();

  /* Enable MSI Oscillator and activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* To avoid undershoot due to maximum frequency, select PLL as system clock source */
  /* with AHB prescaler divider 2 as first step */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* AHB prescaler divider at 1 as second step */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 2000000
  *            HCLK(Hz)                       = 2000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 2000000
  *            PLL_M                          = 1
  *            PLL_N                          = 80
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemLowClock_Config(void)
{
  /* oscillator and clocks configs */
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  uint32_t flatency = 0;

  /* Retrieve clock parameters */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &flatency );
  
  /* switch SYSCLK to MSI in order to modify PLL divider */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, flatency) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Retrieve oscillator parameters */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);
  
  /* turn off PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  System Power Configuration at Boot
  * @param  None
  * @retval None
  */
void SystemHardwareInit(void)
{
  /* Init LED 1 to 4  */
  if(LedInitialized != SET)
  {
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4);
    LedInitialized = SET;
  }
  
  /* Init Wakeup push-button in GPIO Mode */
  if(ButtonInitialized != SET)
  {
    BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_EXTI);
    ButtonInitialized = SET;
  }
  
  /* Init Joystick in interrupt mode */
  if(JoyInitialized != SET)
  {
    BSP_JOY_Init(JOY_MODE_EXTI);
    JoyInitialized = SET;
  }
  

  /* Initialize the LCD */
  if(LcdInitialized != SET)
  {
    /* Initialize the LCD */
    if(BSP_LCD_Init() != LCD_OK)
    {
      Error_Handler();
    }

#if defined(USE_ROCKTECH_480x272)
    /* Initialize the LCD Layers */
    BSP_RGB_LCD_LayerDefaultInit(LTDC_DEFAULT_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
//  BSP_RGB_LCD_LayerRgb565Init(LTDC_DEFAULT_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
#endif
  
    LcdInitialized = SET;
  }
  
  /* Initialize the TouchScreen */
  if(TsInitialized != SET)
  {
    /* Initialize the TouchScreen */
    if(BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize()) != TS_OK)
    {
      Error_Handler();
    }

    /* Configure TS interrupt */
    BSP_TS_ITConfig();
    
    TsInitialized = SET;
  }
  
  /* For external component access over I2C */
  EVAL_I2Cx_CLK_ENABLE();
}

/**
  * @brief  System Low Power Configuration
  * @param  None
  * @retval None
  */
void SystemHardwareDeInit(void)
{
  if(LedInitialized != RESET)
  {
    BSP_LED_DeInit(LED1);
    BSP_LED_DeInit(LED2);
    BSP_LED_DeInit(LED3);
    BSP_LED_DeInit(LED4);
    LedInitialized = RESET;
  }

  if(JoyInitialized != RESET)
  {
    //BSP_JOY_DeInit();
    JoyInitialized = RESET;
  }

  if(LcdInitialized != RESET)
  {
    BSP_LCD_DisplayOff();
    BSP_LCD_DeInit();
    LcdInitialized = RESET;
  }

  /* DeInitialize the TouchScreen */
  if(TsInitialized != RESET)
  {
    BSP_TS_DeInit();
    TsInitialized = RESET;
  }

  /* Disable remaining clocks */
  __HAL_RCC_PWR_CLK_DISABLE();
  __HAL_RCC_FLASH_CLK_DISABLE();
  __HAL_RCC_SYSCFG_CLK_DISABLE();
//  __HAL_RCC_GPIOA_CLK_DISABLE();    /* PA13/PA14 RESERVED (JTAG/SWD) */
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOF_CLK_DISABLE();
  __HAL_RCC_GPIOG_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();
  __HAL_RCC_GPIOI_CLK_DISABLE();

//  EVAL_I2Cx_CLK_DISABLE();

  RCC->AHB1SMENR = 0x0;
  RCC->AHB2SMENR = 0x0;
  RCC->AHB3SMENR = 0x0;
  RCC->APB1SMENR1 = 0x0;
  RCC->APB1SMENR2 = 0x0;
  RCC->APB2SMENR = 0x0;
 }

/**
  * @brief  Display main demo messages
  * @param  None
  * @retval None
  */
static void Display_DemoDescription(void)
{
  char desc[50];
  
  /* Set LCD Foreground Layer  */
  BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);

  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  
  /* Clear the LCD */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
  
#if defined(USE_GVO_390x390)
  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 50, (uint8_t *)"STM32L4R9I BSP", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 75, (uint8_t *)"Drivers examples", CENTER_MODE);
 
  /* Draw Bitmap */
  BSP_LCD_DrawBitmap((BSP_LCD_GetXSize() - 80)/2, 105, (uint8_t *)stlogo);
  
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 80, (uint8_t *)"Copyright (c) STMicroelectronics 2017", CENTER_MODE);
  
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize()/2 + 15, BSP_LCD_GetXSize(), 60);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 15, (uint8_t *)"Press Wakeup push-button", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 30, (uint8_t *)"to start :", CENTER_MODE);
  sprintf(desc,"%s example", BSP_examples[DemoIndex].DemoName);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 45, (uint8_t *)desc, CENTER_MODE);
#else
  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"STM32L4R9I BSP", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"Drivers examples", CENTER_MODE);
 
  /* Draw Bitmap */
  BSP_LCD_DrawBitmap((BSP_LCD_GetXSize() - 80)/2, 65, (uint8_t *)stlogo);
  
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 20, (uint8_t *)"Copyright (c) STMicroelectronics 2017", CENTER_MODE);
  
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize()/2 + 30, BSP_LCD_GetXSize(), 50);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 30, (uint8_t *)"Press Wakeup push-button", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 45, (uint8_t *)"to start :", CENTER_MODE);
  sprintf(desc,"%s example", BSP_examples[DemoIndex].DemoName);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 60, (uint8_t *)desc, CENTER_MODE);
#endif
}

/**
  * @brief  EXTI line detection callbacks.
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Check if interrupt comes from MFX */
  if(GPIO_Pin == MFX_INT_PIN)
  {
    MfxItOccurred = SET;
  }

  /* Check if interrupt comes from Tamper button */
  if(GPIO_Pin == WAKEUP_BUTTON_PIN)
  {
    TamperItOccurred = SET;
  }

#if defined(USE_GVO_390x390)
  /* Check if interrupt comes from TouchScreen */
  if(GPIO_Pin == TS_DSI_INT_PIN)
  {
    TouchItOccurred = SET;
  }
#else
  /* Check if interrupt comes from TouchScreen */
  if(GPIO_Pin == TS_RGB_INT_PIN)
  {
    TouchItOccurred = SET;
  }
#endif
}
        
/**
  * @brief  MFX event management.
  * @retval None
  */
void Mfx_Event(void)
{
  /* Disable MFX interrupt to manage current one */
  HAL_NVIC_DisableIRQ(MFX_INT_EXTI_IRQn);
  
  if(BSP_IO_ReadPin(IO_PIN_ALL) != IO_PIN_RESET)
  {
    /* Check if the interruption comes from the Joystick    */
    /* and determine which Joystick button has been pressed */
    JoyState = BSP_JOY_GetState();
  
    if(JoyState != JOY_NONE)
    {
      if(AudioPlayback == 1)
      {
      if(JoyState == JOY_SEL) 
      {
        /* SEL is used to pause and resume the audio playback */
        if (PressCount == 1)
        {
          /* Resume playing Wave status */
          PauseResumeStatus = RESUME_STATUS;
          PressCount = 0;
        }
        else
        {
          /* Pause playing Wave status */
          PauseResumeStatus = PAUSE_STATUS;
          PressCount = 1;
        }
      }
      else if(JoyState == JOY_UP)
      {
        /* UP is used to increment the volume of the audio playback */
        Volume ++;
        if (Volume > 100)
        {
          Volume = 100;
        }
        VolumeChange = 1;
      }
      else if(JoyState == JOY_DOWN)
      {
        /* DOWN is used to decrement the volume of the audio playback */
        Volume --;
        if ((int8_t)Volume < 50)
        {
          Volume = 50;
        }
        VolumeChange = 1;
      }
    }
    }
    /* Insert a little delay to avoid debounce */ 
    HAL_Delay(200);
    
    /* Clear IO Expander IT */
    BSP_IO_ITClear(BSP_IO_ITGetStatus(JOY_ALL_PINS));
  }

  MfxItOccurred = RESET;

  /* Re-enable MFX interrupt */
  HAL_NVIC_EnableIRQ(MFX_INT_EXTI_IRQn);
}

/**
  * @brief Toggle Leds
  * @param  None
  * @retval None
  */
void Toggle_Leds(void)
{
  static uint8_t ticks = 0;
  
  if(ticks++ > 100)
  {
    BSP_LED_Toggle(LED_BLUE);
    ticks = 0;
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Set Systick interrupt in high priority for HAL_Delay() */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
  
  while(1)
  {
    /* Toggle LED3 */
    BSP_LED_Toggle(LED_RED);
    HAL_Delay(500);
  }
}

#if defined(LCD_DIMMING)
void Dimming_Timer_Enable(RTC_HandleTypeDef * RTCHandle)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef rtcclk;

    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

    rtcclk.PeriphClockSelection = RCC_PERIPHCLK_RTC;

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    rtcclk.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    HAL_RCCEx_PeriphCLKConfig(&rtcclk);

    __HAL_RCC_RTC_ENABLE();

    /* Enable and set Wake-Up Timer to the lowest priority */
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 0x0F, 0x0F);
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);

    RTCHandle->Instance = RTC;

    RTCHandle->Init.HourFormat     = RTC_HOURFORMAT_12;
    RTCHandle->Init.AsynchPrediv   = 0x7F /*RTC_ASYNCH_PREDIV*/;
    RTCHandle->Init.SynchPrediv    = 0xF9 /*RTC_SYNCH_PREDIV*/;
    RTCHandle->Init.OutPut         = RTC_OUTPUT_DISABLE;
    RTCHandle->Init.OutPutRemap    = RTC_OUTPUT_REMAP_NONE;
    RTCHandle->Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RTCHandle->Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;

    if(HAL_RTC_Init(RTCHandle) != HAL_OK)
    {
      Error_Handler();
    }

    if(HAL_RTCEx_SetWakeUpTimer_IT(RTCHandle, DIMMING_COUNTDOWN, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
    {
      Error_Handler();
    }
 }

/**
  * @brief  At each RTC wake-up timer expiration, initiate LCD screen dimming (or turn off) if possible
  * @param  hrtc: RTC handle parameter
  */
void  HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
#if defined(LCD_OFF_AFTER_DIMMING)
  static uint32_t dimming_period_counter = 0;
#endif /* defined(LCD_OFF_AFTER_DIMMING) */


#if defined(LCD_OFF_AFTER_DIMMING)
  if (display_turned_off == 0)
  {
    /* If LCD is not turned off, check whether or not it is dimmed;
       if is not dimmed and if no condition prevents it, dim the screen */
#endif /* defined(LCD_OFF_AFTER_DIMMING) */
  /* If the screen is not dimmed */
  if (display_dimmed == 0)
  {
     if (maintain_display == 0)
     {
      brightness = BRIGHTNESS_MAX;
      Timer_Init(&TimHandle);
      display_dimmed = 1;  /* Screen is dimmed */
    }
  }
#if defined(LCD_OFF_AFTER_DIMMING)
  else
  {
    /* If screen is dimmed, wait for LCD_OFF_COUNTDOWN RTC wake-up timer expirations
       before turning off the LCD */
    dimming_period_counter++;
    if (dimming_period_counter == LCD_OFF_COUNTDOWN)
    {
      dimming_period_counter = 0;
      display_dimmed         = 0;  /* Screen is not dimmed */
      display_turned_off     = 1;  /* Screen is turned off */
      BSP_LCD_DisplayOff();
    }
  }
  }
#endif /* defined(LCD_OFF_AFTER_DIMMING) */

  /* Reset flag preventing screen dimming or turning off */
  maintain_display = 0;

}


static void Timer_Init(TIM_HandleTypeDef * TimHandle)
{
  /* Prescaler declaration */
  uint32_t uwPrescalerValue = 0;

   /* Compute the prescaler value to have TIMx counter clock equal to 10000 Hz */
  uwPrescalerValue = (uint32_t)(SystemCoreClock / 10000) - 1;

  /* Set TIM3 instance */
  TimHandle->Instance = TIM3;

  /* Initialize TIM3 peripheral as follows:
       + Period = 10000 - 1
       + Prescaler = (SystemCoreClock/10000) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle->Init.Period            = 100 - 1;  /* Want 10 ms period */
  TimHandle->Init.Prescaler         = uwPrescalerValue;
  TimHandle->Init.ClockDivision     = 0;
  TimHandle->Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle->Init.RepetitionCounter = 0;

  if (HAL_TIM_Base_Init(TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Start the TIM Base generation in interrupt mode ####################*/
  /* Start Channel1 */
  if (HAL_TIM_Base_Start_IT(TimHandle) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }


}


/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  /*##-1- Enable peripheral clock #################################*/
  /* TIM3 Peripheral clock enable */
  __HAL_RCC_TIM3_CLK_ENABLE();

  /*##-2- Configure the NVIC for TIM3 ########################################*/
  /* Set the TIM3 priority */
  HAL_NVIC_SetPriority(TIM3_IRQn, 0x0F, 0x0F /*3, 0*/);

  /* Enable the TIM3 global Interrupt */
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  HAL_DSI_ShortWrite(&hdsi_eval, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x51, brightness);
  brightness--;
  if (brightness < BRIGHTNESS_MIN)
  {
     HAL_TIM_Base_Stop_IT(htim);
  }
}
#endif /* defined(LCD_DIMMING) */

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
#endif /* USE_FULL_ASSERT */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

