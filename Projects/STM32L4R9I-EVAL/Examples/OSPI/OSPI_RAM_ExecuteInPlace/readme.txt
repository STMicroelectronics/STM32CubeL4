/**
  @page OSPI_RAM_ExecuteInPlace OSPI HyperRAM execute in place example
  
  @verbatim
  ******************************************************************************
  * @file    OSPI/OSPI_RAM_ExecuteInPlace/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the code execution from OSPI HyperRAM memory example.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to execute code from OSPI memory after code loading.

This example describes how to execute a part of the code from a OSPI HyperRAM memory. To do this, 
a section is created where the function is stored.

At the startup, the OSPI HyperRAM is configured in memory-mapped mode and the data are copied from the initialization
section of the flash to the OSPI HyperRAM memory. And finally the code in executed in an infinite loop.

LED1, LED2, LED3 and LED4 toggle in a forever loop.
LED_RED is on as soon as an error is returned by HAL API, and no more LED toggles.

In this example, HCLK is configured at 120 MHz.

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

  - This example runs on STM32L4R9xx devices.
    
  - This example has been tested with STM32L4R9I-EVAL board and can be 
    easily tailored to any other supported device and development board.

  - A specific region for the OSPI HyperRAM memory has been added in the scatter file 
   (STM32L4R9xx_flash.icf for IAR toolchain, STM32L4R9I-EVAL.sct for Keil toolchain)
   in order to map functions in this memory

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
