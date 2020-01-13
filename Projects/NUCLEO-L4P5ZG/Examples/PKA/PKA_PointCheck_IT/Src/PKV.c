/**
  ******************************************************************************
  * @file    PKA/PKA_PointCheck_IT/Src/PKV.c
  * @author  MCD Application Team
  * @brief   This file contains reference buffers from 
  *          NIST Cryptographic Algorithm Validation Program (CAVP).
  *          (http://csrc.nist.gov/groups/STM/cavp/)
  *          2 test vectors are extracted to demonstrate PKA capability to
  *          check whether a given point P(x,y) satisfies or not the curves 
  *          over prime fields equation.
  *          They are adapted from PKV.rsp section [P-256] available under
  *          http://csrc.nist.gov/groups/STM/cavp/documents/dss/186-3ecdsatestvectors.zip
  *          and provided in the same directory for reference.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "main.h"

const uint8_t PKV1_Qx[] = {
  0xe0, 0xf7, 0x44, 0x9c, 0x55, 0x88, 0xf2, 0x44,
  0x92, 0xc3, 0x38, 0xf2, 0xbc, 0x8f, 0x78, 0x65,
  0xf7, 0x55, 0xb9, 0x58, 0xd4, 0x8e, 0xdb, 0x0f,
  0x2d, 0x00, 0x56, 0xe5, 0x0c, 0x3f, 0xd5, 0xb7
};
const uint32_t PKV1_Qx_len = 32;

const uint8_t PKV1_Qy[] = {
  0x86, 0xd7, 0xe9, 0x25, 0x5d, 0x0f, 0x4b, 0x6f,
  0x44, 0xfa, 0x2c, 0xd6, 0xf8, 0xba, 0x3c, 0x0a,
  0xa8, 0x28, 0x32, 0x1d, 0x6d, 0x8c, 0xc4, 0x30,
  0xca, 0x62, 0x84, 0xce, 0x1d, 0x5b, 0x43, 0xa0
};
const uint32_t PKV1_Qy_len = 32;

const uint32_t PKV1_Result = SET;

const uint8_t PKV2_Qx[] = {
  0xf2, 0xd1, 0xc0, 0xdc, 0x08, 0x52, 0xc3, 0xd8,
  0xa2, 0xa2, 0x50, 0x0a, 0x23, 0xa4, 0x48, 0x13,
  0xcc, 0xce, 0x1a, 0xc4, 0xe5, 0x84, 0x44, 0x17,
  0x5b, 0x44, 0x04, 0x69, 0xff, 0xc1, 0x22, 0x73
};
const uint32_t PKV2_Qx_len = 32;

const uint8_t PKV2_Qy[] = {
  0x32, 0xbf, 0xe9, 0x92, 0x83, 0x1b, 0x30, 0x5d,
  0x8c, 0x37, 0xb9, 0x67, 0x2d, 0xf5, 0xd2, 0x9f,
  0xcb, 0x5c, 0x29, 0xb4, 0xa4, 0x05, 0x34, 0x68,
  0x3e, 0x3a, 0xce, 0x23, 0xd2, 0x46, 0x47, 0xdd
};
const uint32_t PKV2_Qy_len = 32;

const uint32_t PKV2_Result = RESET; /* PKV.rsp mentions "F (2 - Point not on curve)" result, PKA returns 1 in this case (Only different from 0 is meaningfull) */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
