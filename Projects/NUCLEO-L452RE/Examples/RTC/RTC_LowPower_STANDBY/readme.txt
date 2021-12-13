/**
  @page RTC_Standby RTC_LowPower_STANDBY example
  
  @verbatim
  ******************************************************************************
  * @file    RTC/RTC_LowPower_STANDBY/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RTC STANDBY example.
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
 
How to enter STANDBY mode and wake up from this mode using the RTC alarm event.

This event is connected to USER_BUTTON_EXTI_LINE.


In the associated software, the system clock is set to 80 MHz, the SysTick is 
programmed to generate an interrupt each 1 ms.
The LSI clock is used as RTC clock source by default.

In the associated software
  - the system clock is set to 80 MHz.
  - the USER_BUTTON_EXTI_LINE connected internally to the RTC Alarm event is configured
    to generate an interrupt on rising edge after 15s.

The system enters STANDBY mode after configurating time, alarm and date:
- time to 23h59:55
- date to 31th of October 2014
- alarm to 00h00:10.

After wake-up from STANDBY mode, program execution restarts in the same way as after
a RESET.

One of the above scenario can occur.
 - LED2 Toggle: An error occurs.
 - LED2 ON: Result is OK.

@note This example can not be used in DEBUG mode, this is due to the fact 
      that the Cortex-M4 core is no longer clocked during low power mode 
      so debugging features are disabled

@note Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select 
      the RTC clock source; in this case the Backup domain will be reset in  
      order to modify the RTC Clock source, as consequence RTC registers (including 
      the backup registers) and RCC_CSR register are set to their reset values.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@par Directory contents 

  - RTC/RTC_LowPower_STANDBY/Inc/stm32l4xx_conf.h         HAL Configuration file
  - RTC/RTC_LowPower_STANDBY/Inc/stm32l4xx_it.h           Header for stm32l4xx_it.c
  - RTC/RTC_LowPower_STANDBY/Inc/main.h                   Header file for main.c
  - RTC/RTC_LowPower_STANDBY/Src/system_stm32l4xx.c       STM32L4xx system clock configuration file
  - RTC/RTC_LowPower_STANDBY/Src/stm32l4xx_it.c           Interrupt handlers
  - RTC/RTC_LowPower_STANDBY/Src/main.c                   Main program
  - RTC/RTC_LowPower_STANDBY/Src/stm32l4xx_hal_msp.c  HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32L452xx devices
    
  - This example has been tested with STMicroelectronics NUCLEO-L452RE Rev C
    evaluation board and can be easily tailored to any other supported device 
    and development board.

 @par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
