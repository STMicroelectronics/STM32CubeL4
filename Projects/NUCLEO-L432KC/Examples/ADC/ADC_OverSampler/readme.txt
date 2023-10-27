/**
  @page ADC_OverSampler ADC OverSampler example

  @verbatim
  ******************************************************************************
  * @file    ADC/ADC_OverSampler/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC Oversampler example.
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

How to use an ADC peripheral in oversampling mode to increase resolution.

The ADC is configured to convert continuously ADC_CHANNEL_9 connected to GPIO_PIN_4 of GPIOA
while the oversampler is enabled. 
Oversampling ratio is set to ADC_OVERSAMPLING_RATIO_128 so the maximum output result 
is 0xFFF * 128 = 0x7FF80 (19 bits).
Since the ADC_DR is a 16-bit long register, a 3-bit right shift is performed to 
keep only the 16 most significant bits.

uwConvertedValue variable contains the 16-bit conversion result and 
uwInputVoltage yields the input voltage in mV. 

Use an external power supply, adjust supply voltage and connect it to pin GPIO_PIN_4 of GPIOA (Arduino pin A3).
This allows to modify ADC_CHANNEL_9 voltage.
The converted value is monitored through debugger: uwConvertedValue and uwInputVoltage variables.

STM32 board LED can be used to monitor the conversion:
  - LED3 is ON when there is an error occurs.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - ADC/ADC_OverSampler/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - ADC/ADC_OverSampler/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - ADC/ADC_OverSampler/Inc/main.h                  Header for main.c module  
  - ADC/ADC_OverSampler/Src/stm32l4xx_it.c          Interrupt handlers
  - ADC/ADC_OverSampler/Src/main.c                  Main program
  - ADC/ADC_OverSampler/Src/stm32l4xx_hal_msp.c     HAL MSP file 
  - ADC/ADC_OverSampler/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment 

  - This example runs on NUCLEO-L432KC RevC devices.

  - This example has been tested with NUCLEO-L432KC RevC board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 */
