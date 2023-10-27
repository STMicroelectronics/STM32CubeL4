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

Considered board is STM32L4R9I-EVAL. 

This example provides 2 project configurations:
- STM32L4R9I_EVAL-DSI: to be used with DSI 390x390 display (MB1314)
- STM32L4R9I_EVAL-RGB: to be used with RGB 480x272 display (MB1315)

Because of bigger LCD screen, non-oriented BSP LCD driver features are only available
in STM32L4R9I_EVAL-RGB project configuration to display test progress and results.

STM32L4R9I_EVAL-DSI project configuration focuses on DSI round display and
associated touchscreen.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 110 MHz.

This example shows how to use the different functionalities of components 
available on the board by switching between all tests using Wakeup button.

Red LED toggles every 500ms whenever any error is detected.

 ** Push the Wakeup push-button to start first Test.  
Press Wakeup push-button to start another test:

 ** JOYSTICK **
Use the joystick button to move a pointer inside a rectangle 
(up/down/right/left) and change the pointer color(select).

 ** LCD **
This example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

 ** SD **
This example shows how to erase, write and read the SD card and also 
how to detect the presence of the card.

 ** EEPROM **
This example shows how to read and write data in EEPROM I2C M24LR64 connected on STM32L4R9I-EVAL
   * The I2C RF EEPROM memory (M24LR64) is available through the connector CN2.

 ** AUDIO PLAY **
This example shows how to play/pause/resume or change the volume (min 50% / max 100 %)
of an audio playback using Key or Joystick button.

      Plug a headphone to the CN24 connector to hear the sound  /!\ Take care of yours ears.
      Default volume is 60%.
      A press of JOY_SEL button will pause the audio file play (LED 1 green keep previous state
       and LED2 orange ON).
      Another press on JOY_SEL button will resumes audio file play (only LED 1 green blink at each replay)
      @Note: Copy file 'audio.bin' (available in BSP\AudioFile folder) directly in the flash 
      at @0x08100000 using ST-Link utility

 ** AUDIO RECORD **
@note : Make sure that JP15 and JP16 set to 3-2 position (Left side)
This example will use microphones (Mic_L and Mic_R) available on the board. Automatically, sound 
will be recorded and played on the CN23 connector at different sample rate.
Sample rate is changed at each BSP example loop.
LED3 will be ON during record (LED1 means that playback is ongoing)

 ** NOR **
This example shows how to erase, write and read data available on the NOR flash 
available on the board.
@note : Due to IO sharing between FMC and LTDC interfaces, LCD display is switched off 
during NOR operations (specially noticeable for few seconds during NOR flash erase)

 ** SRAM **
This example shows how to erase, write and read data available on the external 
SRAM available on the board.
@note : Due to IO sharing between FMC and LTDC interfaces, LCD display is switched off 
during SRAM operations (specially noticeable for few seconds during SRAM erase)

  ** OSPI NOR **
This example shows how to erase, write and read data available on the Octal Flash memory
available on the board.


The note hereafter is applicable only to STM32L4R9I_EVAL-DSI configuration used with DSI 390x390 display (MB1314).

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
  - BSP/Src/dsi_lcd.c              DSI LCD drawing features
  - BSP/Src/dsi_touchscreen.c      DSI LCD touchscreen features
  - BSP/Src/rgb_lcd.c              RGB LCD drawing features
  - BSP/Src/touchscreen.c          RGB LCD touchscreen features
  - BSP/Src/led.c                  Led display features
  - BSP/Src/log.c                  LCD Log firmware functions
  - BSP/Src/sd.c                   SD features
  - BSP/Src/audio_play.c           Audio Playback features
  - BSP/Src/audio_record.c         Audio Record features
  - BSP/Src/eeprom.c               EEPROM Read/Write features
  - BSP/Src/nor.c                  NOR features
  - BSP/Src/ospi_nor.c             OctoSPI NOR features
  - BSP/Src/sram.c                 SRAM features
  - BSP/Inc/main.h                 Main program header file
  - BSP/Inc/stm32l4xx_hal_conf.h   HAL Configuration file
  - BSP/Inc/stm32l4xx_it.h         Interrupt handlers header file
  - BSP/Inc/lcd_log_conf.h         lcd_log configuration template file
  - BSP/Inc/ARGB4444_480x272.h     Image used for BSP example
  - BSP/Inc/RGB565_320x240.h       Image used for BSP example
  - BSP/Inc/stlogo.h               Image used for BSP example
  - BSP/AudioFile/audio.bin        Audio wave extract.
        
@par Hardware and Software environment  

  - This example runs on STM32L4R9xx devices.
    
  - This example has been tested with STMicroelectronics STM32L4R9I-EVAL
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
  
  - STM32L4R9I-EVAL Set-up
    - Make sure that only one daughter display board is plugged at the time
    - STM32L4R9I_EVAL-DSI project configuration:
      - Make sure that display board MB1314 is plugged on CN16 connector
    - STM32L4R9I_EVAL-RGB project configuration:
      - Make sure that display board MB1315 is plugged on CN20 connector

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 */
