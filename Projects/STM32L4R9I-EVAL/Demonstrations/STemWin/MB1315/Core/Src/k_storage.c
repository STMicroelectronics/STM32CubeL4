/**
  ******************************************************************************
  * @file    k_storage.c
  * @author  MCD Application Team 
  * @brief   This file provides the kernel storage functions
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
#include <string.h>
#include "k_storage.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_STORAGE
  * @brief Kernel storage routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
static struct {
  U32 Mask;
  char c;
} _aAttrib[] = {
  { AM_RDO, 'R' },
  { AM_HID, 'H' },
  { AM_SYS, 'S' },
  { AM_DIR, 'D' },
  { AM_ARC, 'A' },
};
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS USBDISK_FatFs;         /* File system object for USB disk logical drive */
FATFS mSDDISK_FatFs;         /* File system object for uSD disk logical drive */
char USBDISK_Drive[4];       /* USB Host logical drive number */
char mSDDISK_Drive[4];       /* uSD Host logical drive number */

#if USE_USB_DISK == 1
USBH_HandleTypeDef  hUSB_Host;
#endif /* USE_USB_DISK */

osMessageQId StorageEvent = 0;
DIR dir;
static char         acAttrib[10];
static char         acExt[FILEMGR_MAX_EXT_SIZE];
//#if _USE_LFN
//static char lfn[_MAX_LFN];
//#endif

static uint32_t StorageStatus[NUM_DISK_UNITS];
static StorageCallBack_t StorageCallBack[NUM_DISK_UNITS];

/* Private function prototypes -----------------------------------------------*/
static void StorageThread(void const * argument);
#if USE_USB_DISK == 1
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id);
#endif /* USE_USB_DISK */
static void GetExt(char * pFile, char * pExt);
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Storage drives initialization
  * @param  None 
  * @retval None
  */
void k_StorageInit(void)
{
  if(StorageEvent == 0)
  {
    /* Create Storage Message Queue */
    osMessageQDef(osqueue, 10, uint16_t);
    StorageEvent = osMessageCreate (osMessageQ(osqueue), NULL);

    if(StorageEvent)
    {
      /* Create Storage background task */
      osThreadDef(STORAGE_Thread, StorageThread, osPriorityRealtime, 0, (8 * configMINIMAL_STACK_SIZE));
      osThreadCreate (osThread(STORAGE_Thread), NULL);
    }

    memset(StorageStatus, 0, sizeof(StorageStatus));
    memset(StorageCallBack, 0, sizeof(StorageCallBack));
  }

#if USE_USB_DISK == 1
  /* Link the USB Host disk I/O driver */
  memset(USBDISK_Drive, 0, sizeof(USBDISK_Drive));
  if(FATFS_LinkDriver(&USBH_Driver, USBDISK_Drive) == 0)
  {
    /* Init Host Library */
    USBH_Init(&hUSB_Host, USBH_UserProcess, 0);
    
    /* Add Supported Class */
    USBH_RegisterClass(&hUSB_Host, USBH_MSC_CLASS);
    
    /* Start Host Process */
    USBH_Start(&hUSB_Host);
  }
#endif /* USE_USB_DISK */

  /* Link the micro SD disk I/O driver */
  memset(mSDDISK_Drive, 0, sizeof(mSDDISK_Drive));
  if(FATFS_LinkDriver(&SD_Driver, mSDDISK_Drive) == 0)
  {
    /* Enable SD Interrupt mode */
    BSP_SD_Init();
    BSP_SD_ITConfig();

    if(BSP_SD_IsDetected())
    {
      osMessagePut ( StorageEvent, MSDDISK_CONNECTION_EVENT, 0);
    }
  }
}

