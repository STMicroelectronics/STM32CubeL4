/**
  @page BLE P2P_LedButton  BLE P2P_LedButton application

  @verbatim
  ******************************************************************************
  * @file    BLE/P2P_LedButton/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BLE Point-to-Point communication application
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

This example aims at demonstrating point-to-point communications using the BLE component.

Two STM32 B-L475E-IOT01A Discovery boards are used: one acting as GATT client,  
the other as GATT server.
The two boards needs to be compiled with different code by changing the #definitions in config.h
  #define LB_CLIENT  0/1    /*1 = Device is GAP Central & GATT Client */
  #define LB_SERVER  1/0    /*1 = Device is GAP Peripheral & GATT Server */

Once the code is downloaded on the two STM32 B-L475E-IOT01A Discovery boards and executed,
the modules are initialized.  The slave device (GAP Peripheral) starts advertising,
the master device (GAP Central) scans and automatically connects to the slave at power up.
ST proprietary Led Button service is implemented on the slave device with Led Characteristics 
& Button Characteristics (GATT Server).
Once connected, the Master Device(GATT Client) start to discover the remote services and Characteristics
The above phase takes some seconds, after that period when pressing the User button 
on one board the service allows toggling the Led on the other board.
The User button can be pressed independently on the GATT Client or on the GATT Server.

To summarise:
  *1*  User presses User Button on GATT Server
  *2*  Server  ----------- GATT Notification ------>   Client
  *3*    LED LD1 Toggles on GATT Client
or
  *1*  User presses User Button on GATT Client
  *2*  Client  ----------- GATT Write command ------>   Server
  *3*    LED LD1 Toggles on GATT Server


@par Directory contents

 - BLE/P2P_LedButton/Src/main.c                 Main Program
 - BLE/P2P_LedButton/Src/system_stm32l4xx.c     STM32L4xx system clock configuration file
 - BLE/P2P_LedButton/Src/stm32l4xx_it.c         STM32 interrupt handlers
 - BLE/P2P_LedButton/Src/lb_demo.c              P2P main Led Button application
 - BLE/P2P_LedButton/Src/lb_client_app.c        P2P Client Led Button application
 - BLE/P2P_LedButton/Src/lb_server_app.c        P2P Server Led Button application
 - BLE/P2P_LedButton/Inc/config.h               Application configuration file
 - BLE/P2P_LedButton/Inc/stm32l4xx_hal_conf.h   HAL configuration file
 - BLE/P2P_LedButton/Inc/stm32l4xx_it.h         STM32 interrupt handlers header file
 - BLE/P2P_LedButton/Inc/lb_demo.h              Exported files and function of lb_demo.c 
 - BLE/P2P_LedButton/Inc/lb_client_app.h        Exported files and function of lb_client_app.c 
 - BLE/P2P_LedButton/Inc/lb_server_app.h        Exported files and function of lb_server_app.c 
 - BLE/Common/                                  These files are Common also to other applic/examples then P2P_LedButton (e.g. to HeartRate)
 - BLE/Common/ble_core/*.*                      BlueNRG-MS Bluetooth Low Energy device driver 
 - BLE/Common/ble_services/*.*                  GATT services according to BLE spec
 - BLE/Common/debug/*.*                         Code usefull for debugging (DBG should be activated in config.h)
 - BLE/Common/tl/*.*                            HCI layer and SPI protocol    
 - BLE/Common/hw/*.*                            HW board and device dependencies (SPI pins, timeserver, low power) 
 - BLE/Common/utilities/*.*                     Scheduler, memory/queue/fifo/list management, otp, etc   

@par Hardware and Software environment

  - This application runs on STM32L475XX devices, B-L475E-IOT01A board (MB1297) 
    
  - This application has been tested with the following environment:
     - B-L475E-IOT01A board. 

  - B-L475E-IOT01A Discovery boards Set-up    
    - Connect the Discovery boards to your PC with a USB cable type A to mini-B 
      to ST-LINK connector (CN1).
    - Please ensure that the ST-LINK connectors and jumpers are fitted.


@par How to use it ? 
   
In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
 
 */
