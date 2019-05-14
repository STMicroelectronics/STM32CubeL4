/**
  ******************************************************************************
  * @file    PWR/PWR_STOP0_SMPS/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L4xx PWR HAL API and external
  *          SMPS BSP to enter and exit the STOP 0 while in SMPS mode.
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

/** @addtogroup PWR_STOP0_SMPS
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LED_TOGGLE_DELAY         100
/* Enabling compilation switch NO_DELAY_TOWARDS_SMPS ensures fast transitions to
   SMPS mode. User can disable this swith to observe more easily the different 
   transitions leading to SMPS activation. */
/* By default, NO_DELAY_TOWARDS_SMPS is enabled. */
#define NO_DELAY_TOWARDS_SMPS        1
/* Enabling compilation switch DISABLE_SMPS de-activates SMPS feature when 
   MCU is low power mode. */
/* By default, DISABLE_SMPS is disabled. */
#define DISABLE_SMPS                 0
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
static __IO uint32_t button_pressed = 0;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config_80MHz(void);
static void SystemClock_Config_24MHz (void);
static void Enter_SMPS_Mode(void);
static void IO_setting(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
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
  
  /* Configure LED4*/
  BSP_LED_Init(LED4);

  /* Configure the system clock to 80 MHz */
  SystemClock_Config_80MHz();
  
  /* User push-button (External line 13) will be used to wakeup the system from STOP mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  
  Enter_SMPS_Mode();
  
  
  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Ensure that MSI is wake-up system clock */ 
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);
 
  while (1)
  {
    /* Insert 5 seconds delay */
    HAL_Delay(5000);

    while (button_pressed != 1)
    {
    }
    button_pressed = 0;
      
    /* ------------------ GPIO ------------------ */
    
    /* Turn OFF LED and suspend Tick */
    BSP_LED_Off(LED4);
    HAL_SuspendTick();
     
#if DISABLE_SMPS
     
    /* Need to move back to lower frequency range to disable SMPS */
    SystemClock_Config_24MHz();
    /* wait 100 ms */
    HAL_Delay(100);  
    
    BSP_SMPS_Supply_Disable();
    /* wait 100 ms */
    HAL_Delay(100);
  
    if (SMPS_KO == BSP_SMPS_Disable())
    {
      Error_Handler();
    }
    /* wait 100 ms */
    HAL_Delay(100);
  
    /* De-init SMPS, it sets the IO to reduce power consumption */
    BSP_SMPS_DeInit();
  
    /* wait 100 ms */
    HAL_Delay(100);     

#endif  

  
    IO_setting();


    /* Enter STOP 0 mode */
    HAL_PWREx_EnterSTOP0Mode(PWR_STOPENTRY_WFI);
   
    /* ... STOP 0 mode ... */    

 
    /* Enable Power Clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* Configure the system clock to 80 MHz */
    SystemClock_Config_80MHz();
 
    /* If SMPS has not been disabled before entering STOP mode (DISABLE_SMPS compilation
       switch is not activated), SMPS is still ON at this point.
      Otherwise, it has to be enabled again */
    
#if DISABLE_SMPS   
    /* if SMPS has been disabled before entering STOP mode, it is enabled again,
     starting from a 80 MHz operating point */ 
    Enter_SMPS_Mode();  
#endif   
     /* Reset user button variable */
    button_pressed = 0;
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
static void SystemClock_Config_24MHz(void)
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
  * @brief  System Clock Configuration to 80 MHz
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
static void SystemClock_Config_80MHz(void)
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
}

/**
  * @brief  Enter in SMPS mode
  * @note   Starting point is assumed to be 80 MHz clock, SMPS off, voltage range scale 1.
  * @param  None
  * @retval None
  */
static void Enter_SMPS_Mode(void)
{
  uint32_t SMPS_status;

  SMPS_status = BSP_SMPS_DeInit();
  if (SMPS_status != SMPS_OK)
  {
    Error_Handler();
  } 
  SMPS_status = BSP_SMPS_Init(PWR_REGULATOR_VOLTAGE_SCALE2);
  if (SMPS_status != SMPS_OK)
  {
    Error_Handler();
  } 
	
#ifndef NO_DELAY_TOWARDS_SMPS   
  /* Insert 3 seconds delay */
  HAL_Delay(3000);
#endif  
  
  /********************************/
  /* 80 MHZ                       */		
  /* PWR_REGULATOR_VOLTAGE_SCALE1 */
  /* SMPS IS OFF                  */
  /********************************/

  /* ------------------------------------------ */
 
  SystemClock_Config_24MHz(); 
  
#ifndef NO_DELAY_TOWARDS_SMPS   
  /* Insert 3 seconds delay */
  HAL_Delay(3000);
#endif  
  
  /********************************/
  /* 24 MHZ                       */		
  /* PWR_REGULATOR_VOLTAGE_SCALE1 */
  /* SMPS IS OFF                  */
  /********************************/

	
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);
  if (SMPS_status != SMPS_OK)
  {
    Error_Handler();
  } 
    
