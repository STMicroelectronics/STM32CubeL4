/**
  ******************************************************************************
  * @file    stm32l47xx_os_timebase_lptim.c 
  * @author  MCD Application Team
  * @brief   RTOS and HAL time base based on LPTIM.
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
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "stm32l4xx_timebase_lptim.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Set the Maximum value of the counter (Auto-Reload) that defines the Period */
#define PeriodValue             (uint32_t) (32)

/* Set the Compare value that defines the duty cycle */
#define PulseValue              (uint32_t) (16)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* LPTIM handle declaration */
LPTIM_HandleTypeDef             LptimHandle;

__IO uint32_t ulTickInit = 0;

/* Clocks structure declaration */
RCC_PeriphCLKInitTypeDef        RCC_PeriphCLKInitStruct;

#if( configUSE_TICKLESS_IDLE == 2 )
extern uint32_t ulTickFlag;
#endif

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function configures the LPTIM to generate an interrupt each 1ms. 
  * @param  TickPriority: Tick interrupt priority.
  * @retval HAL status
*/
void InitTick (uint32_t TickPriority)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  if(ulTickInit == 0)
  {
    /* Enable LSE clock */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
    
    /* ### - 1 - Re-target the LSE to Clock the LPTIM Counter ################# */
    /* Select the LSE clock as LPTIM peripheral clock */
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
    RCC_PeriphCLKInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSE;  
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);	
    
    /* ### - 2 - Initialize LPTIM peripheral ################################## */
    /*
    *  Instance        = LPTIM1
    *  Clock Source    = APB or LowPowerOSCillator (in this example LSE is
    *                    already selected from the RCC stage)
    *  Clock prescaler = 1 (No division)
    *  Counter source  = Internal event.   
    *  Counter Trigger = Software trigger
    *  Output Polarity = High
    *  Update mode     = Immediate (Registers are immediately updated after any
    *                    write access) 
    */
    
    LptimHandle.Instance = LPTIM1;
    
    LptimHandle.Init.Clock.Source    = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
    LptimHandle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
    LptimHandle.Init.CounterSource   = LPTIM_COUNTERSOURCE_INTERNAL;  
    LptimHandle.Init.Trigger.Source  = LPTIM_TRIGSOURCE_SOFTWARE; 
    LptimHandle.Init.OutputPolarity  = LPTIM_OUTPUTPOLARITY_HIGH;
    LptimHandle.Init.UpdateMode      = LPTIM_UPDATE_IMMEDIATE;
    LptimHandle.Init.Input1Source    = LPTIM_INPUT1SOURCE_GPIO;
    LptimHandle.Init.Input2Source    = LPTIM_INPUT2SOURCE_GPIO;
    
    /* Initialize LPTIM peripheral according to the passed parameters */
    if (HAL_LPTIM_Init(&LptimHandle) != HAL_OK)
    {
      while(1);
    }	
    
    /* Enable LPTIM NVIC Interrupt */
    HAL_NVIC_SetPriority(LPTIM1_IRQn, TickPriority, 0U);
    HAL_NVIC_EnableIRQ(LPTIM1_IRQn);
    
    /* ### - 3 - Start generating the PWM signal ############################## */
    /*
    *  Period = 31
    *  Pulse  = 15
    *  According to this configuration, the duty cycle will be equal to 50%
    */
    __HAL_LPTIM_ENABLE_IT(&LptimHandle, LPTIM_IT_ARRM);
    __HAL_LPTIM_ENABLE(&LptimHandle);
    __HAL_LPTIM_AUTORELOAD_SET(&LptimHandle, PeriodValue);
    __HAL_LPTIM_COMPARE_SET(&LptimHandle, PulseValue);
    __HAL_LPTIM_START_CONTINUOUS(&LptimHandle);  
    
    ulTickInit = 1;
  }
}

/**
  * @brief This function configures the source of the time base. 
  *        The time source is configured  to have 1ms time base with a dedicated 
  *        Tick interrupt priority. 
  * @param TickPriority: Tick interrupt priority.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority)
{
  InitTick(TickPriority);
	
  return HAL_OK;
}


void HAL_LPTIM_CompareMatchCallback(LPTIM_HandleTypeDef *hlptim)
{  
#if( configUSE_TICKLESS_IDLE == 2 )
  ulTickFlag = pdTRUE;
#endif
}

/**
  * @brief  Autoreload match callback in non blocking mode 
  * @param  hlptim : LPTIM handle
  * @retval None
  */
void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
  if (HAL_LPTIM_ReadAutoReload(hlptim) != PeriodValue)
  {
    __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_ARRM);
    __HAL_LPTIM_ENABLE(hlptim);
    __HAL_LPTIM_AUTORELOAD_SET(hlptim, PeriodValue);
    __HAL_LPTIM_COMPARE_SET(hlptim, (PulseValue));
    __HAL_LPTIM_START_CONTINUOUS(hlptim);  
  } 
  
  HAL_IncTick();
  osSystickHandler();
}

/**
  * @brief  This function handles  WAKE UP TIMER  interrupt request.
  * @param  None
  * @retval None
  */
void LPTIM1_IRQHandler(void)
{
  /* Compare match interrupt */
  if(__HAL_LPTIM_GET_FLAG(&LptimHandle, LPTIM_FLAG_CMPM) != RESET)
  {
      /* Clear Compare match flag */
      __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_CMPM);
      
      /* Compare match Callback */
      HAL_LPTIM_CompareMatchCallback(&LptimHandle);  
  }
  
  if(__HAL_LPTIM_GET_FLAG(&LptimHandle, LPTIM_FLAG_ARRM) != RESET)
  {
      /* Clear ARRM flag */
      __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_ARRM);
      
      /* ARRM Callback */
      HAL_LPTIM_AutoReloadMatchCallback(&LptimHandle);  
  }
  
}

/**
  * @}
  */

/**
  * @}
  */

