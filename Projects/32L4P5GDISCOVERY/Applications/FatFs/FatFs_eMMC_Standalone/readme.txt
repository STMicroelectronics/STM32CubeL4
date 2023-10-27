/**
  @page FatFs_eMMC_Standalone   FatFs with eMMC card drive application and access times measurements

  @verbatim
  ******************************************************************************
  * @file    FatFs/FatFs_eMMC_Standalone/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FatFs with eMMC card drive application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
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
features to configure a eMMC drive.
Once done, several transfers of varying sizes are achieved to write and read
date on the eMMC drive and measure the access times.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 120 MHz.
Timer TIM2 is enabled to use its 32-bit long counter in order to measure eMMC
access times.

 - Link the eMMC disk I/O driver;
 - Register the file system object (mount) to the FatFs module for the eMMC drive;
 - Create a FAT file system (format) on the eMMC drive;
 - Create and Open new text file object with write access;
 - Write data to the text file;
 - Close the open text file;
 - Open text file object with read access;
 - Read back data from the text file;
 - Close the open text file;
 - Check on read data from text file;

The last operations are repeated in copying varying sized chunks of SRAM to the
eMMC (from 256 bytes to 256 K-bytes).
TIM2 counter is started before and stopped after any access to the eMMC to
measure the read or write access times.

- When resorting to IAR Embedded Workbench IDE, access times are displayed on
  debugger Terminal I/O as follows: View --> Terminal I/O.

- When resorting to MDK-ARM KEIL IDE, plain data, access times are displayed on
  debugger as follows:  View --> Serial Viewer --> Debug (printf) Viewer.

- When resorting to STM32CubeIDE , plain data, decrypted data are displayed on 
 debugger as follows: Window--> Show View--> Console.
 In Debug configuration : 
 1- Use ST-LINK (OpenOCD).
 2- Add the command "monitor arm semihosting enable" into startup Initialization Commands 


It is worth noting that the application manages any error occurred during the
access to FAT volume, when using FatFs APIs. Otherwise, user can check if the
written text file is available on the eMMC card.

It is possible to fine tune needed FatFs features by modifying defines values
in FatFs configuration file "ffconf.h" available under the project includes
directory, in a way to fit the application requirements.



@note the Application, by default, formats the eMMC and erases all the data, it is possible to avoid that
      by defining the flag "FATFS_MKFS_ALLOWED" to "0" in the main.c file.

STM32 Discovery board's LEDs can be used to monitor the application status:
  - Green LED, LED1, is ON when the application runs successfully.
  - Blue LED, LED2, is ON when any error occurs.

@par Directory contents

  - FatFs/FatFs_eMMC_Standalone/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - FatFs/FatFs_eMMC_Standalone/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - FatFs/FatFs_eMMC_Standalone/Inc/main.h                  Main program header file
  - FatFs/FatFs_eMMC_Standalone/Src/mmc_diskio.h            eMMC diskio header file
  - FatFs/FatFs_eMMC_Standalone/Inc/ffconf.h                FAT file system module configuration file
  - FatFs/FatFs_eMMC_Standalone/Src/stm32l4xx_it.c          Interrupt handlers
  - FatFs/FatFs_eMMC_Standalone/Src/main.c                  Main program
  - FatFs/FatFs_eMMC_Standalone/Src/mmc_diskio.c            FatFs eMMC diskio driver
  - FatFs/FatFs_eMMC_Standalone/Src/system_stm32l4xx.c      stm32l4xx system clock configuration file


@par Hardware and Software environment

  - This application runs on STM32L4P5xx devices.

  - This application has been tested with STMicroelectronics STM32L4P5G-DISCO
    evaluation board (MB1535) and can be easily tailored to any other supported device
    and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application

 */
