/**
  @page SPI_FullDuplex_ComPolling SPI Full Duplex Polling example
  
  @verbatim
  ******************************************************************************
  * @file    SPI/SPI_FullDuplex_ComPolling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI Full Duplex Polling example.
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

Data buffer transmission/reception between two boards via SPI using Polling mode.

   _________________________                       __________________________
  |       ______ CN4 _______|                      |______ CN4 _______       |
  |      |SPI1              |                      |             SPI1 |      |
  |      |                  |                      |                  |      |
  |      |(pin A4) CLK(PA5) |______________________|(PA5)CLK  (pin A4)|      |
  |      |                  |                      |                  |      |
  |      |(pin A5) MISO(PA6)|______________________|(PA6)MISO (pin A5)|      |
  |      |                  |                      |                  |      |
  |      |(pin A6) MOSI(PA7)|______________________|(PA7)MOSI (pin A6)|      |
  |      |                  |                      |                  |      |
  |      |__________________|                      |__________________|      |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32L4 Master _________|                      |_STM32L4 Slave __________|

HAL architecture allows user to easily change code to move to IT or DMA mode. 
To see others communication modes please check following examples:
SPI\SPI_FullDuplex_ComDMA
SPI\SPI_FullDuplex_ComIT

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 80 MHz.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK & 
GPIO). You may update this function to change SPI configuration.

The SPI communication is then initiated.
The HAL_SPI_TransmitReceive() function allows the reception and the 
transmission of a predefined data buffer at the same time (Full Duplex Mode) 
The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file.
If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user connects the PA.12 (Arduino D2) to GND, SPI Master starts the
communication by sending aTxBuffer and receiving aRxBuffer through 
HAL_SPI_TransmitReceive(), at the same time SPI Slave transmits aTxBuffer 
and receives aRxBuffer through HAL_SPI_TransmitReceive(). 
The end of this step is monitored through the HAL_SPI_GetState() function
result.
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to 
check buffers correctness.  

@note Timeout is set to 5 seconds which means that if no communication occurs during 5 seconds,
      a timeout error is generated.

STM32 board LED can be used to monitor the transfer status:
 - LED3 toggles quickly on master board waiting PA.12 (Arduino D2) to be connected to GND
 - LED3 turns ON if transmission/reception is complete and OK.
 - LED3 toggles slowly when there is a timeout or an error in transmission/reception process.   

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

  - SPI/SPI_FullDuplex_ComPolling/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - SPI/SPI_FullDuplex_ComPolling/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - SPI/SPI_FullDuplex_ComPolling/Inc/main.h                  Header for main.c module  
  - SPI/SPI_FullDuplex_ComPolling/Src/stm32l4xx_it.c          Interrupt handlers
  - SPI/SPI_FullDuplex_ComPolling/Src/main.c                  Main program
  - SPI/SPI_FullDuplex_ComPolling/Src/system_stm32l4xx.c      stm32l4xx system source file
  - SPI/SPI_FullDuplex_ComPolling/Src/stm32l4xx_hal_msp.c     HAL MSP file

@par Hardware and Software environment

  - This example runs on STM32L432xx devices.

  - This example has been tested with NUCLEO-L432KC RevC board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-L432KC RevC Set-up
    - Connect Master board PA5 to Slave Board PA5 (Arduino A4)
    - Connect Master board PA6 to Slave Board PA6 (Arduino A5)
    - Connect Master board PA7 to Slave Board PA7 (Arduino A6)
    - Connect Master board GND  to Slave Board GND

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example

 */
 