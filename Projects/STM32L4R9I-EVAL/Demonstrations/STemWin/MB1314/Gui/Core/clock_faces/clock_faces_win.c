/**
  ******************************************************************************
  * @file    clock_faces_win.c
  * @author  MCD Application Team
  * @brief   clock faces functions
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
#include "LISTWHEEL.h"
#include "math.h"
#include "k_rtc.h"
#include "Resources.h"
#include "clock_faces_res.h"
#include "dimming.h"

/** @addtogroup CLOCK_FACES_MODULE
  * @{
  */

/** @defgroup CLOCK_FACES
  * @brief clock faces routines
  * @{
  */
    
/* Private defines -----------------------------------------------------------*/
#define ID_BUTTON_MODULE_CENTER    (GUI_ID_USER + 0x01)
#define ID_BUTTON_SEC_SETTING_1    (GUI_ID_USER + 0x02)
#define ID_BUTTON_SEC_SETTING_2    (GUI_ID_USER + 0x03)

#define GUI_ID_BUTTON_SET          (GUI_ID_USER + 0x04)
#define GUI_ID_BUTTON_CANCEL       (GUI_ID_USER + 0x05)

#define GUI_ID_BUTTON_SET_DATE     (GUI_ID_USER + 0x06)
#define GUI_ID_BUTTON_SET_TIME     (GUI_ID_USER + 0x07)

#define GUI_ID_BUTTON_24H          (GUI_ID_USER + 0x08)
#define GUI_ID_BUTTON_12H          (GUI_ID_USER + 0x09)
#define GUI_ID_BUTTON_BACK         (GUI_ID_USER + 0x0A)

#define GUI_ID_BUTTON_ADD_ALARM    (GUI_ID_USER + 0x0B)
#define GUI_ID_BUTTON_ALARM_LOCK   (GUI_ID_USER + 0x0C)
#define GUI_ID_ALARM_LIST          (GUI_ID_USER + 0x0D)
#define GUI_ID_BUTTON_ALARM1       (GUI_ID_USER + 0x0E)
#define GUI_ID_BUTTON_ALARM2       (GUI_ID_USER + 0x0F)
#define GUI_ID_BUTTON_ALARM3       (GUI_ID_USER + 0x10)
#define GUI_ID_BUTTON_ALARM4       (GUI_ID_USER + 0x11)
#define GUI_ID_BUTTON_ALARM5       (GUI_ID_USER + 0x12)

#define GUI_ID_BUTTON_ALARM_LOCK1  (GUI_ID_USER + 0x13)
#define GUI_ID_BUTTON_ALARM_LOCK2  (GUI_ID_USER + 0x14)
#define GUI_ID_BUTTON_ALARM_LOCK3  (GUI_ID_USER + 0x15)
#define GUI_ID_BUTTON_ALARM_LOCK4  (GUI_ID_USER + 0x16)
#define GUI_ID_BUTTON_ALARM_LOCK5  (GUI_ID_USER + 0x17)

#define GUI_ID_BUTTON_ALARM_REPEAT (GUI_ID_USER + 0x18)
#define GUI_ID_BUTTON_ALARM_MON    (GUI_ID_USER + 0x19)
#define GUI_ID_BUTTON_ALARM_TUE    (GUI_ID_USER + 0x1A)
#define GUI_ID_BUTTON_ALARM_WED    (GUI_ID_USER + 0x1B)
#define GUI_ID_BUTTON_ALARM_THU    (GUI_ID_USER + 0x1C)
#define GUI_ID_BUTTON_ALARM_FRI    (GUI_ID_USER + 0x1D)
#define GUI_ID_BUTTON_ALARM_SAT    (GUI_ID_USER + 0x1E)
#define GUI_ID_BUTTON_ALARM_SUN    (GUI_ID_USER + 0x1F)

#define GUI_ID_BUTTON_ALARM_REP1   (GUI_ID_USER + 0x20)
#define GUI_ID_BUTTON_ALARM_REP2   (GUI_ID_USER + 0x21)
#define GUI_ID_BUTTON_ALARM_REP3   (GUI_ID_USER + 0x22)
#define GUI_ID_BUTTON_ALARM_REP4   (GUI_ID_USER + 0x23)
#define GUI_ID_BUTTON_ALARM_REP5   (GUI_ID_USER + 0x24)

#define GUI_ID_BUTTON_ALARM_DISMISS (GUI_ID_USER + 0x25)
#define GUI_ID_BUTTON_ALARM_SNOOZE  (GUI_ID_USER + 0x26)

#define GUI_ID_BUTTON_REMOVE_ALARM  (GUI_ID_USER + 0x27)

/* Arc calculations */
#define  AA_FACTOR  3
#define  PI         3.1415926

/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static void _cbAlarms(WM_MESSAGE * pMsg);

/* External variables --------------------------------------------------------*/
extern WM_HWIN hMotion;

/* Private typedef -----------------------------------------------------------*/
/* Startup module: clock faces */
K_ModuleItem_Typedef  clock_faces =
{
  0,
  "clock faces",
  0,
  Startup,
  NULL,
};

/* Bitmaps stucture for the radial Menu */
typedef struct 
{
  const GUI_BITMAP * pBitmap;
} BITMAP_ITEM;

/* Infos for the sliding menu */
typedef struct 
{
  int xPos;
  int yPos;
  int Index;
} ITEM_INFO;

/* Needed params */
typedef struct 
{
  int                 Pos;
  int                 NumItems;
  int                 xSizeItem,   ySizeItem;
  int                 xSizeWindow, ySizeWindow;
  int                 rx,          ry;
  int                 mx,          my;
  int                 FinalMove;
  const BITMAP_ITEM * pBitmapItem;
  ITEM_INFO         * pItemInfo;
} PARA;

/* Needles draws clock1 & clock2 */
static const GUI_POINT aPoints_Clock1[3][4] = 
{   
  /* Hour Needle */
  {{ 3, 0 * AA_FACTOR}, 
   { 3, -25 * AA_FACTOR},
   {-3, -25 * AA_FACTOR},
   {-3, 0 * AA_FACTOR}},  
  
  /* Min Needle */
  {{ 2, 0 * AA_FACTOR},
   { 2, -33 * AA_FACTOR},
   {-2, -33 * AA_FACTOR},
   {-2, 0 * AA_FACTOR}},
   
   /* Sec Needle */
  {{2, 1 * AA_FACTOR},
   {-2, 1 * AA_FACTOR},
   {-2,-135 * AA_FACTOR},
   {2,-135 * AA_FACTOR}}, 
};

/* Wheel window define */
typedef struct 
{
  WM_HWIN hWin;
} WHEEL;

/* All needed information to create an alarm */
typedef struct 
{  
  int   buttonId;
  int   validButtonId;
  int   repeatButtonId;
  int   IsValid;
  int   repeat[8];
  int   added;
  int   hour;
  int   min;
  char  alarmItems[10];
} AlarmParams;

/* Alarm structure to set on the RTC HW */
typedef struct 
{  
  int   IndexAlarmOnRTC; /* Will be "0" to "4" as 5 alarms can be set, If IndexForRepeat = (-1): so no alarm to set*/
  int   Alarm_A_IsSet;
  int   Alarm_B_IsSet;  
} AlarmOnRTC_t;

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Table of the bitmaps to be used on the Main Menu */
static const BITMAP_ITEM _aBitmapItem[] = 
{  
  {&bmclock3_small},
  {&bmclock4_small},
  {&bmclock1_small},
  {&bmclock2_small},
  {&bmclock3_small},
  {&bmclock4_small},
  {&bmclock1_small},
  {&bmclock2_small},

};

/* handle for different windows used on the clock faces */
WM_HWIN  hclockmain;
WM_HWIN  hclockmainSettings;
WM_HWIN  hMotionClocks;
WM_HWIN  hclockSecSettings;
WM_HWIN  hset_alarm;
WM_HWIN  hsetDateAndTime;
WM_HWIN  hsetDate;
WM_HWIN  hsetTime;
WM_HWIN  hsetTimeFormat;
WM_HWIN  hAlarms;
WM_HWIN  hAlarmAlert;
/* Timer declarations */
WM_HTIMER hTimerTime;

static WM_HWIN hSwipelist;

/* First Init  */
int ClockFirstTime = 0;
/* Alarm repeat index */
int IndexForRepeat;

/* RTC defines */
RTC_TimeTypeDef Time;
RTC_DateTypeDef Date;

/* Points for needle draws */
GUI_POINT aPointsDest_Clock1[3][4];

/* clack face choice: default is clock1 */
static int ClockChoice = 0;

/* 3 Wheels needed */
static WHEEL _aWheel[3];

/* Time format initialisation */
int IsTimeFormat24 = 0;
int TempTimeFormat24 = 0;
int IsAm = 0;
/* Alarm index: 5 SW alarms are possibles */
static int alarmIndex = 0;
AlarmParams AlarmTable[5];
AlarmOnRTC_t AlarmOnRTC;

/* Week days definitions */
static char * _apWeekDay[] = 
{
  "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"
};

