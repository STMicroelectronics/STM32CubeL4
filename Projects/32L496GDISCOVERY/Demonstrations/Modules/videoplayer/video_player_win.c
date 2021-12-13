/**
  ******************************************************************************
  * @file    video_player_win.c
  * @author  MCD Application Team  
  * @brief   video player functions
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
#include "GUI_AVI.h"
#include "DIALOG.h"
#include "k_module.h"
#include "video_player_res.c"

/** @addtogroup VIDEO_PLAYER_MODULE
  * @{
  */

#if defined(DIM_FEATURE_ENABLED)
extern uint32_t dimming_timer_to_start;
extern RTC_HandleTypeDef Dim_RTCHandle;
uint32_t video_playing = 0; 
#endif

/** @defgroup VIDEO_PLAYER
  * @brief video player routines 
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static void _StopPlay(FIL * file) ;
static uint8_t _StartPlay(char * filename, FIL * file, uint16_t x0, uint16_t y0) ;
static void _AddEntireFolder(char *Foldername);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  video_player_board =
{  
  5,
  (uint8_t*)"",
  &bmvideo5,
  Startup,
}
;

/* Private defines -----------------------------------------------------------*/
#define VIDEO_FRAME_ID            (GUI_ID_USER + 0x00)


#define FILENAME1_ID              (GUI_ID_USER + 0x19)
#define ID_BUTTON_EXIT_VIDEO      (GUI_ID_USER + 0x1B)

#define ID_TITLE                  (GUI_ID_USER + 0x51)
 
#define ID_FRAMEWIN_VIDEO              (GUI_ID_USER + 0x20)
#define ID_TEXT_FILENAME_VIDEO         (GUI_ID_USER + 0x21)
#define ID_TEXT_FILENAME_VIDEO2        (GUI_ID_USER + 0x22)
#define ID_BUTTON_OK_MESSAGE_VIDEO     (GUI_ID_USER + 0x23)

#define VIDEO_IDLE             0
#define VIDEO_PLAY             1
#define VIDEO_STOP             2
#define VIDEO_PAUSE            3

#define VIDEO_FRAME_TIME       40



/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  { WINDOW_CreateIndirect, "Window", 0, 0, 0, 240, 240, FRAMEWIN_CF_MOVEABLE }
};

 /* Info Dialog Create */
static const GUI_WIDGET_CREATE_INFO _aFileInfoDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "WARNING", ID_FRAMEWIN_VIDEO, 0, 0, 200, 200, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Please add AVI file", ID_TEXT_FILENAME_VIDEO, 30, 80, 200, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "      in SDCard", ID_TEXT_FILENAME_VIDEO2, 30, 98, 200, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OK", ID_BUTTON_OK_MESSAGE_VIDEO, 65, 120, 70, 70, 0, 0x0, 0 },
};

 GUI_AVI_HANDLE havi;

WM_HWIN  VideoWin = 0;
uint8_t                         VideoPlayer_State = VIDEO_IDLE;
FIL                             Video_File;
uint8_t                         Video_Path[256];
static FILELIST_FileTypeDef     VideoList;
static uint32_t                 Video_FilePos = 0;
GUI_AVI_INFO                    Video_Info;

#define AVI_VIDEO_BUF_SIZE    (20 * 1024)
uint8_t    FrameBuffer[AVI_VIDEO_BUF_SIZE];
#define AUDIO_BUFFER_SIZE      (5 * 1024) 
uint8_t    AudioBuffer[AUDIO_BUFFER_SIZE + 8];

extern int status;
extern GUI_CONST_STORAGE GUI_BITMAP bmwarning;

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Paints record button
  * @param  hObj: button handle
  * @retval None
*/
static void _OnPaint_exit(BUTTON_Handle hObj) 
{
    GUI_DrawBitmap(&bmvideo_exit, 20, 0);
} 

/**
  * @brief  callback for exit button
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
  * @brief  Get data.
  * @param  p: Handle to file structure
  * @param  ppData: pointer to data buffer to be read
  * @param  NumBytesReq: number of bytes to be read
  * @param  Off: offset in the file    
  * @retval number of read bytes
  */
