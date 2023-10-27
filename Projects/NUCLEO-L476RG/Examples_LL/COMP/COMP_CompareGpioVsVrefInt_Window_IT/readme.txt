/**
  @page COMP_CompareGpioVsVrefInt_Window_IT COMP example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/COMP/COMP_CompareGpioVsVrefInt_Window_IT/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the COMP_CompareGpioVsVrefInt_Window_IT example.
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

How to use a pair of comparator peripherals to compare a voltage level applied on
a GPIO pin to two thresholds: the internal voltage reference (VREFINT) and a fraction
of the internal voltage reference (VREFINT/2), in interrupt mode. This example is 
based on the STM32L4xx COMP LL API. The peripheral initialization is done using LL 
unitary service functions for optimization purpose (performance and size).

Example configuration:
Comparator instances COMP1 and COMP2 are configured to work together
in window mode to compare 2 thresholds:
 - COMP1 input plus set to a GPIO pin (cf pin below) in analog mode.
   COMP2 input plus is not used (connected internally to the 
   same input plus as COMP1).
 - COMP1 input minus set to internal voltage reference VrefInt 
   (voltage level 1.2V, refer to device datasheet for min/typ/max values)
 - COMP2 input minus set to 1/2 of internal voltage reference VrefInt 
   (voltage level 0.6V)
Comparator interruption is enabled through EXTI lines 21 and 22
with trigger edge set to both edges rising and falling.
Comparators are set in power mode ultra low power.

Example execution:
From the main program execution, comparator is enabled.
Each time the voltage level applied on GPIO pin (comparator input plus) 
is crossing VrefInt or 1/2 VrefInt voltage thresholds (comparator input minus),
the comparator with corresponding threshold generates an interruption.
LED is turned on or off depending on the pair of comparators output state:
turned on if comparator voltage level is inside the window (between VrefInt
and 1/2 VrefInt (between 1.2V and 0.6V)), turned-off otherwise.

Connection needed:
A voltage must be supplied to the analog input pin (cf pin below),
between Vdda=3.3V and 0V. Otherwise this pin can be let floating (but in this
case COMP comparison level will be undetermined).

To check comparator window thresholds, you can:
 - Either use a wire to connect the GPIO pin of comparator input (cf pin below)
   to other connectors on board:
    - below window thresholds: to ground
    - above window thresholds: to voltage of power supply Vdda
      (on board Nucleo: Vdda=3.3V, available on CN6 pin 4 or CN7 pin 5)
    - into window thresholds: an intermediate voltage level (900mV)
      is generated for this example by DAC1 channel1, 
      connect DAC channel output to comparator input
      (see pins below, with list of GPIO used).
      This waveform generation can be disabled by commenting literal
      definition "CONSTANT_VOLTAGE_GENERATION" (in this case the pin is let floating).
 - Either use an external power supply

Other peripheral used:
  1 GPIO for LED
  1 GPIO for analog input of comparator: PC.05 (Morpho connector CN10 pin 6)
  EXTI line 21 (connected to COMP1 output)
  EXTI line 22 (connected to COMP2 output)
  1 GPIO for DAC channel output PA.04 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32)

@par Directory contents 

  - COMP/COMP_CompareGpioVsVrefInt_Window_IT/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - COMP/COMP_CompareGpioVsVrefInt_Window_IT/Inc/main.h                  Header for main.c module
  - COMP/COMP_CompareGpioVsVrefInt_Window_IT/Inc/stm32_assert.h          Template file to include assert_failed function
  - COMP/COMP_CompareGpioVsVrefInt_Window_IT/Src/stm32l4xx_it.c          Interrupt handlers
  - COMP/COMP_CompareGpioVsVrefInt_Window_IT/Src/main.c                  Main program
  - COMP/COMP_CompareGpioVsVrefInt_Window_IT/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L476xx devices.
    
  - This example has been tested with NUCLEO-L476RG Rev C board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
