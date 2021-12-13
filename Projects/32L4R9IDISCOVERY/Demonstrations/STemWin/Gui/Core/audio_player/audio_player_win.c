/**
  ******************************************************************************
  * @file    audio_player_win.c
  * @author  MCD Application Team
  * @brief   Audio Player functions
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
#include "Resources.h"
#include "audio_player_res.h"
#include "audio_player_app.h"

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
enum
{
  /* Windows */
  ID_AUDIO_PLAYER_WINDOW = (GUI_ID_USER + 0x50),

  /* Texts */
  ID_TEXT_TITLE_NAME,
  ID_TEXT_ALBUM_NAME,
  ID_TEXT_TITLE,
  ID_TEXT_TITLE1,
  ID_TEXT_REMAINING_DURATION,
  ID_TEXT_CURRENT_DURATION,

  /* Buttons */
  ID_BUTTON_BACK,
  ID_PLAY_BUTTON,
  ID_NEXT_BUTTON,
  ID_PREVIOUS_BUTTON,
  ID_VOLUME_MUTE_BUTTON,
  ID_VOLUME_UP_BUTTON,
  ID_VOLUME_DOWN_BUTTON,
  ID_REPEAT_ONCE_BUTTON,
  ID_REPEAT_ALL_BUTTON,

  /* Slider */
  ID_SLIDER_VOLUME,
};

#define REPEAT_NONE            0
#define REPEAT_ONCE            1
#define REPEAT_ALL             2

#define PLAY_INACTIVE                 0x00
#define PLAY_ACTIVE                   0x01

#define PAUSE_INACTIVE                0x00
#define PAUSE_ACTIVE                  0x01

#define STOP_INACTIVE                 0x00  
#define STOP_ACTIVE                   0x01 

#define VOLUME_UP_DOWN_STEP           10

/* Set player default audio volume (volume can vary from 0 to 100).
   Choose a lower value than DEFAULT_AUDIO_OUT_VOLUME (= 80) defined in BSP */
#define  DEMO_DEFAULT_AUDIO_OUT_VOLUME     60

WM_HWIN hAudioPlayer;
WM_HWIN hAudioWindow;
WM_HWIN hItemTitle;
WM_HWIN hItemAlbum;

WM_HTIMER hTimerUpdate = 0;

TEXT_Handle       hInitText1, hInitText2, hItemCurrentDuration, hItemRemainingDuration;

uint32_t duration_sec = 0;

static long progress;
static int IsMoving = 0;

static const char *MediaRoot = "0:/";

/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static void FILEMGR_GetFileOnly (char *file, char *path);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  audio_player =
{
  1,
  "audio player",
  0,
  Startup,
  NULL,
};

typedef enum
{
  AUDIO_PLAYER_NOT_INITIALIZED,
  AUDIO_PLAYER_INITIALIZING,
  AUDIO_PLAYER_NOT_READY,
  AUDIO_PLAYER_READY,
  AUDIO_PLAYER_PLAYING,
  AUDIO_PLAYER_PAUSED,
  AUDIO_PLAYER_STOPPED,
  AUDIO_PLAYER_ERROR
} AudioPlayerState_t;

typedef union
{
  uint32_t d32;
  struct
  {
    uint32_t repeat         : 2;
    uint32_t pause          : 1;
    uint32_t play           : 1;
    uint32_t stop           : 1;    
    uint32_t mute           : 1;
    uint32_t volume         : 8;   
    uint32_t reserved       : 18;
  }b;
}
AudioSettingsTypeDef;

static WAV_InfoTypedef         WavInfo;
FILELIST_FileTypeDef   WavList;
static uint16_t                Audio_FilePos = 0; 
static AudioSettingsTypeDef    PlayerSettings;
static AudioPlayerState_t      AudioPlayerState = AUDIO_PLAYER_NOT_INITIALIZED;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
// Audio Player specific functions
/**
  * @brief  Play wav file.
  * @param  filename: pointer to file name.
  * @retval None
  */
