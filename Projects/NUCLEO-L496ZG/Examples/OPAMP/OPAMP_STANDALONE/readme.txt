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

  
   OPAMP2 is configured as following:
   - Standalone OPAMP mode is configured (as VINM is connected to OPAMP2_VOUT, OPAMP then
     operates as in follower mode (external follower)).
   - Non Inverting input is connected to DAC_OUT2 (sinevawe).
   - OPAMP Output is available on OPAMP2_VOUT (sine wave signal).
   - Push the User Push button to alternatively switch from
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
  
  - This example runs on STM32L496ZG devices.
    
  - This example has been tested with NUCLEO-L496ZG (144 pins) board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-L496ZG (144 pins) Set-up
    
    - Connect with a cable OPAMP_VINN from: pin 15 - connector CN12 (PA7 pin) 
      to OPAMP_OUT2: pin 34 - connector CN11 (PB0 pin).
	  - Connect an oscilloscope to DAC_OUT2 from: pin 11 - connector CN12 (PA5 pin)
	    to display waveform (DAC output).
    - Connect an oscilloscope to OPAMP_OUT2: pin 34 - connector CN11 to display waveform 
      (OPAMP output). 
      The OPAMP voltage output is same as VINP in both normal et low power mode. 
    - In order to start test. Push the User Push button.
      Then push this User Push button again to switch from normal - low power mode


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  
 */

