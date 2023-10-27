/**
  ******************************************************************************
  * @file    PKA/PKA_ModularExponentiation/Inc/rsa_priv_2048.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of rsa_priv_2048.c .
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
#ifndef __RSA_PRIV_2048_H
#define __RSA_PRIV_2048_H

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t  rsa_priv_2048_modulus[];
extern const uint32_t rsa_priv_2048_modulus_len;
extern const uint32_t rsa_priv_2048_publicExponent;
extern const uint8_t  rsa_priv_2048_privateExponent[];
extern const uint32_t rsa_priv_2048_privateExponent_len;
extern const uint8_t  rsa_priv_2048_prime1[];
extern const uint32_t rsa_priv_2048_prime1_len;
extern const uint8_t  rsa_priv_2048_prime2[];
extern const uint32_t rsa_priv_2048_prime2_len;
extern const uint8_t  rsa_priv_2048_exponent1[];
extern const uint32_t rsa_priv_2048_exponent1_len;
extern const uint8_t  rsa_priv_2048_exponent2[];
extern const uint32_t rsa_priv_2048_exponent2_len;
extern const uint8_t  rsa_priv_2048_coefficient[];
extern const uint32_t rsa_priv_2048_coefficient_len;

#ifdef __cplusplus
}
#endif

#endif /* __RSA_PRIV_2048_H */