uint8_t k_StorageDeInit(void)
{
  uint8_t Ret = 0;
  
#if USE_USB_DISK == 1
  if((Ret == 0) && (USBDISK_Drive[0] != 0))
  {
    Ret = FATFS_UnLinkDriver(USBDISK_Drive);
    if(Ret == 0)
    {
      memset(USBDISK_Drive, 0, sizeof(USBDISK_Drive));
      USBD_Stop(&hUSB_Host);
      USBD_DeInit(&hUSB_Host);
    }
  }
#endif /* USE_USB_DISK */

  if((Ret == 0) && (mSDDISK_Drive[0] != 0))
  {
    Ret = FATFS_UnLinkDriver(mSDDISK_Drive);
    if(Ret == 0)
    {
      memset(mSDDISK_Drive, 0, sizeof(mSDDISK_Drive));
      BSP_SD_DeInit();
      /* Keep interrupts alive */
      BSP_SD_ITConfig();
    }
  }

  memset(StorageCallBack, 0, sizeof(StorageCallBack));

  return Ret;
}  

/**
  * @brief  Storage Thread
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void StorageThread(void const * argument)
{
  osEvent event;
 
  for( ;; )
  {
    uint8_t status = 0;
    uint8_t unit;
    FATFS   *fs = NULL;
    char    *dd = NULL;

    event = osMessageGet( StorageEvent, osWaitForever );
    
    if( event.status == osEventMessage )
    {
      switch(event.value.v)
      {
#if USE_USB_DISK == 1
        case USBDISK_CONNECTION_EVENT:
          status = 1;
        case USBDISK_DISCONNECTION_EVENT:
          unit = USB_DISK_UNIT;
          fs = (status ? &USBDISK_FatFs : 0);
          dd = USBDISK_Drive;
        break;
#endif /* USE_USB_DISK */

        case MSDDISK_CONNECTION_EVENT:
          status = 1;
        case MSDDISK_DISCONNECTION_EVENT:
          unit = MSD_DISK_UNIT;
          fs = (status ? &mSDDISK_FatFs : 0);
          dd = mSDDISK_Drive;
        break;
        
        default:
        return;
      }
    }
    
    StorageStatus[unit] = status;
    
    if(status)
    {
      BSP_SD_Init();
      BSP_SD_ITConfig();
    }
    else
    {
      if(StorageCallBack[unit])
        StorageCallBack[unit](unit, status);
    }

    if (dd)
    {
      if(f_mount(fs, dd, 0) == 0)
      {       
        if(!status)
          memset(dd, 0, sizeof(dd));
      }
    }

    if(status)
    {
      if(StorageCallBack[unit])
        StorageCallBack[unit](unit, status);
    }
    else
    {
      BSP_SD_DeInit();
      BSP_SD_ITConfig();
    }
  }
}

/**
  * @brief  Storage get status
  * @param  unit: logical storage unit index.
  * @retval int
  */
uint8_t k_StorageGetStatus (uint8_t unit)
{
  return StorageStatus[unit];
}

uint8_t k_StorageRegisterStatusCallBack (uint8_t unit, StorageCallBack_t cb)
{
  switch (unit)
  {
#if USE_USB_DISK == 1
    case USB_DISK_UNIT :
#endif /* USE_USB_DISK */
    case MSD_DISK_UNIT :
      StorageCallBack[unit] = cb;
    break;

    default:
      return 0;
  }
  
  return 1;
}

/**
  * @brief  Storage get drive
  * @param  unit: logical storage unit index.
  * @retval char[4]
  */
const char *k_Storage_GetDrive (uint8_t unit)
{
  switch (unit)
  {
#if USE_USB_DISK == 1
    case USB_DISK_UNIT :
      return USBDISK_Drive;
#endif /* USE_USB_DISK */

    case MSD_DISK_UNIT :
      return mSDDISK_Drive;

    default:
      return '\0';
  }
}

/**
  * @brief  Storage get capacity
  * @param  unit: logical storage unit index.
  * @retval int
  */
uint32_t k_StorageGetCapacity (uint8_t unit)
{  
  FATFS *fs;
  
  switch (unit)
  {
#if USE_USB_DISK == 1
    case USB_DISK_UNIT :
      fs = &USBDISK_FatFs;
    break;
#endif /* USE_USB_DISK */

    case MSD_DISK_UNIT :
      fs = &mSDDISK_FatFs;
    break;

    default:
      return 0;
  }

  return ((fs->n_fatent - 2) * fs->csize);
}

