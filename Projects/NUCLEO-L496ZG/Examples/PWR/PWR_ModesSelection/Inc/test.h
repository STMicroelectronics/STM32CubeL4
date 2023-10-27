/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Inc/test.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TEST_H
#define __TEST_H


/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void test_shutdown(void);  
void test_standby(void);
void test_standby_rtc(void);
void test_standby_rtc_sram2(void);
void test_stop2(void);
void test_stop2_rtc(void);
void test_stop1_mroff(void);
void test_stop1_mroff_rtc(void);
void test_lpsleep_2mhz(void);
void test_lprun_2mhz(void);
void test_sleep_range2_24mhz(void);
void test_sleep_range1_80mhz(void);
void test_run_range2_24mhz(void);
void test_run_range1_80mhz(void);

extern void GPIO_AnalogState_Config(void);
extern void RTC_Config(void);
extern void SystemClock_100KHz(void);
extern void SystemClock_2MHz(void);
extern void SystemClock_24MHz(void);
extern void SystemClock_80MHz(void);

#endif /* __TEST_H */

