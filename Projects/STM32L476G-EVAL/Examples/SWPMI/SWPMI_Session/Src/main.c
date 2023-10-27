/**
  ******************************************************************************
  * @file    SWPMI_Session/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L4xx SWPMI HAL API to communicate
  *          with card compatible with SWP protocol. 
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


/** @addtogroup SWPMI_Session
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  NO_FRAME  = 0,
  ACT_SYNC_ID,
  ACT_READY,
  OTHER_FRAME
} RxFrameTypeDef;

/* Private define ------------------------------------------------------------*/
/* SWPMI_freq = SWPMI_clk / (((BitRate) + 1)  * 4) */
#define BITRATE     19   /* 100Kbits with PCLK at 8Mhz */

#define TEST_TIMEOUT  2000  /* 2 seconds */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* ACT-Frame SYNC_ID */
const uint32_t aTxBuffer_ACT_SYNC_ID[2]         =  {0xFFFF6904, 0x00000002};

/* ACT-Frame POWER_MODE (Full power mode) */
/*
    	ACT-Frame   	>  	Control field        	62   	Power mode indication                                          	               	
    	            	>  	Power mode           	01   	Full power mode                                                	               	
*/
const uint32_t aTxBuffer_ACT_POWER_MODE_FULL[1] =  {0x00016202};

/* ACT-Frame POWER_MODE (Low power mode) */
/*
    	ACT-Frame   	>  	Control field        	62   	Power mode indication                                          	               	
    	            	>  	Power mode           	00   	Low power mode                                                	               	
*/
const uint32_t aTxBuffer_ACT_POWER_MODE_LOW[1]  =  {0x00006202};

/* U-Frame RESET */
/*
    	U-Frame     	>  	Control field        	F9   	Reset of the data link layer                                   	               	
    	            	>  	Window size          	04   	Sliding window size: 4                                         	               	
    	            	>  	Endpoint capabilities	00   	Selective Reject not supported 
*/
const uint32_t aTxBuffer_U_RESET[1]             =  {0x0004F903};

/* I-Frame OPEN_PIPE */
/*
    	I-Frame     	<  	Control field        	80   	N(S):0, N(R):0                                                 	ACK by: # 26   	
    	HCP packet  	<  	Packet header        	81   	Chaining bit: 1   Pipe ID: 01                                  	               	
    	HCP message 	<  	Message header       	03   	Type: Command   Instruction: ANY_OPEN_PIPE                     	               	
*/
const uint32_t aTxBuffer_I_OPEN_PIPE[1]         =  {0x03818003};

/* 32 bit Data Buffer for expected received frame*/ 
const uint32_t Tab_ExpectedRx_ACT_SYNC_ID[1] =  {0x02FFFF69};
const uint32_t Tab_ExpectedRx_ACT_READY[1] =  {0x00000060};

/* 32 bit Data Buffer for reception*/ 
uint32_t aRxBuffer[8] = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};

/* SWPMI handle */
SWPMI_HandleTypeDef SwpmiHandle;

/* SWPMI status */
volatile RxFrameTypeDef SwpmiStatus = NO_FRAME;

/* Exported function prototypes ----------------------------------------------*/
void     Error_Handler(void);

