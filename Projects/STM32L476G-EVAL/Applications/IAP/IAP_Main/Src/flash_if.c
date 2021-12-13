/**
  ******************************************************************************
  * @file    IAP_Main/Src/flash_if.c 
  * @author  MCD Application Team
  * @brief   This file provides all the memory related operation functions.
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

/** @addtogroup STM32L4xx_IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/*  1M0 flash 1 * 1024 * 1024 */
#define FLASH_START_ADRESS    0x08000000
#define FLASH_PAGE_NBPERBANK  256
#define FLASH_BANK_NUMBER     2

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval None
  */
void FLASH_If_Init(void)
{
  /* Unlock the Program memory */
  HAL_FLASH_Unlock();

  /* Clear all FLASH flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGSERR | FLASH_FLAG_WRPERR | FLASH_FLAG_OPTVERR);
  /* Unlock the Program memory */
  HAL_FLASH_Lock();
}

/**
  * @brief  This function does an erase of all user flash area
  * @param  start: start of user flash area
  * @retval FLASHIF_OK : user flash area successfully erased
  *         FLASHIF_ERASEKO : error occurred
  */
uint32_t FLASH_If_Erase(uint32_t start)
{
  uint32_t NbrOfPages = 0;
  uint32_t PageError = 0;
  FLASH_EraseInitTypeDef pEraseInit;
  HAL_StatusTypeDef status = HAL_OK;

  /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();

  /* Get the number of page to  erase */
  NbrOfPages = (FLASH_START_ADRESS + FLASH_SIZE);
  NbrOfPages = (NbrOfPages - start) / FLASH_PAGE_SIZE;

  if(NbrOfPages > FLASH_PAGE_NBPERBANK)
  {
    pEraseInit.Banks = FLASH_BANK_1;
    pEraseInit.NbPages = NbrOfPages % FLASH_PAGE_NBPERBANK;
    pEraseInit.Page = FLASH_PAGE_NBPERBANK - pEraseInit.NbPages;
    pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);
  
    NbrOfPages = FLASH_PAGE_NBPERBANK;
  }
  
  if(status == HAL_OK)
  {
    pEraseInit.Banks = FLASH_BANK_2;
    pEraseInit.NbPages = NbrOfPages;
    pEraseInit.Page = FLASH_PAGE_NBPERBANK - pEraseInit.NbPages;
    pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);
  }
  
  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  if (status != HAL_OK)
  {
    /* Error occurred while page erase */
    return FLASHIF_ERASEKO;
  }

  return FLASHIF_OK;
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  destination: start address for target location
  * @param  p_source: pointer on buffer with data to write
  * @param  length: length of data buffer (unit is 32-bit word)
  * @retval uint32_t 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  *         2: Written Data in flash memory is different from expected one
  */
uint32_t FLASH_If_Write(uint32_t destination, uint32_t *p_source, uint32_t length)
{
  uint32_t status = FLASHIF_OK;
  uint32_t i = 0;

  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  /* DataLength must be a multiple of 64 bit */
  for (i = 0; (i < length/2) && (destination <= (USER_FLASH_END_ADDRESS-8)); i++)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, destination, *((uint64_t *)(p_source+2*i))) == HAL_OK)      
    {
     /* Check the written value */
      if (*(uint64_t*)destination != *(uint64_t *)(p_source+2*i))
      {
        /* Flash content doesn't match SRAM content */
        status = FLASHIF_WRITINGCTRL_ERROR;
        break;
      }
      /* Increment FLASH destination address */
      destination += 8;
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      status = FLASHIF_WRITING_ERROR;
      break;
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  return status;
}

/**
  * @brief  Returns the write protection status of application flash area.
  * @param  None
  * @retval If a sector in application area is write-protected returned value is a combinaison
            of the possible values : FLASHIF_PROTECTION_WRPENABLED, FLASHIF_PROTECTION_PCROPENABLED, ...
  *         If no sector is write-protected FLASHIF_PROTECTION_NONE is returned.
  */
