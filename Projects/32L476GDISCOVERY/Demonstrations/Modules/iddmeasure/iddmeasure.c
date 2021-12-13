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
#include "k_config.h"
#include "k_menu.h"
#include "k_module.h"
#include "iddmeasure.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  void   (*IddEnter)(void);
  void   (*IddRestore)(void);
  uint8_t IddName[20];
  uint32_t IddIndex;
}Idd_AppliTypedef;
/* Private defines -----------------------------------------------------------*/
#define UNIT_CHAR_NB                              2

/* Private function prototypes -----------------------------------------------*/
/* Idd Module functions */
KMODULE_RETURN IddMeasureExec(void);
KMODULE_RETURN IddMeasureDeInit(void);
KMODULE_RETURN IddMeasureExecCheckResource(void);

/* Idd items functions */
static void IddMeasure(uint8_t Index);
static void IddMeasureRun(void);
static void IddMeasureSleep(void);
static void IddMeasureLpRun(void);
static void IddMeasureLpSleep(void);
static void IddMeasureStop2(void);
static void IddMeasureStandby(void);
static void IddMeasureShutdown(void);

/* Idd private sub-functions */
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
static void Idd_Convert(uint32_t Value, uint16_t *DisplayString);

static void Idd_BatterySupply_ClockIncrease(void);
static void Idd_BatterySupply_ClockDecrease(void);

static void Idd_ExternalSupply_ClockDecrease(void);
static void Idd_ExternalSupply_ClockIncrease(void);

/* Private Variable ----------------------------------------------------------*/
const tMenuItem IddMeasureMenuItems[] =
{
    {"   RUN"             , 0, 0,   SEL_EXEC, 0, IddMeasureRun, NULL, NULL, 0},
    {" SLEEP"             , 0, 0,   SEL_EXEC, 0, IddMeasureSleep, NULL, NULL, 0},
    {"LP RUN"             , 0, 0,   SEL_EXEC, 0, IddMeasureLpRun, NULL, NULL, 0},
    {"LP SLP"             , 0, 0,   SEL_EXEC, 0, IddMeasureLpSleep, NULL, NULL, 0},
    {" STOP2"             , 0, 0,   SEL_EXEC, 0, IddMeasureStop2, NULL, NULL, 0},
    {" STDBY"             , 0, 0,   SEL_EXEC, 0, IddMeasureStandby, NULL, NULL, 0},
    {"SHTDWN"             , 0, 0,   SEL_EXEC, 0, IddMeasureShutdown, NULL, NULL, 0},
    {""                   , 0, 0,   SEL_EXIT, 0, NULL, NULL, NULL, NULL}
};

const tMenu IddMeasureMenu = {
  "", IddMeasureMenuItems, countof(IddMeasureMenuItems), TYPE_TEXT, 1, 0
  };

__IO FlagStatus IddItOccurred = RESET;
uint32_t MfxFwVersion = 0;
uint32_t IddReadValue = 0;
IddBackupData_TypeDef IddBackup;

Idd_AppliTypedef IddTest[]=
{
  {NULL, NULL, "RUN", IDD_RUN},
  {Idd_SleepEnter, Idd_SleepRestore, "SLEEP", IDD_SLEEP},
  {Idd_LprEnter, Idd_LprRestore,"LOW POWER 2MHZ", IDD_LPR_2MHZ},
  {Idd_LprSleepEnter, Idd_LprSleepRestore,"LPR SLEEP", IDD_LPR_SLEEP},
  {Idd_StopEnter, Idd_StopRestore, "STOP2", IDD_STOP2},
  {Idd_StandbyEnter, NULL,"STANDBY", IDD_STANDBY},
  {Idd_ShutdownEnter, NULL,"SHUTDOWN", IDD_SHUTDOWN},
};

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
extern FlagStatus IddInitialized;
const K_ModuleItem_Typedef ModuleIddMeasure =
{
  MODULE_IDDMEASURE,
  IddMeasureInit,
  IddMeasureExec,
  IddMeasureDeInit,
  IddMeasureExecCheckResource
};

/**
  * @brief  Initialize the Idd Measurement application 
  * @param  None.
  * @note   None.
  * @retval None.
  */
KMODULE_RETURN IddMeasureInit(void)
{
  /* read IDD backup data to check is we return from Idd measurement test */
  SystemBackupRead(BACKUP_IDD, (void *)&IddBackup);
  
  /* Initialize Idd measurement component */
  if(IddInitialized != SET)
  {
    if(BSP_IDD_Init() != IDD_OK)
    {
      Error_Handler();
    }
    IddInitialized = SET;
  }

  /* Read FW version */
  MfxFwVersion = mfxstm32l152_ReadFwVersion(IDD_I2C_ADDRESS);

  /* Enable Interrupt to get end of measurement */
  /* This will wahe up MCU from low power mode */
  BSP_IDD_EnableIT();
  BSP_IDD_ErrorEnableIT();

  return KMODULE_OK;
}

