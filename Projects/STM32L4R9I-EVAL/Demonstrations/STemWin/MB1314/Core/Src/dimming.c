/**
  ******************************************************************************
  * @file    dimming.c
  * @author  MCD Application Team 
  * @brief   This file provides the kernel bsp functions
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

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_BSP
  * @brief Kernel BSP routines
  * @{
  */

/* External variables --------------------------------------------------------*/
#define EXTERN_DSI_HANDLE hdsi
extern DSI_HandleTypeDef EXTERN_DSI_HANDLE;

/* Private typedef -----------------------------------------------------------*/
typedef enum dimmint_state_e {
  DIMMING_STATE_STARTED
, DIMMING_STATE_STEP_1
, DIMMING_STATE_STEP_2
, DIMMING_STATE_STOPPED
} dimmint_state_t;

typedef struct dimming_data_s {
  osThreadId      ThreadId;
  dimmint_state_t state;
  uint32_t        value;
  uint32_t        timeout_step1;  /* LCD remain On during this period */
  uint32_t        timeout_step2;  /* Dimming until Min will be done during after this period */
  uint32_t        timeout_stop;   /* LCD will be switched off after this period */
} dimming_data_t;

/* Private defines -----------------------------------------------------------*/
#define DIMMING_TaskPRIORITY        osPriorityLow /* osPriorityIdle osPriorityBelowNormal osPriorityNormal */
#define DIMMING_TaskSTACK_SIZE      (configMINIMAL_STACK_SIZE * 1)

#define BRIGHTNESS_TIMEOUT_MS       20
#define BRIGHTNESS_MIN              50
#define BRIGHTNESS_NORMAL           200

enum
{
  DIMMING_SIGNAL_STOP     = (1L << 0),
  DIMMING_SIGNAL_START    = (1L << 1),
  DIMMING_SIGNAL_RESET    = (1L << 2)
};

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static dimming_data_t dimming_data = { NULL, DIMMING_STATE_STOPPED, BRIGHTNESS_NORMAL, 60000, ((BRIGHTNESS_NORMAL - BRIGHTNESS_MIN) * BRIGHTNESS_TIMEOUT_MS), 600000 };

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Create the Dimming thread and start it
  * @param  None
  * @retval None
  */

