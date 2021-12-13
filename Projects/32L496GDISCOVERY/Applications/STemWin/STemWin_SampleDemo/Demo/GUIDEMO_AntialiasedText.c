/*********************************************************************
*          Portions COPYRIGHT 2013 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2013  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.22 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : AA_Text.c
Purpose     : Shows text with different antialiasing qualities
----------------------------------------------------------------------
*/

/**
  ******************************************************************************
  * @file    AA_Text.c
  * @author  MCD Application Team
  * @brief   Shows text with different antialiasing qualities
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

#include "GUIDEMO.h"

#if (SHOW_GUIDEMO_AATEXT)

/*********************************************************************
*
*       Static const data
*
**********************************************************************
*/
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _DrawAlphaCircles
*/
static void _DrawAlphaCircles(int mx, int my, int r, int a, int FactorAA) {
  int Index;
  int x;
  int y;
  I32 SinHQ;
  I32 CosHQ;
  U32 a1000;
  U32 i;
  const GUI_COLOR aColor[] = {
    0xC00FFF38,
    0xC000FF8E,
    0xC000FCEA,
    0xC000B4FF,
    0xC0004EFF,
    0xC01304FF,
    0xC06E00FF,
    0xC0D200FF,
    0xC0FF00D2,
    0xC0FF006E,
    0xC0FF0413,
    0xC0FF4E00,
    0xC0FFB400,
    0xC0EAFC00,
    0xC08EFF00,
    0xC038FF0F
  };

  mx *= FactorAA;
  my *= FactorAA;
  r  *= FactorAA;
  a1000 = a * -1000;
  for (i = 0, Index = 0; i < 360000; i += 22500, Index++) {
    SinHQ = GUI__SinHQ(i + a1000);
    CosHQ = GUI__CosHQ(i + a1000);
    x = (U32)(r * CosHQ) >> 16;
    y = (U32)(r * SinHQ) >> 16;
    GUI_SetColor(aColor[Index % (int)GUI_COUNTOF(aColor)]);
    GUI_AA_EnableHiRes();
    GUI_AA_SetFactor(FactorAA);
    GUI_AA_FillCircle(mx + x, my + y, r);
    GUI_AA_DisableHiRes();
  }
}

