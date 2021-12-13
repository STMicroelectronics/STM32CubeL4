/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @brief   This file provides main program functions
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
#include "k_demo.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"

#include "main_app.h" 
#include "iddmeasure.h" 
#include "audioplayer.h" 
#include "audiorecorder.h"
#include "compass.h"
#include "soundmeter.h"
#include "batterymeasure.h"
#include "guitartuner.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup MAIN
* @brief main file
* @{
*/ 

DemoBackupData_TypeDef demoBackupData;
DemoBackupData_TypeDef *demoBackupDataAreaPtr = (DemoBackupData_TypeDef *)DEMOBACKUP_AREA_ADDRESS;

/** @defgroup MAIN_Private_TypesDefinitions
* @{
*/ 
/**
* @}
*/ 

/** @defgroup MAIN_Private_Defines
* @{
*/
#define MIN_IDLE_TIME_BATTERY           4000    /* Time in ms to automatically enter low power in Battery mode */
#define MIN_IDLE_TIME_USB               10000   /* Time in ms to automatically enter low power in USB-powered mode */
/**
* @}
*/ 


/** @defgroup MAIN_Private_Macros
* @{
*/ 
/**
* @}
*/ 


/** @defgroup MAIN_Private_Variables
* @{
*/
static uint8_t DemoStartupString[] = "      STM32L476G-DISCO DEMO";

__IO FlagStatus DemoStarted = RESET;             /* Default state at startup */
FlagStatus DemoEnterLowPower = RESET;            /* Default state at startup */
MODULES_INFO DemoSelectedModuleId = MODULE_NONE;  /* No selected module at startup */
osMutexId DemoLowPowerMutex;                     /* Low power resources mutex */
ULPMode DemoULPMode = ULP_STOP2;                 /* Default low power policy  */
void (*EnterLowPowerMode)(void) = NULL;          /* Low power entry function  */
void (*ExitLowPowerMode)(void) = NULL;           /* Low power exit function   */
uint32_t DemoMinIdleTime = 0;                    /* Inactivity timing value   */

FlagStatus DemoDebugMode = RESET;        /* RESET means debug not available via SWDIO/SWCLK/SWO */

SupplyMode_TypeDef PowerSupplyMode = SUPPLY_MODE_ERROR;

/* HW resources initialization variables */
__IO FlagStatus KeyPressed = RESET;
FlagStatus JoyInitialized = RESET;
FlagStatus IddInitialized = RESET;
FlagStatus LcdInitialized = RESET;
FlagStatus LedInitialized = RESET;

/* Flash storage variables */
__IO FlagStatus FlashInit  = RESET ;
uint32_t address = DEMOBACKUP_AREA_ADDRESS;
int64_t  WrBuffer16[4];
uint64_t WrBuffer64;

/**
* @}
*/ 

extern __IO uint8_t bLCDGlass_KeyPressed;
extern __IO FlagStatus IddItOccurred;

/** @defgroup MAIN_Private_FunctionPrototypes
* @{
*/
static void StartThread(void const * argument);
static void SystemBackupInit(void);

extern void CompassInit(void);
extern void CompassDeInit(void);
extern void GuitarTunerPreExec(void);
extern void GuitarTunerPostExec(void);
/**
* @}
*/ 

/** @defgroup MAIN_Private_Functions
* @{
*/ 


/**
* @brief  Main program
* @param  None
* @retval int
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

  /*  Check power supply resource */
  PowerSupplyMode = BSP_SupplyModeDetection();

  /* Configure the system clock according to power supply */  
  if( PowerSupplyMode != SUPPLY_MODE_BATTERY)
  {
    /* 80 MHz PLL ON */
    SystemClock_Config();
    DemoMinIdleTime = MIN_IDLE_TIME_USB;
  }
  else
  {
    /* 2 MHz Battery power mode */
    SystemClock_BatterySupply_Config();
    DemoMinIdleTime = MIN_IDLE_TIME_BATTERY;
  }
  
  /* Create Start task */
  osThreadDef(Demo_Thread, StartThread, osPriorityNormal, 0, 2 * configMINIMAL_STACK_SIZE);
  osThreadCreate (osThread(Demo_Thread), NULL);
  
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  for( ;; );
}

