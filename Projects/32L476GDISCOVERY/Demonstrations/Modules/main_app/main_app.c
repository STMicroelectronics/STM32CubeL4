/**
  ******************************************************************************
  * @file    system_win.c
  * @author  MCD Application Team   
  * @brief   System information functions
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
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "iddmeasure.h"

/* Private function prototypes -----------------------------------------------*/
KMODULE_RETURN  AppMainPreExec(void);
KMODULE_RETURN  AppMainExec(void);
static void     AppMain_Option(void);
static void     AppMain_About(void);
KMODULE_RETURN  AppMainExecCheckResource(void);

/* Private Variable ----------------------------------------------------------*/
/* Option menu */
const tMenuItem OptionMenuItems[] =
{
    {"LPMODE"       , 0, 0,   SEL_EXEC, 0, AppMain_Option, NULL, NULL, NULL},
    {" ABOUT"       , 0, 0,   SEL_EXEC, 0, AppMain_About, NULL, NULL, NULL},
    {""             , 0, 0,   SEL_EXIT, 0, NULL, NULL, NULL, NULL}
 };

const tMenu OptionMenu = {
  "", OptionMenuItems, countof(OptionMenuItems), TYPE_TEXT, 1, 0
  };

/* Main Menu */
const tMenuItem ConstMainMenuItems[] =
{
    {"  IDD "       ,0,0,   SEL_MODULE,  MODULE_IDDMEASURE    , NULL, NULL, NULL, NULL},
    {"  VDD "       ,0,0,   SEL_MODULE,  MODULE_BATTERYMEASURE, NULL, NULL, NULL, NULL},
    {"RECORD"       ,0,0,   SEL_MODULE,  MODULE_AUDIORECORDER , NULL, NULL, NULL, NULL},
    {"PLAYER"       ,0,0,   SEL_MODULE,  MODULE_AUDIOPLAYER   , NULL, NULL, NULL, NULL},
    {"COMPAS"       ,0,0,   SEL_MODULE,  MODULE_COMPASS       , NULL, NULL, NULL, NULL},
    {" SOUND"       ,0,0,   SEL_MODULE,  MODULE_SOUNDMETER    , NULL, NULL, NULL, NULL},
    {"GUITAR"       ,0,0,   SEL_MODULE,  MODULE_GUITARTUNER   , NULL, NULL, NULL, NULL},
    {"OPTION"       ,0,0,   SEL_SUBMENU, MODULE_NONE          , NULL, NULL, (const tMenu*)&OptionMenu, NULL },
    {""             ,0,0,   SEL_NOTHING, MODULE_NONE          , NULL, NULL, NULL, NULL}
};

tMenuItem MainMenuItems[countof(ConstMainMenuItems)] =
{ 0 };

uint32_t MainMenuItemsNb = 0;

tMenu MainMenu = {
  "", MainMenuItems, 0, TYPE_TEXT, 1, 0
  };

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleAppMain =
{
  MODULE_MAIN_APP,
  AppMainPreExec,
  AppMainExec,
  NULL,
  AppMainExecCheckResource
};  


/**
  * @brief  Prepare main menu 
  * @param  None.
  * @note   compute main menu.  
  * @retval None.
  */
KMODULE_RETURN AppMainPreExec(void)
{
  uint8_t i =0;

  for(i = 0; i < countof(ConstMainMenuItems); i++)
  {
    if((ConstMainMenuItems[i].ModuleId != MODULE_NONE) &&
      (kmodule_info[ConstMainMenuItems[i].ModuleId].kModuleId != MODULE_MAIN_APP))
    {
      MainMenuItems[MainMenuItemsNb] = ConstMainMenuItems[i];
      MainMenuItemsNb++;
    }
  }
  MainMenuItems[MainMenuItemsNb] = ConstMainMenuItems[7];
  MainMenuItemsNb++;
  MainMenuItems[MainMenuItemsNb] = ConstMainMenuItems[8];
  MainMenuItemsNb++;
  MainMenu.nItems = MainMenuItemsNb;

  for(i = 0; i < MODULE_MAX; i++)
  {
    if(kmodule_info[i].kModuleId != MODULE_MAIN_APP)
    {
      if(kmodule_info[i].kModulePreExec != NULL)
      {
        kmodule_info[i].kModulePreExec();
      }
      if(kmodule_info[i].kModulePostExec != NULL)
      {
        kmodule_info[i].kModulePostExec();
      }
    }
  }

  /* Reset low power variable & notify Demo is started */
  DemoEnterLowPower = RESET;
  DemoStarted = SET;

  return KMODULE_OK;
}

