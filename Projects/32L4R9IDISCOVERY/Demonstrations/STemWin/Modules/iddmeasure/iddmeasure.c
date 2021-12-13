/**
  ******************************************************************************
  * @file    iddmeasure.c
  * @author  MCD Application Team
  * @brief   System information functions
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
#define __IDDMEASURE_C

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "iddmeasure.h"
#include "dimming.h"

/* Private typedef -----------------------------------------------------------*/
    
/* Private defines -----------------------------------------------------------*/
#define PWR_GPIO_BIT_ALL (PWR_GPIO_BIT_0 | PWR_GPIO_BIT_1 |PWR_GPIO_BIT_2 |PWR_GPIO_BIT_3 |PWR_GPIO_BIT_4 |PWR_GPIO_BIT_5 |PWR_GPIO_BIT_6 |PWR_GPIO_BIT_7 |PWR_GPIO_BIT_8 |PWR_GPIO_BIT_9 |PWR_GPIO_BIT_10 |PWR_GPIO_BIT_11 |PWR_GPIO_BIT_12 |PWR_GPIO_BIT_13 |PWR_GPIO_BIT_14 |PWR_GPIO_BIT_15)

#define DEBUG_ON                0
#define RTC_WAKEUP              1 /* Wakeup from standby and shutdown */
#define OPTIM_STANDBY_SHUTDOWN  0
#define IDD_SIGNATURE           1

#define RTC_CLOCK_SOURCE_LSE

#ifdef RTC_CLOCK_SOURCE_LSE
#define RTC_ASYNCH_PREDIV  (0x7F)
#define RTC_SYNCH_PREDIV   (0x00FF)
#endif

/* Private function prototypes -----------------------------------------------*/
static void Idd_RunEnter(void);
static void Idd_RunRestore(void);
static void Idd_SleepEnter(void);
static void Idd_SleepRestore(void);
static void Idd_LprEnter(void);
static void Idd_LprRestore(void);
static void Idd_LprSleepEnter(void);
static void Idd_LprSleepRestore(void);
static void Idd_StopEnter(void);
static void Idd_StopRestore(void);
static void Idd_StandbyEnter(void);
static void Idd_ShutdownEnter(void);
#if (RTC_WAKEUP == 1)
static void Idd_SetRtcWakeup(void);
#else
static void Idd_WakeUpPinConfig(void);
#endif
static void Idd_Convert(uint32_t Value, Iddvalue_t * idd);
static void Idd_ExternalSupply_ClockDecrease(void);
#ifndef WA_RESET_AFTER_IDD_MEASURE
static void Idd_ExternalSupply_ClockIncrease(void);
#endif
static void Idd_RunModeThread(void const * argument);
#if (IDD_SIGNATURE == 1)
static void IddMeasure_saveSignature(void);
#endif
static void LCD_DeInit(void);

void      SystemLowClock_Config(void);
void      SystemHardwareInit(void);
void      SystemHardwareDeInit(uint32_t mode);
void      SystemPeripheralClockEnable(void);
void      SystemPeripheralClockDisable(uint32_t rtc_on);
void      Error_Handler(void);

uint32_t MfxFwVersion = 0;
Idd_StateTypedef IddMeasureState = IDD_STATE_IDLE;
FlagStatus IddInitialized = RESET;

FlagStatus LCDInitialized = SET;

uint32_t iddTestIndex = 0;
Idd_AppliTypedef IddTest[IDD_TEST_NB]=
{
  {Idd_RunEnter, Idd_RunRestore, "Run 24Mhz", IDD_RUN},
  {Idd_SleepEnter, Idd_SleepRestore, "Sleep 24Mhz", IDD_SLEEP},
  {Idd_LprEnter, Idd_LprRestore,"LP Run 2Mhz", IDD_LPR_2MHZ},
  {Idd_LprSleepEnter, Idd_LprSleepRestore,"LP Sleep", IDD_LPR_SLEEP},
  {Idd_StopEnter, Idd_StopRestore, "Stop2", IDD_STOP2},
  {Idd_StandbyEnter, NULL,"Standby", IDD_STANDBY},
  {Idd_ShutdownEnter, NULL,"Shutdown", IDD_SHUTDOWN},
};

osThreadId runModeThreadId = 0;

#if (OPTIM_STANDBY_SHUTDOWN == 1)
typedef enum
{
  NOT_VALID = 0,
  PU,
  PD, /* update LAST_GPIO_TYPE if you add a mode */
}gpioMode_t;
#define LAST_GPIO_TYPE PD

typedef struct  
{
  gpioMode_t gpioMode;
  uint32_t port;
  uint32_t pin;
} bestConfig_t;

