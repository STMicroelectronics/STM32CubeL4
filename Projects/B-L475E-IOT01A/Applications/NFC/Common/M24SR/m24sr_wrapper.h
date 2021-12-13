/**
  ******************************************************************************
  * @file    m24sr_wrapper.h
  * @author  MCD Application Team
  * @brief   This file makes the bridge between middleware generic NCF TagType4 
  *          function calls and the M24SR component. Furthermore it simplifies 
  *          the use of the M24SR driver by sequencing some commands.
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
#ifndef __M24SR_WRAPPER_H
#define __M24SR_WRAPPER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "nfc_config.h"
#include "m24sr.h"

/** @addtogroup M24SR_WRAPPER
  * @{
  */

/** @defgroup M24SR_WRAPPER_Exported_Constants  M24SR_WRAPPER Exported Constants
  * @{
  */

/**
  * @brief  GPO mode structure 
  */
typedef enum{
  M24SR_RF_GPO= 0,
  M24SR_I2C_GPO
}M24SR_GPO_MODE; 

/**
  * @brief  M24SR_WRAPPER_Status_Code
  */
/* ---------------------- status code ----------------------------------------*/
#define NFC_TT4_ACTION_COMPLETED    M24SR_ACTION_COMPLETED

/**
  * @brief  lib_M24SR_Flag_to_select_open_session_command
  */  
#define NFC_TT4_ASK_FOR_SESSION                     0x0000
#define NFC_TT4_TAKE_SESSION                        0xFFFF

/**
  * @}
  */


/** @defgroup M24SR_WRAPPER_Exported_FunctionsPrototypes  M24SR_WRAPPER Exported FunctionsPrototypes
  * @{
  */

uint16_t NFC_TT4_Initialization (uint8_t* pCCBuffer, uint8_t size );

uint16_t NFC_TT4_GetNDEFFileId ( uint16_t *NDEF_fileID );
uint16_t NFC_TT4_OpenNDEFSession ( uint16_t NDEF_fileID, uint16_t Priority );
uint16_t NFC_TT4_ReadData ( uint16_t Offset , uint16_t DataSize , uint8_t* pData);
uint16_t NFC_TT4_ForceReadData ( uint16_t Offset , uint16_t DataSize , uint8_t* pData);
uint16_t NFC_TT4_WriteData ( uint16_t Offset , uint16_t DataSize , uint8_t* pData);
uint16_t NFC_TT4_CloseNDEFSession ( uint16_t NDEF_fileID );

uint16_t NFC_TT4_EnableReadPassword( uint8_t* pCurrentWritePassword, uint8_t* pNewPassword);
uint16_t NFC_TT4_DisableReadPassword( uint8_t* pCurrentWritePassword );
uint16_t NFC_TT4_EnableWritePassword( uint8_t* pCurrentWritePassword, uint8_t* pNewPassword);
uint16_t NFC_TT4_DisableWritePassword( uint8_t* pCurrentWritePassword );
uint16_t NFC_TT4_DisableAllPassword( uint8_t* pSuperUserPassword);

uint16_t NFC_TT4_EnableReadOnly( uint8_t* pCurrentWritePassword);
uint16_t NFC_TT4_DisableReadOnly( uint8_t* pCurrentWritePassword);
uint16_t NFC_TT4_EnableWriteOnly( uint8_t* pCurrentWritePassword);
uint16_t NFC_TT4_DisableWriteOnly( uint8_t* pCurrentWritePassword);

uint16_t NFC_TT4_ManageGPO( uint8_t gpo_config, uint8_t mode);

/**
  * @}
  */
  
/**
  * @}
  */  
#ifdef __cplusplus
}
#endif


#endif /* __M24SR_WRAPPER_H */
 
