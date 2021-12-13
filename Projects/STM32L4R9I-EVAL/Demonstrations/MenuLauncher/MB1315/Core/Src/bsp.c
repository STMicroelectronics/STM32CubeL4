/**
  ******************************************************************************
  * @file    bsp.c
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
#include "bsp.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_BSP
  * @brief Kernel BSP routines
  * @{
  */

/* External variables --------------------------------------------------------*/
static UART_HandleTypeDef UART_Handle;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define STML_DESCRITION                 "ST Menu Launcher V1.0.0"
#define BOARD_NAME                      "STM32L4R9I-EVAL_MB1315"

#define MAX_FLASH_WRITE_FAILURE         10

#define USARTx                          USART3

/* Private macros ------------------------------------------------------------*/
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void BSP_ConfigSerial( void )
{
  UART_Handle.Instance            = USARTx;
  UART_Handle.Init.BaudRate       = 115200;
  UART_Handle.Init.WordLength     = UART_WORDLENGTH_8B;
  UART_Handle.Init.StopBits       = UART_STOPBITS_1;
  UART_Handle.Init.Parity         = UART_PARITY_NONE;
  UART_Handle.Init.Mode           = UART_MODE_TX_RX;
  UART_Handle.Init.HwFlowCtl      = UART_HWCONTROL_NONE;
  UART_Handle.Init.OverSampling   = UART_OVERSAMPLING_16;

  HAL_UART_Init( &UART_Handle );
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UART_Handle, (uint8_t *)&ch, 1, 0xFFFF); 

  return ch;
}

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
  * @brief  Initializes the STM324x9I-EVAL's LCD and LEDs resources.
  * @param  None
  * @retval None
  */
uint8_t BSP_Config(void)
{
  uint8_t RetVal = 0;

  /* Enable CRC to Unlock GUI */
  __HAL_RCC_CRC_CLK_ENABLE();

  BSP_LED_Init(LED_RED);

  /* Init Wakeup/Tamper push-button in GPIO Mode */
  BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_EXTI);
  
  BSP_ConfigSerial();
  
  printf( "\n\n%s: %s (Build %s at %s)\n", BOARD_NAME, STML_DESCRITION, __DATE__, __TIME__ );
  printf( "CPU running at %ldMHz, Peripherals at %ldMHz/%ldMHz\n", (HAL_RCC_GetHCLKFreq()/1000000U), (HAL_RCC_GetPCLK1Freq()/1000000U), (HAL_RCC_GetPCLK2Freq()/1000000U) );
  
  /* Initialize the NOR OctoSPI flash */
  if ((RetVal = BSP_OSPI_NOR_Init()) != OSPI_NOR_OK)
  {
    printf("Failed to initialize the OSPI !! (Error %d)\n", RetVal);
    return 0;
  }
  else
  {
    if((RetVal = BSP_OSPI_NOR_EnableMemoryMappedMode()) != OSPI_NOR_OK)
    {
      printf("Failed to configure the OSPI !! (Error %d)\n", RetVal);
      return 0;
    }
  }

  /* 
   * This code shall be called before calling the BSP_TS_Init() otherwise it will fail
   */
  {
    /* IO expander init */
    BSP_IO_Init();

    /* LCD_DISP & LCD_RST pins configuration (over MFX IO Expander) */
    BSP_IO_ConfigPin(LCD_RGB_DISP_PIN | LCD_RGB_RST_PIN, IO_MODE_OUTPUT);
    
    /* Screen initialization */
    BSP_IO_WritePin(LCD_RGB_RST_PIN, IO_PIN_RESET );
    HAL_Delay(2);
    BSP_IO_WritePin(LCD_RGB_RST_PIN, IO_PIN_SET );

    /* Assert display enable LCD_DISP pin via MFX expander */
    BSP_IO_WritePin(LCD_RGB_DISP_PIN, IO_PIN_SET);
  }

  /* Initialize the Touch screen */
  RetVal = BSP_TS_Init(480, 272);
  if(RetVal != TS_OK)
  {
    printf("Failed to initialize TS !! (Error %d)\n", RetVal);
    return 0;
  }
  
  return 1;
}

/**
  * @brief  Provide the GUI with current state of the touch screen
  * @param  None
  * @retval None
  */
