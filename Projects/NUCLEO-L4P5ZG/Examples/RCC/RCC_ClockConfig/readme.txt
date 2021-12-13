/**
  @page RCC_ClockConfig RCC Clock Config example

  @verbatim
  ******************************************************************************
  * @file    RCC/RCC_ClockConfig/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the RCC Clock Config example.
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

@par Example Description

How to configure the system clock (SYSCLK) and modify the clock settings in Run mode, using the RCC HAL API.


In this example, after startup SYSCLK is configured to the max frequency using the PLL with
MSI as clock source, the User push-button (connected to External line 13) will be
used to change the PLL source:
- from initial MSI (120 MHz) to HSI (120 MHz)
then recursively:
- from HSI (120 MHz) to MSI (80 MHz)
- from MSI (80 MHz) to HSI (120 MHz)

Each time the User push-button is pressed, EXTI15_10 interrupt is generated and in the ISR
the PLL oscillator source is checked using __HAL_RCC_GET_PLL_OSCSOURCE() macro:

- If the MSI oscillator is selected as PLL source, the following steps will be followed to switch
   the PLL source to HSI oscillator to run at 120 MHz:
     a- Switch the system clock source to MSI to allow modification of the PLL configuration
     b- Switch to voltage range 1 boost mode for frequency above 80 Mhz
     c- Enable HSI Oscillator, select it as PLL source and finally activate the PLL
     d- Select the PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers

- If the HSI oscillator is selected as PLL source, the following steps will be followed to switch
   the PLL source to MSI oscillator to run at 80 MHz:
     a- Switch the system clock source to HSI to allow modification of the PLL configuration
     b- Enable MSI Oscillator, select it as PLL source and finally activate the PLL
     c- Select the PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
     d- Switch to voltage range 1 normal mode for frequency less or equal to 80 Mhz
     e- Disable the HSI oscillator (optional, if the HSI is no more needed by the application)

In this example the SYSCLK is outputted on the MCO1 pin(PA.08).
 => PA.08 connected to pin 23 on CN12 connector


LED1 is toggled with a timing defined by the HAL_Delay() API:
- Fast toggling every 50ms after reset when SYSCLK set to 120MHz based on PLL with MSI as source
- Medium toggling every 200ms when SYSCLK set to 120MHz based on PLL with HSI as source
- Slow toggling every 500ms when SYSCLK set to 80MHz based on PLL with MSI as source

In case of configuration error, LED3 is turned on.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents

  - RCC/RCC_ClockConfig/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - RCC/RCC_ClockConfig/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - RCC/RCC_ClockConfig/Inc/main.h                  Header for main.c module
  - RCC/RCC_ClockConfig/Src/stm32l4xx_it.c          Interrupt handlers
  - RCC/RCC_ClockConfig/Src/main.c                  Main program
  - RCC/RCC_ClockConfig/Src/system_stm32l4xx.c      STM32L4xx system source file

@par Hardware and Software environment

  - This example runs on STM32L4P5xx devices.

  - This example has been tested with NUCLEO-L4P5ZG (144 pins)
    board and can be easily tailored to any other supported device
    and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
