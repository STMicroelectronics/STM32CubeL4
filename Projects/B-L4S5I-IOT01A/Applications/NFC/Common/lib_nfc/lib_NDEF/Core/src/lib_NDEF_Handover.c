/**
  ******************************************************************************
  * @file    lib_NDEF_Handover.c
  * @author  MMY Application Team
  * @version $Revision: 2702 $
  * @date    $Date: 2016-07-13 18:45:05 +0200 (Wed, 13 Jul 2016) $
  * @brief   This file helps to manage a NDEF Handover message.
  * @ingroup libNDEF_Handover 
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
/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF_Handover.h"

/** @addtogroup lib_NDEF_Handover NDEF Handover library
  * @ingroup libNDEF
  *	@brief  This module is used to manage the NDEF Handover messages, to negociate the switch to a non-nfc protocol of communication.
  * @details 
  *          The NDEF Handover process is described by the NFC Forum Connection Handover specification.
  *          It consists in a specific NDEF message construct, as:
  *          - NDEF 1st record: Handover Request or Handover Select record
  *            - Version
  *            - Collision Resolution  (nested record, optional)
  *              - Random number
  *            - Alternative Carrier 1 (nested record)
  *              - Power state
  *              - Reference Data
  *              - Auxiliary Data Reference 1
  *              - ...
  *              - Auxiliary Data Reference N
  *            - ...
  *            - Alternative Carrier N (nested record)
  *          - NDEF record 2+: Alternative Carrier or Auxiliary Data: Connection details for one of the AC (record ID is the AC Reference Data or an Auxiliary Data Reference). 
  *          - ...
  *          - NDEF record N:  Alternative Carrier or Auxiliary Data: Connection details for one of the AC (record ID is the AC Reference Data or an Auxiliary Data Reference).
  *          _________________
  *          @section Handover_Library_Usage NDEF Handover Library usage:
  *          @subsection Static_Handover Static Handover
  *          The Static Handover is used when a device is only equipped with a NFC Tag (no peer-to-peer) to directly propose a set of Alternative Carriers (ACs: bluetooth, wifi,...) 
  *          without running the full handover request-select process.\n
  *          The Handover Select record use in that case is similar to the one used during a regular Handover negociation.\n
  *          1. Declare & initialize an instance of `Ndef_Handover_t`, such as:
  *
  *                 Ndef_Handover_t wHandover = {.type = NDEF_HANDOVER_SELECT_TYPE, .version = NDEF_HANDOVER_VERSION_1_2};
  *          2. Declare a `sRecordInfo_t` instance:
  *
  *                 sRecordInfo_t HandoverRecord;
  *          3. Call the `NDEF_CreateHandover` function, to start preparing the Handover Select record
  *
  *                 NDEF_CreateHandover(&wHandover,&HandoverRecord);
  *          4. Call the `NDEF_AddAlternativeCarrier` for each Alternative Carrier (such as Bluetooth, Wifi, ...) 
  *          5. Declare and initialize as many `Ndef_Handover_alternative_carrier_t` instances as the number of non-NFC protocol available.
  *             And then call the `NDEF_AddAlternativeCarrier` for each of them.
  *
  *                 Ndef_Handover_alternative_carrier_t wAC_BREDR = {.cps = NDEF_HANDOVER_AC_CPS_ACTIVE,
  *                                                                  .aux_data_ref_count = 0};
  *                 Ndef_Handover_alternative_carrier_t wAC_BLE = {.cps = NDEF_HANDOVER_AC_CPS_ACTIVE,
  *                                                                .aux_data_ref_count = 0};
  *                 Ndef_Handover_alternative_carrier_t wAC_Wifi = {.cps = NDEF_HANDOVER_AC_CPS_ACTIVE,
  *                                                                 .aux_data_ref_count = 0};
  *                 
  *                 NDEF_AddAlternativeCarrier(&wAC_BREDR,"urn:nfc:handover:bredr", NULL,&HandoverRecord );
  *                 NDEF_AddAlternativeCarrier(&wAC_BLE,"urn:nfc:handover:ble", NULL,&HandoverRecord );
  *                 NDEF_AddAlternativeCarrier(&wAC_Wifi,"urn:nfc:handover:wifi", NULL,&HandoverRecord );
  *             @note
  *                 1. In this example a single `Ndef_Handover_alternative_carrier_t` could have been used, as the parameters are the same for all ACs.
  *                 2. If more than one NDEF record is required to provide the AC connection details, the `aux_data_ref_count` must specify the number of additional records.
  *
  *                        Ndef_Handover_alternative_carrier_t wAC_Wifi = {.cps = NDEF_HANDOVER_AC_CPS_ACTIVE,
  *                                                                        .aux_data_ref_count = 2};
  *                    Then, it's required to initialize an array of pointers to these Auxiliary Data Reference, as:
  *
  *                        char *aux_data_ref1 = "urn:nfc:handover:wifi:aux_data_ref1";
  *                        char *aux_data_ref2 = "urn:nfc:handover:wifi:aux_data_ref2";
  *                        char *wifi_aux_ref_data_array[2] = {aux_data_ref1,aux_data_ref2};
  *                    And then provide this array as the 3rd argument to the 'NDEF_AddAlternativeCarrier' function:
  *
  *                        NDEF_AddAlternativeCarrier(&wAC_Wifi,"urn:nfc:handover:wifi", wifi_aux_ref_data_array,&HandoverRecord );
  *          6. Call the `NDEF_WriteHandover` function to finalize the NDEF message and write it to the tag.
  *
  *                 NDEF_WriteHandover(&HandoverRecord, NDEF_Buffer);
  *          7. Then call other functions (from the libNDEF or not) to add records, describing the Alternative Carriers, to the NDEF message:
  *
  *                 NDEF_AppendBluetoothOOB ( &w_ble_oob, "urn:nfc:handover:ble" );
  *                 NDEF_AppendBluetoothOOB ( &w_bredr_oob, "urn:nfc:handover:bredr" );
  *              @note
  *                1. The ID of these additional records (2nd argument in the above example) must match the Data Reference provided within the Handover record.
  *                2. If Auxiliary Data References are expected they must also be added to the NDEF message, with their ID matching the ID provided to the Alternative Carrier record.
  *
  *          @section Reading_Handover Reading through a Handover Request message
  *          1. Read the 1st record of the NDEF message:
  *
  *                 sRecordInfo_t rRecord;
  *                 NDEF_ReadNDEF(NDEF_Buffer);
  *                 NDEF_IdentifyBuffer(rRecord,NDEF_Buffer);
  *          2. Decode the handover:
  *
  *                 Ndef_Handover_t rHandover;
  *                 NDEF_ReadHandover(&rRecord ,  &rHandover ); 
  *          3. Read all Alternative Carrier records, and their Auxiliary Data References if any.
  *
  *                 uint8_t ac_index, aux_index;
  *                 for(ac_index=0;ac_index<rHandover.nb_alternative_carrier;ac_index++)
  *                 {
  *                   Ndef_Handover_alternative_carrier_t rAC;
  *                   NDEF_ReadAC( ac_index, &rHandover  , &rAC );
  *                   for(aux_index = 0; aux_index < rAC.aux_data_ref_count; aux_index++)
  *                   {
  *                     sRecordInfo_t AuxRecord;
  *                     NDEF_ReadAuxData( aux_index, &rAC, &AuxRecord );
  *                     // Do something with this Auxiliary Data
  *                   }
  *                   // Process this AC (Extract OOB/or whatever data), and decide if this Carrier is supported or not.
  *                 }
  *          4. Choose the prefered Carrier and write a Handover Select record with the prefered AC as described in the @ref Static_Handover section.
  * @{
  */



