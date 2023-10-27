/**
  ******************************************************************************
  * @file    CRYP/CRYP_AESModes/Src/stm32l4xx_hal_msp.c
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
  
  /* USART2 Configuration */
  if(huart->Instance == USART2)
  {
    /* Enable UART2 GPIO TX/RX clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    /* Enable USART2 clock */
    __HAL_RCC_USART2_CLK_ENABLE();    
    
    /* Configure USART2 Rx and Tx as alternate function  */
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM; // GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* Enable and set USART2 Interrupt to the highest priority */
    HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
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

