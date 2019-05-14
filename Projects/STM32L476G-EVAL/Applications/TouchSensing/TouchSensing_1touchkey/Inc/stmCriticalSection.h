/**
  ******************************************************************************
  * @file    TouchSensing\TouchSensing_1touchkey\Inc\stmCriticalSection.h
  * @author  MCD Application Team
  * @brief   stmCriticalSection.c header file
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
