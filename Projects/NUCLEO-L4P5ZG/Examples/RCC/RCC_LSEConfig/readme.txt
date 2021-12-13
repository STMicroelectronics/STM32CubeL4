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

How to enable/disable at run time the propagation of the low-speed external(LSE) RC oscillator (about 32 KHz) to peripherals
other than RTC, using the RCC HAL API.

Board: NUCLEO-L4P5ZG (144 pins) (embeds a STM32L4P5ZGT6 device)
Microcontroller Clock Output MCO1 Pin: PA.08

   _________________________                   _______________________
  |     _________CN12_______|                 |  _______________      |
  |    |   RCC              |                 | |               | O   |
  |    |                    |                 | |   _   _   _   | O   |
  |    |(pin 23) MCO1(PA.08)|_________________| | _| |_| |_| |_ | --  |
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
MSI as clock source, the User push-button (connected to External line 13)
is used to enable/disable LSE propagation:
- on the very first press, LSE is enabled with propagation to all peripherals enabled.
- then, on each successive user button press, the LSE propagation to all peripherals but RTC
  is alternatively disabled or enabled

LED1 is toggling as follows according to LSE activation:
- when LSE is on and propagation to all peripherals is enabled,
   LED1 is emitting a double flash every half-second
- when LSE is on and propagation to all peripherals but RTC is disabled,
   LED1 is fast-toggling (every 100 ms)
- when LSE is off
   LED1 is slow-toggling (every 1 second)


LSE clock waveform is available on the MCO1 pin PA.08 and can be captured
on an oscilloscope.

In case of configuration error, LED3 is turned on.


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

  - This example runs on STM32L4P5xx devices.

  - This example has been tested with NUCLEO-L4P5ZG (144 pins)
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
