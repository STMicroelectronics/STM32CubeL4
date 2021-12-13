/**
  ******************************************************************************
  * @file    ts.c
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
#include "stm32l4r9i_discovery.h"
#include "stm32l4r9i_discovery_io.h"
#include "stm32l4r9i_discovery_ts.h"

#include "cmsis_os.h"
#include "bsp.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MFX
  * @brief Kernel menu routines
  * @{
  */

/* External variables --------------------------------------------------------*/
extern uint8_t GUI_Initialized;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static osSemaphoreId TouchIntSemaphore;
static osThreadId    TouchIntThread;
static uint8_t       TS_Initialized = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void TouchThread(void const * argument);

/**
  * @brief  Touch hardware abd thread initialization to manage interrupt
  * @param  None 
  * @retval None
  */
void TouchInit(void)
{
  /* Create Touch Screen task */
  osThreadDef(TS_Thread, TouchThread, osPriorityAboveNormal, 0, 128);
  TouchIntThread = osThreadCreate (osThread(TS_Thread), NULL);
}

/**
  * @brief  Touch hardware and thread de-initialization
  * @param  None 
  * @retval None
  */
void TouchDeInit(void)
{
#ifdef TS_INTERRUPT_MODE_ENABLED
  /* DeInitialize the Touch Screen Controller */
  BSP_TS_ITClear();
  BSP_TS_DeInit();
#endif
  /* Delete the Touch Screen task */
  if(TouchIntThread)
  {
    osThreadTerminate(TouchIntThread);
  }

  /* Delete the Semaphore lock */
  if(TouchIntSemaphore)
  {
    osSemaphoreDelete(TouchIntSemaphore);
  }
}

/**
  * @brief  interrupt detection
  * @param  None 
  * @retval None
  */
void TouchDetectInt(void)
{  
  /* Release the semaphore if the thread was created prior to this call */
  if(TouchIntSemaphore)
  {
    osSemaphoreRelease(TouchIntSemaphore);
  }
}


/**
  * @brief  Touch Screen Thread
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void TouchThread(void const * argument)
{
  /* Create Semaphore lock */
  osSemaphoreDef(TOUCH_SCREEN_SEM);
  
  /* Create the Semaphore used by the two threads */
  TouchIntSemaphore = osSemaphoreCreate(osSemaphore(TOUCH_SCREEN_SEM), 1); 

  /* Get the semaphore */
  osSemaphoreWait(TouchIntSemaphore, osWaitForever);

  for( ;; )
  {
    /* WA to force TS init after the LCD init */
    if(GUI_Initialized && !TS_Initialized)
    {
      BSP_IO_Init();

      /* Initialize the Touch Screen Controller */
      if (BSP_TS_Init(390, 390) == TS_OK)
      {
#ifdef TS_INTERRUPT_MODE_ENABLED
        /* Enable TS interrupt */
        if(BSP_TS_ITConfig() == TS_OK)
        {
          TS_Initialized = 1;
        }
#else
        TS_Initialized = 1;
#endif
      }
      else
      {
        TS_Initialized = 0;
        osDelay(20);
      }
    }
    else if(!GUI_Initialized)
    {
      osDelay(100);
    }
    else
    {
      osSemaphoreWait(TouchIntSemaphore, osWaitForever);
#ifdef TS_INTERRUPT_MODE_ENABLED
      /* Disable TS interrupt to manage current one */
      HAL_NVIC_DisableIRQ(TS_DSI_INT_EXTI_IRQn);
#endif
      /* Update TS data */
      BSP_TouchUpdate();
#ifdef TS_INTERRUPT_MODE_ENABLED
      /* Re-enable TS interrupt */
      HAL_NVIC_EnableIRQ(TS_DSI_INT_EXTI_IRQn);
#endif
    }
  }
}



/**
  * @}
  */

/**
  * @}
  */

