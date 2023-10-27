/**
  ******************************************************************************
  * @file    PKA/PKA_PointCheck_IT/Inc/PKV.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of the PKV buffers.
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
#ifndef __PKV_H
#define __PKV_H

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t  PKV1_Qx[];
extern const uint32_t PKV1_Qx_len;
extern const uint8_t  PKV1_Qy[];
extern const uint32_t PKV1_Qy_len;
extern const uint32_t PKV1_Result;
extern const uint8_t  PKV2_Qx[];
extern const uint32_t PKV2_Qx_len;
extern const uint8_t  PKV2_Qy[];
extern const uint32_t PKV2_Qy_len;
extern const uint32_t PKV2_Result;

#ifdef __cplusplus
}
#endif

#endif /* __STM32L4xx_IT_H */