/**
  * @brief  Run the main application 
  * @param  None.
  * @note   run and display main menu.  
  * @retval None.
  */
KMODULE_RETURN AppMainExec(void)
{
  kMenu_Execute(MainMenu);
  return KMODULE_OK;
}

/**
  * @brief  Run the option menu information 
  * @param  None.
  * @note   display About information.  
  * @retval None.
  */
static void AppMain_Option(void)
{
  uint8_t index = 0;
  uint8_t max_index = 1;
  JOYState_TypeDef key = JOY_DOWN;
  SettingsBackupData_TypeDef settings;
  
  /* Display demo information depending Supply mode*/
  if(PowerSupplyMode == SUPPLY_MODE_BATTERY)
  {
    max_index = 0;   /* STOP2 only */
  }
  BSP_LCD_GLASS_ScrollSentence((uint8_t *) "      CHOOSE LOW POWER OPTION", 1, SCROLL_SPEED_HIGH);
  
  do
  {
    if((key == JOY_DOWN) || (key == JOY_UP))
    {
      BSP_LCD_GLASS_Clear();
      switch(index)
      {
        case 0:
          BSP_LCD_GLASS_DisplayString((uint8_t *)" STOP2");
          break;

        case 1:
          BSP_LCD_GLASS_DisplayString((uint8_t *)"DISABLE");
          break;
       }
    }
    
    key = kMenu_GetEvent(10);
    
    switch(key)
    {
    case JOY_DOWN:
      index++;
      if(index > max_index)
      {
        index = 0;
      }
      break;
      
    case JOY_UP:
      if(index == 0)
      {
        index = max_index;
      }
      else
      {
        index--;
      }
      break;
      
    default:
      break;
    }
    
  } while((key != JOY_SEL) && (key != JOY_RIGHT) && (key != JOY_LEFT));
      
  if(key != JOY_LEFT)
  {
    BSP_LCD_GLASS_Clear();
    switch(index)
    {
      case 0:
        DemoULPMode = ULP_STOP2;
        EnterLowPowerMode = EnterStop2Mode;
        ExitLowPowerMode = ExitStop2Mode;
        break;

      case 1:
        DemoULPMode = ULP_OFF;
        EnterLowPowerMode = NULL;
        ExitLowPowerMode = NULL;
        break;
    }

    settings.lpmode = (uint32_t) DemoULPMode;
    SystemBackupWrite(BACKUP_SETTINGS, (void *)&settings);
  }
}

/**
  * @brief  Run the About information 
  * @param  None.
  * @note   display About information.  
  * @retval None.
  */
static void AppMain_About(void)
{
  uint8_t mfxversion[countof(MFX_INFO)] = MFX_INFO;

  mfxversion[countof(MFX_INFO)-4] = ((uint8_t)(MfxFwVersion >> 8) + 0x30);
  mfxversion[countof(MFX_INFO)-2] = (uint8_t) ((MfxFwVersion & 0xFF) + 0x30);

  /* Display demo information */
  BSP_LCD_GLASS_ScrollSentence((uint8_t *) DISCOVERY_BOARD, 1, SCROLL_SPEED_HIGH);
  BSP_LCD_GLASS_ScrollSentence((uint8_t *) DEMO_VERSION, 1, SCROLL_SPEED_HIGH);
  BSP_LCD_GLASS_ScrollSentence((uint8_t *) mfxversion, 1, SCROLL_SPEED_HIGH);
  BSP_LCD_GLASS_ScrollSentence((uint8_t *) DEMO_DATE, 1, SCROLL_SPEED_HIGH);
  BSP_LCD_GLASS_ScrollSentence((uint8_t *) DEMO_INFO1, 1, SCROLL_SPEED_HIGH);
  BSP_LCD_GLASS_ScrollSentence((uint8_t *) DEMO_INFO2, 1, SCROLL_SPEED_HIGH);
}

/**
  * @brief  Check the main application resources.
  * @param  None.
  * @note   None.  
  * @retval None.
  */
KMODULE_RETURN AppMainExecCheckResource(void)
{
  return KMODULE_OK;
}

/**
  * @}
  */

/**
  * @}
  */


