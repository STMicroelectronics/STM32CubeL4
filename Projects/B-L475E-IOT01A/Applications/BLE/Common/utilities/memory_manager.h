/**
 ******************************************************************************
 * @file    memory_manager.h
 * @author  MCD Application Team
 * @brief   Header for memory_manager.c module
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
#ifndef __MEMORY_MANAGER_H
#define __MEMORY_MANAGER_H

/* Includes ------------------------------------------------------------------*/
/* Exported defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef void (*MM_pCb_t)( void );
typedef  uint8_t (*MM_pBufAdd_t);

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void MM_Init(uint8_t *p_pool, uint32_t pool_size,  uint32_t elt_size);
MM_pBufAdd_t MM_GetBuffer(uint32_t size, MM_pCb_t cb );
void MM_ReleaseBuffer( MM_pBufAdd_t p_buffer );

/* Exported functions to be implemented by the user if required ------------- */

#endif /*__MEMORY_MANAGER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
