/**
******************************************************************************
  * @file    spsgrf.c
  * @author  MCD Application Team
  * @brief   This file provides the actual driver of SPSGRF module.
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

/* Includes ------------------------------------------------------------------*/
#include "spsgrf.h"
#include "spsgrf_io.h"
#include <math.h>

/**
 * @addtogroup SPSGRF
 * @{
 */

 /* --------------------------------   Management  ----------------------------------- */ 


/** @defgroup Management_Private_Defines                      Management Private Defines
* @{
*/

#define COMMUNICATION_STATE_TX          0
#define COMMUNICATION_STATE_RX          1
#define COMMUNICATION_STATE_NONE        2

/**
* @}
*/

/** @defgroup Management_Private_Variables                      Management Private Variables
* @{
*/

/**
* @brief  BS value to write in the SYNT0 register according to the selected band
*/
static const uint8_t s_vectcBandRegValue[4]={SYNT0_BS_6, SYNT0_BS_12, SYNT0_BS_16, SYNT0_BS_32};
static uint32_t s_nDesiredFrequency;
volatile static uint8_t s_cCommunicationState = COMMUNICATION_STATE_NONE;
/**
* @brief  Factor is: B/2 used in the formula for SYNTH word calculation
*/
static const uint8_t s_vectcBHalfFactor[4]={(HIGH_BAND_FACTOR/2), (MIDDLE_BAND_FACTOR/2), (LOW_BAND_FACTOR/2), (VERY_LOW_BAND_FACTOR/2)};

/**
* @}
*/

/** @defgroup Management_Private_FunctionsPrototypes            Management Private FunctionsPrototypes
* @{
*/
void SpiritManagementSetFrequencyBase(uint32_t lFBase);

/**
* @}
*/

/* --------------------------------   Radio  ----------------------------------- */ 

/** @defgroup Radio_Private_Macros                      Radio Private Macros
* @{
*/

#define XTAL_FLAG(xtalFrequency)               (xtalFrequency>=25e6) ? XTAL_FLAG_26_MHz:XTAL_FLAG_24_MHz

#define ROUND(A)                                  (((A-(uint32_t)A)> 0.5f)? (uint32_t)A+1:(uint32_t)A)
                                                  /* #1035-D */
/**
* @}
*/


/** @defgroup Radio_Private_Variables                   Radio Private Variables
* @{
*/

/**
* @brief  The Xtal frequency. To be set by the user (see SetXtalFreq() function)
*/
static uint32_t s_lXtalFrequency;

/**
* @brief  Factor is: B/2 used in the formula for SYNTH word calculation
*/
//static const uint8_t s_vectcBHalfFactor[4]={(HIGH_BAND_FACTOR/2), (MIDDLE_BAND_FACTOR/2), (LOW_BAND_FACTOR/2), (VERY_LOW_BAND_FACTOR/2)};

/**
* @brief  BS value to write in the SYNT0 register according to the selected band
*/
//static const uint8_t s_vectcBandRegValue[4]={SYNT0_BS_6, SYNT0_BS_12, SYNT0_BS_16, SYNT0_BS_32};


/**
* @brief  It represents the available channel bandwidth times 10 for 26 Mhz xtal.
* @note   The channel bandwidth for others xtal frequencies can be computed since this table
*         multiplying the current table by a factor xtal_frequency/26e6.
*/
static const uint16_t s_vectnBandwidth26M[90]=
{
  8001, 7951, 7684, 7368, 7051, 6709, 6423, 5867, 5414, \
    4509, 4259, 4032, 3808, 3621, 3417, 3254, 2945, 2703, \
      2247, 2124, 2011, 1900, 1807, 1706, 1624, 1471, 1350, \
        1123, 1062, 1005,  950,  903,  853,  812,  735,  675, \
          561,  530,  502,  474,  451,  426,  406,  367,  337, \
            280,  265,  251,  237,  226,  213,  203,  184,  169, \
              140,  133,  126,  119,  113,  106,  101,   92,   84, \
                70,   66,   63,   59,   56,   53,   51,   46,   42, \
                  35,   33,   31,   30,   28,   27,   25,   23,   21, \
                    18,   17,   16,   15,   14,   13,   13,   12,   11
};

/**
* @brief  It represents the available VCO frequencies
*/
static const uint16_t s_vectnVCOFreq[16]=
{
  4644, 4708, 4772, 4836, 4902, 4966, 5030, 5095, \
    5161, 5232, 5303, 5375, 5448, 5519, 5592, 5663
};

/**
* @brief  This variable is used to enable or disable
*  the VCO calibration WA called at the end of the SpiritRadioSetFrequencyBase fcn.
*  Default is enabled.
*/
static SpiritFunctionalState xDoVcoCalibrationWA=S_ENABLE;


/**
* @brief  These values are used to interpolate the power curves.
*         Interpolation curves are linear in the following 3 regions:
*       - reg value: 1 to 13    (up region)
*       - reg value: 13 to 40   (mid region)
*       - reg value: 41 to 90   (low region)
*       power_reg = m*power_dBm + q
*       For each band the order is: {m-up, q-up, m-mid, q-mid, m-low, q-low}.
* @note The power interpolation curves have been extracted
*       by measurements done on the divisional evaluation boards.
*/
static const float fPowerFactors[5][6]={ 
  {-2.11,25.66,-2.11,25.66,-2.00,31.28},   /* 915 */
  {-2.04,23.45,-2.04,23.45,-1.95,27.66},   /* 868 */
  {-3.48,38.45,-1.89,27.66,-1.92,30.23},   /* 433 */
  {-3.27,35.43,-1.80,26.31,-1.89,29.61},   /* 315 */
  {-4.18,50.66,-1.80,30.04,-1.86,32.22},   /* 169 */
};

/**
* @}
*/

/* --------------------------------   Common  ----------------------------------- */ 


/** @defgroup Common_Private_Types             Common Private Types
 * @{
 */

typedef SpiritStatus StatusBytes;

/**
 * @}
 */

/** @defgroup Common_Private_Variables             Common Private Variables
 * @{
 */

/**
 * @brief  Spirit Status global variable.
 *         This global variable of @ref SpiritStatus type is updated on every SPI transaction
 *         to maintain memory of Spirit Status.
 */

volatile SpiritStatus g_xStatus;

/**
 * @}
 */


/**
 * @defgroup Common_Private_Macros              Common Private Macros
 * @{
 */

#ifdef  SPIRIT_USE_FULL_ASSERT
 /**
   * @brief  The s_assert_param macro is used for function's parameters check.
   * @param  expr If expr is false, it calls assert_failed function which reports
   *         the name of the source file and the source line number of the call
   *         that failed. If expr is true, it returns no value.
   * @retval None
   */
  #define s_assert_param(expr) ((expr) ? (void)0 : s_assert_failed((uint8_t *)__FILE__, __LINE__))
  void s_assert_failed(uint8_t* file, uint32_t line);
#elif  SPIRIT_USE_VCOM_ASSERT
  /**
   * @brief  The s_assert_param macro is used for function's parameters check.
   * @param  expr  If expr is false, it calls assert_failed function which reports
   *         the name of the source file and the source line number of the call
   *         that failed. If expr is true, it returns no value.
   * @retval None
   */
  #define s_assert_param(expr) ((expr) ? (void)0 : s_assert_failed((uint8_t *)__FILE__, __LINE__,#expr))
  void s_assert_failed(uint8_t* file, uint32_t line, char* expression);

#elif SPIRIT_USE_FRAME_ASSERT
   /**
   * @brief  The s_assert_param macro is used for function's parameters check.
   * @param  expr  If expr is false, it calls assert_failed function which reports
   *         the name of the source file and the source line number of the call
   *         that failed. If expr is true, it returns no value.
   * @retval None
   */
#define s_assert_param(expr) ((expr) ? (void)0 : s_assert_failed(#expr))
  void s_assert_failed(uint8_t* expression);
#else
#define s_assert_param(expr)        {}
#endif

/**
 * @brief  Returns the absolute value.
 */
#define S_ABS(a) ((a)>0?(a):-(a))

/**
 * @}
 */

/** @defgroup Common_Private_FunctionsPrototypes             Common Private FunctionsPrototypes
 * @{
 */
StatusBytes SpiritSpiWriteRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer);
StatusBytes SpiritSpiReadRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer);
StatusBytes SpiritSpiCommandStrobes(uint8_t cCommandCode);
StatusBytes SpiritSpiWriteFifo(uint8_t cNbBytes, uint8_t* pcBuffer);
StatusBytes SpiritSpiReadFifo(uint8_t cNbBytes, uint8_t* pcBuffer);
/**
 * @}
 */

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++ Functions ++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* --------------------------------   AES  ----------------------------------- */ 

/**
 * @defgroup Aes_Private_Functions              AES Private Functions
 * @{
 */


/**
 * @brief  Enables or Disables the AES engine.
 * @param  xNewState new state for AES engine.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None
 */
void SpiritAesMode(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue = 0x00;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Modifies the register value */
  g_xStatus = SpiritSpiReadRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= AES_MASK;
  }
  else
  {
    tempRegValue &= ~AES_MASK;
  }

  /* Writes the ANA_FUNC_CONF0 register to enable or disable the AES engine */
  g_xStatus = SpiritSpiWriteRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);

}


/**
 * @brief  Writes the data to encrypt or decrypt, or the encryption key for the 
 *         derive decryption key operation into the AES_DATA_IN registers.
 * @param  pcBufferDataIn pointer to the user data buffer. The first byte of the array
 *        shall be the MSB byte and it will be put in the AES_DATA_IN[0] register, while
 *        the last one shall be the LSB and it will be put in the AES_DATA_IN[cDataLength-1]
 *        register. If data to write are less than 16 bytes the remaining AES_DATA_IN registers
 *        will be filled with bytes equal to 0. This parameter is an uint8_t*.
 * @param  cDataLength length of data in bytes.
 *         This parameter is an uint8_t.
 * @retval None
 */
void SpiritAesWriteDataIn(uint8_t* pcBufferDataIn, uint8_t cDataLength)
{
  uint8_t i, dataInArray[16];

  /* Verifies that there are no more than 16 bytes */
  (cDataLength>16) ? (cDataLength=16) : cDataLength;

  /* Fill the dataInArray with the data buffer, using padding */
  for(i=0;i<16;i++)
  {
    (i<(16 - cDataLength)) ? (dataInArray[i]=0):(dataInArray[i]=pcBufferDataIn[15-i]);

  }

  /* Writes the AES_DATA_IN registers */
  g_xStatus = SpiritSpiWriteRegisters(AES_DATA_IN_15_BASE, 16, dataInArray);
}


/**
 * @brief  Returns the encrypted or decrypted data or the decription key from the AES_DATA_OUT register.
 * @param  pcBufferDataOut pointer to the user data buffer. The AES_DATA_OUT[0]
 *         register value will be put as first element of the buffer (MSB), while the
 *         AES_DAT_OUT[cDataLength-1] register value will be put as last element of the buffer (LSB).
 *        This parameter is a uint8_t*.
 * @param  cDataLength length of data to read in bytes.
 *         This parameter is a uint8_t.
 * @retval None
 */
void SpiritAesReadDataOut(uint8_t* pcBufferDataOut, uint8_t cDataLength)
{
  uint8_t address, dataOutArray[16];

  /* Verifies that there are no more than 16 bytes */
  (cDataLength>16) ? (cDataLength=16) : cDataLength;

  /* Evaluates the address of AES_DATA_OUT from which start to read */
  address = AES_DATA_OUT_15_BASE+16-cDataLength;

  /* Reads the exact number of AES_DATA_OUT registers */
  g_xStatus = (SpiritSpiReadRegisters(address, cDataLength, dataOutArray));

  /* Copy in the user buffer the read values changing the order */
  for(int i = (cDataLength-1); i>=0; i--)
  {
    *pcBufferDataOut = dataOutArray[i];
    pcBufferDataOut++;
  }

}


/**
 * @brief  Writes the encryption key into the AES_KEY_IN register.
 * @param  pcKey pointer to the buffer of 4 words containing the AES key.
 *         The first byte of the buffer shall be the most significant byte AES_KEY_0 of the AES key.
 *         The last byte of the buffer shall be the less significant byte AES_KEY_15 of the AES key.
 *        This parameter is an uint8_t*.
 * @retval None
 */
void SpiritAesWriteKey(uint8_t* pcKey)
{
  uint8_t pcTempKey[16]; 
  for (uint8_t i = 0; i < 16; i++)
  {
    pcTempKey[15-i] = pcKey[i];
  }
  
  /* Writes the AES_DATA_IN registers */
  g_xStatus = SpiritSpiWriteRegisters(AES_KEY_IN_15_BASE, 16, pcTempKey);

}

/**
 * @brief  Returns the encryption/decryption key from the AES_KEY_IN register.
 * @param  pcKey  pointer to the buffer of 4 words (16 bytes) containing the AES key.
 *         The first byte of the buffer shall be the most significant byte AES_KEY_0 of the AES key.
 *         The last byte of the buffer shall be the less significant byte AES_KEY_15 of the AES key.
 *         This parameter is an uint8_t*.
 * @retval None
 */
void SpiritAesReadKey(uint8_t* pcKey)
{
  uint8_t pcTempKey[16];

  /* Reads the AES_DATA_IN registers */
  g_xStatus = SpiritSpiReadRegisters(AES_KEY_IN_15_BASE, 16, pcTempKey);


  for (uint8_t i = 0; i < 16; i++)
    pcKey[i] = pcTempKey[15-i];

}


/**
 * @brief  Derives the decryption key from a given encryption key.
 * @param  None.
 * @retval None.
 */
void SpiritAesDeriveDecKeyFromEnc(void)
{
  /* Sends the COMMAND_AES_KEY command */
  g_xStatus = SpiritSpiCommandStrobes(COMMAND_AES_KEY);

}


/**
 * @brief  Executes the encryption operation.
 * @param  None.
 * @retval None.
 */
void SpiritAesExecuteEncryption(void)
{
  /* Sends the COMMAND_AES_ENC command */
  g_xStatus = SpiritSpiCommandStrobes(COMMAND_AES_ENC);

}


/**
 * @brief  Executes the decryption operation.
 * @param  None.
 * @retval None.
 */
void SpiritAesExecuteDecryption(void)
{
  /* Sends the COMMAND_AES_DEC command */
  g_xStatus = SpiritSpiCommandStrobes(COMMAND_AES_DEC);

}


/**
 * @brief  Executes the key derivation and the decryption operation.
 * @param  None.
 * @retval None.
 */
void SpiritAesDeriveDecKeyExecuteDec(void)
{
  /* Sends the COMMAND_AES_KEY_DEC command */
  g_xStatus = SpiritSpiCommandStrobes(COMMAND_AES_KEY_DEC);

}

/**
 * @}
 */

/* --------------------------------   CALIBRATION  ----------------------------------- */ 

 /**
 * @defgroup Calibration_Private_Functions              Calibration Private Functions
 * @{
 */

/**
 * @brief  Enables or Disables the RCO calibration.
 * @param  xNewState new state for RCO calibration.
           This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritCalibrationRco(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the register value */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL2_BASE, 1, &tempRegValue);

  /* Build new value for the register */
  if(xNewState==S_ENABLE)
  {
    tempRegValue |= PROTOCOL2_RCO_CALIBRATION_MASK;
  }
  else
  {
    tempRegValue &= ~PROTOCOL2_RCO_CALIBRATION_MASK;
  }

  /* Writes register to enable or disable the RCO calibration */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL2_BASE, 1, &tempRegValue);

}


/**
 * @brief  Enables or Disables the VCO calibration.
 * @param  xNewState new state for VCO calibration.
           This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritCalibrationVco(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the register value */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL2_BASE, 1, &tempRegValue);

   /* Build new value for the register */
  if(xNewState==S_ENABLE)
    tempRegValue |= PROTOCOL2_VCO_CALIBRATION_MASK;
  else
    tempRegValue &= ~PROTOCOL2_VCO_CALIBRATION_MASK;

  /* Writes register to enable or disable the VCO calibration */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL2_BASE, 1, &tempRegValue);

}


/**
 * @brief  Sets the RCO calibration words.
 * @param  cRwt RWT word for RCO calibration.
 *         This parameter can be a value of uint8_t.
 * @param  cRfb RFB word for RCO calibration.
 *         This parameter can be a value of uint8_t.
 * @retval None.
 */
void SpiritCalibrationSetRcoCalWords(uint8_t cRwt, uint8_t cRfb)
{
  uint8_t tempRegValue[2];

  /* Build the value of RWT and the MSbits of the RFB word */
  tempRegValue[0] = (cRwt << 4) | (cRfb >> 1);

  /* Reads the register value to update the LSbit of RFB */
  g_xStatus = SpiritSpiReadRegisters(RCO_VCO_CALIBR_IN1_BASE, 1, &tempRegValue[1]);

  /* Build new value for the register */
  tempRegValue[1] = (tempRegValue[1] & 0x7F) | (cRfb<<7);

  /* Writes the new value for RCO calibration words */
  g_xStatus = SpiritSpiWriteRegisters(RCO_VCO_CALIBR_IN2_BASE, 2, tempRegValue);

}


/**
 * @brief  Returns the RCO calibration words.
 * @param  pcRwt pointer to the variable in which the RWT word has to be stored.
 *         This parameter is a variable of uint8_t*.
 * @param  pcRfb pointer to the variable in which the RFB word has to be stored.
 *         This parameter is a variable of uint8_t*.
 * @retval None.
 */
void SpiritCalibrationGetRcoCalWords(uint8_t* pcRwt, uint8_t* pcRfb)
{
  uint8_t tempRegValue[2];

  /* Reads the registers values */
  g_xStatus = SpiritSpiReadRegisters(RCO_VCO_CALIBR_OUT1_BASE, 2, tempRegValue);

  /* Build the RWT value */
  (*pcRwt) = tempRegValue[0] >> 4;
  /* Build the RFB value */
  (*pcRfb) = (tempRegValue[0] & 0x0F)<<1 | (tempRegValue[1]>>7);

}


/**
 * @brief  Returns the VCO calibration data from internal VCO calibrator.
 * @param  None.
 * @retval uint8_t VCO calibration data word.
 */
uint8_t SpiritCalibrationGetVcoCalData(void)
{
  uint8_t tempRegValue;

  /* Reads the register value */
  g_xStatus = SpiritSpiReadRegisters(RCO_VCO_CALIBR_OUT0_BASE, 1, &tempRegValue);

  /* Build and returns the VCO calibration data value */
  return (tempRegValue & 0x7F);

}


/**
 * @brief  Sets the VCO calibration data to be used in TX mode.
 * @param  cVcoCalData calibration data word to be set.
 *         This parameter is a variable of uint8_t.
 * @retval None.
 */
void SpiritCalibrationSetVcoCalDataTx(uint8_t cVcoCalData)
{
  uint8_t tempRegValue;

  /* Reads the register value */
  g_xStatus = SpiritSpiReadRegisters(RCO_VCO_CALIBR_IN1_BASE, 1, &tempRegValue);

  /* Build the value to be written */
  tempRegValue &= 0x80;
  tempRegValue |= cVcoCalData;

  /* Writes the new value of calibration data in TX */
  g_xStatus = SpiritSpiWriteRegisters(RCO_VCO_CALIBR_IN1_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the actual VCO calibration data used in TX mode.
 * @param  None.
 * @retval uint8_t Calibration data word used in TX mode.
 */
uint8_t SpiritCalibrationGetVcoCalDataTx(void)
{
  uint8_t tempRegValue;

  /* Reads the register containing the calibration data word used in TX mode */
  g_xStatus = SpiritSpiReadRegisters(RCO_VCO_CALIBR_IN1_BASE, 1, &tempRegValue);

  /* Mask the VCO_CALIBR_TX field and returns the value */
  return (tempRegValue & 0x7F);

}


/**
 * @brief  Sets the VCO calibration data to be used in RX mode.
 * @param  cVcoCalData calibration data word to be set.
 *         This parameter is a variable of uint8_t.
 * @retval None.
 */
void SpiritCalibrationSetVcoCalDataRx(uint8_t cVcoCalData)
{
  uint8_t tempRegValue;

  /* Reads the register value */
  g_xStatus = SpiritSpiReadRegisters(RCO_VCO_CALIBR_IN0_BASE, 1, &tempRegValue);

  /* Build the value to be written */
  tempRegValue &= 0x80;
  tempRegValue |= cVcoCalData;

  /* Writes the new value of calibration data in RX */
  g_xStatus = SpiritSpiWriteRegisters(RCO_VCO_CALIBR_IN0_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the actual VCO calibration data used in RX mode.
 * @param  None.
 * @retval uint8_t Calibration data word used in RX mode.
 */
uint8_t SpiritCalibrationGetVcoCalDataRx(void)
{
  uint8_t tempRegValue;

  /* Reads the register containing the calibration data word used in TX mode */
  g_xStatus = SpiritSpiReadRegisters(RCO_VCO_CALIBR_IN0_BASE, 1, &tempRegValue);

  /* Mask the VCO_CALIBR_RX field and returns the value */
  return (tempRegValue & 0x7F);

}


/**
 * @brief  Selects a VCO.
 * @param  xVco can be VCO_H or VCO_L according to which VCO select.
 *         This parameter can be a value of @ref VcoSel.
 * @retval None.
 */
void SpiritCalibrationSelectVco(VcoSel xVco)
{
  uint8_t tempRegValue;
  
  /* Check the parameter */
  s_assert_param(IS_VCO_SEL(xVco));
  
  SpiritSpiReadRegisters(SYNTH_CONFIG1_BASE, 1, &tempRegValue);
  
  tempRegValue &= 0xF9;
  
  if(xVco == VCO_H)
  {
    tempRegValue |= 0x02;
    
  }
  else
  {
    tempRegValue |= 0x04;
  }
  SpiritSpiWriteRegisters(SYNTH_CONFIG1_BASE, 1, &tempRegValue);  
  
}


/**
 * @brief  Returns the VCO selected.
 * @param  void.
 * @retval VCO_H or VCO_L according to which VCO selected.
 *         This parameter can be a value of @ref VcoSel.
 */
VcoSel SpiritCalibrationGetVcoSelecttion(void)
{
  uint8_t tempRegValue;
  
  SpiritSpiReadRegisters(SYNTH_CONFIG1_BASE, 1, &tempRegValue);
  
  tempRegValue = (tempRegValue>>1)&0x3;
  
  if(tempRegValue == 0x01)
  {
    return VCO_H;
    
  }
  else
  {
    return VCO_L;
  }
  
}

/**
 *@}
 */
 
/* --------------------------------   COMMANDS  ----------------------------------- */ 
 
 /**
 * @defgroup Commands_Private_Functions                 Commands Private Functions
 * @{
 */

/**
 * @brief  Sends a specific command to SPIRIT.
 * @param  xCommandCode code of the command to send.
           This parameter can be any value of @ref SpiritCmd.
 * @retval None.
 */
void SpiritCmdStrobeCommand(SpiritCmd xCommandCode)
{
  /* Check the parameters */
  s_assert_param(IS_SPIRIT_CMD(xCommandCode));

  g_xStatus = SpiritSpiCommandStrobes((uint8_t) xCommandCode);
}

/**
 *@}
 */
 
/* --------------------------------   CSMA  ----------------------------------- */ 

/**
 * @defgroup Csma_Private_Functions             CSMA Private Functions
 * @{
 */


/**
 * @brief  Initializes the SPIRIT CSMA according to the specified parameters in the CsmaInit.
 * @param  pxCsmaInit Csma init structure.
 *         This parameter is a pointer to @ref CsmaInit.
 * @retval None.
 */
void SpiritCsmaInit(CsmaInit* pxCsmaInit)
{
  uint8_t tempRegValue[5];

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(pxCsmaInit->xCsmaPersistentMode));
  s_assert_param(IS_CCA_PERIOD(pxCsmaInit->xMultiplierTbit));
  s_assert_param(IS_CSMA_LENGTH(pxCsmaInit->xCcaLength));
  s_assert_param(IS_BU_COUNTER_SEED(pxCsmaInit->nBuCounterSeed));
  s_assert_param(IS_BU_PRESCALER(pxCsmaInit->cBuPrescaler));
  s_assert_param(IS_CMAX_NB(pxCsmaInit->cMaxNb));

  /* CSMA BU counter seed (MSB) config */
  tempRegValue[0] = (uint8_t)(pxCsmaInit->nBuCounterSeed >> 8);

  /* CSMA BU counter seed (LSB) config */
  tempRegValue[1] = (uint8_t) pxCsmaInit->nBuCounterSeed;

  /* CSMA BU prescaler config and CCA period config */
  tempRegValue[2] = (pxCsmaInit->cBuPrescaler << 2) | pxCsmaInit->xMultiplierTbit;

  /* CSMA CCA length config and max number of back-off */
  tempRegValue[3] = (pxCsmaInit->xCcaLength | pxCsmaInit->cMaxNb);

  /* Reads the PROTOCOL1_BASE register value, to write the SEED_RELOAD and CSMA_PERS_ON fields */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 1, &tempRegValue[4]);

  /* Writes the new value for persistent mode */
  if(pxCsmaInit->xCsmaPersistentMode==S_ENABLE)
  {
    tempRegValue[4] |= PROTOCOL1_CSMA_PERS_ON_MASK;
  }
  else
  {
    tempRegValue[4] &= ~PROTOCOL1_CSMA_PERS_ON_MASK;
  }

  /* Writes PROTOCOL1_BASE register */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL1_BASE, 1, &tempRegValue[4]);

  /* Writes CSMA_CONFIGx_BASE registers */
  g_xStatus = SpiritSpiWriteRegisters(CSMA_CONFIG3_BASE, 4, tempRegValue);

}


 /**
  * @brief  Returns the fitted structure CsmaInit starting from the registers values.
  * @param  pxCsmaInit Csma structure to be fitted.
  *         This parameter is a pointer to @ref CsmaInit.
  * @retval None.
  */
void SpiritCsmaGetInfo(CsmaInit* pxCsmaInit)
{
   uint8_t tempRegValue[5];

   /* Reads PROTOCOL1_BASE register */
   g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 1, &tempRegValue[4]);

   /* Reads CSMA_CONFIGx_BASE registers */
   g_xStatus = SpiritSpiReadRegisters(CSMA_CONFIG3_BASE, 4, tempRegValue);

   /* Reads the bu counter seed */
   pxCsmaInit->nBuCounterSeed = (uint16_t)tempRegValue[1] | ((uint16_t)(tempRegValue[0] << 8));

   /* Reads the bu prescaler */
   pxCsmaInit->cBuPrescaler = tempRegValue[2]>>2;

   /* Reads the Cca period */
   pxCsmaInit->xMultiplierTbit = (CcaPeriod)(tempRegValue[2] & 0x03);

   /* Reads the Cca length */
   pxCsmaInit->xCcaLength = (CsmaLength)(tempRegValue[3]&0xF0);

   /* Reads the max number of back off */
   pxCsmaInit->cMaxNb = tempRegValue[3] & 0x07;

   /* Reads the persistent mode enable bit */
   pxCsmaInit->xCsmaPersistentMode = (SpiritFunctionalState)((tempRegValue[4]>>1) & 0x01);

}


/**
 * @brief  Enables or Disables the CSMA.
 * @param  xNewState the state of the CSMA mode.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritCsma(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the PROTOCOL1 register value */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 1, &tempRegValue);

  /* Sets or resets the CSMA enable bit */
  if(xNewState==S_ENABLE)
  {
    tempRegValue |= PROTOCOL1_CSMA_ON_MASK;
  }
  else
  {
    tempRegValue &= ~PROTOCOL1_CSMA_ON_MASK;
  }

  /* Writes the new value on the PROTOCOL1 register */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL1_BASE, 1, &tempRegValue);

}

/**
 * @brief  Gets the CSMA mode. Says if it is enabled or disabled.
 * @param  None.
 * @retval SpiritFunctionalState: CSMA mode.
 */
SpiritFunctionalState SpiritCsmaGetCsma(void)
{
  uint8_t tempRegValue;

  /* Reads the PROTOCOL1 register value */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 1, &tempRegValue);

  /* Return if set or reset */
  if(tempRegValue & PROTOCOL1_CSMA_ON_MASK)
  {
    return S_ENABLE;
  }
  else
  {
    return S_DISABLE;
  }

}

/**
 * @brief  Enables or Disables the persistent CSMA mode.
 * @param  xNewState the state of the persistent CSMA mode.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritCsmaPersistentMode(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the PROTOCOL1 register value */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 1, &tempRegValue);

  /* Enables/disables the CSMA persistent mode */
  if(xNewState==S_ENABLE)
  {
    tempRegValue |= PROTOCOL1_CSMA_PERS_ON_MASK;
  }
  else
  {
    tempRegValue &= ~PROTOCOL1_CSMA_PERS_ON_MASK;
  }

  /* Writes the new vaue on the PROTOCOL1 register */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL1_BASE, 1, &tempRegValue);

}


/**
 * @brief  Gets the persistent CSMA mode.
 * @param  None.
 * @retval SpiritFunctionalState: CSMA persistent mode.
 */
SpiritFunctionalState SpiritCsmaGetPersistentMode(void)
{
  uint8_t tempRegValue;

  /* Reads the PROTOCOL1 register value */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 1, &tempRegValue);

  /* Return if set or reset */
  if(tempRegValue & PROTOCOL1_CSMA_PERS_ON_MASK)
  {
    return S_ENABLE;
  }
  else
  {
    return S_DISABLE;
  }

}


/**
 * @brief  Enables or Disables the seed reload mode (if enabled it reloads the back-off generator seed using the value written in the BU_COUNTER_SEED register).
 * @param  xNewState the state of the seed reload mode.
 *       This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritCsmaSeedReloadMode(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the PROTOCOL1 register value */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 1, &tempRegValue);

  /* Enables/disables the seed reload mode */
  if(xNewState==S_ENABLE)
  {
    tempRegValue |= PROTOCOL1_SEED_RELOAD_MASK;
  }
  else
  {
    tempRegValue &= ~PROTOCOL1_SEED_RELOAD_MASK;
  }

  /* Writes the new value on the PROTOCOL1 register */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL1_BASE, 1, &tempRegValue);

}


/**
 * @brief  Gets the seed reload mode.
 * @param  None.
 * @retval SpiritFunctionalState: CSMA seed reload mode.
 */
SpiritFunctionalState SpiritCsmaGetSeedReloadMode(void)
{
  uint8_t tempRegValue;

  /* Reads the PROTOCOL1 register value */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 1, &tempRegValue);

  /* Return if set or reset */
  if(tempRegValue & PROTOCOL1_SEED_RELOAD_MASK)
  {
    return S_ENABLE;
  }
  else
  {
    return S_DISABLE;
  }
}


/**
 * @brief  Sets the BU counter seed (BU_COUNTER_SEED register). The CSMA back off time is given by the formula: BO = rand(2^NB)*BU.
 * @param  nBuCounterSeed seed of the random number generator used to apply the BBE algorithm.
 *       This parameter is an uint16_t.
 * @retval None.
 */
void SpiritCsmaSetBuCounterSeed(uint16_t nBuCounterSeed)
{
  uint8_t tempRegValue[2];

  /* Check parameters */
  s_assert_param(IS_BU_COUNTER_SEED(nBuCounterSeed));

  /* Build value (MSB)*/
  tempRegValue[0]=(uint8_t)(nBuCounterSeed>>8);
  /* Build value (LSB) */
  tempRegValue[1]=(uint8_t)nBuCounterSeed;

  /* Writes the CSMA_CONFIG3 registers */
  g_xStatus = SpiritSpiWriteRegisters(CSMA_CONFIG3_BASE, 2, tempRegValue);

}

/**
 * @brief  Returns the BU counter seed (BU_COUNTER_SEED register).
 * @param  None.
 * @retval uint16_t Seed of the random number generator used to apply the BBE algorithm.
 */
uint16_t SpiritCsmaGetBuCounterSeed(void)
{
  uint8_t tempRegValue[2];

  /* Reads the CSMA_CONFIGx registers value */
  g_xStatus = SpiritSpiReadRegisters(CSMA_CONFIG3_BASE, 2, tempRegValue);

  /* Build the counter seed and return it */
  return ((uint16_t)tempRegValue[1] + (((uint16_t)tempRegValue[0])<<8));

}


/**
 * @brief  Sets the BU prescaler. The CSMA back off time is given by the formula: BO = rand(2^NB)*BU.
 * @param  cBuPrescaler used to program the back-off unit BU.
 *        This parameter is an uint8_t.
 * @retval None.
 */
void SpiritCsmaSetBuPrescaler(uint8_t cBuPrescaler)
{
  uint8_t tempRegValue;

  /* Check parameters */
  s_assert_param(IS_BU_PRESCALER(cBuPrescaler));

  /* Reads the CSMA_CONFIG1 register value */
  g_xStatus = SpiritSpiReadRegisters(CSMA_CONFIG1_BASE, 1, &tempRegValue);

  /* Build the new value for the BU prescaler */
  tempRegValue &= 0x03;
  tempRegValue |= (cBuPrescaler<<2);

  /* Writes the new value on the CSMA_CONFIG1_BASE register */
  g_xStatus = SpiritSpiWriteRegisters(CSMA_CONFIG1_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the BU prescaler.
 * @param  None.
 * @retval uint8_t Value back-off unit (BU).
 */
uint8_t SpiritCsmaGetBuPrescaler(void)
{
  uint8_t tempRegValue;

  /* Reads the CSMA_CONFIG1 register value */
  g_xStatus = SpiritSpiReadRegisters(CSMA_CONFIG1_BASE, 1, &tempRegValue);

  /* Build and return the BU prescaler value */
  return (tempRegValue >> 2);

}


/**
 * @brief  Sets the CCA period.
 * @param  xMultiplierTbit value of CCA period to store.
 *        This parameter can be a value of @ref CcaPeriod.
 * @retval None.
 */
void SpiritCsmaSetCcaPeriod(CcaPeriod xMultiplierTbit)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_CCA_PERIOD(xMultiplierTbit));

  /* Reads the CSMA_CONFIG1 register value */
  g_xStatus = SpiritSpiReadRegisters(CSMA_CONFIG1_BASE, 1, &tempRegValue);

  /* Build the new value setting the the CCA period */
  tempRegValue &= 0xFC;
  tempRegValue |= xMultiplierTbit;

  /* Writes the new value on the CSMA_CONFIG1 register */
  g_xStatus = SpiritSpiWriteRegisters(CSMA_CONFIG1_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the CCA period.
 * @param  None.
 * @retval CcaPeriod CCA period.
 */
CcaPeriod SpiritCsmaGetCcaPeriod(void)
{
  uint8_t tempRegValue;

  /* Reads the CSMA_CONFIG1 register value */
  g_xStatus = SpiritSpiReadRegisters(CSMA_CONFIG1_BASE, 1, &tempRegValue);

  /* Build and return the CCA period value */
  return (CcaPeriod)(tempRegValue & 0x03);

}


/**
 * @brief  Sets the CCA length.
 * @param  xCcaLength the CCA length (a value between 1 and 15 that multiplies the CCA period).
 *       This parameter can be any value of @ref CsmaLength.
 * @retval None.
 */
void SpiritCsmaSetCcaLength(CsmaLength xCcaLength)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_CSMA_LENGTH(xCcaLength));

  /* Reads the CSMA_CONFIG0 register value */
  g_xStatus = SpiritSpiReadRegisters(CSMA_CONFIG0_BASE, 1, &tempRegValue);

  /* Build the value of CCA length to be set */
  tempRegValue &= 0x0F;
  tempRegValue |= xCcaLength;

  /* Writes the new value on the CSMA_CONFIG0 register */
  g_xStatus = SpiritSpiWriteRegisters(CSMA_CONFIG0_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the CCA length.
 * @param  None.
 * @retval uint8_t CCA length.
 */
uint8_t SpiritCsmaGetCcaLength(void)
{
  uint8_t tempRegValue;

  /* Reads the CSMA_CONFIG0 register value */
  g_xStatus = SpiritSpiReadRegisters(CSMA_CONFIG0_BASE, 1, &tempRegValue);

  /* Build and return the CCA length */
  return tempRegValue >> 4;

}


/**
 * @brief  Sets the max number of back-off. If reached Spirit stops the transmission.
 * @param  cMaxNb the max number of back-off.
 *         This parameter is an uint8_t.
 * @retval None.
 */
void SpiritCsmaSetMaxNumberBackoff(uint8_t cMaxNb)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_CMAX_NB(cMaxNb));

  /* Reads the CSMA_CONFIG0 register value */
  g_xStatus = SpiritSpiReadRegisters(CSMA_CONFIG0_BASE, 1, &tempRegValue);

  /* Build the value of max back off to be set */
  tempRegValue &= 0xF8;
  tempRegValue |= cMaxNb;

  /* Writes the new value on the CSMA_CONFIG0 register */
  g_xStatus = SpiritSpiWriteRegisters(CSMA_CONFIG0_BASE, 1, &tempRegValue);
}

/**
 * @brief  Returns the max number of back-off.
 * @param  None.
 * @retval uint8_t Max number of back-off.
 */
uint8_t SpiritCsmaGetMaxNumberBackoff(void)
{
  uint8_t tempRegValue;

  /* Reads the CSMA_CONFIG0 register value */
  g_xStatus = SpiritSpiReadRegisters(CSMA_CONFIG0_BASE, 1, &tempRegValue);

  /* Build and return the max number of back-off */
  return (tempRegValue & 0x07);

}