/**
  * @brief  Start task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void StartThread(void const * argument)
{
  osMutexDef_t mutex_lowpower;

  /* Create mutex to handle low power mode */
  DemoLowPowerMutex = osRecursiveMutexCreate(&mutex_lowpower);
  
  /* Start Demo */
  kDemo_Start();
}

void kDemo_Initialization(void)
{
  SettingsBackupData_TypeDef settings;
  
  /* Configure minimum hardware resources at boot */
  SystemHardwareInit(HWINIT_STARTUP);

  /* Init backup area */
  SystemBackupInit();
  SystemBackupRead(BACKUP_SETTINGS, (void *)&settings);
  DemoULPMode = (ULPMode) settings.lpmode;
  
  if ((DemoULPMode != ULP_STOP2) && 
      (DemoULPMode != ULP_STANDBY) &&
      (DemoULPMode != ULP_OFF))
  {
    DemoULPMode = ULP_STOP2;
  }

  /* If Supply is Battery, discard no low power mode configuration to save consumption */
  if((PowerSupplyMode == SUPPLY_MODE_BATTERY) && (DemoULPMode == ULP_OFF))
  {
    DemoULPMode = ULP_STOP2;
  }
  
  if(DemoULPMode == ULP_STOP2)
  {
    EnterLowPowerMode = EnterStop2Mode;
    ExitLowPowerMode = ExitStop2Mode;
  }
  else if(DemoULPMode == ULP_STANDBY)
  {
    EnterLowPowerMode = EnterStandbyMode;
    ExitLowPowerMode = NULL;
  }
  else
  {
    EnterLowPowerMode = NULL;
    ExitLowPowerMode = NULL;
  }
  
  /* Check system startup status */
  if(SystemStartupStatus() != 0)
  {
    /* Display the demonstration window */
    BSP_LCD_GLASS_ScrollSentence(DemoStartupString, 1, SCROLL_SPEED_HIGH);
    /* Clear LCD Display */
    BSP_LCD_GLASS_Clear();
  }
  
  /* Module Initialization */
  kModule_Init();
  
  /* Control the resources */
  if(kModule_CheckResource() != KMODULE_OK)
  {
    Error_Handler();
  }
  
  /* Put all Components in low power mode */
  SystemComponentsLowPower();
  
}

void kModule_Init(void)
{
  kModule_Add(MODULE_MAIN_APP, ModuleAppMain);
  kModule_Add(MODULE_IDDMEASURE, ModuleIddMeasure);
  kModule_Add(MODULE_BATTERYMEASURE, ModuleBatteryMeasure);
  kModule_Add(MODULE_COMPASS, ModuleCompass);
#if defined (__ICCARM__)    /* Audio library only available for IAR */
  kModule_Add(MODULE_SOUNDMETER, ModuleSoundMeter);
#endif /* __ICCARM__ */
  
  if(PowerSupplyMode != SUPPLY_MODE_BATTERY)
  {
    kModule_Add(MODULE_AUDIOPLAYER, ModuleAudioPlayer);
    kModule_Add(MODULE_AUDIORECORDER, ModuleAudioRecorder);
#if defined (__ICCARM__)    /* Audio library only available for IAR */
    kModule_Add(MODULE_GUITARTUNER, ModuleGuitarTuner);
#endif /* __ICCARM__ */
  }
}
void kDemo_UnInitialization(void)
{
  /* Nothing to do */
}

