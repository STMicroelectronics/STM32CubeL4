/**
  ******************************************************************************
  * @file    HASH/HASH_HMAC_SHA224SHA256_MultiBuffer_DMA/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of multi-buffer HASH and 
  *          HMAC digest calculation applied to SHA224 and SHA256 with DMA transfer.
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

/** @addtogroup HASH_SHA224_SHA256_MultiBuffer
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define INPUT_TAB_SIZE    ((uint32_t) 261)/* The size of the input data "aInput" */
#define KEY_TAB_SIZE      ((uint32_t) 261)

#define HASHTimeout        0xFF
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* HASH handler declaration */
HASH_HandleTypeDef     HashHandle;
/*
  "The hash processor is a fully compliant implementation of the secure
   hash algorithm (SHA-1, SHA-224, SHA-256), the MD5 (message-digest algorithm 5) 
   hash algorithm and the HMAC (keyed-hash message authentication code)
   algorithm suitable for a variety of applications.*** STM32 ***"
*/
__ALIGN_BEGIN uint8_t aInput[INPUT_TAB_SIZE] __ALIGN_END =
                        {0x54,0x68,0x65,0x20,0x68,0x61,0x73,0x68,
                         0x20,0x70,0x72,0x6f,0x63,0x65,0x73,0x73,
                         0x6f,0x72,0x20,0x69,0x73,0x20,0x61,0x20,
                         0x66,0x75,0x6c,0x6c,0x79,0x20,0x63,0x6f,
                         0x6d,0x70,0x6c,0x69,0x61,0x6e,0x74,0x20,
                         0x69,0x6d,0x70,0x6c,0x65,0x6d,0x65,0x6e,
                         0x74,0x61,0x74,0x69,0x6f,0x6e,0x20,0x6f,
                         0x66,0x20,0x74,0x68,0x65,0x20,0x73,0x65,
                         0x63,0x75,0x72,0x65,0x20,0x68,0x61,0x73,
                         0x68,0x20,0x61,0x6c,0x67,0x6f,0x72,0x69,
                         0x74,0x68,0x6d,0x20,0x28,0x53,0x48,0x41,
                         0x2d,0x31,0x29,0x2c,0x20,0x74,0x68,0x65,
                         0x20,0x4d,0x44,0x35,0x20,0x28,0x6d,0x65,
                         0x73,0x73,0x61,0x67,0x65,0x2d,0x64,0x69,
                         0x67,0x65,0x73,0x74,0x20,0x61,0x6c,0x67,
                         0x6f,0x72,0x69,0x74,0x68,0x6d,0x20,0x35,
                         0x29,0x20,0x68,0x61,0x73,0x68,0x20,0x61,
                         0x6c,0x67,0x6f,0x72,0x69,0x74,0x68,0x6d,
                         0x20,0x61,0x6e,0x64,0x20,0x74,0x68,0x65,
                         0x20,0x48,0x4d,0x41,0x43,0x20,0x28,0x6b,
                         0x65,0x79,0x65,0x64,0x2d,0x68,0x61,0x73,
                         0x68,0x20,0x6d,0x65,0x73,0x73,0x61,0x67,
                         0x65,0x20,0x61,0x75,0x74,0x68,0x65,0x6e,
                         0x74,0x69,0x63,0x61,0x74,0x69,0x6f,0x6e,
                         0x20,0x63,0x6f,0x64,0x65,0x29,0x20,0x61,
                         0x6c,0x67,0x6f,0x72,0x69,0x74,0x68,0x6d,
                         0x20,0x73,0x75,0x69,0x74,0x61,0x62,0x6c,
                         0x65,0x20,0x66,0x6f,0x72,0x20,0x61,0x20,
                         0x76,0x61,0x72,0x69,0x65,0x74,0x79,0x20,
                         0x6f,0x66,0x20,0x61,0x70,0x70,0x6c,0x69,
                         0x63,0x61,0x74,0x69,0x6f,0x6e,0x73,0x2e,
                         0x2a,0x2a,0x2a,0x20,0x53,0x54,0x4d,0x33,
                         0x32,0x20,0x2a,0x2a,0x2a};

