/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Src/run_range1_test.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L4xx PWR HAL API to enter
  *          power modes.
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
#include <stdio.h>

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup PWR_ModesSelection
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void test_run_range1_80mhz(void)
  {
  printf("\n\r Executing test (RUN Range 1, 80MHz - with FLASH ART ON) \n\r");
  printf(" Please measure current then use Reset button to select another test \n\r");

  /* Set all GPIO in analog state to reduce power consumption */
  GPIO_AnalogState_Config();
  
#ifdef USE_STM32L4XX_NUCLEO
  /* Disable USART2 clock */
  __HAL_RCC_USART2_CLK_DISABLE();
#elif USE_STM32L4XX_NUCLEO_144
  /* Disable LPUART1 clock */
  __HAL_RCC_LPUART1_CLK_DISABLE();
#endif
  
  /* Set System clock to 80 MHz (MSI) */
  SystemClock_80MHz();
  
  /* Suspend Tick increment to prevent wakeup by Systick interrupt.         */
  /* Otherwise the Systick interrupt will wake up the device within 1ms     */
  /* (HAL time base).                                                       */
  HAL_SuspendTick();
  

  while1Aligned64();
  
}



/**
  * @brief  while(1) aligned on 64 bits
  * @param  argument: None.
  * @retval None
  */
#if defined ( __ICCARM__ )
#pragma location="While1Section" 
#pragma optimize=no_inline
void while1Aligned64(void)
#else
__attribute__((section(".While1Section")))
void while1Aligned64(void) __attribute__((noinline));
void while1Aligned64(void)
#endif
{
  /* while (1) Must be aligned 64 bit to fit with the flash read size*/
  while(1);
}

/**
  * @}
  */

/**
  * @}
  */

