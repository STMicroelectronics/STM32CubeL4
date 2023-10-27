/**
  @page ADC_SingleConversion_Stop2_LP ADC example

  @verbatim
  ******************************************************************************
  * @file    Examples_LL/ADC/ADC_SingleConversion_Stop2_LP/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the ADC_SingleConversion_Stop2_LP example.
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

How to use an ADC peripheral with a system configuration optimized for
power consumption, to perform an acquisition at low frequency
(between 1Hz and 1kHz).
This examples uses low-power Stop 2 mode and RTC.
ADC conversion data are transferred by DMA into a table, indefinitely (circular mode).
This example is based on the STM32L4xx ADC LL API.
The peripheral initialization is done using LL unitary service functions
for optimization purposes (performance and size).

Example configuration:
ADC is configured to convert a single channel, in single conversion mode,
from SW trigger.
DMA is configured to transfer conversion data in an array, in circular mode.
RTC in configured to generate a wake-up event at a frequency of 10 Hz
(frequency can be customized from 1 Hz to 1 kHz: refer to literal ACQUISITION_FREQUENCY_HZ).
System clock and power are optimized for the lowest power consumption
(usage of "wait for event" feature).

Power consumption optimization options:
Compilation switches are available to select optimizations:
 - OPTION_PWR_OPTIM_BUFFER_PROCESSING (default: enabled): Process DMA buffer when completed half by half (triggered by DMA half transfer and transfer complete interruptions): will slightly increase CPU activity (and therefore power consumption). */
 - OPTION_PWR_OPTIM_ADC_DISABLE (default: enabled): Set ADC in the deepest low-power mode possible in idle phases (between ADC conversions). Relevant for ADC conversion triggers at low frequency: less ADC idle current consumption, but more CPU activity to manage ADC activation.
 - OPTION_MONITOR_CPU_ACTIVITY (default: disabled): Monitor CPU activity: when CPU is active, turn LED on. This induces extra power consumption. To be used for debug. */
 - OPTION_THRESHOLD_DEBUG (default: disabled): Monitor converted values: when value is higher than 2048, turn LED on. This induces extra power consumption. To be used for debug.
 WARNING: OPTION_MONITOR_CPU_ACTIVITY and OPTION_THRESHOLD_DEBUG shouldn't be used at the same time.

 
Example execution:
Peripherals and system are configured, RTC is launched and system enters in low-power mode Stop2.
The ADC performs 1 conversion of the selected channel at each wake-up from RTC.
DMA transfers conversion data to the array.
When DMA reaches the selected number of transfers,
DMA half transfer or transfer complete interruptions occurs.
Data array is updated indefinitely (DMA in circular mode).


Profile of current consumption during execution phases:
 - At RTC wake-up, CPU activity: activate ADC, launch ADC conversion, clear RTC status
 - During ADC conversion, CPU is set in sleep mode
 - At ADC end of conversion, CPU activity: clear ADC status. When DMA reaches half buffer size, additional CPU activity is performed (refer to OPTION_PWR_OPTIM_BUFFER_PROCESSING).
 - During wait duration with RTC, CPU is set in low-power stop 2 mode.
                    ____                ____                           ____                ____
                   |    |              |    |                         |    |              |    |
  power            |    |______________|    |                         |    |______________|    |
  consumption	   |                        |                         |                        |
  		   |                        |_____________..._________|                        |__________________

  low-power mode    run      sleep      run             stop             run      sleep      run          stop

  ADC               start  conversion   end            deactivated      start  conversion   end        deactivated
                    conv    on going    conv                            conv    on going    conv

For debug: variables to monitor with debugger watch window:
Due to entry in low-power mode and power optimization configuration (GPIO linked to debugger are disconnected), connection to debugger is lost.
Therefore, ADC conversion data transferred to variables ("aADCxConvertedData" and "aADCxConvertedData_Voltage_mVolt") cannot be monitored outside of test program.

Note: Power consumption value:
      Several parameters have an impact on power consumption (CPU frequency,
      ADC frequency, ADC conversion trigger frequency, DMA buffer size to be processed).
      They can be tuned in function of user application.

      Additionally, LSE clock propagation to other peripherals except RTC is disabled,
      BOR and PVD optimized monitoring for stop 2 mode is enabled.

Connection needed:
None.
Note: Optionally, a voltage can be supplied to the analog input pin (cf pin below),
      between 0V and Vdda=3.3V, to perform a ADC conversion on a determined
      voltage level.
      Otherwise, this pin can be let floating (in this case ADC conversion data
      will be undetermined).

Other peripherals used:
  1 GPIO for User push-button
  1 GPIO for LED3
  1 GPIO for analog input: PA.04 (Arduino connector CN4 pin 9)
  DMA
  RTC

@par Directory contents

  - ADC/ADC_optimized_PWR/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - ADC/ADC_optimized_PWR/Inc/main.h                  Header for main.c module
  - ADC/ADC_optimized_PWR/Inc/stm32_assert.h          Template file to include assert_failed function
  - ADC/ADC_optimized_PWR/Src/stm32l4xx_it.c          Interrupt handlers
  - ADC/ADC_optimized_PWR/Src/main.c                  Main program
  - ADC/ADC_optimized_PWR/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L412xx devices.

  - This example has been tested with STM32L412KB-Nucleo Rev C board and can be
    easily tailored to any other supported device and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
