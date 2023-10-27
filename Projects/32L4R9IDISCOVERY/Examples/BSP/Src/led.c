/**
  ******************************************************************************
  * @file    BSP/Src/led.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the led display feature in the 
  *          STM32L4R9I_EVAL driver
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
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Led_SetHint(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Led demo
  * @param  None
  * @retval None
  */
void Led_demo(void)
{
  uint32_t index = 0;
  
  Led_SetHint();
  
  /* Turn on LED 1 to 4  */
  while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(0, 150, (uint8_t *)"Initialize then turn on", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 165, (uint8_t *)"each leds after 500 ms", CENTER_MODE);
  BSP_LCD_Refresh();
  HAL_Delay(500);
  BSP_LED_On(LED1);
  
  HAL_Delay(500);
  BSP_LED_On(LED2);
    
  /* Keep the LEDs on for two seconds */
  HAL_Delay(2000);
  
  /* Turn off the LEDs */ 
  while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
  BSP_LCD_DisplayStringAt(0, 150, (uint8_t *)"   Turn off each leds   ", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 165, (uint8_t *)"     after 500 ms     ", CENTER_MODE);
  BSP_LCD_Refresh();
  HAL_Delay(500);
  BSP_LED_Off(LED1);
  HAL_Delay(500);
  BSP_LED_Off(LED2);
  
  HAL_Delay(2000);
  
  /* After two seconds, turn on LED1 and LED2 */
  while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
  BSP_LCD_DisplayStringAt(0, 150, (uint8_t *)"     Turn on led 2     ", CENTER_MODE);
  BSP_LCD_Refresh();
  BSP_LED_On(LED2);
  HAL_Delay(500);
  
  while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
  BSP_LCD_DisplayStringAt(0, 150, (uint8_t *)"     Turn on led 1     ", CENTER_MODE);
  BSP_LCD_Refresh();
  BSP_LED_On(LED1);
  HAL_Delay(500);
  
  /* For about five seconds, toggle all the LEDs */
  while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
  BSP_LCD_DisplayStringAt(0, 150, (uint8_t *)"   Toggle all on leds   ", CENTER_MODE);
  BSP_LCD_Refresh();
  while (index < 25)
  {
    HAL_Delay(200);
    index++;
    BSP_LED_Toggle(LED1);
    BSP_LED_Toggle(LED2);
  }
}

/**
  * @brief  Display Led demo hint
  * @param  None
  * @retval None
  */
static void Led_SetHint(void)
{
  while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set Led Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 130);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"Led", CENTER_MODE);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"Shows how to turn on,", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 75, (uint8_t *)"turn off and toggle", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 90, (uint8_t *)"the leds", CENTER_MODE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(40, 140, BSP_LCD_GetXSize() - 80, BSP_LCD_GetYSize()- 240);
  BSP_LCD_DrawRect(41, 141, BSP_LCD_GetXSize() - 82, BSP_LCD_GetYSize()- 242);
  BSP_LCD_Refresh();
}
/**
  * @}
  */ 

/**
  * @}
  */ 
  
