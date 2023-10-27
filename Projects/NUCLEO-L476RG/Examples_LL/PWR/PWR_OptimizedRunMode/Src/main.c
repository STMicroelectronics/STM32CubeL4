/**
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_OptimizedRunMode/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to enter or exit Low Power Run mode and update
  *          the core frequency on the fly through the STM32L4xx PWR LL API.
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

/** @addtogroup STM32L4xx_LL_Examples
  * @{
  */

/** @addtogroup PWR_OptimizedRunMode
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
#define USE_LED

typedef enum {
  RUN_MODE_DOWN_TO_24MHZ = 0,
  RUN_MODE_DOWN_TO_2MHZ  = 1,
  RUN_MODE_UP_TO_24MHZ   = 2,
  RUN_MODE_UP_TO_80MHZ   = 3
}RunMode_Typedef;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RunMode_Typedef RunMode = RUN_MODE_DOWN_TO_24MHZ;
__IO uint8_t ubExecuteRunMode = 1;
#ifdef USE_LED  
__IO uint16_t uhLedBlinkSpeed = LED_BLINK_FAST;
#endif
/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_PWR(void);
#ifdef USE_LED
void     LED_Init(void);
void     LED_Blinking(void);
#endif
void     UserButton_Init(void);
void     EnterRunMode_DownTo24MHz(void);
void     EnterRunMode_LowPower_DownTo2MHz(void);
void     EnterRunMode_UpTo24MHz(void);
void     EnterRunMode_UpTo80MHz(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 80 MHz 
   * Voltage Scaling Range 1
   */
  SystemClock_Config();

#ifdef USE_LED
  /* Initialize LED2 */
  LED_Init();
#endif

  /* Initialize User push-button */
  UserButton_Init();

  /* Configure Power IP */
  Configure_PWR();

  while(ubExecuteRunMode)
  {
#ifdef USE_LED   
    /* Led blinking until User push-button action */
    LED_Blinking();
#endif
  }

  /* Infinite loop */
  while (1)
  {
#ifdef USE_LED   
    /* Led blinking in infinite loop*/
    LED_Blinking();
#endif
  }
}

#ifdef USE_LED
/**
  * @brief  Initialize LED2.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED2 Clock */
  LED2_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED2 */
  LL_GPIO_SetPinMode(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Set LED2 to Blinking mode (Shall be call in a Loop).
  * @param  None
  * @retval None
  */
void LED_Blinking(void)
{
  /* Toggle IO. Shall be call in a loop to toggle */
  LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
  LL_mDelay(uhLedBlinkSpeed);
}
#endif

/**
  * @brief  Configures User push-button in GPIO or EXTI Line Mode.
  * @param  None
  * @retval None
  */
void UserButton_Init(void)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();

  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO);

  /* Connect External Line to the GPIO*/
  USER_BUTTON_SYSCFG_SET_EXTI();

  /* Enable a rising trigger External line 13 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();

  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);
}

/**
  * @brief  Function to configure and initialize PWR Peripheral.
  * @param  None
  * @retval None
  */
void Configure_PWR(void)
{
  /* Enable Power Clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* Ensure that MSI is wake-up system clock */ 
  /* MSI is the default value. Not mandatory to call function in this case */
  //LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_MSI);
}

/**
  * @brief  Function to decrease Frequency at 24MHz in Run Mode.
  * @param  None
  * @retval None
  */
void EnterRunMode_DownTo24MHz(void)
{
  /* 1 - Set Frequency to 24MHz (MSI) to set VOS to Range 2 */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);    
  /* Disable PLL to decrease power consumption */
  LL_RCC_PLL_Disable();
  LL_RCC_PLL_DisableDomain_SYS();
  /* Enable MSI Range Selection. Not done in SystemClock_Config() */
  LL_RCC_MSI_EnableRangeSelection();
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_9);
  /* Switch clock source on MSI */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);    
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI) 
  {
  };
  /* Set systick to 1ms in using frequency set to 24MHz */
  LL_Init1msTick(24000000);
  /* Update CMSIS variable */
  SystemCoreClock = 24000000;

  /* 2 - Adjust Flash Wait state after decrease Clock Frequency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);

  /* 3 - Set Voltage scaling to Range 2. Decrease VCore  */
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
}

/**
  * @brief  Function to decrease Frequency at 2MHZ in Low Power Run Mode.
  * @param  None
  * @retval None
  */
void EnterRunMode_LowPower_DownTo2MHz(void)
{
  /* 1 - Set Frequency to 2MHz to activate Low Power Run Mode: 2MHz */
  /* Range Selection already enabled. Need to change Range only */
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_5);
  /* Set systick to 1ms in using frequency set to 2MHz */
  LL_Init1msTick(2000000);
  /* Update CMSIS variable */
  SystemCoreClock = 2000000;

  /* 2 - Adjust Flash Wait state after decrease Clock Frequency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  /* Voltage Scaling already set to Range 2. VCore already decreased */

  /* 3 - Activate Low Power Run Mode */
  LL_PWR_EnableLowPowerRunMode();
}

