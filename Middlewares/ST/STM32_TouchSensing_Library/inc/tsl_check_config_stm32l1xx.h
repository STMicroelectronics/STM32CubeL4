/**
  ******************************************************************************
  * @file    tsl_check_config_stm32l1xx.h
  * @author  MCD Application Team
  * @brief   This file contains the check of all parameters defined in the
  *          STM32L1XX configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TSL_CHECK_CONFIG_STM32L1XX_H
#define __TSL_CHECK_CONFIG_STM32L1XX_H

// Check parameters common to all acquisitions
#include "tsl_check_config.h"

//------------------------------------------------------------------------------

/* 
Channel number can reach 37 on STM32L1 serie.
On Group 2 and Group 7 all pins are not availlable at the same time.
   * Groupe 2 (IO1..IO5): we can use 4 IOs instead of 5 (PG0, PG1)
   * Groupe 7 (IO1..IO7): we can use 5 IOs insteag of 7 (PG2, PG3, PG4)
This mean we can get only 37-1-2=34 channels.
On STM32 L0, L4, F0 and F3 we can have from 3 to 24 channels
*/ 
#define MAX_CHANNEL  34

/* This value is equal to int(MAX_CHANNEL/3) = 11 */
#define MAX_LINROT   11


#if ((TSLPRM_TOTAL_CHANNELS < 1) || (TSLPRM_TOTAL_CHANNELS > MAX_CHANNEL))
#error "TSLPRM_TOTAL_CHANNELS is out of range (1 .. MAX_CHANNEL)."
#endif

#if ((TSLPRM_TOTAL_BANKS < 1) || (TSLPRM_TOTAL_BANKS > MAX_LINROT))
#error "TSLPRM_TOTAL_BANKS is out of range (1 .. MAX_LINROT)."
#endif

#if ((TSLPRM_TOTAL_TOUCHKEYS < 0) || (TSLPRM_TOTAL_TOUCHKEYS > MAX_CHANNEL))
#error "TSLPRM_TOTAL_TOUCHKEYS is out of range (0 .. MAX_CHANNEL)."
#endif

#if ((TSLPRM_TOTAL_TOUCHKEYS_B < 0) || (TSLPRM_TOTAL_TOUCHKEYS_B > MAX_CHANNEL))
#error "TSLPRM_TOTAL_TOUCHKEYS_B is out of range (0 .. MAX_CHANNEL)."
#endif

#if ((TSLPRM_TOTAL_LINROTS < 0) || (TSLPRM_TOTAL_LINROTS > MAX_LINROT))
#error "TSLPRM_TOTAL_LINROTS is out of range (0 .. MAX_LINROT)."
#endif

#if ((TSLPRM_TOTAL_LINROTS_B < 0) || (TSLPRM_TOTAL_LINROTS_B > MAX_LINROT))
#error "TSLPRM_TOTAL_LINROTS_B is out of range (0 .. MAX_LINROT)."
#endif

#if ((TSLPRM_TOTAL_OBJECTS < 1) || (TSLPRM_TOTAL_OBJECTS > MAX_CHANNEL))
#error "TSLPRM_TOTAL_OBJECTS is out of range (1 .. MAX_CHANNEL)."
#endif

#if ((TSLPRM_TOTAL_TKEYS + TSLPRM_TOTAL_LNRTS) > MAX_CHANNEL)
#error "The Sum of TouchKeys and Linear/Rotary sensors exceeds MAX_CHANNEL."
#endif

//------------------------------------------------------------------------------

#ifndef TSLPRM_USE_SHIELD
#error "TSLPRM_USE_SHIELD is not defined."
#endif

#if ((TSLPRM_USE_SHIELD < 0) || (TSLPRM_USE_SHIELD > 1))
#error "TSLPRM_USE_SHIELD is out of range (0 .. 1)."
#endif

//------------------------------------------------------------------------------

#ifndef TSLPRM_IODEF
#error "TSLPRM_IODEF is not defined."
#endif

#if ((TSLPRM_IODEF < 0) || (TSLPRM_IODEF > 1))
#error "TSLPRM_IODEF is out of range (0 .. 1)."
#endif

