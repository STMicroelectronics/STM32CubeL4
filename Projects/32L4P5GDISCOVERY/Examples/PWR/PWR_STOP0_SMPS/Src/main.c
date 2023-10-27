/**
  ******************************************************************************
  * @file    PWR/PWR_STOP0_SMPS/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L4xx PWR HAL API to enter
  *          and exit the STOP 0 SMPS mode.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
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

/** @addtogroup PWR_STOP0_SMPS
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LED_TOGGLE_DELAY         100
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config_80MHz(void);
static void SystemClock_Config_24MHz (void);
static void LowPower_IO_Config(void);
static uint8_t SEL_JOY_Init(JOYMode_TypeDef Joy_Mode);
static void SEL_JOY_DeInit(void);

#if defined(__ICCARM__)
#pragma section="ROM_While1_region"
void whileloop1(void) @ "While1Section";
void whileloop1(void)
#elif defined(__GNUC__)
void __attribute__((section(".ROM_While1_section"), noinline)) whileloop1(void)
#endif
#if !defined(__CC_ARM)
{
  while(1);
}
#endif
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
  uint32_t SMPS_status = 0;

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

  /* Configure LED1, and LED2 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  /* Configure the system clock to 80 MHz */
  SystemClock_Config_80MHz();

  /* ------------------------------------------ */

  if (SMPS_OK != BSP_SMPS_DeInit())
  {
    Error_Handler();
  }
  if (SMPS_OK != BSP_SMPS_Init())
  {
    Error_Handler();
  }

  /* Insert 5 seconds delay */
  HAL_Delay(5000);

  /********************************/
  /* 80 MHZ                       */
  /* PWR_REGULATOR_VOLTAGE_SCALE1 */
  /* SMPS IS OFF                  */
  /********************************/

  /* ------------------------------------------ */

  SystemClock_Config_24MHz();

  /* Insert 3 seconds delay */
  HAL_Delay(3000);

  /********************************/
  /* 24 MHZ                       */
  /* PWR_REGULATOR_VOLTAGE_SCALE1 */
  /* SMPS IS OFF                  */
  /********************************/

  /* ------------------------------------------ */

  /* PWR_REGULATOR_VOLTAGE_SCALE2 */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);
  if (SMPS_status != SMPS_OK)
  {
    Error_Handler();
  }

  /* Insert 2 seconds delay */
  HAL_Delay(2000);

  /********************************/
  /* 24 MHZ                       */
  /* PWR_REGULATOR_VOLTAGE_SCALE2 */
  /* SMPS IS OFF                  */
  /********************************/

  /* ------------------------------------------ */

  /* Enable SMPS */
  BSP_SMPS_Enable (0 , 1);
  /* Check of PG */
  if (SMPS_OK != BSP_SMPS_Supply_Enable (4 , 1))
  {
    Error_Handler();
  }

  /* Insert 1 second delay */
  HAL_Delay(1000);

  /********************************/
  /* 24 MHZ                       */
  /* PWR_REGULATOR_VOLTAGE_SCALE2 */
  /* SMPS IS ON                  */
  /********************************/

  /* ------------------------------------------ */

  SystemClock_Config_80MHz();

  /********************************/
  /* 80 MHZ                       */
  /* PWR_REGULATOR_VOLTAGE_SCALE2 */
  /* SMPS IS ON                  */
  /********************************/

  /* ------------------------------------------ */


  /* Ensure that MSI is wake-up system clock */
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);



  while (1)
  {

    /* Turn on LED2 to indicate user can press the joystick SEL button from that point */
    BSP_LED_Init(LED2);
    BSP_LED_On(LED2);
    /* Joystick SEL button is used to exit infinite loop in Run mode */
    SEL_JOY_Init(JOY_MODE_EXTI);
    whileloop1();

    /* Wait that user releases the joystick SEL button */
    SEL_JOY_DeInit();
    SEL_JOY_Init(JOY_MODE_GPIO);
    while (HAL_GPIO_ReadPin(SEL_JOY_GPIO_PORT, SEL_JOY_PIN) == GPIO_PIN_RESET){}
    SEL_JOY_DeInit();

    /* Turn off LED's */
    BSP_LED_Off(LED1);
    BSP_LED_Off(LED2);

    /* Set IOs in low power mode configuration */
    LowPower_IO_Config();

    /* Joystick SEL button is used to exit Stop0 low power moded */
    SEL_JOY_Init(JOY_MODE_EXTI);

    /* Enter STOP 0 mode */
    HAL_PWREx_EnterSTOP0Mode(PWR_STOPENTRY_WFI);

    /* ... STOP 0 mode ... */

    /* Wait that user releases the joystick SEL button */
    SEL_JOY_DeInit();
    SEL_JOY_Init(JOY_MODE_GPIO);
    while (HAL_GPIO_ReadPin(SEL_JOY_GPIO_PORT, SEL_JOY_PIN) == GPIO_PIN_RESET){}
    SEL_JOY_DeInit();


    /********************************/
    /* 4 MHZ                        */
    /* PWR_REGULATOR_VOLTAGE_SCALE2 */
    /* SMPS IS OFF                  */
    /********************************/
    if (SMPS_OK != BSP_SMPS_Init())
    {
      Error_Handler();
    }

    while (SMPS_OK != BSP_SMPS_Enable (0,1)) { }

    if (SMPS_OK != BSP_SMPS_Supply_Enable (10 , 1))
    {
      Error_Handler();
    }

    /********************************/
    /* 4 MHZ                        */
    /* PWR_REGULATOR_VOLTAGE_SCALE2 */
    /* SMPS IS ON                   */
    /********************************/

    /* Configure the system clock to 80 MHz */
    SystemClock_Config_80MHz();

    /********************************/
    /* 80 MHZ                       */
    /* PWR_REGULATOR_VOLTAGE_SCALE2 */
    /* SMPS IS ON                   */
    /********************************/
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 24000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 24000000
  * @param  None
  * @retval None
  */

static void SystemClock_Config_24MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Select MSI as system clock source */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Update MSI to 24Mhz (RCC_MSIRANGE_9) */
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

static void LowPower_IO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

      /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /* Set all GPIO in analog state to reduce power consumption,                */
  /*   except GPIOC to keep joystick SEL button interrupt enabled             */
  /* Note: Debug using ST-Link is not possible during the execution of this   */
  /*       example because communication between ST-link and the device       */
  /*       under test is done through UART. All GPIO pins are disabled (set   */
  /*       to analog input mode) including  UART I/O pins.                    */
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.Pin ^= GPIO_PIN_13;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);

  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOF_CLK_DISABLE();
  __HAL_RCC_GPIOG_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();
  __HAL_RCC_GPIOI_CLK_DISABLE();

}

