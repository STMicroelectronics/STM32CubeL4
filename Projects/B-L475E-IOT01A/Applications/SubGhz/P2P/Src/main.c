/**
******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   Main program body.
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
#include "clocks_and_power.h"
#include "p2p_app.h"
#include "stm32l475e_iot01.h"

/** @addtogroup USER
* @{
*/

/* Private typedef -----------------------------------------------------------*/ 
/* Private define ------------------------------------------------------------*/
#define TX_BUFFER_SIZE   20
#define RX_BUFFER_SIZE   96

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t TxLength = TX_BUFFER_SIZE;
uint8_t RxLength = 0;
uint8_t aTransmitBuffer[TX_BUFFER_SIZE] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,\
                                                                16,17,18,19,20};
uint8_t aReceiveBuffer[RX_BUFFER_SIZE] = {0x00};

/* Private function prototypes -----------------------------------------------*/


int main(void);

/* Private functions ---------------------------------------------------------*/
/**
* @brief  Main program.
* @param  None
* @retval None
*/
int main(void)
{
  GPIO_InitTypeDef  gpioinitstruct = {0};

  /* MCU Configuration----------------------------------------------------------*/
  
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  
  /* Configure the system clock */
  SystemClock_Config();  
  SystemCoreClockUpdate();

  /* Initialize LED2*/
  BSP_LED_Init(LED2);
  
  /* Initialize LED1*/
  __HAL_RCC_GPIOA_CLK_ENABLE();
  gpioinitstruct.Pin   = GPIO_PIN_5;
  gpioinitstruct.Mode  = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull  = GPIO_NOPULL;
  gpioinitstruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gpioinitstruct);

  
  HAL_Spirit1_Init();
    
  /* Initialize Buttons*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  
  P2P_Init();
  
  while (1)
  { 
    /* Data communication start */   
    P2P_Process(aTransmitBuffer, TxLength, aReceiveBuffer, RxLength);
  } 
}


#ifdef USE_FULL_ASSERT

/**
* @brief Reports the name of the source file and the source line number
* where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
}

#endif

/**
* @}
*/ 

/**
* @}
*/ 

