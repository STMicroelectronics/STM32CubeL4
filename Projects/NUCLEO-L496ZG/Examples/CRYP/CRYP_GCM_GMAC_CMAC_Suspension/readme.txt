/**
  @page CRYP_GCM_GMAC_CMAC_Suspension  Suspension and Resumption of AES GCM, GMAC and CMAC algorithms.
  
  @verbatim
  ******************************************************************************
  * @file    CRYP/CRYP_GCM_GMAC_CMAC_Suspension/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the suspension and resumption of GCM, GMAC and CMAC processing  
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

How to use the CRYP AES peripheral to suspend then resume the AES GCM, GMAC and
CMAC processing of a message in order to carry out the encryption, decryption 
or authentication tag computation of a higher-priority message. 


For GCM algorithm,
  AES 128 GCM header phase of a low priority message (32-bit data type) is interrupted by the
  AES 256 GCM processing of a high priority message (32-bit data type).
  When the latter is done (encryption and tag generation), the low priority GCM header
  phase is resumed.
  Next, its payload phase is suspended by another processing of an AES 256 GCM high priority 
  message (32-bit data type).
  Once done, the low priority GCM payload phase is resumed followed by the GCM final phase to 
  wrap up the complete low-priority message processing.
   
For GMAC algorithm, 
  AES 256 GMAC header phase of a low priority message (1-bit data type) is interrupted by the
  AES 256 GCM processing of a high priority message (32-bit data type).
  When the latter is done (encryption and tag generation), the low priority GMAC header
  phase is resumed followed by the GMAC final phase.  
   
For CMAC algorithm,  
  AES 128 CMAC header phase of a low priority message (32-bit data type) is interrupted by the
  AES 256 GCM processing of a high priority message (32-bit data type).
  When the latter is done (encryption and tag generation), the low priority CMAC header
  phase is resumed followed by the CMAC final phase.
  



The example unrolls as follows. 
- an AES GCM low-priority message processing is suspended twice due to two consecutive
  interruptions by high priority messages GCM processing. Low priority message GCM header then
  GCM payload phases are in turn suspended then resumed.
  
- Next, an AES GMAC low-priority message processing is suspended due to the interruption 
  by a high priority messages GCM processing. Low-priority message GMAC header phase is 
  suspended then resumed.
    
- Finally, an AES CMAC low-priority message processing is suspended due to the interruption 
  by a high priority messages GCM processing. Low-priority message CMAC header phase is 
  suspended then resumed. 




Low and high-priority blocks AES outputs are compared with expected outputs.
When resorting to IAR Embedded Workbench IDE, AES algorithms processing status
are displayed on debugger Terminal I/O as follows: View --> Terminal I/O.
When resorting to MDK-ARM KEIL IDE, AES algorithms processing status 
are displayed on debugger as follows:  View --> Serial Viewer --> Debug (printf) Viewer.
When resorting to AC6 SW4STM32 IDE:
 In Debug configuration window\ Startup, in addition to "monitor reset halt" add the command "monitor arm semihosting enable"
 Command Code is displayed on debugger as follows: Window--> Show View--> Console.
 
When all ciphering and deciphering operations are successful, LED1 is turned on.
In case of ciphering or deciphering issue, LED3 is turned on. 
  

@par Directory contents 

  - CRYP/CRYP_GCM_GMAC_CMAC_Suspension/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_GCM_GMAC_CMAC_Suspension/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_GCM_GMAC_CMAC_Suspension/Inc/main.h                  Header for main.c module  
  - CRYP/CRYP_GCM_GMAC_CMAC_Suspension/Src/stm32l4xx_it.c          Interrupt handlers
  - CRYP/CRYP_GCM_GMAC_CMAC_Suspension/Src/main.c                  Main program
  - CRYP/CRYP_GCM_GMAC_CMAC_Suspension/Src/stm32l4xx_hal_msp.c     HAL MSP module
  - CRYP/CRYP_GCM_GMAC_CMAC_Suspension/Src/system_stm32l4xx.c      STM32L4xx system source file


@par Hardware and Software environment

   - This example runs on STM32L4A6ZG devices.
  
    - This example has been tested with a STM32L4A6ZG mounted on a 
    Nucleo (144 pins) board and can be easily tailored to any other supported 
    device and development board.


@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