static char * _apWeekDayForPrint[] = 
{
  "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

/* Years interval to be used on the Demo [2000 - 2050] */
static char * _apYear[] = 
{
  "2000", "2001", "2002", "2003", "2004", "2005", "2006", "2007", "2008", "2009", \
  "2010", "2011", "2012", "2013", "2014", "2015", "2016", "2017", "2018", "2019", \
  "2020", "2021", "2022", "2023", "2024", "2025", "2026", "2027", "2028", "2029", \
  "2030", "2031", "2032", "2033", "2034", "2035", "2036", "2037", "2038", "2039", \
  "2040", "2041", "2042", "2043", "2044", "2045", "2046", "2047", "2048", "2049", \
  "2050", NULL
};

/* Months on the listwheel */
static char * _apMonth[] = 
{
  "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

/* Months for display on the clock face */
static char * _apMonthForPrint[] = 
{
  "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "Novomber", "December"
};

/* Days for all configuration */
static char * _apDay[] = 
{
  "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"
};

static char * _apDay28[] = 
{
  "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13",\
  "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26",\
  "27", "28", NULL
};

static char * _apDay29[] = 
{
  "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13",\
  "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26",\
  "27", "28", "29", NULL
};

static char * _apDay30[] = 
{
  "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13",\
  "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26",\
  "27", "28", "29", "30", NULL
};

static char * _apDay31[] = 
{
  "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13",\
  "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26",\
  "27", "28", "29", "30", "31", NULL
};

/* Hours */
static char * _apHours24[] = 
{
  "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23"
};

static char * _apHours12[] = 
{
  "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"
};

/* Minutes */
static char * _apMinutes[] = 
{
  "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", \
    "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59"
};

/* AmPm enum */
static char * _apAmPm[] = 
{
  "AM", "PM"
};

/* Alarm Animation */ 
static GUI_CONST_STORAGE GUI_BITMAP *AlarmAnim1[] = 
{
  &bmalarm_animation1_10,
  &bmalarm_animation1_20,
  &bmalarm_animation1_30,
  &bmalarm_animation1_40,
  &bmalarm_animation1_50,
  &bmalarm_animation1_60,
  &bmalarm_animation1_70,
  &bmalarm_animation1_80,
  &bmalarm_animation1_90,
  &bmalarm_animation1_100,
};

static GUI_CONST_STORAGE GUI_BITMAP *AlarmAnim2[] = 
{
  &bmalarm_animation2_10,
  &bmalarm_animation2_20,
  &bmalarm_animation2_30,
  &bmalarm_animation2_40,
  &bmalarm_animation2_50,
  &bmalarm_animation2_60,
  &bmalarm_animation2_70,
  &bmalarm_animation2_80,
  &bmalarm_animation2_90,
  &bmalarm_animation2_100,
};

static GUI_CONST_STORAGE GUI_BITMAP *AlarmAnim3[] = 
{
  &bmalarm_animation3_10,
  &bmalarm_animation3_20,
  &bmalarm_animation3_30,
  &bmalarm_animation3_40,
  &bmalarm_animation3_50,
  &bmalarm_animation3_60,
  &bmalarm_animation3_70,
  &bmalarm_animation3_80,
  &bmalarm_animation3_90,
  &bmalarm_animation3_100,
};


/* Functions -----------------------------------------------------------------*/
/**
  * @brief  Switch alarms with id1 & id2
  * @param  id1: index of the alarm1 table       
  * @param  id2: index of the alarm2 table       
  * @retval None
  */
static void SwitchAlarmCases(int id1, int id2) 
{
  AlarmParams TempAlarm;
  
  /* Buttons Id must remain the same, so can't do a memcpy */
  TempAlarm.IsValid = AlarmTable[id1].IsValid;
  memcpy((void*)&TempAlarm.repeat, (void*)&AlarmTable[id1].repeat, sizeof(TempAlarm.repeat));
  TempAlarm.added   = AlarmTable[id1].added;
  TempAlarm.hour    = AlarmTable[id1].hour;
  TempAlarm.min     = AlarmTable[id1].min;  
  strcpy(TempAlarm.alarmItems, AlarmTable[id1].alarmItems);
  
  
  AlarmTable[id1].IsValid = AlarmTable[id2].IsValid;
  memcpy((void*)&AlarmTable[id1].repeat, (void*)&AlarmTable[id2].repeat, sizeof(TempAlarm.repeat));

  AlarmTable[id1].added   = AlarmTable[id2].added;
  AlarmTable[id1].hour    = AlarmTable[id2].hour;
  AlarmTable[id1].min     = AlarmTable[id2].min;  
  strcpy(AlarmTable[id1].alarmItems, AlarmTable[id2].alarmItems);
  
  AlarmTable[id2].IsValid = TempAlarm.IsValid;
  memcpy((void*)&AlarmTable[id1].repeat, (void*)&TempAlarm.repeat, sizeof(TempAlarm.repeat));
  AlarmTable[id2].added   = TempAlarm.added;
  AlarmTable[id2].hour    = TempAlarm.hour;
  AlarmTable[id2].min     = TempAlarm.min;  
  strcpy(AlarmTable[id2].alarmItems, TempAlarm.alarmItems);
   
  return;
}

/**
  * @brief  Compare to previous alarm case
  * @param  id: index of the alarm table       
  * @retval None
  */
static void CompareToPreviousAlarm(int id) 
{
  if(strstr(AlarmTable[id].alarmItems, "AM") != NULL)
  {
    if(strstr(AlarmTable[id - 1].alarmItems, "PM") != NULL)
    {
      SwitchAlarmCases(id, id - 1);
      return;
    }
  }
  if(strstr(AlarmTable[id].alarmItems, "PM") != NULL)
  {
    if(strstr(AlarmTable[id - 1].alarmItems, "AM") != NULL)
    {
      return;
    }
  }
  
  if(strstr(AlarmTable[id].alarmItems, "M") != NULL)
  {
    if(AlarmTable[id].hour == 11)
    {
      SwitchAlarmCases(id, id - 1);
      return;
    }     
  }
  if(strstr(AlarmTable[id].alarmItems, "M") != NULL)
  {
    if(AlarmTable[id - 1].hour == 11)
    {
      return;
    }     
  }
  
  if(AlarmTable[id].hour < AlarmTable[id - 1].hour)
  {
    SwitchAlarmCases(id, id - 1);    
  }
  else if(AlarmTable[id].hour == AlarmTable[id - 1].hour)
  {
    if(AlarmTable[id].min < AlarmTable[id - 1].min)
    {
      SwitchAlarmCases(id, id - 1);
    }
  }
  return;
}

/**
  * @brief  Switch alarms from 12H format to 24H format 
  * @param  Is12h_Or_24h: input is 12H format Or 24H format
  * @retval None
  */
static void SwitchAlarmFormat(int Is12h_Or_24h) 
{
  int i;
  
  if(Is12h_Or_24h == 1)
  {
    for(i=0; i < alarmIndex; i++)
    {
      if((strstr(AlarmTable[i].alarmItems, "AM")) != NULL)
      {
        if(AlarmTable[i].hour == 11)
        {
          AlarmTable[i].hour = 0;
          sprintf((char *)AlarmTable[i].alarmItems, "%s:%s", _apHours24[AlarmTable[i].hour], _apMinutes[AlarmTable[i].min]);
        }
        else
        {
          AlarmTable[i].hour += 1;
          sprintf((char *)AlarmTable[i].alarmItems, "%s:%s", _apHours24[AlarmTable[i].hour], _apMinutes[AlarmTable[i].min]);
        }
      }
      else
      {
        if(AlarmTable[i].hour == 11)
        {
          AlarmTable[i].hour = 12;
          sprintf((char *)AlarmTable[i].alarmItems, "%s:%s", _apHours24[AlarmTable[i].hour], _apMinutes[AlarmTable[i].min]);
        }
        else
        {
          AlarmTable[i].hour += 13;        
          sprintf((char *)AlarmTable[i].alarmItems, "%s:%s", _apHours24[AlarmTable[i].hour], _apMinutes[AlarmTable[i].min]);
        }
      }
    }
  }
  else
  {
    for(i=0; i < alarmIndex; i++)
    {
      if(AlarmTable[i].hour > 12)
      {
        AlarmTable[i].hour -= 13;
        sprintf((char *)AlarmTable[i].alarmItems, "%s:%s %s", _apHours12[AlarmTable[i].hour], _apMinutes[AlarmTable[i].min], "PM");
      }
      else
      {
        /* The case 00 hours is treated before */
        if((strstr(AlarmTable[i].alarmItems, "00:")) != NULL)
        {
          AlarmTable[i].hour = 11;
          sprintf((char *)AlarmTable[i].alarmItems, "%s:%s %s", _apHours12[11], _apMinutes[AlarmTable[i].min], "AM");
        }
        else if((strstr(AlarmTable[i].alarmItems, "12:")) != NULL)
        {
          AlarmTable[i].hour = 11;
          sprintf((char *)AlarmTable[i].alarmItems, "%s:%s %s", _apHours12[11], _apMinutes[AlarmTable[i].min], "PM");
        }
        else
        {
          AlarmTable[i].hour -= 1;
          sprintf((char *)AlarmTable[i].alarmItems, "%s:%s %s", _apHours12[AlarmTable[i].hour], _apMinutes[AlarmTable[i].min], "AM");
        }
      }
    }
  }
}

/**
  * @brief  Get the rigth alarm to set
  * @param  none 
  * @retval int
  */ 
static int IndexOfAlarmToSet(void) 
{
  int i;  
  
  /* First of all get the Time and the Date */
  k_GetTime(&Time); 
  
  /* We need to find the alarm with the closest hour, then the closest minutes */
  for(i=0; i < alarmIndex; i++)
  {    
    if(AlarmTable[i].IsValid)
    {
      if((strstr(AlarmTable[i].alarmItems, "M")) != NULL)
      {
        if((AlarmTable[i].hour + 1) >= Time.Hours)
        {
          if(AlarmTable[i].min > Time.Minutes)
          {
            return i;
          }
        }
      }
      else
      {
        if((AlarmTable[i].hour) >= Time.Hours)
        {
          if(AlarmTable[i].min >= Time.Minutes)
          {
            return i;
          }
        }
      }
    } 
  }
  return 0;
}

/**
  * @brief  callback for alarm alert callback
  * @param  None
  * @retval None
  */
void AlarmAlertCallback(void)
{
  WM_ShowWindow(hAlarmAlert);
  /* Reset the dimming */  
  DIMMING_Reset();
}

/**
  * @brief  Program the RTC with the right alarm
  * @param  none 
  * @retval int
  */
static void SetAlarmOnRTC(void) 
{
  RTC_AlarmTypeDef alarm;
  
  /* Set the First alarm */
  alarm.Alarm                 = RTC_ALARM_A;
  alarm.AlarmDateWeekDay      = RTC_WEEKDAY_MONDAY | RTC_WEEKDAY_WEDNESDAY;
  alarm.AlarmDateWeekDaySel   = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
  alarm.AlarmMask             = RTC_ALARMMASK_DATEWEEKDAY;
  alarm.AlarmSubSecondMask    = RTC_ALARMSUBSECONDMASK_NONE;
  if (!IsTimeFormat24)
  {
    if((strstr(AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].alarmItems, "AM")) != NULL)
    {
      alarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
    }
    else
    {
      alarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_PM;
    }
    alarm.AlarmTime.Hours       = AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour + 1;  //AlarmHour + 1;
    alarm.AlarmTime.Minutes     = AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min;
    alarm.AlarmTime.Seconds     = 0;
    alarm.AlarmTime.SubSeconds  = 0;
  }
  else
  {
    alarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
    alarm.AlarmTime.Hours      = AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour;//AlarmHour;
    alarm.AlarmTime.Minutes     = AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min;
    alarm.AlarmTime.Seconds     = 0;
    alarm.AlarmTime.SubSeconds  = 0;
  }
  k_SetAlarm(&alarm);
  k_SetAlarmCallback(AlarmAlertCallback);
  AlarmOnRTC.Alarm_A_IsSet = 1;
  
  return;
}


/**
  * @brief  Draw Needle
  * @param  clockNbre: clock index
  * @param  index    : Needle index  
  * @param  x0       :    x position
  * @param  y0       :    y position      
  * @retval None
  */
static void DrawNeedle(int clockNbre, uint32_t index, uint16_t x0, uint16_t y0) 
{
  if(clockNbre == 0)
  {
    /* draw Needles */
    if(index >= 2)
    {
      GUI_SetColor(0xE80285FD);
    }    
    else
    {   
      GUI_SetColor(GUI_WHITE);      
    }
  }
  else
  {
    /* draw Needles */
    if(index >= 2)
    {
      GUI_SetColor(GUI_BLACK);      
    }    
    else
    {  
      GUI_SetColor(0xE80285FD);      
    }     
  }
  GUI_AA_FillPolygon(aPointsDest_Clock1[index], 4, AA_FACTOR * x0, AA_FACTOR * y0);
}

/**
  * @brief  Display date
  * @param  clockNbre: clock index  
  * @retval None
  */
static void DisplayDate(int clockNbre) 
{
  static uint8_t TempStr[150]; 
  
  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_SetFont(&GUI_FontRobotoC_bold22);
  
  if(clockNbre == 0)
  {    
    GUI_SetColor(GUI_WHITE);      
    sprintf((char *)TempStr, "%s %s, %s", _apMonthForPrint[Date.Month - 1], _apDay[Date.Date - 1],  _apYear[Date.Year - 1]);
    GUI_DispStringHCenterAt((char *)TempStr, 195, 279);
        
  }
  else
  {    
    GUI_SetColor(GUI_BLUE);      
    sprintf((char *)TempStr, "%s %s, %s", _apMonthForPrint[Date.Month - 1], _apDay[Date.Date - 1],  _apYear[Date.Year - 1]);
    GUI_DispStringHCenterAt((char *)TempStr, 195, 110);        
  }
}

/**
  * @brief  Get the day of the week
  * @param  d: The current day
  * @param  m: The current month
  * @param  y: The current year
  * @retval The day of the week
  */
int GetDayOfWeek(int d, int m, int y)
{
    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };

    y -= m < 3;

    return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}

/**
  * @brief  Get the number of days on a given month
  * @param  month: The current month
  * @param  year:  The current year
  * @retval The number of days in the given month
  */
int GetNumberOfDays(int month, int year)
{
  int numberOfDays = 0;

  if (month == 1  || /*JAN*/
      month == 3  || /*MAR*/
      month == 5  || /*MAY*/
      month == 7  || /*JUL*/
      month == 8  || /*AUG*/
      month == 10 || /*OCT*/
      month == 12)   /*DEC*/
  {
    numberOfDays = 31;
  }
  else if (month == 4 || /*APR*/
           month == 6 || /*JUN*/
           month == 9 || /*SEP*/
           month == 11)  /*NOV*/
  {
    numberOfDays = 30;
  }
  else if (month == 2 /*FEB*/)
  {
    numberOfDays = 28;

    if (year % 400 == 0)
    {
      numberOfDays = numberOfDays + 1;
    }
    else if (year % 100 == 0)
    {
    }
    else if (year % 4 == 0)
    {
      numberOfDays = numberOfDays + 1;
    }
  }

  return numberOfDays;
}

/**
  * @brief  Update clock
  * @param  clockNbre: clock index
  * @param  x0       : x position
  * @param  y0       : y position     
  * @retval None
  */
