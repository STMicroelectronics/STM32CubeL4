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

Considered board is STM32L476G-Discovery. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 80 MHz.

This example shows how to use the different functionalities of components 
available on the board by switching between all tests using Joystick UP/DOWN 
buttons and RIGHT button to launch the tests. 

 ** LED **  
LED4 and LED5 are ON for a second, then OFF for a second, then toggle for 5 seconds 
and finally LED4 and LED5 are set back to OFF.

 ** JOYSTICK **
Use any joystick buttons to display the corresponding direction on the LCD glass
(UP/DOWN/SEL/RIGHT/LEFT), LEFT button is used to exit the Joystick test.

 ** LCD GLASS **
This example shows how to use the different LCD glass features at any Joystick 
buttons press.

 ** GYRO **
This example shows how to use read the Gyroscope angular data.

 ** IDD **
This example shows how to use the IDD measurement driver to measure the following 
MCU modes automatically one after each other: RUN (80Mhz), SLEEP, LPRUN (2Mhz), 
LPSLEEP, STOP2, STANDBY and SHUTDOWN (LSE being Off).

 ** AUDIO PLAY **
This example show how to play/pause/resume or change the volume (min 50% / max 100 %)
of an audio playback using Joystick button.

      Plug a headphone to ear the sound  /!\ Take care of yours ears.
      Default volume is 60%.
      LED5 toggles during the audio playback
      A press of JOY_SEL button pauses the audio file play (LED5 is ON).
      Another press on JOY_SEL button resumes audio file play (LED5 toggles again)
  @Note: Copy file 'audio.bin' (available in Utilities\Media folder) directly in the flash 
  at @0x08020000 using ST-Link utility

 ** AUDIO RECORD **
This example will use the microphone available on the board. Automatically, sound 
will be recorded and played on headphone in loopback at the selected sample rate: 
use UP/DOWN joystick key to reach sample rate and press RIGHT to launch audio record.
LED5 toggles during record until LEFT joystick key is pressed.

  ** QSPI **
This example shows how to read the QuadSPI flash memory information, erase, write 
and read data available on the QSPI memory available on the board.
LED4 (Error) or LED5 (OK) toggles during the QSPI tests until SEL joystick key is pressed.


@par Directory contents 

  - BSP/Src/main.c                 Main program
  - BSP/Src/system_stm32l4xx.c     STM32L4xx system clock configuration file
  - BSP/Src/stm32l4xx_it.c         Interrupt handlers 
  - BSP/Src/joystick.c             joystick features
  - BSP/Src/lcdglass.c             LCD Glass features
  - BSP/Src/led.c                  LEDs features
  - BSP/Src/gyro.c                 gyroscope features
  - BSP/Src/idd.c                  IDD measurement consumption features
  - BSP/Src/qspi.c                 QSPI Erase/Read/Write features
  - BSP/AudioFile/audio.bin        Audio wave extract.
  - BSP/Src/audio_play.c           Audio Playback features
  - BSP/Src/audio_record.c         Audio Record features
  - BSP/Inc/main.h                 Main program header file  
  - BSP/Inc/stm32l4xx_conf.h       HAL Configuration file
  - BSP/Inc/stm32l4xx_it.h         Interrupt handlers header file
        
@par Hardware and Software environment  

  - This example runs on STM32L476xx devices.
    
  - This example has been tested with STMicroelectronics STM32L476G-Discovery
    boards and can be easily tailored to any other supported device 
    and development board.
  
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 */
