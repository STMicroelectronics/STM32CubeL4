/**
  @page HID_Standalone USB Device Human Interface (HID) example
  
  @verbatim
  ******************************************************************************
  * @file    USB_Device/HID_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB HID example.
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

Use of the USB device application based on the Human Interface (HID).

This example is part of the USB Device Library package using STM32Cube firmware.

This example supports the remote wakeup feature (the ability to bring the USB suspended bus back
to the active condition), and the Virtual Tamper button ( PB0 connected to GND) is used as the remote wakeup source. 
   
By default, in Windows OS, the USB mouse Power Management feature is turned off. This setting 
is different from classic PS/2 computer functionality. To enable the Wake up from  standby 
option, user has to manually turn on the Power Management feature for the USB mouse.

To enable the wake from standby option for the USB mouse, the following steps have to be followed: 
 - Start "Device Manager",
 - Select "Mice and other pointing devices",
 - Select the "HID-compliant mouse" device (make sure that PID &VID are equal to 0x5710 & 0x0483 respectively) 
 - Right click and select "Properties", 
 - Select "Power Management" tab,
 - Finally click to select "Allow this device to wake the computer" check box.
 
The function GetPointerData() is responsible to move the mouse cursor automatically,
which can be easily customized (square, circle...).
In this application the cursor is moving forward and backward in an infinite loop.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick. The user is 
provided with the SystemClock_Config()function to configure the clock (SYSCLK).
In this application the system clock is set to 48MHz.

The 48 MHz clock for the USB can be derived from one of the two following sources:
  – PLL clock(clocked by the HSE): If the USB uses the PLL as clock source.
  – RC48 Clock (HSI48): The HSI48 clock is generated from an internal 48 MHz RC oscillator, coupled with
    the clock recovery system (CRS). 
    The CRS allows automatic trimming of the internal HSI48, synchronized with USB SOF signal at 1KHz rate, 
    to guarantee its optimal accuracy over the whole device operational range.    
User can select USB clock from HSI48 or PLL through macro defined in main.h
(USE_USB_CLKSOURCE_CRSHSI48 and USE_USB_CLKSOURCE_PLL).

@note Care must be taken when using HAL_Delay(), this function provides accurate delay 
      (in milliseconds) based on variable incremented in SysTick ISR. This implies that 
       if HAL_Delay() is called from a peripheral ISR process, then the SysTick interrupt 
       must have higher priority (numerically lower) than the peripheral interrupt. 
       Otherwise the caller ISR process will be blocked. To change the SysTick interrupt 
       priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL configuration.
      
@note To reduce the example footprint, the toolchain dynamic allocation is replaced by a static allocation
      by returning the address of a pre-defined static buffer with the HID class structure size.   

@par Directory contents

  - USB_Device/HID_Standalone/Src/main.c                  Main program
  - USB_Device/HID_Standalone/Src/system_stm32l4xx.c      STM32L4xx system clock configuration file
  - USB_Device/HID_Standalone/Src/stm32l4xx_it.c          Interrupt handlers
  - USB_Device/HID_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/HID_Standalone/Src/usbd_desc.c             USB device HID descriptor
  - USB_Device/HID_Standalone/Inc/main.h                  Main program header file
  - USB_Device/HID_Standalone/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - USB_Device/HID_Standalone/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - USB_Device/HID_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/HID_Standalone/Inc/usbd_desc.h             USB device MSC descriptor header file


@par Hardware and Software environment

	- This example runs on STM32L432xx devices.
	- This demo has been tested with NUCLEO-L432KC RevC board and can be
    easily tailored to any other supported device and development board.
	- Since there is no USB 2.0 Full speed connector (Type B) on the nucleo board, user has to make 
    his own USB shield daughter board with the a USB connector and plug it on top of the  CN13 
    connectors of the STM32L432KC-Nucleo. The USB connector has to be connected to the USB device associated GPIOs
    as follows:
    - DP (D+ of the USB connector) <======> PA12 (Nucleo board)
    - DM (D- of the USB connector) <======> PA11 (Nucleo board)
    To improve EMC performance (noise immunity and signal integrity), it is recommended to connect a 100nF
	ceramic capacitor to the USB VDD pin.

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
