/**
  ******************************************************************************
  * @file    BSP/Src/idd.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use idd current measurement component 
  *          in the STM32L476G-Discovery driver
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

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  void   (*IddEnter)(void);
  void   (*IddRestore)(void);
  uint8_t IddName[DEMO_NAME_CHAR_NB];
  uint32_t IddIndex;
}Idd_AppliTypedef;

/* Private defines -----------------------------------------------------------*/
#define UNIT_CHAR_NB                              2
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Idd_SleepEnter(void);
static void Idd_LprEnter(void);
static void Idd_LprRestore(void);
static void Idd_LprSleepEnter(void);
static void Idd_StopEnter(void);
static void Idd_StopRestore(void);
static void Idd_StandbyEnter(void);
static void Idd_ShutdownEnter(void);
static void Idd_WakeUpPinConfig(void);
static void Idd_Convert(uint32_t Value, uint16_t *DisplayString);

/* Private variables ---------------------------------------------------------*/
__IO FlagStatus IddItOccurred = RESET;
FlagStatus ReturnFromStandbyShutdown = RESET;
uint32_t IddReadValue = 0;
uint8_t IddIndex = IDD_RUN;
Idd_AppliTypedef IddTest[]=
{
  {NULL, NULL, "RUN", IDD_RUN},
  {Idd_SleepEnter, NULL, "SLEEP", IDD_SLEEP},
  {Idd_LprEnter, Idd_LprRestore,"LOW POWER 2MHZ", IDD_LPR_2MHZ},
  {Idd_LprSleepEnter, Idd_LprRestore,"LOW POWER SLEEP", IDD_LPR_SLEEP},
  {Idd_StopEnter, Idd_StopRestore, "STOP2", IDD_STOP2},
  {Idd_StandbyEnter, NULL,"STANDBY", IDD_STANDBY},
  {Idd_ShutdownEnter, NULL,"SHUTDOWN", IDD_SHUTDOWN},
};

/* External variables --------------------------------------------------------*/
extern FlagStatus IddInitialized;

/**
  * @brief  Idd demo
  * @retval None
  */
void Idd_demo(void)
{
  /* display variable */
  uint16_t datatodisplay[LCD_DIGIT_MAX_NUMBER] = {0};
  uint8_t idddisplay[DEMO_NAME_CHAR_NB + 6] = {0};
  uint8_t idderror = 0;
  uint8_t i = 0;
  
  /* Initialize Idd measurement component */
  if(IddInitialized != SET)
  {
    if(BSP_IDD_Init() != IDD_OK)
    {
      Error_Handler();
    }
    IddInitialized = SET;
  }

  /* Enable Interrupt to get end of measurement */
  /* This will wahe up MCU from low power mode */
  BSP_IDD_EnableIT();
  BSP_IDD_ErrorEnableIT();
  IddItOccurred = RESET;

  /* Idd display first characters initialization */
  for(i = 0; i < 6; i++)
  {
    idddisplay[i] = (uint8_t) ' ';
  }

  while(IddIndex < IDD_TEST_NB)
  {
    if(ReturnFromStandbyShutdown != RESET)
    {
      /* reset flag */
      ReturnFromStandbyShutdown = RESET;
    }
    else
    {
      /* Store Idd Index in case of unwanted reboot during Standby or Shutdown test */
      SystemRtcBackupWrite(IddIndex);
      
      /* display tested configuration */
      for(i = 0; i < DEMO_NAME_CHAR_NB; i++)
      {
        idddisplay[i+6] = IddTest[IddIndex].IddName[i];
      }
      BSP_LCD_GLASS_ScrollSentence(idddisplay, 1, SCROLL_SPEED_MEDIUM);

      /* Start measurement campaign */
      BSP_IDD_StartMeasure();

      /* un configure HW resources */
      SystemHardwareDeInit();

      /* if function pointer exists, execute correponsing low power action */
      if(IddTest[IddIndex].IddEnter != NULL)
      {
        IddTest[IddIndex].IddEnter();
      }

// fibonacci_flash(1);
      while(IddItOccurred == RESET);

      /* Reset measurement done global variable */
      IddItOccurred = RESET;

      /* if function pointer exists, restore context clocks and power context */
      if(IddTest[IddIndex].IddRestore != NULL)
      {
        IddTest[IddIndex].IddRestore();
      }

      /* configure HW resources */
      SystemHardwareInit();
    }

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
      Error_Handler();
    }
    else if(BSP_IDD_GetITStatus() != 0)
    {
      /* Clear IddIT */
      BSP_IDD_ClearIT();

      /* Get Idd value */
      BSP_IDD_GetValue(&IddReadValue);

      /* Convert Idd value in order to display it on LCD glass */
      Idd_Convert(IddReadValue, datatodisplay);

      /* display measure */
      BSP_LCD_GLASS_DisplayStrDeci(datatodisplay);

      /* reset display buffer */
      for(i = 0; i < LCD_DIGIT_MAX_NUMBER; i++)
      {
        datatodisplay[i] = 0;
      }
    }
    else
    {
      SystemRtcBackupWrite(IDD_RUN);
      BSP_LCD_GLASS_DisplayString((uint8_t *)" ERROR");
      Error_Handler();
      break;
    }

    /* wait 2,5 sec. */
    HAL_Delay(2500);

    /* Increment test index */
    IddIndex++;
  }

  /* reset IddIndex for next loop */
  IddIndex = IDD_RUN;
  SystemRtcBackupWrite(IDD_RUN);

  /* Enter component in standby mode */
  BSP_IDD_LowPower();
  BSP_IDD_DeInit();
  IddInitialized = RESET;
}

