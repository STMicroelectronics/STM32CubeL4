/**
  @page MSC_Standalone USB Host Mass Storage (MSC) application
  
  @verbatim
  ******************************************************************************
  * @file    USB_Host/MSC_Standalone/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB Host MSC application.
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

@par Application Description 

Use of the USB host application based on the Mass Storage Class (MSC).

This example is part of the USB Host Library package using STM32Cube firmware.

This is a typical application on how to use the STM32L4xx USB OTG Host peripheral to operate with an USB 
flash disk using the Bulk Only Transfer (BOT) and Small Computer System Interface (SCSI) transparent
commands combined with a file system FatFs (Middleware component).

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 120 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock, which is generated from an integrated PLL.

When the application is started, the connected USB flash disk device is detected in MSC mode and gets 
initialized. The STM32 MCU behaves as a MSC Host, it enumerates the device and extracts VID, PID, 
manufacturer name, Serial no and product name information and displays it via the STLink Virtual Com Port 
on a terminal. This application is based on read/write file and explore the USB flash disk content 
trough a MSC routine.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about the STM32Cube USB Host library, please refer to UM1720  
"STM32Cube USB Host library".


@par USB Library Configuration

It is possible to fine tune needed USB Host features by modifying defines values in USBH configuration
file “usbh_conf.h” available under the project includes directory, in a way to fit the application
requirements, such as:
- Level of debug: USBH_DEBUG_LEVEL
                  0: No debug messages
                  1: Only User messages are shown
                  2: User and Error messages are shown
                  3: All messages and internal debug messages are shown
      By default debug messages are displayed via the STLink Virtual Com Port on a terminal:
   - Open an hyperterminal with following setting:
     + COM: STLink Virtual COM Port(COMxxx). This can be found by opening computer
            Device Manager, Port(COM & LPT) once the board is connected to the PC.
     + BaudRate = 115200 baud  
     + Word Length = 8 Bits (7 data bit + 1 parity bit)
     + One Stop Bit
     + Parity none
     + Hardware flow control disabled (RTS and CTS signals)

@par Directory contents

  - USB_Host/MSC_Standalone/Src/main.c                  Main program
  - USB_Host/MSC_Standalone/Src/system_stm32l4xx.c      STM32L4xx system clock configuration file
  - USB_Host/MSC_Standalone/Src/stm32l4xx_it.c          Interrupt handlers
  - USB_Host/MSC_Standalone/Src/menu.c                  MSC State Machine
  - USB_Host/MSC_Standalone/Src/usbh_conf.c             General low level driver configuration
  - USB_Host/MSC_Standalone/Src/usbh_diskio.c           USB diskio interface for FatFs
  - USB_Host/MSC_Standalone/Inc/main.h                  Main program header file
  - USB_Host/MSC_Standalone/Inc/stm32l4xx_it.h          Interrupt handlers header file
  - USB_Host/MSC_Standalone/Inc/lcd_log_conf.h          LCD log configuration file
  - USB_Host/MSC_Standalone/Inc/stm32l4xx_hal_conf.h    HAL configuration file
  - USB_Host/MSC_Standalone/Inc/usbh_conf.h             USB Host driver Configuration file
  - USB_Host/MSC_Standalone/Inc/ffconf.h                FatFs Module Configuration file 

@par Hardware and Software environment

  - This application runs on STM32L4R9xx devices.
    
  - This example has been tested with STMicroelectronics STM32L4R9I-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32L4R9I-EVAL Set-up
    - Plug the USB Key (USB Flash Disk) into the STM32L4R9I-EVAL board through 'USB micro A-Male 
      to A-Female' cable to the connector CN3 (OTG-FS).  

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 */
