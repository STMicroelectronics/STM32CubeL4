/**
  ******************************************************************************
  * @file    FLASH/FLASH_DualBoot/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example provides a description of how to boot from bank1 or bank2
  *          of the STM32L4xx FLASH.
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

/** @addtogroup FLASH_DualBoot
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE1     "STM32L496G" 
#define MESSAGE2     "Flash Dual-boot"
#define MESSAGE2_BIS "example"
#define MESSAGE3     "Device running on" 

#ifdef FLASH_BANK1
 #define MESSAGE4    "FLASH BANK1"
#else
 #define MESSAGE4    "FLASH BANK2"
#endif

#define MESSAGE5     "PUSH JOYSTICK SEL"
#define MESSAGE6     "to swap BFB2 option byte"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FLASH_OBProgramInitTypeDef    OBInit; 
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Display_ExampleDescription(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
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

  /* Configure the system clock to 80 MHz */
  SystemClock_Config();

  /* Configure JOY_SEL */
  BSP_JOY_Init(JOY_MODE_GPIO);

  /* Initialize LED1 and LED2 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  /* Display messages on LCD */
  Display_ExampleDescription();
  
  /* Turn on LEDs */
  BSP_LED_On(LED1);
  BSP_LED_On(LED2);

  /*--- If Wake-up button is pushed, Set or reset BFB2 bit to enable or disable 
  boot from Bank2 (active after next reset, w/ Boot pins set in Boot from Flash 
  memory position ---*/

  while (1)
  {
    /* Wait for JOY_SEL is released */
    if (BSP_JOY_GetState() == JOY_SEL)
    {
      while (BSP_JOY_GetState() == JOY_SEL);

      /* Set BFB2 bit to enable boot from Flash Bank2 */
      /* Allow Access to Flash control registers and user Flash */
      HAL_FLASH_Unlock();
	  
      /* Clear OPTVERR bit set on virgin samples */
      __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

      /* Allow Access to option bytes sector */ 
      HAL_FLASH_OB_Unlock();
      
      /* Get the Dual boot configuration status */
      HAL_FLASHEx_OBGetConfig(&OBInit);
      
      /* Enable/Disable dual boot feature */
      OBInit.OptionType = OPTIONBYTE_USER;
      OBInit.USERType   = OB_USER_BFB2;
      
      if (((OBInit.USERConfig) & (OB_BFB2_ENABLE)) == OB_BFB2_ENABLE)
      {
        OBInit.USERConfig = OB_BFB2_DISABLE;
      }
      else
      {
        OBInit.USERConfig = OB_BFB2_ENABLE;
      }
      
      if(HAL_FLASHEx_OBProgram (&OBInit) != HAL_OK)
      {
        /*
        Error occurred while setting option bytes configuration.
        User can add here some code to deal with this error.
        To know the code error, user can call function 'HAL_FLASH_GetError()'
        */
        /* Infinite loop */
        while (1)
        {
          BSP_LED_Toggle(LED1);
          BSP_LED_Toggle(LED2);
          HAL_Delay(1000);
        }
      }
      
      /* Start the Option Bytes programming process */  
      if (HAL_FLASH_OB_Launch() != HAL_OK)
      {
        /*
        Error occurred while reloading option bytes configuration.
        User can add here some code to deal with this error.
        To know the code error, user can call function 'HAL_FLASH_GetError()'
        */
        /* Infinite loop */
        while (1)
        {
          BSP_LED_Toggle(LED1);
          BSP_LED_Toggle(LED2);
          HAL_Delay(1000);
        }
      }

      /* Prevent Access to option bytes sector */
      HAL_FLASH_OB_Lock();
    
      /* Disable the Flash option control register access (recommended to protect 
      the option Bytes against possible unwanted operations) */
      HAL_FLASH_Lock();

    }
    else
    {
#ifdef FLASH_BANK1        
      /* Toggle LED1 */
      BSP_LED_Toggle(LED1);
#else 
      /* Toggle LED2 */    
      BSP_LED_Toggle(LED2);
#endif    
      
      /* Insert 50 ms delay */
      HAL_Delay(50);
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
  *            PLL_P                          = 7
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
  * @brief  Display main example messages
  * @param  None
  * @retval None
  */
static void Display_ExampleDescription(void)
{
  /* Initialize the LCD */
  BSP_LCD_Init();
  
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);

#ifdef FLASH_BANK1
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
#else
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
#endif /* BOOT_FROM_BANK1 */
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);


  /* Display LCD messages */
#ifdef FLASH_BANK1
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
#else
  BSP_LCD_SetBackColor(LCD_COLOR_RED);
#endif /* BOOT_FROM_BANK1 */
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)MESSAGE1, CENTER_MODE);

  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 40, (uint8_t *)MESSAGE2, CENTER_MODE);

  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)MESSAGE2_BIS, CENTER_MODE);

#ifdef FLASH_BANK1
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
#else
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
#endif /* BOOT_FROM_BANK1 */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 95, (uint8_t *)MESSAGE3, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 110, (uint8_t *)MESSAGE4, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 140, (uint8_t *)MESSAGE5, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 155, (uint8_t *)MESSAGE6, CENTER_MODE);
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

