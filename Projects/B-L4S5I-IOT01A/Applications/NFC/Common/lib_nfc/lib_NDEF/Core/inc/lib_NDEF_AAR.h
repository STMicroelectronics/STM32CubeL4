/**
  ******************************************************************************
  * @file    lib_NDEF_AAR.h
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file help to manage AAR.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIB_NDEF_AAR_H
#define __LIB_NDEF_AAR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF.h"

typedef struct
{
  char PackageName[80];
}sAARInfo;
	 	 
uint16_t NDEF_AddAAR( const sAARInfo *pAARStruct );

#ifdef __cplusplus
}
#endif

#endif /* __LIB_NDEF_AAR_H */

