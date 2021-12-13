/**
  @page ADC_Sequencer ADC conversion example using related peripherals (GPIO, DMA), voltage input from DAC, user control by push button and LED

  @verbatim
  ******************************************************************************
  * @file    ADC/ADC_Sequencer/readme.txt 
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

How to use the ADC peripheral with a sequencer to convert several channels. 
The channels converted are, in order, one external channel and two internal 
channels (VrefInt and temperature sensors).


WAVEFORM_VOLTAGE_GENERATION_FOR_TEST compilation switch (located in main.h) is  
available to generate a waveform voltage for the test :
 - "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" defined: waveform voltage for test is generated
   on DAC_CHANNEL_1 (output pin PA.04) by DAC peripheral,
   so user has just to connect a wire between DAC channel output and ADC input 
   (input pin PA.00) to run this example.
 - "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" not defined: no voltage is generated, user has
   to connect a voltage source to the selected ADC channel input to run this example
   or resort to the Eval Board potentiometer (RV3) connected to PA.00 
   in order to make the external channel ADC_CHANNEL_5 voltage vary.


Other peripherals related to ADC are used:
Mandatory:
 - GPIO peripheral is used in analog mode to drive signal from device pin to
   ADC input.
Optionally:
 - DMA peripheral is used to transfer ADC converted data.

ADC settings:
  Sequencer is enabled and set to convert 3 ranks (3 channels) in discontinuous
  mode, one by one, each conversion being software-triggered.

ADC conversion results:
 - ADC conversions results are transferred automatically by DMA, into variable
   array "aADCxConvertedValues".
 - Each entry of this array contains one of the converted data of the ADC sequencer three ranks.
 - When DMA transfer half-buffer and buffer lengths are reached, callbacks
   HAL_ADC_ConvHalfCpltCallback() and HAL_ADC_ConvCpltCallback() are called.
 - When the ADC sequence is complete (the three ADC conversions are carried out), 
   the voltages and temperature are computed and stored in variables:
   uhADCChannelToDAC_mVolt, uhVrefInt_mVolt, wTemperature_DegreeCelsius.


Board settings:
 - ADC rank 1 is configured to convert ADC_CHANNEL_5 (pin PA.00).
  If compilation switch "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" is defined, the voltage input on ADC channel comes 
  from the DAC channel. This means that pins PA.00 and PA.04 must
  be connected by a wire. 
  Otherwise, the input of ADC rank 1 channel can be provided by the Eval Board potentiometer (RV3).
   Turning this potentiometer makes the voltage vary over the full Vdda range: from 0 to Vdda (3.3V).
   ==> Therefore, there is no external connection needed to run this example.

  Ranks 2 and 3 are internal channel conversions: no wiring is required.



Test flow: 
  the user is expected to press the Tamper button to trigger each conversion.

STM32L476G-EVAL board's LEDs are used to monitor the program execution status:
 - Normal operation: LED1 is turned-on/off with respect to ADC conversion results.
    - Turned-off if sequencer has not yet converted all ranks
    - Turned-on if sequencer has converted all ranks
 - Error: In case of error, LED3 is toggling at a frequency of 1Hz.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - ADC/ADC_Sequencer/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - ADC/ADC_Sequencer/Inc/stm32l4xx_it.h          DMA interrupt handlers header file
  - ADC/ADC_Sequencer/Inc/main.h                  Header for main.c module  
  - ADC/ADC_Sequencer/Src/stm32l4xx_it.c          DMA interrupt handlers
  - ADC/ADC_Sequencer/Src/main.c                  Main program
  - ADC/ADC_Sequencer/Src/stm32l4xx_hal_msp.c     HAL MSP file
  - ADC/ADC_Sequencer/Src/system_stm32l4xx.c      STM32L4xx system source file

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
