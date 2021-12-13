/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @brief   This file provides main program functions
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

#include "stm32l4xx_ll_bus.h" 



extern void I2C2_DeInit(void);

/** @addtogroup CORE
  * @{
  */

/** @defgroup MAIN
* @brief main file
* @{
*/ 

/** @defgroup MAIN_Private_TypesDefinitions
* @{
*/ 

typedef  void (*pFunc)(void);

/**
* @}
*/ 

/** @defgroup MAIN_Private_Defines
* @{
*/ 
#if defined(DIM_FEATURE_ENABLED)
/*
    To configure the screen dimming timer to 7s, the WakeUpCounter is set to 0x36B0:
    Wakeup Time Base = 16 /(~32.000KHz) = ~0.5 ms
    Wakeup Time = 0.5 ms  * WakeUpCounter
    Therefore, with wake-up counter = 0x36B0 = 14000  
       Wakeup Time =  ~ 0,5 ms * 14000  ~ 7 sec. 
  */
#define DIMMING_TIMER_DELAY        ((uint32_t)0x036B0)

/* Optional delay to slow down the dimming transition.
   Not used (set to 0x00) in the demonstration software */
#define DIMMING_DELAY        ((uint32_t)0x00)

#endif



/**
* @}
*/ 


/** @defgroup MAIN_Private_Macros
* @{
*/ 
/**
* @}
*/ 


/** @defgroup MAIN_Private_Variables
* @{
*/
#if defined(DIM_FEATURE_ENABLED)
uint32_t dimming_timer_to_start     = 1;
uint32_t dimming_enable             = 0;
uint32_t dimming_complete           = 0;
RTC_HandleTypeDef Dim_RTCHandle;

extern uint32_t video_playing;
extern uint32_t dimming_on; 
/* Timer handler declaration */
extern TIM_HandleTypeDef    LCD_TimHandle;  
/* Timer Output Compare Configuration Structure declaration */
extern TIM_OC_InitTypeDef LCD_sConfig;  
#endif
extern uint32_t iddTestIndex;
uint32_t low_power_mode_forbidden   = 0;

/* Flags used to recover Idd measurements results from Standby or Shutdown mode */
FlagStatus Fetch_IddResults = RESET;
FlagStatus Simulate_SecondPress = RESET;

/* Flag to indicate whether or not software is coming back from a software reset */
uint32_t software_reset_flag = 0;
/* Flag to indicate user-requested TouchScreen calibration at start-up */
uint32_t joystick_press = 0;
int sd_status = 0;
/**
* @}
*/ 


/** @defgroup MAIN_Private_FunctionPrototypes
* @{
*/ 
#if defined(DIM_FEATURE_ENABLED)
static void DimRTC_Config(void);
static void LCD_ScreenDimmingOn_AdHoc(void);
#endif
static void GUIThread(void const * argument);
static void SystemClock_Decrease(void);
static void DeInit_Before_LPSleepMode(void);
static void ReInit_After_LPSleepMode(void);

extern K_ModuleItem_Typedef  STemWin_board;
extern K_ModuleItem_Typedef  EW_board;
extern K_ModuleItem_Typedef  TGFX_board;
extern K_ModuleItem_Typedef  TGFX_FULL_board;

extern K_ModuleItem_Typedef  video_player_board;
extern K_ModuleItem_Typedef  audio_player_board;
extern K_ModuleItem_Typedef  audio_recorder_board;
extern K_ModuleItem_Typedef  INFORMATION_board;
extern K_ModuleItem_Typedef  USB_Storage_board;
extern K_ModuleItem_Typedef  idd_measure_board;
extern K_ModuleItem_Typedef  analog_clock_board;
extern K_ModuleItem_Typedef  return_board;

uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];


/**
* @}
*/ 

/** @defgroup MAIN_Private_Functions
* @{
*/ 