/*********************************************************************
*
*       _DrawSample
*/
static void _DrawSample(GUI_RECT * pRect, int yd) {
  GUI_RECT Rect;

  Rect = *pRect;
  //
  // Use application defined clip rectangle
  //
  GUI_SetClipRect(&Rect);
  //
  // Move clip rectangle
  //
  GUI_MoveRect(&Rect, 65, 0);
  //
  // Draw sample
  //
  _DrawAlphaCircles((Rect.x0 + Rect.x1) / 2, (Rect.y0 + Rect.y1) / 2, 35,   0, 4);
  GUI_SetColor(GUI_WHITE);
  GUI_DispStringInRectWrap("ABC\nABC\nABC", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER, GUI_WRAPMODE_WORD);
  //
  // Move clip rectangle
  //
  GUI_MoveRect(&Rect, 65, 0);

  GUI_DrawGradientH(Rect.x0, Rect.y0, Rect.x1, Rect.y1, GUI_BLACK, GUI_WHITE);
  Rect.y1 = Rect.y0 + yd;
  GUI_SetColor(GUI_RED);
  GUI_DispStringInRect("ABC", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
  GUI_MoveRect(&Rect, 0, yd);
  GUI_SetColor(GUI_GREEN);
  GUI_DispStringInRect("ABC", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
  GUI_MoveRect(&Rect, 0, yd);
  GUI_SetColor(GUI_BLUE);
  GUI_DispStringInRect("ABC", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
  GUI_MoveRect(&Rect, 0, -yd * 2);
  Rect.y1 = Rect.y0 + yd * 3;
  //
  // Move clip rectangle
  //
  GUI_MoveRect(&Rect, 65, 0);
  //
  // Draw sample
  //
  Rect.y1 = Rect.y0 + yd;
  GUI_SetBkColor(GUI_RED);
  GUI_Clear();
  GUI_MoveRect(&Rect, 0, yd);
  GUI_SetBkColor(GUI_GREEN);
  GUI_Clear();
  GUI_MoveRect(&Rect, 0, yd);
  GUI_SetBkColor(GUI_BLUE);
  GUI_Clear();
  GUI_MoveRect(&Rect, 0, -yd * 2);
  Rect.y1 = Rect.y0 + yd * 3;
  GUI_SetColor(GUI_WHITE);
  GUI_DispStringInRectWrap("ABC\nABC\nABC", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER, GUI_WRAPMODE_WORD);
  //
  // Move clip rectangle
  //
  GUI_MoveRect(&Rect, 65, 0);
  //
  // Draw sample
  //
  Rect.y1 = Rect.y0 + yd;
  GUI_DrawGradientV(Rect.x0, Rect.y0, Rect.x1, Rect.y1, GUI_RED,   GUI_BLACK);
  GUI_MoveRect(&Rect, 0, yd);
  GUI_DrawGradientV(Rect.x0, Rect.y0, Rect.x1, Rect.y1, GUI_GREEN, GUI_BLACK);
  GUI_MoveRect(&Rect, 0, yd);
  GUI_DrawGradientV(Rect.x0, Rect.y0, Rect.x1, Rect.y1, GUI_BLUE,  GUI_BLACK);
  GUI_MoveRect(&Rect, 0, -yd * 2);
  Rect.y1 = Rect.y0 + yd * 3;
  GUI_SetColor(GUI_WHITE);
  GUI_DispStringInRectWrap("ABC\nABC\nABC", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER, GUI_WRAPMODE_WORD);
  //
  // Disable application defined clip rectangle
  //
  GUI_SetClipRect(NULL);
}

/*********************************************************************
*
*       _DrawText
*/
static void _DrawText(char * s, GUI_RECT * pRect) {
  GUI_SetFont(GUI_FONT_13_ASCII);
  GUI_SetColor(GUI_WHITE);
  GUI_DispStringInRectWrap(s, pRect, GUI_TA_HCENTER | GUI_TA_VCENTER, GUI_WRAPMODE_WORD);
}

/*********************************************************************
*
*       _DrawScreen
*/
static void _DrawScreen(void) {
  GUI_RECT Rect;
  int      yd;

  GUI_SetFont(&GUI_FontRounded22);
  GUI_DispStringHCenterAt("Antialiased text sample", 190, 12);
  Rect.x0 = 0;
  Rect.y0 = 43;
  Rect.x1 = 59;
  Rect.y1 = Rect.y0 + 95;
  yd = (Rect.y1 - Rect.y0) / 3;
  //
  // 4 bit antialiasing sample
  //
  _DrawText("Antialiased text\n (4 bpp)", &Rect);
  GUI_SetFont(&GUI_FontAA4_32);
  _DrawSample(&Rect, yd);
  //
  // 2 bit antialiasing sample
  //
  GUI_MoveRect(&Rect, 0, 101);
  _DrawText("Antialiased text\n (2 bpp)", &Rect);
  GUI_SetFont(&GUI_FontAA2_32);
  _DrawSample(&Rect, yd);
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/
void GUIDEMO_AntialiasedText(void) {
  unsigned OldAlphaState;

  GUIDEMO_ShowIntro("Antialiased Text",
                    "Output antialiased text\n"
                    "on different backgrounds.");
  GUIDEMO_DrawBk(1);
  OldAlphaState = GUI_EnableAlpha(1);
  _DrawScreen();
  GUIDEMO_Wait(4000);
  GUI_EnableAlpha(OldAlphaState);
}

#else

void GUIDEMO_AntialiasedText(void) {}

#endif /* SHOW_GUIDEMO_AATEXT */

