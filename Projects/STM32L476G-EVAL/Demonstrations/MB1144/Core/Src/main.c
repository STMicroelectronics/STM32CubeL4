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
#include "k_mem.h"
#include "k_bsp.h"
#include "k_log.h"
#include "k_rtc.h"
#include "k_calibration.h"
#include "k_storage.h"
#include "k_temperature.h"

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
/**
* @}
*/

/** @defgroup MAIN_Private_Defines
* @{
*/
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
extern uint32_t iddStandbyMeasured;
extern uint32_t IDDRestart;
extern osMessageQId StorageEvent;
/**
* @}
*/


/** @defgroup MAIN_Private_FunctionPrototypes
* @{
*/
void SystemClock_Config(void);
static void StartThread(void const * argument);
static void GUIThread(void const * argument);
static void TimerCallback(void const *n);

extern K_ModuleItem_Typedef  system_info;
extern K_ModuleItem_Typedef  video_player;
extern K_ModuleItem_Typedef  game_board;
extern K_ModuleItem_Typedef  image_browser;
extern K_ModuleItem_Typedef  file_browser;
extern K_ModuleItem_Typedef  usb_device;
extern K_ModuleItem_Typedef  audio_player;
extern K_ModuleItem_Typedef  audio_recorder;
extern K_ModuleItem_Typedef  idd_measure;

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

  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Check if the system was resumed from StandBy mode */
  if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
  {
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN5);
    IDDRestart = 1;

    /* Clear Standby flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

    /* Check and Clear the Wakeup flag */
    if (__HAL_PWR_GET_FLAG(PWR_FLAG_WUF2) != RESET)
    {
      __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
    }

    /* Initializes BSP IDD to perform measured after standby mode */
    BSP_IDD_Init();

    /* Get IDD measured during standby mode */
    BSP_IDD_GetValue(BSP_IDD_MODE_STANDBY, 266, &iddStandbyMeasured);
  }

  /* Create Start task */
  osThreadDef(Kernel_Thread, StartThread, osPriorityNormal, 0, 2 * configMINIMAL_STACK_SIZE);
  osThreadCreate (osThread(Kernel_Thread), NULL);

BSP_LED_On(LED1);

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
static void StartThread(void const * argument)
{
  osTimerId lcd_timer;

  /* Initialize Joystick, Touch screen and Leds */
  k_BspInit();

  /* Initialize memory pools */
  k_MemInit();

  /* Initialize Log area */
  k_LogInit();

  /* Initialize GUI */
  GUI_Init();
  WM_SetCreateFlags(WM_CF_MEMDEV);

  k_StartUp();

  /* Initialize RTC and Backup domain */
  k_CalendarBkupInit();

  /*Initialize audio Interface */
  k_BspAudioInit();

  /* Initialize IDD Interface */
  k_BspIDDInit();

  /* Initialize Storage Units */
  k_StorageInit();

  /* Initialize Temperature */
  k_TemperatureInit();

  /* Add Modules*/
  k_ModuleInit();

  k_ModuleAdd(&audio_player);
  k_ModuleOpenLink(&audio_player, "wav");
  k_ModuleOpenLink(&audio_player, "WAV");
  k_ModuleAdd(&audio_recorder);
  k_ModuleAdd(&image_browser);
  k_ModuleOpenLink(&image_browser, "jpg");
  k_ModuleOpenLink(&image_browser, "JPG");
  k_ModuleOpenLink(&image_browser, "bmp");
  k_ModuleOpenLink(&image_browser, "BMP");
  k_ModuleAdd(&idd_measure);
  k_ModuleAdd(&system_info);
  k_ModuleAdd(&file_browser);
  k_ModuleAdd(&game_board);
  k_ModuleAdd(&usb_device);

  /* Create GUI task */
  osThreadDef(GUI_Thread, GUIThread, osPriorityHigh, 0, 16 * configMINIMAL_STACK_SIZE);
  osThreadCreate (osThread(GUI_Thread), NULL);

  /* Create Touch screen Timer */
  osTimerDef(TS_Timer, TimerCallback);
  lcd_timer =  osTimerCreate(osTimer(TS_Timer), osTimerPeriodic, (void *)0);

  /* Start the TS Timer */
  osTimerStart(lcd_timer, 100);

  for( ;; )
  {
    /* Toggle LED1 */
    BSP_LED_Toggle(LED1);
    osDelay(250);
  }
}

/**
  * @brief  Start task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void GUIThread(void const * argument)
{
  /* Set General Graphical proprieties */
  k_SetGuiProfile();

  if(k_CalibrationIsDone() == 0)
  {
    k_CalibrationInit();
  }

  /* Show the main menu */
  k_InitMenu();

  /* Gui background Task */
  while(1)
  {
    GUI_Exec();
    osDelay(30);
  }
}

/**
  * @brief  Timer callback (40 ms)
  * @param  n: Timer index
  * @retval None
  */
static void TimerCallback(void const *n)
{
  k_TouchUpdate();
}

/** @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_8)
  {
    if (SD_DETECT_PIN)
    {
      if(BSP_SD_IsDetected())
      {
        osMessagePut ( StorageEvent, MSDDISK_CONNECTION_EVENT, 0);
      }
      else
      {
        osMessagePut ( StorageEvent, MSDDISK_DISCONNECTION_EVENT, 0);
      }
    }
  }
}
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
#if defined(USE_STM32L476G_EVAL_REVA)
  * @ note REVA depency, need AHBCLK div 2 to perform correctly LCD access
  *            AHB Prescaler                  = 2
#elif defined(USE_STM32L476G_EVAL_REVB)
  *            AHB Prescaler                  = 1
#endif
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            MSI Frequency(Hz)              = 48000000
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /* Enable HSE Oscillator and activate PLL with HSE as source   */
  /* (Default MSI Oscillator enabled at system reset remains ON) */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState            = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM            = 1;
  RCC_OscInitStruct.PLL.PLLN            = 20;
  RCC_OscInitStruct.PLL.PLLR            = 2;
  RCC_OscInitStruct.PLL.PLLP            = 7;
  RCC_OscInitStruct.PLL.PLLQ            = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
#if defined(USE_STM32L476G_EVAL_REVA)
  /* REVA depency, need AHBCLK div 2 to perform correctly LCD access */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
#elif defined(USE_STM32L476G_EVAL_REVB)
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
#endif
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select SAI1 PLL output as USB clock source */
  PeriphClkInitStruct.PeriphClockSelection  = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
  PeriphClkInitStruct.PLLSAI1.PLLSAI1M      = 1;
  PeriphClkInitStruct.PLLSAI1.PLLSAI1N      = 24;
  PeriphClkInitStruct.PLLSAI1.PLLSAI1Q      = 4;
  PeriphClkInitStruct.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Configure the MSI oscillator clock source for wakeup from Stop */
  HAL_RCCEx_WakeUpStopCLKConfig(RCC_STOP_WAKEUPCLOCK_MSI);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);

  while(1)
  {
  }
}

/* FreeRTOS hooks */

/**
  * @brief  Application Malloc failure Hook
  * @param  None
  * @retval None
  */
void vApplicationMallocFailedHook(void)
{
  Error_Handler();
}

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

