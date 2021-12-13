/**
  @page IWDG_Reset Independent Watchdog Reset Example
  
  @verbatim
  ******************************************************************************
  * @file    IWDG/IWDG_Reset/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Window Watchdog example.
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

How to handle the IWDG reload counter and simulate a software fault that generates 
an MCU IWDG reset after a preset laps of time.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 80 MHz.

The IWDG timeout is set to 1 second.

First, the TIM16 timer is configured to measure the LSI frequency as the 
LSI is internally connected to TIM16 CH1, in order to adjust the IWDG clock.
The LSI frequency value could be monitored on debugger in "uwLsiFreq" variable 

The LSI measurement using the TIM16 is described below:
  - Configure the TIM16 to remap internally the TIM16 CH1 Input Capture to the LSI
    clock output.
  - Enable the TIM16 Input Capture interrupt: after one cycle of LSI clock, the
    period value is stored in a variable and compared to the HCLK clock to get
    its real value. 

Then, the IWDG reload counter is configured as below to obtain 1 second according 
to the measured LSI frequency after setting the prescaler value:
  
    IWDG counter clock Frequency = LSI Frequency / Prescaler value

The IWDG reload counter is refreshed each 990 ms in the main program infinite 
loop to prevent a IWDG reset.
  
LED2 is also toggling each 990 ms indicating that the program is running.

An EXTI Line is connected to a GPIO pin, and configured to generate an interrupt
on the rising edge of the signal.

The EXTI Line is used to simulate a software failure: once the EXTI Line event 
occurs, by pressing the User push-button (PC.13), the corresponding interrupt  
is served. 

In the ISR, a write to invalid address generates a Hardfault exception 
containing an infinite loop and preventing to return to main program (the IWDG 
reload counter is not refreshed).
As a result, when the IWDG counter reaches 0, the IWDG reset occurs.

If the IWDG reset is generated, after the system resumes from reset, LED2 turns on for 4 seconds.
If the EXTI Line event does not occur, the IWDG counter is indefinitely refreshed in the main 
program infinite loop, and there is no IWDG reset.

LED2 will turn off if any error occurs.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - IWDG/IWDG_Reset/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - IWDG/IWDG_Reset/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - IWDG/IWDG_Reset/Inc/main.h                  Header for main.c module  
  - IWDG/IWDG_Reset/Src/stm32l4xx_it.c          Interrupt handlers
  - IWDG/IWDG_Reset/Src/main.c                  Main program
  - IWDG/IWDG_Reset/Src/stm32l4xx_hal_msp.c     HAL MSP file
  - IWDG/IWDG_Reset/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L452xx devices.
    
  - This example has been tested with NUCLEO-L452RE Rev C board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 
 */
 