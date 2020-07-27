#ifndef _LIB_NDEF_CONFIG_H_
#define _LIB_NDEF_CONFIG_H_

#include <stdint.h>
#include "lib_NDEF.h"

int32_t NDEF_Wrapper_ReadData(uint8_t* pData, uint32_t offset, uint32_t length );
int32_t NDEF_Wrapper_WriteData(const uint8_t* pData, uint32_t offset, uint32_t length );
uint32_t NDEF_Wrapper_GetMemorySize(void);

#endif // _LIB_NDEF_CONFIG_H_
