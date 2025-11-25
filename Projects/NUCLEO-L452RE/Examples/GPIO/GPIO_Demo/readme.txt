/**
  @page GPIO_Demo GPIO Demo example
  
  @verbatim
  ******************************************************************************
  * @file    GPIO/GPIO_Demo/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the GPIO Demo example.
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

Demonstration firmware based on STM32Cube. This GPIO_Demo example helps you to
discover STM32 Cortex-M devices that are plugged onto your STM32 Nucleo board.

This example demonstrates how to use the NUCLEO-L452RE board to blink an LED at different speeds when the user button is pressed.
The LED blinks at three different speeds, and each press of the user button changes the blinking speed.

On NUCLEO-L452RE Rev C:
    - External line 13 is connected to User push-button

In this example, HCLK is configured to run at 80 MHz.

@note The GPIO_Demo example provides the source code for the demonstration binary package, which can be downloaded
      from the NUCLEO-L452RE webpage. For more details, please see the Binary Resources section here:
      https://www.st.com/en/evaluation-tools/nucleo-l452re.html#tools-software

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - GPIO/GPIO_Demo/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - GPIO/GPIO_Demo/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - GPIO/GPIO_Demo/Inc/main.h                  Header for main.c module  
  - GPIO/GPIO_Demo/Src/stm32l4xx_it.c          Interrupt handlers
  - GPIO/GPIO_Demo/Src/main.c                  Main program
  - GPIO/GPIO_Demo/Src/system_stm32l4xx.c      STM32L4xx system source file

@par Hardware and Software environment

  - This example runs on STM32L452xx devices.
    
  - This example has been tested with NUCLEO-L452RE Rev C board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