/**
  * @brief  Function to increase Frequency at 24MHz in Run Mode.
  * @param  None
  * @retval None
  */
void EnterRunMode_UpTo24MHz(void)
{
  /* 1 - Deactivate Low Power Run Mode to increase Frequency up to 24MHz */
  LL_PWR_DisableLowPowerRunMode();

  /* Voltage Scaling already set to Range 2. VCore already decreased */

  /* 2 - Adjust Flash Wait state before increase Clock Frequency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);

  /* 3 - Set Frequency to 24MHz (MSI) */
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_9);  
  /* Set systick to 1ms in using frequency set to 24MHz */
  LL_Init1msTick(24000000);
  /* Update CMSIS variable */
  SystemCoreClock = 24000000;
}

/**
  * @brief  Function to decrease Frequency at 80MHz in Run Mode.
  * @param  None
  * @retval None
  */
void EnterRunMode_UpTo80MHz(void)
{
  /* 1 - Set Voltage scaling to Range 1 before increase Clock Frequency */
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);

  /* 2 - Wait Voltage Scaling 1 before inscrease frequency */
  while(LL_PWR_IsActiveFlag_VOSF() !=  0)
  {
  };

  /* 3 - Adjust Flash Wait state before increase Clock Frequency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);    

  /* 4 - Set Frequency to 80MHz (PLL) */
  /* Set default MSI range used by SystemClock_Config */
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_6);  
  /* Enable PLL*/ 
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1) 
  {
  };
  /* Switch on PLL. Previous configuration done by SystemClock_Config is used */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) 
  {
  };
  /* Set systick to 1ms in using frequency set to 80MHz */
  LL_Init1msTick(80000000);
  /* Update CMSIS variable */
  SystemCoreClock = 80000000;
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
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* MSI configuration and activation */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
  LL_RCC_MSI_Enable();
  while(LL_RCC_MSI_IsReady() != 1) 
  {
  };
  
  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 40, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1) 
  {
  };
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) 
  {
  };
  
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 80MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ(__LL_RCC_CALC_MSI_FREQ(LL_RCC_MSIRANGESEL_RUN, LL_RCC_MSIRANGE_6), 
                                  LL_RCC_PLLM_DIV_1, 40, LL_RCC_PLLR_DIV_2)*/
  LL_Init1msTick(80000000);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(80000000);
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Function to manage BUTTON IRQ Handler
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  switch(RunMode)
  {
  case RUN_MODE_DOWN_TO_24MHZ:
    {
      /* Decrease core frequency and voltage
       * Frequency: 80MHz -> 24MHz
       * Voltage Scaling Range 2
       */
      EnterRunMode_DownTo24MHz();
#ifdef USE_LED
      uhLedBlinkSpeed = LED_BLINK_MEDIUM;
#endif
      /* Set Next RunMode to execute */
      RunMode = RUN_MODE_DOWN_TO_2MHZ;
      break;
    }
  case RUN_MODE_DOWN_TO_2MHZ:
    {
      /* Decrease core frequency and enter Low Power Run mode
       * Frequency: 2MHz
       * Voltage Scaling Range 2
       * LowPowerRunMode activated
       */
      EnterRunMode_LowPower_DownTo2MHz();
#ifdef USE_LED
      uhLedBlinkSpeed = LED_BLINK_SLOW;
#endif
      /* Set Next RunMode to execute */
      RunMode = RUN_MODE_UP_TO_24MHZ;
      break;
    }
  case RUN_MODE_UP_TO_24MHZ:
    {
      /* Increase core frequency and exit Low Power Run mode 
       * Frequency: 2MHz -> 24MHz
       * Voltage Scaling Range 2
       * LowPowerRunMode deactivated
       */
      EnterRunMode_UpTo24MHz();
#ifdef USE_LED
      uhLedBlinkSpeed = LED_BLINK_MEDIUM;
#endif
      /* Set Next RunMode to execute */
      RunMode = RUN_MODE_UP_TO_80MHZ;
      break;
    }
  case RUN_MODE_UP_TO_80MHZ:
    {
      /* Increase core frequency and voltage:
       * Frequency: 24MHz -> 80MHz
       * Voltage Scaling Range 1
       */
      EnterRunMode_UpTo80MHz(); 
#ifdef USE_LED
      uhLedBlinkSpeed = LED_BLINK_FAST;
#endif
      /* Exit Test */
      ubExecuteRunMode = 0;
      break;
    }
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

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

