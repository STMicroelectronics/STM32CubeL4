/**
  ******************************************************************************
  * @file    k_rtc.c
  * @author  MCD Application Team   
  * @brief   This file provides the kernel rtc functions 
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
#include "k_rtc.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_RTC
  * @brief Kernel rtc routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
#define RTC_SYNCH_PREDIV   0x00FF /* LSE as RTC clock */
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static k_AlarmCallback AlarmCallback;

RTC_HandleTypeDef RtcHandle;
RTC_TimeTypeDef   RTC_TimeStructure;
RTC_DateTypeDef   RTC_DateStructure;

/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
void k_CalendarBkupInit(void)
{
  
  /*##-1- Configure the RTC peripheral #######################################*/
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follow:
  - Hour Format    = Format 24
  - Asynch Prediv  = Value according to source clock
  - Synch Prediv   = Value according to source clock
  - OutPut         = Output Disable
  - OutPutPolarity = High Polarity
  - OutPutType     = Open Drain */ 
  RtcHandle.Instance = RTC;
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  DeConfigure the current time and date.
  * @param  None
  * @retval None
  */
void k_CalendarBkupDeInit(void)
{
  if(HAL_RTC_DeInit(&RtcHandle) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief  Backup save parameter 
  * @param  address: RTC Backup data Register number.
  *                  This parameter can be: RTC_BKP_DRx where x can be from 0 to 19 to 
  *                                         specify the register.
  * @param  Data:    Data to be written in the specified RTC Backup data register.
  * @retval None
  */
void k_BkupSaveParameter(uint32_t address, uint32_t data)
{
  HAL_RTCEx_BKUPWrite(&RtcHandle,address,data);  
}

/**
  * @brief  Backup restore parameter. 
  * @param  address: RTC Backup data Register number.
  *                  This parameter can be: RTC_BKP_DRx where x can be from 0 to 19 to 
  *                                         specify the register. 
  * @retval None
  */
uint32_t k_BkupRestoreParameter(uint32_t address)
{
   return HAL_RTCEx_BKUPRead(&RtcHandle,address);  
}

/**
  * @brief  RTC Get time. 
  * @param  Time: Pointer to Time structure
  * @retval None
  */
void k_GetTime(  RTC_TimeTypeDef *Time)
{
   HAL_RTC_GetTime(&RtcHandle, Time, RTC_FORMAT_BIN);
}

/**
  * @brief  RTC Set time. 
  * @param  Time: Pointer to Time structure
  * @retval None
  */
void k_SetTime(  RTC_TimeTypeDef *Time)
{
   Time->StoreOperation = 0;
   Time->SubSeconds = 0;
   Time->DayLightSaving = 0;
   HAL_RTC_SetTime(&RtcHandle, Time, RTC_FORMAT_BIN);
}

/**
  * @brief  RTC Get date
  * @param  Date: Pointer to Date structure
  * @retval None
  */
void k_GetDate(  RTC_DateTypeDef *Date)
{
   HAL_RTC_GetDate(&RtcHandle, Date, RTC_FORMAT_BIN);
   
   if((Date->Date == 0) || (Date->Month == 0))
   {
     Date->Date = Date->Month = 1;
   }    
}

/**
  * @brief  RTC Set date
  * @param  Date: Pointer to Date structure
  * @retval None
  */
void k_SetDate(  RTC_DateTypeDef *Date)
{
   HAL_RTC_SetDate(&RtcHandle, Date, RTC_FORMAT_BIN);
}

/**
  * @brief  RTC Set alarm
  * @param  Alarm: Pointer to Alarm structure
  * @retval None
  */
void k_SetAlarm(RTC_AlarmTypeDef *Alarm)
{
  if (HAL_RTC_SetAlarm_IT(&RtcHandle, Alarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
}

/**
  * @brief  RTC Deactivate alarm
  *  @param  Alarm: Specifies the Alarm.
  *          This parameter can be one of the following values:
  *            @arg RTC_ALARM_A:  AlarmA
  *            @arg RTC_ALARM_B:  AlarmB
  * @retval None
  */
void k_DeactivateAlarm(uint32_t Alarm)
{
  if (HAL_RTC_DeactivateAlarm(&RtcHandle, Alarm) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
}

/**
  * @brief  Alarm callback
  * @param  hrtc : RTC handle
  * @retval None
  */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
  /* Turn LED1 on: Alarm generation */
  BSP_LED_On(LED1);

  if (AlarmCallback != NULL)
  {
    AlarmCallback();
  }
}

void k_SetAlarmCallback (k_AlarmCallback alarmCallback)
{
  AlarmCallback = alarmCallback;
}

/**
  * @brief  Set the RTC
  * @param  None
  * @retval None
  */
void Set_RTC(void)
{
  RTC_TimeStructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  RTC_TimeStructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  k_SetTime(&RTC_TimeStructure);
  k_SetDate(&RTC_DateStructure);

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(&RtcHandle);
}

/**
  * @brief  Backup the RTC
  * @param  None
  * @retval None
  */
void BackUp_RTC(void)
{
  uint32_t leap_year = 0;
  
  k_GetTime(&RTC_TimeStructure);
  k_GetDate(&RTC_DateStructure);
    
  /* Update time based in case of Idd measurements */
  RTC_TimeStructure.Seconds = RTC_TimeStructure.Seconds +3; 
  if (RTC_TimeStructure.Seconds >= 60)
  {
    RTC_TimeStructure.Seconds -= 60;
    RTC_TimeStructure.Minutes++;
    if (RTC_TimeStructure.Minutes == 60)
    {
      RTC_TimeStructure.Minutes = 0;
      RTC_TimeStructure.Hours++;
      /* Time format is 24-hour */
      if (RTC_TimeStructure.Hours == 24)
      {
        RTC_TimeStructure.Hours = 0;
        RTC_DateStructure.Date++;
        RTC_DateStructure.WeekDay++;
        if (RTC_DateStructure.WeekDay == RTC_WEEKDAY_SUNDAY+1)
        {
          RTC_DateStructure.WeekDay = RTC_WEEKDAY_MONDAY; 
        }
        if ((((RTC_DateStructure.Year % 4) == 0) && ((RTC_DateStructure.Year % 100) != 0))
           || ((RTC_DateStructure.Year % 400) == 0))
        {
          leap_year = 1;  
        }        
        if (((RTC_DateStructure.Date == 29) && (leap_year ==0))
          || (RTC_DateStructure.Date == 30))
        {
          if (RTC_DateStructure.Month == 2)
          {
            RTC_DateStructure.Date = 1;
            RTC_DateStructure.Month = 3;            
          }
        }
        else if (RTC_DateStructure.Date == 31)
        {
          if ((RTC_DateStructure.Month == 4)     || \
              (RTC_DateStructure.Month == 6)      || \
              (RTC_DateStructure.Month == 9) || \
              (RTC_DateStructure.Month == 11))
          {
            RTC_DateStructure.Date = 1;
            RTC_DateStructure.Month++;            
          }      
        }
        else if (RTC_DateStructure.Date == 32)
        {
          if ((RTC_DateStructure.Month == 1) || \
              (RTC_DateStructure.Month == 3)   || \
              (RTC_DateStructure.Month == 5)     || \
              (RTC_DateStructure.Month == 7)    || \
              (RTC_DateStructure.Month == 8)  || \
              (RTC_DateStructure.Month == 10) || \
              (RTC_DateStructure.Month == 12))
          {
            RTC_DateStructure.Date = 1;
            if (RTC_DateStructure.Month == 12)
            {
              RTC_DateStructure.Month = 1;
              RTC_DateStructure.Year++;  
            }
            else
            {
              RTC_DateStructure.Month++;      
            }
          }      
        } 
      }
    }
  }
    
  
    k_BkupSaveParameter(RTC_BKP_DR2, RTC_TimeStructure.Hours);
    k_BkupSaveParameter(RTC_BKP_DR3, RTC_TimeStructure.Minutes);
    k_BkupSaveParameter(RTC_BKP_DR4, RTC_TimeStructure.Seconds);
    k_BkupSaveParameter(RTC_BKP_DR5, RTC_DateStructure.Date);
    k_BkupSaveParameter(RTC_BKP_DR6, RTC_DateStructure.Month);
    k_BkupSaveParameter(RTC_BKP_DR7, RTC_DateStructure.Year);
    k_BkupSaveParameter(RTC_BKP_DR8, RTC_DateStructure.WeekDay);   
}

/**
  * @brief  Restore the RTC
  * @param  None
  * @retval None
  */
void Restore_RTC(void)
{
  RTC_TimeStructure.Hours   = k_BkupRestoreParameter(RTC_BKP_DR2);
  RTC_TimeStructure.Minutes = k_BkupRestoreParameter(RTC_BKP_DR3);
  RTC_TimeStructure.Seconds = k_BkupRestoreParameter(RTC_BKP_DR4);
  RTC_DateStructure.Date    = k_BkupRestoreParameter(RTC_BKP_DR5);
  RTC_DateStructure.Month   = k_BkupRestoreParameter(RTC_BKP_DR6);
  RTC_DateStructure.Year    = k_BkupRestoreParameter(RTC_BKP_DR7);
  RTC_DateStructure.WeekDay = k_BkupRestoreParameter(RTC_BKP_DR8);  
  
  Set_RTC(); 
}

/**
  * @}
  */

/**
  * @}
  */

