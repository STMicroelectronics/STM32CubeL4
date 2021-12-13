/**
  @page Demo   Demo MB1144
 
  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of MB1144 Demonstration
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

The STM32Cube demonstration platform comes on top of the STM32Cube as a
firmware package that offers a full set of software components based on a modular
architecture. All modules can be reused separately in standalone applications. All 
these modules are managed by the STM32Cube demonstration kernel that allows to 
dynamically add new modules and access common resources (storage, graphical components 
and widgets, memory management, real-time operating system). The STM32Cube demonstration
platform is built around the powerful graphical STemWin library and the FreeRTOS realtime
operating system. It uses almost all STM32 features and offers a large scope of use cases
based on the STM32Cube HAL BSP and several middleware components.

A Binary file STM32CubeDemo_STM32L476G_EVAL-V1.0.5.hex is available in the 
"binary" directory. It can be downloaded using ST-Link Utility (see "How to use it?" below).

@par Demonstration Overview

When the STM32L476G-EVAL demonstration is launched for the first time, the touch screen 
needs to be calibrated. You have to follow the displayed calibration instructions by touching 
the screen at the indicated positions.
Once the touch screen calibrated, the supported demos will be viewed per icon. 

 @note All icons are stored into external NOR Flash memory. 

Some Media files are available in the "Utilities\Media" directory. These Media Files can be copy on the SD card, and used as
examples files for the demonstration Audio Player and Image Viewer applications usage.

Below you find an overview of the different modules present in the demonstration:

 + System
 --------
 The system module provides three control tabs: 
  - system information 
  - general settings
  - clock settings 
 To set the global demonstration settings. The system module retrieves
 demonstration information from internal kernel settings data structures and acts 
 on the several kernel services to changes settings.
 
 + Temperature
 --------------
 Temperature is displayed in the Window Top bar and is measured from PT100 (thermistor) and STPMS2 (sigma-delta modulator) mounted on the
 MB1144 board.
 
 + Audio Player
 --------------
 The audio player module provides a complete audio playback solution based on the STM32L4xx and
 delivers a high-quality music experience. It supports playing music in WAV format but may
 be extended to support other compressed formats such as MP3 and WMA audio formats.
 You can use the *.wav audio files provided under "Utilities/Media/Audio" or any other ones.
 
 + Audio Recorder
 ----------------
 The audio recorder module provides a complete audio record solution based on the STM32L4xx and
 delivers a high-quality record and playback experience. It supports record in WAV format at 8Khz, 16Khz,
 44.1khz and 48Khz sample rate in stereo. The record file list explore the SD card content
 to manage a play, delete, properties of file selected. 
  @note : JP14 & JP16 must be set to 3-2 position (Left side).
 
 + File browser
 --------------
 The File browser module is a system module that allows to explore the connected
 storage unit(s), to delete or to open a selected file. The file list structure 
 is built during the media connection and updated after a connection status change of one
 of the used media.
 
 + Image viewer
 --------------
 The Image viewer module allows displaying bmp and jpg pictures. It is possible to load the
 full images list from a folder or to add the images manually to the playlist. Once the playlist is
 created, navigation between pictures can be done either via Next and previous buttons or by
 enabling the slide show mode. The slide show timer can be changed on the fly (there is no
 need to restart the module).
 You can use the *.jpg image files provided under "Utilities/Media/Pictures/JPG" or any other ones.
 
 + IDD Measurement
 -----------------
 The IDD Measurement module allows to perform consumption measurement based on the IDD circuitry present 
 on the MB1144 board and display the current measure by IDD component.
 Following modes with RTC enabled with LSE and VDD at 3.3V can be measured :
 - RUN at 80Mhz from PLL
 - SLEEP at 80Mhz from PLL
 - STOP2 mode
 Wakeup is managed with an EXTI.
 A calibration phase is necessary but only once as data is saved in backup register
 @note JP11 must be set to VDD position for this purpose

 + USB Mass storage Device
 -------------------------
 The USB device module includes mass storage device application using the MicroSD
 memory. It uses the USB OTG FS peripheral.
 Connect the USB cable and press "Connect USB" button to enable the USB Device mass storage class.

 + Game
 ------
 The game coming in the STM32Cube demonstration is based on the Reversi game. It is a
 strategy board game for two players, played on an 8×8 board. The goal of the game is to
 have the majority of disks turned to display your color when the last playable empty square
 is filled.

For more details about the demonstration modules please refers to UM1937 "STM32CubeL4 demonstration
for STM32L476G-EVAL board" User Manual.

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

  - This demonstration runs on STM32L476xx devices.
    
  - This demonstration has been tested with STM32L476G_EVAL RevC (MB1144-C01)
    evaluation board and can be easily tailored to any other supported device and development board. 
  
  @note Make sure that LCD Glass is in IO Position.
  
  @note Jumpers setup on MB1144-C01 (RevC):
   JP1  : 1-2 (VDD_MCU) / JP2  : 1-2 (+3V3)    / JP3  : CLOSED         / JP4  : 1-2 (LEFT)
   JP5  : 1-2 (Pot)     / JP6  : CLOSED        / JP7  : 1-2 (Oop/ADC)  / JP8  : OPENED
   JP9  : OPENED        / JP10 : 1-2 (VDD_MCU) / JP11 : 2-3 (IDD)      / JP12 : 1-2 (VDD)
   JP13 : OPENED        / JP14 : 2-3 (MCU)     / JP15 : CLOSED (RS232) / JP16 : 2-3 (VDD)
   JP17 : STlink        / JP18 : OPENED        / JP19 : CLOSED         / CN10 : CLOSED (ADC_VREF)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Open STM32 ST-Link Utility V3.6 or later, click on "External Loader" from the bar menu then check "M29W128GL_STM32L476G-EVAL" box 
 - Connect the STM32L476G-EVAL board to PC with USB cable through CN17
 - Program "STM32CubeDemo_STM32L476G_EVAL-MB1144.hex" file with STM32 ST-Link Utility to program both internal Flash and 
   external NOR memories
 - Run the demonstration

 */
 
