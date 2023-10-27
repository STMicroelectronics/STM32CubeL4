/**
  @page COMP_PWMSignalControl COMP PWM Signal control example
  
  @verbatim
  ******************************************************************************
  * @file    COMP/COMP_PWMSignalControl/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the COMP PWM Signal Control example.
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

How to configure a comparator peripheral to automatically hold the TIMER PWM output
in the safe state (low level) as soon as the comparator output is set to a high
level.

  - COMP1 is configured as follows:
     - Inverting input is internally connected to VREFINT = 1.22V
     - Non Inverting input is connected to pin PB.02
     - Output is connected to TIM2 ETR signal

  - TIM2 counter is used for PWM signal generation.
     - Timer frequency is set to 1 kHz, PWM duty cycle 25%. Frequency can be tuned using defined label "TIMER_FREQUENCY_HZ" (unit: Hz).
     - TIM2 channel TIM_CHANNEL_2 is configured in PWM output mode, connected to pin PB.03.

  - While voltage applied to COMP1 non-inverting input (PB.02 pin) is lower than VREFINT (1.22V), 
     PWM signal is output on PB.03 pin.
  - While voltage is higher than VREFINT, PB.03 is at low level.

The Autoreload is equal to 99 so the output frequency (OutputFrequency) will
be equal to the external counter clock (InputFrequency) divided by (99+1).

  OutputFrequency = InputFrequency / (Autoreload + 1)
                  = InputFrequency / 100

Pulse value equal to 49 and the duty cycle (DutyCycle) is computed as follows:

  DutyCycle = 1 - [(PulseValue + 1)/ (Autoreload + 1)]
  DutyCycle = 50%


Board settings:
  - NUCLEO-L412RB-P Rev C Set-up:
    Connect with a wire COMP1 output (PB.00 pin 10 CN6) to TIM2 ETR signal (PA.05 pin 28 CN6).
    Apply a voltage generator output to COMP1 inverting input (PB.02 pin 18 CN6): above or below Vrefint (1.22V).
    Connect an oscilloscope to TIM output (PB.03 pin 31 CN6) and observe the waveforms.
    
    Note: to avoid the need of a voltage supply, user can connect a wire between COMP1 inverting input (PB.02 pin)
    and a pin of the board: 
	     Vdda pin (3.3V) or ground pin (0V).    
    
NUCLEO-L412RB-P Rev C board's LED is used to monitor the program execution status:
 - Error: In case of error, LED4 is toggling at a frequency of 2Hz.
 

@note To have a precise value of the pulse, HSI as a source of clock system should be 
calibrated or use an external clock.


@note Care must be taken when using HAL_Delay(), this function provides 
      accurate delay (in milliseconds) based on variable incremented in SysTick ISR. 
      This implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower) than the 
      peripheral interrupt. Otherwise the caller ISR process will be blocked. 
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set 
      to 1 millisecond to have correct HAL operation.

@par Directory contents 

  - COMP/COMP_PWMSignalControl/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - COMP/COMP_PWMSignalControl/Inc/stm32l4xx_it.h          COMP interrupt handlers header file
  - COMP/COMP_PWMSignalControl/Inc/main.h                  Header for main.c module
  - COMP/COMP_PWMSignalControl/Src/stm32l4xx_it.c          COMP interrupt handlers
  - COMP/COMP_PWMSignalControl/Src/main.c                  Main program
  - COMP/COMP_PWMSignalControl/Src/stm32l4xx_hal_msp.c     HAL MSP file 
  - COMP/COMP_PWMSignalControl/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment 

  - This example runs on STM32L412xx devices.
  - In this example, the clock is set to 80 MHz.

  - This example has been tested with NUCLEO-L412RB-P Rev C board and can be
    easily tailored to any other supported device and development board.
    

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 */