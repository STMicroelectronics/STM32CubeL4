/**
  ******************************************************************************
  * @file    BSP/Src/main.c 
  * @author  MCD Application Team
  * @brief   This file provides a BSP example tailored for STM32L476G-Discovery
  *          board, in order to manage the LEDs, the Joystick, the audio mems, 
  *          the gyroscope, quadSPI flash and LCD glass mounted on STM32L476G-Discovery
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
typedef enum
{
  DEMO_STATE_LED            = 0x00,   /*!< Led application launched */
  DEMO_STATE_JOYSTICK,                /*!< Joystick application launched */
  DEMO_STATE_LCD ,                    /*!< LCD application launched */
  DEMO_STATE_GYRO,                    /*!< Gyroscope application launched */
  DEMO_STATE_IDD,                     /*!< Idd application launched */
  DEMO_STATE_AUDIO_PLAY,              /*!< Audio playback application launched */
  DEMO_STATE_AUDIO_RECORD,            /*!< Audio record application launched */
  DEMO_STATE_FIRST_STARTUP,           /*!< Demo first Startup */
  DEMO_STATE_IDLE,                    /*!< Demo in idle */
} Demo_StateTypeDef;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Menu / Application management */
uint32_t AppStatus = 0x00;
FlagStatus AppLaunched = RESET;
uint8_t DemoIndex = 0;

/* Wav Player Pause/Resume Status. Defined as external in audio_play.c file */
__IO uint32_t PauseResumeStatus = IDLE_STATUS;

/* Counter for Sel Joystick pressed*/
__IO uint32_t PressCount = 0;

/* Volume of the audio playback */
/* Initial volume level (from 50% to 100% (Max)) */
__IO uint8_t Volume = 70;
__IO uint8_t VolumeChange = 0;
__IO uint8_t AudioPlaybackExit = 0;
__IO uint8_t AudioRecordExit = 0;
__IO uint8_t AudioRecordSampleRateSelected = 0;  
__IO uint8_t SampleRateIndex = 0;
__IO uint8_t AudioRecordSampleRateChange = 0;

/* initialization variable */
__IO FlagStatus KeyPressed = RESET;
FlagStatus JoyInitialized = RESET;
FlagStatus IddInitialized = RESET;
FlagStatus LcdInitialized = RESET;
FlagStatus LedInitialized = RESET;

JOYState_TypeDef JoyState = JOY_NONE;

__IO uint8_t NbLoop = 1;

/* Variable indicating which audio demo is currently running (playback v.s. record) */
Audio_DemoTypeDef AudioDemo = AUDIO_DEMO_NONE;


/* Private function prototypes -----------------------------------------------*/
static void Display_DemoDescription(void);

BSP_DemoTypedef  BSP_examples[]=
{
  {Led_demo, "LED", DEMO_STATE_LED},
  {Joystick_demo, "JOYSTICK", DEMO_STATE_JOYSTICK},
  {LCDGlass_demo, "LCD GLASS", DEMO_STATE_LCD},
  {Gyro_demo, "GYRO", DEMO_STATE_GYRO},
  {Idd_demo, "IDD", DEMO_STATE_IDD},
  {AudioPlay_demo, "AUDIO PLAYBACK", DEMO_STATE_AUDIO_PLAY},
  {AudioRecord_demo, "AUDIO RECORD", DEMO_STATE_AUDIO_RECORD},
  {QSPI_demo, "QSPI", 0}
};
/* Private functions ---------------------------------------------------------*/

/* Extern variable -----------------------------------------------------------*/
extern __IO uint8_t bLCDGlass_KeyPressed;
extern FlagStatus IddItOccurred;
extern uint8_t IddIndex;
extern FlagStatus ReturnFromStandbyShutdown;

/**
  * @brief  Main program
  *         The program follows the following steps 
  *         -1- LEDs initialization.
  *         -2- User-based LEDs management: all the LEDs are turned on and the user
  *         can next turn on/off/toggle the LEDs in resorting to the Wakeup/Tamper
  *         button or to the Joystick.
  *         Pressing the SEL joystick button turns on all the LEDs
  * @retval None
  */
