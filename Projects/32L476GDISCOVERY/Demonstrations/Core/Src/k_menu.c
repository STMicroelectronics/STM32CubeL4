/**
  ******************************************************************************
  * @file    k_menu.c
  * @author  MCD Application Team   
  * @brief   This file provides the kernel menu functions 
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
#include "k_menu.h"
#include "k_module.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MENU
  * @brief Kernel menu routines
  * @{
  */

/* External variables --------------------------------------------------------*/
extern MODULES_INFO       DemoSelectedModuleId;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
enum {
  KMENU_HEADER,
  KMENU_TEXT,
  KMENU_EXEC,
  KMENU_WAITEVENT,
  KMENU_EXIT
};
  
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Variable used to forward an user event to an application */
static tExecAction kMenuEventForward = NULL;
osMessageQId JoyEvent = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void kMenu_HandleSelection(tMenu MainMenu, uint8_t *sel);

/**
  * @brief  Function to initialize the module menu
  * @retval None
  */
void kMenu_Init(void)
{
  /* Create the Push Button event handler */
  osMessageQDef(JoyQueue, 1, uint16_t);
  JoyEvent = osMessageCreate(osMessageQ(JoyQueue), NULL);
}

/**
  * @brief  Function in charge to handle the menu selection
  * @param  menu: menu structure
  * @param  sel: selected item
  * @retval None
  */
void kMenu_HandleSelection(tMenu MainMenu, uint8_t *sel)
{
  uint8_t exit = 0;
  tMenu psCurrentMenu = MainMenu;
  JOYState_TypeDef joystate = JOY_NONE;
  
  switch(psCurrentMenu.nType)
  {
  case TYPE_TEXT :
    {
      do
      {
        /* Clear the LCD GLASS */
        BSP_LCD_GLASS_Clear();
        
        /* Get the current menu */
        BSP_LCD_GLASS_DisplayString((uint8_t *)psCurrentMenu.psItems[*sel].pszTitle);

        joystate = kMenu_GetEvent(DemoMinIdleTime);

        switch(joystate)
        {
        case JOY_NONE :
          DemoEnterLowPower = SET;
          break;
        case JOY_LEFT :
          *sel = (psCurrentMenu.nItems - 1);
          exit = 1;
          break;

        case JOY_RIGHT :
        case JOY_SEL :
          exit = 1;
          break;
        case JOY_DOWN :
          (*sel)++;
          /* check if sel is on the latest line position */
          if(*sel >= (psCurrentMenu.nItems -1))
          {
            *sel = 0;
          }
          break;
        case JOY_UP :
          /** check if sel is on the first line */
          if ((*sel) == 0)
          {
          *sel = (psCurrentMenu.nItems - 2);
          }
          else
          {
          (*sel)--;
          }
          break;
        default:
          break;
        }
      }while(exit == 0);
    }
    break;
  }
  
  return;
}

/**
  * @brief  Function in charge to execute a menu entry point 
  * @param  menu 
  * @retval None
  */
