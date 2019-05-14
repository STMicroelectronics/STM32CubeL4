/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Src/lpsleep_test.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L4xx PWR HAL API to enter
  *          power modes.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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


void test_lpsleep_2mhz(void)
  {
  printf("\n\r Executing test (LPSLEEP 2MHz - FLASH OFF) \n\r");
  printf(" Please measure current then use Reset button to select another test \n\r");

  /* Set all GPIO in analog state to reduce power consumption */
  GPIO_AnalogState_Config();
   
  /* Enable Flash power down mode during Sleep mode     */
  /* (uncomment this line if power consumption figures  */
  /*  must be measured with Flash still on in Low Power */
  /*  Sleep mode)                                       */
  __HAL_FLASH_SLEEP_POWERDOWN_ENABLE(); 
  
  /* Set the System clock to 2 MHz (MSI) */
  SystemClock_2MHz();
  
  /* Suspend Tick increment to prevent wakeup by Systick interrupt.         */
  /* Otherwise the Systick interrupt will wake up the device within 1ms     */
  /* (HAL time base).                                                       */
  HAL_SuspendTick();

  /* Switch off all clock enable ... */
  RCC->AHB1SMENR = 0x0; 
  RCC->AHB2SMENR = 0x0;
  RCC->AHB3SMENR = 0x0;
  RCC->APB1SMENR1 = 0x0;
  RCC->APB1SMENR2 = 0x0; 
  RCC->APB2SMENR = 0x0;
  
  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();
    
  /* Enter SLEEP Mode, Main regulator is OFF */
  HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
  
  
}


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
