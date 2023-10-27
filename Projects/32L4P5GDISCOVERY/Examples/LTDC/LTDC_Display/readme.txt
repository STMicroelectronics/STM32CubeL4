/**
  @page LTDC Display 
  
  @verbatim
  ******************************************************************************
  * @file    LTDC/LTDC Display/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LTDC display example
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

How to configure and use LTDC peripheral to display an image.

The example describes how to configure the LTDC peripheral in order to display an image.

The LTDC is specially configured for the 6-bit pixel round LCD supported by the MB1609 board
to be plugged on top of the Discovery board MB1535. 

After the initialization phase, two images are alternately displayed on the screen every three seconds.

User may uncomment two defines at lines 49 and 50 of main.h file to enable the CLUT-based display 
as well as the dimming feature resorting to the low-power timer.
Note that in this example, dimming feature is available only in CLUT mode. 

The CLUT option allows to use a 6-bit pixel image instead of 24-bit pixel.
Since the MB1609 LCD display resolution is 6-bit pixel, there is no quality loss but 
gain in RAM requirements.


@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents  

  - LTDC/LTDC Display/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - LTDC/LTDC Display/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - LTDC/LTDC Display/Inc/main.h                  Header for main.c module  
  - LTDC/LTDC Display/Inc/Lena.h                  Lena reference image file 
  - LTDC/LTDC Display/Inc/mire.h                  LCD screen pattern file
  - LTDC/LTDC Display/Src/stm32l4xx_it.c          Interrupt handlers
  - LTDC/LTDC Display/Src/main.c                  Main program
  - LTDC/LTDC Display/Src/stm32l4xx_hal_msp.c     HAL MSP module
  - LTDC/LTDC Display/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L4P5xx devices.
    
  - This example has been tested with STMicroelectronics 32L4P5GDISCOVERY
    board MB1535 with the MB1309 daughter board supporting the LCD.   
	
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred tool chain 
 - Rebuild all files and load your image into target memory
 - Run the example 

 */
