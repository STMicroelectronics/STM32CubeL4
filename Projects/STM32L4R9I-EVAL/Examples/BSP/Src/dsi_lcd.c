/**
  ******************************************************************************
  * @file    dsi_lcd.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use DSI_LCD drawing features.
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
#define DSI_LCD_FEATURES_NUM                3
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t DSI_LCD_Feature = 0;
extern __IO FlagStatus TamperItOccurred;
/* Private function prototypes -----------------------------------------------*/
static void DSI_LCD_SetHint(void);
static void DSI_LCD_Show_Feature(uint8_t feature);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  LCD demo
  * @param  None
  * @retval None
  */
void DSI_LCD_demo (void)
{ 
  DSI_LCD_SetHint();
  DSI_LCD_Feature = 0;
  DSI_LCD_Show_Feature(DSI_LCD_Feature);
  
  while (1)
  {
    if(TamperItOccurred == SET)
    {
      /* Add delay to avoid rebound and reset it status */
      HAL_Delay(500);
      TamperItOccurred = RESET;
      
      if(++DSI_LCD_Feature < DSI_LCD_FEATURES_NUM)
      {
        DSI_LCD_Show_Feature(DSI_LCD_Feature);
      }
      else
      {
        return;
      }
    }
  }
}

/**
  * @brief  Display DSI LCD demo hint
  * @param  None
  * @retval None
  */
static void DSI_LCD_SetHint(void)
{
  while(BSP_DSI_LCD_IsFrameBufferAvailable() != LCD_OK);
  /* Clear the LCD */ 
  BSP_DSI_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_DSI_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_DSI_LCD_FillRect(0, 0, BSP_DSI_LCD_GetXSize(), 130);
  BSP_DSI_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_DSI_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_DSI_LCD_SetFont(&Font24);
  BSP_DSI_LCD_DisplayStringAt(0, 10, (uint8_t *)"LCD", CENTER_MODE);
  BSP_DSI_LCD_SetFont(&Font12);
  BSP_DSI_LCD_DisplayStringAt(0, 60, (uint8_t *)"This example shows the different", CENTER_MODE);
  BSP_DSI_LCD_DisplayStringAt(0, 75, (uint8_t *)"LCD Features, use Wakeup push-button", CENTER_MODE);
  BSP_DSI_LCD_DisplayStringAt(0, 90, (uint8_t *)"to display next page", CENTER_MODE);
  
  /* Set the LCD Text Color */
  BSP_DSI_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_DSI_LCD_DrawRect(40, 140, BSP_DSI_LCD_GetXSize() - 80, BSP_DSI_LCD_GetYSize()- 240);
  BSP_DSI_LCD_DrawRect(41, 141, BSP_DSI_LCD_GetXSize() - 82, BSP_DSI_LCD_GetYSize()- 242);
  BSP_DSI_LCD_Refresh();
}

/**
  * @brief  Show DSI LCD Features
  * @param  feature : feature index
  * @retval None
  */
static void DSI_LCD_Show_Feature(uint8_t feature)
{
  Point Points[]= {{50, 200}, {110, 200}, {110, 250}};
  Point Points2[]= {{BSP_DSI_LCD_GetXSize() - 110, 200}, {BSP_DSI_LCD_GetXSize() - 50, 200}, {BSP_DSI_LCD_GetXSize() - 50, 250}};
  
  while(BSP_DSI_LCD_IsFrameBufferAvailable() != LCD_OK);
  BSP_DSI_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_DSI_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_DSI_LCD_FillRect(42, 142, BSP_DSI_LCD_GetXSize() - 84, BSP_DSI_LCD_GetYSize()- 244);
  BSP_DSI_LCD_SetTextColor(LCD_COLOR_BLACK);
  
  switch (feature)
  {
  case 0:
    /* Text Feature */
    BSP_DSI_LCD_DisplayStringAt(44, 150, (uint8_t *)"Left aligned Text", LEFT_MODE);
    BSP_DSI_LCD_DisplayStringAt(0, 165, (uint8_t *)"Center aligned Text", CENTER_MODE);
    BSP_DSI_LCD_DisplayStringAt(44, 180, (uint8_t*)"Right aligned Text", RIGHT_MODE);
    BSP_DSI_LCD_SetFont(&Font24);
    BSP_DSI_LCD_DisplayStringAt(44, 205, (uint8_t *)"Font24", LEFT_MODE);
    BSP_DSI_LCD_SetFont(&Font20);
    BSP_DSI_LCD_DisplayStringAt(44, 230, (uint8_t *)"Font20", LEFT_MODE);
    BSP_DSI_LCD_SetFont(&Font16);
    BSP_DSI_LCD_DisplayStringAt(44, 251, (uint8_t *)"Font16", LEFT_MODE);
    break;
    
  case 1:
    /* Draw misc. Shapes */
    BSP_DSI_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_DSI_LCD_DrawRect(50, 150, 60 , 40);
    BSP_DSI_LCD_FillRect(130, 150, 60 , 40);
    
    BSP_DSI_LCD_SetTextColor(LCD_COLOR_GRAY);
    BSP_DSI_LCD_DrawCircle(BSP_DSI_LCD_GetXSize() - 150, 170, 20);
    BSP_DSI_LCD_FillCircle(BSP_DSI_LCD_GetXSize() - 90, 170, 20);
    
    BSP_DSI_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_DSI_LCD_DrawPolygon(Points, 3);
    
    BSP_DSI_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_DSI_LCD_DrawEllipse(160, 220, 30, 20);
    BSP_DSI_LCD_FillEllipse(230, 220, 30, 20);
    
    BSP_DSI_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_DSI_LCD_DrawHLine(50, 270, BSP_DSI_LCD_GetXSize() / 5);
    BSP_DSI_LCD_DrawLine (130, 280, 260, 250);
    BSP_DSI_LCD_DrawLine (130, 250, 260, 280);
    
    BSP_DSI_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_DSI_LCD_FillPolygon(Points2, 3);
    break;
    
  case 2:
    /* Draw Bitmap */
    BSP_DSI_LCD_DrawBitmap(50, 150, (uint8_t *)stlogo);
    
    BSP_DSI_LCD_DrawBitmap(BSP_DSI_LCD_GetXSize()/2 - 40, 150, (uint8_t *)stlogo);
    
    BSP_DSI_LCD_DrawBitmap(BSP_DSI_LCD_GetXSize()-140, 150, (uint8_t *)stlogo);
    
    BSP_DSI_LCD_DrawBitmap(50, 220, (uint8_t *)stlogo);
    
    BSP_DSI_LCD_DrawBitmap(BSP_DSI_LCD_GetXSize()/2 - 40, 220, (uint8_t *)stlogo);
    
    BSP_DSI_LCD_DrawBitmap(BSP_DSI_LCD_GetXSize()-140, 220, (uint8_t *)stlogo);
    break;
  }
  BSP_DSI_LCD_Refresh();
}

/**
  * @}
  */ 

/**
  * @}
  */ 
