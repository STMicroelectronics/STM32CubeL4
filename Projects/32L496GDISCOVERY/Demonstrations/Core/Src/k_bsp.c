/**
  ******************************************************************************
  * @file    k_bsp.c
  * @author  MCD Application Team 
  * @brief   This file provides the kernel bsp functions
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
#include "k_bsp.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_BSP
  * @brief Kernel BSP routines
  * @{
  */

/* External variables --------------------------------------------------------*/
TS_StateTypeDef  TS_State = {0};

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define MAX_FLASH_WRITE_FAILURE   10

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr)
{
  uint32_t page = 0;
  
  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }
  
  return page;
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
static uint32_t GetBank(uint32_t Addr)
{
  uint32_t bank = 0;
  
  if (READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == 0)
  {
  	/* No Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_1;
    }
    else
    {
      bank = FLASH_BANK_2;
    }
  }
  else
  {
  	/* Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_2;
    }
    else
    {
      bank = FLASH_BANK_1;
    }
  }
  
  return bank;
}

/**
  * @brief  Initializes QSPI and CRC.
  * @param  None 
  * @retval None
  */
void k_BspInit(void)
{  

  /* Configure the QSPI device */
  if (BSP_QSPI_Init() != QSPI_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  else
  {
    if(BSP_QSPI_EnableMemoryMappedMode() != QSPI_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
  }

  /* LCD Init */
  if (BSP_LCD_InitEx(LCD_ORIENTATION_PORTRAIT) == LCD_ERROR)
  {
    /* Initialization Error */
    Error_Handler();
  }

  BSP_TS_InitEx(BSP_LCD_GetXSize(), BSP_LCD_GetYSize(), BSP_LCD_GetOrientation());  

  /* DO not Enable TS interrupt to avoid conflict w/ calibration*/
  //if (BSP_TS_ITConfig() != TS_OK) {
  //  Error_Handler();
  //}
  
  /* Enable CRC to Unlock GUI */
  __HAL_RCC_CRC_CLK_ENABLE();  
}

/**
  * @brief  Read the coordinate of the point touched and assign their
  *         value to the variables u32_TSXCoordinate and u32_TSYCoordinate
  * @param  None
  * @retval None
  */
void k_TouchUpdate(void)
{
  static GUI_PID_STATE TS_State = {0, 0, 0, 0};
  __IO TS_StateTypeDef  ts;

  BSP_TS_GetState((TS_StateTypeDef *)&ts);
  
  ts.touchX[0] = TouchScreen_Get_Calibrated_X(ts.touchX[0]);
  ts.touchY[0] = TouchScreen_Get_Calibrated_Y(ts.touchY[0]);
  
  if((ts.touchX[0] >= LCD_GetXSize()) ||(ts.touchY[0] >= LCD_GetYSize()) ) 
  {
    ts.touchX[0] = 0;
    ts.touchY[0] = 0;
  }
  
  if((TS_State.Pressed != ts.touchDetected )||
    (TS_State.x != ts.touchX[0]) ||
      (TS_State.y != ts.touchY[0]))
{
  TS_State.Pressed = ts.touchDetected;
  if(ts.touchDetected) 
  {
    TS_State.x = ts.touchX[0];
    TS_State.y = ts.touchY[0];
    GUI_TOUCH_StoreStateEx(&TS_State);
  }
  else
  {
    GUI_TOUCH_StoreStateEx(&TS_State);
    TS_State.x = 0;
    TS_State.y = 0;      
  }
}
}

///**
//  * @brief  Initializes the SD MSP.
//  * @param  hsd: SD handle
//  * @param  Params
//  * @retval None
//  */
//void BSP_SD_MspInit(SD_HandleTypeDef *hsd, void *Params)
//{
//  GPIO_InitTypeDef gpio_init_structure;
//
//  /* Enable SDIO clock */
//  __HAL_RCC_SDIO_CLK_ENABLE();
//  
//  /* Enable DMA2 clocks */
//  __DMAx_TxRx_CLK_ENABLE();
//
//  /* Enable GPIOs clock */
//  __HAL_RCC_GPIOC_CLK_ENABLE();
//  __HAL_RCC_GPIOD_CLK_ENABLE();
//  
//  /* Common GPIO configuration */
//  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
//  gpio_init_structure.Pull      = GPIO_PULLUP;
//  gpio_init_structure.Speed     = GPIO_SPEED_HIGH;
//  gpio_init_structure.Alternate = GPIO_AF12_SDIO;
//  
//  /* GPIOC configuration */
//  gpio_init_structure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
//   
//  HAL_GPIO_Init(GPIOC, &gpio_init_structure);
//
//  /* GPIOD configuration */
//  gpio_init_structure.Pin = GPIO_PIN_2;
//  HAL_GPIO_Init(GPIOD, &gpio_init_structure);
//
//  /* NVIC configuration for SDIO interrupts */
//  HAL_NVIC_SetPriority(SDIO_IRQn, 0x0E, 0x00);
//  HAL_NVIC_EnableIRQ(SDIO_IRQn);
//}

/**
  * @brief  Copy resource file into the QSPI memory
  * @param  hItem    : Progress bar used to indicate the transfer progression
  * @param  pResFile : Resource file to be copied in the QSPI memory
  * @param  Address  : Address where the resource will be copied
  * @retval None
  */
void k_ResourcesCopy(WM_HWIN hItem, FIL * pResFile, uint32_t Address)
{
  QSPI_Info QspiInfo;
  uint32_t numOfReadBytes, sectorNb, qspiAddr, nbTotalBytes;
  uint8_t qspiStatus, *pSdData;

  /* Re-initialize QSPI to exit memory-mapped mode */
  if (BSP_QSPI_Init() != QSPI_OK)
  {
    Error_Handler();
  }
  
  if (BSP_QSPI_GetInfo(&QspiInfo) != QSPI_OK)
  {
    Error_Handler();
  }
  
  qspiAddr = Address - QSPI_BASE;
  sectorNb = qspiAddr / QspiInfo.EraseSectorSize;
  nbTotalBytes = 0;
  pSdData = malloc(QspiInfo.EraseSectorSize);
  if (pSdData == NULL)
  {
    Error_Handler();
  }
  
  if (f_lseek(pResFile, 0) != FR_OK)
  {
    Error_Handler();
  }
  
  do
  {
    PROGBAR_SetValue(hItem, nbTotalBytes);
    GUI_Delay(1);

    /* Erase and copy data sector by sector */
    if (BSP_QSPI_Erase_Sector(sectorNb) != QSPI_OK)
    {
      Error_Handler();
    }
    
    /* Wait the end of erase */
    do
    {
      qspiStatus = BSP_QSPI_GetStatus();
    } while (qspiStatus == QSPI_BUSY);
    
    if (qspiStatus == QSPI_ERROR)
    {
      Error_Handler();
    }
    
    if(f_read(pResFile, pSdData, QspiInfo.EraseSectorSize, (void *)&numOfReadBytes) != FR_OK)
    {
      Error_Handler();
    }
    
    if (BSP_QSPI_Write(pSdData, qspiAddr, numOfReadBytes) != QSPI_OK)
    {
      Error_Handler();
    }
    
    sectorNb++;
    qspiAddr += QspiInfo.EraseSectorSize;
    nbTotalBytes += numOfReadBytes;
  } while((numOfReadBytes == QspiInfo.EraseSectorSize) && (sectorNb < QspiInfo.EraseSectorsNumber));
  
  free(pSdData);
}

/**
  * @brief  Jump to Sub demo application
  * @param  Address  : Address where the sub demonstration code is located
  * @retval None
  */
void k_JumpToSubDemo(__IO uint32_t BackUpReg, uint32_t SubDemoAddress)
{
    /* Enable RTC back-up registers access */
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    /* Store the address of the Sub Demo binary */
    WRITE_REG(RTC->BKP30R, (uint32_t) SubDemoAddress);
    LCD_X_DisplayDriver(0, LCD_X_OFF, NULL); 
    NVIC_SystemReset();
}

/**
  * @brief  Copy resource file into the QSPI memory
  * @param  hItem    : Progress bar used to indicate the transfer progression
  * @param  pResFile : Resource file to be copied in the QSPI memory
  * @param  Address  : Address where the resource will be copied
  * @retval None
  */
void k_FlashProgram(WM_HWIN hItem, FIL * pResFile, uint32_t Address)
{
  FLASH_EraseInitTypeDef EraseInitStruct;

  uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
  uint32_t PAGEError = 0;
  uint32_t numOfReadBytes = 0, nbTotalBytes = 0;
  uint32_t offset = 0;
  uint8_t *pSdData;
  uint64_t *pSdData64, *pFlashAdd64;

  pSdData = (uint8_t *)malloc(FLASH_PAGE_SIZE);
  if (pSdData == NULL)
  {
    Error_Handler();
  }

  if (f_lseek(pResFile, 0) != FR_OK)
  {
    Error_Handler();
  }

  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR); 
  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();
  /* Clear all error flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);  

  /* Get the 1st page to erase */
  FirstPage = GetPage(Address);
  /* Get the number of pages to erase from 1st page */
  NbOfPages = GetPage(Address + f_size(pResFile)) - FirstPage + 1;
  /* Get the bank */
  BankNumber = GetBank(Address);

  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks       = BankNumber;
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;

  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
  {
    BSP_LCD_DisplayStringAt(0, 240 - 65, (uint8_t *)"Flash erase error", CENTER_MODE);
    Error_Handler();
  }

  pFlashAdd64 = (uint64_t *)Address;
  do
  {
    PROGBAR_SetValue(hItem, nbTotalBytes);
    GUI_Delay(1);

    if(f_read(pResFile, pSdData, FLASH_PAGE_SIZE, (void *)&numOfReadBytes) == FR_OK)
    {
      uint8_t failure = 0;
      offset          = 0;
      pSdData64       = (uint64_t *)pSdData;
      /* Program the user Flash area word by word */
      while (offset < numOfReadBytes)
      {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t )pFlashAdd64, *pSdData64) == HAL_OK)
        {
          pSdData64++;
          pFlashAdd64++;
          offset += 8;
          failure = 0;
        }
        else
        {
          if (failure++ > MAX_FLASH_WRITE_FAILURE)
          {
            BSP_LCD_DisplayStringAt(0, 240 - 65, (uint8_t *)"Flash program error", CENTER_MODE);
            Error_Handler();
          }
        }
      }
      nbTotalBytes += numOfReadBytes;
    }
  } while(numOfReadBytes == FLASH_PAGE_SIZE);


  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();
  
  free(pSdData);
}

/**
  * @}
  */

/**
  * @}
  */