/**
  * @brief  System Clock Configuration on USB/ST-Link
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
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
void SystemClock_Config(void)
{
  /* oscillator and clocks configs */
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  if(HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Disable Power Control clock */
  __HAL_RCC_PWR_CLK_DISABLE();

  /* 80 Mhz from PLL with MSI 8Mhz as source clock */
  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_7;   /* 8 Mhz */
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
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
  * @brief  System Clock Configuration on Battery
  *         The system Clock is configured as follow : 
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 2000000
  *            HCLK(Hz)                       = 2000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 2000000
  *            Flash Latency(WS)              = 0
  * @param  None
  * @retval None
  */
void SystemClock_BatterySupply_Config(void)
{
  /* oscillator and clocks configs */
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI 2Mhz as source clock for Low Power Run mode*/
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
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  if(HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Enter Low Power Run mode */
   HAL_PWREx_EnableLowPowerRunMode();

  /* Disable Power Control clock */
  __HAL_RCC_PWR_CLK_DISABLE();
}

/**
  * @brief  System Clock Speed decrease when power is battery supplied
  *         Turn PLL off and set the system clock source MSI to range 5 (2 MHz)
  *         (system clock speed is reduced from 20 to 2 MHz)  
  * @note  API is called once SoundMeter test is done 
  * @note  API is called only when the device power is battery-supplied         
  * @param  None
  * @retval None
  */
void SystemClock_BatterySupply_ClockDecrease(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Turn off PLL and set the MSI to range 5 (2Mhz) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5; /* back to 2 MHz */
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  HAL_PWREx_EnableLowPowerRunMode();
   
  /* Disable Power Control clock once PWR register is updated */
  __HAL_RCC_PWR_CLK_DISABLE();
}


/**
  * @brief  System Clock Speed increase when power is battery supplied
  *         Set MSI to range 8 (16 MHz) and turn on PLL to apply PLLM divider 2
  *         (system clock speed is increased from 2 to 16 MHz)  
  * @note  API is called before starting SoundMeter tests 
  * @note  API is called only when the device power is battery-supplied         
  * @param  None
  * @retval None
  */  
void SystemClock_BatterySupply_ClockIncrease(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  HAL_PWREx_DisableLowPowerRunMode();
  
  /* Disable Power Control clock once PWR register is updated */
  __HAL_RCC_PWR_CLK_DISABLE();  

  /* MSI is enabled in range 8 (16 Mhz) with PLLM divider 2 from PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_8; /* to 16 MHz */
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}


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
void SystemHardwareInit(HwInitMode Mode)
{
  if(Mode == HWINIT_ERROR)
  {
    Error_Handler();
  }
  
  /* Initialize LED 3 and LED 4  */
  if(LedInitialized != SET)
  {
    BSP_LED_Init(LED_RED);
    BSP_LED_Init(LED_GREEN);
    LedInitialized = SET;
  }

  /* STOP2 Low power mode:  Need to keep LCD and joystick keys active */
  /*                        Else reactivate all consuming resources   */
  if(Mode != HWINIT_STOP2)
  {
    /* Initialize the joystick in interrupt mode */
    if(JoyInitialized != SET)
    {
      BSP_JOY_Init(JOY_MODE_EXTI);
      JoyInitialized = SET;
    }
    
    /* Initialize the LCD */
    if(LcdInitialized != SET)
    {
      BSP_LCD_GLASS_Init();
      LcdInitialized = SET;
    }
    
    /* Return from IDD measurement */
    if(Mode == HWINIT_IDD)
    {
      GPIO_InitTypeDef GPIO_InitStruct;

      /*##-1- Configure the GPIOs for MFX ######################################*/
      /* Enable GPIO clock */
      DISCOVERY_I2C2_SDA_GPIO_CLK_ENABLE();
      DISCOVERY_I2C2_SCL_GPIO_CLK_ENABLE();

      /* Configure I2C Rx/Tx as alternate function  */
      GPIO_InitStruct.Pin       = DISCOVERY_I2C2_SCL_PIN | DISCOVERY_I2C2_SDA_PIN;
      GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
      GPIO_InitStruct.Pull      = GPIO_PULLUP;
      GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStruct.Alternate = DISCOVERY_I2C2_SCL_SDA_AF;
      HAL_GPIO_Init(DISCOVERY_I2C2_SCL_GPIO_PORT, &GPIO_InitStruct);

      /*##-2- Configure the Discovery I2C2 peripheral for MFX ##################*/
      /* Enable Discovery_I2C2 clock */
      DISCOVERY_I2C2_CLK_ENABLE();

      /* Enable PWR clock */
      __HAL_RCC_PWR_CLK_ENABLE();
    }
  }
  else
  {
    /* Enable PWR clock */
    __HAL_RCC_PWR_CLK_ENABLE();
    
    /* Enable I2C2 clock */
    __HAL_RCC_I2C2_CLK_ENABLE();

    /* Re-initialize Module */
    if((DemoSelectedModuleId != MODULE_MAIN_APP) && (DemoSelectedModuleId != MODULE_NONE))
    {
      if(kModule_ExitLowPower(DemoSelectedModuleId) != KMODULE_OK)
      {
        Error_Handler();
      }
    }
  }
}

/**
  * @brief  System Low Power Configuration
  * @param  Mode: Purpose of HW deinitialization 
   *              (HWINIT_ERROR/HWINIT_STARTUP/HWINIT_IDD/HWINIT_STOP2/HWINIT_STANDBY)
  
  * @retval None
  */
void SystemHardwareDeInit(HwInitMode Mode)
{
  RCC_OscInitTypeDef oscinitstruct = {0};

  if(Mode == HWINIT_ERROR)
  {
    Error_Handler();
  }
  
  if(LedInitialized != RESET)
  {
    BSP_LED_DeInit(LED_RED);
    BSP_LED_DeInit(LED_GREEN);
    LedInitialized = RESET;
  }           
  /* Enable PWR  peripheral Clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_DisablePullUpPullDownConfig();
  
  /* STOP2 Low power mode:  Need to keep LCD and joystick keys active */
  /*                        Else deactivate all consuming resources   */
  if(Mode != HWINIT_STOP2)
  {
    if(JoyInitialized != RESET)
    {
      BSP_JOY_DeInit();
      JoyInitialized = RESET;
    }

    if(LcdInitialized != RESET)
    {
      BSP_LCD_GLASS_DeInit();
      LcdInitialized = RESET;
    }

    /* LSE off in all modes */
    oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    oscinitstruct.PLL.PLLState   = RCC_PLL_NONE;
    oscinitstruct.LSEState       = RCC_LSE_OFF;
    if(HAL_RCC_OscConfig(&oscinitstruct) != HAL_OK)
    {
      Error_Handler();
    }

    HAL_PWR_DisableBkUpAccess();

    /* Purpose of HW deinitialization is: IDD measurement */
    if(Mode == HWINIT_IDD)
    {
      /* Configure MFX I2C Rx/Tx as Analog floating during IDD */
      HAL_GPIO_DeInit(DISCOVERY_I2C2_SCL_GPIO_PORT, (DISCOVERY_I2C2_SCL_PIN | DISCOVERY_I2C2_SDA_PIN)); /* No STOP2 after FW upgrade */
    } 
  }
  else
  {
    /* Low power reason: HWINIT_STOP2 */
    /* De-initialize Module */
    if((DemoSelectedModuleId != MODULE_MAIN_APP) && (DemoSelectedModuleId != MODULE_NONE))
    {
      if(kModule_EnterLowPower(DemoSelectedModuleId) != KMODULE_OK)
      {
        Error_Handler();
      }
    }
  }
}

/**
  * @brief  check necessary system flags at boot and apply necessary code
  * @param  None
  * @retval Value telling if we need to display welcome message
  */
uint8_t SystemStartupStatus(void)
{
  uint8_t               displaydemo = 0;
  IddBackupData_TypeDef backup;

  if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET)
  {
    /* Clear reset flags */
    __HAL_RCC_CLEAR_RESET_FLAGS();

    /* return from error, do not display demo info */
    displaydemo = 1;
  }
  else
  {
    /* Check if software reset has been set to handle debugger cases */
    if(__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST) != RESET)
    {
      displaydemo = 1;
    }
    else
    {
      /* Read IDD backup data to check is we return from Idd measurement test */
      SystemBackupRead(BACKUP_IDD, (void *)&backup);

      /* Check if the system was resumed from Shutdown mode or real BOR reset */
      if(__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST) != RESET)
      {
        if(backup.mode == IDD_SHUTDOWN)
        {
          IddMeasureInit();
          IddMeasureGetAndDisplayValue();
          backup.mode = IDD_RUN;
          SystemBackupWrite(BACKUP_IDD, (void *)&backup);
          BSP_LCD_GLASS_Clear();
        }
        else
        {
          displaydemo = 1;
        }
      }
      else
      {
        /* Check if the system  was resumed from StandBy mode */
        if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
        {
          /* Clear the Standby flag */
          __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

          /* Check and Clear the Wakeup flag */
          if(__HAL_PWR_GET_FLAG(PWR_FLAG_WUF1) != RESET)
          {
            __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF1);
          }
          if(__HAL_PWR_GET_FLAG(PWR_FLAG_WUF2) != RESET)
          {
            __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
          }
          
          /* Read backup register 0 to check is we return from Idd measurement test */
          if(backup.mode == IDD_STANDBY)
          {
            IddMeasureInit();
            IddMeasureGetAndDisplayValue();
            backup.mode = IDD_RUN;
            SystemBackupWrite(BACKUP_IDD, (void *)&backup);
            BSP_LCD_GLASS_Clear();
          }
        }
        else
        {
          displaydemo = 1;
        }
      }
    }
  }
  
  /* Clear the reset flags */
  __HAL_RCC_CLEAR_RESET_FLAGS();

  /* Return status to display or not welcome message */
  return displaydemo;
}

