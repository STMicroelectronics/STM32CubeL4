/**
  @page STemWin_HelloWorld Readme file
 
  @verbatim
  ******************************************************************************
  * @file    STemWin_HelloWorld/readme.txt 
  * @author  MCD Application Team
  * @brief
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


@par Application Description

Simple "Hello World" example based on STemWin.

The example allows also to run the different Segger samples that can be
downloaded from here:
http://www.segger.com/emwin-samples.html
To do this, user has only to replace the file "Basic_HelloWorld.c" into the
project workspace by the downloaded one.

Note that the following user files may need to be updated:
  LCDConf_stm32l476g_eval.c
  GUIConf_stm32l476g_eval.c
(if for example more GUI allocated memory is needed)

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - STemWin/STemWin_HelloWorld/Inc/GUIConf.h                Header for GUIConf_stm32l476g_eval.c
  - STemWin/STemWin_HelloWorld/Inc/LCDConf_stm32l476g_eval.h  Header for LCDConf_stm32l476g_eval.c
  - STemWin/STemWin_HelloWorld/Inc/stm32l4xx_hal_conf.h     HAL configuration file
  - STemWin/STemWin_HelloWorld/Inc/stm32l4xx_it.h           Interrupt handlers header file
  - STemWin/STemWin_HelloWorld/Inc/main.h                   Header for main.c module  
  - STemWin/STemWin_HelloWorld/Src/stm32l4xx_it.c           Interrupt handlers
  - STemWin/STemWin_HelloWorld/Src/BASIC_HelloWorld.c       Simple demo drawing "Hello world"
  - STemWin/STemWin_HelloWorld/Src/GUIConf_stm32l476g_eval.c  Display controller initialization
  - STemWin/STemWin_HelloWorld/Src/LCDConf_stm32l476g_eval.c  Configuration file for the GUI library
  - STemWin/STemWin_HelloWorld/Src/main.c                   Main program
  - STemWin/STemWin_HelloWorld/Src/system_stm32l4xx.c       STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L476xx devices.
    
  - This example has been tested with STM32L476G-EVAL board and can be
    easily tailored to any other supported device and development board.  
    
    @note Make sure that LCD Glass is in IO Position

@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
 
 */
