/**
  @page PKA_ECCscalarMultiplication_IT Elliptic Curve Cryptography scalar multiplication example
  
  @verbatim
  ******************************************************************************
  * @file    PKA/PKA_ECCscalarMultiplication_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ECC scalar multiplication example
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

How to use the PKA peripheral to execute ECC scalar multiplication. This 
allows generating a public key from a private key in interrupt mode.

For this example, a .pem file has been created using openssl external tool. (https://www.openssl.org)
This file is located under Src/ecdsa_priv_prime256v1.pem.
To ease the usage of this .pem files, 2 files, Src/ecdsa_priv_prime256v1.c and Inc/ecdsa_priv_prime256v1.h 
are present to reflect its content. The explanation of how to create those file from a .pem is embedded in 
Src/ecdsa_priv_prime256v1.c.

The selected curve for this example is P-256 (ECDSA-256) published by NIST in 
Federal Information Processing Standards Publication FIPS PUB 186-4. The description
of this curve is present in file Src/prime256v1.c. 

Those two information are provided to PKA using the PKA_ECCMulInTypeDef "in" variable.

The result of the operation is the public key part of the .pem file. So the example compare the computed
result to the content of the .pem file to ensure the validity of the operation.

In this example, the PKA interrupt is triggered at the end of the operation. The interrupt handler then
call the pka callback where a global variable is used to notify the main function.

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

PKA, PKA_ECCscalarMultiplication_IT, Security, FIPS PUB 186-4, ECC scalar multiplication

@par Directory contents 
  
  - PKA/PKA_ECCscalarMultiplication_IT/Inc/stm32l4xx_hal_conf.h      HAL configuration file
  - PKA/PKA_ECCscalarMultiplication_IT/Inc/stm32l4xx_it.h            Interrupt handlers header file
  - PKA/PKA_ECCscalarMultiplication_IT/Inc/main.h                    Header for main.c module
  - PKA/PKA_ECCscalarMultiplication_IT/Src/stm32l4xx_it.c            Interrupt handlers
  - PKA/PKA_ECCscalarMultiplication_IT/Src/main.c                    Main program
  - PKA/PKA_ECCscalarMultiplication_IT/Src/stm32l4xx_hal_msp.c       HAL MSP module 
  - PKA/PKA_ECCscalarMultiplication_IT/Src/system_stm32l4xx.c        STM32L4xx system source file
  - PKA/PKA_ECCscalarMultiplication_IT/Src/prime256v1.c              Description of P-256 (ECDSA-256)
  - PKA/PKA_ECCscalarMultiplication_IT/Inc/prime256v1.h              Header for prime256v1.c
  - PKA/PKA_ECCscalarMultiplication_IT/Src/ecdsa_priv_prime256v1.pem ecdsa private key generated using openssl
  - PKA/PKA_ECCscalarMultiplication_IT/Src/ecdsa_priv_prime256v1.c   Reflect the content of ecdsa_priv_prime256v1.pem in array format
  - PKA/PKA_ECCscalarMultiplication_IT/Inc/ecdsa_priv_prime256v1.h   Header for ecdsa_priv_prime256v1.c

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
 