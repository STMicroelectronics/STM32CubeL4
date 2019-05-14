/**
  ******************************************************************************
  * @file    PWR/PWR_STANDBY_SMPS/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L4xx PWR HAL API to enter
  *          and enter the Standby SMPS mode.
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

/** @addtogroup PWR_STANDBY_SMPS
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LED_TOGGLE_DELAY           100
/* Enabling compilation switch NO_DELAY_TOWARDS_SMPS ensures fast transitions to
   SMPS mode. User can disable this swith to observe more easily the different 
   transitions leading to SMPS activation. */
/* By default, NO_DELAY_TOWARDS_SMPS is disabled. */
#define NO_DELAY_TOWARDS_SMPS        0


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
static __IO uint32_t button_pressed = 0;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config_80MHz(void);
static void SystemClock_Config_24MHz (void);
static void Error_Handler(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{  
  uint32_t SMPS_status = 0;
  uint32_t resumed_from_standby = 0;
  
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
   
   
  /* Check if the system was resumed from Standby mode */ 
  if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
  {
    /* Clear Standby flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

    /* Set resumed_from_standby flag accordingly */
    resumed_from_standby = 1;
      
    /* Wait that user release the User push-button */
    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
    while(BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_SET){}       
    
    /********************************/
    /* Wake up from standby         */
    /* 4 MHZ                        */		
    /* PWR_REGULATOR_VOLTAGE_SCALE1 */
    /* SMPS IS OFF                  */
    /********************************/    

#ifndef NO_DELAY_TOWARDS_SMPS 
    HAL_Delay(2500);
#endif

  } 
  else
  {
    /* User push-button (External line 13) will be used to wakeup the system from Standby mode */
    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

    /* Configure the system clock to 80 MHz */
    SystemClock_Config_80MHz();
    
    /********************************/
    /* After reset                  */       
    /* 80 MHZ                       */		
    /* PWR_REGULATOR_VOLTAGE_SCALE1 */
    /* SMPS IS OFF                  */
    /********************************/    
 

    /* Insert 5 seconds delay */
    /* Delay at wake-up       */
    HAL_Delay(5000);
    
    /* ------------------------------------------ */

  }
  
  
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

  /* When coming back from Standby, clock is already below 26 MHz */
  /* (MSI default value yields 4 MHz system clock).               */
  /* So, no need to move down to 24 MHz as is done when coming    */
  /* from  80 MHz.                                                */
  if (resumed_from_standby == 0)
  {
    /* ------------------------ */
    SystemClock_Config_24MHz(); 

#ifndef NO_DELAY_TOWARDS_SMPS     
    /* Insert 3 seconds delay */
    HAL_Delay(3000);
#endif
    
    /********************************/
    /* After reset                  */    
    /* 24 MHZ                       */		
    /* PWR_REGULATOR_VOLTAGE_SCALE1 */
    /* SMPS IS OFF                  */
    /********************************/
  }
  
  /* ---------------------------------------------------------- */
  /* Next steps are required whether or not coming from Standby */
  	
  SMPS_status = HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);
  if (SMPS_status != SMPS_OK)  
  {
    Error_Handler();
  } 

#ifndef NO_DELAY_TOWARDS_SMPS 
  /* Insert 2 seconds delay */
  HAL_Delay(2000);
#endif
     
  /********************************/
  /* After reset                  */
  /* 24 MHZ                       */		
  /* PWR_REGULATOR_VOLTAGE_SCALE2 */
  /* SMPS IS OFF                  */
  /********************************/
  /*             OR               */
  /********************************/
  /* Wake up from standby         */
  /* 4 MHZ                        */		
  /* PWR_REGULATOR_VOLTAGE_SCALE2 */
  /* SMPS IS OFF                  */
  /********************************/
  
  /* ------------------------------------------ */
  
  /* Enable SMPS */
  /* Check of PG but not blocking */
  SMPS_status = BSP_SMPS_Enable (0 , 1);  

  /* Check of Power Good */
  SMPS_status = BSP_SMPS_Supply_Enable (10 , 1);   
  if (SMPS_status != SMPS_OK)
  {
    Error_Handler();
  } 
  
  
#ifndef NO_DELAY_TOWARDS_SMPS 
  /* Insert 1 second delay */
  HAL_Delay(1000);
#endif

  /********************************/
  /* After reset                  */
  /* 24 MHZ                       */		
  /* PWR_REGULATOR_VOLTAGE_SCALE2 */
  /* SMPS IS ON                   */
  /********************************/
  /*             OR               */
  /********************************/
  /* Wake up from standby         */
  /* 4 MHZ                        */		
  /* PWR_REGULATOR_VOLTAGE_SCALE2 */
  /* SMPS IS ON                   */
  /********************************/
  
  /* ------------------------------------------ */
  
  SystemClock_Config_80MHz(); 
   
  /********************************/
  /* 80 MHZ                       */		
  /* PWR_REGULATOR_VOLTAGE_SCALE2 */
  /* SMPS IS ON                  */
  /********************************/
  
  /* ------------------------------------------ */

  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  
  while (1)
  {    
    /* Turn OFF LED */
    BSP_LED_Off(LED4);

    /* Wait that user release the User push-button */
    while(BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_SET){}
      
    /* Disable all used wakeup sources: WKUP pin */
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);
    
    /* Clear wake up Flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
      
    /* Enable wakeup pin WKUP2 */
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_HIGH);  
    
    /* Insert 5 seconds delay */
    HAL_Delay(5000);
  
    /* PWR_PUCRx/PDCRx registers have been set in BSP_SMPS_Init(), */
    /* for SMPS as well as non-SMPS pins. It only remains to set   */
    /* APC bit:                                                    */ 
    HAL_PWREx_EnablePullUpPullDownConfig();
    
    /* Enter STANDBY mode */
    HAL_PWR_EnterSTANDBYMode();
     
    /* ... STANDBY mode ... */
  
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