uint32_t FLASH_If_GetWriteProtectionStatus(void)
{
  uint32_t ProtectedPAGE = FLASHIF_PROTECTION_NONE;
  FLASH_OBProgramInitTypeDef OptionsBytesStruct1, OptionsBytesStruct2, OptionsBytesStruct3, OptionsBytesStruct4;
  
  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();
 
  OptionsBytesStruct1.WRPArea = OB_WRPAREA_BANK1_AREAA;
  OptionsBytesStruct1.PCROPConfig = FLASH_BANK_1;
  OptionsBytesStruct2.WRPArea = OB_WRPAREA_BANK1_AREAB;
  OptionsBytesStruct2.PCROPConfig = FLASH_BANK_1;
  OptionsBytesStruct3.WRPArea = OB_WRPAREA_BANK2_AREAA;
  OptionsBytesStruct3.PCROPConfig = FLASH_BANK_2;
  OptionsBytesStruct4.WRPArea = OB_WRPAREA_BANK2_AREAB;
  OptionsBytesStruct4.PCROPConfig = FLASH_BANK_2;
  
  /* Check if there are write protected sectors inside the user flash area ***/
  HAL_FLASHEx_OBGetConfig(&OptionsBytesStruct1);
  HAL_FLASHEx_OBGetConfig(&OptionsBytesStruct2);
  HAL_FLASHEx_OBGetConfig(&OptionsBytesStruct3);
  HAL_FLASHEx_OBGetConfig(&OptionsBytesStruct4);
  
  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();
  
  /* Check PCROP areas */
  if(OptionsBytesStruct1.PCROPEndAddr > OptionsBytesStruct1.PCROPStartAddr)
  {
    /* check if user area are included inside this range */
    if(OptionsBytesStruct1.PCROPStartAddr > APPLICATION_ADDRESS) 
    {
      ProtectedPAGE|= FLASHIF_PROTECTION_PCROPENABLED;
    }
  }
  
  if(OptionsBytesStruct2.PCROPEndAddr > OptionsBytesStruct2.PCROPStartAddr)
  {
    /* check if user area are included inside this range */
    if(OptionsBytesStruct1.PCROPStartAddr > APPLICATION_ADDRESS)
    {
       ProtectedPAGE|= FLASHIF_PROTECTION_PCROPENABLED;
    }
  }
  
  /* check WRP */
  if(OptionsBytesStruct1.WRPEndOffset > OptionsBytesStruct1.WRPStartOffset)
  {
    /* check if area is inside the WRP Range */ 
    if((OptionsBytesStruct1.WRPStartOffset * FLASH_PAGE_SIZE + FLASH_BASE) >= APPLICATION_ADDRESS)
    {
       ProtectedPAGE|= FLASHIF_PROTECTION_WRPENABLED;
    }   
  }

  if(OptionsBytesStruct2.WRPEndOffset > OptionsBytesStruct2.WRPStartOffset)
  {
    /* check if area is inside the WRP Range */ 
    if((OptionsBytesStruct2.WRPStartOffset * FLASH_PAGE_SIZE + FLASH_BASE) >= APPLICATION_ADDRESS)
    {
       ProtectedPAGE|= FLASHIF_PROTECTION_WRPENABLED;
    }   
  }

  if(OptionsBytesStruct3.WRPEndOffset > OptionsBytesStruct3.WRPStartOffset)
  {
    /* check if area is inside the WRP Range */ 
    if((OptionsBytesStruct3.WRPStartOffset * FLASH_PAGE_SIZE + FLASH_BASE + FLASH_PAGE_SIZE * FLASH_PAGE_NBPERBANK) >= APPLICATION_ADDRESS)
    {
       ProtectedPAGE|= FLASHIF_PROTECTION_WRPENABLED;
    }   
  }  
  
  if(OptionsBytesStruct4.WRPEndOffset > OptionsBytesStruct4.WRPStartOffset)
  {
    /* check if area is inside the WRP Range */ 
    if((OptionsBytesStruct4.WRPStartOffset * FLASH_PAGE_SIZE + FLASH_BASE + FLASH_PAGE_SIZE * FLASH_PAGE_NBPERBANK) >= APPLICATION_ADDRESS)
    {
       ProtectedPAGE|= FLASHIF_PROTECTION_WRPENABLED;
    }   
  }
  
  if(OptionsBytesStruct4.RDPLevel != OB_RDP_LEVEL_0)
  {
    ProtectedPAGE|= FLASHIF_PROTECTION_RDPENABLED;
  }
    
  return ProtectedPAGE;
}