/**
 *@}
 */
 
/* --------------------------------   DIRECT_RF  ----------------------------------- */ 

 /**
 * @defgroup DirectRf_Private_Functions                 Direct RF Private Functions
 * @{
 */

/**
 * @brief  Sets the DirectRF RX mode of SPIRIT.
 * @param  xDirectRx code of the desired mode.
 *         This parameter can be any value of @ref DirectRx.
 * @retval None.
 */
void SpiritDirectRfSetRxMode(DirectRx xDirectRx)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_DIRECT_RX(xDirectRx));

  /* Reads the register value */
  SpiritSpiReadRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);

  /* Build the value to be stored */
  tempRegValue &= ~PCKTCTRL3_RX_MODE_MASK;
  tempRegValue |= (uint8_t)xDirectRx;

  /* Writes value on register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the DirectRF RX mode of SPIRIT.
 * @param  None.
 * @retval DirectRx Direct Rx mode.
 */
DirectRx SpiritDirectRfGetRxMode(void)
{
  uint8_t tempRegValue;

  /* Reads the register value and mask the RX_Mode field */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);

  /* Rebuild and return value */
  return (DirectRx)(tempRegValue & 0x30);

}


/**
 * @brief  Sets the TX mode of SPIRIT.
 * @param  xDirectTx code of the desired source.
 *         This parameter can be any value of @ref DirectTx.
 * @retval None.
 */
void SpiritDirectRfSetTxMode(DirectTx xDirectTx)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_DIRECT_TX(xDirectTx));

  /* Reads the register value */
  SpiritSpiReadRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);

  /* Build the value to be stored */
  tempRegValue &= ~PCKTCTRL1_TX_SOURCE_MASK;
  tempRegValue |= (uint8_t)xDirectTx;

  /* Writes value on register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the DirectRF TX mode of SPIRIT.
 * @param  None.
 * @retval DirectTx Direct Tx mode.
 */
DirectTx SpiritDirectRfGetTxMode(void)
{
  uint8_t tempRegValue;

  /* Reads the register value and mask the RX_Mode field */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);

  /* Returns value */
  return (DirectTx)(tempRegValue & 0x0C);

}

/**
 *@}
 */

/* --------------------------------   GENERAL  ----------------------------------- */ 

 /**
 * @defgroup General_Private_Functions                          General Private Functions
 * @{
 */

/**
 * @brief  Enables or Disables the output of battery level detector.
 * @param  xNewState new state for battery level detector.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None
 */
void SpiritGeneralBatteryLevel(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the ANA_FUNC_CONF0_BASE register value */
  g_xStatus = SpiritSpiReadRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);

  /* Build the value to be stored */
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= BATTERY_LEVEL_MASK;
  }
  else
  {
    tempRegValue &= ~BATTERY_LEVEL_MASK;
  }

  /* Writes the new value */
  g_xStatus = SpiritSpiWriteRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);

}


/**
 * @brief  Sets the battery level.
 * @param  xBatteryLevel new state for battery level.
 *         This parameter can be a value of @ref BatteryLevel.
 * @retval None.
 */
void SpiritGeneralSetBatteryLevel(BatteryLevel xBatteryLevel)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_BLD_LVL(xBatteryLevel));

  /* Reads the ANA_FUNC_CONF1_BASE register value */
  g_xStatus = SpiritSpiReadRegisters(ANA_FUNC_CONF1_BASE, 1, &tempRegValue);

  /* Build the value to be stored */
  tempRegValue &= ~ANA_FUNC_CONF1_SET_BLD_LVL_MASK;
  switch(xBatteryLevel)
  {
    case BLD_LVL_2_7_V:
      tempRegValue |= BLD_LVL_2_7;
      break;
    case BLD_LVL_2_5_V:
      tempRegValue |= BLD_LVL_2_5;
      break;
    case BLD_LVL_2_3_V:
      tempRegValue |= BLD_LVL_2_3;
      break;
    case BLD_LVL_2_1_V:
      tempRegValue |= BLD_LVL_2_1;
      break;
  }

  /* Writes the new value */
  g_xStatus = SpiritSpiWriteRegisters(ANA_FUNC_CONF1_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the settled battery level.
 * @param  None.
 * @retval BatteryLevel Settled battery level. This parameter can be a value of @ref BatteryLevel.
 */
BatteryLevel SpiritGeneralGetBatteryLevel(void)
{
  uint8_t tempRegValue;

  /* Reads the ANA_FUNC_CONF1_BASE register value */
  g_xStatus = SpiritSpiReadRegisters(ANA_FUNC_CONF1_BASE, 1, &tempRegValue);

  /* Mask the battery level field and returns the settled battery level */
  return ((BatteryLevel)(tempRegValue & ANA_FUNC_CONF1_SET_BLD_LVL_MASK));

}


/**
 * @brief  Enables or Disables the output of brown out detector.
 * @param  xNewState new state for brown out detector.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritGeneralBrownOut(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the ANA_FUNC_CONF0_BASE register value */
  g_xStatus = SpiritSpiReadRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);

  /* Build the value to be stored */
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= BROWN_OUT_MASK;
  }
  else
  {
    tempRegValue &= ~BROWN_OUT_MASK;
  }

  /* Writes value on register */
  g_xStatus = SpiritSpiWriteRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);

}


/**
 * @brief  Sets High Power Mode.
 * @param  xNewState new state for High Power Mode.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritGeneralHighPwr(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the ANA_FUNC_CONF0_BASE register value */
  g_xStatus = SpiritSpiReadRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);

  /* Build the value to write */
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= HIGH_POWER_MODE_MASK;
  }
  else
  {
    tempRegValue &= ~HIGH_POWER_MODE_MASK;
  }

  /* Writes the new value on register */
  g_xStatus = SpiritSpiWriteRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);

}


/**
 * @brief  Sets External Reference.
 * @param  xExtMode new state for the external reference.
 *         This parameter can be: MODE_EXT_XO or MODE_EXT_XIN.
 * @retval None.
 */
void SpiritGeneralSetExtRef(ModeExtRef xExtMode)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_MODE_EXT(xExtMode));

  /* Reads the ANA_FUNC_CONF0_BASE register value */
  SpiritSpiReadRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);

  /* Build the value to write */
  if(xExtMode == MODE_EXT_XO)
  {
    tempRegValue &= ~EXT_REF_MASK;
  }
  else
  {
    tempRegValue |= EXT_REF_MASK;
  }

  /* Writes value on register */
  g_xStatus = SpiritSpiWriteRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns External Reference.
 * @param  None.
 * @retval ModeExtRef Settled external reference.
 *         This parameter can be: MODE_EXT_XO or MODE_EXT_XIN.
 */
ModeExtRef SpiritGeneralGetExtRef(void)
{
  uint8_t tempRegValue;

  /* Reads the ANA_FUNC_CONF0_BASE register value and return the result */
  g_xStatus = SpiritSpiReadRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);

  /* Mask the EXT_REF field field and returns the settled reference signal */
  return ((ModeExtRef)((tempRegValue & 0x10)>>4));

}


/**
 * @brief  Sets XO gm at startup.
 * @param  xGm transconductance value of XO at startup.
 *         This parameter can be a value of @ref GmConf.
 * @retval None.
 */
void SpiritGeneralSetXoGm(GmConf xGm)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_GM_CONF(xGm));

  /* Reads the ANA_FUNC_CONF1_BASE register value */
  g_xStatus = SpiritSpiReadRegisters(ANA_FUNC_CONF1_BASE, 1, &tempRegValue);

  /* Build the value to write */
  tempRegValue &= ~ANA_FUNC_CONF1_GMCONF_MASK;
  switch(xGm)
  {
    case GM_SU_13_2:
      tempRegValue |= GM_13_2;
      break;
    case GM_SU_18_2:
      tempRegValue |= GM_18_2;
      break;
    case GM_SU_21_5:
      tempRegValue |= GM_21_5;
      break;
    case GM_SU_25_6:
      tempRegValue |= GM_25_6;
      break;
    case GM_SU_28_8:
      tempRegValue |= GM_28_8;
      break;
    case GM_SU_33_9:
      tempRegValue |= GM_33_9;
      break;
    case GM_SU_38_5:
      tempRegValue |= GM_38_5;
      break;
    case GM_SU_43_0:
      tempRegValue |= GM_43_0;
      break;
  }

  /* Writes new value on register */
  g_xStatus = SpiritSpiWriteRegisters(ANA_FUNC_CONF1_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the configured XO gm at startup.
 * @param  None.
 * @retval GmConf Settled XO gm. This parameter can be a value of @ref GmConf.
 */
GmConf SpiritGeneralGetXoGm(void)
{
  uint8_t tempRegValue;

  /* Reads the ANA_FUNC_CONF1_BASE register value */
  g_xStatus = SpiritSpiReadRegisters(ANA_FUNC_CONF1_BASE, 1, &tempRegValue);

  /* Mask the GM_CONF field field and returns the settled transconductance of the XO at startup */
  return ((GmConf)((tempRegValue & 0x1C)>>2));

}


/**
 * @brief  Returns the settled packet format.
 * @param  None.
 * @retval PacketType Settled packet type. This parameter can be a value of @ref PacketType.
 */
PacketType SpiritGeneralGetPktType(void)
{
  uint8_t tempRegValue;

  /* Reads the PROTOCOL1 register */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);

  /* cast and return value */
  return (PacketType)(tempRegValue>>6);

}


/**
 * @brief  Returns device part number.
 * @param  None.
 * @retval uint16_t Device part number.
 */
uint16_t SpiritGeneralGetDevicePartNumber(void)
{
  uint8_t tempRegValue[2];

  /* Reads the register value containing the device part number */
  g_xStatus = SpiritSpiReadRegisters(DEVICE_INFO1_PARTNUM, 2, tempRegValue);

  return ((((uint16_t)tempRegValue[0])<<8) | ((uint16_t)tempRegValue[1]));

}

/**
 * @brief  Returns SPIRIT RF board version.
 * @param  None.
 * @retval SPIRIT RF board version: 0x30 is the only admitted value
 */
uint8_t SpiritGeneralGetSpiritVersion(void)
{
  uint8_t ver; 
  SpiritSpiReadRegisters(DEVICE_INFO0_VERSION, 1, &ver);
  return ver;
}

/**
 *@}
 */

/* --------------------------------   GPIO  ----------------------------------- */ 

 /** @defgroup Gpio_Private_Functions            GPIO Private Functions
 * @{
 */

/**
 * @brief  Initializes the SPIRIT GPIOx according to the specified
 *         parameters in the pxGpioInitStruct.
 * @param  pxGpioInitStruct pointer to a SGpioInit structure that
 *         contains the configuration information for the specified SPIRIT GPIO.
 * @retval None.
 */
void SpiritGpioInit(SGpioInit* pxGpioInitStruct)
{
  uint8_t tempRegValue = 0x00;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_GPIO(pxGpioInitStruct->xSpiritGpioPin));
  s_assert_param(IS_SPIRIT_GPIO_MODE(pxGpioInitStruct->xSpiritGpioMode));
  s_assert_param(IS_SPIRIT_GPIO_IO(pxGpioInitStruct->xSpiritGpioIO));

  tempRegValue = ((uint8_t)(pxGpioInitStruct->xSpiritGpioMode) | (uint8_t)(pxGpioInitStruct->xSpiritGpioIO));

  g_xStatus = SpiritSpiWriteRegisters(pxGpioInitStruct->xSpiritGpioPin, 1, &tempRegValue);

}


/**
 * @brief  Enables or Disables the output of temperature sensor on SPIRIT GPIO_0.
 * @param  xNewState new state for temperature sensor.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritGpioTemperatureSensor(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue = 0x00;
  uint8_t gpio0tempRegValue = 0x00;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the ANA_FUNC_CONF0 register and mask the result to enable or disable the
     temperature sensor */
  g_xStatus = SpiritSpiReadRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= TEMPERATURE_SENSOR_MASK;
  }
  else
  {
    tempRegValue &= (~TEMPERATURE_SENSOR_MASK);
    gpio0tempRegValue = 0x0A; /* Default value */
  }
  g_xStatus = SpiritSpiWriteRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);

  /* Sets the SPIRIT GPIO_0 according to input request */
  g_xStatus = SpiritSpiWriteRegisters(GPIO0_CONF_BASE, 1, &gpio0tempRegValue);

}


/**
 * @brief  Forces SPIRIT GPIO_x configured as digital output, to VDD or GND.
 * @param  xGpioX Specifies the GPIO to be configured.
 *   This parameter can be one of following parameters:
 *     @arg SPIRIT_GPIO_0: SPIRIT GPIO_0
 *     @arg SPIRIT_GPIO_1: SPIRIT GPIO_1
 *     @arg SPIRIT_GPIO_2: SPIRIT GPIO_2
 *     @arg SPIRIT_GPIO_3: SPIRIT GPIO_3
 * @param  xLevel Specifies the level.
 *   This parameter can be: HIGH or LOW.
 * @retval None.
 */
void SpiritGpioSetLevel(SpiritGpioPin xGpioX, OutputLevel xLevel)
{
  uint8_t tempRegValue = 0x00;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_GPIO(xGpioX));
  s_assert_param(IS_SPIRIT_GPIO_LEVEL(xLevel));

  /* Reads the SPIRIT_GPIOx register and mask the GPIO_SELECT field */
  g_xStatus = SpiritSpiReadRegisters(xGpioX, 1, &tempRegValue);
  tempRegValue &= 0x04;

  /* Sets the value of the SPIRIT GPIO register according to the specified level */
  if(xLevel == HIGH)
  {
    tempRegValue |= (uint8_t)SPIRIT_GPIO_DIG_OUT_VDD | (uint8_t)SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP;
  }
  else
  {
    tempRegValue |= (uint8_t)SPIRIT_GPIO_DIG_OUT_GND | (uint8_t)SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP;
  }

  /* Writes the SPIRIT GPIO register */
  g_xStatus = SpiritSpiWriteRegisters(xGpioX, 1, &tempRegValue);

}


/**
 * @brief  Returns output value (VDD or GND) of SPIRIT GPIO_x, when it is configured as digital output.
 * @param  xGpioX Specifies the GPIO to be read.
 *         This parameter can be one of following parameters:
 *         @arg SPIRIT_GPIO_0: SPIRIT GPIO_0
 *         @arg SPIRIT_GPIO_1: SPIRIT GPIO_1
 *         @arg SPIRIT_GPIO_2: SPIRIT GPIO_2
 *         @arg SPIRIT_GPIO_3: SPIRIT GPIO_3
 * @retval OutputLevel Logical level of selected GPIO configured as digital output.
 *         This parameter can be: HIGH or LOW.
 */
OutputLevel SpiritGpioGetLevel(SpiritGpioPin xGpioX)
{
  uint8_t tempRegValue = 0x00;
  OutputLevel level;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_GPIO(xGpioX));

  /* Reads the SPIRIT_GPIOx register */
  g_xStatus = SpiritSpiReadRegisters(xGpioX, 1, &tempRegValue);

  /* Mask the GPIO_SELECT field and returns the value according */
  tempRegValue &= 0xF8;
  if(tempRegValue == SPIRIT_GPIO_DIG_OUT_VDD)
  {
    level = HIGH;
  }
  else
  {
    level = LOW;
  }

  return level;

}


/**
 * @brief  Enables or Disables the MCU clock output.
 * @param  xNewState new state for the MCU clock output.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritGpioClockOutput(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the MCU_CK_CONF register and mask the result to enable or disable the clock output */
  g_xStatus = SpiritSpiReadRegisters(MCU_CK_CONF_BASE, 1, &tempRegValue);

  if(xNewState)
  {
    tempRegValue |= MCU_CK_ENABLE;
  }
  else
  {
    tempRegValue &= (~MCU_CK_ENABLE);
  }

  /* Writes the MCU_CK_CONF register */
  g_xStatus = SpiritSpiWriteRegisters(MCU_CK_CONF_BASE, 1, &tempRegValue);

}


/**
 * @brief  Initializes the SPIRIT Clock Output according to the specified
 *         parameters in the xClockOutputInitStruct.
 * @param  pxClockOutputInitStruct pointer to a ClockOutputInit structure that
 *         contains the configuration information for the SPIRIT Clock Output.
 * @retval None.
 * @note   The function SpiritGpioClockOutput() must be called in order to enable
 *         or disable the MCU clock dividers.
 */
void SpiritGpioClockOutputInit(ClockOutputInit* pxClockOutputInitStruct)
{
  uint8_t tempRegValue = 0x00;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_CLOCK_OUTPUT_XO(pxClockOutputInitStruct->xClockOutputXOPrescaler));
  s_assert_param(IS_SPIRIT_CLOCK_OUTPUT_RCO(pxClockOutputInitStruct->xClockOutputRCOPrescaler));
  s_assert_param(IS_SPIRIT_CLOCK_OUTPUT_EXTRA_CYCLES(pxClockOutputInitStruct->xExtraClockCycles));

  /* Calculates the register value to write according to the specified configuration */
  tempRegValue = ((uint8_t)(pxClockOutputInitStruct->xClockOutputXOPrescaler) | (uint8_t)(pxClockOutputInitStruct->xClockOutputRCOPrescaler) | \
           (uint8_t)(pxClockOutputInitStruct->xExtraClockCycles));

  /* Writes the MCU_CLOCK register */
  g_xStatus = SpiritSpiWriteRegisters(MCU_CK_CONF_BASE, 1, &tempRegValue);

}


/**
 * @brief  Sets the XO ratio as clock output.
 * @param  xXOPrescaler the XO prescaler to be used as clock output.
 *         This parameter can be any value of @ref ClockOutputXOPrescaler .
 * @retval None
 */
void SpiritGpioSetXOPrescaler(ClockOutputXOPrescaler xXOPrescaler)
{
  uint8_t tempRegValue = 0x00;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_CLOCK_OUTPUT_XO(xXOPrescaler));

  /* Reads the MCU_CLK_CONFIG register */
  g_xStatus = SpiritSpiReadRegisters(MCU_CK_CONF_BASE, 1, &tempRegValue);

  /* Mask the XO_RATIO field and writes the new value */
  tempRegValue &= 0x61;
  tempRegValue |= ((uint8_t)xXOPrescaler);

  /* Writes the new XO prescaler in the MCU_CLOCK register */
  g_xStatus = SpiritSpiWriteRegisters(MCU_CK_CONF_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the settled XO prescaler as clock output.
 * @param  None.
 * @retval ClockOutputXOPrescaler Settled XO prescaler used for clock
 *         output. This parameter can be a value of @ref ClockOutputXOPrescaler .
 */
ClockOutputXOPrescaler SpiritGpioGetXOPrescaler(void)
{
  uint8_t tempRegValue = 0x00;

  /* Reads the MCU_CLK_CONFIG register */
  g_xStatus = SpiritSpiReadRegisters(MCU_CK_CONF_BASE, 1, &tempRegValue);

  /* Mask the XO_RATIO field and return the value */
  return ((ClockOutputXOPrescaler)(tempRegValue & 0x1E));

}


/**
 * @brief  Sets the RCO ratio as clock output
 * @param  xRCOPrescaler the RCO prescaler to be used as clock output.
 *         This parameter can be any value of @ref ClockOutputRCOPrescaler .
 * @retval None.
 */
void SpiritGpioSetRCOPrescaler(ClockOutputRCOPrescaler xRCOPrescaler)
{
  uint8_t tempRegValue = 0x00;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_CLOCK_OUTPUT_RCO(xRCOPrescaler));

  /* Reads the MCU_CLK_CONFIG register */
  g_xStatus = SpiritSpiReadRegisters(MCU_CK_CONF_BASE, 1, &tempRegValue);

  /* Mask the RCO_RATIO field and writes the new value */
  tempRegValue &= 0xFE;
  tempRegValue |= ((uint8_t)xRCOPrescaler);

  /* Writes the new RCO prescaler in the MCU_CLOCK register */
  g_xStatus = SpiritSpiWriteRegisters(MCU_CK_CONF_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the settled RCO prescaler as clock output.
 * @param  None.
 * @retval ClockOutputRCOPrescaler Settled RCO prescaler used for clock
 *         output. This parameter can be a value of @ref ClockOutputRCOPrescaler.
 */
ClockOutputRCOPrescaler SpiritGpioGetRCOPrescaler(void)
{
  uint8_t tempRegValue = 0x00;

  /* Reads the MCU_CLK_CONFIG register */
  g_xStatus = SpiritSpiReadRegisters(MCU_CK_CONF_BASE, 1, &tempRegValue);

  /* Mask the RCO_RATIO field and returns the value */
  return ((ClockOutputRCOPrescaler)(tempRegValue & 0x01));

}


/**
 * @brief  Sets the RCO ratio as clock output.
 * @param  xExtraCycles the number of extra clock cycles provided before switching
 *         to STANDBY state. This parameter can be any value of @ref ExtraClockCycles .
 * @retval None.
 */
void SpiritGpioSetExtraClockCycles(ExtraClockCycles xExtraCycles)
{
  uint8_t tempRegValue = 0x00;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_CLOCK_OUTPUT_EXTRA_CYCLES(xExtraCycles));

  /* Reads the MCU_CLK_CONFIG register */
  g_xStatus = SpiritSpiReadRegisters(MCU_CK_CONF_BASE, 1, &tempRegValue);

  /* Mask the CLOCK_TAIL field and writes the new value */
  tempRegValue &= 0x9F;
  tempRegValue |= ((uint8_t)xExtraCycles);

  /* Writes the new number of extra clock cycles in the MCU_CLOCK register */
  g_xStatus = SpiritSpiWriteRegisters(MCU_CK_CONF_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the settled RCO prescaler as clock output.
 * @param  None.
 * @retval ExtraClockCycles Settled number of extra clock cycles
 *         provided before switching to STANDBY state. This parameter can be
 *         any value of @ref ExtraClockCycles .
 */
ExtraClockCycles SpiritGpioGetExtraClockCycles(void)
{
  uint8_t tempRegValue = 0x00;

  /* Reads the MCU_CLK_CONFIG register */
  g_xStatus = SpiritSpiReadRegisters(MCU_CK_CONF_BASE, 1, &tempRegValue);

  /* Mask the CLOCK_TAIL field and returns the value */
  return ((ExtraClockCycles)(tempRegValue & 0x60));

}

/**
 * @}
 */
 
/* --------------------------------   IRQ  ----------------------------------- */ 

/**
 * @defgroup Irq_Private_Functions              IRQ Private Functions
 * @{
 */


/**
 * @brief  De initializate the SpiritIrqs structure setting all the bitfield to 0.
 *         Moreover, it sets the IRQ mask registers to 0x00000000, disabling all IRQs.
 * @param  pxIrqInit pointer to a variable of type @ref SpiritIrqs, in which all the
 *         bitfields will be settled to zero.
 * @retval None.
 */
void SpiritIrqDeInit(SpiritIrqs* pxIrqInit)
{
  uint8_t tempRegValue[4]={0x00,0x00,0x00,0x00};

  if(pxIrqInit!=NULL)
  {
    uint32_t tempValue = 0x00000000;
    
    /* Sets the bitfields of passed structure to one */
    *pxIrqInit = (*(SpiritIrqs*)(&tempValue));
  }

  /* Writes the IRQ_MASK registers */
  g_xStatus = SpiritSpiWriteRegisters(IRQ_MASK3_BASE, 4, tempRegValue);
}


/**
 * @brief  Enables all the IRQs according to the user defined pxIrqInit structure.
 * @param  pxIrqInit pointer to a variable of type @ref SpiritIrqs, through which the
 *         user enable specific IRQs. This parameter is a pointer to a SpiritIrqs.
 *         For example suppose to enable only the two IRQ Low Battery Level and Tx Data Sent:
 * @code
 * SpiritIrqs myIrqInit = {0};
 * myIrqInit.IRQ_LOW_BATT_LVL = 1;
 * myIrqInit.IRQ_TX_DATA_SENT = 1;
 * SpiritIrqInit(&myIrqInit);
 * @endcode
 * @retval None.
 */
void SpiritIrqInit(SpiritIrqs* pxIrqInit)
{
  uint8_t tempRegValue[4];
  uint8_t* tmpPoint;

  /* Cast the bitfields structure in an array of char using */
  tmpPoint = (uint8_t*)(pxIrqInit);
  for(uint8_t i=0; i<4; i++)
  {
    tempRegValue[3-i]= tmpPoint[i];
  }

  /* Writes the IRQ_MASK registers */
  g_xStatus = SpiritSpiWriteRegisters(IRQ_MASK3_BASE, 4, tempRegValue);

}


/**
 * @brief  Enables or disables a specific IRQ.
 * @param  xIrq IRQ to enable or disable.
 *         This parameter can be any value of @ref IrqList.
 * @param  xNewState new state for the IRQ.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritIrq(IrqList xIrq, SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue[4];
  uint32_t tempValue = 0;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_IRQ_LIST(xIrq));
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the IRQ_MASK registers */
  g_xStatus = SpiritSpiReadRegisters(IRQ_MASK3_BASE, 4, tempRegValue);

  /* Build the IRQ mask word */
  for(uint8_t i=0; i<4; i++)
  {
    tempValue += ((uint32_t)tempRegValue[i])<<(8*(3-i));
  }
  
  /* Rebuild the new mask according to user request */
  if(xNewState == S_DISABLE)
  {
    tempValue &= (~xIrq);
  }
  else
  {
    tempValue |= (xIrq);
  }

  /* Build the array of bytes to write in the IRQ_MASK registers */
  for(uint8_t j=0; j<4; j++)
  {
    tempRegValue[j] = (uint8_t)(tempValue>>(8*(3-j)));
  }
  
  /* Writes the new IRQ mask in the corresponding registers */
  g_xStatus = SpiritSpiWriteRegisters(IRQ_MASK3_BASE, 4, tempRegValue);

}


/**
 * @brief  Fills a pointer to a structure of SpiritIrqs type reading the IRQ_MASK registers.
 * @param  pxIrqMask pointer to a variable of type @ref SpiritIrqs, through which the
 *         user can read which IRQs are enabled. All the bitfields equals to zero correspond
 *         to enabled IRQs, while all the bitfields equals to one correspond to disabled IRQs.
 *         This parameter is a pointer to a SpiritIrqs.
 *         For example suppose that the Power On Reset and RX Data ready are the only enabled IRQs.
 * @code
 * SpiritIrqs myIrqMask;
 * SpiritIrqGetStatus(&myIrqMask);
 * @endcode
 * Then
 * myIrqMask.IRQ_POR and myIrqMask.IRQ_RX_DATA_READY are equal to 0
 * while all the other bitfields are equal to one.
 * @retval None.
 */
void SpiritIrqGetMask(SpiritIrqs* pxIrqMask)
{
  uint8_t tempRegValue[4];
  uint8_t* pIrqPointer = (uint8_t*)pxIrqMask;

  /* Reads IRQ_MASK registers */
  g_xStatus = SpiritSpiReadRegisters(IRQ_MASK3_BASE, 4, tempRegValue);

  /* Build the IRQ mask word */
  for(char i=0; i<4; i++)
  {
    *pIrqPointer = tempRegValue[3-i];
    pIrqPointer++;
  }

}


/**
 * @brief  Filla a pointer to a structure of SpiritIrqs type reading the IRQ_STATUS registers.
 * @param  pxIrqStatus pointer to a variable of type @ref SpiritIrqs, through which the
 *         user can read the status of all the IRQs. All the bitfields equals to one correspond
 *         to the raised interrupts. This parameter is a pointer to a SpiritIrqs.
 *         For example suppose that the XO settling timeout is raised as well as the Sync word
 *         detection.
 * @code
 * SpiritIrqs myIrqStatus;
 * SpiritIrqGetStatus(&myIrqStatus);
 * @endcode
 * Then
 * myIrqStatus.IRQ_XO_COUNT_EXPIRED and myIrqStatus.IRQ_VALID_SYNC are equals to 1
 * while all the other bitfields are equals to zero.
 * @retval None.
 */
void SpiritIrqGetStatus(SpiritIrqs* pxIrqStatus)
{
  uint8_t tempRegValue[4];
  uint8_t* pIrqPointer = (uint8_t*)pxIrqStatus;
  
  /* Reads IRQ_STATUS registers */
  g_xStatus = SpiritSpiReadRegisters(IRQ_STATUS3_BASE, 4, tempRegValue);

  /* Build the IRQ Status word */
  for(uint8_t i=0; i<4; i++)
  {
    *pIrqPointer = tempRegValue[3-i];
    pIrqPointer++;
  }
}


/**
 * @brief  Clear the IRQ status registers.
 * @param  None.
 * @retval None.
 */
void SpiritIrqClearStatus(void)
{
  uint8_t tempRegValue[4];

  /* Reads the IRQ_STATUS registers clearing all the flags */
  g_xStatus = SpiritSpiReadRegisters(IRQ_STATUS3_BASE, 4, tempRegValue);

}


/**
 * @brief  Verifies if a specific IRQ has been generated.
 *         The call resets all the IRQ status, so it can't be used in case of multiple raising interrupts.
 * @param  xFlag IRQ flag to be checked.
 *         This parameter can be any value of @ref IrqList.
 * @retval SpiritBool S_TRUE or S_FALSE.
 */
SpiritBool SpiritIrqCheckFlag(IrqList xFlag)
{
  uint8_t tempRegValue[4];
  uint32_t tempValue = 0;
  SpiritBool flag;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_IRQ_LIST(xFlag));

  /* Reads registers and build the status word */
  g_xStatus = SpiritSpiReadRegisters(IRQ_STATUS3_BASE, 4, tempRegValue);
  for(uint8_t i=0; i<4; i++)
  {
    tempValue += ((uint32_t)tempRegValue[i])<<(8*(3-i));
  }
  
  if(tempValue & xFlag)
  {
    flag = S_TRUE;
  }
  else
  {
    flag = S_FALSE;
  }

  return flag;

}

/**
 *@}
 */

/* --------------------------------   LINEAR FIFO  ----------------------------------- */ 

/**
 * @defgroup LinearFifo_Private_Functions               Linear FIFO Private Functions
 * @{
 */

/**
 * @brief  Returns the number of elements in the Rx FIFO.
 * @param  None.
 * @retval uint8_t Number of elements in the Rx FIFO.
 */
uint8_t SpiritLinearFifoReadNumElementsRxFifo(void)
{
  uint8_t tempRegValue;

  /* Reads the register value */
  g_xStatus = SpiritSpiReadRegisters(LINEAR_FIFO_STATUS0_BASE, 1, &tempRegValue);

  /* Build and return value */
  return (tempRegValue & 0x7F);

}


/**
 * @brief  Returns the number of elements in the Tx FIFO.
 * @param  None.
 * @retval uint8_t Number of elements in the Tx FIFO.
 */
uint8_t SpiritLinearFifoReadNumElementsTxFifo(void)
{
  uint8_t tempRegValue;

  /* Reads the number of elements in TX FIFO and return the value */
  g_xStatus = SpiritSpiReadRegisters(LINEAR_FIFO_STATUS1_BASE, 1, &tempRegValue);

  /* Build and return value */
  return (tempRegValue & 0x7F);

}


/**
 * @brief  Sets the almost full threshold for the Rx FIFO. When the number of elements in RX FIFO reaches this value an interrupt can be generated to the MCU.
 * @note   The almost full threshold is encountered from the top of the FIFO. For example, if it is set to 7 the almost
 *         full FIFO irq will be raised when the number of elements is equals to 96-7 = 89.
 * @param  cThrRxFifo almost full threshold.
 *        This parameter is an uint8_t.
 * @retval None.
 */
void SpiritLinearFifoSetAlmostFullThresholdRx(uint8_t cThrRxFifo)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_FIFO_THR(cThrRxFifo));

  /* Build the register value */
  tempRegValue = cThrRxFifo & 0x7F;

  /* Writes the Almost Full threshold for RX in the corresponding register */
  g_xStatus = SpiritSpiWriteRegisters(FIFO_CONFIG3_RXAFTHR_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the almost full threshold for RX FIFO.
 * @note   The almost full threshold is encountered from the top of the FIFO. For example, if it is 7 the almost
 *         full FIFO irq will be raised when the number of elements is equals to 96-7 = 89.
 * @param  None.
 * @retval uint8_t Almost full threshold for Rx FIFO.
 */
uint8_t SpiritLinearFifoGetAlmostFullThresholdRx(void)
{
  uint8_t tempRegValue;

  /* Reads the almost full threshold for RX FIFO and return the value */
  g_xStatus = SpiritSpiReadRegisters(FIFO_CONFIG3_RXAFTHR_BASE, 1, &tempRegValue);

  /* Build and return value */
  return (tempRegValue & 0x7F);

}


/**
 * @brief  Sets the almost empty threshold for the Rx FIFO. When the number of elements in RX FIFO reaches this value an interrupt can be generated to the MCU.
 * @param  cThrRxFifo almost empty threshold.
 *        This parameter is an uint8_t.
 * @retval None.
 */
void SpiritLinearFifoSetAlmostEmptyThresholdRx(uint8_t cThrRxFifo)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_FIFO_THR(cThrRxFifo));

  /* Build the register value */
  tempRegValue = cThrRxFifo & 0x7F;

  /* Writes the Almost Empty threshold for RX in the corresponding register */
  g_xStatus = SpiritSpiWriteRegisters(FIFO_CONFIG2_RXAETHR_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the almost empty threshold for Rx FIFO.
 * @param  None.
 * @retval uint8_t Almost empty threshold for Rx FIFO.
 */
uint8_t SpiritLinearFifoGetAlmostEmptyThresholdRx(void)
{
  uint8_t tempRegValue;

  /* Reads the almost empty threshold for RX FIFO and returns the value */
  g_xStatus = SpiritSpiReadRegisters(FIFO_CONFIG2_RXAETHR_BASE, 1, &tempRegValue);

  /* Build and return value */
  return (tempRegValue & 0x7F);

}


/**
 * @brief  Sets the almost full threshold for the Tx FIFO. When the number of elements in TX FIFO reaches this value an interrupt can be generated to the MCU.
 * @note   The almost full threshold is encountered from the top of the FIFO. For example, if it is set to 7 the almost
 *         full FIFO irq will be raised when the number of elements is equals to 96-7 = 89.
 * @param  cThrTxFifo almost full threshold.
 *        This parameter is an uint8_t.
 * @retval None.
 */
void SpiritLinearFifoSetAlmostFullThresholdTx(uint8_t cThrTxFifo)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_FIFO_THR(cThrTxFifo));

  /* Reads the register value */
  g_xStatus = SpiritSpiReadRegisters(FIFO_CONFIG1_TXAFTHR_BASE, 1, &tempRegValue);

  /* Build the register value */
  tempRegValue &= 0x80;
  tempRegValue |= cThrTxFifo;

  /* Writes the Almost Full threshold for Tx in the corresponding register */
  g_xStatus = SpiritSpiWriteRegisters(FIFO_CONFIG1_TXAFTHR_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the almost full threshold for Tx FIFO.
 * @note   The almost full threshold is encountered from the top of the FIFO. For example, if it is set to 7 the almost
 *         full FIFO irq will be raised when the number of elements is equals to 96-7 = 89.
 * @param  None.
 * @retval uint8_t Almost full threshold for Tx FIFO.
 */
uint8_t SpiritLinearFifoGetAlmostFullThresholdTx(void)
{
  uint8_t tempRegValue;

  /* Reads the almost full threshold for Tx FIFO and returns the value */
  g_xStatus = SpiritSpiReadRegisters(FIFO_CONFIG1_TXAFTHR_BASE, 1, &tempRegValue);

  /* Build and returns value */
  return (tempRegValue & 0x7F);

}


/**
 * @brief  Sets the almost empty threshold for the Tx FIFO. When the number of elements in Tx FIFO reaches this value an interrupt can can be generated to the MCU.
 * @param  cThrTxFifo: almost empty threshold.
 *         This parameter is an uint8_t.
 * @retval None.
 */
void SpiritLinearFifoSetAlmostEmptyThresholdTx(uint8_t cThrTxFifo)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_FIFO_THR(cThrTxFifo));

  /* Reads the register value */
  g_xStatus = SpiritSpiReadRegisters(FIFO_CONFIG0_TXAETHR_BASE, 1, &tempRegValue);

  /* Build the register value */
  tempRegValue &= 0x80;
  tempRegValue |= cThrTxFifo;

  /* Writes the Almost Empty threshold for Tx in the corresponding register */
  g_xStatus = SpiritSpiWriteRegisters(FIFO_CONFIG0_TXAETHR_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the almost empty threshold for Tx FIFO.
 * @param  None.
 * @retval uint8_t Almost empty threshold for Tx FIFO.
 */
uint8_t SpiritLinearFifoGetAlmostEmptyThresholdTx(void)
{
  uint8_t tempRegValue;

  /* Reads the almost empty threshold for TX FIFO and returns the value */
  g_xStatus = SpiritSpiReadRegisters(FIFO_CONFIG0_TXAETHR_BASE, 1, &tempRegValue);

  /* Build and return value */
  return (tempRegValue & 0x7F);

}

/**
 *@}
 */
 
/* --------------------------------   MANAGEMENT  ----------------------------------- */ 

/**
* @defgroup SPIRIT_MANAGEMENT_FUNCTIONS    SPIRIT Management Functions
* @{
*/

/**
* @brief  Private SpiritRadioSetFrequencyBase function only used in SpiritManagementWaVcoCalibration.
* @param  lFBase the base carrier frequency expressed in Hz as unsigned word.
* @retval None.
*/
void SpiritManagementSetFrequencyBase(uint32_t lFBase)
{
  uint32_t synthWord, Fc;
  uint8_t band, anaRadioRegArray[4], wcp;
  
  /* Check the parameter */
  s_assert_param(IS_FREQUENCY_BAND(lFBase));
  
  /* Search the operating band */
  if(IS_FREQUENCY_BAND_HIGH(lFBase))
  {
    band = HIGH_BAND;
  }
  else if(IS_FREQUENCY_BAND_MIDDLE(lFBase))
  {
    band = MIDDLE_BAND;
  }
  else if(IS_FREQUENCY_BAND_LOW(lFBase))
  {
    band = LOW_BAND;
  }
  else //if(IS_FREQUENCY_BAND_VERY_LOW(lFBase))
  {
    band = VERY_LOW_BAND;
  }
  
  int32_t FOffset  = SpiritRadioGetFrequencyOffset();
  uint32_t lChannelSpace  = SpiritRadioGetChannelSpace();
  uint8_t cChannelNum = SpiritRadioGetChannel();
  
  /* Calculates the channel center frequency */
  Fc = lFBase + FOffset + lChannelSpace*cChannelNum;
  
  /* Reads the reference divider */
  uint8_t cRefDiv = (uint8_t)SpiritRadioGetRefDiv()+1;
  
  switch(band)
  {
  case VERY_LOW_BAND:
    if(Fc<161281250)
    {
      SpiritCalibrationSelectVco(VCO_L);
    }
    else
    {
      SpiritCalibrationSelectVco(VCO_H);
    }
    break;
    
  case LOW_BAND:
    if(Fc<322562500)
    {
      SpiritCalibrationSelectVco(VCO_L);
    }
    else
    {
      SpiritCalibrationSelectVco(VCO_H);
    }
    break;
    
  case MIDDLE_BAND:
    if(Fc<430083334)
    {
      SpiritCalibrationSelectVco(VCO_L);
    }
    else
    {
      SpiritCalibrationSelectVco(VCO_H);
    }
    break;
    
  case HIGH_BAND:
    if(Fc<860166667)
    {
      SpiritCalibrationSelectVco(VCO_L);
    }
    else
    {
      SpiritCalibrationSelectVco(VCO_H);
    }
  }
  
  /* Search the VCO charge pump word and set the corresponding register */
  wcp = SpiritRadioSearchWCP(Fc);
  
  synthWord = (uint32_t)(lFBase*(((double)(FBASE_DIVIDER*cRefDiv*s_vectcBHalfFactor[band]))/SpiritRadioGetXtalFrequency()));
  
  /* Build the array of registers values for the analog part */
  anaRadioRegArray[0] = (uint8_t)(((synthWord>>21)&(0x0000001F))|(wcp<<5));
  anaRadioRegArray[1] = (uint8_t)((synthWord>>13)&(0x000000FF));
  anaRadioRegArray[2] = (uint8_t)((synthWord>>5)&(0x000000FF));
  anaRadioRegArray[3] = (uint8_t)(((synthWord&0x0000001F)<<3)| s_vectcBandRegValue[band]);
  
  /* Configures the needed Analog Radio registers */
  g_xStatus = SpiritSpiWriteRegisters(SYNT3_BASE, 4, anaRadioRegArray);
}

uint8_t SpiritManagementWaVcoCalibration(void)
{
  uint8_t s_cVcoWordRx;
  uint8_t s_cVcoWordTx;
  uint32_t nFreq;
  uint8_t cRestore = 0;
  uint8_t cStandby = 0;
  uint32_t xtal_frequency = SpiritRadioGetXtalFrequency();
  uint8_t nLockwon=0;
  
  /* Enable the reference divider if the XTAL is between 48 and 52 MHz */
  if(xtal_frequency>DOUBLE_XTAL_THR)
  {
    if(!SpiritRadioGetRefDiv())
    {
      cRestore = 1;
      nFreq = SpiritRadioGetFrequencyBase();
      SpiritRadioSetRefDiv(S_ENABLE);
      SpiritManagementSetFrequencyBase(nFreq);
    }
  }
  nFreq = SpiritRadioGetFrequencyBase();
  
  /* Increase the VCO current */
  uint8_t tmp = 0x25; SpiritSpiWriteRegisters(0xA1,1,&tmp);
  
  SpiritCalibrationVco(S_ENABLE);
  
  SpiritRefreshStatus();
  if(g_xStatus.MC_STATE == MC_STATE_STANDBY)
  {
    cStandby = 1;
    SpiritCmdStrobeReady();
    do{
      SpiritRefreshStatus();
      
    }while(g_xStatus.MC_STATE != MC_STATE_READY); 
  }
  
  SpiritCmdStrobeLockTx();
  
  nLockwon=0;
  do{
    SpiritRefreshStatus();
    if(g_xStatus.MC_STATE == MC_STATE_LOCKWON)
    {
      if(nLockwon++==5) return 1;
    }
  }while(g_xStatus.MC_STATE != MC_STATE_LOCK);
    
  s_cVcoWordTx = SpiritCalibrationGetVcoCalData();
  
  SpiritCmdStrobeReady();
  
  do{
    SpiritRefreshStatus();
  }while(g_xStatus.MC_STATE != MC_STATE_READY); 
  
    
  SpiritCmdStrobeLockRx();
  
  nLockwon=0;
  do{
    SpiritRefreshStatus();
    if(g_xStatus.MC_STATE == MC_STATE_LOCKWON)
    {
      if(nLockwon++==5) return 1;
    }
  }while(g_xStatus.MC_STATE != MC_STATE_LOCK);
  
  s_cVcoWordRx = SpiritCalibrationGetVcoCalData();
  
  SpiritCmdStrobeReady();
  
  do{
    SpiritRefreshStatus();
   
  }while(g_xStatus.MC_STATE != MC_STATE_READY);
  
  if(cStandby == 1)
  {
    SpiritCmdStrobeStandby();    
  }
  SpiritCalibrationVco(S_DISABLE);
  
  /* Disable the reference divider if the XTAL is between 48 and 52 MHz */
  if(cRestore)
  {
    SpiritRadioSetRefDiv(S_DISABLE);    
    SpiritManagementSetFrequencyBase(nFreq);
  }
  
  
  SpiritCalibrationSetVcoCalDataTx(s_cVcoWordTx);
  SpiritCalibrationSetVcoCalDataRx(s_cVcoWordRx);
  
  return 0;
}


void SpiritManagementWaCmdStrobeTx(void)
{
  if(s_cCommunicationState != COMMUNICATION_STATE_TX)
  {
    //uint32_t xtal_frequency = SpiritRadioGetXtalFrequency();
    
    /* To achive the max output power */
    if(s_nDesiredFrequency>=150000000 && s_nDesiredFrequency<=470000000)
    {
      /* Optimal setting for Tx mode only */
      SpiritRadioSetPACwc(LOAD_3_6_PF);
    }
    else
    {
      /* Optimal setting for Tx mode only */
      SpiritRadioSetPACwc(LOAD_0_PF);
    }
    
    uint8_t tmp = 0x11; SpiritSpiWriteRegisters(0xa9, 1, &tmp); /* Enable VCO_L buffer */
    tmp = 0x20; SpiritSpiWriteRegisters(PM_CONFIG1_BASE, 1, &tmp); /* Set SMPS switching frequency */
    
    s_cCommunicationState = COMMUNICATION_STATE_TX;
  }
}


void SpiritManagementWaCmdStrobeRx(void)
{
  if(s_cCommunicationState != COMMUNICATION_STATE_RX)
  {    
    uint8_t tmp = 0x98; SpiritSpiWriteRegisters(PM_CONFIG1_BASE, 1, &tmp); /* Set SMPS switching frequency */    
    SpiritRadioSetPACwc(LOAD_0_PF); /* Set the correct CWC parameter */
    
    s_cCommunicationState = COMMUNICATION_STATE_RX;
  }
}

void SpiritManagementWaTRxFcMem(uint32_t nDesiredFreq)
{
  s_cCommunicationState = COMMUNICATION_STATE_NONE;
  s_nDesiredFrequency = nDesiredFreq;
}


void SpiritManagementWaExtraCurrent(void)
{          
  uint8_t tmp= 0xCA;SpiritSpiWriteRegisters(0xB2, 1, &tmp); 
  tmp= 0x04;SpiritSpiWriteRegisters(0xA8, 1, &tmp); 
  /* just a read to loose some microsecs more */
  SpiritSpiReadRegisters(0xA8, 1, &tmp);
  tmp= 0x00;SpiritSpiWriteRegisters(0xA8, 1, &tmp); 
}

/**
* @}
*/ 
 
/* --------------------------------   PKT BASIC  ----------------------------------- */ 
 
/**
 * @defgroup PktBasic_Private_Functions                 Pkt Basic Private Functions
 * @{
 */

/**
 * @brief  Initializes the SPIRIT Basic packet according to the specified parameters in the PktBasicInit struct.
 *         Notice that this function sets the autofiltering option on CRC if it is set to any value different from BASIC_NO_CRC.
 * @param  pxPktBasicInit Basic packet init structure.
 *         This parameter is a pointer to @ref PktBasicInit.
 * @retval None.
 */
void SpiritPktBasicInit(PktBasicInit* pxPktBasicInit)
{
  uint8_t tempRegValue[4], i;

  /* Check the parameters */
  s_assert_param(IS_BASIC_PREAMBLE_LENGTH(pxPktBasicInit->xPreambleLength));
  s_assert_param(IS_BASIC_SYNC_LENGTH(pxPktBasicInit->xSyncLength));
  s_assert_param(IS_BASIC_CRC_MODE(pxPktBasicInit->xCrcMode));
  s_assert_param(IS_BASIC_LENGTH_WIDTH_BITS(pxPktBasicInit->cPktLengthWidth));
  s_assert_param(IS_BASIC_FIX_VAR_LENGTH(pxPktBasicInit->xFixVarLength));
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(pxPktBasicInit->xAddressField));
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(pxPktBasicInit->xFec));
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(pxPktBasicInit->xDataWhitening));
  s_assert_param(IS_BASIC_CONTROL_LENGTH(pxPktBasicInit->xControlLength));

  /* Reads the PROTOCOL1 register */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 1, &tempRegValue[0]);

  /* Mask a reserved bit */
  tempRegValue[0] &= ~0x20;

  /* Always set the automatic packet filtering */
  tempRegValue[0] |= PROTOCOL1_AUTO_PCKT_FLT_MASK;

  /* Writes the value on register */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL1_BASE, 1, &tempRegValue[0]);

  /* Reads the PCKT_FLT_OPTIONS register */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue[0]);

  /* Always reset the control and source filtering (also if it is not present in basic) */
  tempRegValue[0] &= ~(PCKT_FLT_OPTIONS_SOURCE_FILTERING_MASK | PCKT_FLT_OPTIONS_CONTROL_FILTERING_MASK);

  /* Writes the value on register */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue[0]);

  if(pxPktBasicInit->xAddressField == S_ENABLE)
  {
    tempRegValue[0]=0x08;
  }
  else
  {
    tempRegValue[0]=0x00;
  }
  /* Address and control length setting */
  tempRegValue[0] |= ((uint8_t) pxPktBasicInit->xControlLength);

  /* Packet format and width length setting */
  pxPktBasicInit->cPktLengthWidth == 0 ? pxPktBasicInit->cPktLengthWidth=1 : pxPktBasicInit->cPktLengthWidth;
  tempRegValue[1] = ((uint8_t) PCKTCTRL3_PCKT_FRMT_BASIC) | ((uint8_t)(pxPktBasicInit->cPktLengthWidth-1));

  /* Preamble, sync and fixed or variable length setting */
  tempRegValue[2] = ((uint8_t) pxPktBasicInit->xPreambleLength) | ((uint8_t) pxPktBasicInit->xSyncLength) |
                    ((uint8_t) pxPktBasicInit->xFixVarLength);

  /* CRC length, whitening and FEC setting */
  tempRegValue[3] = (uint8_t) pxPktBasicInit->xCrcMode;

  if(pxPktBasicInit->xDataWhitening == S_ENABLE)
  {
     tempRegValue[3] |= PCKTCTRL1_WHIT_MASK;
  }

  if(pxPktBasicInit->xFec == S_ENABLE)
  {
     tempRegValue[3] |= PCKTCTRL1_FEC_MASK;
  }

  /* Writes registers */
  SpiritSpiWriteRegisters(PCKTCTRL4_BASE, 4, tempRegValue);

  /* Sync words setting */
  for(i=0;i<4;i++)
  {
    if(i<3-(pxPktBasicInit->xSyncLength >>1))
    {
      tempRegValue[i]=0;
    }
    else
    {
      tempRegValue[i] = (uint8_t)(pxPktBasicInit->lSyncWords>>(8*i));
    }
  }

  /* Sets CRC check bit */
  if(pxPktBasicInit->xCrcMode == PKT_NO_CRC)
  {
    SpiritPktBasicFilterOnCrc(S_DISABLE);
  }
  else
  {
    SpiritPktBasicFilterOnCrc(S_ENABLE);
  }

  
  g_xStatus = SpiritSpiWriteRegisters(SYNC4_BASE, 4, tempRegValue);

}


