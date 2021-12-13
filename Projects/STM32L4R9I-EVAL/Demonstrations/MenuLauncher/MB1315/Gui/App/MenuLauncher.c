/**
  ******************************************************************************
  * @file    MenuLauncher.c
  * @author  MCD Application Team 
  * @brief   This file provides the kernel bsp functions
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

#include "DIALOG.h"

// USER START (Optionally insert additional includes)
#include "main.h"
#include "MenuLauncher.h"
// USER END

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0                     (GUI_ID_USER + 0x00)
#define ID_IMAGE_0                      (GUI_ID_USER + 0x01)
#define ID_BUTTON_0                     (GUI_ID_USER + 0x02)
#define ID_BUTTON_1                     (GUI_ID_USER + 0x03)
#define ID_BUTTON_2                     (GUI_ID_USER + 0x04)

#define ID_TEXT_BOARD                   (GUI_ID_USER + 0x10)
#define ID_TEXT_CPU_ID                  (GUI_ID_USER + 0x11)
#define ID_TEXT_CPU_FREQ                (GUI_ID_USER + 0x12)
#define ID_TEXT_FW_VER                  (GUI_ID_USER + 0x13)

#define ID_FRAMEWIN_0                   (GUI_ID_USER + 0x20)
#define ID_TEXT_0                       (GUI_ID_USER + 0x21)
#define ID_TEXT_1                       (GUI_ID_USER + 0x22)
#define ID_PROGBAR_0                    (GUI_ID_USER + 0x28)

// USER START (Optionally insert additional defines)
/*
 * STemWin Demonstration context
 */
#define STEW_FIRMWARE_FILE_NAME      (const char*)"MB1315_STEW_firmware.bin"
#define STEW_FIRMWARE_VERSION        0xC3010000
#define STEW_FIRMWARE_ADDRESS        0x08080000
#define STEW_FIRMWARE_OFFSET         0x200
#define STEW_RESOURCES_FILE_NAME     (const char*)"MB1315_STEW_resources.bin"
#define STEW_RESOURCES_VERSION       0xD3010000
#define STEW_RESOURCES_ADDRESS       0x90400000
#define STEW_RESOURCES_OFFSET        0x0

/*
 * TouchGFX Demonstration context
 */
#define TGFX_FIRMWARE_FILE_NAME     (const char*)"MB1315_TGFX_firmware.bin"
#define TGFX_FIRMWARE_VERSION       0xC4010000
#define TGFX_FIRMWARE_ADDRESS       0x08100000
#define TGFX_FIRMWARE_OFFSET        0x200
#define TGFX_RESOURCES_FILE_NAME    (const char*)"MB1315_TGFX_resources.bin"
#define TGFX_RESOURCES_VERSION      0xD4010000
#define TGFX_RESOURCES_ADDRESS      0x91800000
#define TGFX_RESOURCES_OFFSET       0x0

/*
 * EmbeddedWizard Demonstration context
 */
#define EWIZ_FIRMWARE_FILE_NAME      (const char*)"MB1315_EWIZ_firmware.bin"
#define EWIZ_FIRMWARE_VERSION        0xC5010000
#define EWIZ_FIRMWARE_ADDRESS        0x08180000
#define EWIZ_FIRMWARE_OFFSET         0x200
#define EWIZ_RESOURCES_FILE_NAME     (const char*)"MB1315_EWIZ_resources.bin"
#define EWIZ_RESOURCES_VERSION       0xD5010000
#define EWIZ_RESOURCES_ADDRESS       0x92C00000
#define EWIZ_RESOURCES_OFFSET        0x0

#define GUI_PROGBAR_LIGHTBLUE         0x01DDA949
#define GUI_PROGBAR_DARKBLUE          0x00421000

#define OCTOSPI_PROG_SECTOR_DURATION_MS 40     /* Assuming one secor erease then programming is taking 40 ms */

// #define ML_FORCE_SUB_DEMO_LOADING

#if defined ( ML_FORCE_SUB_DEMO_LOADING )
  #define SET_SUB_DEMO_TAG(TAG)       (0xFF000000 & TAG) /* Keep only ID and set zeroed version */
#else
  #define SET_SUB_DEMO_TAG(TAG)       TAG
#endif /* ML_FORCE_SUB_DEMO_LOADING */

#define GET_SUB_DEMO_ID(TAG)      (0xFF000000 & TAG)
#define GET_SUB_DEMO_VER(TAG)     (0x00FFFFFF & TAG)

#define ML_TaskPRIORITY           osPriorityNormal /* osPriorityIdle osPriorityBelowNormal osPriorityNormal */
#define ML_TaskSTACK_SIZE         (configMINIMAL_STACK_SIZE * 4)

// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)

typedef struct
{
  uint32_t     address;
  uint32_t     offset;
  uint32_t     tag;
  const char * filename;
} DemoInfo_t;

typedef struct
{
  char     demo_name[32];

  DemoInfo_t firmware;
  DemoInfo_t resources;
} DemoContext_t;

