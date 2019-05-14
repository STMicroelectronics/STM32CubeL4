#ifndef MENULAUNCHER_H__
#define MENULAUNCHER_H__

/*********************************************************************
*
*       Includes
*
**********************************************************************
*/
#include <stdlib.h>

#include "GUI.h"

/*********************************************************************
*
*       Exported constants
*
**********************************************************************
*/
#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

/*********************************************************************
*
*       Exported variables
*
**********************************************************************
*/
extern GUI_CONST_STORAGE GUI_BITMAP bmSTemWin;
extern GUI_CONST_STORAGE GUI_BITMAP bmEmbeddedWizard;
extern GUI_CONST_STORAGE GUI_BITMAP bmTouchGFX;
extern GUI_CONST_STORAGE GUI_BITMAP bmbackground;
extern GUI_CONST_STORAGE GUI_BITMAP bminformation;
extern GUI_CONST_STORAGE GUI_BITMAP bmquestion;
extern GUI_CONST_STORAGE GUI_BITMAP bmquestion_sep;
extern GUI_CONST_STORAGE GUI_BITMAP bmSTLogo;

extern GUI_CONST_STORAGE GUI_FONT GUI_FontLucidaSansUnicode25;

#endif  /* Avoid multiple inclusion */
/*************************** End of file ****************************/
