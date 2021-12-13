/**
  ******************************************************************************
  * @file    k_menu.c
  * @author  MCD Application Team
  * @brief   This file provides the first demo menu functions
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
#include "k_menu.h"
#include "DIALOG.h"


/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MENU
  * @brief Kernel MENU routines
  * @{
  */

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define ID_BUTTON_MODULE (GUI_ID_USER + 0x01)

/* Private variables ---------------------------------------------------------*/
extern WM_HWIN hclockmain;
extern WM_HWIN hAudioPlayer;
extern WM_HWIN hAudioRecorder;
extern WM_HWIN hPowerMeasurement;
extern WM_HWIN hResetScreen;
extern WM_HWIN hInformation;

int     FirstTime;

WM_HWIN hMainWin;
WM_HWIN hMotion;
static int FirstModuleX = 0;
static int Reset_From_IDD_Measure = 0;

/* Timer declarations */
WM_HTIMER hTimerDoubleTap;

/* Private typedef -----------------------------------------------------------*/
typedef struct 
{
  int xPos;
  int yPos;
  int Index;
} ITEM_INFO;

typedef struct 
{
  int                 Pos;
  int                 NumItems;
  int                 xSizeItem,   ySizeItem;
  int                 xSizeWindow, ySizeWindow;
  int                 rx,          ry;
  int                 mx,          my;
  int                 FinalMove;
  ITEM_INFO         * pItemInfo;
} PARA;
    
/**
  * @brief  principle window callback
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbBk(WM_MESSAGE * pMsg) 
{
  PARA      * pPara;

  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      WM_GetUserData(pMsg->hWin, &pPara, sizeof(pPara));
      GUI_DrawBitmap(&bmbackground, 0, 0);
      break;
  }
}
    
/**
  * @brief  Return to the clock face screen
  * @param  None
  * @retval None
  */
void BackToMainScreen(void)
{
  FirstModuleX = 0;

  WM_SetWindowPos(hclockmain,           390 * 0, 0, 390, 390);
  WM_SetWindowPos(hAudioPlayer,         390 * 1, 0, 390, 390);
  WM_SetWindowPos(hAudioRecorder,       390 * 2, 0, 390, 390);
  WM_SetWindowPos(hPowerMeasurement,    390 * 3, 0, 390, 390);
  WM_SetWindowPos(hInformation,         390 * 4, 0, 390, 390);
  WM_SetWindowPos(hResetScreen,         390 * 5, 0, 390, 390);

  WM_ShowWindow(hMotion);   
}

/**
  * @brief  Return the visibility state of the motion window
  * @param  None
  * @retval 0: Window is not visible or 1: Window is visible
  */
int GetMotionWindowVisibility(void)
{
  return WM_IsVisible(hMotion); 
}

/**
  * @brief  Set that the reset was done in IDD module
  * @param  resetFromIDD: Status for the reset from IDD
  * @retval None
  */
void Set_Reset_From_IDD_Measure(int resetFromIDD)
{
  Reset_From_IDD_Measure = resetFromIDD;
}

/**
  * @brief  Check the reset from IDD module status
  * @param  resetFromIDD: Status for the reset from IDD
  * @retval Reset status
  */
int Get_Reset_From_IDD_Measure(void)
{
  return Reset_From_IDD_Measure;
}