#ifndef NO_DELAY_TOWARDS_SMPS     
  /* Insert 2 seconds delay */
  HAL_Delay(2000);
#endif  
  
  /********************************/
  /* 24 MHZ                       */		
  /* PWR_REGULATOR_VOLTAGE_SCALE2 */
  /* SMPS IS OFF                  */
  /********************************/
  
  /* ------------------------------------------ */
  
  /* Enable SMPS */ 
  SMPS_status = BSP_SMPS_Enable (10 , 1);  
  if (SMPS_status != SMPS_OK)
  {
    Error_Handler();
  } 

  /* Check of PG */
  SMPS_status = BSP_SMPS_Supply_Enable (4 , 1);   
  if (SMPS_status != SMPS_OK)
  {
    Error_Handler();
  } 
    
#ifndef NO_DELAY_TOWARDS_SMPS     
  /* Insert 1 second delay */
  HAL_Delay(1000);
#endif    
  
  /********************************/
  /* 24 MHZ                       */		
  /* PWR_REGULATOR_VOLTAGE_SCALE2 */
  /* SMPS IS ON                  */
  /********************************/
  
  /* ------------------------------------------ */
  
  SystemClock_Config_80MHz(); 
  
#ifndef NO_DELAY_TOWARDS_SMPS     
  /* Insert 5 second delay */
  HAL_Delay(2000);
#endif    
  
  /********************************/
  /* 80 MHZ                       */		
  /* PWR_REGULATOR_VOLTAGE_SCALE2 */
  /* SMPS IS ON                  */
  /********************************/
    
}


/**
  * @brief  IO setting to optimize power consumption in low power mode
  * @param  None
  * @retval None
  */
static void IO_setting(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();  
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();      
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /* Set all GPIO in analog state to reduce power consumption, except IOs that */
  /* require a specific config:                                                */
  /* SMPS pins, LPUART1_TX, SWD/TMS, SWO and User button pins                  */
  /*                                                                           */
  /* Note: Debug using ST-Link is not possible during the execution of this    */
  /*       example because communication between ST-link and the device        */
  /*       under test is done through UART. All GPIO pins are disabled (set    */
  /*       to analog input mode) including  UART I/O pins.                     */
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;

   GPIO_InitStructure.Pin = GPIO_PIN_All;
  GPIO_InitStructure.Pin ^= (GPIO_PIN_2 | GPIO_PIN_13 | PIN_SMPS_ENABLE | PIN_SMPS_SWITCH_ENABLE | PIN_SMPS_POWERGOOD); 
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.Pin = GPIO_PIN_All;  
  GPIO_InitStructure.Pin ^= (GPIO_PIN_3);   
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.Pin = GPIO_PIN_All;  
  GPIO_InitStructure.Pin ^= (GPIO_PIN_13); 
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);  
  GPIO_InitStructure.Pin = GPIO_PIN_All;    
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure); 
  HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 |GPIO_PIN_3;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
  
  /* Disable GPIOs clock */
  __HAL_RCC_GPIOC_CLK_DISABLE();  
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();
  
  
  /* SMPS pins and PC13 (user buttons) are already configured */
          
   GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
   GPIO_InitStructure.Pull =  GPIO_PULLDOWN;
   
   GPIO_InitStructure.Pin = GPIO_PIN_2 | GPIO_PIN_13;  
   HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
   GPIO_InitStructure.Pin = GPIO_PIN_3;  
   HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
		
  __HAL_RCC_GPIOA_CLK_DISABLE();		
  __HAL_RCC_GPIOB_CLK_DISABLE();		
                         
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
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
    /* Reconfigure LED4 */
    BSP_LED_Init(LED4); 
    /* Switch on LED4 */
    BSP_LED_On(LED4);
    
    /**/
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