static uint32_t _PlayFile(char *filename)
{
  char * input_file = filename;
  uint32_t erroneous_files_counter = 0;
  uint32_t ret = 0;
 
  if(haudio.out.state == AUDIOPLAYER_STOP)
  {
     /* Look for readable wav file */
     while (AUDIOPLAYER_GetFileInfo(input_file, &WavInfo) != 0)
     {
       erroneous_files_counter++;
       if (erroneous_files_counter == WavList.ptr)
       {
         /* All wav files yield access or format errors */
         /* A warning message will be displayed */
         ret = 1;
         break; /* exit while loop */
       }
       if(Audio_FilePos < (WavList.ptr - 1))
       {
         Audio_FilePos++;
       }
       else
       {            
         Audio_FilePos = 0;  
       }
       input_file = (char *)WavList.file[Audio_FilePos].name;
     }
     
     /* Readable wav file found */
     if (ret == 0)
     {            
       /* Open audio file */
       if(AUDIOPLAYER_SelectFile(input_file) == 0)
       {
        /* Displayed Album name name */
        if (hItemAlbum != 0)
        {
          TEXT_SetText(hItemAlbum, (char *)WavList.file[Audio_FilePos].album);
          WM_InvalidateWindow(hItemAlbum);
          WM_Update(hItemAlbum);
        }

        /* Update the Duraction texts */
        if (hItemCurrentDuration != 0)
        {
          TEXT_SetText(hItemCurrentDuration, "00:00");
          WM_InvalidateWindow(hItemCurrentDuration);
          WM_Update(hItemCurrentDuration);
        }

        /* Update the Duraction texts */
        if (hItemRemainingDuration != 0)
        {
          char duration[] = "00:00"; /* 'xx:xx format" */
          duration_sec = (WavInfo.FileSize - 44) / (WavInfo.ByteRate);

          sprintf(duration, "%02hhu:%02hhu", (char)(duration_sec / 60), (char)(duration_sec % 60));

          TEXT_SetText(hItemRemainingDuration, duration);
          WM_InvalidateWindow(hItemRemainingDuration);
          WM_Update(hItemRemainingDuration);
        }

        /* start playing */
        AUDIOPLAYER_Play(WavInfo.SampleRate, WavInfo.NbrChannels);
        if(PlayerSettings.b.mute == MUTE_ON)
        {
          AUDIOPLAYER_Mute(MUTE_ON);
        }
      }
    } 
  }
  return ret;
}

/**
  * @brief  Notify the end of wav file.
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_NotifyEndOfFile(void)
{
  WM_HWIN      hItem;
  char         title_name[32];
  
  AUDIOPLAYER_Stop();

  progress = 390;
  WM_InvalidateWindow(hAudioWindow);
  WM_Update(hAudioWindow);  
  
  if(PlayerSettings.b.repeat != REPEAT_NONE)
  {
    if (PlayerSettings.b.repeat == REPEAT_ALL)
    {
      if(Audio_FilePos < (WavList.ptr - 1))
      {
        Audio_FilePos++;
      }
      else 
      {        
        Audio_FilePos = 0; 
      }
    }
    
    hItem = WM_GetDialogItem(hAudioPlayer, ID_TEXT_TITLE_NAME);
    FILEMGR_GetFileOnly (title_name, (char *)WavList.file[Audio_FilePos].name);      
    TEXT_SetText(hItem, title_name); 
    
    hItem = WM_GetDialogItem(hAudioPlayer, ID_TEXT_ALBUM_NAME);     
    TEXT_SetText(hItem, (char *)WavList.file[Audio_FilePos].album);
            
    _PlayFile((char *)WavList.file[Audio_FilePos].name);
  }
  else
  {
    PlayerSettings.b.play = PLAY_INACTIVE;
    
    hItem = WM_GetDialogItem(hAudioPlayer, ID_PLAY_BUTTON);
    WM_InvalidateWindow(hItem);
    WM_Update(hItem);      
  }

  return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @brief  Add entire folder to play list.
  * @param  Foldername: pointer to folder name.
  * @retval None
  */
static void _AddEntireFolder(char *Album, int update)
{
  FRESULT res;
  FILINFO fno;
  DIR dir;
  char *fn;
  static char tmp[FILEMGR_FILE_NAME_SIZE]; 
  
  res = f_opendir(&dir, Album);
  
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
            strcpy(tmp, MediaRoot);
            if(strcmp(Album, MediaRoot) == 0)
            {
              strncpy((char *)WavList.file[WavList.ptr].album, "----", FILEMGR_FILE_NAME_SIZE);
            }
            else
            {
              strncpy((char *)WavList.file[WavList.ptr].album, (char *)Album, FILEMGR_FILE_NAME_SIZE);
              strcpy(tmp, Album);
            }
            strcat(tmp, "/");
            strcat(tmp, fn);
            strncpy((char *)WavList.file[WavList.ptr].name, (char *)tmp, FILEMGR_FILE_NAME_SIZE);
            WavList.ptr++;
          }
        }
      }   
    }
  }
  else
  {
  }
  
  f_closedir(&dir);
}

/**
  * @brief  Paints repeat one button
  * @param  repeat_status: repeat button status
  * @retval None
  */
static void _OnPaint_repeat_one(BUTTON_Handle hObj, uint32_t repeat_status) {
  if(PlayerSettings.b.repeat == REPEAT_ONCE)
  {
    GUI_DrawBitmap(&bmplayer_icon_repeat_one_pressed, 0, 30);
  }
  else   
  {
    GUI_DrawBitmap(&bmplayer_icon_repeat_one_unpressed, 0, 30);
  }
}

/**
  * @brief  callback for repeat one button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_repeat_one(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_repeat_one(pMsg->hWin, PlayerSettings.b.repeat);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints repeat all button
  * @param  repeat_status: repeat button status
  * @retval None
  */
