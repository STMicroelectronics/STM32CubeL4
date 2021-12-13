/**
  ******************************************************************************
  * @file    reset_win.c
  * @author  MCD Application Team
  * @brief   reset window functions
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
#include "reset_res.h"

#include "DIALOG.h"


/** @addtogroup RESET_MODULE
  * @{
  */

/** @defgroup RESET_MODULE
  * @brief reset module routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  reset =
{
  5,
  "reset",
  0,
  Startup,
  NULL,
};

/* Private defines -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
enum
{
  /* Buttons */  
  ID_BUTTON_BACK  = (GUI_ID_USER + 0x01),
  ID_BUTTON_RESET,
  
  /* Texts */
  ID_TEXT_TITLE,
};

static unsigned int WindowIsMoving   = 0;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
WM_HWIN hResetScreen;

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
  * @brief  callback for reset button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButtonReset(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      break;

    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  principle window callback
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbResetScreen(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;

  switch (pMsg->MsgId) 
  {
    case WM_CREATE:
      hItem = TEXT_CreateEx(0, 25, 390, 40, pMsg->hWin, WM_CF_HIDE, 0, ID_TEXT_TITLE, "Reset Now !");
      TEXT_SetFont(hItem, &GUI_FontRobotoC_bold22);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER);
      break;

    case WM_PAINT:
      if (GetMotionWindowVisibility() == 0)
      {
        GUI_DrawBitmap(&bmreset_button, 0, 0);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE);
        WM_HideWindow(hItem);
      }
      else
      {
        if (WindowIsMoving == 0)
        {
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE);
          WM_ShowWindow(hItem);
        }
        else
        {
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE);
          WM_HideWindow(hItem);
        }

        GUI_DrawBitmap(&bmmodule_entry_icon, (LCD_GetXSize() - bmmodule_entry_icon.XSize) / 2, (LCD_GetYSize() - bmmodule_entry_icon.YSize) / 2);
        GUI_DrawBitmap(&bmreset_icon, (LCD_GetXSize() - bmreset_icon.XSize) / 2, ((LCD_GetYSize() - bmreset_icon.YSize) / 2) - 2);
      }
      break;

    case WM_NOTIFY_PARENT:
    {
      switch(WM_GetId(pMsg->hWinSrc))
      {
        /* Back button from the info window */
        case ID_BUTTON_BACK:
        {
          switch(pMsg->Data.v)
          {
            case WM_NOTIFICATION_CLICKED:
              BackToMainScreen();  
              break;
          }
        }
        break;
        /* Reset button from the info window */
        case ID_BUTTON_RESET:
        {
          switch(pMsg->Data.v)
          {
            case WM_NOTIFICATION_RELEASED:
              HAL_NVIC_SystemReset();
              break;
          }
        }
        break;        
      }
    }
    break;

    case WM_MOVE:
    {
      if ((WM_GetWindowOrgX(pMsg->hWin) % LCD_GetXSize()) == 0)
      {
        WindowIsMoving = 0;
      }
      else
      {
        WindowIsMoving = 1;
      }
      WM_DefaultProc(pMsg);
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
  int xSize;
  int ySize;
  BUTTON_Handle hButtonBack, hButtonReset;

  /* Get the LCD sizes */
  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize();

  hResetScreen       = WM_CreateWindowAsChild(xpos, ypos, xSize, ySize, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, _cbResetScreen, 0);

  /* Create buttons to be used */
  hButtonBack  = BUTTON_CreateEx(90, 335, 210, 55, hResetScreen, WM_CF_SHOW, 0 , ID_BUTTON_BACK);
  WM_SetCallback(hButtonBack, _cbButtonBack);

  hButtonReset = BUTTON_CreateEx((xSize - 220)/2, (xSize - 90)/2, 220, 90, hResetScreen, WM_CF_SHOW | WM_CF_STAYONTOP, 0 , ID_BUTTON_RESET);
  WM_SetCallback(hButtonReset, _cbButtonReset);
}


/**
  * @}
  */

/**
  * @}
  */

