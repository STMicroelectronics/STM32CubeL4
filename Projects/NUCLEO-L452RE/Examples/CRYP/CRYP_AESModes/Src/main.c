/**
  ******************************************************************************
  * @file    CRYP/CRYP_AESModes/Src/main.c
  * @author  MCD Application Team   
  * @brief   This example provides a short description of how to use the CRYP
  *          peripheral to encrypt and decrypt data using AES algorithm in 
  *          chaining modes (ECB, CBC, CTR) and with all key sizes.
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

/** @addtogroup CRYP_AESModes
  * @{
  */
  

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


#define AES_TEXT_SIZE     64 /* 16 x 4 */

#define ECB               1
#define CBC               2
#define CTR               3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* CRYP handler declaration */
CRYP_HandleTypeDef     CrypHandle;
/* UART handler declaration */
UART_HandleTypeDef     UartHandle;


/* Key size 128 bits */
uint8_t aAES128key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                          0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
                         };


/* Key size 256 bits */
uint8_t aAES256key[32] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
                          0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                          0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
                          0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
                         };

/* Initialization vector */
uint8_t aInitVector[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                           0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
                          };

/* Plaintext */
uint8_t aPlaintext[AES_TEXT_SIZE] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
                                     0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
                                     0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
                                     0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
                                     0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11,
                                     0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
                                     0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17,
                                     0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
                                    };

uint8_t aPlaintext256[64] = 
                        {0xE2,0xBE,0xC1,0x6B,
                         0x96,0x9F,0x40,0x2E,
                         0x11,0x7E,0x3D,0xE9,
                         0x2A,0x17,0x93,0x73, 
                         0x57,0x8A,0x2D,0xAE,
                         0x9C,0xAC,0x03,0x1E,
                         0xAC,0x6F,0xB7,0x9E,
                         0x51,0x8E,0xAF,0x45, 
                         0x46,0x1C,0xC8,0x30,
                         0x11,0xE4,0x5C,0xA3,
                         0x19,0xC1,0xFB,0xE5,
                         0xEF,0x52,0x0A,0x1A,
                         0x45,0x24,0x9F,0xF6,
                         0x17,0x9B,0x4F,0xDF,
                         0x7B,0x41,0x2B,0xAD,
                         0x10,0x37,0x6C,0xE6};

/* Cyphertext */
/* which is aPlaintext encrypted with AES 128 Mode CBC */
uint8_t aCyphertext[AES_TEXT_SIZE] = {0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46,
                                      0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
                                      0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee,
                                      0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2,
                                      0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b,
                                      0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16,
                                      0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09,
                                      0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 0xe1, 0xa7
                                     };

