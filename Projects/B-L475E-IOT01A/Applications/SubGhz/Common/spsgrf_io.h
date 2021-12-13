/**
******************************************************************************
  * @file    spsgrf_io.h
  * @author  MCD Application Team
  * @brief   This file provides code for the configuration of all  GPIO pins 
             and the SPI instance used for Radio inetrface.
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
#ifndef __SPSGRF_IO_H
#define __SPSGRF_IO_H

#ifdef __cplusplus
extern "C" {
#endif
  
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx.h"
  
/**
 * @addtogroup SPSGRF_IO
 * @{
 */

/**
 * @defgroup SPSGR_IO_GPIO_Exported_Types             SPSGR_GPIO Exported Types
 * @{
 */
  
/* Exported types ------------------------------------------------------------*/
  /* MCU GPIO pin working mode for GPIO */
typedef enum                                                                                          
{
    RADIO_MODE_GPIO_IN  = 0x00,   /*!< Work as GPIO input */
    RADIO_MODE_EXTI_IN,           /*!< Work as EXTI */
    RADIO_MODE_GPIO_OUT,          /*!< Work as GPIO output */
}RadioGpioMode;  

 /* MCU GPIO pin enumeration for GPIO */
typedef enum 
{
  RADIO_GPIO_0     = 0x00, /*!< GPIO_0 selected */
  RADIO_GPIO_1     = 0x01, /*!< GPIO_1 selected */
  RADIO_GPIO_2     = 0x02, /*!< GPIO_2 selected */
  RADIO_GPIO_3     = 0x03, /*!< GPIO_3 selected */
  RADIO_GPIO_SDN   = 0x04, /*!< GPIO_SDN selected */
} RadioGpioPin;   

/**
 * @}
 */
  
/**
 * @defgroup SPSGR_IO_GPIO_Exported_Macros             SPSGR_GPIO Exported Macros
 * @{
 */  
 
/* Exported macro ------------------------------------------------------------*/
 /* MCU GPIO pin working mode for GPIO */
#define IS_RADIO_GPIO_MODE(MODE) (((MODE) == RADIO_MODE_GPIO_IN) || \
                               ((MODE) == RADIO_MODE_EXTI_IN) || \
                               ((MODE) == RADIO_MODE_GPIO_OUT))

/* Number of Arduino pins used for RADIO GPIO interface */
#define RADIO_GPIO_NUMBER    ((uint8_t)5)

/* MCU GPIO pin enumeration for GPIO */
#define IS_RADIO_GPIO_PIN(PIN)   (((PIN) == RADIO_GPIO_0) || \
                               ((PIN) == RADIO_GPIO_1) || \
                               ((PIN) == RADIO_GPIO_2) || \
                               ((PIN) == RADIO_GPIO_3) || \
                               ((PIN) == RADIO_GPIO_SDN))

/**
 * @}
 */

/**
 * @defgroup SPSGR_IO_GPIO_Exported_Constants             SPSGR_GPIO Exported Constants
 * @{
 */                               

/* Define for RADIO board */
#if !defined (USE_SPIRIT1_DEFAULT)
 #define USE_SPIRIT1_DEFAULT
#endif  

/* @defgroup Radio_Gpio_config_Define */
/*NOTE: GPIO0, GPIO1, GPIO2 of SPIRIT1 are not used in the expansion board */
#define RADIO_GPIO_0_PORT                          0
#define RADIO_GPIO_0_PIN                           0
#define RADIO_GPIO_0_CLOCK_ENABLE()                __HAL_RCC_GPIOC_CLK_ENABLE()
#define RADIO_GPIO_0_CLOCK_DISABLE()               __HAL_RCC_GPIOC_CLK_DISABLE()   
#define RADIO_GPIO_0_SPEED                         0
#define RADIO_GPIO_0_PUPD                          0
#define RADIO_GPIO_0_EXTI_LINE                     0
#define RADIO_GPIO_0_EXTI_MODE                     0
#define RADIO_GPIO_0_EXTI_PREEMPTION_PRIORITY      2
#define RADIO_GPIO_0_EXTI_SUB_PRIORITY             2

#define RADIO_GPIO_0_EXTI_IRQN                     EXTI0_IRQn
#define RADIO_GPIO_0_EXTI_IRQ_HANDLER              EXTI0_IRQHandler


/******************************************************************************/

#define RADIO_GPIO_1_PORT                          0
#define RADIO_GPIO_1_PIN                           0
#define RADIO_GPIO_1_CLOCK_ENABLE()                __HAL_RCC_GPIOB_CLK_ENABLE()
#define RADIO_GPIO_1_CLOCK_DISABLE()               __HAL_RCC_GPIOB_CLK_DISABLE()   
#define RADIO_GPIO_1_SPEED                         0
#define RADIO_GPIO_1_PUPD                          0
#define RADIO_GPIO_1_EXTI_LINE                     0
#define RADIO_GPIO_1_EXTI_MODE                     0
#define RADIO_GPIO_1_EXTI_PREEMPTION_PRIORITY      2
#define RADIO_GPIO_1_EXTI_SUB_PRIORITY             2

#define RADIO_GPIO_1_EXTI_IRQN                     EXTI1_IRQn
#define RADIO_GPIO_1_EXTI_IRQ_HANDLER              EXTI1_IRQHandler


/******************************************************************************/

