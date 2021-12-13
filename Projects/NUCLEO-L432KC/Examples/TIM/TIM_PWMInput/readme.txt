/**
  @page TIM_PWMInput TIM PWM Input example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_PWMInput/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM PWM_Input example.
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

How to use the TIM peripheral to measure the frequency and 
duty cycle of an external signal.

The TIM2CLK frequency is set to SystemCoreClock (Hz), the Prescaler is 0 so the 
counter clock is SystemCoreClock (Hz).
SystemCoreClock is set to 80 MHz for STM32L432xx Devices.

TIM2 is configured in PWM Input Mode: the external signal is connected to 
TIM2 Channel2 used as input pin.
To measure the frequency and the duty cycle, we use the TIM2 CC2 interrupt request,
so in the TIM2_IRQHandler routine, the frequency and the duty cycle of the external 
signal are computed.

"uwFrequency" variable contains the external signal frequency:
TIM2 counter clock = SystemCoreClock,
uwFrequency = TIM2 counter clock / TIM2_CCR2 in Hz, 

"uwDutyCycle" variable contains the external signal duty cycle:
uwDutyCycle = (TIM2_CCR1*100)/(TIM2_CCR2) in %.

The minimum frequency value to measure is (TIM2 counter clock / CCR MAX)
                                         = (80 MHz)/ 65535
                                         = 1220 Hz

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - TIM/TIM_PWMInput/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - TIM/TIM_PWMInput/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - TIM/TIM_PWMInput/Inc/main.h                  Header for main.c module  
  - TIM/TIM_PWMInput/Src/stm32l4xx_it.c          Interrupt handlers
  - TIM/TIM_PWMInput/Src/main.c                  Main program
  - TIM/TIM_PWMInput/Src/stm32l4xx_hal_msp.c     HAL MSP file
  - TIM/TIM_PWMInput/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L432xx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-L432KC RevC 
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-L432KC RevC Set-up
    - Connect the external signal to measure to the TIM2 CH2 pin (PA.01) (connected to A1 (pin 11 in CN4 connector)).

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
