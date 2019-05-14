
/**
 ******************************************************************************
 * @file    blesvc.h
 * @author  MCD Application Team
 * @brief   BLE Service Interface
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
#ifndef __BLESVC_H
#define __BLESVC_H

#ifdef __cplusplus
extern "C" 
{
#endif


/* Includes ------------------------------------------------------------------*/
#include "svc_ctl.h"
#include "ble_status.h"
#include "dis.h"
#include "hrs.h"
#include "lbs_stm.h"
/* Other services not provided by this release */
/*
#include "ans.h"
#include "bas.h"
#include "hids.h"
#include "hps.h"
#include "ipss.h"
#include "lns.h"
#include "scps.h"
*/
  

/* Exported functions ------------------------------------------------------- */
void BLESVC_AddCustomChar( uint16_t svc_uuid, uint16_t svc_hdle );
void BLESVC_InitCustomSvc( void );


#ifdef __cplusplus
}
#endif

#endif /*__BLESVC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