void BSP_TouchUpdate(void)
{
  static GUI_PID_STATE TS_State = {0, 0, 0, 0};
  __IO TS_StateTypeDef  ts;
  uint16_t xDiff, yDiff;
  
  BSP_TS_GetState((TS_StateTypeDef *)&ts);

  if((ts.touchX[0] >= LCD_GetXSize()) ||(ts.touchY[0] >= LCD_GetYSize()) ) 
  {
    ts.touchX[0] = 0;
    ts.touchY[0] = 0;
  }

  xDiff = (TS_State.x > ts.touchX[0]) ? (TS_State.x - ts.touchX[0]) : (ts.touchX[0] - TS_State.x);
  yDiff = (TS_State.y > ts.touchY[0]) ? (TS_State.y - ts.touchY[0]) : (ts.touchY[0] - TS_State.y);
  
  if((TS_State.Pressed != ts.touchDetected ) ||
     (xDiff > 8 )||
       (yDiff > 8))
  {
    TS_State.Pressed = ts.touchDetected;
    TS_State.Layer = 0;
    if(ts.touchDetected) 
    {
      TS_State.x = ts.touchX[0];
      TS_State.y = ts.touchY[0] ;
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

/**
  * @brief  Jump to Sub demo application
  * @param  Address  : Address where the sub demonstration code is located
  * @retval None
  */
void BSP_JumpToSubDemo(uint32_t SubDemoAddress)
{
    /* Enable RTC back-up registers access */
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    /* Store the address of the Sub Demo binary */
    WRITE_REG(RTC->BKP26R, (uint32_t) SubDemoAddress);
    // LCD_X_DisplayDriver(0, LCD_X_OFF, NULL); 
    NVIC_SystemReset();
}

/**
  * @brief  Copy resource file into the OctoSPI memory
  * @param  hItem    : Progress bar used to indicate the transfer progression
  * @param  pResFile : Resource file to be copied in the OctoSPI memory
  * @param  Address  : Address where the resource will be copied
  * @retval ErrorCode : 0 if success -1 otherwise
  */
int BSP_ResourcesCopy(WM_HWIN hItem, FIL * pResFile, uint32_t Address)
{
  OSPI_NOR_Info OSPIInfo;
  uint32_t file_size = 0;
  uint32_t numOfReadBytes = 0, ospiAddr = 0, nbTotalBytes = 0;
  uint8_t ospiStatus, *pSdData;

  /* Sop Memory Mapping mode */
  BSP_OSPI_NOR_DeInit();

  /* Re-initialize NOR OctoSPI flash to exit memory-mapped mode */
  if (BSP_OSPI_NOR_Init() != OSPI_NOR_OK)
  {
    /* Initialization Error */
    return -1;
  }
  
  /* Initialize the structure */
  OSPIInfo.FlashSize          = (uint32_t)0x00;
  OSPIInfo.EraseSectorSize    = (uint32_t)0x00;
  OSPIInfo.EraseSectorsNumber = (uint32_t)0x00;
  OSPIInfo.ProgPageSize       = (uint32_t)0x00;
  OSPIInfo.ProgPagesNumber    = (uint32_t)0x00;

  if (BSP_OSPI_NOR_GetInfo(&OSPIInfo) != OSPI_NOR_OK)
  {
    return -1;
  }

  /* Test the correctness */
  if((OSPIInfo.FlashSize          != MX25LM51245G_FLASH_SIZE) || 
     (OSPIInfo.EraseSectorSize    != MX25LM51245G_BLOCK_SIZE) || 
     (OSPIInfo.ProgPageSize       != MX25LM51245G_PAGE_SIZE)  || 
     (OSPIInfo.EraseSectorsNumber != (MX25LM51245G_FLASH_SIZE/MX25LM51245G_BLOCK_SIZE)) ||
     (OSPIInfo.ProgPagesNumber    != (MX25LM51245G_FLASH_SIZE/MX25LM51245G_PAGE_SIZE)))
  {
    return -1;
  }
  
  pSdData = ff_malloc(OSPIInfo.EraseSectorSize);
  file_size = f_size(pResFile);
  if (pSdData == NULL)
  {
    return -1;
  }
  
  if (f_lseek(pResFile, 0) != FR_OK)
  {
    ff_free(pSdData);
    return -1;
  }

  ospiAddr = (Address - OCTOSPI1_BASE);  
  do
  {
    /* Erase and copy data sector by sector */
    if (BSP_OSPI_NOR_Erase_Block(ospiAddr) != OSPI_NOR_OK)
    {
      ff_free(pSdData);
      return -1;
    }
    
    /* Wait the end of erase operation */
    do
    {
      ospiStatus = BSP_OSPI_NOR_GetStatus();
    } while (ospiStatus == OSPI_NOR_BUSY);

    /* Check the erase operation correctness */
    if (ospiStatus != OSPI_NOR_OK)
    {
      ff_free(pSdData);
      return -1;
    }
    
    if(f_read(pResFile, pSdData, OSPIInfo.EraseSectorSize, (void *)&numOfReadBytes) != FR_OK)
    {
      ff_free(pSdData);
      return -1;
    }
    
    if (BSP_OSPI_NOR_Write(pSdData, ospiAddr, numOfReadBytes) != OSPI_NOR_OK)
    {
      ff_free(pSdData);
      return -1;
    }

    /* Wait the end of write operation */
    do
    {
      ospiStatus = BSP_OSPI_NOR_GetStatus();
    } while (ospiStatus == OSPI_NOR_BUSY);

    /* Check the write operation correctness */
    if (ospiStatus != OSPI_NOR_OK)
    {
      ff_free(pSdData);
      return -1;
    }

    ospiAddr      += numOfReadBytes;
    nbTotalBytes  += numOfReadBytes;

    PROGBAR_SetValue(hItem, nbTotalBytes);
    GUI_Exec1();

  } while((numOfReadBytes == OSPIInfo.EraseSectorSize) && (nbTotalBytes < file_size));

  ff_free(pSdData);
  
  /* Reconfigure memory mapped mode */
  if(BSP_OSPI_NOR_EnableMemoryMappedMode() != OSPI_NOR_OK)
  {
    return -1;
  }

  return 0;
}

/**
  * @brief  Copy program code file into the Internal Flash memory
  * @param  hItem    : Progress bar used to indicate the transfer progression
  * @param  pResFile : Program code file to be copied in the Internal Flash memory
  * @param  Address  : Address where the code will be copied
  * @retval ErrorCode : 0 if success -1 otherwise
  */
int BSP_FlashProgram(WM_HWIN hItem, FIL * pResFile, uint32_t Address)
{
  FLASH_EraseInitTypeDef EraseInitStruct;

  uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
  uint32_t PAGEError = 0;
  uint32_t numOfReadBytes = 0, nbTotalBytes = 0;
  uint32_t offset = 0;
  uint8_t *pSdData;
  uint64_t *pSdData64, *pFlashAdd64;

  pSdData = (uint8_t *)ff_malloc(FLASH_PAGE_SIZE);
  if (pSdData == NULL)
  {
    return -1;
  }

  if (f_lseek(pResFile, 0) != FR_OK)
  {
    return -1;
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
    return -1;
  }

  pFlashAdd64 = (uint64_t *)Address;
  do
  {
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
            ff_free(pSdData);
            return -1;
          }
        }
      }
      nbTotalBytes += numOfReadBytes;
    }
    
    PROGBAR_SetValue(hItem, nbTotalBytes);
    GUI_Exec1();

  } while(numOfReadBytes == FLASH_PAGE_SIZE);


  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();
  
  ff_free(pSdData);
  return 0;
}

