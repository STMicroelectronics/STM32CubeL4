/**
  ******************************************************************************
  * @file    iddmeasure_win.c
  * @author  MCD Application Team
  * @brief   IDD Measure functions
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
#include "dialog.h"
#include "k_module.h"
#include "iddmeasure_res.c"
#include "k_modules_res.h"
#include "k_storage.h"
#include "k_rtc.h"    
#include "iddmeasure_app.h"

/** @addtogroup IDD_MEASURE_MODULE
  * @{
  */

/** @defgroup IDD_MEASURE
  * @brief idd measure routines 
  * @{
  */


/* External variables --------------------------------------------------------*/
uint32_t iddStandbyMeasured = 0;

/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  idd_measure =
{
  11,
  "IDD Measure",
  &bmiddmeasure,
  Startup,
  NULL,
}
;

typedef union
{
  uint32_t d32;
  struct
  {
    uint32_t mode         : 3;
    uint32_t calibration  : 29;
  }b;
}
IDDSettingsTypeDef;

typedef enum
{
  IDDMEASURE_NO_MODE = 0,
  IDDMEASURE_RUN_MODE,
  IDDMEASURE_SLEEP_MODE,
  IDDMEASURE_STOP_MODE,
  IDDMEASURE_STANDBY_MODE,
  
}IDDMEASURE_ModeTypedef;

typedef enum
{
  IDDCAL_STEP_0 = 0,
  IDDCAL_STEP_1,
  IDDCAL_STEP_2,
  IDDCAL_STEP_3,
  IDDCAL_STEP_4,

}IDDMEASURE_CalTypedef;

/* Private defines -----------------------------------------------------------*/
#define ID_CALIBRATION_TIMER    1

#define ID_FRAMEWIN_0           (GUI_ID_USER + 0x01)
#define ID_BUTTON_RUN           (GUI_ID_USER + 0x03)
#define ID_BUTTON_SLEEP         (GUI_ID_USER + 0x04)
#define ID_BUTTON_STOP          (GUI_ID_USER + 0x05)
#define ID_BUTTON_STANDBY       (GUI_ID_USER + 0x06)
#define ID_BUTTON_CALIBRATION   (GUI_ID_USER + 0x07)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Dialog resource using a WINDOW widget */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
  { FRAMEWIN_CreateIndirect, "IDD Measure", ID_FRAMEWIN_0, 0, 0, 320, 214, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "RUN (80Mhz)", ID_BUTTON_RUN, 20, 20, 100, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "SLEEP (80Mhz)", ID_BUTTON_SLEEP, 20, 120, 100, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "STOP", ID_BUTTON_STOP, 190, 20, 100, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CALIBRATION", ID_BUTTON_CALIBRATION, 190, 120, 100, 50, 0, 0x0, 0 }
};

static WM_HWIN          IDDMEASURE_hWin;
static WM_HWIN          hWindowMessageBox;
static WM_HWIN          hFrameMessageBox;
static WM_HWIN          hButMessageBox;
static WM_HTIMER        hProcessCalibrationTimer;

IDDSettingsTypeDef      IDDSettings;
IDDMEASURE_CalTypedef   CalibrationStep = IDDCAL_STEP_0;

uint32_t         IDDRestart = 0;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  callback for Message Box 
  * @param  pMsg : pointer to data structure
  * @retval None.
  */
static void _cbMessageBox(WM_MESSAGE* pMsg) {
  WM_HWIN hWin;
  int Id;

  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED)
    {
      Id = WM_GetId(pMsg->hWinSrc);
      GUI_EndDialog(hWin, (Id == GUI_ID_OK) ? 1 : 0);
    }
    break;

  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Show Message Box
  * @param  pTitle: pointer to the title
  * @param  pText: pointer to the text
  * @param  OkButton: 1 mean add OK button, 0 mean no OK button
  * @retval int 
  */ 
static int _ShowMessageBox(WM_HWIN hWin, const char* pTitle, const char* pText, int OkButton) {
  WM_HWIN hClient;
  int r = 0;
  /* Create frame win */
  hFrameMessageBox = FRAMEWIN_CreateEx(65, 62, 190, 90, hWin, WM_CF_SHOW, FRAMEWIN_CF_MOVEABLE, 0, pTitle, &_cbMessageBox);
  FRAMEWIN_SetClientColor   (hFrameMessageBox, GUI_WHITE);
  FRAMEWIN_SetFont          (hFrameMessageBox, &GUI_Font16B_ASCII);
  FRAMEWIN_SetTextAlign     (hFrameMessageBox, GUI_TA_HCENTER);
  /* Create dialog items */
  hClient = WM_GetClientWindow(hFrameMessageBox);
  TEXT_CreateEx(10, 7, 170, 30, hClient, WM_CF_SHOW, GUI_TA_HCENTER, 0, pText);
  if(OkButton)
  {
    hButMessageBox = BUTTON_CreateEx(64, 45, 55, 18, hClient, WM_CF_SHOW, 0, GUI_ID_OK);
    BUTTON_SetText        (hButMessageBox, "Ok");
  }
  
  WM_SetFocus(hFrameMessageBox);  
  WM_MakeModal(hFrameMessageBox);
  r = GUI_ExecCreatedDialog(hFrameMessageBox);  
  return r;
}