/**
  * @brief  Put all board components in low power mode
  * @param  None
  * @retval None
  */
void SystemComponentsLowPower(void)
{   
     
  GPIO_InitTypeDef GPIO_InitStruct;
  DFSDM_Channel_HandleTypeDef hDfsdmLeftChannel;

  /*************************************/
  /* Set USB OTG I/O at default values */
  /*************************************/  
  __HAL_RCC_GPIOA_CLK_ENABLE();   
  __HAL_RCC_GPIOB_CLK_ENABLE();   
  __HAL_RCC_GPIOC_CLK_ENABLE();

  if(DemoDebugMode == RESET)
  {
    /* SWDIO / SWCLK not available for debug (Analog floating) */
    HAL_GPIO_DeInit(GPIOA, (GPIO_PIN_13 | GPIO_PIN_14));
    /* SWO not available for debug (Analog floating) */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3);
  }

  /* OTG_PSO / OTG_OC / OTG_ID in Analog floating */
  HAL_GPIO_DeInit(GPIOC, (GPIO_PIN_9  | GPIO_PIN_10 |GPIO_PIN_12));

  /* OTG_VBUS in Output no pull, set to Low */
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP  ;
  GPIO_InitStruct.Pin       = GPIO_PIN_11;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW; 
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET); 

  /* OTG_DM / OTG_DP in Analog floating */
  HAL_GPIO_DeInit(GPIOA, (GPIO_PIN_11  | GPIO_PIN_12));
        
  /*********************************************/
  /* Set gyroscope component in low power mode */
  /*********************************************/ 
  BSP_GYRO_Init();
  BSP_GYRO_LowPower();
  BSP_GYRO_DeInit();
  
  /*********************************************/
  /* Set compass component in low power mode   */
  /*********************************************/ 
  BSP_COMPASS_Init();
  BSP_COMPASS_LowPower();
  BSP_COMPASS_DeInit();
  
  /**************************************************************************/
  /* Specific Low Power mode settings when device power is battery-supplied */
  /**************************************************************************/     
  if( PowerSupplyMode == SUPPLY_MODE_BATTERY)
  {
    /***********************************************************************/
    /* In case of battery-supplied powered, there is no audio codec-based 
       features available. Set audio codec I/O default setting */
    /***********************************************************************/ 
    AUDIO_IO_Init();
    AUDIO_IO_DeInit();
     
    /**********************************************************************/
    /* QUADSPI interface de-initialization for power consumption purposes */
    /**********************************************************************/   
    BSP_QSPI_Init();
    BSP_QSPI_DeInit();  
  
    /**********************************************************************/
    /* Digital filters for sigma-delta modulators (DFSDM) pins            */
    /* de-initialization for power consumption purposes                   */
    /**********************************************************************/   
    HAL_DFSDM_ChannelMspInit(&hDfsdmLeftChannel);
    HAL_DFSDM_ChannelMspDeInit(&hDfsdmLeftChannel);
  }
}

