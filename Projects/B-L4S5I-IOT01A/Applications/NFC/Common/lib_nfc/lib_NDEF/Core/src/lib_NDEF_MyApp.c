/**
  ******************************************************************************
  * @file    lib_NDEF_MyApp.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file help to manage the NDEF file of a private application.
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

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF_MyApp.h"


/** @addtogroup NFC_libraries
  * @{
  * @brief  <b>This is the library used to manage the content of the TAG (data)
  *          But also the specific feature of the tag, for instance
  *          password, gpio... </b>
  */


/** @addtogroup libNFC_FORUM
  * @{
  * @brief  This part of the library manage data which follow NFC forum organisation.
  */

/**
  * @brief  This buffer contains the data send/received by TAG
  */
extern uint8_t NDEF_Buffer [];

/** @defgroup libMyApp_Private_Functions
  * @{
  */

static void NDEF_Extract_M24SRDiscoveryApp_Input( sRecordInfo_t *pRecordStruct, sMyAppInfo *pMyAppStruct );

/**
  * @brief  This function read the NDEF file and store application data in a structure.
  * @param  pRecordStruct : Pointer on the record structure.
  * @param  pMyAppStruct : pointer on the structure to fill.
  */
static void NDEF_Extract_M24SRDiscoveryApp_Input( sRecordInfo_t *pRecordStruct, sMyAppInfo *pMyAppStruct )
{
  uint8_t* pPayload;
  uint8_t* pLook4Word;
  uint16_t BackGroundColor, FontColor;
  uint8_t i;

  /* Read record header */
  pPayload = (uint8_t*)(pRecordStruct->PayloadBufferAdd);

  /* initialize struct in case not matching found */
  for( i = 0; i < 8; i++ )
  {
    /* Set the Back Color */
    pMyAppStruct->LineX[i].BackGroundColor = 0xFFFF;
    /* Set the Text Color */
    pMyAppStruct->LineX[i].FontColor = 0x0000;
    /* Set the line number */
    pMyAppStruct->LineX[i].LineNb = i + 1; 
    /* Set the line content */
    memcpy( pMyAppStruct->LineX[i].String, "                    ", 20 );
  }

  pLook4Word = pPayload;

  for( i = 0; i < 4; i++ )
    {
      pMyAppStruct->LedBlinkConf.LedConf[i] = *pLook4Word;
      pLook4Word++;
    }
    pMyAppStruct->LedBlinkConf.Speed = *pLook4Word;
    pLook4Word++;

  for( i = 0; i < 8; i++ )
  {
      /* Set the line number */
      pMyAppStruct->LineX[i].LineNb = *pLook4Word; 
      pLook4Word++;
      /* Set the Back Color */
      BackGroundColor = (uint16_t)(*pLook4Word << 8);
      BackGroundColor = BackGroundColor | (uint16_t)(*++pLook4Word );
      pMyAppStruct->LineX[i].BackGroundColor = BackGroundColor;
      pLook4Word++;
      /* Set the Text Color */
      FontColor = (uint16_t)(*pLook4Word << 8);
      FontColor = FontColor | (uint16_t)(*++pLook4Word);
      pMyAppStruct->LineX[i].FontColor = FontColor;
      pLook4Word++;
      /* Set the line content */
      memcpy( pMyAppStruct->LineX[i].String, (char*)pLook4Word, 20 );
      pLook4Word += 20;
    }

}

/**
  * @}
  */

/** @defgroup libMyApp_Public_Functions
  * @{
  * @brief  This file is used to manage proprietary NDEF (stored or loaded in tag)
  */ 

/**
  * @brief  This function read NDEF and retrieve Application information if any.
  * @param  pRecordStruct : Pointer on the record structure.
  * @param  pMyAppStruct : pointer on the structure to fill.
  * @retval NDEF_OK : NDEF file data read in the tag.
  * @retval NDEF_ERROR : not able to read NDEF in tag.
  */
uint16_t NDEF_ReadMyApp( sRecordInfo_t *pRecordStruct, sMyAppInfo *pMyAppStruct )
{
  uint16_t status = NDEF_ERROR;

  if( pRecordStruct->NDEF_Type == M24SR_DISCOVERY_APP_TYPE )
  {
    NDEF_Extract_M24SRDiscoveryApp_Input( pRecordStruct, pMyAppStruct );
    status = NDEF_OK;
  }
  else 
  {
    status = NDEF_ERROR;
  }

  return status;
}

