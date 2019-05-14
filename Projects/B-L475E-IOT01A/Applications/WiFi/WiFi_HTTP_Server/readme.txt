/**
  @page WiFi_HTTP_Server es WiFi module in Web server demonstration
  
  @verbatim
  ******************************************************************************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Es_WIFI Module in Web server mode.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        http://www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Application Description 

This application shows how to make HTTP requests using the Es-WiFi module based 
on STM32Cube HAL. 

In this example, you will use Es_WiFi module and a web Browser to create a simple 
Web server. 

After opening a browser and navigating to es-WiFi shield's IP address, the 
B_L475E-IOT01ax board will be able to answer a HTTP request with es-WiFI shield. 

An HTTP client sends a request message to an HTTP server.
     
This application may be used with a Smartphone (more detailed setup
instructions are described in section "Hardware and Software environment").

If the USART is used to output a message on hyperterminal(#define TERMINAL_USE in 
main.c), log messages will be displayed on Hyperterminal to inform user about 
es_wifi module status 

The communication is done with a web browser application in a SmartPhone.

This HTTP server contains a html page:
  + LED control: allows to control the Green Led LED2 located in the 
    B_L475E-IOT01ax board
  + ADC conversion: it shows the temperature sensor converted value

@par Directory contents
 - WiFi/WiFi_HTTP_Server/Inc/main.h                   header for main.c module
 - WiFi/WiFi_HTTP_Server/Inc/stm32l4xx_hal_conf.h     HAL configuration file
 - WiFi/WiFi_HTTP_Server/Inc/stm32l4xx_it.h           STM32 interrupt handlers header file
 - WiFi/WiFi_HTTP_Server/Src/main.c                   Main Program
 - WiFi/WiFi_HTTP_Server/Src/system_stm32l4xx.c       STM32L4xx system clock configuration file
 - WiFi/WiFi_HTTP_Server/Src/stm32l4xx_it.c           STM32 interrupt handlers
 - WiFi/Common/Src/es_wifi.c                          Implementation of the ES_WIFI_XXX() API.
 - WiFi/Common/Src/es_wifi_io.c                       Implementation of the ES_WIFI_IO_XXX() API.
 - WiFi/Common/Src/wifi.c                             Implementation of the WIFI_XXX() API.
 - WiFi/Common/Inc/es_wifi.h                          Header for the functions and defines used by the es_wifi.c 
 - WiFi/Common/Inc/es_wifi_io.h                       Header for the functions and defines used by the es_wifi_io.c   
 - WiFi/Common/Inc/wifi.h                             Header for the functions and defines used by the wifi.c   

@par Hardware and Software environment

  - This application runs on STM32L475XX devices.
    
  - This application has been tested with the following environment:
     - B-L475E-IOT01A Rev-C board. 
 
 - Connect ST-Link cable to the PC USB port to display data on the HyperTerminal. 
    A virtual COM port will then appear in the HyperTerminal. 

  - Hyperterminal configuration:
    - Data Length = 7 Bits
    - One Stop Bit
    - Parity None
    - BaudRate = 115200 baud
    - Flow control: None 
	  
  - When using Android Phone
    - Configure your phone as Access Point
    - Update in main.c the following defines SSID and PASSWORD with your own AP settings
    - Open a Web browser
    - Put the es-wifi IP address displayed on the terminal in the web browser
    
In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Check that DK board is connected to a remote PC before running the application
 - Run the application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
