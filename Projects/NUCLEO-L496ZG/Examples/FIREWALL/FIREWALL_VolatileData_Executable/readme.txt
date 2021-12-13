/**
  @page FIREWALL_VolatileData_Executable FIREWALL executable volatile data protection example
  @verbatim
  ******************************************************************************
  * @file    FIREWALL/FIREWALL_VolatileData_Executable/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FIREWALL executable volatile data segment protection example.
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
 
How to use the Firewall Peripheral to protect a volatile data segment and to define it
as executable.

The volatile data segment is located in SRAM1.

In the example Firewall initialization, the protected volatile data segment is 
defined as non-shared so that it can't be accessed by non protected code when
the Firewall is closed.

The Firewall-protected memory segment is split in two sub segments ("a" region
and "b" region) defined as follows by the scatter file 


  |___________________________| 
  |                           |   0x20017FFF
  |                           |
  |            RAM            |  
  |                           |
  |___________________________|   0x20010000
  |                           |   0x2000FFFF
  |   Volatile data segment   |
  |        "b" region         |
  |           (data)          |  
  |___________________________|  0x2000FE00
  |                           |  0x2000FDFF
  |   Volatile data segment   |
  |        "a" region         | 
  |          (code)           |     
  |___________________________|  0x2000F104
  |___________________________|   
  |                           |  0x2000F0FF
  |                           |
  |            ROM            | 
  |                           |  
  |___________________________|  0x20000000
  |                           |

  

 The example unrolls as follows.
 
 The Firewall is first configured such that the volatile data segment is defined
 as executable.
 Protected code (that could be the user application code) located in protected
 volatile data segment (in "a" region) is executed twice and it is verified that
 its processing returns the expected output each time.

 Two different ways to invoke the protected executable data are shown in the
 example, as well as the FPA bit reset when the Firewall is closed to avoid
 any unwanted access to the protected area. 

 
 If the code unrolls as expected, LED1 is turned on.
 If an error occurs (unexpected Firewall reset or any other unexpected
 result), LED1 blinks in transmitting a sequence of three dots, three dashes, three dots.


 The Firewall software reset leads to resort to RTC back-up registers
 to keep track of the code unrolling. The latter are not related to Firewall
 feature but their usage is shown for possible debugging purposes.

 The table below lists them and provides their meaning.

              
 reg.\value   |       0       |             1             |        2
 ------------------------------------------------------------------------------
 RTC->BKP4R   |  Example very |  main() has been started  |        N/A
              |  first start  |    at least one time      |  
 -----------------------------------------------------------------------------                  
 RTC->BKP0R   | Initial value |     Unexpected Firewall   | Incorrect protected
              |               |           reset           | function first run 
 -----------------------------------------------------------------------------                  
 RTC->BKP1R   | Initial value | Unexpected software reset | Incorrect protected
              |               | (not Firewall-triggered)  | function second run
 -----------------------------------------------------------------------------                                       

                                                                                                                 

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
      
@note MDK-ARM project is configured to run this example in debug session.      

@par Directory contents 

  - FIREWALL/FIREWALL_VolatileData_Executable/Inc/stm32l4xx_conf.h         HAL Configuration file
  - FIREWALL/FIREWALL_VolatileData_Executable/Inc/stm32l4xx_it.h           Header for stm32l4xx_it.c
  - FIREWALL/FIREWALL_VolatileData_Executable/Inc/main.h                   Header file for main.c
  - FIREWALL/FIREWALL_VolatileData_Executable/Inc/protected_vdata_code.h   Header file for protected_vdata_code.c 
  - FIREWALL/FIREWALL_VolatileData_Executable/Src/system_stm32l4xx.c       STM32L4xx system clock configuration file
  - FIREWALL/FIREWALL_VolatileData_Executable/Src/stm32l4xx_it.c           Interrupt handlers
  - FIREWALL/FIREWALL_VolatileData_Executable/Src/main.c                   Main program
  - FIREWALL/FIREWALL_VolatileData_Executable/Src/protected_vdata_code.c         Code to be located in protected volatile data segment
  - FIREWALL/FIREWALL_VolatileData_Executable/Src/protected_volatile_data.c       Data to be located in protected volatile data segment
  - FIREWALL/FIREWALL_VolatileData_Executable/EWARM/stm32l496xx_sram.icf   Scatter file for proper placing in memory            

@par Hardware and Software environment


  - This example runs on STM32L4xx devices
      
  - This example has been tested with STMicroelectronics NUCLEO-L496ZG (144 pins)
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory. In using ST-Link Utility tool, 
   load your image into target memory SRAM1 area at start address 0x20000000. Make sure 
   option bytes are correctly set to select SRAM1 as boot area (refer to Reference Manual 
   if needed).
 - Run the example


 */
