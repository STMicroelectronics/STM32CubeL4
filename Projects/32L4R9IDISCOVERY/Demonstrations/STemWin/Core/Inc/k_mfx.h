/**
  ******************************************************************************
  * @file    k_mfx.h
  * @author  MCD Application Team
  * @brief   Header for k_mfx.c file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __K_MFX_H
#define __K_MFX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"   

/* Exported constants --------------------------------------------------------*/
#define MFX_IRQ_PENDING_GPIO    0x01                                                              
#define MFX_IRQ_PENDING_IDD     0x02                                                              
#define MFX_IRQ_PENDING_ERROR   0x04
#define MFX_IRQ_PENDING_TS_DET  0x08
#define MFX_IRQ_PENDING_TS_NE   0x10
#define MFX_IRQ_PENDING_TS_TH   0x20
#define MFX_IRQ_PENDING_TS_FULL 0x40
#define MFX_IRQ_PENDING_TS_OVF  0x80   
   
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  MFX_INTERRUPT_EVENT = 1,  
  TS_INTERRUPT_EVENT = 2,
}   
MFX_EventTypeDef;


/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void     k_MfxInit(void);
void     k_MfxWakeUp(void);

extern osMessageQId MfxEvent;
extern osSemaphoreId MfxIddSemaphore;


#ifdef __cplusplus
}
#endif

#endif /*__K_MFX_H */

