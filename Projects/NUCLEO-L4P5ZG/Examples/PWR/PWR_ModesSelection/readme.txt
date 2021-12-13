/**
  @page PWR_ModesSelection Power Modes Selection Example

  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the Power Modes Selection example.
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

How to configure the system to measure the current consumption in different
low-power modes.
The objective is to provide a mean to
measure the power consumption using an amperemeter on IDD connector.

The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick
interrupt handler.

@note To measure the current consumption remove JP5 jumper
      and connect an amperemeter to JP5 to measure IDD current.

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

  - PWR/PWR_ModesSelection/Inc/stm32l4xx_conf.h         HAL Configuration file
  - PWR/PWR_ModesSelection/Inc/stm32l4xx_it.h           Header for stm32l4xx_it.c
  - PWR/PWR_ModesSelection/Inc/main.h                   Header file for main.c
  - PWR/PWR_ModesSelection/Src/system_stm32l4xx.c       STM32L4xx system clock configuration file
  - PWR/PWR_ModesSelection/Src/stm32l4xx_it.c           Interrupt handlers
  - PWR/PWR_ModesSelection/Src/main.c                   Main program
  - PWR/PWR_ModesSelection/Src/lprun_test.c             Low Power RUN mode test
  - PWR/PWR_ModesSelection/Src/lpsleep_test.c           Low Power SLEEP mode test
  - PWR/PWR_ModesSelection/Src/run_range1_test.c        RUN mode in range 1 test
  - PWR/PWR_ModesSelection/Src/run_range2_test.c        RUN mode in range 2 test
  - PWR/PWR_ModesSelection/Src/shutdown_test.c          SHUTDOWN mode test
  - PWR/PWR_ModesSelection/Src/sleep_range1_test.c      SLEEP mode in range 1 test
  - PWR/PWR_ModesSelection/Src/sleep_range2_test.c      SLEEP mode in range 2 test
  - PWR/PWR_ModesSelection/Src/standby_rtc_sram2_test.c STANDBY mode with RTC and SRAM2 preserved test
  - PWR/PWR_ModesSelection/Src/standby_rtc_test.c       STANDBY mode with RTC test
  - PWR/PWR_ModesSelection/Src/standby_test.c           STANDBY mode test
  - PWR/PWR_ModesSelection/Src/stop1_mroff_rtc_test.c   STOP1 mode with RTC but Regulator OFF test
  - PWR/PWR_ModesSelection/Src/stop1_mroff_test.c       STOP1 mode with Regulator OFF  test
  - PWR/PWR_ModesSelection/Src/stop2_rtc_test.c         STOP2 mode with RTC test
  - PWR/PWR_ModesSelection/Src/stop2_test.c             STOP2 mode test

@par Hardware and Software environment

  - This example runs on STM32L4xx devices

  - This example has been tested with STMicroelectronics NUCLEO-L4P5ZG (144 pins)
    board embedding a low-speed clock LSE and can be easily tailored to any other
    supported device and development board.

  - L4P5ZG-L4R5ZI (144 pins) set-up:
	  - LPUART1 interface connected to ST-LINK

  - HyperTerminal setup of COM port associated to STLink Virtual COM Port:
    - Baud rate = 115200
    - Data = 8 bit
	- Parity = none
    - Stop = 1 bit
    - No hardware flow control

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
