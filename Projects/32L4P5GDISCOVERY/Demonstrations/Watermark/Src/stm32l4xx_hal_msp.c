/**
  ******************************************************************************
  * @file    Demonstrations/Watermark/Src/stm32l4xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32L4xx_HAL_Driver
  * @{
  */

/** @defgroup HAL_MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OSPI_P1_CS_PIN                   GPIO_PIN_11
#define OSPI_P1_CLK_PIN                  GPIO_PIN_10
#define OSPI_P1_NCLK_PIN                 GPIO_PIN_9
#define OSPI_P1_D0_PIN                   GPIO_PIN_12
#define OSPI_P1_D1_PIN                   GPIO_PIN_13
#define OSPI_P1_D2_PIN                   GPIO_PIN_7
#define OSPI_P1_D3_PIN                   GPIO_PIN_6
#define OSPI_P1_D4_PIN                   GPIO_PIN_4
#define OSPI_P1_D5_PIN                   GPIO_PIN_5
#define OSPI_P1_D6_PIN                   GPIO_PIN_3
#define OSPI_P1_D7_PIN                   GPIO_PIN_7
#define OSPI_P1_DQS_PIN                  GPIO_PIN_6

#define OSPI_P2_CS_PIN                   GPIO_PIN_12
#define OSPI_P2_CLK_PIN                  GPIO_PIN_4
#define OSPI_P2_NCLK_PIN                 GPIO_PIN_5
#define OSPI_P2_D0_PIN                   GPIO_PIN_0
#define OSPI_P2_D1_PIN                   GPIO_PIN_1
#define OSPI_P2_D2_PIN                   GPIO_PIN_2
#define OSPI_P2_D3_PIN                   GPIO_PIN_3
#define OSPI_P2_D4_PIN                   GPIO_PIN_0
#define OSPI_P2_D5_PIN                   GPIO_PIN_1
#define OSPI_P2_D6_PIN                   GPIO_PIN_9
#define OSPI_P2_D7_PIN                   GPIO_PIN_10
#define OSPI_P2_DQS_PIN                  GPIO_PIN_12

#define OSPI_P1_CS_GPIO_PORT             GPIOE
#define OSPI_P1_CLK_GPIO_PORT            GPIOE
#define OSPI_P1_NCLK_GPIO_PORT           GPIOE
#define OSPI_P1_D0_GPIO_PORT             GPIOE
#define OSPI_P1_D1_GPIO_PORT             GPIOE
#define OSPI_P1_D2_GPIO_PORT             GPIOA
#define OSPI_P1_D3_GPIO_PORT             GPIOA
#define OSPI_P1_D4_GPIO_PORT             GPIOD
#define OSPI_P1_D5_GPIO_PORT             GPIOD
#define OSPI_P1_D6_GPIO_PORT             GPIOC
#define OSPI_P1_D7_GPIO_PORT             GPIOD
#define OSPI_P1_DQS_GPIO_PORT            GPIOG

#define OSPI_P2_CS_GPIO_PORT             GPIOG
#define OSPI_P2_CLK_GPIO_PORT            GPIOF
#define OSPI_P2_NCLK_GPIO_PORT           GPIOF
#define OSPI_P2_D0_GPIO_PORT             GPIOF
#define OSPI_P2_D1_GPIO_PORT             GPIOF
#define OSPI_P2_D2_GPIO_PORT             GPIOF
#define OSPI_P2_D3_GPIO_PORT             GPIOF
#define OSPI_P2_D4_GPIO_PORT             GPIOG
#define OSPI_P2_D5_GPIO_PORT             GPIOG
#define OSPI_P2_D6_GPIO_PORT             GPIOG
#define OSPI_P2_D7_GPIO_PORT             GPIOG
#define OSPI_P2_DQS_GPIO_PORT            GPIOF

#define OSPI_P1_CS_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define OSPI_P1_CLK_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOE_CLK_ENABLE()
#define OSPI_P1_NCLK_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE()
#define OSPI_P1_D0_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define OSPI_P1_D1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define OSPI_P1_D2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define OSPI_P1_D3_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define OSPI_P1_D4_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI_P1_D5_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI_P1_D6_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define OSPI_P1_D7_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI_P1_DQS_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOG_CLK_ENABLE()

#define OSPI_P2_CS_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI_P2_CLK_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_P2_NCLK_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_P2_D0_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_P2_D1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_P2_D2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_P2_D3_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_P2_D4_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI_P2_D5_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI_P2_D6_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI_P2_D7_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI_P2_DQS_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()

#define OSPI_P1_CS_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOE_CLK_DISABLE()
#define OSPI_P1_CLK_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOE_CLK_DISABLE()
#define OSPI_P1_NCLK_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOE_CLK_DISABLE()
#define OSPI_P1_D0_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOE_CLK_DISABLE()
#define OSPI_P1_D1_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOE_CLK_DISABLE()
#define OSPI_P1_D2_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define OSPI_P1_D3_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define OSPI_P1_D4_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOD_CLK_DISABLE()
#define OSPI_P1_D5_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOD_CLK_DISABLE()
#define OSPI_P1_D6_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOC_CLK_DISABLE()
#define OSPI_P1_D7_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOD_CLK_DISABLE()
#define OSPI_P1_DQS_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOG_CLK_DISABLE()

#define OSPI_P2_CS_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOG_CLK_DISABLE()
#define OSPI_P2_CLK_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOF_CLK_DISABLE()
#define OSPI_P2_NCLK_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOF_CLK_DISABLE()
#define OSPI_P2_D0_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOF_CLK_DISABLE()
#define OSPI_P2_D1_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOF_CLK_DISABLE()
#define OSPI_P2_D2_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOF_CLK_DISABLE()
#define OSPI_P2_D3_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOF_CLK_DISABLE()
#define OSPI_P2_D4_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOG_CLK_DISABLE()
#define OSPI_P2_D5_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOG_CLK_DISABLE()
#define OSPI_P2_D6_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOG_CLK_DISABLE()
#define OSPI_P2_D7_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOG_CLK_DISABLE()
#define OSPI_P2_DQS_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOF_CLK_DISABLE()

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern OSPI_HandleTypeDef OSPIHandle;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspInit(void)
{
}

/**
  * @brief  DeInitializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspDeInit(void)
{
}



/**
  * @brief DMA2D MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hdma2d: DMA2D handle pointer
  * @retval None
  */
