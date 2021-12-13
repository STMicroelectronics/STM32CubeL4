/**
  ******************************************************************************
  * @file    iddmeasure_win.c
  * @author  MCD Application Team   
  * @brief   Idd measures window file
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
#include "iddmeasure.h"
#include "iddmeasure_res.c"
#include <math.h> 

/** @addtogroup IDD_Measure_MODULE
  * @{
  */

/** @defgroup IDD measure
  * @brief IDD measure routines  
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, U16 xpos, U16 ypos);
BUTTON_SKINFLEX_PROPS props, props1;
extern uint32_t iddTestIndex;
/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  idd_measure_board =
{  
  10,
  (uint8_t*)"",
  &bmlowpower,
  Startup,
}
;


/* Private defines -----------------------------------------------------------*/
#define ID_FRAMEWIN_INFO        (GUI_ID_USER + 0x01)
#define ID_INFO_EXIT            (GUI_ID_USER + 0x02)
#define ID_TEXT_COPYRIGHT       (GUI_ID_USER + 0x03)
#define ID_TEXT_TITLE           (GUI_ID_USER + 0x04)
#define ID_TEXT_BOARD           (GUI_ID_USER + 0x05)
#define ID_TEXT_CORE            (GUI_ID_USER + 0x06)
#define ID_TEXT_CPU             (GUI_ID_USER + 0x07)
#define ID_TEXT_VERSION         (GUI_ID_USER + 0x08)

#define ID_TEXT_BOARD_1           (GUI_ID_USER + 0x09)
#define ID_TEXT_CORE_1            (GUI_ID_USER + 0x0A)
#define ID_TEXT_CPU_1             (GUI_ID_USER + 0x0B)
#define ID_TEXT_VERSION_1         (GUI_ID_USER + 0x0C)

#define ID_BUTTON_ACTIVATE        (GUI_ID_USER + 0x0D)
#define ID_TEXT_AMPERE            (GUI_ID_USER + 0x0E)

#define ID_TEXT_RIGHT            (GUI_ID_USER + 0x0F)
#define ID_TEXT_LEFT             (GUI_ID_USER + 0x10)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  { WINDOW_CreateIndirect, "Information", 0, 0,   0, 240, 240, FRAMEWIN_CF_MOVEABLE },
};

static WM_HTIMER hTimerTime;
static long progress;
static WM_HWIN hWin_LP;
static WM_HWIN hWin_child;

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Paints exit button
* @param  repeat_status: repeat button status
* @retval None
*/
static void _OnPaint_exit(BUTTON_Handle hObj) {

  GUI_DrawBitmap(&bmLP_exit, 0, 0);
}

/**
* @brief  callback for usb button
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

static void _cbMainDialog(WM_MESSAGE * pMsg) {
  U32 NCode;

  switch (pMsg->MsgId)
  {
  case WM_PAINT:

    break;

  case WM_NOTIFY_PARENT:
    NCode = pMsg->Data.v;

    switch (NCode)
    {
    case WM_NOTIFICATION_CLICKED:

      break;

    case WM_NOTIFICATION_RELEASED:

      break;

    default:
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
* @brief  Callback routine of informing user about LCD shut down
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbHint(WM_MESSAGE * pMsg)
{
  WM_HWIN hItem;

  switch (pMsg->MsgId)
  {
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font20_ASCII);
    if (iddTestIndex != 0)
    {    
      /* in all modes except Run mode, shut down the LCD screen during power consumption measurement */ 
      GUI_DispStringAt("LCD shut down", 60, 127);      
      
      GUI_SetColor(GUI_GREEN);
      GUI_SetPenSize(15);
      GUI_AA_DrawArc(120, 140, 80, 80, 190, 550);            
    }
    else
    {
      /* in Run mode, keep LCD screen on */
      hItem = TEXT_CreateEx(65, 10, 120, 25, hWin_child, WM_CF_SHOW, 0, ID_TEXT_TITLE, IddTest[IDD_RUN].modeStr);
      TEXT_SetFont(hItem, &GUI_Font20B_1);
      TEXT_SetTextColor(hItem, GUI_WHITE); 
      GUI_DispStringAt("Measuring...", 70, 127);
    
      GUI_SetColor(0x0E4501);
      GUI_FillRect(0, 0, 240, 40);
      
      GUI_SetColor(GUI_GREEN);
      GUI_SetPenSize(15);
      GUI_AA_DrawArc(120 - (WM_GetWindowOrgX(hWin_child)), 140, 80, 80, 190, 550);        
    }  
    GUI_SetFont(GUI_DEFAULT_FONT);
    break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbDialog
*
* Purpose
*   Callback routine of the dialog
*/

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode, mode;
  WM_HWIN Hint;

  WM_MOTION_INFO  * pInfo_LP;

  static int IsPressed = 0;

  switch (pMsg->MsgId) {
  case WM_CREATE:
    
    /* Run Mode **********************************************************************/
    mode = 0;
    hItem = BUTTON_CreateEx(196, 3, 40, 70, pMsg->hWin, WM_CF_SHOW, 0, ID_INFO_EXIT);
    WM_SetCallback(hItem, _cbButton_exit);

    hItem = TEXT_CreateEx(65, 10, 120, 25, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_TITLE, IddTest[IDD_RUN].modeStr);
    TEXT_SetFont(hItem, &GUI_Font20B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);

    hItem = BUTTON_CreateEx(70, 150, 100, 34, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_ACTIVATE);
    BUTTON_SetFont(hItem, &GUI_Font20B_1);
    BUTTON_SetText(hItem, "Activate");
    
    hItem = TEXT_CreateEx(120-35, 100, 70, 20, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_AMPERE, 0);
    IddTest[IDD_RUN].userData1 = hItem;
    TEXT_SetFont(hItem, &GUI_Font20B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);
    TEXT_SetTextAlign(hItem, GUI_TA_CENTER);
    TEXT_SetText(hItem, "- -");

    WM_InvalidateWindow(hItem);

    hItem = TEXT_CreateEx(220, 95, 20, 32, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_RIGHT, ">");
    TEXT_SetFont(hItem, &GUI_Font32B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);
    /* Run Mode **********************************************************************/
    
    /* Sleep Mode ********************************************************************/
    mode++;
    hItem = TEXT_CreateEx(295, 10, 120, 25, pMsg->hWin, WM_CF_SHOW, WM_CF_ANCHOR_TOP, ID_TEXT_TITLE, IddTest[IDD_SLEEP].modeStr);
    TEXT_SetFont(hItem, &GUI_Font20B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);

    hItem = TEXT_CreateEx(360-35, 100, 70, 20, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_AMPERE, 0);
    IddTest[IDD_SLEEP].userData1 = hItem;
    TEXT_SetFont(hItem, &GUI_Font20B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);
    TEXT_SetTextAlign(hItem, GUI_TA_CENTER);
    TEXT_SetText(hItem, "- -");
    
    hItem = TEXT_CreateEx(460, 95, 20, 32, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_RIGHT, ">");
    TEXT_SetFont(hItem, &GUI_Font32B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);

    hItem = TEXT_CreateEx(246, 95, 20, 32, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_LEFT, "<");
    TEXT_SetFont(hItem, &GUI_Font32B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);

    hItem = BUTTON_CreateEx(196+(240*mode), 3, 40, 70, pMsg->hWin, WM_CF_SHOW, 0, ID_INFO_EXIT);
    WM_SetCallback(hItem, _cbButton_exit);

    hItem = BUTTON_CreateEx(310, 150, 100, 34, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_ACTIVATE);
    BUTTON_SetFont(hItem, &GUI_Font20B_1);
    BUTTON_SetText(hItem, "Activate");

    WM_InvalidateWindow(hItem);
    /* Sleep Mode ********************************************************************/

    /* LPRUN Mode ******************************************************************/
    mode++;
    hItem = TEXT_CreateEx(525, 10, 140, 25, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_TITLE, IddTest[IDD_LPR_2MHZ].modeStr);
    TEXT_SetFont(hItem, &GUI_Font20B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);

    hItem = TEXT_CreateEx(600-35, 100, 70, 20, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_AMPERE, 0);
    IddTest[IDD_LPR_2MHZ].userData1 = hItem;
    TEXT_SetFont(hItem, &GUI_Font20B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);
    TEXT_SetTextAlign(hItem, GUI_TA_CENTER);
    TEXT_SetText(hItem, "- -");

    hItem = TEXT_CreateEx(700, 95, 20, 32, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_RIGHT, ">");
    TEXT_SetFont(hItem, &GUI_Font32B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);

    hItem = TEXT_CreateEx(486, 95, 20, 32, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_LEFT, "<");
    TEXT_SetFont(hItem, &GUI_Font32B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);

    hItem = BUTTON_CreateEx(196+(240*mode), 3, 40, 70, pMsg->hWin, WM_CF_SHOW, 0, ID_INFO_EXIT);
    WM_SetCallback(hItem, _cbButton_exit);

    hItem = BUTTON_CreateEx(550, 150, 100, 34, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_ACTIVATE);
    BUTTON_SetFont(hItem, &GUI_Font20B_1);
    BUTTON_SetText(hItem, "Activate");
    /* LPRUN Mode ******************************************************************/

    /* LPSLEEP Mode *********************************************************************/
    mode++;
    hItem = TEXT_CreateEx(760, 10, 140, 25, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_TITLE, IddTest[IDD_LPR_SLEEP].modeStr);
    TEXT_SetFont(hItem, &GUI_Font20B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);

    hItem = TEXT_CreateEx(840-35, 100, 70, 20, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_AMPERE, 0);
    IddTest[IDD_LPR_SLEEP].userData1 = hItem;
    TEXT_SetFont(hItem, &GUI_Font20B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);
    TEXT_SetTextAlign(hItem, GUI_TA_CENTER);
    TEXT_SetText(hItem, "- -");

    hItem = TEXT_CreateEx(940, 95, 20, 32, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_RIGHT, ">");
    TEXT_SetFont(hItem, &GUI_Font32B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);

    hItem = TEXT_CreateEx(726, 95, 20, 32, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_LEFT, "<");
    TEXT_SetFont(hItem, &GUI_Font32B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);

    hItem = BUTTON_CreateEx(196+(240*mode), 3, 40, 70, pMsg->hWin, WM_CF_SHOW, 0, ID_INFO_EXIT);
    WM_SetCallback(hItem, _cbButton_exit);

    hItem = BUTTON_CreateEx(790, 150, 100, 34, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_ACTIVATE);
    BUTTON_SetFont(hItem, &GUI_Font20B_1);
    BUTTON_SetText(hItem, "Activate");
    /* LPSLEEP Mode *********************************************************************/

    /* Stop2 Mode *********************************************************************/
    mode++;
    hItem = TEXT_CreateEx(1015, 10, 120, 25, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_TITLE, IddTest[IDD_STOP2].modeStr);
    TEXT_SetFont(hItem, &GUI_Font20B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);

    hItem = TEXT_CreateEx(1080-35, 100, 70, 20, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_AMPERE, 0);
    IddTest[IDD_STOP2].userData1 = hItem;
    TEXT_SetFont(hItem, &GUI_Font20B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);
    TEXT_SetTextAlign(hItem, GUI_TA_CENTER);
    TEXT_SetText(hItem, "- -");

    hItem = TEXT_CreateEx(1180, 95, 20, 32, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_RIGHT, ">");
    TEXT_SetFont(hItem, &GUI_Font32B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);

    hItem = TEXT_CreateEx(966, 95, 20, 32, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_LEFT, "<");
    TEXT_SetFont(hItem, &GUI_Font32B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);

    hItem = BUTTON_CreateEx(196+(240*mode), 3, 40, 70, pMsg->hWin, WM_CF_SHOW, 0, ID_INFO_EXIT);
    WM_SetCallback(hItem, _cbButton_exit);

    hItem = BUTTON_CreateEx(1030, 150, 100, 34, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_ACTIVATE);
    BUTTON_SetFont(hItem, &GUI_Font20B_1);
    BUTTON_SetText(hItem, "Activate");
    /* Stop2 Mode *********************************************************************/

    /* Standby Mode *********************************************************************/
    mode++;
    hItem = TEXT_CreateEx(1240, 10, 140, 25, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_TITLE, IddTest[IDD_STANDBY].modeStr);
    TEXT_SetFont(hItem, &GUI_Font20B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);

    hItem = TEXT_CreateEx(1320-35, 100, 70, 20, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_AMPERE, 0);
    IddTest[IDD_STANDBY].userData1 = hItem;
    TEXT_SetFont(hItem, &GUI_Font20B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);
    TEXT_SetTextAlign(hItem, GUI_TA_CENTER);
    TEXT_SetText(hItem, "- -");

    hItem = TEXT_CreateEx(1420, 95, 20, 32, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_RIGHT, ">");
    TEXT_SetFont(hItem, &GUI_Font32B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);

    hItem = TEXT_CreateEx(1206, 95, 20, 32, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_LEFT, "<");
    TEXT_SetFont(hItem, &GUI_Font32B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);

    hItem = BUTTON_CreateEx(196+(240*mode), 3, 40, 70, pMsg->hWin, WM_CF_SHOW, 0, ID_INFO_EXIT);
    WM_SetCallback(hItem, _cbButton_exit);

    hItem = BUTTON_CreateEx(1270, 150, 100, 34, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_ACTIVATE);
    BUTTON_SetFont(hItem, &GUI_Font20B_1);
    BUTTON_SetText(hItem, "Activate");
    /* Standby Mode *********************************************************************/

    /* Shutdown Mode *********************************************************************/
    mode++;
    hItem = TEXT_CreateEx(1480, 10, 140, 25, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_TITLE, IddTest[IDD_SHUTDOWN].modeStr);
    TEXT_SetFont(hItem, &GUI_Font20B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);


    hItem = TEXT_CreateEx(1560-35, 100, 70, 20, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_AMPERE, 0);
    IddTest[IDD_SHUTDOWN].userData1 = hItem;
    TEXT_SetFont(hItem, &GUI_Font20B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);
    TEXT_SetTextAlign(hItem, GUI_TA_CENTER);
    TEXT_SetText(hItem, "- -");

    hItem = TEXT_CreateEx(1446, 95, 20, 32, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_LEFT, "<");
    TEXT_SetFont(hItem, &GUI_Font32B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);

    hItem = BUTTON_CreateEx(196+(240*mode), 3, 40, 70, pMsg->hWin, WM_CF_SHOW, 0, ID_INFO_EXIT);
    WM_SetCallback(hItem, _cbButton_exit);

    hItem = BUTTON_CreateEx(1510, 150, 100, 34, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_ACTIVATE);
    BUTTON_SetFont(hItem, &GUI_Font20B_1);
    BUTTON_SetText(hItem, "Activate");
    /* Shutdown Mode *********************************************************************/
    break;

  case WM_PAINT:

    /* Header and background Drawing */
    GUI_SetColor(0xFF272223);
    GUI_FillRect(0, 0, 1680, 240);

    GUI_SetColor(0x0E4501);
    GUI_FillRect(0, 0, 240, 40);

    GUI_SetColor(0x146402);
    GUI_FillRect(240, 0, 480, 40);

    GUI_SetColor(0x1B8A04);
    GUI_FillRect(480, 0, 720, 40);

    GUI_SetColor(0x22AF04);
    GUI_FillRect(720, 0, 960, 40);

    GUI_SetColor(0x26CC02);
    GUI_FillRect(960, 0, 1200, 40);

    GUI_SetColor(0x2AE302);
    GUI_FillRect(1200, 0, 1440, 40);

    GUI_SetColor(0xFF00);
    GUI_FillRect(1440, 0, 1680, 40);

    GUI_SetColor(GUI_GREEN);
    GUI_SetPenSize(15);
    GUI_AA_DrawArc(120 - (WM_GetWindowOrgX(hWin_child)), 140, 80, 80, 190, 550);

    GUI_SetColor(GUI_WHITE);
    GUI_AA_DrawArc(120 - (WM_GetWindowOrgX(hWin_child)), 140, 80, 80, 90, (90 + progress));

 
    break;

  case WM_MOTION:
    pInfo_LP = (WM_MOTION_INFO *)pMsg->Data.p;
    if (pInfo_LP) {
      switch (pInfo_LP->Cmd) {
      case WM_MOTION_INIT:
        pInfo_LP->SnapX = LCD_GetXSize();
        pInfo_LP->SnapY = LCD_GetYSize();
        break;

      case WM_MOTION_MOVE:
        /* Get the index of the sub screen after a move */
        if (pInfo_LP->FinalMove) {
          iddTestIndex = (uint32_t) (fabs(pInfo_LP->xPos) + LCD_GetXSize()/2) / LCD_GetXSize();
        }
        IsPressed = 0;
        break;
      }
    }
    break;

  case WM_TIMER:

    if (progress <= 360)
    {
      progress+=10;
      WM_RestartTimer(pMsg->Data.v, 20);
    }
    else
    {
      if (hTimerTime != 0)
      {
        WM_DeleteTimer(hTimerTime);
      }

      hTimerTime = 0;
      progress = 0;
    }
    
    WM_InvalidateWindow(pMsg->hWin);
    break;

  case WM_DELETE:

    if (hTimerTime != 0)
    {
      WM_DeleteTimer(hTimerTime);
    }
    
    hTimerTime = 0;
    progress = 0;
    break;
        
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
  
  switch (NCode) {
  case WM_NOTIFICATION_CLICKED:

    IsPressed = 1;
    break;

  case WM_NOTIFICATION_RELEASED:
    if (IsPressed) {
      switch (Id) {

      case ID_INFO_EXIT:
        if (NCode == WM_NOTIFICATION_RELEASED)
        {
          
         module_prop[k_ModuleGetIndex(&idd_measure_board)].in_use = 0;
          GUI_EndDialog(hWin_LP, 0);
        }
        break;

      case ID_BUTTON_ACTIVATE:
        if (NCode == WM_NOTIFICATION_RELEASED)
        {
          Hint = WM_CreateWindowAsChild((-(WM_GetWindowOrgX(hWin_child))),
               0,
               240, 240,
               pMsg->hWin,
               WM_CF_SHOW,
               _cbHint,
               0);

          WM_Update(Hint);

          GUI_Delay(500);

          /* Delete message */
          WM_DeleteWindow(Hint);

          TEXT_SetText((WM_HWIN)IddTest[iddTestIndex].userData1, IddMeasureGetIDD());
          
          if (hTimerTime == 0)
          {
            hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 20, 0);
          }
          else
          {
            Error_Handler();
          }
        }
        break;
      }
      IsPressed = 0;
    }
    break;
  case WM_NOTIFICATION_CHILD_DELETED:

      WM_MOTION_Enable(1);

    break;
  }
    break;
   
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  USB Storage window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, U16 xpos, U16 ypos)
{
  WM_MOTION_Enable(1);
  hTimerTime = 0;
  progress = 0;  
  module_prop[k_ModuleGetIndex(&idd_measure_board)].in_use = 1;
  
  hWin_LP = GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbMainDialog, hWin, xpos, ypos);
  hWin_child = WM_CreateWindowAsChild(0, 40, 7*LCD_GetXSize(), 200, hWin_LP, 
                                      WM_CF_SHOW | WM_CF_MOTION_X, _cbDialog, 0);

  if (WakeUpFromStandby != RESET)
  {
    iddTestIndex = IDD_STANDBY;
    GUI_Exec(); /* update display */
    WM_SetWindowPos(hWin_child, - iddTestIndex*LCD_GetXSize(), 0, 7*LCD_GetXSize(), LCD_GetYSize());
    GUI_Exec(); /* update display */

    /* Backlight control signal assertion */
    HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_SET );
    
    TEXT_SetText((WM_HWIN)IddTest[iddTestIndex].userData1, IddMeasureGetIDD());
    
    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();
     /* Clear StandBy flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
    /* Disable Power Control clock */
    __HAL_RCC_PWR_CLK_DISABLE();
    
    hTimerTime = WM_CreateTimer(hWin_child, 0, 20, 0);
  }
  else if(WakeUpFromShutdown != RESET)
  {
    iddTestIndex = IDD_SHUTDOWN;

    GUI_Exec(); /* update display */
    WM_SetWindowPos(hWin_child, - iddTestIndex*LCD_GetXSize(), 0, 7*LCD_GetXSize(), LCD_GetYSize());
    GUI_Exec(); /* update display */

    /* Backlight control signal assertion */
    HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_SET );
    
    TEXT_SetText((WM_HWIN)IddTest[iddTestIndex].userData1, IddMeasureGetIDD());
    
    WRITE_REG( RTC->BKP31R, 0x0 );  /* reset back-up register */

    hTimerTime = WM_CreateTimer(hWin_child, 0, 20, 0);
  }
  else
  {
    WM_SetWindowPos(hWin_child, 0, 0, 7*LCD_GetXSize(), LCD_GetYSize());
    iddTestIndex = IDD_RUN;
  }
}

/**
  * @}
  */

/**
  * @}
  */

