/**
  @page CAN_Networking CAN Networking example
  
  @verbatim
  ******************************************************************************
  * @file    CAN/CAN_Networking/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CAN Networking example.
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

How to configure the CAN peripheral to send and receive CAN frames in 
normal mode. 

The sent frames are used to control LEDs by pressing Wkup/Tamper push-button.
	
The CAN serial communication link is a bus to which a number of units may be
connected. This number has no theoretical limit. Practically the total number
of units will be limited by delay times and/or electrical loads on the bus line.

This program behaves as follows:
  - After reset, all LEDs are OFF on the N eval boards
  - By Pressing on Wkup/Tamper push-button : LED1 turns ON and all other LEDs are OFF, on the N
    eval boards connected to the bus. 
  - Press on Wkup/Tamper push-button again to send CAN Frame to command LEDn+1 ON, all other LEDs 
    are OFF on the N eval boards.
    
@note This example is tested with a bus of 3 units. The same program example is 
      loaded in all units to send and receive frames.
@note Any unit in the CAN bus may play the role of sender (by pressing Wkup/Tamper push-button)
      or receiver.

  The CAN is configured as follows:
    - Bit Rate   = 1 Mbit/s  
    - CAN Clock  = external clock (HSE)
    - ID Filter  = All identifiers are allowed
    - RTR = Data
    - DLC = 1 byte
    - Data: Led number that should be turned ON


@par Directory contents 

  - CAN/CAN_Networking/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - CAN/CAN_Networking/Inc/stm32l4xx_it.h          DMA interrupt handlers header file
  - CAN/CAN_Networking/Inc/main.h                  Header for main.c module  
  - CAN/CAN_Networking/Src/stm32l4xx_it.c          DMA interrupt handlers
  - CAN/CAN_Networking/Src/main.c                  Main program
  - CAN/CAN_Networking/Src/stm32l4xx_hal_msp.c     HAL MSP file
  - CAN/CAN_Networking/Src/system_stm32l4xx.c      STM32L4xx system source file
  
@par Hardware and Software environment

  - This example runs on STM32L476xx devices.
  - This example has been tested with STM32L476G-EVAL board and can be
    easily tailored to any other supported device and development board.

  - STM32L476G-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4 connected respectively to PB.02, IO_PIN_1 (IOExpander1), PC.01
      and IO_PIN_2 (IOExpander1) pins
    - Use Wkup/Tamper push-button connected to PC.13
    - Connect a male/male CAN cable between at least 2 EVAL CAN connectors (CN5)
    - You have to configure the jumpers as follows:

  @verbatim
  
       +-------------------------------+
       |   Jumper    |       CAN       |
       +-------------------------------+
       |   JP4       |       1-2       |
       |   JP6       |      fitted     |
       +-------------------------------+
  @endverbatim
  
@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 */
 