void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef *hdma2d)
{
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  __HAL_RCC_DMA2D_CLK_ENABLE();

  /*##-2- NVIC configuration  ################################################*/
  /* NVIC configuration for DMA2D transfer complete interrupt */
  HAL_NVIC_SetPriority(DMA2D_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2D_IRQn);
}

/**
  * @brief DMA2D MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *        - Reset Peripheral
  *        - Disable the Peripheral's clock
  *        - Deactivate Peripheral IRQ
  * @param hdma2d: DMA2D handle pointer
  * @retval None
  */
void HAL_DMA2D_MspDeInit(DMA2D_HandleTypeDef *hdma2d)
{
  /*##-1- Reset peripherals ##################################################*/
  /* Enable DMA2D reset state */
  __HAL_RCC_DMA2D_FORCE_RESET();

  /* Release DMA2D from reset state */
  __HAL_RCC_DMA2D_RELEASE_RESET();

  /* ##-2- Disable DMA2D Clock ##################################################*/
  __HAL_RCC_DMA2D_CLK_DISABLE();

  /* ##-3- Disable DMA2D IRQ in NVIC #############################################*/
  HAL_NVIC_DisableIRQ(DMA2D_IRQn);
}


/**
  * @brief OSPI MSP Init
  * @param hospi: OSPI handle
  * @retval None
  */