/**
  * @brief  Initialize Backup area
  * @param  None
  * @retval None
  */
static void SystemBackupInit(void)
{
  /* Initialize Flash access (used to backup data) */
  if(FlashInit == RESET)
  {
    /* Initialize Flash */
    /* Temp patch (due to OPTVERR bit set on virgin samples) */ 
    if(__HAL_RCC_GET_FLAG(RCC_FLAG_OBLRST) != RCC_FLAG_OBLRST)
    {
      __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR); 
    }
    
    /* Unlock the User Flash area */
    HAL_FLASH_Unlock();

    FlashInit = SET;
  }
  
  /* Initialize local copy of backup area */
  SystemBackupRead(BACKUP_IDD, (void *)&demoBackupData.idd);
  SystemBackupRead(BACKUP_COMPASS, (void *)&demoBackupData.compass);
  SystemBackupRead(BACKUP_SETTINGS, (void *)&demoBackupData.settings);
}

/**
  * @brief  Read from Backup area
  * @param  None
  * @retval None
  */
uint32_t SystemBackupRead(DemoBackupId Id, void *Data)
{
  
  switch(Id)
  {
  case BACKUP_IDD:
    {
      IddBackupData_TypeDef *Idd;
      
      Idd = (IddBackupData_TypeDef *)Data;
      Idd->mode = demoBackupDataAreaPtr->idd.mode;
    }
    break;

  case BACKUP_COMPASS:
    {
      CompassBackupData_TypeDef *Compass;
      
      Compass = (CompassBackupData_TypeDef *)Data;
      Compass->calibration = demoBackupDataAreaPtr->compass.calibration;
      Compass->xmax = demoBackupDataAreaPtr->compass.xmax;
      Compass->xmin = demoBackupDataAreaPtr->compass.xmin;
      Compass->ymax = demoBackupDataAreaPtr->compass.ymax;
      Compass->ymin = demoBackupDataAreaPtr->compass.ymin;
      Compass->zmax = demoBackupDataAreaPtr->compass.zmax;
      Compass->zmin = demoBackupDataAreaPtr->compass.zmin;
    }
    break;

    case BACKUP_SETTINGS:
    {
      SettingsBackupData_TypeDef *Settings;
      
      Settings = (SettingsBackupData_TypeDef *)Data;
      Settings->lpmode = demoBackupDataAreaPtr->settings.lpmode;
    }
    break;
    
  default:
    break;
  }
  
  return 1;
}