/**
  * @brief  This function searches the NDEF message to retrieve the nth Auxiliary Data record if present.
  * @param  aux_data_nb Position of the Auxiliary Data Reference in the Alternative Carrier.
  * @param  pAC         Pointer on the Alternative Carrier structure where to find the Auxiliary Data Reference.
  * @param  pRecord     Pointer to return the output Auxiliary Data record.
  * @retval NDEF_OK     The Auxiliary Data record has been retrieved.
  * @retval NDEF_ERROR  Not able to find the Auxiliary Data in the NDEF message.
  */
uint16_t NDEF_ReadAuxData( uint8_t aux_data_nb, Ndef_Handover_alternative_carrier_t *pAC, sRecordInfo_t *pRecord )
{
  uint16_t status;
  uint8_t* pData = pAC->aux_data_ref_start; 
  uint8_t current_aux = 0;
  uint8_t* aux_id = 0;
  uint8_t  aux_id_length = 0;

  if((pAC->aux_data_ref_start == NULL) ||
     (pAC->aux_data_ref_end == NULL) ||
     (pAC->aux_data_ref_count == 0) ||
     (aux_data_nb >= pAC->aux_data_ref_count))
    return NDEF_ERROR;
  

  while((current_aux < aux_data_nb) && (pData < pAC->aux_data_ref_end))
  {
    aux_id_length = *pData++;
    aux_id = pData;
    pData += aux_id_length;
    current_aux++;
   }

   pData = pAC->aux_data_ref_end;
  /* if ac has been found */
  if(current_aux == aux_data_nb)
  {
    /* let's now look for the corresponding record - must be after the Handover record */
    do {
      status = NDEF_IdentifyBuffer(pRecord,pData);
      if(status != NDEF_OK) return status;
      pData  = pAC->ac_record.PayloadBufferAdd;

      if((pRecord->IDLength == aux_id_length) &&
          !memcmp(pRecord->ID, aux_id,aux_id_length))
      {
        /* this is the record we were looking for, so exit */
        return NDEF_OK;
      }
      
      // go to next record
      pData  = pRecord->PayloadBufferAdd + pRecord->PayloadLength;    
      
    } while (!(pRecord->RecordFlags & MB_Mask));
    
  }
 // if we go there, it means that the record ID is not found
  return NDEF_ERROR;
}