/**
  * @brief  This function write the NDEF file with the Application data given in the structure.
  * @brief  Only used for debug purpose in this firmware version.
  * @param  pMyAppStruct : pointer on structure that contain the application information.
  * @retval NDEF_OK : NDEF file data written in the tag.
  * @retval NDEF_ERROR : not able to store NDEF in tag.
  * @retval NDEF_ERROR_MEMORY_INTERNAL : Cannot write to tag.
  * @retval NDEF_ERROR_NOT_FORMATED : CCFile data not supported or not present.
  * @retval NDEF_ERROR_MEMORY_TAG : Size not compatible with memory.
  * @retval NDEF_ERROR_LOCKED : Tag locked, cannot be write.
  */
uint16_t NDEF_WriteMyApp( sMyAppInfo *pMyAppStruct )
{
  uint16_t status = NDEF_ERROR;
  uint16_t DataSize;
  uint32_t PayloadSize;
  uint8_t  i;
  uint8_t* pPayload;


/* External Type Record Header */
/************************************/
/*  7 |  6 |  5 |  4 |  3 | 2  1  0 */
/*----------------------------------*/
/* MB   ME   CF   SR   IL    TNF    */  /* <---- IL=0, CF=0 and SR=1 TNF=4 NFC Forum external type*/
/*----------------------------------*/
/*          TYPE LENGTH             */
/*----------------------------------*/
/*        PAYLOAD LENGTH 3          */  /* <---- Not Used  */
/*----------------------------------*/
/*        PAYLOAD LENGTH 2          */  /* <---- Not Used  */
/*----------------------------------*/
/*        PAYLOAD LENGTH 1          */  /* <---- Not Used  */
/*----------------------------------*/
/*        PAYLOAD LENGTH 0          */  /* The payload will always be 5 + 25*8=205 bytes for this application */
/*----------------------------------*/
/*          ID LENGTH               */  /* <---- Not Used  */
/*----------------------------------*/
/*              TYPE                */  /* st.com:m24sr_discovery_democtrl */
/*----------------------------------*/
/*               ID                 */  /* <---- Not Used  */
/************************************/

 
  /* fill URI record header */
  NDEF_Buffer[0] = 0xD4;   /* Record Flag */
  NDEF_Buffer[1] = M24SR_DISCOVERY_APP_STRING_LENGTH;
  NDEF_Buffer[2] = 0x00; /* Will be filled at the end when payload size is known */

  memcpy( &NDEF_Buffer[3], M24SR_DISCOVERY_APP_STRING, M24SR_DISCOVERY_APP_STRING_LENGTH );

  pPayload = &NDEF_Buffer[ 3 + M24SR_DISCOVERY_APP_STRING_LENGTH];
  PayloadSize = 0;

  /**************************************************************/
  /* BLINK CONFIG data */

  /* led 1 blinking */
  *pPayload = 0x03;
  pPayload++;
  /* led 2 blinking */
  *pPayload = 0x02;
  pPayload++;
  /* led 3 blinking */
  *pPayload = 0x02;
  pPayload++;
  /* led 4 blinking */
  *pPayload = 0x03;
  pPayload++;
  /* speed */
  *pPayload = 0x03;
  pPayload++;

  PayloadSize += 5;

  /**************************************************************/

  /**************************************************************/
  /* SCREEN config  data */
  for( i = 0; i < 8; i++ )
  {
    /* Line number */
    *pPayload = (uint8_t)(i + 1);
    pPayload++;
    /* Background color */
    *pPayload = 0xFF - (0xFF/i);
    pPayload++;
    *pPayload = 0xFF - (0xFF/i);
    pPayload++;
    /* Font Color */
    *pPayload = 0xFF/i;
    pPayload++;
    *pPayload = 0xFF/i;
    pPayload++;
    /* String */
    memcpy( pPayload, "ABCDEFGHIJKLMNOPQRST", 20 );
    pPayload += 20;

    PayloadSize += 25;

  }

  NDEF_Buffer[2] = PayloadSize & 0x000000FF;

  DataSize = PayloadSize + 5 + M24SR_DISCOVERY_APP_STRING_LENGTH;

  /* Write NDEF */
  status = NfcTag_WriteNDEF(  DataSize, NDEF_Buffer );


  return status;
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
