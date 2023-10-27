/**
  ******************************************************************************
  * @file    BSP/Src/joystick.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the joystick feature in the 
  *          STM32L476G-Discovery driver
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
/* extern variables ----------------------------------------------------------*/
extern JOYState_TypeDef JoyState;
extern FlagStatus KeyPressed;

/**
  * @brief  Joystick demo
  * @param  None
  * @retval None
  */
void Joystick_demo (void)
{
  JOYState_TypeDef previousstate = JOY_NONE;
  
  while(previousstate != JOY_LEFT)
  {
    if (KeyPressed)
    {
      switch(JoyState)
      {
      case JOY_LEFT:
        BSP_LED_Toggle(LED4);
        BSP_LCD_GLASS_DisplayString((uint8_t *)"  LEFT");
        HAL_Delay(1000);
        break;
        
      case JOY_RIGHT:
        BSP_LED_Toggle(LED5);
        BSP_LCD_GLASS_DisplayString((uint8_t *)" RIGHT");
        break;
        
      case JOY_UP:
        BSP_LED_Toggle(LED4);
        BSP_LED_Toggle(LED5);
        BSP_LCD_GLASS_DisplayString((uint8_t *)"    UP");
        break;
        
      case JOY_DOWN:
        BSP_LED_Toggle(LED4);
        BSP_LED_Toggle(LED5);
        BSP_LCD_GLASS_DisplayString((uint8_t *)"  DOWN");
        break;
        
      case JOY_SEL:
        BSP_LED_On(LED4);
        BSP_LED_On(LED5);
        BSP_LCD_GLASS_DisplayString((uint8_t *)"   SEL");
        break;
        
      default:
        break;
      }
      
      if(previousstate != JoyState)
      {
        BSP_LED_Off(LED4);
        BSP_LED_Off(LED5);
        if(JoyState == JOY_UP)
        {
          BSP_LED_On(LED5);
        }
      }
      
      previousstate = JoyState;
      KeyPressed = RESET;   
      HAL_Delay(200);
    }
  }
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