int main(void)
{
  uint8_t i = 0;
  uint8_t menudisplay[DEMO_NAME_CHAR_NB + 5] = {0};
  
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

  /*##-1- Configure the system clock #########################################*/
  /*  */
  SystemClock_Config(); 

  /*##-2-Configure minimum hardware resources at boot ########################*/
  SystemHardwareInit();

  /*##-3- Check if we resume from low power mode #############################*/
  /* Check if the system was resumed from Shutdown mode */
  if(__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST) != RESET)
  {
    /* Clear the reset flag */
    __HAL_RCC_CLEAR_RESET_FLAGS();

    /* read backup register 0 to check is we return from Idd measurement test */
    IddIndex = SystemRtcBackupRead();

    if(IddIndex == IDD_SHUTDOWN)
    {
      /* Notify we return from Standby or Shutdown Low Power mode */
      ReturnFromStandbyShutdown = SET;
    }
  }

  /* Check if the system was resumed from StandBy mode */
  if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
  {
    /* Clear the Standby flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

    /* Check and Clear the Wakeup flag */
    if(__HAL_PWR_GET_FLAG(PWR_FLAG_WUF2) != RESET)
    {
      __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
    }

    /* read backup register 0 to check is we return from Idd measurement test */
    IddIndex = SystemRtcBackupRead();

    /* Notify we return from Standby or Shutdown Low Power mode */
    ReturnFromStandbyShutdown = SET;
  }

  if(ReturnFromStandbyShutdown != RESET)
  {
    /* Idd measeurement current application was running */
    DemoIndex = DEMO_STATE_IDD;
    
    /* Display demo explanation on LCD Glass */
    AppStatus = DEMO_STATE_IDD;
    
    /* Standby test was on going */
    BSP_examples[DemoIndex].DemoFunc();
    
    /* Set back demo status to Idle */
    AppStatus = DEMO_STATE_IDLE;
  }
  else
  {
    /* First startup: manage mfx component: put it in standby mode */
    if(BSP_IDD_Init() != IDD_OK)
    {
      Error_Handler();
    }
    else
    {
      BSP_IDD_LowPower();
      BSP_IDD_DeInit();
    }
    
    /* Display demo explanation on LCD Glass */
    AppStatus = DEMO_STATE_FIRST_STARTUP;
    Display_DemoDescription();
  }

  /* Menu display first characters initialization */
  for(i = 0; i < 5; i++)
  {
    menudisplay[i] = ' ';
  }

  /* Wait For User inputs */
  while (1)
  {
     for(i = 0; i < DEMO_NAME_CHAR_NB; i++)
     {
       menudisplay[i+5] = BSP_examples[DemoIndex].DemoName[i];
     }
      
      BSP_LCD_GLASS_ScrollSentence(menudisplay, 1, SCROLL_SPEED_HIGH);

      if(AppLaunched != RESET)
      {
        AppLaunched = RESET;
        KeyPressed = RESET;
        
        /* Clear screen */
        BSP_LCD_GLASS_Clear();

        /* Set Application status in case of specific joystick usage */
        AppStatus = BSP_examples[DemoIndex].DemoIndex;

        /* launched application */
        BSP_examples[DemoIndex].DemoFunc();
        
        /* Set Application status to Idle */
        AppStatus = DEMO_STATE_IDLE;
        
        /* Notify end of test and clear screen */
        BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      END OF TEST", 1, SCROLL_SPEED_HIGH);
        BSP_LCD_GLASS_Clear();
      }
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 8000000
  *            PLL_M                          = 1
  *            PLL_N                          = 20
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
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

  /* 80 Mhz from MSI 8Mhz */
  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_7;
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
}

/**
  * @brief  System Clock Configuration
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
  
  /* Retrieve oscillator parameters */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);
  
  /* turn off PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  System Power Configuration at Boot
  * @retval None
  */
void SystemHardwareInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Init LED4 and LED5  */
  if(LedInitialized != SET)
  {
    BSP_LED_Init(LED_RED);
    BSP_LED_Init(LED_GREEN);
    LedInitialized = SET;
  }
  
  /* Init STM32L476G-Discovery joystick in interrupt mode */
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

  /* Configure the GPIOs for MFX */
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

  /* Configure the Discovery I2C2 peripheral for MFX */
  /* Enable Discovery_I2C2 clock */
  DISCOVERY_I2C2_CLK_ENABLE();

  /* Enable PWR clock */
  __HAL_RCC_PWR_CLK_ENABLE();
}

/**
  * @brief  System Low Power Configuration
  * @retval None
  */