/* AES_ECB */ 
/* Expected text: Encrypted Data with AES 128 Mode ECB */
uint8_t aEncryptedtextECB[AES_TEXT_SIZE] = {0x3A, 0xD7, 0x7B, 0xB4, 0x0D, 0x7A,
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
uint8_t aDecryptedtextECB[AES_TEXT_SIZE] = {0x6B, 0xC0, 0xBC, 0xE1, 0x2A, 0x45, 
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


/*AES_CBC*/ 
/* Expected text: Encrypted Data with AES 128 Mode CBC */
uint8_t aDecryptedtextCBC[AES_TEXT_SIZE] = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 
                                            0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 
                                            0x73, 0x93, 0x17, 0x2A, 0xAE, 0x2D, 
                                            0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 
                                            0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 
                                            0x8E, 0x51, 0x30, 0xC8, 0x1C, 0x46, 
                                            0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB, 
                                            0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
                                            0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 
                                            0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 
                                            0xE6, 0x6C, 0x37, 0x10};

/*AES_CTR*/  
/* Expected text: Encrypted Data with AES 128 Mode CTR */
uint8_t aEncryptedtextCTR[AES_TEXT_SIZE] = {0x3B, 0x3F, 0xD9, 0x2E, 0xB7, 0x2D, 
                                            0xAD, 0x20, 0x33, 0x34, 0x49, 0xF8, 
                                            0xE8, 0x3C, 0xFB, 0x4A, 0x01, 0x0C, 
                                            0x04, 0x19, 0x99, 0xE0, 0x3F, 0x36, 
                                            0x44, 0x86, 0x24, 0x48, 0x3E, 0x58, 
                                            0x2D, 0x0E, 0xA6, 0x22, 0x93, 0xCF, 
                                            0xA6, 0xDF, 0x74, 0x53, 0x5C, 0x35, 
                                            0x41, 0x81, 0x16, 0x87, 0x74, 0xDF,
                                            0x2D, 0x55, 0xA5, 0x47, 0x06, 0x27, 
                                            0x3C, 0x50, 0xD7, 0xB4, 0xF8, 0xA8, 
                                            0xCD, 0xDC, 0x6E, 0xD7};

/* Expected text: Decrypted Data with AES 128 Mode CTR */
uint8_t aDecryptedtextCTR[AES_TEXT_SIZE] = {0x26, 0xB7, 0xCC, 0x60, 0x18, 0x74, 
                                            0x80, 0xF0, 0x14, 0xE0, 0xB9, 0x72, 
                                            0x89, 0x46, 0xF5, 0x1D, 0xFF, 0xA7, 
                                            0x45, 0xD5, 0xD7, 0x91, 0x8A, 0x44,
                                            0x4F, 0xEA, 0x5A, 0xDE, 0xEA, 0x81, 
                                            0xDB, 0xED, 0xE5, 0x54, 0x59, 0x31, 
                                            0xE6, 0x42, 0xE4, 0x79, 0xC8, 0xD8, 
                                            0x66, 0x06, 0x2E, 0xAF, 0xB3, 0x26,
                                            0xE4, 0x3B, 0x4B, 0xA3, 0xB1, 0x77, 
                                            0x0B, 0x4E, 0x68, 0x91, 0x73, 0xE3, 
                                            0x5E, 0x36, 0xB8, 0x60};
                                            
/* Expected text: Encrypted Data with AES 256 Mode ECB */
uint8_t CiphertextAESECB256[AES_TEXT_SIZE] =  {0xBD,0xD1,0xEE,0xF3,
                                      0x3C,0xA0,0xD2,0xB5,
                                      0x7E,0x5A,0x4B,0x06,
                                      0xF8,0x81,0xB1,0x3D, 
                                      0x10,0xCB,0x1C,0x59,
                                      0x26,0xED,0x10,0xD4,
                                      0x4A,0xA7,0x5B,0xDC,
                                      0x70,0x28,0x36,0x31,
                                      0xB9,0x21,0xED,0xB6,
                                      0xF9,0xF4,0xA6,0x9C,
                                      0xB1,0xE7,0x53,0xF1,
                                      0x1D,0xED,0xAF,0xBE, 
                                      0x7A,0x4B,0x30,0x23,
                                      0xFF,0xF3,0xF9,0x39,
                                      0x8F,0x8D,0x7D,0x06,
                                      0xC7,0xEC,0x24,0x9E}; 

/* Expected text: Encrypted Data with AES 256 Mode CBC */                                      
uint8_t CiphertextAESCBC256[AES_TEXT_SIZE] = {0x04,0x4C,0x8C,0xF5,
                                      0xBA,0xF1,0xE5,0xD6,
                                      0xFB,0xAB,0x9E,0x77,
                                      0xD6,0xFB,0x7B,0x5F, 
                                      0x96,0x4E,0xFC,0x9C,
                                      0x8D,0x80,0xDB,0x7E,
                                      0x7B,0x77,0x9F,0x67,
                                      0x7D,0x2C,0x70,0xC6, 
                                      0x69,0x33,0xF2,0x39,
                                      0xCF,0xBA,0xD9,0xA9,
                                      0x63,0xE2,0x30,0xA5,
                                      0x61,0x14,0x23,0x04, 
                                      0xE2,0x05,0xEB,0xB2,
                                      0xFC,0xE9,0x9B,0xC3,
                                      0x07,0x19,0x6C,0xDA,
                                      0x1B,0x9D,0x6A,0x8C}; 

/* Expected text: Encrypted Data with AES 256 Mode CTR */                                       
uint8_t CiphertextAESCTR256[AES_TEXT_SIZE] = {0xBF,0x84,0x7E,0xDC,
                                      0x4B,0x16,0x79,0xDA,
                                      0x86,0x84,0xCD,0x7E,
                                      0x60,0x38,0x5D,0x98, 
                                      0x8B,0x78,0x77,0xD5,
                                      0x74,0x85,0x8A,0x8D,
                                      0xD5,0xA5,0x13,0x55,
                                      0x30,0x1F,0x82,0x0F, 
                                      0x5C,0x6D,0xE9,0xFF,
                                      0x8D,0x23,0x4B,0xF5,
                                      0x83,0x67,0x8D,0xCC,
                                      0xEA,0x3B,0x7F,0xA8, 
                                      0x63,0x54,0xAF,0xE9,
                                      0xA4,0x9C,0xCB,0x44,
                                      0xFF,0x53,0xE5,0xD1,
                                      0x3E,0xC7,0x6B,0xC0};                                                                                                                                                        

/* Used for storing the encrypted text */
uint8_t aEncryptedtext[AES_TEXT_SIZE];

/* Used for storing the decrypted text */
uint8_t aDecryptedtext[AES_TEXT_SIZE];


/* Private function prototypes -----------------------------------------------*/
static void USART2_Config(void);
static void Display_PlainData(uint32_t datalength);
static void Display_PlainData256(uint32_t datalength);
static void Display_CypherData(uint32_t datalength);
static void Display_EncryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength);
static void Display_DecryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength);
void SystemClock_Config(void);
static void Error_Handler(void);
void data_cmp(uint8_t *EncryptedText, uint8_t *RefText, uint8_t Size);
/* Private functions ---------------------------------------------------------*/
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */



/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
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

  /*##- Configure the CRYP peripheral ######################################*/
  /* Set the common CRYP parameters */
  CrypHandle.Instance = AES;

  /* Display init via USART2 */
  USART2_Config(); 

  /* Insert 100 milliseconds delay */
  HAL_Delay(100);

  /* Display Plain Data*/
  Display_PlainData(AES_TEXT_SIZE);
    
  /* Display Plain Data used with the 256-bit long key */ 
  Display_PlainData256(AES_TEXT_SIZE);
  /* Display Cypher Data*/
  Display_CypherData(AES_TEXT_SIZE);

  BSP_LED_Off(LED2);


    /******************************************************************************/
    /*                             AES mode ECB                                   */
    /******************************************************************************/

    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    }

    /*=====================================================
        Encryption ECB mode
    ======================================================*/

    /*****************  AES 128   ****************/
    /* Initialize the CRYP peripheral */
    CrypHandle.Init.DataType      = CRYP_DATATYPE_8B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_128B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_ENCRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_ECB;
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey          = aAES128key;

    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }

    /* Start encrypting aPlaintext, the cypher data is available in aEncryptedtext */
    if (HAL_CRYPEx_AES(&CrypHandle, aPlaintext, AES_TEXT_SIZE, aEncryptedtext, TIMEOUT_VALUE) == HAL_OK)
    {
      /* Display encrypted Data */
      Display_EncryptedData(ECB, 128, AES_TEXT_SIZE);
    }
    else
    {
      /* Processing Error */
      Error_Handler();
    }
    
    /* Compare the encrypted text with the expected one *************************/ 
    data_cmp(aEncryptedtext, aEncryptedtextECB, AES_TEXT_SIZE);
  

    /*****************  AES 256   ****************/
    CrypHandle.Init.DataType      = CRYP_DATATYPE_32B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_256B;  
    CrypHandle.Init.pKey          = aAES256key;
  
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    } 
    
    if (HAL_CRYPEx_AES_IT(&CrypHandle, aPlaintext256, AES_TEXT_SIZE, aEncryptedtext)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for processing to be done */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
  
  
    /* Display encrypted Data */
    Display_EncryptedData(ECB, 256, AES_TEXT_SIZE);
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aEncryptedtext, CiphertextAESECB256, AES_TEXT_SIZE);
    
    


    /*=====================================================
        Decryption ECB mode
    ======================================================*/
    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    }
    /*****************  AES 128   ****************/
    /* Initialize the CRYP peripheral */
    CrypHandle.Init.DataType      = CRYP_DATATYPE_8B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_128B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_KEYDERIVATION_DECRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_ECB;
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey    = aAES128key;

    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }

    /* Start decrypting aCyphertext, the decrypted data is available in aDecryptedtext */
    if (HAL_CRYPEx_AES(&CrypHandle, aCyphertext, AES_TEXT_SIZE, aDecryptedtext, TIMEOUT_VALUE) == HAL_OK)
    {
      /* Display decrypted Data */
      Display_DecryptedData(ECB, 128, AES_TEXT_SIZE);
    }
    else
    {
      /* Processing Error */
      Error_Handler();
    }
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aDecryptedtext, aDecryptedtextECB, AES_TEXT_SIZE);


    /*****************  AES 256   ****************/
    CrypHandle.Init.DataType      = CRYP_DATATYPE_32B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_256B;  
    CrypHandle.Init.pKey          = aAES256key;
  
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    } 
    
    if (HAL_CRYPEx_AES_IT(&CrypHandle, CiphertextAESECB256, AES_TEXT_SIZE, aDecryptedtext)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for processing to be done */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
  
  
    /* Display decrypted Data */
    Display_DecryptedData(ECB, 256, AES_TEXT_SIZE);
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aDecryptedtext, aPlaintext256, AES_TEXT_SIZE);
    
    


    /******************************************************************************/
    /*                             AES mode CBC                                   */
    /******************************************************************************/

    /*=====================================================
        Encryption CBC mode
    ======================================================*/
    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    }
    /*****************  AES 128   ****************/
    /* Initialize the CRYP peripheral */
    CrypHandle.Init.DataType      = CRYP_DATATYPE_8B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_128B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_ENCRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_CBC;
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey      = aAES128key;
    CrypHandle.Init.pInitVect = aInitVector;

    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }

    /* Start encrypting aPlaintext, the cypher data is available in aEncryptedtext */
    if (HAL_CRYPEx_AES(&CrypHandle, aPlaintext, AES_TEXT_SIZE, aEncryptedtext, TIMEOUT_VALUE) == HAL_OK)
    {
      /* Display encrypted Data */
      Display_EncryptedData(CBC, 128, AES_TEXT_SIZE);
    }
    else  
    {
      /* Processing Error */
      Error_Handler();
    }
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aEncryptedtext, aCyphertext, AES_TEXT_SIZE);


    /*****************  AES 256   ****************/ 
    CrypHandle.Init.DataType      = CRYP_DATATYPE_32B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_256B;  
    CrypHandle.Init.pKey          = aAES256key;
  
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    } 
    
    if (HAL_CRYPEx_AES_IT(&CrypHandle, aPlaintext256, AES_TEXT_SIZE, aEncryptedtext)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for processing to be done */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
  
  
    /* Display encrypted Data */
    Display_EncryptedData(CBC, 256, AES_TEXT_SIZE);
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aEncryptedtext, CiphertextAESCBC256, AES_TEXT_SIZE);
    
    


    /*=====================================================
        Decryption CBC mode
    ======================================================*/
    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    }
    /*****************  AES 128   ****************/
    /* Initialize the CRYP peripheral */
    CrypHandle.Init.DataType      = CRYP_DATATYPE_8B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_128B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_KEYDERIVATION_DECRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_CBC;
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey      = aAES128key;
    CrypHandle.Init.pInitVect = aInitVector;

    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }

    /* Start decrypting aCyphertext, the decrypted data is available in aDecryptedtext */
    if (HAL_CRYPEx_AES(&CrypHandle, aCyphertext, AES_TEXT_SIZE, aDecryptedtext, TIMEOUT_VALUE) == HAL_OK)
    {
      /* Display decrypted Data */
      Display_DecryptedData(CBC, 128, AES_TEXT_SIZE);
    }
    else   
    {
      /* Processing Error */
      Error_Handler();
    }
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aDecryptedtext, aDecryptedtextCBC, AES_TEXT_SIZE);


    /*****************  AES 256   ****************/ 
    CrypHandle.Init.DataType      = CRYP_DATATYPE_32B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_256B;  
    CrypHandle.Init.pKey          = aAES256key;
  
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    } 
    
    if (HAL_CRYPEx_AES_IT(&CrypHandle, CiphertextAESCBC256, AES_TEXT_SIZE, aDecryptedtext)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for processing to be done */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
  
  
    /* Display decrypted Data */
    Display_DecryptedData(CBC, 256, AES_TEXT_SIZE);
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aDecryptedtext, aPlaintext256, AES_TEXT_SIZE);
    
    


    /******************************************************************************/
    /*                             AES mode CTR                                   */
    /******************************************************************************/

    /*=====================================================
        Encryption CTR mode
    ======================================================*/
    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    }
    /*****************  AES 128   ****************/
    /* Initialize the CRYP peripheral */
    CrypHandle.Init.DataType      = CRYP_DATATYPE_8B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_128B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_ENCRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_CTR;
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey      = aAES128key;
    CrypHandle.Init.pInitVect = aInitVector;

    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }

    /* Start encrypting aPlaintext, the cypher data is available in aEncryptedtext */
    if (HAL_CRYPEx_AES(&CrypHandle, aPlaintext, AES_TEXT_SIZE, aEncryptedtext, TIMEOUT_VALUE) == HAL_OK)
    {
      /* Display encrypted Data */
      Display_EncryptedData(CTR, 128, AES_TEXT_SIZE);
    }
    else     
    {
      /* Processing Error */
      Error_Handler();
    }
    
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aEncryptedtext, aEncryptedtextCTR, AES_TEXT_SIZE);


    /*****************  AES 256   ****************/ 
    CrypHandle.Init.DataType      = CRYP_DATATYPE_32B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_256B;  
    CrypHandle.Init.pKey          = aAES256key;
  
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    } 
    
    if (HAL_CRYPEx_AES_IT(&CrypHandle, aPlaintext256, AES_TEXT_SIZE, aEncryptedtext)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for processing to be done */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
  
  
    /* Display encrypted Data */
    Display_EncryptedData(CTR, 256, AES_TEXT_SIZE);
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aEncryptedtext, CiphertextAESCTR256, AES_TEXT_SIZE);
    
    

    /*=====================================================
        Decryption CTR mode
    ======================================================*/
    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    }
    /*****************  AES 128   ****************/
    /* Initialize the CRYP peripheral */
    CrypHandle.Init.DataType      = CRYP_DATATYPE_8B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_128B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_DECRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_CTR;
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey      = aAES128key;
    CrypHandle.Init.pInitVect = aInitVector;

    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }

    /* Start decrypting aCyphertext, the decrypted data is available in aDecryptedtext */
    if (HAL_CRYPEx_AES(&CrypHandle, aCyphertext, AES_TEXT_SIZE, aDecryptedtext, TIMEOUT_VALUE) == HAL_OK)
    {
      /* Display decrypted Data */
      Display_DecryptedData(CTR, 128, AES_TEXT_SIZE);
    }
    else  
    {
      /* Processing Error */
      Error_Handler();

    }

    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aDecryptedtext, aDecryptedtextCTR, AES_TEXT_SIZE);
  

    /*****************  AES 256   ****************/
    CrypHandle.Init.DataType      = CRYP_DATATYPE_32B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_256B;  
    CrypHandle.Init.pKey          = aAES256key;
  
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    } 
    
    if (HAL_CRYPEx_AES_IT(&CrypHandle, CiphertextAESCTR256, AES_TEXT_SIZE, aDecryptedtext)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for processing to be done */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
  
  
    /* Display decrypted Data */
    Display_DecryptedData(CTR, 256, AES_TEXT_SIZE);
    /* Check the encrypted text with the expected one *************************/ 
    data_cmp(aDecryptedtext, aPlaintext256, AES_TEXT_SIZE);
    
    



  printf("\n\r ===================================================\n ");
  printf("\n\r ECB, CBC and CTR encryptions/decryptions done.\n ");
  printf("\n\r No issue detected.\n ");
  
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  
  while (1)
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
  printf(" ================= Crypt Using HW Crypto  ====================\n\r");
  printf(" =============================================================\n\r");
  printf(" -----------------------------------------------\n\r");
  printf(" Plain Data (Input data for AES 128 encryption):\n\r");
  printf(" -----------------------------------------------\n\r");

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
  * @brief  Display Plain Data
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_PlainData256(uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;

  printf("\n\r  =============================================================\n\r");
  printf(" -----------------------------------------------\n\r");
  printf(" Plain Data (Input data for AES 256 encryption):\n\r");
  printf(" -----------------------------------------------\n\r");

  for (BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
  {
    printf("[0x%02X]", aPlaintext256[BufferCounter]);
    count++;

    if (count == 16)
    {
      count = 0;
      printf("  Block %lu \n\r", BufferCounter / 16);
    }
  }
}

/**
  * @brief  Display Cypher data
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_CypherData(uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;

  printf("\n\r =============================================================\n\r");
  printf(" ------------------------------------------------\n\r");
  printf(" Cypher Data (Input data for AES 128 decryption):\n\r");
  printf(" ------------------------------------------------\n\r");

  for (BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
  {
    printf("[0x%02X]", aCyphertext[BufferCounter]);
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
    printf("[0x%02X]", aEncryptedtext[BufferCounter]);

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
    printf("[0x%02X]", aDecryptedtext[BufferCounter]);
    count++;

    if (count == 16)
    {
      count = 0;
      printf(" Block %lu \n\r", BufferCounter / 16);
    }
  }
}


/**
  * @brief  buffer data comparison
  * @param  
  * @retval None
  */
void data_cmp(uint8_t *EncryptedText, uint8_t *RefText, uint8_t Size) 
{
  /*  Before starting a new process, you need to check the current state of the peripheral; 
      if it’s busy you need to wait for the end of current transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the 
      process, but application may perform other tasks while transfer operation
      is ongoing. */ 
  while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY)
  {
  }
  
  /*##-3- Check the encrypted text with the expected one #####################*/
  if(memcmp(EncryptedText, RefText, Size) != 0)
  {
    Error_Handler();
  }
  else
  {
    /* Right encryption */
  }
}

/**
  * @brief  Configure USART2(in UART mode)
  * @param  None
  * @retval None
  */
  /*
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Tx and Rx enabled
 */
static void USART2_Config(void)
{
  UartHandle.Instance = USART2;
  UartHandle.Init.BaudRate = 9600;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits = UART_STOPBITS_1;
  UartHandle.Init.Parity = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  UartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;

  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  Retargets the C library printf function to the UART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF); 

  return ch;
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

