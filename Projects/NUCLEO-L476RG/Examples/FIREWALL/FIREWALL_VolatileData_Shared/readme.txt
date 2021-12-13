/**
  @page FIREWALL_VolatileData_Shared FIREWALL Volatile Data sharing example
  @verbatim
  ******************************************************************************
  * @file    FIREWALL/FIREWALL_VolatileData_Shared/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FIREWALL volatile data segment sharing example.
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
 
How to use the Firewall Peripheral to protect a code segment as well as volatile and
non-volatile data segments.

Volatile data segment is located in SRAM1.
Non-volatile data segment is located in FLASH.

In the example Firewall initialization, the protected volatile data segment is 
first defined as shared so that it can be accessed by non protected code.

For Firewall demonstration's sake, two Firewall software resets are purposely
triggered.
- the first one in accessing the protected non-volatile data segment
- the second one in accessing the protected volatile data segment once 
the sharing feature has been disabled.


The Firewall-protected memory segments are defined as follows

  |                           |
  |                           |
  |___________________________|  0x2000023F
  |                           |
  |   Volatile data segment   |
  |___________________________|  0x20000000
  |                           |
  |                           |
  |                           |  
  |___________________________| 0x080FF0FF
  |                           |
  | Non-volatile data segment |
  |___________________________| 0x080FF000
  |                           |
  |___________________________| 0x080101FF
  |                           |
  |       Code segment        |
  |___________________________| 0x08010000
  |                           |
  

 The example unrolls as follows.
 
 The main() function starts its first run.
 The Firewall is configured such that the volatile data segment is shared
 with non protected code.
 Protected code (that could be the user application code) is executed and it is verified that
 - the array located in the volatile data segment is updated as expected
 - no Firewall reset occurs when unprotected code segment accesses the array.
 
 Next, while the Firewall is closed, a Firewall reset is purposely triggered
 in accessing an array located in the protected non-volatile data segment.
 
 This leads to a software reset and a second main() run.
 The second run is similar to the first one, the firewall is configured the
 same way but this time, the protected code when called disables the volatile 
 data sharing feature.
 
 A second Firewall reset is purposely triggered in accessing the no more
 shared array located in the protected volatile data segment.
 
 If the code unrolls as expected, LED2 is turned on.
 If an error occurs (unexpected Firewall reset or any other unexpected
 result), LED2 blinks in transmitting a sequence of three dots, three dashes, three dots.


 The Firewall-triggered software resets lead to resort to RTC back-up registers
 to keep track of the code unrolling. The latter are not related to Firewall
 feature but their usage is shown for possible debugging purposes.

 The table below lists them and provides their meaning.

                
 register\value |        0         |               1              |     2
 ------------------------------------------------------------------------------
 RTC->BKP4R     |   Example very   |      main() first run        |    main() 
                |   first start    |                              |  second run
 ------------------------------------------------------------------------------                  
 RTC->BKP0R     |  Initial value   |   Access to protected        |   VDS has 
                |                  |   volatile data segment      |    been
                |                  |   successful when VDS=1      |    reset
 ------------------------------------------------------------------------------                  
 RTC->BKP1R     |  Initial value   |  Unexpected Firewall reset   |    N/A
 ------------------------------------------------------------------------------                  
 RTC->BKP2R     |  Initial value   | No Firewall reset occurred    |    N/A 
                |                  | when protected non-volatile  |  
                |                  | data were accessed,          |  
                |                  | although reset was expected  |  
 ------------------------------------------------------------------------------                  
 RTC->BKP3R     |  Initial value   | No Firewall reset occurred    |    N/A 
                |                  | when protected volatile      |  
                |                  | data were accessed, although |  
                |                  | reset was expected           |  
 ------------------------------------------------------------------------------                                       
                                      
 When the software detects a Firewall reset with RTC->BKP0R = 2, it infers
 that the code properly behaved, ending up by an expected Firewall software
 reset due to unauthorized access to protected volatile data while the
 Firewall was closed.
                                                                                                                 

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note While running the example on an IDE, make sure to close as many debug windows as
      possible to avoid any unwanted Firewall-triggered software reset due to a debugger
      access to a protected code or variable.

@par Directory contents 

  - FIREWALL/FIREWALL_VolatileData_Shared/Inc/stm32l4xx_conf.h         HAL Configuration file
  - FIREWALL/FIREWALL_VolatileData_Shared/Inc/stm32l4xx_it.h           Header for stm32l4xx_it.c
  - FIREWALL/FIREWALL_VolatileData_Shared/Inc/main.h                   Header file for main.c
  - FIREWALL/FIREWALL_VolatileData_Shared/Inc/protected_code.h         Header file for protected_code.c  
  - FIREWALL/FIREWALL_VolatileData_Shared/Src/system_stm32l4xx.c       STM32L4xx system clock configuration file
  - FIREWALL/FIREWALL_VolatileData_Shared/Src/stm32l4xx_it.c           Interrupt handlers
  - FIREWALL/FIREWALL_VolatileData_Shared/Src/main.c                   Main program
  - FIREWALL/FIREWALL_VolatileData_Shared/Src/protected_code.c         Code to be located in protected code segment
  - FIREWALL/FIREWALL_VolatileData_Shared/Src/proprotected_nonvolatile_data.c Data to be located in protected non-volatile data segment
  - FIREWALL/FIREWALL_VolatileData_Shared/Src/protected_volatile_data.c       Data to be located in protected volatile data segment
  - FIREWALL/FIREWALL_VolatileData_Shared/EWARM/stm32l476xx_flash.icf Scatter file for proper placing in memory            

@par Hardware and Software environment


  - This example runs on STM32L476xx devices
      
  - This example has been tested with STMicroelectronics NUCLEO-L476RG Rev C
    evaluation board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
