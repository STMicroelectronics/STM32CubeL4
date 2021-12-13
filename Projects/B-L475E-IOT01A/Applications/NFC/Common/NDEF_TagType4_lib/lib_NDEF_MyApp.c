/**
  ******************************************************************************
  * @file    lib_NDEF_MyApp.c
  * @author  MMY Application Team
  * @brief   This file help to manage the NDEF file of a private application.
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

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF_private.h"
#include "lib_NDEF_MyApp.h"


/** @addtogroup NFC_libraries
 *  @{
 */


/** @addtogroup TAGTYPE4_NDEF_MYAPP
  * @{
  */

/**
 * @brief  This buffer contains the data send/received by TAG
 */
extern uint8_t NDEF_Buffer [NFC_TT4_NDEF_MAX_SIZE];

/** @defgroup TAGTYPE4_NDEF_MYAPP_Private_Functions    NDEF_MYAPP Private Functions
  * @{
  */

static void NDEF_Extract_M24SRDiscoveryApp_Input ( sRecordInfo *pRecordStruct, sMyAppInfo *pMyAppStruct );

/**
  * @brief  This fonction read the NDEF file and store application data in a structure
  * @param  pRecordStruct : Pointer on the record structure
  * @param  pMyAppStruct : pointer on the structure to fill
  * @retval NONE 
  */
static void NDEF_Extract_M24SRDiscoveryApp_Input ( sRecordInfo *pRecordStruct, sMyAppInfo *pMyAppStruct )
{
  uint8_t* pPayload;
  uint8_t* pLook4Word;
  uint16_t BackGroundColor, FontColor;
  uint8_t i;
  
  /* Read record header */
  pPayload = (uint8_t*)(pRecordStruct->PayloadBufferAdd);
  
  /* initialize struct in case not matching found */
  for(i=0; i<8; i++)
  {
    /* Set the Back Color */
    pMyAppStruct->LineX[i].BackGroundColor = 0xFFFF;
    /* Set the Text Color */
    pMyAppStruct->LineX[i].FontColor = 0x0000;
    /* Set the line number */
    pMyAppStruct->LineX[i].LineNb = i+1; 
    /* Set the line content */
    memcpy(pMyAppStruct->LineX[i].String , "                    ",20);
  }  
  
  pLook4Word = pPayload;
  
  for(i=0; i<4; i++)
  {
    pMyAppStruct->LedBlinkConf.LedConf[i] = *pLook4Word;
    pLook4Word++;
  }
  pMyAppStruct->LedBlinkConf.Speed = *pLook4Word;
  pLook4Word++;
  
  for(i=0; i<8; i++)
  {    
    /* Set the line number */
    pMyAppStruct->LineX[i].LineNb = *pLook4Word; 
    pLook4Word++;
    /* Set the Back Color */
    BackGroundColor = (uint16_t) (*pLook4Word << 8);
    BackGroundColor = BackGroundColor | (uint16_t) (*++pLook4Word );
    pMyAppStruct->LineX[i].BackGroundColor = BackGroundColor;
    pLook4Word++;
    /* Set the Text Color */
    FontColor = (uint16_t) (*pLook4Word << 8);
    FontColor = FontColor | (uint16_t) (*++pLook4Word );
    pMyAppStruct->LineX[i].FontColor = FontColor;
    pLook4Word++;    
    /* Set the line content */
    memcpy(pMyAppStruct->LineX[i].String, (char*) pLook4Word, 20);
    pLook4Word += 20;
  }  
  
}

/**
  * @}
  */

/** @defgroup TAGTYPE4_NDEF_MYAPP_Public_Functions  NDEF_MYAPP Public Functions
  * @{
  *  @brief  This file is used to manage proprietary NDEF (stored or loaded in tag)
  */ 

/**
  * @brief  This fonction read NDEF and retrieve Application information if any
  * @param  pRecordStruct : Pointer on the record structure
  * @param  pMyAppStruct : pointer on the structure to fill 
  * @retval SUCCESS : Application information from NDEF have been retrieved
  * @retval ERROR : Not able to retrieve Application information
  */
uint16_t NDEF_ReadMyApp(sRecordInfo *pRecordStruct, sMyAppInfo *pMyAppStruct)
{
  uint16_t status = NDEF_ERROR;
  
  if( pRecordStruct->NDEF_Type == M24SR_DISCOVERY_APP_TYPE )
  {    
    NDEF_Extract_M24SRDiscoveryApp_Input(pRecordStruct, pMyAppStruct );
    status = NDEF_SUCCESS;
  }
  else 
  {
    status = NDEF_ERROR;
  }
  
  return status;
}

/**
  * @brief  This fonction write the NDEF file with the Application data given in the structure
  * @brief  Only used for debug purpose in this firmware version
  * @param  pMyAppStruct : pointer on structure that contain the application information
  * @retval SUCCESS : the function is succesful
  * @retval ERROR : Not able to store NDEF file inside tag.
  */
uint16_t NDEF_WriteMyApp(sMyAppInfo *pMyAppStruct)
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
  
  /* NDEF file must be written in 2 phases, first phase NDEF size is Null */
  NDEF_Buffer[NDEF_SIZE_OFFSET] = 0x00;
  NDEF_Buffer[NDEF_SIZE_OFFSET+1] = 0x00;
  
  /* fill URI record header */
  NDEF_Buffer[FIRST_RECORD_OFFSET] = 0xD4;   /* Record Flag */
  NDEF_Buffer[FIRST_RECORD_OFFSET+1] = M24SR_DISCOVERY_APP_STRING_LENGTH;
  NDEF_Buffer[FIRST_RECORD_OFFSET+2] = 0x00; /* Will be filled at the end when payload size is known */
  
  memcpy(&NDEF_Buffer[FIRST_RECORD_OFFSET+3], M24SR_DISCOVERY_APP_STRING, M24SR_DISCOVERY_APP_STRING_LENGTH);
  
  pPayload = &NDEF_Buffer[FIRST_RECORD_OFFSET+3+M24SR_DISCOVERY_APP_STRING_LENGTH];
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
  for(i=0; i<8; i++)
  {
    /* Line number */
    *pPayload = (uint8_t)(i+1);
    pPayload++;
    /* Background color */
    *pPayload = 0xFF;
    pPayload++;
    *pPayload = 0xFF;
    pPayload++;
    /* Font Color */
    *pPayload = 0x00;
    pPayload++;
    *pPayload = 0x00;
    pPayload++;
    /* String */
    memcpy(pPayload, "ABCDEFGHIJKLMNOPQRST",20);
    pPayload += 20;
    
    PayloadSize += 25;
    
  }
  
  NDEF_Buffer[FIRST_RECORD_OFFSET+2] = (PayloadSize & 0x000000FF);
  
  DataSize = PayloadSize + 5 + M24SR_DISCOVERY_APP_STRING_LENGTH;
  
  /* Write NDEF */
  status = NFC_TT4_WriteData ( 0x00 , DataSize , NDEF_Buffer);
  
  /* Write NDEF size to complete*/
  if( status == NFC_TT4_ACTION_COMPLETED)
  {
    DataSize -= 2; /* Must not count the 2 byte that represent the NDEF size */
    NDEF_Buffer[0] = (DataSize & 0xFF00)>>8;
    NDEF_Buffer[1] = (DataSize & 0x00FF);
    
    status = NFC_TT4_WriteData ( 0x00 , 2 , NDEF_Buffer);
  }
  
  if( status == NFC_TT4_ACTION_COMPLETED)
    return NDEF_SUCCESS;
  else
    return NDEF_ERROR;
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



