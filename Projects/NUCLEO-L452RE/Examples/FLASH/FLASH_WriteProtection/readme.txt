/**
  @page FLASH_WriteProtection FLASH write protection
  
  @verbatim
  ******************************************************************************
  * @file    FLASH/FLASH_WriteProtection/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FLASH write protection example.
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

How to configure and use the FLASH HAL API to enable and disable the write 
protection of the internal Flash memory.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) 
to run at 80 MHz.

  - If WRITE_PROTECTION_ENABLE is selected, the write protection will be enabled
    for the defined pages.
    To load the new option byte values, a system Reset is necessary, for this, the
    function HAL_FLASH_OB_Launch() is used.
    
  - If WRITE_PROTECTION_DISABLE is selected, the write protection will be disabled
    for the defined pages.
    To load the new option byte values, a system Reset is necessary, for this, the
    function HAL_FLASH_OB_Launch() is used.
    
=> !!! Be careful, both WRITE_PROTECTION_ENABLE and WRITE_PROTECTION_DISABLE should not be 
       selected at the same time, otherwise WRITE_PROTECTION_DISABLE is taken in account !!!
    
  - If FLASH_PAGE_PROGRAM is selected, then an erase operation is done by filling 
    the erase init structure giving the starting erase page and the number of 
    pages to erase. At this stage, all these pages will be erased one by one separately.

    @note: if problem occurs on a page, erase will be stopped and faulty page will 
    be returned to user (through variable 'PageError').

    Once this operation is finished, double-word programming operation will be performed 
    in the Flash memory. The written data is then read back and checked.

NUCLEO-L452RE Rev C board's LED can be used to monitor the transfer status:
 - LED2 is ON when there are no errors detected after programming
    => should be the case when WRITE_PROTECTION_DISABLE flag is enabled
 - LED2 is blinking with a 1s period when there are errors detected after programming
    => should be the case when WRITE_PROTECTION_ENABLE flag is enabled
 - LED2 is blinking (100ms on, 2s off) when there is an issue during erase, program or OB program procedure

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - FLASH/FLASH_WriteProtection/Inc/stm32l4xx_hal_conf.h        HAL Configuration file  
  - FLASH/FLASH_WriteProtection/Inc/stm32l4xx_it.h              Header for stm32l4xx_it.c
  - FLASH/FLASH_WriteProtection/Inc/main.h                      Header for main.c module 
  - FLASH/FLASH_WriteProtection/Src/stm32l4xx_it.c              Interrupt handlers
  - FLASH/FLASH_WriteProtection/Src/main.c                      Main program
  - FLASH/FLASH_WriteProtection/Src/system_stm32l4xx.c          STM32L4xx system clock configuration file

@par Hardware and Software environment

  - This example runs on STM32L452xx devices.
    
  - This example has been tested with NUCLEO-L452RE Rev C board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
