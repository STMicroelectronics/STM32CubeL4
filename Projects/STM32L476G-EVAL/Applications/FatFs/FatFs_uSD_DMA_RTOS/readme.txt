/**
  @page FatFs_uSD_DMA_RTOS   FatFs with uSD card drive in RTOS mode application

  @verbatim
  ******************** (C) COPYRIGHT  2016 STMicroelectronics *******************
  * @file    FatFs/FatFs_uSD_DMA_RTOS/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FatFs with uSD/DMA card drive in RTOS mode application
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
file system module. This example develops an application exploiting FatFs
features, with a microSD drive in RTOS mode configuration.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 80 MHz.

The application is based on writing and reading back a text file from a drive,
it creates a normal priority thread and it's performed using FatFs APIs to
access the FAT volume as described in the following steps:

 - Link the uSD/DMA disk I/O driver;
 - Register the file system object (mount) to the FatFs module for the uSD drive;
 - Create a FAT file system (format) on the uSD drive;
 - Create and Open new text file object with write access;
 - Write data to the text file;
 - Close the open text file;
 - Open text file object with read access;
 - Read back data from the text file;
 - Check on read data from text file;
 - Close the open text file;
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


STM32L476G-EVAL's LED can be used to monitor the application status:
  - LED1 is ON when the application runs successfully.
  - LED2 is ON when any error occurs.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set according to
      the OS resources memory requirements of the application with +10% margin and rounded to the
      upper Kbyte boundary.

@note the Application, by default, formats the uSD and erases all the data, it is possible to avoid that
      by defining the flag "FATFS_MKFS_ALLOWED" to "0" in the main.c file.

@par Keywords

Middleware, SD Card, FatFs, File system, FAT Volume, Format, Mount, Read, Write,FreeRTOS, RTOS

@par Directory contents

  - FatFs/FatFs_uSD_DMA_RTOS/Inc/stm32l4xx_hal_conf.h                            HAL configuration file
  - FatFs/FatFs_uSD_DMA_RTOS/Inc/stm32l4xx_it.h                                  Interrupt handlers header file
  - FatFs/FatFs_uSD_DMA_RTOS/Inc/main.h                                          Main program header file
  - FatFs/FatFs_uSD_DMA_RTOS/Inc/ffconf.h                                        FAT file system module configuration file
  - FatFs/FatFs_uSD_DMA_RTOS/Inc/FreeRTOSConfig.h                                FreeRTOS configuration file
  - FatFs/FatFs_uSD_DMA_RTOS/Inc/sd_diskio_dma_rtos.h                            uSD/DMA diskio driver header file
  - FatFs/FatFs_uSD_DMA_RTOS/Src/stm32l4xx_it.c                                  Interrupt handlers
  - FatFs/FatFs_uSD_DMA_RTOS/Src/main.c                                          Main program
  - FatFs/FatFs_uSD_DMA_RTOS/Src/system_stm32l4xx.c                              stm32l4xx system clock configuration file
  - FatFs/FatFs_uSD_DMA_RTOS/Src/stm32l4xx_hal_timebase_tim.c                    HAL time base functions.
  - FatFs/FatFs_uSD_DMA_RTOS/Src/sd_diskio_dma_rtos.c                            the uSD/DMA FatFs diskio driver



@par Hardware and Software environment

  - This application runs on STM32L4xx devices.

  - This application has been tested with STMicroelectronics STM32L476G-EVAL
    evaluation boards and can be easily tailored to any other supported device
    and development board.

  -STM32L476G-EVAL Set-up
    - Connect a uSD Card to the MSD connector (CN18).

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application

 */
