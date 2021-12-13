/**
  @page PWR_STOP0_SMPS Power Stop 0 SMPS Mode Example

  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_STOP0_SMPS/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the Power STOP0 SMPS Mode example.
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

How to enter Stop 0 mode after Run mode with SMPS enabled and how to wake up
from this mode using an interrupt.

In the associated software, the system clock is set to 80 MHz, an EXTI line
is connected to the joystick SEL button through PC.13 and configured to generate an
interrupt on falling edge upon key press.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick
interrupt handler, LED1 is toggled in order to indicate whether the MCU is in STOP 0 mode
or RUN mode.

After start-up,

1) The system automatically runs at 80 MHz without SMPS at voltage scale range 1.

2) After the transitions listed below, the system ends in Run mode with
  external SMPS enabled, voltage scale range 2, system clock at 80 MHz (step 2)d) below)
  2)a) external SMPS disabled, voltage scale range 1, system clock at 24 MHz
  2)b) external SMPS disabled, voltage scale range 2, system clock at 24 MHz
  2)c) external SMPS enabled,  voltage scale range 2, system clock at 24 MHz
  2)d) external SMPS enabled,  voltage scale range 2, system clock at 80 MHz

As long as the system is in Run mode (whatever the speed), green LED1 is blinking.

3) An infinite while(1) loop is executed at operating point 2)d)
Blue LED2 is on, indicating to the user that it is possible to press the joystick
SEL button to enter low power Stop 0 mode.

4) Upon joystick SEL button press, external SMPS is disabled for power consumption saving's sake
and the system is in Stop0 mode, all LEDS are turned off.

5) Upon a new joystick SEL button press, the system wakes up and follows the transitions below
  5)a) external SMPS disabled, voltage scale range 2, system clock at 4 MHz
  5)b) external SMPS enabled,  voltage scale range 2, system clock at 4 MHz
  5)d) external SMPS enabled,  voltage scale range 2, system clock at 80 MHz = 2)d)

The system is back to 3) and the user can repeat the steps 3) to 5) indefinitely.

In case of error, system goes in an infinite loop with blue LED2 toggling.

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

  - PWR/PWR_STOP0_SMPS/Inc/stm32l4xx_conf.h         HAL Configuration file
  - PWR/PWR_STOP0_SMPS/Inc/stm32l4xx_it.h           Header for stm32l4xx_it.c
  - PWR/PWR_STOP0_SMPS/Inc/main.h                   Header file for main.c
  - PWR/PWR_STOP0_SMPS/Src/system_stm32l4xx.c       STM32L4xx system clock configuration file
  - PWR/PWR_STOP0_SMPS/Src/stm32l4xx_it.c           Interrupt handlers
  - PWR/PWR_STOP0_SMPS/Src/main.c                   Main program
  - PWR/PWR_STOP0_SMPS/Src/stm32l4xx_hal_msp.c      HAL MSP module

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

 */
