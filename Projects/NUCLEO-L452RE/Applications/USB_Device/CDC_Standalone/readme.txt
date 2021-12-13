/**
  @page CDC_Standalone USB Device Communication (CDC) application
  
  @verbatim
  ******************************************************************************
  * @file    USB_Device/CDC_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB Device CDC application.
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

@par Application Description 

Use of the USB device application based on the Device Communication Class (CDC) and 
following the PSTN subprotocol. This application uses the USB Device and UART peripherals.

This example is part of the USB Device Library package using STM32Cube firmware.

This is a typical application on how to use the STM32L452xx USB Device peripheral where the STM32 MCU
behaves as a USB-to-UART bridge following the Virtual COM Port (VCP) implementation.
 - On one side, the STM32 exchanges data with a PC host through USB interface in Device mode.
 - On the other side, the STM32 exchanges data with other devices (same host, other host,
   other devices…).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick. The user is 
provided with the SystemClock_Config()function to configure the clock (SYSCLK).
In this application the system clock is set to 48MHz.

When the VCP application is started, the STM32 MCU is enumerated as serial communication port and is
configured in the same way (baudrate, data format, parity, stop bit) as it would configure a standard 
COM port. The 7-bit data length with no parity control is NOT supported.

During enumeration phase, three communication pipes "endpoints" are declared in the CDC class
implementation (PSTN sub-class):
 - 1 x Bulk IN endpoint for receiving data from STM32 device to PC host:
   When data are received over UART they are saved in the buffer "UserTxBuffer". Periodically, in a 
   timer callback the state of the buffer "UserTxBuffer" is checked. If there are available data, they
   are transmitted in response to IN token otherwise it is NAKed.
   The polling period depends on "CDC_POLLING_INTERVAL" value.
    
 - 1 x Bulk OUT endpoint for transmitting data from PC host to STM32 device:
   When data are received through this endpoint they are saved in the buffer "UserRxBuffer" then they
   are transmitted over UART using interrupt mode and in meanwhile the OUT endpoint is NAKed.
   Once the transmission is over, the OUT endpoint is prepared to receive next packet in
   HAL_UART_TxCpltCallback().
    
 - 1 x Interrupt IN endpoint for setting and getting serial-port parameters:
   When control setup is received, the corresponding request is executed in CDC_Itf_Control().
   In this application, two requests are implemented:
    - Set line: Set the bit rate, number of Stop bits, parity, and number of data bits 
    - Get line: Get the bit rate, number of Stop bits, parity, and number of data bits
   The other requests (send break, control line state) are not implemented.

@note Receiving data over UART is handled by interrupt while transmitting is handled by DMA allowing
      hence the application to receive data at the same time it is transmitting another data (full- 
      duplex feature).

The support of the VCP interface is managed through the ST Virtual COM Port driver available for 
download from www.st.com.

@note The user has to check the list of the COM ports in Device Manager to find out the number of the
      COM ports that have been assigned (by OS) to the VCP interface.

This application uses UART as a communication interface. The UART instance and associated resources
(GPIO, NVIC) can be tailored in "usbd_cdc_interface.h" header file according to your hardware 
configuration. Moreover, this application can be customized to communicate with interfaces other than UART.
For that purpose a template CDC interface is provided in: 
Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src directory.

To run this application, the user can use the following configuration:

   Connect USB cable to Host and connect UART TX pin to UART RX pin on the NUCLEO-L452RE board
   (Loopback mode). In this case, you can open one terminal (relative to USB com port)
   and all data sent from this terminal will be received by the same terminal in loopback mode.
   This mode is useful for test and performance measurements.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
For more details about the STM32Cube USB Device library, please refer to UM1734 
"STM32Cube USB Device library".
      
@par Directory contents 

  - USB_Device/CDC_Standalone/Src/main.c                  Main program
  - USB_Device/CDC_Standalone/Src/system_stm32l4xx.c      STM32L4xx system clock configuration file
  - USB_Device/CDC_Standalone/Src/stm32l4xx_it.c          Interrupt handlers
  - USB_Device/CDC_Standalone/Src/stm32l4xx_hal_msp.c     HAL MSP module
  - USB_Device/CDC_Standalone/Src/usbd_cdc_interface.c    USBD CDC interface
  - USB_Device/CDC_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/CDC_Standalone/Src/usbd_desc.c             USB device CDC descriptor
  - USB_Device/CDC_Standalone/Inc/main.h                  Main program header file
  - USB_Device/CDC_Standalone/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - USB_Device/CDC_Standalone/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - USB_Device/CDC_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/CDC_Standalone/Inc/usbd_desc.h             USB device CDC descriptor header file
  - USB_Device/CDC_Standalone/Inc/usbd_cdc_interface.h    USBD CDC interface header file  


@par Hardware and Software environment

  - This example runs on STM32L452xx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-L452RE 
    board and can be easily tailored to any other supported device and development board.
	
  - NUCLEO-L452RE Set-up
    Since there is no USB 2.0 Full speed connector (Type B) on the nucleo board, user has to make 
    his own USB shield daughter board with the a USB connector and plug it on top of the  CN10 
    connectors of the STM32L452RE-Nucleo. The USB connector has to be connected to the USB device associated GPIOs
    as follows:
    - DP (D+ of the USB connector) <======> PA12 (Nucleo board)
    - DM (D- of the USB connector) <======> PA11 (Nucleo board)
    To improve EMC performance (noise immunity and signal integrity), it is recommended to connect a 100nF
	ceramic capacitor to the USB VDD pin.
    - For loopback mode test: connect directly USART2 TX and RX pins,
	The USART2 interface available on PA2 and PA3 of the microcontroller can be
	connected to ST-LINK MCU. The choice can be changed by setting the related solder bridges. 
	By default the USART2 communication between the target MCU and ST-LINK MCU is enabled.

  - Hyperterminal configuration:
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - Stop Bit = 1
    - Parity = None
    - Flow control = None 


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
 - Install the USB virtual COM port driver
 - Find out the number of the COM port assigned to the STM32 CDC device
 - Open a serial terminal application and start the communication

 */
