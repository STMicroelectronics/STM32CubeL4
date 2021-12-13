/**
  @page PWR_LPRUN_SRAM1 Low Power Run Mode Example (code running on SRAM1)
  
  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_LPRUN_SRAM1/readme.txt 
  * @author  MCD Application Team
  * @brief   This example shows how to use PWR HAL API to enter the
  *          system to Low Power Run mode and exit from this mode using Key 
  *          push button. Code running on SRAM1.
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
 
How to enter and exit the Low-power run mode.

In the associated software, the system clock is set to 32 MHz, an EXTI line
is connected to the user button through PC.13 and configured to generate an 
interrupt on falling edge upon key press.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick 
interrupt handler, LED2 is toggled in order to indicate whether the MCU is in LP RUN mode 
or RUN mode.

5 seconds after start-up, the system automatically enters LP RUN mode and 
LED2 stops toggling.

The User push-button can be pressed at any time to exit from Low Power Run. 
The software then comes back in RUN mode for 5 sec. before automatically entering LP RUN mode again. 

LED2 is used to monitor the system state as follows:
 - LED2 toggling : system in RUN mode
 - LED2 off : system in LP RUN mode

These steps are repeated in an infinite loop.

@note To measure the current consumption in LP RUN mode, remove JP6 jumper 
      and connect an amperemeter to JP6 to measure IDD current.     

@note All the executable code is mapped in SRAM1 area. Macro __HAL_FLASH_POWER_DOWN_ENABLE()
      powers down the FLASH during LP RUN thus improving power consumption. 

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

  - PWR/PWR_LPRUN_SRAM1/Inc/stm32l4xx_conf.h         HAL Configuration file
  - PWR/PWR_LPRUN_SRAM1/Inc/stm32l4xx_it.h           Header for stm32l4xx_it.c
  - PWR/PWR_LPRUN_SRAM1/Inc/main.h                   Header file for main.c
  - PWR/PWR_LPRUN_SRAM1/Src/system_stm32l4xx.c       STM32L4xx system clock configuration file
  - PWR/PWR_LPRUN_SRAM1/Src/stm32l4xx_it.c           Interrupt handlers
  - PWR/PWR_LPRUN_SRAM1/Src/main.c                   Main program

@par Hardware and Software environment

  - This example runs on STM32L476xx devices
    
      
  - This example has been tested with STMicroelectronics NUCLEO-L476RG Rev C
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-L476RG Rev C set-up:
    - LED2 connected to PA.05 pin
    - User push-button connected to pin PC.13 (External line 13)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