/**
  * @brief  Storage get free space
  * @param  unit: logical storage unit index. 
  * @retval int
  */
uint32_t k_StorageGetFree (uint8_t unit)
{ 
  FATFS *fs;
  char  *dd;
  uint32_t   fre_clust = 0;
  uint32_t   free_sz = 0;

  switch (unit)
  {
#if USE_USB_DISK == 1
    case USB_DISK_UNIT :
      fs = &USBDISK_FatFs;
      dd = USBDISK_Drive;
    break;
#endif /* USE_USB_DISK */

    case MSD_DISK_UNIT :
      fs = &mSDDISK_FatFs;
      dd = mSDDISK_Drive;
    break;

    default:
      return 0;
  }
  
  if(f_getfree(dd, (DWORD *)&fre_clust, &fs) == FR_OK)
    free_sz = (fre_clust * fs->csize);

  return free_sz;
}

/**
  * @brief  User Process
  * @param  phost: Host handle
  * @param  id:    Host Library user message ID
  * @retval None
  */
#if USE_USB_DISK == 1
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id)
{  
  switch (id)
  { 
    case HOST_USER_SELECT_CONFIGURATION:
      break;
      
    case HOST_USER_DISCONNECTION:
      osMessagePut ( StorageEvent, USBDISK_DISCONNECTION_EVENT, 0);
      break;
      
    case HOST_USER_CLASS_ACTIVE:
      osMessagePut ( StorageEvent, USBDISK_CONNECTION_EVENT, 0);
      break;
  }
}
#endif /* USE_USB_DISK */

/**
  * @brief  SD detect callback
  * @param  None
  * @retval None
  */ 
void BSP_SD_DetectCallback(void)
{
  if((BSP_SD_IsDetected()))
  {  
    /* After sd disconnection, a SD Init is required */
    // BSP_SD_Init();
    // BSP_SD_ITConfig();
        
    osMessagePut ( StorageEvent, MSDDISK_CONNECTION_EVENT, 0);
  }
  else
  {
    osMessagePut ( StorageEvent, MSDDISK_DISCONNECTION_EVENT, 0);
  }
}

/**
  * @brief  Return file extension and removed from file name.
  * @param  pFile: pointer to the file name.
  * @param  pExt:  pointer to the file extension
  * @retval None
  */
static void GetExt(char * pFile, char * pExt) 
{
  int Len;
  int i;
  int j;
  
  /* Search beginning of extension */
  Len = strlen(pFile);
  for (i = Len; i > 0; i--) {
    if (*(pFile + i) == '.') {
      *(pFile + i) = '\0';     /* Cut extension from filename */
      break;
    }
  }
  
  /* Copy extension */
  j = 0;
  while (*(pFile + ++i) != '\0') {
    *(pExt + j) = *(pFile + i);
    j++;
  }
  *(pExt + j) = '\0';          /* Set end of string */
}

/**
  * @brief  Return the extension Only
  * @param  pFile: pointer to the file name.
  * @param  pExt:  pointer to the file extension
  * @retval None
  */
void k_GetExtOnly(char * pFile, char * pExt) 
{
  int Len;
  int i;
  int j;
  
  /* Search beginning of extension */
  Len = strlen(pFile);
  for (i = Len; i > 0; i--) {
    if (*(pFile + i) == '.') {
      break;
    }
  }
  
  /* Copy extension */
  j = 0;
  while (*(pFile + ++i) != '\0') {
    *(pExt + j) = *(pFile + i);
    j++;
  }
  *(pExt + j) = '\0';          /* Set end of string */
}
/**
  * @brief  This function is responsible to pass information about the requested file
  * @param  pInfo: Pointer to structure which contains all details of the requested file.
  * @retval None
  */
