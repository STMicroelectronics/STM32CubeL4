/**
  @page Demo MB1261

  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of MB1261 Demonstration
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

@par Demo Description

The STM32Cube demonstration platform comes on top of the STM32CubeTM as a firmware
package that offers a full set of software components based on a modular architecture.
All modules can be reused separately in standalone applications.
All these modules are managed by the STM32Cube demonstration kernel that allows to
dynamically add new modules and access common resources (storage, graphical
components and widgets, memory management, real-time operating system).
The STM32Cube demonstration platform is built around the FreeRTOS real-time operating
system and uses almost all STM32 features and offers a large scope of use cases based
on the STM32Cube HAL BSP and several middleware components.

A Binary file STM32CubeDemo_STM32L496G-Discovery-MB1261-B01_V1.0.2.hex is available in the
"\Projects\32L496GDISCOVERY\Demonstrations\Binary" directory.
It can be downloaded using ST-Link Utility.

The demonstration must run with the SD card coming with the Discovery Kit inserted in CN17 connector.

For the demo to run as expected, this SD card contains the following folders and files:
        * Audio folders listed hereafter, containing the audio files used by the demonstration
           - Electronica
           - Jazz
           - NoCover
           - Rock
        * Video folder: video file used by the demonstration
        * EWIZ_flash.bin: binary used by Embedded Wizard graphic demonstration
        * EWIZ_ressources.bin: graphic resources used by Embedded Wizard  graphic demonstration
        * TGFX_Full_flash.bin: binary used by TouchGFX graphic full demonstration
        * TGFX_Full_resources.bin: graphic resources used by TouchGFX graphic full demonstration
        * TGFX_Lite_flash.bin: binary used by TouchGFX graphic reduced demonstration
        * TGFX_Lite_resources.bin: graphic resources used by TouchGFX graphic reduced demonstration

These folders and files are all available in the FW package at the following location:
"\Utilities\Media\Audio"
"\Utilities\Media\Video"
"\Projects\32L496GDISCOVERY\Demonstrations\Binary\SDCard"

The complete folders and files list in SDCard root directory shall be:
       Electronica/Dubstep.wav
       Electronica/Scifi.wav
       Electronica/ScifiTwo.wav
       Empty/empty_folder.txt
       Jazz/Love.wav
       Jazz/The Jazz Piano.wav
       NoCover/art of gardens 48KHz.wav
       NoCover/dublin forever 44KHz.wav
       NoCover/gardenss 22KHz.wav
       NoCover/love letters 32KHz.wav
       NoCover/lrcastapiano_s8.wav
       Rock/Clear Day.wav
       Rock/Little Idea.wav
       Video/Plane.avi
       EWIZ_flash.bin
       EWIZ_ressources.bin
       TGFX_Full_flash.bin
       TGFX_Full_resources.bin
       TGFX_Lite_flash.bin
       TGFX_Lite_resources.bin


Below you find an overview of the different modules offered in the demonstration.

 The LCD screen displays several menus that can be selected in sliding the
 screen then pressing the proper icon in order to choose one of 4 modules:
 - STemWin graphic demonstration
 - TouchGFX graphic (reduced) demonstration
 - TouchGFX graphic full demonstration
 - Embedded Wizard graphic demonstration

@note When the demonstration is launched for the very first time, the touchscreen
needs to be calibrated. To calibrate again the Touchscreen, user needs to press
the joystick SEL button while powering on the board or during a software reset
(e.g., during a RESET button press).

Upon STemWin, TouchGFX (reduced) or Embedded Wizard demonstration icon press, the
demonstration starts.
Upon TouchGFX full demonstation icon press, the relevant graphic resources and binary
are downloaded from the SD card to the Quad-SPI external Flash and when the copy is over
(indicated by progress bars), the demonstration starts.

STemWin
=======

Upon STemWin demonstration pick up, the following icons show up:

  - Video player
  - USB Device
  - Audio player
  - Audio recorder
  - Power measurement
  - Analog clock
  - Information
  - 'Return' icon

 In the various modules, you can follow the instructions on the LCD screen.

 + Video player
 ----------------
 This module allows to play a *.avi file stored on the SD card and tailored for
 the Discovery LCD screen size.
 User may use *.avi video files with (200 x 240) dimensions.

 + Audio player
 ----------------
 This module allows to play various *.wav stored on the SD card in NoCover folder.

 + Audio recorder
 ----------------
 This module allows to record the user either through the headset microphone
 or through the board microphones.

 + USB device
 -----------
 The USB device module includes mass storage device application using the MicroSD
 memory .The user has just to plug the USB cable into a PC host and the device is
 automatically detected. A new removable drive appears in the system window and
 write/read/format operations can be performed as with any other removable drive.

 + Power measurements
 -------------------------
 This module provides the MCU power consumption in various measurements. Scroll through
 the various proposed low power modes. Measures start when user presses "activate" button.
 The screen is shut down (except in Run mode) and the measurement result is reported in
 about four seconds.

@note JP2 should be fitted in 1-2 position (IDD)

 + Analog clock
 -------------------------
 This module displays an analog clock. The user can set the hour and the minute hands thanks to
 the setting menu.

 + Information icon
 -------------------------
 This module yields the following information:
 - The demonstration firmware version
 - The MCU core
 - The board name
 - The CPU speed

 + 'Return' icon
 -------------------------
 Pressing this icon allows to immediately come back to the initial menu.



Embedded Wizard
=================

Upon Embedded Wizard demonstration pick up, the following icons show up:

  - Running
  - Watch
  - Info
  - 'Back' icon


 + Running
 -------------------------
 This module displays several graphic submenus related to activity monitoring (measures,
 heart bit, statistics, map ...).

 + Watch
 -------------------------
 This module provides different kinds of watch displays.

 + Info
 -------------------------
 This module yields several information on Embedded Wizard experience.

 + 'Back' icon
 -------------------------
 Pressing this icon allows to immediately come back to the initial menu.



TouchGFX (reduced/full)
========

Reduced and Full TouchGFX demonstration are described in the same section.
Both demonstrations provide the same set of applications but the full demonstration
offers more graphic elements (e.g. waveforms in the background when audioplayer is
running, more different watch displays, more activity monitoring sub menus...)


 + Audio player
 ----------------
 This module allows to play various *.wav stored on the SD card.
 While playing audio, to move from one audio album to another, stop playing the audio first.

 + Game
 -------------------------
 This "Bird" game stresses the gain provided by the Chrom-ART IP. Pressing the "ST"
 button enables/disables this IP and the CPU load, displayed in real time, allows
 to check the hw IP gain in real time.

 + Watch
 -------------------------
 This module provides different kinds of watch displays.

 + Heartbeats
 -------------------------
 This module displays several graphic submenus related to different activity monitoring
 (measures and statistics).

 + 'Back' icon
 -------------------------
 Pressing this icon allows to immediately come back to the initial menu.


For more details about the demonstration modules please refer to UM2145 "STM32CubeL4
STM32L496G-Discovery demonstration firmware" User Manual.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in HAL time base ISR.
      This implies that if HAL_Delay() is called from a peripheral ISR process, then
      the HAL time base interrupt must have higher priority (numerically lower) than
      the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority()
      function.

@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set accordingly to the
      OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications
on STM32Cube with RTOS".


@par Hardware and Software environment

 1 - The jumpers configuration is as follows:
     JP2 fitted in 1-2 position (IDD)
     JP3 fitted in 1-2 position (ARD_V5_IN)
     JP4 fitted in 1-2 position (3V3)
     JP5 fitted in 1-2 position (3V3)
     JP6 fitted in 1-2 position
     JP7 set on STLK
     JP8 closed
     JP9 opened

 2 - Open STM32 ST-Link Utility V4.1.0, click on "External Loader" from the bar menu
     then check "MX25R6435F_STM32L496G-DISCO" box

     Connect the STM32L496G-DISCO board to PC with USB cable through CN5
     Use the demonstration binary file generated by your preferred toolchain with
     ST-Link Utility to program both internal Flash and external QSPI memory

 3 - Copy the audio and video files from respectively "\Utilities\Media\Audio" and
     "\Utilities\Media\Video" folders and the binary files provided under "Binary\SDCard"
     folder within the demonstration package in the SD card (root directory) and
     insert the SD card into CN17 connector

 4 - Run the demonstration


  - This demonstration runs on STM32L496xx devices.

  - This example has been tested with STM32L496G-DISCO board (MB1261-revB01) and can be
    easily tailored to any other supported device and development board.

  - Binary file STM32CubeDemo_STM32L496G-Discovery-MB1261-B01_V1.0.2.hex in the "binary" directory can be downloaded
  using ST-Link Utility.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the demonstration

 */

