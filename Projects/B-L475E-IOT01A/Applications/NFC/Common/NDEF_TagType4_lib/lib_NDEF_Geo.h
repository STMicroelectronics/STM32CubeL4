/**
  ******************************************************************************
  * @file    lib_NDEF_Geo.h
  * @author  MMY Application Team
  * @brief   This file help to manage Geolocation NDEF file.
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
#ifndef __LIB_NDEF_GEO_H
#define __LIB_NDEF_GEO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF.h"
   
/** @addtogroup NFC_libraries
  * @{
  */

/** @addtogroup TAGTYPE4_NDEF_GEO
  * @{
  */

  /**
 * @defgroup TAGTYPE4_NDEF_GEO_Exported_Types             TAGTYPE4_NDEF_GEO Exported Types
 * @{
 */
/**
  * @brief  Geolocation structure, to store latitude, longitude and optional information
  */   
   
typedef struct 
{
  char Latitude[20];
  char Longitude[20];
  char Information[100];
}sGeoInfo;
/**
  * @}
  */
    
/**
 * @defgroup TAGTYPE4_NDEF_GEO_Exported_FunctionPrototypes             TAGTYPE4_NDEF_GEO Exported FunctionPrototypes
 * @{
 */
uint16_t NDEF_ReadGeo(sRecordInfo *pRecordStruct, sGeoInfo *pGeoStruct);
uint16_t NDEF_WriteGeo( sGeoInfo *pGeoStruct );
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
   
#endif /* __LIB_NDEF_GEO_H */

