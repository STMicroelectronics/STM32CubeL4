/**
  @page Watermark example

  @verbatim
  ******************************************************************************
  * @file    Demonstrations/Watermark/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the watermark demonstration.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Demonstration Description

The software watermarks the PNG xxxxx.png files with logo.png.
In other words,  xxxxx.png files + logo.png = xxxxx_logo.png.

To achieve that :
1°) The e-mmc is used to mount a fatfs filesystem.
2°) Files are shared with PC thanks to the USB device feature.
3°) AP memory RAM accessible through OctoSPI is used to store the raw image buffer (up to 8 MBytes).
4°) library libPNG 100% software is used to decode/encode png format.
5°) DMA2D peripheral is used to blend logo into images.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 120 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock, which is generated from an integrated PLL.

Demonstration behavior :
1°) At application start, both Green LED1 and Blue LED2 are On for half a second to indicate the software is up and running.
2°) Plug a USB cable from the USB OTG connector to a PC and the device is automatically detected. 
    As long as the USB cable is not connected, Blue LED2 is blinking with a 100 ms period.
3°) Add png images xxxxx.png and a logo.png thanks to the windows explorer (*).
4°) Press the joystick SEL button (select button).
5°) The Green LED1 is On meaning that the processing is on going. This step can last several seconds depending on the number and the sizes of the png files.
6°) When Green LED1 is Off, press again the SEL joystick button. The device is again detected by the PC.
7°) Get the watermarked png files thanks to the windows explorer.
8°) Go to step 3°) if you wish to watermark some other images

(*) You can find png files and logo here : Demonstrations/Watermark.

PNG Size limitation due to memory constraint and 4 channels decoding (32 bpp) :
1°) logo.png resolution is limited to 16 000 pixels (you may choose for instance a size 150*100 for the logo)
2°) xxxxx.png resolution is limited to 2 000 000 pixels (for instance, a size 1500*1000 is fine for the demonstration)

LED meanings :
1°) Green LED1 and Blue LED2 are On together for half a second = application has just started.
2°) Blue LED2 is blinking with a 100 ms period = USB cable is not connected
3°) Green LED1 is On =  Decoding/Watermarking/Encoding is on-going. USB Device is Off.
4°) Blue LED2 is On = Warning. logo.png is missing or too big or one of xxxxx.png is too big.
5°) Green LED1 and Blue LED2 are toggling together with a 200 ms period = fatal error.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Hardware and Software environment

  - This application runs on STM32L4P5xx devices.

  - This example has been tested with STMicroelectronics STM32L4P5G-DISCO
    evaluation board (MB1535) and can be easily tailored to any other supported device
    and development board.

  - STM32L4P5G-DISCO Set-up
    - Connect the STM32L4P5G-DISCO board to the PC through 'USB micro A-Male
      to A-Male' cable to the connector CN7 (USB_OTG)

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
