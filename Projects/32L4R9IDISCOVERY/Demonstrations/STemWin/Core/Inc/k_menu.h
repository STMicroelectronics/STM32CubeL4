#ifndef MENULAUNCHER_H__
#define MENULAUNCHER_H__

/*********************************************************************
*
*       Includes
*
**********************************************************************
*/
#include <stdlib.h>
#include "Resources.h"

/* Exported functions ------------------------------------------------------- */
void BackToMainScreen(void);
int GetMotionWindowVisibility(void);
void Set_Reset_From_IDD_Measure(int resetFromIDD);
int Get_Reset_From_IDD_Measure(void);

#endif  /* Avoid multiple inclusion */
/*************************** End of file ****************************/
