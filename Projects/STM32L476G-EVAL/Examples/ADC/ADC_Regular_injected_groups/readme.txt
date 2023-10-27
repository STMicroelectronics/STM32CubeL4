/**
  @page ADC_Regular_injected_groups ADC conversion example showing the usage of
   the 2 ADC groups: regular group for ADC conversions on main stream and 
   injected group for ADC conversions limited on specific events (conversions
   injected within main conversions stream),
   using related peripherals (GPIO, DMA, Timer), voltage input from DAC, user 
   control by push button and LED

  @verbatim
  ******************************************************************************
  * @file    ADC/ADC_Regular_injected_groups/readme.txt 
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

How to use the ADC peripheral to perform conversions using the two ADC groups: 
regular group for ADC conversions on the main stream, and injected group for 
ADC conversions limited to specific events (conversions injected into the 
main conversion stream).

Other peripherals related to ADC are used:
Mandatory:
 - GPIO peripheral is used in analog mode to drive signal from device pin to
   ADC input.
Optionally:
 - Timer peripheral is used to trigger ADC conversions.
 - DMA peripheral is used to transfer ADC converted data.

ADC settings:
 - Regular group:
   Conversion is triggered by external event (timer at 1kHz).
   Number of conversions is set to 1 and continuous mode is disabled: there is
   only 1 conversion at each trigger event.
 - Injected group: 
   Conversion is software-triggered by a mere call to HAL_ADCEx_InjectedStart_IT()  
   to convert VREFINT channel at each press on Tamper button.

ADC conversion results:
 - ADC regular conversions results are transferred automatically by DMA, into variable
   array "aADCxConvertedValues".
 - When DMA transfer half-buffer and buffer lengths are reached, callbacks 
   HAL_ADC_ConvCpltCallback() and HAL_ADC_ConvCpltCallback() are called.
 - ADC injected conversions results are retrieved upon JEOC interrupt by
   HAL_ADCEx_InjectedConvCpltCallback() function.   

Board settings:
ADC is configured to convert ADC_CHANNEL_5 (pin PA.00).
Two ways are possible to feed analog data to the ADC channel.  
 1°) The Eval Board potentiometer (RV3) connected to PA.00 is able to 
 make the ADC_CHANNEL_5 voltage vary.
 Therefore, the user can turn the potentiometer then press the tamper button.
 The injected converted data is then compared to the average value of the regular converted data.
 Note that there is no external connection needed to run this way.
 
 2°) Alternatively, the user can connect the Eval Board ADC1 ADC_CHANNEL_5 PA.00 pin
 to the DAC1 output PA.04 pin.
 The voltage at the input of the ADC is increased in successive steps (up to 4 steps)
  at each click on tamper button.
  When the voltage has reached its maximum value, the voltage restarts from its minimum value for a new steps cycle.
  Again, at each tamper button key press, the injected converted data is compared to the average value of the 
  regular converted data 
  

STM32L476G-EVAL board's LEDs are be used to monitor the program execution status:
 - Normal operation: LED1 is turned-on/off in function of ADC conversion
   result.
    - Turned-off if voltage measured by injected group is below voltage measured by regular group (average of results table)
    - Turned-on if voltage measured by injected group is above voltage measured by regular group (average of results table)
 - Error: In case of error, LED3 is toggling at a frequency of 1Hz.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - ADC/ADC_Regular_injected_groups/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - ADC/ADC_Regular_injected_groups/Inc/stm32l4xx_it.h          HAL interrupt handlers header file
  - ADC/ADC_Regular_injected_groups/Inc/main.h                  Header for main.c module  
  - ADC/ADC_Regular_injected_groups/Src/stm32l4xx_it.c          HAL interrupt handlers
  - ADC/ADC_Regular_injected_groups/Src/main.c                  Main program
  - ADC/ADC_Regular_injected_groups/Src/stm32l4xx_hal_msp.c     HAL MSP file
  - ADC/ADC_Regular_injected_groups/Src/system_stm32l4xx.c      STM32L4xx system source file

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