/**
* @brief  Main program
* @param  None
* @retval int
*/
int main(void)
{  
  static __IO uint32_t thirdPartyAdress;

  __HAL_RCC_PWR_CLK_ENABLE();

   /* Enable RTC back-up registers access */
  HAL_PWR_EnableBkUpAccess();
  
  thirdPartyAdress = READ_REG(RTC->BKP30R);
  if (thirdPartyAdress != 0)
  {
    WRITE_REG(RTC->BKP30R, 0);
    /* Reinitialize the Stack pointer*/
    __set_MSP(*(__IO uint32_t*) thirdPartyAdress);
    /* Jump to application address */  
    ((pFunc) (*(__IO uint32_t*) (thirdPartyAdress + 4)))();
  }  

  /* STM32L4xx HAL library initialization:
   - Configure the Flash prefetch, instruction and Data caches
   - Configure the Systick to generate an interrupt each 1 msec
   - Set NVIC Group Priority to 4
   - Global MSP (MCU Support Package) initialization
  */
  HAL_Init();

  /* Configure the system clock to 80 MHz */
  SystemClock_Config();

  __HAL_RCC_PWR_CLK_ENABLE();

  if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
  {
    WakeUpFromStandby = SET;
  }
  else
  {
    WakeUpFromStandby = RESET;
  }  
    
  /* Check whether or not coming back from a software reset.
     Used to avoid displaying ST logo again */
  software_reset_flag = READ_REG(RTC->BKP27R);
  WRITE_REG(RTC->BKP27R, 1);

#if defined(DIM_FEATURE_ENABLED)
  DimRTC_Config();
#endif
  
  /* Enable USB power on Pwrctrl CR2 register */
  HAL_PWREx_EnableVddUSB();
  
   /* Enable RTC back-up registers access */
  HAL_PWR_EnableBkUpAccess();

  /* Check if the system was resumed from shutdown mode,
     resort to RTC back-up register RTC_BKP31R to verify 
     whether or not shutdown entry flag was set by software
     before entering shutdown mode.  */
  if (READ_REG(RTC->BKP31R) == 1)
  {
    WakeUpFromShutdown = SET;
  }
  else
  {
    WakeUpFromShutdown = RESET;
  }   
  
  /* Initialize Joystick, Touch screen and Leds */
  BSP_JOY_Init(JOY_MODE_GPIO);
  k_BspInit();
  
  /* Initializes backup domain */
  k_CalendarBkupInit();  
  
  /* Create GUI task */
  osThreadDef(GUI_Thread, GUIThread, osPriorityNormal, 0, 4096);
  osThreadCreate (osThread(GUI_Thread), NULL); 
  
  /* Add Modules*/
  k_ModuleInit();
  
  k_ModuleAdd(&STemWin_board);
  k_ModuleAdd(&EW_board);
  k_ModuleAdd(&TGFX_board);
  k_ModuleAdd(&TGFX_FULL_board);
  
  
  k_ModuleAdd(&video_player_board);
  k_ModuleAdd(&audio_player_board);
  k_ModuleAdd(&audio_recorder_board);  
  k_ModuleAdd(&INFORMATION_board);
  
  k_ModuleAdd(&USB_Storage_board);
  k_ModuleAdd(&idd_measure_board);
  k_ModuleAdd(&analog_clock_board);
  k_ModuleAdd(&return_board);  
  
  /* Initialize GUI */
  GUI_Init();  
  
  /* Enable memory devices */
  WM_SetCreateFlags(WM_CF_MEMDEV);  
  
  /* Set General Graphical proprieties */
  k_SetGuiProfile();
  
  GUI_X_InitOS();

  /* Initialize MFX interrupt manager */
  k_MfxInit();  
  
  /* Check whether or not joystick SEL button is pressed
     at start-up (if yes, triggers a user-requested
     Touch Screen calibration) */
  if (BSP_JOY_GetState() == JOY_SEL)
  {
    joystick_press = 1;  
  }
  /* De-initialize joystick (not used anymore) */
  BSP_JOY_DeInit();
  
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  for( ;; );
}