//------------------------------------------------------------------------------

#if defined(STM32L1XX_MD) || defined(TSLPRM_STM32L1XX_SW_ACQ)

#ifndef TSLPRM_DELAY_TRANSFER
#error "TSLPRM_DELAY_TRANSFER is not defined."
#endif

#if ((TSLPRM_DELAY_TRANSFER < 0) || (TSLPRM_DELAY_TRANSFER > 65535))
#error "TSLPRM_DELAY_TRANSFER is out of range (0 .. 65535)."
#endif

#endif

//------------------------------------------------------------------------------

#if defined(STM32L1XX_MD) || defined(TSLPRM_STM32L1XX_SW_ACQ)

#ifndef TSLPRM_USE_SPREAD_SPECTRUM
#error "TSLPRM_USE_SPREAD_SPECTRUM is not defined."
#endif

#if ((TSLPRM_USE_SPREAD_SPECTRUM < 0) || (TSLPRM_USE_SPREAD_SPECTRUM > 1))
#error "TSLPRM_USE_SPREAD_SPECTRUM is out of range (0 .. 1)."
#endif

#endif

//------------------------------------------------------------------------------

#if defined(STM32L1XX_MD) || defined(TSLPRM_STM32L1XX_SW_ACQ)

#ifndef TSLPRM_SPREAD_MIN
#error "TSLPRM_SPREAD_MIN is not defined."
#endif

#if (TSLPRM_USE_SPREAD_SPECTRUM == 1)
#if ((TSLPRM_SPREAD_MIN < 1) || (TSLPRM_SPREAD_MIN >= TSLPRM_SPREAD_MAX))
#error "TSLPRM_SPREAD_MIN is out of range (1 .. TSLPRM_SPREAD_MAX-1)."
#endif
#endif

#endif

//------------------------------------------------------------------------------

#if defined(STM32L1XX_MD) || defined(TSLPRM_STM32L1XX_SW_ACQ)

#ifndef TSLPRM_SPREAD_MAX
#error "TSLPRM_SPREAD_MAX is not defined."
#endif

#if (TSLPRM_USE_SPREAD_SPECTRUM == 1)
#if ((TSLPRM_SPREAD_MAX > 255) || (TSLPRM_SPREAD_MAX <= TSLPRM_SPREAD_MIN))
#error "TSLPRM_SPREAD_MAX is out of range (TSLPRM_SPREAD_MIN+1 .. 255)."
#endif
#endif

#endif

//------------------------------------------------------------------------------

#if (defined(STM32L1XX_HD) || defined(STM32L1XX_XL)) && !defined(TSLPRM_STM32L1XX_SW_ACQ)

#ifndef TSLPRM_CT_PERIOD
#error "TSLPRM_CT_PERIOD is not defined."
#endif

#endif

//------------------------------------------------------------------------------

#if (defined(STM32L1XX_HD) || defined(STM32L1XX_XL)) && !defined(TSLPRM_STM32L1XX_SW_ACQ)

#ifndef TSLPRM_TIMER_FREQ
#error "TSLPRM_TIMER_FREQ is not defined."
#endif

#endif

//------------------------------------------------------------------------------

#if (defined(STM32L1XX_HD) || defined(STM32L1XX_XL)) && !defined(TSLPRM_STM32L1XX_SW_ACQ)

#define TMP_RELOAD ((TSLPRM_CT_PERIOD * TSLPRM_TIMER_FREQ)/2)

#if ((TMP_RELOAD < 4) || (TMP_RELOAD > 16534))
#error "The calculated Timer RELOAD value is out of range (4 .. 65534)."
#endif

#if ((TMP_RELOAD % 2) != (0))
#error "The calculated Timer RELOAD value is odd and must be even."
#endif

#endif

//------------------------------------------------------------------------------

#if (defined(STM32L1XX_HD) || defined(STM32L1XX_XL)) && defined(TSLPRM_STM32L1XX_SW_ACQ)

