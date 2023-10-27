/**
  @page WiFi_HTTP_Server es WiFi module in Web server demonstration

  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of the Es_WIFI Module in Web server mode.
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

This application shows how to make HTTP requests using the Es-WiFi module based
on STM32Cube HAL.

In this example, you will use Es_WiFi module and a web Browser to create a simple
Web server.

After opening a browser and navigating to es-WiFi shield's IP address, the
B-L4S5I-IOT01A board will be able to answer a HTTP request with es-WiFI shield.

An HTTP client sends a request message to an HTTP server.

This application may be used with a Smartphone (more detailed setup
instructions are described in section "Hardware and Software environment").

The UART is used to output a message on terminal emulator (#define TERMINAL_USE in
main.c). Log messages will be displayed on terminal to inform user about
es_wifi module status

The communication is done with a web browser application in a SmartPhone.

This HTTP server contains a html page:
  + LED control: allows to control the Green Led LED2 located in the
    B-L4S5I-IOT01A board
  + sensor conversion: it shows the temperature sensor converted value

@par Directory contents
 - WiFi/WiFi_HTTP_Server/Inc/main.h                   header for main.c module
 - WiFi/WiFi_HTTP_Server/Inc/stm32l4xx_hal_conf.h     HAL configuration file
 - WiFi/WiFi_HTTP_Server/Inc/stm32l4xx_it.h           STM32 interrupt handlers header file
 - WiFi/WiFi_HTTP_Server/Src/main.c                   Main Program
 - WiFi/WiFi_HTTP_Server/Src/flash_l4.c               Flash management (for storing configuration)
 - WiFi/WiFi_HTTP_Server/Src/system_stm32l4xx.c       STM32L4xx system clock configuration file
 - WiFi/WiFi_HTTP_Server/Src/stm32l4xx_it.c           STM32 interrupt handlers
 - WiFi/Common/Src/es_wifi.c                          Implementation of the ES_WIFI_XXX() API.
 - WiFi/Common/Src/es_wifi_io.c                       Implementation of the ES_WIFI_IO_XXX() API.
 - WiFi/Common/Src/wifi.c                             Implementation of the WIFI_XXX() API.
 - WiFi/Common/Inc/es_wifi.h                          Header for the functions and defines used by the es_wifi.c
 - WiFi/Common/Inc/es_wifi_io.h                       Header for the functions and defines used by the es_wifi_io.c
 - WiFi/Common/Inc/wifi.h                             Header for the functions and defines used by the wifi.c

@par Hardware and Software environment

  - This application has been tested with the following environment:
     - B-L4S5I-IOT01A Rev-D board.

  - Connect the board ST-Link USB port to a PC USB port

  - On the PC, start a Terminal Emulator software (example: Tera Term).
    Terminal configuration:
    - Data Length = 8 Bits
    - One Stop Bit
    - Parity None
    - BaudRate = 115200 baud
    - Flow control: None
    A virtual COM port will then appear in the Terminal.

  - Start a WiFi Access Point on a smartphone or use a WiFi router

  - On virtual COM port enter the SSID and PASSWORD with your own AP settings.
    The WiFi configuration settings are stored in Flash memory.
    They will be retrieved from Flash memory the next time the board is reset.
    If you need to change the stored settings, during start the application proposes
    to change them by pressing the board's blue button ("User" button).

  - Open a Web browser on a PC or a smartphone. Enter the board's es-wifi IP address displayed on virtual COM port.
    Example: http://192.168.1.3

  - The web server sends an HTML page with temperature reading and button to switch the board LED ON/OFF .

  - Click on "Submit" to send the changes and read temperature again.

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Check that DK board is connected to a remote PC before running the application
 - Run the application

 */
