/**
  @page FatFs_USBDisk_Standalone   FatFs with USB disk drive application

   @verbatim
  ******************** (C) COPYRIGHT  2017 STMicroelectronics *******************
  * @file    FatFs/FatFs_USBDisk_Standalone/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FatFs with uSD/DMA card drive application.
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

How to use STM32Cube firmware with FatFs middleware component as a generic FAT
file system module and STM32 USB On-The-Go (OTG) host library, in both Full
Speed (HS) mode. This example develops an application
exploiting FatFs features, with USB disk drive configuration.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 120 MHz.


The 48 MHz clock for the USB FS can be derived from the following source:
    PLLSAI1 clock (clocked by the HSE): If the USB uses the PLLSAI1 as clock source,
    then the  VCO clock must be programmed  to output 288 MHz frequency, 
    and PLLSAI1Q divider set to 6 (USBCLK = PLL3VCO/PLL3Q).
           
The application is based on writing a text file to a drive, and it's performed 
using FatFs APIs to access the FAT volume as described in the following steps: 

 - Link the USB Host disk I/O driver;
 - Register the file system object (mount) to the FatFs module for the USB drive;
 - Create and Open new text file object with write access;
 - Write data to the text file;
 - Close the open text file;
 - Unlink the USB Host disk I/O driver.
 
It is worth noting that the application manages any error occurred during the 
access to FAT volume, when using FatFs APIs. Otherwise, user can check if the
written text file is available on the USB disk.

It is possible to fine tune needed FatFs features by modifying defines values 
in FatFs configuration file “ffconf.h” available under the project includes 
directory, in a way to fit the application requirements. 

STM32L4R9I-EVAL's LED can be used to monitor the application status:
  - LED1 is ON when the application runs successfully.
  - LED3 is ON when any error occurs.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents
 
  - FatFs/FatFs_USBDisk_Standalone/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - FatFs/FatFs_USBDisk_Standalone/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - FatFs/FatFs_USBDisk_Standalone/Inc/main.h                  Main program header file
  - FatFs/FatFs_USBDisk_Standalone/Inc/ffconf.h                FAT file system module configuration file   
  - FatFs/FatFs_USBDisk_Standalone/Src/usbh_conf.h             usb otg low level configuration header file
  - FatFs/FatFs_USBDisk_Standalone/Src/usbh_diskio.h           FatFs diskio driver header file
  - FatFs/FatFs_USBDisk_Standalone/Src/stm32l4xx_it.c          Interrupt handlers
  - FatFs/FatFs_USBDisk_Standalone/Src/main.c                  Main program
  - FatFs/FatFs_USBDisk_Standalone/Src/usbh_conf.c             usb otg low level configuration file
  - FatFs/FatFs_USBDisk_Standalone/Src/usbh_diskio.c           FatFs usb diskio driver
  - FatFs/FatFs_USBDisk_Standalone/Src/system_stm32l4xx.c      stm32l4xx system clock configuration file
         
 
@par Hardware and Software environment
  - This application runs on STM32L4R9xx devices.

  - This application has been tested with STMicroelectronics STM32L4R9I-EVAL
    evaluation boards and can be easily tailored to any other supported device
    and development board.

  - STM32L4R9I-EVAL Set-up
    - Plug the USB key into the STM32L4R9I-EVAL board through 'USB micro A-Male 
      to A-Female' cable to the USB Full Speed (FS) connector CN3
    

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application

 @note
 the USB flash disk gets formatted by the application, please make sure to backup any relevant data before using it.
 */
 
