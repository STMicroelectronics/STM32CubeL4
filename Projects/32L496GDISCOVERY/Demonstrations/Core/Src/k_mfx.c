/**
  ******************************************************************************
  * @file    k_mfx.c
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
#include "main.h"
#include "iddmeasure.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MFX
  * @brief Kernel menu routines
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define MFX_IRQ_PENDING_GPIO    0x01                                                              
#define MFX_IRQ_PENDING_IDD     0x02                                                              
#define MFX_IRQ_PENDING_ERROR   0x04
#define MFX_IRQ_PENDING_TS_DET  0x08
#define MFX_IRQ_PENDING_TS_NE   0x10
#define MFX_IRQ_PENDING_TS_TH   0x20
#define MFX_IRQ_PENDING_TS_FULL 0x40
#define MFX_IRQ_PENDING_TS_OVF  0x80

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
osMessageQId MfxEvent;
osSemaphoreId MfxIddSemaphore;

/* Flags used to recover Idd measurements results from Standby or Shutdown mode */
extern FlagStatus Fetch_IddResults;
extern FlagStatus Simulate_SecondPress;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void MfxThread(void const * argument);


/**
  * @brief  Mfx thread initialization
  * @param  None 
  * @retval None
  */
void k_MfxInit(void)
{  
  /* Create Mfx Message Queue */
  osMessageQDef(osqueue, 3, uint16_t);
  MfxEvent = osMessageCreate (osMessageQ(osqueue), NULL);

  /* Create Semaphore lock */
  osSemaphoreDef(MFX_IDD_SEM);
  
  /* Create the Semaphore used by the two threads */
  MfxIddSemaphore = osSemaphoreCreate(osSemaphore(MFX_IDD_SEM), 1); 
  /* Get the semaphore */
  osSemaphoreWait(MfxIddSemaphore, osWaitForever);
  
  /* Create  background task */
  osThreadDef(MFX_Thread, MfxThread, osPriorityRealtime, 0, 128);
  osThreadCreate (osThread(MFX_Thread), NULL);
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
  osEvent event;
  uint32_t irqPending;
  
  for( ;; ){
    event = osMessageGet( MfxEvent, osWaitForever );
    
    if( event.status == osEventMessage ){
      switch(event.value.v){        

        case TS_INTERRUPT_EVENT:

          if (IddMeasureState != IDD_STATE_MEASURE_ON_GOING){
            k_TouchUpdate();
            
             if (Simulate_SecondPress == SET)
             { 
              Fetch_IddResults = SET ;
              Simulate_SecondPress = RESET;                    
             }
          }
          break;

        case MFX_INTERRUPT_EVENT:
          if (IddMeasureState == IDD_STATE_MEASURE_ON_GOING) {
            /* if function pointer exists, restore context clocks and power context */
            if(IddTest[iddTestIndex].IddRestore != NULL){
              IddTest[iddTestIndex].IddRestore();
              /* Configure HW resources */
              SystemHardwareInit(HWINIT_IDD);      
            }
          }
          
          irqPending = MFX_IO_Read(IO1_I2C_ADDRESS, MFXSTM32L152_REG_ADR_IRQ_PENDING);
          
          /* GPIO IT from MFX */
          if(irqPending & MFX_IRQ_PENDING_GPIO){
            uint32_t statusGpio = BSP_IO_ITGetStatus(SD_DETECT_PIN);

            if(statusGpio & SD_DETECT_PIN){
              osMessagePut(StorageEvent, MSDDISK_CONNECTION_EVENT, 0);
            }
            else{
              Error_Handler();
            }
            
            BSP_IO_ITClear(statusGpio);
          }
          else if(irqPending & MFX_IRQ_PENDING_IDD){
            if (IddMeasureState == IDD_STATE_MEASURE_ON_GOING){
              IddMeasureState = IDD_STATE_MEASURE_OK;
              osMutexRelease(MfxIddSemaphore);
            }
          }
          else if(irqPending & MFX_IRQ_PENDING_ERROR){
            if (IddMeasureState == IDD_STATE_MEASURE_ON_GOING) {
              IddMeasureState = IDD_STATE_MEASURE_ERROR;
              /* Error register will be read and acknoledged by iddmeasure */
              osMutexRelease(MfxIddSemaphore);
            }
            else{
              errorSrc = MFX_IO_Read(IO1_I2C_ADDRESS, MFXSTM32L152_REG_ADR_ERROR_SRC);
              errorMsg = MFX_IO_Read(IO1_I2C_ADDRESS, MFXSTM32L152_REG_ADR_ERROR_MSG);
            }
          }
          
          /* Ack all IRQ pending except GPIO if any */
          irqPending &= ~MFX_IRQ_PENDING_GPIO;
          if (irqPending){
            MFX_IO_Write(IO1_I2C_ADDRESS, MFXSTM32L152_REG_ADR_IRQ_ACK, irqPending);
          }
          break;         
      }
    }
  }
}



/**
  * @}
  */

/**
  * @}
  */

