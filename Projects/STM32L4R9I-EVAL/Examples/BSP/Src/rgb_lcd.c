/**
  ******************************************************************************
  * @file    BSP/Src/rgb_lcd.c
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
#include "RGB565_320x240.h"
#include "flying.h"
#include "ARGB4444_480x272.h"

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

extern __IO FlagStatus TamperItOccurred;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LCD_FEATURES_NUM        3 /* 5 (2 layers & color keying validation to do) */
#define ARGB4444_COLOR_KEY	0xFFFF
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t LCD_Feature = 0;

__IO uint32_t ReloadFlag = 0;
/* Pictures position */
uint32_t Xpos1 = 0;
uint32_t Xpos2 = 160;
uint32_t Ypos1 = 0;
uint32_t Ypos2 = 32;

/* Private function prototypes -----------------------------------------------*/
static void LCD_SetHint(void);
static void LCD_Show_Feature(uint8_t feature);
static void LCD_2layers(void);
static void LCD_ColorKeying(void);
static void LCD_LayerInit(void);
static void LCD_ColorKeyingLayerInit(void);
static void PicturesPosition(uint32_t* x1, 
                         uint32_t* y1, 
                         uint32_t* x2, 
                         uint32_t* y2, 
                         uint32_t index);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  LCD demo
  * @param  None
  * @retval None
  */
