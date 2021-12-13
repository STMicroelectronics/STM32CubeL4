/**
  @page HelloWorld Readme file
 
  @verbatim
  ******************************************************************************
  * @file    STemWin/MB1315/HelloWorld/readme.txt 
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

The application allows also to run the different Segger samples that can be
downloaded from here:
http://www.segger.com/emwin-samples.html
To do this, user has only to replace the file "Basic_HelloWorld.c" into the
project workspace by the downloaded one.

Note that the following user files may need to be updated:
  LCDConf.c
  GUIConf.c
(if for example more GUI allocated memory is needed)

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note If the application is not running normally as mentionned above , you can accordingly modify either
      the Heap and Stack of the application or the GUI_NUMBYTES define in the GUIConf.c file

@par Directory contents 

  - STemWin/MB1315/HelloWorld/STemWin/Target/GUIConf.h                  Header for GUIConf.c
  - STemWin/MB1315/HelloWorld/STemWin/Target/LCDConf.h                  Header for LCDConf.c
  - STemWin/MB1315/HelloWorld/Core/Inc/main.h                           Main program header file
  - STemWin/MB1315/HelloWorld/Core/Inc/stm32l4xx_hal_conf.h             Library Configuration file
  - STemWin/MB1315/HelloWorld/Core/Inc/stm32l4xx_it.h                   Interrupt handlers header file
  - STemWin/MB1315/HelloWorld/STemWin/App/BASIC_HelloWorld.c            Simple demo drawing "Hello world"
  - STemWin/MB1315/HelloWorld/STemWin/Target/GUIConf.c                  Display controller initialization
  - STemWin/MB1315/HelloWorld/STemWin/Target/LCDConf.c                  Configuration file for the GUI library
  - STemWin/MB1315/HelloWorld/Core/Src/main.c                           Main program file
  - STemWin/MB1315/HelloWorld/Core/Src/stm32l4xx_it.c                   STM32L4xx Interrupt handlers
  - STemWin/MB1315/HelloWorld/Core/Src/stm32l4xx_hal_msp.c              HAL MSP module
  - STemWin/MB1315/HelloWorld/Core/Src/system_stm32l4xx.c               STM32L4xx system file
 
 
@par Hardware and Software environment  

  - This application runs on STM32L4R9I devices.
    
  - This application has been tested with STMicroelectronics STM32L4R9I-EVAL RevB
    (STM32L4R9I Devices) evaluation boards and can be easily 
    tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
  
 */
