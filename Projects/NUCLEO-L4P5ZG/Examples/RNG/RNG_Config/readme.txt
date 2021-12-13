/**
  @page RNG_Config RNG Peripheral Configuration Example

  @verbatim
  ******************************************************************************
  * @file    RNG/RNG_Config/readme.txt
  * @author  MCD Application Team
  * @brief   Description of RNG peripheral configuration example.
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

Configuration of the RNG using the HAL API. This example uses the RNG to generate 32-bit long random numbers.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 120 MHz.

The RNG peripheral initialization is ensured by the HAL_RNG_Init() function.
The latter is calling the HAL_RNG_MspInit() function which implements
the configuration of the needed RNG resources according to the used hardware (CLOCK,
GPIO, DMA and NVIC). You may update this function to change RNG initialization.

Next, the peripheral entropy configuration parameters are updated by HAL_RNGEx_SetConfig().
The configuration is locked by HAL_RNGEx_LockConfig() then read back by HAL_RNGEx_GetConfig()
to check that the configuration is that expected.

Finally, user is asked to press User push-button.
The 8-entry array aRandom32bit[] is filled up by 32-bit long random numbers
at each User push-button press.


The random numbers can be displayed on the debugger in aRandom32bit variable.

In case of error, LED3 is toggling at a frequency of 1Hz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.




@par Directory contents

  - RNG/RNG_Config/Inc/stm32l4xx_hal_conf.h         HAL configuration file
  - RNG/RNG_Config/Inc/stm32l4xx_it.h               Interrupt handlers header file
  - RNG/RNG_Config/Inc/main.h                       Header for main.c module
  - RNG/RNG_Config/Src/stm32l4xx_it.c               Interrupt handlers
  - RNG/RNG_Config/Src/main.c                       Main program
  - RNG/RNG_Config/Src/stm32l4xx_hal_msp.c          HAL MSP module
  - RNG/RNG_Config/Src/system_stm32l4xx.c           STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L4P5xx devices.

  - This example has been tested with NUCLEO-L4P5ZG (144 pins) board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