/**
 * @brief  Returns the SPIRIT Basic packet structure according to the specified parameters in the registers.
 * @param  pxPktBasicInit Basic packet init structure.
 *         This parameter is a pointer to @ref PktBasicInit.
 * @retval None.
 */
void SpiritPktBasicGetInfo(PktBasicInit* pxPktBasicInit)
{
  uint8_t tempRegValue[10];

  /* Reads registers */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL4_BASE, 10, tempRegValue);

  /* Length width */
  pxPktBasicInit->cPktLengthWidth=(tempRegValue[1] & 0x0F)+1;

  /* Address field */
  pxPktBasicInit->xAddressField=(SpiritFunctionalState)((tempRegValue[0]>>3) & 0x01);

  /* Control length */
  pxPktBasicInit->xControlLength=(BasicControlLength)(tempRegValue[0] & 0x07);

  /* CRC mode */
  pxPktBasicInit->xCrcMode=(BasicCrcMode)(tempRegValue[3] & 0xE0);

  /* Whitening */
  pxPktBasicInit->xDataWhitening=(SpiritFunctionalState)((tempRegValue[3] >> 4) & 0x01);

  /* FEC */
  pxPktBasicInit->xFec=(SpiritFunctionalState)(tempRegValue[3] & 0x01);

  /* FIX or VAR bit */
  pxPktBasicInit->xFixVarLength=(BasicFixVarLength)(tempRegValue[2] & 0x01);

  /* Preamble length */
  pxPktBasicInit->xPreambleLength=(BasicPreambleLength)(tempRegValue[2] & 0xF8);

  /* Sync length */
  pxPktBasicInit->xSyncLength=(BasicSyncLength)(tempRegValue[2] & 0x06);

  /* sync Words */
  pxPktBasicInit->lSyncWords=0;
  for(uint8_t i=0 ; i<4 ; i++)
  {
      if(i>2-(((uint8_t)pxPktBasicInit->xSyncLength) >>1))
      {
        pxPktBasicInit->lSyncWords |= (uint32_t)(tempRegValue[i+6])<<(8*i);
      }
  }

}


/**
 * @brief  Initializes the SPIRIT Basic packet addresses according to the specified
 *         parameters in the PktBasicAddressesInit struct.
 * @param  pxPktBasicAddresses Basic packet addresses init structure.
 *         This parameter is a pointer to @ref PktBasicAddresses.
 * @retval None.
 */
void SpiritPktBasicAddressesInit(PktBasicAddressesInit* pxPktBasicAddresses)
{
  uint8_t tempRegValue[3];

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(pxPktBasicAddresses->xFilterOnMyAddress));
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(pxPktBasicAddresses->xFilterOnMulticastAddress));
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(pxPktBasicAddresses->xFilterOnBroadcastAddress));


  /* Reads the PCKT_FLT_OPTIONS ragister */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue[0]);
  
  /* Enables or disables filtering on my address */
  if(pxPktBasicAddresses->xFilterOnMyAddress == S_ENABLE)
  {
    tempRegValue[0] |= PCKT_FLT_OPTIONS_DEST_VS_TX_ADDR_MASK;
  }
  else
  {
    tempRegValue[0] &= ~PCKT_FLT_OPTIONS_DEST_VS_TX_ADDR_MASK;
  }
  
  /* Enables or disables filtering on multicast address */
  if(pxPktBasicAddresses->xFilterOnMulticastAddress == S_ENABLE)
  {
    tempRegValue[0] |= PCKT_FLT_OPTIONS_DEST_VS_MULTICAST_ADDR_MASK;
  }
  else
  {
    tempRegValue[0] &= ~PCKT_FLT_OPTIONS_DEST_VS_MULTICAST_ADDR_MASK;
  }
  
  /* Enables or disables filtering on broadcast address */
  if(pxPktBasicAddresses->xFilterOnBroadcastAddress == S_ENABLE)
  {
    tempRegValue[0] |= PCKT_FLT_OPTIONS_DEST_VS_BROADCAST_ADDR_MASK;
  }
  else
  {
    tempRegValue[0] &= ~PCKT_FLT_OPTIONS_DEST_VS_BROADCAST_ADDR_MASK;
  }
  
  /* Writes the new value on the PCKT_FLT_OPTIONS register */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue[0]);
  
  /* Fills the array with the addresses passed in the structure */
  tempRegValue[0] = pxPktBasicAddresses->cBroadcastAddress;
  tempRegValue[1] = pxPktBasicAddresses->cMulticastAddress;
  tempRegValue[2] = pxPktBasicAddresses->cMyAddress;
  
  /* Writes values on the PCKT_FLT_GOALS registers */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_GOALS_BROADCAST_BASE, 3, tempRegValue);
  
  
}


/**
 * @brief  Returns the SPIRIT Basic packet addresses structure according to the specified
 *         parameters in the registers.
 * @param  pxPktBasicAddresses Basic packet addresses init structure.
 *         This parameter is a pointer to @ref PktBasicAddresses.
 * @retval None.
 */
void SpiritPktBasicGetAddressesInfo(PktBasicAddressesInit* pxPktBasicAddresses)
{
  uint8_t tempRegValue[3];

  /* Reads values on the PCKT_FLT_GOALS registers */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_GOALS_BROADCAST_BASE, 3, tempRegValue);

  /* Fit the structure with the read addresses */
  pxPktBasicAddresses->cBroadcastAddress = tempRegValue[0];
  pxPktBasicAddresses->cMulticastAddress = tempRegValue[1];
  pxPktBasicAddresses->cMyAddress = tempRegValue[2];

  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue[0]);

  /* Fit the structure with the read filtering bits */
  pxPktBasicAddresses->xFilterOnBroadcastAddress = (SpiritFunctionalState)((tempRegValue[0] >> 1) & 0x01);
  pxPktBasicAddresses->xFilterOnMulticastAddress = (SpiritFunctionalState)((tempRegValue[0] >> 2) & 0x01);
  pxPktBasicAddresses->xFilterOnMyAddress = (SpiritFunctionalState)((tempRegValue[0] >> 3) & 0x01);

}


/**
 * @brief  Configures the Basic packet format as packet used by SPIRIT.
 * @param  None.
 * @retval None.
 */
void SpiritPktBasicSetFormat(void)
{
  uint8_t tempRegValue;

  /* Reads the register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);

  /* Build the new value. Also set to 0 the direct RX mode bits */
  tempRegValue &= 0x0F;
  tempRegValue |= (uint8_t)PCKTCTRL3_PCKT_FRMT_BASIC;

  /* Writes the  value on the PCKTCTRL3 register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);

  /* Reads the PCKTCTRL1_BASE register */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);

  /* Build the new value. Set to 0 the direct TX mode bits */
  tempRegValue &= 0xF3;

  /* Writes the value on the PCKTCTRL1 register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);

  /* Reads the PROTOCOL1 register */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 1, &tempRegValue);

  /* Mask a reserved bit */
  tempRegValue &= ~0x20;

  /* Writes the value on register */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL1_BASE, 1, &tempRegValue);
}


/**
 * @brief  Sets the address length for SPIRIT Basic packets.
 * @param  xAddressField length of ADDRESS in bytes.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritPktBasicAddressField(SpiritFunctionalState xAddressField)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xAddressField));

  /* Reads the PCKTCTRL4 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL4_BASE, 1, &tempRegValue);

  /* Build the address length for the register */
  if(xAddressField==S_ENABLE)
  {
    tempRegValue |= 0x08;
  }
  else
  {
    tempRegValue &= 0x07;
  }

  /* Writes the new value on the PCKTCTRL4 register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL4_BASE, 1, &tempRegValue);

}


/**
 * @brief  Specifies if the Address field for SPIRIT Basic packets is enabled or disabled.
 * @param  None.
 * @retval SpiritFunctionalState Notifies if the address field is enabled or disabled.
 */
SpiritFunctionalState SpiritPktBasicGetAddressField(void)
{
  uint8_t tempRegValue;

  /* Reads the PCKTCTRL4 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL4_BASE, 1, &tempRegValue);

  /* Returns the address field value */
  if(tempRegValue & PCKTCTRL4_ADDRESS_LEN_MASK)
  {
    return S_ENABLE;
  }
  else
  {
    return S_DISABLE;
  }

}


/**
 * @brief  Sets the payload length for SPIRIT Basic packets. Since the packet length
 *         depends from the address and the control field size, this
 *         function reads the correspondent registers in order to determine
 *         the correct packet length to be written.
 * @param  nPayloadLength payload length in bytes.
 *         This parameter is an uint16_t.
 * @retval None.
 */
void SpiritPktBasicSetPayloadLength(uint16_t nPayloadLength)
{
  uint8_t tempRegValue[2];
  uint16_t overSize=0;

  /* Computes the oversize (address + control) size */
  if(SpiritPktBasicGetAddressField())
  {
    overSize=1;
  }
  overSize += (uint16_t) SpiritPktBasicGetControlLength();

  /* Computes PCKTLEN0 value from nPayloadLength */
  tempRegValue[1]=BASIC_BUILD_PCKTLEN0(nPayloadLength+overSize);
  /* Computes PCKTLEN1 value from nPayloadLength */
  tempRegValue[0]=BASIC_BUILD_PCKTLEN1(nPayloadLength+overSize);

  /* Writes data on the PCKTLEN1/0 register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTLEN1_BASE, 2, tempRegValue);

}


/**
 * @brief  Returns the payload length for SPIRIT Basic packets. Since the
 *         packet length depends from the address and the control
 *         field size, this function reads the correspondent
 *         registers in order to determine the correct payload length
 *         to be returned.
 * @param  None.
 * @retval uint16_t Payload length in bytes.
 */
uint16_t SpiritPktBasicGetPayloadLength(void)
{
  uint8_t tempRegValue[2];
  uint16_t overSize=0;

  /* Computes the oversize (address + control) size */
  if(SpiritPktBasicGetAddressField())
  {
    overSize=1;
  }
  overSize += (uint16_t) SpiritPktBasicGetControlLength();

  /* Reads the packet length registers */
  g_xStatus = SpiritSpiReadRegisters(PCKTLEN1_BASE, 2, tempRegValue);

  /* Returns the packet length */
  return ((((uint16_t)tempRegValue[0])<<8) + (uint16_t) tempRegValue[1]) - overSize;

}

/**
 * @brief  Returns the packet length field of the received packet.
 * @param  None.
 * @retval uint16_t Packet length.
 */
uint16_t SpiritPktBasicGetReceivedPktLength(void)
{
  uint8_t tempRegValue[2];
  uint16_t overSize=0;

  /* Computes the oversize (address + control) size */
  if(SpiritPktBasicGetAddressField())
  {
    overSize=1;
  }
  overSize += (uint16_t) SpiritPktBasicGetControlLength();
  
  /* Reads the RX_PCKT_LENx registers value */
  g_xStatus = SpiritSpiReadRegisters(RX_PCKT_LEN1_BASE, 2, tempRegValue);

  /* Rebuild and return the the length field */
  return ((((uint16_t) tempRegValue[0]) << 8) + (uint16_t) tempRegValue[1] - overSize);

}

/**
 * @brief  Computes and sets the variable payload length for SPIRIT Basic packets.
 * @param  nMaxPayloadLength payload length in bytes.
 *         This parameter is an uint16_t.
 * @param  xAddressField Enable or Disable Address Field.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @param  xControlLength Control length in bytes.
 *         This parameter can be any value of @ref BasicControlLength.
 * @retval None.
 */
void SpiritPktBasicSetVarLengthWidth(uint16_t nMaxPayloadLength, SpiritFunctionalState xAddressField, BasicControlLength xControlLength)
{
  uint8_t tempRegValue,
          addressLength,
          i;
  uint32_t packetLength;

  /* Sets the address length according to xAddressField */
  if(xAddressField == S_ENABLE)
  {
    addressLength=1;
  }
  else
  {
    addressLength=0;
  }

  /* packet length = payload length + address length + control length */
  packetLength=nMaxPayloadLength+addressLength+xControlLength;

  /* Computes the number of bits */
  for(i=0;i<16;i++)
  {
    if(packetLength == 0) break;
    {
    packetLength >>= 1;
    }
  }
  i==0 ? i=1 : i;

  /* Reads the PCKTCTRL3 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);

  /* Build value for the length width */
  tempRegValue &= ~PCKTCTRL3_LEN_WID_MASK;
  tempRegValue |= (uint8_t)(i-1);

  /* Writes the PCKTCTRL3 register value */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);

}

/**
 *@}
 */
 
/* --------------------------------   PKT COMMON  ----------------------------------- */ 

/**
 * @defgroup PktCommon_Private_Functions                Pkt Common Private Functions
 * @{
 */

/**
 * @brief  Sets the CONTROL field length for SPIRIT packets.
 * @param  xControlLength length of CONTROL field in bytes.
 *         This parameter can be any value of @ref PktControlLength.
 * @retval None.
 */
void SpiritPktCommonSetControlLength(PktControlLength xControlLength)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_PKT_CONTROL_LENGTH(xControlLength));

  /* Reads the PCKTCTRL4 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL4_BASE, 1, &tempRegValue);

  /* Set the control length */
  tempRegValue &= ~PCKTCTRL4_CONTROL_LEN_MASK;
  tempRegValue |= (uint8_t)xControlLength;

  /* Writes the new value on the PCKTCTRL4 register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL4_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the CONTROL field length for SPIRIT packets.
 * @param  None.
 * @retval uint8_t Control field length.
 */
uint8_t SpiritPktCommonGetControlLength(void)
{
  uint8_t tempRegValue;

  /* Reads the PCKTCTRL4 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL4_BASE, 1, &tempRegValue);

  /* Rebuild and return value */
  return (tempRegValue & PCKTCTRL4_CONTROL_LEN_MASK);

}


/**
 * @brief  Sets the PREAMBLE field Length mode for SPIRIT packets.
 * @param  xPreambleLength length of PREAMBLE field in bytes.
 *         This parameter can be any value of @ref PktPreambleLength.
 * @retval None.
 */
void SpiritPktCommonSetPreambleLength(PktPreambleLength xPreambleLength)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_PKT_PREAMBLE_LENGTH(xPreambleLength));

  /* Reads the PCKTCTRL2 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL2_BASE, 1, &tempRegValue);

  /* Set the preamble length */
  tempRegValue &= ~PCKTCTRL2_PREAMBLE_LENGTH_MASK;
  tempRegValue |= (uint8_t)xPreambleLength;

  /* Writes the new value on the PCKTCTRL2 register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL2_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the PREAMBLE field Length mode for SPIRIT packets.
 * @param  None.
 * @retval uint8_t Preamble field length in bytes.
 */
uint8_t SpiritPktCommonGetPreambleLength(void)
{
  uint8_t tempRegValue;

  /* Reads the PCKTCTRL2 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL2_BASE, 1, &tempRegValue);

  /* Rebuild and return value */
  return ((tempRegValue & PCKTCTRL2_PREAMBLE_LENGTH_MASK)>>3) + 1;

}


/**
 * @brief  Sets the SYNC field Length for SPIRIT packets.
 * @param  xSyncLength length of SYNC field in bytes.
 *         This parameter can be any value of @ref PktSyncLength.
 * @retval None.
 */
void SpiritPktCommonSetSyncLength(PktSyncLength xSyncLength)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_PKT_SYNC_LENGTH(xSyncLength));

  /* Reads the PCKTCTRL2 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL2_BASE, 1, &tempRegValue);

  /* Set the sync length */
  tempRegValue &= ~PCKTCTRL2_SYNC_LENGTH_MASK;
  tempRegValue |= (uint8_t)xSyncLength;

  /* Writes the new value on the PCKTCTRL2 register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL2_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the SYNC field Length for SPIRIT packets.
 * @param  None.
 * @retval uint8_t Sync field length in bytes.
 */
uint8_t SpiritPktCommonGetSyncLength(void)
{
  uint8_t tempRetValue;

  /* Reads the PCKTCTRL2 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL2_BASE, 1, &tempRetValue);

  /* Rebuild and return value */
  return ((tempRetValue & PCKTCTRL2_SYNC_LENGTH_MASK)>>1) + 1;

}


/**
 * @brief  Sets fixed or variable payload length mode for SPIRIT packets.
 * @param  xFixVarLength variable or fixed length.
 *         PKT_FIXED_LENGTH_VAR -> variable (the length is extracted from the received packet).
 *         PKT_FIXED_LENGTH_FIX -> fix (the length is set by PCKTLEN0 and PCKTLEN1).
 * @retval None.
 */
void SpiritPktCommonSetFixVarLength(PktFixVarLength xFixVarLength)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_PKT_FIX_VAR_LENGTH(xFixVarLength));

  /* Reads the PCKTCTRL2 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL2_BASE, 1, &tempRegValue);

  /* Set fixed or variable address mode */
  tempRegValue &= ~PCKTCTRL2_FIX_VAR_LEN_MASK;
  tempRegValue |= (uint8_t)xFixVarLength;

  /* Writes the new value on the PCKTCTRL2 register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL2_BASE, 1, &tempRegValue);

}


/**
 * @brief  Enables or Disables the filtering on CRC.
 * @param  xNewState new state for CRC_CHECK.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritPktCommonFilterOnCrc(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the PCKT_FLT_OPTIONS register value */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);

  /* Modify the register value: enable or disable the CRC filtering */
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= PCKT_FLT_OPTIONS_CRC_CHECK_MASK;
  }
  else
  {
    tempRegValue &= ~PCKT_FLT_OPTIONS_CRC_CHECK_MASK;
  }

  /* Writes the PCKT_FLT_OPTIONS register value */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the CRC filtering enable bit.
 * @param  None.
 * @retval SpiritFunctionalState CRC filtering.
 */
SpiritFunctionalState SpiritPktCommonGetFilterOnCrc(void)
{
  uint8_t tempRegValue;


  /* Reads the PCKT_FLT_OPTIONS register value */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);

  /* Check the CRC filtering bit */
  if(tempRegValue & PCKT_FLT_OPTIONS_CRC_CHECK_MASK)
  {
    return S_ENABLE;
  }
  else
  {
    return S_DISABLE;
  }

}


/**
 * @brief  Sets the CRC mode for SPIRIT packets.
 * @param  xCrcMode length of CRC field in bytes.
 *         This parameter can be any value of @ref PktCrcMode.
 * @retval None.
 */
void SpiritPktCommonSetCrcMode(PktCrcMode xCrcMode)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_PKT_CRC_MODE(xCrcMode));

  /* Reads the PCKTCTRL1 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);

  /* Build data to write setting the CRC mode */
  tempRegValue &= ~PCKTCTRL1_CRC_MODE_MASK;
  tempRegValue |= (uint8_t)xCrcMode;

  /* Writes the new value on the PCKTCTRL1 register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the CRC mode for SPIRIT packets.
 * @param  None.
 * @retval PktCrcMode Crc mode.
 */
PktCrcMode SpiritPktCommonGetCrcMode(void)
{
  uint8_t tempRegValue;

  /* Reads the PCKTCTRL1 register */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);

  /* Rebuild and return value */
  return (PktCrcMode)(tempRegValue & 0xE0);

}


/**
 * @brief  Enables or Disables WHITENING for SPIRIT packets.
 * @param  xNewState new state for WHITENING mode.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritPktCommonWhitening(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the PCKTCTRL1 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);

  /* Build data to write: set or reset the whitening enable bit */
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= PCKTCTRL1_WHIT_MASK;
  }
  else
  {
    tempRegValue &= ~PCKTCTRL1_WHIT_MASK;
  }

  /* Writes the new value on the PCKTCTRL1 register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);

}


/**
 * @brief  Enables or Disables FEC for SPIRIT packets.
 * @param  xNewState new state for FEC mode.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritPktCommonFec(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the PCKTCTRL1 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);

  /* Build data to write: set or reset the FEC enable bit */
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= PCKTCTRL1_FEC_MASK;
  }
  else
  {
    tempRegValue &= ~PCKTCTRL1_FEC_MASK;
  }

  /* Writes data on the PCKTCTRL1 register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);

}


/**
 * @brief  Sets a specific SYNC word for SPIRIT packets.
 * @param  xSyncX SYNC word number to be set.
 *         This parameter can be any value of @ref PktSyncX.
 * @param  cSyncWord SYNC word.
 *         This parameter is an uint8_t.
 * @retval None.
 */
void SpiritPktCommonSetSyncxWord(PktSyncX xSyncX ,  uint8_t cSyncWord)
{
  uint8_t tempRegAddress=0;

  /* Check the parameters */
  s_assert_param(IS_PKT_SYNCx(xSyncX));

  /* Set the specified address */
  switch(xSyncX)
  {
    case PKT_SYNC_WORD_1:
      tempRegAddress=SYNC1_BASE;
      break;
    case PKT_SYNC_WORD_2:
      tempRegAddress=SYNC2_BASE;
      break;
    case PKT_SYNC_WORD_3:
      tempRegAddress=SYNC3_BASE;
      break;
    case PKT_SYNC_WORD_4:
      tempRegAddress=SYNC4_BASE;
      break;
  }

  /* Writes value on the selected register */
  g_xStatus = SpiritSpiWriteRegisters(tempRegAddress, 1, &cSyncWord);

}


/**
 * @brief  Returns a specific SYNC word for SPIRIT packets.
 * @param  xSyncX SYNC word number to be get.
 *         This parameter can be any value of @ref PktSyncX.
 * @retval uint8_t Sync word x.
 */
uint8_t SpiritPktCommonGetSyncxWord(PktSyncX xSyncX)
{
  uint8_t tempRegAddress=0, tempRegValue=0;

  /* Set the specified address */
  switch(xSyncX)
  {
    case PKT_SYNC_WORD_1:
      tempRegAddress=SYNC1_BASE;
      break;
    case PKT_SYNC_WORD_2:
      tempRegAddress=SYNC2_BASE;
      break;
    case PKT_SYNC_WORD_3:
      tempRegAddress=SYNC3_BASE;
      break;
    case PKT_SYNC_WORD_4:
      tempRegAddress=SYNC4_BASE;
      break;
  }

  /* Reads the selected register value */
  g_xStatus = SpiritSpiReadRegisters(tempRegAddress, 1, &tempRegValue);

  /* Returns the read value */
  return tempRegValue;

}


/**
 * @brief  Sets multiple SYNC words for SPIRIT packets.
 * @param  lSyncWords SYNC words to be set with format: 0x|SYNC1|SYNC2|SYNC3|SYNC4|.
 *         This parameter is a uint32_t.
 * @param  xSyncLength SYNC length in bytes. The 32bit word passed will be stored in the SYNCx registers from the MSb
 *         until the number of bytes in xSyncLength has been stored.
 *         This parameter is a @ref PktSyncLength.
 * @retval None.
 */
void SpiritPktCommonSetSyncWords(uint32_t lSyncWords, PktSyncLength xSyncLength)
{
  uint8_t tempRegValue[4];

  /* Split the 32-bit value in 4 8-bit values */
  for(uint8_t i=0 ; i<4 ; i++)
  {
    //if(i< ((3-xSyncLength) >>1) ) /* This may lead to negative value */
    if(i< (3-(xSyncLength>>1)) )
    {
      tempRegValue[i]=0;
    }
    else
    {
      tempRegValue[i]=(uint8_t)(lSyncWords>>(8*i));
    }
  }

  /* Writes SYNC value on the SYNCx registers */
  g_xStatus = SpiritSpiWriteRegisters(SYNC4_BASE, 4, tempRegValue);

}


/**
 * @brief  Returns multiple SYNC words for SPIRIT packets.
 * @param  xSyncLength SYNC length in bytes. The 32bit word passed will be stored in the SYNCx registers from the MSb
 *         until the number of bytes in xSyncLength has been stored.
 *         This parameter is a pointer to @ref PktSyncLength.
 * @retval uint32_t Sync words. The format of the read 32 bit word is 0x|SYNC1|SYNC2|SYNC3|SYNC4|.
 */
uint32_t SpiritPktCommonGetSyncWords(PktSyncLength xSyncLength)
{
  uint8_t tempRegValue[4];
  uint32_t tempRetValue=0;

  /* Reads the SYNCx registers value */
  g_xStatus = SpiritSpiReadRegisters(SYNC4_BASE, 4, tempRegValue);

  /* Rebuild the SYNC words */
  for(uint8_t i=0 ; i<4 ; i++)
  {
    if(i>2-(xSyncLength >>1))
    {
      tempRetValue |= tempRegValue[i]<<(8*i);
    }
  }

  /* Return SYNC words */
  return tempRetValue;

}


