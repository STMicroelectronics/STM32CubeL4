/**
  @page ADC_DifferentialMode ADC conversion of 2 channels in differential mode, voltage input from DAC, user control by push button and LED

  @verbatim
  ******************************************************************************
  * @file    ADC/ADC_DifferentialMode/readme.txt 
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

How to use an ADC peripheral to perform a conversion in differential mode 
between 2 ADC channels.

ADC settings:
  Continuous mode to have conversions kept running after 1st conversion trig.
  Software start to trig the 1st conversion manually, without external event.
  Channel in differential mode.

ADC conversion results:
  ADC conversions results are transferred automatically by ADC IRQ handler, through
  function HAL_ADC_ConvCpltCallback(), into variable "uhADCxConvertedValue".

  ADC Differential conversion result calculation:                       
   - An offset of half of full range is added to keep the full dynamic range of differential voltage.                                     
   - Channels voltage is divided by 2, resolution is lowered of 1 bit.  
   => Diff conversion result = mid-range + (channel_high-channel_low)/2                

Board settings:
   Instance ADC2 is used in the example.
   User has to connect two voltage sources to the selected ADC channel inputs to run this example.
   - channel_high input is connected to ADC_CHANNEL_9 (pin PA.04, Morpho connector CN6 pin 30)
   - channel_low input is connected to ADC_CHANNEL_10 (pin PA.05, Morpho connector CN6 pin 28)
   
   User can read "uhADCxConvertedValue" to get the ADC converted differential value.


STM32 board's LEDs are be used to monitor the transfer status:
 - Normal operation: LED4 is turned-on/off in function of ADC conversion result.
    - Turned-on if differential voltage is positive
    - Turned-off if differential voltage is negative

 - Error: In case of error, LED4 is toggling at a frequency of 1Hz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - ADC/ADC_DifferentialMode/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - ADC/ADC_DifferentialMode/Inc/stm32l4xx_it.h          ADC interrupt handlers header file
  - ADC/ADC_DifferentialMode/Inc/main.h                  Header for main.c module  
  - ADC/ADC_DifferentialMode/Src/stm32l4xx_it.c          ADC interrupt handlers
  - ADC/ADC_DifferentialMode/Src/main.c                  Main program
  - ADC/ADC_DifferentialMode/Src/stm32l4xx_hal_msp.c     HAL MSP file
  - ADC/ADC_DifferentialMode/Src/system_stm32l4xx.c      stm32l4xx system source file

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