#define RADIO_GPIO_2_PORT                          0
#define RADIO_GPIO_2_PIN                           0
#define RADIO_GPIO_2_CLOCK_ENABLE()                __HAL_RCC_GPIOA_CLK_ENABLE()
#define RADIO_GPIO_2_CLOCK_DISABLE()               __HAL_RCC_GPIOA_CLK_DISABLE()   
#define RADIO_GPIO_2_SPEED                         0
#define RADIO_GPIO_2_PUPD                          0
#define RADIO_GPIO_2_EXTI_LINE                     0
#define RADIO_GPIO_2_EXTI_MODE                     0 
#define RADIO_GPIO_2_EXTI_PREEMPTION_PRIORITY      2
#define RADIO_GPIO_2_EXTI_SUB_PRIORITY             2

#define RADIO_GPIO_2_EXTI_IRQN                     EXTI4_IRQn
#define RADIO_GPIO_2_EXTI_IRQ_HANDLER              EXTI4_IRQHandler


/******************************************************************************/

#if defined (USE_SPIRIT1_DEFAULT)

#define RADIO_GPIO_3_PORT                          GPIOE
#define RADIO_GPIO_3_PIN                           GPIO_PIN_5
#define RADIO_GPIO_3_CLOCK_ENABLE()              __HAL_RCC_GPIOE_CLK_ENABLE()
#define RADIO_GPIO_3_CLOCK_DISABLE()             __HAL_RCC_GPIOE_CLK_DISABLE()   
#define RADIO_GPIO_3_SPEED                         GPIO_SPEED_FREQ_VERY_HIGH
#define RADIO_GPIO_3_PUPD                          GPIO_NOPULL
#define RADIO_GPIO_3_EXTI_LINE                     GPIO_PIN_5
#define RADIO_GPIO_3_EXTI_MODE                     GPIO_MODE_IT_FALLING
#define RADIO_GPIO_3_EXTI_PREEMPTION_PRIORITY      2
#define RADIO_GPIO_3_EXTI_SUB_PRIORITY             2

#define RADIO_GPIO_3_EXTI_IRQN                     EXTI9_5_IRQn 
#define RADIO_GPIO_3_EXTI_IRQ_HANDLER              EXTI9_5_IRQHandler

#else

#define RADIO_GPIO_3_PORT                        0
#define RADIO_GPIO_3_PIN                         0
#define RADIO_GPIO_3_CLOCK_ENABLE()                __HAL_RCC_GPIOA_CLK_ENABLE()
#define RADIO_GPIO_3_CLOCK_DISABLE()               __HAL_RCC_GPIOA_CLK_DISABLE() 
#define RADIO_GPIO_3_SPEED                       0
#define RADIO_GPIO_3_PUPD                        0
#define RADIO_GPIO_3_EXTI_LINE                   0
#define RADIO_GPIO_3_EXTI_MODE                   0
#define RADIO_GPIO_3_EXTI_PREEMPTION_PRIORITY    2
#define RADIO_GPIO_3_EXTI_SUB_PRIORITY           2

#define RADIO_GPIO_3_EXTI_IRQN                     EXTI0_IRQn 
#define RADIO_GPIO_3_EXTI_IRQ_HANDLER              EXTI0_IRQHandler

#endif

/******************************************************************************/

#define RADIO_GPIO_SDN_PORT                        GPIOB
#define RADIO_GPIO_SDN_PIN                         GPIO_PIN_15
#define RADIO_GPIO_SDN_CLOCK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define RADIO_GPIO_SDN_CLOCK_DISABLE()           __HAL_RCC_GPIOB_CLK_DISABLE()
#define RADIO_GPIO_SDN_SPEED                       GPIO_SPEED_FREQ_VERY_HIGH
#define RADIO_GPIO_SDN_PUPD                        GPIO_PULLUP

#define RADIO_GPIO_IRQ          RADIO_GPIO_3
#define SPIRIT_GPIO_IRQ         SPIRIT_GPIO_3

/**
 * @}
 */

 
/**
 * @defgroup SPSGRF_IO_SPI_Exported_FunctionPrototypes             SPSGRF_IO_SPI Exported FunctionPrototypes
 * @{
 */
 
/* Radio SPI Exported functions ------------------------------------------------------- */
void RadioSpiInit(void);
uint8_t * RadioSpiWriteRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer);
uint8_t * RadioSpiReadRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer);
uint8_t * RadioSpiCommandStrobes(uint8_t cCommandCode);
uint8_t * RadioSpiWriteFifo(uint8_t cNbBytes, uint8_t* pcBuffer);
uint8_t * RadioSpiReadFifo(uint8_t cNbBytes, uint8_t* pcBuffer);

/**
 * @}
 */

/**
 * @defgroup SPSGR_IO_GPIO_Exported_Functions             SPSGR_GPIO Exported Functions
 * @{
 */
 
/* GPIO Exported functions ------------------------------------------------------- */
FlagStatus RadioGpioGetLevel(RadioGpioPin xGpio);
void RadioGpioSetLevel(RadioGpioPin xGpio, GPIO_PinState xState);
void RadioEnterShutdown(void);
void RadioExitShutdown(void);
FlagStatus RadioCheckShutdown(void);
void RadioGpioInit(RadioGpioPin xGpio, RadioGpioMode xGpioMode);
void RadioGpioInterruptCmd(RadioGpioPin xGpio, uint8_t nPreemption, uint8_t nSubpriority, FunctionalState xNewState);

/**
 * @}
 */

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /*__SPSGRF_IO_H */

