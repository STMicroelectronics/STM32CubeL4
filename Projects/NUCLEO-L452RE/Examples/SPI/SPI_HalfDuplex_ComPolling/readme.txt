/**
  @page SPI_HalfDuplex_ComPolling SPI Half Duplex Polling example
  
  @verbatim
  ******************************************************************************
  * @file    SPI/SPI_HalfDuplex_ComPolling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI Half Duplex Polling example.
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

Data buffer half-duplex transmission/reception between two boards via SPI using Polling mode.

   _________________________                       __________________________
  |       _______ CN10 _____|                      |_______ CN10 _____       |
  |      |SPI1              |                      |             SPI1 |      |
  |      |                  |                      |                  |      |
  |      |(pin 31) CLK(PB3) |______________________|(PB3)CLK  (pin 31)|      |
  |      |                  |                      |                  |      |
  |      |(pin 29) MOSI(PB5)|______1k resitor______|(PB4)MISO (pin 27)|      |
  |      |                  |                      |                  |      |
  |       _______ CN7  _____|                      |_______ CN7  _____       |
  |      |(pin 18) +5V      |______________________| E5V      (pin 6) |      |
  |      |__________________|                      |__________________|      |
  |      __                 |                      |                         |
  |     |__|                |                      |     JP5 (E5V)*-* *(U5V) |
  |     USER                |                      |                         |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32   Master _________|                      |_STM32   Slave __________|

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 80 MHz.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK & 
GPIO). You may update this function to change SPI configuration.

The communication is handled with direct register access. 
On slave side, a state machine manage the differents communication step, to handle
two kinds of communication:
 - register read : master use this operation to get a value from the slave.
 - register write: master use this operation to update a value on slave side.
On master side basic operations are done by calling the functions : Command_write 
and Command_read

The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.h" file.
If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.

This example desmonstrate how to implement a basic protocol between master and slave. 
The master is writing and reading a register and check that write and read value are 
identical. The slave sw is an infinite loop which execute the operation requested by the master.

In a first step after the user press the User push-button, SPI Master starts slave communication 
, a loop to request READ_COUNTER_COMMAND and WRITE_COUNTER_COMMAND. The master control the 
communication by a comparison between the read and write value. 

STM32 board's LEDs can be used to monitor the master transfer status:
 - LED2 toggles slowly on master board waiting user button to be pressed.
 - LED2 toggles quickly on master when the test is running.
 - LED2 ON on master and slave when register comparison failed.   

@note SPIx instance used and associated resources can be updated in "main.h"
      file depending hardware configuration used.

@note You need to perform a reset on Slave board, then perform it on Master board
      to have the correct behaviour of this example.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - SPI/SPI_HalfDuplex_ComPolling/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - SPI/SPI_HalfDuplex_ComPolling/Inc/stm32l4xx_it.h          SPI interrupt handlers header file
  - SPI/SPI_HalfDuplex_ComPolling/Inc/main.h                  Header for main.c module  
  - SPI/SPI_HalfDuplex_ComPolling/Src/stm32l4xx_it.c          SPI interrupt handlers
  - SPI/SPI_HalfDuplex_ComPolling/Src/main.c                  Main program
  - SPI/SPI_HalfDuplex_ComPolling/Src/system_stm32l4xx.c      STM32L4xx system source file
  - SPI/SPI_HalfDuplex_ComPolling/Src/stm32l4xx_hal_msp.c     HAL MSP file
  

@par Hardware and Software environment

  - This example runs on STM32L452xx devices.
    
  - This example has been tested with NUCLEO-L452RE Rev C board and can be
    easily tailored to any other supported device and development board.

  -NUCLEO-L452RE Rev C Set-up
    - Connect Master board PB3 to Slave Board PB3 (pin 31 on CN10 connector)
    - Connect Master board PB5 (pin 29 on CN10 connector) to Slave Board PB4 (pin 27 on CN10 connector)
    - Connect Master board GND to Slave Board GND
    - Slave board powered by master board
          - JP5 fitted 3-2
          - Connect Master pin18 (CN7 connector) to Slave pin6 (connector CN7)

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example

 */
 