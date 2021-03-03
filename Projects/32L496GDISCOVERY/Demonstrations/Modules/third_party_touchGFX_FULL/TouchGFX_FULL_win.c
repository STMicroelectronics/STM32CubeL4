/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 2008         SEGGER Microcontroller Systeme GmbH        *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

**** emWin/GSC Grafical user interface for embedded applications ****
emWin is protected by international copyright laws. Knowledge of the
source code may not be used to write a similar product. This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : MOTION_RadialMenu.c
Purpose     : Shows how to create a radial menu with motion support
----------------------------------------------------------------------
*/

/*********************************************************************
*
*       Includes
*
**********************************************************************
*/
#include "DIALOG.h"
#include "k_module.h"
#include "TouchGFX_FULL_res.c"
#include "main.h"

#define ID_TGFX_FRAMEWIN_0                   (GUI_ID_USER + 0x01)
#define ID_TGFX_FRAMEWIN_1                   (GUI_ID_USER + 0x02)
#define ID_TGFX_FRAMEWIN_2                   (GUI_ID_USER + 0x03)
#define ID_TGFX_BUTTON_OK_MESSAGE_0          (GUI_ID_USER + 0x04)
#define ID_TGFX_BUTTON_OK_MESSAGE_1          (GUI_ID_USER + 0x05)
#define ID_TGFX_BUTTON_OK_MESSAGE_2          (GUI_ID_USER + 0x06)
#define ID_TGFX_TEXT_FILENAME_0              (GUI_ID_USER + 0x07)
#define ID_TGFX_TEXT_FILENAME_1              (GUI_ID_USER + 0x08)
#define ID_TGFX_TEXT_FILENAME_2              (GUI_ID_USER + 0x09)
#define ID_TGFX_TEXT_FILENAME_3              (GUI_ID_USER + 0x10)
#define ID_TGFX_TEXT_FILENAME_4              (GUI_ID_USER + 0x11)
#define ID_TGFX_PROGBAR_ID                   (GUI_ID_USER + 0x12)
#define ID_TEXT1                             (GUI_ID_USER + 0x13)
#define ID_TEXT2                             (GUI_ID_USER + 0x14)
#define ID_TEXT3                             (GUI_ID_USER + 0x15)

#define TGFX_FULL_DEMO_REVISION              0xF1010001 /* 0xAAXXYYZZ - 0xAA : Demo ID | 0xXXYYZZ : MajorMinorPatch Version number */
#define DEMO_TGFX_FULL_ADDRESS               0x080C0000
#define DEMO_TGFX_FULL_RES_ADDRESS           0x90140000
#define DEMO_TGFX_HEADER_ADDRESS_OFFSET      0x200
#define DEMO_TGFX_HEADER_ADDRESS_OFFSET_QSPI 0x0035A000

extern GUI_CONST_STORAGE GUI_BITMAP bmwarning;
extern const unsigned char Out_of_Box_TouchGFXDemo[]; 

/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, U16 xpos, U16 ypos);
static int _ShowMessageBox(WM_HWIN hWin, const char* pTitle, const char* pText);

/*********************************************************************
*
*       Public routines
*
**********************************************************************/
static void Startup(WM_HWIN hWin, U16 xpos, U16 ypos);

K_ModuleItem_Typedef  TGFX_FULL_board =
{  
  4,
  (uint8_t*)"",
  &bmtgfxfull,
  Startup,
}
;

 /* No Filename available on SD Card Dialog Create */
static const GUI_WIDGET_CREATE_INFO _aNoFileDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "WARNING", ID_TGFX_FRAMEWIN_0, 0, 0, 200, 200, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Please Add binary files", ID_TGFX_TEXT_FILENAME_0, 5, 75, 180, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "For TGFX Full demo", ID_TGFX_TEXT_FILENAME_1, 30, 95, 150, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OK", ID_TGFX_BUTTON_OK_MESSAGE_0, 60, 120, 70, 70, 0, 0x0, 0 },
};

 /* No SD Card Detected Dialog Create */
