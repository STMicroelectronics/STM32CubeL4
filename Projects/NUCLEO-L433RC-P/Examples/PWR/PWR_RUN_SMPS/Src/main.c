/**
  ******************************************************************************
  * @file    PWR/PWR_RUN_SMPS/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L4xx PWR HAL API to enter
  *          and exit the Low Power Run SMPS mode.
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

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup PWR_RUN_SMPS
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LED_TOGGLE_DELAY         100
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
static __IO uint32_t button_pressed = 0;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_80MHz(void);
static void SystemClock_24MHz(void);
static void Error_Handler(void);
static void test_run_range2_80mhz_SMPS(void);


/* Default 0x1FE for 0x200 alignement. Best result with 0x200 ie alignement 0x202*/
#if defined(__CC_ARM)
#pragma arm section code = ".ROM_While1_section"
#pragma no_inline
void whileloop1(void)	
#elif defined(__ICCARM__)
#pragma section="ROM_While1_region"
void whileloop1(void) @ "While1Section";
void whileloop1(void)
#elif defined(__GNUC__)
void __attribute__((section(".ROM_While1_section"), noinline)) whileloop1(void)
#endif
{ 
while(1);
}
#if defined(__CC_ARM)
#pragma arm section code
#endif

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
 
  GPIO_InitTypeDef  GPIO_InitStruct;

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

  /* Configure LED4 */
  BSP_LED_Init(LED4);

  /* Configure the system clock to 80 MHz */
  SystemClock_80MHz();
  
  

  /* User push-button (External line 13) will be used to enable or not the SMPS mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* ------------------------------------------ */

    /* Insert 5 seconds delay */
     HAL_Delay(5000);
  
  /********************************/
  /* 80 MHZ                       */		
  /* PWR_REGULATOR_VOLTAGE_SCALE1 */
  /* SMPS IS OFF                  */
  /********************************/
  
  /* ------------------ GPIO ------------------ */
  BSP_LED_Off(LED4);
  
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Pin = GPIO_PIN_All; 

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
 
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();

  __HAL_RCC_GPIOH_CLK_DISABLE();
    
  if (button_pressed == 1)
  {    
    test_run_range2_80mhz_SMPS();
  } 
  else
  {
    
  }
    
  whileloop1();  
}

/**
  * @brief  Move to Run SMPS mode
  * @note   Manage all the required transitions to move from 80 MHz, SMPS off,
           voltage scale range 1 to 80 MHz, SMPS on, voltage scale range 2.
  * @param  None
  * @retval None
  */
static void test_run_range2_80mhz_SMPS(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;	

  uint32_t SMPS_status_local = 0;
  
  SMPS_status_local = BSP_SMPS_DeInit();
  if (SMPS_status_local != SMPS_OK)
    {
    Error_Handler();
    }
  SMPS_status_local = BSP_SMPS_Init(PWR_REGULATOR_VOLTAGE_SCALE2);
  if (SMPS_status_local != SMPS_OK)
  {
    Error_Handler();
  } 

  /* ------------------------------------------ */
 
  
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
  /* Set the System clock to 24 MHz */
  /* so that SMPS can be enabled */
  SystemClock_24MHz(); 

  /* Insert 1 second delay */
  HAL_Delay(1000);
     
  /********************************/
  /* 24 MHZ                       */		
  /* PWR_REGULATOR_VOLTAGE_SCALE1 */
  /* SMPS IS OFF                  */
  /********************************/

  /* ------------------------------------------ */
  
  /* PWR_REGULATOR_VOLTAGE_SCALE2 only with AD SMPS */		
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);
  if (SMPS_status_local != SMPS_OK)
    {
    Error_Handler();
    }

  /* Insert 1 second delay */
  HAL_Delay(1000);
     
  /********************************/
  /* 24 MHZ                       */		
  /* PWR_REGULATOR_VOLTAGE_SCALE2 */
  /* SMPS IS OFF                  */
  /********************************/
  
  /* ------------------------------------------ */
  
  /* Enable SMPS */

  /* Check of PG but not blocking */
  SMPS_status_local = BSP_SMPS_Enable (0 , 1);  

  /* Check of PG */
  SMPS_status_local = BSP_SMPS_Supply_Enable (4 , 1);
    
  if (SMPS_status_local != SMPS_OK)
  {
    Error_Handler();
  }
  
  /* Insert 2 seconds delay */
  HAL_Delay(2000);
  
  /********************************/
  /* 24 MHZ                       */		
  /* PWR_REGULATOR_VOLTAGE_SCALE2 */
  /* SMPS IS ON                  */
  /********************************/
  
  /* ------------------------------------------ */
  
  /* Set System clock to 80 MHz (MSI) */
  SystemClock_80MHz();
  
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
 
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  GPIO_InitStruct.Pin ^= (PIN_SMPS_ENABLE | PIN_SMPS_SWITCH_ENABLE | PIN_SMPS_POWERGOOD);
  
  HAL_GPIO_Init(PORT_SMPS, &GPIO_InitStruct);
  
  NUCLEO_SMPS_GPIO_CLK_DISABLE(); 

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
static void SystemClock_80MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_3);
  HAL_RCC_DeInit();
  
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
    Error_Handler();
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
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief  System Clock Configuration to 24 MHz
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 24000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  * @param  None
  * @retval None
  */
static void SystemClock_24MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_3);
  HAL_RCC_DeInit();

  /* MSI is enabled after System reset, update MSI to 24Mhz (RCC_MSIRANGE_9) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_9;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI; 
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct,__HAL_FLASH_GET_LATENCY()) != HAL_OK)
  {
    /* Initialization Error */
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
  /* Suspend tick */
  HAL_SuspendTick();
  
  /* Turn LED4 */
  BSP_LED_On(LED4);
  while (1)
  {
  }
}

/**
  * @brief SYSTICK callback
  * @param None
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  HAL_IncTick();

  if (TimingDelay != 0)
  {
    TimingDelay--;
  }
  else
  {
    /* Toggle LED4 */
    
    BSP_LED_Toggle(LED4);
    TimingDelay = LED_TOGGLE_DELAY;
  }
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == USER_BUTTON_PIN)
  {
    button_pressed = 1;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
