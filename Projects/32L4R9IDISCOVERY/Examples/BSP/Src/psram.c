/**
  ******************************************************************************
  * @file    BSP/Src/psram.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the PSRAM Driver
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
#define BUFFER_SIZE         ((uint32_t)0x0100)
#define WRITE_READ_ADDR     ((uint32_t)0x0800)

/* Private macro -------------------------------------------------------------*/
#define PSRAM_TEXT_XPOS       50
#define PSRAM_TEXT_YPOS_START 150
#define PSRAM_TEXT_YPOS_INC   15

/* Private variables ---------------------------------------------------------*/
uint16_t psram_aTxBuffer[BUFFER_SIZE];
uint16_t psram_aRxBuffer[BUFFER_SIZE];
uint8_t ubSramDmaCplt = 0;

extern __IO JOYState_TypeDef JoyState;

/* Private function prototypes -----------------------------------------------*/
static void PSRAM_SetHint(void);
static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  PSRAM Demo
  * @param  None
  * @retval None
  */
void PSRAM_demo (void)
{ 
  uint16_t lcd_text_ypos = PSRAM_TEXT_YPOS_START;
  uint8_t ubSramWrite = 0, ubSramRead = 0, ubSramInit = 0, ubSramDmaWrite = 0, ubSramDmaRead = 0, ubSramCmp = 0, ubSramDmaCmp = 0;

  Fill_Buffer(psram_aRxBuffer, BUFFER_SIZE, 0xdead);   
  
  /*##-1- Configure the PSRAM device ##########################################*/
  /* PSRAM device configuration */ 
  if(BSP_PSRAM_Init() != PSRAM_OK)
  {
    ubSramInit++;
  }
  else
  {
    /*##-2- PSRAM memory read/write access ######################################*/  
    /* Fill the buffer to write */
    Fill_Buffer(psram_aTxBuffer, BUFFER_SIZE, 0xC20F);   
    
    /* Write data to the PSRAM memory */
    if(BSP_PSRAM_WriteData(PSRAM_DEVICE_ADDR + WRITE_READ_ADDR, psram_aTxBuffer, BUFFER_SIZE) != PSRAM_OK)
    {
      ubSramWrite++;
    } 
    else
    {
      /* Read back data from the PSRAM memory */
      if(BSP_PSRAM_ReadData(PSRAM_DEVICE_ADDR + WRITE_READ_ADDR, psram_aRxBuffer, BUFFER_SIZE) != PSRAM_OK)
      {
        ubSramRead++;
      } 
      else
      {
        /*##-3- Checking data integrity ############################################*/
        if(Buffercmp(psram_aRxBuffer, psram_aTxBuffer, BUFFER_SIZE) > 0)
        {
          ubSramCmp++;
        }
        else
        {    
          /*##-4- PSRAM memory read/write access in DMA mode ##########################*/  
          /* Fill the buffer to write */
          Fill_Buffer(psram_aTxBuffer, BUFFER_SIZE, 0xA1F0);   
          ubSramDmaCplt =0;
          
          /* Write data to the PSRAM memory */
          if(BSP_PSRAM_WriteData_DMA(PSRAM_DEVICE_ADDR + WRITE_READ_ADDR, psram_aTxBuffer, BUFFER_SIZE) != PSRAM_OK)
          {
            ubSramDmaWrite++;
          }   
          else
          {
            /* Wait end of DMA transfer */
            while (ubSramDmaCplt == 0)
            {
              if(MfxItOccurred == SET)
              {
                Mfx_Event();
                if(JoyState == JOY_RIGHT)
                {
                  break;
                }
              }
              BSP_LED_Toggle(LED_GREEN);
              HAL_Delay(100);
            }
            
            if(ubSramDmaCplt != 0)
            {
              ubSramDmaCplt =0;
              
              /* Read back data from the PSRAM memory */
              if(BSP_PSRAM_ReadData_DMA(PSRAM_DEVICE_ADDR + WRITE_READ_ADDR, psram_aRxBuffer, BUFFER_SIZE) != PSRAM_OK)
              {
                ubSramDmaRead++;
              }
              else
              {
                /* Wait end of DMA transfer */
                while (ubSramDmaCplt == 0)
                {
                  if(MfxItOccurred == SET)
                  {
                    Mfx_Event();
                    if(JoyState == JOY_RIGHT)
                    {
                      break;
                    }
                  }
                  BSP_LED_Toggle(LED_GREEN);
                  HAL_Delay(100);
                }
                
                if(ubSramDmaCplt != 0)
                {
                  /*##-5- Checking data integrity ############################################*/
                  if(Buffercmp(psram_aRxBuffer, psram_aTxBuffer, BUFFER_SIZE) > 0)
                  {
                    ubSramDmaCmp++;
                  }
                }
                else
                {
                  ubSramDmaRead++;
                }
              }
            }
            else
            {
              ubSramDmaWrite++;
            }
          }
        }
      }
    }
  }
  
  PSRAM_SetHint();
  
  while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
  if (ubSramInit == 0)
  {
    BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM Initialization : OK.", LEFT_MODE);
    lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
    
    if (ubSramWrite == 0)
    {
      BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM WRITE : OK.", LEFT_MODE);
      lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
      
      if (ubSramRead == 0)
      {
        BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM READ : OK.", LEFT_MODE);
        lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
        
        if (ubSramCmp == 0)
        {
          BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM COMPARE : OK.", LEFT_MODE);
          lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;

          if (ubSramDmaWrite == 0)
          {
            BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM DMA WRITE : OK.", LEFT_MODE);
            lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
            
            if (ubSramDmaRead == 0)
            {
              BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM DMA READ : OK.", LEFT_MODE);
              lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
              
              if (ubSramDmaCmp == 0)
              {
                BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM COMPARE : OK.", LEFT_MODE);
                lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
                BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM Test : OK.", LEFT_MODE);
                lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
              }
              else
              {
                BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM COMPARE : FAILED.", LEFT_MODE);
                lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
                BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM Test Aborted.", LEFT_MODE);
                lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
              }
            }
            else
            {
              BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM DMA READ : FAILED.", LEFT_MODE);
              lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
              BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM Test Aborted.", LEFT_MODE);
              lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
            }
          }
          else
          {
            BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM DMA WRITE : FAILED.", LEFT_MODE);
            lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
            BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM Test Aborted.", LEFT_MODE);
            lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
          }
        }
        else
        {
          BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM COMPARE : FAILED.", LEFT_MODE);
          lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
          BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM Test Aborted.", LEFT_MODE);
          lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
        }
      }
      else
      {
        BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM READ : FAILED.", LEFT_MODE);
        lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
        BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM Test Aborted.", LEFT_MODE);
        lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
      }
    }
    else
    {
      BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM WRITE : FAILED.", LEFT_MODE);
      lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
      BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM Test Aborted.", LEFT_MODE);
      lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
    }
  }
  else
  {
    BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM Initialization : FAILED.", LEFT_MODE);
    lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
    BSP_LCD_DisplayStringAt(PSRAM_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"PSRAM Test Aborted.", LEFT_MODE);
    lcd_text_ypos+=PSRAM_TEXT_YPOS_INC;
  }
  BSP_LCD_Refresh();

  while (1)
  {    
    if(MfxItOccurred == SET)
    {
      Mfx_Event();
      if(JoyState == JOY_RIGHT)
      {
        BSP_LED_Off(LED_GREEN);
        BSP_LED_Off(LED_ORANGE);
        return;
      }
    }
    if ((ubSramWrite == 0) && (ubSramRead == 0) && (ubSramInit == 0) && (ubSramDmaWrite == 0) &&
        (ubSramDmaRead == 0) && (ubSramCmp == 0) && (ubSramDmaCmp == 0))
    {
      BSP_LED_Toggle(LED_GREEN);
    }
    else
    {
      BSP_LED_Toggle(LED_ORANGE);
    }
    HAL_Delay(100);
  }
}

