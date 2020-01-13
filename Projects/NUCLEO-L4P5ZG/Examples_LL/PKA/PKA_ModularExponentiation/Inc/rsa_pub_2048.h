/**
  ******************************************************************************
  * @file    PKA/PKA_ModularExponentiation/Inc/rsa_pub_2048.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of rsa_pub_2048.c .
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
#ifndef __RSA_PUB_2048_H
#define __RSA_PUB_2048_H

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t  rsa_pub_2048_modulus[];
extern const uint32_t rsa_pub_2048_modulus_len;
extern const uint8_t rsa_pub_2048_publicExponent[];
extern const uint32_t rsa_pub_2048_publicExponent_len;

#ifdef __cplusplus
}
#endif

#endif /* __RSA_PUB_2048_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
