/**
  ******************************************************************************
  * @file    audiorecorder_win.c
  * @author  MCD Application Team
  * @brief   Audio recorder functions
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
#include "audiorecorder_app.h"
#include "audiorecorder_res.c"
#include "k_modules_res.h"
#include "k_storage.h"
#include "k_rtc.h"
#include "k_temperature.h"
#include "k_mem.h"
#include "../Modules/filebrowser/filebrowser_app.h"
#include "../Modules/audioplayer/audioplayer_app.h"

/** @addtogroup AUDIO_RECORDER_MODULE
  * @{
  */

/** @defgroup AUDIO_RECORDER
  * @brief audio player routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  audio_recorder =
{
  9,
  "Audio Recorder",
  &bmaudiorecorder,
  Startup,
  NULL,
};

typedef union
{
  uint32_t d32;
  struct
  {
    uint32_t samplerate     : 3;
    uint32_t stereomono     : 1;
    uint32_t volume         : 8;
    uint32_t status         : 4;
    uint32_t reserved       : 20;
  }b;
}
AudioRecordSettingsTypeDef;

/* Private defines -----------------------------------------------------------*/
#define ID_RECORD_TIMER                 1

#define ID_FRAMEWIN_INFO                (GUI_ID_USER + 0x01)

/* Record List */
#define ID_RECFILE_LIST                 (GUI_ID_USER + 0x10)

/* Buttons */
#define ID_RECORD_BUTTON                (GUI_ID_USER + 0x20)
#define ID_PAUSE_BUTTON                 (GUI_ID_USER + 0x21)
#define ID_STOP_BUTTON                  (GUI_ID_USER + 0x22)
#define ID_SETTINGS_BUTTON              (GUI_ID_USER + 0x23)
#define ID_CLOSE_BUTTON                 (GUI_ID_USER + 0x24)
#define ID_ADD_BUTTON                   (GUI_ID_USER + 0x25)
#define ID_OPEN_BUTTON                  (GUI_ID_USER + 0x26)

/* Audio Record information */
#define ID_ELAPSED_TIME                 (GUI_ID_USER + 0x42)

/* Record List Information*/
#define ID_SAMPLING_CAPTION             (GUI_ID_USER + 0x52)
#define ID_SAMPLING_VALUE               (GUI_ID_USER + 0x53)
#define ID_TOTAL_TIME                   (GUI_ID_USER + 0x54)

/* Record List Menu Item */
#define ID_MENU_PLAYFILE                (GUI_ID_USER + 0x55)
#define ID_MENU_DELETE                  (GUI_ID_USER + 0x56)
#define ID_MENU_DELETE_ALL              (GUI_ID_USER + 0x57)
#define ID_MENU_PROPERTIES              (GUI_ID_USER + 0x58)
#define ID_MENU_EXIT                    (GUI_ID_USER + 0x59)

/* file chooser */
#define ID_FRAMEWIN_0                   (GUI_ID_USER + 0x60)
#define ID_MULTIPAGE                    (GUI_ID_USER + 0x61)
#define ID_RADIO_0                      (GUI_ID_USER + 0x62)
#define ID_EDIT_71                      (GUI_ID_USER + 0x63)
#define ID_EDIT_70                      (GUI_ID_USER + 0x64)
#define ID_BUTTON_70                    (GUI_ID_USER + 0x65)
#define ID_BUTTON_71                    (GUI_ID_USER + 0x66)
#define ID_BUTTON_72                    (GUI_ID_USER + 0x67)
#define ID_BUTTON_73                    (GUI_ID_USER + 0x68)

/* Audio recorder settings */
#define ID_FRAMEWIN_SETTINGS            (GUI_ID_USER + 0x70)
#define ID_SAMPLE_RATE_LIST             (GUI_ID_USER + 0x71)
#define ID_BUTTON_OK                    (GUI_ID_USER + 0x72)
#define ID_BUTTON_CANCEL                (GUI_ID_USER + 0x73)
#define ID_TEXT_RATE                    (GUI_ID_USER + 0x74)

/* Audio Recorder File properties */
#define ID_FRAMEWIN_PROPERTIES          (GUI_ID_USER + 0x80)
#define ID_TEXT_FILENAME                (GUI_ID_USER + 0x81)
#define ID_TEXT_SOLID_SAMPLE_RATE       (GUI_ID_USER + 0x82)
#define ID_TEXT_SAMPLE_RATE             (GUI_ID_USER + 0x83)
#define ID_TEXT_SOLID_DURATION          (GUI_ID_USER + 0x84)
#define ID_TEXT_DURATION                (GUI_ID_USER + 0x85)
#define ID_TEXT_SOLID_FILESIZE          (GUI_ID_USER + 0x86)
#define ID_TEXT_FILESIZE                (GUI_ID_USER + 0x87)
#define ID_BUTTON_OK_FILEINFO           (GUI_ID_USER + 0x88)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Record Dialog Create */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect,    "Audio Recorder", ID_FRAMEWIN_INFO,        0,   0, 320, 214, 0, 0x64, 0 },
  { LISTVIEW_CreateIndirect,  "RecordListview",  ID_RECFILE_LIST,        8, 134, 183,  74, 0,  0x0, 0 },
  { TEXT_CreateIndirect,      "Duration",       ID_ELAPSED_TIME,        65,  90, 120,  40, 0,  0x0, 0 },
};

