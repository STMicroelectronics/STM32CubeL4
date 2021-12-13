/**
  ******************************************************************************
  * @file    BSP/Src/sd.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the SD Driver
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
#define SD_CARD_PRESENCE_POLLING_MODE         0
#define SD_CARD_PRESENCE_INTERRUPT_MODE      1

#define SD_CARD_PRESENCE_VALIDATION_MODE      SD_CARD_PRESENCE_INTERRUPT_MODE       

#define BLOCK_START_ADDR         0     /* Block start address      */
#define NUM_OF_BLOCKS            5     /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t aTxBuffer[BUFFER_WORDS_SIZE];
uint32_t aRxBuffer[BUFFER_WORDS_SIZE];
__IO uint32_t SDWriteStatus = 0, SDReadStatus = 0;

/* Private function prototypes -----------------------------------------------*/
static void SD_SetHint(void);
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SD Demo
  * @param  None
  * @retval None
  */
void SD_demo (void)
{
  uint8_t SD_state = MSD_OK;
  static uint8_t prev_status = 2;  /* Undefined state */

  SD_SetHint();

  while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 60, (uint8_t *)"SD Not Connected", CENTER_MODE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_Refresh();
  
  SD_state = BSP_SD_Init();

#if (SD_CARD_PRESENCE_VALIDATION_MODE == SD_CARD_PRESENCE_INTERRUPT_MODE)
  BSP_SD_ITConfig();
#endif
  
  if (SD_state != MSD_OK)
  {
    while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
    if(SD_state == MSD_ERROR_SD_NOT_PRESENT)
    {
      BSP_LCD_DisplayStringAt(45, 150, (uint8_t *)"SD shall be inserted", LEFT_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(45, 150, (uint8_t *)"SD Initialization : FAIL.", LEFT_MODE);
    }
    BSP_LCD_DisplayStringAt(45, 165, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
    BSP_LCD_Refresh();
  }
  else
  {
    while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
    BSP_LCD_DisplayStringAt(45, 150, (uint8_t *)"SD Initialization : OK.", LEFT_MODE);

    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_ClearStringLine(BSP_LCD_GetYSize() - 60);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 60,   (uint8_t *)"  SD Connected  ", CENTER_MODE);
    BSP_LCD_Refresh();
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    
    prev_status = SD_PRESENT;

    /* Wait 1 sec before start */
    HAL_Delay(1000);
    
#if 1
    SD_state = BSP_SD_Erase(BLOCK_START_ADDR, (BLOCKSIZE * NUM_OF_BLOCKS));
      /* Wait until SD card is ready to use for new operation */
      while(BSP_SD_GetCardState() != SD_TRANSFER_OK)
      {
      }
    if (SD_state != MSD_OK)
    {
      while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
      BSP_LCD_DisplayStringAt(45, 165, (uint8_t *)"SD ERASE : FAILED.", LEFT_MODE);
      BSP_LCD_DisplayStringAt(45, 180, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
      BSP_LCD_Refresh();
    }
    else
    {
      while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
      BSP_LCD_DisplayStringAt(45, 165, (uint8_t *)"SD ERASE : OK.", LEFT_MODE);
      BSP_LCD_Refresh();

      /* Fill the buffer to write */
      Fill_Buffer(aTxBuffer, BUFFER_WORDS_SIZE, 0x22FF);
      SD_state = BSP_SD_WriteBlocks_DMA(aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
        /* Wait for the write process is completed */
        while((SDWriteStatus == 0))
        {
        }
        SDWriteStatus = 0;
        
        /* Wait until SD cards are ready to use for new operation */
        while((BSP_SD_GetCardState() != SD_TRANSFER_OK))
        {
        } 
        
      if (SD_state != MSD_OK)
      {
        while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
        BSP_LCD_DisplayStringAt(45, 180, (uint8_t *)"SD WRITE : FAILED.", LEFT_MODE);
        BSP_LCD_DisplayStringAt(45, 195, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
        BSP_LCD_Refresh();
      }
      else
      {
        while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
        BSP_LCD_DisplayStringAt(45, 180, (uint8_t *)"SD WRITE : OK.", LEFT_MODE);
        BSP_LCD_Refresh();
        
        SD_state = BSP_SD_ReadBlocks_DMA(aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
          /* Wait for the read process is completed */
          while(SDReadStatus == 0)
          {
          }
          SDReadStatus = 0;
          
          /* Wait until SD card is ready to use for new operation */
          while(BSP_SD_GetCardState() != SD_TRANSFER_OK)
          {
          }         
        if (SD_state != MSD_OK)
        {
          while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
          BSP_LCD_DisplayStringAt(45, 195, (uint8_t *)"SD READ : FAILED.", LEFT_MODE);
          BSP_LCD_DisplayStringAt(45, 210, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
          BSP_LCD_Refresh();
        }
        else
        {
          while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
          BSP_LCD_DisplayStringAt(45, 195, (uint8_t *)"SD READ : OK.", LEFT_MODE);
          BSP_LCD_Refresh();
          
          if (Buffercmp(aTxBuffer, aRxBuffer, BUFFER_WORDS_SIZE) > 0)
          {
            while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
            BSP_LCD_DisplayStringAt(45, 210, (uint8_t *)"SD COMPARE : FAILED.", LEFT_MODE);
            BSP_LCD_DisplayStringAt(45, 225, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
            BSP_LCD_Refresh();
          }
          else
          {
            while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
            BSP_LCD_DisplayStringAt(45, 210, (uint8_t *)"SD Test : OK.", LEFT_MODE);
            BSP_LCD_DisplayStringAt(45, 225, (uint8_t *)"SD can be removed.", LEFT_MODE);
            BSP_LCD_Refresh();
          }
        }
      }
    }
#endif
  }

  while (1)
  {
#if (SD_CARD_PRESENCE_VALIDATION_MODE == SD_CARD_PRESENCE_INTERRUPT_MODE)
    /* Check MFX interrupt occurred */    
    if(MfxItOccurred == SET)
    {
      Mfx_Event();    
#endif
      if(JoyState == JOY_RIGHT)
      {
        /* Exit */
        return;
      }
      else
      {
        /* Check if the SD card is plugged in the slot */
        if (BSP_SD_IsDetected() != SD_PRESENT)
        {
          if(prev_status != SD_NOT_PRESENT)
          {
            BSP_SD_Init();
            prev_status = SD_NOT_PRESENT; 
            while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
            BSP_LCD_SetTextColor(LCD_COLOR_RED);
            BSP_LCD_ClearStringLine(BSP_LCD_GetYSize() - 60);
            BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 60, (uint8_t *)"SD Not Connected", CENTER_MODE);
            BSP_LCD_Refresh();
          }
        }
        else if (prev_status != SD_PRESENT)
        {
          while(BSP_LCD_IsFrameBufferAvailable() != LCD_OK);
          BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
          BSP_LCD_ClearStringLine(BSP_LCD_GetYSize() - 60);
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 60,   (uint8_t *)"  SD Connected  ", CENTER_MODE);
          BSP_LCD_Refresh();
          prev_status = SD_PRESENT;
        }
      }
#if (SD_CARD_PRESENCE_VALIDATION_MODE == SD_CARD_PRESENCE_INTERRUPT_MODE)
    }
#endif
  }
}

/**
  * @brief  Display SD Demo Hint
  * @param  None
  * @retval None
  */
static void SD_SetHint(void)
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
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"SD", CENTER_MODE);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 50, (uint8_t *)"Detect microSD card.", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 70, (uint8_t *)"Shows how the erase,", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 90, (uint8_t *)"write and read data.", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 110, (uint8_t *)"Press RIGHT to exit", CENTER_MODE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(40, 140, BSP_LCD_GetXSize() - 80, BSP_LCD_GetYSize()- 240);
  BSP_LCD_DrawRect(41, 141, BSP_LCD_GetXSize() - 82, BSP_LCD_GetYSize()- 242);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_Refresh();
}

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength)
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
  * @brief Tx Transfer completed callback
  * @param None
  * @retval None
  */
void BSP_SD_WriteCpltCallback(void)
{
  SDWriteStatus = 1;
}

/**
  * @brief Rx Transfer completed callback
  * @param None
  * @retval None
  */
void BSP_SD_ReadCpltCallback(void)
{
  SDReadStatus = 1; 
}

/**
  * @brief SDMMC error callback
  * @param None
  * @retval None
  */
void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
  Error_Handler();
}

/**
  * @}
  */

/**
  * @}
  */
