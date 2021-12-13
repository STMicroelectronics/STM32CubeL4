/**
  @page PKA_ModularExponentiationCRT Modular exponentiation (CRT) example
  
  @verbatim
  ******************************************************************************
  * @file    PKA/PKA_ModularExponentiationCRT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the modular exponentiation (CRT) example
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

How to compute the Chinese Remainder Theorem (CRT) optimization.

For this example, two .pem file have been created using openssl external tool. (https://www.openssl.org).
Src/rsa_pub_2048.pem and Src/rsa_priv_2048.pem, contains respectively the public and private key used in
this example.
To ease the usage of this .pem files, 2 set of files, Src/rsa_pub_2048.c and Inc/rsa_pub_2048.h and
Src/rsa_priv_2048.c and Inc/rsa_priv_2048.h are present to reflect their content. The explanation of 
how to create those file from a .pem are embedded inside both .c files.

The selected algorithm is RSA.

A ciphered buffer is deciphered using the CRT modular exponentiation and the private key.
The result is stored in the decipheredBuffer. After this, the buffer is identical to the plain text.

In case of success, the LED1 (GREEN) is ON.
In case of any error, the LED1 (GREEN) is toggling slowly.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

PKA, PKA_ModularExponentiationCRT, Security, RSA, modular exponentiation, CRT ,decipher

@par Directory contents 
  
  - PKA/PKA_ModularExponentiationCRT/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - PKA/PKA_ModularExponentiationCRT/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - PKA/PKA_ModularExponentiationCRT/Inc/main.h                  Header for main.c module
  - PKA/PKA_ModularExponentiationCRT/Src/stm32l4xx_it.c          Interrupt handlers
  - PKA/PKA_ModularExponentiationCRT/Src/main.c                  Main program
  - PKA/PKA_ModularExponentiationCRT/Src/stm32l4xx_hal_msp.c     HAL MSP module 
  - PKA/PKA_ModularExponentiationCRT/Src/system_stm32l4xx.c      STM32L4xx system source file
  - PKA/PKA_ModularExponentiationCRT/Src/plaintext.bin           Random 2048 bits
  - PKA/PKA_ModularExponentiationCRT/Src/plaintext.c             Reflect the content of plaintext.bin in array format
  - PKA/PKA_ModularExponentiationCRT/Inc/plaintext.h             Header for plaintext.c
  - PKA/PKA_ModularExponentiationCRT/Src/ciphertext.bin          Ciphered plaintext.bin using openssl
  - PKA/PKA_ModularExponentiationCRT/Src/ciphertext.c            Reflect the content of ciphertext.bin in array format
  - PKA/PKA_ModularExponentiationCRT/Inc/ciphertext.h            Header for ciphertext.c
  - PKA/PKA_ModularExponentiationCRT/Src/rsa_priv_2048.pem       RSA private key generated using openssl
  - PKA/PKA_ModularExponentiationCRT/Src/rsa_priv_2048.c         Reflect the content of rsa_priv_2048.pem in array format
  - PKA/PKA_ModularExponentiationCRT/Inc/rsa_priv_2048.h         Header for rsa_priv_2048.c
  - PKA/PKA_ModularExponentiationCRT/Src/rsa_pub_2048.pem        RSA public key extracted from rsa_priv_2048.pem using openssl
  - PKA/PKA_ModularExponentiationCRT/Src/rsa_pub_2048.c          Reflect the content of rsa_pub_2048.pem in array format
  - PKA/PKA_ModularExponentiationCRT/Inc/rsa_pub_2048.h          Header for rsa_pub_2048.c

@par Hardware and Software environment

  - This example runs on STM32L4Q5xx devices.
  
  - This example has been tested with an STMicroelectronics NUCLEO-L4P5ZG (144 pins)
    board mounted with an STM32L4Q5ZG chip and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 */
 