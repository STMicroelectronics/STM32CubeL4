/**
  @page RCC_LSEConfig RCC Clock Config example
  
  @verbatim
  ******************************************************************************
  * @file    RCC/RCC_LSEConfig/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the RCC Clock Config example.
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

Enabling/disabling at run time the propagation of the low-speed external(LSE) RC oscillator (about 32 KHz) to peripherals
other than RTC, using the RCC HAL API. 

Board: NUCLEO-L412RB-P Rev C (embeds a STM32L412RBT6P device)
Microcontroller Clock Output MCO1 Pin: PA.08 (pin 19 on CN6 connector)

   _________________________                   _______________________
  |     _________CN6________|                 |  _______________      |
  |    |   RCC              |                 | |               | O   |
  |    |                    |                 | |   _   _   _   | O   |
  |    |(pin 19) MCO1(PA.08)|_________________| | _| |_| |_| |_ | --  |
  |    |                    |                 | |_______________| --  |
  |    |                    |                 |                       |
  |    |                    |                 |_Oscilloscope__________|
  |    |                    |                    
  |    |                    | 
  |    |____________________|  
  |                         |
  |                         |
  |_STM32_Board ____________|

In this example, after startup SYSCLK is configured to the max frequency using the PLL with
LSE as clock source, the User push-button (connected to External line 13)
is used to enable/disable LSE:
each time the User push-button is pressed, External line 13 interrupt is generated and the ISR
enables the LSE when it is off, then alternatively enables the LSE propagation to peripherals other than RTC, 
disables the propagation otherwise.

LED4 is toggling as follows according to LSE activation:
- when LSE is on, LED4 is emitting a double flash every half-second
  whatever the propagation setting (enabled or disabled)
- when LSE is off, LED4 is fast-toggling (every 100 ms)


LSE clock waveform is available on the MCO1 pin PA.08 and can be captured
on an oscilloscope.

In case of configuration error, LED4 is slowly blinking (1 sec. Period).


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

  - RCC/RCC_LSEConfig/Inc/stm32l4xx_hal_conf.h          HAL configuration file
  - RCC/RCC_LSEConfig/Inc/stm32l4xx_it.h                Interrupt handlers header file
  - RCC/RCC_LSEConfig/Inc/main.h                        Header for main.c module
  - RCC/RCC_LSEConfig/Src/stm32l4xx_it.c                Interrupt handlers
  - RCC/RCC_LSEConfig/Src/main.c                        Main program
  - RCC/RCC_LSEConfig/Src/system_stm32l4xx.c            STM32L4xx system source file

@par Hardware and Software environment

  - This example runs on STM32L412xx devices.

  - This example has been tested with NUCLEO-L412RB-P Rev C
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 */
