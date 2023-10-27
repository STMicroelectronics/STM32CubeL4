/**
  @page ADC_Sequencer ADC conversion example using related peripherals (GPIO, DMA), voltage input from DAC, user control by User push-button and LED

  @verbatim
  ******************************************************************************
  * @file    ADC/ADC_Sequencer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC conversion example
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

How to use the ADC peripheral with a sequencer to convert several channels. 
The channels converted are, in order, one external channel and two internal 
channels (VrefInt and temperature sensors).

Other peripherals related to ADC are used:
Mandatory:
 - GPIO peripheral is used in analog mode to drive signal from device pin to
   ADC input.
Optionally:
 - DMA peripheral is used to transfer ADC converted data.

ADC settings:
  Sequencer is enabled and set to convert 3 ranks (3 channels) in discontinuous
  mode, one by one, each conversion being software-triggered at each press on User push-button.

ADC conversion results:
 - ADC conversions results are transferred automatically by DMA, into variable
   array "aADCxConvertedData".
 - Each entry of this array contains one of the converted data of the ADC sequencer three ranks.
 - When DMA transfer half-buffer and full buffer lengths are reached, callbacks
   HAL_ADC_ConvHalfCpltCallback() and HAL_ADC_ConvCpltCallback() are called.
 - When the ADC sequence is complete (the three ADC conversions are carried out), 
   the voltages and temperature are computed and stored in variables:
     uhADCxConvertedData_VoltageGPIO_mVolt
     uhADCxConvertedData_VrefInt_mVolt
     hADCxConvertedData_Temperature_DegreeCelsius
     uhADCxConvertedData_VrefAnalog_mVolt


Board settings:
 - ADC rank 1 is configured to convert ADC_CHANNEL_9 (pin PA.04, Morpho connector CN6 pin 30).
 - User has to connect a voltage source to the selected ADC channel input to run this example.
   If ADC channel is let unconnected to a voltage source (pin floating), example will run but ADC conversions value will be not valid.

Channels on sequencer ranks 2 and 3 are ADC internal channels: no external connection is required.
  
To observe voltage level applied on ADC channel through GPIO, connect a voltmeter on
pin PA.04 (Morpho connector CN6 pin 30).


NUCLEO-L412RB-P Rev C board LED is be used to monitor the program execution status:
 - Normal operation: LED4 is turned-on/off with respect to ADC conversion results.
    - Turned-off if sequencer has not yet converted all ranks
    - Turned-on if sequencer has converted all ranks
 - Error: In case of error, LED4 is toggling at a frequency of 1Hz.


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

  - This example runs on STM32L412xx devices.

  - This example has been tested with NUCLEO-L412RB-P Rev C board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 */
