/**
  ******************************************************************************
  * @file    FatFs/FatFs_eMMC_Standalone/Src/mmc_diskio.c
  * @author  MCD Application Team
  * @brief   MMC Disk I/O driver
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ff_gen_drv.h"
#include "mmc_diskio.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define MMC_DEFAULT_BLOCK_SIZE 512

/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* Private function prototypes -----------------------------------------------*/
static DSTATUS MMC_CheckStatus(BYTE lun);
DSTATUS MMC_initialize (BYTE);
DSTATUS MMC_status (BYTE);
DRESULT MMC_read (BYTE, BYTE*, DWORD, UINT);
#if _USE_WRITE == 1
  DRESULT MMC_write (BYTE, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT MMC_ioctl (BYTE, BYTE, void*);
#endif  /* _USE_IOCTL == 1 */

const Diskio_drvTypeDef  MMC_Driver =
{
  MMC_initialize,
  MMC_status,
  MMC_read,
#if  _USE_WRITE == 1
  MMC_write,
#endif /* _USE_WRITE == 1 */

#if  _USE_IOCTL == 1
  MMC_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/
static DSTATUS MMC_CheckStatus(BYTE lun)
{
  Stat = STA_NOINIT;

  if(BSP_MMC_GetCardState() == MMC_TRANSFER_OK)
  {
    Stat &= ~STA_NOINIT;
  }

  return Stat;
}

/**
  * @brief  Initializes a Drive
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS MMC_initialize(BYTE lun)
{

    if (MMC_OK == BSP_MMC_DeInit())
    {
      if (MMC_OK == BSP_MMC_Init())
      {
        Stat = MMC_CheckStatus(lun);
      }
    }

   return Stat;
}

/**
  * @brief  Gets Disk Status
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS MMC_status(BYTE lun)
{
  return MMC_CheckStatus(lun);
}

/**
  * @brief  Reads Sector(s)
  * @param  lun : not used
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT MMC_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_ERROR;

  if(BSP_MMC_ReadBlocks((uint32_t*)buff,
                       (uint32_t) (sector),
                       count, SDMMC_DATATIMEOUT) == MMC_OK)
  {
    /* wait until the read operation is finished */
    while(BSP_MMC_GetCardState()!= MMC_TRANSFER_OK)
    {
    }
    res = RES_OK;
  }

  return res;
}

/**
  * @brief  Writes Sector(s)
  * @param  lun : not used
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT MMC_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_ERROR;

  if(BSP_MMC_WriteBlocks((uint32_t*)buff,
                        (uint32_t)(sector),
                        count, SDMMC_DATATIMEOUT) == MMC_OK)
  {
	/* wait until the Write operation is finished */
    while(BSP_MMC_GetCardState() != MMC_TRANSFER_OK)
    {
    }
    res = RES_OK;
  }

  return res;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  lun : not used
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT MMC_ioctl(BYTE lun, BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;
  BSP_MMC_CardInfo CardInfo;

  if (Stat & STA_NOINIT) return RES_NOTRDY;

  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC :
    res = RES_OK;
    break;

  /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT :
    BSP_MMC_GetCardInfo(&CardInfo);
    *(DWORD*)buff = CardInfo.LogBlockNbr;
    res = RES_OK;
    break;

  /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE :
    BSP_MMC_GetCardInfo(&CardInfo);
    *(WORD*)buff = CardInfo.LogBlockSize;
    res = RES_OK;
    break;

  /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE :
    BSP_MMC_GetCardInfo(&CardInfo);
    *(DWORD*)buff = CardInfo.LogBlockSize / MMC_DEFAULT_BLOCK_SIZE;
	res = RES_OK;
    break;

  default:
    res = RES_PARERR;
  }

    while(BSP_MMC_GetCardState() != MMC_TRANSFER_OK)
  {
  }

  return res;
}
#endif /* _USE_IOCTL == 1 */