/**
  * @brief  Run the Idd Measurement application 
  * @param  None.
  * @note   run and display Idd Menu.  
  * @retval None.
  */
KMODULE_RETURN IddMeasureExec(void)
{
  kMenu_Execute(IddMeasureMenu);
  return KMODULE_OK;
}

/**
  * @brief  DeInitialize the Idd Measurement application 
  * @param  None.
  * @note   None.
  * @retval None.
  */
KMODULE_RETURN IddMeasureDeInit(void)
{
  /* Enters MFX in standby mode */
  BSP_IDD_LowPower();
  
  BSP_IDD_DeInit();
  IddInitialized = RESET;

  return KMODULE_OK;
}
/**
  * @brief  check the Idd Measure application resources 
  * @param  None.
  * @note   None.  
  * @retval None.
  */
KMODULE_RETURN IddMeasureExecCheckResource(void)
{
  return KMODULE_OK;
}

/**
  * @brief  Idd current measurement
  * @param  None.
  * @note   None.  
  * @retval None.
  */
static void IddMeasure(uint8_t Index)
{
  IDD_StatusTypeDef errorcode = IDD_ERROR;
  uint8_t retrynb = 0;
  uint8_t shuntused = MFXSTM32L152_IDD_SHUNT_NB_4;

  /* Store Idd Index in case of unwanted reboot during Standby or Shutdown test */
  IddBackup.mode = Index;
  SystemBackupWrite(BACKUP_IDD, (void *)&IddBackup);
  
  while((errorcode != IDD_OK) && (retrynb < 5))
  {
    /* Reset measurement done global variable */
    IddItOccurred = RESET;

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

    /* un configure HW resources */
    SystemHardwareDeInit(HWINIT_IDD);

    /* if function pointer exists, execute correponsing low power action */
    if(IddTest[Index].IddEnter != NULL)
    {
      IddTest[Index].IddEnter();
    }
    
    while(IddItOccurred == RESET);

    /* Reset measurement done global variable */
    IddItOccurred = RESET;

    /* if function pointer exists, restore context clocks and power context */
    if(IddTest[Index].IddRestore != NULL)
    {
      IddTest[Index].IddRestore();
    }

    /* configure HW resources */
    SystemHardwareInit(HWINIT_IDD);

    /* Get Idd Measured value and display it */
    errorcode = IddMeasureGetAndDisplayValue();
    retrynb++;
  }

  /* If modified during retry procedure, restore Shunt used to shunt number on board */
  if(shuntused != MFXSTM32L152_IDD_SHUNT_NB_4)
  {
    mfxstm32l152_IDD_ConfigShuntNbLimit(IDD_I2C_ADDRESS, MFXSTM32L152_IDD_SHUNT_NB_4);
  }
}

IDD_StatusTypeDef IddMeasureGetAndDisplayValue(void)
{
  uint32_t tempo = 0;
  /* display variable */
  uint16_t datatodisplay[LCD_DIGIT_MAX_NUMBER] = {0};
  uint8_t idderror = 0;
  uint8_t i = 0;

  /* lear screen */
  BSP_LCD_GLASS_Clear();

  /* check if idd interrupt that occurred is measurement done or error */
  if(BSP_IDD_ErrorGetITStatus() != 0)
  {
    /* Clear Error IT */
    BSP_IDD_ErrorClearIT();

    /* Get error code */
    idderror = BSP_IDD_ErrorGetCode();

    Convert_IntegerIntoChar(idderror, datatodisplay);
    datatodisplay[0] = 'E';
    datatodisplay[1] = 'R';
    datatodisplay[2] = 'R';
    BSP_LCD_GLASS_DisplayStrDeci(datatodisplay);

    idderror = IDD_ERROR;
    
    if(DemoDebugMode != RESET)
    {
      tempo = 2500;
    }
    
  }
  else if(BSP_IDD_GetITStatus() != 0)
  {
    /* Clear IddIT */
    BSP_IDD_ClearIT();

    /* Get Idd value */
    BSP_IDD_GetValue(&IddReadValue);

    if(IddReadValue == 0)
    {
      BSP_LCD_GLASS_DisplayString((uint8_t *) "000000");

      if(DemoDebugMode != RESET)
      {
        tempo = 2500;
      }

      idderror = IDD_ZERO_VALUE;
    }
    else
    {
      /* Convert Idd value in order to display it on LCD glass */
      Idd_Convert(IddReadValue, datatodisplay);

      /* display measure */
      BSP_LCD_GLASS_DisplayStrDeci(datatodisplay);

      /* reset display buffer */
      for(i = 0; i < LCD_DIGIT_MAX_NUMBER; i++)
      {
        datatodisplay[i] = 0;
      }

      tempo = 2500;
      idderror = IDD_OK;
    }
  }
  else
  {
    BSP_LCD_GLASS_DisplayStrDeci((uint16_t *) "IDD KO");

    if(DemoDebugMode != RESET)
    {
      tempo = 2500;
    }

    idderror = IDD_ERROR;
  }

  /* wait for 2,5sec */
  HAL_Delay(tempo);

  return (IDD_StatusTypeDef)idderror;
}