/**
  * @brief  This function reads the NDEF message and retrieves the nth Alternative Carrier record if present.
  * @param  ac_nb       Position of the Alternative Carrier Reference in the Handover record.
  * @param  pHandover   Pointer to the Handover record where to find the AC Reference Data.
  * @param  pAC         Pointer used to return the output Alternative Carrier record.
  * @retval NDEF_OK     The Alternative Carrier record has been retrieved.
  * @retval NDEF_ERROR  Not able to find the Alternative Carrier in the NDEF message.
  */
uint16_t NDEF_ReadAC( uint8_t ac_nb, Ndef_Handover_t *pHandover  , Ndef_Handover_alternative_carrier_t *pAC )
{
  uint16_t status;
  uint8_t* pData = pHandover->ac_start; 
  uint8_t current_ac = 0;
  uint8_t* ac_id;
  uint8_t  ac_id_length;
  sRecordInfo_t NestedRecord;
  uint8_t ac_found = 0;

  if((pHandover->ac_start == NULL) ||
     (pHandover->ac_end == NULL) ||
     (pHandover->nb_alternative_carrier == 0) ||
     (ac_nb >= pHandover->nb_alternative_carrier))
    return NDEF_ERROR;
  
  // Default handover init
  pAC->aux_data_ref_count = 0;

  while((current_ac <= ac_nb)&&(current_ac <= pHandover->nb_alternative_carrier) && (pData < pHandover->ac_end))
  {
     status = NDEF_IdentifyBuffer(&NestedRecord,pData);
     if(status != NDEF_OK) return status;
     // go to payload address
     pData  = NestedRecord.PayloadBufferAdd;
    
     if(!memcmp(NestedRecord.Type,NDEF_HANDOVER_ALTERNATIVE_CARRIER_TYPE_STR,strlen(NDEF_HANDOVER_ALTERNATIVE_CARRIER_TYPE_STR)))
     {  
       if(current_ac == ac_nb)
       {
         // parse the AC now
          pAC->cps = pData[0] & NDEF_HANDOVER_AC_CPS_MASK;
          ac_id_length = pData[1];
          ac_id = &pData[2];
          pAC->aux_data_ref_count = pData[2 + ac_id_length];
          pAC->aux_data_ref_start = &pData[3 + ac_id_length];
          pAC->aux_data_ref_end = pData + NestedRecord.PayloadLength;
          ac_found = 1;
       }
       current_ac++;
     }
     // go to next record
     pData  += NestedRecord.PayloadLength;    
  }
  pData = pHandover->ac_end;
  /* if ac has been found */
  if(ac_found)
  {
    /* let's now look for the corresponding record - must be after the Handover record */
    do {
      status = NDEF_IdentifyBuffer(&pAC->ac_record,pData);
      if(status != NDEF_OK) return status;
      pData  = pAC->ac_record.PayloadBufferAdd;

      if((pAC->ac_record.IDLength == ac_id_length) &&
          !memcmp(pAC->ac_record.ID, ac_id,ac_id_length))
      {
        /* this is the record we were looking for, so exit */
        return NDEF_OK;
      }
      
      // go to next record
      pData  = pAC->ac_record.PayloadBufferAdd + pAC->ac_record.PayloadLength;    
      // TO DO: add a security condition to avoid infinite loop if NDEF file is corrupted
    } while (!(pAC->ac_record.RecordFlags & ME_Mask));
    
  }
 // if we go there, it means that the record ID is not found
  return NDEF_ERROR;
}


