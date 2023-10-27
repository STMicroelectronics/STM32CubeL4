/**
  @page ADC_Oversampling ADC example
  
  @verbatim
  ******************************************************************************
  * @file    Examples/ADC/ADC_Oversampling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_Oversampling example.
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

How to use an ADC peripheral in oversampling mode to increase resolution.

Description of ADC oversampling:
Multiple successive ADC conversions and average computation
are performed automatically, by ADC hardware, and therefore
off-load the CPU to do the equivalent task.
This feature can be used for the functions: averaging,
data rate reduction, SNR improvement, basic filtering.
This example uses 3 configurations of oversampling,
for comparison of final data to evaluate oversampling benefits.


Example configuration:
ADC is configured to convert a single channel, in single conversion mode,
from SW trigger.
ADC oversampling feature is used with 3 settings:
 - ADC oversampling enabled: ratio 16, bits right shift 4.
 - ADC oversampling enabled: ratio 16, no bits right shift.
 - ADC oversampling disabled.

Connection needed:
A voltage between 0V and Vdda=3.3V can be supplied to the analog input pin PA.04 
(Morpho connector CN6 pin 30) to perform a ADC conversion at a determined voltage level.
Otherwise, this pin can be left floating (in this case ADC conversion data will be undetermined).

Other peripherals used:
  1 GPIO for User push-button
  1 GPIO for LED
  1 GPIO for analog input: PA.04 (Morpho connector CN6 pin 30)



For debug: variables to monitor with debugger watch window:
 - "uhADCxConvertedData_OVS_ratio16_shift4":   ADC group regular conversion data, oversampling ratio 16 and shift 4 (data scale: 12 bits)
 - "uhADCxConvertedData_OVS_ratio16_shift0":   ADC group regular conversion data, oversampling ratio 16 and shift 4 (data scale: 16 bits)
 - "uhADCxConvertedData_OVS_disabled":         ADC group regular conversion data, oversampling disabled (data scale corresponds to ADC resolution: 12 bits)
 - "fConvertedData_OVS_EquivalentValue12bits": Calculation of oversampling raw data (from variable "uhADCxConvertedData_OVS_ratio16_shift0") to the equivalent data on 12 bits with floating point. 

 @par Directory contents

  - ADC/ADC_Oversampling/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - ADC/ADC_Oversampling/Inc/main.h                  Header for main.c module  
  - ADC/ADC_Oversampling/Src/stm32l4xx_it.c          Interrupt handlers
  - ADC/ADC_Oversampling/Src/main.c                  Main program
  - ADC/ADC_Oversampling/Src/system_stm32l4xx.c      STM32L4xx system source file


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