/**
  * @brief  Perform Idd measurement in run mode.
  * @param  None.
  * @note   None.  
  * @retval None.
  */
static void IddMeasureRun(void)
{
  if (PowerSupplyMode == SUPPLY_MODE_BATTERY)
  {
   Idd_BatterySupply_ClockIncrease();
  }
  else
  {
    Idd_ExternalSupply_ClockDecrease();
  }  

  IddMeasure(IDD_RUN);
  
  if (PowerSupplyMode == SUPPLY_MODE_BATTERY)
  {
    Idd_BatterySupply_ClockDecrease();
  }
   else
  {
    Idd_ExternalSupply_ClockIncrease();
  }  
}

/**
  * @brief  Perform Idd measurement in sleep mode.
  * @param  None.
  * @note   None.  
  * @retval None.
  */
static void IddMeasureSleep(void)
{ 
  IddMeasure(IDD_SLEEP);
}

/**
  * @brief  Perform Idd measurement in Low Power Run mode.
  * @param  None.
  * @note   None.  
  * @retval None.
  */
static void IddMeasureLpRun(void)
{
  IddMeasure(IDD_LPR_2MHZ);
}

/**
  * @brief  Perform Idd measurement in Low Power Sleep mode.
  * @param  None.
  * @note   None.  
  * @retval None.
  */
static void IddMeasureLpSleep(void)
{
  IddMeasure(IDD_LPR_SLEEP);
}

/**
  * @brief  Perform Idd measurement in Stop2 mode.
  * @param  None.
  * @note   None.  
  * @retval None.
  */
static void IddMeasureStop2(void)
{
  IddMeasure(IDD_STOP2);
}

/**
  * @brief  Perform Idd measurement in Standby mode.
  * @param  None.
  * @note   None.  
  * @retval None.
  */
static void IddMeasureStandby(void)
{
  IddMeasure(IDD_STANDBY);
}

/**
  * @brief  Perform Idd measurement in Shutdown mode.
  * @param  None.
  * @note   None.  
  * @retval None.
  */
static void IddMeasureShutdown(void)
{
  IddMeasure(IDD_SHUTDOWN);
}

/**
  * @brief  Enter MCU in Sleep mode.
  * @param  None
  * @retval None
  */
static void Idd_SleepEnter(void)
{
  if (PowerSupplyMode == SUPPLY_MODE_BATTERY)
  {
    Idd_BatterySupply_ClockIncrease();
  }
  else
  {
    Idd_ExternalSupply_ClockDecrease();
  }
  
  /* Suspend HAL tick irq */
  HAL_SuspendTick();
  
  /* Suspend RTOS Systick */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
  
  /* System peripheral clock disable */
  SystemPeripheralClockDisable();
  
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
  /* System peripheral clock enable */
  SystemPeripheralClockEnable();
  
  /* Resume HAL tick irq */
  HAL_ResumeTick();

  /* Resume RTOS Systick */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
  
  if (PowerSupplyMode == SUPPLY_MODE_BATTERY)
  {
    Idd_BatterySupply_ClockDecrease();
  }
  else
  {
    Idd_ExternalSupply_ClockIncrease();
  }
  
  /* Disable Power Control clock */
  __HAL_RCC_PWR_CLK_DISABLE();
}

/**
  * @brief  Enter MCU in Low Power Run mode.
  * @param  None
  * @retval None
  */
static void Idd_LprEnter(void)
{
  if (PowerSupplyMode != SUPPLY_MODE_BATTERY)
  {
    /* go to 2MHz PLL Off */
    SystemLowClock_Config();
  }
}

/**
  * @brief  Restore MCU configuration from Low Power mode.
  * @param  None
  * @retval None
  */
static void Idd_LprRestore(void)
{
  if (PowerSupplyMode != SUPPLY_MODE_BATTERY)
  {
    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* Disable low power mode */
    HAL_PWREx_DisableLowPowerRunMode();

    /* Disable Power Control clock */
    __HAL_RCC_PWR_CLK_DISABLE();

    /* restore clock to PLL 80MHz */
    SystemClock_Config();
  }
}

