#include "lib_wrapper.h"
#include "lib_NDEF_config.h"

uint16_t NfcType4_GetLength(uint16_t* Length)
{
  uint8_t err = NDEF_Wrapper_ReadData((uint8_t*)Length,0,2);
  *Length = (*Length & 0xFF) << 8 | (*Length & 0xFF00) >> 8;

  if(err != NDEF_OK)
  {
    return NDEF_ERROR;
  }
  return NDEF_OK;
}

uint16_t NfcType4_ReadNDEF( uint8_t* pData )
{
  uint16_t length;
  uint8_t err;
  uint16_t status = NfcType4_GetLength(&length);
  if(status != NDEF_OK)
  {
    return status;
  }
  err = NDEF_Wrapper_ReadData(pData,2,length);
  if(err != NDEF_OK)
  {
    return NDEF_ERROR;
  }
  return NDEF_OK;

}

uint16_t NfcType4_WriteNDEF(uint16_t Length, uint8_t* pData )
{
  uint8_t txLen[2];
  txLen[0] = Length >> 8;
  txLen[1] = Length & 0xFF;
  uint16_t status = NDEF_Wrapper_WriteData(txLen, 0, 2);
  if(status != NDEF_OK)
  {
    return status;
  }
  return NDEF_Wrapper_WriteData(pData, 2, Length);
}

uint16_t NfcType4_WriteProprietary(uint16_t Length, uint8_t* pData )
{
  return NDEF_ERROR;
}
