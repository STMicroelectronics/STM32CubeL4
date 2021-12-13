/**
  @page PKA_ModularExponentiation Modular exponentiation example
  
  @verbatim
  ******************************************************************************
  * @file    PKA/PKA_ModularExponentiation/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the modular exponentiation example
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

How to use the PKA peripheral to execute modular exponentiation. This 
allows ciphering/deciphering a text.

For this example, two .pem file have been created using openssl external tool. (https://www.openssl.org).
Src/rsa_pub_2048.pem and Src/rsa_priv_2048.pem, contains respectively the public and private key used in
this example.
To ease the usage of this .pem files, 2 set of files, Src/rsa_pub_2048.c and Inc/rsa_pub_2048.h and
Src/rsa_priv_2048.c and Inc/rsa_priv_2048.h are present to reflect their content. The explanation of 
how to create those file from a .pem are embedded inside both .c files.

The selected algorithm is RSA.

Based on the public key, the plain text message is ciphered using the modular exponentiation 
and stored in cipheredBuffer.

Then this ciphered buffer is deciphered using the modular exponentiation again but based on the private key and
stored in the decipheredBuffer. After this, the buffer is identical to the initial plain text.

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

PKA, PKA_ModularExponentiation, Security, RSA, modular exponentiation, cipher ,decipher

@par Directory contents 
  
  - PKA/PKA_ModularExponentiation/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - PKA/PKA_ModularExponentiation/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - PKA/PKA_ModularExponentiation/Inc/main.h                  Header for main.c module
  - PKA/PKA_ModularExponentiation/Src/stm32l4xx_it.c          Interrupt handlers
  - PKA/PKA_ModularExponentiation/Src/main.c                  Main program
  - PKA/PKA_ModularExponentiation/Src/stm32l4xx_hal_msp.c     HAL MSP module 
  - PKA/PKA_ModularExponentiation/Src/system_stm32l4xx.c      STM32L4xx system source file
  - PKA/PKA_ModularExponentiation/Src/plaintext.bin           Random 2048 bits
  - PKA/PKA_ModularExponentiation/Src/plaintext.c             Reflect the content of plaintext.bin in array format
  - PKA/PKA_ModularExponentiation/Inc/plaintext.h             Header for plaintext.c
  - PKA/PKA_ModularExponentiation/Src/ciphertext.bin          Ciphered plaintext.bin using openssl
  - PKA/PKA_ModularExponentiation/Src/ciphertext.c            Reflect the content of ciphertext.bin in array format
  - PKA/PKA_ModularExponentiation/Inc/ciphertext.h            Header for ciphertext.c
  - PKA/PKA_ModularExponentiation/Src/rsa_priv_2048.pem       RSA private key generated using openssl
  - PKA/PKA_ModularExponentiation/Src/rsa_priv_2048.c         Reflect the content of rsa_priv_2048.pem in array format
  - PKA/PKA_ModularExponentiation/Inc/rsa_priv_2048.h         Header for rsa_priv_2048.c
  - PKA/PKA_ModularExponentiation/Src/rsa_pub_2048.pem        RSA public key extracted from rsa_priv_2048.pem using openssl
  - PKA/PKA_ModularExponentiation/Src/rsa_pub_2048.c          Reflect the content of rsa_pub_2048.pem in array format
  - PKA/PKA_ModularExponentiation/Inc/rsa_pub_2048.h          Header for rsa_pub_2048.c

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
 