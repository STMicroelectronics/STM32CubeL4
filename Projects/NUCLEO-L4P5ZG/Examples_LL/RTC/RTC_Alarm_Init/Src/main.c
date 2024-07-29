/**
  ******************************************************************************
  * @file    Examples_LL/RTC/RTC_Alarm_Init/Src/main.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use STM32L4xx RTC LL API to configure
  *          an alarm.
  *          Peripheral initialization done using LL initialization function.
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

/** @addtogroup STM32L4xx_LL_Examples
  * @{
  */

/** @addtogroup RTC_Alarm_Init
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Oscillator time-out values */
#define LSI_TIMEOUT_VALUE          ((uint32_t)2)     /* 2 ms */
#define LSE_TIMEOUT_VALUE          ((uint32_t)5000)  /* 5 s */

/* Defines related to Clock configuration */
/* Uncomment to enable the adequate Clock Source */
#define RTC_CLOCK_SOURCE_LSE
/*#define RTC_CLOCK_SOURCE_LSI*/

#ifdef RTC_CLOCK_SOURCE_LSI
/* ck_apre=LSIFreq/(ASYNC prediv + 1) with LSIFreq=32 kHz RC */
#define RTC_ASYNCH_PREDIV          ((uint32_t)0x7F)
/* ck_spre=ck_apre/(SYNC prediv + 1) = 1 Hz */
#define RTC_SYNCH_PREDIV           ((uint32_t)0x00F9)
#endif

#ifdef RTC_CLOCK_SOURCE_LSE
/* ck_apre=LSEFreq/(ASYNC prediv + 1) = 256Hz with LSEFreq=32768Hz */
#define RTC_ASYNCH_PREDIV          ((uint32_t)0x7F)
/* ck_spre=ck_apre/(SYNC prediv + 1) = 1 Hz */
#define RTC_SYNCH_PREDIV           ((uint32_t)0x00FF)
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Buffers used for displaying Time and Date */
uint8_t aShowTime[16] = "hh:ms:ss";
uint8_t aShowDate[16] = "dd/mm/aaaa";

#if (USE_TIMEOUT == 1)
uint32_t Timeout = 0; /* Variable used for Timeout management */
#endif /* USE_TIMEOUT */

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_RTC(void);
void     Configure_RTC_Alarm(void);
void     Show_RTC_Calendar(void);
void     LED_Init(void);
void     LED_On(void);
void     LED_Blinking(uint32_t Period);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 120 MHz */
  SystemClock_Config();

  /* Initialize LED1 */
  LED_Init();

  /*##-1- Configure the RTC peripheral #######################################*/
  Configure_RTC();

  /*##-2- Configure Alarm ####################################################*/
  /* Configure RTC Alarm */
  Configure_RTC_Alarm();

  /* Infinite loop */
  while (1)
  {
    /*##-3- Display the updated Time and Date ################################*/
    Show_RTC_Calendar();
  }
}

/**
  * @brief  Configure RTC.
  * @param  None
  * @retval None
  */
void Configure_RTC(void)
{
  LL_RTC_InitTypeDef rtc_initstruct;

  /*##-1- Enables the PWR Clock and Enables access to the backup domain #######*/
  /* To change the source clock of the RTC feature (LSE, LSI), you have to:
     - Enable the power clock
     - Enable write access to configure the RTC clock source (to be done once after reset).
     - Reset the Back up Domain
     - Configure the needed RTC clock source */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  LL_PWR_EnableBkUpAccess();

  /*##-2- Configure LSE/LSI as RTC clock source ###############################*/
#ifdef RTC_CLOCK_SOURCE_LSE
  /* Enable LSE only if disabled.*/
  if (LL_RCC_LSE_IsReady() == 0)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_LSE_DisablePropagation();
    LL_RCC_LSE_Enable();
#if (USE_TIMEOUT == 1)
    Timeout = LSE_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
    while (LL_RCC_LSE_IsReady() != 1)
    {
#if (USE_TIMEOUT == 1)
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        Timeout --;
      }
      if (Timeout == 0)
      {
        /* LSE activation error */
        LED_Blinking(LED_BLINK_ERROR);
      }
#endif /* USE_TIMEOUT */
    }
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);

    /*##-3- Enable RTC peripheral Clocks #######################################*/
    /* Enable RTC Clock */
    LL_RCC_EnableRTC();
  }
#elif defined(RTC_CLOCK_SOURCE_LSI)
  if (LL_RCC_LSI_IsReady() == 0)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_LSI_Enable();
#if (USE_TIMEOUT == 1)
    Timeout = LSI_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
    while (LL_RCC_LSI_IsReady() != 1)
    {
#if (USE_TIMEOUT == 1)
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        Timeout --;
      }
      if (Timeout == 0)
      {
        /* LSI activation error */
        LED_Blinking(LED_BLINK_ERROR);
      }
#endif /* USE_TIMEOUT */
    }
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);

    /*##-3- Enable RTC peripheral Clocks #######################################*/
    /* Enable RTC Clock */
    LL_RCC_EnableRTC();
  }

#else
#error "configure clock for RTC"
#endif

  /*##-4 Configure RTC ######################################################*/
  /* Configure RTC prescaler and RTC data registers */
  if (LL_RTC_DeInit(RTC) != SUCCESS)
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /* Set fields of initialization structure:
      - Hour Format    = Format 12
      - Asynch Prediv  = Value according to source clock
      - Synch Prediv   = Value according to source clock
  */
  rtc_initstruct.HourFormat      = LL_RTC_HOURFORMAT_AMPM;
  rtc_initstruct.AsynchPrescaler = RTC_ASYNCH_PREDIV;
  rtc_initstruct.SynchPrescaler  = RTC_SYNCH_PREDIV;

  /* Initialize RTC instance according to parameters defined in initialization structure. */
  if (LL_RTC_Init(RTC, &rtc_initstruct) != SUCCESS)
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /* Disable RTC registers write protection */
  LL_RTC_DisableWriteProtection(RTC);
  /* Enable Calibration Low Power */
  LL_RTC_CAL_LowPower_Enable(RTC);
  /* Enable RTC registers write protection */
  LL_RTC_EnableWriteProtection(RTC);

}

/**
  * @brief Configure the current time and date and set alarm.
  * Note   __LL_RTC_CONVERT_BIN2BCD helper macro can be used if user wants to
  *        provide directly the decimal value:
  *        LL_RTC_DATE_Config(RTC, LL_RTC_WEEKDAY_MONDAY,
  *                          __LL_RTC_CONVERT_BIN2BCD(31), (...))
  * @param  None
  * @retval None
  */
