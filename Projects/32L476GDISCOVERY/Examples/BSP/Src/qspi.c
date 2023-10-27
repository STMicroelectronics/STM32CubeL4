/**
  ******************************************************************************
  * @file    BSP/Src/qspi.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the QSPI flash in the  
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
#define BUFFER_SIZE         ((uint32_t)0x0200)
#define WRITE_READ_ADDR     ((uint32_t)0x0050)
#define QSPI_BASE_ADDR      ((uint32_t)0x90000000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t qspi_aTxBuffer[BUFFER_SIZE];
uint8_t qspi_aRxBuffer[BUFFER_SIZE];

/* External variables --------------------------------------------------------*/
extern __IO FlagStatus KeyPressed;

/* Private function prototypes -----------------------------------------------*/
static void     Fill_Buffer (uint8_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t  Buffercmp   (uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  QSPI Demo
  * @param  None
  * @retval None
  */
void QSPI_demo (void)
{ 
  /* QSPI info structure */
  static QSPI_Info pQSPI_Info;
  uint8_t status;
  __IO uint8_t *data_ptr;
  uint32_t index;
  Led_TypeDef led = LED_RED;
  
  /*##-1- Configure the QSPI device ##########################################*/
  /* QSPI device configuration */ 
  status = BSP_QSPI_Init();
  
  if (status != QSPI_OK)
  {
    BSP_LCD_GLASS_ScrollSentence((uint8_t *)"QSPI Initialization : FAILED.", 1, SCROLL_SPEED_HIGH);
  }
  else
  {
    /*##-2- Read & check the QSPI info #######################################*/
    /* Initialize the structure */
    pQSPI_Info.FlashSize          = (uint32_t)0x00;
    pQSPI_Info.EraseSectorSize    = (uint32_t)0x00;
    pQSPI_Info.EraseSectorsNumber = (uint32_t)0x00;
    pQSPI_Info.ProgPageSize       = (uint32_t)0x00;
    pQSPI_Info.ProgPagesNumber    = (uint32_t)0x00;
    
    /* Read the QSPI memory info */
    BSP_QSPI_GetInfo(&pQSPI_Info);
    
    /* Test the correctness */
    if((pQSPI_Info.FlashSize != 0x1000000) || (pQSPI_Info.EraseSectorSize != 0x1000)  || 
       (pQSPI_Info.ProgPageSize != 0x100)  || (pQSPI_Info.EraseSectorsNumber != 4096) ||
       (pQSPI_Info.ProgPagesNumber != 65536))
    {
      BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      QSPI GET INFO : FAILED.", 1, SCROLL_SPEED_HIGH);
    }
    else
    {
      /*##-3- Erase QSPI memory ################################################*/ 
      if(BSP_QSPI_Erase_Block(WRITE_READ_ADDR) != QSPI_OK)
      {
        BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      QSPI ERASE : FAILED.", 1, SCROLL_SPEED_HIGH);
      }
      else
      {
        /*##-4- QSPI memory read/write access  ###############################*/   
        /* Fill the buffer to write */
        Fill_Buffer(qspi_aTxBuffer, BUFFER_SIZE, 0xD20F);   
        
        /* Write data to the QSPI memory */
        if(BSP_QSPI_Write(qspi_aTxBuffer, WRITE_READ_ADDR, BUFFER_SIZE) != QSPI_OK)
        {
          BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      QSPI WRITE : FAILED.", 1, SCROLL_SPEED_HIGH);
        }
        else
        {
          /* Read back data from the QSPI memory */
          if(BSP_QSPI_Read(qspi_aRxBuffer, WRITE_READ_ADDR, BUFFER_SIZE) != QSPI_OK)
          {
            BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      QSPI READ : FAILED.", 1, SCROLL_SPEED_HIGH);
          }
          else
          {
            /*##-5- Checking data integrity ##################################*/  
            if(Buffercmp(qspi_aRxBuffer, qspi_aTxBuffer, BUFFER_SIZE) > 0)
            {
              BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      QSPI COMPARE : FAILED.", 1, SCROLL_SPEED_HIGH);
            }
            else
            {    
              /*##-6- QSPI memory in memory-mapped mode#######################*/
              if(BSP_QSPI_EnableMemoryMappedMode() != QSPI_OK)
              {
                BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      QSPI MEMORY-MAPPED CFG : FAILED.", 1, SCROLL_SPEED_HIGH);
              }
              else
              {
                for(index = 0, data_ptr = (__IO uint8_t *)(QSPI_BASE_ADDR + WRITE_READ_ADDR); 
                    index < BUFFER_SIZE; index++, data_ptr++)
                {
                  if(*data_ptr != qspi_aTxBuffer[index])
                  {
                    BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      QSPI MEMORY-MAPPED ACCESS : FAILED.", 1, SCROLL_SPEED_HIGH);
                    break;
                  }
                }
                
                if(index == BUFFER_SIZE)
                {
                    BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      QSPI Test OK.", 1, SCROLL_SPEED_HIGH);
                    led = LED_GREEN;
                }
              }
            }  
          }
        }
      }
    }
  }
  
  while (1)
  {    
    if(CheckForUserInput() > 0)
    {
      BSP_LED_Off(led);
      return;
    }
    BSP_LED_Toggle(led);
    HAL_Delay(100);
  }
}

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint8_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
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
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
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
  * @}
  */ 

/**
  * @}
  */ 

