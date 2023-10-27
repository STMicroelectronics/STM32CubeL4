/**
  @page OSPI_RAM_MemoryMapped OSPI PSRAM memory mapped example

  @verbatim
  ******************************************************************************
  * @file    OSPI/OSPI_RAM_MemoryMapped/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the OSPI PSRAM memory mapped mode example.
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
  @endverbatim

@par Example Description

How to use a OSPI PSRAM memory in memory-mapped mode.

This example describes how to write and read data in memory-mapped mode in a OSPI
PSRAM memory and compare the result in an infinite loop.

LED_GREEN toggles each time the data have been checked
LED_BLUE is on as soon as a data is wrong or as soon as an error is returned by HAL API

In this example, HCLK is configured at 110 MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents

  - OSPI/OSPI_RAM_MemoryMapped/Src/main.c                 Main program
  - OSPI/OSPI_RAM_MemoryMapped/Src/system_stm32l4xx.c     STM32L4xx system clock configuration file
  - OSPI/OSPI_RAM_MemoryMapped/Src/stm32l4xx_it.c         Interrupt handlers
  - OSPI/OSPI_RAM_MemoryMapped/Src/stm32l4xx_hal_msp.c    HAL MSP module
  - OSPI/OSPI_RAM_MemoryMapped/Inc/main.h                 Main program header file
  - OSPI/OSPI_RAM_MemoryMapped/Inc/stm32l4xx_hal_conf.h   HAL Configuration file
  - OSPI/OSPI_RAM_MemoryMapped/Inc/stm32l4xx_it.h         Interrupt handlers header file


@par Hardware and Software environment

  - This example runs on STM32L4P5xx devices

  - This example has been tested with STM32L4P5G-Discovery (MB1535) board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
