/**
 ******************************************************************************
 * @file    memory_manager.c
 * @author  MCD Application Team
 * @brief   Memory Manager
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
#include "common.h"
#include "memory_manager.h"
#include "list.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t QueueSize;
static tListNode BufferPool;
static MM_pCb_t BufferFreeCb;

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/**
 * @brief  Initialize the Pools
 * @param  p_pool: The pool of memory to manage
 * @param  pool_size: The size of the pool
 * @param  elt_size: The size of one element in the pool
 * @retval None
 */
void MM_Init(uint8_t *p_pool, uint32_t pool_size,  uint32_t elt_size)
{
  uint32_t elt_size_corrected;

  QueueSize = 0;
  elt_size_corrected = 4*DIVC( elt_size, 4 );

  /**
   *  Initialize list
   */
  LST_init_head (&BufferPool);

  /**
   *  Initialize the queue
   */
  while(pool_size >= elt_size_corrected)
  {
    LST_insert_tail(&BufferPool, (tListNode *)p_pool);
    p_pool += elt_size_corrected;
    QueueSize++;
    pool_size -= elt_size_corrected;
  }

  return;
}

/**
 * @brief  Provide a buffer
 * @note   The buffer allocated to the user shall be at least sizeof(tListNode) bytes
 *         to store the memory manager chaining information
 *
 * @param  size: The size of the buffer requested
 * @param  cb: The callback to be called when a buffer is made available later on
 *                   if there is no buffer currently available when this API is called
 * @retval The buffer address when available or NULL when there is no buffer
 */
MM_pBufAdd_t MM_GetBuffer( uint32_t size, MM_pCb_t cb )
{
  MM_pBufAdd_t buffer_address;
  uint32_t primask_bit;

  primask_bit = __get_PRIMASK();  /**< backup PRIMASK bit */
  __disable_irq();                  /**< Disable all interrupts by setting PRIMASK bit on Cortex*/
  if ( QueueSize )
  {
    QueueSize--;
    BufferFreeCb = 0;
    LST_remove_head( &BufferPool, ( tListNode ** )&buffer_address );
  }
  else
  {
    BufferFreeCb = cb;
    buffer_address = 0;
  }
  __set_PRIMASK( primask_bit );     /**< Restore PRIMASK bit*/

  return buffer_address;
}

/**
 * @brief  Release a buffer
 * @param  p_buffer: The data buffer address
 * @retval None
 */
void MM_ReleaseBuffer( MM_pBufAdd_t p_buffer )
{
  uint32_t primask_bit;

  primask_bit = __get_PRIMASK();  /**< backup PRIMASK bit */
  __disable_irq();                  /**< Disable all interrupts by setting PRIMASK bit on Cortex*/
  LST_insert_tail( &BufferPool, ( tListNode * )p_buffer );
  QueueSize++;
  __set_PRIMASK( primask_bit );     /**< Restore PRIMASK bit*/
  if( BufferFreeCb )
  {
    /**
     * The application is waiting for a free buffer
     */
    BufferFreeCb();
  }

  return;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
