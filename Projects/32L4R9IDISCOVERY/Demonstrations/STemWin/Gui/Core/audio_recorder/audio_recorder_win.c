/**
  ******************************************************************************
  * @file    audio_recorder_win.c
  * @author  MCD Application Team
  * @brief   audio recorder functions
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
#include "k_rtc.h"
#include "Resources.h"
#include "audio_recorder_res.h"
#include "audio_recorder_app.h"

/** @addtogroup AUDIO_RECORDER_MODULE
  * @{
  */

/** @defgroup AUDIO_RECORDER
  * @brief audio recorder routines
  * @{
  */

/* External variables --------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static void _AddEntireFolder(char *Foldername, int update);
static void FILEMGR_GetFileOnly (char *file, char *path);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  audio_recorder =
{
  2,
  "audio recorder",
  0,
  Startup,
  NULL,
};

/* Private defines -----------------------------------------------------------*/
#define   ID_BUTTON_BACK           (GUI_ID_USER + 0x01)
#define   ID_BUTTON_MENU           (GUI_ID_USER + 0x02)
#define   ID_BUTTON_MICRO          (GUI_ID_USER + 0x03)
#define   ID_TEXT_TITLE            (GUI_ID_USER + 0x04)
#define   ID_TEXT_TITLE_REC        (GUI_ID_USER + 0x05)
#define   ID_BUTTON_BACKLIST       (GUI_ID_USER + 0x06)
#define   ID_BUTTON_RECORD_PAUSE   (GUI_ID_USER + 0x07)
#define   ID_BUTTON_SAVE           (GUI_ID_USER + 0x08)
#define   ID_BUTTON_CANCEL         (GUI_ID_USER + 0x09)
#define   ID_TEXT_ELAPSED          (GUI_ID_USER + 0x0A)
#define   ID_TEXT_SAVE             (GUI_ID_USER + 0x0B)
#define   ID_TEXT_RECORD           (GUI_ID_USER + 0x0C)
#define   ID_TEXT_CANCEL           (GUI_ID_USER + 0x0D)
#define   ID_RECORD_LIST           (GUI_ID_USER + 0x0E)
#define   ID_PROCESS_TIMER         (GUI_ID_USER + 0x0F)
#define   ID_TEXT_TITLE1           (GUI_ID_USER + 0x10)
#define   ID_TEXT_RECORDED_FILES   (GUI_ID_USER + 0x11)

#define RECORDER_MODE_REC_DISABLED            0x00  
#define RECORDER_MODE_REC_IDLE                0x01  
#define RECORDER_MODE_RECORDING               0x02 
#define RECORDER_MODE_REC_PAUSE               0x03

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static GUI_CONST_STORAGE GUI_BITMAP *AnimationCircles[] = 
{
  &bmrecording_cercle1,
  &bmrecording_cercle2,
  &bmrecording_cercle3,
  &bmrecording_cercle4,
};

WM_HWIN                      hAudioRecorderWin;
WM_HWIN                      hAudioRecorder;
WM_HWIN                      hList;

RNG_HandleTypeDef            hRNG;
static WM_HTIMER             hTimerTime;

static uint8_t                 RecorderMode; 
static char                    FileName[256]; 
static  FILELIST_FileTypeDef   WavList;

uint32_t duration         = 0;
int    MicroButtonPressed = 0;
int    animationNum       = 0;
int    graph_index        = 0;
int    Drawback           = 0;
int    movebutton         = 0;
int    direction          = 0; 
int    Audio_init         = 0;
int    animation_done     = 1;
static WAV_InfoTypedef         WavInfo;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Build random file name.
  * @param  None.
  * @retval None.
  */
static void _BuildFileName(void) {

  hRNG.Instance = RNG;
  __HAL_RCC_RNG_CLK_ENABLE();
  HAL_RNG_Init(&hRNG);
  sprintf(FileName, "0:/Record/Record_%04lu.wav", HAL_RNG_GetRandomNumber(&hRNG) % 10000);
}

