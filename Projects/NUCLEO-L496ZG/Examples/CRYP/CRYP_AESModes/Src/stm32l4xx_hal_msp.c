/**
  ******************************************************************************
  * @file    CRYP/CRYP_AESModes/Src/stm32l4xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
  * @brief  Initializes the CRYP MSP.
  *        This function configures the hardware resources used in this example:
  *           - CRYP's clock enable
  * @param  hcryp: CRYP handle pointer
  * @retval None
  */
void HAL_CRYP_MspInit(CRYP_HandleTypeDef *hcryp)
{
  /* Enable CRYP clock */
  __HAL_RCC_AES_CLK_ENABLE();
  
    /* Force the CRYP Periheral Clock Reset */  
  __HAL_RCC_AES_FORCE_RESET(); 
  
  /* Release the CRYP Periheral Clock Reset */  
  __HAL_RCC_AES_RELEASE_RESET();

    /* Enable and set CRYP Interrupt to the highest priority */
    HAL_NVIC_SetPriority(AES_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(AES_IRQn);
 

}

/**
  * @brief  DeInitializes the CRYP MSP.
  * @param  hcryp: CRYP handle pointer
  * @retval None
  */
void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef *hcryp)
{
  /* Disable CRYP clock */
  __HAL_RCC_AES_CLK_DISABLE();

    /* Disable CRYP Interrupt */
    HAL_NVIC_DisableIRQ(AES_IRQn);  

}

/**
* @brief UART MSP Init
* @param huart: uart handle
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
    
    /* Enable UARTx GPIO TX/RX clock */
    UARTx_TX_PIN_CLK_ENABLE();
    UARTx_RX_PIN_CLK_ENABLE();
    
    /* Enable UARTx clock */
    UARTx_CLK_ENABLE();    
    
    /* IOSV bit MUST be set to access GPIO port G[2:15] */
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWREx_EnableVddIO2();    
    
    /* Configure UARTx Rx and Tx as alternate function  */
    GPIO_InitStruct.Pin = UARTx_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = UARTx_TX_AF;
    HAL_GPIO_Init(UARTx_TX_GPIO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = UARTx_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = UARTx_RX_AF;
    HAL_GPIO_Init(UARTx_RX_GPIO_PORT, &GPIO_InitStruct);    
    
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
