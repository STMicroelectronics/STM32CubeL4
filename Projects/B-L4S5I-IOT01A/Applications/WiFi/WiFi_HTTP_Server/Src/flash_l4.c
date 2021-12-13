/**
  ******************************************************************************
  * @file    flash_l4.c
  * @author  MCD Application Team
  * @brief   Management of the L4 internal flash memory.
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
#include "flash.h"
#include <string.h>
#include <stdbool.h>

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define ROUND_DOWN(a,b) (((a) / (b)) * (b))
#define MIN(a,b)        (((a) < (b)) ? (a) : (b))

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static int FLASH_get_pageInBank(uint32_t addr);
static uint32_t FLASH_Init(void);
static int FLASH_write_at(uint32_t address, uint64_t *pData, uint32_t len_bytes);
static int FLASH_unlock_erase(uint32_t address, uint32_t len_bytes);
static uint32_t FLASH_get_bank(uint32_t addr);


/* Functions Definition ------------------------------------------------------*/

/**
  * @brief  Erase FLASH memory page(s) at address.
  * @note   The range to erase shall not cross the bank boundary.
  * @param  In: address     Start address to erase from.
  * @param  In: len_bytes   Length to be erased.
  * @retval  0:  Success.
            -1:  Failure.
  */
int FLASH_unlock_erase(uint32_t address, uint32_t len_bytes)
{
  int rc = -1;
  uint32_t PageError = 0;
  FLASH_EraseInitTypeDef EraseInit;

  /* L4 ROM memory map, with 2 banks split into 2kBytes pages.
   * WARN: ABW. If the passed address and size are not page-aligned,
   * the start of the first page and the end of the last page are erased anyway.
   * After erase, the flash is left in unlocked state.
   */
  EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;

  EraseInit.Banks = FLASH_get_bank(address);
  if (EraseInit.Banks != FLASH_get_bank(address + len_bytes - 1))
  {
    return rc;
  }
  else
  {
    EraseInit.Page = FLASH_get_pageInBank(address);
    EraseInit.NbPages = FLASH_get_pageInBank(address + len_bytes - 1) - EraseInit.Page + 1;

    HAL_FLASH_Unlock();
    if (__HAL_FLASH_GET_FLAG(FLASH_SR_PEMPTY) == SET)
    {
      __HAL_FLASH_CLEAR_FLAG(FLASH_SR_PEMPTY);
    }
    if (HAL_FLASHEx_Erase(&EraseInit, &PageError) == HAL_OK)
    {
      rc = 0;
    }
  }
  return rc;
}

/**
  * @brief  Write to FLASH memory.
  * @param  In: address     Destination address.
  * @param  In: pData       Data to be programmed: Must be 8 byte aligned.
  * @param  In: len_bytes   Number of bytes to be programmed.
  * @retval  0: Success.
            -1: Failure.
  */
static int FLASH_write_at(uint32_t address, uint64_t *pData, uint32_t len_bytes)
{
  int i;
  int ret = -1;

  for (i = 0; i < len_bytes; i += 8)
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,
                          address + i,
                          *(pData + (i / 8))) != HAL_OK)
    {
      break;
    }
  }
  /* Memory check */
  for (i = 0; i < len_bytes; i += 4)
  {
    uint32_t *dst = (uint32_t *)(address + i);
    uint32_t *src = ((uint32_t *) pData) + (i / 4);

    if (*dst != *src)
    {
      break;
    }
    ret = 0;
  }

  return ret;
}

/**
  * @brief  Get the bank of a given address.
  * @param  In: addr      Address in the FLASH Memory.
  * @retval Bank identifier.
  *           FLASH_BANK_1
  *           FLASH_BANK_2
  */
uint32_t FLASH_get_bank(uint32_t addr)
{
  uint32_t bank = 0;

  if (READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == 0)
  {
    /* No Bank swap */
    bank = (addr < (FLASH_BASE + FLASH_BANK_SIZE)) ? FLASH_BANK_1 : FLASH_BANK_2;
  }
  else
  {
    /* Bank swap */
    bank = (addr < (FLASH_BASE + FLASH_BANK_SIZE)) ? FLASH_BANK_2 : FLASH_BANK_1;
  }

  return bank;
}

