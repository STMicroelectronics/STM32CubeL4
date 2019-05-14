/**
  @page PWR_RUN_SMPS Power Mode Example
  
  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_RUN_SMPS/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Power RUN SMPS Mode example.
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
 
How to use SMPS in Run mode and access the power consumption
gain obtained when the SMPS feature is used.

In the associated software, the system clock is set to 80 MHz, an EXTI line
is connected to the user button thru PC.13 and configured to generate an 
interrupt on falling edge upon key press.
The SysTick is programmed to generate an interrupt each 1 ms.


After start-up, the user has five seconds to press the user button.
If the user button is pressed, at the end of the 5-second period, the MCU moves to 
RUN mode @ 80 MHz, SMPS on, power voltage scale 2.
If the user button is not pressed during that time, at the end of the 5-second 
period, the MCU remains in RUN mode @ 80 MHz, SMPS off, power voltage scale 1.  

The five-secong long period is indicated to the user by LED4 which is blinking
during that time.


LED4 is used to monitor the system state as follows:
 - LED4 ON:       configuration failed (system will go to an infinite loop)
 - LED4 toggling: 5-second long period on-going, during which the user can
                  press the user button to activate SMPS
 - LED4 off:      system in RUN mode (with or without SMPS)


@note To measure the current consumption, please connect an amperemeter to JP6.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      

@par Directory contents 

  - PWR/PWR_RUN_SMPS/Inc/stm32l4xx_conf.h         HAL Configuration file
  - PWR/PWR_RUN_SMPS/Inc/stm32l4xx_it.h           Header for stm32l4xx_it.c
  - PWR/PWR_RUN_SMPS/Inc/main.h                   Header file for main.c
  - PWR/PWR_RUN_SMPS/Src/system_stm32l4xx.c       STM32L4xx system clock configuration file
  - PWR/PWR_RUN_SMPS/Src/stm32l4xx_it.c           Interrupt handlers
  - PWR/PWR_RUN_SMPS/Src/main.c                   Main program

@par Hardware and Software environment

  - This example runs on STM32L433xx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-L433RC-P MB1319B (64 pins)
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-L433RC-P MB1319B (64 pins) set-up:
    - Use LED4 connected to PB.13 pin
    - User push-button connected to pin PC.13 (External line 13)

    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
