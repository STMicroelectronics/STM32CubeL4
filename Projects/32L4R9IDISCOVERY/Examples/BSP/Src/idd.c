/**
  ******************************************************************************
  * @file    BSP/Src/idd.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use idd current measurement component
  *          in the STM32L4R9I-Discovery driver
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

/* Private typedef -----------------------------------------------------------*/
#define IDD_VALUE_STRING_SIZE 4
#define IDD_UNIT_STRING_SIZE 2

typedef struct
{
  char value[IDD_VALUE_STRING_SIZE + 1];  /* 3 significant digit only */
  char unit[IDD_UNIT_STRING_SIZE + 1];    /* 2 letters */
  uint32_t value_na;                      /* value in nano amps */
  uint8_t error_code;                     /* 0 means no error else see mfx documentation
                                             In case of error please retry */
} Iddvalue_t;

/* Private define ------------------------------------------------------------*/
#define IDD_TEST_STEP_MAX    4    /* 0: Run, 1: Sleep, 2: Stop1, 3: Stop2 */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO JOYState_TypeDef JoyState;
extern __IO uint32_t IddEvent;
static volatile uint32_t IddValue;
static uint32_t IddTestStep;


#if defined(LCD_DIMMING)
extern RTC_HandleTypeDef RTCHandle;
extern TIM_HandleTypeDef TimHandle;
//extern uint32_t rtc_initialized;
#endif /* defined(LCD_DIMMING) */

/* Private functions ---------------------------------------------------------*/
static void Idd_Convert(uint32_t Value, Iddvalue_t * idd);
static void Idd_SetHint(uint32_t TestPart);
static void Idd_PeriphClockEnable(void);
static void Idd_PeriphClockDisable(void);



/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void Idd_demo(void)
{
  Iddvalue_t idd;

#if defined(LCD_DIMMING)
  HAL_RTCEx_DeactivateWakeUpTimer(&RTCHandle);

  HAL_RTC_DeInit(&RTCHandle);
  HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);
  __HAL_RCC_RTC_DISABLE();

  HAL_TIM_Base_DeInit(&TimHandle);
  __HAL_RCC_TIM3_CLK_DISABLE();
#endif

  /*##- IDD Initialization ############################*/
  if(BSP_IDD_Init() != IDD_OK)
  {
    Error_Handler();
  }

  /* Enable Interrupt to get end of measurement */
  /* This will wahe up MCU from low power mode */
  BSP_IDD_ClearIT();
  BSP_IDD_ErrorClearIT();

  BSP_IDD_EnableIT();
  BSP_IDD_ErrorEnableIT();

  IddTestStep = 0;

  while(IddTestStep < IDD_TEST_STEP_MAX)
  {
    /* Display menu and Idd test step */
    Idd_SetHint(IddTestStep);

    /* Next menu if JOY RIGHT */
    JoyState = JOY_NONE;

    while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
    BSP_LCD_DisplayStringAt(50, 150, (uint8_t*) "Wait 5 seconds...", LEFT_MODE);
    BSP_LCD_DisplayStringAt(50, 175, (uint8_t*) "Screen goes off", LEFT_MODE);
    BSP_LCD_Refresh();

    HAL_Delay(1000);

    /* Pre-measurement system update part */
    /* De-initialize BSP LCD resources */
    BSP_LCD_DeInit();

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

#if (DEBUG_ON == 1)
    HAL_DBGMCU_EnableDBGSleepMode();
    HAL_DBGMCU_EnableDBGStopMode();
#endif

    IddEvent = 0;
    BSP_IDD_StartMeasure();

    switch(IddTestStep)
    {
      case 0:
        /* Do nothing in run mode */
        break;
      case 1:
         /* Suspend HAL tick irq */
         HAL_SuspendTick();

        /* Sleep mode */
        HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
        break;
      case 2:
        /* Stop1 mode */
#if (DEBUG_ON == 1)
        HAL_SuspendTick();
#endif
        Idd_PeriphClockDisable();

        HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);
        break;
      case 3:
        /* Stop2 mode */
        SystemLowClock_Config();
#if (DEBUG_ON == 1)
        HAL_SuspendTick();
#endif
        Idd_PeriphClockDisable();

        HAL_PWREx_DisableLowPowerRunMode();
        HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
        break;
      default:
        break;
    }

    while((IddEvent == 0) && (JoyState != JOY_RIGHT))
    {
      if(MfxItOccurred == SET)
      {
        /* Resume HAL tick irq */
        HAL_ResumeTick();

        switch(IddTestStep)
        {
          case 0:
            /* Do nothing in run mode */
          case 1:
            /* Do nothing in Sleep mode */
            break;
          case 2:
            /* Back from Stop1 mode */
          case 3:
            /* Back from Stop2 mode */
            Idd_PeriphClockEnable();

            SystemClock_ConfigFromLowPower();
            break;
          default:
            break;
        }

        /* Post-measurement system update part */
        /* Re-initialize BSP LCD and TS resources */
        BSP_LCD_Init();


        /* Handle MFX event */
        Mfx_Event();
      }
    }

    if(IddEvent == 1)
    {
      BSP_IDD_GetValue((uint32_t*) &IddValue);

      /* Fill value in Nano amps */
      idd.value_na = 10 * IddValue;

      /* Convert Idd value in order to display it */
      Idd_Convert(IddValue, &idd);

      /* Display LCD messages */
      while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
      BSP_LCD_SetFont(&Font16);
      BSP_LCD_DisplayStringAt(50, 150, (uint8_t*) "Measure done      ", LEFT_MODE);
      BSP_LCD_DisplayStringAt(50, 175, (uint8_t*) "                  ", LEFT_MODE);
      BSP_LCD_DisplayStringAt(50, 175, (uint8_t*) idd.value, LEFT_MODE);
      BSP_LCD_DisplayStringAt(120, 175, (uint8_t*) idd.unit, LEFT_MODE);
      BSP_LCD_DisplayStringAt(50, 240, (uint8_t*) "If bad result,", LEFT_MODE);
      BSP_LCD_DisplayStringAt(50, 255, (uint8_t*) "check JP1 is on 1-2 IDD", LEFT_MODE);
      BSP_LCD_Refresh();

      /* Wait for next action */
      while(JoyState != JOY_RIGHT)
      {
        if(MfxItOccurred == SET)
        {
          Mfx_Event();
        }
      }
    }
    else
    {
      /* Exit while loop */
      break;
    }

    IddTestStep++;
  }

