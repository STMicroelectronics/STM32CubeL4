/**
  @page IWDG_WindowMode IWDG Reset with window mode
  
  @verbatim
  ******************************************************************************
  * @file    IWDG/IWDG_WindowMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the IWDG Reset with window mode.
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

How to periodically update the IWDG reload counter and simulate a software fault that generates 
an MCU IWDG reset after a preset laps of time.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 80 MHz.

The IWDG time-out is set to 762 ms (the time-out may varies due to LSI frequency 
dispersion).

The Window option is enabled with a window register value set to 400 ms.
To prevent a reset, the down-counter must be reloaded when its value is:
 -lower than the window register value (400ms)
 -greater than 0x0
The IWDG counter is therefore refreshed each 450 ms in the main program infinite loop to 
prevent a IWDG reset (762 - 450 = 312 within the interval).
LED2 is also toggled each 450 ms indicating that the program is running. 
LED2 will turn off if any error occurs.

An EXTI Line is connected to a GPIO pin, and configured to generate an interrupt
on the rising edge of the signal.

Once the EXTI Line event occurs by pressing the User push-button (PC.13), 
the refresh period is set to 200 ms.
That will make refresh being outside window value. As a result, when the IWDG counter is reloaded, 
the IWDG reset occurs.

If the IWDG reset is generated, after the system resumes from reset, LED2 turns on for 4 seconds.

If the EXTI Line event does not occur, the IWDG counter is indefinitely refreshed
in the main program infinite loop, and there is no IWDG reset.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - IWDG/IWDG_WindowMode/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - IWDG/IWDG_WindowMode/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - IWDG/IWDG_WindowMode/Inc/main.h                  Header for main.c module  
  - IWDG/IWDG_WindowMode/Src/stm32l4xx_it.c          Interrupt handlers
  - IWDG/IWDG_WindowMode/Src/main.c                  Main program
  - IWDG/IWDG_WindowMode/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L476xx devices.
    
  - This example has been tested with NUCLEO-L476RG Rev C board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 

 */
