/**
  @page DFSDM_Thermometer  Description of the DFSDM thermometer example
  
  @verbatim
  ******************************************************************************
  * @file    DFSDM/DFSDM_Thermometer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DFSDM thermometer example.
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

How to use the DFSDM HAL API to perform temperature measurements. This example
uses the PT100 (thermistor) and STPMS2 (sigma-delta modulator)
mounted on the board. The STPMS2 allows voltage and current values to be 
obtained from the PT100. The temperature value is thus deduced.

STPMS2 is mounted to run in low precision mode with a clock frequency of 1MHz.
This clock frequency has to be delivered by DFSDM. So first channel of DFSDM will be
configurated to enable output clock generation, to choose system clock as output clock source,
and output clock divider will be set to generate output clock of 1MHz.

STPMS2 is also mounted to output multiplexed signals. So when CLK = 0, the current channel 
output sigma-delta value is set on the DAT pin, and when CLK = 1, the voltage channel output
sigma-delta value is set on the DAT pin.
DAT pin of STPMS2 is connected to PC7. PC7 can be configurated as DFSDM_DATIN3 (DFSDM channel 3
external data input).
We will use DFSDM channel 3 to get voltage value. DFSDM channel 3 will be configurated to take
inputs from external serial inputs and from pins of channel 3. DFSDM channel 3 will also be 
configurated to operate in SPI mode with falling edge and with internal clock.
We will use also DFSDM channel 2 to get current value. DFSDM channel 2 will be configurated to take
inputs from external serial inputs and from pins of channel 3. DFSDM channel 2 will also be 
configurated to operate in SPI mode with rising edge and with internal clock.

DFSDM filter 0 will be used to perform voltage and current conversions.
We will use a SINC3 filter with oversampling at 64 and integrator oversampling at 64 (for averaging).
Injected conversions in scan mode on channels 2 and 3 will be perfomed in interrupt mode.
A timer will be used to periodically launch new injected conversions (every second).
When new conversion values are available, temperature is computed and displayed on LCD.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents  

  - DFSDM/DFSDM_Thermometer/Src/main.c                  Main program
  - DFSDM/DFSDM_Thermometer/Src/system_stm32l4xx.c      STM32L4xx system source file
  - DFSDM/DFSDM_Thermometer/Src/stm32l4xx_it.c          Interrupt handlers
  - DFSDM/DFSDM_Thermometer/Inc/main.h                  Main program header file
  - DFSDM/DFSDM_Thermometer/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - DFSDM/DFSDM_Thermometer/Inc/stm32l4xx_it.h          Interrupt handlers header file


@par Hardware and Software environment

  - This example runs on STM32L476xx devices.
    
  - This example has been tested with STMicroelectronics STM32L476G-EVAL
    board and can be easily tailored to any other supported device
    and development board.      

  - STM32L476G-EVAL Set-up :
    - Make sure that LCD Glass is in position IO.
    - Make sure that SB4, SB30 and SB42 are in default open state.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 

 */
