/**
  ******************************************************************************
  * @file    bsp.h
  * @author  MCD Application Team
  * @brief   Header for bsp.c file
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
#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
uint8_t BSP_Config(void);
void BSP_TouchUpdate(void);
void BSP_JumpToSubDemo(uint32_t SubDemoAddress);
int  BSP_ResourcesCopy(WM_HWIN hItem, FIL * pResFile, uint32_t Address);
int  BSP_FlashProgram(WM_HWIN hItem, FIL * pResFile, uint32_t Address);
int BSP_FlashUpdate(uint8_t *pData, uint32_t Address);

#ifdef __cplusplus
}
#endif

#endif /*__BSP_H */

