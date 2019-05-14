/**
  @page Templates  Description of the Templates example
  
  @verbatim
  ******************************************************************************
  * @file    Templates/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Templates example.
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

Reference template based on the STM32Cube HAL API that can be used
to build any firmware application.

One workspace is provided for NUCLEO-L433RC-P board.
This workspace includes the right set of peripherals, linker and startup files.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.

@par Directory contents

  - Templates/Src/main.c                 Main program
  - Templates/Src/system_stm32l4xx.c     STM32L4xx system clock configuration file
  - Templates/Src/stm32l4xx_it.c         Interrupt handlers 
  - Templates/Src/stm32l4xx_hal_msp.c    HAL MSP module
  - Templates/Inc/main.h                 Main program header file  
  - Templates/Inc/stm32l4xx_hal_conf.h   HAL Configuration file
  - Templates/Inc/stm32l4xx_it.h         Interrupt handlers header file

        
@par Hardware and Software environment  

  - This template runs on STM32L433xx devices.
    
  - This template has been tested with NUCLEO-L433RC-P board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
