/**
  ******************************************************************************
  * @file    dis.h
  * @author  MCD Application Team
  * @brief   Header for dis.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        http://www.st.com/SLA0044
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DIS_H
#define __DIS_H

#ifdef __cplusplus
extern "C" 
{
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint8_t     *pPayload;
  uint8_t     Length;
}DIS_Data_t;


/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DIS_Init(uint16_t *p_options);
tBleStatus DIS_UpdateChar(uint16_t uuid, DIS_Data_t *p_data);

#ifdef __cplusplus
}
#endif

#endif /*__DIS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
