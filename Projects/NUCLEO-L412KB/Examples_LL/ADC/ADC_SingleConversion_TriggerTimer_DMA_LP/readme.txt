/**
  @page ADC_SingleConversion_TriggerTimer_DMA_LP ADC example

  @verbatim
  ******************************************************************************
  * @file    Examples_LL/ADC/ADC_optimized_PWR/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the ADC_SingleConversion_TriggerTimer_DMA_LP example.
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
power consumption, to perform an acquisition at medium frequency
(between 10 Hz and 100 kHz).
This examples uses low-power sleep and low-power run modes.
ADC conversion data are transferred by DMA into a table, indefinitely (circular mode).
This example is based on the STM32L4xx ADC LL API.
The peripheral initialization is done using LL unitary service functions
for optimization purposes (performance and size).

Example configuration:
ADC is configured to convert a single channel, in single conversion mode;
conversion is triggered by timer peripheral.
DMA is configured to transfer conversion data in an array, in circular mode.
Timer is configured in time base and to generate TRGO events.
(frequency can be customized from 1 kHz to 320 kHz: refer to literal TIMER_FREQUENCY).
System clock and power are optimized for the lowest power consumption
(usage of "wait for event" feature).

Power consumption optimization options:
Compilation switches are available in main.h file to select optimizations:
 - OPTION_PWR_OPTIM_CPU_LOW_POWER_MODE_SLEEP (default: enabled): Set CPU in low-power mode "sleep" in idle phases (out of ADC, DMA or TIM interruption threads).
 - OPTION_PWR_OPTIM_ADC_DISABLE (default: enabled): Set ADC in the deepest low-power mode possible in idle phases (between ADC conversions). Relevant for ADC conversion triggers at low frequency (below 1kHz approximatively for configuration of this example): less ADC idle current consumption, but more CPU activity to manage ADC activation.
 - OPTION_MONITOR_CPU_ACTIVITY (default: disabled): Monitor CPU activity: when CPU is active (used by ADC, DMA or TIM interruption threads), turn LED on. This induces extra power consumption.
 - OPTION_THRESHOLD_DEBUG (default: disabled): Monitor converted values: when value is higher than 2048, turn LED on. This induces extra power consumption. To be used for debug.
 WARNING 1: OPTION_MONITOR_CPU_ACTIVITY and OPTION_THRESHOLD_DEBUG shouldn't be used at the same time.
 WARNING 2: when OPTION_THRESHOLD_DEBUG is used, the converted value is checked against the threshold only after the DMA transfer complete interruption and only one among the whole buffer of
            converted values is checked. This may lead to a few seconds latency between the time when the input voltage is modified and that when the LED behaves accordingly.

Example execution:
1. Peripherals and system are configured, LED3 is turned-on.
2. User has to connect PA.12 (CN3 connector pin D2) to ground to start the ADC conversion.
3. LED3 is turned-off and the ADC performs 1 conversion of the selected channel each time it is triggered by the timer.
   DMA transfers converted data to the array aADCxConvertedData.
4. When DMA reaches the selected number of transfers, DMA half transfer or transfer complete interruptions occur
   and data are converted into mV values in aADCxConvertedData_Voltage_mVolt array.
   Data array is updated indefinitely (DMA in circular mode).

For debug: variables to monitor with debugger watch window:
 - "aADCxConvertedData": ADC group regular conversion data
 - "aADCxConvertedData_Voltage_mVolt": ADC conversion data computation to physical values (array of data)

Note: Power consumption value:
      Several parameters have an impact on power consumption (CPU frequency,
      ADC frequency, ADC conversion trigger frequency, DMA buffer size to be processed).
      They can be tuned in function of user application.

Connection needed:
 Connect with a wire PA.12 to ground to start the ADC conversions.
Note: Optionally, a voltage can be supplied to the analog input pin (cf pin below),
      between 0V and Vdda=3.3V, to perform a ADC conversion on a determined
      voltage level.
      Otherwise, this pin can be let floating (in this case ADC conversion data
      will be undetermined).

Other peripherals used:
  1 GPIO for PA.12 (CN3 connector pin D2)
  1 GPIO for LED3
  1 GPIO for analog input: PA.04 (CN4 connector pin 9)
  DMA
  Timer

@note This example can not be used in debug mode due to the fact 
      that the Cortex-M4 core is no longer clocked during low power mode 
      so development environment debugging features are disabled. 

@par Directory contents

  - ADC/ADC_optimized_PWR/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - ADC/ADC_optimized_PWR/Inc/main.h                  Header for main.c module
  - ADC/ADC_optimized_PWR/Inc/stm32_assert.h          Template file to include assert_failed function
  - ADC/ADC_optimized_PWR/Src/stm32l4xx_it.c          Interrupt handlers
  - ADC/ADC_optimized_PWR/Src/main.c                  Main program
  - ADC/ADC_optimized_PWR/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L412xx devices.

  - This example has been tested with NUCLEO-L412KB MB1180 Rev C board and can be
    easily tailored to any other supported device and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