#if defined(LCD_DIMMING)
  Dimming_Timer_Enable(&RTCHandle);
#endif

  /* Reset JoyState */
  JoyState = JOY_NONE;
}

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
  * @brief  Display Idd demo hint
  * @param  TestPart   Test part identifier
  * @retval None
  */
static void Idd_SetHint(uint32_t TestPart)
{
  while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 130);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"Idd", CENTER_MODE);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"Press RIGHT to go", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 75, (uint8_t *)"to next menu", CENTER_MODE);

  switch(TestPart)
  {
    case 0:
      BSP_LCD_DisplayStringAt(0, 100, (uint8_t*)"RUN mode", CENTER_MODE);
      break;
    case 1:
      BSP_LCD_DisplayStringAt(0, 100, (uint8_t*)"SLEEP mode", CENTER_MODE);
      break;
    case 2:
      BSP_LCD_DisplayStringAt(0, 100, (uint8_t*)"STOP1 mode", CENTER_MODE);
      break;
    case 3:
      BSP_LCD_DisplayStringAt(0, 100, (uint8_t*)"STOP2 mode", CENTER_MODE);
      break;
    default:
      break;
  }

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(40, 140, BSP_LCD_GetXSize() - 80, BSP_LCD_GetYSize()- 240);
  BSP_LCD_DrawRect(41, 141, BSP_LCD_GetXSize() - 82, BSP_LCD_GetYSize()- 242);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_Refresh();
}

static void Idd_PeriphClockEnable(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  __HAL_RCC_RTCAPB_CLK_ENABLE();
  __HAL_RCC_DMA2D_CLK_ENABLE();
  __HAL_RCC_GFXMMU_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_LTDC_CLK_ENABLE();
  __HAL_RCC_DSI_CLK_ENABLE();

    /*##-2- Configure the GPIOs ################################################*/
    /* Enable GPIO clock */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /* VddIO2 must be enabled to access GPIO port G[2:15] */
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWREx_EnableVddIO2();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /* Configure I2C pins as alternate function  */
    GPIO_InitStructure.Pin       = GPIO_PIN_6;
    GPIO_InitStructure.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStructure.Pull      = GPIO_PULLUP;
    GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStructure.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure); /* I2C1_SCL on PB6 */
    GPIO_InitStructure.Pin       = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure); /* I2C1_SDA on PG13 */
}


static void Idd_PeriphClockDisable(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  __HAL_RCC_RTCAPB_CLK_DISABLE();
  __HAL_RCC_DMA2D_CLK_DISABLE();
  __HAL_RCC_GFXMMU_CLK_DISABLE();
  __HAL_RCC_SYSCFG_CLK_DISABLE();
  __HAL_RCC_LTDC_CLK_DISABLE();
  __HAL_RCC_DSI_CLK_DISABLE();
  RCC->AHB1SMENR = 0;
  RCC->AHB2SMENR = 0;
  RCC->AHB3SMENR = 0;
  RCC->APB1SMENR1 = 0;
  RCC->APB1SMENR2 = 0;
  RCC->APB2SMENR = 0;

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

  /* Set all GPIO in analog state to reduce power consumption,                */
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);

#if (DEBUG_ON == 1)
  /* Preserve SWDIO / SWCLK for Debug */
  GPIO_InitStructure.Pin = GPIO_PIN_All & ~(GPIO_PIN_13 | GPIO_PIN_14);
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Preserve SWO */
  GPIO_InitStructure.Pin = GPIO_PIN_All & ~(GPIO_PIN_3);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
#else
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif


  /* Preserve MFX_INT interface */
  GPIO_InitStructure.Pin = GPIO_PIN_All & ~GPIO_PIN_1;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);

  /* Preserve SEL_JOY_PIN interface */
  GPIO_InitStructure.Pin = GPIO_PIN_All & ~GPIO_PIN_13;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Disable GPIOs clock */
#if (DEBUG_ON == 1)
#else
  __HAL_RCC_GPIOA_CLK_DISABLE();  /* SWD interface for Debug */
  __HAL_RCC_GPIOB_CLK_DISABLE();  /* SWD interface for Debug */
#endif
  //__HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOF_CLK_DISABLE();
  __HAL_RCC_GPIOG_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();
  //__HAL_RCC_GPIOI_CLK_DISABLE();  MFX_INT interrupt

  HAL_PWREx_DisableVddIO2();
}
