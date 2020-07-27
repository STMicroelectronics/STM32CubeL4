#ifndef _TAGNFCA_H_
#define _TAGNFCA_H_

#include "lib_NDEF.h"

uint16_t NfcType4_GetLength(uint16_t* Length);
uint16_t NfcType4_ReadNDEF( uint8_t* pData );
uint16_t NfcType4_WriteNDEF(uint16_t Length, uint8_t* pData );

#endif // _TAGNFCA_H_
