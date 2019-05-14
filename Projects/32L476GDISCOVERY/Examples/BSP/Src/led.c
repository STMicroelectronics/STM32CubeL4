/**
  ******************************************************************************
  * @file    BSP/Src/led.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the led display feature in the 
  *          STM32L476G-Discovery driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Led demo
  * @param  None
  * @retval None
  */
void Led_demo(void)
{ 
  uint32_t index = 0; 

  /* -1- LEDs initialization =================================================*/ 
  /* Init LED4 and LED5 */
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);
  
  /* -2- LEDs turn on / turn off =============================================*/
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      LED4 AND LED5 ON", 1, SCROLL_SPEED_HIGH);

  BSP_LED_On(LED4);
  BSP_LED_On(LED5);
  
  /* Keep the LEDs on for one seconds */
  HAL_Delay(1000);
  
  /* Turn off the LEDs */ 
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      LED4 AND LED5 OFF", 1, SCROLL_SPEED_HIGH);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);

  /* Keep the LEDs off for one seconds */
  HAL_Delay(1000);
  
  /* -3- LEDs toggling =======================================================*/
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      ALTERNATE TOGGLE", 1, SCROLL_SPEED_HIGH);
  
  /* For about five seconds, toggle all the LEDs */
  BSP_LED_On(LED4);
  BSP_LED_On(LED4);
  index = 0;
  
  while (index < 25)
  {
    HAL_Delay(200);
    index++;
    BSP_LED_Toggle(LED4);
    BSP_LED_Toggle(LED5);
  }
  
  /* -3- LEDs Off  =======================================================*/
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      LEDS OFF", 1, SCROLL_SPEED_HIGH);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
