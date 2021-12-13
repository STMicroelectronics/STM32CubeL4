/**
  ******************************************************************************
  * @file    CRYP/CRYP_DMA/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the CRYPTO
  *          peripheral to encrypt and decrypt data using CRYP Algorithm with
  *          AES ECB chaining mode.
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
#if defined(__GNUC__)
#include <stdio.h>
#include <stdlib.h>
#endif

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup CRYP_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define KEY_SIZE          128 /* Key size in bits */

/* The size of the plaintext in Byte */
#define PLAINTEXT_SIZE          ((uint32_t)64)
#define AES_TEXT_SIZE           ((uint32_t)64)

#define ECB               1
#define CBC               2
#define CTR               3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* CRYP handler declaration */
CRYP_HandleTypeDef     CrypHandle;


/* Key size 128 bits */
uint8_t aAES128Key[16] = 
                         {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                          0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
                         };

/* Plaintext */
uint8_t aPlaintext[PLAINTEXT_SIZE]  =    {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,
                                     0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
                                     0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
                                     0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
                                     0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11,
                                     0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
                                     0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17,
                                     0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
                                    };
/* Cyphertext */
uint8_t aCyphertext[AES_TEXT_SIZE] =     {0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46,
                                      0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
                                      0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee,
                                      0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2,
                                      0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b,
                                      0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16,
                                      0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09,
                                      0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 0xe1, 0xa7
                                     };

/* Expected text: Encrypted Data with AES 128 Mode ECB */
uint8_t aEncryptedtextExpected[AES_TEXT_SIZE] = {0x3A, 0xD7, 0x7B, 0xB4, 0x0D, 0x7A,
                                            0x36, 0x60, 0xA8, 0x9E, 0xCA, 0xF3,
                                            0x24, 0x66, 0xEF, 0x97, 0xF5, 0xD3,
                                            0xD5, 0x85, 0x03, 0xB9, 0x69, 0x9D,
                                            0xE7, 0x85, 0x89, 0x5A, 0x96, 0xFD,
                                            0xBA, 0xAF, 0x43, 0xB1, 0xCD, 0x7F,
                                            0x59, 0x8E, 0xCE, 0x23, 0x88, 0x1B,
                                            0x00, 0xE3, 0xED, 0x03, 0x06, 0x88, 
                                            0x7B, 0x0C, 0x78, 0x5E, 0x27, 0xE8, 
                                            0xAD, 0x3F, 0x82, 0x23, 0x20, 0x71, 
                                            0x04, 0x72, 0x5D, 0xD4};

/* Expected text: Decrypted Data with AES 128 Mode ECB */
uint8_t aDecryptedtextExpected[PLAINTEXT_SIZE] = {0x6B, 0xC0, 0xBC, 0xE1, 0x2A, 0x45, 
                                            0x99, 0x91, 0xE1, 0x34, 0x74, 0x1A, 
                                            0x7F, 0x9E, 0x19, 0x25, 0xD8, 0x64, 
                                            0x21, 0xFB, 0x9F, 0x1A, 0x1E, 0xDA, 
                                            0x50, 0x5E, 0xE1, 0x37, 0x57, 0x46, 
                                            0x97, 0x2C, 0x60, 0x4E, 0xD7, 0xDD, 
                                            0xF3, 0x2E, 0xFD, 0xFF, 0x70, 0x20, 
                                            0xD0, 0x23, 0x8B, 0x7C, 0x2A, 0x5D,
                                            0x85, 0x21, 0xF2, 0xFD, 0x3C, 0x8E, 
                                            0xEF, 0x2C, 0xDC, 0x3D, 0xA7, 0xE5, 
                                            0xC4, 0x4E, 0xA2, 0x06};

/* Used for storing the encrypted text */
uint8_t aEncryptedText[AES_TEXT_SIZE] = {0};

/* Used for storing the decrypted text */
uint8_t aDecryptedText[PLAINTEXT_SIZE] = {0};


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);
static void Display_PlainData(uint32_t datalength);
static void Display_EncryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength);
static void Display_DecryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength);
/* Private functions ---------------------------------------------------------*/
#if defined(__GNUC__)
extern void initialise_monitor_handles(void);
#endif

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
#if defined(__GNUC__)
  initialise_monitor_handles(); 