/**
  * @brief  SD detect callback
  * @param  None
  * @retval None
  */ 
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* IT coming from MFX */
  if(GPIO_Pin == MFX_INT_PIN) 
  {
    
    if((sd_status == 1) && (WakeUpFromStandby == RESET) && (WakeUpFromShutdown == RESET))
    {
          /* Check if the SD card is plugged in the slot */
    if(k_StorageGetStatus (MSD_DISK_UNIT) == 0)
    {
        BSP_IO_ConfigPin(SD_DETECT_PIN, IO_MODE_IT_RISING_EDGE_PU); 
        osMessagePut ( StorageEvent, MSDDISK_CONNECTION_EVENT, 0); 
        sd_status = 0; 
      }
    else
    {
      BSP_IO_ConfigPin(SD_DETECT_PIN, IO_MODE_IT_FALLING_EDGE_PU);
      osMessagePut ( StorageEvent, MSDDISK_DISCONNECTION_EVENT, 0); 
      sd_status = 0;
    } 
    }
    else
    {    
    
    
#if defined(DIM_FEATURE_ENABLED)
     dimming_timer_to_start = 1;
     dimming_complete = 0;
     dimming_enable = 0;        
     BSP_LCD_ScreenDimmingOff(); 
     HAL_RTCEx_DeactivateWakeUpTimer(&Dim_RTCHandle);
#endif
    osMessagePut (MfxEvent, MFX_INTERRUPT_EVENT, 0);
  }
  }
  /* IT coming from TS */
  if(GPIO_Pin == TS_INT_PIN) 
  {
#if defined(DIM_FEATURE_ENABLED)
     dimming_timer_to_start = 1;
     dimming_complete = 0;
     dimming_enable = 0;       
     BSP_LCD_ScreenDimmingOff();
     HAL_RTCEx_DeactivateWakeUpTimer(&Dim_RTCHandle);
#endif
     osMessagePut (MfxEvent, TS_INTERRUPT_EVENT, 0);
  }
  
}


#if defined(DIM_FEATURE_ENABLED)
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
  dimming_enable = 1;  
  HAL_RTCEx_DeactivateWakeUpTimer(&Dim_RTCHandle);
}  
#endif

/**
  * @brief  Start task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void GUIThread(void const * argument)
{  
  if(TouchScreen_IsCalibrationDone() == 0)
  {
    Touchscreen_Calibration();
  }

  /* Enable TS interrupt */
  if (BSP_TS_ITConfig() != TS_OK) {
    Error_Handler();
  }
  
  if (software_reset_flag == 0)
  {
    /* Demo Startup */
    k_StartUp();   
  }
  
  /* Initialize Storage Units */
  k_StorageInit(); 
  
  /* Show the main menu */
  k_InitMenu();
  
  /* Gui background Task */
  while(1) {
    GUI_Exec(); /* Do the background work ... Update windows etc.) */   
    
#if defined(DIM_FEATURE_ENABLED)
    /* If dimming timer is authorized to start and video player is not running,
       start the timer */
    if ((dimming_timer_to_start == 1) && (video_playing == 0))
    {
      HAL_RTCEx_SetWakeUpTimer_IT(&Dim_RTCHandle, DIMMING_TIMER_DELAY, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
      dimming_timer_to_start = 0;
    }
    if (dimming_enable == 1)
    {
      LCD_ScreenDimmingOn_AdHoc();       
    }
#endif
    
    if (Fetch_IddResults != RESET)
    {
      static GUI_PID_STATE TS_State = {0, 0, 0, 0};

      TS_State.x = 180;
      TS_State.y = 180;     
      TS_State.Pressed = 1;

      Fetch_IddResults = RESET;
             
      /* Emulate a touch on the IDD Measure icon */
      GUI_TOUCH_StoreStateEx(&TS_State);
      osMessagePut (MfxEvent, TS_INTERRUPT_EVENT, 0);      
      
    }    
    else if ((WakeUpFromStandby != RESET) || (WakeUpFromShutdown != RESET))
    {  
      static GUI_PID_STATE TS_State = {0, 0, 0, 0};

      TS_State.x = 0;
      TS_State.y = 0;      
      TS_State.Pressed = 1;

      Simulate_SecondPress = SET;
      /* Emulate a touch on STemWin Menu icon */
      GUI_TOUCH_StoreStateEx(&TS_State);
      osMessagePut (MfxEvent, TS_INTERRUPT_EVENT, 0);
    }
    
    osDelay(20); /* Nothing left to do for the moment ... Idle processing */

    /* Enter Low Power Sleep Mode if nothing else to do */ 
    if ((dimming_complete == 1) && (low_power_mode_forbidden == 0))
    {
      DeInit_Before_LPSleepMode();
      
      /* Enter Low Power Sleep Mode */
      HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI); 
      
      /* ... Low Power Sleep Mode ... */
  
      ReInit_After_LPSleepMode();
    }

  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 8000000
  *            PLL_M                          = 1
  *            PLL_N                          = 20
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable MSI Oscillator and activate PLL with MSI as source   */
  /* (Default MSI Oscillator enabled at system reset remains ON) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_7; /*!< MSI = 8 MHz. Do not modify this value used as SAI Source, MMC  */
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
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
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}