/**
  * @brief  Enter MCU in Low Power Sleep mode.
  * @param  None
  * @retval None
  */
static void Idd_LprSleepEnter(void)
{
  /* Enter in LPR mode */
  Idd_LprEnter();

  /* Suspend HAL tick irq */
  HAL_SuspendTick();
  
  /* Suspend RTOS Systick */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  /* System peripheral clock disable */
  SystemPeripheralClockDisable();
  
  /* Then enter in sleep mode */
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
  /* System peripheral clock enable */
  SystemPeripheralClockEnable();
  
  /* Resume HAL tick irq */
  HAL_ResumeTick();

  /* Resume RTOS Systick */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  Idd_LprRestore();
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
  /* Idd component measurement Interrupt pin is on PC13 which can be 
     configured as wake up pin source. Use it to wake up main MCU */
  /* Disable all used wakeup sources: WKUP pin */
  HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);
  
  /* Clear wake up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
  
  /* Enable wakeup pin WKUP2 */
  HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_HIGH);
}


/**
  * @brief  Convert value to display correct amper unit.
  * @param  None
  * @retval None
  */
static void Idd_Convert(uint32_t Value, uint16_t *DisplayString)
{
  uint32_t valuetoconvert = 0;
  uint16_t SIprefix = 0;
  uint8_t i = 0;
  uint8_t unitindex = LCD_DIGIT_MAX_NUMBER - UNIT_CHAR_NB;
  FlagStatus zerofilter = RESET;
  
  if(Value < 100)
  {
    /* measured value is in nA & we want 1 digit after coma */
    valuetoconvert = Value * 100;
    
    /* measured value is in nA */
    /* Add SI prefix information */
    SIprefix = (uint16_t) 'n';
  }
  else if(Value < 100000)
  {
    /* measured value is in µA & we want 1 digit after coma */
    valuetoconvert = Value / 10;
    
    /* Add SI prefix information */
    SIprefix = (uint16_t) 'µ';
  }
  else
  {
    /* measured value is in mA & we want 1 digit after coma */
    valuetoconvert = Value / 10000;
    
    /* Add SI prefix information */
    SIprefix = (uint16_t) 'm';
  }

  Convert_IntegerIntoChar(valuetoconvert, DisplayString);

  /* reorder display string to have following template WXY.Z mA */
  while(i < unitindex)
  {
    *(DisplayString + i) = *(DisplayString + UNIT_CHAR_NB + i);

    /* remove 0 in front of value */
    if((*(DisplayString + i) == '0') && (zerofilter == RESET))
    {
      *(DisplayString + i) = (uint16_t) ' ';
    }
    else
    {
      zerofilter = SET;
    }
    i++;
  }

  /* Add SI prefix & ampere information */
  *(DisplayString + (unitindex - 2)) |= DOT;
  
  /* Add SI preficx & ampere information */
  *(DisplayString + (unitindex)) = SIprefix;
  *(DisplayString + (unitindex + 1)) = (uint16_t) 'A';
}



/**
  * @brief  System Clock Speed increase when power is battery supplied
  *         Exit Low Power Run mode  
  *         Set the system clock source MSI to range 9 (24 MHz)
  *         (system clock speed is increased from 2 to 24 MHz)  
  * @note  API is called for Idd measures in Run mode and Sleep mode         
  * @param  None
  * @retval None
  */  
static void Idd_BatterySupply_ClockIncrease(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
    /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  HAL_PWREx_DisableLowPowerRunMode();
  
   /* Disable Power Control clock once PWR registers are updated */
   __HAL_RCC_PWR_CLK_DISABLE();  
 
  
  /* MSI is set to range 9 */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_9; /* 24 MHz */
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;

  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
 
}


/**
  * @brief  System Clock Speed decrease when power is battery supplied
  *         Set the system clock source MSI to range 5 (2 MHz)
  *         (system clock speed is reduced from 24 to 2 MHz)  
  *         Enable Low Power Run mode
  * @note  API is called once Idd measures in Run and Sleep modes are done          
  * @param  None
  * @retval None
  */  
static void Idd_BatterySupply_ClockDecrease(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Set MSI range 5 */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5; /* 2 MHz */
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  
  /* Move in Low Power Run mode */
  
   /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE(); 
  HAL_PWREx_EnableLowPowerRunMode();
   /* Disable Power Control clock once PWR registers are updated */
   __HAL_RCC_PWR_CLK_DISABLE();

  
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
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */

#undef __IDDMEASURE_C