static void GUI_UpdateClock (int clockNbre, uint16_t x0, uint16_t y0)
{
  static int i = 0;
  static int j = 0;
  static uint8_t TempStr[150]; 
  float a;
  int xp, yp;
  int dayOfTheWeek = 0; 

  /* First of all get the Time and the Date */
  k_GetTime(&Time); 
  k_GetDate(&Date);
  
  /* Need to be inside our table */
  if((Date.Year > GUI_COUNTOF(_apYear) || (Date.Year == 0)))
  {
      Date.Year = 18;
  }
  
  if(((Date.Month - 1) == 1) && (Date.Date >= 30))
  {
      Date.Date = 2;
  }
  
  if(Date.Month > GUI_COUNTOF(_apMonth))
  {
      Date.Month = 10;
  } 
 
  if(!IsTimeFormat24)    
  {
    if((Time.Hours == 12) && (Time.Minutes == 0) && (Time.Seconds == 0))
    {
      IsAm ^= 1;
    }
    
    if(Time.Hours > 12)
    {
      Time.Hours -= 12;
      
      k_SetTime(&Time); 
      k_SetDate(&Date);
    }    
    if(Time.Hours == 0)
    {
      Time.Hours = 12;
      k_SetTime(&Time); 
      k_SetDate(&Date);
    }
  }
  
  /* Then update the needed clock */
  switch(clockNbre)
  {
    case 0:      
    case 1:
      DisplayDate(clockNbre);
      /* Analog Clocks */
      GUI_AA_EnableHiRes();
      GUI_AA_SetFactor(AA_FACTOR);
      /* Clock1 */
      GUI_MagnifyPolygon(aPointsDest_Clock1[0], aPoints_Clock1[0], 4, 4);
      GUI_RotatePolygon(aPointsDest_Clock1[0], aPointsDest_Clock1[0], 4, - 2 * PI * (float)((float)Time.Hours + (float)Time.Minutes /60) / 12);    
      DrawNeedle(clockNbre, 0, x0, y0);

      GUI_MagnifyPolygon(aPointsDest_Clock1[1], aPoints_Clock1[1], 4, 4);
      GUI_RotatePolygon(aPointsDest_Clock1[1], aPointsDest_Clock1[1], 4, - 2 * PI * (float)((float)Time.Minutes + (float)Time.Seconds / 60) / 60);
      DrawNeedle(clockNbre, 1, x0, y0);  

      GUI_MagnifyPolygon(aPointsDest_Clock1[2], aPoints_Clock1[2], 4, 1);
      GUI_RotatePolygon(aPointsDest_Clock1[2], aPointsDest_Clock1[2], 4, - 2 * PI * Time.Seconds / 60);    
      DrawNeedle(clockNbre, 2, x0, y0);     

      GUI_AA_DisableHiRes();
      
      /* The circle is only displayed on the analog clock 1 */
      if (clockNbre == 0)
      {
        GUI_DrawBitmap(&bmanalog_clock1_cercle, 174, 174);
      }
      if(alarmIndex > 0)
      {
        if (clockNbre == 0)
        {
          GUI_DrawBitmap(&bmsmall_white_bell, (390 - bmsmall_white_bell.XSize)/2 ,78);
        }
        else
        {
          GUI_DrawBitmap(&bmsmall_blue_bell, (390 - bmsmall_blue_bell.XSize)/2, 250);
        }        
      }
      
      break;

    case 2:
      /* Clock 3 */
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetFont(&GUI_FontRobotoC_bold22);
      GUI_SetColor(GUI_BLUE);

      /* Get the day of the week */
      dayOfTheWeek = GetDayOfWeek(Date.Date, Date.Month, 2000 + Date.Year - 1);

      sprintf((char *)TempStr, "%s", _apWeekDay[dayOfTheWeek]);
      GUI_DispStringHCenterAt((char *)TempStr, 195, 36);
      
      GUI_SetFont(&GUI_FontRobotoRegular50);
      GUI_SetColor(GUI_BLUE);
      sprintf((char *)TempStr, "%s", _apDay[Date.Date - 1]);
      GUI_DispStringHCenterAt((char *)TempStr, 195, 54);
      
      GUI_SetFont(&GUI_FontRobotoC_bold22);
      GUI_SetColor(GUI_WHITE);
      sprintf((char *)TempStr, "%s", _apMonthForPrint[Date.Month - 1]);
      
      GUI_DispStringHCenterAt((char *)TempStr, 195, 134);
      
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetFont(&GUI_FontRoboto_Light100);
      GUI_SetColor(GUI_WHITE);
      if((Time.Seconds % 2) == 0)
      {
        sprintf((char *)TempStr, ":");
        GUI_DispStringHCenterAt((char *)TempStr, 194, 219);       
      }
      sprintf((char *)TempStr, "%02d", Time.Minutes);
      GUI_DispStringAt((char *)TempStr, 210, 219);
      sprintf((char *)TempStr, "%02d", Time.Hours);
      GUI_DispStringAt((char *)TempStr, 98, 219);
      
      GUI_SetFont(&GUI_FontRoboto_Light31);
      GUI_SetColor(GUI_WHITE);
      if(!IsTimeFormat24)
      {
        if(!IsAm)
        {
          sprintf((char *)TempStr, "AM");
        }
        else
        {
          sprintf((char *)TempStr, "PM");
        }
        GUI_DispStringHCenterAt((char *)TempStr, 320, 280);
      }
      if(alarmIndex > 0)
      {
        GUI_DrawBitmap(&bmsmall_white_bell, (390 - bmsmall_white_bell.XSize)/2, 320);        
      }
      break;

    case 3:
      /* Clock4 */
      i = Time.Minutes;
      j = Time.Seconds;
      GUI_SetColor(0xFF0062FF);
      GUI_SetPenSize(12);
      GUI_AA_DrawArc(195, 195, 151, 151, 90 - i*6, 90);
      
      GUI_SetPenSize(6);
      GUI_AA_DrawArc(195, 195, 180, 180, 90 - j*6, 90);    
          
      /* Calculate and draw the white circle: Minutes */ 
      a = (90 + i * 6 ) * 3.1415926 / 180;
      xp = (int)(-150 * cos(a) + 195);
      yp = (int)(-150 * sin(a) + 195);
      
      GUI_SetColor(GUI_WHITE);
      GUI_SetPenSize( 15 );
      GUI_DrawPoint(xp, yp); 
      
      /* Calculate and draw the white circle: Seconds */ 
      a = (90 + j * 6 ) * 3.1415926 / 180;
      xp = (int)(-179 * cos(a) + 195);
      yp = (int)(-179 * sin(a) + 195);
      
      GUI_SetColor(GUI_WHITE);
      GUI_SetPenSize( 10 );
      GUI_DrawPoint(xp, yp);

      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetFont(&GUI_FontRoboto_C_regular100);
      GUI_SetColor(GUI_WHITE);
      if((Time.Seconds % 2) == 0)
      {
        sprintf((char *)TempStr, ":");
        GUI_DispStringAt((char *)TempStr, 194, 120);
      }
      sprintf((char *)TempStr, "%02d", Time.Minutes);
      GUI_DispStringAt((char *)TempStr, 210, 120);
      sprintf((char *)TempStr, "%02d", Time.Hours);
      GUI_DispStringAt((char *)TempStr, 100, 120);
      
      GUI_SetFont(&GUI_FontRobotoC_bold22);
      GUI_SetColor(GUI_WHITE);
      if(!IsTimeFormat24)
      {
        if(!IsAm)
        {
          sprintf((char *)TempStr, "AM");
        }
        else
        {
          sprintf((char *)TempStr, "PM");
        }
        GUI_DispStringHCenterAt((char *)TempStr, 195, 98);
      } 
      
      GUI_SetFont(&GUI_FontRobotoC_bold22);
      GUI_SetColor(GUI_WHITE);
      sprintf((char *)TempStr, "%s %s", _apDay[Date.Date - 1], _apMonthForPrint[Date.Month - 1]);
      GUI_DispStringHCenterAt((char *)TempStr, 195, 265);    
      
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetFont(&GUI_FontRoboto_Light31);
      GUI_SetColor(GUI_WHITE);
     
      /* Get the day of the week */
      dayOfTheWeek = GetDayOfWeek(Date.Date, Date.Month, 2000 + Date.Year - 1);

      sprintf((char *)TempStr, "%s", _apWeekDay[dayOfTheWeek]);
      GUI_DispStringHCenterAt((char *)TempStr, 195, 235);
      if(alarmIndex > 0)
      {
        GUI_DrawBitmap(&bmsmall_white_bell, (390 - bmsmall_white_bell.XSize)/2, 295);        
      }
      break;
  } 
}


/**
  * @brief  callback for alarm alert window
  * @param  pMsg: Pointer to message structure
  * @retval None
  */
static void _cbAlarmAlert(WM_MESSAGE * pMsg) 
{
  static char TempStr[20];
  int dayOfTheWeek = 0;
  static int anim1 = 0;
  static int anim2 = 0;
  static int anim3 = 0;
  static int animDir = 1;
  int i;
  
  switch (pMsg->MsgId)
  {
    case WM_TIMER:
      if (anim2 < 2)
      {
        anim1 = anim1 + (1 * animDir);
      }

      if ((anim1 > 7) && (anim3 < 2))
      {
        anim2 = anim2 + (1 * animDir);
      }

      if (anim2 > 7)
      {
        anim3 = anim3 + (1 * animDir);
      }
      
      if ((anim1 == 0) && (anim2 == 0) && (anim3 == 0))
      {
        animDir = 1;
      }
      if ((anim1 == 9) && (anim2 == 9) && (anim3 == 9))
      {
        animDir = -1;
      }

      WM_InvalidateWindow(pMsg->hWin);
      WM_RestartTimer(pMsg->Data.v, 30);
      break;

    case WM_DELETE:
      WM_DeleteTimer(hTimerTime);
      break;

    case WM_CREATE:
      hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 30, 0);
      break;

    case WM_PAINT:
      GUI_DrawBitmap(&bmbkc_setalarm, 0, 0);
      GUI_DrawBitmap(&bmic_alarm_repeat_alarm, 173, 27);
      /* Draw the animation */
      GUI_DrawBitmap(AlarmAnim1[anim1], (390 - AlarmAnim1[anim1]->XSize) / 2, 41);
      GUI_DrawBitmap(AlarmAnim2[anim2], (390 - AlarmAnim2[anim1]->XSize) / 2, 34);
      GUI_DrawBitmap(AlarmAnim3[anim3], (390 - AlarmAnim3[anim1]->XSize) / 2, 27);
      
      /* Get the Time and the Date */
      k_GetTime(&Time); 
      k_GetDate(&Date);
      
      GUI_SetTextMode(GUI_TM_TRANS);      
      
      /* Get the day of the week */
      dayOfTheWeek = GetDayOfWeek(Date.Date, Date.Month, 2000 + Date.Year - 1);      
      GUI_SetFont(&GUI_FontRoboto_C_Regular35);
      sprintf((char *)TempStr, "%s", _apWeekDayForPrint[dayOfTheWeek]);
      GUI_DispStringHCenterAt((char *)TempStr, 195, 120);     
      
      GUI_SetFont(&GUI_FontRobotoCnBold48);
      sprintf((char *)TempStr, "%02d : %02d", Time.Hours, Time.Minutes);
      GUI_DispStringHCenterAt((char *)TempStr, 195, 167);
      GUI_SetFont(&GUI_FontRobotoC_regular22);
      if(!IsTimeFormat24)
      {
        if(!IsAm)
        {
          sprintf((char *)TempStr, "AM");
        }
        else
        {
          sprintf((char *)TempStr, "PM");
        }
        GUI_DispStringAt((char *)TempStr, 260, 184);
      }
      GUI_SetFont(&GUI_FontRobotoCn_bold27);
      GUI_DispStringHCenterAt((char *)"Repeat", 122, 310);
      GUI_SetFont(&GUI_FontRobotoC_regular22);
      GUI_DispStringHCenterAt((char *)"\n5min", 122, 310);
      GUI_SetFont(&GUI_FontRobotoCn_bold27);
      GUI_DispStringHCenterAt((char *)"Stop", 268, 310);

      break;
      
    case WM_TOUCH:
      break;

    case WM_NOTIFY_PARENT:
      switch (pMsg->Data.v) 
      {
        case WM_NOTIFICATION_RELEASED:
          switch (WM_GetId(pMsg->hWinSrc)) 
          {
            case GUI_ID_BUTTON_ALARM_DISMISS:
              k_DeactivateAlarm(RTC_ALARM_A);
              AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].IsValid = 0;
              if(alarmIndex > 0)
              {
                for(i=(alarmIndex-1); i > 0; i--)
                {
                  CompareToPreviousAlarm(i);
                }
              }
              
              AlarmOnRTC.IndexAlarmOnRTC = IndexOfAlarmToSet();
              
              /* Now Program the new alarm on the RTC HW */              
              SetAlarmOnRTC();
              WM_HideWindow(hAlarmAlert);
              break;

            case GUI_ID_BUTTON_ALARM_SNOOZE:              
              if(strstr(AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].alarmItems, "M") == NULL)
              {
                if(AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min > 55)
                {
                  AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min += 5;
                  AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min %= 60;
                  AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour += 1;
                  AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour %= 24;
                }
                else
                {
                  AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min += 5;
                }
                sprintf((char *)AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].alarmItems, "%s:%s", 
                        _apHours24[AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour], _apMinutes[AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min]);
              }
              else 
              {
                if(strstr(AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].alarmItems, "AM") != NULL)
                {
                  if(AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min > 55)
                  {
                    AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min += 5;
                    AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour += 1;
                    if(AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour > 12)
                    {
                      AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour %= 12;
                      sprintf((char *)AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].alarmItems, "%s:%s %s", 
                        _apHours12[AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour], _apMinutes[AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min], "AM");
                    }
                    else
                    {
                      if(AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour == 12)
                      {
                        sprintf((char *)AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].alarmItems, "%s:%s %s", 
                        _apHours12[AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour], _apMinutes[AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min], "PM");
                      }
                    }
                  }
                  else
                  {
                    AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min += 5;
                    sprintf((char *)AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].alarmItems, "%s:%s %s", 
                        _apHours12[AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour], _apMinutes[AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min], "AM");
                  }
                }
                else
                {
                  if(AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min > 55)
                  {
                    AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min += 5;
                    AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour += 1;
                    if(AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour > 12)
                    {
                      AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour %= 12;
                      sprintf((char *)AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].alarmItems, "%s:%s %s", 
                        _apHours12[AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour], _apMinutes[AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min], "PM");
                    }
                    else
                    {
                      if(AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour == 12)
                      {
                        sprintf((char *)AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].alarmItems, "%s:%s %s", 
                        _apHours12[AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour], _apMinutes[AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min], "AM");
                      }
                    }
                  }
                  else
                  {
                    AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min += 5;
                    sprintf((char *)AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].alarmItems, "%s:%s %s", 
                        _apHours12[AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour], _apMinutes[AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min], "PM");
                  }                  
                }
              }
                
              if(alarmIndex > 0)
              {
                for(i=(alarmIndex-1); i > 0; i--)
                {
                  CompareToPreviousAlarm(i);
                }
              }
              
              AlarmOnRTC.IndexAlarmOnRTC = IndexOfAlarmToSet();
              
              /* Now Program the new alarm on the RTC HW */              
              SetAlarmOnRTC();
              WM_HideWindow(hAlarmAlert);
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
  * @brief  Draw clock
  * @param  ClockNbre: which clock to draw  
  * @retval None
  */
