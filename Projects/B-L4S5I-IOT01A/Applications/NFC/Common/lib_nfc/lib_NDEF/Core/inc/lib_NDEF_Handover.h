/**
  ******************************************************************************
  * @file    lib_NDEF_Handover.h
  * @author  MMY Application Team
  * @version $Revision: 2475 $
  * @date    $Date: 2016-06-24 12:11:59 +0200 (Fri, 24 Jun 2016) $
  * @brief   This file helps to manage a NDEF Handover message.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
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
#ifndef __LIB_NDEF_HANDOVER_H
#define __LIB_NDEF_HANDOVER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF.h"

   
/** @addtogroup lib_NDEF_Handover NDEF Handover library
  * @{
  */
   
/** @brief SIze of the buffer used to store an Alternative Carrier (allocated on the stack).
  * @details 128 bytes should be enough, the specification recommends to use short URIs as AC identifiers.
  */
#define NDEF_AC_BUFFER_SIZE                 (128)

/** @brief NFC Forum Well-Known Type name for a Handover Select record */
#define NDEF_HANDOVER_SELECT_TYPE_STR       "Hs"
/** @brief NFC Forum Well-Known Type name for a Handover Request record */
#define NDEF_HANDOVER_REQUEST_TYPE_STR      "Hr"
/** @brief NFC Forum Well-Known Type name for a Handover Select Select (ascii value) */
#define NDEF_HANDOVER_SELECT_TYPE           0x4872
/** @brief NFC Forum Well-Known Type name for a Handover Request record (ascii value) */
#define NDEF_HANDOVER_REQUEST_TYPE          0x4873
/** @brief NFC Forum Handover record implemented version */
#define NDEF_HANDOVER_VERSION_1_2           0x12

/** @brief NFC Forum Well-Known Type name for a Colision Resolution record */
#define NDEF_HANDOVER_COLLISION_RESOLUTION_TYPE_STR     "cr"
/** @brief NFC Forum Well-Known Type name for an Alternative Carrier record */
#define NDEF_HANDOVER_ALTERNATIVE_CARRIER_TYPE_STR      "ac"

/** @brief NFC Forum Power States values mask */
#define NDEF_HANDOVER_AC_CPS_MASK (0x3)
/** @brief Enumerates the NFC Forum Power States possible values */
typedef enum {
  NDEF_HANDOVER_AC_CPS_INACTIVE   = 0x0, /**< The HW for this Alternative Carrier has to be switch on before proceeding. */
  NDEF_HANDOVER_AC_CPS_ACTIVE     = 0x1, /**< The HW for this Alternative Carrier is currently running. */
  NDEF_HANDOVER_AC_CPS_ACTIVATING = 0x2, /**< The HW for this Alternative Carrier is being activated. */
  NDEF_HANDOVER_AC_CPS_UNKNOWN    = 0x3  /**< The HW for this Alternative Carrier is unknown. */
} Ndef_Handover_ac_cps_t;



typedef struct {
  uint8_t                       cps;                /**< Current Power State for this Alternative Carrier Hardware. */
  sRecordInfo_t                 ac_record;          /**< The record structure used to store/build the Alternative Carrier record. */
  uint8_t                       aux_data_ref_count; /**< The number of Auxiliary Data Reference for this Alternative Carrier. */
  /* following fields are only used when reading NDEF */
  uint8_t*                      aux_data_ref_start; /**< Internal field, managed by the library, to keep track of the Auxiliary Data Reference start address in the AC record. */
  uint8_t*                      aux_data_ref_end;   /**< Internal field, managed by the library, to keep track of the Auxiliary Data Reference end address in the AC record. */
} Ndef_Handover_alternative_carrier_t;


typedef struct {
  uint16_t  type;                                   /**< Handover type: could be Select or Request. */
  uint8_t   version;                                /**< Version of the Handover record. */
  /* following fields are only used when reading NDEF */
  uint8_t   has_cr;                                 /**< Boolean, if true this Handover has a Collision Resolution nested record. */
  uint16_t  cr_random_number;                       /**< Random number for the Collision Resolution. */
  uint8_t   nb_alternative_carrier;                 /**< Number of Alternative Carrier nested records un this Handover. */
  uint8_t*  ac_start;                               /**< Internal field, managed by the library, to keep track of the Alternative Carrier Reference Data start address in the Handover record. */
  uint8_t*  ac_end;                                 /**< Internal field, managed by the library, to keep track of the Alternative Carrier Reference Data end address in the Handover record. */
} Ndef_Handover_t;



uint16_t NDEF_ReadHandover(sRecordInfo_t *pRecord ,  Ndef_Handover_t *pHandover );
uint16_t NDEF_ReadAC( uint8_t ac_nb, Ndef_Handover_t *pHandover  , Ndef_Handover_alternative_carrier_t *pAC );
uint16_t NDEF_ReadAuxData( uint8_t aux_data_nb, Ndef_Handover_alternative_carrier_t *pAC, sRecordInfo_t *pRecord );

uint16_t NDEF_CreateHandover(Ndef_Handover_t  *pHandover, sRecordInfo_t* pRecord );
uint16_t NDEF_AddAlternativeCarrier(Ndef_Handover_alternative_carrier_t *pAC, char* CarrierDataRef, char **AuxDataRefID, sRecordInfo_t* pRecord );
uint16_t NDEF_WriteHandover( sRecordInfo_t* pRecord , uint8_t* pNdef);

uint32_t NDEF_GetACDataLength(Ndef_Handover_alternative_carrier_t *pAC,char *CarrierDataRef, char **AuxDataRefID);


#ifdef __cplusplus
}
#endif

/** @}
  */

#endif /* __LIB_NDEF_HANDOVER_H */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
