/**
  @page FLASH_FastProgram FLASH Erase and Fast Program example
  
  @verbatim
  ******************************************************************************
  * @file    FLASH/FLASH_FastProgram/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FLASH Erase and Fast Program example.
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

How to configure and use the FLASH HAL API to erase and fast program the
internal Flash memory.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) 
to run at 80 MHz. 

After Reset, the Flash memory Program/Erase Controller is locked. A dedicated function
is used to enable the FLASH control register access.
Before programming the desired addresses, an erase operation is performed using 
the flash erase page feature. The erase procedure is done by filling the erase init 
structure giving the bank which will be mass erased.

Once this operation is finished, page fast programming operation will be performed 
in the Flash memory. The written data is then read back and checked.

The NUCLEO-L476RG Rev C board LEDs can be used to monitor the transfer status:
 - LED2 is ON when there are no errors detected after programming
 - LED2 is blinking with a 1s period when there are errors detected after programming
 - LED2 is blinking (100ms on, 2s off) when there is an issue during erase or program procedure

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - FLASH/FLASH_FastProgram/Inc/stm32l4xx_hal_conf.h        HAL Configuration file  
  - FLASH/FLASH_FastProgram/Inc/stm32l4xx_it.h              Header for stm32l4xx_it.c
  - FLASH/FLASH_FastProgram/Inc/main.h                      Header for main.c module 
  - FLASH/FLASH_FastProgram/Src/stm32l4xx_it.c              Interrupt handlers
  - FLASH/FLASH_FastProgram/Src/main.c                      Main program
  - FLASH/FLASH_FastProgram/Src/system_stm32l4xx.c          STM32L4xx system clock configuration file

@par Hardware and Software environment

  - This example runs on STM32L476xx devices.
    
  - This example has been tested with NUCLEO-L476RG Rev C board and can be
    easily tailored to any other supported device and development board.
      
@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
