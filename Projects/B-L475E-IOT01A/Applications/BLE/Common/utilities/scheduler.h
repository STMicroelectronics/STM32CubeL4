/**
 ******************************************************************************
 * @file    scheduler.h
 * @author  MCD Application Team
 * @brief   scheduler interface
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ------------------------------------------------------------------*/
  /* Exported types ------------------------------------------------------------*/
  /* Exported constants --------------------------------------------------------*/
  /* External variables --------------------------------------------------------*/
  /* Exported macros -----------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */

  /**
   * @brief This is called by the scheduler in critical section (PRIMASK bit) when
   *          - there is no more tasks to be executed
   *          - there is no pending event or the pending event is still not set
   *        The application should enter low power mode
   *
   * @param  None
   * @retval None
   */
  void SCH_Idle( void );

  /**
   * @brief This requests the scheduler to execute all pending tasks. When no task are pending, it calls SCH_Idle();
   *
   * @param  None
   * @retval None
   */
  void SCH_Run( void );

  /**
   * @brief This registers a task in the scheduler.
   *        A task can be either Set, Pause or Resume.
   *
   * @param task_id: id of the task ( It shall be different for all tasks)
   * @param task: Callback to the task to be executed
   *
   * @retval None
   */
  void SCH_RegTask( uint32_t task_id, void (*task)(void) );

  /**
   * @brief  Request a task to be executed
   *
   * @param  task_id: The Id of the task
   * @retval None
   */
  void SCH_SetTask( uint32_t task_id );

  /**
   * @brief Prevents a task to be called by the scheduler even when set with SCH_SetTask()
   *        By default, all tasks are executed by the scheduler when set with SCH_SetTask()
   *
   * @param task_id
   * @retval None
   */
  void SCH_PauseTask( uint32_t task_id );

  /**
   * @brief Allows a task to be called by the scheduler if set with SCH_SetTask()
   *        By default, all tasks are executed by the scheduler when set with SCH_SetTask()
   *
   * @param task_id
   * @retval None
   */
  void SCH_ResumeTask( uint32_t task_id );

  /**
   * @brief It waits for a specific event to be set. The scheduler loops SCH_EvtIdle() until the event is set
   *        When called recursively, it acts as a First in / Last out mechanism. The scheduler waits for the
   *        last event requested to be set even though one of the already requested event has been set.
   *
   * @param evt_id
   * @retval None
   */
  void SCH_WaitEvt( uint32_t evt_id );

  /**
   * @brief It sets an event that is waited with SCH_WaitEvt()
   *
   * @param evt_id
   * @retval None
   */
  void SCH_SetEvt( uint32_t evt_id );


  /**
   * @brief The scheduler loops in that function until the waited event is set
   *        The application may either enter low power mode or call SCH_Run()
   *        When not implemented by the application, it calls call SCH_Run()
   *
   * @param  None
   * @retval None
   */
  void SCH_EvtIdle( void );

#ifdef __cplusplus
}
#endif

#endif /*__SCHEDULER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
