/**
  @page DFSDM_AudioRecord  Description of the DFSDM audio record example
  
  @verbatim
  ******************************************************************************
  * @file    DFSDM/DFSDM_AudioRecord/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DFSDM audio record example.
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

How to use the DFSDM HAL API to perform mono audio recording. This example uses
the MP34DT01 digital microphone mounted on the board.

MP34DT01 runs with a clock frequency from 1MHz to 3,25MHz.
This clock frequency has to be delivered by DFSDM. So channel of DFSDM will be configurated
to enable output clock generation, to choose audio clock as output clock source,
and output clock divider will be set to generate output clock on MP34DT01 frequency clock range.
Audio clock will be configurated to 11.294MHz, so DFSDM output clock divider will be set to 4.

The digital audio output from the microphone is coded in PDM (Pulse Density Modulation) and 
is connected to PE7. When CLK = 0, the audio PDM signal is sent on PE7.

PE7 can be configurated as DFSDM_DATIN2 (DFSDM channel 2 external data input).
We will use DFSDM channel 2 to get audio PDM signal. DFSDM channel 2 will be configurated to take
inputs from external serial inputs and from pins of channel 2. DFSDM channel 2 will also be 
configurated to operate in SPI mode with rising edge and with internal clock.

DFSDM filter 0 will be used to perform audio PDM signal conversions.
We will use a SINC3 filter with oversampling at 64 and integrator oversampling at 1.
Oversampling is set to 64 in order to have audio sample frequency at 44.1KHz (11.294MHz/(4*64)).
Regular conversions in fast mode will be performed in DMA mode on DFSDM filter 0 in order to fill
a buffer containing audio PCM samples at 44.1KHz.

Playback of the recorded data will be performed on headset using HAL_SAI and CS43L22 audio codec.
A circular playback buffer will be filled as soon as recorded data are available. 
When half record buffer will be filled, we put first part of recorded data on first half of playback buffer. 
When record buffer will be full filled, we put second part of recorded data on second half of playback buffer.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents  

  - DFSDM/DFSDM_AudioRecord/Src/main.c                  Main program
  - DFSDM/DFSDM_AudioRecord/Src/system_stm32l4xx.c      STM32L4xx system source file
  - DFSDM/DFSDM_AudioRecord/Src/stm32l4xx_it.c          Interrupt handlers
  - DFSDM/DFSDM_AudioRecord/Inc/main.h                  Main program header file
  - DFSDM/DFSDM_AudioRecord/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - DFSDM/DFSDM_AudioRecord/Inc/stm32l4xx_it.h          Interrupt handlers header file


@par Hardware and Software environment

  - This example runs on STM32L476xx devices.
    
  - This example has been tested with STMicroelectronics STM32L476G-Discovery
    board and can be easily tailored to any other supported device
    and development board.      

  - STM32L476G-Discovery Set-up :
    - Plug headset on AUDIO JACK connector.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 

 */
