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
typedef struct
{
  uint32_t progress_percent;
  uint32_t last_prog_duration;
  uint32_t remaining_time;
  void *   user_data;
} BSP_FlashProgramInfo_t;

typedef void BSP_Flash_CallBack( BSP_FlashProgramInfo_t * pInfo);

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void BSP_Config(void);
void BSP_TouchUpdate(void);
void BSP_JumpToSubDemo(uint32_t SubDemoAddress);
int  BSP_ResourcesCopy(WM_HWIN hItem, FIL * pResFile, uint32_t Address, BSP_Flash_CallBack *cb);
int  BSP_FlashProgram(WM_HWIN hItem, FIL * pResFile, uint32_t Address, BSP_Flash_CallBack *cb);

#ifdef __cplusplus
}
#endif

#endif /*__BSP_H */