static int _GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off)
{
  unsigned int NumBytesRead;
  FRESULT res;
  
  if(Off != 0xFFFFFFFF)
  {
    /* Set file pointer to the required position */
    if(f_lseek((FIL *)p, Off) != 0)
    {
      Error_Handler();
    }
  }
  
  /* Read data into buffer */
  res = f_read((FIL *)p, (U8 *)*ppData, NumBytesReq, &NumBytesRead);

  if(res != FR_OK)
  {
//    Error_Handler();
  }
   
  /* Return number of available bytes */
  return NumBytesRead;  
}

/**
  * @brief  Stop play 
  * @param  None
  * @retval None
  */
static void _StopPlay(FIL * file) 
{
  GUI_AVI_Stop(havi);

  f_close(file);
 
  WM_InvalidateWindow(VideoWin);   
}


/**
  * @brief  Callback function for movie player. Uses multiple buffering if
  *         available to avoid tearing effects.
  * @param  hMem:         pointer to parent handle
  * @param  Notification: notification ID
  * @param  CurrentFrame: current playing frame
  * @retval None
  */
void _cbNotify(GUI_HMEM hMem, int Notification, U32 CurrentFrame) 
{  
  switch (Notification) 
  {      
  case GUI_AVI_NOTIFICATION_EOF:
    {
      _StopPlay(&Video_File);

      if(Video_FilePos < (VideoList.ptr - 1))
      {
        Video_FilePos++;
      }
      else 
      {        
        Video_FilePos = 0;          
      }        
  _StartPlay((char *)VideoList.file[Video_FilePos].name, &Video_File,0, 40);    
} 
    break;
  }
}

/**
  * @brief  Start play
  * @param  filename: pointer to the video file name
  * @retval None
  */
static uint8_t _StartPlay(char * filename, FIL * file, uint16_t x0, uint16_t y0) 
{  
  if(f_open(file, filename, FA_OPEN_EXISTING | FA_READ) == FR_OK)
  {
    GUI_AVI_Start(havi, file, f_size(file));
    GUI_AVI_GetInfo (havi, &Video_Info);
        
    if((Video_Info.xSize == 0) || (Video_Info.ySize == 0) || 
       (Video_Info.xSize > LCD_GetXSize()) || (Video_Info.ySize > LCD_GetYSize()))
    {
      _StopPlay(file);
      VideoPlayer_State = VIDEO_STOP;
  
      return 1;
    }
    if(GUI_AVI_Show(havi, x0, y0, 0) != 0)
    {
      Error_Handler();
    }
  }
  return 0;
}

/**
  * @brief  Callback function of the media connection status
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbMediaConnection(WM_MESSAGE * pMsg) 
{
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
    
//    if(!BSP_SD_IsDetected())
//    {    
      VideoList.ptr = 0;
      Video_FilePos = 0;
      
      _StopPlay(&Video_File);
      
      GUI_EndDialog(VideoWin, 0);
      
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
  * @brief  main callback for Video Player
  * @param  pMsg: Pointer to Date structure
  * @retval None
  */

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
   
    hItem = BUTTON_CreateEx(186, 3, 50, 50, pMsg->hWin, WM_CF_SHOW|WM_CF_HASTRANS, 0, ID_BUTTON_EXIT_VIDEO);
    WM_SetCallback(hItem, _cbButton_exit);     

    /* Detect if no wav file in the SDCard */
    if(VideoList.ptr == 0)
    {
      hItem = TEXT_CreateEx(20,  10,  170,  25, pMsg->hWin, WM_CF_SHOW, 0, ID_TITLE, "No available AVI files");
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TITLE);
      TEXT_SetFont(hItem, &GUI_Font20B_ASCII); 
      TEXT_SetTextColor(hItem,GUI_WHITE); 
    }

#if defined(DIM_FEATURE_ENABLED)    
    /* Flag that video player is now running */
    video_playing = 1;