/**
  * @brief  Callback routine of transparent motion window which is responsible for processing the motion messages.
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbMotion(WM_MESSAGE * pMsg) 
{
  WM_MOTION_INFO * pInfo;
  PARA           * pPara;
  const GUI_PID_STATE * pState;
  static GUI_TIMER_TIME current_time = 0;
  int dx = 0;
  int dy = 0;
  static int xPos      = 0;
  static int yPos      = 0;
  static int doubleTap = 0;
  static int isMoving  = 0;

  switch (pMsg->MsgId) 
  {
    case WM_MOTION:
      WM_GetUserData(pMsg->hWin, &pPara, sizeof(pPara));
      pInfo = (WM_MOTION_INFO *)pMsg->Data.p;

      switch (pInfo->Cmd) 
      {
        case WM_MOTION_INIT:
          pInfo->Flags = WM_CF_MOTION_X | WM_MOTION_MANAGE_BY_WINDOW;
          pInfo->SnapX = 390;
          break;

        case WM_MOTION_MOVE:
          if (doubleTap == 0)
          {
            isMoving  = 1;

            pPara->FinalMove = pInfo->FinalMove;
            pPara->Pos += pInfo->dx;

            if ((FirstModuleX == 0) && (pPara->Pos > 0))
            {
              pInfo->SnapX = 3900;
              WM_MoveWindow(hclockmain, (pInfo->dx), 0);
              WM_MoveWindow(hAudioPlayer, (pInfo->dx), 0);
              WM_MoveWindow(hAudioRecorder, (pInfo->dx), 0);
              WM_MoveWindow(hPowerMeasurement, (pInfo->dx), 0);
              WM_MoveWindow(hInformation, (pInfo->dx), 0);
              WM_MoveWindow(hResetScreen, (pInfo->dx), 0);
              
              if (pPara->Pos > 60)
              {
                pInfo->StopMotion = 1;
              }
            }
            else if((FirstModuleX == (-5 * 390)) && (pPara->Pos < 0))
            {
              pInfo->SnapX = 3900;
              WM_MoveWindow(hclockmain, (pInfo->dx), 0);
              WM_MoveWindow(hAudioPlayer, (pInfo->dx), 0);
              WM_MoveWindow(hAudioRecorder, (pInfo->dx), 0);
              WM_MoveWindow(hPowerMeasurement, (pInfo->dx), 0);
              WM_MoveWindow(hInformation, (pInfo->dx), 0);
              WM_MoveWindow(hResetScreen, (pInfo->dx), 0);

              if (pPara->Pos < -60)
              {
                pInfo->StopMotion = 1;
              }
            }
            else
            {
              pInfo->SnapX = 390;
              WM_MoveWindow(hclockmain, (pInfo->dx), 0);
              WM_MoveWindow(hAudioPlayer, (pInfo->dx), 0);
              WM_MoveWindow(hAudioRecorder, (pInfo->dx), 0);
              WM_MoveWindow(hPowerMeasurement, (pInfo->dx), 0);
              WM_MoveWindow(hInformation, (pInfo->dx), 0);
              WM_MoveWindow(hResetScreen, (pInfo->dx), 0); 
            }
            if (pPara->Pos >= 390)
            {
              pInfo->StopMotion = 1;
            }
            if (pPara->Pos < -(390))
            {
              pInfo->StopMotion = 1;
            }
            if (pPara->FinalMove == 1)
            {
              FirstModuleX = WM_GetWindowOrgX(hclockmain);
              pPara->Pos = 0;

              if (FirstModuleX == 0)
              {
                FirstTime = 0;
              }

              isMoving  = 0;
              doubleTap = 0;
            }

            WM_Invalidate(WM_GetParent(pMsg->hWin));
          }
          break;

        case WM_MOTION_GETPOS:
          pInfo->xPos = pPara->Pos;
          break;
      }
    break;

    case WM_TOUCH:
      if (isMoving == 0)
      {
        pState = (const GUI_PID_STATE *)pMsg->Data.p;
        if (pState) 
        {
          if (pState->Pressed)
          {
            /* Reset the last position if the user takes too long between two taps */
            if (GUI_GetTime() - current_time > 1000)
            {
              xPos = 0;
              yPos = 0;
            }

            if (xPos > pState->x)
            {
              dx = xPos - pState->x;
            }
            else
            {
              dx = pState->x - xPos;
            }

            if (yPos > pState->y)
            {
              dy = yPos - pState->y;
            }
            else
            {
              dy = pState->y - yPos;
            }
            
            if ((GUI_GetTime() - current_time < 700) && (dx < 20) && (dy < 20) && (isMoving == 0))
            {
              doubleTap = 1;
              WM_HideWindow(hMotion);
            }
            else
            {
              doubleTap = 0;
            }

            xPos = pState->x;
            yPos = pState->y;

            current_time = GUI_GetTime();
          }
        }
      }
      break;
  }
}

/**
  * @brief  Creates and executes the main menu.
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
void k_InitMenu(void) 
{
  static PARA       Para;
  PARA            * pPara;
  
  /* Initialize parameter structure for items to be shown */
  pPara              = &Para;
  pPara->Pos         = 0;
  
  /* Enable motion support */
  WM_MOTION_Enable(1);
  
  /* Set the buttons to react only on level changes, only.*/
  BUTTON_SetReactOnLevel();

  /* Create the main window */
  hMainWin = WM_HBKWIN;
  WM_SetCallback(hMainWin, _cbBk); 

  module_prop[0].module->startup(hMainWin, 0, 0);
  module_prop[1].module->startup(hMainWin, 390, 0);
  module_prop[2].module->startup(hMainWin, 390 * 2, 0);
  module_prop[3].module->startup(hMainWin, 390 * 3, 0);
  module_prop[4].module->startup(hMainWin, 390 * 4, 0);
  module_prop[5].module->startup(hMainWin, 390 * 5, 0);

  hMotion = WM_CreateWindowAsChild(0, 0, 390, 390, hMainWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbMotion, sizeof(pPara));

  /* Go back to the IDD measurement module to display the measure value */
  if (Reset_From_IDD_Measure == 1)
  {
    FirstModuleX = -3 * 390;

    WM_SetWindowPos(hclockmain,        390 * -3, 0, 390, 390);
    WM_SetWindowPos(hAudioPlayer,      390 * -2, 0, 390, 390);
    WM_SetWindowPos(hAudioRecorder,    390 * -1, 0, 390, 390);
    WM_SetWindowPos(hPowerMeasurement, 390 *  0, 0, 390, 390);
    WM_SetWindowPos(hInformation,      390 *  1, 0, 390, 390);
    WM_SetWindowPos(hResetScreen,      390 *  2, 0, 390, 390);

    WM_HideWindow(hMotion);
  }

  WM_SetUserData(hMainWin, &pPara, sizeof(pPara));
  WM_SetUserData(hMotion, &pPara, sizeof(pPara));
}

