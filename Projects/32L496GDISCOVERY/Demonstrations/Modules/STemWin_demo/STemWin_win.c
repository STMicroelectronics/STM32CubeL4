/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 2008         SEGGER Microcontroller Systeme GmbH        *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

**** emWin/GSC Grafical user interface for embedded applications ****
emWin is protected by international copyright laws. Knowledge of the
source code may not be used to write a similar product. This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : MOTION_RadialMenu.c
Purpose     : Shows how to create a radial menu with motion support
----------------------------------------------------------------------
*/

/*********************************************************************
*
*       Includes
*
**********************************************************************
*/
#include "DIALOG.h"
#include "k_module.h"
#include "STemWin_res.c"

/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, U16 xpos, U16 ypos);
extern int status;
/*********************************************************************
*
*       Public routines
*
**********************************************************************/
static void Startup(WM_HWIN hWin, U16 xpos, U16 ypos);

K_ModuleItem_Typedef  STemWin_board =
{  
  1,
  (uint8_t*)"",
  &bmSTemWin,
  Startup,
}
;

/******************************************************************************/

#define STEMWIN_FRAME_ID            (GUI_ID_USER + 0x00)
#define ID_ICONVIEW_MENU_ST         (GUI_ID_USER + 0x01)

#define ID_FRAMEWIN_2                   (GUI_ID_USER + 0x02)
#define ID_BUTTON_OK_MESSAGE2           (GUI_ID_USER + 0x03)
#define ID_TEXT_FILENAME2               (GUI_ID_USER + 0x04)

extern GUI_CONST_STORAGE GUI_BITMAP bmwarning;

ICONVIEW_Handle hIcon_SubMenu;

 /* Info Dialog Create */
static const GUI_WIDGET_CREATE_INFO _aFileInfoDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "WARNING", ID_FRAMEWIN_2, 0, 0, 200, 200, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Please insert SDCard", ID_TEXT_FILENAME2, 30, 80, 150, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OK", ID_BUTTON_OK_MESSAGE2, 65, 120, 70, 70, 0, 0x0, 0 },
};


/**
  * @brief  Paints ok message button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_okMessage(BUTTON_Handle hObj) {

  GUI_SetBkColor(GUI_WHITE);
  GUI_Clear();

  if((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_SetColor(GUI_DARKGRAY);
    GUI_AA_FillCircle(35, 35, 35);
    GUI_SetBkColor(GUI_DARKGRAY);
    GUI_SetColor(GUI_WHITE);   
  }
  else
  {
    GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
    GUI_AA_FillCircle(35, 35, 35);
    GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
    GUI_SetColor(GUI_WHITE);    
  } 
  
  GUI_SetFont(&GUI_Font20B_ASCII);
  GUI_DispStringAt("OK", 25, 10);
}

/**
  * @brief  callback for ok message button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_okMessage(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_okMessage(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}

/**
  * @brief  Callback routine of Info dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE 
  * @retval None
  */
static void _cbFileInfoDialog(WM_MESSAGE * pMsg) {
  int     NCode;
  int     Id;
  WM_HWIN hItem;
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_OK_MESSAGE2);
    WM_SetCallback(hItem, _cbButton_okMessage);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FILENAME2);
    TEXT_SetFont(hItem, &GUI_Font16B_ASCII);

    break;
        
  case WM_PAINT:    
    GUI_DrawBitmap(&bmwarning, 65, 10);
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_OK_MESSAGE2: /* Notifications sent by 'OK' */
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
  * @brief  callback routine of main menu.
  * @param  None.
  * @retval None
  */
