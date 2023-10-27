/**
  @page FreeRTOS_Signal FreeRTOS Signal example
 
  @verbatim
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_Signal/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS Signal example.
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

How to perform thread signaling using CMSIS RTOS API.

This application creates three threads with the same priority.

Thread1 calls osSignalWait to wait for a signal that sets bit0, then toggles LED2

Thread2 calls osSignalWait to wait for a signal that sets bit1 and bit2, then toggles LED2

Thread3 performs the following actions:
  - calls osSetSignal to send a signal with bit0 to Thread1
  - delay for 500ms
  - calls osSetSignal to send a signal with bit1 and bit2 to Thread2
  - delay for 500ms

As a result, LED2 toggles every 500 ms.


@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in HAL time base ISR.
      This implies that if HAL_Delay() is called from a peripheral ISR process, then
      the HAL time base interrupt must have higher priority (numerically lower) than
      the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority()
      function.
 
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set accordingly to the 
      OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".


@par Directory contents
    - FreeRTOS/FreeRTOS_Signal/Src/main.c                Main program
    - FreeRTOS/FreeRTOS_Signal/Src/stm32l4xx_hal_timebase_tim.c HAL timebase file
    - FreeRTOS/FreeRTOS_Signal/Src/stm32l4xx_it.c        Interrupt handlers
    - FreeRTOS/FreeRTOS_Signal/Src/system_stm32l4xx.c    STM32L4xx system clock configuration file
    - FreeRTOS/FreeRTOS_Signal/Inc/main.h                Main program header file
    - FreeRTOS/FreeRTOS_Signal/Inc/stm32l4xx_hal_conf.h  HAL Library Configuration file
    - FreeRTOS/FreeRTOS_Signal/Inc/stm32l4xx_it.h        Interrupt handlers header file
    - FreeRTOS/FreeRTOS_Signal/Inc/FreeRTOSConfig.h      FreeRTOS Configuration file

@par Hardware and Software environment

  - This application runs on STM32L452xx devices.
    
  - This application has been tested with NUCLEO-L452RE Rev C board and can be
    easily tailored to any other supported device and development board.
    

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  
 */