__ALIGN_BEGIN uint8_t aKey[KEY_TAB_SIZE] __ALIGN_END=
                        {0x54,0x68,0x65,0x20,0x68,0x61,0x73,0x68,
                         0x20,0x70,0x72,0x6f,0x63,0x65,0x73,0x73,
                         0x6f,0x72,0x20,0x69,0x73,0x20,0x61,0x20,
                         0x66,0x75,0x6c,0x6c,0x79,0x20,0x63,0x6f,
                         0x6d,0x70,0x6c,0x69,0x61,0x6e,0x74,0x20,
                         0x69,0x6d,0x70,0x6c,0x65,0x6d,0x65,0x6e,
                         0x74,0x61,0x74,0x69,0x6f,0x6e,0x20,0x6f,
                         0x66,0x20,0x74,0x68,0x65,0x20,0x73,0x65,
                         0x63,0x75,0x72,0x65,0x20,0x68,0x61,0x73,
                         0x68,0x20,0x61,0x6c,0x67,0x6f,0x72,0x69,
                         0x74,0x68,0x6d,0x20,0x28,0x53,0x48,0x41,
                         0x2d,0x31,0x29,0x2c,0x20,0x74,0x68,0x65,
                         0x20,0x4d,0x44,0x35,0x20,0x28,0x6d,0x65,
                         0x73,0x73,0x61,0x67,0x65,0x2d,0x64,0x69,
                         0x67,0x65,0x73,0x74,0x20,0x61,0x6c,0x67,
                         0x6f,0x72,0x69,0x74,0x68,0x6d,0x20,0x35,
                         0x29,0x20,0x68,0x61,0x73,0x68,0x20,0x61,
                         0x6c,0x67,0x6f,0x72,0x69,0x74,0x68,0x6d,
                         0x20,0x61,0x6e,0x64,0x20,0x74,0x68,0x65,
                         0x20,0x48,0x4d,0x41,0x43,0x20,0x28,0x6b,
                         0x65,0x79,0x65,0x64,0x2d,0x68,0x61,0x73,
                         0x68,0x20,0x6d,0x65,0x73,0x73,0x61,0x67,
                         0x65,0x20,0x61,0x75,0x74,0x68,0x65,0x6e,
                         0x74,0x69,0x63,0x61,0x74,0x69,0x6f,0x6e,
                         0x20,0x63,0x6f,0x64,0x65,0x29,0x20,0x61,
                         0x6c,0x67,0x6f,0x72,0x69,0x74,0x68,0x6d,
                         0x20,0x73,0x75,0x69,0x74,0x61,0x62,0x6c,
                         0x65,0x20,0x66,0x6f,0x72,0x20,0x61,0x20,
                         0x76,0x61,0x72,0x69,0x65,0x74,0x79,0x20,
                         0x6f,0x66,0x20,0x61,0x70,0x70,0x6c,0x69,
                         0x63,0x61,0x74,0x69,0x6f,0x6e,0x73,0x2e,
                         0x2a,0x2a,0x2a,0x20,0x53,0x54,0x4d,0x33,
                         0x32,0x20,0x2a,0x2a,0x2a};

												 
__ALIGN_BEGIN	static uint8_t aSHA224Digest[28] __ALIGN_END;

__ALIGN_BEGIN static uint8_t aExpectSHA224Digest[28] __ALIGN_END = {0x76, 0xcc, 0xdd, 0x1f, 0xde, 0x03, 0x6c, 0xde, 0x39, 
                        0xc9, 0xd2, 0x3c, 0xe6, 0xbd, 0xb1, 0x69, 0xff, 0x74, 
						            0x3b, 0x89, 0xd1, 0x57, 0x91, 0xbf, 0xba, 0x10, 0x9a, 0x55};

																			 


__ALIGN_BEGIN static uint8_t aSHA256Digest[32] __ALIGN_END;

__ALIGN_BEGIN static uint8_t ExpectedHMAC224[28] __ALIGN_END  = {0x6b, 0xd3, 0x13, 0x2e, 0x89, 0xc7, 0x71, 0x5c, 0x9c, 
                                                 0xba, 0xee, 0x01, 0x50, 0xd0, 0xb2, 0x5c, 0x57, 0x72, 
                                                 0x97, 0xe4, 0x26, 0x40, 0x31, 0xca, 0xe9, 0x65, 0x60, 0x78};
                              

