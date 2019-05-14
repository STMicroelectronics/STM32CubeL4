/**
  @page AWS AWS Cloud demonstration

  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of AWS Cloud demonstration.
  ******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Demonstration description

The B-L475E-IOT01A board embedded demonstration is available in source code under
https://www.st.com/en/embedded-software/x-cube-aws.html

You can possibly upload your board with a more recent software version using this link.
This package starts with a STM32CubeExpansion_Cloud_AWS_VX.Y.Z directory.
STM32CubeExpansion_Cloud_AWS_VX.Y.Z\Projects\B-L475E-IOT01A\Applications\Cloud\AWS\readme.txt
will provide you with the details of the embedded demonstration.

It implements an Amazon AWS Cloud IoT client for the B-L475E-IOT01 board using
the on-board Inventek ISM43362 Wi-Fi module connectivity, and mbedTLS for secure
network communication.

The application connects to Amazon AWS IoT Cloud with the credentials provided
by the user. When the User button is pushed, it sends a LED toggle command to the
IoT Cloud endpoint, which returns back the message to the board and trigs the LED toggle.

The board sensor data are also collected and published to the cloud.

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
