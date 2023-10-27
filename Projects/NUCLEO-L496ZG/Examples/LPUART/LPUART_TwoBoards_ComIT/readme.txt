/**
  @page LPUART_TwoBoards_ComIT LPUART Two Boards Communication IT example

  @verbatim
  ******************************************************************************
  * @file    UART/LPUART_TwoBoards_ComIT/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LPUART Two Boards Communication IT example.
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

LPUART transmission (transmit/receive) in Interrupt mode 
between two boards.

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
  |                      GND|_____________________|GND                      |
  |_STM32_Board 1___________|                     |_STM32_Board 2___________|



Two identical boards are connected as shown on the picture above.
Board 1: transmitting then receiving board
Board 2: receiving then transmitting board

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 80 MHz.

The user presses the User push-button on board 1.
Then data are transmitted through LPUART1 in IT mode. 2nd board has configured LPUART1
to receive data in IT mode. Once data received is completed, 2nd board also 
transmit same characters on LPUART1 which will be received by 1st board.
Both boards compare transmitted data to received ones. If they are same,
LED1 is turned on, else LED1 is turned off.

Warning: As both boards do not behave same way, "TRANSMITTER_BOARD" switch 
compilation exists to determine either software is for 1st transmitter board or 
2nd receiver (then transmitter) board. In other words, 1st board has to be 
flashed with software compiled with switch enable, 2nd board has to be flashed 
with software compiled with switch disable.

LED1 can be used to monitor the transfer status:
 - LED1 turns ON on master board waiting User push-button to be pressed.
 - LED1 turns OFF on master board waiting the transmission process complete.
 - LED1 turns ON when the transmission/reception process is correct.
 - If there is an initialization or transfer error, LED1 is slowly blinking (1 sec. period).

The LPUART is configured as follow:
    - BaudRate = 115200 baud
    - Word Length = 8 Bits (7 data bit + 1 parity bit)
    - One Stop Bit
    - None parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled in the time

@note LPUART instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

@note When the parity is enabled, the computed parity is inserted at the MSB
position of the transmitted data.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - LPUART/LPUART_TwoBoards_ComIT/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - LPUART/LPUART_TwoBoards_ComIT/Inc/stm32l4xx_it.h          IT interrupt handlers header file
  - LPUART/LPUART_TwoBoards_ComIT/Inc/main.h                  Header for main.c module  
  - LPUART/LPUART_TwoBoards_ComIT/Src/stm32l4xx_it.c          IT interrupt handlers
  - LPUART/LPUART_TwoBoards_ComIT/Src/main.c                  Main program
  - LPUART/LPUART_TwoBoards_ComIT/Src/stm32l4xx_hal_msp.c     HAL MSP module
  - LPUART/LPUART_TwoBoards_ComIT/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment 

  - This example runs on STM32L496xx devices.
    
  - This example has been tested with NUCLEO-L496ZG (144 pins) and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-L496ZG (144 pins) set-up
    - Connect a wire between 1st board PB.11 (pin 18 on CN12) pin (LPUART Tx) and 2nd board PB.10 (pin 25 on CN12) pin (LPUART Rx).
    - Connect a wire between 1st board PB.10 (pin 25 on CN12) pin (LPUART Rx) and 2nd board PB.11 (pin 18 on CN12) pin (LPUART Tx).
    - Connect 1st board GND to 2nd Board GND.
  
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 */
