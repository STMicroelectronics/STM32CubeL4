/**
  @page BSP  Example on how to use the BSP drivers
  
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

How to use the different BSP drivers of the board.

Considered board is STM32L4R9I-Discovery. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 120 MHz.

This example shows how to use the different functionalities of components 
available on the board by switching between all tests using key button.

Red LED toggles every 500ms whenever any error is detected.

 ** Push the Joystick right (JOY_RIGHT) button to start first test.  
Press JOY_RIGHT to carry on other tests.

Available tests are:

 ** JOYSTICK **
Use the joystick button to move a pointer inside a rectangle 
(up/down/right/left) and change the pointer color(select).

 ** LCD **
This example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

 ** OSPI NOR **
This example shows how to erase, write and read data available on the Octal Flash memory
available on the board.

 ** SD **
This example shows how to erase, write and read the SD card and also 
how to detect the presence of the card.

 ** AUDIO PLAY **
This example show how to play/pause/resume or change the volume (min 50% / max 100 %)
of an audio playback using Joystick button.

      Plug a headphone to hear the sound  /!\ Take care of yours ears.
      Default volume is 60%.
      A press of JOY_SEL button will pause the audio file play (LED2 green keep previous state
       and LED1 orange ON).
      Another press on JOY_SEL button will resumes audio file play (only LED 1 green blink at each replay)
      @Note: Copy file 'audio.bin' (available in BSP\AudioFile folder) directly in the flash 
      at @0x08100000 using ST-Link utility

 ** AUDIO RECORD **
This example will use microphones (Mic_L (U1) and Mic_R (U2)) available on the board.
Automatically, sound will be recorded and played on the CN12 connector at different sample rate.
Sample rate is set at 22Khz.

@note LCD_DIMMING and LCD_OFF_AFTER_DIMMING compilation switches enable screen dimming then
      LCD display automatic turning off in case of no activity: 
      after 7 seconds without activity (no joystick key press, no touchscreen, ...), the screen is
      dimmed. If the no-activity period carries on for 14 seconds afterwards, the LCD display is turned off.
      Any new activity (joystick key press, etc ...) is sufficient to stop screen dimming
      or to turn the LCD display back on to maximum brightness.
      User can change the dimming and screen display off countdown parameters in modifying
      DIMMING_COUNTDOWN or LCD_OFF_COUNTDOWN constants defined in stm32l4r9i_discovery_lcd.h.
      In disabling LCD_OFF_AFTER_DIMMING compilation switch, user can resort to screen dimming only.
      Disabling LCD_DIMMING compilation switch disables the full LCD screen dimming/turning off feature.

@par Directory contents 

  - BSP/Src/main.c                 Main program
  - BSP/Src/system_stm32l4xx.c     STM32L4xx system clock configuration file
  - BSP/Src/stm32l4xx_it.c         Interrupt handlers 
  - BSP/Src/joystick.c             joystick feature
  - BSP/Src/lcd.c                  LCD drawing features
  - BSP/Src/touchscreen.c          LCD touchscreen features
  - BSP/Src/sd.c                   SD features
  - BSP/Src/audio_play.c           Audio Playback features
  - BSP/Src/audio_record.c         Audio Record features
  - BSP/Src/nor.c                  NOR features
  - BSP/Src/ospi_nor.c             OctoSPI NOR features
  - BSP/Inc/main.h                 Main program header file  
  - BSP/Inc/stm32l4xx_hal_conf.h   HAL Configuration file
  - BSP/Inc/stm32l4xx_it.h         Interrupt handlers header file
  - BSP/Inc/lcd_log_conf.h         lcd_log configuration template file
  - BSP/Inc/stlogo.h               Image used for BSP example
  - BSP/AudioFile/audio.bin        Audio wave extract.
        
@par Hardware and Software environment  

  - This example runs on STM32L4R9xx devices.
    
  - This example has been tested with STMicroelectronics STM32L4R9I-DISCO
    boards and can be easily tailored to any other supported device 
    and development board.
  
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 */