static const bestConfig_t bestConfigStandbyShutdown[] = {
//  {PU, PWR_GPIO_B, PWR_GPIO_BIT_9}, 
//  {PU, PWR_GPIO_B, PWR_GPIO_BIT_10},
//  {PU, PWR_GPIO_B, PWR_GPIO_BIT_12},
  {PU, PWR_GPIO_B, PWR_GPIO_BIT_13},
  {PU, PWR_GPIO_C, PWR_GPIO_BIT_2},
//  {PU, PWR_GPIO_C, PWR_GPIO_BIT_8},
//  {PU, PWR_GPIO_C, PWR_GPIO_BIT_9},
//  {PU, PWR_GPIO_C, PWR_GPIO_BIT_10},
//  {PU, PWR_GPIO_D, PWR_GPIO_BIT_2},
//  {PU, PWR_GPIO_D, PWR_GPIO_BIT_6},
//  {PU, PWR_GPIO_D, PWR_GPIO_BIT_7},
//  {PU, PWR_GPIO_G, PWR_GPIO_BIT_7},
//  {PU, PWR_GPIO_G, PWR_GPIO_BIT_8},
//  {PU, PWR_GPIO_G, PWR_GPIO_BIT_10},
//  {PU, PWR_GPIO_G, PWR_GPIO_BIT_15},
//  {PU, PWR_GPIO_H, PWR_GPIO_BIT_4},
//  {PU, PWR_GPIO_H, PWR_GPIO_BIT_5},
};
#define NB_BEST_CONFIG_STANDBY_SHUTDOWN (sizeof(bestConfigStandbyShutdown) / sizeof(bestConfig_t))
void GPIOoptimStandbyShutdown(void);
#endif /* OPTIM_STANDBY_SHUTDOWN */


/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/

static void SuspendAllTasks(void)
{
  TaskStatus_t *pxTaskStatusArray;
  volatile UBaseType_t uxArraySize, x;
  uint32_t ulTotalRunTime;

  portENTER_CRITICAL();
  /* Take a snapshot of the number of tasks in case it changes while this
  function is executing. */
  uxArraySize = uxTaskGetNumberOfTasks();

  /* Allocate a TaskStatus_t structure for each task.  An array could be
  allocated statically at compile time. */
  pxTaskStatusArray = pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

  if( pxTaskStatusArray != NULL )
  {
    /* Generate raw status information about each task. */
    uxArraySize = uxTaskGetSystemState( pxTaskStatusArray,
                               uxArraySize,
                               &ulTotalRunTime );

    /* For each populated position in the pxTaskStatusArray array,
    format the raw data as human readable ASCII data. */
    for( x = 0; x < uxArraySize; x++ )
    {
      if(xTaskGetCurrentTaskHandle() != pxTaskStatusArray[x].xHandle)
      {
        vTaskSuspend(pxTaskStatusArray[x].xHandle);
      }
    }
  }
  portEXIT_CRITICAL();
}

static void ResumeAllTasks(void)
{
#ifndef WA_RESET_AFTER_IDD_MEASURE
  TaskStatus_t *pxTaskStatusArray;
  volatile UBaseType_t uxArraySize, x;
  uint32_t ulTotalRunTime;

  portENTER_CRITICAL();
  /* Take a snapshot of the number of tasks in case it changes while this
  function is executing. */
  uxArraySize = uxTaskGetNumberOfTasks();

  /* Allocate a TaskStatus_t structure for each task.  An array could be
  allocated statically at compile time. */
  pxTaskStatusArray = pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

  if( pxTaskStatusArray != NULL )
  {
    /* Generate raw status information about each task. */
    uxArraySize = uxTaskGetSystemState( pxTaskStatusArray,
                               uxArraySize,
                               &ulTotalRunTime );

    /* For each populated position in the pxTaskStatusArray array,
    format the raw data as human readable ASCII data. */
    for( x = 0; x < uxArraySize; x++ )
    {
      if(xTaskGetCurrentTaskHandle() != pxTaskStatusArray[x].xHandle)
      {
        vTaskResume(pxTaskStatusArray[x].xHandle);
      }
    }
  }
  portEXIT_CRITICAL();
#endif  
}

#if (IDD_SIGNATURE == 1)
/**
  * @brief  Automatically restore index if signature is TRUE
  * @param  None.
  * @note   None.
  * @retval Boolean 0 = FALSE; 1 = TRUE.
  */
uint32_t IddMeasure_isSignature(void)
{
  uint32_t bkup;

  __HAL_RCC_PWR_CLK_ENABLE();
   /* Enable RTC back-up registers access */
  HAL_PWR_EnableBkUpAccess();

  bkup = READ_REG(RTC->BKP29R);
  
  if (bkup != 0)
  {
    /* Restore index */
    iddTestIndex = bkup - 1;
    return 1;    
  }
  else
  {
    return 0;
  }
}

/**
  * @brief  clear signature
  *         Signature is also cleared if TRUE
  * @param  None.
  * @note   None.
  * @retval Boolean 0 = FALSE; 1 = TRUE.
  */
void IddMeasure_clearSignature(void)
{
  __HAL_RCC_PWR_CLK_ENABLE();
   /* Enable RTC back-up registers access */
  HAL_PWR_EnableBkUpAccess();

  /* Clear signature */
  WRITE_REG(RTC->BKP29R, 0);
}


