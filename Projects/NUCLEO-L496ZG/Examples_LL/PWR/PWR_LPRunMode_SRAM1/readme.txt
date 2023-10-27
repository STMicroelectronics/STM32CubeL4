/**
  @page PWR_LPRunMode_SRAM1 PWR standby example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_LPRunMode_SRAM1/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR Low Power Run Mode (SRAM1) example.
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

How to execute code in Low-power run mode from SRAM1..

In the associated software, the system clock is set to 80 MHz, an EXTI line
is connected to the User push-button through PC.13 and configured to generate an 
interrupt on falling edge upon key press.

After start-up LED1 is toggling FAST(100-ms blinking period), 
indicates that device is running at 80 MHz.

The User push-button can be pressed to set system in LowPowerRun mode (Freq: 100KHz). 
In this case LED1 toggling SLOW(400-ms blinking period).

LED1 toggling speed is controlled by variable "uhLedBlinkSpeed".

@note To measure the current consumption in Run mode, remove JP5 jumper 
      and connect an ampere meter to JP5 to measure IDD current.
      NB: LED1 have an impact on power consumption. 
          Remove LED1 blinking to have a stable power consumption, 
          comment line  "#define USE_LED" in main.c file  

@note This example is executed from SRAM1 to decrease power consumption.
      Flash is deactivated during LowPowerRun mode activation. 
      This example cannot be executed in standalone mode, so using debugger is mandatory to run this example.

@par Directory contents 

  - PWR/PWR_LPRunMode_SRAM1/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - PWR/PWR_LPRunMode_SRAM1/Inc/main.h                  Header for main.c module
  - PWR/PWR_LPRunMode_SRAM1/Inc/stm32_assert.h          Template file to include assert_failed function
  - PWR/PWR_LPRunMode_SRAM1/Src/stm32l4xx_it.c          Interrupt handlers
  - PWR/PWR_LPRunMode_SRAM1/Src/main.c                  Main program
  - PWR/PWR_LPRunMode_SRAM1/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L496xx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-L496ZG (144 pins)
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-L496ZG (144 pins) Set-up
    - LED1 connected to PC.07 pin
    - User push-button connected to pin PC.13 (External line 15 to 10)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files
 - In using ST-Link Utility tool, load your image into target memory SRAM1 area 
 at start address 0x20000000, having checked beforehand the “Reset after programming" 
 box. Make sure option bytes are correctly set to select SRAM1 as boot area (refer to 
 Reference Manual if needed).
  More precisely, ST-Link utility version is  v4.0.0.0
  Regarding the option bytes 
  * Uncheck BOOT0 nSW Config
  * Uncheck nBOOT0
  * Uncheck nBOOT1
 - Run the example

 */
