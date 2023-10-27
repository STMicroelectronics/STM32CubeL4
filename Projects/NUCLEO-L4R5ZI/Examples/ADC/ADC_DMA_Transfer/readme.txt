/**
  @page ADC_DMA_Transfer ADC DMA Transfer example

  @verbatim
  ******************************************************************************
  * @file    ADC/ADC_DMA_Transfer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC DMA Transfer example.
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

How to configure and use the ADC to convert an external analog input and get 
the result using a DMA transfer through the HAL API.

The ADC is configured to convert continuously ADC_CHANNEL_9, resolution is set to 12
bits right aligned, conversion is software-triggered.

DMA1_Channel1 is configured in Circular mode to transfer continuously
the content of ADC_DR (Data Register) to "aADCxConvertedData" variable which 
stores the conversion result.

User may watch variable array "aADCxConvertedData" and check that its values are modified
according to the voltage applied to pin PA.04 (Arduino connector CN7 pin 17, Morpho connector CN11 pin 32).

STM32 board LED can be used to monitor the conversion:
  - LED1 is ON when there is an error in initialization.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - ADC/ADC_DMA_Transfer/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - ADC/ADC_DMA_Transfer/Inc/stm32l4xx_it.h          HAL interrupt handlers header file
  - ADC/ADC_DMA_Transfer/Inc/main.h                  Header for main.c module  
  - ADC/ADC_DMA_Transfer/Src/stm32l4xx_it.c          HAL interrupt handlers
  - ADC/ADC_DMA_Transfer/Src/main.c                  Main program
  - ADC/ADC_DMA_Transfer/Src/stm32l4xx_hal_msp.c     HAL MSP file 
  - ADC/ADC_DMA_Transfer/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L4R5xx devices.

  - This example has been tested with NUCLEO-L4R5ZI (144 pins) board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-L4R5ZI (144 pins) Set-up
    Use an external power supply, adjust supply voltage and connect it to pin PA.04 (Arduino connector CN7 pin 17, Morpho connector CN11 pin 32).

  - To monitor the conversion result, put the "aADCxConvertedData" variable in the debugger live watch.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 */