static const GUI_WIDGET_CREATE_INFO _aNoSdCardDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "WARNING", ID_TGFX_FRAMEWIN_1, 0, 0, 200, 200, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Please insert SDCard", ID_TGFX_TEXT_FILENAME_2, 30, 80, 150, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OK", ID_TGFX_BUTTON_OK_MESSAGE_1, 65, 120, 70, 70, 0, 0x0, 0 },
};

 /* Invalid Binary file Dialog Create */
static const GUI_WIDGET_CREATE_INFO _aInvalidFileDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "WARNING", ID_TGFX_FRAMEWIN_2, 0, 0, 200, 200, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Invalid binary files", ID_TGFX_TEXT_FILENAME_3, 32, 80, 180, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "For TGFX Full demo", ID_TGFX_TEXT_FILENAME_4, 15, 95, 180, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OK", ID_TGFX_BUTTON_OK_MESSAGE_2, 65, 120, 70, 70, 0, 0x0, 0 },
};

static FIL TGFX_file;      /* File object */

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       _cbDialog
*
* Purpose
*   Callback routine of the dialog
*/

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
* @brief  Paints yes message button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_yesMessage(BUTTON_Handle hObj) {
  
  GUI_SetBkColor(GUI_WHITE);
  GUI_Clear();
  
  if ((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_SetColor(GUI_DARKGRAY);
    GUI_AA_FillCircle(0, 50, 50);
    GUI_SetBkColor(GUI_DARKGRAY);
    GUI_SetColor(GUI_WHITE);
  }
  else
  {
    GUI_SetColor(0x00DCA939);
    GUI_AA_FillCircle(0, 50, 50);
    GUI_SetBkColor(0x00DCA939);
    GUI_SetColor(GUI_WHITE);
  }
  
  GUI_SetFont(&GUI_Font20B_ASCII);
  GUI_DispStringAt("YES", 3, 20);
}

/**
* @brief  callback for yes message button
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbButton_yesMessage(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
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
static void _OnPaint_noMessage(BUTTON_Handle hObj) {
  
  GUI_SetBkColor(GUI_WHITE);
  GUI_Clear();
  
  if ((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_SetColor(GUI_DARKGRAY);
    GUI_AA_FillCircle(50, 50, 50);
    GUI_SetBkColor(GUI_DARKGRAY);
    GUI_SetColor(GUI_WHITE);
  }
  else
  {
    GUI_SetColor(0x00DCA939);
    GUI_AA_FillCircle(50, 50, 50);
    GUI_SetBkColor(0x00DCA939);
    GUI_SetColor(GUI_WHITE);
  }
  
  GUI_SetFont(&GUI_Font20B_ASCII);
  GUI_DispStringAt("NO", 15, 20);
}

/**
* @brief  callback for no message button
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbButton_noMessage(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
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
static void _cbDialogMessage(WM_MESSAGE * pMsg) {
  int     NCode;
  int     Id;
  WM_HWIN hItem;
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TGFX_BUTTON_OK_MESSAGE_0);
    WM_SetCallback(hItem, _cbButton_okMessage);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TGFX_BUTTON_OK_MESSAGE_1);
    WM_SetCallback(hItem, _cbButton_okMessage);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TGFX_BUTTON_OK_MESSAGE_2);
    WM_SetCallback(hItem, _cbButton_okMessage);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TGFX_TEXT_FILENAME_0);
    TEXT_SetFont(hItem, &GUI_Font16B_ASCII);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TGFX_TEXT_FILENAME_1);
    TEXT_SetFont(hItem, &GUI_Font16B_ASCII);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TGFX_TEXT_FILENAME_2);
    TEXT_SetFont(hItem, &GUI_Font16B_ASCII);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TGFX_TEXT_FILENAME_3);
    TEXT_SetFont(hItem, &GUI_Font16B_ASCII);
    break;
        
  case WM_PAINT:    
    GUI_DrawBitmap(&bmwarning, 65, 10);
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_TGFX_BUTTON_OK_MESSAGE_0: /* Notifications sent by 'OK' */
    case ID_TGFX_BUTTON_OK_MESSAGE_1: /* Notifications sent by 'OK' */
    case ID_TGFX_BUTTON_OK_MESSAGE_2: /* Notifications sent by 'OK' */
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
  * @brief  DEMO_Starup
  * @param  None
  * @retval None
  */
static void Startup(WM_HWIN hWin, U16 xpos, U16 ypos)
{
  WM_HWIN hItem;
  Demo_Header_t * pQspiDemoHeader1 = (Demo_Header_t *)DEMO_TGFX_FULL_RES_ADDRESS;
  Demo_Header_t * pQspiDemoHeader2 = (Demo_Header_t *)(DEMO_TGFX_FULL_RES_ADDRESS + DEMO_TGFX_HEADER_ADDRESS_OFFSET_QSPI);
  Demo_Header_t * pDemoDemoHeader = (Demo_Header_t *)(DEMO_TGFX_FULL_ADDRESS + DEMO_TGFX_HEADER_ADDRESS_OFFSET);
  Demo_Header_t   SdDemoHeader    = (Demo_Header_t  ){0};
  FRESULT result = FR_NOT_READY;
  uint32_t numOfReadBytes;
  int card_detected = BSP_SD_IsDetected();

  /* First Check if the current flashed demo is the Lite version
   * If yes then check QSPI resources version.
   */
  if (pDemoDemoHeader->demo_id == TGFX_FULL_DEMO_REVISION)
  {
    /* Now Check if the current flashed demo version is matching flashed resources
     * in the QSPI.
     * If yes then immediately jump to the third party demonstration.
     */
    if ((pQspiDemoHeader1->demo_id == pDemoDemoHeader->demo_id) &&
        (pQspiDemoHeader2->demo_id == pDemoDemoHeader->demo_id))
    {
      result = FR_OK;
    }
  }
  
  if((card_detected) && (result != FR_OK))
  {
    /* Check if the demo resources are loaded otherwise, 
     * notify the user that the expected duration is around 1mn 45s
     */     
    if ((pQspiDemoHeader1->demo_id != TGFX_FULL_DEMO_REVISION) ||
        (pQspiDemoHeader2->demo_id != TGFX_FULL_DEMO_REVISION))
    {      
      if(!_ShowMessageBox(hWin, "WARNING", " TouchGFX Full demo resources\ndownload, expected duration:\n 1 mn 45 sec.\n\nCarry on?"))
      {
        return;
      }
    }
    
    /* If the binary file in the SD card :
     * - SD card should be detected.
     * - Binary files should be placed at root with appropriate demo name.
     * - Demo Code and Resources should matches the require version (check demo ID - version matching).
     * - Start flashing Flash binary file to internal Flash if required.
     * - Start flashing Resources binary file to QSPI  if required.
     * - TODO : check flashing was OK or not using CRC and size of copied bytes.
     */

    /* Copy Flash code from SD card to Internal Flash memory */
    result = f_open(&TGFX_file, (const char*)"TGFX_Full_flash.bin", FA_READ);
    if(result == FR_OK)      
    {
      if (f_lseek(&TGFX_file, DEMO_TGFX_HEADER_ADDRESS_OFFSET) != FR_OK)
      {
        Error_Handler();
      }

      result = f_read(&TGFX_file, &SdDemoHeader, sizeof(Demo_Header_t), (void *)&numOfReadBytes);
      if(result == FR_OK)
      {
        /* Check if the current flashed demo version is matching the required demo
         * If yes then immediately jump to the QSPI resources loading.
         */
        if (pDemoDemoHeader->demo_id == TGFX_FULL_DEMO_REVISION)
        {
          result = FR_OK;
        }
        /* Check if the demo located in SD Card is matching the required demo
         * If yes then start flashing this demo into the internal flash.
         */
        else if (SdDemoHeader.demo_id == TGFX_FULL_DEMO_REVISION)
        {
          hItem = TEXT_CreateEx((20-WM_GetWindowOrgX(hWin)),  135,  190,  20, hWin, WM_CF_SHOW, 0, ID_TEXT1, "TGFX FLASH PROGRAMMING");
          TEXT_SetFont(hItem, &GUI_Font16B_ASCII); 
          TEXT_SetTextColor(hItem,GUI_BLACK); 
          TEXT_SetBkColor(hItem,GUI_WHITE);
          
          hItem = PROGBAR_CreateEx((40-WM_GetWindowOrgX(hWin)), 110, 150, 20, hWin, WM_CF_SHOW, 0, ID_TGFX_PROGBAR_ID);
          PROGBAR_SetMinMax(hItem, 0, f_size(&TGFX_file));
          WM_MakeModal(hItem); 
          
          k_FlashProgram(hItem, &TGFX_file, DEMO_TGFX_FULL_ADDRESS);
          
          hItem = WM_GetDialogItem(hWin, ID_TEXT1);
          WM_DeleteWindow(hItem);
        }
        else
        {
          /* Display error message: Invalid Demo Resources file */
          hItem = GUI_CreateDialogBox(_aInvalidFileDialogCreate, 
                                      GUI_COUNTOF(_aInvalidFileDialogCreate), 
                                      _cbDialogMessage, 
                                      hWin, 
                                      (20-WM_GetWindowOrgX(hWin)), 20);
          WM_MakeModal(hItem);
          result = FR_INVALID_OBJECT;
        }
      }
      f_close(&TGFX_file);      
    }
    else
    {
      hItem = GUI_CreateDialogBox(_aNoFileDialogCreate, 
                                  GUI_COUNTOF(_aNoFileDialogCreate), 
                                  _cbDialogMessage, 
                                  hWin, 
                                  (20-WM_GetWindowOrgX(hWin)), 20);
      WM_MakeModal(hItem);
    }

    /* Now Flush Resources from SD card to QSPI memory */
    result = f_open(&TGFX_file, (const char*)"TGFX_Full_resources.bin", FA_READ);
    if(result == FR_OK)      
    {
      if (f_lseek(&TGFX_file, DEMO_TGFX_HEADER_ADDRESS_OFFSET_QSPI) != FR_OK)
      {
        Error_Handler();
      }

      result = f_read(&TGFX_file, &SdDemoHeader, sizeof(Demo_Header_t), (void *)&numOfReadBytes);
      if(result == FR_OK)
      {
        /* Check if the current flashed demo version is matching the required demo
         * If yes then no thing to do we are done.
         */
        if ((pQspiDemoHeader1->demo_id == TGFX_FULL_DEMO_REVISION) &&
            (pQspiDemoHeader2->demo_id == TGFX_FULL_DEMO_REVISION))
        {
          result = FR_OK;
        }
        /* Check if the demo resources located in SD Card is matching the required demo
         * If yes then start flashing this demo into the internal flash.
         */
        else if (SdDemoHeader.demo_id == TGFX_FULL_DEMO_REVISION)
        {
          hItem = TEXT_CreateEx((50-WM_GetWindowOrgX(hWin)),  135,  130,  20, hWin, WM_CF_SHOW, 0, ID_TEXT2, " TGFX RESOURCES");
          TEXT_SetFont(hItem, &GUI_Font16B_ASCII); 
          TEXT_SetTextColor(hItem,GUI_BLACK); 
          TEXT_SetBkColor(hItem,GUI_WHITE);
          
          hItem = TEXT_CreateEx((50-WM_GetWindowOrgX(hWin)),  155,  130,  20, hWin, WM_CF_SHOW, 0, ID_TEXT3, "   PROGRAMMING  ");
          TEXT_SetFont(hItem, &GUI_Font16B_ASCII); 
          TEXT_SetTextColor(hItem,GUI_BLACK); 
          TEXT_SetBkColor(hItem,GUI_WHITE);          
          
          hItem = PROGBAR_CreateEx((40-WM_GetWindowOrgX(hWin)), 110, 150, 20, hWin, WM_CF_SHOW, 0, ID_TGFX_PROGBAR_ID);
          PROGBAR_SetMinMax(hItem, 0, f_size(&TGFX_file));
          WM_MakeModal(hItem);
          
          k_ResourcesCopy(hItem, &TGFX_file, DEMO_TGFX_FULL_RES_ADDRESS);
        }
        else
        {
          /* Display error message: Invalid Demo Resources file */
          hItem = GUI_CreateDialogBox(_aInvalidFileDialogCreate, 
                                      GUI_COUNTOF(_aInvalidFileDialogCreate), 
                                      _cbDialogMessage, 
                                      hWin, 
                                      (20-WM_GetWindowOrgX(hWin)), 20);
          WM_MakeModal(hItem);
          result = FR_INVALID_OBJECT;
        }
      }
      f_close(&TGFX_file);      
    }
    else
    {
      hItem = GUI_CreateDialogBox(_aNoFileDialogCreate, 
                                  GUI_COUNTOF(_aNoFileDialogCreate), 
                                  _cbDialogMessage, 
                                  hWin, 
                                  (20-WM_GetWindowOrgX(hWin)), 20);
      WM_MakeModal(hItem);
    }
  }
  else
  {
    hItem = GUI_CreateDialogBox(_aNoSdCardDialogCreate, 
                                GUI_COUNTOF(_aNoSdCardDialogCreate), 
                                _cbDialogMessage, 
                                hWin, 
                                (20-WM_GetWindowOrgX(hWin)), 20);
    WM_MakeModal(hItem); 
  }

  /* Jump to new Sub Demo if no errors */
  if(result == FR_OK)      
  {
    k_JumpToSubDemo(RTC->BKP30R, DEMO_TGFX_FULL_ADDRESS);
  }
}

/**
* @brief callback Message Box 
* @param  pMsg : pointer to data structure
* @retval None
*/
static void _cbMessageBox(WM_MESSAGE* pMsg) {
  WM_HWIN hWin;
  int Id;
  
  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
      Id = WM_GetId(pMsg->hWinSrc);
      if (Id == GUI_ID_OK)
      {
        GUI_EndDialog(hWin, 1);
      }
      else if (Id == GUI_ID_CANCEL)
      {
        GUI_EndDialog(hWin, 0);
      }  
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
static int _ShowMessageBox(WM_HWIN hWin, const char* pTitle, const char* pText) {
  WM_HWIN hFrame, hClient, hBut, htext;
  int r = 0;
  /* Create frame win */
  hFrame = FRAMEWIN_CreateEx(10, 10, 220, 200, hWin, WM_CF_SHOW, FRAMEWIN_CF_MOVEABLE, 0, pTitle, &_cbMessageBox);
  FRAMEWIN_SetClientColor(hFrame, GUI_WHITE);
  FRAMEWIN_SetFont(hFrame, &GUI_Font32B_ASCII);
  FRAMEWIN_SetTextAlign(hFrame, GUI_TA_HCENTER);
  /* Create dialog items */
  hClient = WM_GetClientWindow(hFrame);
  htext = TEXT_CreateEx(5, 7, 200, 100, hClient, WM_CF_SHOW, GUI_TA_HCENTER, 0, pText);
  TEXT_SetFont(htext, &GUI_Font16B_ASCII);
  
  hBut = BUTTON_CreateEx(160, 110, 50, 50, hClient, WM_CF_SHOW, 0, GUI_ID_CANCEL);
  WM_SetCallback(hBut, _cbButton_noMessage);
  
  hBut = BUTTON_CreateEx(0, 110, 50, 50, hClient, WM_CF_SHOW, 0, GUI_ID_OK);
  WM_SetCallback(hBut, _cbButton_yesMessage);
  
  WM_MakeModal(hFrame);
  r = GUI_ExecCreatedDialog(hFrame);
  return r;
}

/*************************** End of file ****************************/
