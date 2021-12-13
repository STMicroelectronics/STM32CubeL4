
/**
 ******************************************************************************
 * @file    hr.h
 * @author  MCD Application Team
 * @brief   Header for hr.c module
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
#ifndef __HR_H
#define __HR_H

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ------------------------------------------------------------------*/
  /* Exported types ------------------------------------------------------------*/
  typedef enum
  {
    HR_Limited,
    HR_Full,
  } HR_InitMode_t;

  /* Exported constants --------------------------------------------------------*/
  /* External variables --------------------------------------------------------*/
  /* Exported macros -----------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
  void HR_Init( HR_InitMode_t init_mode );
  void HR_AdvUpdate( void );

#ifdef __cplusplus
}
#endif

#endif /*__HR_H */