/**
  * @brief  callback for calibration process
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbCalibrationProcess(WM_MESSAGE * pMsg) {

  uint32_t Id;
  static char tmp[50];
  
  switch (pMsg->MsgId) 
  {

  case WM_CREATE:
    hProcessCalibrationTimer = WM_CreateTimer(pMsg->hWin, ID_CALIBRATION_TIMER, 500, 0);
    CalibrationStep = IDDCAL_STEP_1;
    break;
    
  case WM_TIMER:
    
    Id = WM_GetTimerId(pMsg->Data.v);
            
    if(Id == ID_CALIBRATION_TIMER)
    {
      switch(CalibrationStep)
      {
        case IDDCAL_STEP_1 :
          if(IDDSettings.b.calibration == 0)
          {
            _ShowMessageBox(pMsg->hWin, "IDD Calibration", "At first time, you need to\ncalibrate the IDD", 1);
            _ShowMessageBox(pMsg->hWin, "IDD Calibration", "Before start calibration, JP2 must\nbe set on +3.3V position", 1);

          }

          /* Update Next Calibration Step */
          CalibrationStep = IDDCAL_STEP_2;

          WM_RestartTimer(pMsg->Data.v, 100);
          _ShowMessageBox(pMsg->hWin, "IDD Calibration", "Now set JP11 on VDD position\nPress Tamper button when done", 0);
          break;

        case IDDCAL_STEP_2 :
          /* Call Stop mode to move JP11 jumper */
          IDDMEASURE_StopModeForJumperSwitching();
      
          /* Call calibration function */
          IDDSettings.b.calibration = IDDMEASURE_GetCalibrationOffset();
          k_BkupSaveParameter(CALIBRATION_IDDMEASURE_SETTING_BKP, IDDSettings.d32);
          
          /* Close the Previous Message Box */
          GUI_EndDialog(hFrameMessageBox, 1);
          
          /* Update Next Calibration Step */
          CalibrationStep = IDDCAL_STEP_3;
          WM_RestartTimer(pMsg->Data.v, 100);
          break;

        case IDDCAL_STEP_3 :
          sprintf((char *)tmp , "Calibration IDD measured :\n %02d", IDDSettings.b.calibration);
          _ShowMessageBox(pMsg->hWin, "IDD measurement Result", (char *)tmp, 1); 

          /* Update Next Calibration Step */
          CalibrationStep = IDDCAL_STEP_4;
          WM_RestartTimer(pMsg->Data.v, 100);
          _ShowMessageBox(pMsg->hWin, "IDD Calibration (Done)", "Now set JP11 on IDD position\nPress Tamper button when done", 0);
          break;

        case IDDCAL_STEP_4 :
          /* Call Stop mode to move JP11 jumper */
          IDDMEASURE_StopModeForJumperSwitching();

          /* Close the Previous Message Box */
          GUI_EndDialog(hFrameMessageBox, 1);

          _ShowMessageBox(pMsg->hWin, "IDD Module", "Calibration done. Board ready\nfor current measurement.", 1);

          /* Reset Calibration Step and Window */
          CalibrationStep = IDDCAL_STEP_0;
          WM_DeleteWindow(hWindowMessageBox);
          break;

        default:
          break; 
      }
    }
    break; 
    
  case WM_DELETE:
    WM_DeleteTimer(hProcessCalibrationTimer);
    break;

  default:
    WM_DefaultProc(pMsg);
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
  uint32_t value;
  static char tmp[50];
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    /* Initialization of 'IDD measure'  */
    hItem = pMsg->hWin;
    FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);     

    IDDSettings.d32 = k_BkupRestoreParameter(CALIBRATION_IDDMEASURE_SETTING_BKP);

    if(IDDSettings.b.calibration == 0)
    {
      hWindowMessageBox = WM_CreateWindowAsChild(0, 0, 320, 240, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbCalibrationProcess, 0);
    }
    
    if(IDDSettings.b.mode == IDDMEASURE_STANDBY_MODE)
    {
      /* First reset the mode */
      IDDSettings.b.mode = IDDMEASURE_NO_MODE;
      k_BkupSaveParameter(CALIBRATION_IDDMEASURE_SETTING_BKP, IDDSettings.d32);

      /* Display Standby Current Measure */
      sprintf((char *)tmp , "STANDBY IDD measured :\n %02d nA", iddStandbyMeasured);
      _ShowMessageBox(pMsg->hWin, "IDD measurement Result", (char *)tmp, 1); 
    }
    break;
    
  case WM_PAINT:
    break;

  case WM_DELETE:
    break;
    
 case WM_NOTIFY_PARENT:
   Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
   NCode = pMsg->Data.v;               /* Notification code */
   
   switch(Id) {
   case ID_BUTTON_CALIBRATION: // Notifications sent by 'New Calibration'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
          hWindowMessageBox = WM_CreateWindowAsChild(0, 0, 320, 240, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbCalibrationProcess, 0);
        break;
      }
      break;

   case ID_BUTTON_RUN: // Notifications sent by 'RUN'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        IDDSettings.b.mode = IDDMEASURE_RUN_MODE;
        k_BkupSaveParameter(CALIBRATION_IDDMEASURE_SETTING_BKP, IDDSettings.d32);
        break;
      case WM_NOTIFICATION_RELEASED:
        value = IDDMEASURE_GetRunConsumption(IDDSettings.b.calibration);
        sprintf((char *)tmp , "RUN IDD measured :\n %lu mA", value/1000);
        _ShowMessageBox(IDDMEASURE_hWin, "IDD measurement Result", (char *)tmp, 01); 
        IDDSettings.b.mode = IDDMEASURE_NO_MODE;
        k_BkupSaveParameter(CALIBRATION_IDDMEASURE_SETTING_BKP, IDDSettings.d32);
        break;
      }
      break;

    case ID_BUTTON_SLEEP: // Notifications sent by 'SLEEP'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        IDDSettings.b.mode = IDDMEASURE_SLEEP_MODE;
        k_BkupSaveParameter(CALIBRATION_IDDMEASURE_SETTING_BKP, IDDSettings.d32);
        break;
      case WM_NOTIFICATION_RELEASED:
        value = IDDMEASURE_GetSleepConsumption(IDDSettings.b.calibration);
        sprintf((char *)tmp , "SLEEP IDD measured (80Mhz):\n %lu mA", value/1000);
        _ShowMessageBox(IDDMEASURE_hWin, "IDD measurement Result", (char *)tmp, 1); 
        IDDSettings.b.mode = IDDMEASURE_NO_MODE;
        k_BkupSaveParameter(CALIBRATION_IDDMEASURE_SETTING_BKP, IDDSettings.d32);
        break;
      }
      break;

   case ID_BUTTON_STOP: // Notifications sent by 'STOP'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        IDDSettings.b.mode = IDDMEASURE_STOP_MODE;
        k_BkupSaveParameter(CALIBRATION_IDDMEASURE_SETTING_BKP, IDDSettings.d32);
        break;
      case WM_NOTIFICATION_RELEASED:
        value = IDDMEASURE_GetStopConsumption(IDDSettings.b.calibration);
        sprintf((char *)tmp , "STOP IDD measured :\n %lu nA", value);
        _ShowMessageBox(IDDMEASURE_hWin, "IDD measurement Result", (char *)tmp, 1); 
        IDDSettings.b.mode = IDDMEASURE_NO_MODE;
        k_BkupSaveParameter(CALIBRATION_IDDMEASURE_SETTING_BKP, IDDSettings.d32);
        break;
      }
      break;
     
    case ID_BUTTON_STANDBY: // Notifications sent by 'STANDBY'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        IDDSettings.b.mode = IDDMEASURE_STANDBY_MODE;
        k_BkupSaveParameter(CALIBRATION_IDDMEASURE_SETTING_BKP, IDDSettings.d32);
        break;
      case WM_NOTIFICATION_RELEASED:
        IDDMEASURE_EnterStandbyMode();
        value = 0;
        sprintf((char *)tmp , "STANDBY IDD measured :\n %lu nA", value);
        _ShowMessageBox(IDDMEASURE_hWin, "IDD measurement Result", (char *)tmp, 1); 
        IDDSettings.b.mode = IDDMEASURE_NO_MODE;
        k_BkupSaveParameter(CALIBRATION_IDDMEASURE_SETTING_BKP, IDDSettings.d32);
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
  * @brief  IDD Measure Device window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  IDDMEASURE_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, xpos, ypos);
}

/**
  * @}
  */

/**
  * @}
  */