/* Recorder Settings Dialog Create */
static const GUI_WIDGET_CREATE_INFO _aSettingsDialogCreate[] = {
  { FRAMEWIN_CreateIndirect,  "Settings",       ID_FRAMEWIN_SETTINGS,   50,  50, 220, 100, 0,  0x0, 0 },
  { BUTTON_CreateIndirect,    "OK",             ID_BUTTON_OK,          130,   5,  70,  30, 0,  0x0, 0 },
  { BUTTON_CreateIndirect,    "Cancel",         ID_BUTTON_CANCEL,      130,  45,  70,  30, 0,  0x0, 0 },
  { LISTBOX_CreateIndirect,   "List",           ID_SAMPLE_RATE_LIST,    10,  30, 100,  40, 0,  0x0, 0 },
  { TEXT_CreateIndirect,      "Sample Rate : ", ID_TEXT_RATE,           10,  15, 100,  20, 0,  0x0, 0 },
};

/* Record File information Dialog Create */
static const GUI_WIDGET_CREATE_INFO _aRecordFileInfoDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "File Information", ID_FRAMEWIN_PROPERTIES, 0, 0, 190, 130, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "File name", ID_TEXT_FILENAME, 10, 5, 300, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Sample rate : ", ID_TEXT_SOLID_SAMPLE_RATE, 10, 25, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_SAMPLE_RATE, 75, 25, 300, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Duration : ", ID_TEXT_SOLID_DURATION, 10, 40, 60, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_DURATION, 60, 40, 300, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "File Size : ", ID_TEXT_SOLID_FILESIZE, 10, 55, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_FILESIZE, 57, 55, 300, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OK", ID_BUTTON_OK_FILEINFO, 70, 81, 40, 26, 0, 0x0, 0 },
};

/* Array of Record list menu items */
static MENU_ITEM _aRecordListMenuItems[] =
{
  {"Play File"          , ID_MENU_PLAYFILE,  0},
  {"Delete File"        , ID_MENU_DELETE, 0},
  {"Delete All Files"   , ID_MENU_DELETE_ALL, 0},
  {"Properties"         , ID_MENU_PROPERTIES, 0},
  {0                    , 0           ,  MENU_IF_SEPARATOR},
  {"Cancel"             , ID_MENU_EXIT, 0},
};

static WM_HWIN   AUDIORECORDER_hWin;
static WM_HWIN   hRecordListPopUp = 0;
static WM_HWIN   hRecordFileInfo = 0;
static WM_HTIMER hProcessRecordTimer;

static WAVE_FormatTypeDef       WavRecordInfo;
static FILELIST_FileTypeDef     *pWavRecordList;
static CHOOSEFILE_INFO          *pRecordFileInfo;

static char const               *apDrives[1] = {"0:/Record"};
static char                     RecFileName[25];
static char                     SelectedFileName[FILEMGR_FULL_PATH_SIZE];
static int                      SelectedFileNameIndex;

#define NB_SAMPLE_RATE  4
static char const               *SampleRateList[NB_SAMPLE_RATE] = {"8 Khz",
                                                                   "16 Khz",
                                                                   "44.1 Khz",
                                                                   "48 Khz"};

static uint16_t                 SampleRate[NB_SAMPLE_RATE] = {BSP_AUDIO_FREQUENCY_8K,
                                                              BSP_AUDIO_FREQUENCY_16K,
                                                              BSP_AUDIO_FREQUENCY_44K,
                                                              BSP_AUDIO_FREQUENCY_48K};

static AudioRecordSettingsTypeDef       RecorderSettings;

FIL MyRecordFile;
DIR MyRecordDir;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Paints Record button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_record(BUTTON_Handle hObj) {
  int Index;

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();

  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;

  if(Index)
  {
    if(AUDIORECORDER_GetState() == AUDIORECORDER_START)
    {
      if(RecorderSettings.b.status == PAUSE_ACTIVE)
      {
        GUI_DrawBitmap(&bmrecord_pressed, 0, 0);
      }
      else if(RecorderSettings.b.status == RECORD_ACTIVE)
      {
        GUI_DrawBitmap(&bmpause_pressed, 0, 0);
      }
    }
    else  if(AUDIORECORDER_GetState() == AUDIORECORDER_STOP)
    {
      if(RecorderSettings.b.status == RECORD_ACTIVE)
      {
        GUI_DrawBitmap(&bmrecord_pressed, 0, 0);
      }
      else if(RecorderSettings.b.status == PAUSE_ACTIVE)
      {
        GUI_DrawBitmap(&bmpause_pressed, 0, 0);
      }
    }
  }
  else
  {
    if(AUDIORECORDER_GetState() == AUDIORECORDER_START)
    {
      if(RecorderSettings.b.status == PAUSE_ACTIVE)
      {
        GUI_DrawBitmap(&bmrecord_not_pressed, 0, 0);
      }
      else if(RecorderSettings.b.status == RECORD_ACTIVE)
      {
        GUI_DrawBitmap(&bmpause_not_pressed, 0, 0);
      }
    }
    else  if(AUDIORECORDER_GetState() == AUDIORECORDER_STOP)
    {
      if(RecorderSettings.b.status == RECORD_ACTIVE)
      {
        GUI_DrawBitmap(&bmrecord_not_pressed, 0, 0);
      }
      else if(RecorderSettings.b.status == PAUSE_ACTIVE)
      {
        GUI_DrawBitmap(&bmpause_not_pressed, 0, 0);
      }
    }
  }
}

/**
  * @brief  Paints Stop button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_stop(BUTTON_Handle hObj) {
  int Index;

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();

  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;

  if(Index)
  {
    GUI_DrawBitmap(&bmstop_pressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmstop_not_pressed, 0, 0);
  }
}

/**
  * @brief  Paints Pause button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_pause(BUTTON_Handle hObj) {
  int Index;

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();

  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;

  if(Index)
  {
    GUI_DrawBitmap(&bmpause_pressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmpause_not_pressed, 0, 0);
  }
}

/**
  * @brief  Paints Close button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_close(BUTTON_Handle hObj) {
  int Index;

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();

  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;

  if(Index)
  {
    GUI_DrawBitmap(&bmclose_pressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmclose_not_pressed, 0, 0);
  }
}

/**
  * @brief  Paints Settings button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_settings(BUTTON_Handle hObj) {
  int Index;

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();

  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;

  if(Index)
  {
    GUI_DrawBitmap(&bmsettings_button_pressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmsettings_button_not_pressed, 0, 0);
  }
}

/**
  * @brief  Adds one menu item to the given menu
  * @param  hMenu:    pointer to the handle of menu
  * @param  hSubmenu: pointer to the handle of Sub menu
  * @param  pText:    pointer to menu item description
  * @param  Id:       ID of the menu item
  * @param  Flags:    window creation flags
  * @retval None
  */
static void _AddMenuItem(MENU_Handle hMenu, MENU_Handle hSubmenu, const char* pText, U16 Id, U16 Flags) {
  MENU_ITEM_DATA Item;
  Item.pText    = pText;
  Item.hSubmenu = hSubmenu;
  Item.Flags    = Flags;
  Item.Id       = Id;
  MENU_AddItem(hMenu, &Item);
}

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
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
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
  * @param  YesNo player response
  * @retval int
  */
static int _ShowMessageBox(WM_HWIN hWin, const char* pTitle, const char* pText, int YesNo) {
  WM_HWIN hFrame, hClient, hBut;
  int r = 0;
  /* Create frame win */
  hFrame = FRAMEWIN_CreateEx(65, 62, 190, 90, hWin, WM_CF_SHOW, FRAMEWIN_CF_MOVEABLE, 0, pTitle, &_cbMessageBox);
  FRAMEWIN_SetClientColor   (hFrame, GUI_WHITE);
  FRAMEWIN_SetFont          (hFrame, &GUI_Font16B_ASCII);
  FRAMEWIN_SetTextAlign     (hFrame, GUI_TA_HCENTER);
  /* Create dialog items */
  hClient = WM_GetClientWindow(hFrame);
  TEXT_CreateEx(10, 7, 170, 30, hClient, WM_CF_SHOW, GUI_TA_HCENTER, 0, pText);
  if (YesNo) {
    hBut = BUTTON_CreateEx(97, 45, 55, 18, hClient, WM_CF_SHOW, 0, GUI_ID_CANCEL);
    BUTTON_SetText        (hBut, "No");
    hBut = BUTTON_CreateEx(32, 45, 55, 18, hClient, WM_CF_SHOW, 0, GUI_ID_OK);
    BUTTON_SetText        (hBut, "Yes");
  } else {
    hBut = BUTTON_CreateEx(64, 45, 55, 18, hClient, WM_CF_SHOW, 0, GUI_ID_OK);
    BUTTON_SetText        (hBut, "Ok");
  }

  WM_SetFocus(hFrame);
  WM_MakeModal(hFrame);
  r = GUI_ExecCreatedDialog(hFrame);
  return r;
}

