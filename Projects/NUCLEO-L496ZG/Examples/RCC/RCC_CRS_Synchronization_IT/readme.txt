/**
  @page RCC_CRS_Synchronization_IT RCC Clock Recovery Service example
  
  @verbatim
  ******************************************************************************
  * @file    RCC/RCC_CRS_Synchronization_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RCC Clock Recovery Service example.
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

Configuration of the clock recovery service (CRS) in Interrupt mode, using the RCC HAL API.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 80 MHz.

Then, HSI48 and LSE oscillators are enabled. 
In this example, as a first step a synchronization miss is raised when using a wrong synchronization 
source (GPIO instead of LSE which is not configured). In a 2nd step, a reconfiguration is done to
use LSE and the correct synchronization.

In this test, this is the scenario:
 - set the source to GPIO and then enable IT
 - to provoke a SYNC event, call the software event generation function
 - Interrupt is raised to indicate a SYNC MISS and LED1 remains off
 - in main program, wait for 1 sec and reconfigure source to LSE
 - Toggle quickly LED1 when SYNC WARN is detected
 - Increase tolerance (FELIM value) when SYNC WARN is detected
 - At the end of the test LED1 should stay ON
 
In case of error, LED3 toggles. 

Note: In this example the SYSCLK is outputted on the MCO1 pin(PA.08).
  - PA.08 connected to pin 23 on CN12 connector


@par Directory contents 

  - RCC/RCC_CRS_Synchonization_IT/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - RCC/RCC_CRS_Synchonization_IT/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - RCC/RCC_CRS_Synchonization_IT/Inc/main.h                  Header for main.c module  
  - RCC/RCC_CRS_Synchonization_IT/Src/stm32l4xx_it.c          Interrupt handlers
  - RCC/RCC_CRS_Synchonization_IT/Src/main.c                  Main program
  - RCC/RCC_CRS_Synchonization_IT/Src/system_stm32l4xx.c      STM32L4xx system source file

@par Hardware and Software environment

  - This example runs on STM32L496ZG Devices.
    
  - This example has been tested with NUCLEO-L496ZG (144 pins) board and can be
    easily tailored to any other supported device and development board.    


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 */
