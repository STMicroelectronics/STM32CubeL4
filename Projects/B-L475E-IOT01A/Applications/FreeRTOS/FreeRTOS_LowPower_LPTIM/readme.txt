/**
  @page FreeRTOS_LowPower_LPTIM FreeRTOS Low Power LPTIM example
 
  @verbatim
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_LowPower_LPTIM/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS low power example.
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

How to enter and exit low-power mode with CMSIS RTOS API with LPTIM used as clock source
for both RTOS and HAL ticks.

This example creates one thread that toggle a LED then blocks for 1 sec . Blocking for a 
finite period allows the kernel to stop the tick interrupt and place the STM32 into Stop mode.

In this example, non-used GPIOs are configured to analog, thus helping to reduce 
the power consumption of the device.

The RTOS tick is suppressed while the MCU is in the low power state.
The LPTIM is used as clock source for both RTOS and HAL ticks.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set according 
      to the OS resources memory requirements of the application with +10% margin and rounded to 
	  the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".


@par Directory contents
    - FreeRTOS/FreeRTOS_LowPower_LPTIM/Inc/main.h                                  main config file
    - FreeRTOS/FreeRTOS_LowPower_LPTIM/Inc/stm32l4xx_hal_conf.h                    HAL Configuration file
    - FreeRTOS/FreeRTOS_LowPower_LPTIM/Inc/stm32l4xx_it.h                          Header for stm32l4xx_it.c
    - FreeRTOS/FreeRTOS_LowPower_LPTIM/Inc/FreeRTOSConfig.h                        FreeRTOS Configuration file
    - FreeRTOS/FreeRTOS_LowPower_LPTIM/Inc/stm32l4xx_timebase_lptim.h              RTOS time base using LPTIM header file
    - FreeRTOS/FreeRTOS_LowPower_LPTIM/Src/main.c                                  main program file
    - FreeRTOS/FreeRTOS_LowPower_LPTIM/Src/low_power_tick_management_LPTIM.c       RTOS low power tick management
    - FreeRTOS/FreeRTOS_LowPower_LPTIM/Src/stm32l4xx_timebase_lptim.c              Time base using LPTIM 
    - FreeRTOS/FreeRTOS_LowPower_LPTIM/Src/stm32l4xx_it.c                          STM32 Interrupt handlers

@par Hardware and Software environment

  - This example runs on STM32L475xx devices.
    
  - This example has been tested with a B-L475E_IOT01 Rev C board embedding
    a STM32L475xx device and can be easily tailored to any other supported device 
    and development board.


@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 */