static void cbWin(WM_MESSAGE * pMsg) {

  /* Animation */
  U32 NCode, Id;
  static U8 sel = 0;
  WM_MOTION_INFO    * pInfo;
  static int IsPressed = 0;
  WM_HWIN hItem;
  
  switch (pMsg->MsgId)
  {
        
  case WM_MOTION:
    pInfo = (WM_MOTION_INFO *)pMsg->Data.p;
    if (pInfo) {
      switch (pInfo->Cmd) {
      case WM_MOTION_INIT:
        pInfo->SnapX = LCD_GetXSize()/ 2;
        pInfo->SnapY = LCD_GetYSize()/ 2;
        
        break;
        
      case WM_MOTION_MOVE:
        IsPressed = 0;
        break;
        
      }
    }
    break;
    
  case WM_NOTIFY_PARENT:
    Id = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    
    switch (NCode)
    {
    case WM_NOTIFICATION_CLICKED:
      
      IsPressed = 1;
      if (Id == ID_ICONVIEW_MENU_ST)
      {
        sel = ICONVIEW_GetSel(pMsg->hWinSrc);
      }
      
      break;
      
    case WM_NOTIFICATION_RELEASED:
      if (IsPressed) {
        if (Id == ID_ICONVIEW_MENU_ST)
        {
          if (sel < 12)
          {
            sel += 4;
            WM_MOTION_Enable(0);
            if (sel != 4)
            {
              status = 1;
            }
            if ((sel == 4) || (sel == 5) || (sel == 6) || (sel == 8))
            {            
             if(k_StorageGetStatus(MSD_DISK_UNIT))
              {
                module_prop[sel].module->startup(pMsg->hWin, -WM_GetWindowOrgX(pMsg->hWin), 0);
              }
             else
             {
               if (sel == 4)
               {
                 status = 1;
               }
               hItem = GUI_CreateDialogBox(_aFileInfoDialogCreate, 
                                           GUI_COUNTOF(_aFileInfoDialogCreate), 
                                           _cbFileInfoDialog, 
                                            pMsg->hWin, 
                                            (20-WM_GetWindowOrgX(pMsg->hWin)), 20);
                WM_MakeModal(hItem);  
              }
            }
            else if (sel == 11)
            { 
              HAL_NVIC_SystemReset();
            }
            else
            {
              module_prop[sel].module->startup(pMsg->hWin, -WM_GetWindowOrgX(pMsg->hWin), 0);
            }                     
            sel = 0;
          }
        }
        IsPressed = 0;
      }
      break;
      
    case WM_NOTIFICATION_CHILD_DELETED:
      if (pMsg->hWinSrc != hIcon_SubMenu)
      {
        WM_MOTION_Enable(1);
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
  * @brief  Callback routine of desktop window
  * @param  pMsg:
  * @retval None
  */
static void _cbBk(WM_MESSAGE * pMsg) {
  GUI_RECT r;

  switch (pMsg->MsgId) 
  {      
  case WM_PAINT:
    WM_GetInsideRect(&r);
    GUI_ClearRectEx(&r);    
    break;
 
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  DEMO_Starup
  * @param  None
  * @retval None
  */
static void Startup(WM_HWIN hWin, U16 xpos, U16 ypos)
{
  uint8_t i = 0;
  
  WM_SetCallback(WM_HBKWIN, _cbBk);
  WM_SetWindowPos(WM_HBKWIN, -LCD_GetXSize(), 0, 3 * LCD_GetXSize() + 10, LCD_GetYSize() );
  WM_MOTION_Enable(1);
  
  hWin = WM_CreateWindowAsChild(0, 0, 4 * (LCD_GetXSize() /2), LCD_GetYSize(), WM_HBKWIN,
                                WM_CF_SHOW | WM_CF_MOTION_X, cbWin, 0);
  
  hIcon_SubMenu = ICONVIEW_CreateEx(0, 
                            0, 
                            4 * (LCD_GetXSize() /2), 
                            LCD_GetYSize(), 
                            hWin, 
                            WM_CF_SHOW,
                            0,
                            ID_ICONVIEW_MENU_ST, 
                            110, 
                            110);
  
  
  ICONVIEW_SetSpace(hIcon_SubMenu, GUI_COORD_Y, 10);
  ICONVIEW_SetFrame(hIcon_SubMenu, GUI_COORD_Y, 5);
  ICONVIEW_SetSpace(hIcon_SubMenu, GUI_COORD_X, 10);
  ICONVIEW_SetFrame(hIcon_SubMenu, GUI_COORD_X, 5);  
  
  ICONVIEW_SetBkColor(hIcon_SubMenu, ICONVIEW_CI_BK, GUI_BLACK);
    
  for (i = 4; i < 12; i++)
  {
    if (ICONVIEW_AddBitmapItem(hIcon_SubMenu,module_prop[i].module->icon, (char *)module_prop[i].module->name)) {
      Error_Handler();
    }
  }

  WM_SetWindowPos(hWin, 0, 0, 4 * (LCD_GetXSize() /2), LCD_GetYSize());
  WM_MOTION_SetDefaultPeriod(200);
}
/*************************** End of file ****************************/