/**
  * @brief  Save index and set signature
  * @param  None.
  * @note   None.
  * @retval None.
  */
static void IddMeasure_saveSignature(void)
{
  __HAL_RCC_PWR_CLK_ENABLE();
   /* Enable RTC back-up registers access */
  HAL_PWR_EnableBkUpAccess();

  WRITE_REG( RTC->BKP29R, iddTestIndex + 1);

#define STEMWIN_FIRMWARE_ADDRESS       0x08080000
  /* Ensure that Launcher will switch to this demonstration just after the reset */
  WRITE_REG( RTC->BKP26R, STEMWIN_FIRMWARE_ADDRESS);
}
#endif

/**
  * @brief  Initialize the Idd Measurement application 
  * @param  None.
  * @note   None.
  * @retval None.
  */
void IddMeasureInit(void)
{
  /* Initialize Idd measurement component */
  if(IddInitialized != SET)
  {
    /* Avoid spurious Interrupts */
    MFX_IO_DeInit();

    if(BSP_IDD_Init() != IDD_OK)
    {
      Error_Handler();
    }
 
    /* Read FW version */
    MfxFwVersion = mfxstm32l152_ReadFwVersion(IDD_I2C_ADDRESS);

    /* Enable Interrupt to get end of measurement */
    /* This will wahe up MCU from low power mode */
    BSP_IDD_EnableIT();
    BSP_IDD_ErrorEnableIT();
    
    IddInitialized = SET;
  }

  BSP_IDD_ClearIT();
  BSP_IDD_ErrorClearIT();

  return;
}

/**
  * @brief  DeInitialize the Idd Measurement application 
  * @param  None.
  * @note   None.
  * @retval None.
  */
void IddMeasureDeInit(void)
{
  /* Enters MFX in standby mode */
  BSP_IDD_LowPower();
  
  BSP_IDD_DeInit();
  IddInitialized = RESET;

  return;
}

Idd_StateTypedef IddMeasureGetState()
{
  return IddMeasureState;
}

void IddMeasureStart(void)
{
  SuspendAllTasks();

  IddMeasureInit();

  /* Save current clock */
  BackUp_RTC();

  /* Start measurement campaign */
  BSP_IDD_StartMeasure();
    
#if (IDD_SIGNATURE == 1)
  IddMeasure_saveSignature();  
#endif

  IddMeasureState = IDD_STATE_MEASURE_ON_GOING;

  /* Un configure HW resources */
  SystemHardwareDeInit(iddTestIndex);

  /* if function pointer exists, execute correponsing low power action */
  if(IddTest[iddTestIndex].IddEnter != NULL)
  {
    IddTest[iddTestIndex].IddEnter();
  }

  ResumeAllTasks();
}

void IddMeasureGetValue(Iddvalue_t * idd)
{
  uint32_t IddReadValue;

  IddMeasureInit();
  
  /* check if idd interrupt that occurred is measurement done or error */
  if(IddMeasureState == IDD_STATE_MEASURE_ERROR)
  {
    /* Get error code */
    idd->error_code = BSP_IDD_ErrorGetCode();
    idd->value[0] = 0;
    idd->unit[0] = 0;
    idd->value_na = 0;
  }
  else 
  {
      idd->error_code = 0;
      
      /* Get Idd value */
      IddReadValue = 0;
      BSP_IDD_GetValue(&IddReadValue);
  
      /* Fill value in Nano amps */
      idd->value_na = 10 * IddReadValue;

      /* Convert Idd value in order to display it on LCD glass */
      Idd_Convert(IddReadValue, idd);
  }
}

/**
  * @brief  Enter MCU in Run mode @24 Mhz.
  * @param  None
  * @retval None
  */
static void Idd_RunEnter(void)
{
  Idd_ExternalSupply_ClockDecrease();

  /* Suspend HAL tick irq */
  HAL_SuspendTick();
  
  /* Suspend RTOS Systick */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  /* System peripheral clock disable, rtc off */
  SystemPeripheralClockDisable(0);
  
  /* Create and start an external thread to execute while(1) in ram context */
  osThreadDef(osRun_Mode_Thread, Idd_RunModeThread, osPriorityHigh, 0, 128);

  runModeThreadId = osThreadCreate (osThread(osRun_Mode_Thread), NULL);  
  
}

/**
  * @brief  Restore MCU configuration from Run mode.
  * @param  None
  * @retval None
  */
