/**
  ******************************************************************************
  * @file    COMP/COMP_PWMSignalControl/Inc/main.h
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
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor COMPx instance under use and associated
   resources */

/* ## Definition of COMPx related resources ################################# */
/* Definition for COMPx clock resources */
#define COMPx_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define COMPx_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOB_CLK_DISABLE()

#define COMPx_OUTPUT_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define COMPx_OUTPUT_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()

/* Definition for COMPx Channel Pin */
#define COMPx_PIN                      GPIO_PIN_2
#define COMPx_GPIO_PORT                GPIOB 

/* Definition for COMPx Output Channel */
#define COMPx_OUTPUT_GPIO_AFx          GPIO_AF12_COMP1
#define COMPx_OUTPUT_PIN               GPIO_PIN_0
#define COMPx_OUTPUT_GPIO_PORT         GPIOB 

/* Definition for COMPx's NVIC */
#define COMPx_IRQn                     COMP_IRQn
#define COMPx_IRQHandler               COMP_IRQHandler

/* ## Definition of TIM related resources ################################### */
/* Definition of TIMx clock resources */
#define TIMx                            TIM2
#define TIMx_CLK_ENABLE()               __HAL_RCC_TIM2_CLK_ENABLE()

#define TIMx_FORCE_RESET()              __HAL_RCC_TIM2_FORCE_RESET()
#define TIMx_RELEASE_RESET()            __HAL_RCC_TIM2_RELEASE_RESET()

#define TIMx_IRQn                       TIM2_IRQn
#define TIMx_IRQHandler                 TIM2_IRQHandler

#define TIMx_GPIO_AFx                   GPIO_AF1_TIM2

/* Definition for TIMx Channel Pin */
#define TIMx_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define TIMx_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOB_CLK_DISABLE()

#define TIMx_PIN                      GPIO_PIN_3
#define TIMx_GPIO_PORT                GPIOB 

/* Definition for TIMx ETR signal */
#define TIMx_ETR_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define TIMx_ETR_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOA_CLK_DISABLE()

#define TIMx_ETR_PIN                      GPIO_PIN_5
#define TIMx_ETR_GPIO_PORT                GPIOA 

#define TIMx_ETR_GPIO_AFx                 GPIO_AF2_TIM2


/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

