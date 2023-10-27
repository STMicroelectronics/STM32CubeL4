/**
  ******************************************************************************
  * @file    BSP/Src/touchscreen.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the touchscreen driver on round LCD.
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
extern __IO uint32_t TouchEvent;


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  CIRCLE_RADIUS        30
/* Private macro -------------------------------------------------------------*/
#define  CIRCLE_XPOS(i)       ((i * BSP_LCD_GetXSize()) / 5)
#define  CIRCLE_YPOS(i)       (BSP_LCD_GetYSize() - CIRCLE_RADIUS - 160)
/* Private variables ---------------------------------------------------------*/
static TS_StateTypeDef  TS_State;
uint32_t GestureTest = 0;
/* Private function prototypes -----------------------------------------------*/
static void Touchscreen_SetHint(void);
static void Touchscreen_DrawBackground (uint8_t state);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Touchscreen Demo
  * @param  None
  * @retval None
  */
void Touchscreen_demo (void)
{
  uint16_t x, y;
  uint8_t  state = 0;
  uint8_t  text[30];
  uint8_t  radius;
  uint8_t  radius_previous = 0;

  Touchscreen_SetHint();

  Touchscreen_DrawBackground(state);
  
  while (1)
  {
    if(GestureTest == 0)
    {
      /* Check in interrupt mode in touch screen the touch status and coordinates */
      /* if touch occurred                                                        */
      if(MfxItOccurred == SET)
      {
        Mfx_Event();
        
        if(TouchEvent != 0)
        {
          BSP_TS_GetState(&TS_State);
          if(TS_State.touchDetected)
          {
            /* Get X and Y position of the touch post calibrated */
            x = TS_State.touchX[0];
            y = TS_State.touchY[0];
            
            while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
            
            BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
            sprintf((char*)text, "Nb touch detected = %d", TS_State.touchDetected);
            BSP_LCD_ClearStringLine(19);
            BSP_LCD_DisplayStringAtLine(19, (uint8_t *)&text);
            
            /* Display 1st touch detected coordinates */
            sprintf((char*)text, "1[%d,%d], ev = %d", x, y, TS_State.touchEventId[0]);
            BSP_LCD_ClearStringLine(20);
            BSP_LCD_DisplayStringAtLine(20, (uint8_t *)&text);
            
            if (TS_State.touchDetected >= 2)  /* Display 2nd touch detected coordinates if applicable */
            {
              sprintf((char*)text, "2[%d,%d], ev = %d", TS_State.touchX[1], TS_State.touchY[1], TS_State.touchEventId[1]);
            }
            else
            {
              sprintf((char*)text, "              ");
            }
            BSP_LCD_ClearStringLine(21);
            BSP_LCD_DisplayStringAtLine(21, (uint8_t *)&text);
            
            /* Weight not supported so radius maximum */
            radius = CIRCLE_RADIUS;
            
            if ((y > (CIRCLE_YPOS(1) - CIRCLE_RADIUS)) &&
                (y < (CIRCLE_YPOS(1) + CIRCLE_RADIUS)))
            {
              
              if ((x > (CIRCLE_XPOS(1) - CIRCLE_RADIUS)) &&
                  (x < (CIRCLE_XPOS(1) + CIRCLE_RADIUS)))
              {
                if ((radius != radius_previous) || (state != 1))
                {
                  if (state != 1) /* Erase previous filled circle */
                  {
                    Touchscreen_DrawBackground(state);
                  }
                  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
                  BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), radius);
                  radius_previous = radius;
                  state = 1;
                }
              }
              if ((x > (CIRCLE_XPOS(2) - CIRCLE_RADIUS)) &&
                  (x < (CIRCLE_XPOS(2) + CIRCLE_RADIUS)))
              {
                if ((radius != radius_previous) || (state != 2))
                {
                  if (state != 2) /* Erase previous filled circle */
                  {
                    Touchscreen_DrawBackground(state);
                  }
                  BSP_LCD_SetTextColor(LCD_COLOR_RED);
                  BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), radius);
                  radius_previous = radius;
                  state = 2;
                }
              }
              
              if ((x > (CIRCLE_XPOS(3) - CIRCLE_RADIUS)) &&
                  (x < (CIRCLE_XPOS(3) + CIRCLE_RADIUS)))
              {
                if ((radius != radius_previous) || (state != 4))
                {
                  if (state != 4) /* Erase previous filled circle */
                  {
                    Touchscreen_DrawBackground(state);
                  }
                  BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
                  BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), radius);
                  radius_previous = radius;
                  state = 4;
                }
              }
              
              if ((x > (CIRCLE_XPOS(4) - CIRCLE_RADIUS)) &&
                  (x < (CIRCLE_XPOS(4) + CIRCLE_RADIUS)))
              {
                if ((radius != radius_previous) || (state != 8))
                {
                  if (state != 8) /* Erase previous filled circle */
                  {
                    Touchscreen_DrawBackground(state);
                  }
                  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
                  BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), radius);
                  radius_previous = radius;
                  state = 8;
                }
              }
            }
            
            BSP_LCD_Refresh();
          } /* if(TS_State.touchDetected) */
          TouchEvent = 0;
        } /* if(TouchEvent != 0) */
        else if(JoyState == JOY_RIGHT)
        {
          /* Switch to gesture test */
          GestureTest = 1;
          BSP_TS_GestureConfig(ENABLE);
          Touchscreen_SetHint();
        }
      } /* if(MfxItOccurred == SET) */
    }
    else /* Gesture Test */
    {
      /* Check in interrupt in touch screen the touch status and coordinates    */
      /* if touch occurred                                                      */
      if(MfxItOccurred == SET)
      {
        Mfx_Event();
        
        if(TouchEvent != 0)
        {
          BSP_TS_Get_GestureId(&TS_State);
          
          while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
          BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
          BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
          BSP_LCD_SetFont(&Font24);
          BSP_LCD_ClearStringLine(8);
          switch(TS_State.gestureId)
          {
          case GEST_ID_MOVE_RIGHT:
            {
              BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"MOVE_RIGHT");
            }
            break;
          case GEST_ID_MOVE_LEFT:
            {
              BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"MOVE_LEFT");
            }
            break;
          case GEST_ID_MOVE_UP:
            {
              BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"MOVE_UP");
            }
            break;
          case GEST_ID_MOVE_DOWN:
            {
              BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"MOVE_DOWN");
            }
            break;
          case GEST_ID_DOUBLE_CLICK:
            {
              BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"DOUBLE_CLICK");
            }
            break;
          default:
            {
            }
            break;
          }
          BSP_LCD_Refresh();
          TouchEvent = 0;
        }
        else if(JoyState == JOY_RIGHT)
        {
          /* Switch to gesture test */
          GestureTest = 0;
          BSP_TS_GestureConfig(DISABLE);
          return;
        }
      }
    }
  }
}

