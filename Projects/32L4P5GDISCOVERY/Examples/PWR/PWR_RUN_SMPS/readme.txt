/**
  @page PWR_RUN_SMPS Power Mode Example

  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_RUN_SMPS/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the Power RUN SMPS Mode example.
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

How to use SMPS in Run mode and access the power consumption
gain obtained when the SMPS feature is used.

In the associated software, the system clock is set to 80 MHz, the joystick is
configured to generate an interrupt on falling edge upon SEL button press.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick
interrupt handler, LEDs are toggled .


The joystick SEL button can be pressed within 5s after start-up.
If the button is pressed,
- the software goes in Run mode with SMPS ENABLED when the 5-sec period has elapsed.
- for clarity's sake, LED2 is toggling to indicated SEL button was pressed
If the button is NOT pressed, the software remains in Run mode with SMPS DISABLED.

Two leds LED1 and LED2 are used to monitor the system state as following:
 - LED2 ON: configuration failed (system will go to an infinite loop)
 - LED1 toggling: the example initial 5-second period is on-going
 - LED2 toggling: the joystick SEL button has been pressed
 - LED1 and LED2 off : system in Run mode (with joystick and LEDs IOs de-initialized)

For illustration purposes, if the joystick SEL button has been pressed, the
software first moves to 24 MHz system clock in Voltage Range 2. After two seconds,
the software moves to a 80 MHz system clock in Voltage Range 1 and stays at this
operating point.

@note To measure the current consumption in Run mode, please connect to JP5 1-2 pins (IDD)

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
  - PWR/PWR_RUN_SMPS/Src/stm32l4xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32L4P5xx devices

  - This example has been tested with STM32L4P5G-Discovery (MB1535) board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Disconnect toolchain
 - Unplug the USB cable from CN11 ST-Link connector
 - Connect Amperemeter between the 2 pins 1 and 2 of JP5 (IDD).
 - Plug the USB cable back to CN11 ST-Link connector (starts the example)
 - Wait 10 seconds and measure power consumption (A: 80 MHz Run mode, Voltage Range 1).
 - Reset board (using reset button)
 - Press user button within 5 seconds (while LED1 is toggling) for SMPS
 - Wait 10s and start measuring power consumption (B: 80 MHz un mode, Voltage Range 1, SMPS enabled)

 */
