/**
  @page PWR_STOP1 Power Stop 1 Mode Example
  
  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_STOP1/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Power Stop 1 Mode example.
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
  @endverbatim

@par Example Description 
 
How to enter the Stop 1 mode and wake up from this mode by using an interrupt.

In the associated software, the system clock is set to 80 MHz, an EXTI line
is connected to GPIO PA.12 (Arduino D2) in pull up mode and configured to generate an 
interrupt on falling edge.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick 
interrupt handler, LED3 is toggled in order to indicate whether the MCU is in STOP 1 mode 
or RUN mode.

5 seconds after start-up, the system automatically enters STOP 1 mode and LED3 stops toggling.

Connecting PA.12 (Arduino D2) and GND wakes-up the system. 
The software then comes back in RUN mode for 5 sec. before automatically entering STOP 1 mode again. 
Low power mode is entered only if PA.12 (Arduino D2) and GND are not connected anymore. 

LED3 is used to monitor the system state as follows:
 - LED3 toggling: system in RUN mode
 - LED3 off : system in STOP 1 mode

These steps are repeated in an infinite loop.

@note To measure the current consumption in STOP 1 mode, remove jumper JP1 
      and connect an amperemeter to measure IDD current.

@note This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M4 core is no longer clocked during low power mode 
      so debugging features are disabled.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      

@par Directory contents 

  - PWR/PWR_STOP1/Inc/stm32l4xx_hal_conf.h     HAL Configuration file
  - PWR/PWR_STOP1/Inc/stm32l4xx_it.h           Header for stm32l4xx_it.c
  - PWR/PWR_STOP1/Inc/main.h                   Header file for main.c
  - PWR/PWR_STOP1/Src/system_stm32l4xx.c       STM32L4xx system clock configuration file
  - PWR/PWR_STOP1/Src/stm32l4xx_it.c           Interrupt handlers
  - PWR/PWR_STOP1/Src/main.c                   Main program
  - PWR/PWR_STOP1/Src/stm32l4xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32L4xx devices
    
  - This example has been tested with STMicroelectronics NUCLEO-L412KB Rev C
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-L412KB Rev C set-up:
    - LED3 connected to PB.03 pin
    - Use jumper or a wire between PA.12 (Arduino D2) and GND when required.

    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