/**
 * @brief  Returns the variable length width (in number of bits).
 * @param  None.
 * @retval uint8_t Variable length width in bits.
 */
uint8_t SpiritPktCommonGetVarLengthWidth(void)
{
  uint8_t tempRegValue;

  /* Reads the PCKTCTRL3 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);

  /* Rebuild and return value */
  return (tempRegValue & PCKTCTRL3_LEN_WID_MASK)+1;

}


/**
 * @brief  Sets the destination address for the Tx packet.
 * @param  cAddress Destination address.
 *         This parameter is an uint8_t.
 * @retval None.
 */
void SpiritPktCommonSetDestinationAddress(uint8_t cAddress)
{
  /* Writes value on PCKT_FLT_GOALS_SOURCE_ADDR register */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_GOALS_SOURCE_ADDR_BASE, 1, &cAddress);

}


/**
 * @brief  Returns the settled destination address.
 * @param  None.
 * @retval uint8_t Transmitted destination address.
 */
uint8_t SpiritPktCommonGetTransmittedDestAddress(void)
{
  uint8_t tempRegValue;

  /* Reads value on the PCKT_FLT_GOALS_SOURCE_ADDR register */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_GOALS_SOURCE_ADDR_BASE, 1, &tempRegValue);

  /* Return value */
  return tempRegValue;

}


/**
 * @brief  Sets the node my address. When the filtering on my address is on, if the destination address extracted from the received packet is equal to the content of the
 *         my address, then the packet is accepted (this is the address of the node).
 * @param  cAddress Address of the present node.
 *         This parameter is an uint8_t.
 * @retval None.
 */
void SpiritPktCommonSetMyAddress(uint8_t cAddress)
{
  /* Writes value on the PCKT_FLT_GOALS_TX_ADDR register */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_GOALS_TX_ADDR_BASE, 1, &cAddress);

}


/**
 * @brief  Returns the address of the present node.
 * @param  None.
 * @retval uint8_t My address (address of this node).
 */
uint8_t SpiritPktCommonGetMyAddress(void)
{
  uint8_t tempRegValue;

  /* Reads value on the PCKT_FLT_GOALS_TX_ADDR register */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_GOALS_TX_ADDR_BASE, 1, &tempRegValue);

  /* Return value */
  return tempRegValue;

}


/**
 * @brief  Sets the broadcast address. If the destination address extracted from the received packet is equal to the content of the
 *         BROADCAST_ADDR register, then the packet is accepted.
 * @param  cAddress Broadcast address.
 *         This parameter is an uint8_t.
 * @retval None.
 */
void SpiritPktCommonSetBroadcastAddress(uint8_t cAddress)
{
  /* Writes value on the PCKT_FLT_GOALS_BROADCAST register */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_GOALS_BROADCAST_BASE, 1, &cAddress);

}


/**
 * @brief  Returns the broadcast address.
 * @param  None.
 * @retval uint8_t Broadcast address.
 */
uint8_t SpiritPktCommonGetBroadcastAddress(void)
{
  uint8_t tempRegValue;

  /* Reads value on the PCKT_FLT_GOALS_BROADCAST register */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_GOALS_BROADCAST_BASE, 1, &tempRegValue);

  /* Return value */
  return tempRegValue;

}


/**
 * @brief  Sets the multicast address. When the multicast filtering is on, if the destination address extracted from the received packet is equal to the content of the
 *         MULTICAST_ADDR register, then the packet is accepted.
 * @param  cAddress Multicast address.
 *         This parameter is an uint8_t.
 * @retval None.
 */
void SpiritPktCommonSetMulticastAddress(uint8_t cAddress)
{
  /* Writes value on the PCKT_FLT_GOALS_MULTICAST register */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_GOALS_MULTICAST_BASE, 1, &cAddress);

}


/**
 * @brief  Returns the multicast address.
 * @param  None.
 * @retval uint8_t Multicast address.
 */
uint8_t SpiritPktCommonGetMulticastAddress(void)
{
  uint8_t tempRegValue;

  /* Reads value on the PCKT_FLT_GOALS_MULTICAST register */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_GOALS_MULTICAST_BASE, 1, &tempRegValue);

  /* Return value */
  return tempRegValue;

}


/**
 * @brief  Sets the control mask. The 1 bits of the CONTROL_MASK indicate the
 *         bits to be used in filtering. (All 0s no filtering)
 * @param  lMask Control mask.
 *         This parameter is an uint32_t.
 * @retval None.
 */
void SpiritPktCommonSetCtrlMask(uint32_t lMask)
{
  uint8_t tempRegValue[4];

  /* Split the 32-bit value in 4 8-bit values */
  tempRegValue[0] = (uint8_t) lMask;
  tempRegValue[1] = (uint8_t)(lMask >> 8);
  tempRegValue[2] = (uint8_t)(lMask >> 16);
  tempRegValue[3] = (uint8_t)(lMask >> 24);

  /* Writes values on the CKT_FLT_GOALS_CONTROLx_MASK registers */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_GOALS_CONTROL0_MASK_BASE, 4, tempRegValue);

}


/**
 * @brief  Returns the control mask. The 1 bits of the CONTROL_MASK indicate the
 *         bits to be used in filtering. (All 0s no filtering)
 * @param  None.
 * @retval uint32_t Control mask.
 */
uint32_t SpiritPktCommonGetCtrlMask(void)
{
  uint8_t tempRegValue[4];
  uint32_t tempRetValue=0;

  /* Reads the PCKT_FLT_GOALS_CONTROLx_MASK registers */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_GOALS_CONTROL0_MASK_BASE, 4, tempRegValue);

  /* Rebuild the control mask value on a 32-bit integer variable */
  for(uint8_t i=0 ; i<4 ; i++)
  {
    tempRetValue |= ((uint32_t)tempRegValue[i])<<(8*i);
  }

  /* Return value */
  return tempRetValue;
}

/**
 * @brief  Sets the control field reference. If the bits enabled by the CONTROL_MASK
 *         match the ones of the control fields extracted from the received packet
 *         then the packet is accepted.
 * @param  lReference Control reference.
 *         This parameter is an uint32_t.
 * @retval None.
 */
void SpiritPktCommonSetCtrlReference(uint32_t lReference)
{
  uint8_t tempRegValue[4];

  /* Split the 32-bit value in 4 8-bit values */
  tempRegValue[0] = (uint8_t) lReference;
  tempRegValue[1] = (uint8_t)(lReference >> 8);
  tempRegValue[2] = (uint8_t)(lReference >> 16);
  tempRegValue[3] = (uint8_t)(lReference >> 24);

  /* Writes values on the CKT_FLT_GOALS_CONTROLx_FIELD registers */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_GOALS_CONTROL0_FIELD_BASE, 4, tempRegValue);

}


/**
 * @brief  Returns the control field reference.
 * @param  None.
 * @retval uint32_t Control reference.
 */
uint32_t SpiritPktCommonGetCtrlReference(void)
{
  uint8_t tempRegValue[4];
  uint32_t tempRetValue=0;

  /* Reads the PCKT_FLT_GOALS_CONTROLx_FIELD registers */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_GOALS_CONTROL0_FIELD_BASE, 4, tempRegValue);

  /* Rebuild the control mask value on a 32-bit integer variable */
  for(uint8_t i=0 ; i<4 ; i++)
  {
    tempRetValue |= ((uint32_t)tempRegValue[i])<<(8*i);
  }

  /* Return value */
  return tempRetValue;
}


/**
 * @brief  Sets the TX control field.
 * @param  lField Tx contro field.
 *         This parameter is an uint32_t.
 * @retval None.
 */
void SpiritPktCommonSetTransmittedCtrlField(uint32_t lField)
{
  uint8_t tempRegValue[4];

  /* Split the 32-bit value in 4 8-bit values */
  tempRegValue[3] = (uint8_t) lField;
  tempRegValue[2] = (uint8_t)(lField >> 8);
  tempRegValue[1] = (uint8_t)(lField >> 16);
  tempRegValue[0] = (uint8_t)(lField >> 24);

  /* Writes value on the TX_CTRL_FIELDx register */
  g_xStatus = SpiritSpiWriteRegisters(TX_CTRL_FIELD3_BASE, 4, tempRegValue);

}


/**
 * @brief  Returns the Tx control field.
 * @param  None.
 * @retval uint32_t Control field of the transmitted packet.
 */
uint32_t SpiritPktCommonGetTransmittedCtrlField(void)
{
  uint8_t tempRegValue[4];
  uint32_t tempRetValue=0;

  /* Reads the TX_CTRL_FIELDx registers */
  g_xStatus = SpiritSpiReadRegisters(TX_CTRL_FIELD3_BASE, 4, tempRegValue);

  /* Rebuild value: build a 32-bit value from the read bytes */
  for(uint8_t i=0 ; i<4 ; i++)
  {
    tempRetValue |= ((uint32_t)tempRegValue[i])<<(8*(3-i));
  }

  /* Return value */
  return tempRetValue;

}


/**
 * @brief  If enabled RX packet is accepted if its destination address matches with My address.
 * @param  xNewState new state for DEST_VS_SOURCE_ADDRESS.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritPktCommonFilterOnMyAddress(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

   /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));


  /* Modify the register value: set or reset the TX source address control */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);

  /* Set or reset the DESTINATION vs TX enabling bit */
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= PCKT_FLT_OPTIONS_DEST_VS_TX_ADDR_MASK;
  }
  else
  {
    tempRegValue &= ~PCKT_FLT_OPTIONS_DEST_VS_TX_ADDR_MASK;
  }

  /* Writes the new value on the PCKT_FLT_OPTIONS register */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);

}


/**
 * @brief  If enabled RX packet is accepted if its destination address matches with multicast address.
 * @param  xNewState new state for DEST_VS_MULTICAST_ADDRESS.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritPktCommonFilterOnMulticastAddress(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the PCKT_FLT_OPTIONS register value */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);

  /* Enable or disable the filtering option */
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= PCKT_FLT_OPTIONS_DEST_VS_MULTICAST_ADDR_MASK;
  }
  else
  {
    tempRegValue &= ~PCKT_FLT_OPTIONS_DEST_VS_MULTICAST_ADDR_MASK;
  }

  /* Writes the new value on the PCKT_FLT_OPTIONS register */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);

}


/**
 * @brief  If enabled RX packet is accepted if its destination address matches with broadcast address.
 * @param  xNewState new state for DEST_VS_BROADCAST_ADDRESS.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritPktCommonFilterOnBroadcastAddress(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the register value */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);

  /* Enable or disable the filtering option */
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= PCKT_FLT_OPTIONS_DEST_VS_BROADCAST_ADDR_MASK;
  }
  else
  {
    tempRegValue &= ~PCKT_FLT_OPTIONS_DEST_VS_BROADCAST_ADDR_MASK;
  }

  /* Writes the new value on the PCKT_FLT_OPTIONS register */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the enable bit of the my address filtering.
 * @param  None.
 * @retval SpiritFunctionalState This parameter can be S_ENABLE or S_DISABLE.
 */
SpiritFunctionalState SpiritPktCommonGetFilterOnMyAddress(void)
{
  uint8_t tempRegValue;

  /* Reads the register value */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);

  /* Gets the enable/disable bit in form of SpiritFunctionalState type */
  if(tempRegValue & 0x08)
  {
    return S_ENABLE;
  }
  else
  {
    return S_DISABLE;
  }

}

/**
 * @brief  Returns the enable bit of the multicast address filtering.
 * @param  None.
 * @retval SpiritFunctionalState This parameter can be S_ENABLE or S_DISABLE.
 */
SpiritFunctionalState SpiritPktCommonGetFilterOnMulticastAddress(void)
{
  uint8_t tempRegValue;

  /* Reads the register value */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);

  /* Get the enable/disable bit in form of SpiritFunctionalState type */
  if(tempRegValue & 0x04)
  {
    return S_ENABLE;
  }
  else
  {
    return S_DISABLE;
  }

}

/**
 * @brief  Returns the enable bit of the broadcast address filtering.
 * @param  None.
 * @retval SpiritFunctionalState This parameter can be S_ENABLE or S_DISABLE.
 */
SpiritFunctionalState SpiritPktCommonGetFilterOnBroadcastAddress(void)
{
  uint8_t tempRegValue;

  /* Reads the register value */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);

  /* Get the enable/disable bit in form of SpiritFunctionalState type */
  if(tempRegValue & 0x02)
  {
    return S_ENABLE;
  }
  else
  {
    return S_DISABLE;
  }

}


/**
 * @brief  Returns the destination address of the received packet.
 * @param  None.
 * @retval uint8_t Destination address of the received address.
 */
uint8_t SpiritPktCommonGetReceivedDestAddress(void)
{
  uint8_t tempRegValue;

  /* Reads the RX_ADDR_FIELD0 register value */
  g_xStatus = SpiritSpiReadRegisters(RX_ADDR_FIELD0_BASE, 1, &tempRegValue);

  /* Return value */
  return tempRegValue;

}


/**
 * @brief  Returns the control field of the received packet.
 * @param  None.
 * @retval uint32_t Received control field.
 */
uint32_t SpiritPktCommonGetReceivedCtrlField(void)
{
  uint8_t tempRegValue[4];
  uint32_t tempRetValue=0;

  /* Reads the PCKT_FLT_GOALS_CONTROLx_MASK registers */
  g_xStatus = SpiritSpiReadRegisters(RX_CTRL_FIELD0_BASE, 4, tempRegValue);

  /* Rebuild the control mask value on a 32-bit integer variable */
  for(uint8_t i=0 ; i<4 ; i++)
  {
    tempRetValue |= ((uint32_t)tempRegValue[i])<<(8*i);
  }

  /* Returns value */
  return tempRetValue;
}


/**
 * @brief  Returns the CRC field of the received packet.
 * @param  cCrcFieldVect array in which the CRC field has to be stored.
 *         This parameter is an uint8_t array of 3 elements.
 * @retval None.
 */
void SpiritPktCommonGetReceivedCrcField(uint8_t* cCrcFieldVect)
{
  uint8_t tempRegValue[3],crcLength;
  PktCrcMode crcMode;
  
  /* Gets the CRC mode in PktCrcMode enum */
  crcMode=SpiritPktCommonGetCrcMode();
  
  /* Cast to uint8_t */
  crcLength = (uint8_t)crcMode;
  
  /* Obtains the real length: see the @ref PktCrcMode enumeration */
  crcLength >>= 5;
  if(crcLength>=3) crcLength--;
  
  /* Reads the CRC_FIELDx registers value */
  g_xStatus = SpiritSpiReadRegisters(CRC_FIELD2_BASE, 3,tempRegValue);
  
  /* Sets the array to be returned */
  for(uint8_t i=0 ; i<3 ; i++)
  {
    if(i<crcLength) 
    {
      cCrcFieldVect[i]=tempRegValue[2-i];
    }
    else 
    {
      cCrcFieldVect[i]=0;
    }
  }
  
}


/**
 * @brief  Sets the AUTO ACKNOLEDGEMENT mechanism on the receiver. When the feature is enabled and
 *         a data packet has been correctly received, then an acknowledgement packet is sent back to the originator of the received
 *         packet. If the PIGGYBACKING bit is also set, payload data will be read from the FIFO; otherwise an empty packet is sent
 *         only containing the source and destination addresses and the sequence number of the packet being acknowledged.
 * @param  xAutoAck new state for autoack.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @param  xPiggybacking new state for autoack.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritPktCommonAutoAck(SpiritFunctionalState xAutoAck , SpiritFunctionalState xPiggybacking)
{
  uint8_t tempRegValue[2];

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xAutoAck));
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xPiggybacking));
  /* Check if piggybacking is enabled and autoack is disabled */
  s_assert_param(!(xPiggybacking==S_ENABLE && xAutoAck==S_DISABLE));

  /* Reads the PROTOCOL[1:0] registers value */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 2, tempRegValue);

  /* Sets the specified LLP option */
  /* Autoack setting */
  if(xAutoAck == S_ENABLE)
  {
    tempRegValue[1] |= PROTOCOL0_AUTO_ACK_MASK;
  }
  else
  {
    tempRegValue[1] &= (~PROTOCOL0_AUTO_ACK_MASK);
  }

  /* Piggybacking setting */
  if(xPiggybacking == S_ENABLE)
  {
    tempRegValue[0] |= PROTOCOL1_PIGGYBACKING_MASK;
  }
  else
  {
    tempRegValue[0] &= (~PROTOCOL1_PIGGYBACKING_MASK);
  }

  /* Writes data on the PROTOCOL[1:0] registers */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL1_BASE, 2, tempRegValue);

}


/**
 * @brief  Sets the AUTO ACKNOLEDGEMENT mechanism on the transmitter. On the transmitter side, the NACK_TX field can be used to require or not an acknowledgment for each individual packet: if
 *         NACK_TX is set to "1" then acknowledgment will not be required; if NACK_TX is set to "0" then acknowledgment will be
 *         required.
 * @param  xNewState new state for TX_AUTOACK.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritPktCommonRequireAck(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads value on the PROTOCOL0 register */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL0_BASE, 1, &tempRegValue);

  /* Enables or disables the ack requirement option */
  if(xNewState == S_DISABLE)
  {
    tempRegValue |= PROTOCOL0_NACK_TX_MASK;
  }
  else
  {
    tempRegValue &= ~PROTOCOL0_NACK_TX_MASK;
  }

  /* Writes value on the PROTOCOL0 register */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL0_BASE, 1, &tempRegValue);

}


/**
 * @brief  Sets the TX sequence number to be used to start counting.
 * @param  cSeqNumberReload new value for Tx seq number reload.
 * @retval None.
 */
void SpiritPktCommonSetTransmittedSeqNumberReload(uint8_t cSeqNumberReload){
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_PKT_SEQ_NUMBER_RELOAD(cSeqNumberReload));

  /* Reads value on the PROTOCOL2 register */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL2_BASE, 1, &tempRegValue);

  tempRegValue &= 0xE7;
  tempRegValue |= (cSeqNumberReload << 3);

  /* Writes value on the PROTOCOL2 register */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL2_BASE, 1, &tempRegValue);

}


/**
 * @brief  Sets the TX sequence number to be used to start counting.
 * @param  cSeqNumberReload new value for Tx seq number reload.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritPktCommonSetNMaxReTx(PktNMaxReTx xNMaxReTx)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_PKT_NMAX_RETX(xNMaxReTx));

  /* Reads the PROTOCOL0 register value */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL0_BASE, 1, &tempRegValue);

  /* Build the value to be written */
  tempRegValue &= ~PROTOCOL0_NMAX_RETX_MASK;
  tempRegValue |= xNMaxReTx;

  /* Writes value on the PROTOCOL0 register */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL0_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the max number of automatic retransmission.
 * @param  None.
 * @retval uint8_t Max number of retransmissions.
 *         This parameter is an uint8_t.
 */
uint8_t SpiritPktCommonGetNMaxReTx(void)
{
  uint8_t tempRegValue;

  /* Reads the PROTOCOL0 register value */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL0_BASE, 1, &tempRegValue);

  /* Build the value to be written */
  return ((tempRegValue & PROTOCOL0_NMAX_RETX_MASK)>>4);

}

/**
 * @brief  Returns the TX ACK request
 * @param  None.
 * @retval uint8_t Max number of retransmissions.
 *         This parameter is an uint8_t.
 */
SpiritFunctionalState SpiritPktCommonGetTxAckRequest(void)
{
  uint8_t tempRegValue;

  /* Reads the PROTOCOL0 register value */
  g_xStatus = SpiritSpiReadRegisters(RX_PCKT_INFO_BASE, 1, &tempRegValue);

  /* Build the value to be written */
  return (SpiritFunctionalState)((tempRegValue & TX_PCKT_INFO_NACK_RX)>>2);

}
   
   
/**
 * @brief  Returns the source address of the received packet.
 * @param  None.
 * @retval uint8_t Source address of the received packet.
 */
uint8_t SpiritPktCommonGetReceivedSourceAddress(void)
{
  uint8_t tempRegValue;

  /* Reads the RX_ADDR_FIELD1 register value */
  g_xStatus = SpiritSpiReadRegisters(RX_ADDR_FIELD1_BASE, 1, &tempRegValue);

  /* Returns value */
  return tempRegValue;

}


/**
 * @brief  Returns the sequence number of the received packet.
 * @param  None.
 * @retval uint8_t Received Sequence number.
 */
uint8_t SpiritPktCommonGetReceivedSeqNumber(void)
{
  uint8_t tempRegValue;

  /* Reads the RX_PCKT_INFO register value */
  g_xStatus = SpiritSpiReadRegisters(RX_PCKT_INFO_BASE, 1, &tempRegValue);

  /* Obtains and returns the sequence number */
  return tempRegValue & 0x03;

}


/**
 * @brief  Returns the Nack bit of the received packet
 * @param  None.
 * @retval uint8_t Value of the Nack bit.
 */
uint8_t SpiritPktCommonGetReceivedNackRx(void)
{
  uint8_t tempRegValue;

  /* Reads the RX_PCKT_INFO register value */
  g_xStatus = SpiritSpiReadRegisters(RX_PCKT_INFO_BASE, 1, &tempRegValue);

  /* Obtains and returns the RX nack bit */
  return (tempRegValue >> 2) & 0x01;

}


/**
 * @brief  Returns the sequence number of the transmitted packet.
 * @param  None.
 * @retval uint8_t Sequence number of the transmitted packet.
 */
uint8_t SpiritPktCommonGetTransmittedSeqNumber(void)
{
  uint8_t tempRegValue;

  /* Reads the TX_PCKT_INFO register value */
  g_xStatus = SpiritSpiReadRegisters(TX_PCKT_INFO_BASE, 1, &tempRegValue);

  /* Obtains and returns the TX sequence number */
  return (tempRegValue >> 4) & 0x07;

}


/**
 * @brief  Returns the number of retransmission done on the transmitted packet.
 * @param  None.
 * @retval uint8_t Number of retransmissions done until now.
 */
uint8_t SpiritPktCommonGetNReTx(void)
{
  uint8_t tempRetValue;

  /* Reads the TX_PCKT_INFO register value */
  g_xStatus = SpiritSpiReadRegisters(TX_PCKT_INFO_BASE, 1, &tempRetValue);

  /* Obtains and returns the number of retransmission done */
  return (tempRetValue & 0x0F);

}


/**
 * @brief  If enabled RX packet is accepted only if the masked control field matches the
 *         masked control field reference (CONTROL_MASK & CONTROL_FIELD_REF == CONTROL_MASK & RX_CONTROL_FIELD).
 * @param  xNewState new state for Control filtering enable bit.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 * @note   This filtering control is enabled by default but the control mask is by default set to 0.
 *         As a matter of fact the user has to enable the control filtering bit after the packet initialization
 *         because the PktInit routine disables it.
 */
void SpiritPktCommonFilterOnControlField(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

   /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));


  /* Modify the register value: set or reset the control bit filtering */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);

  /* Set or reset the CONTROL filtering enabling bit */
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= PCKT_FLT_OPTIONS_CONTROL_FILTERING_MASK;
  }
  else
  {
    tempRegValue &= ~PCKT_FLT_OPTIONS_CONTROL_FILTERING_MASK;
  }

  /* Writes the new value on the PCKT_FLT_OPTIONS register */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the enable bit of the control field filtering.
 * @param  None.
 * @retval SpiritFunctionalState This parameter can be S_ENABLE or S_DISABLE.
 */
SpiritFunctionalState SpiritPktCommonGetFilterOnControlField(void)
{
  uint8_t tempRegValue;

  /* Reads the register value */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);

  /* Gets the enable/disable bit in form of SpiritFunctionalState type */
  if(tempRegValue & PCKT_FLT_OPTIONS_CONTROL_FILTERING_MASK)
  {
    return S_ENABLE;
  }
  else
  {
    return S_DISABLE;
  }

}

/**
 *@}
 */ 
 
/* --------------------------------   PKT MBUS  ----------------------------------- */ 

/**
 * @defgroup PktMbus_Private_Functions                  Pkt MBUS Private Functions
 * @{
 */

/**
 * @brief  Initializes the SPIRIT MBUS packet according to the specified parameters in the PktMbusInit struct.
 * @param  pxPktMbusInit pointer to a PktMbusInit structure that contains the configuration information for the specified SPIRIT MBUS PACKET FORMAT.
 *         This parameter is a pointer to @ref PktMbusInit.
 * @retval None.
 */
void SpiritPktMbusInit(PktMbusInit* pxPktMbusInit)
{
  uint8_t tempRegValue[3];

  /* Check the parameters */
  s_assert_param(IS_MBUS_SUBMODE(pxPktMbusInit->xMbusSubmode));

  /* Packet format config */
  SpiritPktMbusSetFormat();
  SpiritPktCommonFilterOnCrc(S_DISABLE);
  SpiritRadioCsBlanking(S_ENABLE);
  
  /* Preamble, postamble and submode config */
  tempRegValue[0] = pxPktMbusInit->cPreambleLength;
  tempRegValue[1] = pxPktMbusInit->cPostambleLength;
  tempRegValue[2] = (uint8_t) pxPktMbusInit->xMbusSubmode;

  /* Writes the new values on the MBUS_PRMBL registers */
  g_xStatus = SpiritSpiWriteRegisters(MBUS_PRMBL_BASE, 3, tempRegValue);

}

/**
 * @brief  Returns the SPIRIT MBUS packet structure according to the specified parameters in the registers.
 * @param  pxPktMbusInit MBUS packet init structure.
 *         This parameter is a pointer to @ref PktMbusInit.
 * @retval None.
 */
void SpiritPktMbusGetInfo(PktMbusInit* pxPktMbusInit)
{
  uint8_t tempRegValue[3];

  /* Reads the MBUS regs value */
  g_xStatus = SpiritSpiReadRegisters(MBUS_PRMBL_BASE, 3, tempRegValue);

  /* Fit the structure */
  pxPktMbusInit->cPreambleLength = tempRegValue[0];
  pxPktMbusInit->cPostambleLength = tempRegValue[1];
  pxPktMbusInit->xMbusSubmode = (MbusSubmode) (tempRegValue[2]&0x0E);

}


/**
 * @brief  Configures the MBUS packet format as the one used by SPIRIT.
 * @param  None.
 * @retval None.
 */
void SpiritPktMbusSetFormat(void)
{
  uint8_t tempRegValue;

  /* Reads the PCKTCTRL3 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);

  /* Sets format bits. Also set to 0 the direct RX mode bits */
  tempRegValue &= 0x0F;
  tempRegValue |= ((uint8_t)PCKTCTRL3_PCKT_FRMT_MBUS);

  /* Writes value on the PCKTCTRL3 register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);

  /* Reads the PCKTCTRL1 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);

  /* Build the new value. Set to 0 the direct TX mode bits */
  tempRegValue &= 0xF3;

  /* Writes the value on the PCKTCTRL1 register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);

  /* Reads the PROTOCOL1 register */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 1, &tempRegValue);

  /* Mask a reserved bit */
  tempRegValue &= ~0x20;

  /* Writes the value on the PROTOCOL1 register */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL1_BASE, 1, &tempRegValue);

}


/**
 * @brief  Sets how many chip sequence “01” shall be added in the preamble
 *         respect to the minimum value as defined according to the specified sub-mode.
 * @param  cPreamble the number of chip sequence.
 *         This parameter is an uint8_t.
 * @retval None.
 */
void SpiritPktMbusSetPreamble(uint8_t cPreamble)
{
  /* Modifies the MBUS_PRMBL register value */
  g_xStatus = SpiritSpiWriteRegisters(MBUS_PRMBL_BASE, 1, &cPreamble);

}


/**
 * @brief  Returns how many chip sequence "01" are added in the preamble
 *         respect to the minimum value as defined according to the specified sub-mode.
 * @param  None.
 * @retval uint8_t Preable in number of "01" chip sequences.
 */
uint8_t SpiritPktMbusGetPreamble(void)
{
  uint8_t tempRegValue;

  /* Modifies the MBUS_PRMBL register value */
  g_xStatus = SpiritSpiReadRegisters(MBUS_PRMBL_BASE, 1, &tempRegValue);

  /* Return value */
  return tempRegValue;

}


/**
 * @brief  Sets how many chip sequence “01” will be used in postamble
 * @param  cPostamble the number of chip sequence.
 *         This parameter is an uint8_t.
 * @retval None.
 */
void SpiritPktMbusSetPostamble(uint8_t cPostamble)
{
  /* Modifies the MBUS_PSTMBL register value */
  g_xStatus = SpiritSpiWriteRegisters(MBUS_PSTMBL_BASE, 1, &cPostamble);

}


/**
 * @brief  Returns how many chip sequence "01" are used in the postamble
 * @param  None.
 * @retval uint8_t Postamble in number of "01" chip sequences.
 */
uint8_t SpiritPktMbusGetPostamble(void)
{
  uint8_t tempRegValue;

  /* Reads the MBUS_PSTMBL register */
  g_xStatus = SpiritSpiReadRegisters(MBUS_PSTMBL_BASE, 1, &tempRegValue);

  /* Returns value */
  return tempRegValue;

}


/**
 * @brief  Sets the MBUS submode used.
 * @param  xMbusSubmode the submode used.
 *         This parameter can be any value of @ref MbusSubmode.
 * @retval None.
 */
void SpiritPktMbusSetSubmode(MbusSubmode xMbusSubmode)
{
  /* Modifies the MBUS_CTRL register value */
  g_xStatus = SpiritSpiWriteRegisters(MBUS_CTRL_BASE, 1, (uint8_t*)xMbusSubmode);

}


/**
 * @brief  Returns the MBUS submode used.
 * @param  None.
 * @retval MbusSubmode MBUS submode.
 */
MbusSubmode SpiritPktMbusGetSubmode(void)
{
  uint8_t tempRegValue;

  /* Reads the MBUS_CTRL register value */
  g_xStatus = SpiritSpiReadRegisters(MBUS_CTRL_BASE, 1, &tempRegValue);

  /* Returns value */
  return (MbusSubmode) tempRegValue;

}


/**
 * @brief  Sets the payload length for SPIRIT MBUS packets.
 * @param  nPayloadLength payload length in bytes.
 *         This parameter is an uint16_t.
 * @retval None.
 */
void SpiritPktMbusSetPayloadLength(uint16_t nPayloadLength)
{
  uint8_t tempRegValue[2];

  /* Computes PCKTLEN0 value from nPayloadLength */
  tempRegValue[1]=BUILD_PCKTLEN0(nPayloadLength);//(uint8_t)nPayloadLength;
  /* Computes PCKTLEN1 value from nPayloadLength */
  tempRegValue[0]=BUILD_PCKTLEN1(nPayloadLength);//(uint8_t)(nPayloadLength>>8);

  /* Writes data on the PCKTLEN1/0 register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTLEN1_BASE, 2, tempRegValue);

}


/**
 * @brief  Returns the payload length for SPIRIT MBUS packets.
 * @param  None.
 * @retval uint16_t Payload length in bytes.
 */
uint16_t SpiritPktMbusGetPayloadLength(void)
{
  uint8_t tempRegValue[2];

  /* Reads the packet length registers */
  g_xStatus = SpiritSpiReadRegisters(PCKTLEN1_BASE, 2, tempRegValue);

  /* Returns the packet length */
  return ((((uint16_t)tempRegValue[0])<<8) + (uint16_t) tempRegValue[1]);

}

/**
 *@}
 */
 
/* --------------------------------   PKT STACK  ----------------------------------- */ 

/**
 * @defgroup PktStack_Private_Functions                 Pkt STack Private Functions
 * @{
 */

/**
 * @brief  Initializes the SPIRIT STack packet according to the specified
 *         parameters in the PktStackInit.
 * @param  pxPktStackInit STack packet init structure.
 *         This parameter is a pointer to @ref PktStackInit.
 * @retval None.
 */
void SpiritPktStackInit(PktStackInit* pxPktStackInit)
{
  uint8_t tempRegValue[4], i;

  /* Check the parameters */
  s_assert_param(IS_STACK_PREAMBLE_LENGTH(pxPktStackInit->xPreambleLength));
  s_assert_param(IS_STACK_SYNC_LENGTH(pxPktStackInit->xSyncLength));
  s_assert_param(IS_STACK_CRC_MODE(pxPktStackInit->xCrcMode));
  s_assert_param(IS_STACK_LENGTH_WIDTH_BITS(pxPktStackInit->cPktLengthWidth));
  s_assert_param(IS_STACK_FIX_VAR_LENGTH(pxPktStackInit->xFixVarLength));
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(pxPktStackInit->xFec));
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(pxPktStackInit->xDataWhitening));
  s_assert_param(IS_STACK_CONTROL_LENGTH(pxPktStackInit->xControlLength));


  /* Reads the PROTOCOL1 register */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 1, &tempRegValue[0]);

  /* Mask a reserved bit */
  tempRegValue[0] &= ~0x20;

  /* Always (!) set the automatic packet filtering */
  tempRegValue[0] |= PROTOCOL1_AUTO_PCKT_FLT_MASK;

  /* Writes the value on register */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL1_BASE, 1, &tempRegValue[0]);

  /* Reads the PCKT_FLT_OPTIONS register */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue[0]);

  /* Always reset the control and source filtering */
  tempRegValue[0] &= ~(PCKT_FLT_OPTIONS_SOURCE_FILTERING_MASK | PCKT_FLT_OPTIONS_CONTROL_FILTERING_MASK);

  /* Writes the value on register */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue[0]);


  /* Address and control length setting: source and destination address are always present so ADDRESS_LENGTH=2 */
  tempRegValue[0] = 0x10 | ((uint8_t) pxPktStackInit->xControlLength);


  /* Packet format and width length setting */
  pxPktStackInit->cPktLengthWidth == 0 ? pxPktStackInit->cPktLengthWidth=1 : pxPktStackInit->cPktLengthWidth;
  tempRegValue[1] = ((uint8_t) PCKTCTRL3_PCKT_FRMT_STACK) | ((uint8_t)(pxPktStackInit->cPktLengthWidth-1));

  /* Preamble, sync and fixed or variable length setting */
  tempRegValue[2] = ((uint8_t) pxPktStackInit->xPreambleLength) | ((uint8_t) pxPktStackInit->xSyncLength) |
                    ((uint8_t) pxPktStackInit->xFixVarLength);

  /* CRC length, whitening and FEC setting */
  tempRegValue[3] = (uint8_t) pxPktStackInit->xCrcMode;

  if(pxPktStackInit->xDataWhitening == S_ENABLE)
  {
     tempRegValue[3] |= PCKTCTRL1_WHIT_MASK;
  }

  if(pxPktStackInit->xFec == S_ENABLE)
  {
     tempRegValue[3] |= PCKTCTRL1_FEC_MASK;
  }
  
  /* Writes registers */
  SpiritSpiWriteRegisters(PCKTCTRL4_BASE, 4, tempRegValue);

  /* Sync words setting */
  for(i=0;i<4;i++)
  {
    if(i<3-(pxPktStackInit->xSyncLength >>1))
    {
      tempRegValue[i]=0;
    }
    else
    {
      tempRegValue[i] = (uint8_t)(pxPktStackInit->lSyncWords>>(8*i));
    }
  }

  /* Enables or disables the CRC check */
  if(pxPktStackInit->xCrcMode == PKT_NO_CRC)
  {
    SpiritPktStackFilterOnCrc(S_DISABLE);
  }
  else
  {
    SpiritPktStackFilterOnCrc(S_ENABLE);
  }

  /* Writes registers */
  g_xStatus = SpiritSpiWriteRegisters(SYNC4_BASE, 4, tempRegValue);

}


/**
 * @brief  Returns the SPIRIT STack packet structure according to the specified parameters in the registers.
 * @param  pxPktStackInit STack packet init structure.
 *         This parameter is a pointer to @ref PktStackInit.
 * @retval None.
 */
void SpiritPktStackGetInfo(PktStackInit* pxPktStackInit)
{
  uint8_t tempRegValue[10];

  /* Reads registers */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL4_BASE, 10, tempRegValue);

  /* Length width */
  pxPktStackInit->cPktLengthWidth=(tempRegValue[1] & 0x0F)+1;

  /* Control length */
  pxPktStackInit->xControlLength=(StackControlLength)(tempRegValue[0] & 0x07);

  /* CRC mode */
  pxPktStackInit->xCrcMode=(StackCrcMode)(tempRegValue[3] & 0xE0);

  /* Whitening */
  pxPktStackInit->xDataWhitening=(SpiritFunctionalState)((tempRegValue[3] >> 4) & 0x01);

  /* FEC */
  pxPktStackInit->xFec=(SpiritFunctionalState)(tempRegValue[3] & 0x01);

  /* FIX or VAR bit */
  pxPktStackInit->xFixVarLength=(StackFixVarLength)(tempRegValue[2] & 0x01);

  /* Preamble length */
  pxPktStackInit->xPreambleLength=(StackPreambleLength)(tempRegValue[2] & 0xF8);

  /* Sync length */
  pxPktStackInit->xSyncLength=(StackSyncLength)(tempRegValue[2] & 0x06);

  /* sync Words */
  pxPktStackInit->lSyncWords=0;
  for(uint8_t i=0 ; i<4 ; i++)
  {
      if(i>2-(pxPktStackInit->xSyncLength >>1))
      {
        pxPktStackInit->lSyncWords |= tempRegValue[i+6]<<(8*i);
      }
  }

}