#ifndef TSLPRM_PROTECT_IO_ACCESS
#error "TSLPRM_PROTECT_IO_ACCESS is not defined."
#endif

#if ((TSLPRM_PROTECT_IO_ACCESS < 0) || (TSLPRM_PROTECT_IO_ACCESS > 1))
#error "TSLPRM_PROTECT_IO_ACCESS is out of range (0 .. 1)."
#endif

#endif

//------------------------------------------------------------------------------

#if (defined(STM32L1XX_HD) || defined(STM32L1XX_XL)) && defined(TSLPRM_STM32L1XX_SW_ACQ)

#ifndef TSLPRM_USE_GPIOA
#error "TSLPRM_USE_GPIOA is not defined."
#endif

#if ((TSLPRM_USE_GPIOA < 0) || (TSLPRM_USE_GPIOA > 1))
#error "TSLPRM_USE_GPIOA is out of range (0 .. 1)."
#endif

#ifndef TSLPRM_USE_GPIOB
#error "TSLPRM_USE_GPIOB is not defined."
#endif

#if ((TSLPRM_USE_GPIOB < 0) || (TSLPRM_USE_GPIOB > 1))
#error "TSLPRM_USE_GPIOB is out of range (0 .. 1)."
#endif

#ifndef TSLPRM_USE_GPIOC
#error "TSLPRM_USE_GPIOC is not defined."
#endif

#if ((TSLPRM_USE_GPIOC < 0) || (TSLPRM_USE_GPIOC > 1))
#error "TSLPRM_USE_GPIOC is out of range (0 .. 1)."
#endif

#ifndef TSLPRM_USE_GPIOF
#error "TSLPRM_USE_GPIOA is not defined."
#endif

#if ((TSLPRM_USE_GPIOF < 0) || (TSLPRM_USE_GPIOF > 1))
#error "TSLPRM_USE_GPIOF is out of range (0 .. 1)."
#endif

#ifndef TSLPRM_USE_GPIOG
#error "TSLPRM_USE_GPIOG is not defined."
#endif

#if ((TSLPRM_USE_GPIOG < 0) || (TSLPRM_USE_GPIOG > 1))
#error "TSLPRM_USE_GPIOG is out of range (0 .. 1)."
#endif

#endif

//------------------------------------------------------------------------------

#if defined(STM32L1XX_MD)

#ifndef TSLPRM_PROTECT_IO_ACCESS
#error "TSLPRM_PROTECT_IO_ACCESS is not defined."
#endif

#if ((TSLPRM_PROTECT_IO_ACCESS < 0) || (TSLPRM_PROTECT_IO_ACCESS > 1))
#error "TSLPRM_PROTECT_IO_ACCESS is out of range (0 .. 1)."
#endif

#endif

//------------------------------------------------------------------------------

#if defined(STM32L1XX_MD)

#ifndef TSLPRM_USE_GPIOA
#error "TSLPRM_USE_GPIOA is not defined."
#endif

#if ((TSLPRM_USE_GPIOA < 0) || (TSLPRM_USE_GPIOA > 1))
#error "TSLPRM_USE_GPIOA is out of range (0 .. 1)."
#endif

#ifndef TSLPRM_USE_GPIOB
#error "TSLPRM_USE_GPIOB is not defined."
#endif

#if ((TSLPRM_USE_GPIOB < 0) || (TSLPRM_USE_GPIOB > 1))
#error "TSLPRM_USE_GPIOB is out of range (0 .. 1)."
#endif

#ifndef TSLPRM_USE_GPIOC
#error "TSLPRM_USE_GPIOC is not defined."
#endif

#if ((TSLPRM_USE_GPIOC < 0) || (TSLPRM_USE_GPIOC > 1))
#error "TSLPRM_USE_GPIOC is out of range (0 .. 1)."
#endif

#ifndef TSLPRM_USE_GPIOF
#error "TSLPRM_USE_GPIOA is not defined."
#endif

#if ((TSLPRM_USE_GPIOF < 0) || (TSLPRM_USE_GPIOF > 1))
#error "TSLPRM_USE_GPIOF is out of range (0 .. 1)."
#endif