/**
  * @brief  Paints back button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_ButtonBack(BUTTON_Handle hObj)
{
  if (GetMotionWindowVisibility() == 0)
  {
    GUI_DrawBitmap(&bmback_button_bkg, 0, 17);
    GUI_DrawBitmap(&bmbutton_back, 63, 25);
  }
}

/**
  * @brief  callback for back button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButtonBack(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_ButtonBack(pMsg->hWin);
      break;

    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints menu button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_ButtonMenu(BUTTON_Handle hObj)
{
  if (GetMotionWindowVisibility() == 0)
  {  
    GUI_DrawBitmap(&bmmenu_button, 20, 20);
  }
}

/**
  * @brief  callback for menu button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButtonMenu(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_ButtonMenu(pMsg->hWin);
      break;

    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints record button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_ButtonRecord(BUTTON_Handle hObj)
{    
  if(RecorderMode == RECORDER_MODE_REC_PAUSE)   
  {
    GUI_DrawBitmap(&bmrecord_button_unselected, 0, 0);
  }
  else 
  {
    GUI_DrawBitmap(&bmpause_button_unselected, 0, 0);  
  }  
}

/**
  * @brief  callback for record button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButtonRecord(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_ButtonRecord(pMsg->hWin);
      break;

    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Callback function of the media connection status
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbMediaConnection(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  static WM_HTIMER      hStatusTimer;  
  static uint8_t        prev_msd_status = 0;
  
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    prev_msd_status = k_StorageGetStatus(MSD_DISK_UNIT);    
    hStatusTimer = WM_CreateTimer(pMsg->hWin, 0, 500, 0); 
    
    break;
    
  case WM_TIMER:
    if(prev_msd_status != k_StorageGetStatus(MSD_DISK_UNIT))
    {
      prev_msd_status = k_StorageGetStatus(MSD_DISK_UNIT);
      if(prev_msd_status == 0)
      {             
          if((RecorderMode == RECORDER_MODE_RECORDING) || (RecorderMode == RECORDER_MODE_REC_PAUSE))
          {
            AUDIO_RECORDER_StopRec();
            AUDIO_RECORDER_RemoveAudioFile(FileName);
          }
          
          AUDIO_RECORDER_DeInit();
          
          RecorderMode = RECORDER_MODE_REC_IDLE;
          
          WM_DeleteTimer(hTimerTime);
          
          MicroButtonPressed = 0;
          animationNum = 0;
          graph_index = 0;
          Drawback = 0;
          movebutton = 0;
          direction = 0;
          Audio_init = 0;
          hTimerTime = 0;
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MICRO);
          WM_ShowWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MENU);
          WM_ShowWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RECORD_PAUSE);
          WM_HideWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SAVE);
          WM_HideWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CANCEL);
          WM_HideWindow(hItem);   
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ELAPSED);
          WM_HideWindow(hItem);

          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SAVE);
          WM_HideWindow(hItem);

          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RECORD);
          WM_HideWindow(hItem);          
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CANCEL);
          WM_HideWindow(hItem);          
       
          WM_DeleteWindow(hList);
          
          BackToMainScreen();
      }
      else
      {
        AUDIO_RECORDER_Init(50);
        Audio_init = 1;
        RecorderMode = RECORDER_MODE_REC_IDLE;     
        
        hItem = WM_GetDialogItem(hAudioRecorder, ID_BUTTON_RECORD_PAUSE);
        WM_EnableWindow(hItem);
        WM_Update(hItem);  
        
        hItem = WM_GetDialogItem(hAudioRecorder, ID_TEXT_TITLE);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);  
        hItem = WM_GetDialogItem(hAudioRecorder, ID_TEXT_TITLE1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);          
      }
    }
    WM_RestartTimer(pMsg->Data.v, 500);
    break;
    
  case WM_DELETE:
    if(hStatusTimer != 0)
    {
      WM_DeleteTimer(hStatusTimer);
      hStatusTimer = 0;
    }
    break;   
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  callback for audio process
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbAudioProcess(WM_MESSAGE * pMsg) {

  uint32_t Id;
  static WM_HTIMER hProcessTimer;
  WM_HWIN hItem;
  char tmp[] = "00:00/00:00";  
  
  switch (pMsg->MsgId) 
  {

  case WM_CREATE:
    hProcessTimer = WM_CreateTimer(pMsg->hWin, ID_PROCESS_TIMER, 1000, 0);

    break;
    
  case WM_TIMER:
    
    Id = WM_GetTimerId(pMsg->Data.v);
   
    if(Id == ID_PROCESS_TIMER)
    {
      if( RecorderMode == RECORDER_MODE_RECORDING)
      {
        /*Set elapsed time */
        duration = AUDIO_RECORDER_GetElapsedTime();     
        sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
        
        hItem = WM_GetDialogItem(hAudioRecorder, ID_TEXT_ELAPSED);
        TEXT_SetText(hItem, tmp);  
        
      }      
      WM_RestartTimer(pMsg->Data.v, 1000);
    }
    break; 
    
  case WM_DELETE:
    WM_DeleteTimer(hProcessTimer);
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Notify State Change
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
void _cbNotifyStateChange (void) 
{
  WM_HWIN hItem;  
  if(AUDIO_RECORDER_GetState() == AUDIO_RECORDER_SUSPENDED)
  {    
    if(RecorderMode == RECORDER_MODE_RECORDING)
    {
      RecorderMode = RECORDER_MODE_REC_IDLE;
    }
        
    if(hAudioRecorder != 0)
    {
      hItem = WM_GetDialogItem(hAudioRecorder, ID_TEXT_ELAPSED);        
      TEXT_SetText(hItem, "00:00"); 
      WM_Update(hItem);
      
    }
  } 
}