/**
  * @brief  System Clock Configuration in Low Power run mode
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
  
  /* turn off PLL and set MSI to 2Mhz */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;   /* 2 Mhz */
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Adapt voltage scaling to low power run */
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  if(HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Enter Low Power Run mode */
   HAL_PWREx_EnableLowPowerRunMode();
   
   /* Disable Power Control clock once PWR registers are updated */
   __HAL_RCC_PWR_CLK_DISABLE();
  
}

/**
  * @brief  System Power Configuration at Boot or from WakeUp
  * @param  Mode: Purpose of HW initialization 
  *               (HWINIT_STARTUP/HWINIT_IDD/HWINIT_STOP2/HWINIT_STANDBY)
  * @retval None
  */
void SystemHardwareInit(HwInitMode Mode)
{
  /* Configure the GPIOs for MFX ######################################*/
  MFX_IO_Init();

  /* Initialize LED 3 and LED 4  */
  BSP_LED_Init(LED_ORANGE);
  BSP_LED_Init(LED_GREEN);

  /* Enable PWR clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Restore LCD */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2D);  
#if defined(DIM_FEATURE_ENABLED)
  TIMx_CLK_ENABLE();
#endif

  BSP_LCD_MspInit();
  LCD_IO_Init();
  /* Backlight control signal assertion */
  HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_SET );
  
  k_BspInit();

  BSP_TS_InitEx(BSP_LCD_GetXSize(), BSP_LCD_GetYSize(), BSP_LCD_GetOrientation());  

  /* Enable TS interrupt */
  if (BSP_TS_ITConfig() != TS_OK) {
    Error_Handler();
  }

#if defined(DIM_FEATURE_ENABLED)
  HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 0x0, 0);
  HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
#endif 
  k_CalendarBkupInit();

}

/**
  * @brief  System Low Power Configuration
  * @param  Mode: Purpose of HW deinitialization 
   *              (HWINIT_ERROR/HWINIT_STARTUP/HWINIT_IDD/HWINIT_STOP2/HWINIT_STANDBY)
  
  * @retval None
  */
void SystemHardwareDeInit(HwInitMode Mode)
{
  RCC_OscInitTypeDef oscinitstruct = {0};
  GPIO_InitTypeDef GPIO_InitStructure;

  k_CalendarBkupDeInit();
  
  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_DMA2D); 
#if defined(DIM_FEATURE_ENABLED) 
  TIMx_CLK_DISABLE();
#endif
  
  BSP_QSPI_DeInit();
  
  BSP_LED_DeInit(LED_ORANGE);

  BSP_LED_DeInit(LED_GREEN);
  
  /* Enable PWR  peripheral Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  HAL_PWREx_DisablePullUpPullDownConfig();

  HAL_PWR_DisableBkUpAccess();

  HAL_PWREx_DisableVddIO2();

  /* All Clock off instead of in all modes */
  oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48;
  oscinitstruct.PLL.PLLState   = RCC_PLL_NONE;
  
  if(HAL_RCC_OscConfig(&oscinitstruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Deinit I2C but keep MFX out IRQ available */
  I2C2_DeInit();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  
  /* Set all GPIO in analog state to reduce power consumption                 */
  /* Note: Debug using ST-Link is not possible during the execution of this   */
  /*       example because communication between ST-link and the device       */
  /*       under test is done through UART. All GPIO pins are disabled (set   */
  /*       to analog input mode) including  UART I/O pins.           */
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;

  /* keep swdio swclk */
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* keep mfx wakeup pin */
  GPIO_InitStructure.Pin = (GPIO_PIN_All & ~(MFX_INT_PIN));
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure); 
  HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOF, &GPIO_InitStructure); 
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);    
  
  /* Keep LCD power on */
  GPIO_InitStructure.Pin = (GPIO_PIN_All & ~(LCD_PWR_CTRL_PIN));
  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);

  if (iddTestIndex == 0)
  {
    /* in Run mode, keep LCD on during measurement */
    GPIO_InitStructure.Pin = (GPIO_PIN_All & ~(LCD_BL_CTRL_PIN));  
  }
  else
  {
    GPIO_InitStructure.Pin = GPIO_PIN_All;
  }
  HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);

  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOF_CLK_DISABLE();  
  __HAL_RCC_GPIOG_CLK_DISABLE();  
  __HAL_RCC_GPIOH_CLK_DISABLE();
  __HAL_RCC_GPIOI_CLK_DISABLE();  
}




