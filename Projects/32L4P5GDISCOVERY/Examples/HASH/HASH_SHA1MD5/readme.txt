/**
  @page HASH_SHA1MD5  HASH digest calculation using SHA1 and MD5 example
  
  @verbatim
  ******************************************************************************
  * @file    HASH/HASH_SHA1MD5/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the digest calculation using SHA1 and MD5 example.
  ******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description

This example shows how to use the HASH peripheral to hash data with SHA-1 and MD5 algorithms.

For this example, DMA is not used for data transfer, the CPU is using HASH peripheral in
polling mode.

The SHA-1 message digest result is a 160-bit long data and the MD5 message digest result
is a 128-bit long data.

The expected HASH digests (for SHA1 and MD5) are already computed using an online
HASH tool. Those values are compared to those computed by the HASH peripheral.

In case there is a mismatch the blue LED (LED2) is turned ON.
In case the SHA1 digest is computed correctly the green LED (LED1) is turned ON.
In case the MD5 digest is computed correctly the green LED (LED1) is turned ON.



@par Directory contents 

  - HASH/HASH_SHA1MD5/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - HASH/HASH_SHA1MD5/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - HASH/HASH_SHA1MD5/Inc/main.h                  Header for main.c module  
  - HASH/HASH_SHA1MD5/Src/stm32l4xx_it.c          Interrupt handlers
  - HASH/HASH_SHA1MD5/Src/main.c                  Main program
  - HASH/HASH_SHA1MD5/Src/stm32l4xx_hal_msp.c     HAL MSP module
  - HASH/HASH_SHA1MD5/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

    - This example runs on STM32L4P5xx devices.
  
    - This example has been tested with a STM32L4P5xx embedded on an 
    32L4P5GDISCOVERY board and can be easily tailored to any other supported 
    device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 