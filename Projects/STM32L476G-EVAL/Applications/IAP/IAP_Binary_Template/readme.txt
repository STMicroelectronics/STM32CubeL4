/**
  @page IAP_Binary_Template AN4657 IAP Binary Template Readme file
  
  @verbatim
  ******************************************************************************
  * @file    IAP/IAP_Binary_Template/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the IAP_Binary_Template directory.
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

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP) through USART.

To build such application, some special configuration has to be performed:
1. Set the program load address at 0x08008000, using your toolchain linker file
2. Relocate the vector table at address 0x08008000, using the "NVIC_SetVectorTable"
   function.

The SysTick example provided within the STM32L4xx HAL Cube examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - IAP/IAP_Binary_Template/EWARM: This folder contains a IAR pre-configured project 
                                  file that produces a binary image of SysTick 
                                  example that could be loaded using IAP.
                                          
 - IAP/IAP_Binary_Template/MDK-ARM: This folder contains a Keil pre-configured project 
                                    file that produces a binary image of SysTick 
                                    example that could be loaded using IAP.

 - IAP/IAP_Binary_Template/SW4STM32: This folder contains a SW4STM32 pre-configured project 
                                     file that produces a binary image of SysTick 
                                     example that could be loaded using IAP.

 - IAP/IAP_Binary_Template/TrueStudio: This folder contains a TrueStudio pre-configured project 
                                       file that produces a binary image of SysTick 
                                       example that could be loaded using IAP.

 - IAP/IAP_Binary_Template/Inc: contains the binary template firmware header files 
     - IAP/IAP_Binary_Template/Inc/stm32l4xx_hal_conf.h  Library Configuration file
     - IAP/IAP_Binary_Template/Inc/stm32l4xx_it.h        Header for stm32l4xx_it.c
     - IAP/IAP_Binary_Template/Inc/main.h                Header for main.c

 - IAP/IAP_Binary_Template/Src: contains the binary template firmware source files 
     - IAP/IAP_Binary_Template/Src/main.c              Main program
     - IAP/IAP_Binary_Template/Src/stm32l4xx_it.c      Interrupt handlers
     - IAP/IAP_Binary_Template/Src/system_stm32l4xx.c  STM32L4xx system source file

@par Hardware and Software environment

  - This example runs on STM32L476xx devices.

  - This example has been tested with STMicroelectronics STM32L476G-EVAL evaluation board
    and can be easily tailored to any other supported device and development board.

@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM:
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM32L476G_EVAL_SysTick.bin" will be generated under "STM32L476G_EVAL/Exe" folder.  
    - Finally load this image with IAP application

 - MDK-ARM:
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "/IAP_Binary_Template/MDK-ARM" directory and run "axftobin.bat"
      (Fromelf Exe path might have to be updated in "axftobin.bat" file, according to your Keil setup).
    - A binary file "STM32L476G_EVAL_SysTick.bin" will be generated under "/IAP_Binary_Template/MDK-ARM/STM32L476G_EVAL" folder. 
    - Finally load this image with IAP application

 - SW4STM32
    - Open the SW4STM32 toolchain.
    - Browse to the SW4STM32 workspace directory, select the project (.project file in \IAP_Binary_Template\SW4STM32\STM32L476G_EVAL directory).
    - Rebuild all project files.
    - A binary file "STM32L476G_EVAL.bin" will be generated under "STM32L476G_EVAL\Debug" folder. 
    - Finally load this image with IAP application

 */