/**
  * @brief  Configure the write protection status of user flash area.
  * @param  protectionstate : FLASHIF_WRP_DISABLE or FLASHIF_WRP_ENABLE the protection
  * @retval uint32_t FLASHIF_OK if change is applied.
  */
uint32_t FLASH_If_WriteProtectionConfig(uint32_t protectionstate)
{
  FLASH_OBProgramInitTypeDef OptionsBytesStruct1;
  HAL_StatusTypeDef retr;
  
  /* Unlock the Flash to enable the flash control register access *************/
  retr = HAL_FLASH_Unlock();

  /* Unlock the Options Bytes *************************************************/
  retr|= HAL_FLASH_OB_Unlock();
  
  OptionsBytesStruct1.OptionType = OPTIONBYTE_WRP;
  OptionsBytesStruct1.WRPArea = OB_WRPAREA_BANK1_AREAA;
  if( protectionstate == FLASHIF_WRP_ENABLE)
  {
    /* Enable the WRP protection for all flash BANK1 */
    OptionsBytesStruct1.WRPEndOffset = FLASH_PAGE_NBPERBANK - 1;
    OptionsBytesStruct1.WRPStartOffset = 0x00;
  }
  else
  {
    /* Remove all WRP protection */
    OptionsBytesStruct1.WRPEndOffset = 0x00;
    OptionsBytesStruct1.WRPStartOffset = 0xFF;
  }
  retr|= HAL_FLASHEx_OBProgram(&OptionsBytesStruct1);
  
  OptionsBytesStruct1.OptionType = OPTIONBYTE_WRP;
  OptionsBytesStruct1.WRPArea = OB_WRPAREA_BANK1_AREAB;
  OptionsBytesStruct1.WRPEndOffset = 0x00;
  OptionsBytesStruct1.WRPStartOffset = 0xFF;
  retr|= HAL_FLASHEx_OBProgram(&OptionsBytesStruct1);
    
  OptionsBytesStruct1.OptionType = OPTIONBYTE_WRP;
  OptionsBytesStruct1.WRPArea = OB_WRPAREA_BANK2_AREAA;
  if( protectionstate == FLASHIF_WRP_ENABLE)
  {
    /* Enable the WRP protection for all flash BANK1 */
    OptionsBytesStruct1.WRPEndOffset = FLASH_PAGE_NBPERBANK - 1;
    OptionsBytesStruct1.WRPStartOffset = 0x00;
  }
  else
  {
    /* Remove all WRP protection */
    OptionsBytesStruct1.WRPEndOffset = 0x00;
    OptionsBytesStruct1.WRPStartOffset = 0xFF;
  }
  retr|= HAL_FLASHEx_OBProgram(&OptionsBytesStruct1);
  
  OptionsBytesStruct1.RDPLevel = OB_RDP_LEVEL_0;
  OptionsBytesStruct1.OptionType = OPTIONBYTE_WRP;
  OptionsBytesStruct1.WRPArea = OB_WRPAREA_BANK2_AREAB;
  OptionsBytesStruct1.WRPEndOffset = 0x00;
  OptionsBytesStruct1.WRPStartOffset = 0xFF;
  retr|= HAL_FLASHEx_OBProgram(&OptionsBytesStruct1);
  
  return (retr == HAL_OK ? FLASHIF_OK: FLASHIF_PROTECTION_ERRROR);
}
/**
  * @}
  */