/**
  * @brief  This function reads a record and retrieves the Handover information if present.
  * @param  pRecord       Pointer on the record structure to be read.
  * @param  pHandover     Pointer used to return the Handover information.
  * @retval NDEF_OK       Handover information has been retrieved from the record.
  * @retval NDEF_ERROR    Not able to read the Handover information from the record.
  */
uint16_t NDEF_ReadHandover(sRecordInfo_t *pRecord ,  Ndef_Handover_t *pHandover )
{
  uint16_t status;
  uint8_t* pData = pRecord->PayloadBufferAdd;
  uint8_t* pEnd = pData + pRecord->PayloadLength;
  sRecordInfo_t NestedRecord;

  /* Default Handover Structure init */
  pHandover->version =0;
  pHandover->nb_alternative_carrier =0;
  pHandover->has_cr = 0; 
  pHandover->ac_start = NULL; 
  pHandover->ac_end = NULL; 

  /* A Handover record should never be the end of the NDEF message */
  if(pRecord->RecordFlags & ME_Mask)
    return NDEF_ERROR;

  if( !memcmp(pRecord->Type,NDEF_HANDOVER_REQUEST_TYPE_STR,strlen(NDEF_HANDOVER_REQUEST_TYPE_STR)) )
  {
    pHandover->type = NDEF_HANDOVER_REQUEST_TYPE;
  } else if ( !memcmp(pRecord->Type,NDEF_HANDOVER_SELECT_TYPE_STR,strlen(NDEF_HANDOVER_SELECT_TYPE_STR)) )
  {
    pHandover->type = NDEF_HANDOVER_SELECT_TYPE;
  } else {
    /* This is not a Handover record! */
    return NDEF_ERROR;
  }  

  pHandover->version = *pData++;
  
  /* Following records are nested into Hr/s record */
  while (pData < pEnd)
  {
     status = NDEF_IdentifyBuffer(&NestedRecord,pData);
     if(status != NDEF_OK) return status;
    /* save record address */
    uint8_t* pACRecord = pData; 
    /* go to payload address */
     pData  = NestedRecord.PayloadBufferAdd;

    /* Parse Collision Resolution if Handover request */
    if(pHandover->type == NDEF_HANDOVER_REQUEST_TYPE)
    {

      if(!memcmp(NestedRecord.Type,NDEF_HANDOVER_COLLISION_RESOLUTION_TYPE_STR,strlen(NDEF_HANDOVER_COLLISION_RESOLUTION_TYPE_STR)))
      {  
        pHandover->has_cr = 1; 
        pHandover->cr_random_number = *(uint16_t*)pData;
      }
    }

  /* Parse AlternativeCarriers just to know how many they are */
    else if(!memcmp(NestedRecord.Type,NDEF_HANDOVER_ALTERNATIVE_CARRIER_TYPE_STR,strlen(NDEF_HANDOVER_ALTERNATIVE_CARRIER_TYPE_STR)))
    {  
      pHandover->nb_alternative_carrier++;
      if((pHandover->ac_start) == NULL)	{
        pHandover->ac_start = pACRecord;
       }
      pHandover->ac_end = pData + NestedRecord.PayloadLength;
      /* don't parse the AC now */
    }
    else {
      /* this is an unexpected type, just ignore it */
    }
    /* go to next record */
     pData  += NestedRecord.PayloadLength;    

  }    
 
  return NDEF_OK;
}

