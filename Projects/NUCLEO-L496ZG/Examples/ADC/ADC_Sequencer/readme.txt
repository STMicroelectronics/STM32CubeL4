/**
  @page ADC_Sequencer ADC conversion example using related peripherals (GPIO, DMA), voltage input from DAC, user control by user button and LED

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
   on DAC_CHANNEL_1 (output pin PA.04) by DAC peripheral.
   If ADC channel and DAC channel are selected on the same GPIO (default configuration
   in this example):
   connection is done internally in GPIO pad, user has nothing to connect.
   If ADC channel and DAC channel are selected on two different GPIO:
   user has just to connect a wire between DAC channel output and ADC input to run this example.
 - "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" not defined: no voltage is generated, user has
   to connect a voltage source to the selected ADC channel input to run this example


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
 - ADC rank 1 is configured to convert ADC_CHANNEL_9 (pin PA.04).
 - There are 2 possibilities:
   1 - If compilation switch "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" is defined:
     - The voltage input on ADC channel is provided from DAC channel.
     ADC and DAC channel have been chosen to have the same pad shared at device level: pin PA.04.
     ==> Therefore, there is no external connection needed to run this example.
     - Voltage is increasing at each click on user button, from 0 to maximum range in 4 steps.
       Clicks on user button follow circular cycles: At clicks counter maximum value reached, counter is set back to 0.
   2 - If compilation switch "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" is NOT defined:
     - The input voltage of ADC rank 1 channel must be provided by an external source connected to pin PA.04
     (Arduino connector CN7 pin 17, Morpho connector CN11 pin 32).

  Channels on sequencer ranks 2 and 3 are ADC internal channels: no external connection is required.
  
  To observe voltage level applied on ADC channel through GPIO, connect a voltmeter on
  pin PA.04 (Arduino connector CN7 pin 17, Morpho connector CN11 pin 32).


Test flow: 
  the user is expected to press the User button to trigger each conversion.

NUCLEO-L496ZG (144 pins) board's LEDs are used to monitor the program execution status:
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

  - This example runs on STM32L496ZG devices.

  - This example has been tested with NUCLEO-L496ZG (144 pins) board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 */