#endif
    
    WM_CreateWindowAsChild(239, 239, 1, 1, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbMediaConnection , 0);
    
    _StartPlay((char *)VideoList.file[Video_FilePos].name, &Video_File,0, 40);
           
    break;
    
  case WM_PAINT:
    GUI_SetColor(0x7F14E7);
    GUI_FillRect(0, 0, 240, 40);    
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
        
    switch (NCode) {
    case WM_NOTIFICATION_RELEASED:      /* React only if released */
      switch (Id) {
      case ID_BUTTON_EXIT_VIDEO:
        status = 1;
        _StopPlay(&Video_File);
        GUI_EndDialog(VideoWin, 0);
#if defined(DIM_FEATURE_ENABLED)
        /* Exit video-sub-demo on user action.
           Enable dimming timer re-start */   
        dimming_timer_to_start = 1;        
        /* Flag that no video player is not running anymore */
         video_playing = 0;
#endif        
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
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_OK_MESSAGE_VIDEO);
    WM_SetCallback(hItem, _cbButton_okMessage);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FILENAME_VIDEO);
    TEXT_SetFont(hItem, &GUI_Font16B_ASCII);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FILENAME_VIDEO2);
    TEXT_SetFont(hItem, &GUI_Font16B_ASCII);
    
    break;
        
  case WM_PAINT:    
    GUI_DrawBitmap(&bmwarning, 65, 10);
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_OK_MESSAGE_VIDEO: /* Notifications sent by 'OK' */
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
  * @brief  Add entire folder to play list.
  * @param  Foldername: pointer to folder name.
  * @retval None
  */
static void _AddEntireFolder(char *Foldername)
{
  FRESULT res;
  FILINFO fno;
  DIR dir;
  char *fn;
  static char tmp[FILEMGR_FILE_NAME_SIZE]; 

  res = f_opendir(&dir, Foldername);
  
  if(res == FR_OK)
  {
    
    while (1)
    {
      res = f_readdir(&dir, &fno);
      
      if(res != FR_OK || fno.fname[0] == 0)
      {
        break;
      }
      if(fno.fname[0] == '.')
      {
        continue;
      }

      fn = fno.fname;

      if(VideoList.ptr < FILEMGR_LIST_DEPDTH)
      {
        if((fno.fattrib & AM_DIR) == 0)
        {
          if((strstr(fn, ".avi")) || (strstr(fn, ".AVI")))
          {
            strcpy(tmp, Foldername);
            strcat(tmp, "/");
            strcat(tmp, fn);
            
            strncpy((char *)VideoList.file[VideoList.ptr].name, (char *)tmp, FILEMGR_FILE_NAME_SIZE);
            VideoList.ptr++;
          }
        }
      }   
    }

    if (f_closedir(&dir) != 0)
    {
      Error_Handler();
    }
  }
}

/**
  * @brief  video player window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  VideoPlayer_State = VIDEO_IDLE;
  Video_FilePos = 0;
  WM_HWIN hItem;
#if defined(DIM_FEATURE_ENABLED)
  /* Ensure dimming timer won't be re-started 
     while video is playing */
  dimming_timer_to_start = 0;  
  HAL_RTCEx_DeactivateWakeUpTimer(&Dim_RTCHandle);  
#endif  
  
  havi = GUI_AVI_CreateEx(_GetData, _cbNotify) ;
  
  if (havi == 0)
  {
    Error_Handler();
  }

  GUI_AVI_SetBuffers(havi, FrameBuffer, AVI_VIDEO_BUF_SIZE, AudioBuffer, AUDIO_BUFFER_SIZE);
  
  if (GUI_AVI_SelectMode(havi, 0) != 0)
  {
    Error_Handler();
  }
  
  /* Configure SD IT pin */
  BSP_SD_Init();      
  
  VideoList.ptr = 0;
  _AddEntireFolder("0:");
  _AddEntireFolder("0:/Video"); 
  
  /* Configure SD IT pin */
  BSP_SD_ITConfig();
  
  VideoWin = GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog, hWin, xpos, ypos);
  
  if(VideoList.ptr == 0)
  {
    hItem = GUI_CreateDialogBox(_aFileInfoDialogCreate, 
                                GUI_COUNTOF(_aFileInfoDialogCreate), 
                                _cbFileInfoDialog, 
                                hWin, 
                                20, 20);
    WM_MakeModal(hItem);  
  }
}

/**
  * @}
  */

/**
  * @}
  */