void kMenu_Execute(tMenu psCurrentMenu) 
{
  uint32_t exit = 1;
  uint32_t k_MenuState = KMENU_HEADER;
  uint8_t sel = 0;
  MODULES_INFO prevModuleId = MODULE_NONE;
  
  do 
  {  
    switch(k_MenuState)
    {
    case KMENU_HEADER :
      {
        /****************************** Display Header *************************/  
        /* Clear the LCD GLASS */
        BSP_LCD_GLASS_Clear();

        if(psCurrentMenu.pszTitle != NULL)
        {
          BSP_LCD_GLASS_ScrollSentence((uint8_t *)psCurrentMenu.pszTitle, 1 , SCROLL_SPEED_HIGH);
        }
        
        switch(psCurrentMenu.nType)
        {
        case TYPE_TEXT :
          k_MenuState = KMENU_TEXT;
          break;
        case TYPE_EXEC :
          k_MenuState = KMENU_EXEC;
          break;
        default :
          k_MenuState = KMENU_EXIT;
          break;
        }
      }
      break;
    case KMENU_TEXT :
      {
        k_MenuState = KMENU_WAITEVENT;
      }
      break;
    case KMENU_EXEC :
      {
        /* if the function need user feedback, set callback function */
        if(psCurrentMenu.psItems[0].pfActionFunc != NULL)
        {
          /* set the function to report joystick event */
          kMenuEventForward = psCurrentMenu.psItems[0].pfActionFunc;
        }
        
        kMenu_Header(psCurrentMenu.psItems[0].pszTitle);
        
        /* execute the menu execution function */
        psCurrentMenu.psItems[0].pfExecFunc();
        
        /* reset user feedback, in polling mode */
        if(psCurrentMenu.psItems[0].pfActionFunc != NULL)
        {
          /* set the function to report to NULL */
          kMenuEventForward = NULL;
        }
        k_MenuState = KMENU_EXIT;
      }
      break;
    case KMENU_WAITEVENT:
      {
        kMenu_HandleSelection(psCurrentMenu,&sel);
        /* The user has selected an execution menu */
        switch(psCurrentMenu.psItems[sel].SelType)
        {
        case SEL_MODULE:
          /* save current Module Id */
          prevModuleId = DemoSelectedModuleId;
          DemoSelectedModuleId = (MODULES_INFO) psCurrentMenu.psItems[sel].ModuleId;
          /* start the module execution */
          kModule_Execute(psCurrentMenu.psItems[sel].ModuleId);
          /* restore Module Id */
          DemoSelectedModuleId = prevModuleId;
          k_MenuState = KMENU_HEADER;
          break;
        case SEL_EXEC :
          
          /* if the function need user feedback, and set callback function */
          if(psCurrentMenu.psItems[sel].pfActionFunc != NULL)
          {
            /* set the function to report joystick event */
            kMenuEventForward = psCurrentMenu.psItems[sel].pfActionFunc;
          }
          /* start the function execution */
          psCurrentMenu.psItems[sel].pfExecFunc();
          
          /* rest user feedback, in polling mode */
          if(psCurrentMenu.psItems[sel].pfActionFunc != NULL)
          {
            /* set the function to report to NULL */
            kMenuEventForward = NULL;
          }
          k_MenuState = KMENU_HEADER;
          break;
        case SEL_SUBMENU :
          /* Select submenu or return on the main module menu */
          kMenu_Execute(*(psCurrentMenu.psItems[sel].psSubMenu));
          k_MenuState = KMENU_HEADER;
          break;
        case SEL_EXIT :
          /* back to main application level */
          DemoSelectedModuleId = MODULE_MAIN_APP;
          k_MenuState = KMENU_EXIT;
          break;
        case SEL_NOTHING:
          /* to avoid exit of main application */
          k_MenuState = KMENU_WAITEVENT;
          sel = 0;
          break;
        }
      }
      break;
    case KMENU_EXIT :
      exit = 0;
      break;
      
    }
  }while(exit);
}


/**
  * @brief  Function to display header information 
  * @param  menu 
  * @retval None
  */
void kMenu_Header(char *string)
{
  if(string != NULL)
  BSP_LCD_GLASS_DisplayString((uint8_t *)string);
}


/**
  * @brief  Function in charge to handle user event and forward them to the module
  * @param  GPIO_Pin
  * @retval None
  */
void kMenu_EventHandler(uint16_t GPIO_Pin)
{
  
  if(kMenuEventForward != NULL)
  {
    switch(GPIO_Pin)
    {
    case DOWN_JOY_PIN :
      (kMenuEventForward)(JOY_DOWN);
      break;
    case UP_JOY_PIN :
      (kMenuEventForward)(JOY_UP);
      break;
    case SEL_JOY_PIN :
      (kMenuEventForward)(JOY_SEL);
      break;
    case RIGHT_JOY_PIN :
      (kMenuEventForward)(JOY_RIGHT);
      break;
    case LEFT_JOY_PIN :
      (kMenuEventForward)(JOY_LEFT);
      break;
    }
  }
}

/**
  * @brief  Function in charge to handle user event and send them to event queue
  * @param  GPIO_Pin
  * @retval None
  */
void kMenu_SendEvent(uint16_t GPIO_Pin)
{
  JOYState_TypeDef joystate = JOY_NONE;

  switch(GPIO_Pin)
  {
  case DOWN_JOY_PIN :
    joystate = JOY_DOWN;
    break;
  case UP_JOY_PIN :
    joystate = JOY_UP;
    break;
  case SEL_JOY_PIN :
    joystate = JOY_SEL;
    break;
  case RIGHT_JOY_PIN :
    joystate = JOY_RIGHT;
    break;
  case LEFT_JOY_PIN :
    joystate = JOY_LEFT;
    break;
  }
  /* Send Message to event handler */
  osMessagePut(JoyEvent, joystate, 0);
}

/**
  * @brief  Function in charge to wait for user event
  * @param  Delay   Delay to wait event for
  * @retval Joystick key event
  */
JOYState_TypeDef kMenu_GetEvent(uint32_t Delay)
{
  osEvent event = osMessageGet(JoyEvent, Delay);

  /* wait mutex for end of WFI execution */
  osRecursiveMutexWait(DemoLowPowerMutex, 0xFFFFFFFF);
  
  /* free mutex */
  osRecursiveMutexRelease(DemoLowPowerMutex);

  if(event.status == osEventMessage)
  {
    return (JOYState_TypeDef) (event.value.v);
  }
  else
  {
    return JOY_NONE;
  }
}

/**
  * @}
  */

/**
  * @}
  */

