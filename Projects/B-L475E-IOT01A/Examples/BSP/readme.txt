/**
  @page BSP  Description of the BSP example
  
  @verbatim
  ******************************************************************************
  * @file    BSP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
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

How to use the different BSP drivers of external devices mounted on B-L475E-IOT01A board.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 80 MHz.

This example shows how to use the different functionalities of QSPI and how to get
information from sensors.
It uses the hyperterminal to display information and to get orders from user. 
At the beginning, the program waits the User Button is pressed to put LED2 ON. Once 
this is done, it goes to run the following tests:

### QSPI ###
This example shows how to use the QSPI features(read/write/erase..).

### QSPI MEMORY MAPPED ###
This example shows how to use the QSPI features(read/write/erase..) in memory mapped
mode.

### TEMPERATURE ###
This example shows how to use the sensors to get tempearture values. By default, 
HTS221 is the used sensor. User can get tempearture value using LPS22HB sensor, 
this by uncommenting the following line in main.h file:
/* #define USE_LPS22HB_TEMP */

### HUMIDITY ###
This example shows how to use the HTS221 sensor to get humidity values.

### PRESSURE ###
This example shows how to use the LPS22HB sensor to get pressure values.

### MAGNETO ###
This example shows how to use the LIS3MDL sensor to get magnetometer values.

### GYROSCOPE ###
This example shows how to use the LIS3MDL sensor to get gyroscope values.

### ACCELERO ###
This example shows how to use the LSM6DSL sensor to get accelerometer values.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - BSP/Src/main.c                 Main program
  - BSP/Src/system_stm32l4xx.c     STM32L4xx system clock configuration file
  - BSP/Src/stm32l4xx_it.c         Interrupt handlers 
  - BSP/Src/sensors.c              Tests of sensors
  - BSP/Src/qspi.c                 Tests of QSPI  
  - BSP/Inc/main.h                 Main program header file  
  - BSP/Inc/stm32l4xx_hal_conf.h   HAL Configuration file
  - BSP/Inc/stm32l4xx_it.h         Interrupt handlers header file

        
@par Hardware and Software environment  

  - This example runs on STM32L475xx devices.
    
  - This example has been tested with STMicroelectronics B-L475E-IOT01A
    boards.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open an hyperterminal with following setting:
    + COM: STLink Virtual COM Port(COMxxx). This can be found by opening computer
           Device Manager, Port(COM & LPT) once the board is connected to the PC.
    + BaudRate = 115200 baud  
    + Word Length = 8 Bits (7 data bit + 1 parity bit)
    + One Stop Bit
    + Parity none
    + Hardware flow control disabled (RTS and CTS signals)
    + Reception and transmission are enabled in the time
    
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 */