static void Idd_RunRestore(void)
{
#ifndef WA_RESET_AFTER_IDD_MEASURE
  Idd_ExternalSupply_ClockIncrease();
  
  /* Resume HAL tick irq */
  HAL_ResumeTick();

  /* Resume RTOS Systick */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  osThreadTerminate(runModeThreadId);
  runModeThreadId = 0;
#endif
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
  * @brief  Run mode while(1) task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void Idd_RunModeThread(void const * argument)
{
  SuspendAllTasks();

  while1Aligned64();
}

/**
  * @brief  Enter MCU in Sleep mode.
  * @param  None
  * @retval None
  */
static void Idd_SleepEnter(void)
{
  Idd_ExternalSupply_ClockDecrease();
  
  /* Suspend HAL tick irq */
  HAL_SuspendTick();
  
  /* Suspend RTOS Systick */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
  
  /* System peripheral clock disable, rtc off */
  SystemPeripheralClockDisable(0);
  
  /* Enable Flash power down mode during Sleep mode     */
  /* (uncomment this line if power consumption figures  */
  /*  must be measured with Flash still on in Low Power */
  /*  Sleep mode)                                       */
  __HAL_FLASH_SLEEP_POWERDOWN_ENABLE();  

  /* Enable PWR clock enable */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Enter in Sleep mode, Main Regulator ON */
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

/**
  * @brief  Restore MCU configuration from Sleep mode.
  * @param  None
  * @retval None
  */
static void Idd_SleepRestore(void)
{
#ifndef WA_RESET_AFTER_IDD_MEASURE
  /* Resume HAL tick irq */
  HAL_ResumeTick();

  /* Resume RTOS Systick */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
  
  Idd_ExternalSupply_ClockIncrease();
#endif
}

/**
  * @brief  Enter MCU in Low Power Run mode.
  * @param  None
  * @retval None
  */
static void Idd_LprEnter(void)
{
  /* go to 2MHz PLL Off */
  SystemLowClock_Config();

  /* Suspend HAL tick irq */
  HAL_SuspendTick();
  
  /* Suspend RTOS Systick */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  /* System peripheral clock disable, rtc off */
  SystemPeripheralClockDisable(0);
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  HAL_PWREx_EnableLowPowerRunMode();
  
  /* Disable Power Control clock */
  __HAL_RCC_PWR_CLK_DISABLE();

  /* Create and start an external thread to execute while(1) */
  osThreadDef(osRun_Mode_Thread, Idd_RunModeThread, osPriorityHigh, 0, 128);
  runModeThreadId = osThreadCreate (osThread(osRun_Mode_Thread), NULL);  
}

/**
  * @brief  Restore MCU configuration from Low Power mode.
  * @param  None
  * @retval None
  */
static void Idd_LprRestore(void)
{
#ifndef WA_RESET_AFTER_IDD_MEASURE
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Disable low power mode */
  HAL_PWREx_DisableLowPowerRunMode();

  /* Disable Power Control clock */
  __HAL_RCC_PWR_CLK_DISABLE();

  /* restore clock to PLL 80MHz */
  SystemClock_Config();

  /* Resume HAL tick irq */
  HAL_ResumeTick();

  /* Resume RTOS Systick */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  osThreadTerminate(runModeThreadId);
  runModeThreadId = 0;
#endif
}

/**
  * @brief  Enter MCU in Low Power Sleep mode.
  * @param  None
  * @retval None
  */
static void Idd_LprSleepEnter(void)
{
  /* go to 2MHz PLL Off */
  SystemLowClock_Config();

  /* Suspend HAL tick irq */
  HAL_SuspendTick();
  
  /* Suspend RTOS Systick */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  /* System peripheral clock disable rtc off */
  SystemPeripheralClockDisable(0);
  
  /* Enable Flash power down mode during Sleep mode     */
  /* (uncomment this line if power consumption figures  */
  /*  must be measured with Flash still on in Low Power */
  /*  Sleep mode)                                       */
  __HAL_FLASH_SLEEP_POWERDOWN_ENABLE();  

  /* Enable PWR clock enable */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Enter in Sleep mode, Low Power Regulator ON */
  HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

/**
  * @brief  Restore MCU configuration from Low Power Sleep mode.
  * @param  None
  * @retval None
  */
static void Idd_LprSleepRestore(void)
{
#ifndef WA_RESET_AFTER_IDD_MEASURE
  HAL_PWREx_DisableLowPowerRunMode();

  /* restore clock to PLL 80MHz */
  SystemClock_Config();

  /* Resume HAL tick irq */
  HAL_ResumeTick();

  /* Resume RTOS Systick */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
#endif  
}

/**
  * @brief  Enter MCU in Stop mode.
  * @param  None
  * @retval None
  */
static void Idd_StopEnter(void)
{
#if (PREFETCH_ENABLE != 0)
  /* Disable Prefetch Buffer */
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
#endif /* PREFETCH_ENABLE */

  /* Ensure that MSI is wake-up system clock */ 
  HAL_RCCEx_WakeUpStopCLKConfig(RCC_STOP_WAKEUPCLOCK_MSI);

  /* Enable PWR clock enable */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  HAL_PWREx_DisableLowPowerRunMode();

  /* Suspend HAL tick interrupt */
  HAL_SuspendTick();

  /* Suspend RTOS Systick */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  /* System peripheral clock disable, rtc off */
  SystemPeripheralClockDisable(0);

  /* Enable PWR clock enable */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Enter in Low power Stop 2 */
  HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
}

/**
  * @brief  Restore MCU configuration from Stop mode.
  * @param  None
  * @retval None
  */
static void Idd_StopRestore(void)
{
#ifndef WA_RESET_AFTER_IDD_MEASURE
#if (PREFETCH_ENABLE != 0)
  /* Re-enable Prefetch Buffer */
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
#endif /* PREFETCH_ENABLE */

  /* System peripheral clock enable */
  SystemPeripheralClockEnable();
  
  /* Resume HAL tick interrupt */
  HAL_ResumeTick();
  
  /* Resume RTOS Systick */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  /* Restore clock to default frequency when not running on battery */
  SystemClock_Config();
#endif
}

/**
  * @brief  Enter MCU in Standby mode.
  * @param  None
  * @retval None
  */
static void Idd_StandbyEnter(void)
{
  /* Configure wake up */
#if (RTC_WAKEUP == 1)
  Idd_SetRtcWakeup();
#else
  Idd_WakeUpPinConfig();
#endif

  /* System peripheral clock disable, rtc on */
  SystemPeripheralClockDisable(1);
#if (PREFETCH_ENABLE != 0)
  /* Disable Prefetch Buffer */
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
#endif /* PREFETCH_ENABLE */

#if (OPTIM_STANDBY_SHUTDOWN == 1)
  /* Configure the relevant I/O in pull-up or pull-down mode */
  GPIOoptimStandbyShutdown();
#endif
  
  /* Enable PWR clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Enter STANDBY mode */
  HAL_PWR_EnterSTANDBYMode();
}

/**
  * @brief  Enter MCU in Shutdown mode.
  * @param  None
  * @retval None
  */
static void Idd_ShutdownEnter(void)
{
  /* Configure wake up */
#if (RTC_WAKEUP == 1)
  Idd_SetRtcWakeup();
#else
  Idd_WakeUpPinConfig();
#endif
  /* System peripheral clock disable, rtc on */
  SystemPeripheralClockDisable(1);

#if (PREFETCH_ENABLE != 0)
  /* Disable Prefetch Buffer */
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
#endif /* PREFETCH_ENABLE */

#if (OPTIM_STANDBY_SHUTDOWN == 1)
  /* Configure the relevant I/O in pull-up or pull-down mode */
  GPIOoptimStandbyShutdown();
#endif

  /* Enable PWR clock */
  __HAL_RCC_PWR_CLK_ENABLE();
    
  /* Enter Shut Down mode */
  HAL_PWREx_EnterSHUTDOWNMode();
}

#if (RTC_WAKEUP == 1)
static void Idd_SetRtcWakeup()
{
  RTC_HandleTypeDef RTCHandle = {0};
  
  RTCHandle.Instance = RTC;                  /**< Define instance */
  /**
   * Set the Asynchronous prescaler
   */
  RTCHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RTCHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RTCHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RTCHandle.Init.HourFormat = RTC_HOURFORMAT_24 /* RTC_HOURFORMAT_12 */; /**< need to be initialized to not corrupt the RTC_CR register */
  RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH; /**< need to be initialized to not corrupt the RTC_CR register */
  RTCHandle.Init.OutPutType  = RTC_OUTPUT_TYPE_OPENDRAIN; /**< need to be initialized to not corrupt the RTC_CR register */
  HAL_RTC_Init(&RTCHandle);

  /**
   * Bypass the shadow register
   */
  HAL_RTCEx_EnableBypassShadow(&RTCHandle);
  
  /* Disable Write Protection */
  __HAL_RTC_WRITEPROTECTION_DISABLE(&RTCHandle) ;
  
  /* Wakeup Time Base = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE))
  Wakeup Time = Wakeup Time Base * WakeUpCounter 
  = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE)) * WakeUpCounter
  ==> WakeUpCounter = Wakeup Time / Wakeup Time Base

  To configure the wake up timer to TIMERSERVER_TICK_VALUE
  Wakeup Time Base = 16 /(~32.000KHz) = 0.5 ms
  4000 = 0.5 ms  * WakeUpCounter
  Wakeup Time =  4000 * 2 */
  HAL_RTCEx_SetWakeUpTimer_IT(&RTCHandle, 4000 * 2, RTC_WAKEUPCLOCK_RTCCLK_DIV16);    
}

#else /* (RTC_WAKEUP == 1) */

/**
  * @brief  Configure the Wake up pin to exit power modes.
  * @param  None
  * @retval None
  */
static void Idd_WakeUpPinConfig(void)
{
  /* Enable PWR clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Idd component measurement Interrupt pin is on PA0 which can be 
     configured as wake up pin source. Use it to wake up main MCU */
  /* Disable all used wakeup sources: WKUP pin */
  HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);
  
  /* Clear wake up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
  
  /* Enable wakeup pin WKUP2 */
  HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_HIGH);
}
#endif /* (RTC_WAKEUP == 1) */

/**
  * @brief  Convert value to display correct amper unit.
  * @param  None
  * @retval None
  */
static void Idd_Convert(uint32_t Value, Iddvalue_t * idd)
{
  float TempIddDisplay = 0;
  idd->value[0]=0;
  idd->unit[0]=0;

  TempIddDisplay = (float) Value * 10; 

  if (TempIddDisplay < 1000){  /* Value in nano amps */
    sprintf(idd->value, "%.0f", TempIddDisplay);
    sprintf(idd->unit, "nA");
  }else{  /* Value in micro amps */
    TempIddDisplay = TempIddDisplay / 1000;
    if (TempIddDisplay < 10){
      sprintf(idd->value, "%.2f", TempIddDisplay);
      sprintf(idd->unit, "uA");
    }else if (TempIddDisplay < 100){
      sprintf(idd->value, "%.1f", TempIddDisplay);
      sprintf(idd->unit, "uA");
    }else if (TempIddDisplay < 1000){
      sprintf(idd->value, "%.0f", TempIddDisplay);
      sprintf(idd->unit, "uA");
    }else{ /* Value in milli Amp */
      TempIddDisplay = TempIddDisplay/1000;
      if (TempIddDisplay < 10){
        sprintf(idd->value, "%.2f", TempIddDisplay);
        sprintf(idd->unit, "mA");
      }else if (TempIddDisplay < 100){
        sprintf(idd->value, "%.1f", TempIddDisplay);
        sprintf(idd->unit, "mA");
      }else if (TempIddDisplay < 1000){
        sprintf(idd->value, "%.0f", TempIddDisplay);
        sprintf(idd->unit, "mA");
      }
    }
  }
}


/**
  * @brief  System Clock Speed decrease when power is externally supplied
  *         Turn PLL off and set the system clock source MSI to range 9 (24 MHz)
  *         (system clock speed is reduced from 80 to 24 MHz)  
  *         Move to Voltage Scaling Range 2 
  * @note  API is called for Idd measures in Run mode          
  * @param  None
  * @retval None
  */
static void Idd_ExternalSupply_ClockDecrease(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* System clock source set to MSI */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK ;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Turn off PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Set MSI to range 9 (24 MHz) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_9; /* 24 MHz */
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Move to Voltage Scale 2 */
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);
  /* Disable Power Control clock once PWR registers are updated */
  __HAL_RCC_PWR_CLK_DISABLE();
 
}

/**
  * @brief  System Clock Speed increase when power is externally supplied
  *         Move to Voltage Scaling Range 1 
  *         Set MSI to range 7 (8 MHz) and turn PLL on
  *         Set PLL as system clock source
  *         (system clock speed is increased from 24 to 80 MHz)  
  * @note  API is called once Idd measures in Run mode are done         
  * @param  None
  * @retval None
  */  
#ifndef WA_RESET_AFTER_IDD_MEASURE
static void Idd_ExternalSupply_ClockIncrease(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  /* Move to Voltage Scale 1 */ 
   
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
   HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
   /* Disable Power Control clock once PWR registers are updated */
   __HAL_RCC_PWR_CLK_DISABLE();  
  
  /* Set MSI range 7, turn PLL on */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_7; /* 8 MHz */
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON ;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1 ;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
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
#endif

/**
  * @brief  System Clock Configuration in Low Power run mode
  *         The system Clock is configured as follow : 
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 2000000
  *            HCLK(Hz)                       = 2000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 2000000
  *            PLL_M                          = 1
  *            PLL_N                          = 80
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemLowClock_Config(void)
{
  /* oscillator and clocks configs */
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  uint32_t flatency = 0;

  /* Retrieve clock parameters */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &flatency );
  
  /* switch SYSCLK to MSI in order to modify PLL divider */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, flatency) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* turn off PLL and set MSI to 2Mhz */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;   /* 2 Mhz */
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Adapt voltage scaling to low power run */
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  if(HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Enter Low Power Run mode */
   HAL_PWREx_EnableLowPowerRunMode();
   
  /* Disable Power Control clock once PWR registers are updated */
  __HAL_RCC_PWR_CLK_DISABLE();
  
}

/**
  * @brief  System Power Configuration at Boot or from WakeUp
  * @param  Mode: Purpose of HW initialization 
  *               (HWINIT_STARTUP/HWINIT_IDD/HWINIT_STOP2/HWINIT_STANDBY)
  * @retval None
  */
void SystemHardwareInit(void)
{
#ifndef WA_RESET_AFTER_IDD_MEASURE
  /* Configure the GPIOs for MFX ######################################*/
  MFX_IO_Init();

  /* Initialize LEDs */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  /* Enable PWR clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  HAL_PWREx_EnableVddUSB();
  HAL_PWREx_EnablePullUpPullDownConfig();
  HAL_PWR_EnableBkUpAccess();
  HAL_PWREx_EnableVddIO2();
  HAL_PWREx_EnableVddUSB();
  
#if(TRACE != 0)
  /* Initialize the VCOM interface */
  vcom_init();
#endif
  
  
#if (MMI_GUI_SUPPORTED == 1)
  /* Initialize Graphics */
  GRAPHICS_Init();
#endif /* (MMI_GUI_SUPPORTED == 1) */

  /* Enable SD detection interrupt */
  if (BSP_SD_ITConfig() != TS_OK) {
    Error_Handler();
  }

  if((BSP_SD_IsDetected()))
  {  
    BSP_SD_Init();
  }

  Init_RTC();
  
  TIMER_Init(TimerInitMode_Full, &RTCHandle);
  
  Restore_RTC();

  HAL_PWREx_EnableSRAM2ContentRetention();
  HAL_PWREx_EnableSRAM3ContentRetention();

  HAL_PWREx_EnableDSIPinsPDActivation();
  // HAL_PWREx_EnableBatteryCharging(PWR_BATTERY_CHARGING_RESISTOR_5);

  DIMMING_Init();
#endif
}


extern LTDC_HandleTypeDef                   hltdc;
extern DSI_HandleTypeDef                    hdsi;
extern GFXMMU_HandleTypeDef                 hgfxmmu;

/**
  * @brief  LCD power off
  *         Power off LCD.
  */
static void LCD_PowerOff(void)
{
  if(LCDInitialized == SET)
  {
    BSP_IO_Init();

    /* Activate DSI_RESET */
    BSP_IO_WritePin(IO_PIN_10, GPIO_PIN_RESET);
    
    /* Wait at least 5 ms */
    HAL_Delay(5);

    /* Set DSI_POWER_ON to analog mode only if psram is not currently used */
#if defined(USE_STM32L4R9I_DISCO_REVA) || defined(USE_STM32L4R9I_DISCO_REVB)
    BSP_IO_ConfigPin(IO_PIN_8, IO_MODE_ANALOG);
#else /* USE_STM32L4R9I_DISCO_REVA || USE_STM32L4R9I_DISCO_REVB */
    /* Disable first DSI_1V8_PWRON then DSI_3V3_PWRON */
    BSP_IO_ConfigPin(AGPIO_PIN_2, IO_MODE_ANALOG);
    BSP_IO_ConfigPin(IO_PIN_8, IO_MODE_ANALOG);
#endif /* USE_STM32L4R9I_DISCO_REVA || USE_STM32L4R9I_DISCO_REVB */

    LCDInitialized = RESET;
  }
}

static void LCD_DeInit(void)
{
  /* Power off the LCD */
  LCD_PowerOff();
  
  if(HAL_DSI_DeInit(&hdsi) != HAL_OK)
  {
    Error_Handler();
  }

  if(HAL_LTDC_DeInit(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }

  if(HAL_GFXMMU_DeInit(&hgfxmmu) != HAL_OK)
  {
    Error_Handler();
  }
}



/**
  * @brief  System Low Power Configuration
  * @param  Mode: Purpose of HW deinitialization 
   *              (HWINIT_ERROR/HWINIT_STARTUP/HWINIT_IDD/HWINIT_STOP2/HWINIT_STANDBY)
  
  * @retval None
  */
void SystemHardwareDeInit(uint32_t mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;

#if(TRACE != 0)
  /* Initialize the VCOM interface */
  vcom_deInit();
#endif

  AUDIO_IO_Init();
  AUDIO_IO_DeInit();

  /* Deinit RTC */
  k_CalendarBkupDeInit();

  /* Stop and then deInitialize the Touch Screen Controller */
  BSP_TS_ITClear();
  BSP_TS_DeInit();

  /* DeInitialize the LCD Display */
  LCD_DeInit();
  
  /* DeInitialize the NOR flash */
  BSP_OSPI_NOR_DeInit();
  BSP_OSPI_NOR_EnableMemoryMappedMode();

  BSP_SD_DeInit();
  
  BSP_LED_DeInit(LED1);
  BSP_LED_DeInit(LED2);

  DIMMING_Term();

  /* Enable PWR  peripheral Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

#if (DEBUG_ON == 1)
  HAL_DBGMCU_EnableDBGSleepMode();
  HAL_DBGMCU_EnableDBGStopMode();
#endif

  HAL_PWREx_EnableVddIO2();

  /* DeInitialize the NOR flash */
  BSP_OSPI_NOR_DeInit();
  
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  
  /* Set all GPIO in analog state to reduce power consumption                 */
  /* Note: Debug using ST-Link is not possible during the execution of this   */
  /*       example because communication between ST-link and the device       */
  /*       under test is done through UART. All GPIO pins are disabled (set   */
  /*       to analog input mode) including  UART I/O pins.           */
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;

#if (DEBUG_ON == 1)
  /* keep swdio swclk */
  GPIO_InitStructure.Pin = (GPIO_InitStructure.Pin & ~(GPIO_PIN_13 | GPIO_PIN_14));
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure); 
  GPIO_InitStructure.Pin = GPIO_PIN_All;
#else
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure); 
#endif
  
#if (DEBUG_ON == 1)
  /* keep swo */
  GPIO_InitStructure.Pin = GPIO_PIN_All & ~(GPIO_PIN_3);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.Pin = GPIO_PIN_All;