/**
 * @brief  Initializes the SPIRIT STack packet addresses according to the specified
 *         parameters in the PktStackAddresses struct.
 * @param  pxPktStackAddresses STack packet addresses init structure.
 *         This parameter is a pointer to @ref PktStackAddressesInit .
 * @retval None.
 */
void SpiritPktStackAddressesInit(PktStackAddressesInit* pxPktStackAddresses)
{
  uint8_t tempRegValue[3];

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(pxPktStackAddresses->xFilterOnMyAddress));
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(pxPktStackAddresses->xFilterOnMulticastAddress));
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(pxPktStackAddresses->xFilterOnBroadcastAddress));
  
  /* Reads the filtering options ragister */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue[0]);
  
  /* Enables or disables filtering on my address */
  if(pxPktStackAddresses->xFilterOnMyAddress == S_ENABLE)
  {
    tempRegValue[0] |= PCKT_FLT_OPTIONS_DEST_VS_TX_ADDR_MASK;
  }
  else
  {
    tempRegValue[0] &= ~PCKT_FLT_OPTIONS_DEST_VS_TX_ADDR_MASK;
  }
  
  /* Enables or disables filtering on multicast address */
  if(pxPktStackAddresses->xFilterOnMulticastAddress == S_ENABLE)
  {
    tempRegValue[0] |= PCKT_FLT_OPTIONS_DEST_VS_MULTICAST_ADDR_MASK;
  }
  else
  {
    tempRegValue[0] &= ~PCKT_FLT_OPTIONS_DEST_VS_MULTICAST_ADDR_MASK;
  }
  
  /* Enables or disables filtering on broadcast address */
  if(pxPktStackAddresses->xFilterOnBroadcastAddress == S_ENABLE)
  {
    tempRegValue[0] |= PCKT_FLT_OPTIONS_DEST_VS_BROADCAST_ADDR_MASK;
  }
  else
  {
    tempRegValue[0] &= ~PCKT_FLT_OPTIONS_DEST_VS_BROADCAST_ADDR_MASK;
  }
  
  /* Writes value on the register */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue[0]);
  
  /* Fills array with the addresses passed in the structure */
  tempRegValue[0] = pxPktStackAddresses->cBroadcastAddress;
  tempRegValue[1] = pxPktStackAddresses->cMulticastAddress;
  tempRegValue[2] = pxPktStackAddresses->cMyAddress;
  
  /* Writes them on the addresses registers */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_GOALS_BROADCAST_BASE, 3, tempRegValue);
  
}


/**
* @brief  Returns the SPIRIT STack packet addresses structure according to the specified
*         parameters in the registers.
* @param  pxPktStackAddresses STack packet addresses init structure.
*         This parameter is a pointer to @ref PktStackAddresses.
* @retval None.
*/
void SpiritPktStackGetAddressesInfo(PktStackAddressesInit* pxPktStackAddresses)
{
  uint8_t tempRegValue[3];
  
  /* Reads values on the PCKT_FLT_GOALS registers */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_GOALS_BROADCAST_BASE, 3, tempRegValue);
  
  /* Fit the structure with the read addresses */
  pxPktStackAddresses->cBroadcastAddress = tempRegValue[0];
  pxPktStackAddresses->cMulticastAddress = tempRegValue[1];
  pxPktStackAddresses->cMyAddress = tempRegValue[2];
  
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue[0]);
  
  /* Fit the structure with the read filtering bits */
  pxPktStackAddresses->xFilterOnBroadcastAddress = (SpiritFunctionalState)((tempRegValue[0] >> 1) & 0x01);
  pxPktStackAddresses->xFilterOnMulticastAddress = (SpiritFunctionalState)((tempRegValue[0] >> 2) & 0x01);
  pxPktStackAddresses->xFilterOnMyAddress = (SpiritFunctionalState)((tempRegValue[0] >> 3) & 0x01);
  
}


/**
* @brief  Initializes the SPIRIT STack packet LLP options according to the specified
*         parameters in the PktStackLlpInit struct.
* @param  pxPktStackLlpInit STack packet LLP init structure.
*         This parameter is a pointer to @ref PktStackLlpInit.
* @retval None.
*/
void SpiritPktStackLlpInit(PktStackLlpInit* pxPktStackLlpInit)
{
  uint8_t tempRegValue[2];

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(pxPktStackLlpInit->xPiggybacking));
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(pxPktStackLlpInit->xAutoAck));
  s_assert_param(IS_STACK_NMAX_RETX(pxPktStackLlpInit->xNMaxRetx));
  /* check if piggybacking is enabled and autoack is disabled */
  s_assert_param(!(pxPktStackLlpInit->xPiggybacking==S_ENABLE && pxPktStackLlpInit->xAutoAck==S_DISABLE));

  /* Piggybacking mechanism setting on the PROTOCOL1 register */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 2, tempRegValue);
  if(pxPktStackLlpInit->xPiggybacking == S_ENABLE)
  {
    tempRegValue[0] |= PROTOCOL1_PIGGYBACKING_MASK;
  }
  else
  {
    tempRegValue[0] &= ~PROTOCOL1_PIGGYBACKING_MASK;
  }

  /* RX and TX autoack mechanisms setting on the PROTOCOL0 register */
  if(pxPktStackLlpInit->xAutoAck == S_ENABLE)
  {
    tempRegValue[1] |= PROTOCOL0_AUTO_ACK_MASK;
  }
  else
  {
    tempRegValue[1] &= ~PROTOCOL0_AUTO_ACK_MASK;
  }

  /* Max number of retransmission setting */
  tempRegValue[1] &= ~PROTOCOL0_NMAX_RETX_MASK;
  tempRegValue[1] |= pxPktStackLlpInit->xNMaxRetx;

  /* Writes registers */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL1_BASE, 2, tempRegValue);

}


/**
 * @brief  Returns the SPIRIT STack packet LLP options according to the specified
 *         values in the registers.
 * @param  pxPktStackLlpInit STack packet LLP structure.
 *         This parameter is a pointer to @ref PktStackLlpInit.
 * @retval None.
 */
void SpiritPktStackLlpGetInfo(PktStackLlpInit* pxPktStackLlpInit)
{
  uint8_t tempRegValue[2];

  /* Piggybacking mechanism setting on the PROTOCOL1 register */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 2, tempRegValue);

  /* Fit the structure with the read values */
  pxPktStackLlpInit->xPiggybacking = (SpiritFunctionalState)((tempRegValue[0] >> 6) & 0x01);
  pxPktStackLlpInit->xAutoAck = (SpiritFunctionalState)((tempRegValue[1] >> 2) & 0x01);
  pxPktStackLlpInit->xNMaxRetx = (StackNMaxReTx)(tempRegValue[1] & PROTOCOL0_NMAX_RETX_MASK);

}


/**
 * @brief  Configures the STack packet format for SPIRIT.
 * @param  None.
 * @retval None.
 */
void SpiritPktStackSetFormat(void)
{
  uint8_t tempRegValue;

  /* Reads the PCKTCTRL3 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);

  /* Build value to be written. Also set to 0 the direct RX mode bits */
  tempRegValue &= 0x0F;
  tempRegValue |= ((uint8_t)PCKTCTRL3_PCKT_FRMT_STACK);

  /* Writes the value on the PCKTCTRL3 register. */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);

  /* Reads the PCKTCTRL1 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);

  /* Build the new value. Set to 0 the direct TX mode bits */
  tempRegValue &= 0xF3;

  /* Writes the PCKTCTRL1 value on register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);

  /* Reads the PROTOCOL1 register */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 1, &tempRegValue);

  /* Mask a reserved bit */
  tempRegValue &= ~0x20;

  /* Writes the value on the PROTOCOL1 register */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL1_BASE, 1, &tempRegValue);

}


/**
 * @brief  Sets the address length for SPIRIT STack packets (always 2).
 * @param  None.
 * @retval None.
 */
void SpiritPktStackSetAddressLength(void)
{
  uint8_t tempRegValue;

  /* Reads the PCKTCTRL4 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL4_BASE, 1, &tempRegValue);

  /* Build the new value */
  tempRegValue &= ~PCKTCTRL4_ADDRESS_LEN_MASK;
  tempRegValue |= ((uint8_t)0x10);

  /* Writes the value on the PCKTCTRL4 register */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL4_BASE, 1, &tempRegValue);

}


/**
 * @brief  Sets the payload length for SPIRIT STack packets. Since the packet length
 *         depends from the address (always 2 for this packet format)
 *         and the control field size, this function reads the control length register
 *         content in order to determine the correct packet length to be written.
 * @param  nPayloadLength payload length in bytes.
 *         This parameter can be any value of uint16_t.
 * @retval None.
 */
void SpiritPktStackSetPayloadLength(uint16_t nPayloadLength)
{
  uint8_t tempRegValue[2];

  /* Computes the oversize (address + control) size */
  uint16_t overSize = 2 + (uint16_t) SpiritPktStackGetControlLength();

  /* Computes PCKTLEN0 value from lPayloadLength */
  tempRegValue[1]=STACK_BUILD_PCKTLEN0(nPayloadLength+overSize);
  /* Computes PCKTLEN1 value from lPayloadLength */
  tempRegValue[0]=STACK_BUILD_PCKTLEN1(nPayloadLength+overSize);

  /* Writes the value on the PCKTLENx registers */
  g_xStatus = SpiritSpiWriteRegisters(PCKTLEN1_BASE, 2, tempRegValue);

}


/**
 * @brief  Returns the payload length for SPIRIT STack packets. Since the
 *         packet length depends from the address and the control
 *         field size, this function reads the correspondent
 *         registers in order to determine the correct payload length
 *         to be returned.
 * @param  None.
 * @retval uint16_t Payload length.
 */
uint16_t SpiritPktStackGetPayloadLength(void)
{
  uint8_t tempRegValue[2];
  /* Computes the oversize (address + control) size */
  uint16_t overSize = 2 + (uint16_t) SpiritPktStackGetControlLength();

  /* Reads the PCKTLEN1 registers value */
  g_xStatus = SpiritSpiReadRegisters(PCKTLEN1_BASE, 2, tempRegValue);

  /* Rebuild and return the payload length value */
  return ( ( (((uint16_t) tempRegValue[1])<<8) + ((uint16_t) tempRegValue[0]) ) - overSize );

}


/**
 * @brief  Computes and sets the variable payload length for SPIRIT STack packets.
 * @param  nMaxPayloadLength payload length in bytes.
 *         This parameter is an uint16_t.
 * @param  xControlLength control length in bytes.
 *         This parameter can be any value of @ref StackControlLength.
 * @retval None.
 */
void SpiritPktStackSetVarLengthWidth(uint16_t nMaxPayloadLength, StackControlLength xControlLength)
{
  uint8_t tempRegValue,
          i;
  uint32_t packetLength;


  /* packet length = payload length + address length (2) + control length */
  packetLength=nMaxPayloadLength+2+xControlLength;

  /* Computes the number of bits */
  for(i=0;i<16;i++)
  {
    if(packetLength == 0) 
    {
      break;
    }
    packetLength >>= 1;
  }
  i==0 ? i=1 : i;

  /* Reads the PCKTCTRL3 register value */
  g_xStatus = SpiritSpiReadRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);

  /* Build the register value */
  tempRegValue &= ~PCKTCTRL3_LEN_WID_MASK;
  tempRegValue |= ((uint8_t)(i-1));

  /* Writes the PCKTCTRL3 register value */
  g_xStatus = SpiritSpiWriteRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);

}


/**
 * @brief  Rx packet source mask. Used to mask the address of the accepted packets. If 0 -> no filtering.
 * @param  cMask Rx source mask.
 *         This parameter is an uint8_t.
 * @retval None.
 */
void SpiritPktStackSetRxSourceMask(uint8_t cMask)
{
  /* Writes value on the register PCKT_FLT_GOALS_SOURCE_MASK */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_GOALS_SOURCE_MASK_BASE, 1, &cMask);

}


/**
 * @brief  Returns the Rx packet source mask. Used to mask the address of the accepted packets. If 0 -> no filtering.
 * @param  None.
 * @retval uint8_t Rx source mask.
 */
uint8_t SpiritPktStackGetRxSourceMask(void)
{
  uint8_t tempRegValue;

  /* Writes value on the PCKT_FLT_GOALS_SOURCE_MASK register */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_GOALS_SOURCE_MASK_BASE, 1, &tempRegValue);

  /* Return the read value */
  return tempRegValue;

}

/**
 * @brief  Returns the packet length field of the received packet.
 * @param  None.
 * @retval uint16_t Packet length.
 */
uint16_t SpiritPktStackGetReceivedPktLength(void)
{
  uint8_t tempRegValue[2];
  uint16_t tempLength;
  
  /* Reads the RX_PCKT_LENx registers value */
  g_xStatus = SpiritSpiReadRegisters(RX_PCKT_LEN1_BASE, 2, tempRegValue);

  /* Rebuild and return the the length field */
  tempLength = ((((uint16_t) tempRegValue[0]) << 8) + (uint16_t) tempRegValue[1]);
  
  /* Computes the oversize (address + control) size */
  tempLength -= 2 + (uint16_t) SpiritPktStackGetControlLength();
  
  return tempLength;

}


/**
 * @brief  If enabled RX packet is accepted only if the masked source address field matches the
 *         masked source address field reference (SOURCE_MASK & SOURCE_FIELD_REF == SOURCE_MASK & RX_SOURCE_FIELD).
 * @param  xNewState new state for Source address filtering enable bit.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 * @note   This filtering control is enabled by default but the source address mask is by default set to 0.
 *         As a matter of fact the user has to enable the source filtering bit after the packet initialization
 *         because the PktInit routine disables it.
 */
void SpiritPktStackFilterOnSourceAddress(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

   /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));


  /* Modify the register value: set or reset the source bit filtering */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);

  /* Set or reset the SOURCE ADDRESS filtering enabling bit */
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= PCKT_FLT_OPTIONS_SOURCE_FILTERING_MASK;
  }
  else
  {
    tempRegValue &= ~PCKT_FLT_OPTIONS_SOURCE_FILTERING_MASK;
  }

  /* Writes the new value on the PCKT_FLT_OPTIONS register */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);

}

/**
 *@}
 */

 /* --------------------------------   QI  ----------------------------------- */ 

/**
 * @defgroup Qi_Private_Functions               QI Private Functions
 * @{
 */

/**
 * @brief  Enables/Disables the PQI Preamble Quality Indicator check. The running peak PQI is
 *         compared to a threshold value and the preamble valid IRQ is asserted as soon as the threshold is passed.
 * @param  xNewState new state for PQI check.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritQiPqiCheck(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the QI register value */
  g_xStatus = SpiritSpiReadRegisters(QI_BASE, 1, &tempRegValue);

  /* Enables or disables the PQI Check bit on the QI_BASE register */
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= QI_PQI_MASK;
  }
  else
  {
    tempRegValue &= ~QI_PQI_MASK;
  }

  /* Writes value on the QI register */
  g_xStatus = SpiritSpiWriteRegisters(QI_BASE, 1, &tempRegValue);

}


/**
 * @brief  Enables/Disables the Synchronization Quality Indicator check. The running peak SQI is
 *         compared to a threshold value and the sync valid IRQ is asserted as soon as the threshold is passed.
 * @param  xNewState new state for SQI check.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritQiSqiCheck(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the QI register value */
  g_xStatus = SpiritSpiReadRegisters(QI_BASE, 1, &tempRegValue);

  /* Enables or disables the SQI Check bit on the QI_BASE register */
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= QI_SQI_MASK;
  }
  else
  {
    tempRegValue &= ~QI_SQI_MASK;
  }

  /* Writes value on the QI register */
  g_xStatus = SpiritSpiWriteRegisters(QI_BASE, 1, &tempRegValue);

}


/**
 * @brief  Sets the PQI threshold. The preamble quality threshold is 4*PQI_TH (PQI_TH = 0..15).
 * @param  xPqiThr parameter of the formula above.
 *        This variable is a @ref PqiThreshold.
 * @retval None.
 */
void SpiritQiSetPqiThreshold(PqiThreshold xPqiThr)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_PQI_THR(xPqiThr));

  /* Reads the QI register value */
  g_xStatus = SpiritSpiReadRegisters(QI_BASE, 1, &tempRegValue);

  /* Build the PQI threshold value to be written */
  tempRegValue &= 0xC3;
  tempRegValue |= ((uint8_t)xPqiThr);

  /* Writes value on the QI register */
  g_xStatus = SpiritSpiWriteRegisters(QI_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the PQI threshold. The preamble quality threshold is 4*PQI_TH (PQI_TH = 0..15).
 * @param  None.
 * @retval PqiThreshold PQI threshold (PQI_TH of the formula above).
 */
PqiThreshold SpiritQiGetPqiThreshold(void)
{
  uint8_t tempRegValue;

  /* Reads the QI register value */
  g_xStatus = SpiritSpiReadRegisters(QI_BASE, 1, &tempRegValue);

  /* Rebuild and return the PQI threshold value */
  return (PqiThreshold)(tempRegValue & 0x3C);

}


/**
 * @brief  Sets the SQI threshold. The synchronization quality
 *         threshold is equal to 8 * SYNC_LEN - 2 * SQI_TH with SQI_TH = 0..3. When SQI_TH is 0 perfect match is required; when
 *         SQI_TH = 1, 2, 3 then 1, 2, or 3 bit errors are respectively accepted. It is recommended that the SQI check is always
 *         enabled.
 * @param  xSqiThr parameter of the formula above.
 *        This parameter is a @ref SqiThreshold.
 * @retval None.
 */
void SpiritQiSetSqiThreshold(SqiThreshold xSqiThr)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SQI_THR(xSqiThr));

  /* Reads the QI register value */
  g_xStatus = SpiritSpiReadRegisters(QI_BASE, 1, &tempRegValue);

  /* Build the SQI threshold value to be written */
  tempRegValue &= 0x3F;
  tempRegValue |= ((uint8_t)xSqiThr);

  /* Writes the new value on the QI register */
  g_xStatus = SpiritSpiWriteRegisters(QI_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the SQI threshold. The synchronization quality threshold is equal to 8 * SYNC_LEN - 2 * SQI_TH with SQI_TH = 0..3.
 * @param  None.
 * @retval SqiThreshold SQI threshold (SQI_TH of the formula above).
 */
SqiThreshold SpiritQiGetSqiThreshold(void)
{
  uint8_t tempRegValue;

  /* Reads the QI register value */
  g_xStatus = SpiritSpiReadRegisters(QI_BASE, 1, &tempRegValue);

  /* Rebuild and return the SQI threshold value */
  return (SqiThreshold)(tempRegValue & 0xC0);

}


/**
 * @brief  Returns the PQI value.
 * @param  None.
 * @retval uint8_t PQI value.
 */
uint8_t SpiritQiGetPqi(void)
{
  uint8_t tempRegValue;

  /* Reads the LINK_QUALIF2 register value */
  g_xStatus = SpiritSpiReadRegisters(LINK_QUALIF2_BASE, 1, &tempRegValue);

  /* Returns the PQI value */
  return tempRegValue;

}


/**
 * @brief  Returns the SQI value.
 * @param  None.
 * @retval uint8_t SQI value.
 */
uint8_t SpiritQiGetSqi(void)
{
  uint8_t tempRegValue;

  /* Reads the register LINK_QUALIF1 value */
  g_xStatus = SpiritSpiReadRegisters(LINK_QUALIF1_BASE, 1, &tempRegValue);

  /* Rebuild and return the SQI value */
  return (tempRegValue & 0x7F);

}


/**
 * @brief  Returns the LQI value.
 * @param  None.
 * @retval uint8_t LQI value.
 */
uint8_t SpiritQiGetLqi(void)
{
  uint8_t tempRegValue;

  /* Reads the LINK_QUALIF0 register value */
  g_xStatus = SpiritSpiReadRegisters(LINK_QUALIF0_BASE, 1, &tempRegValue);

  /* Rebuild and return the LQI value */
  return ((tempRegValue & 0xF0)>> 4);

}


/**
 * @brief  Returns the CS status.
 * @param  None.
 * @retval SpiritFlagStatus CS value (S_SET or S_RESET).
 */
SpiritFlagStatus SpiritQiGetCs(void)
{
  uint8_t tempRegValue;

  /* Reads the LINK_QUALIF1 register value */
  g_xStatus = SpiritSpiReadRegisters(LINK_QUALIF1_BASE, 1, &tempRegValue);

  /* Rebuild and returns the CS status value */
  if((tempRegValue & 0x80) == 0)
  {
    return S_RESET;
  }
  else
  {
    return S_SET;
  }

}


/**
 * @brief  Returns the RSSI value. The measured power is reported in steps of half a dB from 0 to 255 and is offset in such a way that -120 dBm corresponds
 *         to 20.
 * @param  None.
 * @retval uint8_t RSSI value.
 */
uint8_t SpiritQiGetRssi(void)
{
  uint8_t tempRegValue;

  /* Reads the RSSI_LEVEL register value */
  g_xStatus = SpiritSpiReadRegisters(RSSI_LEVEL_BASE, 1, &tempRegValue);

  /* Returns the RSSI value */
  return tempRegValue;

}


/**
 * @brief  Sets the RSSI threshold.
 * @param  cRssiThr RSSI threshold reported in steps of half a dBm with a -130 dBm offset.
 *         This parameter must be a uint8_t.
 * @retval None.
 */
void SpiritQiSetRssiThreshold(uint8_t cRssiThr)
{
  /* Writes the new value on the RSSI_TH register */
  g_xStatus = SpiritSpiWriteRegisters(RSSI_TH_BASE, 1, &cRssiThr);

}


/**
 * @brief  Returns the RSSI threshold.
 * @param  None.
 * @retval uint8_t RSSI threshold.
 */
uint8_t SpiritQiGetRssiThreshold(void)
{
  uint8_t tempRegValue;

  /* Reads the RSSI_TH register value */
  g_xStatus = SpiritSpiReadRegisters(RSSI_TH_BASE, 1, &tempRegValue);

  /* Returns RSSI threshold */
  return tempRegValue;

}


/**
 * @brief  Computes the RSSI threshold from its dBm value according to the formula: (RSSI[Dbm] + 130)/0.5
 * @param  nDbmValue RSSI threshold reported in dBm.
 *         This parameter must be a sint16_t.
 * @retval uint8_t RSSI threshold corresponding to dBm value.
 */
uint8_t SpiritQiComputeRssiThreshold(int32_t nDbmValue)
{
  /* Check the parameters */
  s_assert_param(IS_RSSI_THR_DBM(nDbmValue));

  /* Computes the RSSI threshold for register */
  return 2*(nDbmValue+130);

}

/**
 * @brief  Sets the RSSI threshold from its dBm value according to the formula: (RSSI[Dbm] + 130)/0.5.
 * @param  nDbmValue RSSI threshold reported in dBm.
 *         This parameter must be a sint32_t.
 * @retval None.
 */
void SpiritQiSetRssiThresholddBm(int32_t nDbmValue)
{
  uint8_t tempRegValue=2*(nDbmValue+130);

  /* Check the parameters */
  s_assert_param(IS_RSSI_THR_DBM(nDbmValue));

  /* Writes the new value on the RSSI_TH register */
  g_xStatus = SpiritSpiWriteRegisters(RSSI_TH_BASE, 1, &tempRegValue);

}

/**
 * @brief  Sets the RSSI filter gain. This parameter sets the bandwidth of a low pass IIR filter (RSSI_FLT register, allowed values 0..15), a
 *         lower values gives a faster settling of the measurements but lower precision. The recommended value for such parameter is 14.
 * @param  xRssiFg RSSI filter gain value.
 *         This parameter can be any value of @ref RssiFilterGain.
 * @retval None.
 */
void SpiritQiSetRssiFilterGain(RssiFilterGain xRssiFg)
{
  uint8_t tempRegValue;

   /* Check the parameters */
  s_assert_param(IS_RSSI_FILTER_GAIN(xRssiFg));

  /* Reads the RSSI_FLT register */
  g_xStatus = SpiritSpiReadRegisters(RSSI_FLT_BASE, 1, &tempRegValue);

  /* Sets the specified filter gain */
  tempRegValue &= 0x0F;
  tempRegValue |= ((uint8_t)xRssiFg);

  /* Writes the new value on the RSSI_FLT register */
  g_xStatus = SpiritSpiWriteRegisters(RSSI_FLT_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the RSSI filter gain.
 * @param  None.
 * @retval RssiFilterGain RSSI filter gain.
 */
RssiFilterGain SpiritQiGetRssiFilterGain(void)
{
  uint8_t tempRegValue;

  /* Reads the RSSI_FLT register */
  g_xStatus = SpiritSpiReadRegisters(RSSI_FLT_BASE, 1, &tempRegValue);

  /* Rebuild and returns the filter gain value */
  return (RssiFilterGain)(tempRegValue & 0xF0);

}


/**
 * @brief  Sets the CS Mode. When static carrier sensing is used (cs_mode = 0), the carrier sense signal is asserted when the measured RSSI is above the
 *         value specified in the RSSI_TH register and is deasserted when the RSSI falls 3 dB below the same threshold.
 *         When dynamic carrier sense is used (cs_mode = 1, 2, 3), the carrier sense signal is asserted if the signal is above the
 *         threshold and a fast power increase of 6, 12 or 18 dB is detected; it is deasserted if a power fall of the same amplitude is
 *         detected.
 * @param  xCsMode CS mode selector.
 *         This parameter can be any value of @ref CSMode.
 * @retval None.
 */
void SpiritQiSetCsMode(CSMode xCsMode)
{
  uint8_t tempRegValue;

   /* Check the parameters */
  s_assert_param(IS_CS_MODE(xCsMode));

  /* Reads the RSSI_FLT register */
  g_xStatus = SpiritSpiReadRegisters(RSSI_FLT_BASE, 1, &tempRegValue);

  /* Sets bit to select the CS mode */
  tempRegValue &= ~0x0C;
  tempRegValue |= ((uint8_t)xCsMode);

  /* Writes the new value on the RSSI_FLT register */
  g_xStatus = SpiritSpiWriteRegisters(RSSI_FLT_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the CS Mode.
 * @param  None.
 * @retval CSMode CS mode.
 */
CSMode SpiritQiGetCsMode(void)
{
  uint8_t tempRegValue;

  /* Reads the RSSI_FLT register */
  g_xStatus = SpiritSpiReadRegisters(RSSI_FLT_BASE, 1, &tempRegValue);

  /* Rebuild and returns the CS mode value */
  return (CSMode)(tempRegValue & 0x0C);

}

/**
 * @brief  Enables/Disables the CS Timeout Mask. If enabled CS value contributes to timeout disabling.
 * @param  xNewState new state for CS Timeout Mask.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritQiCsTimeoutMask(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the PROTOCOL2 register value */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL2_BASE, 1, &tempRegValue);

  /* Enables or disables the CS timeout mask */
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= PROTOCOL2_CS_TIMEOUT_MASK;
  }
  else
  {
    tempRegValue &= ~PROTOCOL2_CS_TIMEOUT_MASK;
  }

  /* Writes the new value on the PROTOCOL2 register */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL2_BASE, 1, &tempRegValue);

}


/**
 * @brief  Enables/Disables the PQI Timeout Mask. If enabled PQI value contributes to timeout disabling.
 * @param  xNewState new state for PQI Timeout Mask.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritQiPqiTimeoutMask(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the PROTOCOL2 register */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL2_BASE, 1, &tempRegValue);

  /* Enables or disables the PQI timeout mask */
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= PROTOCOL2_PQI_TIMEOUT_MASK;
  }
  else
  {
    tempRegValue &= ~PROTOCOL2_PQI_TIMEOUT_MASK;
  }

  /* Writes the new value on the PROTOCOL2 register */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL2_BASE, 1, &tempRegValue);

}


/**
 * @brief  Enables/Disables the SQI Timeout Mask. If enabled SQI value contributes to timeout disabling.
 * @param  xNewState new state for SQI Timeout Mask.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritQiSqiTimeoutMask(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));

  /* Reads the PROTOCOL2 register */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL2_BASE, 1, &tempRegValue);

  /* Enables or disables the SQI timeout mask */
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= PROTOCOL2_SQI_TIMEOUT_MASK;
  }
  else
  {
    tempRegValue &= ~PROTOCOL2_SQI_TIMEOUT_MASK;
  }

  /* Writes the new value on the PROTOCOL2 register */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL2_BASE, 1, &tempRegValue);

}

/**
 *@}
 */ 
 
/* --------------------------------   RADIO  ----------------------------------- */ 

/** @defgroup Radio_Private_Functions                    Radio Private Functions
* @{
*/

/**
* @brief  Initializes the SPIRIT analog and digital radio part according to the specified
*         parameters in the pxSRadioInitStruct.
* @param  pxSRadioInitStruct pointer to a SRadioInit structure that
*         contains the configuration information for the analog radio part of SPIRIT.
* @retval Error code: 0=no error, 1=error during calibration of VCO.
*/
uint8_t SpiritRadioInit(SRadioInit* pxSRadioInitStruct)
{
  int32_t FOffsetTmp;
  uint8_t anaRadioRegArray[8], digRadioRegArray[4];
  int16_t xtalOffsetFactor;
  uint8_t drM, drE, FdevM, FdevE, bwM, bwE;
    
  /* Workaround for Vtune */
  uint8_t value = 0xA0; SpiritSpiWriteRegisters(0x9F, 1, &value);
  
  /* Calculates the offset respect to RF frequency and according to xtal_ppm parameter: (xtal_ppm*FBase)/10^6 */
  FOffsetTmp = (int32_t)(((float)pxSRadioInitStruct->nXtalOffsetPpm*pxSRadioInitStruct->lFrequencyBase)/PPM_FACTOR);
  
  /* Check the parameters */
  s_assert_param(IS_FREQUENCY_BAND(pxSRadioInitStruct->lFrequencyBase));
  s_assert_param(IS_MODULATION_SELECTED(pxSRadioInitStruct->xModulationSelect));
  s_assert_param(IS_DATARATE(pxSRadioInitStruct->lDatarate));
  s_assert_param(IS_FREQUENCY_OFFSET(FOffsetTmp,s_lXtalFrequency));
  s_assert_param(IS_CHANNEL_SPACE(pxSRadioInitStruct->nChannelSpace,s_lXtalFrequency));
  s_assert_param(IS_F_DEV(pxSRadioInitStruct->lFreqDev,s_lXtalFrequency));
  
  /* Disable the digital, ADC, SMPS reference clock divider if fXO>24MHz or fXO<26MHz */
  SpiritSpiCommandStrobes(COMMAND_STANDBY);    
  do{
    /* Delay for state transition */
    for(volatile uint8_t i=0; i!=0xFF; i++);
    
    /* Reads the MC_STATUS register */
    SpiritRefreshStatus();
  }while(g_xStatus.MC_STATE!=MC_STATE_STANDBY);
  
  if(s_lXtalFrequency<DOUBLE_XTAL_THR)
  {
    SpiritRadioSetDigDiv(S_DISABLE);
    s_assert_param(IS_CH_BW(pxSRadioInitStruct->lBandwidth,s_lXtalFrequency));
  }
  else
  {      
    SpiritRadioSetDigDiv(S_ENABLE);
    s_assert_param(IS_CH_BW(pxSRadioInitStruct->lBandwidth,(s_lXtalFrequency>>1)));
  }
  
  /* Goes in READY state */
  SpiritSpiCommandStrobes(COMMAND_READY);
  do{
    /* Delay for state transition */
    for(volatile uint8_t i=0; i!=0xFF; i++);
    
    /* Reads the MC_STATUS register */
    SpiritRefreshStatus();
  }while(g_xStatus.MC_STATE!=MC_STATE_READY);
  
  /* Calculates the FC_OFFSET parameter and cast as signed int: FOffsetTmp = (Fxtal/2^18)*FC_OFFSET */
  xtalOffsetFactor = (int16_t)(((float)FOffsetTmp*FBASE_DIVIDER)/s_lXtalFrequency);
  anaRadioRegArray[2] = (uint8_t)((((uint16_t)xtalOffsetFactor)>>8)&0x0F);
  anaRadioRegArray[3] = (uint8_t)(xtalOffsetFactor);
  
  /* Calculates the channel space factor */
  anaRadioRegArray[0] =((uint32_t)pxSRadioInitStruct->nChannelSpace<<9)/(s_lXtalFrequency>>6)+1;
  
  SpiritManagementWaTRxFcMem(pxSRadioInitStruct->lFrequencyBase);
  
  /* 2nd order DEM algorithm enabling */
  uint8_t tmpreg; SpiritSpiReadRegisters(0xA3, 1, &tmpreg);
  tmpreg &= ~0x02; SpiritSpiWriteRegisters(0xA3, 1, &tmpreg);
  
  /* Check the channel center frequency is in one of the possible range */
  s_assert_param(IS_FREQUENCY_BAND((pxSRadioInitStruct->lFrequencyBase + ((xtalOffsetFactor*s_lXtalFrequency)/FBASE_DIVIDER) + pxSRadioInitStruct->nChannelSpace * pxSRadioInitStruct->cChannelNumber)));  
  
  /* Calculates the datarate mantissa and exponent */
  SpiritRadioSearchDatarateME(pxSRadioInitStruct->lDatarate, &drM, &drE);
  digRadioRegArray[0] = (uint8_t)(drM);
  digRadioRegArray[1] = (uint8_t)(0x00 | pxSRadioInitStruct->xModulationSelect |drE);
  
  /* Read the fdev register to preserve the clock recovery algo bit */
  SpiritSpiReadRegisters(0x1C, 1, &tmpreg);
  
  /* Calculates the frequency deviation mantissa and exponent */
  SpiritRadioSearchFreqDevME(pxSRadioInitStruct->lFreqDev, &FdevM, &FdevE);
  digRadioRegArray[2] = (uint8_t)((FdevE<<4) | (tmpreg&0x08) | FdevM);
  
  /* Calculates the channel filter mantissa and exponent */
  SpiritRadioSearchChannelBwME(pxSRadioInitStruct->lBandwidth, &bwM, &bwE);
  
  digRadioRegArray[3] = (uint8_t)((bwM<<4) | bwE);
 
  float if_off=(3.0f*480140)/(s_lXtalFrequency>>12)-64;  /* #1035-D */
  
  uint8_t ifOffsetAna = ROUND(if_off);
  
  if(s_lXtalFrequency<DOUBLE_XTAL_THR)
  {
    /* if offset digital is the same in case of single xtal */
    anaRadioRegArray[1] = ifOffsetAna;
  }
  else
  {
    if_off=(3.0f*480140)/(s_lXtalFrequency>>13)-64;      /* #1035-D */
    
    /* ... otherwise recompute it */
    anaRadioRegArray[1] = ROUND(if_off);
  }
  
  g_xStatus = SpiritSpiWriteRegisters(IF_OFFSET_ANA_BASE, 1, &ifOffsetAna);

  
  /* Sets Xtal configuration */
  if(s_lXtalFrequency>DOUBLE_XTAL_THR)
  {
    SpiritRadioSetXtalFlag(XTAL_FLAG((s_lXtalFrequency/2)));
  }
  else
  {
    SpiritRadioSetXtalFlag(XTAL_FLAG(s_lXtalFrequency));
  }
  
  /* Sets the channel number in the corresponding register */
  SpiritSpiWriteRegisters(CHNUM_BASE, 1, &pxSRadioInitStruct->cChannelNumber);
  
  /* Configures the Analog Radio registers */
  SpiritSpiWriteRegisters(CHSPACE_BASE, 4, anaRadioRegArray);
  
  /* Configures the Digital Radio registers */
  g_xStatus = SpiritSpiWriteRegisters(MOD1_BASE, 4, digRadioRegArray);
  
  /* Enable the freeze option of the AFC on the SYNC word */
  SpiritRadioAFCFreezeOnSync(S_ENABLE);
  
  /* Set the IQC correction optimal value */
  anaRadioRegArray[0]=0x80;
  anaRadioRegArray[1]=0xE3;
  g_xStatus = SpiritSpiWriteRegisters(0x99, 2, anaRadioRegArray);
  
  anaRadioRegArray[0]=0x22;
  SpiritSpiWriteRegisters(0xBC, 1, anaRadioRegArray);
  
  return SpiritRadioSetFrequencyBase(pxSRadioInitStruct->lFrequencyBase);
  
}