int k_GetData(CHOOSEFILE_INFO * pInfo)
{
  char                c;
  int                 i;
  char               tmp[CHOOSEFILE_MAXLEN];  
  FRESULT res = FR_INT_ERR;
  static char fn[CHOOSEFILE_MAXLEN];  
  //char *fn;
  FILINFO fno;
  
  switch (pInfo->Cmd) 
  {
  case CHOOSEFILE_FINDFIRST:
    f_closedir(&dir); 
    
    /* reformat path */
    memset(tmp, 0, CHOOSEFILE_MAXLEN);
    strcpy(tmp, pInfo->pRoot);
    
    for(i= CHOOSEFILE_MAXLEN; i > 0; i--)
    {
      if(tmp[i] == '/')
      {
        tmp[i] = 0;
        break;
      }
    }
    
    res = f_opendir(&dir, tmp);
    
    if (res == FR_OK)
    {
      
      res = f_readdir(&dir, &fno);
    }
    break;
    
  case CHOOSEFILE_FINDNEXT:
    res = f_readdir(&dir, &fno);
    break;
  }
  
  if (res == FR_OK)
  {
    strcpy(fn, fno.fname);
//    fn = fno.fname;
    
    while (((fno.fattrib & AM_DIR) == 0) && (res == FR_OK))
    {
      
      if((strstr(pInfo->pMask, ".img")))
      {
        if((strstr(fn, ".bmp")) || (strstr(fn, ".jpg")) || (strstr(fn, ".BMP")) || (strstr(fn, ".JPG")))
        {
          break;
        }
        else
        {
          res = f_readdir(&dir, &fno);
          
          if (res != FR_OK || fno.fname[0] == 0)
          {
            f_closedir(&dir); 
            return 1;
          }
          else
          {
            strcpy(fn, fno.fname);
//            fn = fno.fname;
          }
        }
        
      }
      else if((strstr(pInfo->pMask, ".audio")))
      {
        if((strstr(fn, ".wav")) || (strstr(fn, ".WAV")))
        {
          break;
        }
        else
        {
          res = f_readdir(&dir, &fno);
          
          if (res != FR_OK || fno.fname[0] == 0)
          {
            f_closedir(&dir); 
            return 1;
          }
          else
          {
            strcpy(fn, fno.fname);
//            fn = fno.fname;
          }
        }
        
      }
      
      else if((strstr(pInfo->pMask, ".video")))
      {
        if((strstr(fn, ".emf")) || (strstr(fn, ".EMF")))
        {
          break;
        }
        else
        {
          res = f_readdir(&dir, &fno);
          
          if (res != FR_OK || fno.fname[0] == 0)
          {
            f_closedir(&dir); 
            return 1;
          }
          else
          {
            strcpy(fn, fno.fname);
//            fn = fno.fname;
          }
        }
        
      }      
      else if(strstr(fn, pInfo->pMask) == NULL)
      {
        
        res = f_readdir(&dir, &fno);
        
        if (res != FR_OK || fno.fname[0] == 0)
        {
          f_closedir(&dir); 
          return 1;
        }
        else
        {
          strcpy(fn, fno.fname);
//          fn = fno.fname;
        }
      }
      else
      {
        break;
      }
    }   
    
    if(fn[0] == 0)
    {
      f_closedir(&dir); 
      return 1;
    } 
    
    pInfo->Flags = ((fno.fattrib & AM_DIR) == AM_DIR) ? CHOOSEFILE_FLAG_DIRECTORY : 0;
    
    for (i = 0; i < GUI_COUNTOF(_aAttrib); i++)
    {
      if (fno.fattrib & _aAttrib[i].Mask)
      {
        c = _aAttrib[i].c;
      }
      else
      {
        c = '-';
      }
      acAttrib[i] = c;
    }
    if((fno.fattrib & AM_DIR) == AM_DIR)
    {
      acExt[0] = 0;
    }
    else
    {
      GetExt(fn, acExt);
    }
    pInfo->pAttrib = acAttrib;
    pInfo->pName = fn;
    pInfo->pExt = acExt;
    pInfo->SizeL = fno.fsize;
    pInfo->SizeH = 0;
    
  }
  return res;
}

/**
  * @}
  */

/**
  * @}
  */

