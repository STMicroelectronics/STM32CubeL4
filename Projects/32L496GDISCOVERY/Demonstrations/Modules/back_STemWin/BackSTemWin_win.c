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
#include "BackSTemWin_res.c"
#include "main.h"

/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, U16 xpos, U16 ypos);

/*********************************************************************
*
*       Public routines
*
**********************************************************************/
static void Startup(WM_HWIN hWin, U16 xpos, U16 ypos);

K_ModuleItem_Typedef  return_board =
{  
  12,
  (uint8_t*)"",
  &bmback_STemWin,
  Startup,
}
;


/*********************************************************************
*
*       Static code
*
**********************************************************************
*/




/**
  * @brief  DEMO_Starup
  * @param  None
  * @retval None
  */
static void Startup(WM_HWIN hWin, U16 xpos, U16 ypos)
{
   
}
/*************************** End of file ****************************/
