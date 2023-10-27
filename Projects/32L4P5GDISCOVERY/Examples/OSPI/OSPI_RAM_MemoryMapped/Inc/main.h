/**
  ******************************************************************************
  * @file    OSPI/OSPI_RAM_MemoryMapped/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
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
#include "stm32l4p5g_discovery.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* OSPI commands */
#define READ_CMD_SRAM                           0x00
#define WRITE_CMD_SRAM                          0x80
#define READ_REG_CMD_SRAM                       0x40
#define WRITE_REG_CMD_SRAM                      0xC0

/* Default dummy clocks cycles */
#define DUMMY_CLOCK_CYCLES_SRAM_READ               5
#define DUMMY_CLOCK_CYCLES_SRAM_WRITE              4

/* OSPI pins definition */
#define OSPI_P1_CS_PIN                   GPIO_PIN_11
#define OSPI_P1_CLK_PIN                  GPIO_PIN_10
#define OSPI_P1_NCLK_PIN                 GPIO_PIN_9
#define OSPI_P1_D0_PIN                   GPIO_PIN_12
#define OSPI_P1_D1_PIN                   GPIO_PIN_13
#define OSPI_P1_D2_PIN                   GPIO_PIN_7
#define OSPI_P1_D3_PIN                   GPIO_PIN_6
#define OSPI_P1_D4_PIN                   GPIO_PIN_4
#define OSPI_P1_D5_PIN                   GPIO_PIN_5
#define OSPI_P1_D6_PIN                   GPIO_PIN_3
#define OSPI_P1_D7_PIN                   GPIO_PIN_7
#define OSPI_P1_DQS_PIN                  GPIO_PIN_6

#define OSPI_P2_CS_PIN                   GPIO_PIN_12
#define OSPI_P2_CLK_PIN                  GPIO_PIN_4
#define OSPI_P2_NCLK_PIN                 GPIO_PIN_5
#define OSPI_P2_D0_PIN                   GPIO_PIN_0
#define OSPI_P2_D1_PIN                   GPIO_PIN_1
#define OSPI_P2_D2_PIN                   GPIO_PIN_2
#define OSPI_P2_D3_PIN                   GPIO_PIN_3
#define OSPI_P2_D4_PIN                   GPIO_PIN_0
#define OSPI_P2_D5_PIN                   GPIO_PIN_1
#define OSPI_P2_D6_PIN                   GPIO_PIN_9
#define OSPI_P2_D7_PIN                   GPIO_PIN_10
#define OSPI_P2_DQS_PIN                  GPIO_PIN_12

#define OSPI_P1_CS_GPIO_PORT             GPIOE
#define OSPI_P1_CLK_GPIO_PORT            GPIOE
#define OSPI_P1_NCLK_GPIO_PORT           GPIOE
#define OSPI_P1_D0_GPIO_PORT             GPIOE
#define OSPI_P1_D1_GPIO_PORT             GPIOE
#define OSPI_P1_D2_GPIO_PORT             GPIOA
#define OSPI_P1_D3_GPIO_PORT             GPIOA
#define OSPI_P1_D4_GPIO_PORT             GPIOD
#define OSPI_P1_D5_GPIO_PORT             GPIOD
#define OSPI_P1_D6_GPIO_PORT             GPIOC
#define OSPI_P1_D7_GPIO_PORT             GPIOD
#define OSPI_P1_DQS_GPIO_PORT            GPIOG

#define OSPI_P2_CS_GPIO_PORT             GPIOG
#define OSPI_P2_CLK_GPIO_PORT            GPIOF
#define OSPI_P2_NCLK_GPIO_PORT           GPIOF
#define OSPI_P2_D0_GPIO_PORT             GPIOF
#define OSPI_P2_D1_GPIO_PORT             GPIOF
#define OSPI_P2_D2_GPIO_PORT             GPIOF
#define OSPI_P2_D3_GPIO_PORT             GPIOF
#define OSPI_P2_D4_GPIO_PORT             GPIOG
#define OSPI_P2_D5_GPIO_PORT             GPIOG
#define OSPI_P2_D6_GPIO_PORT             GPIOG
#define OSPI_P2_D7_GPIO_PORT             GPIOG
#define OSPI_P2_DQS_GPIO_PORT            GPIOF


/* Octal PSRAM APS6408L-3OB-BA memory */
/* Size of the PSRAM */
#define OSPI_PSRAM_SIZE          23   /* 64 Mbits */
#define OSPI_PSRAM_INCR_SIZE     256

/* End address of the OSPI memory */
#define OSPI_PSRAM_END_ADDR      (1 << OSPI_PSRAM_SIZE)

/* Size of buffers */
#define BUFFERSIZE                  (COUNTOF(aTxBuffer) - 1)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)         (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

/* Definition for OSPI clock resources */
#define OSPI_P1_CS_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define OSPI_P1_CLK_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOE_CLK_ENABLE()
#define OSPI_P1_NCLK_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE()
#define OSPI_P1_D0_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define OSPI_P1_D1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define OSPI_P1_D2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define OSPI_P1_D3_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define OSPI_P1_D4_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI_P1_D5_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI_P1_D6_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define OSPI_P1_D7_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI_P1_DQS_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOG_CLK_ENABLE()

#define OSPI_P2_CS_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI_P2_CLK_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_P2_NCLK_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_P2_D0_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_P2_D1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_P2_D2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_P2_D3_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_P2_D4_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI_P2_D5_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI_P2_D6_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI_P2_D7_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI_P2_DQS_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()

#define OSPI_P1_CS_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOE_CLK_DISABLE()
#define OSPI_P1_CLK_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOE_CLK_DISABLE()
#define OSPI_P1_NCLK_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOE_CLK_DISABLE()
#define OSPI_P1_D0_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOE_CLK_DISABLE()
#define OSPI_P1_D1_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOE_CLK_DISABLE()
#define OSPI_P1_D2_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define OSPI_P1_D3_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define OSPI_P1_D4_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOD_CLK_DISABLE()
#define OSPI_P1_D5_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOD_CLK_DISABLE()
#define OSPI_P1_D6_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOC_CLK_DISABLE()
#define OSPI_P1_D7_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOD_CLK_DISABLE()
#define OSPI_P1_DQS_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOG_CLK_DISABLE()

#define OSPI_P2_CS_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOG_CLK_DISABLE()
#define OSPI_P2_CLK_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOF_CLK_DISABLE()
#define OSPI_P2_NCLK_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOF_CLK_DISABLE()
#define OSPI_P2_D0_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOF_CLK_DISABLE()
#define OSPI_P2_D1_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOF_CLK_DISABLE()
#define OSPI_P2_D2_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOF_CLK_DISABLE()
#define OSPI_P2_D3_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOF_CLK_DISABLE()
#define OSPI_P2_D4_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOG_CLK_DISABLE()
#define OSPI_P2_D5_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOG_CLK_DISABLE()
#define OSPI_P2_D6_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOG_CLK_DISABLE()
#define OSPI_P2_D7_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOG_CLK_DISABLE()
#define OSPI_P2_DQS_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOF_CLK_DISABLE()


/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