/**
  * @brief  ernter product in nomal sleep mode
  * @retval None
  */
static void Idd_SleepEnter(void)
{
  /* Enable PWR clock enable */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Suspend tick irq */
  HAL_SuspendTick();

  /* Enter in Low power mode */
  HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);

  /* Resume tick irq */
  HAL_ResumeTick();
  
  /* Disable Power Control clock */
  __HAL_RCC_PWR_CLK_DISABLE();
}

/**
  * @brief  Enter MCU in Low Power Run mode.
  * @retval None
  */
static void Idd_LprEnter(void)
{
  /* decrease main sysclk to 2MHz */
  SystemLowClock_Config();
  
  /* Adapt voltage scaling to low power run */
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  if(HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Set regulator to low power */
  HAL_PWREx_EnableLowPowerRunMode();

  __HAL_RCC_PWR_CLK_DISABLE();
}

/**
  * @brief  Restore MCU configuration from Low Power mode.
  * @retval None
  */
static void Idd_LprRestore(void)
{
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Disable low power mode */
  HAL_PWREx_DisableLowPowerRunMode();

  /* Set back voltage scaling to full power */
  if(HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Disable Power Control clock */
  __HAL_RCC_PWR_CLK_DISABLE();

  /* restore clock to PLL 80MHz */
  SystemClock_Config();
}


/**
  * @brief  Enter MCU in Low Power Sleep mode.
  * @retval None
  */
static void Idd_LprSleepEnter(void)
{
  /* Enter in LPR mode */
  Idd_LprEnter();

  /* Then enter in sleep mode */
  Idd_SleepEnter();
}


/**
  * @brief  Enter MCU in Stop mode.
  * @retval None
  */
static void Idd_StopEnter(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

#if (PREFETCH_ENABLE != 0)
  /* Disable Prefetch Buffer */
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
#endif /* PREFETCH_ENABLE */

  /* Set all GPIO in analog state to reduce power consumption,
       except GPIOC to keep user button interrupt enabled */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();    

    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Pin = GPIO_PIN_All;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_All & ~GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_All;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);  

    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();
    __HAL_RCC_GPIOE_CLK_DISABLE();
    __HAL_RCC_GPIOF_CLK_DISABLE();
    __HAL_RCC_GPIOG_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();

  /* Ensure that MSI is wake-up system clock */ 
  HAL_RCCEx_WakeUpStopCLKConfig(RCC_STOP_WAKEUPCLOCK_MSI);

  /* Enable PWR clock enable */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Suspend tick irq */
  HAL_SuspendTick();

  /* Enter in Low power Stop 2 */
  HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

  /* Resume tick irq */
  HAL_ResumeTick();
  
  /* Disable Power Control clock */
  __HAL_RCC_PWR_CLK_DISABLE();

  /* Restore LCD */
  BSP_LCD_GLASS_Init();
}

/**
  * @brief  Restore MCU configuration from Stop mode.
  * @retval None
  */
static void Idd_StopRestore(void)
{
#if (PREFETCH_ENABLE != 0)
  /* Re-enable Prefetch Buffer */
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
#endif /* PREFETCH_ENABLE */

  /* restore clock to PLL 80MHz */
  SystemClock_Config();
}

/**
  * @brief  Enter MCU in Standby mode.
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
  * @brief  Convert value to display correct amper unit
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
  * @}
  */ 

/**
  * @}
  */ 
  
