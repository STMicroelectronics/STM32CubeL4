/**
  ******************************************************************************
  * @file    PKA/PKA_ECDSA_Sign_IT/Inc/SigGen.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of SigGen.c .
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
