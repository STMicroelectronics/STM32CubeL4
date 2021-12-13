/**
  @page CDC_Standalone USB Host Communication Class (CDC) application
  
  @verbatim
  ******************************************************************************
  * @file    USB_Host/CDC_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB Host CDC application.
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

Use of the USB host application based on the CDC class.

When a CDC Device is detected the CDC application is launched, it is based on 
the two CDC transfer directions and allows a dynamic serial configuration:
 - Transmission: in this configuration the user can select in the Host board, 
   using the menu, a file among the ones available on the SD and send
   it to the Device board. The content of the file could be visualized through the
   HyperTerminal (the link configuration is imposed initially by the Device
   and could be checked using the configuration menu). Data to be transmitted is
   stored in CDC_TX_Buffer buffer.

 - Reception: in reception mode the data entered by the user using the HyperTerminal 
   in ASCII format are transferred by the Device board to the Host board and displayed 
   on its LCD display. The CDC_RX_Buffer is the buffer used for Data Reception.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 80 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock, which is generated from an integrated PLL.

When the application is started, the connected USB CDC device is detected in CDC mode and gets 
initialized. The STM32 MCU behaves as a CDC Host, it enumerates the device and extracts VID, PID, 
manufacturer name, Serial no and product name information and displays it on the LCD screen.

A menu is displayed and the user can select any operation from the menu using the Joystick buttons:
 - "Send Data" operation starts the Data Transmission.
 - "Receive Data" operation starts the Data Reception.
 - "Configuration" operation defines the desired Host CDC configuration (Baudrate,Parity, DataBit and StopBit)
   The baudrate comes with a default value of 115,2 kbps (BAUDRATE = 115200).
 - "Re-Enumerate" operation performs a new Enumeration of the device.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about the STM32Cube USB Host library, please refer to UM1720  
"STM32Cube USB Host library".


@par USB Library Configuration

To select the appropriate USB Core to work with, user must add the following macro defines within the
compiler preprocessor (already done in the preconfigured projects provided with this application):
      - "USE_USB_HS" when using USB High Speed (HS) Core
      - "USE_USB_FS" when using USB Full Speed (FS) Core 

It is possible to fine tune needed USB Host features by modifying defines values in USBH configuration
file “usbh_conf.h” available under the project includes directory, in a way to fit the application
requirements, such as:
- Level of debug: USBH_DEBUG_LEVEL
                  0: No debug messages
                  1: Only User messages are shown
                  2: User and Error messages are shown
                  3: All messages and internal debug messages are shown
   By default debug messages are displayed on the debugger IO terminal; to redirect the Library
   messages on the LCD screen, lcd_log.c driver need to be added to the application sources.


@par Directory contents

  - USB_Host/CDC_Standalone/Src/main.c                  Main program
  - USB_Host/CDC_Standalone/Src/system_stm32l4xx.c      STM32L4xx system clock configuration file
  - USB_Host/CDC_Standalone/Src/stm32l4xx_it.c          Interrupt handlers
  - USB_Host/CDC_Standalone/Src/menu.c                  CDC State Machine
  - USB_Host/CDC_Standalone/Src/usbh_conf.c             General low level driver configuration
  - USB_Host/CDC_Standalone/Src/explorer.c              Explore the uSD content
  - USB_Host/CDC_Standalone/Src/cdc_configuration.c     CDC settings State Machine
  - USB_Host/CDC_Standalone/Src/cdc_receive.c           CDC Receive State Machine
  - USB_Host/CDC_Standalone/Src/cdc_send.c              CDC Send State Machine
  - USB_Host/CDC_Standalone/Inc/main.h                  Main program header file
  - USB_Host/CDC_Standalone/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - USB_Host/CDC_Standalone/Inc/lcd_log_conf.h          LCD log configuration file
  - USB_Host/CDC_Standalone/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - USB_Host/CDC_Standalone/Inc/usbh_conf.h             USB Host driver Configuration file
  - USB_Host/CDC_Standalone/Inc/ffconf.h                FAT file system module configuration file
  

@par Hardware and Software environment

  - This example runs on STM32L476xx devices.
    
  - This example has been tested with STMicroelectronics STM32L476G-EVAL RevA 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32L476G-EVAL RevA Set-up
    - Make sure that LCD Glass is in position IO
    - Plug the USB HID device into the STM32L476G-EVAL board through 'USB micro A-Male 
      to A-Female' cable to the connector CN1 (OTG-FS).
    - Make sure jumper JP1 is in connected in position 3V3 and JP19 is present.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 */
