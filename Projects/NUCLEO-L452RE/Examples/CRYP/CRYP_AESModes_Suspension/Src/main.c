/**
  ******************************************************************************
  * @file    CRYP/CRYP_AESModes_Suspension/Src/main.c
  * @author  MCD Application Team   
  * @brief   This example provides a short description of how to use the CRYP
  *          AES peripheral to suspend and resume the AES ECB, CBC and CTR
  *          processing.   
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

/** @addtogroup CRYP_AESModes_Suspension
  * @{
  */
  

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define ECB_MSG_SIZE       64 /* in bytes  */
#define CBC_MSG_SIZE       64 /* in bytes  */
#define CTR_MSG_SIZE       64 /* in bytes  */


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* CRYP handler declaration */
CRYP_HandleTypeDef     CrypHandle;


/******************************************************************************/
/*                                                                            */
/*                             Encryption keys                                */
/*                                                                            */
/******************************************************************************/

  uint8_t aAES128key[16] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
                           0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c}; /* key size 128 bits */  

  uint8_t aAES256key[32] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,
                           0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,
                           0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,
                           0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4}; /* key size 256 bits */   

/******************************************************************************/
/*                                                                            */
/*                    ECB input / expected output data                        */
/*                                                                            */
/******************************************************************************/

  /************************/
  /* low priority-message */
  /************************/

  
  uint8_t aPlaintext_1_ECB[ECB_MSG_SIZE] = 
                        {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,
                         0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,
                         0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,
                         0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,
                         0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,
                         0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,
                         0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,
                         0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10}; /* plaintext */
  
  uint8_t aCiphertext_1_ECB[ECB_MSG_SIZE] = 
                        {0x3A,0xD7,0x7B,0xB4,0x0D,0x7A,0x36,0x60,
                         0xA8,0x9E,0xCA,0xF3,0x24,0x66,0xEF,0x97,
                         0xF5,0xD3,0xD5,0x85,0x03,0xB9,0x69,0x9D,
                         0xE7,0x85,0x89,0x5A,0x96,0xFD,0xBA,0xAF,
                         0x43,0xB1,0xCD,0x7F,0x59,0x8E,0xCE,0x23,
                         0x88,0x1B,0x00,0xE3,0xED,0x03,0x06,0x88, 
                         0x7B,0x0C,0x78,0x5E,0x27,0xE8,0xAD,0x3F,
                         0x82,0x23,0x20,0x71,0x04,0x72,0x5D,0xD4}; /* ciphertext */
                                              
  

  /*************************/
  /* high priority-message */
  /*************************/  

  uint8_t aPlaintext_2_ECB[ECB_MSG_SIZE] = 
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
                         0x10,0x37,0x6C,0xE6}; /* plaintext */                         
  

  uint8_t aCiphertext_2_ECB[ECB_MSG_SIZE] =  {0xBD,0xD1,0xEE,0xF3,
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
                                      
                                                  



/******************************************************************************/
/*                                                                            */
/*                    CBC input / expected output data                        */
/*                                                                            */
/******************************************************************************/

  /************************/
  /* low priority-message */
  /************************/

  
  uint8_t aPlaintext_1_CBC[CBC_MSG_SIZE] = 
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
                         0x10,0x37,0x6C,0xE6}; /* plaintext */  
  
  uint8_t aCiphertext_1_CBC[CBC_MSG_SIZE] = {0x04,0x4C,0x8C,0xF5,
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
                                      0x1B,0x9D,0x6A,0x8C};  /* ciphertext */
                                              
                           
  uint8_t aIV_1[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
                    0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f}; /* initialization vector */    

  /*************************/
  /* high priority-message */
  /*************************/  

  uint8_t aPlaintext_2_CBC[CBC_MSG_SIZE] = 
                        {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,
                         0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,
                         0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,
                         0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,
                         0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,
                         0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,
                         0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,
                         0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10}; /* plaintext */                     
  

  uint8_t aCiphertext_2_CBC[CBC_MSG_SIZE] =  
                        {0x76,0x49,0xAB,0xAC,0x81,0x19,0xB2,0x46,
                         0xCE,0xE9,0x8E,0x9B,0x12,0xE9,0x19,0x7D,
                         0x50,0x86,0xCB,0x9B,0x50,0x72,0x19,0xEE,
                         0x95,0xDB,0x11,0x3A,0x91,0x76,0x78,0xB2,
                         0x73,0xBE,0xD6,0xB8,0xE3,0xC1,0x74,0x3B,
                         0x71,0x16,0xE6,0x9E,0x22,0x22,0x95,0x16,
                         0x3F,0xF1,0xCA,0xA1,0x68,0x1F,0xAC,0x09,
                         0x12,0x0E,0xCA,0x30,0x75,0x86,0xE1,0xA7}; /* ciphertext */
                         
  uint8_t aIV_2[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
                     0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f}; /* initialization vector */                          
                                  


