/**
  ******************************************************************************
  * @file    Demonstrations/Gravitech_4digits/Src/main.c
  * @author  MCD Application Team
  * @brief   This demo describes how display 4 digits using the Gravitech
             7 Segment Display for Arduino Nano shield mounted on the
             STM32 Nucleo board.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
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

/** @addtogroup STM32L4xx_HAL_Demonstrations
  * @{
  */

/** @addtogroup Demo
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* List of power modes */
typedef enum
{
  MODE_RUN =     1000,
  MODE_SLEEP =   2000,
  MODE_STOP1 =   3000,
  MODE_STOP2 =   4000,
  MODE_STANDBY = 5000,
  MODE_LPRUN =   6000,
  MODE_LPSLEEP = 7000,
  MODE_LATEST =  MODE_LPSLEEP
}PowerModes;

/* Private define ------------------------------------------------------------*/

/* Number of seconds that triggers a power mode change */
#define POWER_MODE_DURATION 10

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* RTC handler declaration */
RTC_HandleTypeDef RTCHandle;
__IO uint32_t RTCInterruptFlag = 0;

/* Private function prototypes -----------------------------------------------*/

void RTC_Init(void);
void RTC_WakeUpActivate(void);
void RTC_WakeUpDeactivate(void);
void SystemClock_Config(void);
void SystemClock_Decrease(void);
void SystemPower_Config(void);
void Error_Handler(void);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Power mode, first digit on display */
  PowerModes powerMode;
  /* Time counter, last digits on display */
  uint32_t timeCounter;

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

  /* Configure the system clock = 80 MHz */
  SystemClock_Config();

  /* Initialize RTC and its backup registers */
  RTC_Init();

  /* Activate the RTC wakeup timer every 1 second */
  RTC_WakeUpDeactivate();
  RTC_WakeUpActivate();

  /* Configure the GPIO to minimize power consumption.
     Warning : the link with the debugger will be lost */
  SystemPower_Config();

  /* Check if the system was resumed from Standby mode */
  if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
  {
    /* The system was resumed from Standby mode,
       clear Standby flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

    /* Retrieve the demo context saved in a backup register */
    timeCounter = READ_REG(TAMP->BKP0R);

    /* Current power mode is standby */
    powerMode = MODE_STANDBY;
  }
  else
  {
    /* The system was resumed from Standby mode,
       Initialize the demo context */
    powerMode = MODE_RUN;
    timeCounter = 0;
  }

  /* Initialize the Gravitech 4 digits 7 segments nano shield */
  if(BSP_DIGIT4_SEG7_Init() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Infinite loop */
  while (1)
  {
    /* Clear the RTC WakeUp timer flag */
    RTCInterruptFlag = 0;

    if (timeCounter >= POWER_MODE_DURATION)
    {
      /* Change power mode */
      powerMode += 1000;
      /* Reset time counter */
      timeCounter = 0;

      if (powerMode > MODE_LATEST)
      {
        powerMode = MODE_RUN;
      }
    }

    /* Display power mode followed by the time counter */
    if(BSP_DIGIT4_SEG7_Display(powerMode + timeCounter++) != HAL_OK)
    {
      /* Communication Error */
      Error_Handler();
    }

    switch (powerMode)
    {
      case MODE_RUN :
        /* Poll for RTC RTC WakeUp timer interrupt (1 second) */
        while (RTCInterruptFlag == 0);
        break;

      case MODE_SLEEP :
        /* Suspend Tick increment to prevent wakeup by Systick interrupt.         */
        /* Otherwise the Systick interrupt will wake up the device within 1ms     */
        /* (HAL time base).                                                       */
        HAL_SuspendTick();

        HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);

        /* Resume Tick interrupt */
        HAL_ResumeTick();
        break;

      case MODE_STOP1 :
          /* Enter in STOP1 mode */
          HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);

        break;

      case MODE_STOP2 :

          /* Enable ultra low power BOR and PVD supply monitoring */
          HAL_PWREx_EnableBORPVD_ULP();

          /* Enter in STOP2 mode */
          HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

        break;

      case MODE_STANDBY :
        /* The Following WakeUp sequence is highly recommended prior to each Standby mode entry
          mainly  when using more than one wakeup source this is to not miss any wakeup event.
          - Disable all used wakeup sources,
          - Clear all related wakeup flags,
          - Re-enable all used wakeup sources,
          - Enter the Standby mode.
        */

        /* Disable all used wakeup sources*/
         RTC_WakeUpDeactivate();

        /* Clear all related wakeup flags */
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
        __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTCHandle, RTC_FLAG_WUTF);

        /* Re-enable all used wakeup sources*/
        RTC_WakeUpActivate();

        /* Save the display count in backup register */
        WRITE_REG(TAMP->BKP0R, timeCounter);

        /* Enable ultra low power BOR and PVD supply monitoring */
        HAL_PWREx_EnableBORPVD_ULP();

        HAL_PWR_EnterSTANDBYMode();

        /* !!! Never reached !!! */

        Error_Handler();
        break;

      case MODE_LPRUN :
        /* Reduce the system clock below 2 MHz */
        SystemClock_Decrease();

        /* Suspend Tick increment for power consumption purposes */
        HAL_SuspendTick();

        HAL_PWREx_EnableLowPowerRunMode();

        /* Poll for RTC RTC WakeUp timer interrupt (1 second) */
        while (RTCInterruptFlag == 0);

        /* System is Low Power Run mode
           disable low power run mode and reset the clock to initialization configuration */
        HAL_PWREx_DisableLowPowerRunMode();

        /* Configure the system clock = 2 MHz */
        SystemClock_Config();
        __HAL_RCC_PWR_CLK_ENABLE();

        /* Resume Tick interrupt if disabled prior to Low Power Run mode entry */
        HAL_ResumeTick();
        break;

      case MODE_LPSLEEP :
        /* Reduce the System clock to below 2 MHz */
        SystemClock_Decrease();

        /* Suspend Tick increment to prevent wakeup by Systick interrupt.         */
        /* Otherwise the Systick interrupt will wake up the device within 1ms     */
        /* (HAL time base).                                                       */
        HAL_SuspendTick();

        HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);

        /* System is Low Power Run mode when exiting Low Power Sleep mode,
           disable low power run mode and reset the clock to initialization configuration */
        HAL_PWREx_DisableLowPowerRunMode();

        /* Configure the system clock = 2 MHz */
        SystemClock_Config();
       __HAL_RCC_PWR_CLK_ENABLE();

        /* Resume Tick interrupt if disabled prior to Low Power Sleep mode entry */
        HAL_ResumeTick();
        break;

      default :
        break;
    }
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

