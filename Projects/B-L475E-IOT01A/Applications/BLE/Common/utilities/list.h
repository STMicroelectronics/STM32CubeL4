/**
 ******************************************************************************
 * @file    list.h
 * @author  MCD Application Team
 * @brief   Header file for linked list library.
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

#ifndef _LIST_H_
#define _LIST_H_

/* Includes ------------------------------------------------------------------*/
#ifdef __GNUC__
typedef struct __packed _tListNode {
#else
typedef __packed struct _tListNode {
#endif
  struct _tListNode * next;
  struct _tListNode * prev;
} tListNode;

void LST_init_head (tListNode * listHead);

uint8_t LST_is_empty (tListNode * listHead);

void LST_insert_head (tListNode * listHead, tListNode * node);

void LST_insert_tail (tListNode * listHead, tListNode * node);

void LST_remove_node (tListNode * node);

void LST_remove_head (tListNode * listHead, tListNode ** node );

void LST_remove_tail (tListNode * listHead, tListNode ** node );

void LST_insert_node_after (tListNode * node, tListNode * ref_node);

void LST_insert_node_before (tListNode * node, tListNode * ref_node);

int list_get_size (tListNode * listHead);

void list_get_next_node (tListNode * ref_node, tListNode ** node);

void list_get_prev_node (tListNode * ref_node, tListNode ** node);

#endif /* _LIST_H_ */
