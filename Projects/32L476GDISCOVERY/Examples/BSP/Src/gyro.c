/**
  ******************************************************************************
  * @file    BSP/Src/gyro.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the gyroscope feature in the 
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
#define GYRO_THRESHOLD_DETECTION 10000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Gyro_Convert(uint8_t Axis, int32_t Value, uint16_t *DisplayString);
/* Private functions ---------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern __IO FlagStatus KeyPressed;

/*
  * @brief  Test Gyroscope MEMS Hardware.
  *         The main objectif of this test is to check gyroscope hardware connection, and 
  *         display angular data.
  * @param  None
  * @retval None
  */
void Gyro_demo(void)
{
  /* display variable */
  uint16_t datatodisplay[LCD_DIGIT_MAX_NUMBER] = {0};
  uint8_t i = 0;
  
  /* Gyroscope variables */
  float buffer[3] = {0};
  int32_t xval = 0;
  int32_t yval = 0;
  int32_t zval = 0;
  uint32_t xvalabs = 0;
  uint32_t yvalabs = 0;
  uint32_t zvalabs = 0;

  /* Init Gyroscope Mems */
  if(BSP_GYRO_Init() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  while(KeyPressed != SET)
  {
    /* Read Gyroscope Angular data */
    BSP_GYRO_GetXYZ(buffer);

    /* Get absolute value */
    xval = (int32_t) buffer[0];
    yval = (int32_t) buffer[1];
    zval = (int32_t) buffer[2];
    xvalabs = ABS(xval);
    yvalabs = ABS(yval);
    zvalabs = ABS(zval);
    
    /* Treat only significant datas */
    if((xvalabs > GYRO_THRESHOLD_DETECTION) || (yvalabs > GYRO_THRESHOLD_DETECTION) ||
       (zvalabs > GYRO_THRESHOLD_DETECTION))
    {
      /* reset display buffer */
      for(i = 0; i < LCD_DIGIT_MAX_NUMBER; i++)
      {
        datatodisplay[i] = 0;
      }
      
      /* Angular acceleration around X axis is higher than on Y */
      if(xvalabs > yvalabs)
      {
        /* Angular acceleration around X axis is higher than on Z */
        if(xvalabs > zvalabs)
        {
          Gyro_Convert('X', xval, datatodisplay);
          BSP_LCD_GLASS_DisplayStrDeci(datatodisplay);
        }
        /* Angular acceleration around X axis is lower than on Z */
        else
        { 
          Gyro_Convert('Z', zval, datatodisplay);
          BSP_LCD_GLASS_DisplayStrDeci(datatodisplay);
        }
      }
      /* Angular acceleration around X axis is lower than on Y */
      else
      {
        /* Angular acceleration around Y axis is higher than on Z */
        if(yvalabs > zvalabs)
        {
          Gyro_Convert('Y', yval, datatodisplay);
          BSP_LCD_GLASS_DisplayStrDeci(datatodisplay);
        }
        /* Angular acceleration around Y axis is lower than on Z */
        else
        { 
          Gyro_Convert('Z', zval, datatodisplay);
          BSP_LCD_GLASS_DisplayStrDeci(datatodisplay);
        }
      }
    }
    else
    {
      BSP_LCD_GLASS_DisplayString((uint8_t *)"STABLE");
    }
    /* insert Delay */
    HAL_Delay(200);
  }

  /* reset KeyPressed variable */
  KeyPressed = RESET;
}
/**
  * @}
  */ 

/**
  * @brief  Convert value to display string
  * @param  None
  * @retval None
  */

static void Gyro_Convert(uint8_t Axis, int32_t Value, uint16_t *DisplayString)
{
  uint8_t i = 0;
  uint32_t valuetoconvert = 0;

  valuetoconvert = (uint32_t) (ABS(Value) >> 6);

  Convert_IntegerIntoChar(valuetoconvert, DisplayString);

  while(*(DisplayString + i) == '0')
  {
    *(DisplayString + i) = ' ';
    i++;
  }

  /* Add Axis information 1st digit */
  *(DisplayString) = (uint16_t) Axis | DOUBLE_DOT;

  /* Handle negative value */
  if(Value < 0)
  {
    *(DisplayString + 1) = '-';
  }
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
