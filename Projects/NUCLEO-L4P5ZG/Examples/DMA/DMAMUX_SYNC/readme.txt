/**
  @page DMAMUX_SYNC DMA & DMAMUX Synchronization Example

  @verbatim
  ******************************************************************************
  * @file    DMA/DMAMUX_SYNC/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the DMA & DMAMUX Synchronization Example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to use the DMA with the DMAMUX to synchronize a transfer with the LPTIM1
output signal. LPUART1 is used in DMA synchronized mode to send a countdown from
10 to 00, with a period of 2 seconds.

The example uses the LPUART1 in DMA synchronized mode to send a countdown from 10 to 00 with 2sec period.
The DMAMUX synchronization block is configured to synchronize the DMA transfer with the LPTIM1 output signal.
Each rising edge of the synchronization signal (i.e LPTIM1 output signal) will authorize 4 LPUART1 requests to be
transmitted to the the LPUART1 peripheral using the DMA. these four requests represent the 2 characters '\n\r' plus
the 2 characters count down itself from '10' to '00'.
The LPTIM1 is configured to generate a PWM with 2 seconds period.

The example requires to connect the board to an HyperTerminal PC application through Virtual Com port feature of STLINK.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 80 MHz for STM32L4xx Devices.

The DMA is configured in memory to peripheral mode to ensure data transfer
from the source transmission buffer (TxSyncMessage) to the LPUART1 (in order to be transmitted to the UART/HyperTerminal).

The DMAMUX synchronization block is configured using function "HAL_DMAEx_ConfigMuxSync" with the following parameters :
- SyncSignalID  : set to HAL_DMAMUX1_SYNC_LPTIM1_OUT which corresponds to LPTIM1_OUT signal.
- SyncPolarity  : Set to RISING to use rising edge of LPTIM1 output signal for synchronization.
- RequestNumber : 4 i.e four LPUART1 requests are authorized after each rising edge of the sync signal.
- EventState : enabled , in order to generate an event each time "RequestNumber" are transmitted.
  Note that this event could be used with the DMAMUX external request generator to trigger another DMA stream transfer.

The LPTIM1 is configured using function "LPTIM_Config" in order to configure and start a PWM on the LPTIM1 timer with
2 seconds period and 50% duty cycle.

The LPUART1 peripheral configuration is ensured by the HAL_UART_Init() function.
This later is calling the HAL_UART_MspInit()function which core is implementing
the configuration of the needed UART resources according to the used hardware (CLOCK,
GPIO and NVIC).
DMA & DMAMUX configurations are performed in the main.c module as the example first purpose is to show
the DMA & DMAMUX configuration when using the DMAMUX synchronization block.
In normal conditions DMA and DMAMUX configuration should be placed in the HAL_UART_MspInit()function.

A first example brief description message is then transmitted using the LPUART1 to the PC HyperTerminal
in Polling mode (using message buffer ).

Then a second UART transfer is started in DMA synchronized mode to transfer the countdown buffer "TxEndMessage".
As result the countdown from 10 to 00 with a period of 2 seconds should be displayed on the HyperTerminal.
The end of this step is monitored through the HAL_UART_GetState() function result.

In the last step an ending message is sent using the LPUART1 in polling mode through the "TxEndMessage" buffer.

STM32L4P5ZG-Nucleo (144 pins) board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the transmission process is complete.
 - LED3 is ON when there is an error in transmission process.

The UART is configured as follows:
    - BaudRate = 38400 baud
    - Word Length = 8 Bits
    - One Stop Bit
    - parity none
    - Hardware flow control disabled (RTS and CTS signals)

@note USARTx/UARTx instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

@note This example can be easily modified to use any other peripheral (than the LPUART1) with a DMA synchronized transfer.
In this case user will need to replace the LPUART1 initialization and processing by the used IP initialization and processing functions.
User can also change the synchronization signal (based on the sync signals list available in the reference manual),
in this case the sync signal IP should be configured (replacing the function LPTIM_Config by the required sync signal IP configuration)

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents

  - DMA/DMAMUX_SYNC/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - DMA/DMAMUX_SYNC/Inc/stm32l4xx_it.h          DMA interrupt handlers header file
  - DMA/DMAMUX_SYNC/Inc/main.h                  Header for main.c module
  - DMA/DMAMUX_SYNC/Src/stm32l4xx_it.c          DMA interrupt handlers
  - DMA/DMAMUX_SYNC/Src/main.c                  Main program
  - DMA/DMAMUX_SYNC/Src/stm32l4xx_hal_msp.c     HAL MSP module
  - DMA/DMAMUX_SYNC/Src/system_stm32l4xx.c      FAMILYNAME_UC system source file


@par Hardware and Software environment

  - This example runs on STM32L4P5xx device.

  - This example has been tested with STM32L4P5ZG-Nucleo (144 pins) board and can be
    easily tailored to any other supported device and development board.

  - STM32L4P5ZG-Nucleo (144 pins) Set-up
    Example is delivered for using Virtual Com port feature of STLINK for connection between STM32L4P5ZG-Nucleo (144 pins) and PC,
    Please ensure that USART communication between the target MCU and ST-LINK MCU is properly enabled
    on HW board in order to support Virtual Com Port (Default HW SB configuration allows use of VCP)
    GPIOs connected to LPUART1 TX/RX (PG.07 and PG.08) are automatically mapped
    on RX and TX pins of PC UART Com port selected on PC side (please ensure VCP com port is selected).

  - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration
    - Word Length = 8 Bits
    - One Stop Bit
    - None parity
    - Baud Rate = 38400 baud
    - flow control: None


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
