#ifndef _TAGTYPE3_WRAPPER_H_
#define _TAGTYPE3_WRAPPER_H_

#include "lib_NDEF.h"

uint16_t NfcType3_GetLength(uint16_t* Length);
uint16_t NfcType3_ReadNDEF( uint8_t* pData );
uint16_t NfcType3_WriteNDEF(uint16_t Length, uint8_t* pData );

#endif // _TAGTYPE3_WRAPPER_H_
