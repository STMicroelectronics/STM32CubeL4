/**
  @page DCMI_CaptureMode : DCMI example with continuous capture mode and DMA transfer size larger than 65535

  @verbatim
  ******************************************************************************
  * @file    DCMI/DCMI_CaptureMode/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the DCMI continuous capture mode with large DMA transfer size
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

@par Example Description

How to use the DCMI peripheral when capturing a large image (size larger than 65535 pixels) and display it on LCD in DSI Mode Command.

The goal of this example is to achieve a continuous capture of RGB565 images thanks to a camera, 
crop them on the fly to a size 390x390 size and display them on LCD in DSI Command mode.

The camera is initialized in QVGA 640x480 mode and DCMI peripheral is configured in continuous capture mode.
DCMI cropping feature is enabled to crop the resulting images to a 390x390-pixel size in order to fit the Discovery board LCD size.


Transfer from DCMI peripheral to memory requires specific care.
Total image size is 390 * 390 * 16 bit/pixels = 76050 32-bit words per image.
The DMA peripheral embedded in STM32L4R9 is only able to carry out DMA transfers with sizes up to 65535.

This is why the DCMI transfer the complete images as three chunks of 25350 words each.
The DMA associated with the DCMI is configured in circular mode and each time a chunk is received
from the DCMI, it is copied by another DMA channel at the final memory expected location.

Each time an image chunk is copied, LED1 toggles.
When a complete image has been transferred, the LCD screen is refreshed by the DSI and LED2 toggles.


@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

  - DCMI/DCMI_CaptureMode/Inc/stm32l4xx_hal_conf.h      HAL configuration file
  - DCMI/DCMI_CaptureMode/Inc/stm32l4xx_it.h            Interrupt handlers header file
  - DCMI/DCMI_CaptureMode/Inc/main.h                    Header for main.c module
  - DCMI/DCMI_CaptureMode/Src/stm32l4xx_it.c            Interrupt handlers
  - DCMI/DCMI_CaptureMode/Src/main.c                    Main program
  - DCMI/DCMI_CaptureMode/Src/stm32l4xx_hal_msp.c       HAL MSP file
  - DCMI/DCMI_CaptureMode/Src/system_stm32l4xx.c        STM32L4xx system source file
  - DCMI/DCMI_CaptureMode/Src/stm32l4r9i_discovery_camera.c    Ad-hoc camera BSP file 

@par Hardware and Software environment

  - This example runs on STM32L4R9xx devices.

  - This example has been tested with STMicroelectronics STM32L4R9I-Discovery
    board supporting the 1.2" 390x390 pixel AMOLED round display plugged on CN4. 
    The camera module DM-CAM130 plugged to CN2 connector is used to capture the image.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