/**
* @brief  Returns the SPIRIT analog and digital radio structure according to the registers value.
* @param  pxSRadioInitStruct pointer to a SRadioInit structure that
*         contains the configuration information for the analog radio part of SPIRIT.
* @retval None.
*/
void SpiritRadioGetInfo(SRadioInit* pxSRadioInitStruct)
{
  uint8_t anaRadioRegArray[8], digRadioRegArray[4];
  BandSelect band;
  int16_t xtalOffsetFactor;
    
  /* Reads the Analog Radio registers */
  SpiritSpiReadRegisters(SYNT3_BASE, 8, anaRadioRegArray);
  
  /* Reads the Digital Radio registers */
  g_xStatus = SpiritSpiReadRegisters(MOD1_BASE, 4, digRadioRegArray);
  
  /* Reads the operating band masking the Band selected field */
  if((anaRadioRegArray[3] & 0x07) == SYNT0_BS_6)
  {
    band = HIGH_BAND;
  }
  else if ((anaRadioRegArray[3] & 0x07) == SYNT0_BS_12)
  {
    band = MIDDLE_BAND;
  }
  else if ((anaRadioRegArray[3] & 0x07) == SYNT0_BS_16)
  {
    band = LOW_BAND;
  }
  else if ((anaRadioRegArray[3] & 0x07) == SYNT0_BS_32)
  {
    band = VERY_LOW_BAND;
  }
  else
  {
    /* if it is another value, set it to a valid one in order to avoid access violation */
    uint8_t tmp=(anaRadioRegArray[3]&0xF8)|SYNT0_BS_6;
    SpiritSpiWriteRegisters(SYNT0_BASE,1,&tmp);
    band = HIGH_BAND;
  }
  
  /* Computes the synth word */
  uint32_t synthWord = (uint32_t)((((uint32_t)(anaRadioRegArray[0]&0x1F))<<21)+(((uint32_t)(anaRadioRegArray[1]))<<13)+\
    (((uint32_t)(anaRadioRegArray[2]))<<5)+(((uint32_t)(anaRadioRegArray[3]))>>3));
  
  /* Calculates the frequency base */
  uint8_t cRefDiv = (uint8_t)SpiritRadioGetRefDiv()+1;
  pxSRadioInitStruct->lFrequencyBase = (uint32_t)round(synthWord*(((double)s_lXtalFrequency)/(FBASE_DIVIDER*cRefDiv*s_vectcBHalfFactor[band])));
  
  /* Calculates the Offset Factor */
  uint16_t xtalOffTemp = ((((uint16_t)anaRadioRegArray[6])<<8)+((uint16_t)anaRadioRegArray[7]));
  
  /* If a negative number then convert the 12 bit 2-complement in a 16 bit number */
  if(xtalOffTemp & 0x0800)
  {
    xtalOffTemp = xtalOffTemp | 0xF000;
  }
  else
  {
    xtalOffTemp = xtalOffTemp & 0x0FFF;
  }
  
  xtalOffsetFactor = *((int16_t*)(&xtalOffTemp));
  
  /* Calculates the frequency offset in ppm */
  pxSRadioInitStruct->nXtalOffsetPpm =(int16_t)((uint32_t)xtalOffsetFactor*s_lXtalFrequency*PPM_FACTOR)/((uint32_t)FBASE_DIVIDER*pxSRadioInitStruct->lFrequencyBase);
  
  /* Channel space */
  pxSRadioInitStruct->nChannelSpace = anaRadioRegArray[4]*(s_lXtalFrequency>>15);
  
  /* Channel number */
  pxSRadioInitStruct->cChannelNumber = SpiritRadioGetChannel();
  
  /* Modulation select */
  pxSRadioInitStruct->xModulationSelect = (ModulationSelect)(digRadioRegArray[1] & 0x70);
  
  /* Reads the frequency deviation for mantissa and exponent */
  uint8_t FDevM = digRadioRegArray[2]&0x07;
  uint8_t FDevE = (digRadioRegArray[2]&0xF0)>>4;
  
  /* Reads the channel filter register for mantissa and exponent */
  uint8_t bwM = (digRadioRegArray[3]&0xF0)>>4;
  uint8_t bwE = digRadioRegArray[3]&0x0F;
  
  uint8_t cDivider = 0;
  cDivider = SpiritRadioGetDigDiv();
  
  /* Calculates the datarate */
  pxSRadioInitStruct->lDatarate = ((s_lXtalFrequency>>(5+cDivider))*(256+digRadioRegArray[0]))>>(23-(digRadioRegArray[1]&0x0F));
  
  /* Calculates the frequency deviation */
  pxSRadioInitStruct->lFreqDev =(uint32_t)((float)s_lXtalFrequency/(((uint32_t)1)<<18)*(uint32_t)((8.0+FDevM)/2*(1<<FDevE)));
  
  /* Reads the channel filter bandwidth from the look-up table and return it */
  pxSRadioInitStruct->lBandwidth = (uint32_t)(100.0*s_vectnBandwidth26M[bwM+(bwE*9)]*((s_lXtalFrequency>>cDivider)/26e6));
  
}


/**
* @brief  Sets the Xtal configuration in the ANA_FUNC_CONF0 register.
* @param  xXtal one of the possible value of the enum type XtalFrequency.
*         @arg XTAL_FLAG_24_MHz:  in case of 24 MHz crystal
*         @arg XTAL_FLAG_26_MHz:  in case of 26 MHz crystal
* @retval None.
*/
void SpiritRadioSetXtalFlag(XtalFlag xXtal)
{
  uint8_t tempRegValue = 0x00;
  
  /* Check the parameters */
  s_assert_param(IS_XTAL_FLAG(xXtal));
  
  /* Reads the ANA_FUNC_CONF_0 register */
  g_xStatus = SpiritSpiReadRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);
  if(xXtal == XTAL_FLAG_26_MHz)
  {
    tempRegValue|=SELECT_24_26_MHZ_MASK;
  }
  else
  {
    tempRegValue &= (~SELECT_24_26_MHZ_MASK);
  }
  
  /* Sets the 24_26MHz_SELECT field in the ANA_FUNC_CONF_0 register */
  g_xStatus = SpiritSpiWriteRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the Xtal configuration in the ANA_FUNC_CONF0 register.
* @param  None.
* @retval XtalFrequency Settled Xtal configuration.
*/
XtalFlag SpiritRadioGetXtalFlag(void)
{
  uint8_t tempRegValue;
  
  /* Reads the Xtal configuration in the ANA_FUNC_CONF_0 register and return the value */
  g_xStatus = SpiritSpiReadRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);
  
  return (XtalFlag)((tempRegValue & 0x40)>>6);
  
}


/**
* @brief  Returns the charge pump word for a given VCO frequency.
* @param  lFc channel center frequency expressed in Hz.
*         This parameter can be a value in one of the following ranges:<ul>
*         <li> High_Band: from 779 MHz to 915 MHz </li>
*         <li> Middle Band: from 387 MHz to 470 MHz </li>
*         <li> Low Band: from 300 MHz to 348 MHz </li>
*         <li> Very low Band: from 150 MHz to 174 MHz </li> </ul>
* @retval uint8_t Charge pump word.
*/
uint8_t SpiritRadioSearchWCP(uint32_t lFc)
{
  int8_t i=0;
  uint32_t vcofreq=0;
  uint8_t BFactor=0;
  
  /* Check the channel center frequency is in one of the possible range */
  s_assert_param(IS_FREQUENCY_BAND(lFc));
  
  /* Search the operating band */
  if(IS_FREQUENCY_BAND_HIGH(lFc))
  {
    BFactor = HIGH_BAND_FACTOR;
  }
  else if(IS_FREQUENCY_BAND_MIDDLE(lFc))
  {
    BFactor = MIDDLE_BAND_FACTOR;
  }
  else if(IS_FREQUENCY_BAND_LOW(lFc))
  {
    BFactor = LOW_BAND_FACTOR;
  }
  else if(IS_FREQUENCY_BAND_VERY_LOW(lFc))
  {
    BFactor = VERY_LOW_BAND_FACTOR;
  }
  
  /* Calculates the VCO frequency VCOFreq = lFc*B */
  vcofreq = lFc/1000*BFactor;
  
  /* Search in the vco frequency array the charge pump word */
  if(vcofreq>=((uint32_t)s_vectnVCOFreq[15])*1000)
  {
    i=15;
  }
  else
  {
    /* Search the value */
    for(i=0 ; i<15 && vcofreq>((uint32_t)s_vectnVCOFreq[i])*1000 ; i++);
    
    /* Be sure that it is the best approssimation */
    if (i!=0)
      i--;
  }
  
  /* Return index */
  return (i%8);
  
}

/**
* @brief  Returns the synth word.
* @param  None.
* @retval uint32_t Synth word.
*/
uint32_t SpiritRadioGetSynthWord(void)
{
  uint8_t regArray[4];
  
  /* Reads the SYNTH registers, build the synth word and return it */
  g_xStatus = SpiritSpiReadRegisters(SYNT3_BASE, 4, regArray);
  return ((((uint32_t)(regArray[0]&0x1F))<<21)+(((uint32_t)(regArray[1]))<<13)+\
    (((uint32_t)(regArray[2]))<<5)+(((uint32_t)(regArray[3]))>>3));
  
}


/**
* @brief  Sets the SYNTH registers.
* @param  lSynthWord the synth word to write in the SYNTH[3:0] registers.
* @retval None.
*/
void SpiritRadioSetSynthWord(uint32_t lSynthWord)
{
  uint8_t tempArray[4];
  uint8_t tempRegValue;
  
  /* Reads the SYNT0 register */
  g_xStatus = SpiritSpiReadRegisters(SYNT0_BASE, 1, &tempRegValue);
  
  /* Mask the Band selected field */
  tempRegValue &= 0x07;
  
  /* Build the array for SYNTH registers */
  tempArray[0] = (uint8_t)((lSynthWord>>21)&(0x0000001F));
  tempArray[1] = (uint8_t)((lSynthWord>>13)&(0x000000FF));
  tempArray[2] = (uint8_t)((lSynthWord>>5)&(0x000000FF));
  tempArray[3] = (uint8_t)(((lSynthWord&0x0000001F)<<3)| tempRegValue);
  
  /* Writes the synth word in the SYNTH registers */
  g_xStatus = SpiritSpiWriteRegisters(SYNT3_BASE, 4, tempArray);
  
}


