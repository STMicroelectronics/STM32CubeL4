/**
 ******************************************************************************
 * @file    scheduler.c
 * @author  MCD Application Team
 * @brief   Timer Server
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license SLA0044,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        http://www.st.com/SLA0044
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "common.h"

#include "scheduler.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if( CORE_CM0PLUS == 1 )
static const uint8_t clz_table_4bit[16] = { 4, 3, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
#endif
static uint32_t TaskSet = 0;
static uint32_t TaskMask = (~0);
static uint32_t EvtSet = 0;
static uint32_t EvtWaited = 0;
static void (*TaskCb[CFG_SCH_TASK_NBR])( void );


/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
#if( CORE_CM0PLUS == 1)
static uint32_t CountLeadZero(uint32_t value);
#endif

/* Functions Definition ------------------------------------------------------*/

void SCH_Run( void )
{
  uint32_t bit_nbr;
  uint32_t primask_bit;

  while( TaskSet &  TaskMask )
  {
#if( CORE_CM0PLUS == 0 )
    bit_nbr =__CLZ( TaskSet &  TaskMask );
#else
    bit_nbr = CountLeadZero( TaskSet &  TaskMask );
#endif

    primask_bit = __get_PRIMASK();  /**< backup PRIMASK bit */
    __disable_irq();
    TaskSet &= ~( 1 << (31 - bit_nbr) );
    __set_PRIMASK(primask_bit);     /**< Restore PRIMASK bit*/

    TaskCb[31 - bit_nbr]();
  }

  primask_bit = __get_PRIMASK();  /**< backup PRIMASK bit */
  __disable_irq();

  if ( !( (TaskSet & TaskMask ) || (EvtSet & EvtWaited) ) )
  {
    SCH_Idle();
  }
  __set_PRIMASK(primask_bit);     /**< Restore PRIMASK bit*/

  return;
}


void SCH_RegTask( uint32_t task_id, void (*task)(void) )
{
  /**
   * The backup/restore PRIMASK mechanism allow the application to call this API from a critical section protected with
   * the PRIMASK bit (__disable_irq() / __enable_irq())
   */
  uint32_t primask_bit;

  primask_bit = __get_PRIMASK();  /**< backup PRIMASK bit */
  __disable_irq();

  TaskCb[task_id] = task;

  __set_PRIMASK(primask_bit);     /**< Restore PRIMASK bit*/

  return;
}

void SCH_SetTask( uint32_t task_id )
{
  /**
   * The backup/restore PRIMASK mechanism allow the application to call this API from a critical section protected with
   * the PRIMASK bit (__disable_irq() / __enable_irq())
   */
  uint32_t primask_bit;

  primask_bit = __get_PRIMASK();  /**< backup PRIMASK bit */
  __disable_irq();

  TaskSet |= (1 << task_id);

  __set_PRIMASK(primask_bit);     /**< Restore PRIMASK bit*/

  return;
}

void SCH_PauseTask( uint32_t task_id )
{

  /**
   * The backup/restore PRIMASK mechanism allow the application to call this API from a critical section protected with
   * the PRIMASK bit (__disable_irq() / __enable_irq())
   */
  uint32_t primask_bit;

  primask_bit = __get_PRIMASK();  /**< backup PRIMASK bit */
  __disable_irq();

  TaskMask &= ~ ( 1 << task_id );

  __set_PRIMASK(primask_bit);     /**< Restore PRIMASK bit*/

  return;
}

void SCH_ResumeTask( uint32_t task_id )
{

  /**
   * The backup/restore PRIMASK mechanism allow the application to call this API from a critical section protected with
   * the PRIMASK bit (__disable_irq() / __enable_irq())
   */
  uint32_t primask_bit;

  primask_bit = __get_PRIMASK();  /**< backup PRIMASK bit */
  __disable_irq();

  TaskMask |= ( 1 << task_id );

  __set_PRIMASK(primask_bit);     /**< Restore PRIMASK bit*/

  return;
}

void SCH_SetEvt( uint32_t evt_id )
{
  /**
   * The backup/restore PRIMASK mechanism allow the application to call this API from a critical section protected with
   * the PRIMASK bit (__disable_irq() / __enable_irq())
   */
  uint32_t primask_bit;

  primask_bit = __get_PRIMASK();  /**< backup PRIMASK bit */
  __disable_irq();

  EvtSet |= ( 1 << evt_id);

  __set_PRIMASK(primask_bit);     /**< Restore PRIMASK bit*/

  return;
}

void SCH_WaitEvt( uint32_t evt_id )
{
  uint32_t event_waited_id_backup;

  event_waited_id_backup = EvtWaited;
  EvtWaited = ( 1 << evt_id );
  while( (EvtSet & EvtWaited) == 0)
  {
    SCH_EvtIdle();
  }
  EvtSet &= (~EvtWaited);
  EvtWaited = event_waited_id_backup;

  return;
}


#if( CORE_CM0PLUS == 1)
static uint32_t CountLeadZero(uint32_t value)
{
  uint32_t n;

  if ((value & 0xFFFF0000) == 0)
  {
    n  = 16;
    value <<= 16;
  }
  else
  {
    n = 0;
  }

  if ((value & 0xFF000000) == 0)
  {
    n +=  8;
    value <<=  8;
  }

  if ((value & 0xF0000000) == 0)
  {
    n +=  4;
    value <<=  4;
  }

  n += (uint32_t)clz_table_4bit[value >> (32-4)];

  return n;
}
#endif


/* __WEAK functions: The application can overload this function  if needed ----*/
/* ----------------------------------------------------------------------------*/

__weak void SCH_EvtIdle( void )
{
  /**
   * execute scheduler if not implemented by the application
   */
  SCH_Run();

  return;
}

__weak void SCH_Idle( void )
{
  /**
   * Stay in run mode if not implemented by the application
   */
  return;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
