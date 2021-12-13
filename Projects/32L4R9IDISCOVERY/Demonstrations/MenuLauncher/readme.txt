/**
  @page Sub-Demonstration MB1311

  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of MB1311 Demonstration
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

@par Demonstration Description

The STM32Cube demonstration platform comes on top of the STM32CubeTM as a firmware
package that offers a full set of software components based on a modular architecture.
All modules can be reused separately in standalone applications.
All these modules are managed by the STM32Cube demonstration kernel that allows to
dynamically add new modules and access common resources (storage, graphical
components and widgets, memory management, real-time operating system).
The STM32Cube demonstration platform is built around the FreeRTOS real-time operating
system and offers a large scope of use cases based on the STM32Cube HAL BSP and several
middleware components.

The demonstration's executable STM32Cube_Demo-STM32L4R9I-Discovery-V1.0.0.hex can be flashed
to the target using the ST-Link Utility.


Firmware files are available from the SDCard folder within the demonstration package and they are
to be copied on the SD card (root directory) using the host PC.

  EWIZ_firmware.bin,  Embedded Wizard Sub-Sub-Demonstration binary firmware file used by the launcher for IAP programming
  EWIZ_resources.bin, Embedded Wizard Sub-Sub-Demonstration binary resources file used by the launcher for IAP programming
  TGFX_firmware.bin,  TouchGFX Sub-Sub-Demonstration binary firmware file used by the launcher for IAP programming
  TGFX_resources.bin, TouchGFX Sub-Sub-Demonstration binary resources file used by the launcher for IAP programming
  STEW_firmware.bin,  STemWin Sub-Sub-Demonstration binary firmware file used by the launcher for IAP programming
  STEW_resources.bin, STemWin Sub-Sub-Demonstration binary resources file used by the launcher for IAP programming

Multimedia content is available outside the demonstration package from "\Utilities\Media\Audio"
folder and the Audio folder is to be copied on the SD card (root directory) using the host PC.
Below is the listing of these additional files.

Audio files
\---
    |   Dublin Forever.wav
    |   Gardenss 22KHz.wav
    |   Love Letters.wav
    |
    +---Electronica
    |       album_art.bmp
    |       Dubstep.wav
    |       Scifi.wav
    |       ScifiTwo.wav
    |
    +---Empty
    |       empty_folder.txt
    |
    +---Jazz
    |       album_art.bmp
    |       Love.wav
    |       The Jazz Piano.wav
    |
    +---NoCover
    |       Audio Sample.wav
    |
    \---Rock
            album_art.bmp
            Clear Day.wav
            Little Idea.wav

The demonstration starts by displaying a splash screen animation with the ST logo and goes
into the launcher screen where the user can select between one of the three Sub-Demonstrations,
that's one Sub-Sub-Demonstration per graphics stack: STemWin, TouchGFX and Embedded Wizard.
The launcher can be used to upgrade these three Sub-Sub-Demonstration separately.

Within the launcher, the user selects which Sub-Sub-Demonstration to run by sweeping on the screen
from letf to right / right to left and pressing on the selected graphics stack logo icon.
The sections below describe the features of each Sub-Sub-Demonstration.


STemWin
=======

The STemWin Sub-Sub-Demonstration features the following modules:

  - Clock Faces
  - Audio player
  - Audio recorder
  - Power measurement
  - Information
  - Reset Button

 + Clock Faces
 -------------
The module display the current time and date. The user can customize the clock face by double-tapping
on the clock main view. Doing so brings in a carousel view with four clock faces and a settings menu
to configure the time, date and to set alarms. The user has to swipe through the carousel and tap on
the selected clock widget to set the new clock face.

 + Audio player
 --------------
This module enables playing WAVE audio files, as provided with the accompanying SD card. The user starts
by double-tapping on the player screen to get into the module's main view then press the previous, next
and play icons to go through the available audio files (enumerated from the SD card) and to start the playback.
The audio player displays the duration and the progress within the song as playback goes on. It's also possible
to toggle on/off repeat-all and repeat-once functionalities by tapping on the icons at the bottom of the screen.
Audio output can be mutted by pressing on the white speaker icon on the right of the volume slider. The volume
can be adjusted by the user by pressing on +/- icons; doing that the progress bar should be updted and will
display the current volume level.
The user can go back to the main menu (last seleted clock widget gets displayed in fullscreen mode) by
tapping on the Back icon.

@note SD Card removal while playing is not supported.

 + Audio recorder
 ----------------
This module enables recording audio samples via the built-in microphones and saving these on the SD card.
The user has to double-tap on the recorder front screen to get into the module's main view and where the
recorder has already started capturing ambient audio. The use can save the captured audio to the SD card
in real-time by tapping on the save icon. The file listing icon on the right displays the list of the audio
content captured and stored on the SD card.
The user can go back to the main menu (last seleted clock widget gets displayed in fullscreen mode) by
tapping on the Back icon.

@note SD Card removal while recording audio is not supported.

 + Power measurements
 --------------------
This module enables measuring the MCU power consumption in various execution modes. The user starts by double-tapping
the screen to start interacting with the module. By then the user can select between the execution modes by tapping
on the left and right icons and then start the benchmarking run by pressing the middle run icon. Execution modes
include Run, Sleep, LP Run, LP Sleep, Stop, Standby and Shutdown modes. For each run, the module configures the MCU
for the respective execution mode and relies on the MFX chip to measure the consumed power which is then displayed
onscreen. The module temporarily turns off the screen (and other peripherals) while measuring the power.
The user can go back to the main menu (last seleted clock widget gets displayed in fullscreen mode) by
tapping on the Back icon.

 + Information menu
 ------------------
This module displays the following information:
  The CPU speed
  The board name
  The MCU core
  The demonstration firmware version
The user can go back to the main menu (last seleted clock widget gets displayed in fullscreen mode) by
tapping on the Back icon.

 + 'Reset' icon
 --------------
 Pressing this icon allows to immediately come back to the initial launcher menu.

Embedded Wizard
===============

The Embedded Wizard demonstration features the following modules:

  Running
  Watch
  Info
  'Back' icon

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

 + 'Reset' icon
 -------------------------
 Pressing this icon allows to immediately come back to the initial launcher menu.


TouchGFX
========

Reduced and Full TouchGFX demonstration are described in the same section.
Both demonstrations provide the same set of applications but the full demonstration
offers more graphic elements (e.g. waveforms in the background when audioplayer is
running, more different watch displays, more activity monitoring sub menus...)


 + Audio player
 ----------------
This module allows to play various *.wav stored on the SD card.
While playing audio, to move from one audio album to another, stop playing the audio first.

 + Game 2D
 ---------
This "Bird" game stresses the gain provided by the Chrom-ART IP. Pressing the "ST"
button enables/disables this IP and the CPU load, displayed in real time, allows
to check the hw IP gain in real time.

 + Clock Faces
 -------------
This module provides different kinds of watch faces.

 + Power measurements
 --------------------
This module enables measuring the MCU power consumption in various execution modes. The user selects the target
execution mode by swiping between the available mode, which include Run, Sleep, LP Run, LP Sleep, Stop, Standby
and Shutdown, then taps on the selected mode to start the benchmarking run. For each run, the module configures the MCU
for the respective mode and relies on the external MFX chip to measure the consumed power which is then displayed
onscreen. The module temporarily turns off the screen (and other peripherals) while measuring the power.

 + Activty Tracker
 -----------------
This module displays several graphic submenus related to different activity tracking
(measurements and statistics are running in simulation).

 + 'Back' icon
 -------------
Pressing this icon allows to immediately come back to the initial launcher menu.

@par Hardware and Software environment

 1 - The jumpers configuration is as follows:
     JP1 fitted in 1-2 position (IDD)
     JP2 opened
     JP3 fitted in 1-2 position (3V3)
     JP4 set on STLK in 1-2 position
     JP5 fitted in 1-2 position (ARD_V5_OUT)
     JP7 fitted in 1-2 position (3V3)
     JP9 closed
     JP10 fitted in 1-2 position
     SW1 in position ON

 2 - Open STM32 STM32 ST-LINK Utility v4.1.0.3, click on "External Loader" from the bar menu
     then check "MX25LM51245G_STM32L4R9I-DISCO" box.

     Connect the STM32L4R9I-DISCO board to PC with USB cable through CN13
     Use the demonstration STM32Cube_Demo-STM32L4R9I-Discovery-V1.0.0.hex binary file with the
     ST-Link Utility to program both internal Flash and external OSPI memory.
     After flashing the board it might happen that the board doesn't automatically restart; then
     please proceed to manual reset using the RESET button (near to the LCD screen).

 3 - Copy the audio files from "\Utilities\Media\Audio" and the binary files provided
     under "Binary\SDCard" folder within the demonstration package in the SD card (root directory)
     and insert the SD card into CN6 connector

 4 - Run the demonstration

 
  - This demonstration runs on STM32L4R9Ixx devices.

  - This application has been tested with STM32L4R9I-DISCO board (MB1311-RevC01) and can be
    tailored to any other supported device and development board.

 */
