/**
  @page CRS_Synchronization_Polling CRS example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/CRS/CRS_Synchronization_Polling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRS_Synchronization_Polling example.
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

How to configure the clock recovery service in polling mode through the
STM32L4xx CRS LL API. The peripheral initialization uses LL unitary
service functions for optimization purposes (performance and size).

At the beginning of the main program the SystemClock_Config() function is used to 
configure the system clock (SYSCLK) to run at 80 MHz.

Then, HSI48 and LSE oscillators are enabled. In this example CRS will use LSE 
oscillator to calibrate HSI48 frequency.
When HSI48 and LSE are started, automatic calibration starts and program call the
polling function to wait for synchronization status.

In this test, there are results which could occur:
 - Wait for User push-button press to start the test (LED1 fast toggle)
 - few loops occurs up to SYNC OK event (if SYNC WARN, HSI48 trimming value will be
 automatically modified)
 - in case of SYNC MISS or SYNC ERROR, there is an issue with synchronization input
 parameters. In this case, user need to apply new parameters after checking synchronization
 information and restart the synchronization.
 - LED1 is slowly blinking (1 sec. period) in case of timeout during polling 
 procedure. It may due to an issue with LSE synchronization frequency.
 - LED1 will finally power on if SYNC OK is returned by CRS.
 
@par Directory contents 

  - CRS/CRS_Synchronization_Polling/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - CRS/CRS_Synchronization_Polling/Inc/main.h                  Header for main.c module
  - CRS/CRS_Synchronization_Polling/Inc/stm32_assert.h          Template file to include assert_failed function
  - CRS/CRS_Synchronization_Polling/Src/stm32l4xx_it.c          Interrupt handlers
  - CRS/CRS_Synchronization_Polling/Src/main.c                  Main program
  - CRS/CRS_Synchronization_Polling/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L496xx devices.
    
  - This example has been tested with NUCLEO-L496ZG (144 pins) board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-L496ZG (144 pins) Set-up
    - Connect the MCO pin to an oscilloscope to monitor HSI48 frequency:
      - PA.08: connected to pin 23 of CN12 connector for Nucleo-144 (MB1137)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
