/**
  @page LPTIM_RepetitionCounter Low power timer repetition counter example

  @verbatim
  ******************************************************************************
  * @file    LPTIM/LPTIM_RepetitionCounter/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LPTIM repetition counter example
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

How to configure and use LPTIM repetition counter to update
the autoreload counter upon an update event.

Each time the autoreload counter reaches the maximum value (Period/Autoreload), an
interruption is generated, the LED4 toggles.
Parallelly, every autoreload counter match (maximum value reach), the repetition counter
is decremented.
When the repetition counter underflows, an update event is generated and the autoreload
repetition value is updated.

In this example Period value is alternatively set to 10000 and 1000 while the repetition
counter is set to 9.
So each time the counter counts (10000 + 1) or (1000 + 1) internal clock cycles,
an interrupt is generated, LED4 toggles and the repetition counter is decremented.
When the repetition counter underflows (i.e. after 10 maximum value reaches by the
autoreload counter), the period is updated.

In this example the external clock provided to the LPTIM1 is LSE (32 Khz),
and the internal input is sampled with LSE clock.

Therefore, user can observe the LED toggling at a varying period (long LED4 toggling
followed by four short ones).

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

  - LPTIM/LPTIM_RepetitionCounter/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - LPTIM/LPTIM_RepetitionCounter/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - LPTIM/LPTIM_RepetitionCounter/Inc/main.h                  Header for main.c module
  - LPTIM/LPTIM_RepetitionCounter/Src/stm32l4xx_it.c          Interrupt handlers
  - LPTIM/LPTIM_RepetitionCounter/Src/main.c                  Main program
  - LPTIM/LPTIM_RepetitionCounter/Src/stm32l4xx_hal_msp.c     HAL MSP module
  - LPTIM/LPTIM_RepetitionCounter/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L412xx devices.

  - This example has been tested with STMicroelectronics NUCLEO-L412RB-P (64 pins)
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
