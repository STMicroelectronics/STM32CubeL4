/**
  ******************************************************************************
  * @file    TIM/TIM_ExtTriggerSynchro/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to synchronize TIM peripherals in cascade
  *          mode with an external trigger.
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

/** @addtogroup TIM_ExtTriggerSynchro
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* TIM2 handler declaration: Slave for an external trigger/Master */
TIM_HandleTypeDef    TimMasterHandle;
/* TIM3 handler declaration: Slave/Master */
TIM_HandleTypeDef    TimSlaveMasterHandle;
/* TIM4 handler declaration: Slave */
TIM_HandleTypeDef    TimSlaveHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef   sOCConfig;
/* Timer Input Capture Configuration Structure declaration */
TIM_IC_InitTypeDef   sICConfig;

/* Timer Master Configuration Structure declaration */
TIM_MasterConfigTypeDef  sMasterConfig;
/* Timer Slave Configuration Structure declaration */
TIM_SlaveConfigTypeDef   sSlaveConfig;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);

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

  /* Configure LED2 */
  BSP_LED_Init(LED2);

  /* Set Timers instance */
  TimMasterHandle.Instance      = TIM2;
  TimSlaveMasterHandle.Instance = TIM3;
  TimSlaveHandle.Instance       = TIM4;
 
  /*======= Master1/Slave for an external trigger configuration : TIM2 =======*/
  /* Initialize TIM2 peripheral in Output Compare mode*/
  TimMasterHandle.Init.Period            = 79;
  TimMasterHandle.Init.Prescaler         = (SystemCoreClock / 16000000) - 1;;
  TimMasterHandle.Init.ClockDivision     = 0;
  TimMasterHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  if(HAL_TIM_OC_Init(&TimMasterHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }  
  
  /* Configure the output: Channel_1 */
  sOCConfig.OCMode     = TIM_OCMODE_TOGGLE;
  sOCConfig.OCPolarity = TIM_OCPOLARITY_HIGH;    
  if(HAL_TIM_OC_ConfigChannel(&TimMasterHandle, &sOCConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /* Configure the Input: channel_2 */
  sICConfig.ICPolarity  = TIM_ICPOLARITY_RISING;
  sICConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sICConfig.ICPrescaler = TIM_ICPSC_DIV1;
  sICConfig.ICFilter = 0;  
  if(HAL_TIM_IC_ConfigChannel(&TimMasterHandle, &sICConfig, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /* Configure TIM2 in Gated Slave mode for the external trigger (Filtered Timer
     Input 2) */
  sSlaveConfig.InputTrigger = TIM_TS_TI2FP2;
  sSlaveConfig.SlaveMode    = TIM_SLAVEMODE_GATED;
  if( HAL_TIM_SlaveConfigSynchronization(&TimMasterHandle, &sSlaveConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /* Configure TIM2 in Master Enable mode & use the update event as Trigger
     Output (TRGO) */
  sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_ENABLE;
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_ENABLE;
  if( HAL_TIMEx_MasterConfigSynchronization(&TimMasterHandle, &sMasterConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }  
  /*=== End of Master1/Slave for an external trigger configuration : TIM2 ====*/

  
  /*=================== Slave/Master configuration : TIM3 ====================*/
  /* Initialize TIM3 peripheral in Output Compare mode*/
  TimSlaveMasterHandle.Init.Period            = 79;
  TimSlaveMasterHandle.Init.Prescaler         = (SystemCoreClock / 16000000) - 1;;
  TimSlaveMasterHandle.Init.ClockDivision     = 0;
  TimSlaveMasterHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  if(HAL_TIM_OC_Init(&TimSlaveMasterHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Configure the Output Compare channel_1  */
  sOCConfig.OCMode     = TIM_OCMODE_TOGGLE;
  sOCConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  if(HAL_TIM_OC_ConfigChannel(&TimSlaveMasterHandle, &sOCConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /* Configure TIM3 in Gated Slave mode for the internal trigger 1(ITR1) */  
  sSlaveConfig.InputTrigger = TIM_TS_ITR1;
  sSlaveConfig.SlaveMode    = TIM_SLAVEMODE_GATED;
  if( HAL_TIM_SlaveConfigSynchronization(&TimSlaveMasterHandle, &sSlaveConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /* Configure TIM3 in Master Enable mode & use the update event as Trigger
     Output (TRGO) */
  sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_ENABLE;
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_ENABLE;
  if( HAL_TIMEx_MasterConfigSynchronization(&TimSlaveMasterHandle, &sMasterConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  /*=============== End of Slave/Master configuration : TIM3 =================*/
  
  
  /*====================== Slave configuration : TIM4 ========================*/
  /* Initialize TIM4 peripheral in Output Compare mode*/
  TimSlaveHandle.Init.Period            = 79;
  TimSlaveHandle.Init.Prescaler         = (SystemCoreClock / 16000000) - 1;;
  TimSlaveHandle.Init.ClockDivision     = 0;
  TimSlaveHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  if(HAL_TIM_OC_Init(&TimSlaveHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  } 
 
  /* Configure the Output Compare channel_1  */
  sOCConfig.OCMode     = TIM_OCMODE_TOGGLE;
  sOCConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  if(HAL_TIM_OC_ConfigChannel(&TimSlaveHandle, &sOCConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }  
  
  /* Configure TIM4 in Gated Slave mode for the internal trigger 2(ITR2) */ 
  sSlaveConfig.SlaveMode    = TIM_SLAVEMODE_GATED;
  sSlaveConfig.InputTrigger = TIM_TS_ITR2;
  if(HAL_TIM_SlaveConfigSynchronization(&TimSlaveHandle, &sSlaveConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }  
  /*================== End of Slave configuration : TIM4 =====================*/
  
  
  /* 1- Start Timer1 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
  /* Start Channel2 in Input Capture */
  if(HAL_TIM_IC_Start(&TimMasterHandle, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }  
  /* Start the Output Compare */
  if(HAL_TIM_OC_Start(&TimMasterHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

  /* 2- Start Timer3 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
  /* Start the Output Compare */
  if(HAL_TIM_OC_Start(&TimSlaveMasterHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
  
  /* 3- Start Timer3 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
  /* Start the Output Compare */
  if(HAL_TIM_OC_Start(&TimSlaveHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
  
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  while (1)
  {
  }
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

