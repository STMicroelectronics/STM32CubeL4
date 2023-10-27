/**
  @page TIM_InputCapture Input Capture example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_InputCapture/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM_InputCapture example.
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

How to use the TIM peripheral to measure an external signal frequency.

  The TIM1CLK frequency is set to SystemCoreClock (Hz), the Prescaler is 0,
  so the TIM1 counter clock is SystemCoreClock (Hz).
  SystemCoreClock is set to 80 MHz for STM32L452xx Devices.

  TIM1 is configured in Input Capture Mode: the external signal is connected to 
  TIM1 Channel2 used as input pin.
  To measure the frequency we use the TIM1 CC2 interrupt request, so in the 
  TIM1_CC_IRQHandler routine, the frequency of the external signal is computed.

  The "uwFrequency" variable contains the external signal frequency:
  uwFrequency = TIM1 counter clock / uwDiffCapture (Hz),
  where "uwDiffCapture" is the difference between two consecutive TIM1 captures.


  The minimum frequency value to measure is TIM1 counter clock / CCR MAX
                                              = 80 MHz / 65535

  Due to TIM1_CC_IRQHandler processing time (around 3.50us), the maximum
  frequency value to measure is around 300kHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - TIM/TIM_InputCapture/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - TIM/TIM_InputCapture/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - TIM/TIM_InputCapture/Inc/main.h                  Header for main.c module  
  - TIM/TIM_InputCapture/Src/stm32l4xx_it.c          Interrupt handlers
  - TIM/TIM_InputCapture/Src/main.c                  Main program
  - TIM/TIM_InputCapture/Src/stm32l4xx_hal_msp.c     HAL MSP file
  - TIM/TIM_InputCapture/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L452xx devices.
  - In this example, the clock is set to 80 MHz.
    
  - This example has been tested with STMicroelectronics NUCLEO-L452RE Rev C
    board and can be easily tailored to any other supported device 
    and development board.      

  - NUCLEO-L452RE Rev C Set-up
    - Connect the external signal to measure to the TIM1 CH2 pin (PA.09: connected to D8 (pin 21 in CN10 connector)).

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 */
