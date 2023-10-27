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
   - Standalone mode (gain depends on external loads).
   - Non Inverting input is connected to DAC_OUT1 (sinevawe).
   - OPAMP Output is available on OPAMP1_VOUT (amplified sine wave signal).
   - Push the Key Push button to alternatively switch from
     normal power mode to low power mode.
   - In both normal or low power modes, the gain can be used adjusted thanks to RV2 
        potentiometer (from gain = 0 up to maximum - saturation may occurs).
      
    
@par Directory contents 

  - OPAMP/OPAMP_STANDALONE/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - OPAMP/OPAMP_STANDALONE/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - OPAMP/OPAMP_STANDALONE/Inc/main.h                  Header for main.c module  
  - OPAMP/OPAMP_STANDALONE/Src/stm32l4xx_it.c          Interrupt handlers
  - OPAMP/OPAMP_STANDALONE/Src/main.c                  Main program
  - OPAMP/OPAMP_STANDALONE/Src/stm32l4xx_hal_msp.c     HAL MSP file
  - OPAMP/OPAMP_STANDALONE/Src/system_stm32l4xx.c      STM32L4xx system source file

@par Hardware and Software environment  
  
  - This example runs on STM32L476xx devices.
    
  - This example has been tested with STM32L476G-EVAL board and can be
    easily tailored to any other supported device and development board.

  - STM32L476G-EVAL Set-up
    
      - Make sure that LCD Glass is in position IO.
      - Make sure jumper in position on CN10.
      - Make sure jumper jumper JP10 is either in position 12 or 23.
      - Connect an oscilloscope to (DAC_OUT1, PA4: pin38  - connector CN7)
        (i.e. DAC output = OPAMP input).
      - Connect an oscilloscope to TP9 (PA3 can not be accessed easily) pin to display waveform 
        (OPAMP output)
      

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  
 */