#if defined ( __ICCARM__ )
#pragma location="MLDemoTAGSection"
__root
#elif ! defined ( WIN32 )
__attribute__((section(".MLDemoTAGSection"))) __attribute__ ((__used__))
#endif
static const DemoContext_t SubDemo[] = {
  {
    "STemWin",
    { STEW_FIRMWARE_ADDRESS  , STEW_FIRMWARE_OFFSET  , SET_SUB_DEMO_TAG(STEW_FIRMWARE_VERSION)    , STEW_FIRMWARE_FILE_NAME  },
    { STEW_RESOURCES_ADDRESS , STEW_RESOURCES_OFFSET , SET_SUB_DEMO_TAG(STEW_RESOURCES_VERSION)   , STEW_RESOURCES_FILE_NAME }
  },
  {
    "TouchGFX",
    { TGFX_FIRMWARE_ADDRESS  , TGFX_FIRMWARE_OFFSET  , SET_SUB_DEMO_TAG(TGFX_FIRMWARE_VERSION)    , TGFX_FIRMWARE_FILE_NAME  },
    { TGFX_RESOURCES_ADDRESS , TGFX_RESOURCES_OFFSET , SET_SUB_DEMO_TAG(TGFX_RESOURCES_VERSION)   , TGFX_RESOURCES_FILE_NAME }
  },
  {
    "EmbeddedWizard",
    { EWIZ_FIRMWARE_ADDRESS  , EWIZ_FIRMWARE_OFFSET  , SET_SUB_DEMO_TAG(EWIZ_FIRMWARE_VERSION)   , EWIZ_FIRMWARE_FILE_NAME  },
    { EWIZ_RESOURCES_ADDRESS , EWIZ_RESOURCES_OFFSET , SET_SUB_DEMO_TAG(EWIZ_RESOURCES_VERSION)  , EWIZ_RESOURCES_FILE_NAME }
  },
};

static WM_HWIN hWinBase;     // Parent window for buttons and info window
static WM_HWIN hWinInfo;     // Info window for buttons and info window

#ifdef WIN32
static int test_id = 0;
#endif /* WIN32 */

// USER END

