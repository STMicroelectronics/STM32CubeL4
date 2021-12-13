/**
  ******************************************************************************
  * @file    Examples_LL/ADC/ADC_SingleConversion_TriggerTimer_DMA_LP/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_exti.h"
#include "stm32l4xx_ll_adc.h"
#include "stm32l4xx_ll_dma.h"
#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_pwr.h"
#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Define used to enable time-out management*/
#define USE_TIMEOUT       0

/**
  * @brief LED3
  */
#define LED3_PIN                           LL_GPIO_PIN_3
#define LED3_GPIO_PORT                     GPIOB
#define LED3_GPIO_CLK_ENABLE()             LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB)

/**
  * @brief Toggle periods for various blinking modes
  */
#define LED_BLINK_FAST  200
#define LED_BLINK_SLOW  500
#define LED_BLINK_ERROR 1000

/**
  * @brief Key push-button
  */
#define USER_BUTTON_PIN                         LL_GPIO_PIN_12
#define USER_BUTTON_GPIO_PORT                   GPIOA
#define USER_BUTTON_GPIO_CLK_ENABLE()           LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#define USER_BUTTON_EXTI_LINE                   LL_EXTI_LINE_12
#define USER_BUTTON_EXTI_IRQn                   EXTI15_10_IRQn
#define USER_BUTTON_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(USER_BUTTON_EXTI_LINE)
#define USER_BUTTON_EXTI_FALLING_TRIG_ENABLE()  LL_EXTI_EnableFallingTrig_0_31(USER_BUTTON_EXTI_LINE)


#define USER_BUTTON_SYSCFG_SET_EXTI()           do {                                                                     \
                                                  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);                  \
                                                  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE12);  \
                                                } while(0)
#define USER_BUTTON_IRQHANDLER                  EXTI15_10_IRQHandler


/* Definitions of example options */
#define OPTION_PWR_OPTIM_CPU_LOW_POWER_MODE_SLEEP     /* Set CPU in low-power mode "sleep" in idle phases (out of ADC, DMA or TIM interruption threads). */
#define OPTION_PWR_OPTIM_ADC_DISABLE                  /* Set ADC in the deepest low-power mode possible in idle phases (between ADC conversions). Relevant for ADC conversion triggers at low frequency (below 1kHz for configuration of this example): less ADC idle current consumption, but more CPU activity to manage ADC activation. */
//#define OPTION_MONITOR_CPU_ACTIVITY                   /* Monitor CPU activity: when CPU is active (used by ADC, DMA or TIM interruption threads), turn LED on. This induce extra power consumption. */
//#define OPTION_THRESHOLD_DEBUG                        /* Monitor converted values: when value is higher than 2048, turn LED on. This induce extra power consumption. To be used for debug. */

#if defined(OPTION_MONITOR_CPU_ACTIVITY) && defined(OPTION_THRESHOLD_DEBUG) 
  #error Compilation switches OPTION_MONITOR_CPU_ACTIVITY and OPTION_THRESHOLD_DEBUG cannot be used at the same time
#endif    


/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
/* IRQ Handler treatment */
void UserButton_Callback(void);
void AdcDmaTransferComplete_Callback(void);
void AdcDmaTransferHalf_Callback(void);
void AdcDmaTransferError_Callback(void);
void AdcGrpRegularOverrunError_Callback(void);
#if defined(OPTION_PWR_OPTIM_ADC_DISABLE)
void AdcGrpRegularUnitaryConvComplete_Callback(void);
void TimerUpdate_Callback(void);
#endif /* OPTION_PWR_OPTIM_ADC_DISABLE */

#endif /* __MAIN_H */

