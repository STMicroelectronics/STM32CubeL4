/**
 ******************************************************************************
 * @file    osal.c
 * @author  MCD Application Team
 * @brief   Implementation of OS abstraction layer functions used by the
 *          library.
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
  
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "osal.h"
 
 /**
 * @brief  Osal_MemCpy
 * @param  dest: Pointer to the destination buffer
 * @param  src : Pointer to the source buffer
 * @param  size: Number of bytes to copy from the source to the destination
 *               buffer
 * @retval Pointer to the destination buffer
 */
void* Osal_MemCpy(void *dest, const void *src, unsigned int size)
{
    return(memcpy(dest,src,size)); 
}

/**
 * @brief  Osal_MemSet
 * @param  ptr  : Pointer to block of memory to fill  
 * @param  value: Value to assign to each byte of the memory block
 * @param  size : Number of bytes to be set to "value"
 * @retval Pointer to the filled block of memory
 */
void* Osal_MemSet(void *ptr, int value, unsigned int size)
{
    return(memset(ptr,value,size));
}

/******************************************************************************
 * local Functions
 *****************************************************************************/ 
 
 /************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
