/**
  @page LTDC_Display_1Layer LTDC Display Layer 1 example
  
  @verbatim
  ******************************************************************************
  * @file    LTDC/LTDC_Display_1Layer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LTDC Display 1 layer example.
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

  How to configure the LTDC peripheral to display a 480x272 RGB565 (16 bits/pixel) 
  image on LCD using only one layer.

  In this basic example the goal is to explain the different fields of the LTDC 
  structure. 

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  Then the SystemClock_Config() function is used to configure the system
  clock (SYSCLK) to run at 120 MHz.
 
  After LCD initialization, the LCD layer 1 is configured to display image 
  (modelled by an array) loaded directly from flash memory by LTDC integrated DMA.
  

  LCD_TFT synchronous timings configuration :
  -------------------------------------------
 
                                             Total Width
                          <--------------------------------------------------->
                    Hsync width HBP             Active Width                HFP
                          <---><--><--------------------------------------><-->
                      ____    ____|_______________________________________|____ 
                          |___|   |                                       |    |
                                  |                                       |    |
                      __|         |                                       |    |
         /|\    /|\  |            |                                       |    |
          | VSYNC|   |            |                                       |    |
          |Width\|/  |__          |                                       |    |
          |     /|\     |         |                                       |    |
          |  VBP |      |         |                                       |    |
          |     \|/_____|_________|_______________________________________|    |
          |     /|\     |         | / / / / / / / / / / / / / / / / / / / |    |
          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
 Total    |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
 Heigh    |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
          |Active|      |         |/ / / / / / / / / / / / / / / / / / / /|    |
          |Heigh |      |         |/ / / / / / Active Display Area / / / /|    |
          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
          |     \|/_____|_________|_______________________________________|    |
          |     /|\     |                                                      |
          |  VFP |      |                                                      |
         \|/    \|/_____|______________________________________________________|
         
         
  Each LCD device has its specific synchronous timings values.
  This example use :
  The TFT LCD 4.3 inch Rocktech RK043FN48H 480x272 (MB1315A)
    In this case the synchronous timings are as following :
      Horizontal Synchronization (Hsync) = 41
      Horizontal Back Porch (HBP)        = 13
      Active Width                       = 480
      Horizontal Front Porch (HFP)       = 32

      Vertical Synchronization (Vsync)   = 10
      Vertical Back Porch (VBP)          = 2
      Active Heigh                       = 272
      Vertical Front Porch (VFP)         = 2
  
  LCD_TFT windowing configuration :
  ---------------------------------

  To configure the active display window, this example configures the 
  horizontal/vertical start and stop. 

  HorizontalStart = (Offset_X + Hsync + HBP);
  HorizontalStop  = (Offset_X + Hsync + HBP + Window_Width - 1); 
  VarticalStart   = (Offset_Y + Vsync + VBP);
  VerticalStop    = (Offset_Y + Vsync + VBP + Window_Heigh - 1);
  
  Window_width and Window_heigh should be in line with the image size to be 
  displayed.

STM32 Eval board's LED can be used to monitor the transfer status:
 - LED3 is ON when there is an error in Init process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.



@par Directory contents

    - LTDC/LTDC_Display_1Layer/Inc/stm32l4xx_hal_conf.h  HAL configuration file
    - LTDC/LTDC_Display_1Layer/Inc/stm32l4xx_it.h        Interrupt handlers header file
    - LTDC/LTDC_Display_1Layer/Inc/main.h                Main configuration file
    - LTDC/LTDC_Display_1Layer/Src/stm32l4xx_it.c        Interrupt handlers
    - LTDC/LTDC_Display_1Layer/Src/main.c                Main program 
    - LTDC/LTDC_Display_1Layer/Src/stm32l4xx_hal_msp.c   HAL MSP module
    - LTDC/LTDC_Display_1Layer/Src/system_stm32l4xx.c    STM32L4xx system clock configuration file
    - LTDC/LTDC_Display_1Layer/Inc/RGB565_480x272.h      Image of size 480x272 and format RGB565 to be displayed on LCD

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
                                   