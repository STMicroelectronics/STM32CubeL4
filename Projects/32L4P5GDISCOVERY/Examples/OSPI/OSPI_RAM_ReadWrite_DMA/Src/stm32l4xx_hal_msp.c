/**
  ******************************************************************************
  * @file    OSPI/OSPI_RAM_ReadWrite_DMA/Src/stm32l4xx_hal_msp.c
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

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @defgroup HAL_MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief OSPI MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for requests by peripheral
  *           - NVIC configuration for DMA and OSPI interrupts
  * @param hospi: OSPI handle pointer
  * @retval None
  */
void HAL_OSPI_MspInit(OSPI_HandleTypeDef *hospi)
{
  static DMA_HandleTypeDef dma_handle;
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable DMAx clock */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMAMUX1_CLK_ENABLE();

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
    GPIO_InitStruct.Pin  = OSPI_P1_DQS_PIN;
    HAL_GPIO_Init(OSPI_P1_DQS_GPIO_PORT, &GPIO_InitStruct);

  /* Enable the OctoSPI memory interface clock */
  __HAL_RCC_OSPI1_CLK_ENABLE();

  /* Reset the OctoSPI memory interface */
  __HAL_RCC_OSPI1_FORCE_RESET();
  __HAL_RCC_OSPI1_RELEASE_RESET();

  /* Enable and set OctoSPI interrupt to the lowest priority */
  HAL_NVIC_SetPriority(OCTOSPI1_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(OCTOSPI1_IRQn);

  /* Configure common DMA parameters */
  dma_handle.Init.PeriphInc           = DMA_PINC_DISABLE;
  dma_handle.Init.MemInc              = DMA_MINC_ENABLE;
  dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  dma_handle.Init.MemDataAlignment    = DMA_PDATAALIGN_BYTE;
  dma_handle.Init.Mode                = DMA_NORMAL;
  dma_handle.Init.Priority            = DMA_PRIORITY_HIGH;
  /* Configure the OctoSPI DMA */
  dma_handle.Init.Request = DMA_REQUEST_OCTOSPI1;

  dma_handle.Instance = DMA1_Channel5;

  /* Deinitialize the Channel for new transfer */
  HAL_DMA_DeInit(&dma_handle);

    /* Initialize the DMA channel */
  __HAL_LINKDMA(hospi, hdma, dma_handle);

  /* Configure the DMA Channel */
  HAL_DMA_Init(&dma_handle);


  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);

  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);


}

/**
  * @brief OSPI MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hospi: OSPI handle pointer
  * @retval None
  */
void HAL_OSPI_MspDeInit(OSPI_HandleTypeDef *hospi)
{
  static DMA_HandleTypeDef dma_handle;

  /* Disable NVIC configuration for DMA interrupt */
  HAL_NVIC_DisableIRQ(DMA1_Channel5_IRQn);

  /* Deinitialize the stream for new transfer */
  dma_handle.Instance = DMA1_Channel5;
  HAL_DMA_DeInit(&dma_handle);

  /*##-1- Disable the NVIC for OSPI and DMA ##################################*/
  HAL_NVIC_DisableIRQ(OCTOSPI1_IRQn);

  /*##-2- Disable peripherals ################################################*/
  /* De-Configure OSPI pins */
 HAL_GPIO_DeInit(OSPI_P1_CS_GPIO_PORT, OSPI_P1_CS_PIN);
  HAL_GPIO_DeInit(OSPI_P1_CLK_GPIO_PORT, OSPI_P1_CLK_PIN);
  HAL_GPIO_DeInit(OSPI_P1_NCLK_GPIO_PORT, OSPI_P1_NCLK_PIN);
  HAL_GPIO_DeInit(OSPI_P1_D0_GPIO_PORT, OSPI_P1_D0_PIN);
  HAL_GPIO_DeInit(OSPI_P1_D1_GPIO_PORT, OSPI_P1_D1_PIN);
  HAL_GPIO_DeInit(OSPI_P1_D2_GPIO_PORT, OSPI_P1_D2_PIN);
  HAL_GPIO_DeInit(OSPI_P1_D3_GPIO_PORT, OSPI_P1_D3_PIN);
  HAL_GPIO_DeInit(OSPI_P1_D4_GPIO_PORT, OSPI_P1_D4_PIN);
  HAL_GPIO_DeInit(OSPI_P1_D5_GPIO_PORT, OSPI_P1_D5_PIN);
  HAL_GPIO_DeInit(OSPI_P1_D6_GPIO_PORT, OSPI_P1_D6_PIN);
  HAL_GPIO_DeInit(OSPI_P1_D7_GPIO_PORT, OSPI_P1_D7_PIN);
  HAL_GPIO_DeInit(OSPI_P1_DQS_GPIO_PORT, OSPI_P1_DQS_PIN);

  /*##-3- Reset peripherals ##################################################*/
  /* Reset the OctoSPI memory interface */
  __HAL_RCC_OSPI1_FORCE_RESET();
  __HAL_RCC_OSPI1_RELEASE_RESET();


  /* Disable the OctoSPI memory interface clocks */
  __HAL_RCC_OSPI1_CLK_ENABLE();
  __HAL_RCC_OSPIM_CLK_DISABLE();
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