void HAL_OSPI_MspInit(OSPI_HandleTypeDef *hospi)
{
  GPIO_InitTypeDef GPIO_InitStruct;
#if !defined (USE_FPGA_BOARD)
  OSPIM_CfgTypeDef OSPIM_Cfg_Struct;
#endif

  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;

  __HAL_RCC_OSPIM_CLK_ENABLE();

  /* IOSV bit MUST be set to access GPIO port G[2:15] */
  __HAL_RCC_PWR_CLK_ENABLE();
  SET_BIT(PWR->CR2, PWR_CR2_IOSV);

    /* Enable the GPIO clocks */
    OSPI_P1_CS_GPIO_CLK_ENABLE();
    OSPI_P1_CLK_GPIO_CLK_ENABLE();
    OSPI_P1_NCLK_GPIO_CLK_ENABLE();
    OSPI_P1_D0_GPIO_CLK_ENABLE();
    OSPI_P1_D1_GPIO_CLK_ENABLE();
    OSPI_P1_D2_GPIO_CLK_ENABLE();
    OSPI_P1_D3_GPIO_CLK_ENABLE();
    OSPI_P1_D4_GPIO_CLK_ENABLE();
    OSPI_P1_D5_GPIO_CLK_ENABLE();
    OSPI_P1_D6_GPIO_CLK_ENABLE();
    OSPI_P1_D7_GPIO_CLK_ENABLE();
    OSPI_P1_DQS_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;

    /* Configure the OSPI_P1_CS pin */
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Pin  = OSPI_P1_CS_PIN;
    HAL_GPIO_Init(OSPI_P1_CS_GPIO_PORT, &GPIO_InitStruct);

    /* Configure the OSPI_P1_CLK pin */
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Pin  = OSPI_P1_CLK_PIN;
    HAL_GPIO_Init(OSPI_P1_CLK_GPIO_PORT, &GPIO_InitStruct);

    /* Configure the OSPI_P1_NCLK pin */
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Pin  = OSPI_P1_NCLK_PIN;
    HAL_GPIO_Init(OSPI_P1_NCLK_GPIO_PORT, &GPIO_InitStruct);

    /* Configure the OSPI_P1_D0 pin */
    GPIO_InitStruct.Pin  = OSPI_P1_D0_PIN;
    HAL_GPIO_Init(OSPI_P1_D0_GPIO_PORT, &GPIO_InitStruct);

    /* Configure the OSPI_P1_D1 pin */
    GPIO_InitStruct.Pin  = OSPI_P1_D1_PIN;
    HAL_GPIO_Init(OSPI_P1_D1_GPIO_PORT, &GPIO_InitStruct);

    /* Configure the OSPI_P1_D2 pin */
    GPIO_InitStruct.Pin  = OSPI_P1_D2_PIN;
    HAL_GPIO_Init(OSPI_P1_D2_GPIO_PORT, &GPIO_InitStruct);

    /* Configure the OSPI_P1_D3 pin */
    GPIO_InitStruct.Pin  = OSPI_P1_D3_PIN;
    HAL_GPIO_Init(OSPI_P1_D3_GPIO_PORT, &GPIO_InitStruct);

    /* Configure the OSPI_P1_D6 pin */
    GPIO_InitStruct.Pin  = OSPI_P1_D6_PIN;
    HAL_GPIO_Init(OSPI_P1_D6_GPIO_PORT, &GPIO_InitStruct);

    /* Configure the OSPI_P1_D7 pin */
    GPIO_InitStruct.Pin  = OSPI_P1_D7_PIN;
    HAL_GPIO_Init(OSPI_P1_D7_GPIO_PORT, &GPIO_InitStruct);

    /* Configure the OSPI_P1_D4 pin */
    GPIO_InitStruct.Pin  = OSPI_P1_D4_PIN;
    HAL_GPIO_Init(OSPI_P1_D4_GPIO_PORT, &GPIO_InitStruct);

    /* Configure the OSPI_P1_D5 pin */
    GPIO_InitStruct.Pin  = OSPI_P1_D5_PIN;
    HAL_GPIO_Init(OSPI_P1_D5_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Alternate = GPIO_AF3_OCTOSPIM_P1;

    /* Configure the OSPI_P1_DQS pin */
    //GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Pin  = OSPI_P1_DQS_PIN;
    HAL_GPIO_Init(OSPI_P1_DQS_GPIO_PORT, &GPIO_InitStruct);


    OSPIM_Cfg_Struct.ClkPort    = 1;
    OSPIM_Cfg_Struct.DQSPort    = 1;
    OSPIM_Cfg_Struct.IOHighPort = HAL_OSPIM_IOPORT_1_HIGH;
    OSPIM_Cfg_Struct.IOLowPort  = HAL_OSPIM_IOPORT_1_LOW;
    OSPIM_Cfg_Struct.NCSPort    = 1;

  OSPIM_Cfg_Struct.Req2AckTime = 1;

  if (HAL_OSPIM_Config(&OSPIHandle, &OSPIM_Cfg_Struct, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable the OctoSPI memory interface clock */
  __HAL_RCC_OSPI1_CLK_ENABLE();

  /* Reset the OctoSPI memory interface */
  __HAL_RCC_OSPI1_FORCE_RESET();
  __HAL_RCC_OSPI1_RELEASE_RESET();

  /* Enable and set OctoSPI interrupt to the lowest priority */
  HAL_NVIC_SetPriority(OCTOSPI1_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(OCTOSPI1_IRQn);
}


/**
  * @brief OSPI MSP DeInit
  * @param hospi: OSPI handle
  * @retval None
  */
void HAL_OSPI_MspDeInit(OSPI_HandleTypeDef *hospi)
{
  /* Disable OctoSPI interrupts */
  HAL_NVIC_DisableIRQ(OCTOSPI1_IRQn);

  /* Reset the OctoSPI memory interface */
  __HAL_RCC_OSPI1_FORCE_RESET();
  __HAL_RCC_OSPI1_RELEASE_RESET();

  /* Disable the OctoSPI memory interface clock */
  __HAL_RCC_OSPI1_CLK_DISABLE();

  /* De-configure the OctoSPI pins */
  HAL_GPIO_DeInit(OSPI_P2_CS_GPIO_PORT, OSPI_P2_CS_PIN);
  HAL_GPIO_DeInit(OSPI_P2_CLK_GPIO_PORT, OSPI_P2_CLK_PIN);
  HAL_GPIO_DeInit(OSPI_P2_NCLK_GPIO_PORT, OSPI_P2_NCLK_PIN);
  HAL_GPIO_DeInit(OSPI_P2_D0_GPIO_PORT, OSPI_P2_D0_PIN);
  HAL_GPIO_DeInit(OSPI_P2_D1_GPIO_PORT, OSPI_P2_D1_PIN);
  HAL_GPIO_DeInit(OSPI_P2_D2_GPIO_PORT, OSPI_P2_D2_PIN);
  HAL_GPIO_DeInit(OSPI_P2_D3_GPIO_PORT, OSPI_P2_D3_PIN);
  HAL_GPIO_DeInit(OSPI_P2_D4_GPIO_PORT, OSPI_P2_D4_PIN);
  HAL_GPIO_DeInit(OSPI_P2_D5_GPIO_PORT, OSPI_P2_D5_PIN);
  HAL_GPIO_DeInit(OSPI_P2_D6_GPIO_PORT, OSPI_P2_D6_PIN);
  HAL_GPIO_DeInit(OSPI_P2_D7_GPIO_PORT, OSPI_P2_D7_PIN);
  HAL_GPIO_DeInit(OSPI_P2_DQS_GPIO_PORT, OSPI_P2_DQS_PIN);

  /* Disable the GPIO clocks */
  OSPI_P2_CS_GPIO_CLK_DISABLE();
  OSPI_P2_CLK_GPIO_CLK_DISABLE();
  OSPI_P2_NCLK_GPIO_CLK_DISABLE();
  OSPI_P2_D0_GPIO_CLK_DISABLE();
  OSPI_P2_D1_GPIO_CLK_DISABLE();
  OSPI_P2_D2_GPIO_CLK_DISABLE();
  OSPI_P2_D3_GPIO_CLK_DISABLE();
  OSPI_P2_D4_GPIO_CLK_DISABLE();
  OSPI_P2_D5_GPIO_CLK_DISABLE();
  OSPI_P2_D6_GPIO_CLK_DISABLE();
  OSPI_P2_D7_GPIO_CLK_DISABLE();
  OSPI_P2_DQS_GPIO_CLK_DISABLE();

  if (__HAL_RCC_OSPI1_IS_CLK_DISABLED() && __HAL_RCC_OSPI2_IS_CLK_DISABLED())
  {
    __HAL_RCC_OSPIM_FORCE_RESET();
    __HAL_RCC_OSPIM_RELEASE_RESET();

    __HAL_RCC_OSPIM_CLK_DISABLE();
  }
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

