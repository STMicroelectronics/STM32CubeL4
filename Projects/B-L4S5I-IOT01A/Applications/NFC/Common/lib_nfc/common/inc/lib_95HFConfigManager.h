/**
  ******************************************************************************
  * @file    lib_95HFConfigManager.h 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file provides set of firmware functions to manage device modes. 
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under ST MYLIBERTY SOFTWARE LICENSE AGREEMENT (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/myliberty  
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion --------------------------------------------*/
#ifndef _LIB_95HFCONFIGMANAGER_H
#define _LIB_95HFCONFIGMANAGER_H

/* Includes -------------------------------------------------------------------------*/
#if defined(ST95HF) || defined(RX95HF)
	#include "lib_PICCemulator.h"
#endif

#if defined(ST95HF) || defined(CR95HF)
	#include "lib_iso15693pcd.h"
	#include "lib_iso14443Apcd.h"
	#include "lib_iso14443Bpcd.h"
	#include "lib_iso18092pcd.h"
#endif

#if defined(ST95HF)
	#include "lib_nfcdepinitiator.h"
	#include "lib_llcp.h"
	#include "lib_snep.h"
	
	#include "lib_proprietaryP2P.h"	
#endif

/* Manager status and erroc code ----------------------------------------------------*/
#define MANAGER_SUCCESSCODE															RESULTOK
#define MANAGER_ERRORCODE_DEFAULT												0xF1
#define MANAGER_ERRORCODE_PORERROR											0xF2
#define MANAGER_ERRORCODE_COMMUNICATION_LOST						0xF3

/* Flags for PICC/PCD tracking  ----------------------------------------------------------*/
#define	TRACK_NOTHING			0x00
#define	TRACK_NFCTYPE1 		0x01 /* 0000 0001 */
#define	TRACK_NFCTYPE2 		0x02 /* 0000 0010 */
#define	TRACK_NFCTYPE3 		0x04 /* 0000 0100 */
#define	TRACK_NFCTYPE4A 	0x08 /* 0000 1000 */
#define	TRACK_NFCTYPE4B 	0x10 /* 0001 0000 */
#define	TRACK_NFCTYPE5 		0x20 /* 0010 0000 */
#define TRACK_ALL 				0xFF /* 1111 1111 */

/* Flags for Initiator/Target tracking  ------------------------------------------------------*/
#define	P2P_NOTHING				0x00
#define	INITIATOR_NFCA 		0x01 /* 0000 0001 */
#define	INITIATOR_NFCF 		0x02 /* 0000 0010 */
#define	TARGET_NFCA 			0x04 /* 0000 0100 */
#define	TARGET_NFCF 			0x08 /* 0000 1000 */
#define	TARGET_LLCPA 			0x10 /* 0000 0100 */
#define	TARGET_LLCPF 			0x20 /* 0000 1000 */
#define	INITIATOR_LLCPA 	0x40 /* 0000 0100 */
#define	INITIATOR_LLCPF 	0x80 /* 0000 1000 */
#define P2P_ALL 					0xFF /* 1111 1111 */

/* Flags for Proprietary P2P tracking  ------------------------------------------------------*/
#define	PP2P_NOTHING					0x00
#define	PP2P_INITIATOR_NFCA 	0x01 /* 0000 0001 */
#define	PP2P_TARGET_NFCA 		0x10 /* 0000 0100 */
#define PP2P_ALL 							0xFF /* 1111 1111 */

/* Flags for SelectMode  ----------------------------------------------------------*/
#define	SELECT_NOTHING		0x00
#define	SELECT_PCD			 	0x01 /* 0000 0001 */
#define	SELECT_PICC			 	0x02 /* 0000 0010 */
#define	SELECT_P2P			 	0x04 /* 0000 0100 */
#define SELECT_ALL 				0xFF /* 1111 1111 */

/* structure of the manager state --------------------------------------------------*/
typedef struct {
	uint8_t SelectedMode;
	uint8_t PcdMode;
	uint8_t PiccMode;
	uint8_t P2pMode;
	uint8_t Result;
	uint16_t timeoutEmul;
	uint16_t timeoutReadEmul;
}MANAGER_CONFIG;

typedef enum PICCEMULATOR_SELECT_TAG_TYPE PICCEMULATOR_SELECT_TAG_TYPE;
	
/* public function	 ----------------------------------------------------------------*/

void ConfigManager_HWInit (void);

#if defined(ST95HF)
	void ConfigManager_AutoMode (MANAGER_CONFIG *pManagerConfig);
	uint8_t ConfigManager_P2P(uint8_t P2Pmode, uint16_t timeout);
#endif

#if defined(ST95HF) || defined(CR95HF)
	uint8_t ConfigManager_TagHunting ( uint8_t tagsToFind );
#endif

#if defined(ST95HF) || defined(RX95HF)
	uint8_t ConfigManager_TagEmulation (PICCEMULATOR_SELECT_TAG_TYPE TagEmulated, uint16_t timeoutEmul, uint16_t timeoutReadEmul);
	void ConfigManager_CardEmulation (PICCEMULATOR_SELECT_TAG_TYPE TagEmulated, void (*pCallBackFunction)(void));

	/* NFC Forum P2P */
	void ConfigManager_ActivateP2PStack( void );
	void ConfigManager_DesactivateP2PStack( void );
	uint8_t ConfigManager_P2PTarget (void(*pCallBackFunction)(void));
	uint8_t ConfigManager_P2PInitiator (void(*pCallBackFunction)(void));
	
	/* Proprietary P2P */
	uint8_t ConfigManager_P2PP(uint8_t P2Pmode, uint16_t timeout);
	uint8_t ConfigManager_P2PPInitiator (void(*pCallBackFunction)(void));
	uint8_t ConfigManager_PP2PTarget (void(*pCallBackFunction)(void));
#endif

void ConfigManager_Stop(void);

#endif

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
