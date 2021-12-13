/**
  @page Demo   Demo NUCLEO-L412KB Rev C

  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of NUCLEO-L412KB Rev C Demo
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Demo Description

Demonstration of firmware based on STM32Cube. This example provides firmware to
to help you to discover STM32 Cortex-M devices that are plugged onto an your
STM32NUCLEO_32 board.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 80 MHz.


Below you find the sequence to discover the demonstration :

  This demo needs a Gravitech 4 digits 7 segment nano shield.
  You will find it here : http://www.gravitech.us/7sediforarna.html
  Plug the STM32NUCLEO_32 on top of this shield.
  The demo changes the power mode every 10 seconds and updates the display every second.
  The last digit corresponds to the time.
  The first digit corresponds to the power mode as is described below :
    100X = RUN Mode
    200X = SLEEP Mode
    300X = STOP1 Mode
    400X = STOP2 Mode
    500X = STANDBY Mode
    600X = LPRUN Mode
    700X = LPSLEEP Mode
  For details about the low power modes, please look at the stm32l4xx reference manual.

User can resort to the STM32 Power shield X-NUCLEO-LPM01A in order to measure the different power consumptions
as the software goes over the different modes listed above.
Details regarding the connections between the Gravitech shield mounted on the Nucleo and the Power shield
are indicated below in the 'Hardware and Software environmen't section.

It is advised to use as well the graphic display tool to be able to analyze the different measures as the latter
go over very different ranges every second and an amperemeter may not give the correct behavior description.
The graphic tool is available at
www.st.com/stm32softwaretools
with order code: STM32CubeMonPwr
It allows to observe the power consumption measured for each mode with the one-second period peaks occuring when
the display is updated every second.


@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR.
      This implies that if HAL_Delay() is called from a peripheral ISR process,
      then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.

@note: This example (demonstration) can not be used in DEBUG mode due to the fact that the 
       Cortex-M4 core is no longer clocked during low power mode so debugging features are disabled.

@par Directory contents

  - Gravitech_4digits/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - Gravitech_4digits/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - Gravitech_4digits/Inc/main.h                  Header for main.c module
  - Gravitech_4digits/Src/stm32l4xx_it.c          Interrupt handlers
  - Gravitech_4digits/Src/main.c                  Main program
  - Gravitech_4digits/Src/system_stm32l4xx.c      STM32L4xx system source file
  - Gravitech_4digits/Src/stm32l4xx_hal_msp.c     HAL MSP file  


@par Hardware and Software environment

  - This example runs on STM32L412xx devices.
  - This demo has been tested with NUCLEO-L412KB Rev C board and can be
    easily tailored to any other supported device and development board.
  - Gravitech 4 digits 7 segment nano shield is connected on Arduino nano connectors

  If STM32 Power shield X-NUCLEO-LPM01A is used:
  - Remove SB4 and SB9 from the NUCLEO-L412KB
  - Connect Power shield CN14 pin 1 (GND) to NUCLEO-L412KB CN4 GND pin
  - Connect Power shield CN14 pin 3 (VOUT) to NUCLEO-L412KB CN4 3V3 pin
  - Connect Power shield JP3 Ext2 pin to NUCLEO-L412KB CN4 5V pin to supply Gravitech
    shield with the appropriate voltage

@par How to use it ?

In order to make the program work, you must do the following :
  - Open your preferred toolchain
  - Rebuild all files and load your image into target memory
  - Run the example

 */