#else
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure); 
  HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOF, &GPIO_InitStructure); 
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);    
  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);    
  /* Keep mfx wakeup pin expect for shutdown and standby */
  if((iddTestIndex != IDD_STANDBY) && (iddTestIndex != IDD_SHUTDOWN))
  {
    GPIO_InitStructure.Pin = (GPIO_PIN_All & ~(MFX_INT_PIN));
  }
  HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);    

  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOF_CLK_DISABLE();  
  __HAL_RCC_GPIOG_CLK_DISABLE();  
  __HAL_RCC_GPIOH_CLK_DISABLE();
  __HAL_RCC_GPIOI_CLK_DISABLE();

  HAL_PWR_DisableBkUpAccess();
  HAL_PWREx_DisableVddIO2();
  HAL_PWREx_DisableVddUSB();
  HAL_PWREx_DisableSRAM2ContentRetention();
  HAL_PWREx_DisableSRAM3ContentRetention();
  
  HAL_PWREx_DisableDSIPinsPDActivation();
  HAL_PWREx_DisableBatteryCharging();
  
  HAL_RCC_DeInit();
}

/**
  * @brief  Enable System peripheral clocks
  * @retval None
  */
void SystemPeripheralClockEnable(void)
{
  /* Enable minimum System peripheral clocks */
  /* others will be enabled via BSP layers   */
  __HAL_RCC_TIM6_CLK_ENABLE();
}

/**
  * @brief  Disable System peripheral clocks
  * @param  rtc_on : 1 = keep rtc;  0 = don't keep rtc
  * @retval None
  */
void SystemPeripheralClockDisable(uint32_t rtc_on)
{
  RCC_OscInitTypeDef oscinitstruct = {0};

  /* All Clock off instead of in all modes */
  if (rtc_on == 1)
  {
    oscinitstruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48;
  }
  else
  {
    oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48;
  }
 
  oscinitstruct.PLL.PLLState   = RCC_PLL_NONE;
  
  if(HAL_RCC_OscConfig(&oscinitstruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Disable remaining clocks */
  __HAL_RCC_FLASH_CLK_DISABLE();
  __HAL_RCC_SYSCFG_CLK_DISABLE();
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOF_CLK_DISABLE();
  __HAL_RCC_GPIOG_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();
  __HAL_RCC_GPIOI_CLK_DISABLE();
  __HAL_RCC_I2C1_CLK_DISABLE();
  __HAL_RCC_I2C2_CLK_DISABLE();
  __HAL_RCC_SPI2_CLK_DISABLE();
  __HAL_RCC_DFSDM1_CLK_DISABLE();
  __HAL_RCC_DMA1_CLK_DISABLE();
  __HAL_RCC_DMA2_CLK_DISABLE();
  __HAL_RCC_TIM6_CLK_DISABLE();
  __HAL_RCC_FMC_CLK_DISABLE();
  __HAL_RCC_PWR_CLK_DISABLE();
  __HAL_RCC_SAI1_CLK_DISABLE();
  __HAL_RCC_DFSDM1_CLK_DISABLE();
  __HAL_RCC_DCMI_CLK_DISABLE();  
  __HAL_RCC_OSPI1_CLK_DISABLE();
  __HAL_RCC_SDMMC1_CLK_DISABLE();
  if (rtc_on == 0)
  {
    __HAL_RCC_RTC_DISABLE();  
    __HAL_RCC_RTCAPB_CLK_DISABLE();
  }
  __HAL_RCC_DMA2D_CLK_DISABLE();
  __HAL_RCC_GFXMMU_CLK_DISABLE();
  __HAL_RCC_LTDC_CLK_DISABLE();
  __HAL_RCC_DSI_CLK_DISABLE();
  __HAL_RCC_CRC_CLK_DISABLE();


  RCC->AHB1SMENR  = 0x0;
  RCC->AHB2SMENR  = 0x0;
  RCC->AHB3SMENR  = 0x0;
  RCC->APB1SMENR1 = 0x0;
  RCC->APB1SMENR2 = 0x0;
  RCC->APB2SMENR  = 0x0;
}

/**
  * @brief  Configure the relevant I/O in pull-up or pull-down mode
  * @param  None
  * @retval None
  */
#if (OPTIM_STANDBY_SHUTDOWN == 1)
void GPIOoptimStandbyShutdown(void)
{
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_EnablePullUpPullDownConfig();  
  /***** Best config for all GPIO according to bestConfigStandbyShutdown[] ******/  
  for(uint32_t i = 0; i<NB_BEST_CONFIG_STANDBY_SHUTDOWN; i++ )
  {
    if(bestConfigStandbyShutdown[i].gpioMode == PD) {
      HAL_PWREx_EnableGPIOPullDown(bestConfigStandbyShutdown[i].port, bestConfigStandbyShutdown[i].pin);
    }

    if(bestConfigStandbyShutdown[i].gpioMode == PU) {
      HAL_PWREx_EnableGPIOPullUp(bestConfigStandbyShutdown[i].port, bestConfigStandbyShutdown[i].pin);
    }
  }
  __HAL_RCC_PWR_CLK_DISABLE();  
}
#endif


/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */

#undef __IDDMEASURE_C


