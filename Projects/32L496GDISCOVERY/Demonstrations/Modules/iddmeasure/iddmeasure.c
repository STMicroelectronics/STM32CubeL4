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
#include <stdio.h>
#include <string.h>
#include "stm32l496g_discovery_idd.h"
#include "iddmeasure.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define PWR_GPIO_BIT_ALL (PWR_GPIO_BIT_0 | PWR_GPIO_BIT_1 |PWR_GPIO_BIT_2 |PWR_GPIO_BIT_3 |PWR_GPIO_BIT_4 |PWR_GPIO_BIT_5 |PWR_GPIO_BIT_6 |PWR_GPIO_BIT_7 |PWR_GPIO_BIT_8 |PWR_GPIO_BIT_9 |PWR_GPIO_BIT_10 |PWR_GPIO_BIT_11 |PWR_GPIO_BIT_12 |PWR_GPIO_BIT_13 |PWR_GPIO_BIT_14 |PWR_GPIO_BIT_15)

/* Private function prototypes -----------------------------------------------*/
static IDD_StatusTypeDef IddMeasureGetValue(void);
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
static void Idd_WakeUpPinConfig(void);
static void Idd_Convert(uint32_t Value, char *DisplayString);
static void Idd_ExternalSupply_ClockDecrease(void);
static void Idd_ExternalSupply_ClockIncrease(void);
static void Idd_RunModeThread(void const * argument);

uint32_t MfxFwVersion = 0;
Idd_StateTypedef IddMeasureState = IDD_STATE_IDLE;
uint32_t IddReadValue = 0;
FlagStatus IddInitialized = RESET;

#define IDD_VALUE_MAX_SIZE 20
char IddStringValue[IDD_VALUE_MAX_SIZE+1] = {0};


uint32_t iddTestIndex = 0;
Idd_AppliTypedef IddTest[IDD_TEST_NB]=
{
  {Idd_RunEnter, Idd_RunRestore, "Run 24Mhz"},
  {Idd_SleepEnter, Idd_SleepRestore, "Sleep 24Mhz"},
  {Idd_LprEnter, Idd_LprRestore,"LP Run 2Mhz"},
  {Idd_LprSleepEnter, Idd_LprSleepRestore,"LP Sleep", IDD_LPR_SLEEP},
  {Idd_StopEnter, Idd_StopRestore, "Stop2", IDD_STOP2},
  {Idd_StandbyEnter, NULL,"Standby", IDD_STANDBY},
  {Idd_ShutdownEnter, NULL,"Shutdown", IDD_SHUTDOWN},
};

osThreadId runModeThreadId = 0;

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
extern K_ModuleItem_Typedef  idd_measure_board;
extern int sd_status;

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

char * IddMeasureGetIDD(void){
  IDD_StatusTypeDef errorcode = IDD_ERROR;
  uint8_t retrynb = 0;
  uint8_t shuntused = MFXSTM32L152_IDD_SHUNT_NB_4;


  
  IddMeasureInit();

  if ((WakeUpFromStandby != RESET) || (WakeUpFromShutdown != RESET))
  {
    WakeUpFromShutdown = RESET;
    WakeUpFromStandby = RESET;
     module_prop[k_ModuleGetIndex(&idd_measure_board)].in_use = 1;
     sd_status = 0;
    
    /* Get Idd Measured value */
    /* Get Idd value */
    BSP_IDD_GetValue(&IddReadValue);

    if(IddReadValue == 0)
    {
      sprintf(IddStringValue, "ERR 0");
    }
    else
    {
      /* Convert Idd value in order to display it on LCD glass */
      Idd_Convert(IddReadValue, IddStringValue);
    }
  }
  else
  {
    while((errorcode != IDD_OK) && (retrynb < 5))
    {
      /* in case of idd measurement error, restart campaign with latest shunt used as limit */
      if((retrynb != 0) && (errorcode == IDD_ERROR))
      {
        shuntused = mfxstm32l152_IDD_GetShuntUsed(IDD_I2C_ADDRESS);
        if(shuntused == MFXSTM32L152_IDD_SHUNT_NB_4)
        {
          shuntused--;
        }
        if(shuntused == 0)
        {
          shuntused = MFXSTM32L152_IDD_SHUNT_NB_4;
        }
        mfxstm32l152_IDD_ConfigShuntNbLimit(IDD_I2C_ADDRESS, shuntused);
      }
    
      /* Start measurement campaign */
      BSP_IDD_StartMeasure();
      IddMeasureState = IDD_STATE_MEASURE_ON_GOING;

      /* Un configure HW resources */
      SystemHardwareDeInit(HWINIT_IDD);

      /* Get semaphore if available */
      osSemaphoreWait(MfxIddSemaphore, 0);

      /* if function pointer exists, execute correponsing low power action */
      if(IddTest[iddTestIndex].IddEnter != NULL)
      {
        IddTest[iddTestIndex].IddEnter();
      }

      /* MFX task will restore power context, configure Hardware resources and release the semaphore */
      osSemaphoreWait(MfxIddSemaphore, osWaitForever);
      
      /* Get Idd Measured value */
      errorcode = IddMeasureGetValue();
      retrynb++;
    }

    /* If modified during retry procedure, restore Shunt used to shunt number on board */
    if(shuntused != MFXSTM32L152_IDD_SHUNT_NB_4)
    {
      mfxstm32l152_IDD_ConfigShuntNbLimit(IDD_I2C_ADDRESS, MFXSTM32L152_IDD_SHUNT_NB_4);
    }
  }
  
  return IddStringValue;
}

