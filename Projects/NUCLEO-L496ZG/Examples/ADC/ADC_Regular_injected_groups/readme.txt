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


One compilation switch is available to select ADC configuration continuous mode
and external trigger (located in main.h):
 - "ADC_TRIGGER_FROM_TIMER" defined: ADC is operating in not continuous mode
   and conversions are trigger by external trigger: timer.
 - "ADC_TRIGGER_FROM_TIMER" not defined: ADC is operating in continuous mode
   and first conversion is trigger by software trigger.

One compilation switch is available to generate a waveform voltage
for test (located in main.h):
 - "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" defined: For this example purpose, generates a
   waveform voltage on a spare DAC channel: DAC_CHANNEL_1 on pin PA.04 (Arduino connector CN7 pin 17, Morpho connector CN11 pin 32)
   If ADC channel and DAC channel are selected on the same GPIO (default configuration
   in this example):
   connection is done internally in GPIO pad, user has nothing to connect.
   If ADC channel and DAC channel are selected on two different GPIO:
   user has just to connect a wire between DAC channel output and ADC input to run this example.
 - "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" not defined: no voltage is generated, user has
   to connect a voltage source to the selected ADC channel input to run this example.


Other peripherals related to ADC are used:
Mandatory:
 - GPIO peripheral is used in analog mode to drive signal from device pin to
   ADC input.
Optionally:
 - Timer peripheral is used to trigger ADC conversions.
 - DMA peripheral is used to transfer ADC converted data.

ADC settings:
 - Regular group:
   Conversions are triggered by external event (timer at 1kHz).
 - Continuous mode is disabled (and sequencer disabled: only 1 channel selected) to yield only 1 conversion at each conversion trigger.
 - Injected group: 
   Conversions are software-triggered by a mere call to HAL_ADCEx_InjectedStart_IT()  
   to convert VREFINT channel at each press on user button.

ADC conversion results:
 - ADC regular conversions results are transferred automatically by DMA, into variable
   array "aADCxConvertedValues".
 - DMA and ADC are configured to operate continuously, in circular mode.
   When DMA transfer half-buffer and full buffer lengths are reached, callbacks 
   HAL_ADC_ConvHalfCpltCallback() and HAL_ADC_ConvCpltCallback() are called.
 - ADC injected conversions results are retrieved upon JEOC interrupt by
   HAL_ADCEx_InjectedConvCpltCallback() function.

Board settings:
 - ADC is configured to convert ADC_CHANNEL_9 on pin PA.04 (Arduino connector CN7 pin 17, Morpho connector CN11 pin 32).
 - Two ways are possible to feed analog data to the ADC channel.
   1°) The voltage input on ADC channel is provided from DAC channel. ("WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" is defined)
     - ADC and DAC channel have been chosen to have the same pad shared at device level: pin PA.04.
       ==> Therefore, there is no external connection needed to run this example.
     - Voltage is increasing at each click on user button, from 0 to maximum range in 4 steps.
       Clicks on user button follow circular cycles: At clicks counter maximum value reached, counter is set back to 0.
   2°) The voltage input on ADC channel is provided from external voltage source ("WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" is not defined)
     - External voltage source is connected to pin PA.04 (Arduino connector CN7 pin 17, Morpho connector CN11 pin 32).
 - Channel configured on injected group:
   The voltage input is provided from internal reference voltage VrefInt.

NUCLEO-L496ZG (144 pins) board LEDs are used to monitor the program execution status:
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

  - This example runs on STM32L496ZG devices.

  - This example has been tested with NUCLEO-L496ZG (144 pins) board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 */