/**
  * @brief  Enable System peripheral clocks
  * @retval None
  */
void SystemPeripheralClockEnable(void)
{
  /* Enable minimum System peripheral clocks */
  /* others will be enabled via BSP layers   */
  __HAL_RCC_FLASH_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  
  __HAL_RCC_I2C1_CLK_ENABLE();
  __HAL_RCC_SPI2_CLK_ENABLE();
  __HAL_RCC_DFSDM1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA2D_CLK_ENABLE();    
  __HAL_RCC_TIM6_CLK_ENABLE();
  __HAL_RCC_FMC_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_SAI1_CLK_ENABLE();
  __HAL_RCC_DFSDM1_CLK_ENABLE();
  __HAL_RCC_DCMI_CLK_ENABLE();  
  __HAL_RCC_QSPI_CLK_ENABLE();
  __HAL_RCC_SDMMC1_CLK_ENABLE();
  __HAL_RCC_RTC_ENABLE();    
}

/**
  * @brief  Disable System peripheral clocks
  * @retval None
  */
void SystemPeripheralClockDisable(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  
  
   
   /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();  
  
  /* Set all GPIO in analog state to reduce power consumption */  
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;  
  
  GPIO_InitStructure.Pin = (GPIO_PIN_All & ~((GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_7)));    
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);  
  GPIO_InitStructure.Pin = (GPIO_PIN_All & ~((GPIO_PIN_0 | GPIO_PIN_1 |GPIO_PIN_11)));    
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  
  GPIO_InitStructure.Pin = (GPIO_PIN_All & ~(MFX_INT_PIN));
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure); 
  
  GPIO_InitStructure.Pin = GPIO_PIN_All; 
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);   
  HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);   
  HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);   
  GPIO_InitStructure.Pin = (GPIO_PIN_All & ~(TS_INT_PIN));    
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);  
  GPIO_InitStructure.Pin = (GPIO_PIN_All & ~(LCD_PWR_CTRL_PIN));  
  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);   
  GPIO_InitStructure.Pin = (GPIO_PIN_All & ~(LCD_BL_CTRL_PIN));    
  HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);   

  /* Disable remaining clocks */
  __HAL_RCC_FLASH_CLK_DISABLE();
  __HAL_RCC_SYSCFG_CLK_DISABLE();
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOF_CLK_DISABLE();
  __HAL_RCC_GPIOG_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();
  __HAL_RCC_GPIOI_CLK_DISABLE();
  __HAL_RCC_I2C1_CLK_DISABLE();
  __HAL_RCC_SPI2_CLK_DISABLE();
  __HAL_RCC_DFSDM1_CLK_DISABLE();
  __HAL_RCC_DMA1_CLK_DISABLE();
  __HAL_RCC_DMA2_CLK_DISABLE();
  __HAL_RCC_DMA2D_CLK_DISABLE();  
  __HAL_RCC_TIM6_CLK_DISABLE();
  __HAL_RCC_FMC_CLK_DISABLE();
  __HAL_RCC_PWR_CLK_DISABLE();
  __HAL_RCC_SAI1_CLK_DISABLE();
  __HAL_RCC_DFSDM1_CLK_DISABLE();
  __HAL_RCC_DCMI_CLK_DISABLE();  
  __HAL_RCC_QSPI_CLK_DISABLE();
  __HAL_RCC_SDMMC1_CLK_DISABLE();
  __HAL_RCC_RTC_DISABLE();  
  
  
  RCC->AHB1SMENR  = 0x0;
  RCC->AHB2SMENR  = 0x0;
  RCC->AHB3SMENR  = 0x0;
  RCC->APB1SMENR1 = 0x08;  /* TIM5 maintained in LP Sleep mode */
  RCC->APB1SMENR2 = 0x0;
  RCC->APB2SMENR  = 0x0;
}