static void _OnPaint_repeat_all(BUTTON_Handle hObj, uint32_t repeat_status) {
  if(PlayerSettings.b.repeat == REPEAT_ALL)
  {
    GUI_DrawBitmap(&bmplayer_icon_repeat_all_pressed, 0, 30);
  }
  else
  {
    GUI_DrawBitmap(&bmplayer_icon_repeat_all_unpressed, 0, 30);
  }
}

/**
  * @brief  callback for repeat all button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_repeat_all(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_repeat_all(pMsg->hWin, PlayerSettings.b.repeat);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints Volume DOWN button
  * @param  speaker_status: speaker button status
  * @retval None
  */
static void _OnPaint_volume_down(BUTTON_Handle hObj, uint32_t speaker_status) {
  int Index;
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  if(Index)
  {
    GUI_DrawBitmap(&bmplayer_icon_minus_unpressed, 54, 23);    
  }
  else
  {
    GUI_DrawBitmap(&bmplayer_icon_minus_pressed, 54, 23);
  }  
}

/**
  * @brief  callback for volume down button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_volume_down(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_volume_down(pMsg->hWin, PlayerSettings.b.mute);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints Volume UP button
  * @param  speaker_status: speaker button status
  * @retval None
  */
static void _OnPaint_volume_up(BUTTON_Handle hObj, uint32_t speaker_status) {
  int Index;
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  if(Index)
  {
    GUI_DrawBitmap(&bmplayer_icon_plus_unpressed, 14, 15);    
  }
  else
  {
    GUI_DrawBitmap(&bmplayer_icon_plus_pressed, 14, 15);
  }  
}

/**
  * @brief  callback for volume up button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_volume_up(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_volume_up(pMsg->hWin, PlayerSettings.b.mute);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints Mute button
  * @param  speaker_status: speaker button status
  * @retval None
  */
static void _OnPaint_mute(BUTTON_Handle hObj, uint32_t mute_status, uint32_t volume) {
  if((mute_status == MUTE_ON) || (volume < VOLUME_UP_DOWN_STEP))
  {
    GUI_DrawBitmap(&bmplayer_icon_sound_mute, 16, 15);
  }
}

/**
  * @brief  callback for mute button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_mute(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_mute(pMsg->hWin, PlayerSettings.b.mute, PlayerSettings.b.volume);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints Previous button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_previous(BUTTON_Handle hObj) {
  int Index;
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;

  if(Index)
  {
    GUI_DrawBitmap(&bmplayer_icon_previous_pressed, 20, 15);
  }
  else
  {
    GUI_DrawBitmap(&bmplayer_icon_previous_unpressed, 20, 15);
  }
}

/**
  * @brief  callback for previous button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE 
  * @retval None
  */
