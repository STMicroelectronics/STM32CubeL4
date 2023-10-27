/**
  @page LTDC_ColorKeying LTDC Color Keying example
  
  @verbatim
  ******************************************************************************
  * @file    LTDC/LTDC_ColorKeying/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LTDC Color Keying example.
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

How to enable and use the LTDC color keying functionality.

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  Then the SystemClock_Config() function is used to configure the system
  clock (SYSCLK) to run at 120 MHz.
 
 After LCD initialization, LCD layer 1 and color keying configuration, an image 
 of size 480x272 in ARGB4444 format featuring ST logo on light blue background (color key ARGB4444==0xFFFF)
 is displayed on LCD.
 Color keying is enabled/disabled on the image when the wakeup button is pressed. 
 The effect is that at each button press the light blue background is made transparent or not thanks
 to color keying. At time when the color key is enabled, the background of the logo is the background color
 configured for layer 1, which is all zeroes (black color).
 Note that wakeup button press is scanned in polling in the main.c, no use of EXTI in this case.
      
 Color Keying :
 --------------
 If the Color Keying is enabled, all pixels of the image are compared to that color key pixel value
 (0xFFFF), pratically only the background of the logo correspond to that color key in the image. 
 For the pixels that match the color key RGB value, all channels (ARGB) of those pixels 
 are set to 0 (those pixels are then becoming transparent and does not appear on LCD screen, the background
 of image : black is seen instead).

STM32 Eval board's LED can be used to monitor the transfer status:
 - LED3 is ON when there is an error in Init process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, LTDC, Display Controller, TFT, LCD, Graphic, ARGB4444, BMP, color keying, 

@par Directory contents

    - LTDC/LTDC_ColorKeying/Inc/stm32l4xx_hal_conf.h    HAL configuration file
    - LTDC/LTDC_ColorKeying/Inc/stm32l4xx_it.h          Interrupt handlers header file
    - LTDC/LTDC_ColorKeying/Inc/main.h                  Main configuration file
    - LTDC/LTDC_ColorKeying/Src/stm32l4xx_it.c          Interrupt handlers
    - LTDC/LTDC_ColorKeying/Src/main.c                  Main program 
    - LTDC/LTDC_ColorKeying/Src/stm32l4xx_hal_msp.c     HAL MSP module
    - LTDC/LTDC_ColorKeying/Src/system_stm32l4xx.c      STM32L4xx system clock configuration file
    - LTDC/LTDC_ColorKeying/Inc/ARGB4444_480x272.h      Image (single layer 1) of size 480x272 and format ARGB4444 to be displayed on LCD

@par Hardware and Software environment  

  - This example runs on STM32L4R9xx devices.
  
  - This example has been tested with STM32L4R9I-EVAL board and can be
    easily tailored to any other supported device and development board.    

  - STM32L4R9I-EVAL Set-up
    - Make sure that TFT-LCD MB1315A is plugged on CN20

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
                           
 */
                                   