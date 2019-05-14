/**
  ******************************************************************************
  * @file    stm32l4r9i_eval_ospi_ram.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32l4r9i_eval_ospi_ram.c driver.
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
#ifndef __STM32L4R9I_EVAL_OSPI_RAM_H
#define __STM32L4R9I_EVAL_OSPI_RAM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "../Components/iss66wvh8m8/iss66wvh8m8.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L4R9I_EVAL
  * @{
  */

/** @addtogroup STM32L4R9I_EVAL_OSPI_RAM
  * @{
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup STM32L4R9I_EVAL_OSPI_RAM_Exported_Constants Exported Constants
  * @{
  */
/* OSPI Error codes */
#define OSPI_RAM_OK            ((uint8_t)0x00)
#define OSPI_RAM_ERROR         ((uint8_t)0x01)

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @defgroup STM32L4R9I_EVAL_OSPI_RAM_Exported_Functions Exported Functions
  * @{
  */
uint8_t BSP_OSPI_RAM_Init                  (void);
uint8_t BSP_OSPI_RAM_DeInit                (void);
uint8_t BSP_OSPI_RAM_Read                  (uint8_t* pData, uint32_t ReadAddr, uint32_t Size);
uint8_t BSP_OSPI_RAM_Read_DMA              (uint8_t* pData, uint32_t ReadAddr, uint32_t Size);
uint8_t BSP_OSPI_RAM_Write                 (uint8_t* pData, uint32_t WriteAddr, uint32_t Size);
uint8_t BSP_OSPI_RAM_Write_DMA             (uint8_t* pData, uint32_t WriteAddr, uint32_t Size);
uint8_t BSP_OSPI_RAM_EnableMemoryMappedMode(void);
uint8_t BSP_OSPI_RAM_EnterDeepPowerDown    (void);
uint8_t BSP_OSPI_RAM_LeaveDeepPowerDown    (void);
void    BSP_OSPI_RAM_DMA_IRQHandler        (void);
void    BSP_OSPI_RAM_IRQHandler            (void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32L4R9I_EVAL_OSPI__RAM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
