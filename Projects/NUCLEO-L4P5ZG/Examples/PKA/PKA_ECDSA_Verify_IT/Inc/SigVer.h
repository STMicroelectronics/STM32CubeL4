/**
  ******************************************************************************
  * @file    PKA/PKA_ECDSA_Verify_IT/Inc/SigVer.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of SigVer.c .
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
#ifndef __SIGVER_H
#define __SIGVER_H

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t SigVer_Msg[];
extern const uint32_t SigVer_Msg_len;
extern const uint8_t SigVer_Hash_Msg[];
extern const uint8_t SigVer_Hash_Msg_False[];
extern const uint32_t SigVer_Hash_Msg_len;
extern const uint8_t SigVer_Qx[];
extern const uint32_t SigVer_Qx_len;
extern const uint8_t SigVer_Qy[];
extern const uint32_t SigVer_Qy_len;
extern const uint8_t SigVer_R[];
extern const uint32_t SigVer_R_len;
extern const uint8_t SigVer_S[];
extern const uint32_t SigVer_S_len;
extern const uint32_t SigVer_Result;

#ifdef __cplusplus
}
#endif

#endif /* __SIGVER_H */

