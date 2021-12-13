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
typedef struct
{
  uint32_t       demo_id;
  char const     demo_descr[50];
} Demo_Header_t;

typedef struct TS_Data_s
{
  uint8_t           InitDone;
  osThreadId        ThreadID;
  osMessageQId      Queue;
  GUI_PID_STATE     TS_State;
} TS_Data_t;
/**
  * @}
  */

/** @defgroup MAIN_Private_Defines
  * @{
  */
#if !defined(USE_ROCKTECH_480x272)
#error This Example is written for ROCKTECH 480x272 LCD so please define USE_ROCKTECH_480x272
#endif /* USE_ROCKTECH_480x272 */

#define GUI_TaskPRIORITY        osPriorityNormal /* osPriorityNormal osPriorityRealtime */
#define GUI_TaskSTACK_SIZE      (configMINIMAL_STACK_SIZE * 8)

#define TS_TaskPRIORITY         osPriorityAboveNormal /* osPriorityNormal osPriorityRealtime */
#define TS_TaskSTACK_SIZE       (configMINIMAL_STACK_SIZE * 2)
#define TS_QUEUE_SIZE           30

/* Demonstration ID : 3 */
#define STEW_CODE_REVISION    0xC3010000 /* 0xAAXXYYZZ - 0xCx : Demo Code ID | 0xXXYYZZ : MajorMinorPatch Version number */
#define STEW_DATA_REVISION    0xD3010000 /* 0xAAXXYYZZ - 0xDx : Demo Data ID | 0xXXYYZZ : MajorMinorPatch Version number */
#define STEW_DEMO_DESCRITION  "STemWin Out of Box Demo V1.0.0"
#define BOARD_NAME            "STM32L4R9I-EVAL_MB1315"

#if defined ( __ICCARM__ )
#pragma location="IntThirdPartyHeaderSection"
__root
#else
__attribute__((section(".IntThirdPartyHeaderSection"))) __attribute__ ((__used__))
#endif
static const Demo_Header_t IntThirdPartyHeader = {
  .demo_id = STEW_CODE_REVISION,
  .demo_descr = STEW_DEMO_DESCRITION
};

#if defined ( __ICCARM__ )
#pragma location="ExtThirdPartyHeaderSection"
__root
#else
__attribute__((section(".ExtThirdPartyHeaderSection"))) __attribute__ ((__used__))
#endif
static const Demo_Header_t ExtThirdPartyHeader = {
  .demo_id = STEW_DATA_REVISION,
  .demo_descr = STEW_DEMO_DESCRITION
};

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
#if configAPPLICATION_ALLOCATED_HEAP == 1
#if defined ( __ICCARM__ )
#pragma location="heap_mem"
#else
__attribute__((section("heap_mem")))
#endif
uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
#endif /* configAPPLICATION_ALLOCATED_HEAP */

static TS_Data_t TS_Data;

/**
  * @}
  */

/** @defgroup MAIN_Private_FunctionPrototypes
  * @{
  */
static void SystemClock_Config(void);
static void GUIThread(void const * argument);

static void TouchScreenTask(void const *argument);
static uint8_t TouchScreenInit(void);
static void TouchScreenUpdate(void);

extern K_ModuleItem_Typedef  audio_player_board;
extern K_ModuleItem_Typedef  audio_recorder_board;
extern K_ModuleItem_Typedef  home_alarm_board;
extern K_ModuleItem_Typedef  games_board;
#if defined (USE_IMAGE_BROWSER_MODULE)
extern K_ModuleItem_Typedef  file_browser;
extern K_ModuleItem_Typedef  image_browser;
#else
extern K_ModuleItem_Typedef  gardening_control_board;
//extern K_ModuleItem_Typedef  video_player_board;
extern K_ModuleItem_Typedef  cpu_bench;
#endif /* USE_IMAGE_BROWSER_MODULE */
extern K_ModuleItem_Typedef  usb_device_board;
extern K_ModuleItem_Typedef  settings_board;


