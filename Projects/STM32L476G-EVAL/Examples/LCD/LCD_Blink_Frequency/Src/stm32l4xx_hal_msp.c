/**
  ******************************************************************************
  * @file    LCD/LCD_Blink_Frequency/Src/stm32l4xx_hal_msp.c
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
  * @brief LCD MSP Init.
  * @param hlcd: LCD handle
  * @retval None
  */
 void HAL_LCD_MspInit(LCD_HandleTypeDef *hlcd)
{
  GPIO_InitTypeDef  GPIO_InitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /*##-1- Enable PWR  peripheral Clock #######################################*/
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /*##-2- Allow Access and Reset the Backup Domain ##########################*/ 
  /* Allow Access to Backup Domain */
  HAL_PWR_EnableBkUpAccess();
  
  /* Reset the Backup Domain */
  __HAL_RCC_BACKUPRESET_FORCE(); 
  __HAL_RCC_BACKUPRESET_RELEASE();
  
  /*##-3- Configure LSE as RTC clock source ###################################*/ 
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  { 
    /* Turn LED3 on */
    BSP_LED_On(LED3);
  }
  __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
  
  
  /*##-4- Enable LCD GPIO Clocks #############################################*/
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  
  /*##-5- Configure peripheral GPIO ##########################################*/
  /* Configure Output for LCD */
  /* Port A */  
  GPIO_InitStruct.Pin       = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6 | \
                              GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | \
                              GPIO_PIN_15;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_LCD;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  
  /* Configure Output for LCD */
  /* Port B */  
  GPIO_InitStruct.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4 | \
                              GPIO_PIN_5 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | \
                              GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | \
                              GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  
  /* Configure Output for LCD */
  /* Port C*/  
  GPIO_InitStruct.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                              GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
                              GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
                              GPIO_PIN_12;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  /* Configure Output for LCD */
  /* Port D*/  
  GPIO_InitStruct.Pin       = GPIO_PIN_2 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | \
                              GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | \
                              GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  /* Configure Output for LCD */
  /* Port E*/  
  GPIO_InitStruct.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
 
  /*##-6- Enable LCD peripheral Clock ########################################*/
  __HAL_RCC_LCD_CLK_ENABLE();
}

/**
  * @brief LCD MSP De-Initialization 
  *        This function freeze the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hlcd: LCD handle pointer
  * @retval None
  */
void HAL_LCD_MspDeInit(LCD_HandleTypeDef *hlcd)
{
  /* Enable LCD reset state */
  __HAL_RCC_LCD_FORCE_RESET();
  
  /* Release LCD from reset state */
  __HAL_RCC_LCD_RELEASE_RESET();
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

