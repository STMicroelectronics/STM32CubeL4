
/**
 ******************************************************************************
 * @file    svc_private.h
 * @author  MCD Application Team
 * @brief   Header for stm32xx_service.c module
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
#ifndef __SVC_PRIVATE_H
#define __SVC_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ------------------------------------------------------------------*/
  /* Exported types ------------------------------------------------------------*/
  typedef enum
  {
    COMSVC_Notification = ( 1 << 0 ),
    COMSVC_Indication =   ( 1 << 1 ),
  } COMSVC_ClientCharConfMask_t;

  /* Exported constants --------------------------------------------------------*/
  /* External variables --------------------------------------------------------*/
  /* Exported macros -----------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /*__SVC_PRIVATE_H */