/**
  * @brief  Get the page of a given address within its FLASH bank.
  * @param  In: addr    Address in the FLASH memory.
  * @retval >=0 Success: Page number.
  *          <0 Failure: The address in not mapped in the internal FLASH memory.
  */
static int FLASH_get_pageInBank(uint32_t addr)
{
  int page = -1;

  if (((FLASH_BASE + FLASH_SIZE) > addr) && (addr >= FLASH_BASE))
  {
    /* The address is in internal FLASH range. */
    if (addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      /* Addr in the first bank */
      page = (addr - FLASH_BASE) / FLASH_PAGE_SIZE;
    }
    else
    {
      /* Addr in the second bank */
      page = (addr - FLASH_BASE - FLASH_BANK_SIZE) / FLASH_PAGE_SIZE;
    }
  }

  return page;
}

/**
  * @brief  Update a chunk of the FLASH memory.
  * @note   The FLASH chunk must no cross a FLASH bank boundary.
  * @note   The source and destination buffers have no specific alignment constraints.
  * @param  In: dst_addr    Destination address in the FLASH memory.
  * @param  In: data        Source address.
  * @param  In: size        Number of bytes to update.
  * @retval  0:  Success.
  *        !=0:  Failure.
  */
int FLASH_update(uint32_t dst_addr, const void *data, uint32_t size)
{
  int ret = 0;
  int remaining = size;
  uint8_t *src_addr = (uint8_t *) data;
  uint64_t *page_cache = (uint64_t *) malloc(FLASH_PAGE_SIZE);

  if (page_cache == NULL)
  {
    return HAL_ERROR;
  }

  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

  if (__HAL_FLASH_GET_FLAG(FLASH_SR_PEMPTY) == SET)
  {
    __HAL_FLASH_CLEAR_FLAG(FLASH_SR_PEMPTY);
  }
  do
  {
    uint32_t fl_addr = ROUND_DOWN(dst_addr, FLASH_PAGE_SIZE);
    int fl_offset = dst_addr - fl_addr;
    int len = MIN(FLASH_PAGE_SIZE - fl_offset, size);

    /* Load from the flash into the cache */
    memcpy(page_cache, (void *) fl_addr, FLASH_PAGE_SIZE);
    /* Update the cache from the source */
    memcpy((uint8_t *)page_cache + fl_offset, src_addr, len);
    /* Erase the page, and write the cache */
    ret = FLASH_unlock_erase(fl_addr, FLASH_PAGE_SIZE);
    if (ret == 0)
    {
      /* NOTE: we may Consider disabling interrupts if needed during Flash writing */
      ret = FLASH_write_at(fl_addr, page_cache, FLASH_PAGE_SIZE);

      if (ret == 0)
      {
        dst_addr += len;
        src_addr += len;
        remaining -= len;
      }
    }
  } while ((ret == 0) && (remaining > 0));
  HAL_FLASH_Lock();
  free(page_cache);

  return ret;
}