/**
  * @brief  Display TS Demo Hint
  * @param  None
  * @retval None
  */
static void Touchscreen_SetHint(void)
{
  if(GestureTest == 0)
  {
    while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
    
    /* Clear the LCD */
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    
    /* Set Touchscreen Demo description */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 130);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"Touchscreen", CENTER_MODE);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0, 65, (uint8_t *)"Use the Touchscreen to fill", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 80, (uint8_t *)"the colored circles. Up to 2", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 95, (uint8_t *)"finger coord. are displayed", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 110, (uint8_t *)"Press Joystick right to exit", CENTER_MODE);
    
    /* Set the LCD Text Color */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_DrawRect(40, 140, BSP_LCD_GetXSize() - 80, BSP_LCD_GetYSize() - 240);
    BSP_LCD_DrawRect(41, 141, BSP_LCD_GetXSize() - 82, BSP_LCD_GetYSize() - 242);
    
    BSP_LCD_Refresh();
  }
  else /* Gesture Test */
  {
    while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
    
    /* Clear the LCD */
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    
    /* Set Touchscreen Demo description */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 130);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"Touchscreen", CENTER_MODE);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0, 65, (uint8_t *)"Please use the Touchscreen", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 80, (uint8_t *)"to try gesture feature", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 95, (uint8_t *)"(left to right for instance)", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 110, (uint8_t *)"Press Joystick right to exit", CENTER_MODE);
    
    BSP_LCD_Refresh();
  }
}

/**
  * @brief  Draw Touchscreen Background
  * @param  state : touch zone state
  * @retval None
  */
static void Touchscreen_DrawBackground (uint8_t state)
{
  if(GestureTest == 0)
  {
    while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
    switch (state)
    {
      
    case 0:
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
      BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS);
      
      
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS);
      
      
      BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
      BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS);
      
      
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS);
      
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2);
      BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2);
      BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2);
      BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2);
      break;
      
    case 1:
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
      BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2);
      break;
      
    case 2:
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2);
      break;
      
    case 4:
      BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
      BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2);
      break;
      
    case 8:
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4), CIRCLE_RADIUS);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4), CIRCLE_RADIUS - 2);
      break;
      
    }
    BSP_LCD_Refresh();
  }
}

/**
  * @}
  */

/**
  * @}
  */