/**
  * @brief  Write to Backup area
  * @param  Id: index of backup data
  * @param  Data: pointer to data buffer to backup
  * @retval None
  */
void SystemBackupWrite(DemoBackupId Id, void *Data)
{
  FLASH_EraseInitTypeDef FLASH_EraseInitStruct;
  uint32_t  PageError = 0;
  uint16_t* internalBuffer;
  uint32_t  index = 0;
  
  /* Always write to first sector of bank2 */
  address = DEMOBACKUP_AREA_ADDRESS;
  
  switch(Id)
  {
  case BACKUP_IDD:
    demoBackupData.idd.mode = ((IddBackupData_TypeDef *)Data)->mode;
    break;
  case BACKUP_COMPASS:
    demoBackupData.compass.calibration = ((CompassBackupData_TypeDef *)Data)->calibration;
    demoBackupData.compass.xmax = ((CompassBackupData_TypeDef *)Data)->xmax;
    demoBackupData.compass.xmin = ((CompassBackupData_TypeDef *)Data)->xmin;
    demoBackupData.compass.ymax = ((CompassBackupData_TypeDef *)Data)->ymax;
    demoBackupData.compass.ymin = ((CompassBackupData_TypeDef *)Data)->ymin;
    demoBackupData.compass.zmax = ((CompassBackupData_TypeDef *)Data)->zmax;
    demoBackupData.compass.zmin = ((CompassBackupData_TypeDef *)Data)->zmin;
    break;
  case BACKUP_SETTINGS:
    demoBackupData.settings.lpmode = ((SettingsBackupData_TypeDef *)Data)->lpmode;
    break;
  default:
    break;
  }
  
  FLASH_EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  FLASH_EraseInitStruct.Banks     = FLASH_BANK_2;
  FLASH_EraseInitStruct.Page      = 0;
  FLASH_EraseInitStruct.NbPages   = 1;
  while(HAL_FLASHEx_Erase(&FLASH_EraseInitStruct, &PageError) != HAL_OK)
  {
   //  Error_Handler();
  }
  
  internalBuffer = (uint16_t *)&demoBackupData;
  
  for (index = 0; index < sizeof(DemoBackupData_TypeDef); index+=4)
  {
    WrBuffer16[0] = (uint16_t)(internalBuffer[index]);
    WrBuffer16[1] = (uint16_t)(internalBuffer[index+1]);
    WrBuffer16[2] = (uint16_t)(internalBuffer[index+2]);
    WrBuffer16[3] = (uint16_t)(internalBuffer[index+3]);
    WrBuffer64 = (uint64_t)(((WrBuffer16[3])<<48) | ((WrBuffer16[2])<<32) | ((WrBuffer16[1])<<16) | WrBuffer16[0]);
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, WrBuffer64) == HAL_OK)
    {
      address = address + 8;
    }
    else
    {
      Error_Handler();
    }
   }
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
  * @retval None
  */