/**
  * @brief  Open the Record List popup menu at the given position.
  * @note   It returns immediately after creation.
  *         On the first call it creates the menu
  * @param  hParent:    pointer to the handle of the parent
  * @param  pMenuItems: pointer to menu items
  * @param  NumItems:   number of menu items
  * @param  x:          x position of the popup
  * @param  y:          y position of the popup
  * @retval None
  */
static void _OpenRecordListPopup(WM_HWIN hParent, MENU_ITEM * pMenuItems, int NumItems, int x, int y)
{
  static MENU_Handle hMenu;
  if (!hMenu) {
    int i;
    /* Create the popup window only one time */
    hMenu = MENU_CreateEx(0, 0, 0, 0, WM_UNATTACHED, 0, MENU_CF_VERTICAL, 0);
    MENU_SetBkColor(hMenu, MENU_CI_SELECTED, GUI_LIGHTBLUE);


    for (i = 0; i < NumItems; i++) {
      _AddMenuItem(hMenu, 0, pMenuItems[i].sText, pMenuItems[i].Id, pMenuItems[i].Flags);
    }
  }
  /* Open the popup menu. After opening the menu the function returns immediately.
   * After selecting menu item or after touching the display outside the menu the
   * popup menu will be closed, but not deleted.
   */
  MENU_Popup(hMenu, hParent, x, y, 0, 0, 0);
}