void SystemHardwareDeInit(void)
{
  RCC_OscInitTypeDef oscinitstruct = {0};

  if(LedInitialized != RESET)
  {
    BSP_LED_DeInit(LED_RED);
    BSP_LED_DeInit(LED_GREEN);
    LedInitialized = RESET;
  }

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

  BSP_QSPI_DeInit();
  BSP_GYRO_DeInit();
  
  /* LSE off in all modes */
  if(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) != RESET)
  {
    oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    oscinitstruct.PLL.PLLState   = RCC_PLL_NONE;
    oscinitstruct.LSEState       = RCC_LSE_OFF;
    if(HAL_RCC_OscConfig(&oscinitstruct) != HAL_OK)
    {
      while(1);
    }
  }
  
  HAL_PWR_DisableBkUpAccess();

  /* Disable remaining clocks */
  __HAL_RCC_PWR_CLK_DISABLE();
  HAL_PWREx_DisablePullUpPullDownConfig();

  /* Configure MFX I2C Rx/Tx as Analog floating during IDD */
  HAL_GPIO_DeInit(DISCOVERY_I2C2_SCL_GPIO_PORT, (DISCOVERY_I2C2_SCL_PIN | DISCOVERY_I2C2_SDA_PIN)); /* No STOP2 after FW upgrade */
  
  __HAL_RCC_FLASH_CLK_DISABLE();
  __HAL_RCC_SYSCFG_CLK_DISABLE();
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_I2C2_CLK_DISABLE();
  
  RCC->AHB1SMENR = 0x0;
  RCC->AHB2SMENR = 0x0;
  RCC->AHB3SMENR = 0x0;
  RCC->APB1SMENR1 = 0x0;
  RCC->APB1SMENR2 = 0x0;
  RCC->APB2SMENR = 0x0;
}

/**
  * @brief  Read from RTC Backup area
  * @retval None
  */
uint32_t SystemRtcBackupRead(void)
{
  RTC_HandleTypeDef hrtc;
  hrtc.Instance = RTC;

  /* Enable PWR clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* enable Back up register access */
  HAL_PWR_EnableBkUpAccess();

  return(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0));
}

/**
  * @brief  Write to Backup area
  * @param  SaveIndex: item index to save
  * @retval None
  */
void SystemRtcBackupWrite(uint32_t SaveIndex)
{
  RTC_HandleTypeDef hrtc;
  hrtc.Instance = RTC;

  /* enable Back up register access */
  HAL_PWR_EnableBkUpAccess();

  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, SaveIndex);
}


/**
  * @brief  Display main demo messages
  * @retval None
  */
static void Display_DemoDescription(void)
{
  /* Clear the LCD */ 
  BSP_LCD_GLASS_Clear();

  /* Display LCD messages */
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)"     STM32L476G-DISCO BSP DRIVER EXAMPLE", 1, SCROLL_SPEED_HIGH);
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)"     COPYRIGHT STMICROELECTRONICS 2016", 1, SCROLL_SPEED_HIGH);

  /* Introduce short delay before new message displayed */
  HAL_Delay(50);

  BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      Press Joy Right to start ", 0xFFFF, SCROLL_SPEED_HIGH);

  /* Clear the LCD & reset Key pressed variable */
  BSP_LCD_GLASS_Clear();

  AppStatus = DEMO_STATE_IDLE;
}

/**
  * @brief  Check for user input
  * @retval Input state (1 : active / 0 : Inactive)
  */