/**
  * @brief  DeInit peripherals and clocks before entering Low Power Sleep Mode
  * @param  None  
  * @retval None
  */
static void DeInit_Before_LPSleepMode(void)
{
  /* Disable all threads in order block rescheduling after wake up */
  vTaskSuspendAll();
       
  k_CalendarBkupDeInit();
    
  /* I2C2 deinit */
  I2C2_DeInit();
  
  BSP_QSPI_DeInit();
  
  /* MSP deinit FMC */
  FMC_BANK1_MspDeInit();
  
  SystemPeripheralClockDisable();
  
  SystemClock_Decrease();
  
  HAL_PWREx_DisablePullUpPullDownConfig();
  HAL_PWR_DisableBkUpAccess();  
  HAL_PWREx_DisableVddIO2();     
  
  HAL_SuspendTick();
  /* Suspend RTOS Systick */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
}


/**
  * @brief  ReInit peripherals and clocks after exiting Low Power Sleep Mode
  * @param  None
  * @retval None
  */
static void ReInit_After_LPSleepMode(void)
{
  /* At wake-up, first disable Low Power Run Mode */
  HAL_PWREx_DisableLowPowerRunMode();    
  SystemClock_Config();

   HAL_ResumeTick();
  /* Resume RTOS Systick */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);   
    
  SystemPeripheralClockEnable();
  
  HAL_PWREx_EnableVddIO2();
  HAL_PWR_EnableBkUpAccess();
  HAL_PWREx_EnablePullUpPullDownConfig();
  
  /* MSP init FMC */
  FMC_BANK1_MspInit();

  /* I2C2 Init */
  I2C2_Init();      
  
   k_BspInit();

  BSP_TS_InitEx(BSP_LCD_GetXSize(), BSP_LCD_GetYSize(), BSP_LCD_GetOrientation());  

  /* Enable TS interrupt */
  if (BSP_TS_ITConfig() != TS_OK) {
    Error_Handler();
  }
  k_CalendarBkupInit();
  /* Enable all threads in order block rescheduling after wake up */
  xTaskResumeAll();
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
    /* Init Error Led Orange */
    BSP_LED_Init(LED_ORANGE);
    /* Turn Led Orange on */
    BSP_LED_On(LED_ORANGE);
    while(1);
}


/**
  * @brief  This function is executed to enter MCU in STOP2 mode
  * @param  None
  * @retval None
  */
void EnterStop2Mode(void)
{
#if (PREFETCH_ENABLE != 0)
  /* Disable Prefetch Buffer */
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
#endif /* PREFETCH_ENABLE */

  /* Ensure that MSI is wake-up system clock */ 
  HAL_RCCEx_WakeUpStopCLKConfig(RCC_STOP_WAKEUPCLOCK_MSI);

  /* Enable PWR clock enable */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  HAL_PWREx_DisableLowPowerRunMode();

  /* Suspend HAL tick interrupt */
  HAL_SuspendTick();

  /* Suspend RTOS Systick */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  /* System peripheral clock disable */
  SystemPeripheralClockDisable();

  /* Enable PWR clock enable */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Enter in Low power Stop 2 */
  HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
}

/**
  * @brief  This function is executed when MCU exits from STOP2 mode
  * @param  None
  * @retval None
  */
void ExitStop2Mode(void)
{
#if (PREFETCH_ENABLE != 0)
  /* Re-enable Prefetch Buffer */
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
#endif /* PREFETCH_ENABLE */

  /* System peripheral clock enable */
  SystemPeripheralClockEnable();
  
  /* Resume HAL tick interrupt */
  HAL_ResumeTick();
  
  /* Resume RTOS Systick */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  /* Disable Power Control clock */
  __HAL_RCC_PWR_CLK_DISABLE();

  /* Restore clock to default frequency when not running on battery */
  SystemClock_Config();
}