/**
  * @brief  Initialize RTC
  * @param  None
  * @retval None
  */
void RTC_Init(void)
{
  /* RTC config */
  RTCHandle.Instance = RTC;

  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follows:
      - Hour Format    = Format 24
      - Asynch Prediv  = Value according to source clock
      - Synch Prediv   = Value according to source clock
      - OutPut         = Output Disable
      - OutPutPolarity = High Polarity
      - OutPutType     = Open Drain */
  RTCHandle.Init.HourFormat     = RTC_HOURFORMAT_24;
  RTCHandle.Init.AsynchPrediv   = RTC_ASYNCH_PREDIV;
  RTCHandle.Init.SynchPrediv    = RTC_SYNCH_PREDIV;
  RTCHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
  RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RTCHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&RTCHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Set RTC calibration low power mode */
  HAL_RTCEx_SetLowPowerCalib(&RTCHandle, RTC_LPCAL_SET);
}

/**
  * @brief  Configure RTC timer wake up every 1 second.
  * @param  None
  * @retval None
  */
void RTC_WakeUpActivate(void)
{
  /* RTC WakeUp Interrupt Generation: the wake-up counter is set to one second.

    WakeUp Time Base = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE))
    WakeUp Time = WakeUp Time Base * WakeUpCounter
                = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE)) * WakeUpCounter
    WakeUpCounter = WakeUp Time / WakeUp Time Base

    To configure the wake up timer to 1s, calculate WakeUpCounter :
    WakeUp Time Base = 2/(~38.000KHz) = ~52.6 µs
    WakeUp Time = 0.5 ms  * WakeUpCounter
    WakeUpCounter = 1s / 52.6 µs = 19011 = 0x4A43 */
    if(HAL_RTCEx_SetWakeUpTimer_IT(&RTCHandle, 0x4A43, RTC_WAKEUPCLOCK_RTCCLK_DIV2, 0) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  Deactivate RTC wake up timer.
  * @param  None
  * @retval None
  */
void RTC_WakeUpDeactivate(void)
{
  if(HAL_RTCEx_DeactivateWakeUpTimer(&RTCHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  Alarm callback
  * @param  hrtc : RTC handle
  * @retval None
  */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
  RTCInterruptFlag = 1;
}

/**
  * @brief  System Clock Speed decrease
  *         The system Clock source is shifted from HSI to MSI
  *         while at the same time, MSI range is set to RCC_MSIRANGE_0
  *         to go down to 65kHz
  * @param  None
  * @retval None
  */
void SystemClock_Decrease(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_0;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  System Power Configuration
  * @param None
  * @retval None
  */
void SystemPower_Config(void)
{
  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
  /* Note: Debug using ST-Link is not possible during the execution of this   */
  /*       example because communication between ST-link and the device       */
  /*       under test is done through UART. All GPIO pins are disabled (set   */
  /*       to analog input mode) including  UART I/O pins.           */
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);

  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Configure LED3 */
  BSP_LED_Init(LED3);

  while (1)
  {
    /* Toggle LED3 with a period of one second */
    BSP_LED_Toggle(LED3);
    HAL_Delay(1000);
  }
}

/**
  * @}
  */

/**
  * @}
  */

