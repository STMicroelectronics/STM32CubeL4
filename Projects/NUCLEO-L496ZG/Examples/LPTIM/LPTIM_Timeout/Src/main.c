/**
  ******************************************************************************
  * @file    LPTIM/LPTIM_Timeout/Src/main.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use STM32L4xx LPTIM HAL API to realize a
  *          time-out function to wakeup the system from Stop mode.
  *
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

/** @addtogroup LPTIM_Timeout
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Set the Maximum value of the counter (Auto-Reload) that defines the Period */
#define Period               (uint32_t) 65535

/* Set the Timeout value */
#define Timeout              (uint32_t) (32768 - 1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* LPTIM handle declaration */
LPTIM_HandleTypeDef             LptimHandle;

/* Clocks structure declaration */
RCC_PeriphCLKInitTypeDef        RCC_PeriphCLKInitStruct;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void LSE_ClockEnable(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
 /* This sample code shows how to use STM32L4xx LPTIM HAL API to generate a
    PWM at the lowest power consumption, using an external counter clock, in 
    Low Power mode (STOP mode).
    To proceed, 3 steps are required: */
  
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

  /* Initialize LED1 */
  BSP_LED_Init(LED1);  
  
  /* Enable the LSE source */
  LSE_ClockEnable();

  /* ### - 1 - Re-target the LSE to Clock the LPTIM Counter ################# */
  /* Select the LSE clock as LPTIM peripheral clock */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
  RCC_PeriphCLKInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSE;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

  /* ### - 2 - Initialize LPTIM peripheral ################################## */
  /*
   *  Instance        = LPTIM1.
   *  Clock Source    = APB or LowPowerOSCillator
   *  Counter source  = Internal event.   
   *  Clock prescaler = 1 (No division).
   *  Counter Trigger = Trigger1: PB.06
   *  Active Edge     = Rising edge.
   */

  LptimHandle.Instance = LPTIM1;
  
  LptimHandle.Init.Clock.Source       = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  LptimHandle.Init.Clock.Prescaler    = LPTIM_PRESCALER_DIV1;  
  LptimHandle.Init.Trigger.Source     = LPTIM_TRIGSOURCE_0;
  LptimHandle.Init.Trigger.ActiveEdge = LPTIM_ACTIVEEDGE_RISING;
  LptimHandle.Init.CounterSource      = LPTIM_COUNTERSOURCE_INTERNAL;
  LptimHandle.Init.Input1Source       = LPTIM_INPUT1SOURCE_GPIO;
  LptimHandle.Init.Input2Source       = LPTIM_INPUT2SOURCE_GPIO;
  
  /* Initialize LPTIM peripheral according to the passed parameters */
  if (HAL_LPTIM_Init(&LptimHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* ### - 3 - Start the Timeout function in interrupt mode ################# */
  /*
   *  Period = 65535
   *  Pulse  = 32767
   *  According to this configuration (LPTIMER clocked by LSE & compare = 32767,
   *  the Timeout period = (compare + 1)/LSE_Frequency = 1s
   */
  if (HAL_LPTIM_TimeOut_Start_IT(&LptimHandle, Period, Timeout) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* ### - 4 - Enter in Stop mode ########################################### */
  HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);

  /* Infinite Loop */
  while (1)
  {
  }

}

/**
  * @brief  Compare match callback in non blocking mode 
  * @param  hlptim : LPTIM handle
  * @retval None
  */
void HAL_LPTIM_CompareMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
  /* Timeout was reached, toggle LED1 */
  BSP_LED_Toggle(LED1);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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
  * @brief  Enable External Low Speed Clock (LSE)
  * @param  None
  * @retval None
  */
static void LSE_ClockEnable(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable LSE clock */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
      Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while (1)
  {
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

