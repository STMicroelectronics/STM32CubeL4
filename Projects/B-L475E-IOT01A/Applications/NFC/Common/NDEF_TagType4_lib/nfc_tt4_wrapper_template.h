/**
  ******************************************************************************
  * @file    nfc_tt4_wrapper_template.h
  * @author  MCD Application Team
  * @brief   This file makes the bridge between middleware generic NCF TagType4 
  *          function calls and the component.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NFC_TT4_WRAPPER_H
#define __NFC_TT4_WRAPPER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "component.h"

/** @addtogroup NFC_TT4_WRAPPER
  * @{
  */

/** @defgroup NFC_TT4_WRAPPER_Exported_Constants  NFC_TT4_WRAPPER Exported Constants
  * @{
  */

/** @defgroup NFC_TT4_NDEF_EEPROM_Size_Version
  * @{
  */
/* ---------------------- M24SR properties -----------------------------------*/

/* The maximum size of a NDEF  */
#define NFC_TT4_NDEF_MAX_SIZE          0x2000

/**
  * @}
  */
  
/** @defgroup NFC_TT4_WRAPPER_Status_Code
  * @{
  */
/* ---------------------- status code ----------------------------------------*/
#define NFC_TT4_ACTION_COMPLETED        0x9000
/**
  * @}
  */
  
/** @defgroup NFC_TT4__Flag_to_select_open_session_command
  * @{
  */
#define NFC_TT4_ASK_FOR_SESSION                     0x0000
#define NFC_TT4_TAKE_SESSION                        0xFFFF

/**
  * @}
  */

/**
  * @}
  */


/** @defgroup NFC_TT4_WRAPPER_Exported_FunctionsPrototypes  NFC_TT4_WRAPPER Exported FunctionsPrototypes
  * @{
  */

uint16_t NFC_TT4_Initialization (uint8_t* pCCBuffer, uint8_t size );
uint16_t NFC_TT4_OpenNDEFSession ( uint16_t NDEF_fileID, uint16_t Priority );
uint16_t NFC_TT4_ReadData ( uint16_t Offset , uint16_t DataSize , uint8_t* pData);
uint16_t NFC_TT4_ForceReadData ( uint16_t Offset , uint16_t DataSize , uint8_t* pData);
uint16_t NFC_TT4_WriteData ( uint16_t Offset , uint16_t DataSize , uint8_t* pData);
uint16_t NFC_TT4_CloseNDEFSession ( uint16_t NDEF_fileID );

/**
  * @}
  */
  
/**
  * @}
  */  
#ifdef __cplusplus
}
#endif


#endif /* __NFC_TT4_WRAPPER_H */
 