__ALIGN_BEGIN static uint8_t ExpectedHMAC256[32] __ALIGN_END   = {0xc5, 0x01, 0xfc, 0x6c, 0x17, 0x37, 0x78, 0xe6, 
                                                  0x97, 0x44, 0xae, 0x06, 0x94, 0x4f, 0x09, 0x4c, 
                                                  0x9e, 0xaf, 0x1c, 0x56, 0x2f, 0x58, 0xde, 0xbc, 
                                                  0x47, 0x8c, 0xde, 0xa8, 0x5a, 0x42, 0x1f, 0x58};

																				
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

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

  /* Configure LEDs */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* Configure the system clock to 120 MHz */
  SystemClock_Config();


  /****************************************************************************/
  /*                                                                          */
  /*                Multi-buffer SHA224 digest computation                    */
  /*                                                                          */  
  /****************************************************************************/
  if (HAL_HASH_DeInit(&HashHandle) != HAL_OK)
  {
    Error_Handler();
  }  
  HashHandle.Init.DataType = HASH_DATATYPE_8B;

  if (HAL_HASH_Init(&HashHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set multiple DMA transfer feature. 
     MDMAT bit is set when hashing large files where 
     multiple DMA transfers are needed. */
  __HAL_HASH_SET_MDMAT();
  
  /***********************************************/
  /* Start hashing a sequence of several buffers */
  /***********************************************/  
  if (HAL_HASHEx_SHA224_Start_DMA(&HashHandle, aInput, 64) != HAL_OK)
  {
    Error_Handler();
  }
  while (HAL_HASH_GetState(&HashHandle) == HAL_HASH_STATE_BUSY);
  
  if (HAL_HASHEx_SHA224_Start_DMA(&HashHandle, aInput+64, 64) != HAL_OK)
  {
    Error_Handler();
  }
  while (HAL_HASH_GetState(&HashHandle) == HAL_HASH_STATE_BUSY);
  
  if (HAL_HASHEx_SHA224_Start_DMA(&HashHandle, aInput+128, 64) != HAL_OK)
  {
    Error_Handler();
  }  
  while (HAL_HASH_GetState(&HashHandle) == HAL_HASH_STATE_BUSY);
  
  if (HAL_HASHEx_SHA224_Start_DMA(&HashHandle, aInput+192, 64) != HAL_OK)
  {
    Error_Handler();
  }  
  while (HAL_HASH_GetState(&HashHandle) == HAL_HASH_STATE_BUSY);
  
  
  /************************************************************************/
  /* Before  hashing the last buffer, reset multiple DMA transfer feature */
  /************************************************************************/  
  __HAL_HASH_RESET_MDMAT();
  
  /*******************************/
  /* Last buffer feed to HASH IP */
  /*******************************/
  if (HAL_HASHEx_SHA224_Start_DMA(&HashHandle, aInput+256, 5) != HAL_OK)
  {
    Error_Handler();
  }  
  while (HAL_HASH_GetState(&HashHandle) == HAL_HASH_STATE_BUSY);
  
  /*******************/  
  /* Retrieve digest */
  /*******************/
  if (HAL_HASHEx_SHA224_Finish(&HashHandle, aSHA224Digest, HASHTimeout) != HAL_OK)
  {
    Error_Handler();
  }  

  /*********************************************/ 
  /* Compare computed digest with expected one */
  /*********************************************/
  if (memcmp(aSHA224Digest, aExpectSHA224Digest, sizeof(aExpectSHA224Digest) / sizeof(aExpectSHA224Digest[0])) != 0)
  {
    BSP_LED_On(LED3);
  }
  else
  {



  /****************************************************************************/
  /*                                                                          */
  /*            Multi-buffer SHA224 HMAC digest computation                   */
  /*                                                                          */  
  /****************************************************************************/

 if (HAL_HASH_DeInit(&HashHandle) != HAL_OK)
  {
    Error_Handler();
  }  
  HashHandle.Init.DataType = HASH_DATATYPE_8B;
  HashHandle.Init.pKey = aKey;
  HashHandle.Init.KeySize = 261;  

  if (HAL_HASH_Init(&HashHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  /****************************************************************************/
  /* Start HMAC digest compution in feeding the first input buffer to the IP. */
  /* This achieves step 1 and starts step 2.                                  */
  /* Step 1: inner key feeding to the HASH IP                                 */
  /* Step 2: message feeding to the HASH IP                                   */
  /****************************************************************************/ 
  if (HAL_HMACEx_SHA224_Step1_2_DMA(&HashHandle, aInput, 64) != HAL_OK)
  {
    Error_Handler();
  }
  while (HAL_HASH_GetState(&HashHandle) == HAL_HASH_STATE_BUSY);
  
  /*****************************************************************************/  
  /* Carry out HMAC step 2 processing in feeding the sequence of input buffers */
  /*****************************************************************************/  
  if (HAL_HMACEx_SHA224_Step2_DMA(&HashHandle, aInput+64, 64) != HAL_OK)
  {
    Error_Handler();
  }
  while (HAL_HASH_GetState(&HashHandle) == HAL_HASH_STATE_BUSY);  
  
  if (HAL_HMACEx_SHA224_Step2_DMA(&HashHandle, aInput+128, 64) != HAL_OK)
  {
    Error_Handler();
  }
  while (HAL_HASH_GetState(&HashHandle) == HAL_HASH_STATE_BUSY);
  
  if (HAL_HMACEx_SHA224_Step2_DMA(&HashHandle, aInput+192, 64) != HAL_OK)
  {
    Error_Handler();
  }
  while (HAL_HASH_GetState(&HashHandle) == HAL_HASH_STATE_BUSY); 
 
  /****************************************************************************/
  /* Feed the last buffer and wraps up HMAC step 3 processing.                */
  /* Step 3: outer key (same as the inner key) feeding to the HASH IP         */  
  /****************************************************************************/  
  if (HAL_HMACEx_SHA224_Step2_3_DMA(&HashHandle, aInput+256, 5) != HAL_OK)
  {
    Error_Handler();
  }
  while (HAL_HASH_GetState(&HashHandle) == HAL_HASH_STATE_BUSY);         

  /*******************/  
  /* Retrieve digest */
  /*******************/  
  if (HAL_HASHEx_SHA224_Finish(&HashHandle, aSHA224Digest, HASHTimeout) != HAL_OK)
  {
    Error_Handler();
  }
  
  /*********************************************/ 
  /* Compare computed digest with expected one */
  /*********************************************/  
  if(memcmp(aSHA224Digest, ExpectedHMAC224,  sizeof(aExpectSHA224Digest) / sizeof(aExpectSHA224Digest[0])) != 0)
  {
    BSP_LED_On(LED3);
  }
  else
  {
    BSP_LED_On(LED1);
  }
  }


  /****************************************************************************/
  /*                                                                          */
  /*            Multi-buffer SHA256 HMAC digest computation                   */
  /*                                                                          */  
  /****************************************************************************/
 if (HAL_HASH_DeInit(&HashHandle) != HAL_OK)
  {
    Error_Handler();
  }  
  HashHandle.Init.DataType = HASH_DATATYPE_8B;
  HashHandle.Init.pKey = aKey;
  HashHandle.Init.KeySize = 261;  

  if (HAL_HASH_Init(&HashHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  /*****************************************************************************/
  /* Start HMAC digest compution in feeding the first input buffers to the IP. */
  /* This achieves step 1 and starts step 2.                                   */
  /*****************************************************************************/   
  if (HAL_HMACEx_SHA256_Step1_2_DMA(&HashHandle, aInput, 128) != HAL_OK)
  {
    Error_Handler();
  }
  while (HAL_HASH_GetState(&HashHandle) == HAL_HASH_STATE_BUSY);
 
  /******************************************************************************/
  /* There are only two buffers to hash, so wrap-up the step 2 computation in   */
  /* feeding the second and last buffer then carry out step 3.                  */
  /* When the file to hash is made of only two buffers (case illustrated here), */
  /* there is no need to call HAL_HMACEx_SHAxxx_Step2_DMA() API.                */
  /******************************************************************************/     
  if (HAL_HMACEx_SHA256_Step2_3_DMA(&HashHandle, aInput+128, 133) != HAL_OK)
  {
    Error_Handler();
  }
  while (HAL_HASH_GetState(&HashHandle) == HAL_HASH_STATE_BUSY);         

  /*******************/  
  /* Retrieve digest */
  /*******************/   
  if (HAL_HASHEx_SHA256_Finish(&HashHandle, aSHA256Digest, HASHTimeout) != HAL_OK)
  {
    Error_Handler();
  }
  
  if(memcmp(aSHA256Digest, ExpectedHMAC256, sizeof(aSHA256Digest) / sizeof(aSHA256Digest[0])) != 0)
  {
    Error_Handler();
  } 
  else
  {
    BSP_LED_On(LED2);  
  }   
  
  
 
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 120000000
  *            HCLK(Hz)                       = 120000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 60
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  /* Enable voltage range 1 boost mode for frequency above 80 Mhz */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
  __HAL_RCC_PWR_CLK_DISABLE();

  /* Enable MSI Oscillator and activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* To avoid undershoot due to maximum frequency, select PLL as system clock source */
  /* with AHB prescaler divider 2 as first step */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* AHB prescaler divider at 1 as second step */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while (1)
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