/**
  * @brief  This function prepares the Handover record with the data given in the Handover structure.
  * @param  pHandover   Pointer on structure containing the handover basic information.
  * @param  pRecord     Pointer used to return the prepared Handover record. 
  * @retval NDEF_OK     The record has been prepared.
  * @retval NDEF_ERROR  The record has not been prepared.
  */
uint16_t NDEF_CreateHandover(Ndef_Handover_t  *pHandover, sRecordInfo_t* pRecord )
{
  uint16_t status = NDEF_ERROR;

  /* Use a static buffer to prepare the Handover record */
  pRecord->PayloadBufferAdd = NDEF_Record_Buffer;
  /* Alternative, where the user must first allocate the Payload buffer in the record:
   * if (pRecord->PayloadBufferAdd == NULL)
   *  return NDEF_ERROR;
   */
 
  /* Handover MUST be the first record (SR mask to be updated when actually writing the record) */
  pRecord->RecordFlags = MB_Mask | ME_Mask | TNF_WellKnown;

  if(pHandover->type == NDEF_HANDOVER_SELECT_TYPE)
  {
    pRecord->TypeLength = strlen(NDEF_HANDOVER_SELECT_TYPE_STR);
    memcpy(pRecord->Type, NDEF_HANDOVER_SELECT_TYPE_STR,pRecord->TypeLength); 
  }
  else if (pHandover->type == NDEF_HANDOVER_REQUEST_TYPE)
  {
    pRecord->TypeLength = strlen(NDEF_HANDOVER_SELECT_TYPE_STR);
    memcpy(pRecord->Type, NDEF_HANDOVER_REQUEST_TYPE_STR,pRecord->TypeLength); 
  }
  else
    return NDEF_ERROR;
  
  pRecord->PayloadLength = sizeof(pHandover->version);  
  *pRecord->PayloadBufferAdd = pHandover->version;
  
  /* Don't write the record for now, additionnal Alternative Carriers to come as nested records. */
  
  return status;
}


/**
  * @brief  This function adds an Alternative Carrier record to a Handover record using the data given in the AC structure.
  * @param  pAC             Pointer on input AC structure. 
  * @param  CarrierDataRef  String with the Alternative Carrier Data Reference (ID of the corresponding record in the NDEF message).
  * @param  AuxDataRefID    Array with pointers to the Auxiliary Data References (as many as defined in pAC structure).
  * @param  pRecord         Pointer on the Handover record to be filled with the AC data, must have been previously initialized with the NDEF_CreateHandover function.
  * @retval NDEF_OK                     The Handover record has been updated with AC information.
  * @retval NDEF_ERROR                  The Handover record cannot be updated with the AC information.
  * @retval NDEF_ERROR_MEMORY_INTERNAL  The internal buffer for records is too small to add the AC information.
  */
