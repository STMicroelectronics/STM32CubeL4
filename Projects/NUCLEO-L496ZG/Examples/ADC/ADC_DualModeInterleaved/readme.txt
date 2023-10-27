/**
  @page ADC_DualModeInterleaved Two ADC in interleaved dual-mode, data transfer by DMA

  @verbatim
  ******************************************************************************
  * @file    ADC/ADC_DualModeInterleaved/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Dual interleaved mode example
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

How to use two ADC peripherals to perform conversions in dual interleaved mode.


A compilation switch is available to select ADC configuration continuous mode
and external trigger (located in main.h):
 - "ADC_TRIGGER_FROM_TIMER" defined: ADC is operating in not continuous mode
   and conversions are triggered by external trigger: timer.
 - "ADC_TRIGGER_FROM_TIMER" not defined: ADC is operating in continuous mode
   and first conversion is triggered by software trigger.

A compilation switch is available to generate a waveform voltage
for test (located in main.h):
 - "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" defined: For this example purpose, generates a
   waveform voltage by DAC peripheral on a spare DAC channel: DAC_CHANNEL_1 on pin PA.04 (Arduino connector CN7 pin 17, Morpho connector CN11 pin 32).
   If ADC channel and DAC channel are selected on the same GPIO (default configuration
   in this example):
   connection is done internally in GPIO pad, user has nothing to connect.
   If ADC channel and DAC channel are selected on two different GPIO:
   user has just to connect a wire between DAC channel output and ADC input to run this example.
   This voltage follows a 5-step curve, ADC sampling rate is set so that for a given
   ADC, two conversions are carried out for one DAC output level.
 - "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" not defined: no voltage is generated, user has
   to connect a voltage source to the selected ADC channel input

Note: In this example, ADC sampling rate is not set as the highest possible (ADC parameters not set for this purpose: resolution 12 bits, ... ).
      ADC sampling rate can be increased by modifying resolution and sampling time, but this will decrease the conversion accuracy.

Other peripherals related to ADC are used:
Mandatory:
 - GPIO peripheral is used in analog mode to drive signal from device pin to
   ADC input.
Optionally:
 - Timer peripheral is used to trigger ADC conversions.
 - DMA peripheral is used to transfer ADC converted data.
   Both ADC are using the same DMA (DMA of ADC master). DMA is used as well by DAC in case of WAVEFORM_VOLTAGE_GENERATION_FOR_TEST enabled. 

ADC conversion results:
 - ADC conversions results are transferred automatically by DMA, into variable
   array "aADCDualConvertedValues".
   ADC master and ADC slave results are concatenated in a common 32-bit data register:
   - ADC master results in the 16 LSB [15:0] (with ADC resolution 12 bits, bits effectively used are [11:0])
   - ADC slave results in the 16 MSB [31:16] (with ADC resolution 12 bits, bits effectively used are [27:16])


 - DMA are configured to operate continuously, in circular mode.
   Data is transferred by words every ADC1+ADC2 conversions.
   When DMA transfer half-buffer and complete buffer length are reached, callbacks 
   HAL_ADC_ConvCpltCallback() and HAL_ADC_ConvCpltCallback() are called.

 - In this example, dual conversion values are extracted from the common register and
   stored into 2 arrays corresponding to each ADC conversion values
   (aADCxConvertedValues, aADCyConvertedValues).


Board settings:
  ADC1 and ADC2 are configured to convert the same channel: ADC_CHANNEL_9 on pin PA.04 (Arduino connector CN7 pin 17, Morpho connector CN11 pin 32).

  If compilation switch "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" is defined, the voltage input on ADC channel comes 
  from the DAC channel. This means that, if they are different, ADC pin PA.04 and DAC pin PA.04 must
  be connected by a wire. 

NUCLEO-L496ZG (144 pins) board LEDs are used to monitor the program execution status:
 - Normal operation: LED1 is turned-on/off in function of ADC conversion
   status:
    - Turned-off if ADC conversions have just filled up half of the buffer (DMA half-transfer interrupt has been triggered)
    - Turned-on if ADC conversions have filled up the complete buffer (DMA transfer complete interrupt has been triggered)
   The buffer fill up carries on indefinitely. 
 - Error: In case of error, LED3 is toggling at a frequency of 1Hz.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - ADC/ADC_DualModeInterleaved/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - ADC/ADC_DualModeInterleaved/Inc/stm32l4xx_it.h          DMA interrupt handlers header file
  - ADC/ADC_DualModeInterleaved/Inc/main.h                  Header for main.c module  
  - ADC/ADC_DualModeInterleaved/Src/stm32l4xx_it.c          DMA interrupt handlers
  - ADC/ADC_DualModeInterleaved/Src/main.c                  Main program
  - ADC/ADC_DualModeInterleaved/Src/stm32l4xx_hal_msp.c     HAL MSP file
  - ADC/ADC_DualModeInterleaved/Src/system_stm32l4xx.c      stm32l4xx system source file

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
