/**
  @page WriteTag Readme file
 
  @verbatim
  ******************************************************************************
  * @file    NFC/WriteTag/readme.txt 
  * @author  MCD Application Team
  * @brief   This application writes NDEF messages on ST25DV-I2C type 5 NFC tag.
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

This application aims at showing how to write NDEF messages to an ST25DV-I2C type 5 NFC tag 
so that the associated application is launched on the smartphone when it comes near the NFC antenna.

First the NFC component is initialised, the LED2 starts blinking to indicate Initialization is terminated.
Then when the smartphone approches the NFC antenna and action will be taken by the smartphone 
depending on the NDEF message written on the tag.
This example writes a different message each time the USER BUTTON is pressed:
 - URI:    the "www.st.com" website is opened by the browser
 - SMS:    an SMS is prepeaed on the smartphone and ready to be sent to +33612345678
 - EMAIL:  an email is prepeaed on the smartphone and ready to be sent to customer.service@st.com
 - VCARD: a vCard content from VcardStruct is shown on the smartphone display
 - AAR:    the STM BLE application is opened on the smartphone (downloaded the first time)

@note Each time the USER BUTTON is pressed, the LED2 blinking frequency change to indicate that a new
      NDEF message has been successfully written to the ST25DV-I2C NFC Dynamic tag.

@note The ST25DV-I2C NFC Dynamic Tag can only communicate through its NFC or I2C interface at a given time.
      As a result if the USER BUTTON is pressed while the smartphone is accessing the ST25DV-I2C,
      the I2C communication could fail. If such error occurs, the LED1 is switched ON.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - NFC/WriteTag/Src/main.c                Main program file with several NDEF examples.
  - NFC/WriteTag/Src/system_stm32l4xx.c    STM32L4xx system clock configuration file.
  - NFC/WriteTag/Src/stm32l4xx_it.c        STM32 interrupt handlers (USER button).
  - NFC/WriteTag/Src/lib_NDEF_config.c     lib_nfc interface file implementation.
  - NFC/WriteTag/Inc/stm32l4xx_hal_conf.h  HAL configuration file.
  - NFC/WriteTag/Inc/stm32l4xx_it.h        STM32 interrupt handlers header file.
  - NFC/Common/                            These files are Common also to other applic/examples then WriteTag.
  - NFC/Common/lib_nfc/                    This directory contains a set of source files that write NDEF messages
                                           to be exchanged with smartphone devices.
 
@par Hardware and Software environment 

  - This application runs on STM32L4S5XX devices, B-L4S5I-IOT01A board (MB1297) 
    
  - This application has been tested with the following environment:
     - B-L4S5I-IOT01A board. 

  - B-L4S5I-IOT01A Discovery board Set-up    
    - Connect the Discovery board to your PC with a USB cable type A to mini-B 
      to ST-LINK connector (CN1).
    - Please ensure that the ST-LINK connectors and jumpers are fitted.

  - When using Android Phone
    - Activate "NFC"
    - Default Android application should handle the reading of the NFC tag

@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
 
 */
