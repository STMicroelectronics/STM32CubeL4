/**
  @page DMA2D_MemToMemWithBlending DMA2D Memory to Memory with blending example
  
  @verbatim
  ******************************************************************************
  * @file    DMA2D/DMA2D_MemToMemWithBlending/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the STM32L4xx DMA2D Memory to Memory with blending example.
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

  How to configure the DMA2D peripheral in Memory-to-memory with blending
  transfer mode.

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  Then the SystemClock_Config() function is used to configure the system
  clock (SYSCLK) to run at 80 MHz.
 
  In this transfer mode two input sources are fetched : foreground and background.
  In this example, the foreground and background are configured as follows :
   - Foreground object 
		- input memory address at Flash memory (static image array coded in FLASH).
		- Format : RGB565
		- Size   : 240x130
   - Background object 
		- input memory address at Flash memory (static image array coded in FLASH).
		- Format : RGB565
		- Size   : 240x130
   - The alpha mode for foreground and background is configured to see two 
     superposed images in the resulting image out of the ChromArt DMA2D operation.
        
  The DMA2D blends the two sources pixels from FLASH to internal FIFOs to compute 
  the resulting pixels in dedicated output FIFO. 
  The transferred data to the output memory address is the result of the blending 
  operation between background and foreground.
 
  The blending result of the foreground and background is shown on the LCD panel
  through a DMA2D-based transfer of the resulting image to the LCD. 
  
  This process is repeated in an infinite loop where the foreground and background
  transparency values are respectively increased then decreased so that each image 
  is alternatively and smoothly replaced by the other in a blending way.
  
  In this example, the background object represents the ST logo and the foreground
  object represents the "life.augmented" slogan.

STM32L496G-Discovery board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when there is an initialization or a DMA2D transfer error.
 - LED2 is ON when the DMA2D transfer is complete.

         
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

    - DMA2D/DMA2D_MemToMemWithBlending/Inc/main.h                      Main configuration file
    - DMA2D/DMA2D_MemToMemWithBlending/Inc/stm32l4xx_it.h              Interrupt handlers header file
    - DMA2D/DMA2D_MemToMemWithBlending/Inc/stm32l4xx_hal_conf.h        HAL configuration file 
    - DMA2D/DMA2D_MemToMemWithBlending/Inc/RGB565_230x170_1.h          Image used for DMAD2D validation
    - DMA2D/DMA2D_MemToMemWithBlending/Inc/RGB565_230x170_2.h          Image used for DMAD2D validation
    - DMA2D/DMA2D_MemToMemWithBlending/Src/main.c                      Main program  
    - DMA2D/DMA2D_MemToMemWithBlending/Src/stm32l4xx_it.c              Interrupt handlers
    - DMA2D/DMA2D_MemToMemWithBlending/Src/stm32l4xx_hal_msp.c         HAL MSP module
    - DMA2D/DMA2D_MemToMemWithBlending/Src/system_stm32l4xx.c          STM32L4xx system clock configuration file

@par Hardware and Software environment  

  - This example runs on STM32L496xx devices.
  
  - This example has been tested with STM32L496G-Discovery boards and can be easily 
    tailored to any other supported device and development board. 

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 
                          
 */
                                   