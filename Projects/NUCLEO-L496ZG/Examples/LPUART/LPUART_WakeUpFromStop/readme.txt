/**
  @page LPUART_WakeUpFromStop LPUART Wake up from Stop mode example

  @verbatim
  ******************************************************************************
  * @file    UART/LPUART_WakeUpFromStop/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LPUART-triggered MCU wake-up from STOP mode.
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

Configuration of an LPUART to wake up the MCU from Stop mode 
when a given stimulus is received.

Board: NUCLEO-L496ZG (144 pins) (embeds a STM32L496ZGT6 device)
Tx Pin: PB.11 (pin 18 on CN12)
Rx Pin: PB.10 (pin 25 on CN12)
   _________________________                       _________________________ 
  |           ______________|                     |______________           |
  |          |LPUART        |                     |        LPUART|          |
  |          |              |                     |              |          |
  |          |           TX |_____________________| RX           |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |           RX |_____________________| TX           |          |
  |          |              |                     |              |          |
  |          |______________|                     |______________|          |
  |                         |                     |                         |
  |                         |                     |                         |
  | STM32 Board 1           |                     | STM32 Board 2           |
  | (follows a sequence of  |                     | (sends stimuli to wake  |
  | RUN/STOP modes)         |                     |  up STM32 Board 1)      |
  |_________________________|                     |_________________________|


Board 1 enters STOP mode and is awoken by Board 2 which sends
the proper data to wake up the board 1 MCU.
When the proper wake-up event is recognized, the WUF interrupt is triggered which wakes
up board 1.
To confirm its wake up, board 1 sends a confirmation message to board 2 which
checks it is the expected message.
This cycle is repeated 4 times to verify 4 different events
1°) wake-up by RXNE flag rising
2°) wake-up by Start Bit detection
3°) wake-up by 7-bit long address match
4°) wake-up by 4-bit long address match

WARNING: as both boards do not behave the same way, "BOARD_IN_STOP_MODE" 
compilation switch is defined in UART/LPUART_WakeUpFromStop/Src/main.c and must be
enabled at compilation time before loading the executable in board 1 (that which
is set in STOP mode).
The stimuli-transmitting board (board 2) needs to be loaded with an executable
software obtained with BOARD_IN_STOP_MODE disabled. 

At the very beginning, board 1 LED1 is ON then is turned off.
This means that board 1 has entered STOP mode.
Parallelly, board 2 LED1 is quickly blinking (100 ms period). The user has 
just to wait for board 1 LED1 to be turned off then to press board 2 User push-button 
to send the first wake-up stimulus to board 1. 
This starts the test sequence and no more action is required from the user
(a single User push-button press is needed to start the full 4 wake-up events
test sequence).

Board 1 LED1 behavior as described below allows to follow the 
test progress:

ON ____          _______          _______          _______          ____
       |        |       |        |       |        |       |        |    
OFF    |________|       |________|       |________|       |________|    
      Enter    Exit    Enter    Exit    Enter    Exit    Enter    Exit  
      Stop     Stop    Stop     Stop    Stop     Stop    Stop     Stop  
      Mode     Mode    Mode     Mode    Mode     Mode    Mode     Mode  
            (RXNE flag)      (Start Bit      (7-bit add.       (4-bit add.
                              Detection)        match)            match)
                               


Simultaneously, board 2 LED1 behavior shown below allows to follow the 
test progress on the other side:

  BLINKING   
ON ****________          _______          _______          _______          ____
               |        |       |        |       |        |       |        |    
OFF            |________|       |________|       |________|       |________|    
     USER     WkUp   2nd stim  WkUp   3rd stim  WkUp   4th stim  WkUp    LED ON  
     button   conf     sent    conf     sent    conf     sent    conf    to mark  
    pushed,   rcv'ed          rcv'ed           rcv'ed           rcv'ed   passed
  1st stimulus                                                            test 
     sent                                                                    
  (RXNE flag)      (Start Bit       (7-bit add.      (4-bit add.
                    Detection)         match)           match)
     
    
- If the test fails or if there is an initialization or transfer error, LED1
transmits a sequence of three dots, three dashes, three dots.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.

Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 80 MHz with MSI as source of main PLL.
Additionally the HSI is enabled to demonstrate LPUART wake-up capabilities.
Each time board 1 is awoken, the system clock (SYSCLK) at 80 MHz is restored.



The LPUART is configured as follows:
    - BaudRate = 9600 baud  
    - Word Length = 8 bits (8 data bits, no parity bit)
    - One Stop Bit
    - Parity none
    - Hardware flow control disabled (RTS and CTS signals)


@note LPUART instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

@note This example can not be used in DEBUG mode due to the fact that the 
    Cortex-M4 core is no longer clocked during low power mode and so debugging 
    features are not available.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@par Directory contents 

  - LPUART/LPUART_WakeUpFromStop/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - LPUART/LPUART_WakeUpFromStop/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - LPUART/LPUART_WakeUpFromStop/Inc/main.h                  Header for main.c module  
  - LPUART/LPUART_WakeUpFromStop/Src/stm32l4xx_it.c          Interrupt handlers
  - LPUART/LPUART_WakeUpFromStop/Src/main.c                  Main program
  - LPUART/LPUART_WakeUpFromStop/Src/stm32l4xx_hal_msp.c     HAL MSP module
  - LPUART/LPUART_WakeUpFromStop/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment 

  - This example runs on STM32L496xx devices.
    
  - This example has been tested with a couple of NUCLEO-L496ZG (144 pins) boards embedding a 
    STM32L496ZGT6 device and can be easily tailored to any other supported device 
    and development board.
    
  - NUCLEO-L496ZG (144 pins) set-up
    - Connect a wire between 1st board PB.11 (pin 18 on CN12) pin (LPUART Tx) and 2nd board PB.10 (pin 25 on CN12) pin (LPUART Rx)
    - Connect a wire between 1st board PB.10 (pin 25 on CN12) pin (LPUART Rx) and 2nd board PB.11 (pin 18 on CN12) pin (LPUART Tx)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 */
