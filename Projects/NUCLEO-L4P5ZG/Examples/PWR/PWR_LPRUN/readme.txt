/**
  @page PWR_LPRUN Low Power Run Mode Example

  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_LPRUN/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the Low Power Run Mode example.
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

How to enter and exit the Low-power run mode.

In the associated software, the system clock is set to 32 MHz.
The SysTick is programmed to generate an interrupt each 1 ms.

5 seconds after start-up, the system automatically enters LP Run mode
and LED1 stops toggling.
The User push-button can be pressed at any time to exit from Low Power Run.
to exit LP Run Mode
The software then comes back in Run mode for 5 sec. before automatically
entering LP Run mode again.

LED1 is used to monitor the system state as follows:
 - LED1 toggling : system in Run mode
 - LED1 off : system in LP Run mode

These steps are repeated in an infinite loop.

@note To measure the current consumption in LP Run mode, remove JP5 jumper
      and connect an amperemeter to JP5 to measure IDD current.


    - Low Power Run Mode from FLASH
    ================================
          - Regulator in LP mode
          - System Running at MSI (~100 KHz)
          - Flash 0 wait state
          - MSI Range 0
          - Voltage Range 2
          - Code running from Internal FLASH
          - All peripherals OFF
          - Exit from LP Run mode using User push-button PC.13

@note By default, the software resorts to __HAL_FLASH_PREFETCH_BUFFER_DISABLE() macro
      to disable the Adaptive Real Time (ART). The user needs to uncomment this line in
      main() function if power consumption figures with ART ON in LP Run mode must
      be measured.

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

  - PWR/PWR_LPRUN/Inc/stm32l4xx_conf.h         HAL Configuration file
  - PWR/PWR_LPRUN/Inc/stm32l4xx_it.h           Header for stm32l4xx_it.c
  - PWR/PWR_LPRUN/Inc/main.h                   Header file for main.c
  - PWR/PWR_LPRUN/Src/system_stm32l4xx.c       STM32L4xx system clock configuration file
  - PWR/PWR_LPRUN/Src/stm32l4xx_it.c           Interrupt handlers
  - PWR/PWR_LPRUN/Src/main.c                   Main program

@par Hardware and Software environment

  - This example runs on STM32L4xx devices

  - This example has been tested with STMicroelectronics NUCLEO-L4P5ZG (144 pins)
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-L4P5ZG (144 pins) set-up:
    - LED1 connected to PC.07 pin
    - Use the User push-button connected to pin PC.13.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
