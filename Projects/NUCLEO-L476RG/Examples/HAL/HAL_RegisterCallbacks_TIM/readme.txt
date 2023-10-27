/**
  @page HAL_RegisterCallbacks_TIM HAL callback registration for Timer example
  
  @verbatim
  ******************************************************************************
  * @file    HAL/HAL_RegisterCallbacks_TIM/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the HAL callback registration for Timer example
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

Register a callback function called every second based on TIM peripheral
configuration to generate a timebase of one second with the corresponding
interrupt request.

    In this example TIM3 input clock (TIM3CLK)  is set to APB1 clock (PCLK1),
    since APB1 prescaler is equal to 1.
      TIM3CLK = PCLK1
      PCLK1 = HCLK
      => TIM3CLK = HCLK = SystemCoreClock
    To get TIM3 counter clock at 10 KHz, the Prescaler is computed as following:
    Prescaler = (TIM3CLK / TIM3 counter clock) - 1
    Prescaler = (SystemCoreClock /10 KHz) - 1

SystemCoreClock is set to 80 MHz for STM32L4xx Devices.

The TIM3 ARR register value is equal to 10000 - 1, 
Update rate = TIM3 counter clock / (Period + 1) = 1 Hz,
So the TIM3 generates an interrupt each 1 s

When the counter value reaches the auto-reload register value, the TIM update 
interrupt is generated and, in the handler routine, pin PA.05 (connected to LED2 on board NUCLEO-L476RG Rev C)
is toggled at the following frequency: 0.5Hz.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - HAL/HAL_RegisterCallbacks_TIM/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - HAL/HAL_RegisterCallbacks_TIM/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - HAL/HAL_RegisterCallbacks_TIM/Inc/main.h                  Header for main.c module  
  - HAL/HAL_RegisterCallbacks_TIM/Src/stm32l4xx_it.c          Interrupt handlers
  - HAL/HAL_RegisterCallbacks_TIM/Src/main.c                  Main program
  - HAL/HAL_RegisterCallbacks_TIM/Src/stm32l4xx_hal_msp.c     HAL MSP file
  - HAL/HAL_RegisterCallbacks_TIM/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L476xx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-L476RG Rev C
    board and can be easily tailored to any other supported device
    and development board.      

  - NUCLEO-L476RG Rev C Set-up
    - Use LED2 connected to PA.05 pin and connect them on an oscilloscope 
      to show the Time Base signal.  


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
