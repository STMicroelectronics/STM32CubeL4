/**
  ******************************************************************************
  * @file    power_measurement_win.c
  * @author  MCD Application Team
  * @brief   power measurement functions
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
#include "Resources.h"
#include "power_measurement_res.h"
#include "iddmeasure.h"

/** @addtogroup CLOCK_FACES_MODULE
  * @{
  */

/** @defgroup CLOCK_FACES
  * @brief clock faces routines
  * @{
  */

/* External variables --------------------------------------------------------*/
WM_HWIN  hPowerMeasurement;

extern uint32_t iddTestIndex;

/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  power_measurement =
{
  3,
  "power measurement",
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
  ID_BUTTON_BACK = (GUI_ID_USER + 0x01),
  ID_BUTTON_LEFT,
  ID_BUTTON_RIGHT,
  ID_BUTTON_RUN,

  /* Texts */
  ID_TEXT_TITLE,
};

static GUI_CONST_STORAGE GUI_BITMAP *PowerModesIcons[] = 
{
  &bmrun_mode,
  &bmsleep_mode,
  &bmlow_power_mode,
  &bmlow_power_sleep_mode,
  &bmstop_mode,
  &bmstandby_mode,
  &bmshutdown,
};

static GUI_CONST_STORAGE GUI_BITMAP *PowerModesTexts[] = 
{
  &bmrun_mode_text,
  &bmsleep_mode_text,
  &bmlow_power_run_mode_text,
  &bmlow_power_sleep_mode_text,
  &bmstop_2_mode_text,
#ifdef USE_STM32L4R9I_EVAL
  &bmstandby_mode_text,
  &bmshutdown_mode_text,
#else
  &bmstandby_mode_rtc_text,
  &bmshutdown_mode_rtc_text,
#endif
};

static GUI_CONST_STORAGE GUI_BITMAP *AnimationCircle1[] = 
{
  &bmanimation_circle1_opacity10,
  &bmanimation_circle1_opacity20,
  &bmanimation_circle1_opacity30,
  &bmanimation_circle1_opacity40,
  &bmanimation_circle1_opacity50,
  &bmanimation_circle1_opacity60,
  &bmanimation_circle1_opacity70,
  &bmanimation_circle1_opacity80,
  &bmanimation_circle1_opacity90,
  &bmanimation_circle1_opacity100,
};

static GUI_CONST_STORAGE GUI_BITMAP *AnimationCircle2[] = 
{
  &bmanimation_circle2_opacity100,
  &bmanimation_circle2_opacity90,
  &bmanimation_circle2_opacity80,
  &bmanimation_circle2_opacity70,
  &bmanimation_circle2_opacity60,
  &bmanimation_circle2_opacity50,
  &bmanimation_circle2_opacity40,
  &bmanimation_circle2_opacity30,
  &bmanimation_circle2_opacity20,
  &bmanimation_circle2_opacity10,
};

static unsigned int PowerModeId      = 0;
static unsigned int RunButtonPressed = 0;
static unsigned int WindowIsMoving   = 0;

static WM_HTIMER hTimerTime;

static uint8_t MeasurementStr[IDD_VALUE_STRING_SIZE + IDD_UNIT_STRING_SIZE + 5];

static unsigned int AnimationDuration = 0;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/

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
  * @brief  Paints left button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_ButtonLeft(BUTTON_Handle hObj)
{
  if (GetMotionWindowVisibility() == 0)
  {
    if (PowerModeId == 0)
    {
      GUI_DrawBitmap(&bmarrow_left_gray, 10, 31);
    }
    else
    {
      GUI_DrawBitmap(&bmarrow_left, 10, 31);
    }
  }
}

/**
  * @brief  callback for left button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButtonLeft(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_ButtonLeft(pMsg->hWin);
      break;

    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints right button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_ButtonRight(BUTTON_Handle hObj)
{
  if (GetMotionWindowVisibility() == 0)
  {
    if (PowerModeId == 6)
    {
      GUI_DrawBitmap(&bmarrow_right_gray, 37, 31);
    }
    else
    {
      GUI_DrawBitmap(&bmarrow_right, 37, 31);
    }
  }
}

/**
  * @brief  callback for right button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButtonRight(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_ButtonRight(pMsg->hWin);
      break;

    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints run button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_ButtonRun(BUTTON_Handle hObj)
{
  if (GetMotionWindowVisibility() == 0)
  {
    if (RunButtonPressed == 0)
    {
      GUI_DrawBitmap(&bmbutton_run_unselected, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmbutton_run_selected, 0, 0);
    }
  }
}

/**
  * @brief  callback for run button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButtonRun(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
       _OnPaint_ButtonRun(pMsg->hWin);
      break;

    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Starts the measurements
  * @retval None
  */
static void StartMeasurement(void)
{
  IddMeasureStart();

  RunButtonPressed = 0;
}

/**
  * @brief  Prints the measurements
  * @retval None
  */
static void PrintMeasurement(void)
{
  Iddvalue_t idd;

  IddMeasureGetValue(&idd);
  sprintf((char *)MeasurementStr, "%s %s", idd.value, idd.unit);
}


/**
  * @brief  callback for Clocks drawing
  * @param  pMsg: Pointer to Date structure
  * @retval None
  */
