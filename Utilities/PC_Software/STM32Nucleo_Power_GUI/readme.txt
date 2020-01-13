/**
  @page Demo_Binary   Description of the STM32 Nucleo Power GUI tool
  
  @verbatim
  ******************************************************************************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the STM32 Nucleo Power GUI tool.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Image license SLA0047,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                                 www.st.com/SLA0047

  *
  ******************************************************************************
   @endverbatim

@par Description

This directory contains the STM32 Nucleo Power GUI tool and the binaries that
must be used on the Nucleo boards when running this tool.
 
The STM32 Nucleo Power GUI tool connects to the STM32 Nucleo hardware boards 
supported by the firmware package. It allows selecting the various low-power modes 
and benchmarking codes thanks to the available binary files. Users can therefore 
quickly assess STM32L4 low-power performance.

 + Using "STM32 Nucleo Power GUI tool"    
    - Connect the STM32 Nucleo board to a PC with a 'USB type A to Micro-B' 
      (Nucleo-32 or Nucleo-144) or 'USB type A to Mini-B' (Nucleo-64) cable 
      through USB connector CN1 to power the board.
    - Select the wanted binary and reprogram the STM32 Nucleo firmware 
      (ex. STM32 ST-LINK Utility, available for download from www.st.com).
      Once done, to ensure all IOs are at their reset levels, unplug then plug
      the board again. 
    - Connect an amperemeter to the IDD jumper (JP5 for Nucleo-144, JP6 for 
      Nucleo-64, JP1 for Nucleo-32).      
    - Unzip "Nucleo GUI.zip" on your laptop or desktop and launch Qtserial.exe.
    - Obey the instructions appearing on the screen in connecting to the COM port
      associated with the Nucleo board.
    - Pick one low power mode proposed among those by the GUI tool and press
      "Confirm".
    - Measure the MCU power consumption on the amperemeter.
    - Reset the board before picking up another low power mode. Use the Reset 
      button on Nucleo-64 or Nucleo-144, connect a wire between GND and NRST pins
      on Nucleo-32.   
    
    
 + Binaries are available under "Binaries" folder:
      - NUCLEO-L412KB.hex for STM32L412KB Nucleo board without external SMPS
      - NUCLEO-L412RB-P.hex for STM32L412RB Nucleo board with external SMPS      
      - NUCLEO-L433RC-P.hex for STM32L433RC Nucleo board with external SMPS
      - NUCLEO-L452RE.hex for STM32L452RE Nucleo board without external SMPS
      - NUCLEO-L452RE-P.hex for STM32L452RE Nucleo board with external SMPS      
      - NUCLEO-L476RG.hex for STM32L476RG Nucleo board
      - NUCLEO-L496ZG.hex for STM32L496ZG Nucleo board without external SMPS
      - NUCLEO-L496ZG-P.hex for STM32L496ZG Nucleo board with external SMPS
      - NUCLEO-L4P5ZG.hex for STM32L4P5ZG Nucleo board without external SMPS
      - NUCLEO-L4R5ZI.hex for STM32L4R5ZI Nucleo board without external SMPS
      - NUCLEO-L4R5ZI-P.hex for STM32L4R5ZI Nucleo board with external SMPS              

          
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
