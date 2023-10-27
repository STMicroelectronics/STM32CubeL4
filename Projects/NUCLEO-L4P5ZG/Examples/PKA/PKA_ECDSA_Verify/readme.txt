/**
  @page PKA_ECDSA_Verify ECDSA verification example
  
  @verbatim
  ******************************************************************************
  * @file    PKA/PKA_ECDSA_Verify/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ECDSA verification example
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

How to determine if a given signature is valid regarding the Elliptic curve digital signature algorithm
(ECDSA).

For this example, a test vector have been extracted from National Institute of Standards and Technology (NIST)
 - Cryptographic Algorithm Validation Program (CAVP) in order to demonstrate the usage of the hal.
This reference files can be found under:
"http://csrc.nist.gov/groups/STM/cavp/documents/dss/186-3ecdsatestvectors.zip (ZIP SigGen.rsp)"

This test vector has been chosen to demonstrate the behavior in a case where the input signature
is valid. A second input is provided where one element of the hash message has been modified to 
demonstrate the behavior in a case where the signature is invalid. Their definitions are included 
in SigVer.c. You can refer to this file for more information.

The selected curve for this example is P-256 (ECDSA-256) published by NIST in 
Federal Information Processing Standards Publication FIPS PUB 186-4. The description
of this curve is present in file Src/prime256v1.c.

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

PKA, PKA_ECDSA_Verify, Security, NIST, CAVP, ECDSA verification

@par Directory contents 
  
  - PKA/PKA_ECDSA_Verify/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - PKA/PKA_ECDSA_Verify/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - PKA/PKA_ECDSA_Verify/Inc/main.h                  Header for main.c module
  - PKA/PKA_ECDSA_Verify/Src/stm32l4xx_it.c          Interrupt handlers
  - PKA/PKA_ECDSA_Verify/Src/main.c                  Main program
  - PKA/PKA_ECDSA_Verify/Src/stm32l4xx_hal_msp.c     HAL MSP module 
  - PKA/PKA_ECDSA_Verify/Src/system_stm32l4xx.c      STM32L4xx system source file
  - PKA/PKA_ECDSA_Verify/Src/prime256v1.c            Description of P-256 (ECDSA-256)
  - PKA/PKA_ECDSA_Verify/Inc/prime256v1.h            Header for prime256v1.c
  - PKA/PKA_ECDSA_Verify/Src/SigVer.c                Reflect the content of the test vector from SigVer.rsp
  - PKA/PKA_ECDSA_Verify/Src/SigVer.rsp              Extract from NIST CAVP
  - PKA/PKA_ECDSA_Verify/Inc/SigVer.h                Header of SigVer.c

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
 