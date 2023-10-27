/**
  @page PKA_ModularExponentiation PKA : Public Key Accelerator

  @verbatim
  ******************************************************************************
  * @file    Examples_LL/PKA/PKA_ModularExponentiation/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the PKA_ModularExponentiation example.
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

How to use the low-layer PKA API to execute RSA modular exponentiation.

Example execution:
After startup from reset and system configuration, PKA configuration is performed.

To demonstrate the RSA modular exponentiation, four file have been created using an
external tool, openssl:
  Src/rsa_priv_2048.pem (Refer to rsa_priv_2048.c for its creation)
  Src/rsa_pub_2048.pem  (extracted from rsa_priv_2048.pem. Refer to rsa_pub_2048.c
                        for its creation)
  Src/plaintext.bin     (Refer to plaintext.c for its creation)
  Src/cipher.bin        (build from rsa_pub_2048.pem and plaintext.bin. Refer to
                        plaintext.c for its creation)

The example:
  Compute ciphertext.bin from plaintext.bin and rsa_pub_2048.pem (This is done
    with the public key)
  Compute plaintext.bin from ciphertext.bin and rsa_priv_2048.pem

After successful sequence, LED1 is turned On.
In case of errors, LED1 is slowly blinking (1sec period).

@par Keywords

PKA, interrupt, Modular Exponentiation, cipher, RSA, pem

@par Directory contents

  - PKA/PKA_ModularExponentiation/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - PKA/PKA_ModularExponentiation/Inc/main.h                  Header for main.c module
  - PKA/PKA_ModularExponentiation/Inc/stm32_assert.h          Template file to include assert_failed function
  - PKA/PKA_ModularExponentiation/Src/stm32l4xx_it.c          Interrupt handlers
  - PKA/PKA_ModularExponentiation/Src/main.c                  Main program
  - PKA/PKA_ModularExponentiation/Src/system_stm32l4xx.c      STM32L4xx system source file

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