static IDD_StatusTypeDef IddMeasureGetValue(void)
{
  uint8_t idderror = 0;
  IddStringValue[0] = 0;

  /* check if idd interrupt that occurred is measurement done or error */
  if(IddMeasureState == IDD_STATE_MEASURE_ERROR)
  {
    /* Get error code */
    idderror = BSP_IDD_ErrorGetCode();
    sprintf(IddStringValue, "Error 0x%x", idderror);
    idderror = IDD_ERROR;
  }
  else 
  {
    /* Get Idd value */
    BSP_IDD_GetValue(&IddReadValue);
  
    if(IddReadValue == 0)
    {
      sprintf(IddStringValue, "0 nA");
      idderror = IDD_ZERO_VALUE;
    }
    else
    {
      /* Convert Idd value in order to display it on LCD glass */
      Idd_Convert(IddReadValue, IddStringValue);
      idderror = IDD_OK;
    }
  }
  return (IDD_StatusTypeDef)idderror;
}

/**
  * @brief  Enter MCU in Run mode @24 Mhz.
  * @param  None
  * @retval None
  */
static void Idd_RunEnter(void)
{
  HAL_RCC_DeInit();
  
  Idd_ExternalSupply_ClockDecrease();

  /* Suspend HAL tick irq */
  HAL_SuspendTick();
  
  /* Suspend RTOS Systick */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  /* System peripheral clock disable */
  SystemPeripheralClockDisable();
  
  /* Create and start an external thread to execute while(1) in ram context */
  osThreadDef(osRun_Mode_Thread, Idd_RunModeThread, osPriorityNormal, 0, 128);
  runModeThreadId = osThreadCreate (osThread(osRun_Mode_Thread), NULL);  
}

/**
  * @brief  Restore MCU configuration from Run mode.
  * @param  None
  * @retval None
  */
static void Idd_RunRestore(void)
{
  Idd_ExternalSupply_ClockIncrease();
  
  /* Resume HAL tick irq */
  HAL_ResumeTick();

  /* Resume RTOS Systick */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  osThreadTerminate(runModeThreadId);
  runModeThreadId = 0;
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
  while1Aligned64();
}

/**
  * @brief  Enter MCU in Sleep mode.
  * @param  None
  * @retval None
  */
static void Idd_SleepEnter(void)
{
  HAL_RCC_DeInit();

  Idd_ExternalSupply_ClockDecrease();
  
  /* Suspend HAL tick irq */
  HAL_SuspendTick();
  
  /* Suspend RTOS Systick */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
  
  /* System peripheral clock disable */
  SystemPeripheralClockDisable();
  
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
  /* Disable Power Control clock */
  __HAL_RCC_PWR_CLK_DISABLE();

  /* Resume HAL tick irq */
  HAL_ResumeTick();

  /* Resume RTOS Systick */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
  
  Idd_ExternalSupply_ClockIncrease();
}

/**
  * @brief  Enter MCU in Low Power Run mode.
  * @param  None
  * @retval None
  */
static void Idd_LprEnter(void)
{
  HAL_RCC_DeInit();

  /* go to 2MHz PLL Off */
  SystemLowClock_Config();

  /* Suspend HAL tick irq */
  HAL_SuspendTick();
  
  /* Suspend RTOS Systick */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  /* System peripheral clock disable */
  SystemPeripheralClockDisable();
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  HAL_PWREx_EnableLowPowerRunMode();
  
  /* Disable Power Control clock */
  __HAL_RCC_PWR_CLK_DISABLE();

  /* Create and start an external thread to execute while(1) in ram context */
  osThreadDef(osRun_Mode_Thread, Idd_RunModeThread, osPriorityNormal, 0, 128);
  runModeThreadId = osThreadCreate (osThread(osRun_Mode_Thread), NULL);  
}