#endif

  /* STM32L4xx HAL library initialization:
       - Configure the Flash prefetch and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 80 MHz */
  SystemClock_Config();
  

  /* Configure LED */
  BSP_LED_Init(LED2);

   /*#######################################################################*/
   /*                                                                       */
   /*##-         DMA-based AES 128 ECB encryption              #############*/
   /*                                                                       */   
   /*#######################################################################*/
  /*##- Configure the CRYP peripheral ######################################*/
  if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  CrypHandle.Instance = AES;
  CrypHandle.Init.DataType      = CRYP_DATATYPE_8B;
  CrypHandle.Init.pKey          = aAES128Key;
  CrypHandle.Init.KeySize       = CRYP_KEYSIZE_128B;
  CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_ENCRYPT;
  CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_ECB;
  CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  

  if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Display Plain Data*/
  Display_PlainData(PLAINTEXT_SIZE);
  


  if (HAL_CRYPEx_AES_DMA(&CrypHandle, aPlaintext, PLAINTEXT_SIZE, aEncryptedText) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler(); 
  }

  /*  Before starting a new process, the current state of the peripheral is checked; 
      as long as the state is not set back to READY, no new ciphering processing
      can be started.
      For simplicity's sake, this example is just waiting till the end of the 
      process, but application may perform other tasks while cihering operation
      is ongoing. */ 
  while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY)
  {
  }
  
      /* Display encrypted Data */
      Display_EncryptedData(ECB, 128, AES_TEXT_SIZE);
  
  /*##- Compare the encrypted text with the expected one #####################*/
  if(memcmp(aEncryptedText, aEncryptedtextExpected, PLAINTEXT_SIZE) != 0)
  {
    Error_Handler(); 
  }
  else
  {
    /* Correct encryption */
  }
  
  
   /*#######################################################################*/
   /*                                                                       */
   /*##-         DMA-based AES 128 ECB decryption              #############*/
   /*                                                                       */   
   /*#######################################################################*/  
  /*##-  Set AES IP in deciphering mode with key derivation ################*/
  CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_KEYDERIVATION_DECRYPT;  
  
  if(HAL_CRYP_Init(&CrypHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  if (HAL_CRYPEx_AES_DMA(&CrypHandle, aCyphertext, PLAINTEXT_SIZE, aDecryptedText) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler(); 
  }
  
  /*  Before starting a new process, the current state of the peripheral is checked; 
      as long as the state is not set back to READY, no new ciphering processing
      can be started.
      For simplicity's sake, this example is just waiting till the end of the 
      process, but application may perform other tasks while cihering operation
      is ongoing. */ 
  while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY)
  {
  }
  
      /* Display decrypted Data */
      Display_DecryptedData(ECB, 128, PLAINTEXT_SIZE);

  /*##- Compare the decrypted text with the expected one #####################*/
  if(memcmp(aDecryptedText, aDecryptedtextExpected, PLAINTEXT_SIZE) != 0)
  {
  
    Error_Handler(); 
  }
  else
  {
    /* Correct decryption */
  }
  
  printf("======================================================\n");
  printf("\n\r DMA-based AES 128 ECB encryption/decryption done.\n");
  printf("\n\r No issue detected.\n");

  /* Turn LED2 on */
  BSP_LED_On(LED2);

  /* Infinite loop */
  while(1)
  {
  }

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
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
  printf("\n\r Error Detected...\n ");  

  while(1)
  {
    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);
    HAL_Delay(200);
  }
}

/**
  * @brief  Display Plain Data
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_PlainData(uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;

  printf("\n\r =============================================================\n\r");
  printf(" ================== Crypt Using HW Cryp  =====================\n\r");
  printf(" =============================================================\n\r");
  printf(" ---------------------------------------\n\r");
  printf(" Plain Data :\n\r");
  printf(" ---------------------------------------\n\r");

  for (BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
  {
    printf("[0x%02X]", aPlaintext[BufferCounter]);
    count++;

    if (count == 16)
    {
      count = 0;
      printf("  Block %lu \n\r", BufferCounter / 16);
    }
  }
}

/**
  * @brief  Display Encrypted Data
  * @param  mode: chaining mode
  * @param  keysize: AES key size used
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_EncryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;

  printf("\n\r =======================================\n\r");
  printf(" Encrypted Data with AES %d  Mode  ", keysize);

  if (mode == ECB)
  {
    printf("ECB\n\r");
  }
  else if (mode == CBC)
  {
    printf("CBC\n\r");
  }
  else /* if(mode == CTR)*/
  {
    printf("CTR\n\r");
  }

  printf(" ---------------------------------------\n\r");

  for (BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
  {
    printf("[0x%02X]", aEncryptedText[BufferCounter]);

    count++;
    if (count == 16)
    {
      count = 0;
      printf(" Block %lu \n\r", BufferCounter / 16);
    }
  }
}

/**
  * @brief  Display Decrypted Data
  * @param  mode: chaining mode
  * @param  keysize: AES key size used
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_DecryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;

  printf("\n\r =======================================\n\r");
  printf(" Decrypted Data with AES %d  Mode  ", keysize);

  if (mode == ECB)
  {
    printf("ECB\n\r");
  }
  else if (mode == CBC)
  {
    printf("CBC\n\r");
  }
  else /* if(mode == CTR)*/
  {
    printf("CTR\n\r");
  }

  printf(" ---------------------------------------\n\r");

  for (BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
  {
    printf("[0x%02X]", aDecryptedText[BufferCounter]);
    count++;

    if (count == 16)
    {
      count = 0;
      printf(" Block %lu \n\r", BufferCounter / 16);
    }
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