uint32_t GUI_FreeMem = 0;

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
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
  */
  HAL_Init();

  /* Configure the system clock to 120 MHz */
  SystemClock_Config();

  k_BspConfigSerial();

  printf( "\n\n%s: %s (Build %s at %s)\n", BOARD_NAME, IntThirdPartyHeader.demo_descr, __DATE__, __TIME__ );
  printf( "CPU running at %dMHz, Peripherals at %dMHz/%dMHz\n", (HAL_RCC_GetHCLKFreq()/1000000U), (HAL_RCC_GetPCLK1Freq()/1000000U), (HAL_RCC_GetPCLK2Freq()/1000000U) );

  /* Initialize Joystick, Touch screen and LEDs */
  k_BspInit();

  /* Initialize RTC */
  k_CalendarBkupInit();

  /* Create GUI task */
  osThreadDef(GUI_Thread, GUIThread, GUI_TaskPRIORITY, 0, GUI_TaskSTACK_SIZE);
  osThreadCreate (osThread(GUI_Thread), NULL);

  /* Add Modules*/
  k_ModuleInit();

  /* Link modules */
  k_ModuleAdd(&audio_player_board);
  k_ModuleAdd(&audio_recorder_board);
  k_ModuleAdd(&home_alarm_board);
  k_ModuleAdd(&games_board);
#if defined (USE_IMAGE_BROWSER_MODULE)
  k_ModuleAdd(&file_browser);
  k_ModuleAdd(&image_browser);
#else
  k_ModuleAdd(&gardening_control_board);
//  k_ModuleAdd(&video_player_board);
  k_ModuleAdd(&cpu_bench);
#endif /* USE_IMAGE_BROWSER_MODULE */
  k_ModuleAdd(&usb_device_board);
  k_ModuleAdd(&settings_board);

  TouchScreenInit();

//  GUI_X_InitOS();

  /* Initialize/DeInitialize the Storage Units : Just start the Storage thread and get the initial Status */
  // k_StorageInit();
  // k_StorageDeInit();

  /* Initialize MFX interrupt manager */
  k_MfxInit();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  for( ;; );
}

/**
  * @brief  Start task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static uint8_t TouchScreenInit(void)
{
  uint8_t counter = 10;
  uint8_t status = TS_OK;

  if(TS_Data.InitDone)
    return status;

  do
  {
    status = BSP_TS_Init(480, 272);
    counter--;
  }
  while (counter && (status != TS_OK));

  if (status == TS_OK)
  {
    status = BSP_TS_ITConfig();
  }

  if (status == TS_OK)
  {
    memset(&TS_Data, 0 , sizeof(TS_Data_t));

    osMessageQDef(TS_Queue, TS_QUEUE_SIZE, void*);
    osThreadDef(TS_Task, TouchScreenTask, TS_TaskPRIORITY, 1, TS_TaskSTACK_SIZE);

    /* Create Mfx Message Queue */
    TS_Data.Queue = osMessageCreate (osMessageQ(TS_Queue), NULL);

    /* Create TS Thread */
    TS_Data.ThreadID = osThreadCreate(osThread(TS_Task), NULL);

    TS_Data.InitDone = 1;
  }

  return status;
}

/**
  * @brief  Start task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void TouchScreenTask(void const *argument)
{
  static TS_StateTypeDef  TS_State = { 0 };

  memset(&TS_State, 0, sizeof(TS_StateTypeDef));

  while(1)
  {
    osEvent event = osMessageGet(TS_Data.Queue, osWaitForever);

    if(BSP_TS_GetState((TS_StateTypeDef *)&TS_State) == TS_OK)
    {
      if((TS_State.touchX[0] > 480) || (TS_State.touchY[0] > 272))
      {
        TS_State.touchX[0] = 0;
        TS_State.touchY[0] = 0;
      }

      if((TS_Data.TS_State.Pressed != TS_State.touchDetected )||
        (TS_Data.TS_State.x != TS_State.touchX[0]) ||
        (TS_Data.TS_State.y != TS_State.touchY[0]))
      {
        TS_Data.TS_State.Pressed = TS_State.touchDetected;
        if(TS_State.touchDetected)
        {
          TS_Data.TS_State.x = TS_State.touchX[0];
          TS_Data.TS_State.y = TS_State.touchY[0];
          GUI_TOUCH_StoreStateEx(&TS_Data.TS_State);
        }
        else
        {
          GUI_TOUCH_StoreStateEx(&TS_Data.TS_State);
          TS_Data.TS_State.x = 0;
          TS_Data.TS_State.y = 0;
        }
      }
    }
  }
}

/**
  * @brief  Put message
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void TouchScreenUpdate(void)
{
  uint32_t timeout = __get_IPSR() ? 0:5;
  int retry_count = 2; /* retry sending message in case of issue */

  while(retry_count)
  {
      retry_count--;
      if (osMessagePut (TS_Data.Queue, 1 , timeout) == osOK)
      {
        retry_count = 0;
      }
  }

  /* TODO : rise error to upper level instead of breaking execution right here! */
  if(retry_count < 0)
  {
    Error_Handler();
  }
}