uint16_t NDEF_AddAlternativeCarrier(Ndef_Handover_alternative_carrier_t *pAC,char *CarrierDataRef, char **AuxDataRefID, sRecordInfo_t* pRecord )
{
  /* Specific buffer to prepare the Alternative Carrier record */
  uint8_t NDEF_AlternativeCarrier_Buffer[NDEF_AC_BUFFER_SIZE];
  
  /* check that there is enough space in the buffers */
  pAC->ac_record.PayloadLength = NDEF_GetACDataLength(pAC,CarrierDataRef,AuxDataRefID);
  if(((pRecord->PayloadLength + pAC->ac_record.PayloadLength) > NDEF_RECORD_MAX_SIZE) ||
      (pAC->ac_record.PayloadLength > NDEF_AC_BUFFER_SIZE))
    return NDEF_ERROR_MEMORY_INTERNAL;

  /* Use specific buffer to prepare the nested record */
  uint8_t *pData =  NDEF_AlternativeCarrier_Buffer;
  pAC->ac_record.PayloadBufferAdd = pData;
  /* Following line is an alternative where the user must allocate the payload buffer of the ac_record:
   * uint8_t* pData =   pAC->ac_record.PayloadBufferAdd ;
   */
  
  if ((pRecord->PayloadBufferAdd == NULL) ||
      (pRecord->PayloadLength == 0))
      return NDEF_ERROR;

  /* AC is not the first record */
  pAC->ac_record.RecordFlags =   TNF_WellKnown;

  pAC->ac_record.TypeLength = strlen(NDEF_HANDOVER_ALTERNATIVE_CARRIER_TYPE_STR);  
  memcpy(pAC->ac_record.Type, NDEF_HANDOVER_ALTERNATIVE_CARRIER_TYPE_STR,pAC->ac_record.TypeLength);  

  /* Length : cps byte + data ref length byte + auxiliary data ref count byte + data ref length */
  *pData++ = pAC->cps & NDEF_HANDOVER_AC_CPS_MASK;
  *pData++ = strlen(CarrierDataRef);
  memcpy(pData, CarrierDataRef, strlen(CarrierDataRef));
  pData += strlen(CarrierDataRef);
  *pData++ = pAC->aux_data_ref_count;
  
  uint8_t AuxDataIndex;
  for(AuxDataIndex = 0;AuxDataIndex<pAC->aux_data_ref_count;AuxDataIndex++)
  {
    *pData++ = strlen(AuxDataRefID[AuxDataIndex]);
    memcpy(pData, AuxDataRefID[AuxDataIndex], strlen(AuxDataRefID[AuxDataIndex]));
    pData += strlen(AuxDataRefID[AuxDataIndex]);
  }
  
  /* Append the nested record right after the Handover record - increase its length accordingly */
  pRecord->PayloadLength += NDEF_WriteRecord(&pAC->ac_record,pRecord->PayloadBufferAdd + pRecord->PayloadLength);
  return NDEF_OK;
}


/**
  * @brief  This function returns the length of an Alternative Carrier record data (excluding the record metadata).
  * @param  pAC             Pointer on the input AC structure.
  * @param  CarrierDataRef  String with the Alternative Carrier Data Reference.
  * @param  AuxDataRefID    Array with the Auxiliary Data References (as many as defined in the pAC structure).
  * @return The computed length in bytes corresponding to the provided Alternative Carrier information.
  */
uint32_t NDEF_GetACDataLength(Ndef_Handover_alternative_carrier_t *pAC,char *CarrierDataRef, char **AuxDataRefID)
{
  uint8_t AuxDataIndex;

  /* First compute the Data length */
  uint32_t length =       1 +                           // cps
                          1 +                           // Carrier data ref length
                          strlen(CarrierDataRef) +      // Carrier data ref
                          1 +                           // auxiliary data count
                          pAC->aux_data_ref_count * 1;  // auxiliary data lengths
  
  /* Then adds the length of the Auxiliary Data */
  for(AuxDataIndex = 0;AuxDataIndex<pAC->aux_data_ref_count;AuxDataIndex++)
  {
     length += strlen(AuxDataRefID[AuxDataIndex]);
  }

  return length;
}

/**
  * @brief  This function writes the Handover record into the memory.
  * @param  pRecord   Pointer on the input Handover record.
  * @param  pNdef     Pointer to a NDEF buffer (used to prepare the data before actual writing to the memory).
  * @retval NDEF_OK   The memory has been written.
  * @retval NDEF_ERROR_MEMORY_INTERNAL  Cannot write to tag.
  * @retval NDEF_ERROR_NOT_FORMATED CCFile data not supported or not present.
  * @retval NDEF_ERROR_MEMORY_TAG Size not compatible with memory.
  * @retval NDEF_ERROR_LOCKED Tag locked, cannot be write.
  */
uint16_t NDEF_WriteHandover( sRecordInfo_t* pRecord , uint8_t* pNdef)
{
  /* Note: in case of Handover Select for no matching alternative carrier, the ME bit flag must be set by the caller */
  
  uint32_t Size = NDEF_WriteRecord(pRecord,pNdef);
  return NDEF_WriteNDEF(Size,pNdef);
}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