/**
  * @brief  Display PSRAM Demo Hint
  * @param  None
  * @retval None
  */
static void PSRAM_SetHint(void)
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
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"PSRAM", CENTER_MODE);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 65, (uint8_t *)"This example shows", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 80, (uint8_t *)"how to write and read", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 95, (uint8_t *)"data on the PSRAM", CENTER_MODE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(40, 140, BSP_LCD_GetXSize() - 80, BSP_LCD_GetYSize() - 240);
  BSP_LCD_DrawRect(41, 141, BSP_LCD_GetXSize() - 82, BSP_LCD_GetYSize() - 242);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
  BSP_LCD_Refresh();
}

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLength: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset)
{
  uint32_t tmpindex = 0;

  /* Put in global buffer different values */
  for (tmpindex = 0; tmpindex < uwBufferLength; tmpindex++ )
  {
    pBuffer[tmpindex] = tmpindex + uwOffset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  DMA transfer complete callback.
  * @param  hdma: pointer to a PSRAM_HandleTypeDef structure that contains
  *                the configuration information for PSRAM module.
  * @retval None
  */
void HAL_SRAM_DMA_XferCpltCallback(DMA_HandleTypeDef *hdma)
{
  ubSramDmaCplt++;
}

/**
  * @}
  */ 

/**
  * @}
  */ 