/******************************************************************************/
/*                                                                            */
/*                    CTR input / expected output data                        */
/*                                                                            */
/******************************************************************************/

  /************************/
  /* low priority-message */
  /************************/

  
  uint8_t aPlaintext_1_CTR[CTR_MSG_SIZE] = 
                        {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,
                         0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,
                         0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,
                         0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,
                         0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,
                         0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,
                         0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,
                         0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10}; /* plaintext */
  
  uint8_t aCiphertext_1_CTR[CTR_MSG_SIZE] = 
                        {0x3B,0x3F,0xD9,0x2E,0xB7,0x2D,0xAD,0x20,
                         0x33,0x34,0x49,0xF8,0xE8,0x3C,0xFB,0x4A,
                         0x01,0x0C,0x04,0x19,0x99,0xE0,0x3F,0x36,
                         0x44,0x86,0x24,0x48,0x3E,0x58,0x2D,0x0E,
                         0xA6,0x22,0x93,0xCF,0xA6,0xDF,0x74,0x53,
                         0x5C,0x35,0x41,0x81,0x16,0x87,0x74,0xDF,
                         0x2D,0x55,0xA5,0x47,0x06,0x27,0x3C,0x50,
                         0xD7,0xB4,0xF8,0xA8,0xCD,0xDC,0x6E,0xD7}; /* ciphertext */
                                              
  

  /*************************/
  /* high priority-message */
  /*************************/  

  uint8_t aPlaintext_2_CTR[CTR_MSG_SIZE] = 
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
                         0x10,0x37,0x6C,0xE6}; /* plaintext */       

  uint8_t aCiphertext_2_CTR[CTR_MSG_SIZE] = {0xBF,0x84,0x7E,0xDC,
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
                                      0x3E,0xC7,0x6B,0xC0}; /* ciphertext */         
                                      


/******************************************************************************/
/*                                                                            */
/*                           Output buffers                                   */
/*                                                                            */
/******************************************************************************/  
  
  uint8_t aAESOutput_1[ECB_MSG_SIZE]; /* low priority message AES output data   */
  uint8_t aAESOutput_2[ECB_MSG_SIZE]; /* high priority message AES output data  */    
     
  
