/**
  ******************************************************************************
  * @file    OPAMP/OPAMP_CALIBRATION/Inc/main.h
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"
#include "stm32l4xx_ll_opamp.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor COMPx instance under use and associated
   resources */

/* ## Definition of OPAMP related resources ################################### */
/* Definition of OPAMPx clock resources */
#define OPAMPx                            OPAMP2
#define OPAMPx_CLK_ENABLE()               __HAL_RCC_OPAMP_CLK_ENABLE()
#define OPAMPx_INPUT_NONINVERTING_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define OPAMPx_OUTPUT_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

#define OPAMPx_FORCE_RESET()              __HAL_RCC_OPAMP_FORCE_RESET()
#define OPAMPx_RELEASE_RESET()            __HAL_RCC_OPAMP_RELEASE_RESET()
   
/* Definition of OPAMPx channels pins */
#define OPAMP2_NON_INVERTING_INPUT_PIN        GPIO_PIN_6
#define OPAMP2_NON_INVERTING_INPUT_GPIO_PORT  GPIOA
#define OPAMP2_OUTPUT_PIN                     GPIO_PIN_0
#define OPAMP2_OUTPUT_GPIO_PORT               GPIOB
   
/* ## Definition of DAC related resources ################################### */
/* Definition of DACx clock resources */
#define DACx                            DAC
#define DACx_CLK_ENABLE()               __DAC1_CLK_ENABLE()
#define DACx_CHANNEL_GPIO_CLK_ENABLE()  __GPIOA_CLK_ENABLE()

#define DACx_FORCE_RESET()              __HAL_RCC_DAC1_FORCE_RESET()
#define DACx_RELEASE_RESET()            __HAL_RCC_DAC1_RELEASE_RESET()

/* Definition of DACx channels */
#define DACx_CHANNELa                   DAC_CHANNEL_1

/* Definition of DACx channels pins */
#define DACx_CHANNELa_PIN               GPIO_PIN_4
#define DACx_CHANNELa_GPIO_PORT         GPIOA

/* Definition of DACx DMA resources */
#define DACx_CHANNELa_DMA_CLK_ENABLE()           __DMA1_CLK_ENABLE()
#define DACx_CHANNELa_DMA                        DMA1_Channel3
#define DACx_CHANNELa_DMA_REQUEST                DMA_REQUEST_6

#define DACx_CHANNELa_DMA_IRQn                   DMA1_Channel3_IRQn
#define DACx_CHANNELa_DMA_IRQHandler             DMA1_Channel3_IRQHandler


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

