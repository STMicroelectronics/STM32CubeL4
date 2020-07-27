# STM32CubeL4 MCU Firmware Package

**STM32Cube** is an STMicroelectronics original initiative to ease the developers life by reducing efforts, time and cost.

**STM32Cube** covers the overall STM32 products portfolio. It includes a comprehensive embedded software platform (this repo), delivered for each series (such as the STM32CubeL4 for the STM32L4 series).
   * The CMSIS modules (core and device) corresponding to the ARM tm core implemented in this STM32 product
   * The STM32 HAL-LL drivers : an abstraction drivers layer, the API ensuring maximized portability across the STM32 portfolio 
   * The BSP Drivers of each evaluation or demonstration board provided by this STM32 series 
   * A consistent set of middlewares components such as RTOS, USB, FatFS, Graphics, STM32_TouchSensing_Library ...
   * A full set of software projects (basic examples, applications or demonstrations) for each board provided by this STM32 series
   
The **STM32CubeL4 MCU Package** projects are directly running on the STM32L4 series boards. You can find in each Projects/*Board name* directories a set of software projects (Applications/Demonstration/Examples). 

In this FW Package, the modules **Middlewares/ST/TouchGFX** **Middlewares/ST/STemWin** are not directly accessible. They must be downloaded from a ST server, the respective URL are available in a readme.txt file inside each module.

## Release note

Details about the content of this release are available in the release note [here](https://htmlpreview.github.io/?https://github.com/STMicroelectronics/STM32CubeL4/blob/master/Release_Notes.html).

## Boards available
  * STM32L4 
    * [32L4R9IDISCOVERY](https://www.st.com/en/evaluation-tools/32l4r9idiscovery.html)
    * [32L476GDISCOVERY](https://www.st.com/en/evaluation-tools/32l476gdiscovery.html)
    * [32L496GDISCOVERY](https://www.st.com/en/evaluation-tools/32l496gdiscovery.html)
    * [B-L475E-IOT01A](https://www.st.com/en/evaluation-tools/b-l475e-iot01a.html)
    * [B-L4S5I-IOT01A](https://www.st.com/en/evaluation-tools/b-l4s5i-iot01a.html)
    * [NUCLEO-L4R5ZI](https://www.st.com/en/evaluation-tools/nucleo-l4r5zi.html)
    * [NUCLEO-L4R5ZI-P](https://www.st.com/en/evaluation-tools/nucleo-l4r5zi-p.html)
    * [NUCLEO-L412KB](https://www.st.com/en/evaluation-tools/nucleo-l412kb.html)
    * [NUCLEO-L412RB-P](https://www.st.com/en/evaluation-tools/nucleo-l412rb-p.html)    
    * [NUCLEO-L432KC](https://www.st.com/en/evaluation-tools/nucleo-l432kc.html)
    * [NUCLEO-L433RC-P](https://www.st.com/en/evaluation-tools/nucleo-l433rc-p.html)
    * [NUCLEO-L452RE](https://www.st.com/en/evaluation-tools/nucleo-l452re.html)
    * [NUCLEO-L452RE-P](https://www.st.com/en/evaluation-tools/nucleo-l452re-p.html)
    * [NUCLEO-L476RG](https://www.st.com/en/evaluation-tools/nucleo-l476rg.html)
    * [NUCLEO-L496ZG](https://www.st.com/en/evaluation-tools/nucleo-l496zg.html)
    * [NUCLEO-L496ZG-P](https://www.st.com/en/evaluation-tools/nucleo-l496zg-p.html)
    * [STM32L4R9I-EVAL](https://www.st.com/en/evaluation-tools/stm32l4r9i-eval.html)
    * [STM32L476G-EVAL](https://www.st.com/en/evaluation-tools/stm32l476g-eval.html)

## Troubleshooting

**Caution** : The **Issues** requests are strictly limited to submit problems or suggestions related to the software delivered in this repo 

**For any question** related to the STM32L4 product, the hardware performance, the hardware characteristics, the tools, the environment, you can submit a topic on the [ST Community/STM32 MCUs forum](https://community.st.com/s/group/0F90X000000AXsASAW/stm32-mcus)