static void _cbPowerMeasurement(WM_MESSAGE * pMsg)
{
  int Id, NCode;
  WM_HWIN hItem;
  static unsigned int animationNum = 0;
  static int animationDirection    = 1;

  switch (pMsg->MsgId)
  {
    case WM_CREATE:
      sprintf((char *)MeasurementStr, "%s", "--");

      hItem = TEXT_CreateEx(0, 25, 390, 40, pMsg->hWin, WM_CF_HIDE, 0, ID_TEXT_TITLE, "Measure Now !");
      TEXT_SetFont(hItem, &GUI_FontRobotoC_bold22);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER);

      hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 100, 0);
      break;

    case WM_DELETE:
      WM_DeleteTimer(hTimerTime);
      break;

    case WM_TIMER:
      if ((RunButtonPressed == 1) && (AnimationDuration < 20))
      {
        animationNum = animationNum + (1 * animationDirection);

        if (animationNum == 0)
        {
          animationDirection = 1;
        }

        if (animationNum == 9)
        {
          animationDirection = -1;
        }

        AnimationDuration++;
      }
      else
      {
        AnimationDuration = 0;

        if (RunButtonPressed == 1)
        {
          StartMeasurement();
        }
      }

      WM_RestartTimer(pMsg->Data.v, 100);
      WM_InvalidateWindow(pMsg->hWin);
      break;

    case WM_PAINT:
      if (GetMotionWindowVisibility() == 0)
      {
        GUI_DrawBitmap(&bmpower_measurement_background, 0, 0);
        GUI_DrawBitmap(PowerModesIcons[PowerModeId], 173, 20);
        GUI_DrawBitmap(PowerModesTexts[PowerModeId], ((390 - PowerModesTexts[PowerModeId]->XSize) / 2), 80);
        GUI_DrawBitmap(&bmbutton_run_cercle, 122, 195);

        if (RunButtonPressed == 1)
        {
          GUI_DrawBitmap(AnimationCircle1[animationNum], 115, 188);
          GUI_DrawBitmap(AnimationCircle2[animationNum], 108, 181);
        }

        GUI_SetTextMode(GUI_TM_TRANS);
        GUI_SetFont(&GUI_FontRoboto_C_Regular35);
        GUI_SetColor(GUI_WHITE);
        GUI_DispStringHCenterAt((char *)MeasurementStr, 200, 120);

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
        GUI_DrawBitmap(&bmpower_icon, (390 - bmpower_icon.XSize) / 2, (390 - bmpower_icon.YSize) / 2);
      }

      break;

    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      NCode = pMsg->Data.v;               /* Notification code */

      switch (NCode) 
      {
        case WM_NOTIFICATION_CLICKED:
          if (Id == ID_BUTTON_BACK)
          {
            RunButtonPressed = 0;
            sprintf((char *)MeasurementStr, "%s", "--");

            BackToMainScreen();
          }

          if (Id == ID_BUTTON_LEFT)
          {
            if (RunButtonPressed == 0)
            {
              if (PowerModeId > 0)
              {
                PowerModeId--;
                iddTestIndex--;
				sprintf((char *)MeasurementStr, "%s", "--");
              }
            }
          }

          if (Id == ID_BUTTON_RIGHT)
          {
            if (RunButtonPressed == 0)
            {
              if (PowerModeId < 6)
              {
                PowerModeId++;
                iddTestIndex++;
				sprintf((char *)MeasurementStr, "%s", "--");
              }
            }
          }

          if (Id == ID_BUTTON_RUN)
          {
            RunButtonPressed = 1;
            sprintf((char *)MeasurementStr, "%s", "--");
          }
          break;

        case WM_NOTIFICATION_RELEASED:
          break;
      }
      break;

      case WM_MOVE:
        if ((WM_GetWindowOrgX(pMsg->hWin) % 390) == 0)
        {
          WindowIsMoving = 0;
        }
        else
        {
          WindowIsMoving = 1;
        }
        WM_DefaultProc(pMsg);
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
  BUTTON_Handle hButtonLeft;
  BUTTON_Handle hButtonRight;
  BUTTON_Handle hButtonRun;

  hPowerMeasurement = WM_CreateWindowAsChild(xpos, ypos, 390, 390, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, _cbPowerMeasurement, 0);

  /* Create buttond to be used */
  hButtonBack = BUTTON_CreateEx(90, 335, 210, 55, hPowerMeasurement, WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_STAYONTOP, 0 , ID_BUTTON_BACK);
  WM_SetCallback(hButtonBack, _cbButtonBack);

  hButtonLeft = BUTTON_CreateEx(0, 154, 65, 78, hPowerMeasurement, WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_STAYONTOP, 0 , ID_BUTTON_LEFT);
  WM_SetCallback(hButtonLeft, _cbButtonLeft);

  hButtonRight = BUTTON_CreateEx(330, 154, 65, 78, hPowerMeasurement, WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_STAYONTOP, 0 , ID_BUTTON_RIGHT);
  WM_SetCallback(hButtonRight, _cbButtonRight);

  hButtonRun = BUTTON_CreateEx(145, 245, 99, 49, hPowerMeasurement, WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_STAYONTOP, 0 , ID_BUTTON_RUN);
  WM_SetCallback(hButtonRun, _cbButtonRun);

  if (Get_Reset_From_IDD_Measure() == 1)
  {
    PowerModeId = iddTestIndex;
    PrintMeasurement();
  }
}


/**
  * @}
  */

/**
  * @}
  */

