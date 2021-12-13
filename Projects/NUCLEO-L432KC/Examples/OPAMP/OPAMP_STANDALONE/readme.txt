/**
  @page OPAMP_STANDALONE Amplify an applied signal example  
  @verbatim
  ******************************************************************************
  * @file    OPAMP/OPAMP_STANDALONE/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the OPAMP standalone signals generation example.
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

How to configure the OPAMP peripheral in standalone mode. The gain 
in this mode can be set externally (external gain setting mode).

  
   OPAMP1 is configured as following:
   - Standalone OPAMP mode is configured (as VINM is connected to OPAMP1_VOUT, OPAMP then
     operates as in follower mode (external follower)).
   - Non Inverting input is connected to DAC_OUT1 (sinevawe).
   - OPAMP Output is available on OPAMP1_VOUT (sine wave signal).
   - Use a Jumper between Arduino GND and D2 pins to switch from
     normal power mode to low power mode.
      
    
@par Directory contents 

  - OPAMP/OPAMP_STANDALONE/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - OPAMP/OPAMP_STANDALONE/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - OPAMP/OPAMP_STANDALONE/Inc/main.h                  Header for main.c module  
  - OPAMP/OPAMP_STANDALONE/Src/stm32l4xx_it.c          Interrupt handlers
  - OPAMP/OPAMP_STANDALONE/Src/main.c                  Main program
  - OPAMP/OPAMP_STANDALONE/Src/stm32l4xx_hal_msp.c     HAL MSP file
  - OPAMP/OPAMP_STANDALONE/Src/system_stm32l4xx.c      STM32L4xx system source file

@par Hardware and Software environment  
  
  - This example runs on STM32L432xx devices.
    
  - This example has been tested with NUCLEO-L432KC RevC board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-L432KC RevC Set-up
    
    - Connect with a wire PA.01 (OPAMP_VINM) from Arduino A1 (connector CN4) to 
      PA.03 (OPAMP_VOUT) from Arduino A2 (connector CN4).
    - Connect an oscilloscope to PA.03 (OPAMP_VOUT) from Arduino A2 (connector CN4) 
      to display waveform (OPAMP output). 
      The OPAMP voltage output is same as VINP in both normal et low power mode. 
    - In order to start test, connect a jumper between Arduino GND and D2 (connector CN3). 
      Then disconnect/connect this jumper to alternate from normal and low power mode
      

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  
 */

