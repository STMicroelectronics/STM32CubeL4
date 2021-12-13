/**
  ******************************************************************************
  * @file    PKA/PKA_ModularExponentiation_IT/Inc/rsa_pub_2048.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of rsa_pub_2048.c .
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