static void _cbButton_previous(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_previous(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints Next button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_next(BUTTON_Handle hObj) {
  int Index;
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;

  if(Index)
  {
    GUI_DrawBitmap(&bmplayer_icon_next_pressed, 20, 15);
  }
  else
  {
    GUI_DrawBitmap(&bmplayer_icon_next_unpressed, 20, 15);
  }
}

/**
  * @brief  callback for next button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE 
  * @retval None
  */
static void _cbButton_next(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_next(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints Play button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_play(BUTTON_Handle hObj) {
  if((PlayerSettings.b.pause == PAUSE_ACTIVE)
  || ((PlayerSettings.b.pause == PAUSE_INACTIVE) && (PlayerSettings.b.play == PLAY_INACTIVE)))
  {
    if(BUTTON_IsPressed(hObj))
    {
      GUI_DrawBitmap(&bmplayer_icon_play_pressed, 20, 10);
    }
    else
    {
      GUI_DrawBitmap(&bmplayer_icon_play_unpressed, 20, 10);
    }
  }
  else if(PlayerSettings.b.play == PLAY_ACTIVE)
  {
    if(BUTTON_IsPressed(hObj))
    {
      GUI_DrawBitmap(&bmplayer_icon_pause_pressed, 28, 18);
    }
    else
    {
      GUI_DrawBitmap(&bmplayer_icon_pause_unpressed, 28, 18);
    }
  }
}

/**
  * @brief  callback for play button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE 
  * @retval None
  */
static void _cbButton_play(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_play(pMsg->hWin);
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
  static WM_HTIMER      hStatusTimer = 0;
  static uint8_t        prev_msd_status = 0;
  WM_HWIN      hItem;
  
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


    AUDIOPLAYER_DeInit();
    AudioPlayerState = AUDIO_PLAYER_NOT_INITIALIZED;
    PlayerSettings.b.mute = MUTE_OFF;  
    PlayerSettings.b.pause = PAUSE_INACTIVE;
    PlayerSettings.b.play = PLAY_INACTIVE;
    PlayerSettings.b.stop = STOP_INACTIVE;
    progress = 0;
    
    WM_HideWindow(hAudioWindow);
    
    hAudioWindow = 0;
 
      WM_DeleteTimer(hTimerUpdate);
      hTimerUpdate = 0;    
    
    
    BackToMainScreen();
      }
      else
      {
        hItem = WM_GetDialogItem(hAudioPlayer, ID_TEXT_TITLE);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);  
        hItem = WM_GetDialogItem(hAudioPlayer, ID_TEXT_TITLE1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);        
      }      
    }
    if(hStatusTimer != 0)
    {
      WM_RestartTimer(hStatusTimer, 500);
    }
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
  * @brief  Paints back button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_ButtonBack(BUTTON_Handle hObj)
{
  if (GetMotionWindowVisibility() == 0)
  {
    GUI_DrawBitmap(&bmback_button_bkg, 0, 2);
    GUI_DrawBitmap(&bmbutton_back, 63, 10);
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

/*********************************************************************
*
*       _cbAudioWindow
*/
static void _cbAudioWindow(WM_MESSAGE * pMsg) {
  WM_HWIN      hItem;
  static WM_HWIN hMediaConnection = 0;
  int          NCode;
  int          Id;
  static WM_HTIMER      hTimerProgress;
  char title_name[32];
      
  switch (pMsg->MsgId) {
  case WM_CREATE:
    
    /* Initialization of 'Play_Button' */
    hItem = BUTTON_CreateEx(155, 97, 77, 65, pMsg->hWin, WM_CF_SHOW, 0, ID_PLAY_BUTTON);
    WM_SetCallback(hItem, _cbButton_play);

    /* Initialization of 'Next_Button' */
    hItem = BUTTON_CreateEx(289, 74, 77, 58, pMsg->hWin, WM_CF_SHOW, 0, ID_NEXT_BUTTON);
    WM_SetCallback(hItem, _cbButton_next);

    /* Initialization of 'Prev_Button' */
    hItem = BUTTON_CreateEx(28, 74, 77, 58, pMsg->hWin, WM_CF_SHOW, 0, ID_PREVIOUS_BUTTON);
    WM_SetCallback(hItem, _cbButton_previous);

    /* Initialization of 'Vol_Mute' */
    hItem = BUTTON_CreateEx(332, 214, 65, 56, pMsg->hWin, WM_CF_SHOW, 0, ID_VOLUME_MUTE_BUTTON);
    WM_SetCallback(hItem, _cbButton_mute);

    /* Initialization of 'Vol_Plus' */
    hItem = BUTTON_CreateEx(262, 219, 46, 46, pMsg->hWin, WM_CF_SHOW, 0, ID_VOLUME_UP_BUTTON);
    WM_SetCallback(hItem, _cbButton_volume_up);

    /* Initialization of 'Vol_Minus' */
    hItem = BUTTON_CreateEx(50, 219, 78, 46, pMsg->hWin, WM_CF_SHOW, 0, ID_VOLUME_DOWN_BUTTON);
    WM_SetCallback(hItem, _cbButton_volume_down);

    /* Initialization of 'Repeat_One_Button' */
    hItem = BUTTON_CreateEx(230, 270, 60, 78, pMsg->hWin, WM_CF_SHOW, 0, ID_REPEAT_ONCE_BUTTON);
    WM_SetCallback(hItem, _cbButton_repeat_one);

    /* Initialization of 'Repeat_All_Button' */
    hItem = BUTTON_CreateEx(124, 274, 60, 73, pMsg->hWin, WM_CF_SHOW, 0, ID_REPEAT_ALL_BUTTON);
    WM_SetCallback(hItem, _cbButton_repeat_all);

    /* Initialization of 'Remain_Duration' */
    hItem = TEXT_CreateAsChild(286, 166, 80, 20, pMsg->hWin, ID_TEXT_REMAINING_DURATION, WM_CF_SHOW, "--:--", GUI_TA_HCENTER | GUI_TA_VCENTER);
    hItemRemainingDuration = hItem;
    TEXT_SetText(hItem, "--:--");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
    TEXT_SetFont(hItem, GUI_FONT_24_ASCII);

    /* Initialization of 'Current_Duration' */
    hItem = TEXT_CreateAsChild(30, 166, 80, 20, pMsg->hWin, ID_TEXT_CURRENT_DURATION, WM_CF_SHOW, "--:--", GUI_TA_HCENTER | GUI_TA_VCENTER);
    hItemCurrentDuration = hItem;
    TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
    TEXT_SetText(hItem, "--:--");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    
    /* Initialization of 'Album_Name' */
    hItem = TEXT_CreateAsChild(80, 65, 230, 20, pMsg->hWin, ID_TEXT_ALBUM_NAME, WM_CF_SHOW, "WAV Format", GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
    TEXT_SetTextColor(hItem, GUI_WHITE);
    
    /* Initialization of 'Title_Text' */
    hItem = TEXT_CreateAsChild(105, 32, 180, 20, pMsg->hWin, ID_TEXT_TITLE_NAME, WM_CF_SHOW, "NO MEDIA FILES", GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_24B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);   
    
    hTimerProgress = WM_CreateTimer(pMsg->hWin, 0, 333, 0);
    
    hMediaConnection = WM_CreateWindowAsChild(389, 389, 1, 1, pMsg->hWin, WM_CF_SHOW, _cbMediaConnection , 0);
    break;

  case WM_TIMER:
    Id = WM_GetTimerId(pMsg->Data.v);
    
    if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
    {
      /* 'xx:xx format" */
      char tmp[] = "00:00";     
      uint32_t current_duration;
      uint32_t remaining_duration;

      progress = AUDIOPLAYER_GetProgress();

      current_duration = progress / WavInfo.ByteRate;
      remaining_duration = ((WavInfo.FileSize - 44) / (WavInfo.ByteRate)) - current_duration;

      /* Update the Duraction texts */
      if (hItemCurrentDuration != 0)
      {
        sprintf(tmp, "%02hhu:%02hhu", (char)(current_duration / 60), (char)(current_duration % 60));

        TEXT_SetText(hItemCurrentDuration, tmp);
        WM_InvalidateWindow(hItemCurrentDuration);
        WM_Update(hItemCurrentDuration);
      }
      
      /* Update the Duraction texts */
      if (hItemRemainingDuration != 0)
      {
        sprintf(tmp, "%02hhu:%02hhu", (char)(remaining_duration / 60), (char)(remaining_duration % 60));

        TEXT_SetText(hItemRemainingDuration, tmp);
        WM_InvalidateWindow(hItemRemainingDuration);
        WM_Update(hItemRemainingDuration);
      }

      /* Set progress bitmap size and niddle position */
      progress = (long)(bmplayer_pb_playback_full.XSize * current_duration)/(duration_sec+1);

      WM_InvalidateWindow(pMsg->hWin);
    }
    else if(AUDIOPLAYER_GetState() == AUDIOPLAYER_EOF)
    {
      AUDIOPLAYER_Process();
    }
    
    AUDIOPLAYER_Process();
    WM_RestartTimer(pMsg->Data.v, 1000);    
    break;

  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_PLAY_BUTTON:
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_CLICKED:
        {
          if(AUDIOPLAYER_GetState() == AUDIOPLAYER_STOP)
          {
            if(WavList.ptr > 0)
            {
              if (_PlayFile((char *)WavList.file[Audio_FilePos].name) == 0)
              {  
                PlayerSettings.b.play = PLAY_ACTIVE;
                AudioPlayerState = AUDIO_PLAYER_PLAYING;
              }
              else
              {
                /* Audio files issue */
                AudioPlayerState = AUDIO_PLAYER_ERROR;  
              }
            }
            else
            {
            }          
          }
          else if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
          {
            PlayerSettings.b.pause = PAUSE_ACTIVE;
            PlayerSettings.b.play = PLAY_INACTIVE;
            AUDIOPLAYER_Pause();
            AudioPlayerState = AUDIO_PLAYER_PAUSED;   
          }
          else if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PAUSE)
          {
            PlayerSettings.b.pause = PAUSE_INACTIVE;
            PlayerSettings.b.play = PLAY_ACTIVE;
            AUDIOPLAYER_Resume();
            AudioPlayerState = AUDIO_PLAYER_PLAYING;
            if(PlayerSettings.b.mute == MUTE_ON)
            {
              AUDIOPLAYER_Mute(MUTE_OFF);
              PlayerSettings.b.mute = MUTE_OFF;
              hItem = WM_GetDialogItem(pMsg->hWin, ID_VOLUME_MUTE_BUTTON);
              WM_InvalidateWindow(hItem);
              WM_Update(hItem);               
            }            
          }        
        }
        break;
      }
      break;
    case ID_NEXT_BUTTON:
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_CLICKED:
        {
          if( WavList.ptr > 0)
          {        
            if(Audio_FilePos < (WavList.ptr - 1))
            {
              Audio_FilePos++;
            }
            else
            {            
              Audio_FilePos = 0;  
            }
            
            if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
            {    
              if(PlayerSettings.b.pause == PAUSE_ACTIVE)
              {  
                PlayerSettings.b.pause = PAUSE_INACTIVE;                
                PlayerSettings.b.play = PLAY_ACTIVE;             
              }
              
              AUDIOPLAYER_Stop();
              _PlayFile((char *)WavList.file[Audio_FilePos].name); 
            }
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
            WM_InvalidateWindow(hItem);
            WM_Update(hItem);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE_NAME);
            FILEMGR_GetFileOnly (title_name, (char *)WavList.file[Audio_FilePos].name);      
            TEXT_SetText(hItem, title_name); 
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ALBUM_NAME);     
            TEXT_SetText(hItem, (char *)WavList.file[Audio_FilePos].album);            
          }
        }
        break;
      }
      break;
    case ID_PREVIOUS_BUTTON:
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_CLICKED:
        {
          if( WavList.ptr > 0)
          {
            if(Audio_FilePos > 0)
            {             
              Audio_FilePos--;       
            }
            else if(PlayerSettings.b.repeat == REPEAT_ALL)
            {          
              Audio_FilePos = (WavList.ptr - 1);         
            }          
            
            if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
            {  
              if(PlayerSettings.b.pause == PAUSE_ACTIVE)
              {  
                PlayerSettings.b.pause = PAUSE_INACTIVE;                
                PlayerSettings.b.play = PLAY_ACTIVE;   
              }
              
              AUDIOPLAYER_Stop();      
              _PlayFile((char *)WavList.file[Audio_FilePos].name);              
            }
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
            WM_InvalidateWindow(hItem);
            WM_Update(hItem);
          
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE_NAME);
            FILEMGR_GetFileOnly (title_name, (char *)WavList.file[Audio_FilePos].name);      
            TEXT_SetText(hItem, title_name); 
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ALBUM_NAME);     
            TEXT_SetText(hItem, (char *)WavList.file[Audio_FilePos].album);            
          }
        }
        break;
      }
      break;
    case ID_VOLUME_MUTE_BUTTON:
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        if(PlayerSettings.b.mute == MUTE_ON)
        {
          AUDIOPLAYER_Mute(MUTE_OFF);
          PlayerSettings.b.mute = MUTE_OFF;
        }
        else
        {
          AUDIOPLAYER_Mute(MUTE_ON);
          PlayerSettings.b.mute = MUTE_ON;
        }
        break;
      }
      break;
    case ID_VOLUME_UP_BUTTON:
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        if(PlayerSettings.b.mute == MUTE_ON)
        {
          AUDIOPLAYER_Mute(MUTE_OFF);
          PlayerSettings.b.mute = MUTE_OFF;
        } 
        if(PlayerSettings.b.volume < (100 - VOLUME_UP_DOWN_STEP))
        {
          AUDIOPLAYER_SetVolume(PlayerSettings.b.volume + VOLUME_UP_DOWN_STEP);
          PlayerSettings.b.volume = PlayerSettings.b.volume + VOLUME_UP_DOWN_STEP;
        }
        else
        {
          AUDIOPLAYER_SetVolume(100);
          PlayerSettings.b.volume = 100;
        }
          WM_InvalidateWindow(pMsg->hWin);
          WM_Update(pMsg->hWin);        
        break;
      }
      break;
    case ID_VOLUME_DOWN_BUTTON:
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        if(PlayerSettings.b.volume > VOLUME_UP_DOWN_STEP)
        {
          AUDIOPLAYER_SetVolume(PlayerSettings.b.volume - VOLUME_UP_DOWN_STEP);
          PlayerSettings.b.volume = PlayerSettings.b.volume - VOLUME_UP_DOWN_STEP;
        }
        else
        {
          if(PlayerSettings.b.mute == MUTE_OFF)
          {          
            AUDIOPLAYER_Mute(MUTE_ON);
            PlayerSettings.b.mute = MUTE_ON;
            AUDIOPLAYER_SetVolume(1);
            PlayerSettings.b.volume = 1;
          }          
        }
          WM_InvalidateWindow(pMsg->hWin);
          WM_Update(pMsg->hWin);        
        break;
      }
      break;
    case ID_REPEAT_ONCE_BUTTON:
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        if (PlayerSettings.b.repeat == REPEAT_ONCE)
        {
          PlayerSettings.b.repeat = REPEAT_NONE;
        }
        else
        {
          PlayerSettings.b.repeat = REPEAT_ONCE;
          hItem = WM_GetDialogItem(pMsg->hWin, ID_REPEAT_ALL_BUTTON);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);
        }
        break;
      }
      break;
    case ID_REPEAT_ALL_BUTTON:
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        if (PlayerSettings.b.repeat == REPEAT_ALL)
        {
          PlayerSettings.b.repeat = REPEAT_NONE;
        }
        else
        {
          PlayerSettings.b.repeat = REPEAT_ALL;
          hItem = WM_GetDialogItem(pMsg->hWin, ID_REPEAT_ONCE_BUTTON);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);
        }
        break;
      }
      break;
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

  case WM_PAINT:
    GUI_DrawBitmap(&bmplayer_background, 0, 0);
    if( WavList.ptr > 0)
    {
      GUI_RECT Rect = { 0 };

      GUI_DrawBitmap(&bmplayer_icon_speaker_transparent, 43, 229);
      GUI_DrawBitmap(&bmplayer_icon_speaker_white, 329, 229);
      GUI_DrawBitmap(&bmplayer_pb_volume_bkg, (390 - bmplayer_pb_volume_bkg.XSize)/2, 229 + bmplayer_icon_speaker_white.YSize/2 - bmplayer_pb_volume_bkg.YSize/2);
      GUI_DrawBitmap(&bmplayer_pb_volume_empty, (390 - bmplayer_pb_volume_empty.XSize)/2, 229 + bmplayer_icon_speaker_white.YSize/2 - bmplayer_pb_volume_empty.YSize/2);

      Rect.x0 = (390 - bmplayer_pb_volume_full.XSize)/2;
      Rect.y0 = (229 + bmplayer_icon_speaker_white.YSize/2 - bmplayer_pb_volume_full.YSize/2);
      Rect.x1 = Rect.x0 + (bmplayer_pb_volume_full.XSize * PlayerSettings.b.volume) / 100;
      Rect.y1 = Rect.y0 + bmplayer_pb_volume_full.YSize;
      GUI_SetClipRect(&Rect);
      GUI_DrawBitmap(&bmplayer_pb_volume_full, Rect.x0, Rect.y0);
      GUI_SetClipRect(NULL);
      
      Rect.x0 = 5;
      Rect.y0 = 127;
      Rect.x1 = Rect.x0 + progress;
      Rect.y1 = Rect.y0 + bmplayer_pb_playback_full.YSize;
      GUI_SetClipRect(&Rect);
      GUI_DrawBitmap(&bmplayer_pb_playback_full, Rect.x0, Rect.y0);
      GUI_SetClipRect(NULL);
      
      static const uint8_t niddle_ypos[200] = {   0,   0,   0,   1,   1,   1,   2,   2,    2,   3,   3,   4,   4,   5,   5,   5,   5,   6,   6,   7,
        7,   8,   8,   9,   9,  10,  10,  11,  11,  12,  13,  14,  14,  15,  16,  17,  17,  17,  18,  18,
       19,  20,  20,  21,  21,  22,  22,  22,  23,  23,  24,  24,  25,  25,  26,  26,  27,  27,  28,  28,
       28,  29,  29,  30,  30,  30,  31,  31,  32,  32,  33,  33,  33,  34,  34,  35,  35,  35,  35,  35,
       35,  36,  36,  36,  36,  37,  37,  37,  37,  37,  37,  38,  38,  38,  38,  38,  38,  38,  38,  38,
       38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  37,  37,
       37,  37,  36,  36,  36,  36,  35,  35,  35,  35,  34,  34,  33,  33,  33,  32,  32,  31,  31,  30,
       30,  30,  29,  29,  28,  28,  28,  27,  27,  26,  26,  25,  25,  24,  24,  23,  23,  22,  22,  22,
       21,  21,  20,  20,  19,  18,  18,  17,  17,  17,  16,  15,  14,  14,  13,  12,  11,  11,  10,  10,
        9,   9,   8,   8,   7,   7,   6,   6,   5,   5,   4,   4,   3,   3,   2,   2,   1,   1,   0,   0
      };

      if((Rect.x1 > 92) && ((Rect.x1 < 292)))
      {
        GUI_DrawBitmap(&bmplayer_pb_playback_niddle, (Rect.x1 - bmplayer_pb_playback_niddle.XSize/2), (Rect.y0 + 14 - bmplayer_pb_playback_niddle.YSize/2) + niddle_ypos[Rect.x1 - 92]);
      }
      else
      {
        GUI_DrawBitmap(&bmplayer_pb_playback_niddle, (Rect.x1 - bmplayer_pb_playback_niddle.XSize/2), (Rect.y0 + 14 - bmplayer_pb_playback_niddle.YSize/2));
      }
    }  
    break;

  case WM_DELETE:
    if(hTimerProgress != 0)
    {
      WM_DeleteTimer(hTimerProgress);
      hTimerProgress = 0;
    }
    if(hMediaConnection)
    {
      WM_DeleteWindow(hMediaConnection);
      hMediaConnection = 0;
    }
    AUDIOPLAYER_DeInit();
    AudioPlayerState = AUDIO_PLAYER_NOT_INITIALIZED;
    PlayerSettings.b.mute = MUTE_OFF;  
    PlayerSettings.b.pause = PAUSE_INACTIVE;
    PlayerSettings.b.play = PLAY_INACTIVE;
    PlayerSettings.b.stop = STOP_INACTIVE;
    progress = 0;
    hAudioWindow = 0;
    BackToMainScreen();
    break; 

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  callback for Audio Player drawing
  * @param  pMsg: Pointer to Date structure
  * @retval None
  */
