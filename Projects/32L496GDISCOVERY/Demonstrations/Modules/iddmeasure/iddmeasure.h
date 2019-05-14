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

/* Includes ------------------------------------------------------------------*/

#undef GLOBAL
#ifdef __LOWPOWER_APP_C
#define GLOBAL
#else
#define GLOBAL extern
#endif

/** @addtogroup  IDD_MEASURE_MODULE
  * @{
  */

/** @defgroup  IDD_MEASURE

  * @brief Idd Measure routines 
  * @{
  */

/* Exported typedef -----------------------------------------------------------*/
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

/* Exported defines -----------------------------------------------------------*/
#define IDD_RUN        0
#define IDD_SLEEP      IDD_RUN + 1
#define IDD_LPR_2MHZ   IDD_SLEEP + 1
#define IDD_LPR_SLEEP  IDD_LPR_2MHZ + 1
#define IDD_STOP2      IDD_LPR_SLEEP + 1
#define IDD_STANDBY    IDD_STOP2 + 1
#define IDD_SHUTDOWN   IDD_STANDBY + 1

#define IDD_TEST_NB  (IDD_SHUTDOWN + 1)

/* Exported macros ------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
GLOBAL uint32_t MfxFwVersion;
GLOBAL Idd_AppliTypedef IddTest[IDD_TEST_NB];
GLOBAL uint32_t iddTestIndex;
GLOBAL Idd_StateTypedef IddMeasureState;

/* Exported functions ---------------------------------------------------------*/
char* IddMeasureGetIDD(void);
void  IddMeasureDeInit(void);
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