void SystemPeripheralClockDisable(void)
{
  /* Disable remaining clocks */
  __HAL_RCC_FLASH_CLK_DISABLE();
  __HAL_RCC_SYSCFG_CLK_DISABLE();
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_I2C2_CLK_DISABLE();
  __HAL_RCC_SPI2_CLK_DISABLE();
  __HAL_RCC_DFSDM1_CLK_DISABLE();
  __HAL_RCC_DMA1_CLK_DISABLE();
  __HAL_RCC_DMA2_CLK_DISABLE();
  __HAL_RCC_TIM6_CLK_DISABLE();
  
  RCC->AHB1SMENR  = 0x0;
  RCC->AHB2SMENR  = 0x0;
  RCC->AHB3SMENR  = 0x0;
  RCC->APB1SMENR1 = 0x0;
  RCC->APB1SMENR2 = 0x0;
  RCC->APB2SMENR  = 0x0;
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if((GPIO_Pin & (DOWN_JOY_PIN | UP_JOY_PIN | SEL_JOY_PIN | RIGHT_JOY_PIN | LEFT_JOY_PIN))
      != RESET)
  {
    KeyPressed = SET;
    bLCDGlass_KeyPressed = 0x01;

    /* Treat push button if demo has been started */
    if(DemoStarted !=  RESET)
    {
      kMenu_SendEvent(GPIO_Pin);
    }
  }
  else if((GPIO_Pin & IDD_INT_PIN) != RESET)
  {
    IddItOccurred = SET;
  }
}


/**
  * @brief converts a 32bit unsigned int into ASCII 
  * @caller several callers for display values
  * @param Number digit to displays
  *  p_tab values in array in ASCII   
  * @retval None
  */ 
void Convert_IntegerIntoChar(uint32_t number, uint16_t *p_tab)
{
  uint16_t units=0, tens=0, hundreds=0, thousands=0, tenthousand=0, hundredthousand=0;
  
  units = ((((number%100000)%10000)%1000)%100)%10;
  tens = (((((number-units)/10)%10000)%1000)%100)%10;
  hundreds = ((((number-tens-units)/100)%1000)%100)%10;
  thousands = (((number-hundreds-tens-units)/1000)%100)%10;
  tenthousand = ((number-thousands-hundreds-tens-units)/10000)%10;
  hundredthousand = ((number-tenthousand-thousands-hundreds-tens-units)/100000);
  
  *(p_tab+5) = units + 0x30;
  *(p_tab+4) = tens + 0x30;
  *(p_tab+3) = hundreds + 0x30;
  *(p_tab+2) = thousands + 0x30;
  *(p_tab+1) = tenthousand + 0x30;
  *(p_tab+0) = hundredthousand + 0x30;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  if(DemoDebugMode != RESET)
  {
    /* Init Error Led Red */
    BSP_LED_Init(LED_RED);

    /* Turn Led Red on */
    BSP_LED_On(LED_RED);

    while(1);
  }
  else
  {
    IWDG_HandleTypeDef hiwdg;
    RCC_OscInitTypeDef rcc_oscinitstruct;

    /* FW automatic restart based on Internal Watchdog */
    
    /* RCC LSI clock enable */
    rcc_oscinitstruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI;
    rcc_oscinitstruct.PLL.PLLState = RCC_PLL_NONE;
    rcc_oscinitstruct.LSIState = RCC_LSI_ON;
    if(HAL_RCC_OscConfig(&rcc_oscinitstruct) != HAL_OK)
    {
      /* Init Error Led Red */
      BSP_LED_Init(LED_RED);

      /* Turn Led Red on */
      BSP_LED_On(LED_RED);

      while(1);
    }
    
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
    hiwdg.Init.Reload = 0;
    hiwdg.Init.Window = IWDG_WINDOW_DISABLE;

    HAL_IWDG_Init(&hiwdg);

    while(1);
  }
  
}

