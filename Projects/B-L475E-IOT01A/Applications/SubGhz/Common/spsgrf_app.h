/**
******************************************************************************
  * @file    spsgrf_app.h
  * @author  MCD Application Team
  * @brief   This file provides some abstraction (upper layer) to SPSGRF driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        http://www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPSGRF_APP_H
#define __SPSGRF_APP_H


/* Includes ------------------------------------------------------------------*/
#include "spsgrf.h"
#include "stm32l4xx.h"
#include "spsgrf_io.h"

#ifdef __cplusplus
  "C" {
#endif


/**
 * @addtogroup SPSGRF_APP
 * @{
 */

/**
 * @defgroup SPSGRF_APP_Exported_Constants                SPSGR_GPIO Exported Constants
 * @{
 */
   
#define CUT_MAX_NO 3
#define CUT_2_1v3 0x0103
#define CUT_2_1v4 0x0104
#define CUT_3_0   0x0130

/**
* @}
*/

/**
 * @defgroup SPSGRF_APP_Exported_Types                SPSGR_GPIO Exported Types
 * @{
 */
    
typedef struct 
{
  uint16_t nSpiritVersion;
  SpiritVersion xSpiritVersion;
}SpiritVersionMap;


/**
 * @brief  Range extender type
 */
typedef enum
{
  RANGE_EXT_NONE = 0x00,
  RANGE_EXT_SKYWORKS_169,
  RANGE_EXT_SKYWORKS_868
} RangeExtType;

/**
* @}
*/

/**
 * @defgroup SPSGRF_APP_Exported_Functions                SPSGR_GPIO Exported Functions
 * @{
 */
void SpiritManagementIdentificationRFBoard(void);

RangeExtType SpiritManagementGetRangeExtender(void);
void SpiritManagementSetRangeExtender(RangeExtType xRangeType);
void SpiritManagementRangeExtInit(void);
void SpiritManagementSetBand(uint8_t value);
uint8_t SpiritManagementGetBand(void);

uint8_t SdkEvalGetHasEeprom(void);

void Spirit1InterfaceInit(void);
void Spirit1GpioIrqInit(SGpioInit *pGpioIRQ);
void Spirit1RadioInit(SRadioInit *pRadioInit);
void Spirit1SetPower(uint8_t cIndex, float fPowerdBm);
void Spirit1PacketConfig(void);
void Spirit1SetPayloadlength(uint8_t length);
void Spirit1SetDestinationAddress(uint8_t address);
void Spirit1EnableTxIrq(void);
void Spirit1EnableRxIrq(void);
void Spirit1DisableIrq(void);
void Spirit1SetRxTimeout(float cRxTimeOut);
void Spirit1EnableSQI(void);
void Spirit1SetRssiTH(int dbmValue);
float Spirit1GetRssiTH(void);
void Spirit1ClearIRQ(void);
void Spirit1StartRx(void);
void Spirit1GetRxPacket(uint8_t *buffer, uint8_t *size );
void Spirit1StartTx(uint8_t *buffer, uint8_t size);
int32_t SpiritManagementGetOffset(void);
void SpiritManagementSetOffset(int32_t value);
void SdkEvalSetHasEeprom(uint8_t eeprom);

/**
* @}
*/

/**
* @}
*/

#ifdef __cplusplus
}
#endif


#endif /* __SPSGRF_APP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

