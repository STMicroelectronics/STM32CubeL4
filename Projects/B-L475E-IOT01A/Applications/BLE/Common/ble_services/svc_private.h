
/**
 ******************************************************************************
 * @file    svc_private.h
 * @author  MCD Application Team
 * @brief   Header for stm32xx_service.c module
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
