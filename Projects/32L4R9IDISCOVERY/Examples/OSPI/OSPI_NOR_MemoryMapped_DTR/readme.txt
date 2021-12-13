/**
  @page OSPI_NOR_MemoryMapped_DTR OSPI NOR memory mapped example
  
  @verbatim
  ******************************************************************************
  * @file    OSPI/OSPI_NOR_MemoryMapped_DTR/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the OSPI NOR memory mapped mode example.
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
  @endverbatim

@par Example Description

How to use a OSPI NOR memory in memory-mapped mode.

This example describes how to erase a part of a OSPI NOR memory, write data in memory-mapped mode and 
access to OSPI NOR memory in memory-mapped mode to check the data in an infinite loop. The memory is 
configured in octal DTR mode.

LED_GREEN toggles each time the data have been checked
LED_ORANGE is on as soon as a data is wrong
LED_ORANGE toggles as soon as an error is returned by HAL API

In this example, HCLK is configured at 120 MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 

  - OSPI/OSPI_NOR_MemoryMapped_DTR/Src/main.c                 Main program
  - OSPI/OSPI_NOR_MemoryMapped_DTR/Src/system_stm32l4xx.c     STM32L4xx system clock configuration file
  - OSPI/OSPI_NOR_MemoryMapped_DTR/Src/stm32l4xx_it.c         Interrupt handlers 
  - OSPI/OSPI_NOR_MemoryMapped_DTR/Src/stm32l4xx_hal_msp.c    HAL MSP module
  - OSPI/OSPI_NOR_MemoryMapped_DTR/Inc/main.h                 Main program header file  
  - OSPI/OSPI_NOR_MemoryMapped_DTR/Inc/stm32l4xx_hal_conf.h   HAL Configuration file
  - OSPI/OSPI_NOR_MemoryMapped_DTR/Inc/stm32l4xx_it.h         Interrupt handlers header file

        
@par Hardware and Software environment  

  - This example runs on STM32L4R9xx devices.
    
  - This example has been tested with  32L4R9IDISCOVERY board and can be 
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 */
