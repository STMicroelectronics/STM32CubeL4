/**
  @page DCMI_CaptureMode DCMI Capture Mode example
  
  @verbatim
  ******************************************************************************
  * @file    DCMI/DCMI_CaptureMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the STM32L4xx DCMI_CaptureMode example.
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

How to use the DCMI to interface with a camera module to continuously capture
RGB565 images, crop them from size 320x240 to 240x240 then display the video
stream on the LCD.

The Digital camera interface (DCMI) is configured to receive the captured frame from 
the camera module connected to the Discovery board CN1 connector (for example,
STM32F4DIS-CAM camera module can be used). 
The DCMI IP is configured to crop the camera module frame set to QVGA 320x240 resolution 
to the FRIDA LCD format 240x240.

Each time the DCMI End of Frame event callback is raised, the picture is transferred to the LCD frame buffer through DMA2D.   


At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 80 MHz.



@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - DCMI/DCMI_CaptureMode/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - DCMI/DCMI_CaptureMode/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - DCMI/DCMI_CaptureMode/Inc/main.h                  Header for main.c module  
  - DCMI/DCMI_CaptureMode/Src/stm32l4xx_it.c          Interrupt handlers
  - DCMI/DCMI_CaptureMode/Src/main.c                  Main program
  - DCMI/DCMI_CaptureMode/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L496xx devices.
    
  - This example has been tested on a STM32L496G-Discovery board with a STM32F4DIS-CAM 
    camera module connected to the Discovery board CN1 connector. It can be
    easily tailored to any other supported device and development board. 


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 


 */
