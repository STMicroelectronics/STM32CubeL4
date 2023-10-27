/**
  @page OSPI_RAM_ExecuteInPlace OSPI PSRAM execute in place example

  @verbatim
  ******************************************************************************
  * @file    OSPI/OSPI_RAM_ExecuteInPlace/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the code execution from OSPI PSRAM memory example.
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

How to execute code from OCTOSPI memory after code loading.

This example describes how to execute a part of the code from a OSPI PSRAM memory. To do this,
a section is created where the function is stored.

At the startup, the OSPI PSRAM is configured in memory-mapped mode and the data are copied from the initialization
section of the flash to the OSPI PSRAM memory. And finally the code in executed in an infinite loop.

LED1 and LED2 toggle in a forever loop.
Blue LED2 is on as soon as an error is returned by HAL API, and LED don't toggle anymore.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents

  - OSPI/OSPI_RAM_ExecuteInPlace/Src/main.c                 Main program
  - OSPI/OSPI_RAM_ExecuteInPlace/Src/system_stm32l4xx.c     STM32L4xx system clock configuration file
  - OSPI/OSPI_RAM_ExecuteInPlace/Src/stm32l4xx_it.c         Interrupt handlers
  - OSPI/OSPI_RAM_ExecuteInPlace/Src/stm32l4xx_hal_msp.c    HAL MSP module
  - OSPI/OSPI_RAM_ExecuteInPlace/Inc/main.h                 Main program header file
  - OSPI/OSPI_RAM_ExecuteInPlace/Inc/stm32l4xx_hal_conf.h   HAL Configuration file
  - OSPI/OSPI_RAM_ExecuteInPlace/Inc/stm32l4xx_it.h         Interrupt handlers header file


@par Hardware and Software environment

  - This example runs on STM32L4P5xx devices

  - This example has been tested with STM32L4P5G-Discovery (MB1535) board and can be
    easily tailored to any other supported device and development board.

  - A specific region for the OSPI PSRAM memory has been added in the scatter file
   (stm32l4p5xx_flash.icf for IAR toolchain, Project.sct for Keil toolchain)
   in order to map functions in this memory

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
