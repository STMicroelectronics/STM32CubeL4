/**
  @page DSI_ULPM_Data : enter and exit DSI ULPM Mode on data lane only,
  while displaying an RGB888 picture 390x390 in DSI mode Command on LCD screen.

  @verbatim
  ******************************************************************************
  * @file    DSI/DSI_ULPM_Data/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LCD DSI enter and exit DSI ULPM Mode on data lane only example.
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

How to use the embedded LCD DSI controller (using the LTDC and DSI Host IPs) to
drive the KoD LCD mounted on-board and manage entry and exit in DSI ULPM mode
on the data lane only. In this mode, the DSI PHY state machine enters a
low-power state on the data lane, allowing some power saving when the LCD does
not need to display. When the display is needed again, the DSI ULPM on the
data lane is exited, and the display operates as before.

In this example a RGB888 picture (390x390) is displayed on LCD in DSI Mode Command
(LED1 is switched on).

On a Wakeup push-button press from the user within 10 seconds, the LCD display will be
switched off.
One second later,  the DSI PHY Data lane will enter ULPM mode (LED1 will switch off).
After 5 seconds in Off mode, the ULPM on data lane will be exited, the LCD will be
switched back on (LED1 back to on) and the LCD will display the same image as before.

Ten seconds after the image is displayed, a dimming is initiated with a final LCD display off
after ten more seconds.

In case of error, LED2 will be switched on.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

  - DSI/DSI_ULPM_Data/Inc/stm32l4xx_hal_conf.h      HAL configuration file
  - DSI/DSI_ULPM_Data/Inc/stm32l4xx_it.h            Interrupt handlers header file
  - DSI/DSI_ULPM_Data/Inc/main.h                    Header for main.c module
  - DSI/DSI_ULPM_Data/Inc/image_390x390_rgb888.h    Image 390x390 in RGB888 to display on LCD
  - DSI/DSI_ULPM_Data/Src/stm32l4xx_it.c            Interrupt handlers
  - DSI/DSI_ULPM_Data/Src/main.c                    Main program
  - DSI/DSI_ULPM_Data/Src/stm32l4xx_hal_msp.c       HAL MSP file
  - DSI/DSI_ULPM_Data/Src/system_stm32l4xx.c        STM32L4xx system source file

@par Hardware and Software environment

  - This example runs on STM32L4R9xx devices.

  - This example has been tested with STMicroelectronics STM32L4R9I-EVAL
    board and can be easily tailored to any other supported device
    and development board.

  - STM32L4R9I-EVAL Set-up
    - Make sure that round DSI LCD MB1314 is plugged on CN16 and that CN20 is free

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