static void _cbAudioPlayer(WM_MESSAGE * pMsg) {

  int          NCode;
  int          Id;
  WM_HWIN      hItem;
  char         title_name[32];
  
  switch (pMsg->MsgId) {

  case WM_CREATE:
      hItem = TEXT_CreateEx(0, 25, 390, 40, pMsg->hWin, WM_CF_HIDE, 0, ID_TEXT_TITLE, "Play Now !");
      TEXT_SetFont(hItem, &GUI_FontRobotoC_bold22);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER);
      
      hItem = TEXT_CreateEx(0, 45, 390, 25, pMsg->hWin, WM_CF_HIDE, 0, ID_TEXT_TITLE1, "No SDCard detected");
      TEXT_SetFont(hItem, &GUI_FontRobotoC_bold22);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER);
      break; 
      
    case WM_PAINT:
      if(hAudioWindow == 0)
      {
        hAudioWindow  = WM_CreateWindowAsChild(0, 0, 390, 390, pMsg->hWin, WM_CF_HIDE, _cbAudioWindow, 0);
      }
      if((AudioPlayerState == AUDIO_PLAYER_NOT_INITIALIZED) || (AudioPlayerState == AUDIO_PLAYER_NOT_READY)
         || (!k_StorageGetStatus(MSD_DISK_UNIT)))
      {
        GUI_DrawBitmap(&bmmodule_entry_icon, (LCD_GetXSize() - bmmodule_entry_icon.XSize) / 2, (LCD_GetYSize() - bmmodule_entry_icon.YSize) / 2);
        GUI_DrawBitmap(&bmplayer_icon, ((LCD_GetXSize() - bmplayer_icon.XSize) / 2) + 5, ((LCD_GetYSize() - bmplayer_icon.YSize) / 2));
      }
      else if(AudioPlayerState == AUDIO_PLAYER_INITIALIZING)
      {
        /* TODO : Show initilization message */
      }
      else if(AudioPlayerState == AUDIO_PLAYER_READY)
      {
        if(k_StorageGetStatus(MSD_DISK_UNIT))
        {
          GUI_DrawBitmap(&bmplayer_background, 0, 0);
          WM_ShowWindow(hAudioWindow);
        }
      }
      else if(AudioPlayerState == AUDIO_PLAYER_NOT_READY)
      {
        /* TODO : Show message failure */
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
  case WM_TOUCH:
    /* Initilize the Audio Player and check the status of the SD card content */
    if((IsMoving == 0) && (AudioPlayerState == AUDIO_PLAYER_NOT_INITIALIZED))
    {
      /* Initilize the Audio Player */      
      AudioPlayerState = AUDIO_PLAYER_INITIALIZING;
      WM_InvalidateWindow(pMsg->hWin);

      hItemTitle = 0;
      hItemAlbum = 0;

      /* Prevent Volume 0 */
      if(PlayerSettings.b.volume == 0)
      {
        PlayerSettings.b.volume = DEMO_DEFAULT_AUDIO_OUT_VOLUME;
      }

      Audio_FilePos = 0;

      WavList.ptr = 0;
      _AddEntireFolder("Record", 0);
      _AddEntireFolder((char *)MediaRoot, 0);
      _AddEntireFolder("Electronica", 0);
      _AddEntireFolder("Jazz", 0);
      _AddEntireFolder("NoCover", 0);
      _AddEntireFolder("Rock", 0);
      _AddEntireFolder("Audio", 0);

      /* Initialize audio Interface */
      if(AUDIOPLAYER_Init(PlayerSettings.b.volume, 0) == AUDIOPLAYER_ERROR_NONE)
      {
        AudioPlayerState = AUDIO_PLAYER_READY;
      }
      else
      {
        AudioPlayerState = AUDIO_PLAYER_NOT_READY;
      }
      strcpy(title_name, (char *)WavList.file[0].album);      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ALBUM_NAME);     
      TEXT_SetText(hItem, title_name); 
      
      hItem = WM_GetDialogItem(hAudioWindow, ID_TEXT_TITLE_NAME);
      FILEMGR_GetFileOnly (title_name, (char *)WavList.file[0].name);      
      TEXT_SetText(hItem, title_name);        
      
      
    }
    break;

  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_BACK:
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        {
          WM_DeleteWindow(hAudioWindow);
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
  * @brief  Retrieve the file name from a full file path
  * @param  file: pointer to base path
  * @param  path: pointer to full path
  * @retval None
*/
static void FILEMGR_GetFileOnly (char *file, char *path)
{
  /* Filter the path and get only the file name */
  uint32_t pos = 0, 
  size = strlen (path);
  uint32_t i = 0;
  for (i = 0; i < size; i++){
    if (path[i] == '/'){
      pos = i + 1;
    }
  }
  for (i = 0; i < (size - pos) ; i++){
    if (path[i + pos] == '.'){
      break;
    }
    file[i] = path[i + pos];
  }
  file[i] = '\0';
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/**
  * @brief  clock faces window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  int xSize;
  int ySize;
  BUTTON_Handle hButtonBack;

  /* Get the LCD sizes */
  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize();
  
  hAudioPlayer       = WM_CreateWindowAsChild(xpos, ypos, xSize, ySize, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, _cbAudioPlayer, 0);

  /* Create buttons to be used */
  hButtonBack = BUTTON_CreateEx(90, 350, 210, 55, hAudioPlayer, WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_STAYONTOP, 0 , ID_BUTTON_BACK);
  WM_SetCallback(hButtonBack, _cbButtonBack);
}

/**
  * @}
  */

/**
  * @}
  */