/**
  * @brief  Paints save button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_ButtonSave(BUTTON_Handle hObj)
{
  if((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {  
    GUI_DrawBitmap(&bmsave_button_selected, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmsave_button_unselected, 0, 0);
  }
}

/**
  * @brief  callback for save button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButtonSave(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_ButtonSave(pMsg->hWin);
      break;

    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints cancel button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_ButtonCancel(BUTTON_Handle hObj)
{
  if((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {   
    GUI_DrawBitmap(&bmcancel_button_selected, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmcancel_button_unselected, 0, 0);
  }
}

/**
  * @brief  callback for cancel button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButtonCancel(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_ButtonCancel(pMsg->hWin);
      break;

    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints micro button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_ButtonMicro(BUTTON_Handle hObj)
{
    GUI_DrawBitmap(&bmmicro_icon, 20, 20);
}

/**
  * @brief  callback for micro button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButtonMicro(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_ButtonMicro(pMsg->hWin);
      break;

    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for record swipeList
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static int _CreateRecordSwipeList(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {

  switch (pDrawItemInfo->Cmd) 
  {
    case WIDGET_ITEM_GET_XSIZE:
      return SWIPELIST_OwnerDraw(pDrawItemInfo);
    case WIDGET_ITEM_GET_YSIZE:
      return SWIPELIST_OwnerDraw(pDrawItemInfo);
    case WIDGET_ITEM_DRAW_BACKGROUND:
      break;
    case WIDGET_ITEM_DRAW_TEXT:
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetTextAlign(GUI_TA_VCENTER | GUI_TA_HCENTER);
      return SWIPELIST_OwnerDraw(pDrawItemInfo);
  default:
    return SWIPELIST_OwnerDraw(pDrawItemInfo);
  }
  return 0;
}

/**
  * @brief  Paint for swipeList widget
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbSwipe(WM_MESSAGE * pMsg) 
{  
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      GUI_EnableAlpha(1);
      SWIPELIST_Callback(pMsg);
      break;

    default:
      SWIPELIST_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Callback routine of recording list window
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbRecList(WM_MESSAGE * pMsg) 
{
  int Id, NCode;
  BUTTON_Handle hButton;
  WM_HWIN hItem;
  SWIPELIST_Handle hSwipelist;
  int i = 0;
  static char tmp[FILEMGR_FILE_NAME_SIZE];
  char time_record[] = "00:00"; /* 'xx:xx format" */
  uint32_t duration_sec = 0;
  
  switch (pMsg->MsgId) 
  {
    case WM_CREATE:
    
    /* Create buttond to be used */
    hButton = BUTTON_CreateEx(90, 335, 210, 55, pMsg->hWin, WM_CF_SHOW | WM_CF_STAYONTOP, 0 , ID_BUTTON_BACKLIST);
    WM_SetCallback(hButton, _cbButtonBack);
    
    hItem = TEXT_CreateEx(100, 50, 200, 50, pMsg->hWin, WM_CF_SHOW,  0, ID_TEXT_RECORDED_FILES, "Recorded files");
    TEXT_SetFont(hItem, &GUI_FontRoboto_C_Regular35);
    TEXT_SetTextColor(hItem, GUI_WHITE);  
    
    /* Create SwipeList */
    hSwipelist = SWIPELIST_CreateEx(42, 95, 306, 210, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, 0, ID_RECORD_LIST);
    WM_SetCallback(hSwipelist, _cbSwipe);
    
    SWIPELIST_SetOwnerDraw(hSwipelist, _CreateRecordSwipeList);     
    
    SWIPELIST_SetFont(hSwipelist, SWIPELIST_FI_SEP_ITEM, &GUI_FontRobotoC_regular22);
    SWIPELIST_SetFont(hSwipelist, SWIPELIST_FI_ITEM_HEADER, &GUI_FontRobotoC_regular22);
    SWIPELIST_SetFont(hSwipelist, SWIPELIST_FI_ITEM_TEXT, &GUI_FontRobotoC_regular22);
    SWIPELIST_AddSepItem(hSwipelist, " ", 40);
    SWIPELIST_SetBitmapSpace(hSwipelist, 20);
    
    _AddEntireFolder("0:/Record", 0);
      
    for (i = 1; i < (WavList.ptr + 1); i++) 
    {      
      AUDIO_RECORDER_GetFileInfo((char *)WavList.file[i-1].name, &WavInfo);
      
      duration_sec = (WavInfo.FileSize - 44) / (WavInfo.ByteRate);
      sprintf(time_record, "%02hhu:%02hhu", (char)(duration_sec / 60),(char)(duration_sec % 60));
          
      FILEMGR_GetFileOnly (tmp, (char *)WavList.file[i-1].name);
      SWIPELIST_AddItem(hSwipelist, tmp, 50);
      SWIPELIST_AddItemText(hSwipelist, i, time_record);
      SWIPELIST_SetBitmap(hSwipelist, i, SWIPELIST_BA_LEFT , &bmrecord_info);
    }
    
    break;

    case WM_PAINT:
      GUI_DrawBitmap(&bmbackground, 0, 0);
     break;  
      
    case WM_NOTIFY_PARENT:
      Id = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;          
      switch(Id)
      {
        case ID_BUTTON_BACKLIST:
        switch (NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          WavList.ptr = 0;
          GUI_EnableAlpha(0);
          WM_DeleteWindow(pMsg->hWin);            

          break;
        }
        break;
      }
      break;  
  }
}

