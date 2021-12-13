/**
  ******************************************************************************
  * @file    lib_NDEF_AAR.h
  * @author  MMY Application Team
  * @brief   This file help to manage AAR.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014 STMicroelectronics.
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
   
/** @addtogroup NFC_libraries
  * @{
  */


/** @addtogroup TAGTYPE4_NDEF_AAR
  * @{
  */

  /**
 * @defgroup TAGTYPE4_NDEF_AAR_Exported_Types             TAGTYPE4_NDEF_AAR Exported Types
 * @{
 */

/**
  * @brief  AAR structure, mainly to store package name
  */   
typedef struct 
{
  char PakageName[80];
}sAARInfo;

/**
  * @}
  */
    
/**
 * @defgroup TAGTYPE4_NDEF_AAR_Exported_FunctionPrototypes             TAGTYPE4_NDEF_AAR Exported FunctionPrototypes
 * @{
 */
    
uint16_t NDEF_AddAAR ( sAARInfo *pAARStruct );

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif
   
#endif /* __LIB_NDEF_AAR_H */

