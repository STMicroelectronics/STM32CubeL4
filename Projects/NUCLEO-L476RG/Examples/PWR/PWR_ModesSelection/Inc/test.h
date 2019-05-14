/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Inc/test.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