/**
  * @brief  callback for recording drawing
  * @param  pMsg: Pointer to Date structure
  * @retval None
  */
static void _cbAudioRecorder(WM_MESSAGE * pMsg)
{
  int Id, NCode;
  WM_HWIN hItem;
  static int IsMoving = 1;
  
  BUTTON_Handle hButton;

  switch (pMsg->MsgId)
  {
    case WM_DELETE:
      WM_DeleteTimer(hTimerTime);
      MicroButtonPressed = 0;
      animationNum = 0;
      graph_index = 0;   
      movebutton = 0;
      direction = 0;
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MICRO);
      WM_ShowWindow(hItem);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MENU);
      WM_ShowWindow(hItem);
          
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RECORD_PAUSE);
      WM_SetWindowPos(hItem, 155, 154, 80, 81);
      WM_HideWindow(hItem);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SAVE);
      WM_SetWindowPos(hItem, 155, 154, 80, 81);
      WM_HideWindow(hItem);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CANCEL);
      WM_SetWindowPos(hItem, 155, 154, 80, 81);
      WM_HideWindow(hItem);       
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ELAPSED);
      WM_HideWindow(hItem);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SAVE);
      WM_HideWindow(hItem);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RECORD);
      WM_HideWindow(hItem);          
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CANCEL);
      WM_HideWindow(hItem); 
          
      break;

    case WM_CREATE:
      
      hItem = TEXT_CreateEx(390, 25, 390, 40, pMsg->hWin, WM_CF_SHOW,  0, ID_TEXT_TITLE_REC, "Recording !");
      TEXT_SetFont(hItem, &GUI_FontRobotoC_bold22);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER);

      hItem = TEXT_CreateEx(175, 300, 60, 30, pMsg->hWin, WM_CF_HIDE,  0, ID_TEXT_ELAPSED, "00:00");
      TEXT_SetFont(hItem, &GUI_FontRobotoC_bold22);
      TEXT_SetTextColor(hItem, GUI_WHITE);  

      hItem = TEXT_CreateEx(55, 240, 60, 30, pMsg->hWin, WM_CF_HIDE,  0, ID_TEXT_SAVE, "Save");
      TEXT_SetFont(hItem, &GUI_FontRobotoC_bold22);
      TEXT_SetTextColor(hItem, GUI_WHITE);  

      hItem = TEXT_CreateEx(165, 240, 60, 30, pMsg->hWin, WM_CF_HIDE,  0, ID_TEXT_RECORD, " Pause");
      TEXT_SetFont(hItem, &GUI_FontRobotoC_bold22);
      TEXT_SetTextColor(hItem, GUI_WHITE);  

      hItem = TEXT_CreateEx(285, 240, 60, 30, pMsg->hWin, WM_CF_HIDE,  0, ID_TEXT_CANCEL, "Cancel");
      TEXT_SetFont(hItem, &GUI_FontRobotoC_bold22);
      TEXT_SetTextColor(hItem, GUI_WHITE);  
      
      /* Create buttond to be used */
      hButton = BUTTON_CreateEx(((LCD_GetXSize() - bmmicro_icon.XSize - 40) / 2), ((LCD_GetYSize() - bmmicro_icon.YSize - 40) / 2), (bmmicro_icon.XSize + 40), (bmmicro_icon.YSize + 40), pMsg->hWin, WM_CF_SHOW, 0 , ID_BUTTON_MICRO);
      WM_SetCallback(hButton, _cbButtonMicro);

      hButton = BUTTON_CreateEx(((LCD_GetXSize() - bmrecord_button_unselected.XSize) / 2), ((LCD_GetYSize() - bmrecord_button_unselected.YSize) / 2), 80, 81, pMsg->hWin, WM_CF_HIDE, 0 , ID_BUTTON_RECORD_PAUSE);
      WM_SetCallback(hButton, _cbButtonRecord);

      hButton = BUTTON_CreateEx(((LCD_GetXSize() - bmsave_button_unselected.XSize) / 2), ((LCD_GetYSize() - bmsave_button_unselected.YSize) / 2), 80, 81, pMsg->hWin, WM_CF_HIDE, 0 , ID_BUTTON_SAVE);
      WM_SetCallback(hButton, _cbButtonSave);

      hButton = BUTTON_CreateEx(((LCD_GetXSize() - bmcancel_button_unselected.XSize) / 2), ((LCD_GetYSize() - bmcancel_button_unselected.YSize) / 2), 80, 81, pMsg->hWin, WM_CF_HIDE, 0 , ID_BUTTON_CANCEL);
      WM_SetCallback(hButton, _cbButtonCancel);
      
      hButton = BUTTON_CreateEx(300, ((LCD_GetXSize() - bmmenu_button.XSize - 40) / 2), (bmmenu_button.YSize + 70), (bmmenu_button.YSize + 40), pMsg->hWin, (WM_CF_SHOW), 0 , ID_BUTTON_MENU);
      WM_SetCallback(hButton, _cbButtonMenu);
      
      MicroButtonPressed = 0;
      animationNum = 0;
      graph_index = 0;
        
      RecorderMode = RECORDER_MODE_REC_IDLE;
      WM_CreateWindowAsChild(0,
                             100,
                             10,
                             10,
                             pMsg->hWin, 
                             WM_CF_SHOW | WM_CF_HASTRANS,
                             _cbAudioProcess, 
                             0);
      
      WM_CreateWindowAsChild(239, 239, 1, 1, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbMediaConnection , 0);
      
      hItem = TEXT_CreateEx(0, 25, 390, 40, pMsg->hWin, WM_CF_HIDE, 0, ID_TEXT_TITLE, "Record Now !");
      TEXT_SetFont(hItem, &GUI_FontRobotoC_bold22);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER);
      
      hItem = TEXT_CreateEx(0, 45, 390, 25, pMsg->hWin, WM_CF_HIDE, 0, ID_TEXT_TITLE1, "No SDCard detected");
      TEXT_SetFont(hItem, &GUI_FontRobotoC_bold22);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER);
      
      
      break;

    case WM_PAINT:
        GUI_DrawBitmap(AnimationCircles[animationNum], ((LCD_GetXSize() - AnimationCircles[animationNum]->XSize) / 2), ((LCD_GetYSize() - AnimationCircles[animationNum]->YSize) / 2));
        
        if(direction == 0)
        {
          if(animationNum != 0)
          {
            GUI_DrawBitmap(&bmgraphleft, 7, 67);
            GUI_DrawBitmap(bmanimation_graph[graph_index], 140, 65);
            GUI_DrawBitmap(&bmgraphright, 250, 63);
          }
        }
        
        if(Drawback != 0)
        {
          GUI_DrawBitmap(&bmrecord_time_shape, 34, (390 - Drawback*24));
        }
        
        if(animationNum == 3)
        {
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MICRO);
          WM_HideWindow(hItem);

          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MENU);
          WM_HideWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RECORD_PAUSE);
          WM_ShowWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SAVE);
          WM_ShowWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CANCEL);
          WM_ShowWindow(hItem);
        }
        if(animationNum == 0)
        {
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MICRO);
          WM_ShowWindow(hItem);

          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MENU);
          WM_ShowWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RECORD_PAUSE);
          WM_HideWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SAVE);
          WM_HideWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CANCEL);
          WM_HideWindow(hItem);
        }
        
        if((movebutton <= 6) && (!animation_done))
        {           
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SAVE);
          WM_SetWindowPos(hItem, (155 - 20*movebutton), 154, 80, 81);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CANCEL);        
          WM_SetWindowPos(hItem, (155 + 20*movebutton), 154, 80, 81);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE);
          WM_SetWindowPos(hItem, -(65*movebutton), 25, 390, 40);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE_REC);        
          WM_SetWindowPos(hItem, (390 - 65*movebutton), 25, 390, 40);          
        }
        
        if(movebutton == 7)
        { 
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ELAPSED);
          WM_ShowWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SAVE);
          WM_ShowWindow(hItem);

          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RECORD);
          WM_ShowWindow(hItem);

          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CANCEL);
          WM_ShowWindow(hItem);          
        }
        if (movebutton < 7)
        {         
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ELAPSED);
          WM_HideWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SAVE);
          WM_HideWindow(hItem);

          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RECORD);
          WM_HideWindow(hItem);

          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CANCEL);
          WM_HideWindow(hItem);           
        }        
        if (movebutton == 0)
        {
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MICRO);
          WM_ShowWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MENU);
          WM_ShowWindow(hItem);          
        }
        
        if ((GetMotionWindowVisibility() == 0) && (Audio_init == 0))
        {
          AUDIO_RECORDER_Init(50);
          Audio_init = 1;
        }

        if(k_StorageGetStatus(MSD_DISK_UNIT))
        {      
          if(IsMoving == 0)
          {
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE);
            WM_ShowWindow(hItem);  
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE1);
            WM_HideWindow(hItem);             
          }
          else
          {
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE);
            WM_HideWindow(hItem);              
          }
        }
        else
        {
          if(IsMoving == 0)
          {
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE1);
            WM_ShowWindow(hItem); 
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE);
            WM_HideWindow(hItem);            
          }
          else
          {
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE1);
            WM_HideWindow(hItem);           
          }          
        }
        
      break;

  case WM_MOVE:
    if((WM_GetWindowOrgX(pMsg->hWin) % 390) == 0)
    {
      IsMoving = 0;
    }
    else
    {
      IsMoving = 1;
    }
    WM_DefaultProc(pMsg);
    break;
     
  case WM_TIMER:
    
    if(RecorderMode == RECORDER_MODE_RECORDING)
    {
      if (graph_index == 47)
        graph_index = 0;
      else
        graph_index++;
    }
    
    if(direction == 0)
    {
      if(animationNum == 3)
      {
        if(movebutton <= 6)
        {
          movebutton++;
        }
        else if (movebutton == 7)
        {
          animation_done = 1;
        }
      }
      else
      {
        animationNum++;
      }
    }
    else
    {
      if(movebutton > 0)
      {
        if(animationNum > 0)
        {
          animationNum--;
        }
        movebutton--;
      }
      else if (movebutton == 0)
      {
        animation_done = 1;
      }
    }
    
    if(direction == 0)
    {    
      if(Drawback == 5)
      {
        Drawback = 5;
      }
    else
    {
      Drawback++;
    }
    }
    else
    {
      if(Drawback > 0)
      {
        Drawback--;     
      }
    }
    
    WM_InvalidateWindow(pMsg->hWin);
    WM_RestartTimer(pMsg->Data.v, 50);
    
    break;

    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      NCode = pMsg->Data.v;               /* Notification code */
      
      switch (Id)
      {
      case ID_BUTTON_BACK:
        switch (NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          
          if((RecorderMode == RECORDER_MODE_RECORDING) || (RecorderMode == RECORDER_MODE_REC_PAUSE))
          {
            AUDIO_RECORDER_StopRec();
          }
          
          AUDIO_RECORDER_DeInit();
          
          RecorderMode = RECORDER_MODE_REC_IDLE;
          
          WM_DeleteTimer(hTimerTime);
          
          MicroButtonPressed = 0;
          animationNum = 0;
          graph_index = 0;
          Drawback = 0;
          movebutton = 0;
          direction = 0;
          Audio_init = 0;
          hTimerTime = 0;

          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MICRO);
          WM_ShowWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MENU);
          WM_ShowWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RECORD_PAUSE);
          WM_HideWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SAVE);
          WM_HideWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CANCEL);
          WM_HideWindow(hItem);   
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ELAPSED);
          WM_HideWindow(hItem);

          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SAVE);
          WM_HideWindow(hItem);

          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RECORD);
          WM_HideWindow(hItem);          
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CANCEL);
          WM_HideWindow(hItem);          
       
          BackToMainScreen();
          
          break;
        }
        break;
        
      case ID_BUTTON_MICRO:
        switch (NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          if(k_StorageGetStatus(MSD_DISK_UNIT))
          {
            if(animation_done == 1)
            {
              animation_done = 0;
            direction = 0;
            if(hTimerTime == 0)
              hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 50, 0);
            MicroButtonPressed = 1;
            Drawback = 1;
            
            _BuildFileName();   
            
            AUDIO_RECORDER_SelectFile(FileName, FA_CREATE_ALWAYS | FA_WRITE);
            AUDIO_RECORDER_StartRec(DEFAULT_AUDIO_IN_FREQ);
            RecorderMode = RECORDER_MODE_RECORDING; 
          }
          }
          break;
        }
        break;
        
      case ID_BUTTON_RECORD_PAUSE:
        switch (NCode) 
        {
        case WM_NOTIFICATION_RELEASED:

            if(animation_done == 1)
            {          
          if(RecorderMode == RECORDER_MODE_REC_PAUSE)
          {
            AUDIO_RECORDER_PauseResume();
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RECORD);
            TEXT_SetText(hItem, " Pause");
            WM_InvalidateWindow(hItem);
            WM_Update(hItem);    
            
            RecorderMode = RECORDER_MODE_RECORDING;
          }
          else if(RecorderMode == RECORDER_MODE_RECORDING)
          {
            AUDIO_RECORDER_PauseResume();
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RECORD);
            TEXT_SetText(hItem, "Record");
            WM_InvalidateWindow(hItem);
            WM_Update(hItem); 
            
            RecorderMode = RECORDER_MODE_REC_PAUSE;
          }
          animation_done = 0;
            }
          break;
        }
        break;

      case ID_BUTTON_SAVE:
        switch (NCode) 
        {
        case WM_NOTIFICATION_RELEASED:
            if(animation_done == 1)
            { 
          direction = 1;
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MICRO);
            WM_ShowWindow(hItem);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MENU);
            WM_ShowWindow(hItem);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RECORD_PAUSE);
            WM_SetWindowPos(hItem, 155, 154, 80, 81);
            WM_HideWindow(hItem);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SAVE);
            WM_SetWindowPos(hItem, 155, 154, 80, 81);
            WM_HideWindow(hItem);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CANCEL);
            WM_SetWindowPos(hItem, 155, 154, 80, 81);
            WM_HideWindow(hItem);       
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ELAPSED);
            WM_HideWindow(hItem);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SAVE);
            WM_HideWindow(hItem);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RECORD);
            WM_HideWindow(hItem);          
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CANCEL);
            WM_HideWindow(hItem); 
            
            AUDIO_RECORDER_StopRec();
            RecorderMode = RECORDER_MODE_REC_IDLE;
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RECORD_PAUSE);
            WM_InvalidateWindow(hItem);
            WM_Update(hItem); 
            
            animation_done = 0;
            }
          break;
        }
        break;

      case ID_BUTTON_CANCEL:
        switch (NCode) 
        {
        case WM_NOTIFICATION_RELEASED:
            if(animation_done == 1)
            {             
          direction = 1;
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MICRO);
            WM_ShowWindow(hItem);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MENU);
            WM_ShowWindow(hItem);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RECORD_PAUSE);
            WM_SetWindowPos(hItem, 155, 154, 80, 81);
            WM_HideWindow(hItem);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SAVE);
            WM_SetWindowPos(hItem, 155, 154, 80, 81);
            WM_HideWindow(hItem);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CANCEL);
            WM_SetWindowPos(hItem, 155, 154, 80, 81);
            WM_HideWindow(hItem);       
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ELAPSED);
            WM_HideWindow(hItem);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SAVE);
            WM_HideWindow(hItem);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RECORD);
            WM_HideWindow(hItem);          
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CANCEL);
            WM_HideWindow(hItem); 
            
            AUDIO_RECORDER_StopRec();
            RecorderMode = RECORDER_MODE_REC_IDLE;
            AUDIO_RECORDER_RemoveAudioFile(FileName); 
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RECORD_PAUSE);
            WM_InvalidateWindow(hItem);
            WM_Update(hItem); 
            
            animation_done = 0;
            }
            break;
        }
        break;        
        
      case ID_BUTTON_MENU:
        switch (NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          if(k_StorageGetStatus(MSD_DISK_UNIT))
          {          
            hList = WM_CreateWindow(0, 0, 390, 390, WM_CF_SHOW , _cbRecList , 0);
          }
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
  * @brief  clock faces window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  BUTTON_Handle hButtonBack;
  
  hAudioRecorder = WM_CreateWindowAsChild(xpos, ypos, 390, 390, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, _cbAudioRecorder, 0);

  /* Create buttond to be used */
  hButtonBack = BUTTON_CreateEx(90, 335, 210, 55, hAudioRecorder, WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_STAYONTOP, 0 , ID_BUTTON_BACK);
  WM_SetCallback(hButtonBack, _cbButtonBack);
}

