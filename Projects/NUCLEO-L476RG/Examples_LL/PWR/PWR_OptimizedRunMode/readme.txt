/**
  @page PWR_OptimizedRunMode PWR standby example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_OptimizedRunMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR Optimized Run Mode example.
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

How to increase/decrease frequency and VCORE and how to enter/exit the
Low-power run mode.

In the associated software, the system clock is set to 80 MHz, an EXTI line
is connected to the User push-button through PC.13 and configured to generate an 
interrupt on falling edge upon key press.

After start-up LED2 is toggling FAST(100-ms blinking period), indicates that device 
is running at 80MHZ.

LED toggling speed is controlled by variable "uhLedBlinkSpeed".

The User push-button can be pressed at any time to change Frequency, VCore(VOS) 
and Low Power Run mode. 

Initial STATE: 
--> Freq: 80MHz, VCore 1.2V, Core in Run Mode
--> LED2 toggling FAST (100ms)   - wait User push-button action

STATE 2: 
User push-button pressed:
--> Freq: 24MHz, VCore 1.0V, Core in Run Mode
--> LED2 toggling MEDIUM (200ms) - wait User push-button action

STATE 3: 
User push-button pressed:
--> Freq:  2MHz, VCore 1.0V, Core in Low Power Run Mode
--> LED2 toggling SLOW (400ms)   - wait User push-button action

STATE 4: 
User push-button pressed:
--> Freq: 24MHz, VCore 1.0V, Core in Run Mode
--> LED2 toggling MEDIUM (200ms) - wait User push-button action

Final STATE: 
User push-button pressed:
--> Freq: 80MHz, VCore 1.2V, Core in Run Mode
--> LED2 toggling FAST (100ms) in infinite loop


@note To measure the current consumption in Run mode, remove JP6 jumper 
      and connect an ampere meter to JP6 to measure IDD current.
      NB: LED2 have an impact on power consumption. 
          Remove LED2 blinking to have a stable power consumption, 
          comment line  "#define USE_LED" in main.c file  
@par Directory contents 

  - PWR/PWR_OptimizedRunMode/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - PWR/PWR_OptimizedRunMode/Inc/main.h                  Header for main.c module
  - PWR/PWR_OptimizedRunMode/Inc/stm32_assert.h          Template file to include assert_failed function
  - PWR/PWR_OptimizedRunMode/Src/stm32l4xx_it.c          Interrupt handlers
  - PWR/PWR_OptimizedRunMode/Src/main.c                  Main program
  - PWR/PWR_OptimizedRunMode/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L476xx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-L476RG Rev C
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-L476RG Rev C Set-up
    - LED2 connected to PA.05 pin
    - User push-button connected to pin PC.13 (External line 15 to 10)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
