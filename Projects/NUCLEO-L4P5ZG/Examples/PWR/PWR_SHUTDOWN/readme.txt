/**
  @page PWR_SHUTDOWN PWR Shutdown example

  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_SHUTDOWN/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the PWR Shutdown example.
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

How to enter the Shutdown mode and wake up from this mode by using an external
reset or the WKUP pin.

In the associated software, the system clock is set to 120 MHz, an EXTI line
is connected to the user button through PC.13 and configured to generate an
interrupt on falling edge.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick
interrupt handler, LED1 is toggled in order to indicate whether the MCU is in Shutdown or RUN mode.

When the User push-button is pressed a falling edge is detected on the EXTI line and
an interrupt is generated. In the EXTI handler routine, the wake-up pin PWR_WAKEUP_PIN2
is enabled and the corresponding wake-up flag cleared.
Then, the system enters Shutdown mode causing LED1 to stop toggling.

Next, the user can wake-up the system in pressing the User push-button which is connected
to the wake-up pin PWR_WAKEUP_PIN2.
A falling edge on the wake-up pin wakes-up the system from Shutdown.
Alternatively, an external RESET of the board leads to a system wake-up as well.

After wake-up from Shutdown mode, program execution restarts from the beginning.
Exit from Shutdown is detected through a flag set in an RTC back-up register; in
such a case, LED1 is kept on for about 3 sec. before toggling again.

USer can re-enter Shutdown in pressing again the User push-button.

LED1 is used to monitor the system state as follows:
 - LED1 toggling: system in RUN mode
 - LED1 is on for about 3 sec.: system is restarting and out-of-shutdown has been detected
 - LED1 off : system in Shutdown mode
 - LED1 transmits a sequence of three dots, three dashes, three dots : system in ERROR mode

These steps are repeated in an infinite loop.

@note To measure the current consumption in Shutdown mode, remove IDD_JP jumper
      and connect an amperemeter to IDD_JP to measure IDD current.

@note This example can not be used in DEBUG mode, this is due to the fact
      that the Cortex-M4 core is no longer clocked during low power mode
      so debugging features are disabled

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

  - PWR/PWR_SHUTDOWN/Inc/stm32l4xx_conf.h         HAL Configuration file
  - PWR/PWR_SHUTDOWN/Inc/stm32l4xx_it.h           Header for stm32l4xx_it.c
  - PWR/PWR_SHUTDOWN/Inc/main.h                   Header file for main.c
  - PWR/PWR_SHUTDOWN/Src/system_stm32l4xx.c       STM32L4xx system clock configuration file
  - PWR/PWR_SHUTDOWN/Src/stm32l4xx_it.c           Interrupt handlers
  - PWR/PWR_SHUTDOWN/Src/main.c                   Main program

@par Hardware and Software environment

  - NUCLEO-L4P5ZG (144 pins) Set-up
    - LED1 connected to PC.07 pin
    - User push-button connected to pin PC.13 (External line 15 to 10)
    - WakeUp Pin PWR_WAKEUP_PIN2 connected to PC.13

  - This example runs on STM32L4xx devices

  - This example has been tested with STMicroelectronics NUCLEO-L4P5ZG (144 pins)
    evaluation board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