static uint32_t GetPage(uint32_t uAddr);
static uint32_t GetBank(uint32_t uAddr);
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Gets the page of a given address
  * @param  uAddr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t uAddr)
{
  uint32_t page = 0U;

  if (uAddr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (uAddr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (uAddr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }

  return page;
}

/**
  * @brief  Gets the bank of a given address
  * @param  uAddr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
static uint32_t GetBank(uint32_t uAddr)
{
  uint32_t bank = 0U;

  if (READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == 0U)
  {
    /* No Bank swap */
    if (uAddr < (FLASH_BASE + FLASH_BANK_SIZE))
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
    if (uAddr < (FLASH_BASE + FLASH_BANK_SIZE))
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

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval HAL Status.
  */
uint32_t FLASH_Init(void)
{
  uint32_t ret = HAL_ERROR;

  /* Unlock the Program memory */
  if (HAL_FLASH_Unlock() == HAL_OK)
  {

    /* Clear all FLASH flags */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGSERR | FLASH_FLAG_WRPERR | FLASH_FLAG_OPTVERR);

    /* Unlock the Program memory */
    if (HAL_FLASH_Lock() == HAL_OK)
    {
      ret = HAL_OK;
    }
    else
    {
      printf("FLASH_If_Init : failed to lock\n");
    }
  }
  else
  {
    printf("FLASH_If_Init : failed to unlock\n");
  }
  return ret;
}

/**
  * @brief  This function erases bytes in user flash area
  * @param  Start: Start of user flash area
  * @param  uLength: number of bytes.
  * @retval HAL status.
  */
int FLASH_Erase_Size(uint32_t uStart, uint32_t uLength)
{
  uint32_t page_error = 0U;
  FLASH_EraseInitTypeDef x_erase_init;
  uint32_t e_ret_status = HAL_ERROR;
  uint32_t first_page = 0U, nb_of_pages = 0U, bank_number = 0U;

  /* Initialize Flash */
  e_ret_status = FLASH_Init();

  if (e_ret_status == HAL_OK)
  {
    /* Unlock the Flash to enable the flash control register access *************/
    if (HAL_FLASH_Unlock() == HAL_OK)
    {
      /* Get the 1st page to erase */
      first_page = GetPage(uStart);
      /* Get the number of pages to erase from 1st page */
      nb_of_pages = GetPage(uStart + uLength - 1U) - first_page + 1U;
      /* Get the bank */
      bank_number = GetBank(uStart);

      /* Fill EraseInit structure*/
      x_erase_init.TypeErase   = FLASH_TYPEERASE_PAGES;
      x_erase_init.Banks       = bank_number;
      x_erase_init.Page        = first_page;
      x_erase_init.NbPages     = nb_of_pages;

      if (HAL_FLASHEx_Erase(&x_erase_init, &page_error) != HAL_OK)
      {
        /* Error occurred while page erase */
        HAL_FLASH_GetError();
        printf("ERROR flash erase\n");
        e_ret_status = HAL_ERROR;
      }

      /* Lock the Flash to disable the flash control register access (recommended
      to protect the FLASH memory against possible unwanted operation) *********/
      HAL_FLASH_Lock();
    }
    else
    {
      printf("ERROR cannot unlock\n");
    }
  }
  else
  {
    printf("cannot init\n");
  }
  return e_ret_status;
}


/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  uDestination: Start address for target location
  * @param  pSource: pointer on buffer with data to write
  * @param  uLength: Length of data buffer in byte. It has to be 64-bit aligned.
  * @retval uint32_t 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  *         2: Written Data in flash memory is different from expected one
  */
int FLASH_Write(uint32_t uDestination, uint32_t *pSource, uint32_t uLength)
{
  uint32_t e_ret_status = HAL_ERROR;
  uint32_t i = 0U;

  /* Initialize Flash */
  e_ret_status = FLASH_Init();

  if (e_ret_status == HAL_OK)
  {
    /* Unlock the Flash to enable the flash control register access *************/
    if (HAL_FLASH_Unlock() != HAL_OK)
    {
      return HAL_ERROR;
    }
    else
    {
      /* DataLength must be a multiple of 64 bit */
      for (i = 0U; i < uLength / 8U; i++)
      {
        /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
        be done by word */
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, uDestination, *((uint64_t *)(pSource + 2U * i))) == HAL_OK)
        {
          /* Check the written value */
          if (*(uint64_t *)uDestination != *(uint64_t *)(pSource + 2U * i))
          {
            /* Flash content doesn't match SRAM content */
            e_ret_status = HAL_ERROR;
            break;
          }
          /* Increment FLASH Destination address */
          uDestination += 8U;
        }
        else
        {
          /* Error occurred while writing data in Flash memory */
          e_ret_status = HAL_ERROR;
          break;
        }
      }
      /* Lock the Flash to disable the flash control register access (recommended
      to protect the FLASH memory against possible unwanted operation) *********/
      HAL_FLASH_Lock();
    }
  }
  return e_ret_status;
}

