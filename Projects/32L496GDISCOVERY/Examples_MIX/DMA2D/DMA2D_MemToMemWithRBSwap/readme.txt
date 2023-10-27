/**
  @page DMA2D_MemToMemWithRBSwap DMA2D : DMA2D Memory to Memory with R&B swap example (HAL/LL mixed usage example)
  
  @verbatim
  ******************************************************************************
  * @file    Examples_MIX/DMA2D/DMA2D_MemToMemWithRBSwap/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA2D_MemToMemWithRBSwap example.
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

How to configure DMA2D peripheral in Memory-to-memory transfer mode with pixel
format conversion and image blending, then display the result on LCD. The DMA2D
LL APIs are used for performance improvement.

  The source buffer in format ARGB8888 and size 240x160 located in Flash is copied 
  in Memory to Memory mode with Pixel Format Conversion to create three
  different images:
  - the first one is an artificially distorted image (converted in 1RGB555 but read as RGB565)
  - the second one is converted to RGB565 but Red and Blue are swapped for illustration purposes
  - the third one is converted to RGB565 with the correct color setting
 
  These three images are alternatively shown on the LCD, blended two by two so that each
  image smoothly gives way to the next one.
  The last image finally fades away to let a white screen only.
  Then the same process is repeated in an infinite loop. 
  
 
  In this basic example, the goal is to show how to set some of the DMA2D IP
  parameters through DMAD LL APIs:
  - input and output image format, alpha value
  - origin and destination buffers addresses
  - foreground and background addresses
  - transfer size
  - pixel format conversion, blending
  - DMA2D interruptions enabling
  - DMA2D transfer start 
  
  STM32L496G-Discovery board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when there is an initialization or a DMA2D transfer error.
 - LED2 is ON when the DMA2D transfer is complete.
 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 
  
  - DMA2D/DMA2D_MemToMemWithRBSwap/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - DMA2D/DMA2D_MemToMemWithRBSwap/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - DMA2D/DMA2D_MemToMemWithRBSwap/Inc/main.h                  Header for main.c module
  - DMA2D/DMA2D_MemToMemWithRBSwap/Src/stm32l4xx_it.c          Interrupt handlers
  - DMA2D/DMA2D_MemToMemWithRBSwap/Src/main.c                  Main program
  - DMA2D/DMA2D_MemToMemWithRBSwap/Src/stm32l4xx_hal_msp.c     HAL MSP module 
  - DMA2D/DMA2D_MemToMemWithRBSwap/Src/system_stm32l4xx.c      STM32L4xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32L496xx devices.
  
  - This example has been tested with STMicroelectronics STM32L496G-Discovery 
    board and can be easily tailored to any other supported device 
    and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
 