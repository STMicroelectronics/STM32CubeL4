/**
  @page ADC_AnalogWatchdog ADC conversion example with analog watchdog, using 
   related peripherals (GPIO, DMA, Timer), voltage input from DAC, user control
   by push button, LED, and optionally by potentiometer (RV3).

  @verbatim
  ******************************************************************************
  * @file    ADC/ADC_AnalogWatchdog/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC conversion example
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

How to use the ADC peripheral to perform conversions with an analog watchdog 
and out-of-window interrupts enabled.

Other peripherals related to ADC are used:
Mandatory:
 - GPIO peripheral is used in analog mode to drive signal from device pin to
   ADC input.
Optionally:
 - Timer peripheral is used to trigger ADC conversions.
 - DMA peripheral is used to transfer ADC converted data.

ADC settings:
 Regular conversion are triggered by external event (timer at 1kHz).
 Continuous mode is disabled to yield only 1 conversion at each conversion trigger.
 Analog watchdog 1 is enabled, minimum and maximum thresholds are respectively set 
to 1/8 and 5/8 of full range scale (between 0.41V and 2.06V with full range of 3.3V).
  
ADC conversion results:
 - ADC conversions results are transferred automatically by DMA, into variable
   array "aADCxConvertedValues".
 - DMA and ADC are configured to operate continuously, in circular mode.
   When DMA transfer half-buffer and full buffer lengths are reached, callbacks 
   HAL_ADC_ConvHalfCpltCallback() and HAL_ADC_ConvCpltCallback() are called.

Board settings:
 - ADC is configured to convert ADC_CHANNEL_5 (pin PA.00).
Two ways are possible to feed analog data to the ADC channel.  
 1°) The Eval Board potentiometer (RV3) connected to PA.00 is able to 
 make the ADC_CHANNEL_5 voltage vary.
 Therefore, the user can turn the potentiometer then press the tamper button.
Pressing the tamper button leads to a software check that verifies whether or not
a converted data is out of the analog watchdog 1 range. If this the case, LED1 
is turned on, otherwise, if the converted data at the time of the button press
is not out-of-range, LED1 is turned off.   
 
 2°) Alternatively, the user can connect the Eval Board ADC1 ADC_CHANNEL_5 PA.00 pin
 to the DAC1 output PA.04 pin.
 The voltage at the input of the ADC is increased in successive steps (up to 4 steps)
  at each click on the tamper button.
  When the voltage has reached its maximum value, the voltage restarts from its minimum value for a new steps cycle.
  Again, at each tamper button key press, the converted data is compared to the analog watchdog thresholds
 and LED1 is turned on if a data is out-of-range.


STM32L476G-EVAL board LEDs are used to monitor the program execution status:
 - Normal operation: LED1 is turned-on/off with respect to ADC conversion
   result.
    - Turned-off if voltage is within AWD window
    - Turned-on if voltage is out of AWD window
 - Error: In case of error, LED3 is toggling at a frequency of 1Hz.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - ADC/ADC_AnalogWatchdog/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - ADC/ADC_AnalogWatchdog/Inc/stm32l4xx_it.h          HAL interrupt handlers header file
  - ADC/ADC_AnalogWatchdog/Inc/main.h                  Header for main.c module  
  - ADC/ADC_AnalogWatchdog/Src/stm32l4xx_it.c          HAL interrupt handlers
  - ADC/ADC_AnalogWatchdog/Src/main.c                  Main program
  - ADC/ADC_AnalogWatchdog/Src/stm32l4xx_hal_msp.c     HAL MSP file
  - ADC/ADC_AnalogWatchdog/Src/system_stm32l4xx.c      STM32L4xx system source file

@par Hardware and Software environment 

  - This example runs on STM32L476xx devices.

  - This example has been tested with STM32L476G-EVAL board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 */
