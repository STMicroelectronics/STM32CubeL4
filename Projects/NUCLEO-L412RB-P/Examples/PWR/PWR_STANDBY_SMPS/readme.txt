/**
  @page PWR_STANDBY_SMPS Power Standby SMPS Mode Example
  
  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_STANDBY_SMPS/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Power Standby and SMPS mode example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
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
is connected to the user button through PC.13 and configured to generate an 
interrupt on falling edge upon key press.

After start-up, 

1) The system automatically moves to RUN mode at 80 MHz, external SMPS off, Voltage Scale Range 1.

2) After some transitions the system ends up in RUN mode with 
   external SMPS on, voltage scaling Range 2 at 80 MHz. 
  
3) The MCU waits for 5 seconds before entering Standby mode.

4) The system stays in Standby mode until the user presses the User Button.

5) Coming back from Standby mode, the MCU runs at 4 MHz, Voltage Scale Range 1. It
   moves to RUN mode at 80 MHz with external SMPS on, voltage scaling Range 2.

6) Once SMPS switch is closed , the voltage scaling is further reduced 
   from 1.0 V to 0.95V. 

7) The system repeats steps 3) to 6) indefinitely.


The software illustrates the different steps required to end up in Run mode at 80MHz,
SMPS on, Voltage Scale Range 2:
- after reset, the system clock is set to 80 MHz, SMPS off, Voltage Scale Range 1.
All the transitions are initiated from that state.
- coming back from Standby, System clock is 4 MHz, SMPS off, Voltage Scale Range 1:
less steps are required to reach the desired operating point of 80 MHz, SMPS on, 
Voltage Scale Range 2.
In addition, the example utilizes an additional feature available on STM32L412xx
& STM32L422xx,
range 2 output voltage is reduced from 1.0V to 0.95V.


One compilation switch is provided in main.c for illustration purposes:

- Compilation switch NO_DELAY_TOWARDS_SMPS ensures fast transitions to SMPS mode. 
  By default, NO_DELAY_TOWARDS_SMPS is disabled to allow to observe more easily 
  the different transitions leading to SMPS.

  
@note To measure the current consumption, please connect an amperemeter to JP6.  
  
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

@par Hardware and Software environment

  - This example runs on STM32L412xx devices.
    
  - This example has been tested with 
    STMicroelectronics NUCLEO-L412RB-P MB1319 Rev C (64 pins)
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-L412RB-P MB1319 Rev C (64 pins) set-up:
    - Use LED4 connected to PB.13 pin
    - User push-button connected to pin PC.13 (External line 13)

    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