/**
  * @brief  This function is executed to enter MCU in STOP2 mode
  * @param  None
  * @retval None
  */
void EnterStop2Mode(void)
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

  /* System peripheral clock disable */
  SystemPeripheralClockDisable();
  
  /* Enter in Low power Stop 2 */
  HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
}

/**
  * @brief  This function is executed when MCU exits from STOP2 mode
  * @param  None
  * @retval None
  */
void ExitStop2Mode(void)
{
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

  /* Disable Power Control clock */
  __HAL_RCC_PWR_CLK_DISABLE();

  if (PowerSupplyMode != SUPPLY_MODE_BATTERY)
  {
    /* Restore clock to default frequency when not running on battery */
    SystemClock_Config();
  }
  else
  {
    /* Restore clock to default frequency when running on battery */
    SystemClock_BatterySupply_Config();
  }
}

/**
  * @brief  This function is executed to enter MCU in STANDBY mode
  * @param  None
  * @retval None
  */
void EnterStandbyMode(void)
{
#if (PREFETCH_ENABLE != 0)
  /* Disable Prefetch Buffer */
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
#endif /* PREFETCH_ENABLE */

  /* Enable PWR clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  
  /* Configure the relevant I/O in pull-up or pull-down mode
     for power consumption purposes */
  /* Port GPIOA */
  /* Port GPIOB */
  /* Port GPIOC */
  HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_C, PWR_GPIO_BIT_0);
  /* Port GPIOD */
  HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_D, PWR_GPIO_BIT_7);
  HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_D, (PWR_GPIO_BIT_1 | PWR_GPIO_BIT_4));
  /* Port GPIOE */
  HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_E, (PWR_GPIO_BIT_0 | PWR_GPIO_BIT_9));
  HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_E, (PWR_GPIO_BIT_2 | PWR_GPIO_BIT_3 | 
                              PWR_GPIO_BIT_4 | PWR_GPIO_BIT_5 | PWR_GPIO_BIT_6));

  /* Enable configuration */
  HAL_PWREx_EnablePullUpPullDownConfig();
  
  /* Enable Wake pins 1: press SEL joystick button to wake up */
  HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
  
  /* Clear wake up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF1);
  
  /* Enable wakeup pin WKUP1 */
  HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1_HIGH);
  
  /* Request to enter STANDBY mode */
  HAL_PWR_EnterSTANDBYMode();
}

/* FreeRTOS hooks */
/**
  * @brief  Application Idle Hook
  * @param  None 
  * @retval None
  */
void vApplicationIdleHook(void) 
{
  HwInitMode hwmode = HWINIT_ERROR;  
  
  if((DemoEnterLowPower != RESET) && (DemoULPMode != ULP_OFF))
  {
    /* Get Mutex */
     osRecursiveMutexWait(DemoLowPowerMutex, 0xFFFFFFFF);

    /* Check which type of low power to be entered */
    if(DemoULPMode == ULP_STOP2)
    {
      hwmode = HWINIT_STOP2;
    }
    else if(DemoULPMode == ULP_STANDBY)
    {
      hwmode = HWINIT_STANDBY;
    }

    /* DeInit HW resources for low consumption*/
    SystemHardwareDeInit(hwmode);

    if(EnterLowPowerMode != NULL)
    {
      (EnterLowPowerMode)();
    }

    if(ExitLowPowerMode != NULL)
    {
      (ExitLowPowerMode)();
    }

    /* ReInit HW resources upon exit from low power mode */
    SystemHardwareInit(hwmode);

    /* Reset low power variable */
    DemoEnterLowPower = RESET;

    /* Release Mutex to allow event action */
    osRecursiveMutexRelease(DemoLowPowerMutex);
  }
}

/**
  * @brief  Application Malloc failure Hook
  * @param  None 
  * @retval None
  */
void vApplicationMallocFailedHook(void)
{
  Error_Handler();
}

#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
  number,ex: printf("Wrong parameters value: file %s on line %d\r\n", 
  file, line) */
  
  /* Infinite loop */
  while (1)
  {}
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

