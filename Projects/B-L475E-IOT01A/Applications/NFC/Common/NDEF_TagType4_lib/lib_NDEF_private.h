/**
  ******************************************************************************
  * @file    lib_NDEF.h
  * @author  MMY Application Team
  * @brief   This file help to manage NDEF file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MMY-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIB_NDEF_PRIVATE_H
#define __LIB_NDEF_PRIVATE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* include file which match the HW configuration */
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "lib_NDEF.h"
#include "m24sr_wrapper.h"
#include "stm32l4xx.h"

/** @addtogroup NFC_libraries
  * @{
  */

/** @addtogroup TAGTYPE4_NDEF_PRIVATE
 *  @{
 */

/**
 * @defgroup TAGTYPE4_NDEF_Private_Constants             TAGTYPE4_NDEF Private Constants
 * @{
 */

#define NDEF_ERROR          ERROR
#define NDEF_SUCCESS        SUCCESS

#define NDEF_SIZE_OFFSET           0
#define FIRST_RECORD_OFFSET        2

#define RECORD_FLAG_FIELD          1
#define TYPE_LENGTH_FIELD          1
#define ID_LENGTH_FIELD            1

/**
  * @}
  */

/** @defgroup lib_NDEF_Record_area_mask
  * @{
  */
#define  MB_Mask                  ((uint8_t)(0x80))
#define ME_Mask                   ((uint8_t)(0x40))
#define CF_Mask                   ((uint8_t)(0x20))
#define SR_Mask                   ((uint8_t)(0x10))
#define IL_Mask                   ((uint8_t)(0x08))
#define TNF_Mask                  ((uint8_t)(0x07))
/**
  * @}
  */

/** @defgroup lib_NDEF_TNF_type
  * @{
  */
#define TNF_Empty                  0x00
#define TNF_WellKnown              0x01
#define TNF_MediaType              0x02
#define TNF_AbsoluteURI            0x03
#define TNF_NFCForumExternal       0x04
#define TNF_Unknown                0x05
#define TNF_Unchanged              0x06
#define TNF_Reserved               0x07
/**
  * @}
  */

/** @defgroup lib_NDEF_well_known_string
  * @{
  */
#define AAR_TYPE_STRING                          "android.com:pkg"
#define AAR_TYPE_STRING_LENGTH                   15

#define M24SR_DISCOVERY_APP_STRING              "st.com:m24sr_discovery_democtrl"
#define M24SR_DISCOVERY_APP_STRING_LENGTH         31

#define VCARD_TYPE_STRING                        "text/vcard"
#define VCARD_TYPE_STRING_LENGTH                 10

#define XVCARD_TYPE_STRING                      "text/x-vCard"
#define XVCARD_TYPE_STRING_LENGTH                 12

#define SMART_POSTER_TYPE_STRING                "Sp"
#define SMART_POSTER_TYPE_STRING_LENGTH           2

#define URI_TYPE_STRING                          "U"
#define URI_TYPE_STRING_LENGTH                  1

#define SMS_TYPE_STRING                          "sms:"
#define SMS_TYPE_STRING_LENGTH                   4

#define GEO_TYPE_STRING                          "geo:"
#define GEO_TYPE_STRING_LENGTH                   4

#define URI_LATITUDE_END                        ","
#define URI_LATITUDE_END_LENGTH                  1

#define EMAIL_TYPE_STRING                        "mailto:"
#define EMAIL_TYPE_STRING_LENGTH                7

#define URI_FIRST_DATA_END                      "?"
#define URI_FIRST_DATA_END_LENGTH                1

#define SUBJECT_BEGIN_STRING                    "subject="
#define SUBJECT_BEGIN_STRING_LENGTH              8

#define MESSAGE_BEGIN_STRING                    "body="
#define MESSAGE_BEGIN_STRING_LENGTH              5

#define URI_SECOND_DATA_END                      "&"
#define URI_SECOND_DATA_END_LENGTH              1

#define TEXT_TYPE_STRING                        "T"
#define TEXT_TYPE_STRING_LENGTH                  1

#define ISO_ENGLISH_CODE_STRING                  "en"
#define ISO_ENGLISH_CODE_STRING_LENGTH          2
/**
  * @}
  */

/** @defgroup TAGTYPE4_NDEF_Private_FunctionsPrototypes   TAGTYPE4_NDEF Private FunctionsPrototypes
  * @{
  */
uint16_t NDEF_IdentifyNDEF ( sRecordInfo *pRecordStruct, uint8_t* pNDEF );
uint16_t NDEF_ReadNDEF( uint8_t *pNDEF);
uint16_t NDEF_WriteNDEF( uint8_t *pNDEF);
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
#endif /* __LIB_NDEF_PRIVATE_H */


/******************* (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