/******************************************************************************/
/*                                                                            */
/*                 Suspend/Resume work variables                              */
/*                                                                            */
/******************************************************************************/    
  
  CRYP_HandleTypeDef hcryp_saved;  
  uint8_t * pInput;
  uint8_t * pOutput;  
  uint16_t Size; 
  uint8_t SavedIV[4*4] = {0};                                          

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);
void data_cmp(uint8_t *EncryptedText, uint8_t *RefText, uint8_t Size);
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

  /*##- Configure the CRYP peripheral ######################################*/
  /* Set the common CRYP parameters */

  CrypHandle.Instance = AES;
  
  
    /**************************************************************************/
    /*                                                                        */
    /*       AES 128 ECB low-priority message encryption interrupted by       */
    /*             AES 256 ECB high priority message encryption               */
    /*                       (interrupt mode)                                 */
    /*                                                                        */    
    /**************************************************************************/  
  
  
     /*************************************************************************/
     /* Start processing low priority message                                 */
     /*************************************************************************/    
  
    CrypHandle.Init.DataType      = CRYP_DATATYPE_8B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_128B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_ENCRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_ECB;
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey          = aAES128key;
    
    
    /**************************************************************************/
    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    
    /* Start processing under IT */
    if (HAL_CRYPEx_AES_IT(&CrypHandle, aPlaintext_1_ECB, ECB_MSG_SIZE, aAESOutput_1) != HAL_OK)
    {
      Error_Handler();
    }
    /* While processing is on-going, set flag requesting to suspend the 
       on-going process */
    /* Depending on the user application, this flag may be set upon
      detection of a higher priority message to process */
    HAL_CRYPEx_ProcessSuspend(&CrypHandle);
    /* Wait for the IP to be suspended */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_SUSPENDED);
  
       
    /* At that point, CrypHandle processing is suspended.
       Disable AES */
    __HAL_CRYP_DISABLE(&CrypHandle);
    
    
    /* Save low-priority message CRYP handle parameters */
    hcryp_saved = CrypHandle;
    

    /**************************************************************************/
    /* Start processing high priority message                                 */
    /**************************************************************************/  
        
    /* Configure AES to handle higher priority message */ 
    CrypHandle.Init.DataType      = CRYP_DATATYPE_32B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_256B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_ENCRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_ECB;
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey          = aAES256key; 
      
      
    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    /* Set the new CRYP parameters */
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    
    /* Start high-priority message processing under IT */  
    if (HAL_CRYPEx_AES_IT(&CrypHandle, aPlaintext_2_ECB, ECB_MSG_SIZE, aAESOutput_2)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for the encryption to be over */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY); 
    
    
    /* Compare ciphered text to the expected one */
    data_cmp(aAESOutput_2, aCiphertext_2_ECB, ECB_MSG_SIZE);
    
    
    /**************************************************************************/
    /* Resume low priority message processing                                 */
    /**************************************************************************/ 
         
    __HAL_CRYP_DISABLE(&CrypHandle);
    /* Recover CrypHandle parameter */
    CrypHandle = hcryp_saved;  
    
    /* Ciphering will restart from where it stopped */
    pInput  =  CrypHandle.pCrypInBuffPtr;
    pOutput =  CrypHandle.pCrypOutBuffPtr; 
    Size    =  CrypHandle.CrypInCount;
    
    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    
    /* Resume low-priority message processing under IT */ 
    if (HAL_CRYPEx_AES_IT(&CrypHandle, pInput, Size, pOutput)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for the encryption to be over */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY); 
    
    /* Compare ciphered text to the expected one */
    data_cmp(aAESOutput_1, aCiphertext_1_ECB, ECB_MSG_SIZE);


  


  printf("\n======================================\n ");
  printf("\n\r ECB processing suspend/resume done.\n ");
  printf("No issue detected.\n ");
  
  
  
    /**************************************************************************/
    /*                                                                        */
    /*       AES 256 CBC low-priority message encryption interrupted by       */
    /*             AES 128 CBC high priority message encryption               */
    /*                       (interrupt mode)                                 */
    /*                                                                        */    
    /**************************************************************************/  
  
  
     /*************************************************************************/
     /* Start processing low priority message                                 */
     /*************************************************************************/    
  
    CrypHandle.Init.DataType      = CRYP_DATATYPE_32B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_256B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_ENCRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_CBC;
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey          = aAES256key;
    CrypHandle.Init.pInitVect     = aIV_1;    
    
    
    /**************************************************************************/
    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    
    /* Start processing under IT */
    if (HAL_CRYPEx_AES_IT(&CrypHandle, aPlaintext_1_CBC, CBC_MSG_SIZE, aAESOutput_1) != HAL_OK)
    {
      Error_Handler();
    }
    /* While processing is on-going, set flag requesting to suspend the 
       on-going process */
    /* Depending on the user application, this flag may be set upon
      detection of a higher priority message to process */
    HAL_CRYPEx_ProcessSuspend(&CrypHandle);
    /* Wait for the IP to be suspended */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_SUSPENDED);
  
       
    /* At that point, CrypHandle processing is suspended.
       Disable AES */
    __HAL_CRYP_DISABLE(&CrypHandle);
    /* Save updated Initialization Vector */
     HAL_CRYPEx_Read_IVRegisters(&CrypHandle, SavedIV);     
    
    /* Save low-priority message CRYP handle parameters */
    hcryp_saved = CrypHandle;
    

    /**************************************************************************/
    /* Start processing high priority message                                 */
    /**************************************************************************/  
        
    /* Configure AES to handle higher priority message */ 
    CrypHandle.Init.DataType      = CRYP_DATATYPE_8B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_128B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_ENCRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_CBC;
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey          = aAES128key; 
    CrypHandle.Init.pInitVect     = aIV_2;    
      
      
    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    /* Set the new CRYP parameters */
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    
    /* Start high-priority message processing under IT */  
    if (HAL_CRYPEx_AES_IT(&CrypHandle, aPlaintext_2_CBC, CBC_MSG_SIZE, aAESOutput_2)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for the encryption to be over */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY); 
    
    
    /* Compare ciphered text to the expected one */
    data_cmp(aAESOutput_2, aCiphertext_2_CBC, CBC_MSG_SIZE);
    
    
    /**************************************************************************/
    /* Resume low priority message processing                                 */
    /**************************************************************************/ 
         
    __HAL_CRYP_DISABLE(&CrypHandle);
    /* Recover CrypHandle parameter */
    CrypHandle = hcryp_saved; 
    /* Point to saved Initialization Vector */
    CrypHandle.Init.pInitVect = SavedIV;     
    
    /* Ciphering will restart from where it stopped */
    pInput  =  CrypHandle.pCrypInBuffPtr;
    pOutput =  CrypHandle.pCrypOutBuffPtr; 
    Size    =  CrypHandle.CrypInCount;
    
    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    
    /* Resume low-priority message processing under IT */ 
    if (HAL_CRYPEx_AES_IT(&CrypHandle, pInput, Size, pOutput)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for the encryption to be over */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY); 
    
    /* Compare ciphered text to the expected one */
    data_cmp(aAESOutput_1, aCiphertext_1_CBC, CBC_MSG_SIZE);
    
  printf("\n======================================\n ");
  printf("\n\r CBC processing suspend/resume done.\n ");
  printf("No issue detected.\n ");    
  
  
    /**************************************************************************/
    /*                                                                        */
    /*       AES 128 CTR low-priority message encryption interrupted by       */
    /*             AES 256 CTR high priority message decryption               */
    /*                       (interrupt mode)                                 */
    /*                                                                        */    
    /**************************************************************************/  
  
  
     /*************************************************************************/
     /* Start processing low priority message                                 */
     /*************************************************************************/    
  
    CrypHandle.Init.DataType      = CRYP_DATATYPE_8B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_128B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_ENCRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_CTR;
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey          = aAES128key;
    CrypHandle.Init.pInitVect     = aIV_1;    
    
    
    /**************************************************************************/
    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    
    /* Start processing under IT */
    if (HAL_CRYPEx_AES_IT(&CrypHandle, aPlaintext_1_CTR, CTR_MSG_SIZE, aAESOutput_1) != HAL_OK)
    {
      Error_Handler();
    }
    /* While processing is on-going, set flag requesting to suspend the 
       on-going process */
    /* Depending on the user application, this flag may be set upon
      detection of a higher priority message to process */
    HAL_CRYPEx_ProcessSuspend(&CrypHandle);
    /* Wait for the IP to be suspended */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_SUSPENDED);
  
       
    /* At that point, CrypHandle processing is suspended.
       Disable AES */
    __HAL_CRYP_DISABLE(&CrypHandle);
    /* Save updated Initialization Vector */
     HAL_CRYPEx_Read_IVRegisters(&CrypHandle, SavedIV);     
    
    /* Save low-priority message CRYP handle parameters */
    hcryp_saved = CrypHandle;
    

    /**************************************************************************/
    /* Start processing high priority message                                 */
    /**************************************************************************/  
        
    /* Configure AES to handle higher priority message */ 
    CrypHandle.Init.DataType      = CRYP_DATATYPE_32B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_256B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_DECRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_CTR;
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey          = aAES256key; 
    CrypHandle.Init.pInitVect     = aIV_2;    
      
      
    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    /* Set the new CRYP parameters */
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    
    /* Start high-priority message processing under IT */  
    if (HAL_CRYPEx_AES_IT(&CrypHandle, aCiphertext_2_CTR, CTR_MSG_SIZE, aAESOutput_2)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for the decryption to be over */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY); 
    
                                                         
    /* Compare ciphered text to the expected one */
    data_cmp(aAESOutput_2, aPlaintext_2_CTR, CTR_MSG_SIZE);
    
    
    /**************************************************************************/
    /* Resume low priority message processing                                 */
    /**************************************************************************/ 
         
    __HAL_CRYP_DISABLE(&CrypHandle);
    /* Recover CrypHandle parameter */
    CrypHandle = hcryp_saved; 
    /* Point to saved Initialization Vector */
    CrypHandle.Init.pInitVect = SavedIV;     
    
    /* Ciphering will restart from where it stopped */
    pInput  =  CrypHandle.pCrypInBuffPtr;
    pOutput =  CrypHandle.pCrypOutBuffPtr; 
    Size    =  CrypHandle.CrypInCount;
    
    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    } 
    
    /* Resume low-priority message processing under IT */ 
    if (HAL_CRYPEx_AES_IT(&CrypHandle, pInput, Size, pOutput)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for the encryption to be over */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY); 
    
    /* Compare ciphered text to the expected one */
    data_cmp(aAESOutput_1, aCiphertext_1_CTR, CTR_MSG_SIZE);
    
  printf("\n======================================\n ");
  printf("\n\r CTR processing suspend/resume done.\n ");
  printf("No issue detected.\n ");     
  
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
    /* Correct encryption */
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

