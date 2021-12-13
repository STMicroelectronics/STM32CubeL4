/**
  @page PWR_STANDBY_SMPS Power Standby SMPS Mode Example

  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_STANDBY_SMPS/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the Power Standby SMPS Mode example.
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

How to enter SMPS Standby mode and wake up from this mode using the wake-up pin.

In the associated software, the system clock is set to 80 MHz, an EXTI line
is connected to the joystick SEL button through PC.13 and configured to generate an
interrupt on falling edge upon key press.

After start-up,

1) The system automatically runs at 80 MHz without SMPS at voltage scale range 1.

2) After the transitions listed below, the system ends in Run mode with
  external SMPS enabled, voltage scale range 2, system clock at 80 MHz (step 2)d) below)
  2)a) external SMPS disabled, voltage scale range 1, system clock at 24 MHz
  2)b) external SMPS disabled, voltage scale range 2, system clock at 24 MHz
  2)c) external SMPS enabled,  voltage scale range 2, system clock at 24 MHz
  2)d) external SMPS enabled,  voltage scale range 2, system clock at 80 MHz

3) then stays 10 seconds before entering standby mode.

4) And stays in standby mode until joystick SEL button is pressed.
  When SEL button is pressed, the system wakes up with
  external SMPS disabled, voltage scale range 1, system clock at 4 MHz.
  Voltage scale range is moved to range 2 then SMPS is enabled.
  Finally, the software moves back to step 2)d) and the iteration carries on.


While the system is in Run mode, LED1 is toggling.
Upon wake-up from Standby, LED2 is turned on for three seconds.
In case of error, LED2 is turned on and stays on in an infinite loop.

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

  - PWR/PWR_STANDBY_SMPS/Inc/stm32l4xx_conf.h         HAL Configuration file
  - PWR/PWR_STANDBY_SMPS/Inc/stm32l4xx_it.h           Header for stm32l4xx_it.c
  - PWR/PWR_STANDBY_SMPS/Inc/main.h                   Header file for main.c
  - PWR/PWR_STANDBY_SMPS/Src/system_stm32l4xx.c       STM32L4xx system clock configuration file
  - PWR/PWR_STANDBY_SMPS/Src/stm32l4xx_it.c           Interrupt handlers
  - PWR/PWR_STANDBY_SMPS/Src/main.c                   Main program
  - PWR/PWR_STANDBY_SMPS/Src/stm32l4xx_hal_msp.c      HAL MSP module

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


Option:
 - Fast transition towards SMPS
   in main.c uncomment the following line
   // #define NO_DELAY_TOWARDS_SMPS        1

 -  Comment
   #define NO_DELAY_TOWARDS_SMPS        1
   allows clearer step transitions.

 */