/* Private function prototypes -----------------------------------------------*/
static void     SystemClock_Config(void);
static uint32_t wBuffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32L4xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 16 MHz */
  SystemClock_Config();
  
  /* Initialize LED *************************/
  BSP_LED_Init(LED1);
  BSP_LED_Off(LED1);

  /* Initialize IO expander ****************/
  BSP_IO_Init();

  /* Configure Smartcard 3/5V pin */
  BSP_IO_ConfigPin(IO2_PIN_7, IO_MODE_OUTPUT);
  /* Select 3V (active low) */
  BSP_IO_WritePin(IO2_PIN_7, 0); /* 3V */
 
  /* Configure Smartcard CMDVCC pin */
  BSP_IO_ConfigPin(IO2_PIN_6, IO_MODE_OUTPUT);
  /* Reset CMDVCC (active low) */
  BSP_IO_WritePin(IO2_PIN_6, 1); /* OFF */

  HAL_Delay(1);
  
  /* Set CMDVCC (active low) */
  BSP_IO_WritePin(IO2_PIN_6, 0); /* ON */
  
  /*##-1- Configure the SWPMI peripheral #####################################*/
  /* SWPMI configured as follow:
      - BitRate = defined by user
      - Tx Buffering mode = None
      - Rx Buffering mode = None */
  SwpmiHandle.Instance = SWPMI1;
  SwpmiHandle.Init.VoltageClass = SWPMI_VOLTAGE_CLASS_B;
  SwpmiHandle.Init.BitRate = BITRATE;
  SwpmiHandle.Init.TxBufferingMode = SWPMI_TX_NO_SOFTWAREBUFFER;
  SwpmiHandle.Init.RxBufferingMode = SWPMI_TX_NO_SOFTWAREBUFFER;
 
  if(HAL_SWPMI_Init(&SwpmiHandle) != HAL_OK)
  {
    Error_Handler();
  }  
  
  /*##-2- Start the rx process under interrupt #####################*/    
  if(HAL_SWPMI_Receive_IT(&SwpmiHandle, (uint32_t *)aRxBuffer, 1) != HAL_OK)
  {
    Error_Handler();  /* Rx error */
  }

  /* Wait for SYNC_ID verification process completion */
  while(SwpmiStatus != ACT_SYNC_ID);
  
  /*##-3- Start the rx process under interrupt #####################*/    
  if(HAL_SWPMI_Receive_IT(&SwpmiHandle, (uint32_t *)aRxBuffer, 1) != HAL_OK)
  {
    Error_Handler();  /* Rx error */
  }
  
  /*##-4- Start the Tx process #####################################*/    
  /* Send ACT_POWER_MODE (Low power) command */
  if(HAL_SWPMI_Transmit(&SwpmiHandle, (uint32_t *)aTxBuffer_ACT_POWER_MODE_LOW, 1, TEST_TIMEOUT) != HAL_OK)
  {
    Error_Handler(); /* Bad Tx */
  }
  
  /* Wait for end of activation from card */
  while(SwpmiStatus != ACT_READY);

  /***** Open session with SWP SHDLC  OPEN-PIPE Command *******/
  
  /*##-5- Start the rx process under interrupt #####################*/    
  if(HAL_SWPMI_Receive_IT(&SwpmiHandle, (uint32_t *)aRxBuffer, 1) != HAL_OK)
  {
    Error_Handler();  /* Rx error */
  }
  
  /*##-6- Start the Tx process #####################################*/    
  /* Send I-Frame OPEN_PIPE */
  if(HAL_SWPMI_Transmit(&SwpmiHandle, (uint32_t *)aTxBuffer_I_OPEN_PIPE, 1, TEST_TIMEOUT) != HAL_OK)
  {
    Error_Handler(); /* Bad Tx */
  }
  
  /*-------------------------------- Idle task ---------------------------------*/
  /* Toggle LED1 */
  while (1)
  {
    BSP_LED_Toggle(LED1);
    HAL_Delay(200);
  }
}


/**
  * @brief Rx Transfer completed callback.
  * @param hswpmi: SWPMI handle
  * @retval None
  */
void HAL_SWPMI_RxCpltCallback(SWPMI_HandleTypeDef *hswpmi)
{
  /* Compare the received data with the expected data */
  
  /* ACT SYNC ID received ? */
  if (wBuffercmp(aRxBuffer, (uint32_t *)Tab_ExpectedRx_ACT_SYNC_ID, 1) != 0)
  {
    /* ACT READY ID received ? */
    if (wBuffercmp(aRxBuffer, (uint32_t *)Tab_ExpectedRx_ACT_READY, 1) != 0)
    {  
			/* Other frame received */
      SwpmiStatus = OTHER_FRAME;
    }
    else
    {
      SwpmiStatus = ACT_READY;
    }
  }
  else
  {
    SwpmiStatus = ACT_SYNC_ID;
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = HSI
  *            SYSCLK(Hz)                     = 16000000
  *            HCLK(Hz)                       = 16000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 1600000
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable HSI Oscillator                                       */
  /* (Default MSI Oscillator enabled at system reset remains ON) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select HSI as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

/**
  * @brief  Compares two buffers of words.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length in words
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint32_t wBuffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
    BufferLength--;
  }

  return 0;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED1 on */
  BSP_LED_On(LED1);
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/**
  * @}
  */ 

/**
  * @}
  */ 

