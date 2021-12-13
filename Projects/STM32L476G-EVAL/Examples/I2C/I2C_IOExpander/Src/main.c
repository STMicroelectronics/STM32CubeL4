/**
  ******************************************************************************
  * @file    I2C/I2C_IOExpander/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L4xx I2C HAL API to run
  *          the IO expander functionalities with a communication process based
  *          on I2C.
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

/** @addtogroup I2C_IOExpander
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TS_StateTypeDef TS_State;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);

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

  /* Configure LED1, LED2, LED3 and LED4 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* Select the Button test mode (polling or interrupt) BUTTON_MODE in main.h */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE);
  
  /*##-1- Initialize the LCD #################################################*/
  /* Initialize the LCD */
  if (BSP_LCD_Init() != LCD_OK)
  {
    Error_Handler();
  }

  /* Set LCD font */
  BSP_LCD_SetFont(&Font20);

  /*##-2- Display messages on LCD ############################################*/
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

  /* Display LCD messages */
  BSP_LCD_DisplayStringAtLine(0, (uint8_t *)MESSAGE1);
  BSP_LCD_DisplayStringAtLine(1, (uint8_t *)MESSAGE2);
  BSP_LCD_DisplayStringAtLine(2, (uint8_t *)MESSAGE3);

  /*##-3- Configure the Touch screen and IO functionalities modules ##########*/
  /* Initilaize the TS functionality */
  if (BSP_TS_Init(320, 240) == TS_OK)
  {
    BSP_LCD_DisplayStringAtLine(3, (uint8_t *)"TS module OK      ");
  }
  else
  {
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *)" TS module FAILED  ");
    BSP_LCD_DisplayStringAtLine(5, (uint8_t *)" Please Reset the  ");
    BSP_LCD_DisplayStringAtLine(6, (uint8_t *)"  board and start  ");
    BSP_LCD_DisplayStringAtLine(7, (uint8_t *)"      again        ");

    /* Execute error handler */
    Error_Handler();
  }

#ifdef INTERRUPT_MODE
  /* Configure TS interrupt acquisition mode*/
  BSP_TS_ITConfig();

  /* Configure the joystick interrupt acquisition mode */
  if (BSP_JOY_Init(JOY_MODE_EXTI) == IO_OK)
  {
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"JOY module OK      ");
  }
  else
  {
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *)" JOY module FAILED  ");
    BSP_LCD_DisplayStringAtLine(5, (uint8_t *)" Please Reset the   ");
    BSP_LCD_DisplayStringAtLine(6, (uint8_t *)"  board and start   ");
    BSP_LCD_DisplayStringAtLine(7, (uint8_t *)"      again         ");

    /* Execute error handler */
    Error_Handler();
  }
#else
  if (BSP_JOY_Init(JOY_MODE_GPIO) == IO_OK)
  {
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"JOY module OK  ");
  }
  else
  {
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *)" JOY module FAILED  ");
    BSP_LCD_DisplayStringAtLine(5, (uint8_t *)" Please Reset the   ");
    BSP_LCD_DisplayStringAtLine(6, (uint8_t *)"  board and start   ");
    BSP_LCD_DisplayStringAtLine(7, (uint8_t *)"      again         ");

    /* Execute error handler */
    Error_Handler();
  }
#endif /* INTERRUPT_MODE */

  /* LEDs Control blocks */
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_DrawRect(250 , 150, 60, 40);
  BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
  BSP_LCD_DrawRect(170, 150, 60, 40);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_DrawRect(90, 150, 60, 40);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 150, 60, 40);

  /* Set LCD font */
  BSP_LCD_SetFont(&Font24);

  /* Default joystick display */
  BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"JOY:     ----        ");

  /* Infinite loop */
  while (1)
  {
#ifdef POLLING_MODE
    static TS_StateTypeDef TS_State;
    static JOYState_TypeDef JoyState = JOY_NONE;

    /* Update the structure with the current position */
    BSP_TS_GetState(&TS_State);

    if ((TS_State.TouchDetected) && (TS_State.y < 190) && (TS_State.y > 150))
    {
      if ((TS_State.x > 20) && (TS_State.x < 70))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
        BSP_LCD_DisplayStringAtLine(8, (uint8_t *)" LD4               ");
        BSP_LED_On(LED4);
      }
      else if ((TS_State.x > 90) && (TS_State.x < 140))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"      LD3           ");
        BSP_LED_On(LED3);
      }
      else if ((TS_State.x > 160) && (TS_State.x < 210))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
        BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"          LD2       ");
        BSP_LED_On(LED2);
      }
      else if ((TS_State.x > 230) && (TS_State.x < 280))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
        BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"               LD1  ");
        BSP_LED_On(LED1);
      }
    }
    else
    {
      BSP_LED_Off(LED1);
      BSP_LED_Off(LED2);
      BSP_LED_Off(LED3);
      BSP_LED_Off(LED4);
    }

    /* Get the Joystick State */
    JoyState = BSP_JOY_GetState();

    /* Set the text color */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

    switch (JoyState)
    {
      case JOY_NONE:
        BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"JOY:     ----       ");
        break;
      case JOY_UP:
        BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"JOY:     UP         ");
        break;
      case JOY_DOWN:
        BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"JOY:    DOWN        ");
        break;
      case JOY_LEFT:
        BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"JOY:    LEFT        ");
        break;
      case JOY_RIGHT:
        BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"JOY:    RIGHT       ");
        break;
      case JOY_SEL:
        BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"JOY:     SEL        ");
        break;
      default:
        BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"JOY:    ERROR       ");
        break;
    }
#endif /* POLLING_MODE */

#ifdef BUTTON_POLLING_MODE
    /* Insert 10 ms delay */
    HAL_Delay(1);

    /* Set the LCD Text Color */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

    if (BSP_PB_GetState(BUTTON_TAMPER) != 0)
    {
      /* Toggle LED1 */
      BSP_LED_Toggle(LED1);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"Pol: TAMPER Pressed   ");
    }    
#endif /* BUTTON_POLLING_MODE */
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 1
  *            PLL_N                          = 20
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable HSE Oscillator and activate PLL with HSE as source   */
  /* (Default MSI Oscillator enabled at system reset remains ON) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

/** @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
#ifdef INTERRUPT_MODE
  if (GPIO_Pin == JOYSTICK_PIN)
  {

    static JOYState_TypeDef JoyState = JOY_NONE;
    static TS_StateTypeDef TS_State;

    /* Get the Joystick State */
    JoyState = BSP_JOY_GetState();

    /* Set the text color */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

    switch (JoyState)
    {
      case JOY_NONE:
        BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"JOY:     ----       ");
        break;
      case JOY_UP:
        BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"JOY:     UP         ");
        break;
      case JOY_DOWN:
        BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"JOY:    DOWN        ");
        break;
      case JOY_LEFT:
        BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"JOY:    LEFT        ");
        break;
      case JOY_RIGHT:
        BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"JOY:    RIGHT       ");
        break;
      case JOY_SEL:
        BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"JOY:     SEL        ");
        break;
      default:
        BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"JOY:    ERROR       ");
        break;
    }

    /* Clear joystick interrupt pending bits */
    BSP_IO_ITClear(JOY_ALL_PINS);

    /* Update the structure with the current position */
    BSP_TS_GetState(&TS_State);

    if ((TS_State.TouchDetected) && (TS_State.y < 190) && (TS_State.y > 150))
    {
      if ((TS_State.x > 20) && (TS_State.x < 70))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
        BSP_LCD_DisplayStringAtLine(8, (uint8_t *)" LD4               ");
        BSP_LED_On(LED4);
      }
      else if ((TS_State.x > 90) && (TS_State.x < 140))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"      LD3           ");
        BSP_LED_On(LED3);
      }
      else if ((TS_State.x > 160) && (TS_State.x < 210))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
        BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"          LD2       ");
        BSP_LED_On(LED2);
      }
      else if ((TS_State.x > 230) && (TS_State.x < 280))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
        BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"               LD1  ");
        BSP_LED_On(LED1);
      }
    }
    else
    {
      BSP_LED_Off(LED1);
      BSP_LED_Off(LED2);
      BSP_LED_Off(LED3);
      BSP_LED_Off(LED4);
    }

    /* Clear Touch screen global interrupt pending bits */
    BSP_TS_ITClear();
  }
#endif /* INTERRUPT_MODE */

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

#ifdef BUTTON_INTERRUPT_MODE
  if ((GPIO_Pin & TAMPER_BUTTON_PIN) == TAMPER_BUTTON_PIN)
  {
    /* Toggle LD1 */
    BSP_LED_Toggle(LED1);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"IT: TAMPER Pressed   ");
  }    
#endif /* BUTTON_INTERRUPT_MODE */

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while (1)
  {
  }
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
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

