/**
  ******************************************************************************
  * @file    PKA/PKA_ECDSA_Sign/Inc/SigGen.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of SigGen.c .
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SIGGEN_H
#define __SIGGEN_H

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t  SigGen_Msg[];
extern const uint32_t SigGen_Msg_len;
extern const uint8_t  SigGen_Hash_Msg[];
extern const uint32_t SigGen_Hash_Msg_len;
extern const uint8_t  SigGen_d[];
extern const uint32_t SigGen_d_len;
extern const uint8_t  SigGen_Qx[];
extern const uint32_t SigGen_Qx_len;
extern const uint8_t  SigGen_Qy[];
extern const uint32_t SigGen_Qy_len;
extern const uint8_t  SigGen_k[];
extern const uint32_t SigGen_k_len;
extern const uint8_t  SigGen_R[];
extern const uint32_t SigGen_R_len;
extern const uint8_t  SigGen_S[];
extern const uint32_t SigGen_S_len;

#ifdef __cplusplus
}
#endif

#endif /* __SIGGEN_H */

