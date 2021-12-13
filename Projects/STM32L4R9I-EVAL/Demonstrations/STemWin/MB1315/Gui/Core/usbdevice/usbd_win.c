/**
  ******************************************************************************
  * @file    usbd_win.c
  * @author  MCD Application Team 
  * @brief   USB Device functions
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
#include "DIALOG.h"
#include "k_module.h"
#include "usbd_res.c"
//#include "k_modules_res.h"
#include "k_storage.h"
#include "usbd_app.h"

/** @addtogroup USB_DEVICE_MODULE
  * @{
  */

/** @defgroup USB_DEVICE
  * @brief usb device routines 
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  usb_device_board =
{
  MODULE_ID_USBD,
  "USB Device",
  open_usbdevice,
  0,
  Startup,
  NULL,
}
;

typedef union
{
  uint32_t d32;
  struct
  {
    uint32_t sd_mounted         : 1;    
    uint32_t connection         : 1;
  }b;
}
USBDSettingsTypeDef;

/* Private defines -----------------------------------------------------------*/
#define ID_FRAMEWIN_INFO        (GUI_ID_USER + 0x01)
#define ID_USB_NOT_CONNECTED    (GUI_ID_USER + 0x02)
#define ID_MSD_NOT_CONNECTED    (GUI_ID_USER + 0x03)
#define ID_BUTTON_USB           (GUI_ID_USER + 0x04)

/* USB and MSD info */
#define ID_IMAGE1_CONNECTED      (GUI_ID_USER + 0x12)
#define ID_IMAGE1_NOT_CONNECTED  (GUI_ID_USER + 0x13)
#define ID_IMAGE2_CONNECTED      (GUI_ID_USER + 0x14)
#define ID_IMAGE2_NOT_CONNECTED  (GUI_ID_USER + 0x15)

#define ID_BUTTON_EXIT           (GUI_ID_USER + 0x20)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  { WINDOW_CreateIndirect,   "USB Mass Storage Device [mSD]",   ID_FRAMEWIN_INFO,        1,   0,   480, 272, 0, 0x0, 0 },
  { IMAGE_CreateIndirect,    "Image",        ID_USB_NOT_CONNECTED,    120,  26,  100, 100,  0, 0,  0 },
  { IMAGE_CreateIndirect,    "Image",        ID_MSD_NOT_CONNECTED,    245,  26,  100, 100,  0, 0,  0 },
  { BUTTON_CreateIndirect,   "Connect USB ", ID_BUTTON_USB,           120, 150,  230,  35,  0, 0,  0 },
  { IMAGE_CreateIndirect,    "",             ID_IMAGE1_CONNECTED,     176,  87,   18,  19,  0, 0,  0 },
  { IMAGE_CreateIndirect,    "",             ID_IMAGE2_CONNECTED,     301,  87,   18,  19,  0, 0,  0 },
  

};

static WM_HWIN hMainWin;
static uint8_t prev_storage_status = -1;
USBDSettingsTypeDef  USBDSettings;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Paints exit button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_exit(BUTTON_Handle hObj) {

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();

  GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
  GUI_AA_FillCircle(60, 0, 60);

  GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_FontLubalGraph16);
  GUI_DispStringAt("Menu", 15, 13);
}

/**
  * @brief  callback for Exit button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_exit(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_exit(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Callback function of the media connection status
  * @param  uint id, status
  * @retval None
  */
static void storage_update_status(uint8_t uint, uint8_t status)
{
  WM_HWIN hItem;  

  if ((uint != MSD_DISK_UNIT) || (prev_storage_status == status))
    return;

  prev_storage_status = status;

  if(prev_storage_status == 1)
  {
    hItem = WM_GetDialogItem(WM_GetParent(hMainWin), ID_IMAGE2_CONNECTED);
    IMAGE_SetBitmap(hItem, &bmconnected);
  }
  else
  {
    hItem = WM_GetDialogItem(WM_GetParent(hMainWin), ID_IMAGE2_CONNECTED);
    IMAGE_SetBitmap(hItem, &bmnot_connected);  
  }
}

/**
  * @brief  Callback routine of the dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    hItem = BUTTON_CreateEx(420, 0, 60, 60, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
    WM_SetCallback(hItem, _cbButton_exit);   
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_USB_NOT_CONNECTED);
    IMAGE_SetBitmap(hItem, &bmUSB_not_connected); 
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MSD_NOT_CONNECTED);
    IMAGE_SetBitmap(hItem, &bmsdcard_not_connected); 
    
    USBDSettings.b.connection = DISCONNECTED;  
    
    break;
    
  case WM_PAINT:
    DrawRect3D(110,  20, 250, 180);   
    break;
   
  case WM_DELETE:
    USBDSTOR_Stop();
    k_StorageRegisterStatusCallBack(MSD_DISK_UNIT, 0);
    break;
    
 case WM_NOTIFY_PARENT:
   Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
   NCode = pMsg->Data.v;               /* Notification code */
   
   switch(Id) {
     
     /* Notifications sent by 'Connect' Button */
   case ID_BUTTON_USB: 
     if(NCode == WM_NOTIFICATION_RELEASED)
     {
       hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_USB);         
       if(USBDSettings.b.connection == DISCONNECTED)
       {
         BUTTON_SetText(hItem, "Disconnect USB");
         hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE1_CONNECTED);
         IMAGE_SetBitmap(hItem, &bmconnected);          
         USBDSettings.b.connection = CONNECTED;
         USBDSTOR_Connect();
       }
       else
       { 
         BUTTON_SetText(hItem, "Connect USB ");
         hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE1_CONNECTED);
         IMAGE_SetBitmap(hItem, &bmnot_connected);           
         USBDSettings.b.connection = DISCONNECTED;
         USBDSTOR_Disconnect();          
       }
     }
     break;  
   case ID_BUTTON_EXIT: 
     switch(NCode) {
     case WM_NOTIFICATION_RELEASED:
       GUI_EndDialog(pMsg->hWin, 0);       
       break;
     }
     break;  
   }
   break;
   
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  USB Device window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  USBDSettings.b.connection = DISCONNECTED;
  BUTTON_SetReactOnLevel();

  /* Initialize/DeInitialize the Storage Units : Just start the Storage thread and get the initial Status */
  k_StorageInit();
  k_StorageDeInit();

  if(k_StorageRegisterStatusCallBack(MSD_DISK_UNIT, storage_update_status) == 0)
    printf("Error While registering Storage Callback !!!!\n");

  USBDSTOR_Start();
  
  hMainWin = GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog, hWin, xpos, ypos);
  
  prev_storage_status = -1;
  if (k_StorageGetStatus(MSD_DISK_UNIT))
    storage_update_status(MSD_DISK_UNIT, 1);
  else
    storage_update_status(MSD_DISK_UNIT, 0);
}

/**
  * @}
  */

/**
  * @}
  */

