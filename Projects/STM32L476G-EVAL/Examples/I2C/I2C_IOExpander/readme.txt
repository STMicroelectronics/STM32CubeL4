/**
  @page I2C_IOExpander  I2C IO Expander example
  
  @verbatim
  ******************************************************************************
  * @file    I2C/I2C_IOExpander/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C IO Expander example.
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

How to handle I2C data communication with the I/O expander device mounted on 
the evaluation board.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 80 MHz.

This firmware provides an example of configuring and using the IO and the TS
BSP modules to run the joystick, output IOs and the Touch Screen features. 
This example provides the elementary steps to control the following modules 
and use them in an application:
  - IO module used for getting Joystick pins status (Polling and interrupt).
  - The Touch Screen controller module used to get the Touched area detection information
    in a single point mode (Polling and interrupt).
    
The input information can be used either in Polling mode (uncomment the define
#define POLLING_MODE) or in Interrupt mode (uncomment the define #define
INTERRUPT_MODE). These defines are in main.h file.

After configuring the IO and TS modules and Interrupts (if needed), the output
IOs are set to their default state. If one of the modules is not operational 
(or the I2C pins are not in the correct level), a message is displayed on the LCD 
screen.

To get the status of an input:
  - Dedicated function is used for Joystick which returns a JOYState_TypeDef
    value indicating the currently pressed key. This information is used to update 
    the LCD display (the Joystick pressed key name is displayed).
  - Dedicated function is used for Touch Screen which returns a pointer to 
    a structure holding the Touch Screen status (did a Touch Detection happened, 
    X, Y, and Z positions). Only detection into the drawn rectangles areas is 
    considered and used to update the LEDs status and LCD display (corresponding  
    LED turned ON when its rectangle is touched, and the LED name is displayed on
    the LCD screen).

Besides, the example checks the status of the other Push-Button implemented on
the board (KEY_BUTTON_NAME). It is checked in parallel 
with the IOs and the LCD display and LEDs are updated according to the push button
state. 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - I2C/I2C_IOExpander/Inc/stm32l4xx_hal_conf.h          HAL configuration file
  - I2C/I2C_IOExpander/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - I2C/I2C_IOExpander/Inc/main.h                  Header for main.c module  
  - I2C/I2C_IOExpander/Src/stm32l4xx_it.c          Interrupt handlers
  - I2C/I2C_IOExpander/Src/main.c                  Main program
  - I2C/I2C_IOExpander/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

  - This example runs on STM32L476xx devices.
    
  - This example has been tested with STM32L476G-EVAL board and can be
    easily tailored to any other supported device and development board.      
    @note Make sure that LCD Glass is in IO Position.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 


 */