static void DIMMING_Task( const void* argument )
{
  (void) argument;
  osEvent       event;
  uint32_t      timer_value     = 0;
  uint32_t      timeout_value   = osWaitForever;
  uint32_t      valid_events    = (DIMMING_SIGNAL_START | DIMMING_SIGNAL_STOP | DIMMING_SIGNAL_RESET);

  for(;;)
  {
    event = osSignalWait( valid_events, timeout_value );

    if (event.status == osEventTimeout)
    {
      switch(dimming_data.state)
      {
        case DIMMING_STATE_STARTED:
          if (timer_value == 0)
          {
            timeout_value = 1000;
            timer_value = (dimming_data.timeout_step1 / timeout_value);
            /* Send Brightness DCS Command to display */ 
            HAL_DSI_ShortWrite( &EXTERN_DSI_HANDLE,
                                0,
                                DSI_DCS_SHORT_PKT_WRITE_P1,
                                0x51,
                                BRIGHTNESS_NORMAL);
          }
          else if (--timer_value == 0)
          {
            dimming_data.state = DIMMING_STATE_STEP_1;
          }
        break;
        
        case DIMMING_STATE_STEP_1:
          if (timer_value == 0)
          {
            timeout_value = BRIGHTNESS_TIMEOUT_MS;
            timer_value   = (dimming_data.timeout_step2 / timeout_value);
            dimming_data.value = BRIGHTNESS_NORMAL;
          }
          else if (--timer_value > 0)
          {
            dimming_data.value -= ((BRIGHTNESS_NORMAL - BRIGHTNESS_MIN) / BRIGHTNESS_TIMEOUT_MS);
            if(dimming_data.value <= BRIGHTNESS_MIN)
            {
              dimming_data.value = BRIGHTNESS_MIN;
              dimming_data.state = DIMMING_STATE_STEP_2;
              timer_value        = 0;
            }
            /* Send Brightness DCS Command to display */ 
            HAL_DSI_ShortWrite( &EXTERN_DSI_HANDLE,
                                0,
                                DSI_DCS_SHORT_PKT_WRITE_P1,
                                0x51,
                                dimming_data.value);
          }
        break;
        
        case DIMMING_STATE_STEP_2:
          if (timer_value == 0)
          {
            timeout_value = 1000;
            timer_value = (dimming_data.timeout_stop / timeout_value);
          }
          else if (--timer_value == 0)
          {
            dimming_data.state = DIMMING_STATE_STOPPED;
          }
        break;

        case DIMMING_STATE_STOPPED:
          timeout_value = osWaitForever;
          timer_value   = 0;
          /* Send Brightness DCS Command to display */ 
          HAL_DSI_ShortWrite( &EXTERN_DSI_HANDLE,
                              0,
                              DSI_DCS_SHORT_PKT_WRITE_P1,
                              0x51,
                              0);
        break;
      }
    }
    else if( event.value.signals & valid_events )
    {
      switch(event.value.signals)
      {
        case DIMMING_SIGNAL_START:
        case DIMMING_SIGNAL_RESET:
          timer_value   = 0;
          timeout_value = 10;
          valid_events  = (DIMMING_SIGNAL_STOP | DIMMING_SIGNAL_RESET);
          dimming_data.state = DIMMING_STATE_STARTED;
        break;
        
        case DIMMING_SIGNAL_STOP:
          valid_events = DIMMING_SIGNAL_START;
          dimming_data.state = DIMMING_STATE_STOPPED;
        break;
      }
    }
  }
}

/**
  * @brief  Create the Dimming thread and start it
  * @param  None
  * @retval TRUE if success, 0 otherwise
  */
uint8_t DIMMING_Init(void)
{
  if (dimming_data.ThreadId != NULL)
    return 0;

  osThreadDef(DIMMING_Task, DIMMING_Task, DIMMING_TaskPRIORITY, 0, DIMMING_TaskSTACK_SIZE);

  dimming_data.ThreadId = osThreadCreate(osThread(DIMMING_Task), NULL);
  if(dimming_data.ThreadId == NULL)
    return 0;
  
  /* Send START Signal */
  if(osSignalSet( dimming_data.ThreadId, DIMMING_SIGNAL_START ) != osOK)
    return 0;
  
  return 1;
}

/**
  * @brief  Create the Dimming thread and start it
  * @param  None
  * @retval -1 if it fail, 0 if success and dimming was NOT running, 1 if success and dimming was running otherwise
  */
int DIMMING_Reset(void)
{
  int ret = 0;
  
  if(dimming_data.ThreadId == NULL)
    return -1;

  if((dimming_data.state == DIMMING_STATE_STEP_1) || (dimming_data.state == DIMMING_STATE_STEP_2))
    ret = 1;
  else
    ret = 0;

  /* Send RESET Signal */
  if(osSignalSet( dimming_data.ThreadId, DIMMING_SIGNAL_RESET ) != osOK)
    ret = -1;

  return ret;
}

/**
  * @brief  Terminate the Dimming thread
  * @param  None
  * @retval None
  */
void DIMMING_Term(void)
{
  if(dimming_data.ThreadId != NULL)
  {
    osThreadTerminate(dimming_data.ThreadId);
    dimming_data.ThreadId = NULL;

    /* Send Brightness DCS Command to display */ 
    HAL_DSI_ShortWrite( &EXTERN_DSI_HANDLE,
                        0,
                        DSI_DCS_SHORT_PKT_WRITE_P1,
                        0x51,
                        BRIGHTNESS_NORMAL);
  }
}

/**
  * @}
  */

/**
  * @}
  */

