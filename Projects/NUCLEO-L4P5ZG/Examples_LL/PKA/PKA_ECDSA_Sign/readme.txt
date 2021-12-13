/**
  @page PKA_ECDSA_Sign PKA : Public Key Accelerator

  @verbatim
  ******************************************************************************
  * @file    Examples_LL/PKA/PKA_ECDSA_Sign/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the PKA_ECDSA_Sign example.
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

How to use the low-layer PKA API to generate an ECDSA signature.

Example execution:
After startup from reset and system configuration, PKA configuration is performed.

To demonstrate the generation of an ECDSA signature, input and output buffers of this
example are extracted from National Institute of Standards and Technology (NIST)
Cryptographic Algorithm Validation Program (CAVP) (http://csrc.nist.gov/groups/STM/cavp/).

One test vector has been extracted from section [P-256,SHA-256] of
http://csrc.nist.gov/groups/STM/cavp/documents/dss/186-3ecdsatestvectors.zip
and adapted to C buffers in SigGen.c and SigGen.h.

The example:
  Loads the input buffers to PKA RAM.
  Launch the computation in interrupt mode.
  Wait for the interrupt callback.
  Retrieve the output buffer.
  Compare to expected results.

After successful sequence, LED1 is turned On.
In case of errors, LED1 is slowly blinking (1sec period).

@par Keywords

PKA, interrupt, ECDSA, signature, NIST, CAVP

@par Directory contents

  - PKA/PKA_ECDSA_Sign/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - PKA/PKA_ECDSA_Sign/Inc/main.h                  Header for main.c module
  - PKA/PKA_ECDSA_Sign/Inc/stm32_assert.h          Template file to include assert_failed function
  - PKA/PKA_ECDSA_Sign/Src/stm32l4xx_it.c          Interrupt handlers
  - PKA/PKA_ECDSA_Sign/Src/main.c                  Main program
  - PKA/PKA_ECDSA_Sign/Src/system_stm32l4xx.c      STM32L4xx system source file

@par Hardware and Software environment

  - This example runs on STM32L4Q5xx devices.

  - This example has been tested with an STMicroelectronics NUCLEO-L4P5ZG (144 pins)
    board mounted with an STM32L4Q5ZG chip and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example and observe the LED status

 */
