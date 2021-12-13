/**
  ******************************************************************************
  * @file    k_mfx.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel MFX functions
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

/** @defgroup KERNEL_MFX
  * @brief Kernel menu routines
  * @{
  */

/* External variables --------------------------------------------------------*/
extern void BSP_SD_DetectCallback(void);

/* Private typedef -----------------------------------------------------------*/
typedef struct Mfx_Ctx_s
{
  uint8_t           InitDone;
  osSemaphoreId     IOLockID;
  osThreadId        ThreadID;
  osSemaphoreId     SemaphoreID;
} Mfx_Ctx_t;

/* Private defines -----------------------------------------------------------*/
#define MFX_IO_TIMEOUT_MS               10 /* osWaitForever */

#define MMI_INTERPRETER_QUEUE_POOL_SIZE 10
#define MMI_INTERPRETER_QUEUE_SIZE      30
#define MFX_TaskPRIORITY                osPriorityRealtime /* osPriorityLow osPriorityNormal osPriorityRealtime */
#define MFX_TaskSTACK_SIZE              (configMINIMAL_STACK_SIZE * 2)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static Mfx_Ctx_t Mfx_Ctx = { 0 };

/* Private function prototypes -----------------------------------------------*/
static void MfxThread(void const * argument);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Mfx thread initialization to manage interrupt
  * @param  None
  * @retval None
  */
void k_MfxInit(void)
{
  if(!Mfx_Ctx.InitDone)
  {
    /* Enable SD Interrupt mode */
    // BSP_SD_ITConfig();

    /* Create IO Semaphore lock */
    osSemaphoreDef(IO_Semaphore);

    /* Create the IO Semaphore used by the IO BSP */
    Mfx_Ctx.IOLockID = osSemaphoreCreate(osSemaphore(IO_Semaphore), 1);

    /* Create Semaphore lock */
    osSemaphoreDef(MFX_Semaphore);

    /* Create the Semaphore used by the two threads */
    Mfx_Ctx.SemaphoreID = osSemaphoreCreate(osSemaphore(MFX_Semaphore), 1);

    /* Get the semaphore */
    osSemaphoreWait(Mfx_Ctx.SemaphoreID, osWaitForever);

    /* Create mfx task */
    osThreadDef(MFX_Thread, MfxThread, MFX_TaskPRIORITY, 0, MFX_TaskSTACK_SIZE);
    Mfx_Ctx.ThreadID = osThreadCreate (osThread(MFX_Thread), NULL);

    Mfx_Ctx.InitDone = 1;
  }
}

/**
  * @brief  interrupt detection
  * @param  None
  * @retval None
  */
void k_MfxDetectInt(void)
{
  if(Mfx_Ctx.InitDone)
  {
    /* Release the semaphore */
    osSemaphoreRelease(Mfx_Ctx.SemaphoreID);
  }
}


/**
  * @brief  Mfx Thread
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */

__IO uint32_t errorSrc = 0;
__IO uint32_t errorMsg = 0;

static void MfxThread(void const * argument)
{
  for( ;; )
  {
    uint32_t irqPending = 0;

    osSemaphoreWait(Mfx_Ctx.SemaphoreID, osWaitForever);

    /* Disable MFX interrupt to manage current one */
    HAL_NVIC_DisableIRQ(MFX_INT_EXTI_IRQn);

    irqPending = MFX_IO_Read(IO_I2C_ADDRESS, MFXSTM32L152_REG_ADR_IRQ_PENDING);

    /* GPIO IT from MFX */
    if(irqPending & MFXSTM32L152_IRQ_GPIO)
    {
      uint32_t statusGpio = 0;

      if(BSP_IO_ITGetStatus(SD_DETECT_PIN))
      {
        BSP_SD_DetectCallback();
        statusGpio |= SD_DETECT_PIN;
      }

      if(statusGpio)
      {
        /* Clear IO Expander IT */
        BSP_IO_ITClear(statusGpio);
      }
    }
    else if(irqPending & MFXSTM32L152_IRQ_ERROR)
    {
      errorSrc = MFX_IO_Read(IO_I2C_ADDRESS, MFXSTM32L152_REG_ADR_ERROR_SRC);
      errorMsg = MFX_IO_Read(IO_I2C_ADDRESS, MFXSTM32L152_REG_ADR_ERROR_MSG);
    }

    /* Ack all IRQ pending except GPIO if any */
    irqPending &= ~MFXSTM32L152_IRQ_GPIO;
    if (irqPending)
    {
      MFX_IO_Write(IO_I2C_ADDRESS, MFXSTM32L152_REG_ADR_IRQ_ACK, irqPending);
    }
    
    /* Re-enable MFX interrupt */
    HAL_NVIC_EnableIRQ(MFX_INT_EXTI_IRQn);
  }
}

/**
  * @}
  */

/**
  * @}
  */