void RGB_LCD_demo (void)
{
  LCD_SetHint();
  LCD_Feature = 0;
  LCD_Show_Feature (LCD_Feature);

  while (1)
  {

    if(TamperItOccurred == SET)
    {
      /* Add delay to avoid rebound and reset it status */
      HAL_Delay(500);
      TamperItOccurred = RESET;

      if (++LCD_Feature < LCD_FEATURES_NUM)
      {
        LCD_Show_Feature (LCD_Feature);
      }
      else
      {
        /* LCD re-init */
        BSP_LCD_DisplayOff();
        BSP_LCD_DeInit();
        
        BSP_LCD_Init();
#if defined(USE_ROCKTECH_480x272)
        /* Initialize the LCD Layers */
        BSP_RGB_LCD_LayerDefaultInit(LTDC_DEFAULT_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
//        BSP_RGB_LCD_LayerRgb565Init(LTDC_DEFAULT_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
#endif
        return;
      }
    }
    HAL_Delay(100);
  }
}

/**
  * @brief  Display LCD demo hint
  * @param  None
  * @retval None
  */
static void LCD_SetHint(void)
{
  /* Clear the LCD */
  BSP_RGB_LCD_Clear(LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  BSP_RGB_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_RGB_LCD_FillRect(0, 0, BSP_RGB_LCD_GetXSize(), 80);
  BSP_RGB_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_RGB_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_RGB_LCD_SetFont(&Font24);
  BSP_RGB_LCD_DisplayStringAt(0, 0, (uint8_t *)"LCD", CENTER_MODE);
  BSP_RGB_LCD_SetFont(&Font12);
  BSP_RGB_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows the different", CENTER_MODE);
  BSP_RGB_LCD_DisplayStringAt(0, 45, (uint8_t *)"LCD Features, use Wakeup push-button", CENTER_MODE);
  BSP_RGB_LCD_DisplayStringAt(0, 60, (uint8_t *)"to display next page", CENTER_MODE);

  /* Set the LCD Text Color */
  BSP_RGB_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_RGB_LCD_DrawRect(10, 90, BSP_RGB_LCD_GetXSize() - 20, BSP_RGB_LCD_GetYSize() - 100);
  BSP_RGB_LCD_DrawRect(11, 91, BSP_RGB_LCD_GetXSize() - 22, BSP_RGB_LCD_GetYSize() - 102);
}

/**
  * @brief  Show LCD Features
  * @param  feature : feature index
  * @retval None
  */
static void LCD_Show_Feature(uint8_t feature)
{
  Point Points[] = {{20, 150}, {80, 150}, {80, 200}};
  Point Points2[] = {{100, 150}, {160, 150}, {160, 200}};

  BSP_RGB_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_RGB_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_RGB_LCD_FillRect(12, 92, BSP_RGB_LCD_GetXSize() - 24, BSP_RGB_LCD_GetYSize() - 104);
  BSP_RGB_LCD_SetTextColor(LCD_COLOR_BLACK);

  switch (feature)
  {
    case 0:
      /* Text Feature */

      BSP_RGB_LCD_DisplayStringAt(14, 100, (uint8_t *)"Left aligned Text", LEFT_MODE);
      BSP_RGB_LCD_DisplayStringAt(0, 115, (uint8_t *)"Center aligned Text", CENTER_MODE);
      BSP_RGB_LCD_DisplayStringAt(14, 130, (uint8_t *)"Right aligned Text", RIGHT_MODE);
      BSP_RGB_LCD_SetFont(&Font24);
      BSP_RGB_LCD_DisplayStringAt(14, 180, (uint8_t *)"Font24", LEFT_MODE);
      BSP_RGB_LCD_SetFont(&Font20);
      BSP_RGB_LCD_DisplayStringAt(BSP_RGB_LCD_GetXSize() / 2 - 20, 180, (uint8_t *)"Font20", LEFT_MODE);
      BSP_RGB_LCD_SetFont(&Font16);
      BSP_RGB_LCD_DisplayStringAt(BSP_RGB_LCD_GetXSize() - 80, 184, (uint8_t *)"Font16", LEFT_MODE);
      break;

    case 1:

      /* Draw misc. Shapes */
      BSP_RGB_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_RGB_LCD_DrawRect(20, 100, 60 , 40);
      BSP_RGB_LCD_FillRect(100, 100, 60 , 40);

      BSP_RGB_LCD_SetTextColor(LCD_COLOR_GRAY);
      BSP_RGB_LCD_DrawCircle(BSP_RGB_LCD_GetXSize() - 120, 120, 20);
      BSP_RGB_LCD_FillCircle(BSP_RGB_LCD_GetXSize() - 40, 120, 20);

      BSP_RGB_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_RGB_LCD_DrawPolygon(Points, 3);
      BSP_RGB_LCD_FillPolygon(Points2, 3);

      BSP_RGB_LCD_SetTextColor(LCD_COLOR_RED);
      BSP_RGB_LCD_DrawEllipse(BSP_RGB_LCD_GetXSize() - 120, 170, 30, 20);
      BSP_RGB_LCD_FillEllipse(BSP_RGB_LCD_GetXSize() - 50, 170, 30, 20);

      BSP_RGB_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_RGB_LCD_DrawHLine(20, BSP_RGB_LCD_GetYSize() - 30, BSP_RGB_LCD_GetXSize() / 5);
      BSP_RGB_LCD_DrawLine (BSP_RGB_LCD_GetXSize() - 150, BSP_RGB_LCD_GetYSize() - 20, BSP_RGB_LCD_GetXSize() - 20, BSP_RGB_LCD_GetYSize() - 50);
      BSP_RGB_LCD_DrawLine (BSP_RGB_LCD_GetXSize() - 150, BSP_RGB_LCD_GetYSize() - 50, BSP_RGB_LCD_GetXSize() - 20, BSP_RGB_LCD_GetYSize() - 20);
      break;

    case 2:
      /* Draw Bitmap */
      BSP_RGB_LCD_DrawBitmap(20, 100, (uint8_t *)stlogo);
      HAL_Delay(100);

      BSP_RGB_LCD_DrawBitmap(BSP_RGB_LCD_GetXSize() / 2 - 40, 100, (uint8_t *)stlogo);
      HAL_Delay(100);

      BSP_RGB_LCD_DrawBitmap(BSP_RGB_LCD_GetXSize() - 100, 100, (uint8_t *)stlogo);
      HAL_Delay(100);

      BSP_RGB_LCD_DrawBitmap(20, BSP_RGB_LCD_GetYSize() - 80, (uint8_t *)stlogo);
      HAL_Delay(100);

      BSP_RGB_LCD_DrawBitmap(BSP_RGB_LCD_GetXSize() / 2 - 40, BSP_RGB_LCD_GetYSize() - 80, (uint8_t *)stlogo);
      HAL_Delay(100);

      BSP_RGB_LCD_DrawBitmap(BSP_RGB_LCD_GetXSize() - 100, BSP_RGB_LCD_GetYSize() - 80, (uint8_t *)stlogo);
      HAL_Delay(100);
      break;
  case 3:
    LCD_2layers();
    break;
  case 4:
    LCD_ColorKeying();
    break;
    default :
      break;
  }
}

/**
  * @brief  Show LCD 2 Layers Features
  * @note  Change Layer window, transparency and address without immediate reload.
  *        Reload takes effect at next vertical blanking
  * @retval None
  */
static void LCD_2layers(void)
{
  uint32_t index = 0,layer1_address, layer2_address, perm_address;
  BSP_RGB_LCD_DeInit();
  
  BSP_RGB_LCD_Init(); 
  LCD_LayerInit();
  
  /* Set LTDC Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(LTDC_IRQn, 0xE, 0);
  /* Enable LTDC Interrupt */
  HAL_NVIC_EnableIRQ(LTDC_IRQn);
  
  layer1_address = (uint32_t)&FLYING;
  layer2_address = (uint32_t)&RGB565_320x240;
  
  /* Infinite loop */
  while (1)
  { 
    for (index = 0; index < 32; index++)
    {
      /* calculate new picture position */
      PicturesPosition(&Xpos1, &Ypos1, &Xpos2, &Ypos2, (index+1));
      
      /* reconfigure the layer1 position  without Reloading*/
      BSP_RGB_LCD_SetLayerWindow_NoReload(LTDC_LAYER_1, Xpos1, Ypos1, 320, 240);
      /* reconfigure the layer2 position  without Reloading*/
      BSP_RGB_LCD_SetLayerWindow_NoReload(LTDC_LAYER_2, Xpos2, Ypos2, 320, 240);
      /* reconfigure the layer1 transparency  without Reloading*/
      BSP_RGB_LCD_SetTransparency_NoReload(LTDC_LAYER_1, 8*index);
      BSP_RGB_LCD_SetTransparency_NoReload(LTDC_LAYER_2, 8*index);
      /*Ask for LTDC reload within next vertical blanking*/
      ReloadFlag = 0;
      BSP_RGB_LCD_Reload(LCD_RELOAD_VERTICAL_BLANKING);
      
      while(ReloadFlag == 0)
      {
        /* wait till reload takes effect (in the next vertical blanking period) */
      }
      if(TamperItOccurred == SET)
      {
        /* Add delay to avoid rebound and reset it status */
        HAL_Delay(500);
        TamperItOccurred = RESET;
        
        return;
      }
      HAL_Delay(20);       
    }
    HAL_Delay(500);
    
    perm_address = layer1_address;
    
    layer1_address = layer2_address;
    
    layer2_address = perm_address;
    BSP_RGB_LCD_SetLayerAddress_NoReload(LTDC_LAYER_1, layer1_address);
    BSP_RGB_LCD_SetLayerAddress_NoReload(LTDC_LAYER_2, layer2_address);
    
  }  
}

/**
  * @brief  Show LCD 2 Layers Features
  * @note  Change Layer window, transparency and address without immediate reload.
  *        Reload takes effect at next vertical blanking
  * @retval None
  */
static void LCD_ColorKeying(void)
{
  BSP_RGB_LCD_DeInit();
  
  BSP_RGB_LCD_Init(); 
  LCD_ColorKeyingLayerInit();  
  BSP_RGB_LCD_SetLayerWindow(LTDC_DEFAULT_ACTIVE_LAYER, 0, 0, BSP_RGB_LCD_GetXSize(), BSP_RGB_LCD_GetYSize());  
  
  while(TamperItOccurred == RESET)
  {
    
    /* Enable Color Keying on layer 2 */
    BSP_RGB_LCD_SetColorKeying_NoReload(LTDC_LAYER_2, ARGB4444_COLOR_KEY);
    ReloadFlag = 0;
    BSP_RGB_LCD_Reload(LCD_RELOAD_VERTICAL_BLANKING);
    
    while(ReloadFlag == 0)
    {
      /* wait till reload takes effect (in the next vertical blanking period) */
    }
    HAL_Delay(1000);
    /* Disable Color Keying on layer 2 */
    BSP_RGB_LCD_ResetColorKeying_NoReload(LTDC_LAYER_2);  
    
    ReloadFlag = 0;
    BSP_RGB_LCD_Reload(LCD_RELOAD_VERTICAL_BLANKING);
    
    while(ReloadFlag == 0)
    {
      /* wait till reload takes effect (in the next vertical blanking period) */
    }
    HAL_Delay(1000);
  }
  /* Add delay to avoid rebound and reset it status */
  HAL_Delay(500);
  TamperItOccurred = RESET;

  BSP_RGB_LCD_DeInit();
  
  BSP_RGB_LCD_Init(); 
  
  /* Initialize the LCD Layers */
  BSP_RGB_LCD_LayerDefaultInit(LTDC_DEFAULT_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
  
  BSP_RGB_LCD_SetLayerWindow(LTDC_DEFAULT_ACTIVE_LAYER, 0, 0, BSP_RGB_LCD_GetXSize(), BSP_RGB_LCD_GetYSize());
  return;  
}
/**
  * @brief  Initializes the LCD layers.
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
static void LCD_LayerInit(void)
{     
  LTDC_LayerCfgTypeDef      pLayerCfg1;
  LTDC_LayerCfgTypeDef      pLayerCfg2;
/* Layer1 Configuration ------------------------------------------------------*/
  
  /* Windowing configuration */ 
  pLayerCfg1.WindowX0 = 0;
  pLayerCfg1.WindowX1 = 320;
  pLayerCfg1.WindowY0 = 0;
  pLayerCfg1.WindowY1 = 240;
  
  /* Pixel Format configuration*/ 
  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  
  /* Start Address configuration : frame buffer is located at FLASH memory */
  pLayerCfg1.FBStartAdress = (uint32_t)&FLYING;
  
  /* Alpha constant (255 totally opaque) */
  pLayerCfg1.Alpha = 255;
  
  /* Default Color configuration (configure A,R,G,B component values) */
  pLayerCfg1.Alpha0 = 0;
  pLayerCfg1.Backcolor.Blue = 0;
  pLayerCfg1.Backcolor.Green = 0;
  pLayerCfg1.Backcolor.Red = 0;
  
  /* Configure blending factors */
  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  
  /* Configure the number of lines and number of pixels per line */
  pLayerCfg1.ImageWidth = 320;
  pLayerCfg1.ImageHeight = 240;

/* Layer2 Configuration ------------------------------------------------------*/
  
  /* Windowing configuration */ 
  pLayerCfg2.WindowX0 = 160;
  pLayerCfg2.WindowX1 = 480;
  pLayerCfg2.WindowY0 = 32;
  pLayerCfg2.WindowY1 = 272;
  
  /* Pixel Format configuration*/ 
  pLayerCfg2.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  
  /* Start Address configuration : frame buffer is located at FLASH memory */
  pLayerCfg2.FBStartAdress = (uint32_t)&RGB565_320x240;
  
  /* Alpha constant (255 totally opaque) */
  pLayerCfg2.Alpha = 200;
  
  /* Default Color configuration (configure A,R,G,B component values) */
  pLayerCfg2.Alpha0 = 0;
  pLayerCfg2.Backcolor.Blue = 0;
  pLayerCfg2.Backcolor.Green = 0;
  pLayerCfg2.Backcolor.Red = 0;
  
  /* Configure blending factors */
  pLayerCfg2.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg2.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  
  /* Configure the number of lines and number of pixels per line */
  pLayerCfg2.ImageWidth = 320;
  pLayerCfg2.ImageHeight = 240;  
   
  /* Configure the LTDC */  
  HAL_LTDC_Init(&hltdc_eval);

  /* Configure the Background Layer*/
  HAL_LTDC_ConfigLayer(&hltdc_eval, &pLayerCfg1, LTDC_LAYER_1);

  
  /* Configure the Foreground Layer*/
  HAL_LTDC_ConfigLayer(&hltdc_eval, &pLayerCfg2, LTDC_LAYER_2); 
}

/**
  * @brief  Initializes the LCD layers.
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
static void LCD_ColorKeyingLayerInit(void)
{ 
    LTDC_LayerCfgTypeDef      pLayerCfg;
    
  /* Windowing configuration */ 
  /* In this case all the active display area is used to display a picture then :
     Horizontal start = horizontal synchronization + Horizontal back porch = 43 
     Vertical start   = vertical synchronization + vertical back porch     = 12
     Horizontal stop = Horizontal start + window width -1 = 43 + 480 -1 
     Vertical stop   = Vertical start + window height -1  = 12 + 272 -1      */
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 480;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 272;
  
  /* Pixel Format configuration */ 
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB4444;
  
  /* Start Address configuration : frame buffer is located in FLASH memory */
  pLayerCfg.FBStartAdress = (uint32_t)&ARGB4444_480x272;
  
  /* Alpha constant (255 totally opaque) */
  pLayerCfg.Alpha = 255;
  
  /* Default Color configuration (configure A,R,G,B component values) */
  /* default color == all transparent pixel                           */
  pLayerCfg.Alpha0 = 0; /* completely transparent */
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  
  /* Configure blending factors */
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  
  /* Configure the number of lines and number of pixels per line */
  pLayerCfg.ImageWidth  = 480;
  pLayerCfg.ImageHeight = 272;

  /* Configure the LTDC */  
  HAL_LTDC_Init(&hltdc_eval);
  
  /* Configure the Layer 2 */
  HAL_LTDC_ConfigLayer(&hltdc_eval, &pLayerCfg, LTDC_LAYER_2);   
}

/**
  * @brief  calculate pictures position.
  * @param  x1:    picture1 x position
  * @param  y1:    picture1 y position
  * @param  x2:    picture2 x position
  * @param  y2:    picture2 y position
  * @param  index: 
  * @retval None
  */
static void PicturesPosition(uint32_t* x1, uint32_t* y1, uint32_t* x2, uint32_t* y2, uint32_t index)
{
  /* picture1 position */
  *x1 = index*5;
  *y1 = index; 
  
  /* picture2 position */
  *x2 = 160 - index*5;
  *y2 = 32 - index;
}
/**
  * @brief  Reload Event callback.
  * @param  hltdc: pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
void HAL_LTDC_ReloadEventCallback(LTDC_HandleTypeDef *hltdc)
{
  ReloadFlag = 1;
}

/**
  * @}
  */

/**
  * @}
  */


