/**
  ******************************************************************************
  * @file    STemWin/STemWin_HelloWorld/Src/main.c
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
#include "WM.h"

#if configAPPLICATION_ALLOCATED_HEAP == 1
#if defined ( __ICCARM__ )
#pragma location="heap_mem"
#else
__attribute__((section("heap_mem")))
#endif
uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
#endif /* configAPPLICATION_ALLOCATED_HEAP */

extern void k_StartUp(void);
extern void LauncherStartUp(void);

/* Private typedef -----------------------------------------------------------*/
typedef  void (*pFunc)(void);

/* Private define ------------------------------------------------------------*/
#define TS_TaskPRIORITY                 osPriorityAboveNormal /* osPriorityNormal osPriorityRealtime */
#define TS_TaskSTACK_SIZE               (configMINIMAL_STACK_SIZE * 2)

#define GUI_TaskPRIORITY                osPriorityNormal /* osPriorityNormal osPriorityRealtime */
#define GUI_TaskSTACK_SIZE              (configMINIMAL_STACK_SIZE * 8)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t BSP_Initialized = 0;
static __IO uint32_t SoftwareReset = 1;
static uint32_t TouchScreenTimoutValue = 80;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void GUIThread(void const * argument);
static void TouchScreenTask(void const *argument);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  static __IO uint32_t thirdPartyAdress;

  __HAL_RCC_PWR_CLK_ENABLE();

   /* Enable RTC back-up registers access */
  HAL_PWR_EnableBkUpAccess();

  SoftwareReset = READ_REG(RTC->BKP27R);
  if(SoftwareReset == 0)
  {
    WRITE_REG(RTC->BKP27R, 1);
  }

  thirdPartyAdress = READ_REG(RTC->BKP26R);
  if (thirdPartyAdress != 0)
  {
     WRITE_REG(RTC->BKP26R, 0);
    /* Reinitialize the Stack pointer*/
    __set_MSP(*(__IO uint32_t*) thirdPartyAdress);
    /* Jump to application address */
    ((pFunc) (*(__IO uint32_t*) (thirdPartyAdress + 4)))();
  }

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();

  BSP_Initialized = BSP_Config();

  if(BSP_Initialized)
  {
    /* Init the MFX */
    MfxInit();

    /* Init the SD Card hardware and its IRQ handler manager */
    StorageInit();

    /***********************************************************/
    /* Create TS Thread */
    osThreadDef(TS_Task, TouchScreenTask, TS_TaskPRIORITY, 1, TS_TaskSTACK_SIZE);
    osThreadCreate(osThread(TS_Task), NULL);

    /* Create GUI task */
    osThreadDef(GUI_Thread, GUIThread, GUI_TaskPRIORITY, 0, GUI_TaskSTACK_SIZE);
    osThreadCreate (osThread(GUI_Thread), NULL);

    /* Start scheduler */
    osKernelStart();
  }

  /* We should never get here as control is now taken by the scheduler */
  for( ;; );
}

void TouchScreenEnable(void)
{
  TouchScreenTimoutValue = 80;
}

void TouchScreenDisable(void)
{
  TouchScreenTimoutValue = osWaitForever;
}

/**
  * @brief  Start task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void TouchScreenTask(void const *argument)
{
  /* Create the TS semaphore */
  osSemaphoreDef(TSSemaphore);
  osSemaphoreId TSSemaphoreID = osSemaphoreCreate(osSemaphore(TSSemaphore), 1);
  /* initially take the TS Lock */
  osSemaphoreWait( TSSemaphoreID, osWaitForever );
  
  TouchScreenEnable();

  while(1)
  {
    osSemaphoreWait( TSSemaphoreID, TouchScreenTimoutValue );
    
    /* Capture input event and updade cursor */
    if(BSP_Initialized == 1)
    {
      BSP_TouchUpdate();
    }
  }
}

/**
  * @brief  Start task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void GUIThread(void const * argument)
{
  GUI_Init();

  /* Activate the use of memory device feature */
  WM_MULTIBUF_Enable(1);

  GUI_DispStringAt("Starting...", 0, 0);

  /* Initialize LCD and LEDs */
  GUI_DispStringAt("Initializing lcd...", 0, 12);

  /* Demo Startup */
  if(SoftwareReset == 0)
  {
    k_StartUp();
  }

  LauncherStartUp();

  /* Gui background Task */
  while(1) {
    GUI_Delay(20);
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
      /* Nothing to do */
    }
    break;

    case MFX_INT_PIN:
    {
      MfxDetectInt(); //For SD card detection
    }
    break;

    default:
      break;
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 110000000
  *            HCLK(Hz)                       = 110000000
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

  /* Enable voltage range 1 boost mode for frequency above 80 Mhz */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
  // HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  __HAL_RCC_PWR_CLK_DISABLE();

  /* Enable MSI Oscillator and activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6; /*!< MSI = 8 MHz. Do not modify this value used as SAI Source, MMC  */
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* To avoid overshoot due to maximum frequency, select PLL as system clock source */
  /* with prescaler dividers 2 as first step */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Pprescaler dividers at 1 as second step */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
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

#ifdef configUSE_MALLOC_FAILED_HOOK
/**
  * @brief  Application Malloc failure Hook
  * @param  None
  * @retval None
  */
void vApplicationMallocFailedHook(TaskHandle_t xTask, char *pcTaskName)
{
  printf( "%s(): MALLOC FAILED !!!\n", pcTaskName );

  Error_Handler();
}
#endif /* configUSE_MALLOC_FAILED_HOOK */

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

  /* Infinite loop */
  while (1)
  {}
}
#endif