/*********************************************************************
*
*       _aMainDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aMainDialogCreate[] = {
  { WINDOW_CreateIndirect   , "ST Launcher"     , ID_WINDOW_0   ,   0,   0, 480, 272,   (WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_BGND | WM_CF_MEMDEV), 0x0, 0 },
  { IMAGE_CreateIndirect    , "Background"      , ID_IMAGE_0    ,   0,   0, 480, 272,   0, 0x0, 0 },
  { BUTTON_CreateIndirect   , "STemWin"         , ID_BUTTON_0   ,  40,  82, 108, 108,   0, 0x0, 2 },
  { BUTTON_CreateIndirect   , "TouchGFX"        , ID_BUTTON_1   , 186,  82, 108, 108,   0, 0x0, 2 },
  { BUTTON_CreateIndirect   , "EmbeddedWizard"  , ID_BUTTON_2   , 330,  82, 108, 108,   0, 0x0, 2 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       _aInfoDialogCreate
*/
/*********************************************************************
*
*       _aInfoDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aMessageCreate[] = {
  { WINDOW_CreateIndirect   , "Message"     , ID_WINDOW_0     ,   0 ,    0 , 360 , 240 , (WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_STAYONTOP | WM_CF_MEMDEV) , 0x0 , 0 },
  { IMAGE_CreateIndirect    , "Background"  , ID_IMAGE_0      ,   0 ,    0 , 360 , 240 , IMAGE_CF_ALPHA , 0x0 , 0 },
  { TEXT_CreateIndirect     , "Title"       , ID_TEXT_0       ,  20 ,   20 , 320 ,  20 , (GUI_TA_HCENTER | GUI_TA_TOP)      , 0x0 , 0 },
  { TEXT_CreateIndirect     , "Text"        , ID_TEXT_1       ,  40 ,   50 , 280 , 100 , (GUI_TA_HCENTER | GUI_TA_HCENTER)  , 0x0 , 0 },
  { BUTTON_CreateIndirect   , "YES"         , GUI_ID_OK       ,  35 ,  160 , 140 ,  70 , 0 , 0x0 , 4 },
  { BUTTON_CreateIndirect   , "NO"          , GUI_ID_CANCEL   , 185 ,  160 , 140 ,  70 , 0 , 0x0 , 4 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogMessageCreate[] = {
  { WINDOW_CreateIndirect   , "Dialog Box"  , ID_WINDOW_0     ,   0 ,    0 , 360 , 240 , (WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_STAYONTOP | WM_CF_MEMDEV) , 0x0 , 0 },
  { IMAGE_CreateIndirect    , "Background"  , ID_IMAGE_0      ,   0 ,    0 , 360 , 240 , IMAGE_CF_ALPHA , 0x0 , 0 },
  { TEXT_CreateIndirect     , "Title"       , ID_TEXT_0       ,  20 ,   20 , 320 ,  20 , (GUI_TA_HCENTER | GUI_TA_TOP)      , 0x0 , 0 },
  { TEXT_CreateIndirect     , "Text"        , ID_TEXT_1       ,  40 ,   50 , 280 , 100 , (GUI_TA_HCENTER | GUI_TA_HCENTER)  , 0x0 , 0 },
  { BUTTON_CreateIndirect   , "BACK"        , GUI_ID_OK       , 110 ,  160 , 140 ,  70 ,   0 , 0x0 , 0 },
};

static const GUI_WIDGET_CREATE_INFO _aProgressBarCreate[] = {
  { WINDOW_CreateIndirect   , "Progress Bar", ID_WINDOW_0     ,  0 ,   0 , 360 ,  72 ,   (WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_STAYONTOP | WM_CF_MEMDEV) , 0x0 , 0 },
  { IMAGE_CreateIndirect    , "Background"  , ID_IMAGE_0      ,  0 ,   0 , 360 ,  72 ,   IMAGE_CF_ALPHA , 0x0 , 0 },
  { TEXT_CreateIndirect     , "Title"       , ID_TEXT_0       ,  0 ,  10 , 360 ,  20 ,   (TEXT_CF_HCENTER | TEXT_CF_VCENTER) , 0x0 , 0 },
  { PROGBAR_CreateIndirect  , "ProgressBar" , ID_PROGBAR_0    , 20 ,  35 , 320 ,  20 ,   0 , 0x0 , 0 },
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
/**
  * @brief  Paints ok message button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_okMessage(BUTTON_Handle hObj) {
  // Nothing to do for paint
}

/**
  * @brief  callback for ok message button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_okMessage(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId)
  {
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
* @brief  Paints yes message button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_yesMessage(BUTTON_Handle hObj)
{
  // Nothing to do for paint
}

/**
* @brief  callback for yes message button
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbButton_yesMessage(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId)
  {
    case WM_PAINT:
      _OnPaint_yesMessage(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
* @brief  Paints no message button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_noMessage(BUTTON_Handle hObj)
{
  // Nothing to do for paint
}

/**
* @brief  callback for no message button
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbButton_noMessage(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId)
  {
    case WM_PAINT:
      _OnPaint_noMessage(pMsg->hWin);
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
static void _cbDialogMessage(WM_MESSAGE * pMsg)
{
  int     Id;
  WM_HWIN hItem;

  switch (pMsg->MsgId)
  {
    case WM_INIT_DIALOG:
    {
      WM_SetHasTrans(WM_GetClientWindow(pMsg->hWin));
      WINDOW_SetBkColor(pMsg->hWin, GUI_INVALID_COLOR);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
      IMAGE_SetBitmap(hItem, &bmdialog_box);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
      TEXT_SetBkColor(hItem, GUI_INVALID_COLOR);
      TEXT_SetFont(hItem, GUI_FONT_20_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetWrapMode(hItem, GUI_WRAPMODE_WORD);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
      TEXT_SetBkColor(hItem, GUI_INVALID_COLOR);
      TEXT_SetFont(hItem, &GUI_FontComic18B_ASCII);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetWrapMode(hItem, GUI_WRAPMODE_WORD);
      
      hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_OK);
      WM_SetCallback(hItem, _cbButton_okMessage);
      
      WM_HideWindow(hWinInfo);
    }
    break;

    case WM_NOTIFY_PARENT:
      if (pMsg->Data.v == WM_NOTIFICATION_RELEASED)
      {
        Id = WM_GetId(pMsg->hWinSrc);
        if (Id == GUI_ID_OK)
        {
          GUI_EndDialog(pMsg->hWin, 1);
        }
      }
      break;

    case WM_DELETE :
      WM_ShowWindow(hWinInfo);
      break;

    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/**
* @brief callback Message Box
* @param  pMsg : pointer to data structure
* @retval None
*/
static void _cbMessageBox(WM_MESSAGE* pMsg)
{
  WM_HWIN hWin, hItem;
  int Id;

  hWin = pMsg->hWin;
  switch (pMsg->MsgId)
  {
    case WM_INIT_DIALOG:
      WM_SetHasTrans(WM_GetClientWindow(pMsg->hWin));
      WINDOW_SetBkColor(pMsg->hWin, GUI_INVALID_COLOR);

      hItem = WM_GetDialogItem(hWin, ID_IMAGE_0);
      IMAGE_SetBitmap(hItem, &bmmessage_box);

      hItem = WM_GetDialogItem(hWin, ID_TEXT_0);
      TEXT_SetBkColor(hItem, GUI_INVALID_COLOR);
      TEXT_SetFont(hItem, GUI_FONT_20_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetWrapMode(hItem, GUI_WRAPMODE_WORD);

      hItem = WM_GetDialogItem(hWin, ID_TEXT_1);
      TEXT_SetBkColor(hItem, GUI_INVALID_COLOR);
      TEXT_SetFont(hItem, &GUI_FontComic18B_ASCII);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetWrapMode(hItem, GUI_WRAPMODE_WORD);

      hItem = WM_GetDialogItem(hWin, GUI_ID_OK);
      WM_SetCallback(hItem, _cbButton_yesMessage);

      hItem = WM_GetDialogItem(hWin, GUI_ID_CANCEL);
      WM_SetCallback(hItem, _cbButton_noMessage);
      
      WM_HideWindow(hWinInfo);
      break;

    case WM_NOTIFY_PARENT:
      if (pMsg->Data.v == WM_NOTIFICATION_RELEASED)
      {
        Id = WM_GetId(pMsg->hWinSrc);
        switch (Id) {
          case GUI_ID_OK:
            GUI_EndDialog(hWin, 1);
            break;
          case GUI_ID_CANCEL:
            GUI_EndDialog(hWin, 0);
            break;
        }
      }
      break;

    case WM_DELETE :
      WM_ShowWindow(hWinInfo);
      break;

    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/**
  * @brief  Callback routine of Progress Bar
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbProgressBarWindow(WM_MESSAGE * pMsg)
{
  WM_HWIN hItem;
 
  switch (pMsg->MsgId)
  {
    case WM_INIT_DIALOG:
    {
      WM_SetHasTrans(WM_GetClientWindow(pMsg->hWin));
      WINDOW_SetBkColor(pMsg->hWin, GUI_INVALID_COLOR);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
      IMAGE_SetBitmap(hItem, &bmpbar_bck);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
      TEXT_SetBkColor(hItem, GUI_INVALID_COLOR);
      TEXT_SetFont(hItem, &GUI_Font16B_ASCII);
      TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(GUI_PROGBAR_LIGHTBLUE));
      TEXT_SetWrapMode(hItem, GUI_WRAPMODE_WORD);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0);
      PROGBAR_SetBarColor(hItem, 0, GUI_GREEN);
      PROGBAR_SetBarColor(hItem, 1, GUI_RED);
      PROGBAR_SetFont(hItem, &GUI_Font16B_ASCII);
      PROGBAR_SetTextColor(hItem, 0, GUI_MAKE_COLOR(GUI_PROGBAR_DARKBLUE));
      PROGBAR_SetTextColor(hItem, 1, GUI_MAKE_COLOR(GUI_PROGBAR_DARKBLUE));
      PROGBAR_SetValue(hItem, 0);
    }
    break;

    default:
      WM_DefaultProc(pMsg);
    break;
  }
}

/**
* @brief  Show Message Box
* @param  pTitle: pointer to the title
* @param  pText: pointer to the text
* @param  YesNo player response
* @retval int
*/
static int _ShowMessageBox(WM_HWIN hWin, const char* pTitle, const char* pText)
{
  WM_HWIN hFrame, hItem;

  /* Create and display the Dialog Box */
  hFrame = GUI_CreateDialogBox(_aMessageCreate,
                              GUI_COUNTOF(_aMessageCreate),
                              _cbMessageBox, // No callback - no initial WM_INIT_DIALOG event being treated.
                              hWin,
                              60, 15);

  hItem = WM_GetDialogItem(hFrame, ID_TEXT_0);
  TEXT_SetText(hItem, pTitle);

  hItem = WM_GetDialogItem(hFrame, ID_TEXT_1);
  TEXT_SetText(hItem, pText);

  WM_MakeModal(hFrame);

  return GUI_ExecCreatedDialog(hFrame);
}

/**
* @brief  Show Message Box
* @param  hWin: pointer to the parent window
* @param  pMsgItem: pointer to the message item data
* @param  text_id: text ID used to get then set the Dialog Box user data
* @retval None
*/
static int _ShowDialogBox(WM_HWIN hWin, const char* pTitle, const char* pText)
{
  WM_HWIN hFrame, hItem;

  /* Create and display the Dialog Box */
  hFrame = GUI_CreateDialogBox(_aDialogMessageCreate,
                              GUI_COUNTOF(_aDialogMessageCreate),
                              _cbDialogMessage, // No callback - no initial WM_INIT_DIALOG event being treated.
                              hWin,
                              60, 15);

  hItem = WM_GetDialogItem(hFrame, ID_TEXT_0);
  TEXT_SetText(hItem, pTitle);

  hItem = WM_GetDialogItem(hFrame, ID_TEXT_1);
  TEXT_SetText(hItem, pText);

  /* Show the Dialog Box */
  WM_MakeModal(hFrame);

  return GUI_ExecCreatedDialog(hFrame);
}

/**
* @brief  Show Message Box
* @param  hWin: pointer to the parent window
* @param  pMsgItem: pointer to the message item data
* @param  text_id: text ID used to get then set the Dialog Box user data
* @retval None
*/
static WM_HWIN _ShowProgressBar(WM_HWIN hWin, const char* pTitle, int Min, int Max)
{
  WM_HWIN hFrame, hItem;

  /* Create and display the Dialog Box */
  hFrame = GUI_CreateDialogBox(_aProgressBarCreate,
          GUI_COUNTOF(_aProgressBarCreate),
          _cbProgressBarWindow, // Default callback.
          hWin,
          60, 100);

  /* Set the text data */
  hItem = WM_GetDialogItem(hFrame, ID_TEXT_0);
  TEXT_SetText(hItem, pTitle);

  /* Set Progress Bar Min - Max values */
  hItem = WM_GetDialogItem(hFrame, ID_PROGBAR_0);
  PROGBAR_SetMinMax(hItem, Min, Max);

  WM_MakeModal(hFrame);
  
  WM_Exec();
  return hFrame;
}

#ifdef WIN32
/**
  * @brief  SUBDEMO_Sim
  * @param  None
  * @retval None
  */
static void SUBDEMO_Sim(WM_HWIN hWin, const DemoContext_t *pContext)
{

  WM_HWIN hFrame;
  PROGBAR_Handle hProg;
  TEXT_Handle hText;
  int Value = 0;
  char message[256] = { 0 };

  if (test_id == 5)
  {
    test_id = 0;
  }

  if (test_id == 0)
  {
    /* Calculate estimated programming duration */
    uint32_t file_size = 20 * 1024 * 1024; /* 20 MBytes --> 320 * 64 Kbytes sectors */
    uint32_t duration_ms = (file_size/(1024 * 64)) *  OCTOSPI_PROG_SECTOR_DURATION_MS; /* TODO : use appropriate value */

    snprintf(message, sizeof(message), "%s demo resources download\n\n* Expected duration : %ld minutes *\nCarry on?", pContext->demo_name, (duration_ms/1000));
    if(!_ShowMessageBox(hWin, "QUESTION", message))
      return;
  }

  if (test_id == 0)
  {
    /* Display error message: No SD Card inserted! */
    snprintf(message, sizeof(message), "No SD Card inserted!\nPlease insert an SD Card which includes valid binary files.");
    _ShowDialogBox(hWin, "FAILED", message);

    test_id++;
  }
  else if (test_id == 1)
  {
    /* Display error message: Invalid Firmware binary file */
    snprintf(message, sizeof(message), "Invalid firmware binary file\nfor %s demo", pContext->demo_name);
    _ShowDialogBox(hWin, "FAILED", message);

    test_id++;
  }
  else if (test_id == 2)
  {
    /* Display error message: Invalid Demo Resources file */
    snprintf(message, sizeof(message), "Invalid resources file\nfor %s demo", pContext->demo_name);
    _ShowDialogBox(hWin, "FAILED", message);

    test_id++;
  }
  else if (test_id == 3)
  {
    /* Display error message: Missing Resource binary file */
    snprintf(message, sizeof(message), "Missing resources binary file\nfor %s demo", pContext->demo_name);
    _ShowDialogBox(hWin, "FAILED", message);

    test_id++;
  }
  else if (test_id == 4)
  {
    /* Simulate Firmware binary file programming */
    snprintf(message, sizeof(message), "%s Firmware Programming...", pContext->demo_name);

    /* Create and display the Dialog Box */
    hFrame = _ShowProgressBar(hWin, message, 0, 512);
    hProg = WM_GetDialogItem(hFrame, ID_PROGBAR_0);

    Value = 0;
    while (1)
    {
      GUI_X_Delay(1);
      int val = PROGBAR_GetValue(hProg);
      if (val == 512)
        break;
      Value++;
      PROGBAR_SetValue(hProg, Value);
      GUI_Exec1();
    }

    WM_DeleteWindow(hFrame);

    /* Simulate Resources binary file programming */
    snprintf(message, sizeof(message), "%s Resources Programming...", pContext->demo_name);

    /* Create and display the Dialog Box */
    hFrame = _ShowProgressBar(hWin, message, 0, 128);
    hProg = WM_GetDialogItem(hFrame, ID_PROGBAR_0);

    Value = 0;
    while (1)
    {
      GUI_X_Delay(OCTOSPI_PROG_SECTOR_DURATION_MS);
      int val = PROGBAR_GetValue(hProg);
      if (val == 128)
              break;
      Value++;
      PROGBAR_SetValue(hProg, Value);
      GUI_Exec1();
    }

    WM_DeleteWindow(hFrame);
    GUI_Delay(10);
    
    /* Display error message: Invalid Demo Resources file */
    snprintf(message, sizeof(message), "Firmware and Resources programming successfully done\nfor %s demo", pContext->demo_name);
    _ShowDialogBox(hWin, "RESTART NOW", message);

    test_id++;
  }
}
#endif /* WIN32 */

#ifndef WIN32
/**
  * @brief  SUBDEMO_Start
  * @param  None
  * @retval None
  */
static void SUBDEMO_Start(WM_HWIN hWin, const DemoContext_t *pContext)
{
  WM_HWIN hFrame;
  Demo_Header_t * pOspiDemoHeader = (Demo_Header_t *)(pContext->resources.address + pContext->resources.offset);
  Demo_Header_t * pFirmDemoHeader = (Demo_Header_t *)(pContext->firmware.address + pContext->firmware.offset);
  Demo_Header_t   SdDemoHeader    = (Demo_Header_t  ){0};
  FRESULT result = FR_NOT_READY;
  uint32_t numOfReadBytes;
  int card_detected = BSP_SD_IsDetected();
  uint32_t prog_done = 0;

  FIL DEMO_firmware_file = { 0 };
  FIL DEMO_resources_file = { 0 };

  PROGBAR_Handle hProg;
  char message[256] = {0};

  if ( (pFirmDemoHeader->demo_id == pContext->firmware.tag)
    && (pOspiDemoHeader->demo_id == pContext->resources.tag) )
  {
    result = FR_OK;
  }
  else if(card_detected)
  {
    /* Check if Flash binary is not the same as requested */
    if (pFirmDemoHeader->demo_id == pContext->firmware.tag)
    {
      result = FR_OK;
    }
    else
    {
      /* Open Flash file */
      result = f_open(&DEMO_firmware_file, pContext->firmware.filename, FA_READ);
      if(result == FR_OK)
      {
        /* Read Flash Header */
        result = f_lseek(&DEMO_firmware_file, pContext->firmware.offset);
        if (result == FR_OK)
        {
          result = f_read(&DEMO_firmware_file, &SdDemoHeader, sizeof(Demo_Header_t), (void *)&numOfReadBytes);
          if(result == FR_OK)
          {
            /* Check if the demo located in SD Card is matching the required demo */
            if ( (GET_SUB_DEMO_ID(SdDemoHeader.demo_id) != GET_SUB_DEMO_ID(SET_SUB_DEMO_TAG(pContext->firmware.tag)))
              || (GET_SUB_DEMO_VER(SdDemoHeader.demo_id) < GET_SUB_DEMO_VER(SET_SUB_DEMO_TAG(pContext->firmware.tag))) )
            {
              result = FR_INVALID_OBJECT;
            }
          }
        }

        if(result != FR_OK)
        {
          /* Display error message: Invalid Demo binary files */
          snprintf(message, sizeof(message), "Invalid Firmware binary file\nfor %s demo", pContext->demo_name);
          _ShowDialogBox(hWin, "FAILED", message);

          /* Close flash file as no more needed */
          f_close(&DEMO_firmware_file);
          f_size(&DEMO_firmware_file) = 0;
        }
      }
      else
      {
        /* Display error message: No Demo binary files */
        snprintf(message, sizeof(message), "No Firmware binary file\nfor %s demo", pContext->demo_name);
        _ShowDialogBox(hWin, "FAILED", message);
      }
    }

    /* Check if the OctoSPI binary is not the same as requested */
    if ( (result == FR_OK) && (pOspiDemoHeader->demo_id == pContext->resources.tag) )
    {
      result = FR_OK;
    }
    else if (result == FR_OK)
    {
      /* Open the OctoSPI file */
      result = f_open(&DEMO_resources_file, pContext->resources.filename, FA_READ);
      if(result == FR_OK)
      {
        result = f_lseek(&DEMO_resources_file, pContext->resources.offset);
        if (result == FR_OK)
        {
          result = f_read(&DEMO_resources_file, &SdDemoHeader, sizeof(Demo_Header_t), (void *)&numOfReadBytes);
          if(result == FR_OK)
          {
            /* Check if the demo resources located in SD Card is matching the required demo */
            if ( (GET_SUB_DEMO_ID(SdDemoHeader.demo_id) != GET_SUB_DEMO_ID(pContext->resources.tag))
              || (GET_SUB_DEMO_VER(SdDemoHeader.demo_id) < GET_SUB_DEMO_VER(pContext->resources.tag)) )
            {
              result = FR_INVALID_OBJECT;
            }
          }
        }

        if(result != FR_OK)
        {
          /* Display error message: Invalid Demo Resources file */
          snprintf(message, sizeof(message), "Invalid Resources file\nfor %s demo", pContext->demo_name);
          _ShowDialogBox(hWin, "FAILED", message);

          /* Close flash file as no more needed */
          f_close(&DEMO_resources_file);
          f_size(&DEMO_resources_file) = 0;
        }
        else
        {
          /* Calculate estimated programming duration */
          uint32_t file_size = f_size(&DEMO_resources_file);
          uint32_t duration_ms = (file_size / (1024 * 64)) *  OCTOSPI_PROG_SECTOR_DURATION_MS; /* TODO : use appropriate value */

          /* Notify the user that the expected duration */
          snprintf(message, sizeof(message), "%s demo resources download\n\n* Expected duration : %ld minutes *\nCarry on?", pContext->demo_name, (duration_ms/1000));
          if(!_ShowMessageBox(hWin, "QUESTION", message))
          {
            /* Close flash file before returning */
            f_close(&DEMO_firmware_file);
            f_size(&DEMO_firmware_file) = 0;

            /* Close resources file before returning */
            f_close(&DEMO_resources_file);
            f_size(&DEMO_resources_file) = 0;

            return;
          }
        }
      }
      else
      {
        /* Display error message: Invalid Demo binary files */
        snprintf(message, sizeof(message), "Invalid Resources binary file\nfor %s demo", pContext->demo_name);
        _ShowDialogBox(hWin, "FAILED", message);

        /* Don't update the internal flash. Close flash file before going on */
        f_close(&DEMO_firmware_file);
        f_size(&DEMO_firmware_file) = 0;
      }
    }

    /*
     * Now Flash binaries
     */
    if(f_size(&DEMO_firmware_file) > 0)
    {
      snprintf(message, sizeof(message), "%s Firmware Programming...", pContext->demo_name);

      /* Create and display the Dialog Box */
      hFrame = _ShowProgressBar(hWin, message, 0, f_size(&DEMO_firmware_file));
      hProg = WM_GetDialogItem(hFrame, ID_PROGBAR_0);

      BSP_FlashProgram(hProg, &DEMO_firmware_file, pContext->firmware.address);

      WM_DeleteWindow(hFrame);

      f_close(&DEMO_firmware_file);
      f_size(&DEMO_firmware_file) = 0;

      prog_done = 1;
    }
    
    if(f_size(&DEMO_resources_file) > 0)
    {
      snprintf(message, sizeof(message), "%s Resources Programming...", pContext->demo_name);

      /* Create and display the Dialog Box */
      hFrame = _ShowProgressBar(hWin, message, 0, f_size(&DEMO_resources_file));
      hProg = WM_GetDialogItem(hFrame, ID_PROGBAR_0);

      BSP_ResourcesCopy(hProg, &DEMO_resources_file, pContext->resources.address);

      WM_DeleteWindow(hFrame);

      f_close(&DEMO_resources_file);
      f_size(&DEMO_resources_file) = 0;

      prog_done = 1;
    }
    
    if( prog_done )
    {
      /* Display error message: Invalid Demo Resources file */
      snprintf(message, sizeof(message), "Firmware and Resources programming successfully done\nfor %s demo", pContext->demo_name);
      _ShowDialogBox(hWin, "RESTART NOW", message);
    }
  }
  else
  {
    /* Display error message: No SD Card inserted! */
    snprintf(message, sizeof(message), "No SD Card inserted!\nPlease insert an SD Card which includes valid binary files.");
    _ShowDialogBox(hWin, "FAILED", message);
  }

  /* Jump to new Sub Demo if no errors */
  if(result == FR_OK)
  {   
    /* Now Jump to the programmed demo */
    BSP_JumpToSubDemo(pContext->firmware.address);
  }
}
#endif /* WIN32 */
// USER END

/**
  * @brief  Paints buttons
  * @param  hButton: button handle
  * @retval None
  */
static void _OnPaint(BUTTON_Handle hButton) {

  int Id;

  GUI_SetBkColor(GUI_TRANSPARENT);

  BUTTON_GetUserData(hButton, &Id, sizeof(Id));
  switch (Id)
  {
    case ID_BUTTON_0 :
      GUI_DrawBitmap(&bmSTemWin, 0, 0);
      break;
    case ID_BUTTON_1 :
      GUI_DrawBitmap(&bmTouchGFX, 0, 0);
      break;
    case ID_BUTTON_2 :
      GUI_DrawBitmap(&bmEmbeddedWizard, 0, 0);
      break;
    default:
      break;
  }
}

/**
  * @brief  callback for buttons
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

static void _cbDisableWindow(WM_HWIN hWin, void *p)
{
  int disable = *(int *)p;
  if(disable)
  {
    WM_DisableWindow(hWin);
  }
  else
  {
    WM_EnableWindow(hWin);
  }
}

/**
* @brief  Callback routine of the Info dialog
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbInfoDialog(WM_MESSAGE * pMsg) {
  static int disable = 1;
  int step = 0;

	const GUI_PID_STATE * pState;

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
    WINDOW_SetBkColor(pMsg->hWin, GUI_INVALID_COLOR);
    
    // hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
    // IMAGE_SetBitmap(hItem, &bminfo_screen);

		// GUI_SetFont(GUI_FONT_8X18);
		// GUI_SetColor(GUI_WHITE);
    // GUI_SetTextMode(GUI_TM_TRANS);
		break;

	case WM_PAINT:
    GUI_DrawBitmap(&bminfo_screen, 0, 0);

		GUI_SetFont(GUI_FONT_8X18);
		GUI_SetColor(GUI_WHITE);
    GUI_SetTextMode(GUI_TM_TRANS);

    GUI_DispStringAt("STM32L4R9I", 130, 60);
    GUI_DispStringAt("STM32L4R9I-EVAL", 130, 110);
    GUI_DispStringAt("120 MHz", 130, 158);
    GUI_DispStringAt("V1.0.0", 130, 208);

		break;

	case  WM_TOUCH:
		pState = (const GUI_PID_STATE *)pMsg->Data.p;
		if ((pState) && (pState->Pressed == 0))
		{
      int s = 0;

      WM_ForEachDesc(WM_HBKWIN, _cbDisableWindow, (void *)&disable);
      WM_DisableWindow(pMsg->hWin);
      for (step = 0; step < 11; step++)
      {
        if (disable == 1)
        {
          s = (WM_GetWindowOrgY(pMsg->hWin) - 20);
        }
        else
        {
          s = (WM_GetWindowOrgY(pMsg->hWin) + 20);
        }

        WM_MoveTo(pMsg->hWin, WM_GetWindowOrgX(pMsg->hWin), s);
        GUI_Exec1();
      }
      WM_EnableWindow(pMsg->hWin);

      disable ^= 1;
		}
		break;

	case WM_DELETE:
		break;

  default:
		WM_DefaultProc(pMsg);
		break;
	}
}

/*********************************************************************
*
*       _cbMainDialog
*/
static void _cbMainDialog(WM_MESSAGE * pMsg) {
  WM_HWIN      hItem;
  int          NCode;
  int          Id;
  int          Demo_Id = -1;

  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'launcher'
    //
    //GUI_SetBkColor(GUI_TRANSPARENT);
	  

    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
    IMAGE_SetBitmap(hItem, &bmbackground);

    //
    // Create all buttons
    //
    Id = ID_BUTTON_0;
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetUserData(hItem, &Id, sizeof(Id));
    WM_SetCallback(hItem, _cbButton);

    Id = ID_BUTTON_1;
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetUserData(hItem, &Id, sizeof(Id));
    WM_SetCallback(hItem, _cbButton);

    Id = ID_BUTTON_2;
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetUserData(hItem, &Id, sizeof(Id));
    WM_SetCallback(hItem, _cbButton);

    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;

  case WM_NOTIFY_PARENT:
    Demo_Id = -1;
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    if(NCode == WM_NOTIFICATION_RELEASED)
    {
      switch(Id) {
        case ID_BUTTON_0: // Notifications sent by 'STemWin'
          // USER START (Optionally insert code for reacting on notification message)
          Demo_Id = 0;
          // USER END
          break;
        case ID_BUTTON_1: // Notifications sent by 'TouchGFX'
          // USER START (Optionally insert code for reacting on notification message)
          Demo_Id = 1;
          // USER END
          break;
        case ID_BUTTON_2: // Notifications sent by 'TARA'
          // USER START (Optionally insert code for reacting on notification message)
          Demo_Id = 2;
          // USER END
          break;
      // USER START (Optionally insert additional code for further Ids)
      // USER END
      }
    }

    if (Demo_Id >= 0)
    {
#ifdef WIN32
      SUBDEMO_Sim(pMsg->hWin, &SubDemo[Demo_Id]);
#else
      SUBDEMO_Start(pMsg->hWin, &SubDemo[Demo_Id]);
#endif /* WIN32 */
    }

    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       Createlauncher
*/
WM_HWIN Createlauncher(void);
WM_HWIN Createlauncher(void) {
  WM_HWIN hWinBase;     // Parent window for buttons and info window

  hWinBase = GUI_CreateDialogBox(_aMainDialogCreate, GUI_COUNTOF(_aMainDialogCreate), _cbMainDialog, WM_HBKWIN,  0, 0);

  return hWinBase;
}

void LauncherStartUp(void)
{
  int xSize, ySize;

  BUTTON_SetReactOnLevel();

  //
  // Get display dimension
  //
  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize();
  //
  // Limit desktop window to display size
  //
  WM_SetSize(WM_HBKWIN, xSize, ySize);

  //
  // Create windows
  //
  hWinBase = Createlauncher();
  hWinInfo = WM_CreateWindowAsChild(60, 227, 360, 260, hWinBase, (WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_STAYONTOP | WM_CF_MEMDEV), _cbInfoDialog, 0);
  // hWinInfo = GUI_CreateDialogBox(_aInfoDialogCreate, GUI_COUNTOF(_aInfoDialogCreate), _cbInfoDialog, hWinBase, 60, 227);
}

#ifdef WIN32
/*
*       MainTask
*/
void MainTask(void)
{
  //
  // Enable automatic use of memory devices
  //
  //WM_SetCreateFlags(WM_CF_MEMDEV_ON_REDRAW /*| WM_CF_LATE_CLIP | WM_CF_CONST_OUTLINE*/);

  /* Initialize the GUI */
  GUI_Init();

  WM_MULTIBUF_Enable(1);

  GUI_DispStringAt("Starting...", 0, 0);

  /* Initialize LCD and LEDs */
  GUI_DispStringAt("Initializing lcd...", 0, 12);

  LauncherStartUp();
  GUI_SPY_StartServer();

  //
  // Keep demo alive
  //
  while (1) {
    GUI_Delay(20);
  }
}
#endif /* WIN32 */
// USER START (Optionally insert additional public code)
// USER END

