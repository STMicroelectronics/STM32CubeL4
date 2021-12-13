/**
  @page SWPMI_Loopback_MultiSWBuffer SWPMI Multi buffer communication in loopback mode
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/SWPMI/SWPMI_Loopback_MultiSWBuffer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SWPMI Multi buffer communication in loopback mode example.
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

Configuration of the SWPMI peripheral to start a communication 
using DMA multibuffers in Loopback mode. The peripheral initialization uses 
LL unitary service functions for optimization purposes (performance and size).

In this example, after start-up, SYSCLK is configured to the max frequency using the PLL with
MSI as clock source.

Then, DMA configuration is done to enable TX/RX channels for SWPMI links and set the different buffers 
which will be used for the transfer. 
SWPMI is set in loopback mode with max bitrate of 2Mbit/s. 
Connection needed: 
 - None, because SWPMI is in loopback mode (mode for test purpose, connection is done internally).

LED1 blinks quickly to wait for user-button press. 
As soon as, user-button is pressed, SWPMI communication starts in DMA mode:
- DMA Circular mode enabled
- In SWPMI transfer buffer empty interrupt routine, when 3 1st frames have
  been transmitted, an update of the Multi buffer is done with 3 new frames
- In the same routine, when last frame has been sent, a frame payload size erase is done 
  to stop automatically SWPMI TX DMA.
- A the end of the transfer, a check is done between the reception buffer and the expected reception 
buffer:
   - LED1 ON: successful transmission
   - LED1 blinks with 1 sec period: an error occurred during transmission. 

@par Directory contents 

  - SWPMI/SWPMI_Loopback_MultiSWBuffer/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - SWPMI/SWPMI_Loopback_MultiSWBuffer/Inc/main.h                  Header for main.c module  
  - SWPMI/SWPMI_Loopback_MultiSWBuffer/Inc/stm32_assert.h          Template file to include assert_failed function
  - SWPMI/SWPMI_Loopback_MultiSWBuffer/Src/stm32l4xx_it.c          Interrupt handlers
  - SWPMI/SWPMI_Loopback_MultiSWBuffer/Src/main.c                  Main program
  - SWPMI/SWPMI_Loopback_MultiSWBuffer/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L496ZG devices.
    
  - This example has been tested with NUCLEO-L496ZG (144 pins) board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