/**
  * @brief  Callback routine of Record File Info dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbRecordFileInfoDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  FILINFO fno;
  static char tmp[FILEMGR_FILE_NAME_SIZE];
  uint32_t duration;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    hItem = pMsg->hWin;
    FRAMEWIN_SetClientColor(hItem, GUI_WHITE);

    /* Record File Name */
    FILEMGR_GetFileOnly(tmp, SelectedFileName);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FILENAME);
    TEXT_SetText(hItem, tmp);

    /* Record File Sample Rate */
    AUDIORECORDER_GetFileInfo(SelectedFileName, &WavRecordInfo);
    sprintf((char *)tmp , "%02d Hz", WavRecordInfo.SampleRate);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SAMPLE_RATE);
    TEXT_SetText(hItem, tmp);

    /* Record File Duration */
    duration = WavRecordInfo.FileSize/(WavRecordInfo.SampleRate *
                                     WavRecordInfo.NbrChannels *
                                     (WavRecordInfo.BitPerSample/8));
    sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_DURATION);
    TEXT_SetText(hItem, tmp);

    /* Record File Size */

    f_stat (SelectedFileName, &fno);

    if (fno.fsize < 1024)
    {
      sprintf(tmp, "%lu Byte(s)", fno.fsize);
    }
    else if (fno.fsize < (1024 * 1024))
    {
      sprintf(tmp, "%lu KByte(s)", fno.fsize/ 1024);
    }
    else
    {
      sprintf(tmp, "%lu MByte(s)", fno.fsize/ 1024 / 1024);
    }
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FILESIZE);
    TEXT_SetText(hItem, tmp);

    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    /* Notifications sent by 'OK' */
    case ID_BUTTON_OK_FILEINFO:
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        GUI_EndDialog(pMsg->hWin, 0);
        hRecordFileInfo = 0;
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
  * @brief  Callback routine of Record list popup menu.
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbRecordListPopup(WM_MESSAGE * pMsg) {

  MENU_MSG_DATA* pData;
  WM_HWIN hItem;
  int index;

  switch (pMsg->MsgId)
  {
  case WM_INIT_DIALOG:
    break;

  case WM_MENU:
    /* Process the menu message */
    pData = (MENU_MSG_DATA*)pMsg->Data.p;

    switch (pData->MsgType)
    {
    case MENU_ON_ITEMSELECT:
      /* Menu item has been selected */
      switch (pData->ItemId)
      {
      case ID_MENU_PLAYFILE:
        AUDIORECORDER_GetFileInfo(SelectedFileName, &WavRecordInfo);

        AUDIOPLAYER_Init();
        AUDIOPLAYER_SetVolume(60);

        /* Open audio file */
        if(AUDIOPLAYER_SelectFile(SelectedFileName) == 0)
        {
          /* start playing */
          AUDIOPLAYER_Play(WavRecordInfo.SampleRate);
        }
        else
        {
          AUDIOPLAYER_DeInit();
        }
        break;

      case ID_MENU_DELETE:
        if (_ShowMessageBox(AUDIORECORDER_hWin, "Recorder File List", "Are you sure you want to\ndelete selected file ?", 1))
        {
          f_unlink (SelectedFileName);

          /* Update record file list */
          FILEMGR_ParseDisks((char *)apDrives[0], pWavRecordList);

          hItem = WM_GetDialogItem(AUDIORECORDER_hWin, ID_RECFILE_LIST);
          LISTVIEW_DeleteRow(hItem, SelectedFileNameIndex);
        }
        break;

      case ID_MENU_DELETE_ALL:
        if (_ShowMessageBox(AUDIORECORDER_hWin, "Recorder File List", "Are you sure you want to\ndelete all files ?", 1))
        {
          for(index = 0; index < pWavRecordList->ptr; index++)
          {
            strcpy((char *)SelectedFileName, apDrives[0]);
            strcat((char *)SelectedFileName, "/");
            strcat((char *)SelectedFileName, (char *)pWavRecordList->file[index].name);
            f_unlink (SelectedFileName);
          }

          /* Reset Record List ptr */
          pWavRecordList->ptr = 0;
          hItem = WM_GetDialogItem(AUDIORECORDER_hWin, ID_RECFILE_LIST);
          LISTVIEW_DeleteAllRows(hItem);
        }
        break;

      case ID_MENU_PROPERTIES:
        if(hRecordFileInfo == 0)
        {
          hRecordFileInfo = GUI_CreateDialogBox(_aRecordFileInfoDialogCreate,
                                                GUI_COUNTOF(_aRecordFileInfoDialogCreate),
                                                _cbRecordFileInfoDialog,
                                                AUDIORECORDER_hWin,
                                                60,
                                                50);
        }
        break;

      case ID_MENU_EXIT:
        break;
      }
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  callback for record button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_record(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_record(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for pause button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_pause(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_pause(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for stop button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_stop(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_stop(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for close button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_close(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_close(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for settings button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_settings(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_settings(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Callback function of the settings frame
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbSettingsDialog(WM_MESSAGE * pMsg)
{
  int     Id, NCode, Index;
  WM_HWIN hItem;
  uint32_t tmp_param = 0;


  switch (pMsg->MsgId)
  {
  case WM_INIT_DIALOG:

    WM_MakeModal(pMsg->hWin);
    /* Settings frame initialization */
    hItem = pMsg->hWin;
    FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);

    /* Create and attache the MULTIPAGE dialog windows */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE);

    /* 'OK' button initialization */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_OK);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    /* 'Cancel' button initialization */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CANCEL);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RATE);
    TEXT_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_SAMPLE_RATE_LIST);
    LISTBOX_SetBkColor(hItem, LISTBOX_CI_SEL, GUI_BLUE);
    LISTBOX_SetTextColor(hItem, LISTBOX_CI_SEL, GUI_WHITE);
    LISTBOX_SetBkColor(hItem, LISTBOX_CI_UNSEL, GUI_BLACK);
    LISTBOX_SetTextColor(hItem, LISTBOX_CI_UNSEL, GUI_CYAN);
    LISTBOX_SetAutoScrollV(hItem, 1);
    for(Index = 0; Index < NB_SAMPLE_RATE; Index++)
    {
      LISTBOX_AddString(hItem, SampleRateList[Index]);
    }

    LISTBOX_SetSel(hItem, RecorderSettings.b.samplerate);
    break;

  case WM_NOTIFY_PARENT:
    Id = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;

    switch (Id)
    {
    /* Notification sent by "OK" button */
    case ID_BUTTON_OK:
      switch (NCode) {

      case WM_NOTIFICATION_RELEASED:

        /* Save Recorder settings */
        hItem = WM_GetDialogItem(pMsg->hWin, ID_SAMPLE_RATE_LIST);
        RecorderSettings.b.samplerate = LISTBOX_GetSel(hItem);

        tmp_param = k_BkupRestoreParameter(CALIBRATION_AUDIORECODER_SETTING_BKP);

        /* check if new settings have to be saved */
        if(RecorderSettings.d32 != tmp_param)
        {
          k_BkupSaveParameter(CALIBRATION_AUDIORECODER_SETTING_BKP, RecorderSettings.d32);
        }

        GUI_EndDialog(pMsg->hWin, 0);
        break;
      }
      break;
    /* Notification sent by "Cancel" button */
    case ID_BUTTON_CANCEL:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        GUI_EndDialog(pMsg->hWin, 0);
        break;
      }

    case ID_SAMPLE_RATE_LIST:
      switch (NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
    break;
    }
    break;

  }
}

/**
  * @brief  callback for audio record process
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbAudioRecordProcess(WM_MESSAGE * pMsg) {

  uint32_t Id;

  WM_HWIN hItem;
  char tmp[] = "00:00";
  uint32_t duration;

  switch (pMsg->MsgId)
  {

  case WM_CREATE:
    hProcessRecordTimer = WM_CreateTimer(pMsg->hWin, ID_RECORD_TIMER, 1000, 0);
    break;

  case WM_TIMER:

    Id = WM_GetTimerId(pMsg->Data.v);

    if(Id == ID_RECORD_TIMER)
    {
      if(AUDIORECORDER_GetState() == AUDIORECORDER_START)
      {
        /*Get elapsed time */
        duration = AUDIORECORDER_GetDuration();
        sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );

        hItem = WM_GetDialogItem(AUDIORECORDER_hWin, ID_ELAPSED_TIME);
        TEXT_SetText(hItem, tmp);

        WM_Update(hItem);
      }

      AUDIORECORDER_Process();

      if(AUDIOPLAYER_GetState() == AUDIOPLAYER_EOF)
      {
        AUDIOPLAYER_Stop();
        AUDIOPLAYER_DeInit();
      }

      WM_RestartTimer(pMsg->Data.v, 1000);
    }
    break;

  case WM_DELETE:
    WM_DeleteTimer(hProcessRecordTimer);
    break;

  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Record wav file.
  * @param  None.
  * @retval None
  */
static void _RecordFile(void)
{
  static char tmp[FILEMGR_FILE_NAME_SIZE] = {0};
  RTC_TimeTypeDef   RTC_Time;
  RTC_DateTypeDef   RTC_Date;

  k_GetTime(&RTC_Time);
  k_GetDate(&RTC_Date);

  sprintf((char *)RecFileName, "Rec_%02d%02d%02d%02d.wav", RTC_Date.Date,
                                                           RTC_Time.Hours,
                                                           RTC_Time.Minutes,
                                                           RTC_Time.Seconds);

  strcpy((char *)tmp, apDrives[0]);
  strcat((char *)tmp, "/");
  strcat((char *)tmp, (char *)RecFileName);

  if(f_open(&MyRecordFile, tmp, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
  {
    _ShowMessageBox(AUDIORECORDER_hWin, "Audio Recorder", "File creation issue on SDCard\nPlease Verify SDCard\n", 0);
  }
  else
  {
    WavRecordInfo.SampleRate    = SampleRate[RecorderSettings.b.samplerate];
    WavRecordInfo.NbrChannels   = DEFAULT_AUDIO_IN_CHANNEL_NBR;
    WavRecordInfo.BitPerSample  = DEFAULT_AUDIO_IN_BIT_RESOLUTION;

    AUDIORECORDER_Start(WavRecordInfo);
  }
}

/**
  * @brief  Callback routine of the dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  int duration, index;
  static char tmp[FILEMGR_FILE_NAME_SIZE];
  GUI_PID_STATE State;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    hRecordListPopUp = WM_CreateWindowAsChild(0,
                                              26,
                                              LCD_GetXSize(),
                                              LCD_GetYSize()-26,
                                              pMsg->hWin,
                                              WM_CF_SHOW | WM_CF_HASTRANS ,
                                              _cbRecordListPopup,
                                              0);

    WM_BringToBottom(hRecordListPopUp);

    pWavRecordList = (FILELIST_FileTypeDef *)k_malloc(sizeof(FILELIST_FileTypeDef));
    pRecordFileInfo = (CHOOSEFILE_INFO *)k_malloc(sizeof(CHOOSEFILE_INFO));
    pWavRecordList->ptr = 0;

    /* Get all previous record files */
    FILEMGR_ParseDisks((char *)apDrives[0], pWavRecordList);

    /* Initialization of 'Listview' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_RECFILE_LIST);
    LISTVIEW_AddColumn(hItem, 100, "Record", GUI_TA_VCENTER | GUI_TA_LEFT);
    LISTVIEW_AddColumn(hItem, 30, "Freq", GUI_TA_VCENTER | GUI_TA_CENTER);
    LISTVIEW_AddColumn(hItem, 48, "Duration", GUI_TA_VCENTER | GUI_TA_RIGHT);
    LISTVIEW_SetGridVis(hItem, 0);
    LISTVIEW_SetAutoScrollV(hItem, 1);
    LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_UNSEL, GUI_BLACK);
    LISTVIEW_SetTextColor(hItem, LISTVIEW_CI_UNSEL, GUI_CYAN);

    /* Update "Listview" with previous record file */
    for(index = 0; index < pWavRecordList->ptr; index++)
    {
      LISTVIEW_AddRow(hItem, NULL);
      /* Get and display FileName Only */
      LISTVIEW_SetItemText(hItem, 0, index, (char *)pWavRecordList->file[index].name);

      /* Get and display Record Sample Rate and duration */
      strcpy((char *)tmp, apDrives[0]);
      strcat((char *)tmp, "/");
      strcat((char *)tmp, (char *)pWavRecordList->file[index].name);
      AUDIORECORDER_GetFileInfo(tmp, &WavRecordInfo);

      sprintf((char *)tmp , "%02d", (WavRecordInfo.SampleRate/1000));
      LISTVIEW_SetItemText(hItem, 1, index, tmp);

      duration = WavRecordInfo.FileSize / WavRecordInfo.ByteRate;
      sprintf((char *)tmp , "%02d:%02d", duration/60, duration%60 );
      LISTVIEW_SetItemText(hItem, 2, index, tmp);
    }

    /* Duration */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_ELAPSED_TIME);
    TEXT_SetText(hItem, "00:00");
    TEXT_SetFont(hItem, GUI_FONT_32B_1);
    TEXT_SetTextColor(hItem, GUI_DARKRED);

    RecorderSettings.d32 = k_BkupRestoreParameter(CALIBRATION_AUDIORECODER_SETTING_BKP);
    RecorderSettings.b.status = RECORD_ACTIVE;

    hItem = BUTTON_CreateEx(231, 10, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_PAUSE_BUTTON);
    WM_SetCallback(hItem, _cbButton_pause);

    hItem = BUTTON_CreateEx(231, 10, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_RECORD_BUTTON);
    WM_SetCallback(hItem, _cbButton_record);

    hItem = BUTTON_CreateEx(239, 70, 35, 35, pMsg->hWin, WM_CF_SHOW, 0, ID_STOP_BUTTON);
    WM_SetCallback(hItem, _cbButton_stop);

    hItem = BUTTON_CreateEx(239, 153, 30, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_SETTINGS_BUTTON);
    WM_SetCallback(hItem, _cbButton_settings);

    hItem = BUTTON_CreateEx(280, 170, 30, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_CLOSE_BUTTON);
    WM_SetCallback(hItem, _cbButton_close);

    WM_CreateWindowAsChild(0, 100, 10, 10, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbAudioRecordProcess, 0);
    break;

  case WM_PAINT:
    GUI_SetBkColor(GUI_BLACK);
    GUI_DrawBitmap(&bmaudiorecorder, 70, 20);
    DrawRect3D(195, 3, 118, 127);
    DrawRect3D(195, 131, 118, 77);
    break;

  case WM_DELETE:
    AUDIORECORDER_DeInit();

    if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
    {
      AUDIOPLAYER_Stop();
      AUDIOPLAYER_DeInit();
    }

    f_closedir(&MyRecordDir);

    k_free(pWavRecordList);
    k_free(pRecordFileInfo);

    pWavRecordList->ptr = 0;
    k_BkupSaveParameter(CALIBRATION_AUDIORECODER_SETTING_BKP, RecorderSettings.d32);

    /* Initialize the Temperature application, due to resource conflict */
    k_TemperatureInit();
    break;

  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;

    switch(Id) {

    /* Notification sent by "Button_Close" */
    case ID_CLOSE_BUTTON:
      switch (NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        GUI_EndDialog(pMsg->hWin, 0);
        break;
      }
      break;

    /* Notifications sent by 'Record' Button */
    case ID_RECORD_BUTTON:
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        if(AUDIOPLAYER_GetState() != AUDIOPLAYER_PLAY)
        {
          if(AUDIORECORDER_GetState() == AUDIORECORDER_STOP)
          {
            if(pWavRecordList->ptr >= FILEMGR_LIST_DEPDTH)
            {
              _ShowMessageBox(AUDIORECORDER_hWin, "Audio Recorder", "Too many files recorded\n Please delete file", 0);
            }
            else
            {
              _RecordFile();
            }
          }
          else if(AUDIORECORDER_GetState() == AUDIORECORDER_START)
          {
            RecorderSettings.b.status = (RecorderSettings.b.status == RECORD_ACTIVE ? PAUSE_ACTIVE : RECORD_ACTIVE);

            if(RecorderSettings.b.status == PAUSE_ACTIVE)
            {
              AUDIORECORDER_Pause();
            }
            else if(RecorderSettings.b.status == RECORD_ACTIVE)
            {
              AUDIORECORDER_Resume();
            }
          }
        }
      }
      break;


    /* Notifications sent by 'STOP' Button */
    case ID_STOP_BUTTON:
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        if(AUDIORECORDER_GetState() == AUDIORECORDER_START)
        {
          if(RecorderSettings.b.status == PAUSE_ACTIVE)
          {
            RecorderSettings.b.status = RECORD_ACTIVE;
          }
          hItem = WM_GetDialogItem(pMsg->hWin, ID_RECORD_BUTTON);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);

          /* Stop Audio Recorder */
          AUDIORECORDER_Stop();

          /* Reset Elapsed Time value */
          hItem = WM_GetDialogItem(AUDIORECORDER_hWin, ID_ELAPSED_TIME);
          TEXT_SetText(hItem, "00:00");

          /* Update Record List View with new record filename, Frequency sample rate and duration */
          hItem = WM_GetDialogItem(pMsg->hWin, ID_RECFILE_LIST);
          LISTVIEW_AddRow(hItem, NULL);

          /* Get and display FileName Only */
          strcpy((char *)pWavRecordList->file[pWavRecordList->ptr].name, (char *)RecFileName);
          LISTVIEW_SetItemText(hItem, 0, pWavRecordList->ptr, RecFileName);

          /* Get and display Record Sample Rate and duration */
          strcpy((char *)tmp, apDrives[0]);
          strcat((char *)tmp, "/");
          strcat((char *)tmp, (char *)RecFileName);
          AUDIORECORDER_GetFileInfo(tmp, &WavRecordInfo);

          sprintf((char *)tmp , "%02d", (WavRecordInfo.SampleRate/1000));
          LISTVIEW_SetItemText(hItem, 1, pWavRecordList->ptr, tmp);

          duration = WavRecordInfo.FileSize / WavRecordInfo.ByteRate;
          sprintf((char *)tmp , "%02d:%02d", duration/60, duration%60 );
          LISTVIEW_SetItemText(hItem, 2, pWavRecordList->ptr, tmp);

          /* Update record number */
          pWavRecordList->ptr++;

          LISTVIEW_SetSel(hItem, 0);
          WM_InvalidateWindow(pMsg->hWin);
        }

        if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
        {
          AUDIOPLAYER_Stop();
          AUDIOPLAYER_DeInit();
        }
      }
      break;

    /* Notifications sent by 'Settings' Button */
    case ID_SETTINGS_BUTTON:
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        GUI_CreateDialogBox(_aSettingsDialogCreate, GUI_COUNTOF(_aSettingsDialogCreate), _cbSettingsDialog, AUDIORECORDER_hWin, 0, 0);
      }
      break;

    case ID_RECFILE_LIST:
      switch (NCode) {
      case WM_NOTIFICATION_CLICKED:
        if((AUDIORECORDER_GetState() != AUDIORECORDER_START) &&
           (AUDIOPLAYER_GetState() != AUDIOPLAYER_PLAY))
        {

          hItem = WM_GetDialogItem(pMsg->hWin, ID_RECFILE_LIST);
          SelectedFileNameIndex = LISTVIEW_GetSel(hItem);
          LISTVIEW_GetItemText(hItem, 0, SelectedFileNameIndex, (char *)tmp, FILEMGR_FILE_NAME_SIZE);

          if(tmp[0] != 0x00)
          {
            GUI_TOUCH_GetState(&State);

            State.x += 20;
            State.y-= 140;

            strcpy((char *)SelectedFileName, apDrives[0]);
            strcat((char *)SelectedFileName, "/");
            strcat((char *)SelectedFileName, (char *)tmp);

            /* Create popup menu after touching the display */
            _OpenRecordListPopup(hRecordListPopUp,
                                _aRecordListMenuItems,
                                GUI_COUNTOF(_aRecordListMenuItems),
                                State.x,
                                State.y);
          }
        }
        break;
      case WM_NOTIFICATION_RELEASED:
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
  * @brief  Audio recorder window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  /* Open Record directory, create it if any */
  if(f_opendir(&MyRecordDir, apDrives[0]) != FR_OK)
  {
    if(f_mkdir(apDrives[0]) != FR_OK)
    {
      _ShowMessageBox(hWin, "Audio Recorder", "Initialisation issue on SDCard\nPlease Verify SDCard\n", 0);
    }
    else
    {
      if(f_opendir(&MyRecordDir, apDrives[0]) != FR_OK)
      {
        _ShowMessageBox(hWin, "Audio Recorder", "Initialisation issue on SDCard\nPlease Verify SDCard\n", 0);
      }
      else
      {
        /* DeInitialize the Temperature application, due to resource conflict */
        k_TemperatureDeInit();

        /* Initialise the Audio Recorder application */
        AUDIORECORDER_Init();
        AUDIORECORDER_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, xpos, ypos);
      }
    }
  }
  else
  {
    /* DeInitialize the Temperature application, due to resource conflict */
    k_TemperatureDeInit();

    /* Initialise the Audio Recorder application */
    AUDIORECORDER_Init();
    AUDIORECORDER_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, xpos, ypos);
  }
}

/**
  * @}
  */

/**
  * @}
  */