#ifndef TSLPRM_USE_GPIOG
#error "TSLPRM_USE_GPIOG is not defined."
#endif

#if ((TSLPRM_USE_GPIOG < 0) || (TSLPRM_USE_GPIOG > 1))
#error "TSLPRM_USE_GPIOG is out of range (0 .. 1)."
#endif

#endif

//------------------------------------------------------------------------------

#if (defined(STM32L1XX_HD) || defined(STM32L1XX_XL)) && !defined(TSLPRM_STM32L1XX_SW_ACQ)

#ifndef TSLPRM_CT_PERIOD
#error "TSLPRM_CT_PERIOD is not defined."
#endif

#endif

//------------------------------------------------------------------------------

#if (defined(STM32L1XX_HD) || defined(STM32L1XX_XL)) && !defined(TSLPRM_STM32L1XX_SW_ACQ)

#ifndef TSLPRM_TIMER_FREQ
#error "TSLPRM_TIMER_FREQ is not defined."
#endif

#endif

//------------------------------------------------------------------------------

#if (defined(STM32L1XX_HD) || defined(STM32L1XX_XL)) && !defined(TSLPRM_STM32L1XX_SW_ACQ)

#define TMP_RELOAD ((TSLPRM_CT_PERIOD * TSLPRM_TIMER_FREQ)/2)

#if ((TMP_RELOAD < 4) || (TMP_RELOAD > 16534))
#error "The calculated Timer RELOAD value is out of range (4 .. 65534)."
#endif

#if ((TMP_RELOAD % 2) != (0))
#error "The calculated Timer RELOAD value is odd and must be even."
#endif

#endif

//------------------------------------------------------------------------------

#if defined(STM32L1XX_MDP) && defined(TSLPRM_STM32L1XX_SW_ACQ)

#ifndef TSLPRM_PROTECT_IO_ACCESS
#error "TSLPRM_PROTECT_IO_ACCESS is not defined."
#endif

#if ((TSLPRM_PROTECT_IO_ACCESS < 0) || (TSLPRM_PROTECT_IO_ACCESS > 1))
#error "TSLPRM_PROTECT_IO_ACCESS is out of range (0 .. 1)."
#endif

#endif

//------------------------------------------------------------------------------

#if defined(STM32L1XX_MDP) && defined(TSLPRM_STM32L1XX_SW_ACQ)

#ifndef TSLPRM_USE_GPIOA
#error "TSLPRM_USE_GPIOA is not defined."
#endif

#if ((TSLPRM_USE_GPIOA < 0) || (TSLPRM_USE_GPIOA > 1))
#error "TSLPRM_USE_GPIOA is out of range (0 .. 1)."
#endif

#ifndef TSLPRM_USE_GPIOB
#error "TSLPRM_USE_GPIOB is not defined."
#endif

#if ((TSLPRM_USE_GPIOB < 0) || (TSLPRM_USE_GPIOB > 1))
#error "TSLPRM_USE_GPIOB is out of range (0 .. 1)."
#endif

#ifndef TSLPRM_USE_GPIOC
#error "TSLPRM_USE_GPIOC is not defined."
#endif

#if ((TSLPRM_USE_GPIOC < 0) || (TSLPRM_USE_GPIOC > 1))
#error "TSLPRM_USE_GPIOC is out of range (0 .. 1)."
#endif

#ifndef TSLPRM_USE_GPIOF
#error "TSLPRM_USE_GPIOA is not defined."
#endif

#if ((TSLPRM_USE_GPIOF < 0) || (TSLPRM_USE_GPIOF > 1))
#error "TSLPRM_USE_GPIOF is out of range (0 .. 1)."
#endif

#ifndef TSLPRM_USE_GPIOG
#error "TSLPRM_USE_GPIOG is not defined."
#endif

#if ((TSLPRM_USE_GPIOG < 0) || (TSLPRM_USE_GPIOG > 1))
#error "TSLPRM_USE_GPIOG is out of range (0 .. 1)."
#endif

#endif

#endif /* __TSL_CHECK_CONFIG_STM32L1XX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