/**
  * @brief  Initializes the SD MSP.
  * @param  hsd: SD handle
  * @param  Params
  * @retval None
  */
void BSP_SD_MspInit(SD_HandleTypeDef *hsd, void *Params)
{
  GPIO_InitTypeDef gpioinitstruct = {0};
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  RCC_PeriphClkInit;

  /* Check whether HSI48 is enabled or not */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);
  if(RCC_OscInitStruct.HSI48State != RCC_HSI48_ON)
  {
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
    RCC_OscInitStruct.HSI48State     = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      while(1) {}
    }
  }
  
  /* Configure the Eval SDMMC1 clock source. The clock is derived from the HSI48 */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SDMMC1;
  RCC_PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_HSI48;
  if(HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK)
  {
    while(1) {}
  }
  
  /* Enable SDMMC1 clock */
  __HAL_RCC_SDMMC1_CLK_ENABLE();
  
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  
  /* Common GPIO configuration */
  gpioinitstruct.Mode      = GPIO_MODE_AF_PP;
  gpioinitstruct.Pull      = GPIO_PULLUP;
  gpioinitstruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpioinitstruct.Alternate = GPIO_AF12_SDMMC1;
  
  /* GPIOC configuration */
  gpioinitstruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
  HAL_GPIO_Init(GPIOC, &gpioinitstruct);

  /* GPIOD configuration */
  gpioinitstruct.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &gpioinitstruct);

  /* NVIC configuration for SDMMC1 interrupts */
  HAL_NVIC_SetPriority(SDMMCx_IRQn, 4, 0);
  HAL_NVIC_EnableIRQ(SDMMCx_IRQn);
}

/**
  * @}
  */

/**
  * @}
  */