/**
  * @brief  Start task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void GUIThread(void const * argument)
{
  /* Initialize GUI */
  GUI_Init();
  
  WM_MULTIBUF_Enable(1);
  
  /* Set General Graphical proprieties */
  k_SetGuiProfile();

  GUI_SetBkColor(GUI_WHITE);
  GUI_Clear();

  /* Show the main menu */
  k_InitMenu();

  /* Gui background Task */
  while(1) {
    GUI_Exec();
    GUI_X_Delay(40);

    k_PeriodicProcesses();
  }
}

/**
  * @brief  EXTI line detection callbacks.
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Check if interrupt comes from MFX */
  switch(GPIO_Pin)
  {
    case WAKEUP_BUTTON_PIN :
    {
      /* Turn LED RED off */
      BSP_LED_Off(LED_RED);
    }
    break;

    case TS_RGB_INT_PIN :
    {
      TouchScreenUpdate();
    }
    break;

    case SD_DETECT_PIN :
    {
      /* Nothing to do */
    }
    break;

    case MFX_INT_PIN:
    {
      k_MfxDetectInt();
    }
    break;

    default:
      break;
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *
  *         If define USB_USE_LSE_MSI_CLOCK enabled:
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 120000000
  *            HCLK(Hz)                       = 120000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 48000000
  *            PLL_M                          = 12
  *            PLL_N                          = 60
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            Flash Latency(WS)              = 4
  *
  *         If define USE_USB_CLKSOURCE_CRSHSI48 enabled:
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
  *            Flash Latency(WS)              = 4
  *
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

#if defined (USE_USB_CLKSOURCE_CRSHSI48)
  static RCC_CRSInitTypeDef RCC_CRSInitStruct;
#endif

  /* Enable voltage range 1 boost mode for frequency above 80 Mhz */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
  __HAL_RCC_PWR_CLK_DISABLE();

#if defined (USE_USB_CLKSOURCE_CRSHSI48)

  /* Enable the HSI48 Oscillator */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State          = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_OFF;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Enable MSI Oscillator and activate PLL with MSI as source   */
  /* (Default MSI Oscillator enabled at system reset remains ON) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Enable MSI Auto-calibration through LSE */
  HAL_RCCEx_EnableMSIPLLMode();

  /* Select HSI48 output as USB clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

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
    Error_Handler();
  }

  /* AHB prescaler divider at 1 as second step */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

    /*Configure the clock recovery system (CRS)**********************************/

  /* Enable CRS Clock */
  __HAL_RCC_CRS_CLK_ENABLE();

  /* Default Synchro Signal division factor (not divided) */
  RCC_CRSInitStruct.Prescaler = RCC_CRS_SYNC_DIV1;

  /* Set the SYNCSRC[1:0] bits according to CRS_Source value */
  RCC_CRSInitStruct.Source = RCC_CRS_SYNC_SOURCE_USB;

  /* HSI48 is synchronized with USB SOF at 1KHz rate */
  RCC_CRSInitStruct.ReloadValue =  __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000, 1000);
  RCC_CRSInitStruct.ErrorLimitValue = RCC_CRS_ERRORLIMIT_DEFAULT;

  /* Set the TRIM[5:0] to the default value*/
  RCC_CRSInitStruct.HSI48CalibrationValue = 0x20;

  /* Start automatic synchronization */
  HAL_RCCEx_CRSConfig (&RCC_CRSInitStruct);

#elif defined (USB_USE_LSE_MSI_CLOCK)

  /* Enable MSI Oscillator and activate PLL with MSI as source   */
  /* (Default MSI Oscillator enabled at system reset remains ON) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11; /*!< MSI = 48 MHz. Do not modify this value used as SAI Source, MMC  */
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Enable MSI Auto-calibration through LSE */
  HAL_RCCEx_EnableMSIPLLMode();

  /* Select MSI output as USB clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_MSI;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

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
    Error_Handler();
  }

  /* AHB prescaler divider at 1 as second step */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

#endif /* USB_USE_HSI48_CLOCK */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED RED on */
  BSP_LED_On(LED_RED);

  configASSERT (0);
}

void BSP_ErrorHandler(void)
{
  // FIXME : We just ignore the BSP errors because we got many I2C timeout errors
  // Error_Handler();
  BSP_LED_On(LED_RED);
}

#ifdef configCHECK_FOR_STACK_OVERFLOW
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
  printf( "%s(): STACK OVERFLOW !!!\n", pcTaskName );

  Error_Handler();
}
#endif /* configCHECK_FOR_STACK_OVERFLOW */

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  Error_Handler();
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

