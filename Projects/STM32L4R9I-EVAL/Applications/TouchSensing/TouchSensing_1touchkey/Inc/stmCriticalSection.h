/**
  ******************************************************************************
  * @file    TouchSensing\TouchSensing_1touchkey\Inc\stmCriticalSection.h
  * @author  MCD Application Team
  * @brief   stmCriticalSection.c header file
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

#ifndef STM_CRITICAL_SECTION_H
#define STM_CRITICAL_SECTION_H

/* To call before modifying any critical data. In case the host is inside or
asked for enterring the critical section, this routine will wait for the host to
leave the critical section. */
void enterLock(void);

/* Leave the critical section. If the host is waiting, access will be granted
to him. Otherwise the first next one asking will own the turn. */
void exitLock(void);

#endif /* STM_CRITICAL_SECTION_H */

