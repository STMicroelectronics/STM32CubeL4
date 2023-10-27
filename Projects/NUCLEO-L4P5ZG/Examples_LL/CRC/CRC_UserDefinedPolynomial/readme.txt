/**
  @page CRC_UserDefinedPolynomial CRC : 8-bit CRC calculation using user-defined polynomial value
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/CRC/CRC_UserDefinedPolynomial/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRC_UserDefinedPolynomial example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to configure and use the CRC calculation unit to compute an 8-bit CRC code
for a given data buffer, based on a user-defined generating polynomial. The
peripheral initialization is done using LL unitary service functions for
optimization purposes (performance and size).

The CRC peripheral is configured to work with a user defined polynomial value (8-bit long value).
In this example, the polynomial is set manually to 0x9B that is X^8 + X^7 + X^4 + X^3 + X + 1.
Generated CRC value is then 8-bit long.

Example execution:
After startup from reset and system configuration, CRC configuration is performed (use of user-defined Polynomial value).
CRC code of a given data buffer is computed. 
Data buffer length has been chosen as not an exact nb of u32 (32-bit words), in order to illustrate 
use of offered API for feeding the calculator (u32, u16 or u8 inputs).
The calculated CRC code is stored in ubCRCValue variable.
Once calculated, CRC value (ubCRCValue) is compared to the CRC expected value (ubExpectedCRCValue),
and if both are equal, LED1 is turned On. 
In case of errors, LED1 is blinking (1sec period).

@par Directory contents 

  - CRC/CRC_UserDefinedPolynomial/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - CRC/CRC_UserDefinedPolynomial/Inc/main.h                  Header for main.c module
  - CRC/CRC_UserDefinedPolynomial/Inc/stm32_assert.h          Template file to include assert_failed function
  - CRC/CRC_UserDefinedPolynomial/Src/stm32l4xx_it.c          Interrupt handlers
  - CRC/CRC_UserDefinedPolynomial/Src/main.c                  Main program
  - CRC/CRC_UserDefinedPolynomial/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L4P5xx devices.
    
  - This example has been tested with NUCLEO-L4P5ZG (144 pins) board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
 
