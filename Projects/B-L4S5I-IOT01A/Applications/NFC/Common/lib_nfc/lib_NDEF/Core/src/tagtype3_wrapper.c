#include "lib_wrapper.h"
#include "lib_NDEF_config.h"

typedef struct __attribute__((packed)) {
  uint8_t Ver;
  uint8_t Nbr;
  uint8_t Nbw;
  uint16_t  NmaxB;
  uint8_t rfu[4];
  uint8_t WriteFlag;
  uint8_t RWFlag;
  uint8_t Ln[3];
  uint16_t Checksum;
} TT3_Attr_Info_t;

static TT3_Attr_Info_t NDEF_Attr_Info;
uint16_t NfcType3_GetLength(uint16_t* Length)
{
  uint32_t t3NdefLen;
  uint8_t err = NDEF_Wrapper_ReadData((uint8_t*)&NDEF_Attr_Info,0,16);
  if(err != NDEF_OK)
  {
    return NDEF_ERROR;
  }
  t3NdefLen = NDEF_Attr_Info.Ln[0] << 16 | 
              NDEF_Attr_Info.Ln[1] <<  8 |
              NDEF_Attr_Info.Ln[2];
  if(t3NdefLen < 0x10000)
    *Length = t3NdefLen;
  else
    return NDEF_ERROR;
  
  return NDEF_OK;
}

uint16_t NfcType3_ReadNDEF( uint8_t* pData )
{
  uint16_t length;
  uint8_t err;
  uint16_t status = NfcType3_GetLength(&length);
  if(status != NDEF_OK)
  {
    return status;
  }
  err = NDEF_Wrapper_ReadData(pData,16,length);
  if(err != NDEF_OK)
  {
    return NDEF_ERROR;
  }
  return NDEF_OK;

}

uint16_t NfcType3_WriteNDEF(uint16_t Length, uint8_t* pData )
{
  uint8_t err = NDEF_Wrapper_WriteData(pData,16,Length);
  if(err != NDEF_OK)
  {
    return NDEF_ERROR;
  }

  err = NDEF_Wrapper_ReadData((uint8_t *)&NDEF_Attr_Info,0,16);
  if(err != NDEF_OK)
  {
    return NDEF_ERROR;
  }
  // update the length field
  NDEF_Attr_Info.Ln[0] = (Length >> 16) & 0xFF;
  NDEF_Attr_Info.Ln[1] = (Length >> 8) & 0xFF;
  NDEF_Attr_Info.Ln[2] = Length & 0xFF;
  err = NDEF_Wrapper_WriteData((uint8_t *)&NDEF_Attr_Info,0,16);
  if(err != NDEF_OK)
  {
    return NDEF_ERROR;
  }

  return NDEF_OK;
}

uint16_t NfcType3_WriteProprietary(uint16_t Length, uint8_t* pData )
{
  return NDEF_ERROR;
}
