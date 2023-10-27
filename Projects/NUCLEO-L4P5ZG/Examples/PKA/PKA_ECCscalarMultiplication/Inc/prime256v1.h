/**
  ******************************************************************************
  * @file    PKA/PKA_ECCscalarMultiplication/Inc/PKV.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of prime256v1.c .
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
#ifndef __PRIME256V1_H
#define __PRIME256V1_H

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t  prime256v1_Prime[];
extern const uint32_t prime256v1_Prime_len;
extern const uint8_t  prime256v1_A[];
extern const uint8_t  prime256v1_absA[];
extern const uint32_t prime256v1_A_len;
extern const uint32_t prime256v1_A_sign;
extern const uint8_t  prime256v1_B[];
extern const uint32_t prime256v1_B_len;
extern const uint8_t  prime256v1_Generator[];
extern const uint32_t prime256v1_Generator_len;
extern const uint8_t  prime256v1_GeneratorX[];
extern const uint32_t prime256v1_GeneratorX_len;
extern const uint8_t  prime256v1_GeneratorY[];
extern const uint32_t prime256v1_GeneratorY_len;
extern const uint8_t  prime256v1_Order[];
extern const uint32_t prime256v1_Order_len;
extern const uint32_t prime256v1_Cofactor;
extern const uint8_t  prime256v1_Seed[];
extern const uint32_t prime256v1_Seed_len;

#ifdef __cplusplus
}
#endif

#endif /* __PRIME256V1_H */