void Configure_RTC_Alarm(void)
{
  LL_RTC_DateTypeDef rtc_date_initstruct;
  LL_RTC_TimeTypeDef rtc_time_initstruct;
  LL_RTC_AlarmTypeDef rtc_alarm_initstruct;

  /*## Configure the Date ################################################*/
  /* Set Date: Friday December 29th 2016 */
  rtc_date_initstruct.WeekDay = LL_RTC_WEEKDAY_FRIDAY;
  rtc_date_initstruct.Day     = 0x29;
  rtc_date_initstruct.Month   = LL_RTC_MONTH_DECEMBER;
  rtc_date_initstruct.Year    = 0x16;

  /* Initialize RTC date according to parameters defined in initialization structure. */
  if (LL_RTC_DATE_Init(RTC, LL_RTC_FORMAT_BCD, &rtc_date_initstruct) != SUCCESS)
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /*## Configure the Time ################################################*/
  /* Set Time: 11:59:55 PM*/
  rtc_time_initstruct.TimeFormat = LL_RTC_TIME_FORMAT_PM;
  rtc_time_initstruct.Hours      = 0x11;
  rtc_time_initstruct.Minutes    = 0x59;
  rtc_time_initstruct.Seconds    = 0x55;

  /* Initialize RTC time according to parameters defined in initialization structure. */
  if (LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BCD, &rtc_time_initstruct) != SUCCESS)
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /*## Configure the RTC Alarm peripheral #################################*/
  /* Set Alarm to 12:00:25 */
  rtc_alarm_initstruct.AlarmTime.TimeFormat = LL_RTC_ALMA_TIME_FORMAT_AM;
  rtc_alarm_initstruct.AlarmTime.Hours      = 0x12;
  rtc_alarm_initstruct.AlarmTime.Minutes    = 0x00;
  rtc_alarm_initstruct.AlarmTime.Seconds    = 0x25;

  /* RTC Alarm Generation: Alarm on Hours, Minutes and Seconds (ignore date/weekday)*/
  rtc_alarm_initstruct.AlarmMask            = LL_RTC_ALMA_MASK_DATEWEEKDAY;
  rtc_alarm_initstruct.AlarmDateWeekDaySel  = LL_RTC_ALMA_DATEWEEKDAYSEL_DATE;
  rtc_alarm_initstruct.AlarmDateWeekDay     = 0x01;

  /* Initialize ALARM A according to parameters defined in initialization structure. */
  if (LL_RTC_ALMA_Init(RTC, LL_RTC_FORMAT_BCD, &rtc_alarm_initstruct) != SUCCESS)
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /* Disable the write protection for RTC registers */
  LL_RTC_DisableWriteProtection(RTC);

  /* Enable Alarm*/
  LL_RTC_ALMA_Enable(RTC);

  /* Clear the Alarm interrupt pending bit */
  LL_RTC_ClearFlag_ALRA(RTC);

  /* Enable IT Alarm */
  LL_RTC_EnableIT_ALRA(RTC);

  /* Enable the write protection for RTC registers */
  LL_RTC_EnableWriteProtection(RTC);

  /* RTC Alarm Interrupt Configuration: EXTI configuration */
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_18);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_18);

  /*## Configure the NVIC for RTC Alarm ##################################*/
  NVIC_SetPriority(RTC_Alarm_IRQn, 0x0F);
  NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

/**
  * @brief  Display the current time and date.
  * @param  None
  * @retval None
  */
void Show_RTC_Calendar(void)
{
  /* Note: need to convert in decimal value in using __LL_RTC_CONVERT_BCD2BIN helper macro */
  /* Display time Format : hh:mm:ss */
  sprintf((char *)aShowTime, "%.2d:%.2d:%.2d", __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC)),
          __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC)),
          __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC)));
  /* Display date Format : mm-dd-yy */
  sprintf((char *)aShowDate, "%.2d-%.2d-%.2d", __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetMonth(RTC)),
          __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetDay(RTC)),
          2000 + __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetYear(RTC)));
}

/**
  * @brief  Initialize LED1.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED1 Clock */
  LED1_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED1 */
  LL_GPIO_SetPinMode(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Turn-on LED1.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED1 on */
  LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

/**
  * @brief  Set LED1 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
    LL_mDelay(Period);
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
  *            PLL_R                          = 2
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Enable voltage range 1 boost mode for frequency above 80 Mhz */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_EnableRange1BoostMode();
  LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);
  
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
  
  /* MSI already enabled at reset */
  
  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 60, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1) 
  {
  };
  
  /* Sysclk activation on the main PLL */
  /* Intermediate AHB prescaler 2 when target frequency clock is higher than 80 MHz */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) 
  {
  };
  
  /* Insure 1�s transition state at intermediate medium speed clock based on DWT*/
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  DWT->CYCCNT = 0;
  while(DWT->CYCCNT < 100);

  /* AHB prescaler 1 */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 120MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ(MSI_VALUE, 
                                  LL_RCC_PLLM_DIV_1, 60, LL_RCC_PLLR_DIV_2)*/
  LL_Init1msTick(120000000);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(120000000);
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Alarm callback
  * @param  None
  * @retval None
  */
void Alarm_Callback(void)
{
  /* Turn LED1 on: Alarm generation */
  LED_On();
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
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

