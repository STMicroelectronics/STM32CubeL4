
/**
 ******************************************************************************
 * @file    hr.h
 * @author  MCD Application Team
 * @brief   Header for hr.c module
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
