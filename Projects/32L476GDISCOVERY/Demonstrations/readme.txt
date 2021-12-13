/**
  @page Demo MB1184
 
  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of MB1184 Demonstration
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
platform is built around the FreeRTOS real-time operating system and uses almost all
STM32 features and offers a large scope of use cases based on the STM32Cube HAL BSP
and several middleware components.

A Binary file STM32CubeDemo_STM32L476G-Discovery-V1.0.5.hex is available in the 
"binary" directory. It can be downloaded using ST-Link Utility.
      
      
MENU NAVIGATION
========================================
Done with joystick keys:
- RIGHT & SEL:   launch application or enter in sub-menu 
                (SEL key is also used for Pause/Resume in Audio Playback application)
- LEFT:          exit application or back to upper menu level
- DOWN:          next menu item or down volume control in audio application
- UP:            previous menu item or volume up control in audio application 

LEDs
========================================
- LD5 is toggling during Audio record & playback applications else it remains OFF
- LD4 is set ON upon error detection

LOW POWER SCENARII
========================================
By default MCU goes into STOP2 mode after a period of inactivity:
- 10 seconds when running from USB ST-LINK
- 4 seconds when running from BATTERY (CR2032)
Use OPTION->LPMODE sub-menu to select alternative low power strategy:
- OFF (MCU keeps on running (only applicable when not running from battery))
Change in low power strategy applies upon selection and is saved in Backup area for 
next firmware demonstration start up or reset. 
Wake up is done upon any joystick key selection.

========================================
APPLICATIONS LIST
========================================
==========================
POWER UP from USB ST-LINK
==========================

 + IDD   (COMPATIBILITY WITH MFXSTM32L152 FW V2.6)
 --------------------------------------------------
 IDD measurement of the following modes:
  - RUN   (24Mhz from MSI range 9 in voltage range 2, No PLL, no RTC, no LSE, Flash ART on)
  - SLEEP (24Mhz from MSI range 9 in voltage range 2, No PLL, no RTC, no LSE, Flash ART on)
  - LPRUN  (LP run: 2MHz in voltage range 2 with low power regulator, no RTC, no LSE, Flash ART on)
  - PL SLP (LP sleep: 2MHz in voltage range 2 with low power regulator, no RTC, no LSE, Flash ART on)
  - STOP2  (no RTC, no LSE, Flash ART off)  
  - STDBY  (Standby: no RTC, no LSE, Flash ART off)  
  - SHTDWN (shutdown: no RTC, no LSE, Flash ART off)  

 + RECORD
 --------------
 Audio record 16-bit at 48Khz. Recorded file stored in QuadSPI flash.

 + PLAYER
 --------------
 Audio playback of WAV format file from either:
 - internal FLASH (audio.bin provided under "Utilities/Media/Audio" is to be flashed 
   at @ 0x08020000 with ST-Link Utility V3.5.x)
 - recorded audio file from QuadSPI FLASH
 
 + COMPASS
 --------------
 Compass application using the ST Mems LSM303C mounted on the MB1184 board.
 A calibration phase is require at first run, please perform rotations in the 
 3 axis, then press LEFT/RIGHT/SEL key to stop the calibration.
 Calibration data are saved in backup area.

 + SOUND
 --------------
 Soundmeter application with Audio record 16-bit at 48Khz

 + GUITAR
 --------------
 Acoustic guitar tuner application from the thickest to the thinnest string, 
 respectively :
    “STR1”             String 1 (E or 6E or Mi grave (French))
    “STR2”             String 2 (A or 5A or La (French))
    “STR3”             String 3 (D or 4D or Ré (French))
    “STR4”             String 4 (G or 3G or Sol (French))
    “STR5”             String 5 (B or 2B or Si (French))
    “STR6”             String 6 (e or 1E or Mi aigu (French))
 Output:
    “    “ when recording sample is invalid (board needs to be closed enough to the guitar)
    “ OK “ when string is in tune
    “ ++ “ when string needs to be tightened
    “  + ” when string needs to be tightened but close to in tune
    “ -- “ when string is too tightened
    “  - ” when string is too tightened but close to in tune

 + VDD MCU
 --------------
  VDD MCU from VREFINT (ADC)


==========================
POWER UP from VBAT (CR2032)
==========================
Remove Jumper JP3 (RST) and 2 Jumpers on CN3 for ST -Link
Put Jumper JP6 in BATT position

 + IDD   (COMPATIBILITY WITH MFXSTM32L152 FW V2.6)
 --------------------------------------------------
 IDD measurement of the following modes:
  - RUN    (Run: 24MHz in voltage range 2, No PLL, no RTC, no LSE, Flash ART on)
  - SLEEP  (Sleep: 24MHz in voltage range 2, No PLL, no RTC, no LSE, Flash ART on)
  - LPRUN  (LP run: 2MHz in voltage range 2 with low power regulator, no RTC, no LSE, Flash ART on)
  - PL SLP (LP sleep: 2MHz in voltage range 2 with low power regulator, no RTC, no LSE, Flash ART on)
  - STOP2  (no RTC, no LSE, Flash ART off)  
  - STDBY  (Standby: no RTC, no LSE, Flash ART off)  
  - SHTDWN (shutdown: no RTC, no LSE, Flash ART off)  

 + VDD MCU
 --------------
  VDD MCU from VREFINT (ADC)

 + COMPASS
 --------------
 Compass application using the ST Mems LSM303C mounted on the MB1184 board.
 A calibration phase is require at first run, please perform rotations in the 
 3 axis, then press LEFT/RIGHT/SEL key to stop the calibration.
 Calibration data are saved in backup area.

 + SOUDMETER
 --------------
  Soundmeter application with Audio record 16-bit at 48Khz
  Runs at HCLK 16Mhz
  
  
For more details about the demonstration modules please refers to UM1919 "STM32CubeL4 demonstration
for 32L476GDISCOVERY discovery kit" User Manual.

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
    
  - This example has been tested with STM32L476G-DISCO evaluation board (MB1184C) and can be
    easily tailored to any other supported device and development board. 
    
  - Binary file STM32CubeDemo_STM32L476G-Discovery-V1.0.5.hex in the "binary" directory can be downloaded using
  ST-Link Utility (v3.5.1)

  - Binary file generated with IAR Workbench V7.40 in High optimization mode 
   

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the demonstration

 */
 