/**
  * @brief  System Clock Speed decrease
  *         The system Clock source is shifted from HSI to MSI
  *         while at the same time, MSI range is set to RCC_MSIRANGE_3
  *         to go down to 800 KHz     
  * @param  None
  * @retval None
  */
static void SystemClock_Decrease(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_3;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI; 
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
}

#if defined(DIM_FEATURE_ENABLED)
static void DimRTC_Config(void)
{

  /* Configure RTC */
  Dim_RTCHandle.Instance = RTC;
  /* Set the RTC time base to 1s */
  /* Configure RTC prescaler and RTC data registers as follow:
    - Hour Format = Format 24
    - Asynch Prediv = Value according to source clock
    - Synch Prediv = Value according to source clock
    - OutPut = Output Disable
    - OutPutPolarity = High Polarity
    - OutPutType = Open Drain */
  Dim_RTCHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  Dim_RTCHandle.Init.AsynchPrediv = 0x7F /*RTC_ASYNCH_PREDIV*/;
  Dim_RTCHandle.Init.SynchPrediv = 0xF9 /*RTC_SYNCH_PREDIV*/;
  Dim_RTCHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  Dim_RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  Dim_RTCHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if(HAL_RTC_Init(&Dim_RTCHandle) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 0x0, 0);
  HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}


/**
  * @brief  LCD screen dimming enable
  * @note   Screen brightness is gradually decreased.
  *         API inspired from BSP_LCD_ScreenDimmingOn() modified
  *         to prevent MMI freeze during dimming.    
  * @param  None
  * @retval None
  */
static void LCD_ScreenDimmingOn_AdHoc(void)
{
  /* Counter Prescaler value */
  uint32_t uhPrescalerValue = 0;

  static uint32_t i = 0; 
  
  
  /* Set that dim feature is active */
  if (dimming_on ==0 )
  {
    dimming_on = 1;
    i = PULSE_DECREASE_START;
  

    /* Always redo the full initialization as there is no apriori knowledge
      of IO or timer settings at this point (may have been modified by application) */
    
    /* Compute the prescaler value to have TIM1 counter clock equal to 16000000 Hz */
    uhPrescalerValue = (uint32_t)(SystemCoreClock / 16000000) - 1;  
  
    LCD_TimHandle.Instance = TIMx;

    __HAL_TIM_RESET_HANDLE_STATE(&LCD_TimHandle); /* to force MSP call */
    LCD_TimHandle.Init.Prescaler         = uhPrescalerValue;
    LCD_TimHandle.Init.Period            = PERIOD_VALUE;
    LCD_TimHandle.Init.ClockDivision     = 0;
    LCD_TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    LCD_TimHandle.Init.RepetitionCounter = 0;
    if (HAL_TIM_PWM_Init(&LCD_TimHandle) != HAL_OK)
    {
      /* Initialization Error */
      while(1);
    }  
  
    /* Common configuration for all channels */
    LCD_sConfig.OCMode       = TIM_OCMODE_PWM1;
    LCD_sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
    LCD_sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
    LCD_sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    LCD_sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    
    LCD_sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;

    /* Set the pulse value for channel */
    LCD_sConfig.Pulse = PULSE_DECREASE_START;
    if (HAL_TIM_PWM_ConfigChannel(&LCD_TimHandle, &LCD_sConfig, TIMx_CHANNEL) != HAL_OK)
    {
      /* Configuration Error */
      while(1);
    }
  
  
    /* Start Timer channel */
    if (HAL_TIM_PWM_Start(&LCD_TimHandle, TIMx_CHANNEL) != HAL_OK)
    {
      /* PWM Generation Error */
      while(1);
    }
  
  }
  else
  {
    i--;
  }
    
  /* Set the pulse value for the timer channel */
   
  __HAL_TIM_SET_COMPARE(&LCD_TimHandle, TIMx_CHANNEL, i);
  HAL_Delay(DIMMING_DELAY);

  if (i == PULSE_VALUE)
  {
        dimming_enable = 0;
        dimming_complete = 1;
  }  
}

#endif

#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
  number,ex: printf("Wrong parameters value: file %s on line %d\r\n", 
  file, line) */
  
  /* Infinite loop */
  while (1)
  {}
}

#endif


/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/ 

