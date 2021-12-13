/**
  ******************************************************************************
  * @file    OPAMP/OPAMP_PGA/Src/stm32l4xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @defgroup OPAMP_PGA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static DMA_HandleTypeDef  hdma_dac;

extern uint32_t            powerstatus;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief DAC MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hdac: DAC handle pointer
  * @retval None
  */
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
  GPIO_InitTypeDef          GPIO_InitStruct;

  /*##-1- Enable peripherals and  Clocks ####################################*/

  /* DAC Periph clock enable */
  __HAL_RCC_DAC1_CLK_ENABLE();
  /* Enable GPIO clock ****************************************/
  __HAL_RCC_GPIOA_CLK_ENABLE();
 /* DMA clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* DAC Channelx GPIO pin configuration */
  GPIO_InitStruct.Pin = GPIO_PIN_5;

  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*##-3- Configure the DMA ##################################################*/
  /* Set the parameters to be configured for Channelx*/

  hdma_dac.Instance = DMA1_Channel4;
  
  hdma_dac.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_dac.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_dac.Init.MemInc = DMA_MINC_ENABLE;
  hdma_dac.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_dac.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_dac.Init.Mode = DMA_CIRCULAR;
  hdma_dac.Init.Priority = DMA_PRIORITY_HIGH;

  hdma_dac.Init.Request = DMA_REQUEST_5;

  if(HAL_OK != HAL_DMA_Init(&hdma_dac))
  {
    while(1);
  }

  /* Associate the initialized DMA handle to the the DAC handle */
  __HAL_LINKDMA(hdac, DMA_Handle2, hdma_dac);

  /*##-4- Configure the NVIC for DMA #########################################*/
  /* Enable the DMA Stream IRQ Channel */
  if (powerstatus == 0)
  {
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
  }
}


/**
  * @brief  DeInitializes the DAC MSP.
  * @param  hdac: pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef *hdac)
{
  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_DAC1_FORCE_RESET();
  __HAL_RCC_DAC1_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the DAC Channel GPIO pin */
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);

  /*##-3- Disable the DMA Streams ############################################*/
  /* De-Initialize the DMA Stream associate to Channel */
  if(hdma_dac.Instance != NULL)
    HAL_DMA_DeInit(&hdma_dac);

  /*##-4- Disable the NVIC for DMA ###########################################*/
  HAL_NVIC_DisableIRQ(DMA1_Channel3_IRQn);
}

/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  /* TIM2 Periph clock enable */
  __HAL_RCC_TIM2_CLK_ENABLE();
}

/**
  * @brief TIM MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
{
  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_TIM2_FORCE_RESET();
  __HAL_RCC_TIM2_RELEASE_RESET();
}

/**
  * @brief OPAMP MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hdac: DAC handle pointer
  * @retval None
  */
void HAL_OPAMP_MspInit(OPAMP_HandleTypeDef *hopamp)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /*##-- Configure peripheral GPIO ##########################################*/  
  /* Input GPIO used as OPAMP1 non inverting input is already configured in
     analog mode in HAL_DAC_MspDeInit() */
  /* Enable GPIO clock  */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure the GPIO used as OPAMP1 output */
  GPIO_InitStruct.Pin = (GPIO_PIN_6);

  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configuration of OPAMP clock */
  __HAL_RCC_OPAMP_CLK_ENABLE();  
}

/**
  * @brief  DeInitializes the OPAMP MSP.
  * @param  hdac: pointer to a OPAMP_HandleTypeDef structure that contains
  *         the configuration information for the specified OPAMP.  
  * @retval None
  */
void HAL_OPAMP_MspDeInit(OPAMP_HandleTypeDef *hopamp)
{
  /* De-initialize the OPAMP1 output GPIO pin */
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);
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

