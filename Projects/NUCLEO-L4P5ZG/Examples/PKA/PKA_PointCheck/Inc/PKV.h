/**
  ******************************************************************************
  * @file    PKA/PKA_PointCheck/Inc/PKV.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of the PKV buffers.
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
