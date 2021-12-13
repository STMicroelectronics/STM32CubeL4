/**
  ******************************************************************************
  * @file    PWR/PWR_SHUTDOWN/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L4xx PWR HAL API to enter
  *          and exit the shutdown mode with a wakeup pin or external reset.
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

/** @addtogroup PWR_SHUTDOWN
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Delay set between each LED turn-on/off while LED is toggling */
#define LED_TOGGLE_DELAY         100
/* Delay set to keep LED turned on when out-of-shutdown detected */
#define LED_FREEZE_DELAY         50
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
static __IO uint32_t counter;             /* ad-hoc counter set to keep LED
                                             on for about 4 sec. when out-of-
                                             shutdown exit detected */
static __IO uint32_t out_of_shutdown;     /* flag initialized at 0, set when
                                             exit from shutdown detected */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  out_of_shutdown = 0;
  counter = LED_FREEZE_DELAY;
  
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

  /* Configure the system clock to 120 MHz */
  SystemClock_Config();

  /* Configure LED1, LED2 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

    /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE(); 
  HAL_PWR_EnableBkUpAccess();
  
  /* Check if the system was resumed from shutdown mode,
     resort to RTC back-up register RTC_BKP31R to verify 
     whether or not shutdown entry flag was set by software
     before entering shutdown mode.  */
  if (READ_REG(RTC->BKP31R) == 1)
  {
     WRITE_REG( RTC->BKP31R, 0x0 );  /* reset back-up register */
     out_of_shutdown = 1;            /* out of shutdown detected */
    /* Wait that user release the User push-button */
    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
    while(BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET){}
  }

    /* Insert 5 seconds delay */
    HAL_Delay(5000);
  
    /* Disable all used wakeup sources: WKUP pin */
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);
    
    /* Clear wake up Flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
    
    /* Enable wakeup pin WKUP2 */
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_HIGH);
    
    /* Set RTC back-up register RTC_BKP31R to indicate
       later on that system has entered shutdown mode  */    
    WRITE_REG( RTC->BKP31R, 0x1 );    
    
    /* Enter shutdown mode */
    HAL_PWREx_EnterSHUTDOWNMode();
  
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 120000000
  *            HCLK(Hz)                       = 120000000
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
  __HAL_RCC_PWR_CLK_DISABLE();

  /* Enable MSI Oscillator and activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* To avoid undershoot due to maximum frequency, select PLL as system clock source */
  /* with AHB prescaler divider 2 as first step */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    /* Initialization Error */
     Error_Handler();
  }

  /* AHB prescaler divider at 1 as second step */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
void Error_Handler(void)
{
  while(1)
  {
    /* In case of error, LED1 transmits a sequence of three dots, three dashes, three dots */
    BSP_LED_On(LED1); 
    HAL_Delay(300);
    BSP_LED_Off(LED1);
    HAL_Delay(300); 
    BSP_LED_On(LED1); 
    HAL_Delay(300);
    BSP_LED_Off(LED1);
    HAL_Delay(300);  
    BSP_LED_On(LED1); 
    HAL_Delay(300);
    BSP_LED_Off(LED1);
    HAL_Delay(300);   
    BSP_LED_On(LED1); 
    HAL_Delay(700);
    BSP_LED_Off(LED1);
    HAL_Delay(700); 
    BSP_LED_On(LED1); 
    HAL_Delay(700);
    BSP_LED_Off(LED1);
    HAL_Delay(700);  
    BSP_LED_On(LED1); 
    HAL_Delay(700);
    BSP_LED_Off(LED1);
    HAL_Delay(700); 
    BSP_LED_On(LED1); 
    HAL_Delay(300);
    BSP_LED_Off(LED1);
    HAL_Delay(300); 
    BSP_LED_On(LED1); 
    HAL_Delay(300);
    BSP_LED_Off(LED1);
    HAL_Delay(300);  
    BSP_LED_On(LED1); 
    HAL_Delay(300);
    BSP_LED_Off(LED1);
    HAL_Delay(800); 
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
    /* Keep LED1 on for about 4 sec. when out-of-shutdown detected */
    if ((out_of_shutdown == 1) && (counter > 0))
    {
      BSP_LED_On(LED1);           
      counter--;
    }
    else
    {
    /* Toggle LED1 */
    BSP_LED_Toggle(LED1);      
    }
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
    /* Wait that user release the User push-button */
    while(BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET){}

    /* Disable all used wakeup sources: WKUP pin */
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);
    

    /* Clear wake up Flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
    
    /* Enable wakeup pin WKUP2 */
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_HIGH);
    
    /* Set RTC back-up register RTC_BKP31R to indicate
       later on that system has entered shutdown mode  */
    WRITE_REG( RTC->BKP31R, 0x1 );
    /* Enter shutdown mode */
    HAL_PWREx_EnterSHUTDOWNMode();
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