uint8_t CheckForUserInput(void)
{
  if(BSP_JOY_GetState() == JOY_SEL)
  {
    while (BSP_JOY_GetState() == JOY_SEL);
    return 1;
  }
  return 0;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED RED on */
  while(1)
  {
    BSP_LED_Toggle(LED_RED);
    HAL_Delay(40);
  }
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
    
    switch(AppStatus)
    {
      case DEMO_STATE_FIRST_STARTUP:
      case DEMO_STATE_IDD:
        bLCDGlass_KeyPressed = 0x01;
        break;
      case DEMO_STATE_IDLE:
        bLCDGlass_KeyPressed = 0x01;
        switch(GPIO_Pin)
        {
          case DOWN_JOY_PIN :
            DemoIndex++;
            if(DemoIndex > COUNT_OF_EXAMPLE(BSP_examples))
            {
             DemoIndex = 0;
            }
            break;
          case UP_JOY_PIN :
            DemoIndex--;
            if(DemoIndex > COUNT_OF_EXAMPLE(BSP_examples))
            {
             DemoIndex = (COUNT_OF_EXAMPLE(BSP_examples) - 1);
            }
            break;
          case RIGHT_JOY_PIN :
            AppLaunched = SET;
            break;
        }
        break;
    
      case DEMO_STATE_LED:
        break;
      
      case DEMO_STATE_JOYSTICK:
      case DEMO_STATE_LCD:
        switch(GPIO_Pin)
        {
          case DOWN_JOY_PIN :
            JoyState = JOY_DOWN;
            break;
          case UP_JOY_PIN :
            JoyState = JOY_UP;
            break;
          case SEL_JOY_PIN :
            JoyState = JOY_SEL;
            break;
          case RIGHT_JOY_PIN :
            JoyState = JOY_RIGHT;
            break;
          case LEFT_JOY_PIN :
            JoyState = JOY_LEFT;
            break;
        }
        break;
        
    case DEMO_STATE_AUDIO_PLAY:
        switch(GPIO_Pin)
        {
          case DOWN_JOY_PIN :
            /* DOWN is used to decrement the volume of the audio playback */
            if (VolumeChange == 0)
            {
              Volume --;
              if ((int8_t)Volume < 50)
              {
                Volume = 50;
              }
              VolumeChange = 1;
            }
            break;
          case UP_JOY_PIN :
            /* UP is used to increment the volume of the audio playback */
            if (VolumeChange == 0)
            {
              Volume ++;
              if (Volume > 100)
              {
                Volume = 100;
              }
              VolumeChange = 1;
            }
            break;
          case SEL_JOY_PIN :
            /* SEL is used to pause and resume the audio playback */
            if (PressCount == 1)
            {
              /* Resume playing Wave status */
              PauseResumeStatus = RESUME_STATUS;
              PressCount = 0;
            }
            else
            {
              /* Pause playing Wave status */
              PauseResumeStatus = PAUSE_STATUS;
              PressCount = 1;
            }
            break;
          case LEFT_JOY_PIN :
            /* Set AudioPlaybackExit flag */
            AudioPlaybackExit = 1;
            break;
        }
      break;
      
    case DEMO_STATE_AUDIO_RECORD:
        switch(GPIO_Pin)
        {
          case DOWN_JOY_PIN :
            /* DOWN is used to decrease the audio record sample rate */
            if (AudioRecordSampleRateChange == 0)
            {
              if (SampleRateIndex == 6)
              {
                SampleRateIndex = 0;
              }
              else
              {
                SampleRateIndex++;
              }
              AudioRecordSampleRateChange = 1;
            }
            break;
          case UP_JOY_PIN :
            /* UP is used to increase the audio record sample rate */
            if (AudioRecordSampleRateChange == 0)
            {
              if (SampleRateIndex == 0)
              {
                SampleRateIndex = 6;
              }
              else
              {
                SampleRateIndex--;
              }
              AudioRecordSampleRateChange = 1;
            }
            break;
          case RIGHT_JOY_PIN :
            /* SEL is used to confirm audio record sample rate and start audio recording */
            AudioRecordSampleRateSelected = 1;
            break;
          case LEFT_JOY_PIN :
            /* Set AudioRecordExit flag */
            AudioRecordExit = 1;
            break;
        }
      break;
    }
  }
  else if((GPIO_Pin & IDD_INT_PIN) != RESET)
  {
    if(AppStatus == DEMO_STATE_IDD)
    {
      IddItOccurred = SET;
    }
  }
}

/**
  * @brief converts a 32bit unsined int into ASCII 
  * @caller several callers for display values
  * @param number digit to displays
  * @param p_tab values in array in ASCII   
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
  * @brief  fibonacci_flash processing.
  * @param n loop counter
  * @retval None
  */
  
volatile int result;

void fibonacci_flash(int n)
{
  int a = 0;
  int b = 1;
  int sum = 0;
  int i;
  result = 0;
  while(1)
  {
    a = 0;
    b = 1;
    for(i=0;i<n;i++)
    {
      sum = a + b;
      a = b;
      b = sum;
    }
    result = sum;
  }
}


/**
  * @}
  */ 

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
#endif /* USE_FULL_ASSERT */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