/**
  * @brief  Configure joystick SEL button in GPIO or EXTI modes.
  * @param  Joy_Mode: Joystick mode.
  *    This parameter can be one of the following values:
  *     @arg  JOY_MODE_GPIO: Joystick pin will be used as simple IO
  *     @arg  JOY_MODE_EXTI: Joystick pin will be connected to EXTI line
  *                                 with interrupt generation capability
  * @retval HAL_OK: if all initializations are OK. Other value if error.
  */
static uint8_t SEL_JOY_Init(JOYMode_TypeDef Joy_Mode)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  SEL_JOY_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin = SEL_JOY_PIN;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  if (Joy_Mode == JOY_MODE_GPIO)
  {
      /* Configure Joy pin as input */
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      HAL_GPIO_Init(SEL_JOY_GPIO_PORT, &GPIO_InitStruct);
    }
    else if (Joy_Mode == JOY_MODE_EXTI)
    {
      /* Configure Joy pin as input with External interrupt */
      GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
      HAL_GPIO_Init(SEL_JOY_GPIO_PORT, &GPIO_InitStruct);
      /* Enable and set Joy EXTI Interrupt to the lowest priority */
      HAL_NVIC_SetPriority((IRQn_Type)(SEL_JOY_EXTI_IRQn), 0x0F, 0x00);
      HAL_NVIC_EnableIRQ((IRQn_Type)(SEL_JOY_EXTI_IRQn));
    }

  return HAL_OK;
}

/**
  * @brief  Deinit joystick SEL button
  * @retval None.
  */
static void SEL_JOY_DeInit(void)
{
  /* Enable the JOY clock */
  SEL_JOY_GPIO_CLK_ENABLE();
  HAL_GPIO_DeInit(SEL_JOY_GPIO_PORT, SEL_JOY_PIN);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED2 */
  BSP_LED_Init(LED2);
  BSP_LED_On(LED2);
  while (1)
  {
    HAL_Delay(500);
    BSP_LED_Toggle(LED2);
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
    /* Toggle LED1 */
    BSP_LED_Toggle(LED1);
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
  if ((GPIO_Pin & SEL_JOY_PIN) != 0U)
  {
    /* Reconfigure LED1 */
    BSP_LED_Init(LED1);
    /* Switch on LED1 */
    BSP_LED_On(LED1);
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

