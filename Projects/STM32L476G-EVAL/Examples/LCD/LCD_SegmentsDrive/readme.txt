/**
  @page LCD_SegmentsDrive LCD Segments Drive example
  
  @verbatim
  ******************************************************************************
  * @file    LCD/LCD_SegmentsDrive/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LCD Segments Drive example.
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

How to use the embedded LCD glass controller to drive the on-board 
LCD glass by Pacific Display Devices.

The goal of this example is to display a "STM32L4xx" string on LCD glass in scrolling
mode. The scrolling number is fixed to 100 repetition and the speed is fixed to 600ms.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - LCD/LCD_SegmentsDrive/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - LCD/LCD_SegmentsDrive/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - LCD/LCD_SegmentsDrive/Inc/main.h                  Header for main.c module  
  - LCD/LCD_SegmentsDrive/Src/stm32l4xx_it.c          Interrupt handlers
  - LCD/LCD_SegmentsDrive/Src/main.c                  Main program
  - LCD/LCD_SegmentsDrive/Src/stm32l4xx_hal_msp.c     HAL MSP file
  - LCD/LCD_SegmentsDrive/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L476xx devices.
    
  - This example has been tested with STMicroelectronics STM32L476G-EVAL
    board and can be easily tailored to any other supported device
    and development board.      

  - Warning: The LCD glass has to be moved up in LCD position from default IO position.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
