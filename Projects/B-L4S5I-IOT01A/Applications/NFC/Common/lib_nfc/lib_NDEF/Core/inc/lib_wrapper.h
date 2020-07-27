/**
  ******************************************************************************
  * @file    lib_wrapper.h
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file help to have upper layer independent from HW
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIB_WRAPPER_H
#define __LIB_WRAPPER_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"


typedef enum {
  NFCTAG_TYPE5 = 0,
  NFCTAG_TYPE4,
  NFCTAG_TYPE3,
  NFCTAG_TYPE2,
  NFCTAG_TYPE1,
  NFCTAG_NOTSET
} NFCTAG_Protocol_Id_t;


uint16_t NfcTag_SelectProtocol(NFCTAG_Protocol_Id_t protocol);
uint16_t NfcTag_ReadNDEF( uint8_t* pData );
uint16_t NfcTag_WriteNDEF(uint16_t Length, uint8_t* pData );
uint16_t NfcTag_WriteProprietary(uint16_t Length, uint8_t* pData );
uint16_t NfcTag_GetLength(uint16_t* Length);

#endif /* __LIB_WRAPPER_H */


/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
