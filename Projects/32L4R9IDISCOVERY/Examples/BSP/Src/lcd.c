/**
  ******************************************************************************
  * @file    BSP/Src/lcd.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use LCD drawing features.
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

extern __IO JOYState_TypeDef JoyState;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LCD_FEATURES_NUM                3
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t LCD_Feature = 0;
/* Private function prototypes -----------------------------------------------*/
static void LCD_SetHint(void);
static void LCD_Show_Feature(uint8_t feature);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  LCD demo
  * @param  None
  * @retval None
  */
void LCD_demo (void)
{ 
  LCD_SetHint();
  LCD_Feature = 0;
  LCD_Show_Feature(LCD_Feature);
  
  while (1)
  {
    if(MfxItOccurred == SET)
    {
      Mfx_Event();
      if(JoyState == JOY_RIGHT)
      {
        if(++LCD_Feature < LCD_FEATURES_NUM)
        {
          LCD_Show_Feature(LCD_Feature);
        }
        else
        {
          return;
        }
      }
      HAL_Delay(100);
    }
  }
}

/**
  * @brief  Display DSI LCD demo hint
  * @param  None
  * @retval None
  */
static void LCD_SetHint(void)
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
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"LCD", CENTER_MODE);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"Shows the different LCD", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 75, (uint8_t *)"features, use Joystick right", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 90, (uint8_t *)"to display next page", CENTER_MODE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(40, 140, BSP_LCD_GetXSize() - 80, BSP_LCD_GetYSize()- 240);
  BSP_LCD_DrawRect(41, 141, BSP_LCD_GetXSize() - 82, BSP_LCD_GetYSize()- 242);
  BSP_LCD_Refresh();
}

/**
  * @brief  Show DSI LCD Features
  * @param  feature : feature index
  * @retval None
  */
static void LCD_Show_Feature(uint8_t feature)
{
  Point Points[]= {{50, 200}, {110, 200}, {110, 250}};
  Point Points2[]= {{BSP_LCD_GetXSize() - 110, 200}, {BSP_LCD_GetXSize() - 50, 200}, {BSP_LCD_GetXSize() - 50, 250}};
  
  while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(42, 142, BSP_LCD_GetXSize() - 84, BSP_LCD_GetYSize()- 244);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  
  switch (feature)
  {
  case 0:
    /* Text Feature */
    BSP_LCD_DisplayStringAt(44, 150, (uint8_t *)"Left aligned Text", LEFT_MODE);
    BSP_LCD_DisplayStringAt(0, 165, (uint8_t *)"Center aligned Text", CENTER_MODE);
    BSP_LCD_DisplayStringAt(44, 180, (uint8_t*)"Right aligned Text", RIGHT_MODE);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_DisplayStringAt(44, 205, (uint8_t *)"Font24", LEFT_MODE);
    BSP_LCD_SetFont(&Font20);
    BSP_LCD_DisplayStringAt(44, 230, (uint8_t *)"Font20", LEFT_MODE);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(44, 251, (uint8_t *)"Font16", LEFT_MODE);
    break;
    
  case 1:
    /* Draw misc. Shapes */
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawRect(50, 150, 60 , 40);
    BSP_LCD_FillRect(130, 150, 60 , 40);
    
    BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
    BSP_LCD_DrawCircle(BSP_LCD_GetXSize() - 150, 170, 20);
    BSP_LCD_FillCircle(BSP_LCD_GetXSize() - 90, 170, 20);
    
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DrawPolygon(Points, 3);
    
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DrawEllipse(160, 220, 30, 20);
    BSP_LCD_FillEllipse(230, 220, 30, 20);
    
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawHLine(50, 270, BSP_LCD_GetXSize() / 5);
    BSP_LCD_DrawLine (130, 280, 260, 250);
    BSP_LCD_DrawLine (130, 250, 260, 280);
    
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillPolygon(Points2, 3);
    break;
    
  case 2:
    /* Draw Bitmap */
    BSP_LCD_DrawBitmap(50, 150, (uint8_t *)stlogo);
    
    BSP_LCD_DrawBitmap(BSP_LCD_GetXSize()/2 - 40, 150, (uint8_t *)stlogo);
    
    BSP_LCD_DrawBitmap(BSP_LCD_GetXSize()-140, 150, (uint8_t *)stlogo);
    
    BSP_LCD_DrawBitmap(50, 220, (uint8_t *)stlogo);
    
    BSP_LCD_DrawBitmap(BSP_LCD_GetXSize()/2 - 40, 220, (uint8_t *)stlogo);
    
    BSP_LCD_DrawBitmap(BSP_LCD_GetXSize()-140, 220, (uint8_t *)stlogo);
    break;
  }
  BSP_LCD_Refresh();
}

/**
  * @}
  */ 

/**
  * @}
  */ 
