/**
******************************************************************************
* @file    BSP/Src/ospi_nor.c
* @author  MCD Application Team
* @brief   This example code shows how to use the OSPI Driver
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
#define OSPI_NOR_BUFFER_SIZE     ((uint32_t)0x0200)
#define OSPI_NOR_WRITE_READ_ADDR ((uint32_t)0x0050)
#define OSPI_NOR_BASE_ADDR  ((uint32_t)0x90000000)

/* Private macro -------------------------------------------------------------*/
#define LCD_TEXT_XPOS       20
#define LCD_TEXT_YPOS_START 80
#define LCD_TEXT_YPOS_INC   10

/* Private variables ---------------------------------------------------------*/
uint8_t ospi_nor_aTxBuffer[OSPI_NOR_BUFFER_SIZE];
uint8_t ospi_nor_aRxBuffer[MX25LM51245G_SECTOR_SIZE];

/* External variables --------------------------------------------------------*/
extern __IO FlagStatus TamperItOccurred;

/* Private function prototypes -----------------------------------------------*/
static void     OSPI_NOR_SetHint(void);
static void     Fill_Buffer (uint8_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t  Buffercmp   (uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);
static uint8_t  DataCmp     (uint8_t* pBuffer, uint8_t Pattern, uint32_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
* @brief  OSPI NOR Demo
* @param  None
* @retval None
*/
void OSPI_NOR_demo (void)
{ 
  /* OSPI info structure */
  static OSPI_NOR_Info pOSPI_NOR_Info;
  uint8_t status;
  __IO uint8_t *data_ptr;
  uint32_t index;
  Led_TypeDef led = LED_ORANGE;
  uint16_t lcd_text_ypos = LCD_TEXT_YPOS_START;
  
  OSPI_NOR_SetHint();
  
  /********************************** PART 1 **********************************/
  BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"Part I: Write & Read, Data Integrity, Memory mapped", LEFT_MODE);
  lcd_text_ypos+=(LCD_TEXT_YPOS_INC * 2);
  
  /*##-1- Configure the OSPI NOR device ######################################*/
  /* OSPI NOR device configuration */ 
  status = BSP_OSPI_NOR_Init();
  
  if (status != OSPI_NOR_OK)
  {
    BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Initialization : Failed", LEFT_MODE);
    lcd_text_ypos+=LCD_TEXT_YPOS_INC;
    BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Initialization : OK", LEFT_MODE);
    lcd_text_ypos+=LCD_TEXT_YPOS_INC;

    /*##-2- Read & check the OSPI NOR info ###################################*/
    /* Initialize the structure */
    pOSPI_NOR_Info.FlashSize          = (uint32_t)0x00;
    pOSPI_NOR_Info.EraseSectorSize    = (uint32_t)0x00;
    pOSPI_NOR_Info.EraseSectorsNumber = (uint32_t)0x00;
    pOSPI_NOR_Info.ProgPageSize       = (uint32_t)0x00;
    pOSPI_NOR_Info.ProgPagesNumber    = (uint32_t)0x00;
    
    /* Read the OSPI NOR memory info */
    BSP_OSPI_NOR_GetInfo(&pOSPI_NOR_Info);
    
    /* Test the correctness */
    if((pOSPI_NOR_Info.FlashSize          != MX25LM51245G_FLASH_SIZE) || 
       (pOSPI_NOR_Info.EraseSectorSize    != MX25LM51245G_BLOCK_SIZE)  || 
       (pOSPI_NOR_Info.ProgPageSize       != MX25LM51245G_PAGE_SIZE)  || 
       (pOSPI_NOR_Info.EraseSectorsNumber != (MX25LM51245G_FLASH_SIZE/MX25LM51245G_BLOCK_SIZE)) ||
       (pOSPI_NOR_Info.ProgPagesNumber    != (MX25LM51245G_FLASH_SIZE/MX25LM51245G_PAGE_SIZE)))
    {
      BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Get Info : Failed", LEFT_MODE);
      lcd_text_ypos+=LCD_TEXT_YPOS_INC;
      BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Get Info : OK", LEFT_MODE);
      lcd_text_ypos+=LCD_TEXT_YPOS_INC;
      
      /*##-3- Erase OSPI NOR memory ##########################################*/ 
      if(BSP_OSPI_NOR_Erase_Block(OSPI_NOR_WRITE_READ_ADDR) != OSPI_NOR_OK)
      {
        BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Erase : Failed", LEFT_MODE);
        lcd_text_ypos+=LCD_TEXT_YPOS_INC;
        BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
      }
      else
      {
        BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Erase : OK", LEFT_MODE);
        lcd_text_ypos+=LCD_TEXT_YPOS_INC;

        /*##-4- OSPI NOR memory read/write access  ###########################*/   
        /* Fill the buffer to write */
        Fill_Buffer(ospi_nor_aTxBuffer, OSPI_NOR_BUFFER_SIZE, 0xD20F);   
        
        /* Write data to the OSPI NOR memory */
        if(BSP_OSPI_NOR_Write(ospi_nor_aTxBuffer, OSPI_NOR_WRITE_READ_ADDR, OSPI_NOR_BUFFER_SIZE) != OSPI_NOR_OK)
        {
          BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Write : Failed", LEFT_MODE);
          lcd_text_ypos+=LCD_TEXT_YPOS_INC;
          BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
        }
        else
        {
          BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Write : OK", LEFT_MODE);
          lcd_text_ypos+=LCD_TEXT_YPOS_INC;

          /* Read back data from the OSPI NOR memory */
          if(BSP_OSPI_NOR_Read(ospi_nor_aRxBuffer, OSPI_NOR_WRITE_READ_ADDR, OSPI_NOR_BUFFER_SIZE) != OSPI_NOR_OK)
          {
            BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Read : Failed", LEFT_MODE);
            lcd_text_ypos+=LCD_TEXT_YPOS_INC;
            BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
          }
          else
          {
            BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Read : OK", LEFT_MODE);
            lcd_text_ypos+=LCD_TEXT_YPOS_INC;

            /*##-5- Checking data integrity ##################################*/  
            if(Buffercmp(ospi_nor_aRxBuffer, ospi_nor_aTxBuffer, OSPI_NOR_BUFFER_SIZE) > 0)
            {
              BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Compare : Failed", LEFT_MODE);
              lcd_text_ypos+=LCD_TEXT_YPOS_INC;
              BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
            }
            else
            {    
              BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Compare : OK", LEFT_MODE);
              lcd_text_ypos+=LCD_TEXT_YPOS_INC;

              /*##-6- OSPI NOR memory in memory-mapped mode###################*/
              if(BSP_OSPI_NOR_EnableMemoryMappedMode() != OSPI_NOR_OK)
              {
                BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Mem-Mapped Cfg : Failed", LEFT_MODE);
                lcd_text_ypos+=LCD_TEXT_YPOS_INC;
                BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
              }
              else
              {
                BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Mem-Mapped Cfg : OK", LEFT_MODE);
                lcd_text_ypos+=LCD_TEXT_YPOS_INC;

                for(index = 0, data_ptr = (__IO uint8_t *)(OSPI_NOR_BASE_ADDR + OSPI_NOR_WRITE_READ_ADDR); 
                    index < OSPI_NOR_BUFFER_SIZE; index++, data_ptr++)
                {
                  if(*data_ptr != ospi_nor_aTxBuffer[index])
                  {
                    BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Mem-Mapped Access : Failed", LEFT_MODE);
                    lcd_text_ypos+=LCD_TEXT_YPOS_INC;
                    BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
                    break;
                  }
                }
                
                if(index == OSPI_NOR_BUFFER_SIZE)
                {
                  BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Mem-Mapped Access : OK", LEFT_MODE);
                  lcd_text_ypos+=LCD_TEXT_YPOS_INC;
                  BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test : OK", LEFT_MODE);
                  led = LED_GREEN;
                }
              }
            }  
          }
        }
      }
    }
  }
  
  lcd_text_ypos+=(LCD_TEXT_YPOS_INC*2);
  BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"Press Wakeup push-button to switch to next part", CENTER_MODE);

  while (1)
  {    
    if(TamperItOccurred == SET)
    {
      /* Add delay to avoid rebound and reset it status */
      HAL_Delay(500);
      TamperItOccurred = RESET;

      break;
    }
    BSP_LED_Toggle(led);
    HAL_Delay(100);
  }

  /********************************** PART 2 **********************************/
  /* Suspend/Resume during erase operation */
  led = LED_ORANGE;
  lcd_text_ypos = LCD_TEXT_YPOS_START;
  OSPI_NOR_SetHint();
  
  BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"Part II : Erase Suspend & Resume", LEFT_MODE);
  lcd_text_ypos+=(LCD_TEXT_YPOS_INC * 2);

  /*##-1- Deconfigure the OSPI NOR device ####################################*/
  status = BSP_OSPI_NOR_DeInit();
  
  if (status != OSPI_NOR_OK)
  {
    BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR De-Initialization : Failed", LEFT_MODE);
    lcd_text_ypos+=LCD_TEXT_YPOS_INC;
    BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR De-Initialization : OK", LEFT_MODE);
    lcd_text_ypos+=LCD_TEXT_YPOS_INC;

    /*##-2- Reconfigure the OSPI NOR device ##################################*/
    /* QSPI device configuration */ 
    status = BSP_OSPI_NOR_Init();
    
    if (status != OSPI_NOR_OK)
    {
      BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Initialization : Failed", LEFT_MODE);
      lcd_text_ypos+=LCD_TEXT_YPOS_INC;
      BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Initialization : OK", LEFT_MODE);
      lcd_text_ypos+=LCD_TEXT_YPOS_INC;
      
      /*##-3- Erase OSPI NOR memory ##########################################*/ 
      if(BSP_OSPI_NOR_Erase_Sector(0) != OSPI_NOR_OK)
      {
        BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Erase : Failed", LEFT_MODE);
        lcd_text_ypos+=LCD_TEXT_YPOS_INC;
        BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
      }
      else
      {

        /*##-4- Suspend erase OSPI NOR memory ################################*/
        if(BSP_OSPI_NOR_SuspendErase() != OSPI_NOR_OK)
        {
          BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Erase : OK", LEFT_MODE);
          lcd_text_ypos+=LCD_TEXT_YPOS_INC;
          BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Erase Suspend : Failed", LEFT_MODE);
          lcd_text_ypos+=LCD_TEXT_YPOS_INC;
          BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
        }
        else
        {
          BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Erase : OK", LEFT_MODE);
          lcd_text_ypos+=LCD_TEXT_YPOS_INC;
          BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Erase Suspend : OK", LEFT_MODE);
          lcd_text_ypos+=LCD_TEXT_YPOS_INC;
          
          /*##-6- Resume erase OSPI NOR memory ###############################*/ 
          if(BSP_OSPI_NOR_ResumeErase() != OSPI_NOR_OK)
          {
            BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Erase Resume : Failed", LEFT_MODE);
            lcd_text_ypos+=LCD_TEXT_YPOS_INC;
            BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
          }
          else
          {
            BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Erase Resume : OK", LEFT_MODE);
            lcd_text_ypos+=LCD_TEXT_YPOS_INC;
            
            /*##-7- Check OSPI NOR memory status  ############################*/   
            /* Wait the end of the current operation on memory side */
            do
            {
              status = BSP_OSPI_NOR_GetStatus();
            } while((status != OSPI_NOR_OK) && (status != OSPI_NOR_ERROR));
            
            if(status != OSPI_NOR_OK)
            {
              BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Memory Status : Failed", LEFT_MODE);
              lcd_text_ypos+=LCD_TEXT_YPOS_INC;
              BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
            }
            else
            {
              BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Memory Status : OK", LEFT_MODE);
              lcd_text_ypos+=LCD_TEXT_YPOS_INC;
              
              /*##-8- OSPI NOR memory read access  ###########################*/   
              /* Read back data from the OSPI NOR memory */
              if(BSP_OSPI_NOR_Read(ospi_nor_aRxBuffer, 0, MX25LM51245G_SECTOR_SIZE) != OSPI_NOR_OK)
              {
                BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Read : Failed", LEFT_MODE);
                lcd_text_ypos+=LCD_TEXT_YPOS_INC;
                BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
              }
              else
              {
                BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Read : OK", LEFT_MODE);
                lcd_text_ypos+=LCD_TEXT_YPOS_INC;
                
                /*##-9- Checking data integrity ##############################*/  
                if(DataCmp(ospi_nor_aRxBuffer, 0xFF, MX25LM51245G_SECTOR_SIZE) > 0)
                {
                  BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Compare : Failed", LEFT_MODE);
                  lcd_text_ypos+=LCD_TEXT_YPOS_INC;
                  BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
                }
                else
                {    
                  BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Compare : OK", LEFT_MODE);
                  lcd_text_ypos+=LCD_TEXT_YPOS_INC;
                  BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test : OK", LEFT_MODE);
                  led = LED_GREEN;
                }
              }
            }
          }  
        }
      }
    }
  }

  lcd_text_ypos+=(LCD_TEXT_YPOS_INC*2);
  BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"Press Wakeup push-button to switch to next part", CENTER_MODE);

  while (1)
  {    
    if(TamperItOccurred == SET)
    {
      /* Add delay to avoid rebound and reset it status */
      HAL_Delay(500);
      TamperItOccurred = RESET;

      break;
    }
    BSP_LED_Toggle(led);
    HAL_Delay(100);
  }

  /********************************** PART 3 **********************************/
  led = LED_ORANGE;
  lcd_text_ypos = LCD_TEXT_YPOS_START;
  OSPI_NOR_SetHint();
  
  BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"Part III : Enter & Leave Flash Power Down mode", LEFT_MODE);
  lcd_text_ypos+=(LCD_TEXT_YPOS_INC * 2);

  /*##-1- Deconfigure the OSPI NOR device ####################################*/
  status = BSP_OSPI_NOR_DeInit();
  
  if (status != OSPI_NOR_OK)
  {
    BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR De-Initialization : Failed", LEFT_MODE);
    lcd_text_ypos+=LCD_TEXT_YPOS_INC;
    BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR De-Initialization : OK", LEFT_MODE);
    lcd_text_ypos+=LCD_TEXT_YPOS_INC;

    /*##-2- Reconfigure the OSPI NOR device ##################################*/
    /* OSPI NOR device configuration */ 
    status = BSP_OSPI_NOR_Init();
    
    if (status != OSPI_NOR_OK)
    {
      BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Initialization : Failed", LEFT_MODE);
      lcd_text_ypos+=LCD_TEXT_YPOS_INC;
      BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Initialization : OK", LEFT_MODE);
      lcd_text_ypos+=LCD_TEXT_YPOS_INC;
      
      /*##-3- Erase OSPI NOR memory ##########################################*/ 
      if(BSP_OSPI_NOR_Erase_Block(OSPI_NOR_WRITE_READ_ADDR) != OSPI_NOR_OK)
      {
        BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Erase : Failed", LEFT_MODE);
        lcd_text_ypos+=LCD_TEXT_YPOS_INC;
        BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
      }
      else
      {
        BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Erase : OK", LEFT_MODE);
        lcd_text_ypos+=LCD_TEXT_YPOS_INC;

        /*##-4- OSPI NOR memory write access #################################*/
        /* Fill the buffer to write */
        Fill_Buffer(ospi_nor_aTxBuffer, OSPI_NOR_BUFFER_SIZE, 0xD20F);   
        
        /* Write data to the OSPI NOR memory */
        if(BSP_OSPI_NOR_Write(ospi_nor_aTxBuffer, OSPI_NOR_WRITE_READ_ADDR, OSPI_NOR_BUFFER_SIZE) != OSPI_NOR_OK)
        {
          BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Write : Failed", LEFT_MODE);
          lcd_text_ypos+=LCD_TEXT_YPOS_INC;
          BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
        }
        else
        {
          BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Write : OK", LEFT_MODE);
          lcd_text_ypos+=LCD_TEXT_YPOS_INC;

          /*##-5- Enter OSPI NOR memory in deep power down ###################*/ 
          if(BSP_OSPI_NOR_EnterDeepPowerDown() != OSPI_NOR_OK)
          {
            BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Enter Deep PwrDn : Failed", LEFT_MODE);
            lcd_text_ypos+=LCD_TEXT_YPOS_INC;
            BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
          }
          else
          {
            /* !!! Warning : This is only a test of the API. To check if the memory is really in deep power down, 
                   need to use Idd to check the consumption !!! */
            BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Enter Deep PwrDn : OK", LEFT_MODE);
            lcd_text_ypos+=LCD_TEXT_YPOS_INC;
            
            /*##-6- Leave OSPI NOR memory from deep power down ###############*/ 
            HAL_Delay(100);
            if(BSP_OSPI_NOR_LeaveDeepPowerDown() != OSPI_NOR_OK)
            {
              BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Leave Deep PwrDn : Failed", LEFT_MODE);
              lcd_text_ypos+=LCD_TEXT_YPOS_INC;
              BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
            }
            else
            {
              BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Leave Deep PwrDn : OK", LEFT_MODE);
              lcd_text_ypos+=LCD_TEXT_YPOS_INC;
              
              /*##-7- OSPI NOR memory in memory-mapped mode#######################*/
              if(BSP_OSPI_NOR_EnableMemoryMappedMode() != OSPI_NOR_OK)
              {
                BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Mem-Mapped Cfg : Failed", LEFT_MODE);
                lcd_text_ypos+=LCD_TEXT_YPOS_INC;
                BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
              }
              else
              {
                BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Mem-Mapped Cfg : OK", LEFT_MODE);
                lcd_text_ypos+=LCD_TEXT_YPOS_INC;

                for(index = 0, data_ptr = (__IO uint8_t *)(OSPI_NOR_BASE_ADDR + OSPI_NOR_WRITE_READ_ADDR); 
                    index < OSPI_NOR_BUFFER_SIZE; index++, data_ptr++)
                {
                  if(*data_ptr != ospi_nor_aTxBuffer[index])
                  {
                    BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Mem-Mapped Access : Failed", LEFT_MODE);
                    lcd_text_ypos+=LCD_TEXT_YPOS_INC;
                    BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
                    break;
                  }
                }
                
                if(index == OSPI_NOR_BUFFER_SIZE)
                {
                  BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Mem-Mapped Access : OK", LEFT_MODE);
                  lcd_text_ypos+=LCD_TEXT_YPOS_INC;
                  BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test : OK", LEFT_MODE);
                  led = LED_GREEN;
                }
              }
            }
          }  
        }
      }
    }
  }

  lcd_text_ypos+=(LCD_TEXT_YPOS_INC*2);
  BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"Press Wakeup push-button to switch to next part", CENTER_MODE);

  while (1)
  {    
    if(TamperItOccurred == SET)
    {
      /* Add delay to avoid rebound and reset it status */
      HAL_Delay(500);
      TamperItOccurred = RESET;

      break;
    }
    BSP_LED_Toggle(led);
    HAL_Delay(100);
  }

  /********************************** PART 4 **********************************/
  /* Erase NOR  (up to 300s) */
  led = LED_ORANGE;
  lcd_text_ypos = LCD_TEXT_YPOS_START;
  OSPI_NOR_SetHint();
  
  BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"Part IV : Erase ... wait up to 300s", LEFT_MODE);
  lcd_text_ypos+=(LCD_TEXT_YPOS_INC * 2);

  /*##-1- Deconfigure the OSPI NOR device ####################################*/
  status = BSP_OSPI_NOR_DeInit();
  
  if (status != OSPI_NOR_OK)
  {
    BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR De-Initialization : Failed", LEFT_MODE);
    lcd_text_ypos+=LCD_TEXT_YPOS_INC;
    BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR De-Initialization : OK", LEFT_MODE);
    lcd_text_ypos+=LCD_TEXT_YPOS_INC;

    /*##-2- Reconfigure the OSPI NOR device ##################################*/
    /* OSPI NOR device configuration */ 
    status = BSP_OSPI_NOR_Init();
    
    if (status != OSPI_NOR_OK)
    {
      BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Initialization : Failed", LEFT_MODE);
      lcd_text_ypos+=LCD_TEXT_YPOS_INC;
      BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Initialization : OK", LEFT_MODE);
      lcd_text_ypos+=LCD_TEXT_YPOS_INC;
      
      /*##-3- Erase OSPI NOR memory ##########################################*/ 
      BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Chip Erasing...", LEFT_MODE);

      if(BSP_OSPI_NOR_Erase_Chip() != OSPI_NOR_OK)
      {
        BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Chip Erase : Failed", LEFT_MODE);
        lcd_text_ypos+=LCD_TEXT_YPOS_INC;
        BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
      }
      else
      {
        BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Chip Erase : OK", LEFT_MODE);
        lcd_text_ypos+=LCD_TEXT_YPOS_INC;

        for (index = 0; index < (MX25LM51245G_FLASH_SIZE / MX25LM51245G_SECTOR_SIZE); index++)
        {
          /*##-4- OSPI NOR memory read access  ###############################*/
          /* Read back data from the OSPI NOR memory */
          if(BSP_OSPI_NOR_Read(ospi_nor_aRxBuffer, (index * MX25LM51245G_SECTOR_SIZE), MX25LM51245G_SECTOR_SIZE) != OSPI_NOR_OK)
          {
            BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Read : Failed", LEFT_MODE);
            lcd_text_ypos+=LCD_TEXT_YPOS_INC;
            BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
            break;
          }
          else
          {
            /*##-5- Checking data integrity ################################*/  
            if(DataCmp(ospi_nor_aRxBuffer, 0xFF, MX25LM51245G_SECTOR_SIZE) > 0)
            {
              BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Compare : Failed", LEFT_MODE);
              lcd_text_ypos+=LCD_TEXT_YPOS_INC;
              BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test Aborted", LEFT_MODE);
              break;
            }
          }
        }

        if (index == (MX25LM51245G_FLASH_SIZE / MX25LM51245G_SECTOR_SIZE))
        {
          BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"OSPI NOR Test : OK", LEFT_MODE);
          led = LED_GREEN;
        }
      }
    }
  }

  /* De-initialization in order to have correct configuration memory on next try */
  BSP_OSPI_NOR_DeInit();

  lcd_text_ypos+=(LCD_TEXT_YPOS_INC*2);
  BSP_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t*)"Press Wakeup push-button to switch to next menu", CENTER_MODE);

  while (1)
  {    
    if(TamperItOccurred == SET)
    {
      /* Add delay to avoid rebound and reset it status */
      HAL_Delay(500);
      TamperItOccurred = RESET;

      return;
    }
    BSP_LED_Toggle(led);
    HAL_Delay(100);
  }
}

/**
* @brief  Display OSPI NOR Demo Hint
* @param  None
* @retval None
*/
static void OSPI_NOR_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 60);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"OSPI NOR", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows how to write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t*)"and read data on OSPI NOR memory", CENTER_MODE); 
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 70, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 80);
  BSP_LCD_DrawRect(11, 71, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 82);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
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
* @brief  Compares a buffer with data.
* @param  pBuffer, pBuffer2: buffers to be compared.
* @param  Pattern: data reference pattern.
* @param  BufferLength: buffer's length
* @retval 1: pBuffer identical to pBuffer1
*         0: pBuffer differs from pBuffer1
*/
static uint8_t DataCmp(uint8_t* pBuffer, uint8_t Pattern, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer != Pattern)
    {
      return 1;
    }
    
    pBuffer++;
  }
  
  return 0;
}
/**
* @}
*/ 

/**
* @}
*/ 
