/**
  ******************************************************************************
  * @file    iddmeasure.h
  * @author  MCD Application Team   
  * @brief   Idd Measurement functions
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
#ifndef __IDD_MEASURE_H
#define __IDD_MEASURE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stddef.h>

/** @addtogroup  IDD_MEASURE_MODULE
  * @{
  */

/** @defgroup  IDD_MEASURE

  * @brief Idd Measure routines 
  * @{
  */

/* Exported defines -----------------------------------------------------------*/
#define IDD_VALUE_STRING_SIZE 4
#define IDD_UNIT_STRING_SIZE 2
    
/* Exported typedef -----------------------------------------------------------*/
typedef enum
{
  IDD_RUN = 0,
  IDD_SLEEP,
  IDD_LPR_2MHZ,
  IDD_LPR_SLEEP,
  IDD_STOP2,
  IDD_STANDBY,
  IDD_SHUTDOWN,
  IDD_GETIDD_AFTER_RESET,
}IddAction_t;
    
#define IDD_TEST_NB  (IDD_SHUTDOWN + 1)

typedef struct  
{
  char value[IDD_VALUE_STRING_SIZE + 1];  /* 3 significant digit only */
  char unit[IDD_UNIT_STRING_SIZE + 1];    /* 2 letters */
  uint32_t value_na;                      /* value in nano amps */
  uint8_t error_code;                     /* 0 means no error else see mfx documentation 
                                             In case of error please retry */
} Iddvalue_t;

    
typedef struct
{
  void   (*IddEnter)(void);
  void   (*IddRestore)(void);
  char modeStr[20];
  uint32_t index;
  uint32_t userData1;
}Idd_AppliTypedef;

typedef enum
{
  IDD_STATE_IDLE = 0,
  IDD_STATE_MEASURE_ON_GOING = 1,
  IDD_STATE_MEASURE_OK = 2,
  IDD_STATE_MEASURE_ERROR = 3,
}Idd_StateTypedef;    
    
/* Exported macros ------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
void IddMeasureStart(void);
void IddMeasureGetValue(Iddvalue_t * idd);
Idd_StateTypedef IddMeasureGetState();
uint32_t IddMeasure_isSignature(void); /* Automatically restore index if TRUE */
void IddMeasure_clearSignature(void);


/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __IDD_MEASURE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