/**
  * @brief  Restore MCU configuration from Low Power mode.
  * @param  None
  * @retval None
  */
static void Idd_LprRestore(void)
{
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
}

/**
  * @brief  Enter MCU in Low Power Sleep mode.
  * @param  None
  * @retval None
  */
static void Idd_LprSleepEnter(void)
{
  HAL_RCC_DeInit();
  
  /* go to 2MHz PLL Off */
  SystemLowClock_Config();

  /* Suspend HAL tick irq */
  HAL_SuspendTick();
  
  /* Suspend RTOS Systick */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  /* System peripheral clock disable */
  SystemPeripheralClockDisable();
  
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
  HAL_PWREx_DisableLowPowerRunMode();

  /* restore clock to PLL 80MHz */
  SystemClock_Config();

  /* Resume HAL tick irq */
  HAL_ResumeTick();

  /* Resume RTOS Systick */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
}

/**
  * @brief  Enter MCU in Stop mode.
  * @param  None
  * @retval None
  */
static void Idd_StopEnter(void)
{
  EnterStop2Mode();
}

/**
  * @brief  Restore MCU configuration from Stop mode.
  * @param  None
  * @retval None
  */
static void Idd_StopRestore(void)
{
  /* call predifined function: */
  ExitStop2Mode();
}

/**
  * @brief  Enter MCU in Standby mode.
  * @param  None
  * @retval None
  */
static void Idd_StandbyEnter(void)
{
#if (PREFETCH_ENABLE != 0)
  /* Disable Prefetch Buffer */
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
#endif /* PREFETCH_ENABLE */

  /* Enable PWR clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Configure wakeup pin for Idd measure */
  Idd_WakeUpPinConfig();

  /* Request to enter STANDBY mode */
  HAL_PWR_EnterSTANDBYMode();
}

/**
  * @brief  Enter MCU in Shutdown mode.
  * @param  None
  * @retval None
  */
static void Idd_ShutdownEnter(void)
{
#if (PREFETCH_ENABLE != 0)
  /* Disable Prefetch Buffer */
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
#endif /* PREFETCH_ENABLE */

  /* Enable PWR clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  
  /* Configure wakeup pin for Idd measure */
  Idd_WakeUpPinConfig();

  /* Set RTC back-up register RTC_BKP31R to indicate
     later on that system has entered shutdown mode  */
  HAL_PWR_EnableBkUpAccess();
  WRITE_REG( RTC->BKP31R, 0x1 );
    
  /* Request to enter Shut Down mode */
  HAL_PWREx_EnterSHUTDOWNMode();
}

/**
  * @brief  Configure the Wake up pin to exit power modes.
  * @param  None
  * @retval None
  */
static void Idd_WakeUpPinConfig(void)
{
  /* Idd component measurement Interrupt pin is on PC5 which can be 
     configured as wake up pin source. Use it to wake up main MCU */
  /* Disable all used wakeup sources: WKUP pin */
  HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN5);
  
  /* Clear wake up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF5);
  
  /* Enable wakeup pin WKUP2 */
  HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN5_HIGH);
}


/**
  * @brief  Convert value to display correct amper unit.
  * @param  None
  * @retval None
  */
static void Idd_Convert(uint32_t Value, char *DisplayString)
{
  float TempIddDisplay = 0;
  *DisplayString=0;

  TempIddDisplay = (float) Value * 10;  /* Value in nano amps */

  if (TempIddDisplay < 1000)
  {
    sprintf(DisplayString, "%.0f nA", TempIddDisplay);
  }
  else
  {
    /* Value in uA */
    TempIddDisplay = TempIddDisplay/1000;
    if (TempIddDisplay < 10)
      sprintf(DisplayString, "%.2f uA", TempIddDisplay);
    else if (TempIddDisplay < 100)
      sprintf(DisplayString, "%.1f uA", TempIddDisplay);
    else if (TempIddDisplay < 1000)
      sprintf(DisplayString, "%.0f uA", TempIddDisplay);
    else
    { /* Value in mA */
      TempIddDisplay = TempIddDisplay/1000;
      if (TempIddDisplay < 10)
        sprintf(DisplayString, "%.2f mA", TempIddDisplay);
      else if (TempIddDisplay < 100)
        sprintf(DisplayString, "%.1f mA", TempIddDisplay);
      else if (TempIddDisplay < 1000)
        sprintf(DisplayString, "%.0f mA", TempIddDisplay);
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


