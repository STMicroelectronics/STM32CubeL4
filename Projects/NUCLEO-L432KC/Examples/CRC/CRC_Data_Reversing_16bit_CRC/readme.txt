/**
  @page CRC_Data_Reversing_16bit_CRC 16-bit CRC Computation With Data Reversal Options Enabled Example
  
  @verbatim
  ******************************************************************************
  * @file    CRC/CRC_Data_Reversing_16bit_CRC/readme.txt 
  * @author  MCD Application Team
  * @brief   16-bit long CRC computation with data reversal options enabled.
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

How to configure the CRC using the HAL API. The CRC (cyclic
redundancy check) calculation unit computes a 16-bit CRC code derived from a
buffer of 8-bit data (bytes). Input and output data reversal features are
enabled. The user-defined generating polynomial is manually set to 0x1021,
that is, X^16 + X^12 + X^5 + 1 which is the CRC-CCITT generating polynomial.

At the beginning of the main program, the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK).

The CRC peripheral configuration is ensured by HAL_CRC_Init() function.
The latter is calling HAL_CRC_MspInit() function which core is implementing
the configuration of the needed CRC resources according to the used hardware (CLOCK). 
You can update HAL_CRC_Init() input parameters to change the CRC configuration.

In this example, the user-defined generating polynomial is configured by 
HAL_CRC_Init(). At the same time, input data reversal feature is set to bit 
reversal on full word.
Output data reversal is enabled as well (only bit-level reversal option is available). 
Additionally, the default init value is discarded and a user-defined one is used 
instead. 
It is specified finally that input data type is byte.

The 16-bit long CRC is computed, stored in uwCRCValue variable then compared to the 
CRC expected value stored in uwExpectedCRCValue.


For clarity's sake, a second CRC computation is then carried out with the input 
and output data reversal options disabled, everything else being equal.

The input buffer used for this second CRC computation is derived from the
first input buffer by a bit-reversal operation carried out on full word (and vice versa).

It is checked that the same CRC is derived, taking into account that output data 
reversal feature is disabled (i.e. the newly computed 16-bit CRC is equal to the 
bit-reversed first one).
 



STM32 board LED is used to monitor the example status:
  - LED3 is ON when the correct CRC value is calculated
  - LED3 is slowly blinking (1 sec. period) when there is an error in initialization or if an incorrect CRC value is calculated.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 
  
  - CRC/CRC_Data_Reversing_16bit_CRC/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - CRC/CRC_Data_Reversing_16bit_CRC/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - CRC/CRC_Data_Reversing_16bit_CRC/Inc/main.h                  Header for main.c module
  - CRC/CRC_Data_Reversing_16bit_CRC/Src/stm32l4xx_it.c          Interrupt handlers
  - CRC/CRC_Data_Reversing_16bit_CRC/Src/main.c                  Main program
  - CRC/CRC_Data_Reversing_16bit_CRC/Src/stm32l4xx_hal_msp.c     HAL MSP module 
  - CRC/CRC_Data_Reversing_16bit_CRC/Src/system_stm32l4xx.c      STM32L4xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32L432xx devices.
  - This example has been tested with NUCLEO-L432KC RevC board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 */
 