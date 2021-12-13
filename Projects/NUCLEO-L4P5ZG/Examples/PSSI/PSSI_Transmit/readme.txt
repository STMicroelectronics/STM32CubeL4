/**
  @page PSSI_Transmit Parallel Synchronous Slave Interface Transmission Example

  @verbatim
  ******************************************************************************
  * @file    PSSI/PSSI_Transmit/readme.txt
  * @author  MCD Application Team
  * @brief   Description of PSSI transmission Example.
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

How to configure the PSSI using the HAL API to transmit data in blocking mode.

No camera is actually used, the goal of this example is merely to illustrate the
initialization and the APIs usage for PSSI peripheral.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 120 MHz.

The PSSI peripheral configuration is ensured by HAL_PSSI_Init() function.
The latter is calling HAL_PSSI_MspInit() function which core is implementing
the configuration of the needed PSSI resources according to the used hardware (CLOCK).
You can update HAL_PSSI_Init() input parameters to change the PSSI configuration.

In this example, transmission is set to 16-bit width data with bus width varying between
8 and 16 lines.
MSI clock divided by 16 yields the master clock to drive PSSI peripheral.

NUCLEO-L4P5ZG (144 pins) LEDs are used to monitor the example status:
  - LED1 (GREEN) toggles each time the bus width is modified
  - LED3 (RED) is ON when a time out or initialization error is detected

User can monitor on an oscilloscope the relevant GPIO pins to check PSSI_Di lines behavior
(from PSSI_D0 to PSSI_D15). HAL_PSSI_MspInit() lists the IOs assigned to each line.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
 to have correct HAL operation.


@par Directory contents

  - PSSI/PSSI_Transmit/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - PSSI/PSSI_Transmit/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - PSSI/PSSI_Transmit/Inc/main.h                  Header for main.c module
  - PSSI/PSSI_Transmit/Src/stm32l4xx_it.c          Interrupt handlers
  - PSSI/PSSI_Transmit/Src/main.c                  Main program
  - PSSI/PSSI_Transmit/Src/stm32l4xx_hal_msp.c     HAL MSP module
  - PSSI/PSSI_Transmit/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L4P5xx devices.

  - This example has been tested with an STMicroelectronics NUCLEO-L4P5ZG (144 pins)
    board and can be easily tailored to any other supported device
    and development board.

  - Connect MCO output PA.08 (pin 23 of CN12 connector) to PA.06 (pin 13 of CN12 connector)
    to provide an external clock to PSSI.

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */

