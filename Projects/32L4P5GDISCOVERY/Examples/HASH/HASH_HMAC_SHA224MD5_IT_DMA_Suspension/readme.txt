/**
  @page HASH_HMAC_SHA224MD5_IT_DMA_Suspension HMAC digest calculation suspension example
  
  @verbatim
  ******************************************************************************
  * @file    HASH/HASH_HMAC_SHA224MD5_IT_DMA_Suspension/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the HMAC digest calculation suspension using MD5 and SHA-224 example.
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

How to suspend the HMAC digest computation when data are fed either under 
interruption or by DMA.

The example first computes the HMAC MD5 digest of a low priority block in entering 
input data in interrupt mode.

The processing is suspended in order to process a higher priority block to
compute its SHA-256 digest. For the higher priority block, data are entered in
interrupt mode as well.
Suspension request is triggered by an interrupts counter which is incremented each time
a 512-bit long block has been fed to the IP. Once the counter has reached an arbitrary
value, the low priority block processing is suspended.

It is shown how to save then how to restore later on the low priority block
context to resume its processing.
For both the low and high priority blocks, the computed digests are compared with
the expected ones.


Next, the example describes a similar situation where a multi-buffer HMAC SHA-224 
low priority block processing is suspended to handle the HMAC SHA1 digest computation 
of a higher priority block. 
Once the high priority block processing is over, the low priority block
context is restored and computation is resumed.
For illustration purposes,
- the processing of each buffer of the low priority block is suspended
- suspension request is arbitrarily initiated by an interrupt-triggered event, chosen 
to be that of the UART IP for the example at hand.

For both low and high priority blocks, data are fed to the IP by DMA and for both
of them, the computed digests are compared with the expected ones.



LED1 is turned on when correct digest values are calculated.
In case of digest computation or initialization issue, LED2 is toggled. 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - HASH/HASH_HMAC_SHA224MD5_IT_DMA_Suspension/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - HASH/HASH_HMAC_SHA224MD5_IT_DMA_Suspension/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - HASH/HASH_HMAC_SHA224MD5_IT_DMA_Suspension/Inc/main.h						       Header for main.c module  
  - HASH/HASH_HMAC_SHA224MD5_IT_DMA_Suspension/Src/stm32l4xx_it.c          Interrupt handlers
  - HASH/HASH_HMAC_SHA224MD5_IT_DMA_Suspension/Src/main.c						       Main program
  - HASH/HASH_HMAC_SHA224MD5_IT_DMA_Suspension/Src/stm32l4xx_hal_msp.c     HAL MSP module
  - HASH/HASH_HMAC_SHA224MD5_IT_DMA_Suspension/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L4P5xx devices.
  
  - This example has been tested with 32L4P5GDISCOVERY board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 


 */
 