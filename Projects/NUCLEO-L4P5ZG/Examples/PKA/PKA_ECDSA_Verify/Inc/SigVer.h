/**
  ******************************************************************************
  * @file    PKA/PKA_ECDSA_Verify/Inc/SigVer.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of SigVer.c .
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