static void GUI_DrawClock (int clockNbre)
{ 
  switch(clockNbre)
  {
    case 0:
      /* Clock 1 */
      GUI_DrawBitmap(&bmanalog_clock1_background, 0, 0);
      GUI_DrawBitmap(&bmanalog_clock1, 26, 26);
      break;
    case 1:
      /* Clock 2 */
      GUI_DrawBitmap(&bmanalog_clock2_background, 0, 0);
      GUI_DrawBitmap(&bmanalog_clock2_cercle, 147, 147);
      break;
    case 2:
      /* Clock 3 */
      GUI_DrawBitmap(&bmdigital_clock1_background, 0, 0);
      GUI_DrawBitmap(&bmdigital_clock1_background_day, 144, 20);
      break;
    case 3:
      /* Clock 4 */
      GUI_DrawBitmap(&bmdigital_clock2_background, 0, 0);
      GUI_DrawBitmap(&bmdigital_clock2_cercle_1, 15, 15);
      GUI_DrawBitmap(&bmdigital_clock2_cercle_2, 44, 44);   
      break;
  }  
}


/**
  * @brief  callback for Clocks drawing
  * @param  pMsg: Pointer to Date structure
  * @retval None
  */
static void _cbClocks(WM_MESSAGE * pMsg) 
{ 
  switch (pMsg->MsgId) 
  {
    case WM_TIMER:
      WM_InvalidateWindow(pMsg->hWin);
      WM_RestartTimer(pMsg->Data.v, 1000);
      break; 
    case WM_DELETE:
      WM_DeleteTimer(hTimerTime);
      break;
    case WM_CREATE:
      hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 1000, 0);
      break;
    
    case WM_PAINT:
      GUI_DrawClock(ClockChoice);
      GUI_UpdateClock(ClockChoice, 195, 195);
      break;
      
    case WM_TOUCH:
      WM_ShowWindow(hclockmainSettings);              
      break;    
    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/**
  * @brief  Callback routine of clock setting window.
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbClocksSettings(WM_MESSAGE * pMsg) 
{
  PARA      * pPara;
  int         i, a1000, Swap;
  I32         SinHQ;
  I32         CosHQ;
  I32         a;
  ITEM_INFO   ItemInfo;

  switch (pMsg->MsgId) 
  {
  case WM_PAINT:
    WM_GetUserData(pMsg->hWin, &pPara, sizeof(pPara));
    GUI_DrawBitmap(&bmbackground, 0, 0);

    /* Initialization of parameter structure */
    if (pPara->ySizeItem == 0) 
    {
      pPara->xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
      pPara->ySizeWindow = WM_GetWindowSizeY(pMsg->hWin);
      pPara->xSizeItem   = pPara->pBitmapItem[0].pBitmap->XSize;
      pPara->ySizeItem   = pPara->pBitmapItem[0].pBitmap->YSize;
      pPara->rx          = (pPara->ySizeWindow - pPara->xSizeItem + 50);
      pPara->ry          = (pPara->xSizeWindow - pPara->ySizeItem - 50) ;
      pPara->mx          = (pPara->xSizeItem + 25);
      pPara->my          = (pPara->ySizeItem - 174);
    }
    
    /* Calculate current positions of items */
    a1000 = (pPara->Pos * 3600) / pPara->NumItems;
    for (i = 0; i < pPara->NumItems; i++) 
    {
      a = a1000 + (i * 360000) / pPara->NumItems;
      SinHQ = GUI__SinHQ(a);
      CosHQ = GUI__CosHQ(a);
      pPara->pItemInfo[i].Index = i;
      pPara->pItemInfo[i].xPos  = pPara->mx - ((CosHQ * pPara->rx) >> 16);
      pPara->pItemInfo[i].yPos  = pPara->my + ((SinHQ * pPara->ry) >> 16);
    }
    
    /* Bubble sort items to be able to draw background items first */
    do 
    {
      Swap = 0;
      for (i = 0; i < (pPara->NumItems - 1); i++) 
      {
        if (pPara->pItemInfo[i].yPos > pPara->pItemInfo[i + 1].yPos) 
        {
          ItemInfo                = pPara->pItemInfo[i];
          pPara->pItemInfo[i]     = pPara->pItemInfo[i + 1];
          pPara->pItemInfo[i + 1] = ItemInfo;
          Swap = 1;
        }
      }
    } while (Swap);
    
    /* Draw items */
    for (i = 0; i < pPara->NumItems; i++) 
    {
      GUI_DrawBitmap((pPara->pBitmapItem + pPara->pItemInfo[i].Index)->pBitmap, pPara->pItemInfo[i].xPos - pPara->xSizeItem / 2, pPara->pItemInfo[i].yPos - pPara->ySizeItem / 2);
    }
    
    /* Draw item text only after final move */
    if (pPara->FinalMove | (ClockFirstTime == 0)) 
    {      
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetFont(GUI_FONT_24B_ASCII);
      GUI_SetColor(GUI_WHITE);
      ClockChoice = ((pPara->pItemInfo[pPara->NumItems - 4].Index ) % 4); 
      ClockFirstTime++;
      pPara->FinalMove = 0;
    }        
    break;
  case WM_NOTIFY_PARENT:
      switch (pMsg->Data.v) 
      {
         case WM_NOTIFICATION_RELEASED:
           switch (WM_GetId(pMsg->hWinSrc)) 
           {
             case ID_BUTTON_MODULE_CENTER:
               WM_ShowWindow(hMotion);
               WM_HideWindow(hclockmainSettings);               
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
  * @brief  Callback routine of transparent motion window which is responsible for processing the motion messages.
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbClockMotion(WM_MESSAGE * pMsg) 
{
  WM_MOTION_INFO * pInfo;
  PARA           * pPara;

  switch (pMsg->MsgId) 
  {
    case WM_MOTION:
      WM_GetUserData(pMsg->hWin, &pPara, sizeof(pPara));
      pInfo = (WM_MOTION_INFO *)pMsg->Data.p;
      switch (pInfo->Cmd) 
      {
        case WM_MOTION_INIT:
          pInfo->Flags = WM_CF_MOTION_X | WM_MOTION_MANAGE_BY_WINDOW;
          pInfo->SnapX = 100;
          break;
        case WM_MOTION_MOVE:
          pPara->FinalMove = pInfo->FinalMove;
          pPara->Pos += pInfo->dx;
          if(pPara->Pos > 100)
          {
            pInfo->StopMotion = 1;
          }
          if (pPara->Pos < 0)
          {
            pInfo->StopMotion = 1;
          }
          WM_Invalidate(WM_GetParent(pMsg->hWin));
          break;
        case WM_MOTION_GETPOS:
          pInfo->xPos = pPara->Pos;
          break;
      }
      break;
    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/**
  * @brief  callback for wheel owner draw
  * @param  pDrawItemInfo: pointer to the draw infos needed
  * @retval None
  */
static int _OwnerDraw(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) 
{
  WM_HWIN   hWin;
  WHEEL   * pWheel;
  int       xSizeWin;
  int       ySizeWin;

  hWin = pDrawItemInfo->hWin;
  
  /* Process messages */
  switch (pDrawItemInfo->Cmd) 
  {
    case WIDGET_ITEM_GET_XSIZE:
      /* Return x-size of item */
      return LISTWHEEL_OwnerDraw(pDrawItemInfo);
    case WIDGET_ITEM_GET_YSIZE:
      /* Return y-size of item */
      return LISTWHEEL_OwnerDraw(pDrawItemInfo);
    case WIDGET_ITEM_DRAW:
      /* Draw item */
      return LISTWHEEL_OwnerDraw(pDrawItemInfo);
    case WIDGET_ITEM_DRAW_BACKGROUND:
      xSizeWin = WM_GetWindowSizeX(hWin);
      ySizeWin = WM_GetWindowSizeY(hWin);
      
      /* Draw background */
      GUI_SetColor(GUI_TRANSPARENT);
      break;
    case WIDGET_ITEM_DRAW_OVERLAY:
      LISTWHEEL_GetUserData(hWin, &pWheel, sizeof(pWheel));
      xSizeWin = WM_GetWindowSizeX(hWin);
      ySizeWin = WM_GetWindowSizeY(hWin);
      
      GUI_EnableAlpha(1);
      
      GUI_SetColor(GUI_TRANSPARENT);
      GUI_FillRect(0, 0, xSizeWin - 1, ySizeWin - 1);
     
      break;
  }
  return 0;
}


/**
  * @brief  Function for a personilised liswheel creation
  * @param  x: x position
  * @param  y: y position
  * @param  xSize: x size
  * @param  ySize: y size
  * @param  Id: wheel id
  * @param  pText: text table used 
  * @param  NumItems: number of items of the table
  * @param  TextAlign: text align
  * @param  hParent: parent handle
  * @param  pWheel: wheel to use
  * @retval None
  */
static void _CreateListWheel(int x, int y, int xSize, int ySize, int Id, char ** apText, int NumItems, int TextAlign, WM_HWIN hParent, WHEEL * pWheel) {
  WM_HWIN hWin;
  int     i, LineHeight = 63;
  
  /* Create LISTWHEEL object */
  hWin          = LISTWHEEL_CreateUser(x, y, xSize, ySize, hParent, WM_CF_SHOW | WM_CF_HASTRANS, 0, Id, NULL, sizeof(void *));
  LISTWHEEL_SetFont(hWin, &GUI_Font32B_ASCII);
  LISTWHEEL_SetTextAlign(hWin, TextAlign);
  LISTWHEEL_SetSnapPosition(hWin, LineHeight);
  LISTWHEEL_SetOwnerDraw(hWin, _OwnerDraw);
  LISTWHEEL_SetUserData(hWin, &pWheel, sizeof(pWheel));
  LISTWHEEL_SetLineHeight(hWin, LineHeight); 

  LISTWHEEL_SetTextColor(hWin, LISTWHEEL_CI_SEL, GUI_WHITE);
  LISTWHEEL_SetTextColor(hWin, LISTWHEEL_CI_UNSEL, GUI_LIGHTBLUE);
  LISTWHEEL_SetBkColor(hWin, LISTWHEEL_CI_UNSEL, GUI_TRANSPARENT);
  LISTWHEEL_SetBkColor(hWin, LISTWHEEL_CI_SEL, GUI_TRANSPARENT);
                         
  for (i = 0; i < NumItems; i++)
  {
    LISTWHEEL_AddString(hWin, *(apText + i));
  }
  pWheel->hWin = hWin;
  return;
}

/**
  * @brief  callback for center button (No paint done)
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButtonNoPaint(WM_MESSAGE * pMsg) 
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
  * @brief  Paints for set time button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_SetTime(BUTTON_Handle hObj) 
{
  if((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_DrawBitmap(&bmclock_white_cercle_date, 0, 0);    
  }
  GUI_DrawBitmap(&bmclock_blue_cercle_date, 9, 9);
  GUI_DrawBitmap(&bmscreen_clock_time, 30, 30);  
}

/**
  * @brief  callback for time setting button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_SetTime(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_SetTime(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}


/**
  * @brief  Paints date setting button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_SetDate(BUTTON_Handle hObj) 
{
  if((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_DrawBitmap(&bmclock_white_cercle_date, 0, 0);    
  }
  GUI_DrawBitmap(&bmclock_blue_cercle_date, 9, 9);
  GUI_DrawBitmap(&bmscreen_clock_ic_date, 31, 31);  
}

/**
  * @brief  callback for date setting button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_SetDate(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_SetDate(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints select Time format 12 or 24H button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_Select12_24H(BUTTON_Handle hObj) 
{
  GUI_DrawBitmap(&bmrepeat_unselected, 0, 0);  
}

/**
  * @brief  callback for Time format 12 or 24H setting button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_Select12_24H(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_Select12_24H(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Callback routine of main time settings window
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbSetTime(WM_MESSAGE * pMsg) 
{
  int Id, Ncode;
  BUTTON_Handle hBut;
  
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    
    /* Main lists */
    if(IsTimeFormat24)
    {
      _CreateListWheel(82, 124 - 63,  97, 63 * 3, GUI_ID_LISTWHEEL1, _apHours24,   GUI_COUNTOF(_apHours24),   GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[0]);
      _CreateListWheel(212, 124 - 63, 97, 63 * 3, GUI_ID_LISTWHEEL2, _apMinutes, GUI_COUNTOF(_apMinutes), GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[1]);      
    }
    else
    {
      _CreateListWheel(32, 128 - 63,  97, 63 * 3, GUI_ID_LISTWHEEL1, _apHours12,   GUI_COUNTOF(_apHours12),   GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[0]);
      _CreateListWheel(148, 128 - 63, 97, 63 * 3, GUI_ID_LISTWHEEL2, _apMinutes, GUI_COUNTOF(_apMinutes), GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[1]);
      _CreateListWheel(265, 128 - 63,  97, 63 * 2, GUI_ID_LISTWHEEL3, _apAmPm,  GUI_COUNTOF(_apAmPm),  GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[2]);
    }
    hBut = BUTTON_CreateEx(98, 290, 75, 75, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_SET);
    WM_SetCallback(hBut, _cbButtonNoPaint);
    
    hBut = BUTTON_CreateEx(210, 289, 75, 75, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_CANCEL);
    WM_SetCallback(hBut, _cbButtonNoPaint);    

    break;

    case WM_PAINT:
      GUI_DrawBitmap(&bmbkc_setalarm, 0, 0);
      GUI_DrawBitmap(&bmshape_alarms, 85, 0);
      if(IsTimeFormat24)
      {
        GUI_DrawBitmap(&bmset_time_shape, 80, 122); 
        GUI_DrawBitmap(&bmset_time_shape, 210, 122);
      }
      else
      {
        GUI_DrawBitmap(&bmset_time_shape, 30, 126); 
        GUI_DrawBitmap(&bmset_time_shape, 146, 126);
        GUI_DrawBitmap(&bmset_time_shape, 263, 126);
      }
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetFont(&GUI_FontRobotoC_regular22);
      GUI_DispStringHCenterAt((char *)"Set time", 195, 15);      
      GUI_SetFont(&GUI_FontRobotoCn_bold27);
      GUI_DispStringHCenterAt((char *)"SET", 122, 310);
      GUI_DispStringHCenterAt((char *)"CANCEL", 268, 310);
      break;   
    case WM_NOTIFY_PARENT:
      Id = WM_GetId(pMsg->hWinSrc);
      Ncode = pMsg->Data.v;
      switch (Ncode) 
      {
        case WM_NOTIFICATION_SEL_CHANGED:
          Time.Hours = LISTWHEEL_GetItemFromPos(_aWheel[0].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[0].hWin)));          
          LISTWHEEL_SetSel(_aWheel[0].hWin, Time.Hours);          
          
          Time.Minutes = LISTWHEEL_GetItemFromPos(_aWheel[1].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[1].hWin)));
          LISTWHEEL_SetSel(_aWheel[1].hWin, Time.Minutes);
          
          if(!IsTimeFormat24)
          {
            Time.Hours++;
            IsAm = LISTWHEEL_GetItemFromPos(_aWheel[2].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[2].hWin)));
            LISTWHEEL_SetSel(_aWheel[2].hWin, IsAm);
            Time.Seconds = 1;
          }
          break;
        case WM_NOTIFICATION_RELEASED:
          switch(Id)
          {
            case GUI_ID_BUTTON_SET :
            case GUI_ID_BUTTON_CANCEL :
              k_SetTime(&Time);
              k_SetDate(&Date);
              GUI_EnableAlpha(0);
              WM_DeleteWindow(pMsg->hWin);
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
  * @brief  Callback routine of time format settings window
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbSetTimeFormat(WM_MESSAGE * pMsg) 
{
  int Id, Ncode;
  BUTTON_Handle hBut;
  
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    
    hBut = BUTTON_CreateEx(51, 196, 75, 40, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_12H);
    WM_SetCallback(hBut, _cbButton_Select12_24H);
    
    hBut = BUTTON_CreateEx(51, 124, 75, 40, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_24H);
    WM_SetCallback(hBut, _cbButton_Select12_24H);
    
    hBut = BUTTON_CreateEx(98, 290, 75, 75, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_SET);
    WM_SetCallback(hBut, _cbButtonNoPaint);
    
    hBut = BUTTON_CreateEx(210, 289, 75, 75, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_CANCEL);
    WM_SetCallback(hBut, _cbButtonNoPaint);    

    break;

    case WM_PAINT:
      GUI_DrawBitmap(&bmbkc_setalarm, 0, 0);
      GUI_DrawBitmap(&bmshape_alarms, 85, 0);
      if(TempTimeFormat24)
      {
        GUI_DrawBitmap(&bmrepeat_selected, 51, 124);
      }
      else
      {
        GUI_DrawBitmap(&bmrepeat_selected, 51, 196);
      }
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetFont(&GUI_FontRobotoC_regular22);
      GUI_DispStringHCenterAt((char *)"Time format", 195, 15);      
      GUI_DispStringAt((char *)"24-hour clock", 105, 134);
      GUI_DispStringAt((char *)"12-hour clock (AM/PM)", 105, 208);
      GUI_SetFont(&GUI_FontRobotoCn_bold27);
      GUI_DispStringHCenterAt((char *)"CONFIRM", 122, 310);
      GUI_DispStringHCenterAt((char *)"CANCEL", 268, 310);
      break;   
    case WM_NOTIFY_PARENT:
      Id = WM_GetId(pMsg->hWinSrc);
      Ncode = pMsg->Data.v;
      switch (Ncode) 
      {        
        case WM_NOTIFICATION_RELEASED:
          switch(Id)
          {
            case GUI_ID_BUTTON_24H:
              TempTimeFormat24 = 1;
              WM_InvalidateWindow(pMsg->hWin);
              break;
            case GUI_ID_BUTTON_12H:
              TempTimeFormat24 = 0;
              WM_InvalidateWindow(pMsg->hWin);
              break;              
            case GUI_ID_BUTTON_SET:
              if(TempTimeFormat24)
              {
                IsTimeFormat24 = 1;
                if(IsAm)
                {
                  if(Time.Hours != 12)
                    Time.Hours += 12;
                }
                else
                {
                  if(Time.Hours == 12)
                    Time.Hours = 0;
                }
              }
              else
              {
                IsTimeFormat24 = 0;
                if(Time.Hours >= 12)
                {
                  if(Time.Hours != 12)
                  {
                    Time.Hours -= 12;
                  }                  
                  IsAm = 1;
                }
                else
                {
                  if(Time.Hours == 0)
                  {
                    Time.Hours += 12;                    
                  }
                  IsAm = 0;
                }                 
              }
              /* Modify alarms table using the new time format, only if they exists */
              if(alarmIndex > 0)
              {
                SwitchAlarmFormat(IsTimeFormat24);
              }                
              WM_DeleteWindow(pMsg->hWin);
              hsetTime = WM_CreateWindow(0, 0, 390, 390, WM_CF_SHOW, _cbSetTime, 0);
              break;
            case GUI_ID_BUTTON_CANCEL :               
               WM_DeleteWindow(pMsg->hWin);
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
  * @brief  Callback routine of date settings window
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbSetDate(WM_MESSAGE * pMsg) 
{
  int Id, Ncode;
  int month      = 0;
  int year       = 0;
  int nbDays     = 0;
  int currentDay = 0;
  BUTTON_Handle hBut;
  
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    
    _CreateListWheel(32, 128 - 63,  97, 63 * 3, GUI_ID_LISTWHEEL1, _apDay,   GUI_COUNTOF(_apDay),   GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[0]);
    _CreateListWheel(148, 128 - 63, 97, 63 * 3, GUI_ID_LISTWHEEL2, _apMonth, GUI_COUNTOF(_apMonth), GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[1]);
    _CreateListWheel(265, 128 - 63,  97, 63 * 3, GUI_ID_LISTWHEEL3, _apYear,  GUI_COUNTOF(_apYear),  GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[2]);
    
    LISTWHEEL_SetPos(_aWheel[0].hWin, (Date.Date - 1));
    LISTWHEEL_SetSel(_aWheel[0].hWin, (Date.Date - 1));

    LISTWHEEL_SetPos(_aWheel[1].hWin, (Date.Month - 1));
    LISTWHEEL_SetSel(_aWheel[1].hWin, (Date.Month - 1));

    LISTWHEEL_SetPos(_aWheel[2].hWin, (Date.Year - 1));
    LISTWHEEL_SetSel(_aWheel[2].hWin, (Date.Year - 1)); 
    
    hBut = BUTTON_CreateEx(98, 290, 75, 75, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_SET);
    WM_SetCallback(hBut, _cbButtonNoPaint);
    
    hBut = BUTTON_CreateEx(210, 289, 75, 75, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_CANCEL);
    WM_SetCallback(hBut, _cbButtonNoPaint);    

    break;

    case WM_PAINT:
      GUI_DrawBitmap(&bmbkc_setalarm, 0, 0);
      GUI_DrawBitmap(&bmshape_alarms, 85, 0);
      GUI_DrawBitmap(&bmset_time_shape, 30, 126); 
      GUI_DrawBitmap(&bmset_time_shape, 146, 126);
      GUI_DrawBitmap(&bmset_time_shape, 263, 126);
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetFont(&GUI_FontRobotoC_regular22);
      GUI_DispStringHCenterAt((char *)"Set date", 195, 15);      
      GUI_SetFont(&GUI_FontRobotoCn_bold27);
      GUI_DispStringHCenterAt((char *)"SET", 122, 310);
      GUI_DispStringHCenterAt((char *)"CANCEL", 268, 310);
      break;   
    case WM_NOTIFY_PARENT:
      Id = WM_GetId(pMsg->hWinSrc);
      Ncode = pMsg->Data.v;
      switch (Ncode) 
      {
        case WM_NOTIFICATION_SEL_CHANGED:
          currentDay = 1 + LISTWHEEL_GetItemFromPos(_aWheel[0].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[0].hWin)));
          month      = 1 + LISTWHEEL_GetItemFromPos(_aWheel[1].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[1].hWin)));
          year       = 2000 + LISTWHEEL_GetItemFromPos(_aWheel[2].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[2].hWin)));
          nbDays     = GetNumberOfDays(month, year);
          
          /* If the selected month is FEB only 28 or 29 days have to be displayed in the list wheel */
          if (month == 2)
          {
            /* Only apply the changes if the displayed number of days need to be updated */
            if (LISTWHEEL_GetNumItems(_aWheel[0].hWin) != nbDays)
            {
              /* Delete the displayed days list weel */
              WM_DeleteWindow(_aWheel[0].hWin);

              /* Change the number of days depending on the year */
              if (nbDays == 28)
              {
                _CreateListWheel(32, 128 - 63,  97, 63 * 3, GUI_ID_LISTWHEEL1, _apDay28, GUI_COUNTOF(_apDay28), GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[0]);
              }
              else if (nbDays == 29)
              {
                _CreateListWheel(32, 128 - 63,  97, 63 * 3, GUI_ID_LISTWHEEL1, _apDay29, GUI_COUNTOF(_apDay29), GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[0]);
              }

              /* Restore the user selected day depending on the number of days */
              if (currentDay >= 29)
              {
                LISTWHEEL_SetPos(_aWheel[0].hWin, nbDays - 1);
              }
              else
              {
                LISTWHEEL_SetPos(_aWheel[0].hWin, currentDay - 1);
              }
            }
          }
          else
          {
            if (LISTWHEEL_GetNumItems(_aWheel[0].hWin) != nbDays)
            {
              /* Delete the displayed days list weel */
              WM_DeleteWindow(_aWheel[0].hWin);

              /* Change the number of days depending on the year */
              if (nbDays == 30)
              {
                _CreateListWheel(32, 128 - 63,  97, 63 * 3, GUI_ID_LISTWHEEL1, _apDay30, GUI_COUNTOF(_apDay30), GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[0]);
              }
              else if (nbDays == 31)
              {
                _CreateListWheel(32, 128 - 63,  97, 63 * 3, GUI_ID_LISTWHEEL1, _apDay31, GUI_COUNTOF(_apDay31), GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[0]);
              }

              /* Restore the user selected day depending on the number of days */
              if (currentDay == 31)
              {
                LISTWHEEL_SetPos(_aWheel[0].hWin, nbDays - 1);
              }
              else
              {
                LISTWHEEL_SetPos(_aWheel[0].hWin, currentDay - 1);
              }
            }
          }

          Date.Date = 1 + LISTWHEEL_GetItemFromPos(_aWheel[0].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[0].hWin)));          
          LISTWHEEL_SetSel(_aWheel[0].hWin, (Date.Date - 1));

          Date.Month = 1 + LISTWHEEL_GetItemFromPos(_aWheel[1].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[1].hWin)));
          LISTWHEEL_SetSel(_aWheel[1].hWin, (Date.Month - 1));

          Date.Year = 1 + LISTWHEEL_GetItemFromPos(_aWheel[2].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[2].hWin)));
          LISTWHEEL_SetSel(_aWheel[2].hWin, (Date.Year - 1));          

          break;
        case WM_NOTIFICATION_RELEASED:
          switch(Id)
          {
            case GUI_ID_BUTTON_SET :
              k_SetTime(&Time);
              k_SetDate(&Date);
              GUI_EnableAlpha(0);
              WM_DeleteWindow(pMsg->hWin);
              break;
            case GUI_ID_BUTTON_CANCEL :
              GUI_EnableAlpha(0);
              WM_DeleteWindow(pMsg->hWin);
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
  * @brief  Paints back button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_ButtonBack(BUTTON_Handle hObj)
{
  GUI_DrawBitmap(&bmbutton_back, 10, 25);
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
  * @brief  Paints add alarm button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_AddAlarm(BUTTON_Handle hObj)
{
  if (alarmIndex < 5)
  {
    GUI_DrawBitmap(&bmset_alarm_cercle_new_alarm, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmset_alarm_cercle_new_alarm_disabled, 0, 0);
  }

  GUI_DrawBitmap(&bmset_alarm_ic_new_alarm, 22, 22);
}

/**
  * @brief  callback for add alarm button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_AddAlarm(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_AddAlarm(pMsg->hWin);
      break;

    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints remove alarm button
  * @param  hObj: button handle
  * @retval None
  */
  static void _OnPaint_RemoveAlarm(BUTTON_Handle hObj)
  {
    if (alarmIndex > 0)
    {
      GUI_DrawBitmap(&bmset_alarm_cercle_new_alarm, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmset_alarm_cercle_new_alarm_disabled, 0, 0);
    }

    GUI_DrawBitmap(&bmset_alarm_ic_remove_alarm, 22, 35);
  }
  
  /**
    * @brief  callback for remove alarm button
    * @param  pMsg: pointer to data structure of type WM_MESSAGE
    * @retval None
    */
  static void _cbButton_RemoveAlarm(WM_MESSAGE * pMsg) 
  {
    switch (pMsg->MsgId) 
    {
      case WM_PAINT:
        _OnPaint_RemoveAlarm(pMsg->hWin);
        break;
  
      default:
        /* The original callback */
        BUTTON_Callback(pMsg);
        break;
    }
  }

/**
  * @brief  Callback routine of setting date and time window
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbSetDateAndTime(WM_MESSAGE * pMsg) 
{
  int Id, Ncode;
  BUTTON_Handle hBut;
  
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    
    hBut = BUTTON_CreateEx(76, 138, 96, 96, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_SET_DATE);
    WM_SetCallback(hBut, _cbButton_SetDate);
    
    hBut = BUTTON_CreateEx(212, 138, 96, 96, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_SET_TIME);
    WM_SetCallback(hBut, _cbButton_SetTime);
    
    hBut = BUTTON_CreateEx(143, 335, 90, 60, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_BACK);
    WM_SetCallback(hBut, _cbButtonBack);

    break;

    case WM_PAINT:
      GUI_DrawBitmap(&bmbackground, 0, 0);
      GUI_DrawBitmap(&bmshape_alarms, 85, 0);
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetFont(&GUI_FontRobotoC_regular22);
      GUI_DispStringHCenterAt((char *)"Settings", 195, 15);
      GUI_DispStringHCenterAt((char *)"Date", 134, 240);
      GUI_DispStringHCenterAt((char *)"Time", 260, 240);
      
      break;   
    case WM_NOTIFY_PARENT:
      Id = WM_GetId(pMsg->hWinSrc);
      Ncode = pMsg->Data.v;
      switch (Ncode) 
      {
        case WM_NOTIFICATION_RELEASED:
          switch(Id)
          {
            case GUI_ID_BUTTON_SET_DATE :
              hsetDate = WM_CreateWindow(0, 0, 390, 390, WM_CF_SHOW, _cbSetDate, 0);
              break;
              
            case GUI_ID_BUTTON_SET_TIME :
              hsetTimeFormat = WM_CreateWindow(0, 0, 390, 390, WM_CF_SHOW, _cbSetTimeFormat, 0);
              break;
               
            case GUI_ID_BUTTON_BACK :
              WM_DeleteWindow(pMsg->hWin);
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
  * @brief  Paints for alarm lock button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_alarmLock(BUTTON_Handle hObj) 
{
  int Id;
  
  Id = WM_GetId(hObj) - GUI_ID_BUTTON_ALARM_LOCK1;
   
  if(AlarmTable[Id].IsValid)
  {
    GUI_DrawBitmap(&bmset_alarm_unlocked, 0, 0);
  }
  else
  {    
    GUI_DrawBitmap(&bmset_alarm_locked, 0, 0);    
  }
}

/**
  * @brief  callback for alarm lock button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_AlarmLock(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_alarmLock(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints repeat button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_repeat(BUTTON_Handle hObj) 
{
  int Id;
  
  Id = WM_GetId(hObj) - GUI_ID_BUTTON_ALARM_REP1;
   
  if(AlarmTable[Id].repeat[0] == 0)
  {
    GUI_DrawBitmap(&bmrepeat_unselected, 0, 0);
  }
  else
  {    
    GUI_DrawBitmap(&bmrepeat_selected, 0, 0);    
  }
}

/**
  * @brief  callback for repeat button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_Repeat(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_repeat(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for repeat date button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButtonRepeatDate(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      /* Nothing to do */
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Callback routine of alarm repeat window
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbAlarmRepeat(WM_MESSAGE * pMsg) 
{
  int Id, Ncode;
  BUTTON_Handle hBut; 
  
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    
    hBut = BUTTON_CreateEx(286, 91, 62, 45, pMsg->hWin, WM_CF_SHOW, 0 , AlarmTable[IndexForRepeat].validButtonId);
    WM_SetCallback(hBut, _cbButton_AlarmLock);
    
    hBut = BUTTON_CreateEx(55, 172, 38, 38, pMsg->hWin, WM_CF_SHOW, 0 , AlarmTable[IndexForRepeat].repeatButtonId);
    WM_SetCallback(hBut, _cbButton_Repeat);
    
    hBut = BUTTON_CreateEx(59, 235, 45, 45, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_ALARM_MON);
    WM_SetCallback(hBut, _cbButtonRepeatDate);
    
    hBut = BUTTON_CreateEx(145, 235, 45, 45, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_ALARM_TUE);
    WM_SetCallback(hBut, _cbButtonRepeatDate);
    
    hBut = BUTTON_CreateEx(227, 235, 45, 45, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_ALARM_WED);
    WM_SetCallback(hBut, _cbButtonRepeatDate);
    
    hBut = BUTTON_CreateEx(312, 235, 45, 45, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_ALARM_THU);
    WM_SetCallback(hBut, _cbButtonRepeatDate);
    
    hBut = BUTTON_CreateEx(105, 285, 45, 45, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_ALARM_FRI);
    WM_SetCallback(hBut, _cbButtonRepeatDate);
    
    hBut = BUTTON_CreateEx(184, 285, 45, 45, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_ALARM_SAT);
    WM_SetCallback(hBut, _cbButtonRepeatDate);
    
    hBut = BUTTON_CreateEx(265, 285, 45, 45, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_ALARM_SUN);
    WM_SetCallback(hBut, _cbButtonRepeatDate);
    
    hBut = BUTTON_CreateEx(143, 335, 95, 55, pMsg->hWin, WM_CF_SHOW | WM_CF_STAYONTOP, 0 , GUI_ID_BUTTON_BACK);
    WM_SetCallback(hBut, _cbButtonBack); 

    break;

    case WM_PAINT:
      GUI_DrawBitmap(&bmbackground, 0, 0);
      GUI_DrawBitmap(&bmshape_alarms, 85, 0);
      GUI_DrawBitmap(&bmAlarmList, 44, 78);      
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetFont(&GUI_FontRobotoC_regular22);
      GUI_DispStringHCenterAt((char *)"Repeat", 195, 15);
      if(AlarmTable[IndexForRepeat].repeat[0] == 0)
      {
        GUI_DispStringAt((char *)"One time", 129, 120);
      }
      else
      {
        GUI_DispStringAt((char *)"Repeat", 129, 120);
        /* Monday */
        if(AlarmTable[IndexForRepeat].repeat[1])
          GUI_DrawBitmap(&bmrepeat_cercle_date, 59, 235);
        GUI_DrawBitmap(&bmmo_text, 68, 249);
        /* Tuesday */
        if(AlarmTable[IndexForRepeat].repeat[2])
          GUI_DrawBitmap(&bmrepeat_cercle_date, 145, 235);
        GUI_DrawBitmap(&bmtu_text, 156, 249);
        /* Wednesday */
        if(AlarmTable[IndexForRepeat].repeat[3])
          GUI_DrawBitmap(&bmrepeat_cercle_date, 227, 235);
        GUI_DrawBitmap(&bmwe_text, 236, 249);
        /* Thursday */        
        if(AlarmTable[IndexForRepeat].repeat[4])
          GUI_DrawBitmap(&bmrepeat_cercle_date, 312, 235);
        GUI_DrawBitmap(&bmth_text, 323, 249);
        /* Friday */
        if(AlarmTable[IndexForRepeat].repeat[5])
          GUI_DrawBitmap(&bmrepeat_cercle_date, 105, 285);
        GUI_DrawBitmap(&bmfr_text, 116, 299);
        /* Saturday */
        if(AlarmTable[IndexForRepeat].repeat[6])
          GUI_DrawBitmap(&bmrepeat_cercle_date, 184, 285);
        GUI_DrawBitmap(&bmsa_text, 195, 299);
        /* Sunday */
        if(AlarmTable[IndexForRepeat].repeat[7])
          GUI_DrawBitmap(&bmrepeat_cercle_date, 265, 285);
        GUI_DrawBitmap(&bmsu_text, 276, 299);       
      }
      GUI_SetFont(&GUI_FontRoboto_C_Regular35);
      GUI_DispStringAt((char *)AlarmTable[IndexForRepeat].alarmItems, 125, 78);
      
      
      break;   
    case WM_NOTIFY_PARENT:
      Id = WM_GetId(pMsg->hWinSrc);
      Ncode = pMsg->Data.v;
      switch (Ncode) 
      {
        case WM_NOTIFICATION_RELEASED:
          switch(Id)
          {
            case GUI_ID_BUTTON_ALARM_LOCK1:
            case GUI_ID_BUTTON_ALARM_LOCK2:
            case GUI_ID_BUTTON_ALARM_LOCK3:
            case GUI_ID_BUTTON_ALARM_LOCK4:
            case GUI_ID_BUTTON_ALARM_LOCK5:
              AlarmTable[IndexForRepeat].IsValid ^= 1;
              WM_InvalidateWindow(WM_GetDialogItem(hAlarms, GUI_ID_ALARM_LIST));
              break;
            case GUI_ID_BUTTON_ALARM_REP1:
            case GUI_ID_BUTTON_ALARM_REP2:
            case GUI_ID_BUTTON_ALARM_REP3:
            case GUI_ID_BUTTON_ALARM_REP4:
            case GUI_ID_BUTTON_ALARM_REP5:
              AlarmTable[IndexForRepeat].repeat[0] ^= 1;
              WM_InvalidateWindow(pMsg->hWin);
              WM_InvalidateWindow(WM_GetDialogItem(hAlarms, GUI_ID_ALARM_LIST));
              break;
            case GUI_ID_BUTTON_ALARM_MON:
              if(AlarmTable[IndexForRepeat].repeat[0] == 1)
                AlarmTable[IndexForRepeat].repeat[1] ^= 1;   
              WM_InvalidateWindow(pMsg->hWin);
              break;
            case GUI_ID_BUTTON_ALARM_TUE:
              if(AlarmTable[IndexForRepeat].repeat[0] == 1)
                AlarmTable[IndexForRepeat].repeat[2] ^= 1;
              WM_InvalidateWindow(pMsg->hWin);
              break;
            case GUI_ID_BUTTON_ALARM_WED:
              if(AlarmTable[IndexForRepeat].repeat[0] == 1)
                AlarmTable[IndexForRepeat].repeat[3] ^= 1;
              WM_InvalidateWindow(pMsg->hWin);
              break;
            case GUI_ID_BUTTON_ALARM_THU:
              if(AlarmTable[IndexForRepeat].repeat[0] == 1)
                AlarmTable[IndexForRepeat].repeat[4] ^= 1;
              WM_InvalidateWindow(pMsg->hWin);
              break;
            case GUI_ID_BUTTON_ALARM_FRI:
              if(AlarmTable[IndexForRepeat].repeat[0] == 1)
                AlarmTable[IndexForRepeat].repeat[5] ^= 1;
              WM_InvalidateWindow(pMsg->hWin);
              break;
            case GUI_ID_BUTTON_ALARM_SAT:
              if(AlarmTable[IndexForRepeat].repeat[0] == 1)
                AlarmTable[IndexForRepeat].repeat[6] ^= 1;
              WM_InvalidateWindow(pMsg->hWin);
              break;
            case GUI_ID_BUTTON_ALARM_SUN:
              if(AlarmTable[IndexForRepeat].repeat[0] == 1)
                AlarmTable[IndexForRepeat].repeat[7] ^= 1;
              WM_InvalidateWindow(pMsg->hWin);
              break;
            case GUI_ID_BUTTON_BACK :
              WM_InvalidateWindow(WM_GetParent(pMsg->hWin));
              WM_DeleteWindow(pMsg->hWin);
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
  * @brief  Own draw swipelist widget function
  * @param  pDrawItemInfo: pointer to the draw infos needed
  * @retval None
  */
static int _CreateAlarmSwipeList(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {

  switch (pDrawItemInfo->Cmd) 
  {
    case WIDGET_ITEM_GET_XSIZE:
      return SWIPELIST_OwnerDraw(pDrawItemInfo);
    case WIDGET_ITEM_GET_YSIZE:
      return SWIPELIST_OwnerDraw(pDrawItemInfo);
    case WIDGET_ITEM_DRAW_BACKGROUND:
      break;
    case WIDGET_ITEM_DRAW_TEXT:
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetTextAlign(GUI_TA_VCENTER | GUI_TA_HCENTER);
      return SWIPELIST_OwnerDraw(pDrawItemInfo);
  default:
    return SWIPELIST_OwnerDraw(pDrawItemInfo);
  }
  return 0;
}

/**
  * @brief  Alarm swipelist callback
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbSwipe(WM_MESSAGE * pMsg) 
{
  int Id, Ncode;
  
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      GUI_EnableAlpha(1);
      SWIPELIST_Callback(pMsg);
      break;
    case WM_NOTIFY_PARENT:
      Id = WM_GetId(pMsg->hWinSrc);
      Ncode = pMsg->Data.v;
      switch (Ncode) 
      {        
        case WM_NOTIFICATION_RELEASED:
          switch(Id)
          {
            case GUI_ID_BUTTON_ALARM1 :
            case GUI_ID_BUTTON_ALARM2 :
            case GUI_ID_BUTTON_ALARM3 :
            case GUI_ID_BUTTON_ALARM4 :
            case GUI_ID_BUTTON_ALARM5 :
              IndexForRepeat = Id - GUI_ID_BUTTON_ALARM1;
              GUI_EnableAlpha(0);
              hset_alarm = WM_CreateWindow(0, 0, 390, 390, WM_CF_SHOW, _cbAlarmRepeat, 0);
              break;
            case GUI_ID_BUTTON_ALARM_LOCK1:
            case GUI_ID_BUTTON_ALARM_LOCK2:
            case GUI_ID_BUTTON_ALARM_LOCK3:
            case GUI_ID_BUTTON_ALARM_LOCK4:
            case GUI_ID_BUTTON_ALARM_LOCK5:
              IndexForRepeat = Id - GUI_ID_BUTTON_ALARM_LOCK1;
              AlarmTable[IndexForRepeat].IsValid ^= 1;
              WM_InvalidateWindow(pMsg->hWin);
              break;
          }
          break;
      }
      break;
    default:
      SWIPELIST_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Callback routine of main alarm settings window
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbSetAlarm(WM_MESSAGE * pMsg) 
{
  int Id, Ncode, i;
  BUTTON_Handle hBut;
  static int AlarmHour, AlarmMinutes, AlarmAmPm;
  static char TempStr[10];  
  
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:    
    /* Main lists */
    if(IsTimeFormat24)
    {
      _CreateListWheel(82, 124 - 63,  97, 63 * 3, GUI_ID_LISTWHEEL1, _apHours24,   GUI_COUNTOF(_apHours24),   GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[0]);
      _CreateListWheel(212, 124 - 63, 97, 63 * 3, GUI_ID_LISTWHEEL2, _apMinutes, GUI_COUNTOF(_apMinutes), GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[1]);      
    }
    else
    {
      _CreateListWheel(32, 128 - 63,  97, 63 * 3, GUI_ID_LISTWHEEL1, _apHours12,   GUI_COUNTOF(_apHours12),   GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[0]);
      _CreateListWheel(148, 128 - 63, 97, 63 * 3, GUI_ID_LISTWHEEL2, _apMinutes, GUI_COUNTOF(_apMinutes), GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[1]);
      _CreateListWheel(265, 128 - 63,  97, 63 * 2, GUI_ID_LISTWHEEL3, _apAmPm,  GUI_COUNTOF(_apAmPm),  GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[2]);
    }
    hBut = BUTTON_CreateEx(98, 290, 75, 75, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_SET);
    WM_SetCallback(hBut, _cbButtonNoPaint);
    
    hBut = BUTTON_CreateEx(210, 289, 75, 75, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_CANCEL);
    WM_SetCallback(hBut, _cbButtonNoPaint);    

    break;

    case WM_PAINT:
      GUI_DrawBitmap(&bmbkc_setalarm, 0, 0);
      GUI_DrawBitmap(&bmshape_alarms, 85, 0);
      if(IsTimeFormat24)
      {
        GUI_DrawBitmap(&bmset_time_shape, 80, 122); 
        GUI_DrawBitmap(&bmset_time_shape, 210, 122);
      }
      else
      {
        GUI_DrawBitmap(&bmset_time_shape, 30, 126); 
        GUI_DrawBitmap(&bmset_time_shape, 146, 126);
        GUI_DrawBitmap(&bmset_time_shape, 263, 126);
      }
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetFont(&GUI_FontRobotoC_regular22);
      GUI_DispStringHCenterAt((char *)"Set alarm", 195, 15);      
      GUI_SetFont(&GUI_FontRobotoCn_bold27);
      GUI_DispStringHCenterAt((char *)"SET", 122, 310);
      GUI_DispStringHCenterAt((char *)"CANCEL", 268, 310);
      break;   
    case WM_NOTIFY_PARENT:
      Id = WM_GetId(pMsg->hWinSrc);
      Ncode = pMsg->Data.v;
      switch (Ncode) 
      {
        case WM_NOTIFICATION_SEL_CHANGED:
          AlarmHour = LISTWHEEL_GetItemFromPos(_aWheel[0].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[0].hWin)));          
          LISTWHEEL_SetSel(_aWheel[0].hWin, AlarmHour);
          
          AlarmMinutes = LISTWHEEL_GetItemFromPos(_aWheel[1].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[1].hWin)));
          LISTWHEEL_SetSel(_aWheel[1].hWin, AlarmMinutes);
          
          if(!IsTimeFormat24)
          {
            AlarmAmPm = LISTWHEEL_GetItemFromPos(_aWheel[2].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[2].hWin)));
            LISTWHEEL_SetSel(_aWheel[2].hWin, AlarmAmPm);            
          }
          break;
        case WM_NOTIFICATION_RELEASED:
          switch(Id)
          {
            case GUI_ID_BUTTON_SET :
              /* Set the needed alarm before leaving */
              if(!IsTimeFormat24)
              {
                if(!AlarmAmPm)
                {
                  sprintf((char *)TempStr, "AM");
                }
                else
                {
                  sprintf((char *)TempStr, "PM");
                }
                sprintf((char *)AlarmTable[alarmIndex].alarmItems, "%s:%s %s", _apHours12[AlarmHour], _apMinutes[AlarmMinutes], TempStr);
              }
              else
              {                
                sprintf((char *)AlarmTable[alarmIndex].alarmItems, "%s:%s", _apHours24[AlarmHour], _apMinutes[AlarmMinutes]);
              }
              AlarmTable[alarmIndex].hour = AlarmHour;
              AlarmTable[alarmIndex].min = AlarmMinutes;
              AlarmTable[alarmIndex].added = 1;
              AlarmTable[alarmIndex].buttonId = (GUI_ID_BUTTON_ALARM1 + alarmIndex);
              AlarmTable[alarmIndex].validButtonId = (GUI_ID_BUTTON_ALARM_LOCK1 + alarmIndex);
              AlarmTable[alarmIndex].repeatButtonId = (GUI_ID_BUTTON_ALARM_REP1 + alarmIndex);
              AlarmTable[alarmIndex].IsValid = 1;
              for(i = 0; i < 7; i++)
              {
                AlarmTable[alarmIndex].repeat[i] = 0;
              }
              if(alarmIndex >= 1)
              {
                for(i=alarmIndex; i > 0; i--)
                {
                  CompareToPreviousAlarm(i);
                }
              }
              alarmIndex++;
              
              AlarmOnRTC.IndexAlarmOnRTC = IndexOfAlarmToSet();
              
              /* Now Program the right alarm on the RTC HW */              
              SetAlarmOnRTC();
              
              /* Reset the alpha and delete the window in use */
              GUI_EnableAlpha(0);
              WM_DeleteWindow(pMsg->hWin);
              /* Create the Alarms list window */
              hAlarms = WM_CreateWindow(0, 0, 390, 390, WM_CF_SHOW, _cbAlarms, 0);
              break;
            case GUI_ID_BUTTON_CANCEL :
               GUI_EnableAlpha(0);
               WM_DeleteWindow(pMsg->hWin);
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
  * @brief  Callback routine of alarm listing window
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbAlarms(WM_MESSAGE * pMsg) 
{
  int Id, Ncode, i;
  BUTTON_Handle hBut;

  switch (pMsg->MsgId) 
  { 
    case WM_CREATE:
      /* Set different proprieties of the swipelist */
      SWIPELIST_SetDefaultSepSize(0);
   
      /* Create SwipeList */
      hSwipelist = SWIPELIST_CreateEx(42, 195, 306, 145, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, 0, GUI_ID_ALARM_LIST);
      WM_SetCallback(hSwipelist, _cbSwipe);
      
      SWIPELIST_SetOwnerDraw(hSwipelist, _CreateAlarmSwipeList);     
      
      SWIPELIST_SetFont(hSwipelist, SWIPELIST_FI_ITEM_HEADER, &GUI_FontRoboto_C_Regular35);
      SWIPELIST_SetFont(hSwipelist, SWIPELIST_FI_ITEM_TEXT, &GUI_FontRobotoC_regular22);
           
      SWIPELIST_SetBitmapSpace(hSwipelist, 30);
    
      for (i = 0; i < alarmIndex; i++) 
      {        
        SWIPELIST_AddItem(hSwipelist, AlarmTable[i].alarmItems, 35);
        if(AlarmTable[i].repeat[0] == 0)
        {
          SWIPELIST_AddItemText(hSwipelist, i, "One time");
        }
        else
        {
          SWIPELIST_AddItemText(hSwipelist, i, "Repeat");
        }
        hBut = BUTTON_CreateEx(0, 0, 66, 66, pMsg->hWin, WM_CF_SHOW, 0 , AlarmTable[i].buttonId);
        WM_SetCallback(hBut, _cbButtonNoPaint);
        SWIPELIST_ItemAttachWindow(hSwipelist, i, hBut, 0, 0);
        SWIPELIST_SetBitmap(hSwipelist, i, SWIPELIST_BA_LEFT , &bmAlarmList);
        
        hBut = BUTTON_CreateEx(0, 0, 62, 45, pMsg->hWin, WM_CF_SHOW, 0 , AlarmTable[i].validButtonId);
        WM_SetCallback(hBut, _cbButton_AlarmLock);
        SWIPELIST_ItemAttachWindow(hSwipelist, i, hBut, 244, 20);
      }     
      

      hBut = BUTTON_CreateEx(50, 91, 76, 76, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_ADD_ALARM);
      WM_SetCallback(hBut, _cbButton_AddAlarm);

      hBut = BUTTON_CreateEx(264, 91, 76, 76, pMsg->hWin, WM_CF_SHOW, 0 , GUI_ID_BUTTON_REMOVE_ALARM);
      WM_SetCallback(hBut, _cbButton_RemoveAlarm);

      hBut = BUTTON_CreateEx(143, 335, 90, 60, pMsg->hWin, WM_CF_SHOW | WM_CF_STAYONTOP, 0 , GUI_ID_BUTTON_BACK);
      WM_SetCallback(hBut, _cbButtonBack);
      break;

    case WM_PAINT:
      GUI_DrawBitmap(&bmbackground, 0, 0);
      GUI_DrawBitmap(&bmshape_alarms, 85, 0);
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetFont(&GUI_FontRobotoC_regular22);
      GUI_DispStringHCenterAt((char *)"Alarms", 195, 15);
      GUI_SetFont(&GUI_FontRoboto_C_Regular35);
      GUI_DispStringAt((char *)"Alarm", 155, 113);
      break;  
      
    case WM_NOTIFY_PARENT:
      Id = WM_GetId(pMsg->hWinSrc);
      Ncode = pMsg->Data.v;
      switch (Ncode) 
      {        
        case WM_NOTIFICATION_RELEASED:
          switch(Id)
          {
            case GUI_ID_BUTTON_BACK :
              GUI_EnableAlpha(0);
              WM_DeleteWindow(pMsg->hWin);
              break;
            case GUI_ID_BUTTON_ADD_ALARM :
              if (alarmIndex < 5)
              {
                GUI_EnableAlpha(0);
                WM_DeleteWindow(pMsg->hWin);
                hset_alarm = WM_CreateWindow(0, 0, 390, 390, WM_CF_SHOW, _cbSetAlarm, 0);
              }
              break;
            case GUI_ID_BUTTON_REMOVE_ALARM :
              if (alarmIndex > 0)
              {
                /* Reset the alarm table */
                AlarmTable[alarmIndex - 1].hour           = 0;
                AlarmTable[alarmIndex - 1].min            = 0;
                AlarmTable[alarmIndex - 1].added          = 0;
                AlarmTable[alarmIndex - 1].buttonId       = 0;
                AlarmTable[alarmIndex - 1].validButtonId  = 0;
                AlarmTable[alarmIndex - 1].repeatButtonId = 0;
                AlarmTable[alarmIndex - 1].IsValid        = 0;

                /* Delete the alarm from the swipe list */
                SWIPELIST_SetScrollPos(hSwipelist, 0);
                SWIPELIST_DeleteItem(hSwipelist, alarmIndex - 1);

                alarmIndex--;

                if (alarmIndex > 0)
                {
                  for (i = (alarmIndex - 1); i > 0; i--)
                  {
                    CompareToPreviousAlarm(i);
                  }
                }

                AlarmOnRTC.IndexAlarmOnRTC = IndexOfAlarmToSet();

                /* Now Program the new alarm on the RTC HW */
                SetAlarmOnRTC();

                WM_InvalidateWindow(hSwipelist);
                WM_Update(hSwipelist);
              }

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
  * @brief  Callback routine of second clock settings window
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbClocksSecSettings(WM_MESSAGE * pMsg) 
{
  const GUI_PID_STATE * pState;
  static int arrowUp = 0;
  
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      if(arrowUp == 0)
      {
        GUI_DrawBitmap(&bmswipe_up_arrow_up, 102, 0);
      }
      else
      {
        GUI_DrawBitmap(&bmswipe_up_arrow, 102, 256);
      }
      GUI_DrawBitmap(&bmswipe_up_settings_background, 0, 22);      
      break;
      
    case WM_TOUCH:  
      pState = (const GUI_PID_STATE *)pMsg->Data.p;
      if (pState) 
      {
        if (!pState->Pressed)
        {
          if(WM_GetWindowOrgY(hclockSecSettings) == 288)
          {
            arrowUp = 1;            
            WM_MoveTo(hclockSecSettings, 85, 105);
            WM_MakeModal(hclockSecSettings);
          }
          else
          {
            arrowUp = 0;
            WM_MoveTo(hclockSecSettings, 85, 288);
            WM_MakeModal(0);
          }          
        }
      }
      break;
    case WM_NOTIFY_PARENT:
      switch (pMsg->Data.v) 
      {
         case WM_NOTIFICATION_RELEASED:
           switch (WM_GetId(pMsg->hWinSrc)) 
           {
             case ID_BUTTON_SEC_SETTING_1:
               arrowUp = 0;
               WM_SetWindowPos(hclockSecSettings, 85, 288, 221, 268);
               WM_MakeModal(0);
               hsetDateAndTime = WM_CreateWindow(0, 0, 390, 390, WM_CF_SHOW, _cbSetDateAndTime, 0);
               break;
             case ID_BUTTON_SEC_SETTING_2:
               arrowUp = 0;
               WM_SetWindowPos(hclockSecSettings, 85, 288, 221, 268);
               WM_MakeModal(0);
               if(AlarmTable[0].added == 0)
               {
                 hset_alarm = WM_CreateWindow(0, 0, 390, 390, WM_CF_SHOW, _cbSetAlarm, 0);
               }
               else
               {
                 hAlarms = WM_CreateWindow(0, 0, 390, 390, WM_CF_SHOW, _cbAlarms, 0);                 
               }
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
  * @brief  Paints settings button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_settings(BUTTON_Handle hObj) 
{
  if((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_DrawBitmap(&bmsettings_blue_cercle_selected, 0, 0);
    GUI_DrawBitmap(&bmsettings_white_cercle_selected, 10, 10);    
    GUI_DrawBitmap(&bmic_settings_selected, 25, 25);    
  }
  else
  {    
    GUI_DrawBitmap(&bmsettings_blue_cercle_selected, 0, 0);
    GUI_DrawBitmap(&bmic_settings_unselected, 25, 25);    
  }
}

/**
  * @brief  callback for settings button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_settings(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_settings(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints alarm button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_alarm(BUTTON_Handle hObj) 
{ 
  if((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_DrawBitmap(&bmsettings_blue_cercle_selected, 0, 0);
    GUI_DrawBitmap(&bmsettings_white_cercle_selected, 10, 10);    
    GUI_DrawBitmap(&bmnew_alarm_icon_selected, 25, 25);    
  }
  else
  {    
     GUI_DrawBitmap(&bmsettings_blue_cercle_selected, 0, 0);
     GUI_DrawBitmap(&bmnew_alarm_icon, 25, 25);    
  }
}

/**
  * @brief  callback for alarm button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_alarm(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_alarm(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
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
  static ITEM_INFO  aItemInfo[GUI_COUNTOF(_aBitmapItem)];
  static PARA       Para;
  PARA            * pPara;
  BUTTON_Handle     hBut;
  
  /* Initialize parameter structure for items to be shown */
  pPara              = &Para;
  pPara->NumItems    = GUI_COUNTOF(_aBitmapItem);
  pPara->pBitmapItem = _aBitmapItem;
  pPara->pItemInfo   = aItemInfo;
  pPara->Pos         = 0;
  
  hclockmain          = WM_CreateWindowAsChild(xpos, ypos, 390, 390, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, _cbClocks, 0);  
  hclockmainSettings  = WM_CreateWindowAsChild(0, 0, 390, 390, hclockmain, WM_CF_SHOW, _cbClocksSettings, sizeof(pPara));
  WM_HideWindow(hclockmainSettings);
  hMotionClocks       = WM_CreateWindowAsChild(0, 0, 390, 280, hclockmainSettings, WM_CF_SHOW | WM_CF_HASTRANS, _cbClockMotion, sizeof(pPara));

  hBut = BUTTON_CreateEx(145, 120, 100, 100, hclockmainSettings, WM_CF_SHOW, 0 , ID_BUTTON_MODULE_CENTER);
  WM_SetCallback(hBut, _cbButtonNoPaint);
  
  /* Add pointer to parameter structure to windows */
  WM_SetUserData(hclockmainSettings,   &pPara, sizeof(pPara));
  WM_SetUserData(hMotionClocks, &pPara, sizeof(pPara));
  
  hclockSecSettings  = WM_CreateWindowAsChild(85, 288, 221, 268, hclockmainSettings, WM_CF_SHOW | WM_CF_HASTRANS, _cbClocksSecSettings, 0);
  
  hBut = BUTTON_CreateEx(71, 48, 78, 78, hclockSecSettings, WM_CF_SHOW, 0 , ID_BUTTON_SEC_SETTING_1);
  WM_SetCallback(hBut, _cbButton_settings);
  hBut = BUTTON_CreateEx(71, 153, 78, 78, hclockSecSettings, WM_CF_SHOW, 0 , ID_BUTTON_SEC_SETTING_2);
  WM_SetCallback(hBut, _cbButton_alarm);
  
  /* Initialise the alarms table */
  memset((void *)AlarmTable, 0x0, sizeof(AlarmTable));
  
  hAlarmAlert = WM_CreateWindowAsChild(0, 0, 390, 390, hWin, WM_CF_SHOW | WM_CF_STAYONTOP, _cbAlarmAlert, 0); 
  WM_HideWindow(hAlarmAlert);

  hBut = BUTTON_CreateEx(210, 290, 84, 70, hAlarmAlert, WM_CF_SHOW, 0 , GUI_ID_BUTTON_ALARM_DISMISS);
  WM_SetCallback(hBut, _cbButtonNoPaint);

  hBut = BUTTON_CreateEx(98, 290, 84, 70, hAlarmAlert, WM_CF_SHOW, 0 , GUI_ID_BUTTON_ALARM_SNOOZE);
  WM_SetCallback(hBut, _cbButtonNoPaint);
  
  /* Initialise the alarmOnRTC stucture */
  AlarmOnRTC.IndexAlarmOnRTC = (-1);
  AlarmOnRTC.Alarm_A_IsSet = 0;
  AlarmOnRTC.Alarm_B_IsSet = 0;  
}


/**
  * @}
  */

/**
  * @}
  */

