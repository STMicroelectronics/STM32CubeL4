/**
  @page SampleDemo Readme file
 
  @verbatim
  ******************************************************************************
  * @file    STemWin/MB1315/SampleDemo/readme.txt 
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

How to implement a sample demonstration example allowing 
to show some of the STemWin Library capabilities.

The list of modules to be used is configured into the file Demo/GUIDEMO.h

At the first run, after power OFF/ON, a touch screen calibration is needed: in
order to this, user is just requested to follow the displayed instructions.
Then all the demo modules are run sequentially. Throughout the demonstration, a
control frame window is displayed at the bottom right of the touch screen: it
includes a progress bar and two control buttons (Next + Halt) allowing to skip
or pause the currently running module.

LEDs 1 to 4 are continuously toggling to indicate that the Demo runs properly.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - STemWin/MB1315/HelloWorld/STemWin/Target/GUIConf.h                  Header for GUIConf.c
  - STemWin/MB1315/HelloWorld/STemWin/Target/LCDConf.h                  Header for LCDConf.c
  - STemWin/MB1315/HelloWorld/Core/Inc/main.h                           Main program header file
  - STemWin/MB1315/HelloWorld/Core/Inc/stm32l4xx_hal_conf.h             Library Configuration file
  - STemWin/MB1315/HelloWorld/Core/Inc/stm32l4xx_it.h                   Interrupt handlers header file
  - STemWin/MB1315/HelloWorld/STemWin/Target/GUIConf.c                  Display controller initialization
  - STemWin/MB1315/HelloWorld/STemWin/Target/LCDConf.c                  Configuration file for the GUI library
  - STemWin/MB1315/HelloWorld/Core/Src/main.c                           Main program file
  - STemWin/MB1315/HelloWorld/Core/Src/stm32l4xx_it.c                   STM32L4xx Interrupt handlers
  - STemWin/MB1315/HelloWorld/Core/Src/system_stm32l4xx.c               STM32L4xx system file
  - STemWin/MB1315/HelloWorld/STemWin/App/GUIDEMO_*.c                   All the sources files provided for SEGGER DEMO
  - STemWin/MB1315/HelloWorld/STemWin/App/GUIDEMO.h                     DEMO modules configuration file
 
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