/**
  * @brief  Add entire folder to play list.
  * @param  Foldername: pointer to folder name.
  * @retval None
  */
static void _AddEntireFolder(char *Foldername, int update)
{
  FRESULT res;
  FILINFO fno;
  DIR dir;
  char *fn;
  static char tmp[FILEMGR_FILE_NAME_SIZE]; 
  
  res = f_opendir(&dir, Foldername);
  
  if (res == FR_OK)
  {
    
    while (1)
    {
      res = f_readdir(&dir, &fno);
      
      if (res != FR_OK || fno.fname[0] == 0)
      {
        break;
      }
      if (fno.fname[0] == '.')
      {
        continue;
      }

      fn = fno.fname;

      if (WavList.ptr < FILEMGR_LIST_DEPDTH)
      {
        if ((fno.fattrib & AM_DIR) == 0)
        {
          if(((strstr(fn, ".wav")) || (strstr(fn, ".WAV"))))
          {
            
            strcpy(tmp, Foldername);
            strcat(tmp, "/");
            strcat(tmp, fn);
            strncpy((char *)WavList.file[WavList.ptr].name, (char *)tmp, FILEMGR_FILE_NAME_SIZE);
            WavList.ptr++;
          }
        }
      }   
    }
  }
  f_closedir(&dir);
}

/**
  * @brief  Retrieve the file name from a full file path
  * @param  file: pointer to base path
  * @param  path: pointer to full path
  * @retval None
*/
static void FILEMGR_GetFileOnly (char *file, char *path)
{
  char *baseName1, *baseName2;
  baseName1 = strrchr(path,'/');
  baseName2 = strrchr(path,':');
  
  if(baseName1++) 
  { 
    strcpy(file, baseName1);
  }
  else 
  {
    if (baseName2++) 
    {
      strcpy(file, baseName2);
    }
    else
    {
      strcpy(file, path);
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */

