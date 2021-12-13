/**
  @page FatFs_uSD_Standalone   FatFs with uSD card drive application

  @verbatim
  ******************************************************************************
  * @file    FatFs/FatFs_uSD_Standalone/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FatFs with uSD card drive application.
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
file system module. This example develops an application that exploits FatFs
features to configure a microSD drive.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 80 MHz.

 - Link the uSD disk I/O driver;
 - Register the file system object (mount) to the FatFs module for the uSD drive;
 - Create a FAT file system (format) on the uSD drive;
 - Create and Open new text file object with write access;
 - Write data to the text file;
 - Close the open text file;
 - Open text file object with read access;
 - Read back data from the text file;
 - Close the open text file;
 - Check on read data from text file;
 - Unlink the uSD disk I/O driver.

It is worth noting that the application manages any error occurred during the
access to FAT volume, when using FatFs APIs. Otherwise, user can check if the
written text file is available on the uSD card.

It is possible to fine tune needed FatFs features by modifying defines values
in FatFs configuration file “ffconf.h” available under the project includes
directory, in a way to fit the application requirements.

It is possible to dynamically hot plug/unplug the uSD card, as the application will
handle the plug/unplug events.

@note: for some uSD's, replacing it  while the application is running makes the application
       fail. It is recommended to reset the board using the "Reset button" after replacing
       the uSD.

@note the Application, by default, formats the uSD and erases all the data, it is possible to avoid that
      by defining the flag "FATFS_MKFS_ALLOWED" to "0" in the main.c file.

STM32 Discovery board's LEDs can be used to monitor the application status:
  - Green LED, LED2, is ON when the application runs successfully.
  - Orange LED, LED1, is ON when any error occurs.

@par Directory contents

  - FatFs/FatFs_uSD_Standalone/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - FatFs/FatFs_uSD_Standalone/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - FatFs/FatFs_uSD_Standalone/Inc/main.h                  Main program header file
  - FatFs/FatFs_uSD_Standalone/Src/sd_diskio.h             uSD diskio header file
  - FatFs/FatFs_uSD_Standalone/Inc/ffconf.h                FAT file system module configuration file
  - FatFs/FatFs_uSD_Standalone/Src/stm32l4xx_it.c          Interrupt handlers
  - FatFs/FatFs_uSD_Standalone/Src/main.c                  Main program
  - FatFs/FatFs_uSD_Standalone/Src/sd_diskio.c             FatFs uSD diskio driver
  - FatFs/FatFs_uSD_Standalone/Src/system_stm32l4xx.c      stm32l4xx system clock configuration file


@par Hardware and Software environment

  - This application runs on STM32L4R9xx devices.

  - This application has been tested with STMicroelectronics STM32L4R9I-DISCO
    evaluation boards and can be easily tailored to any other supported device
    and development board.

  - STM32L4R9I-DISCO Set-up
    - Connect a uSD Card to the MSD connector (CN6).

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application

 */