/**
* @brief  Sets the operating band.
* @param  xBand the band to set.
*         This parameter can be one of following parameters:
*         @arg  HIGH_BAND   High_Band selected: from 779 MHz to 915 MHz
*         @arg  MIDDLE_BAND: Middle Band selected: from 387 MHz to 470 MHz
*         @arg  LOW_BAND:  Low Band selected: from 300 MHz to 348 MHz
*         @arg  VERY_LOW_BAND:  Very low Band selected: from 150 MHz to 174 MHz
* @retval None.
*/
void SpiritRadioSetBand(BandSelect xBand)
{
  uint8_t tempRegValue;
  
  /* Check the parameters */
  s_assert_param(IS_BAND_SELECTED(xBand));
  
  /* Reads the SYNT0 register*/
  g_xStatus = SpiritSpiReadRegisters(SYNT0_BASE, 1, &tempRegValue);
  
  /* Mask the SYNTH[4;0] field and write the BS value */
  tempRegValue &= 0xF8;
  tempRegValue |= s_vectcBandRegValue[xBand];
  
  /* Configures the SYNT0 register setting the operating band */
  g_xStatus = SpiritSpiWriteRegisters(SYNT0_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the operating band.
* @param  None.
* @retval BandSelect Settled band.
*         This returned value can be one of the following parameters:
*         @arg  HIGH_BAND   High_Band selected: from 779 MHz to 915 MHz
*         @arg  MIDDLE_BAND: Middle Band selected: from 387 MHz to 470 MHz
*         @arg  LOW_BAND:  Low Band selected: from 300 MHz to 348 MHz
*         @arg  VERY_LOW_BAND:  Very low Band selected: from 150 MHz to 174 MHz
*/
BandSelect SpiritRadioGetBand(void)
{
  uint8_t tempRegValue;
  
  /* Reads the SYNT0 register */
  g_xStatus = SpiritSpiReadRegisters(SYNT0_BASE, 1, &tempRegValue);
  
  /* Mask the Band selected field */
  if((tempRegValue & 0x07) == SYNT0_BS_6)
  {
    return HIGH_BAND;
  }
  else if ((tempRegValue & 0x07) == SYNT0_BS_12)
  {
    return MIDDLE_BAND;
  }
  else if ((tempRegValue & 0x07) == SYNT0_BS_16)
  {
    return LOW_BAND;
  }
  else
  {
    return VERY_LOW_BAND;
  }
  
}


/**
* @brief  Sets the channel number.
* @param  cChannel the channel number.
* @retval None.
*/
void SpiritRadioSetChannel(uint8_t cChannel)
{
  /* Writes the CHNUM register */
  g_xStatus = SpiritSpiWriteRegisters(CHNUM_BASE, 1, &cChannel);
  
}


/**
* @brief  Returns the actual channel number.
* @param  None.
* @retval uint8_t Actual channel number.
*/
uint8_t SpiritRadioGetChannel(void)
{
  uint8_t tempRegValue;
  
  /* Reads the CHNUM register and return the value */
  g_xStatus = SpiritSpiReadRegisters(CHNUM_BASE, 1, &tempRegValue);
  
  return tempRegValue;
  
}


/**
* @brief  Sets the channel space factor in channel space register.
*         The channel spacing step is computed as F_Xo/32768.
* @param  fChannelSpace the channel space expressed in Hz.
* @retval None.
*/
void SpiritRadioSetChannelSpace(uint32_t fChannelSpace)
{
  uint8_t cChannelSpaceFactor;
  
  /* Round to the nearest integer */
  cChannelSpaceFactor = ((uint32_t)fChannelSpace*CHSPACE_DIVIDER)/s_lXtalFrequency;
  
  /* Write value into the register */
  g_xStatus = SpiritSpiWriteRegisters(CHSPACE_BASE, 1, &cChannelSpaceFactor);
  
}


/**
* @brief  Returns the channel space register.
* @param  None.
* @retval uint32_t Channel space. The channel space is: CS = channel_space_factor x XtalFrequency/2^15
*         where channel_space_factor is the CHSPACE register value.
*/
uint32_t SpiritRadioGetChannelSpace(void)
{
  uint8_t channelSpaceFactor;
  
  /* Reads the CHSPACE register, calculate the channel space and return it */
  g_xStatus = SpiritSpiReadRegisters(CHSPACE_BASE, 1, &channelSpaceFactor);
  
  /* Compute the Hertz value and return it */
  return ((channelSpaceFactor*s_lXtalFrequency)/CHSPACE_DIVIDER);
  
}


/**
* @brief  Sets the FC OFFSET register starting from xtal ppm value.
* @param  nXtalPpm the xtal offset expressed in ppm.
* @retval None.
*/
void SpiritRadioSetFrequencyOffsetPpm(int16_t nXtalPpm)
{
  uint8_t tempArray[2];
  int16_t xtalOffsetFactor;
  uint32_t synthWord, fBase;
  int32_t FOffsetTmp;
  BandSelect band;
  
  /* Reads the synth word */
  synthWord = SpiritRadioGetSynthWord();
  
  /* Reads the operating band */
  band = SpiritRadioGetBand();
  
  /* Calculates the frequency base */
  uint8_t cRefDiv = (uint8_t)SpiritRadioGetRefDiv()+1;
  fBase = synthWord*(s_lXtalFrequency/(s_vectcBHalfFactor[band]*cRefDiv)/FBASE_DIVIDER);
  
  /* Calculates the offset respect to RF frequency and according to xtal_ppm parameter */
  FOffsetTmp = (int32_t)(((float)nXtalPpm*fBase)/PPM_FACTOR);
  
  /* Check the Offset is in the correct range */
  s_assert_param(IS_FREQUENCY_OFFSET(FOffsetTmp,s_lXtalFrequency));
  
  /* Calculates the FC_OFFSET value to write in the corresponding register */  
  xtalOffsetFactor = (int16_t)(((float)FOffsetTmp*FBASE_DIVIDER)/s_lXtalFrequency);
  
  /* Build the array related to the FC_OFFSET_1 and FC_OFFSET_0 register */
  tempArray[0]=(uint8_t)((((uint16_t)xtalOffsetFactor)>>8)&0x0F);
  tempArray[1]=(uint8_t)(xtalOffsetFactor);
  
  /* Writes the FC_OFFSET registers */
  g_xStatus = SpiritSpiWriteRegisters(FC_OFFSET1_BASE, 2, tempArray);
  
}


/**
* @brief  Sets the FC OFFSET register starting from frequency offset expressed in Hz.
* @param  lFOffset frequency offset expressed in Hz as signed word.
* @retval None.
*/
void SpiritRadioSetFrequencyOffset(int32_t lFOffset)
{
  uint8_t tempArray[2];
  int16_t offset;
  
  /* Check that the Offset is in the correct range */
  s_assert_param(IS_FREQUENCY_OFFSET(lFOffset,s_lXtalFrequency));
  
  /* Calculates the offset value to write in the FC_OFFSET register */
  offset = (int16_t)(((float)lFOffset*FBASE_DIVIDER)/s_lXtalFrequency);
  
  /* Build the array related to the FC_OFFSET_1 and FC_OFFSET_0 register */
  tempArray[0]=(uint8_t)((((uint16_t)offset)>>8)&0x0F);
  tempArray[1]=(uint8_t)(offset);
  
  /* Writes the FC_OFFSET registers */
  g_xStatus = SpiritSpiWriteRegisters(FC_OFFSET1_BASE, 2, tempArray);
  
}


/**
* @brief  Returns the actual frequency offset.
* @param  None.
* @retval int32_t Frequency offset expressed in Hz as signed word.
*/
int32_t SpiritRadioGetFrequencyOffset(void)
{
  uint8_t tempArray[2];
  int16_t xtalOffsetFactor;
  
  /* Reads the FC_OFFSET registers */
  g_xStatus = SpiritSpiReadRegisters(FC_OFFSET1_BASE, 2, tempArray);
  
  /* Calculates the Offset Factor */
  uint16_t xtalOffTemp = ((((uint16_t)tempArray[0])<<8)+((uint16_t)tempArray[1]));
  
  if(xtalOffTemp & 0x0800)
  {
    xtalOffTemp = xtalOffTemp | 0xF000;
  }
  else
  {
    xtalOffTemp = xtalOffTemp & 0x0FFF;
  }
  
  xtalOffsetFactor = *((int16_t*)(&xtalOffTemp));
  
  /* Calculates the frequency offset and return it */
  return ((int32_t)(xtalOffsetFactor*s_lXtalFrequency)/FBASE_DIVIDER);
  
}


/**
* @brief  Sets the Synth word and the Band Select register according to desired base carrier frequency.
*         In this API the Xtal configuration is read out from
*         the corresponding register. The user shall fix it before call this API.
* @param  lFBase the base carrier frequency expressed in Hz as unsigned word.
* @retval Error code: 0=no error, 1=error during calibration of VCO.
*/
uint8_t SpiritRadioSetFrequencyBase(uint32_t lFBase)
{
  uint32_t synthWord, Fc;
  uint8_t band=0, anaRadioRegArray[4], wcp;
  
  /* Check the parameter */
  s_assert_param(IS_FREQUENCY_BAND(lFBase));
  
  /* Search the operating band */
  if(IS_FREQUENCY_BAND_HIGH(lFBase))
  {
    band = HIGH_BAND;
  }
  else if(IS_FREQUENCY_BAND_MIDDLE(lFBase))
  {
    band = MIDDLE_BAND;
  }
  else if(IS_FREQUENCY_BAND_LOW(lFBase))
  {
    band = LOW_BAND;
  }
  else if(IS_FREQUENCY_BAND_VERY_LOW(lFBase))
  {
    band = VERY_LOW_BAND;
  }
  
  int32_t FOffset  = SpiritRadioGetFrequencyOffset();
  uint32_t lChannelSpace  = SpiritRadioGetChannelSpace();
  uint8_t cChannelNum = SpiritRadioGetChannel();
  
  /* Calculates the channel center frequency */
  Fc = lFBase + FOffset + lChannelSpace*cChannelNum;
  
  /* Reads the reference divider */
  uint8_t cRefDiv = (uint8_t)SpiritRadioGetRefDiv()+1;
  
  /* Selects the VCO */
  switch(band)
  {
  case VERY_LOW_BAND:
    if(Fc<161281250)
    {
      SpiritCalibrationSelectVco(VCO_L);
    }
    else
    {
      SpiritCalibrationSelectVco(VCO_H);
    }
    break;
    
  case LOW_BAND:
    if(Fc<322562500)
    {
      SpiritCalibrationSelectVco(VCO_L);
    }
    else
    {
      SpiritCalibrationSelectVco(VCO_H);
    }
    break;
    
  case MIDDLE_BAND:
    if(Fc<430083334)
    {
      SpiritCalibrationSelectVco(VCO_L);
    }
    else
    {
      SpiritCalibrationSelectVco(VCO_H);
    }
    break;
    
  case HIGH_BAND:
    if(Fc<860166667)
    {
      SpiritCalibrationSelectVco(VCO_L);
    }
    else
    {
      SpiritCalibrationSelectVco(VCO_H);
    }
  }
  
  /* Search the VCO charge pump word and set the corresponding register */
  wcp = SpiritRadioSearchWCP(Fc);
  
  synthWord = (uint32_t)(lFBase*s_vectcBHalfFactor[band]*(((double)(FBASE_DIVIDER*cRefDiv))/s_lXtalFrequency));
  
  /* Build the array of registers values for the analog part */
  anaRadioRegArray[0] = (uint8_t)(((synthWord>>21)&(0x0000001F))|(wcp<<5));
  anaRadioRegArray[1] = (uint8_t)((synthWord>>13)&(0x000000FF));
  anaRadioRegArray[2] = (uint8_t)((synthWord>>5)&(0x000000FF));
  anaRadioRegArray[3] = (uint8_t)(((synthWord&0x0000001F)<<3)| s_vectcBandRegValue[band]);
  
  /* Configures the needed Analog Radio registers */
  g_xStatus = SpiritSpiWriteRegisters(SYNT3_BASE, 4, anaRadioRegArray);
  
  if(xDoVcoCalibrationWA==S_ENABLE)
    return SpiritManagementWaVcoCalibration();
  
  return 0;
}

/**
* @brief  To say to the set frequency base if do or not the VCO calibration WA.
* @param  S_ENABLE or S_DISABLE the WA procedure.
* @retval None.
*/
void SpiritRadioVcoCalibrationWAFB(SpiritFunctionalState xNewstate)
{
  xDoVcoCalibrationWA=xNewstate;
}

/**
* @brief  Returns the base carrier frequency.
* @param  None.
* @retval uint32_t Base carrier frequency expressed in Hz as unsigned word.
*/
uint32_t SpiritRadioGetFrequencyBase(void)
{
  uint32_t synthWord;
  BandSelect band;
  
  /* Reads the synth word */
  synthWord = SpiritRadioGetSynthWord();
  
  /* Reads the operating band */
  band = SpiritRadioGetBand();
  
  uint8_t cRefDiv = (uint8_t)SpiritRadioGetRefDiv() + 1;
  
  /* Calculates the frequency base and return it */
  return (uint32_t)round(synthWord*(((double)s_lXtalFrequency)/(FBASE_DIVIDER*cRefDiv*s_vectcBHalfFactor[band])));
}


/**
* @brief  Returns the actual channel center frequency.
* @param  None.
* @retval uint32_t Actual channel center frequency expressed in Hz.
*/
uint32_t SpiritRadioGetCenterFrequency(void)
{
  int32_t offset;
  uint8_t channel;
  uint32_t fBase;
  uint32_t channelSpace;
  
  /* Reads the frequency base */
  fBase = SpiritRadioGetFrequencyBase();
  
  /* Reads the frequency offset */
  offset = SpiritRadioGetFrequencyOffset();
  
  /* Reads the channel space */
  channelSpace = SpiritRadioGetChannelSpace();
  
  /* Reads the channel number */
  channel = SpiritRadioGetChannel();
  
  /* Calculates the channel center frequency and return it */
  return (uint32_t)(fBase +  offset + (uint32_t)(channelSpace*channel));
  
}


/**
* @brief  Returns the mantissa and exponent, whose value used in the datarate formula
*         will give the datarate value closer to the given datarate.
* @param  fDatarate datarate expressed in bps. This parameter ranging between 100 and 500000.
* @param  pcM pointer to the returned mantissa value.
* @param  pcE pointer to the returned exponent value.
* @retval None.
*/
void SpiritRadioSearchDatarateME(uint32_t lDatarate, uint8_t* pcM, uint8_t* pcE)
{
  volatile SpiritBool find = S_FALSE;
  int8_t i=15;
  uint8_t cMantissaTmp;
  uint8_t cDivider = 0;
  
  /* Check the parameters */
  s_assert_param(IS_DATARATE(lDatarate));
  
  cDivider = (uint8_t)SpiritRadioGetDigDiv();
  
  /* Search in the datarate array the exponent value */
  while(!find && i>=0)
  {
    if(lDatarate>=(s_lXtalFrequency>>(20-i+cDivider)))
    {
      find = S_TRUE;
    }
    else
    {
      i--;
    }
  }
  i<0 ? i=0 : i;
  *pcE = i;
  
  /* Calculates the mantissa value according to the datarate formula */
  cMantissaTmp = (lDatarate*((uint32_t)1<<(23-i)))/(s_lXtalFrequency>>(5+cDivider))-256;
  
  /* Finds the mantissa value with less approximation */
  int16_t mantissaCalculation[3];
  for(uint8_t j=0;j<3;j++)
  {
    if((cMantissaTmp+j-1))
    {
      mantissaCalculation[j]=lDatarate-(((256+cMantissaTmp+j-1)*(s_lXtalFrequency>>(5+cDivider)))>>(23-i));
    }
    else
    {
      mantissaCalculation[j]=0x7FFF;
    }
  }
  uint16_t mantissaCalculationDelta = 0xFFFF;
  for(uint8_t j=0;j<3;j++)
  {
    if(S_ABS(mantissaCalculation[j])<mantissaCalculationDelta)
    {
      mantissaCalculationDelta = S_ABS(mantissaCalculation[j]);
      *pcM = cMantissaTmp+j-1;
    }
  }
  
}


/**
* @brief  Returns the mantissa and exponent for a given bandwidth.
*         Even if it is possible to pass as parameter any value in the below mentioned range,
*         the API will search the closer value according to a fixed table of channel
*         bandwidth values (@ref s_vectnBandwidth), as defined in the datasheet, returning the corresponding mantissa
*         and exponent value.
* @param  lBandwidth bandwidth expressed in Hz. This parameter ranging between 1100 and 800100.
* @param  pcM pointer to the returned mantissa value.
* @param  pcE pointer to the returned exponent value.
* @retval None.
*/
void SpiritRadioSearchChannelBwME(uint32_t lBandwidth, uint8_t* pcM, uint8_t* pcE)
{
  int8_t i, i_tmp;
  uint8_t cDivider = 1;
  
    /* Search in the channel filter bandwidth table the exponent value */
  if(SpiritRadioGetDigDiv())
  {
    cDivider = 2;
  }
  else
  {
    cDivider = 1;
  }
    
  s_assert_param(IS_CH_BW(lBandwidth,s_lXtalFrequency/cDivider));
  
  uint32_t lChfltFactor = (s_lXtalFrequency/cDivider)/100;
  
  for(i=0;i<90 && (lBandwidth<(uint32_t)((s_vectnBandwidth26M[i]*lChfltFactor)/2600));i++);
  
  if(i!=0)
  {
    /* Finds the mantissa value with less approximation */
    i_tmp=i;
    int16_t chfltCalculation[3];
    for(uint8_t j=0;j<3;j++) 
    {
      if(((i_tmp+j-1)>=0) || ((i_tmp+j-1)<=89))
      {
        chfltCalculation[j] = lBandwidth - (uint32_t)((s_vectnBandwidth26M[i_tmp+j-1]*lChfltFactor)/2600);
      }
      else
      {
        chfltCalculation[j] = 0x7FFF;
      }
    }
    uint16_t chfltDelta = 0xFFFF;
    
    for(uint8_t j=0;j<3;j++)
    {
      if(S_ABS(chfltCalculation[j])<chfltDelta)
      {
        chfltDelta = S_ABS(chfltCalculation[j]);
        i=i_tmp+j-1;
      }    
    }
  }
  (*pcE) = (uint8_t)(i/9);
  (*pcM) = (uint8_t)(i%9);
  
}

/**
* @brief  Returns the mantissa and exponent, whose value used in the frequency deviation formula
*         will give a frequency deviation value most closer to the given frequency deviation.
* @param  fFDev frequency deviation expressed in Hz. This parameter can be a value in the range [F_Xo*8/2^18, F_Xo*7680/2^18].
* @param  pcM pointer to the returned mantissa value.
* @param  pcE pointer to the returned exponent value.
* @retval None.
*/
void SpiritRadioSearchFreqDevME(uint32_t lFDev, uint8_t* pcM, uint8_t* pcE)
{
  uint8_t i;
  uint32_t a,bp,b=0;
  float xtalDivtmp=(float)s_lXtalFrequency/(((uint32_t)1)<<18);
  
  /* Check the parameters */
  s_assert_param(IS_F_DEV(lFDev,s_lXtalFrequency));
  
  for(i=0;i<10;i++)
  {
    a=(uint32_t)(xtalDivtmp*(uint32_t)(7.5*(1<<i)));
    if(lFDev<a)
      break;
  }
  (*pcE) = i;
  
  for(i=0;i<8;i++)
  {
    bp=b;
    b=(uint32_t)(xtalDivtmp*(uint32_t)((8.0+i)/2*(1<<(*pcE))));
    if(lFDev<b)
      break;
  }
  
  (*pcM)=i;
  if((lFDev-bp)<(b-lFDev))
    (*pcM)--;
  
}


/**
* @brief  Sets the datarate.
* @param  fDatarate datarate expressed in bps. This value shall be in the range
*         [100 500000].
* @retval None.
*/
void SpiritRadioSetDatarate(uint32_t lDatarate)
{
  uint8_t drE, tempRegValue[2];
  
  /* Check the parameters */
  s_assert_param(IS_DATARATE(lDatarate));
  
  /* Calculates the datarate mantissa and exponent */
  SpiritRadioSearchDatarateME(lDatarate, &tempRegValue[0], &drE);
  
  /* Reads the MOD_O register*/
  SpiritSpiReadRegisters(MOD0_BASE, 1, &tempRegValue[1]);
  
  /* Mask the other fields and set the datarate exponent */
  tempRegValue[1] &= 0xF0;
  tempRegValue[1] |= drE;
  
  /* Writes the Datarate registers */
  g_xStatus = SpiritSpiWriteRegisters(MOD1_BASE, 2, tempRegValue);
  
}


/**
* @brief  Returns the datarate.
* @param  None.
* @retval uint32_t Settled datarate expressed in bps.
*/
uint32_t SpiritRadioGetDatarate(void)
{
  uint8_t tempRegValue[2];
  uint8_t cDivider=0;
  
  /* Reads the datarate registers for mantissa and exponent */
  g_xStatus = SpiritSpiReadRegisters(MOD1_BASE, 2, tempRegValue);
  
  /* Calculates the datarate */
  cDivider = (uint8_t)SpiritRadioGetDigDiv(); 
  
  return (((s_lXtalFrequency>>(5+cDivider))*(256+tempRegValue[0]))>>(23-(tempRegValue[1]&0x0F)));
}


/**
* @brief  Sets the frequency deviation.
* @param  fFDev frequency deviation expressed in Hz. Be sure that this value
*         is in the correct range [F_Xo*8/2^18, F_Xo*7680/2^18] Hz.
* @retval None.
*/
void SpiritRadioSetFrequencyDev(uint32_t lFDev)
{
  uint8_t FDevM, FDevE, tempRegValue;
  
  /* Check the parameters */
  s_assert_param(IS_F_DEV(lFDev, s_lXtalFrequency));
  
  /* Calculates the frequency deviation mantissa and exponent */
  SpiritRadioSearchFreqDevME(lFDev, &FDevM, &FDevE);
  
  /* Reads the FDEV0 register */
  SpiritSpiReadRegisters(FDEV0_BASE, 1, &tempRegValue);
  
  /* Mask the other fields and set the frequency deviation mantissa and exponent */
  tempRegValue &= 0x08;
  tempRegValue |= ((FDevE<<4)|(FDevM));
  
  /* Writes the Frequency deviation register */
  g_xStatus = SpiritSpiWriteRegisters(FDEV0_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the frequency deviation.
* @param  None.
* @retval uint32_t Frequency deviation value expressed in Hz.
*         This value will be in the range [F_Xo*8/2^18, F_Xo*7680/2^18] Hz.
*/
uint32_t SpiritRadioGetFrequencyDev(void)
{
  uint8_t tempRegValue, FDevM, FDevE;  

  
  /* Reads the frequency deviation register for mantissa and exponent */
  g_xStatus = SpiritSpiReadRegisters(FDEV0_BASE, 1, &tempRegValue);
  FDevM = tempRegValue&0x07;
  FDevE = (tempRegValue&0xF0)>>4;
  
  /* Calculates the frequency deviation and return it */  
  return (uint32_t)((float)s_lXtalFrequency/(((uint32_t)1)<<18)*(uint32_t)((8.0+FDevM)/2*(1<<FDevE)));
   
}


/**
* @brief  Sets the channel filter bandwidth.
* @param  lBandwidth channel filter bandwidth expressed in Hz. This parameter shall be in the range [1100 800100]
*         Even if it is possible to pass as parameter any value in the above mentioned range,
*         the API will search the most closer value according to a fixed table of channel
*         bandwidth values (@ref s_vectnBandwidth), as defined in the datasheet. To verify the settled channel bandwidth
*         it is possible to use the SpiritRadioGetChannelBW() API.
* @retval None.
*/
void SpiritRadioSetChannelBW(uint32_t lBandwidth)
{
  uint8_t bwM, bwE, tempRegValue;
  
  /* Search in the channel filter bandwidth table the exponent value */
  if(SpiritRadioGetDigDiv())
  {
    s_assert_param(IS_CH_BW(lBandwidth,(s_lXtalFrequency/2)));
  }
  else
  {
    s_assert_param(IS_CH_BW(lBandwidth,(s_lXtalFrequency)));
  } 
  
  /* Calculates the channel bandwidth mantissa and exponent */
  SpiritRadioSearchChannelBwME(lBandwidth, &bwM, &bwE);
  tempRegValue = (bwM<<4)|(bwE);
  
  /* Writes the Channel filter register */
  g_xStatus = SpiritSpiWriteRegisters(CHFLT_BASE, 1, &tempRegValue);
  
}

/**
* @brief  Returns the channel filter bandwidth.
* @param  None.
* @retval uint32_t Channel filter bandwidth expressed in Hz.
*/
uint32_t SpiritRadioGetChannelBW(void)
{
  uint8_t tempRegValue, bwM, bwE;
  
  /* Reads the channel filter register for mantissa and exponent */
  g_xStatus = SpiritSpiReadRegisters(CHFLT_BASE, 1, &tempRegValue);
  bwM = (tempRegValue&0xF0)>>4;
  bwE = tempRegValue&0x0F;
  
  /* Reads the channel filter bandwidth from the look-up table and return it */
  return (uint32_t)(100.0*s_vectnBandwidth26M[bwM+(bwE*9)]*s_lXtalFrequency/26e6);
  
}


/**
* @brief  Sets the modulation type.
* @param  xModulation modulation to set.
*         This parameter shall be of type @ref ModulationSelect .
* @retval None.
*/
void SpiritRadioSetModulation(ModulationSelect xModulation)
{
  uint8_t tempRegValue;
  
  /* Check the parameters */
  s_assert_param(IS_MODULATION_SELECTED(xModulation));
  
  /* Reads the modulation register */
  SpiritSpiReadRegisters(MOD0_BASE, 1, &tempRegValue);
  
  /* Mask the other fields and set the modulation type */
  tempRegValue &=0x8F;
  tempRegValue |= xModulation;
  
  /* Writes the modulation register */
  g_xStatus = SpiritSpiWriteRegisters(MOD0_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the modulation type used.
* @param  None.
* @retval ModulationSelect Settled modulation type.
*/
ModulationSelect SpiritRadioGetModulation(void)
{
  uint8_t tempRegValue;
  
  /* Reads the modulation register MOD0*/
  g_xStatus = SpiritSpiReadRegisters(MOD0_BASE, 1, &tempRegValue);
  
  /* Return the modulation type */
  return (ModulationSelect)(tempRegValue&0x70);
  
}


/**
* @brief  Enables or Disables the Continuous Wave transmit mode.
* @param  xNewState new state for power ramping.
*         This parameter can be: S_ENABLE or S_DISABLE .
* @retval None.
*/
void SpiritRadioCWTransmitMode(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;
  
  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));
  
  /* Reads the modulation register MOD0 and mask the CW field */
  SpiritSpiReadRegisters(MOD0_BASE, 1, &tempRegValue);
  if(xNewState == S_ENABLE)
  {
    tempRegValue |=MOD0_CW;
  }
  else
  {
    tempRegValue &= (~MOD0_CW);
  }
  
  /* Writes the new value in the MOD0 register */
  g_xStatus = SpiritSpiWriteRegisters(MOD0_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Sets the OOK Peak Decay.
* @param  xOokDecay Peak decay control for OOK.
*         This parameter shall be of type @ref OokPeakDecay .
* @retval None.
*/
void SpiritRadioSetOokPeakDecay(OokPeakDecay xOokDecay)
{
  uint8_t tempRegValue;
  
  /* Check the parameters */
  s_assert_param(IS_OOK_PEAK_DECAY(xOokDecay));
  
  /* Reads the RSSI_FLT register */
  SpiritSpiReadRegisters(RSSI_FLT_BASE, 1, &tempRegValue);
  
  /* Mask the other fields and set OOK Peak Decay */
  tempRegValue &= 0xFC;
  tempRegValue |= xOokDecay;
  
  /* Writes the RSSI_FLT register to set the new OOK peak dacay value */
  g_xStatus = SpiritSpiWriteRegisters(RSSI_FLT_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the OOK Peak Decay.
* @param  None
* @retval OokPeakDecay Ook peak decay value.
*/
OokPeakDecay SpiritRadioGetOokPeakDecay(void)
{
  uint8_t tempRegValue;
  
  /* Reads the OOK peak decay register RSSI_FLT_BASE*/
  g_xStatus = SpiritSpiReadRegisters(RSSI_FLT_BASE, 1, &tempRegValue);
  
  /* Returns the OOK peak decay */
  return (OokPeakDecay) (tempRegValue & 0x03);
}

/**
* @brief  Returns the PA register value that corresponds to the passed dBm power.
* @param  lFbase Frequency base expressed in Hz.
* @param  fPowerdBm Desired power in dBm.
* @retval Register value as byte.
* @note The power interpolation curves used by this function have been extracted
*       by measurements done on the divisional evaluation boards.
*/
uint8_t SpiritRadioGetdBm2Reg(uint32_t lFBase, float fPowerdBm)
{
  uint8_t i=2;
  uint8_t j=0;
  float fReg;
  
  if(IS_FREQUENCY_BAND_HIGH(lFBase))
  {
    i=0;
    if(lFBase<900000000) i=1;// 868   
  }
  else if(IS_FREQUENCY_BAND_MIDDLE(lFBase))
  {
    i=2;
  }
  else if(IS_FREQUENCY_BAND_LOW(lFBase))
  {
    i=3;
  }
  else if(IS_FREQUENCY_BAND_VERY_LOW(lFBase))
  {
    i=4;
  }
  
  j=1;
  if(fPowerdBm>0 && 13.0f/fPowerFactors[i][2]-fPowerFactors[i][3]/fPowerFactors[i][2]<fPowerdBm) /* #1035-D */
      j=0;
  else if(fPowerdBm<=0 && 40.0f/fPowerFactors[i][2]-fPowerFactors[i][3]/fPowerFactors[i][2]>fPowerdBm) /* #1035-D */
      j=2;

  fReg=fPowerFactors[i][2*j]*fPowerdBm+fPowerFactors[i][2*j+1];
  
  if(fReg<1)
    fReg=1;
  else if(fReg>90) 
    fReg=90;
  
  return ((uint8_t)fReg);
}


/**
* @brief  Returns the dBm power that corresponds to the value of PA register.
* @param  lFbase Frequency base expressed in Hz.
* @param  cPowerReg Register value of the PA.
* @retval Power in dBm as float.
* @note The power interpolation curves used by this function have been extracted
*       by measurements done on the divisional evaluation boards.
*/
float SpiritRadioGetReg2dBm(uint32_t lFBase, uint8_t cPowerReg)
{
  uint8_t i=2;
  uint8_t j=0;
  float fPower;
  
  if(cPowerReg==0 || cPowerReg>90)
    return (-130.0);
  
  if(IS_FREQUENCY_BAND_HIGH(lFBase))
  {
    i=0;
    if(lFBase<900000000) i=1;// 868   
  }
  else if(IS_FREQUENCY_BAND_MIDDLE(lFBase))
  {
    i=2;
  }
  else if(IS_FREQUENCY_BAND_LOW(lFBase))
  {
    i=3;
  }
  else if(IS_FREQUENCY_BAND_VERY_LOW(lFBase))
  {
    i=4;
  }
  
  j=1;
  if(cPowerReg<13) j=0;
  else if(cPowerReg>40) j=2;
  
  fPower=(((float)cPowerReg)/fPowerFactors[i][2*j]-fPowerFactors[i][2*j+1]/fPowerFactors[i][2*j]);
  
  return fPower;
}

/**
* @brief  Configures the Power Amplifier Table and registers with value expressed in dBm.
* @param  cPALevelMaxIndex number of levels to set. This parameter shall be in the range [0:7].
* @param  cWidth step width expressed in terms of bit period units Tb/8.
*         This parameter shall be in the range [1:4].
* @param  xCLoad one of the possible value of the enum type PALoadCapacitor.
*         @arg LOAD_0_PF    No additional PA load capacitor
*         @arg LOAD_1_2_PF  1.2pF additional PA load capacitor
*         @arg LOAD_2_4_PF  2.4pF additional PA load capacitor
*         @arg LOAD_3_6_PF  3.6pF additional PA load capacitor
* @param  pfPAtabledBm pointer to an array of PA values in dbm between [-PA_LOWER_LIMIT: PA_UPPER_LIMIT] dbm.
*         The first element shall be the lower level (PA_LEVEL[1]) value and the last element
*         the higher level one (PA_LEVEL[paLevelMaxIndex]).
* @retval None.
*/
void SpiritRadioSetPATabledBm(uint8_t cPALevelMaxIndex, uint8_t cWidth, PALoadCapacitor xCLoad, float* pfPAtabledBm)
{
  uint8_t palevel[9], address, paLevelValue;
  uint32_t lFBase=SpiritRadioGetFrequencyBase();

  /* Check the parameters */
  s_assert_param(IS_PA_MAX_INDEX(cPALevelMaxIndex));
  s_assert_param(IS_PA_STEP_WIDTH(cWidth));
  s_assert_param(IS_PA_LOAD_CAP(xCLoad));
  
  /* Check the PA level in dBm is in the range and calculate the PA_LEVEL value
  to write in the corresponding register using the linearization formula */
  for(int i=0; i<=cPALevelMaxIndex; i++)
  {
    s_assert_param(IS_PAPOWER_DBM(*pfPAtabledBm));
    paLevelValue=SpiritRadioGetdBm2Reg(lFBase,(*pfPAtabledBm));
    palevel[cPALevelMaxIndex-i]=paLevelValue;
    pfPAtabledBm++;
  }
  
  /* Sets the PA_POWER[0] register */
  palevel[cPALevelMaxIndex+1]=xCLoad|(cWidth-1)<<3|cPALevelMaxIndex;
  
  /* Sets the base address */
  address=PA_POWER8_BASE+7-cPALevelMaxIndex;
  
  /* Configures the PA_POWER registers */
  g_xStatus = SpiritSpiWriteRegisters(address, cPALevelMaxIndex+2, palevel);
  
}


/**
* @brief  Returns the Power Amplifier Table and registers, returning values in dBm.
* @param  pcPALevelMaxIndex pointer to the number of levels settled.
*         This parameter will be in the range [0:7].
* @param  pfPAtabledBm pointer to an array of 8 elements containing the PA value in dbm.
*         The first element will be the PA_LEVEL_0 and the last element
*         will be PA_LEVEL_7. Any value higher than PA_UPPER_LIMIT implies no output
*         power (output stage is in high impedance).
* @retval None.
*/
void SpiritRadioGetPATabledBm(uint8_t* pcPALevelMaxIndex, float* pfPAtabledBm)
{
  uint8_t palevelvect[9];
  uint32_t lFBase=SpiritRadioGetFrequencyBase();
  
  /* Reads the PA_LEVEL_x registers and the PA_POWER_0 register */
  g_xStatus = SpiritSpiReadRegisters(PA_POWER8_BASE, 9, palevelvect);
  
  /* Fill the PAtable */
  for(int i=7; i>=0; i--)
  {
    (*pfPAtabledBm)=SpiritRadioGetReg2dBm(lFBase,palevelvect[i]);
    pfPAtabledBm++;
  }
  
  /* Return the settled index */
  *pcPALevelMaxIndex = palevelvect[8]&0x07;
  
}


/**
* @brief  Sets a specific PA_LEVEL register, with a value given in dBm.
* @param  cIndex PA_LEVEL to set. This parameter shall be in the range [0:7].
* @param  fPowerdBm PA value to write expressed in dBm . Be sure that this values is in the
*         correct range [-PA_LOWER_LIMIT: PA_UPPER_LIMIT] dBm.
* @retval None.
* @note This function makes use of the @ref SpiritRadioGetdBm2Reg fcn to interpolate the 
*       power value.
*/
void SpiritRadioSetPALeveldBm(uint8_t cIndex, float fPowerdBm)
{
  uint8_t address, paLevelValue;
  
  /* Check the parameters */
  s_assert_param(IS_PA_MAX_INDEX(cIndex));
  s_assert_param(IS_PAPOWER_DBM(fPowerdBm));
  
  /* interpolate the power level */
  paLevelValue=SpiritRadioGetdBm2Reg(SpiritRadioGetFrequencyBase(),fPowerdBm);

  /* Sets the base address */
  address=PA_POWER8_BASE+7-cIndex;
  
  /* Configures the PA_LEVEL register */
  g_xStatus = SpiritSpiWriteRegisters(address, 1, &paLevelValue);
  
}


/**
* @brief  Returns a specific PA_LEVEL register, returning a value in dBm.
* @param  cIndex PA_LEVEL to read. This parameter shall be in the range [0:7]
* @retval float Settled power level expressed in dBm. A value
*         higher than PA_UPPER_LIMIT dBm implies no output power
*         (output stage is in high impedance).
* @note This function makes use of the @ref SpiritRadioGetReg2dBm fcn to interpolate the 
*       power value.
*/
float SpiritRadioGetPALeveldBm(uint8_t cIndex)
{
  uint8_t address, paLevelValue;
  
  /* Check the parameters */
  s_assert_param(IS_PA_MAX_INDEX(cIndex));
  
  /* Sets the base address */
  address=PA_POWER8_BASE+7-cIndex;
  
  /* Reads the PA_LEVEL[cIndex] register */
  g_xStatus = SpiritSpiReadRegisters(address, 1, &paLevelValue);
  
  return SpiritRadioGetReg2dBm(SpiritRadioGetFrequencyBase(),paLevelValue);
}


/**
* @brief  Configures the Power Amplifier Table and registers.
* @param  cPALevelMaxIndex number of levels to set. This parameter shall be in the range [0:7].
* @param  cWidth step width expressed in terms of bit period units Tb/8.
*         This parameter shall be in the range [1:4].
* @param  xCLoad one of the possible value of the enum type PALoadCapacitor.
*         @arg LOAD_0_PF    No additional PA load capacitor
*         @arg LOAD_1_2_PF  1.2pF additional PA load capacitor
*         @arg LOAD_2_4_PF  2.4pF additional PA load capacitor
*         @arg LOAD_3_6_PF  3.6pF additional PA load capacitor
* @param  pcPAtable pointer to an array of PA values in the range [0: 90], where 0 implies no
*         output power, 1 will be the maximum level and 90 the minimum one
*         The first element shall be the lower level (PA_LEVEL[0]) value and the last element
*         the higher level one (PA_LEVEL[paLevelMaxIndex]).
* @retval None.
*/
void SpiritRadioSetPATable(uint8_t cPALevelMaxIndex, uint8_t cWidth, PALoadCapacitor xCLoad, uint8_t* pcPAtable)
{
  uint8_t palevel[9], address;
  
  /* Check the parameters */
  s_assert_param(IS_PA_MAX_INDEX(cPALevelMaxIndex));
  s_assert_param(IS_PA_STEP_WIDTH(cWidth));
  s_assert_param(IS_PA_LOAD_CAP(xCLoad));
  
  /* Check the PA levels are in the range */
  for(int i=0; i<=cPALevelMaxIndex; i++)
  {
    s_assert_param(IS_PAPOWER(*pcPAtable));
    palevel[cPALevelMaxIndex-i]=*pcPAtable;
    pcPAtable++;
  }
  
  /* Sets the PA_POWER[0] register */
  palevel[cPALevelMaxIndex+1]=xCLoad|((cWidth-1)<<3)|cPALevelMaxIndex;
  
  /* Sets the base address */
  address=PA_POWER8_BASE+7-cPALevelMaxIndex;
  
  /* Configures the PA_POWER registers */
  g_xStatus = SpiritSpiWriteRegisters(address, cPALevelMaxIndex+2, palevel);
  
}


/**
* @brief  Returns the Power Amplifier Table and registers.
* @param  pcPALevelMaxIndex pointer to the number of levels settled.
*         This parameter shall be in the range [0:7].
* @param  pcPAtable pointer to an array of 8 elements containing the PA value.
*         The first element will be the PA_LEVEL_0 and the last element
*         will be PA_LEVEL_7. Any value equals to 0 implies that level has
*         no output power (output stage is in high impedance).
* @retval None
*/
void SpiritRadioGetPATable(uint8_t* pcPALevelMaxIndex, uint8_t* pcPAtable)
{
  uint8_t palevelvect[9];
  
  /* Reads the PA_LEVEL_x registers and the PA_POWER_0 register */
  g_xStatus = SpiritSpiReadRegisters(PA_POWER8_BASE, 9, palevelvect);
  
  /* Fill the PAtable */
  for(int i=7; i>=0; i--)
  {
    *pcPAtable = palevelvect[i];
    pcPAtable++;
  }
  
  /* Return the settled index */
  *pcPALevelMaxIndex = palevelvect[8]&0x07;
  
}


/**
* @brief  Sets a specific PA_LEVEL register.
* @param  cIndex PA_LEVEL to set. This parameter shall be in the range [0:7].
* @param  cPower PA value to write in the register. Be sure that this values is in the
*         correct range [0 : 90].
* @retval None.
*/
void SpiritRadioSetPALevel(uint8_t cIndex, uint8_t cPower)
{
  uint8_t address;
  
  /* Check the parameters */
  s_assert_param(IS_PA_MAX_INDEX(cIndex));
  s_assert_param(IS_PAPOWER(cPower));
  
  /* Sets the base address */
  address=PA_POWER8_BASE+7-cIndex;
  
  /* Configures the PA_LEVEL register */
  g_xStatus = SpiritSpiWriteRegisters(address, 1, &cPower);
  
}


/**
* @brief  Returns a specific PA_LEVEL register.
* @param  cIndex PA_LEVEL to read. This parameter shall be in the range [0:7].
* @retval uint8_t PA_LEVEL value. A value equal to zero
*         implies no output power (output stage is in high impedance).
*/
uint8_t SpiritRadioGetPALevel(uint8_t cIndex)
{
  uint8_t address, tempRegValue;
  
  /* Check the parameters */
  s_assert_param(IS_PA_MAX_INDEX(cIndex));
  
  /* Sets the base address */
  address=PA_POWER8_BASE+7-cIndex;
  
  /* Reads the PA_LEVEL[cIndex] register and return the value */
  g_xStatus = SpiritSpiReadRegisters(address, 1, &tempRegValue);
  return tempRegValue;
  
}


/**
* @brief  Sets the output stage additional load capacitor bank.
* @param  xCLoad one of the possible value of the enum type PALoadCapacitor.
*         @arg LOAD_0_PF    No additional PA load capacitor
*         @arg LOAD_1_2_PF  1.2pF additional PA load capacitor
*         @arg LOAD_2_4_PF  2.4pF additional PA load capacitor
*         @arg LOAD_3_6_PF  3.6pF additional PA load capacitor
* @retval None.
*/
void SpiritRadioSetPACwc(PALoadCapacitor xCLoad)
{
  uint8_t tempRegValue;
  
  /* Check the parameters */
  s_assert_param(IS_PA_LOAD_CAP(xCLoad));
  
  /* Reads the PA_POWER_0 register */
  SpiritSpiReadRegisters(PA_POWER0_BASE, 1, &tempRegValue);
  
  /* Mask the CWC[1:0] field and write the new value */
  tempRegValue &= 0x3F;
  tempRegValue |= xCLoad;
  
  /* Configures the PA_POWER_0 register */
  g_xStatus = SpiritSpiWriteRegisters(PA_POWER0_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the output stage additional load capacitor bank.
* @param  None.
* @retval PALoadCapacitor Output stage additional load capacitor bank.
*         This parameter can be:
*         @arg LOAD_0_PF    No additional PA load capacitor
*         @arg LOAD_1_2_PF  1.2pF additional PA load capacitor
*         @arg LOAD_2_4_PF  2.4pF additional PA load capacitor
*         @arg LOAD_3_6_PF  3.6pF additional PA load capacitor
*/
PALoadCapacitor SpiritRadioGetPACwc(void)
{
  uint8_t tempRegValue;
  
  /* Reads the PA_POWER_0 register */
  g_xStatus = SpiritSpiReadRegisters(PA_POWER0_BASE, 1, &tempRegValue);
  
  /* Mask the CWC[1:0] field and return the value*/
  return (PALoadCapacitor)(tempRegValue & 0xC0);
  
}


/**
* @brief  Sets a specific PA_LEVEL_MAX_INDEX.
* @param  cIndex PA_LEVEL_MAX_INDEX to set. This parameter shall be in the range [0:7].
* @retval None
*/
void SpiritRadioSetPALevelMaxIndex(uint8_t cIndex)
{
  uint8_t tempRegValue;
  
  /* Check the parameters */
  s_assert_param(IS_PA_MAX_INDEX(cIndex));
  
  /* Reads the PA_POWER_0 register */
  SpiritSpiReadRegisters(PA_POWER0_BASE, 1, &tempRegValue);
  
  /* Mask the PA_LEVEL_MAX_INDEX[1:0] field and write the new value */
  tempRegValue &= 0xF8;
  tempRegValue |= cIndex;
  
  /* Configures the PA_POWER_0 register */
  g_xStatus = SpiritSpiWriteRegisters(PA_POWER0_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the actual PA_LEVEL_MAX_INDEX.
* @param  None.
* @retval uint8_t Actual PA_LEVEL_MAX_INDEX. This parameter will be in the range [0:7].
*/
uint8_t SpiritRadioGetPALevelMaxIndex(void)
{
  uint8_t tempRegValue;
  
  /* Reads the PA_POWER_0 register */
  g_xStatus = SpiritSpiReadRegisters(PA_POWER0_BASE, 1, &tempRegValue);
  
  /* Mask the PA_LEVEL_MAX_INDEX[1:0] field and return the value */
  return (tempRegValue & 0x07);
  
}


/**
* @brief  Sets a specific PA_RAMP_STEP_WIDTH.
* @param  cWidth step width expressed in terms of bit period units Tb/8.
*         This parameter shall be in the range [1:4].
* @retval None.
*/
void SpiritRadioSetPAStepWidth(uint8_t cWidth)
{
  uint8_t tempRegValue;
  
  /* Check the parameters */
  s_assert_param(IS_PA_STEP_WIDTH(cWidth));
  
  /* Reads the PA_POWER_0 register */
  SpiritSpiReadRegisters(PA_POWER0_BASE, 1, &tempRegValue);
  
  /* Mask the PA_RAMP_STEP_WIDTH[1:0] field and write the new value */
  tempRegValue &= 0xE7;
  tempRegValue |= (cWidth-1)<<3;
  
  /* Configures the PA_POWER_0 register */
  g_xStatus = SpiritSpiWriteRegisters(PA_POWER0_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the actual PA_RAMP_STEP_WIDTH.
* @param  None.
* @retval uint8_t Step width value expressed in terms of bit period units Tb/8.
*         This parameter will be in the range [1:4].
*/
uint8_t SpiritRadioGetPAStepWidth(void)
{
  uint8_t tempRegValue;
  
  /* Reads the PA_POWER_0 register */
  g_xStatus = SpiritSpiReadRegisters(PA_POWER0_BASE, 1, &tempRegValue);
  
  /* Mask the PA_RAMP_STEP_WIDTH[1:0] field and return the value */
  tempRegValue &= 0x18;
  return  ((tempRegValue>>3)+1);
  
}


/**
* @brief  Enables or Disables the Power Ramping.
* @param  xNewState new state for power ramping.
*         This parameter can be: S_ENABLE or S_DISABLE.
* @retval None.
*/
void SpiritRadioPARamping(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue = 0x00;
  
  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));
  
  /* Reads the PA_POWER_0 register and configure the PA_RAMP_ENABLE field */
  SpiritSpiReadRegisters(PA_POWER0_BASE, 1, &tempRegValue);
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= PA_POWER0_PA_RAMP_MASK;
  }
  else
  {
    tempRegValue &= (~PA_POWER0_PA_RAMP_MASK);
  }
  
  /* Sets the PA_POWER_0 register */
  g_xStatus = SpiritSpiWriteRegisters(PA_POWER0_BASE, 1, &tempRegValue);
  
}

/**
* @brief  Returns the Power Ramping enable bit.
* @param  xNewState new state for power ramping.
*         This parameter can be: S_ENABLE or S_DISABLE.
* @retval None.
*/
SpiritFunctionalState SpiritRadioGetPARamping(void)
{
  uint8_t tempRegValue;
  
  /* Reads the PA_POWER_0 register and configure the PA_RAMP_ENABLE field */
  g_xStatus = SpiritSpiReadRegisters(PA_POWER0_BASE, 1, &tempRegValue);
  
  /* Mask and return data */
  return (SpiritFunctionalState)((tempRegValue>>5) & 0x01);
  
}


/**
* @brief  Enables or Disables the AFC.
* @param  xNewState new state for AFC.
*         This parameter can be: S_ENABLE or S_DISABLE.
* @retval None.
*/
void SpiritRadioAFC(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue = 0x00;
  
  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));
  
  /* Reads the AFC_2 register and configure the AFC Enabled field */
  SpiritSpiReadRegisters(AFC2_BASE, 1, &tempRegValue);
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= AFC2_AFC_MASK;
  }
  else
  {
    tempRegValue &= (~AFC2_AFC_MASK);
  }
  
  /* Sets the AFC_2 register */
  g_xStatus = SpiritSpiWriteRegisters(AFC2_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Enables or Disables the AFC freeze on sync word detection.
* @param  xNewState new state for AFC freeze on sync word detection.
*         This parameter can be: S_ENABLE or S_DISABLE.
* @retval None.
*/
void SpiritRadioAFCFreezeOnSync(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue = 0x00;
  
  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));
  
  /* Reads the AFC_2 register and configure the AFC Freeze on Sync field */
  SpiritSpiReadRegisters(AFC2_BASE, 1, &tempRegValue);
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= AFC2_AFC_FREEZE_ON_SYNC_MASK;
  }
  else
  {
    tempRegValue &= (~AFC2_AFC_FREEZE_ON_SYNC_MASK);
  }
  
  /* Sets the AFC_2 register */
  g_xStatus = SpiritSpiWriteRegisters(AFC2_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Sets the AFC working mode.
* @param  xMode the AFC mode. This parameter can be one of the values defined in @ref AFCMode :
*         @arg AFC_SLICER_CORRECTION     AFC loop closed on slicer
*         @arg AFC_2ND_IF_CORRECTION     AFC loop closed on 2nd conversion stage
* @retval None.
*/
void SpiritRadioSetAFCMode(AFCMode xMode)
{
  uint8_t tempRegValue = 0x00;
  
  /* Check the parameters */
  s_assert_param(IS_AFC_MODE(xMode));
  
  /* Reads the AFC_2 register and configure the AFC Mode field */
  SpiritSpiReadRegisters(AFC2_BASE, 1, &tempRegValue);
  if(xMode == AFC_2ND_IF_CORRECTION)
  {
    tempRegValue |= AFC_2ND_IF_CORRECTION;
  }
  else
  {
    tempRegValue &= (~AFC_2ND_IF_CORRECTION);
  }
  
  /* Sets the AFC_2 register */
  g_xStatus = SpiritSpiWriteRegisters(AFC2_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the AFC working mode.
* @param  None.
* @retval AFCMode Settled AFC mode. This parameter will be one of the values defined in @ref AFCMode :
*         @arg AFC_SLICER_CORRECTION     AFC loop closed on slicer
*         @arg AFC_2ND_IF_CORRECTION     AFC loop closed on 2nd conversion stage
*/
AFCMode SpiritRadioGetAFCMode(void)
{
  uint8_t tempRegValue;
  
  /* Reads the AFC_2 register */
  g_xStatus = SpiritSpiReadRegisters(AFC2_BASE, 1, &tempRegValue);
  
  /* Mask the AFC Mode field and returns the value */
  return (AFCMode)(tempRegValue & 0x20);
  
}


/**
* @brief  Sets the AFC peak detector leakage.
* @param  cLeakage the peak detector leakage. This parameter shall be in the range:
*         [0:31].
* @retval None.
*/
void SpiritRadioSetAFCPDLeakage(uint8_t cLeakage)
{
  uint8_t tempRegValue = 0x00;
  
  /* Check the parameters */
  s_assert_param(IS_AFC_PD_LEAKAGE(cLeakage));
  
  /* Reads the AFC_2 register and configure the AFC PD leakage field */
  SpiritSpiReadRegisters(AFC2_BASE, 1, &tempRegValue);
  tempRegValue &= 0xE0;
  tempRegValue |= cLeakage;
  
  /* Sets the AFC_2 register */
  g_xStatus = SpiritSpiWriteRegisters(AFC2_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the AFC peak detector leakage.
* @param  None.
* @retval uint8_t Peak detector leakage value. This parameter will be in the range:
*         [0:31].
*/
uint8_t SpiritRadioGetAFCPDLeakage(void)
{
  uint8_t tempRegValue;
  
  /* Reads the AFC_2 register */
  g_xStatus = SpiritSpiReadRegisters(AFC2_BASE, 1, &tempRegValue);
  
  /* Mask the AFC PD leakage field and return the value */
  return (tempRegValue & 0x1F);
  
}


/**
* @brief  Sets the length of the AFC fast period expressed as number of samples.
* @param  cLength length of the fast period in number of samples.
* @retval None.
*/
void SpiritRadioSetAFCFastPeriod(uint8_t cLength)
{
  /* Sets the AFC_1 register */
  g_xStatus = SpiritSpiWriteRegisters(AFC1_BASE, 1, &cLength);
  
}


/**
* @brief  Returns the AFC fast period expressed as number of samples.
* @param  None.
* @retval uint8_t Length of the fast period in number of samples.
*/
uint8_t SpiritRadioGetAFCFastPeriod(void)
{
  uint8_t tempRegValue;
  
  /* Reads the AFC 1 register and return the value */
  g_xStatus = SpiritSpiReadRegisters(AFC1_BASE, 1, &tempRegValue);
  
  return tempRegValue;
  
}


/**
* @brief  Sets the AFC loop gain in fast mode.
* @param  cGain AFC loop gain in fast mode. This parameter shall be in the range:
*         [0:15].
* @retval None.
*/
void SpiritRadioSetAFCFastGain(uint8_t cGain)
{
  uint8_t tempRegValue = 0x00;
  
  /* Check the parameters */
  s_assert_param(IS_AFC_FAST_GAIN(cGain));
  
  /* Reads the AFC_0 register and configure the AFC Fast Gain field */
  SpiritSpiReadRegisters(AFC0_BASE, 1, &tempRegValue);
  tempRegValue &= 0x0F;
  tempRegValue |= cGain<<4;
  
  /* Sets the AFC_0 register */
  g_xStatus = SpiritSpiWriteRegisters(AFC0_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the AFC loop gain in fast mode.
* @param  None.
* @retval uint8_t AFC loop gain in fast mode. This parameter will be in the range:
*         [0:15].
*/
uint8_t SpiritRadioGetAFCFastGain(void)
{
  uint8_t tempRegValue;
  
  /* Reads the AFC_0 register, mask the AFC Fast Gain field and return the value  */
  g_xStatus = SpiritSpiReadRegisters(AFC0_BASE, 1, &tempRegValue);
  
  return ((tempRegValue & 0xF0)>>4);
  
}


/**
* @brief  Sets the AFC loop gain in slow mode.
* @param  cGain AFC loop gain in slow mode. This parameter shall be in the range:
*         [0:15].
* @retval None.
*/
void SpiritRadioSetAFCSlowGain(uint8_t cGain)
{
  uint8_t tempRegValue = 0x00;
  
  /* Check the parameters */
  s_assert_param(IS_AFC_SLOW_GAIN(cGain));
  
  /* Reads the AFC_0 register and configure the AFC Slow Gain field */
  SpiritSpiReadRegisters(AFC0_BASE, 1, &tempRegValue);
  tempRegValue &= 0xF0;
  tempRegValue |= cGain;
  
  /* Sets the AFC_0 register */
  g_xStatus = SpiritSpiWriteRegisters(AFC0_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the AFC loop gain in slow mode.
* @param  None.
* @retval uint8_t AFC loop gain in slow mode. This parameter will be in the range:
*         [0:15].
*/
uint8_t SpiritRadioGetAFCSlowGain(void)
{
  uint8_t tempRegValue;
  
  /* Reads the AFC_0 register, mask the AFC Slow Gain field and return the value */
  g_xStatus = SpiritSpiReadRegisters(AFC0_BASE, 1, &tempRegValue);
  
  return (tempRegValue & 0x0F);
  
}


/**
* @brief  Returns the AFC correction from the corresponding register.
* @param  None.
* @retval int8_t AFC correction, read from the corresponding register.
*         This parameter will be in the range [-128:127].
*/
int8_t SpiritRadioGetAFCCorrectionReg(void)
{
  uint8_t tempRegValue;
  
  /* Reads the AFC_CORR register, cast the read value as signed char and return it */
  g_xStatus = SpiritSpiReadRegisters(AFC_CORR_BASE, 1, &tempRegValue);
  
  return (int8_t)tempRegValue;
  
}


/**
* @brief  Returns the AFC correction expressed in Hz.
* @param  None.
* @retval int32_t AFC correction expressed in Hz
*         according to the following formula:<ul>
*         <li> Fafc[Hz]= (Fdig/(12*2^10))*AFC_CORR  where </li>
*         <li> AFC_CORR is the value read in the AFC_CORR register </li> </ul>
*/
int32_t SpiritRadioGetAFCCorrectionHz(void)
{
  int8_t correction;
  uint32_t xtal = s_lXtalFrequency;
  
  /* Reads the AFC correction register */
  correction = SpiritRadioGetAFCCorrectionReg();
  
  if(xtal>DOUBLE_XTAL_THR)
  {
    xtal /= 2;
  }
  
  /* Calculates and return the Frequency Correction */
  return (int32_t)(xtal/(12*pow(2,10))*correction);
  
}


/**
* @brief  Enables or Disables the AGC.
* @param  xNewState new state for AGC.
*         This parameter can be: S_ENABLE or S_DISABLE
* @retval None.
*/
void SpiritRadioAGC(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue = 0x00;
  
  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));
  
  /* Reads the AGCCTRL_0 register and configure the AGC Enabled field */
  SpiritSpiReadRegisters(AGCCTRL0_BASE, 1, &tempRegValue);
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= AGCCTRL0_AGC_MASK;
  }
  else
  {
    tempRegValue &= (~AGCCTRL0_AGC_MASK);
  }
  
  /* Sets the AGCCTRL_0 register */
  g_xStatus = SpiritSpiWriteRegisters(AGCCTRL0_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Sets the AGC measure time.
* @param  nTime AGC measure time expressed in us. This parameter shall be in the range [0, 393216/F_Xo].
* @retval None.
*/
void SpiritRadioSetAGCMeasureTimeUs(uint16_t nTime)
{
  uint8_t tempRegValue, measure;
  
  /* Check the parameter */
  s_assert_param(IS_AGC_MEASURE_TIME_US(nTime,s_lXtalFrequency));
  
  /* Reads the AGCCTRL_2 register */
  SpiritSpiReadRegisters(AGCCTRL2_BASE, 1, &tempRegValue);
  
  /* Calculates the measure time value to write in the register */
  measure = (uint8_t)lroundf(log2((float) (nTime/1e6 * (s_lXtalFrequency/12)) )); /* #1035-D */
  (measure>15) ? (measure=15):(measure);
  
  /* Mask the MEAS_TIME field and write the new value */
  tempRegValue &= 0xF0;
  tempRegValue |= measure;
  
  /* Sets the AGCCTRL_2 register */
  g_xStatus = SpiritSpiWriteRegisters(AGCCTRL2_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the AGC measure time.
* @param  None.
* @retval uint16_t AGC measure time expressed in us. This parameter will be in the range [0, 393216/F_Xo].
*/
uint16_t SpiritRadioGetAGCMeasureTimeUs(void)
{
  uint8_t measure;
  
  /* Reads the AGCCTRL_2 register */
  g_xStatus = SpiritSpiReadRegisters(AGCCTRL2_BASE, 1, &measure);
  
  /* Mask the MEAS_TIME field */
  measure &= 0x0F;
  
  /* Calculates the measure time value to write in the register */
  return (uint16_t)((12.0/s_lXtalFrequency)*(float)pow(2,measure)*1e6);
  
}


/**
* @brief  Sets the AGC measure time.
* @param  cTime AGC measure time to write in the MEAS_TIME field of AGCCTRL_2 register.
*         This parameter shall be in the range [0:15].
* @retval None.
*/
void SpiritRadioSetAGCMeasureTime(uint8_t cTime)
{
  uint8_t tempRegValue;
  
  /* Check the parameter */
  s_assert_param(IS_AGC_MEASURE_TIME(cTime));
  
  /* Reads the AGCCTRL_2 register */
  SpiritSpiReadRegisters(AGCCTRL2_BASE, 1, &tempRegValue);
  
  /* Mask the MEAS_TIME field and write the new value */
  tempRegValue &= 0xF0;
  tempRegValue |= cTime;
  
  /* Sets the AGCCTRL_2 register */
  g_xStatus = SpiritSpiWriteRegisters(AGCCTRL2_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the AGC measure time.
* @param  None.
* @retval uint8_t AGC measure time read from the MEAS_TIME field of AGCCTRL_2 register.
*         This parameter will be in the range [0:15].
*/
uint8_t SpiritRadioGetAGCMeasureTime(void)
{
  uint8_t tempRegValue;
  
  /* Reads the AGCCTRL_2 register, mask the MEAS_TIME field and return the value  */
  g_xStatus = SpiritSpiReadRegisters(AGCCTRL2_BASE, 1, &tempRegValue);
  
  return (tempRegValue & 0x0F);
  
}



/**
* @brief  Sets the AGC high threshold.
* @param  cHighThreshold AGC high threshold to write in the THRESHOLD_HIGH field of AGCCTRL_1 register.
*         This parameter shall be in the range [0:15].
* @retval None.
*/
void SpiritRadioSetAGCHighThreshold(uint8_t cHighThreshold)
{
  uint8_t tempRegValue;
  
  /* Check the parameter */
  s_assert_param(IS_AGC_THRESHOLD(cHighThreshold));
  
  /* Reads the AGCCTRL_1 register */
  SpiritSpiReadRegisters(AGCCTRL1_BASE, 1, &tempRegValue);
  
  /* Mask the THRESHOLD_HIGH field and write the new value */
  tempRegValue &= 0x0F;
  tempRegValue |= cHighThreshold<<4;
  
  /* Sets the AGCCTRL_1 register */
  g_xStatus = SpiritSpiWriteRegisters(AGCCTRL1_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the AGC high threshold.
* @param  None.
* @retval uint8_t AGC high threshold read from the THRESHOLD_HIGH field of AGCCTRL_1 register.
*         This parameter will be in the range [0:15].
*/
uint8_t SpiritRadioGetAGCHighThreshold(void)
{
  uint8_t tempRegValue;
  
  /* Reads the AGCCTRL_1 register, mask the THRESHOLD_HIGH field and return the value */
  g_xStatus = SpiritSpiReadRegisters(AGCCTRL1_BASE, 1, &tempRegValue);
  
  return ((tempRegValue & 0xF0)>>4);
  
}


/**
* @brief  Sets the AGC low threshold.
* @param  cLowThreshold AGC low threshold to write in the THRESHOLD_LOW field of AGCCTRL_1 register.
*         This parameter shall be in the range [0:15].
* @retval None.
*/
void SpiritRadioSetAGCLowThreshold(uint8_t cLowThreshold)
{
  uint8_t tempRegValue;
  
  /* Check the parameter */
  s_assert_param(IS_AGC_THRESHOLD(cLowThreshold));
  
  /* Reads the AGCCTRL_1 register */
  SpiritSpiReadRegisters(AGCCTRL1_BASE, 1, &tempRegValue);
  
  /* Mask the THRESHOLD_LOW field and write the new value */
  tempRegValue &= 0xF0;
  tempRegValue |= cLowThreshold;
  
  /* Sets the AGCCTRL_1 register */
  g_xStatus = SpiritSpiWriteRegisters(AGCCTRL1_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the AGC low threshold.
* @param  None.
* @retval uint8_t AGC low threshold read from the THRESHOLD_LOW field of AGCCTRL_1 register.
*         This parameter will be in the range [0:15].
*/
uint8_t SpiritRadioGetAGCLowThreshold(void)
{
  uint8_t tempRegValue;
  
  /* Reads the AGCCTRL_1 register, mask the THRESHOLD_LOW field and return the value  */
  g_xStatus = SpiritSpiReadRegisters(AGCCTRL1_BASE, 1, &tempRegValue);
  
  return (tempRegValue & 0x0F);
  
}


/**
* @brief  Sets the clock recovery algorithm.
* @param  xMode the Clock Recovery mode. This parameter can be one of the values defined in @ref ClkRecMode :
*         @arg CLK_REC_PLL     PLL alogrithm for clock recovery
*         @arg CLK_REC_DLL     DLL alogrithm for clock recovery
* @retval None.
*/
void SpiritRadioSetClkRecMode(ClkRecMode xMode)
{
  uint8_t tempRegValue;
  
  /* Check the parameter */
  s_assert_param(IS_CLK_REC_MODE(xMode));
  
  /* Reads the FDEV_0 register */
  SpiritSpiReadRegisters(FDEV0_BASE, 1, &tempRegValue);
  
  /* Mask the CLOCK_REC_ALGO_SEL field and write the new value */
  tempRegValue &= 0xF7;
  tempRegValue |= (uint8_t)xMode;
  
  /* Sets the FDEV_0 register */
  g_xStatus = SpiritSpiWriteRegisters(FDEV0_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the Clock Recovery working mode.
* @param  None.
* @retval ClkRecMode Clock Recovery mode. This parameter can be one of the values defined in @ref ClkRecMode :
*         @arg CLK_REC_PLL     PLL alogrithm for clock recovery
*         @arg CLK_REC_DLL     DLL alogrithm for clock recovery
*/
ClkRecMode SpiritRadioGetClkRecMode(void)
{
  uint8_t tempRegValue;
  
  /* Reads the FDEV_0 register, mask the CLOCK_REC_ALGO_SEL field and return the value */
  g_xStatus = SpiritSpiReadRegisters(FDEV0_BASE, 1, &tempRegValue);
  
  return (ClkRecMode)(tempRegValue & 0x08);
  
}


/**
* @brief  Sets the clock recovery proportional gain.
* @param  cPGain the Clock Recovery proportional gain to write in the CLK_REC_P_GAIN field of CLOCKREC register.
*         It represents is log2 value of the clock recovery proportional gain.
*          This parameter shall be in the range [0:7].
* @retval None.
*/
void SpiritRadioSetClkRecPGain(uint8_t cPGain)
{
  uint8_t tempRegValue;
  
  /* Check the parameter */
  s_assert_param(IS_CLK_REC_P_GAIN(cPGain));
  
  /* Reads the CLOCKREC register */
  SpiritSpiReadRegisters(CLOCKREC_BASE, 1, &tempRegValue);
  
  /* Mask the CLK_REC_P_GAIN field and write the new value */
  tempRegValue &= 0x1F;
  tempRegValue |= (cPGain<<5);
  
  /* Sets the CLOCKREC register */
  g_xStatus = SpiritSpiWriteRegisters(CLOCKREC_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the log2 of the clock recovery proportional gain.
* @param  None.
* @retval uint8_t Clock Recovery proportional gain read from the CLK_REC_P_GAIN field of CLOCKREC register.
*         This parameter will be in the range [0:7].
*/
uint8_t SpiritRadioGetClkRecPGain(void)
{
  uint8_t tempRegValue;
  
  /* Reads the CLOCKREC register, mask the CLK_REC_P_GAIN field and return the value  */
  g_xStatus = SpiritSpiReadRegisters(CLOCKREC_BASE, 1, &tempRegValue);
  
  return ((tempRegValue & 0xEF)>>5);
  
}


/**
* @brief  Sets the clock recovery integral gain.
* @param  cIGain the Clock Recovery integral gain to write in the CLK_REC_I_GAIN field of CLOCKREC register.
*         This parameter shall be in the range [0:15].
* @retval None.
*/
void SpiritRadioSetClkRecIGain(uint8_t cIGain)
{
  uint8_t tempRegValue;
  
  /* Check the parameter */
  s_assert_param(IS_CLK_REC_I_GAIN(cIGain));
  
  /* Reads the CLOCKREC register */
  SpiritSpiReadRegisters(CLOCKREC_BASE, 1, &tempRegValue);
  
  /* Mask the CLK_REC_P_GAIN field and write the new value */
  tempRegValue &= 0xF0;
  tempRegValue |= cIGain;
  
  /* Sets the CLOCKREC register */
  g_xStatus = SpiritSpiWriteRegisters(CLOCKREC_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the clock recovery integral gain.
* @param  None.
* @retval uint8_t Clock Recovery integral gain read from the
*         CLK_REC_I_GAIN field of CLOCKREC register.
*         This parameter will be in the range [0:15].
*/
uint8_t SpiritRadioGetClkRecIGain(void)
{
  uint8_t tempRegValue;
  
  /* Reads the CLOCKREC register, mask the CLK_REC_I_GAIN field and return the value */
  g_xStatus = SpiritSpiReadRegisters(CLOCKREC_BASE, 1, &tempRegValue);
  
  return (tempRegValue & 0x0F);
  
}


/**
* @brief  Sets the postfilter length for clock recovery algorithm.
* @param  xLength the postfilter length in symbols. This parameter can be one of the values defined in @ref PstFltLength :
*         @arg PSTFLT_LENGTH_8     Postfilter length is 8 symbols
*         @arg PSTFLT_LENGTH_16    Postfilter length is 16 symbols
* @retval None.
*/
void SpiritRadioSetClkRecPstFltLength(PstFltLength xLength)
{
  uint8_t tempRegValue;
  
  /* Check the parameter */
  s_assert_param(IS_PST_FLT_LENGTH(xLength));
  
  /* Reads the CLOCKREC register */
  SpiritSpiReadRegisters(CLOCKREC_BASE, 1, &tempRegValue);
  
  /* Mask the PSTFLT_LEN field and write the new value */
  tempRegValue &= 0xEF;
  tempRegValue |= (uint8_t)xLength;
  
  /* Sets the CLOCKREC register */
  g_xStatus = SpiritSpiWriteRegisters(CLOCKREC_BASE, 1, &tempRegValue);
  
}


/**
* @brief  Returns the postfilter length for clock recovery algorithm.
* @param  None.
* @retval PstFltLength Postfilter length in symbols. This parameter can be one of the values defined in @ref PstFltLength :
*         @arg PSTFLT_LENGTH_8     Postfilter length is 8 symbols
*         @arg PSTFLT_LENGTH_16    Postfilter length is 16 symbols
*/
PstFltLength SpiritRadioGetClkRecPstFltLength(void)
{
  uint8_t tempRegValue;
  
  /* Reads the CLOCKREC register, mask the PSTFLT_LEN field and return the value */
  g_xStatus = SpiritSpiReadRegisters(CLOCKREC_BASE, 1, &tempRegValue);
  
  return (PstFltLength)(tempRegValue & 0x10);
  
}


/**
* @brief  Enables or Disables the received data blanking when the CS is under the threshold.
* @param  xNewState new state of this mode.
*         This parameter can be: S_ENABLE or S_DISABLE .
* @retval None.
*/
void SpiritRadioCsBlanking(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;
  
  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));
  
  /* Reads the ANT_SELECT_CONF_BASE and mask the CS_BLANKING BIT field */
  SpiritSpiReadRegisters(ANT_SELECT_CONF_BASE, 1, &tempRegValue);
  
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= ANT_SELECT_CS_BLANKING_MASK;
  }
  else
  {
    tempRegValue &= (~ANT_SELECT_CS_BLANKING_MASK);
  }
  
  /* Writes the new value in the ANT_SELECT_CONF register */
  g_xStatus = SpiritSpiWriteRegisters(ANT_SELECT_CONF_BASE, 1, &tempRegValue);
  
  
}

/**
* @brief  Enables or Disables the persistent RX mode.
* @param  xNewState new state of this mode.
*         This parameter can be: S_ENABLE or S_DISABLE .
* @retval None.
*/
void SpiritRadioPersistenRx(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;
  
  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));
  
  /* Reads the PROTOCOL0_BASE and mask the PROTOCOL0_PERS_RX_MASK bitfield */
  SpiritSpiReadRegisters(PROTOCOL0_BASE, 1, &tempRegValue);
  
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= PROTOCOL0_PERS_RX_MASK;
  }
  else
  {
    tempRegValue &= (~PROTOCOL0_PERS_RX_MASK);
  }
  
  /* Writes the new value in the PROTOCOL0_BASE register */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL0_BASE, 1, &tempRegValue);
  
}

/**
* @brief  Enables or Disables the synthesizer reference divider.
* @param  xNewState new state for synthesizer reference divider.
*         This parameter can be: S_ENABLE or S_DISABLE .
* @retval None.
*/
void SpiritRadioSetRefDiv(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;
  
  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));
  
  /* Reads the SYNTH_CONFIG1_BASE and mask the REFDIV bit field */
  SpiritSpiReadRegisters(SYNTH_CONFIG1_BASE, 1, &tempRegValue);
  
  if(xNewState == S_ENABLE)
  {
    tempRegValue |= 0x80;
  }
  else
  {
    tempRegValue &= 0x7F;
  }
  
  /* Writes the new value in the SYNTH_CONFIG1_BASE register */
  g_xStatus = SpiritSpiWriteRegisters(SYNTH_CONFIG1_BASE, 1, &tempRegValue);
  
}

/**
* @brief  Get the the synthesizer reference divider state.
* @param  void.
* @retval None.
*/
SpiritFunctionalState SpiritRadioGetRefDiv(void)
{
  uint8_t tempRegValue;
  
  g_xStatus = SpiritSpiReadRegisters(SYNTH_CONFIG1_BASE, 1, &tempRegValue);
  
  if(((tempRegValue>>7)&0x1))
  {
    return S_ENABLE;
  }
  else
  {
    return S_DISABLE;
  }
  
}

/**
* @brief  Enables or Disables the synthesizer reference divider.
* @param  xNewState new state for synthesizer reference divider.
*         This parameter can be: S_ENABLE or S_DISABLE .
* @retval None.
*/
void SpiritRadioSetDigDiv(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;
  
  /* Check the parameters */
  s_assert_param(IS_SPIRIT_FUNCTIONAL_STATE(xNewState));
  
  /* Reads the XO_RCO_TEST_BASE and mask the PD_CLKDIV bit field */
  SpiritSpiReadRegisters(XO_RCO_TEST_BASE, 1, &tempRegValue);
  
  if(xNewState == S_ENABLE)
  {
    tempRegValue &= 0xf7;
  }
  else
  {
    
    tempRegValue |= 0x08;
  }
  
  /* Writes the new value in the XO_RCO_TEST_BASE register */
  g_xStatus = SpiritSpiWriteRegisters(XO_RCO_TEST_BASE, 1, &tempRegValue);
  
}

/**
* @brief  Get the the synthesizer reference divider state.
* @param  void.
* @retval None.
*/
SpiritFunctionalState SpiritRadioGetDigDiv(void)
{
  uint8_t tempRegValue;
  
  g_xStatus = SpiritSpiReadRegisters(XO_RCO_TEST_BASE, 1, &tempRegValue);
  
  if(((tempRegValue>>3)&0x1))
  {
    return S_DISABLE;
  }
  else
  {
    return S_ENABLE;
  }
  
}

/**
* @brief  Returns the XTAL frequency.
* @param  void.
* @retval uint32_t XTAL frequency.
*/
uint32_t SpiritRadioGetXtalFrequency(void)
{
  return s_lXtalFrequency; 
}

/**
* @brief  Sets the XTAL frequency.
* @param  uint32_t XTAL frequency.
* @retval void.
*/
void SpiritRadioSetXtalFrequency(uint32_t lXtalFrequency)
{
  s_lXtalFrequency = lXtalFrequency; 
}

/**
* @}
*/
 
/* --------------------------------   TIMER  ----------------------------------- */ 

/**
 * @defgroup Timer_Private_Functions                    Timer Private Functions
 * @{
 */

/**
 * @brief  Enables or Disables the LDCR mode.
 * @param  xNewState new state for LDCR mode.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritTimerLdcrMode(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Reads the register value */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL2_BASE, 1, &tempRegValue);

  /* Mask the read value to enable or disable the LDC mode */
  if(xNewState==S_ENABLE)
  {
    tempRegValue |= PROTOCOL2_LDC_MODE_MASK;
  }
  else
  {
    tempRegValue &= ~PROTOCOL2_LDC_MODE_MASK;
  }

  /* Writes the register to Enable or Disable the LDCR mode */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL2_BASE, 1, &tempRegValue);

}


/**
 * @brief  Enables or Disables the LDCR timer reloading with the value stored in the LDCR_RELOAD registers.
 * @param  xNewState new state for LDCR reloading.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void SpiritTimerLdcrAutoReload(SpiritFunctionalState xNewState)
{
  uint8_t tempRegValue;

  /* Reads the register value */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 1, &tempRegValue);

  /* Mask te read value to enable or disable the reload on sync mode */
  if(xNewState==S_ENABLE)
  {
    tempRegValue |= PROTOCOL1_LDC_RELOAD_ON_SYNC_MASK;
  }
  else
  {
    tempRegValue &= ~PROTOCOL1_LDC_RELOAD_ON_SYNC_MASK;
  }

  /* Writes the register to Enable or Disable the Auto Reload */
  g_xStatus = SpiritSpiWriteRegisters(PROTOCOL1_BASE, 1, &tempRegValue);

}


/**
 * @brief  Returns the LDCR timer reload bit.
 * @param  None.
 * @retval SpiritFunctionalState: value of the reload bit.
 */
SpiritFunctionalState SpiritTimerLdcrGetAutoReload(void)
{
  uint8_t tempRegValue;

  /* Reads the register value */
  g_xStatus = SpiritSpiReadRegisters(PROTOCOL1_BASE, 1, &tempRegValue);

  return (SpiritFunctionalState)(tempRegValue & 0x80);

}

/**
 * @brief  Sets the RX timeout timer initialization registers with the values of COUNTER and PRESCALER according to the formula: Trx=PRESCALER*COUNTER*Tck.
 *         Remember that it is possible to have infinite RX_Timeout writing 0 in the RX_Timeout_Counter and/or RX_Timeout_Prescaler registers.
 * @param  cCounter value for the timer counter.
 *         This parameter must be an uint8_t.
 * @param  cPrescaler value for the timer prescaler.
 *         This parameter must be an uint8_t.
 * @retval None.
 */
void SpiritTimerSetRxTimeout(uint8_t cCounter , uint8_t cPrescaler)
{
  uint8_t tempRegValue[2]={cPrescaler,cCounter};

  /* Writes the prescaler and counter value for RX timeout in the corresponding register */
  g_xStatus = SpiritSpiWriteRegisters(TIMERS5_RX_TIMEOUT_PRESCALER_BASE, 2, tempRegValue);

}


/**
 * @brief  Sets the RX timeout timer counter and prescaler from the desired value in ms. it is possible to fix the RX_Timeout to
 *         a minimum value of 50.417us to a maximum value of about 3.28 s.
 * @param  fDesiredMsec desired timer value.
 *         This parameter must be a float.
 * @retval None
 */

void SpiritTimerSetRxTimeoutMs(float fDesiredMsec)
{
  uint8_t tempRegValue[2];

  /* Computes the counter and prescaler value */
  SpiritTimerComputeRxTimeoutValues(fDesiredMsec , &tempRegValue[1] , &tempRegValue[0]);

  /* Writes the prescaler and counter value for RX timeout in the corresponding register */
  g_xStatus = SpiritSpiWriteRegisters(TIMERS5_RX_TIMEOUT_PRESCALER_BASE, 2, tempRegValue);

}


/**
 * @brief  Sets the RX timeout timer counter. If it is equal to 0 the timeout is infinite.
 * @param  cCounter value for the timer counter.
 *         This parameter must be an uint8_t.
 * @retval None.
 */
void SpiritTimerSetRxTimeoutCounter(uint8_t cCounter)
{
  /* Writes the counter value for RX timeout in the corresponding register */
  g_xStatus = SpiritSpiWriteRegisters(TIMERS4_RX_TIMEOUT_COUNTER_BASE, 1, &cCounter);

}


/**
 * @brief  Sets the RX timeout timer prescaler. If it is equal to 0 the timeout is infinite.
 * @param  cPrescaler value for the timer prescaler.
 *         This parameter must be an uint8_t.
 * @retval None
 */
void SpiritTimerSetRxTimeoutPrescaler(uint8_t cPrescaler)
{
  /* Writes the prescaler value for RX timeout in the corresponding register */
  g_xStatus = SpiritSpiWriteRegisters(TIMERS5_RX_TIMEOUT_PRESCALER_BASE, 1, &cPrescaler);

}


/**
 * @brief  Returns the RX timeout timer.
 * @param  pfTimeoutMsec pointer to the variable in which the timeout expressed in milliseconds has to be stored.
 *         If the returned value is 0, it means that the RX_Timeout is infinite.
 *         This parameter must be a float*.
 * @param  pcCounter pointer to the variable in which the timer counter has to be stored.
 *         This parameter must be an uint8_t*.
 * @param  pcPrescaler pointer to the variable in which the timer prescaler has to be stored.
 *         This parameter must be an uint8_t*.
 * @retval None.
 */
void SpiritTimerGetRxTimeout(float* pfTimeoutMsec, uint8_t* pcCounter , uint8_t* pcPrescaler)
{
  uint8_t tempRegValue[2];

  /* Reads the RX timeout registers value */
  g_xStatus = SpiritSpiReadRegisters(TIMERS5_RX_TIMEOUT_PRESCALER_BASE, 2, tempRegValue);

  /* Returns values */
  (*pcPrescaler) = tempRegValue[0];
  (*pcCounter) = tempRegValue[1];
    
  float nXtalFrequency = (float)SpiritRadioGetXtalFrequency();
  if(nXtalFrequency>DOUBLE_XTAL_THR) {
    nXtalFrequency /= 2.0f;   /* #1035-D */
  }
  nXtalFrequency /= 1000.0f;   /* #1035-D */
  *pfTimeoutMsec = (float)((tempRegValue[0]+1)*tempRegValue[1]*(1210.0f/nXtalFrequency));  /* #1035-D */
  

}


/**
 * @brief  Sets the LDCR wake up timer initialization registers with the values of
 *         COUNTER and PRESCALER according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where
 *         Tck = 28.818 us. The minimum vale of the wakeup timeout is 28.818us (PRESCALER and
 *         COUNTER equals to 0) and the maximum value is about 1.89 s (PRESCALER anc COUNTER equals
 *         to 255).
 * @param  cCounter value for the timer counter.
 *         This parameter must be an uint8_t.
 * @param  cPrescaler value for the timer prescaler.
 *         This parameter must be an uint8_t.
 * @retval None.
 */
void SpiritTimerSetWakeUpTimer(uint8_t cCounter , uint8_t cPrescaler)
{
  uint8_t tempRegValue[2]={cPrescaler,cCounter};

  /* Writes the counter and prescaler value of wake-up timer in the corresponding register */
  g_xStatus = SpiritSpiWriteRegisters(TIMERS3_LDC_PRESCALER_BASE, 2, tempRegValue);

}


/**
 * @brief  Sets the LDCR wake up timer counter and prescaler from the desired value in ms,
 *         according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us.
 *         The minimum vale of the wakeup timeout is 28.818us (PRESCALER and COUNTER equals to 0)
 *         and the maximum value is about 1.89 s (PRESCALER anc COUNTER equals to 255).
 * @param  fDesiredMsec desired timer value.
 *         This parameter must be a float.
 * @retval None.
 */
void SpiritTimerSetWakeUpTimerMs(float fDesiredMsec)
{
  uint8_t tempRegValue[2];

  /* Computes counter and prescaler */
  SpiritTimerComputeWakeUpValues(fDesiredMsec , &tempRegValue[1] , &tempRegValue[0]);

  /* Writes the counter and prescaler value of wake-up timer in the corresponding register */
  g_xStatus = SpiritSpiWriteRegisters(TIMERS3_LDC_PRESCALER_BASE, 2, tempRegValue);

}


/**
 * @brief  Sets the LDCR wake up timer counter. Remember that this value is incresead by one in the Twu calculation.
 *         Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us
 * @param  cCounter value for the timer counter.
 *         This parameter must be an uint8_t.
 * @retval None.
 */
void SpiritTimerSetWakeUpTimerCounter(uint8_t cCounter)
{
  /* Writes the counter value for Wake_Up timer in the corresponding register */
  g_xStatus = SpiritSpiWriteRegisters(TIMERS2_LDC_COUNTER_BASE, 1, &cCounter);

}


/**
 * @brief  Sets the LDCR wake up timer prescaler. Remember that this value is incresead by one in the Twu calculation.
 *         Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us
 * @param  cPrescaler value for the timer prescaler.
 *         This parameter must be an uint8_t.
 * @retval None.
 */
void SpiritTimerSetWakeUpTimerPrescaler(uint8_t cPrescaler)
{
  /* Writes the prescaler value for Wake_Up timer in the corresponding register */
  g_xStatus = SpiritSpiWriteRegisters(TIMERS3_LDC_PRESCALER_BASE, 1, &cPrescaler);

}


/**
 * @brief  Returns the LDCR wake up timer, according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us.
 * @param  pfWakeUpMsec pointer to the variable in which the wake-up time expressed in milliseconds has to be stored.
 *         This parameter must be a float*.
 * @param  pcCounter pointer to the variable in which the timer counter has to be stored.
 *         This parameter must be an uint8_t*.
 * @param  pcPrescaler pointer to the variable in which the timer prescaler has to be stored.
 *         This parameter must be an uint8_t*.
 * @retval None.
 */
void SpiritTimerGetWakeUpTimer(float* pfWakeUpMsec, uint8_t* pcCounter , uint8_t* pcPrescaler)
{
  uint8_t tempRegValue[2];
  /* uint32_t xtal=SpiritRadioGetXtalFrequency(); */
  float rco_freq;
  
  rco_freq=(float)SpiritTimerGetRcoFrequency();
  
  /* Reads the Wake_Up timer registers value */
  g_xStatus = SpiritSpiReadRegisters(TIMERS3_LDC_PRESCALER_BASE, 2, tempRegValue);

  /* Returns values */
  (*pcPrescaler)=tempRegValue[0];
  (*pcCounter)=tempRegValue[1];
  *pfWakeUpMsec = (float)((((*pcPrescaler)+1)*((*pcCounter)+1)*(1000.0f/rco_freq))); /* #1035-D */

}


/**
 * @brief  Sets the LDCR wake up timer reloading registers with the values of
 *         COUNTER and PRESCALER according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where
 *         Tck = 28.818 us. The minimum vale of the wakeup timeout is 28.818us (PRESCALER and
 *         COUNTER equals to 0) and the maximum value is about 1.89 s (PRESCALER anc COUNTER equals
 *         to 255).
 * @param  cCounter reload value for the timer counter.
 *         This parameter must be an uint8_t.
 * @param  cPrescaler reload value for the timer prescaler.
 *         This parameter must be an uint8_t.
 * @retval None.
 */
void SpiritTimerSetWakeUpTimerReload(uint8_t cCounter , uint8_t cPrescaler)
{
  uint8_t tempRegValue[2]={cPrescaler,cCounter};

  /* Writes the counter and prescaler value of reload wake-up timer in the corresponding register */
  g_xStatus = SpiritSpiWriteRegisters(TIMERS1_LDC_RELOAD_PRESCALER_BASE, 2, tempRegValue);

}


/**
 * @brief  Sets the LDCR wake up reload timer counter and prescaler from the desired value in ms,
 *         according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us.
 *         The minimum vale of the wakeup timeout is 28.818us (PRESCALER and COUNTER equals to 0)
 *         and the maximum value is about 1.89 s (PRESCALER anc COUNTER equals to 255).
 * @param  fDesiredMsec desired timer value.
 *         This parameter must be a float.
 * @retval None.
 */
void SpiritTimerSetWakeUpTimerReloadMs(float fDesiredMsec)
{
  uint8_t tempRegValue[2];

  /* Computes counter and prescaler */
  SpiritTimerComputeWakeUpValues(fDesiredMsec , &tempRegValue[1] , &tempRegValue[0]);

  /* Writes the counter and prescaler value of reload wake-up timer in the corresponding register */
  g_xStatus = SpiritSpiWriteRegisters(TIMERS1_LDC_RELOAD_PRESCALER_BASE, 2, tempRegValue);

}


/**
 * @brief  Sets the LDCR wake up timer reload counter. Remember that this value is incresead by one in the Twu calculation.
 *         Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us
 * @param  cCounter value for the timer counter.
 *         This parameter must be an uint8_t.
 * @retval None
 */
void SpiritTimerSetWakeUpTimerReloadCounter(uint8_t cCounter)
{
  /* Writes the counter value for reload Wake_Up timer in the corresponding register */
  g_xStatus = SpiritSpiWriteRegisters(TIMERS0_LDC_RELOAD_COUNTER_BASE, 1, &cCounter);

}


/**
 * @brief  Sets the LDCR wake up timer reload prescaler. Remember that this value is incresead by one in the Twu calculation.
 *         Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us
 * @param  cPrescaler value for the timer prescaler.
 *         This parameter must be an uint8_t.
 * @retval None
 */
void SpiritTimerSetWakeUpTimerReloadPrescaler(uint8_t cPrescaler)
{
  /* Writes the prescaler value for reload Wake_Up timer in the corresponding register */
  g_xStatus = SpiritSpiWriteRegisters(TIMERS1_LDC_RELOAD_PRESCALER_BASE, 1, &cPrescaler);

}


/**
 * @brief  Returns the LDCR wake up reload timer, according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us.
 * @param  pfWakeUpReloadMsec pointer to the variable in which the wake-up reload time expressed in milliseconds has to be stored.
 *         This parameter must be a float*.
 * @param  pcCounter pointer to the variable in which the timer counter has to be stored.
 *         This parameter must be an uint8_t*.
 * @param  pcPrescaler pointer to the variable in which the timer prescaler has to be stored.
 *         This parameter must be an uint8_t*.
 * @retval None.
 */
void SpiritTimerGetWakeUpTimerReload(float* pfWakeUpReloadMsec, uint8_t* pcCounter , uint8_t* pcPrescaler)
{
  uint8_t tempRegValue[2];
  /* uint32_t xtal=SpiritRadioGetXtalFrequency(); */
  float rco_freq;
  
  rco_freq=(float)SpiritTimerGetRcoFrequency();
  
  /* Reads the reload Wake_Up timer registers value */
  g_xStatus = SpiritSpiReadRegisters(TIMERS1_LDC_RELOAD_PRESCALER_BASE, 2, tempRegValue);

  /* Returns values */
  (*pcPrescaler)=tempRegValue[0];
  (*pcCounter)=tempRegValue[1];
  *pfWakeUpReloadMsec = (float)((((*pcPrescaler)+1)*((*pcCounter)+1)*(1000.0f/rco_freq)));    /* #1035-D */

}

/**
 * @brief  Computes and returns the RCO frequency. 
 *         This frequency depends on the xtal frequency and the XTAL bit in register 0x01.
 * @retval RCO frequency in Hz as an uint16_t.
 */
uint16_t SpiritTimerGetRcoFrequency(void)
{
  uint16_t rco_freq=34700;
  uint32_t xtal=SpiritRadioGetXtalFrequency();
  
  if(xtal>30000000) xtal/=2;
  
  if(xtal==25000000)
  {
    uint8_t xtal_flag;
    SpiritSpiReadRegisters(0x01, 1, &xtal_flag);
    xtal_flag=(xtal_flag&0x40);
    
    if(xtal_flag==0)
    {
      rco_freq=36100;
    }
    else
    {
      rco_freq=33300;
    }
  }
  
  return rco_freq;
}

/**
 * @brief  Computes the values of the wakeup timer counter and prescaler from the user time expressed in millisecond.
 *         The prescaler and the counter values are computed maintaining the prescaler value as
 *         small as possible in order to obtain the best resolution, and in the meantime minimizing the error.
 * @param  fDesiredMsec desired wakeup timeout in millisecs.
 *         This parameter must be a float. Since the counter and prescaler are 8 bit registers the maximum
 *         reachable value is maxTime = fTclk x 256 x 256.
 * @param  pcCounter pointer to the variable in which the value for the wakeup timer counter has to be stored.
 *         This parameter must be a uint8_t*.
 * @param  pcPrescaler pointer to the variable in which the value for the wakeup timer prescaler has to be stored.
 *         This parameter must be an uint8_t*.
 * @retval None
 */
void SpiritTimerComputeWakeUpValues(float fDesiredMsec , uint8_t* pcCounter , uint8_t* pcPrescaler)
{
  float rco_freq,err;
  uint32_t n;
  
  rco_freq=((float)SpiritTimerGetRcoFrequency())/1000;
  
  /* N cycles in the time base of the timer: 
     - clock of the timer is RCO frequency
     - divide times 1000 more because we have an input in ms (variable rco_freq is already this frequency divided by 1000)
  */
  n=(uint32_t)(fDesiredMsec*rco_freq);
    
  /* check if it is possible to reach that target with prescaler and counter of spirit1 */
  if(n/0xFF>0xFD)
  {
    /* if not return the maximum possible value */
    (*pcCounter) = 0xFF;
    (*pcPrescaler) = 0xFF;
    return;
  }
  
  /* prescaler is really 2 as min value */
  (*pcPrescaler)=(n/0xFF)+2;
  (*pcCounter) = n / (*pcPrescaler);
  
  /* check if the error is minimum */
  err=S_ABS((float)(*pcCounter)*(*pcPrescaler)/rco_freq-fDesiredMsec);
  
  if((*pcCounter)<=254)
  {
    if(S_ABS((float)((*pcCounter)+1)*(*pcPrescaler)/rco_freq-fDesiredMsec)<err)
      (*pcCounter)=(*pcCounter)+1;
  }
    
  /* decrement prescaler and counter according to the logic of this timer in spirit1 */
  (*pcPrescaler)--;
  if((*pcCounter)>1)
    (*pcCounter)--;
  else
    (*pcCounter)=1;
}


/**
 * @brief  Computes the values of the rx_timeout timer counter and prescaler from the user time expressed in millisecond.
 *         The prescaler and the counter values are computed maintaining the prescaler value as
 *         small as possible in order to obtain the best resolution, and in the meantime minimizing the error.
 * @param  fDesiredMsec desired rx_timeout in millisecs.
 *         This parameter must be a float. Since the counter and prescaler are 8 bit registers the maximum
 *         reachable value is maxTime = fTclk x 255 x 255.
 * @param  pcCounter pointer to the variable in which the value for the rx_timeout counter has to be stored.
 *         This parameter must be a uint8_t*.
 * @param  pcPrescaler pointer to the variable in which the value for the rx_timeout prescaler has to be stored.
 *         This parameter must be an uint8_t*.
 * @retval None
 */
void SpiritTimerComputeRxTimeoutValues(float fDesiredMsec , uint8_t* pcCounter , uint8_t* pcPrescaler)
{
  uint32_t nXtalFrequency = SpiritRadioGetXtalFrequency();
  uint32_t n;
  float err;
  
  /* if xtal is doubled divide it by 2 */
  if(nXtalFrequency>DOUBLE_XTAL_THR) {
    nXtalFrequency >>= 1;
  }
  
  /* N cycles in the time base of the timer: 
     - clock of the timer is xtal/1210
     - divide times 1000 more because we have an input in ms
  */
  n=(uint32_t)(fDesiredMsec*nXtalFrequency/1210000);
  
  /* check if it is possible to reach that target with prescaler and counter of spirit1 */
  if(n/0xFF>0xFD)
  {
    /* if not return the maximum possible value */
    (*pcCounter) = 0xFF;
    (*pcPrescaler) = 0xFF;
    return;
  }
  
  /* prescaler is really 2 as min value */
  (*pcPrescaler)=(n/0xFF)+2;
  (*pcCounter) = n / (*pcPrescaler);
  
  /* check if the error is minimum */
  err=S_ABS((float)(*pcCounter)*(*pcPrescaler)*1210000/nXtalFrequency-fDesiredMsec);
  
  if((*pcCounter)<=254)
  {
    if(S_ABS((float)((*pcCounter)+1)*(*pcPrescaler)*1210000/nXtalFrequency-fDesiredMsec)<err)
      (*pcCounter)=(*pcCounter)+1;
  }
    
  /* decrement prescaler and counter according to the logic of this timer in spirit1 */
  (*pcPrescaler)--;
  if((*pcCounter)>1)
    (*pcCounter)--;
  else
    (*pcCounter)=1;
}


/**
 * @brief  Sets the RX timeout stop conditions.
 * @param  xStopCondition new stop condition.
 *         This parameter can be any value of @ref RxTimeoutStopCondition.
 * @retval None
 */
void SpiritTimerSetRxTimeoutStopCondition(RxTimeoutStopCondition xStopCondition)
{
  uint8_t tempRegValue[2];

  /* Check the parameters */
  s_assert_param(IS_RX_TIMEOUT_STOP_CONDITION(xStopCondition));

  /* Reads value on the PKT_FLT_OPTIONS and PROTOCOL2 register */
  g_xStatus = SpiritSpiReadRegisters(PCKT_FLT_OPTIONS_BASE, 2, tempRegValue);

  tempRegValue[0] &= 0xBF;
  tempRegValue[0] |= ((xStopCondition & 0x08)  << 3);

  tempRegValue[1] &= 0x1F;
  tempRegValue[1] |= (xStopCondition << 5);

  /* Writes value on the PKT_FLT_OPTIONS and PROTOCOL2 register */
  g_xStatus = SpiritSpiWriteRegisters(PCKT_FLT_OPTIONS_BASE, 2, tempRegValue);

}

/**
 * @brief  Sends the LDC_RELOAD command to SPIRIT. Reload the LDC timer with the value stored in the LDC_PRESCALER / COUNTER registers.
 * @param  None.
 * @retval None
 */
void SpiritTimerReloadStrobe(void)
{
  /* Sends the CMD_LDC_RELOAD command */
  g_xStatus = SpiritSpiCommandStrobes(COMMAND_LDC_RELOAD);

}

/**
 *@}
 */
 
/* --------------------------------   COMMON  ----------------------------------- */ 


/** @defgroup Common_Exported_Functions                 Common Exported Functions
 * @{
 */

/**
 * @brief  Updates the gState (the global variable used to maintain memory of Spirit Status)
 *         reading the MC_STATE register of SPIRIT.
 * @param  None
 * @retval None
 */
void SpiritRefreshStatus(void)
{
  uint8_t tempRegValue[2];
  
  /* Read the status both from register and from SPI header and exit when they match.
      This will protect against possible transition state changes */
  do
  {
    /* Reads the MC_STATUS register to update the g_xStatus */
    g_xStatus = SpiritSpiReadRegisters(MC_STATE1_BASE, 2, tempRegValue);
  }
  while(!((((uint8_t*)&g_xStatus)[0])==tempRegValue[1] && 
          (((uint8_t*)&g_xStatus)[1]&0x0F)==tempRegValue[0])); 

}

/**
 * @}
 */

/** @defgroup Common_Private_Functions                 Common Private Functions
 * @{
 */
 
#ifdef  SPIRIT_USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param file  pointer to the source file name
 * @param line  assert_param error line source number
 * @retval : None
 */
void s_assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number */
  printf("Wrong parameters value: file %s on line %d\r\n", file, line);

  /* Infinite loop */
  while (1)
  {
  }
}
#elif SPIRIT_USE_VCOM_ASSERT

#include "SDK_EVAL_VC_General.h"

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param file pointer to the source file name
 * @param line  assert_param error line source number
 * @param expression: string representing the assert failed expression
 * @retval : None
 */
void s_assert_failed(uint8_t* file, uint32_t line, char* expression)
{

  printf("\n\rVCOM DEBUG: Incorrect parameter. Please reboot.\n\r");
  printf("%s:%d \n\r",file,line);
  printf("The expression %s returned FALSE.\n\r", expression);

  /* Infinite loop */
  while (1)
  {
  }
}

#elif SPIRIT_USE_FRAME_ASSERT

#include "SdkUsbProtocol.h"

/**
 * @brief Sends a notify frame with a payload indicating the name 
 *        of the assert failed.
 * @param expression: string representing the assert failed expression
 * @retval : None
 */
void s_assert_failed(uint8_t* expression)
{
  char pcPayload[100];
  uint16_t i;
  
  for(i = 0 ; expression[i]!='(' ; i++);
  expression[i]='\0';
  
  strcpy(pcPayload, &expression[3]);
  
  //sprintf(pcPayload, "The expression %s returned FALSE.\n\r", expression);
  SpiritNotifyAssertFailed(pcPayload);

}

#endif

/**
* @brief  Write single or multiple RF Transceivers register
* @param  cRegAddress: base register's address to be write
* @param  cNbBytes: number of registers and bytes to be write
* @param  pcBuffer: pointer to the buffer of values have to be written into registers
* @retval StatusBytes
*/
StatusBytes SpiritSpiWriteRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{
  StatusBytes *pStatus;
  pStatus = (StatusBytes *) RadioSpiWriteRegisters(cRegAddress, cNbBytes, pcBuffer);
  return *pStatus;
}

/**
* @brief  Read single or multiple SPIRIT1 register
* @param  cRegAddress: base register's address to be read
* @param  cNbBytes: number of registers and bytes to be read
* @param  pcBuffer: pointer to the buffer of registers' values read
* @retval StatusBytes
*/
StatusBytes SpiritSpiReadRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{
  StatusBytes *pStatus;
  pStatus = (StatusBytes *) RadioSpiReadRegisters(cRegAddress, cNbBytes, pcBuffer);
  return *pStatus;
}


/**
* @brief  Send a command
* @param  cCommandCode: command code to be sent
* @retval StatusBytes
*/
StatusBytes SpiritSpiCommandStrobes(uint8_t cCommandCode)
{
  StatusBytes *pStatus;
  pStatus = (StatusBytes *) RadioSpiCommandStrobes(cCommandCode);
  return *pStatus;
}


/**
* @brief  Write data into TX FIFO
* @param  cNbBytes: number of bytes to be written into TX FIFO
* @param  pcBuffer: pointer to data to write
* @retval StatusBytes
*/
StatusBytes SpiritSpiWriteFifo(uint8_t cNbBytes, uint8_t* pcBuffer)
{
  StatusBytes *pStatus;
  pStatus = (StatusBytes *) RadioSpiWriteFifo(cNbBytes, pcBuffer);
  return *pStatus;
}

/**
* @brief  Read data from RX FIFO
* @param  cNbBytes: number of bytes to read from RX FIFO
* @param  pcBuffer: pointer to data read from RX FIFO
* @retval StatusBytes
*/
StatusBytes SpiritSpiReadFifo(uint8_t cNbBytes, uint8_t* pcBuffer)
{
  StatusBytes *pStatus;
  pStatus = (StatusBytes *) RadioSpiReadFifo(cNbBytes, pcBuffer);
  return *pStatus;
}


/**
 * @}
 */
 

 /**
 *@}
 */
