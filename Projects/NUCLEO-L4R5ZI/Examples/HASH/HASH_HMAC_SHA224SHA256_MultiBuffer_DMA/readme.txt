/**
  @page HASH_HMAC_SHA224SHA256_MultiBuffer_DMA  HASH and HMAC multi-buffer digest calculation using SHA224 and SHA256 example with DMA transfer
  
  @verbatim
  ******************************************************************************
  * @file    HASH/HASH_HMAC_SHA224SHA256_MultiBuffer_DMA/readme.txt
  * @author  MCD Application Team
  * @brief   Description of HASH and HMAC multi-buffer digest calculation using SHA224 and SHA256 example with DMA transfer.
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

How to handle text messages larger than the maximum DMA transfer length. The input 
data are split into several buffers with sizes within the DMA limit, then fed 
successively to the HASH peripheral.

It is shown how to use the HASH peripheral to hash multi buffer data using SHA-224, 
HMAC SHA-224 and HMAC SHA-256 algorithms with DMA transfer.

The same text message is split in several buffers and is processed by the following 
algorithms and multi buffers configurations:
- SHA-224, the message is split in 5 buffers that are successively fed to the IP.
- HMAC SHA-224, the message is split in 5 buffers that are successively fed to the IP.
- HMAC SHA-256, the message is split in 2 buffers that are successively fed to the IP.

The key used is long type (2088 bit = 261 bytes).
The HASH and HMAC SHA-224 message digest result is a 224-bit (28-byte) data and the 
HMAC SHA-256 message digest result is a 256-bit (32-byte) data.

When HASH SHA-224 and HMAC SHA-224 (respectively HMAC SHA-256) digest computations are successful, 
LED1 (resp. LED2) is turned on.
In case of digest computation or initialization issue, LED3 is turned on. 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - HASH/HASH_HMAC_SHA224SHA256_MultiBuffer_DMA/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - HASH/HASH_HMAC_SHA224SHA256_MultiBuffer_DMA/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - HASH/HASH_HMAC_SHA224SHA256_MultiBuffer_DMA/Inc/main.h                  Header for main.c module  
  - HASH/HASH_HMAC_SHA224SHA256_MultiBuffer_DMA/Src/stm32l4xx_it.c          Interrupt handlers
  - HASH/HASH_HMAC_SHA224SHA256_MultiBuffer_DMA/Src/main.c                  Main program
  - HASH/HASH_HMAC_SHA224SHA256_MultiBuffer_DMA/Src/stm32l4xx_hal_msp.c     HAL MSP module
  - HASH/HASH_HMAC_SHA224SHA256_MultiBuffer_DMA/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L4S5xx devices.
  
  - This example has been tested with a STM32L4S5xx mounted on a 
    Nucleo (144 pins) board and can be easily tailored to any other supported 
    device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 


 */
 