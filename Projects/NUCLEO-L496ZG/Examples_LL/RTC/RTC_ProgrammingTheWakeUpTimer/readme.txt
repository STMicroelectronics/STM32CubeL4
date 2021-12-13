/**
  @page RTC_ProgrammingTheWakeUpTimer RTC example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/RTC/RTC_ProgrammingTheWakeUpTimer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RTC example.
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

Configuration of the RTC to use the WUT. The peripheral 
initialization uses LL unitary service functions for optimization purposes 
(performance and size).

In this example, after start-up, SYSCLK is configured to the max frequency using 
the PLL with MSI as clock source.
USART Peripheral is configured in asynchronous mode (115200 bauds, 8 data bit, 
1 start bit, 1 stop bit, no parity). No HW flow control is used.
GPIO associated to User push-button is linked with EXTI. 

Example execution:
   - Plug cable " USB to TTL 3V3 " (from USB to UART adapter)
   - Connect USB Adapter Rx to USART1 Tx(PA9)and USB Adapter to USART1 Rx(PA10)
   - Launch serial communication SW
   - Launch the program (LED1 should toggle at each WUT (every 1 second))
   - The time should be print on the interface
   - Press the user button to enter in the RTC init mode
   - Follow the step on the interface to change the RTC time

   @note LSI oscillator clock is used as RTC clock source by default.
      The user can use also LSE as RTC clock source. The user uncomment the adequate 
      line on the main.h file.
      @code
        #define RTC_CLOCK_SOURCE_LSI  
        /* #define RTC_CLOCK_SOURCE_LSE */
      @endcode
      LSI oscillator clock is delivered by a 32 kHz RC.
      LSE (when available on board) is delivered by a 32.768 kHz crystal.

- LED1 is toggling faster: This indicates that the system generates an error.

@par Directory contents 

  - RTC/RTC_ProgrammingTheWakeUpTimer/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - RTC/RTC_ProgrammingTheWakeUpTimer/Inc/main.h                  Header for main.c module
  - RTC/RTC_ProgrammingTheWakeUpTimer/Inc/stm32_assert.h          Template file to include assert_failed function
  - RTC/RTC_ProgrammingTheWakeUpTimer/Src/stm32l4xx_it.c          Interrupt handlers
  - RTC/RTC_ProgrammingTheWakeUpTimer/Src/main.c                  Main program
  - RTC/RTC_ProgrammingTheWakeUpTimer/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L496xx devices.
    
  - This example has been tested with NUCLEO-L496ZG (144 pins) board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-L496ZG (144 pins) Set-up
    Connect GPIOs connected to USART1 TX/RX (PA.09 and PA.10)
    to respectively RX and TX pins of PC UART (could be done through a USB to UART adapter).
      - USART1_TX  PA.09: connected to pin 21 of CN12 connector 
      - USART1_RX  PA.10: connected to pin 33 of CN12 connector 
    Connect GND between STM32 board and PC UART.

  - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration 
    (115200 bauds, 8 bits data, 1 stop bit, no parity, no HW flow control)
    

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
