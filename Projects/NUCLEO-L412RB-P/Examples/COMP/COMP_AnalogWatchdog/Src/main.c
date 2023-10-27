/**
  ******************************************************************************
  * @file    COMP/COMP_AnalogWatchdog/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the COMP 
  *          peripheral in window mode to make an analog watchdog.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
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

/** @addtogroup COMP_AnalogWatchdog
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define STATE_OVER_THRESHOLD    0x00000001
#define STATE_WITHIN_THRESHOLD  0x00000002
#define STATE_UNDER_THRESHOLD   0x00000003

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
COMP_HandleTypeDef   Comp1Handle;
COMP_HandleTypeDef   Comp2Handle;
__IO uint32_t State = 0;
/* Variable to indicate that MCU entered in Stop mode */
__IO uint32_t EnterInStopMode = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
void COMP_Config(void);
void InputVoltageLevel_Check(void);
void StopSequence_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32L4xx HAL library initialization:
       - Configure the Flash prefetch and Buffer caches
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
  
  /*******************************************************************************
  *                          Common Configuration Routines                       *
  *******************************************************************************/  
 
  /******* Initialize LEDs available on NUCLEO-L412RB-P Rev C board ******************/
  
  BSP_LED_Init(LED4);
  
  /*## Configure peripherals #################################################*/
  /* Configure the COMP peripherals */
  COMP_Config();
  
  /*## Enable peripherals ####################################################*/
  /* Start COMP1 */
  if(HAL_COMP_Start(&Comp1Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  

  /* Check input voltage level: within the thresholds, above the upper threshold
     or under the lower threshold */
  InputVoltageLevel_Check();
  
  /* Infinite loop */
  while (1)
  {
    if (State == STATE_OVER_THRESHOLD)
    {
      /* Restoration done only in case of MCU was in stop mode */
      if (EnterInStopMode == 1)
      {
        /* Restore config: clock, GPIO... */
        SystemClock_Config();
        
        /* Restore GPIO configuration */
        BSP_LED_Init(LED4);
        
        EnterInStopMode = 0;
      }
      
      while(State == STATE_OVER_THRESHOLD)
      {
        /* add your code here */
        
        /* Toggle LED4 */
        BSP_LED_Toggle(LED4);
        HAL_Delay(50);
      }
    }
    else /* (State == STATE_UNDER_THRESHOLD) */
    {
      /* Turn off LED4 */
      BSP_LED_Off(LED4);
      
      /* Input voltage is under the threshold */
      /* Enter STOP mode with regulator in low power */
      StopSequence_Config();
    }
  }
}

/**
  * @brief  COMP configuration
  * @param  None
  * @retval None
  */
void COMP_Config(void)
{
  /*##-1- Configure the COMPx peripheral ###################################*/
  /* COMP1 Init: the higher threshold is fixed by hardware to VREFINT         */
  /* (~ 1.22V).                                                               */
  Comp1Handle.Instance               = COMP1;
  
  Comp1Handle.Init.InvertingInput    = COMP_INPUT_MINUS_VREFINT;
  Comp1Handle.Init.NonInvertingInput = COMP_INPUT_PLUS_IO2;
  Comp1Handle.Init.OutputPol         = COMP_OUTPUTPOL_NONINVERTED;
  Comp1Handle.Init.Mode              = COMP_POWERMODE_ULTRALOWPOWER;
  Comp1Handle.Init.Hysteresis        = COMP_HYSTERESIS_LOW;
  Comp1Handle.Init.BlankingSrce      = COMP_BLANKINGSRC_NONE;
  Comp1Handle.Init.TriggerMode       = COMP_TRIGGERMODE_IT_RISING_FALLING;
  
  if(HAL_COMP_Init(&Comp1Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  
}

/**
  * @brief  Prepare the system to enter STOP mode.
  * @param  None
  * @retval None
  */

void StopSequence_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* PWR peripheral clock enable */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Set all GPIO in analog state to reduce power consumption,  
     except GPIOB for LED4 and comparator input  */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
  
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();  
  __HAL_RCC_GPIOH_CLK_DISABLE();

  /* Request to enter STOP1 mode */
  HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);
  EnterInStopMode = 1;
}

/**
  * @brief  check input voltage level: within the thresholds, above the upper 
  *         threshold or under the lower threshold
  * @param  None
  * @retval None
  */
void InputVoltageLevel_Check(void)
{
  /*  Check if COMP1 output level is high */
  if ((HAL_COMP_GetOutputLevel(&Comp1Handle)) == COMP_OUTPUT_LEVEL_HIGH) 
  {
    /* A rising edge is detected so the input voltage is higher than VREFINT */
    State = STATE_OVER_THRESHOLD;
  }
  else
  {
    State = STATE_UNDER_THRESHOLD;
  }
}

/**
  * @brief  Comparator interrupt callback.
  * @param  hcomp: COMP handle
  * @retval None
  */
void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp)
{
  /* Check input voltage level: within the thresholds, */
  /* above the upper threshold or under the lower threshold */
  InputVoltageLevel_Check();
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED4 on */
  BSP_LED_On(LED4);
  
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
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

