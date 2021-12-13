/**
  ******************************************************************************
  * @file    CRYP/CRYP_GCM_GMAC_CMAC_Modes/Src/main.c
  * @author  MCD Application Team   
  * @brief   This example provides a short description of how to use the CRYP
  *          AES peripheral to encrypt and decrypt data using AES algorithm as 
  *          well as to generate an authentication tag based on GCM, GMAC or 
  *          CMAC algorithms  
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
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup CRYP_GCM_GMAC_CMAC_Modes
  * @{
  */
  

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define GCM_KEY_SIZE       128 /* in bits  */

#define PAYLOAD_SIZE        64 /* in bytes */
#define GCM_HEADER_SIZE     64 /* in bytes */
#define GMAC_HEADER_SIZE    16 /* in bytes */
#define CMAC_HEADER_SIZE    32 /* in bytes */
#define CMAC_B_SIZE         16 /* in bytes */
#define CMAC_C_SIZE         16 /* in bytes */
#define AES_TAG_SIZE        16 /* in bytes */
#define CMAC_TAG_SIZE       12 /* in bytes, specific to the data used in this example, 
                                 refer to comments below */

#define ECB               1
#define CBC               2
#define CTR               3
#define GCM               4
#define GMAC              5
#define CMAC              6

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* CRYP handler declaration */
CRYP_HandleTypeDef     CrypHandle;
/* UART handler declaration */
UART_HandleTypeDef     UartHandle;

/******************************************************************************/
/*                                                                            */
/*                    GCM input / expected output data                        */
/*                                                                            */
/******************************************************************************/
 /* Header message, will be authenticated but not encrypted */
  uint8_t aHeaderMessage[GCM_HEADER_SIZE] =  {0x18, 0x80, 0xBA, 0xBA, 
                                  0x27, 0xD2, 0x35, 0x90, 
                                  0xF3, 0xBD, 0xC3, 0xCB,
                                  0xC8, 0xF9, 0xE5, 0x0F, 
                                  0xB2, 0x54, 0x1E, 0x76, 
                                  0xC3, 0x36, 0xAE, 0x99,
                                  0xC0, 0x9A, 0x1B, 0x42, 
                                  0xA3, 0x8D, 0xF5, 0xE8, 
                                  0x3F, 0xBB, 0x0D, 0xAF,
                                  0x0F, 0x67, 0x8D, 0x43, 
                                  0x60, 0x02, 0x24, 0x51, 
                                  0x81, 0x29, 0xFB, 0x0A,
                                  0xB3, 0x33, 0x7D, 0x19, 
                                  0x6E, 0x77, 0x6A, 0x2B, 
                                  0x6F, 0x2E, 0x11, 0x86,
                                  0xB6, 0x12, 0xBC, 0x07};


  /* Payload message, will be authenticated and encrypted */
  uint8_t aPlaintext[PAYLOAD_SIZE] =  {0x0D, 0xF6, 0xCD, 0xCA, 
                              0xAC, 0x1C, 0x5D, 0x5B, 
                              0x8D, 0x0D, 0x1F, 0x82, 
                              0x34, 0x0E, 0x36, 0x1A, 
                              0xE0, 0xE7, 0x41, 0xB4,
                              0x31, 0x4E, 0x5E, 0xAB, 
                              0x77, 0xA1, 0x7D, 0x70, 
                              0x04, 0x2E, 0xB3, 0x3A, 
                              0xDC, 0x12, 0x25, 0x81, 
                              0x05, 0x89, 0x2E, 0x83,
                              0x68, 0x92, 0x97, 0x4D, 
                              0x83, 0x9D, 0xA0, 0xCE, 
                              0x8E, 0x63, 0x85, 0xE2, 
                              0x54, 0xC0, 0xB2, 0xE4, 
                              0x6A, 0xCB, 0x61, 0x2F,
                              0x95, 0x6E, 0xFA, 0x14};


  /* 128-bit key used for GCM */
  uint8_t AES128key[16] = {0xB8, 0x91, 0x24, 0x8C, 0x3B, 0x7B, 0x59, 0x84, 0x2A, 0x94, 0xE5, 0xD6, 0x7D, 0xCD, 0x5B, 0xCD};

  /* Initialization Vector used for GCM */
  uint8_t IV[16] = {0x1F, 0x80, 0x72, 0xA2, 0xAF, 0xFC, 0xA8, 0xFD, 0x93, 0x3B, 0x65, 0x61, 0x00, 0x00, 0x00, 0x02};



  /* Expected authentication TAG */
  uint8_t aExpectedAuthTAG[AES_TAG_SIZE] = {0xE9, 0xDB, 0x4B, 0xD3, 
                                 0x4A, 0xAE, 0x16, 0xF7,
                                 0xDB, 0x81, 0x5D, 0x24, 
                                 0xE2, 0x82, 0x0D, 0x3D};                                 

  
  
  /* Expected Cypher text */
  uint8_t aExpectedCypherText[PAYLOAD_SIZE] =  {0x9A, 0xF6, 0xE0, 0x1F, 
                                       0xF7, 0x61, 0xE8, 0x61, 
                                       0x14, 0xB0, 0xA2, 0xC3, 
                                       0x7A, 0x76, 0x5C, 0x05,
                                       0x46, 0x5D, 0x93, 0x1D, 
                                       0x97, 0xAE, 0x62, 0xAB, 
                                       0xEE, 0xE9, 0x7A, 0xA3, 
                                       0x57, 0x69, 0xF9, 0x8B,
                                       0xA2, 0x92, 0xF0, 0x24, 
                                       0xFB, 0x80, 0xE4, 0x6C, 
                                       0x2E, 0xCA, 0xFB, 0x04, 
                                       0xA8, 0x6E, 0x94, 0x7B,
                                       0x21, 0xB8, 0xEC, 0x94, 
                                       0xDA, 0x17, 0x03, 0xD6, 
                                       0x04, 0x1F, 0x74, 0xD4, 
                                       0x1C, 0x5E, 0xDA, 0x8A};
                                       
/******************************************************************************/
/*                                                                            */
/*                    GMAC input / expected output data                       */
/*                                                                            */
/******************************************************************************/
  uint8_t aHeaderMessage_GMAC[GMAC_HEADER_SIZE]  =  {0xD9, 0x63, 0x34, 0xC2, 
                                   0x3A, 0xCB, 0x77, 0x56,
                                   0xBC, 0x00, 0x71, 0xE8, 
                                   0x1A, 0x02, 0xCF, 0x1A};                                 
  uint8_t aExpectedAuthTAG_GMAC[AES_TAG_SIZE] = {0x52, 0x31, 0xD2, 0x5C, 
                                   0xD3, 0x35, 0xA0, 0x76,
                                   0x2F, 0xF4, 0xAB, 0x8D, 
                                   0x8B, 0xEC, 0x99, 0x94};                                                                                                                                                                              

                               

  /* 256-bit key used for GMAC */
  uint8_t AES256key[32] = {0x38, 0xC5, 0x17, 0x12, 0xA8, 0x96, 0xA7, 0xAA,
                     0x0D, 0x69, 0x12, 0x23, 0xDA, 0x57, 0xB2, 0xDB,
                     0x00, 0xFD, 0xCE, 0xCA, 0x2B, 0xC5, 0xFB, 0xE9,
                     0xB2, 0xBB, 0x91, 0xA6, 0xE0, 0xD5, 0xAF, 0x19};

  /* Initialization Vector used for GMAC */
  uint8_t IV_GMAC[16] = {0x3D, 0xC1, 0x23, 0x68, 0x87, 0x1E, 0x52, 0x39,
                    0xD9, 0xE3, 0xDF, 0xBA, 0x00, 0x00, 0x00, 0x02};
                    
                    
/******************************************************************************/
/*                                                                            */
/*                    CMAC input / expected output data                       */
/*                                                                            */
/******************************************************************************/ 

   /* Header message, will be authenticated but not encrypted */
  uint32_t aHeaderMessage_CMAC[8] =  {0x0010621F, 
                                     0x6BCCF5C5, 
                                     0x5E4A66E0, 
                                     0x464DE597,
                                     0x17500000, 
                                     0x00000000, 
                                     0x00000000, 
                                     0x00000000};                                                                                         

  /* Key to be used for AES encryption/decryption */
  uint8_t AES256key_CMAC[8*4] = {0xE3,0xA3,0x21,0xDB, 
                                 0x49,0x0F,0x56,0xCB, 
                                 0xCE,0x5E,0x34,0xBA, 
                                 0xBE,0x62,0x93,0xAB,
                                 0xDE,0xDA,0x8E,0xBE, 
                                 0xFB,0x5F,0xE5,0xA8, 
                                 0x95,0xFE,0x38,0xA1, 
                                 0x4C,0xA0,0x09,0xF6};                                        


  /* B Blocks (used in header phase) */
  uint32_t BBlocks[4] = {0x6EAA1E6A, 
                         0x61F9FF38, 
                         0x1F000000, 
                         0x00000000};

  /* Block C (used in final phase) */
  uint32_t CBlock[4] = {0x06AA1E6A, 
                        0x61F9FF38, 
                        0x1F000000, 
                        0x00000000};

  /* Expected authentication TAG */
  uint32_t aExpectedAuthTAG_CMAC[4] = {0x5DDA3A86, 0xB8BF4FC1, 0xCF03811E, 0x00000000}; 
  /*uint8_t ExpectedAuthTAG_CMAC[AES_TAG_SIZE] = {0x86,0x3A,0xDA,0x5D, 
                                           0xC1,0x4F,0xBF,0xB8, 
                                           0x1E,0x81,0x03,0xCF, 
                                           0x00,0x00,0x00,0x00};    */                  

/******************************************************************************/
/*                                                                            */
/*                           Output buffers                                   */
/*                                                                            */
/******************************************************************************/  
 
  /* Buffer to store the output data and the authentication TAG */
  /* Used for storing the encrypted text */
  uint8_t aEncryptedtext[PAYLOAD_SIZE] = {0};

  /* Used for storing the decrypted text */
  uint8_t aDecryptedtext[PAYLOAD_SIZE] = {0};
  
  /* Used for storing the generated authentication tag */
  uint8_t aAuthTAG[AES_TAG_SIZE];                                            
                                                                                                                                           

/* Private function prototypes -----------------------------------------------*/

static void USART2_Config(void);
static void Display_PlainData(uint32_t datalength);
static void Display_Header(uint8_t *Tag, uint8_t mode, uint32_t datalength);
static void Display_CipheredData(uint32_t datalength);
static void Display_AuthenticationTag(uint8_t *Tag, uint32_t datalength);
static void Display_EncryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength);
static void Display_ComputedTag(uint8_t mode, uint32_t datalength);
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

  /* Display Message Payload */
  Display_PlainData(PAYLOAD_SIZE);
  
  /* Display Message Header */
  Display_Header(aHeaderMessage, GCM, GCM_HEADER_SIZE);


  /* Display Expected Ciphered Data*/
  Display_CipheredData(PAYLOAD_SIZE);
  
  /* Display Expected Authentication tag */
  Display_AuthenticationTag(aExpectedAuthTAG, AES_TAG_SIZE);




    /******************************************************************************/
    /*                                                                            */
    /*   AES 128 GCM mode authentication tag computation and message ciphering    */
    /*                         (interrupt mode)                                   */
    /*                                                                            */    
    /******************************************************************************/

    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    }

    CrypHandle.Init.DataType      = CRYP_DATATYPE_32B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_128B;
    CrypHandle.Init.pKey          = AES128key;

    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_ENCRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_GCM_GMAC;
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCM_INIT_PHASE;  
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pInitVect     = IV; 
    CrypHandle.Init.Header        = aHeaderMessage; 
    CrypHandle.Init.HeaderSize    = GCM_HEADER_SIZE;       
    

    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
    
    /*------------------------------------------------------------*/    
    /* GCM init phase */
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, NULL, 0, NULL)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for GCM init phase to finish
       For simplicity's sake, this example is just waiting till the end of the 
       process, but application may perform other tasks while phase processing
       is ongoing. */ 
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
    /*------------------------------------------------------------*/
    /* GCM header phase */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCMCMAC_HEADER_PHASE;
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, NULL, 0, NULL)!= HAL_OK)
    {
      Error_Handler();
    } 
    /* Wait for GCM header phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
    /*------------------------------------------------------------*/
    /* GCM payload phase */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCM_PAYLOAD_PHASE;
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, aPlaintext, PAYLOAD_SIZE, aEncryptedtext)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for GCM payload phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
    /*------------------------------------------------------------*/    
    /* GCM final phase */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCMCMAC_FINAL_PHASE;
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, NULL, PAYLOAD_SIZE, aAuthTAG)!= HAL_OK)
    {
      Error_Handler();
    } 
    /* Wait for GCM final phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);      
    /*------------------------------------------------------------*/

    /* Display encrypted Data */
    Display_EncryptedData(GCM, GCM_KEY_SIZE, PAYLOAD_SIZE);
    
    /* Display Computed Authentication tag */
    Display_ComputedTag(GCM, AES_TAG_SIZE);

    
    /* Compare the encrypted text with the expected one *************************/ 
    data_cmp(aEncryptedtext, aExpectedCypherText, PAYLOAD_SIZE); 
    
    /* Compare the authentication tag with the expected one *************************/ 
    data_cmp(aAuthTAG, aExpectedAuthTAG, AES_TAG_SIZE);     
  
  
    /******************************************************************************/
    /*                                                                            */
    /*  AES 128 GCM mode authentication tag computation and message deciphering   */
    /*                          (polling mode)                                    */
    /*                                                                            */    
    /******************************************************************************/  


    /* Set the CRYP parameters */
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_DECRYPT;
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCM_INIT_PHASE;     
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
      Error_Handler();
    }  
    
    /*------------------------------------------------------------*/     
    /* GCM init phase */
    if (HAL_CRYPEx_AES_Auth(&CrypHandle, NULL, 0, NULL, TIMEOUT_VALUE)!= HAL_OK)
    {
      Error_Handler();
    }
    /*------------------------------------------------------------*/     
    /* GCM header phase */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCMCMAC_HEADER_PHASE;
    if (HAL_CRYPEx_AES_Auth(&CrypHandle, NULL, 0, NULL, TIMEOUT_VALUE)!= HAL_OK)
    {
      Error_Handler();
    } 
    /*------------------------------------------------------------*/     
    /* GCM payload phase */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCM_PAYLOAD_PHASE;
    if (HAL_CRYPEx_AES_Auth(&CrypHandle, aExpectedCypherText, PAYLOAD_SIZE, aDecryptedtext, TIMEOUT_VALUE)!= HAL_OK)
    {
      Error_Handler();
    }
    /*------------------------------------------------------------*/     
    /* GCM final phase */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCMCMAC_FINAL_PHASE;
    if (HAL_CRYPEx_AES_Auth(&CrypHandle, NULL, PAYLOAD_SIZE, aAuthTAG, TIMEOUT_VALUE)!= HAL_OK)
    {
      Error_Handler();
    }       
    /*------------------------------------------------------------*/     
  
    /* Display encrypted Data */
    Display_DecryptedData(GCM, GCM_KEY_SIZE, PAYLOAD_SIZE);
    
    /* Display Computed Authentication tag */
    Display_ComputedTag(GCM, AES_TAG_SIZE);

    
    /* Compare the decrypted text with the expected one *************************/ 
    data_cmp(aDecryptedtext, aPlaintext, PAYLOAD_SIZE); 
    
    /* Compare the authentication tag with the expected one *************************/ 
    data_cmp(aAuthTAG, aExpectedAuthTAG, AES_TAG_SIZE);     

  printf("===================================================================\n ");
  printf("\n\r GCM ciphering/deciphering/authentication tag computation done.\n ");
  printf("\n\r No issue detected.\n\n ");
  
  
    /******************************************************************************/
    /*                                                                            */
    /*             AES 256 GMAC mode authentication tag computation               */     
    /*                            (DMA mode)                                      */
    /*                                                                            */    
    /******************************************************************************/  
  
    CrypHandle.Init.DataType      = CRYP_DATATYPE_1B ;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_256B ;
    CrypHandle.Init.pKey          = AES256key;
    
    
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_TAG_GENERATION;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_GCM_GMAC;
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCM_INIT_PHASE;  
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pInitVect     = IV_GMAC; 
    CrypHandle.Init.Header        = aHeaderMessage_GMAC; 
    CrypHandle.Init.HeaderSize    = GMAC_HEADER_SIZE;       
  

    /* Display Header */
    Display_Header(aHeaderMessage_GMAC, GMAC, GMAC_HEADER_SIZE);  
    
    /* Display Expected Authentication tag */
    Display_AuthenticationTag(aExpectedAuthTAG_GMAC, AES_TAG_SIZE);
  
    /****************************************************************************/
    /* De-initialize then initialize the AES peripheral                         */
    /****************************************************************************/
    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
        Error_Handler();
    } 
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
        Error_Handler();
    } 
    
    /*------------------------------------------------------------*/   
    /* GMAC init phase (same as GCM) */
    if (HAL_CRYPEx_AES_Auth_DMA(&CrypHandle, NULL, 0, NULL)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for GMAC init phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
    /*------------------------------------------------------------*/      
    /* GMAC header phase (same as GCM) */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCMCMAC_HEADER_PHASE;
    if (HAL_CRYPEx_AES_Auth_DMA(&CrypHandle, NULL, 0, NULL)!= HAL_OK)
    {
      Error_Handler();
    } 
    /* Wait for GMAC header phase to finish */    
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
    /*------------------------------------------------------------*/      
    /* GMAC final phase (same as GCM) */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCMCMAC_FINAL_PHASE;
    if (HAL_CRYPEx_AES_Auth_DMA(&CrypHandle, NULL, 0, aAuthTAG)!= HAL_OK)
    {
      Error_Handler();
    } 
    /* Wait for GMAC final phase to finish */     
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);  
    /*------------------------------------------------------------*/            
  
    /* Display Computed Authentication tag */
    Display_ComputedTag(GMAC, AES_TAG_SIZE);  
  
     
    /* Compare the GMAC authentication tag with the expected one *******************/ 
    data_cmp(aAuthTAG, aExpectedAuthTAG_GMAC, AES_TAG_SIZE);     

  printf("==============================================\n ");
  printf("\n\r GMAC authentication tag computation done.\n ");
  printf("\n\r No issue detected.\n\n ");
  
  
    /******************************************************************************/
    /*                                                                            */
    /*             AES 256 CMAC mode authentication tag computation               */     
    /*                          (polling mode)                                    */
    /*                                                                            */    
    /******************************************************************************/  
  
    CrypHandle.Init.DataType      = CRYP_DATATYPE_32B ;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_256B ;
    CrypHandle.Init.pKey          = AES256key_CMAC;
    
    
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_TAG_GENERATION;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_CMAC;
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCMCMAC_HEADER_PHASE;  
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.Header        = (uint8_t *)aHeaderMessage_CMAC; 
    CrypHandle.Init.HeaderSize    = CMAC_HEADER_SIZE;       
  

    /* Display Header */
    Display_Header((uint8_t *)aHeaderMessage_CMAC, CMAC, CMAC_HEADER_SIZE);  
    
    /* Display Expected Authentication tag */
    /* According to RFC 3610 Counter with CBC-MAC (CCM),
      Since BBlocks[0] = 0x0x6EAA1E6A, Octet 0 of B0 block is 0x6A.
      (DataType is 32-bit: the less-significant data occupies the lowest address location.)
    
      Therefore, M' = 0b 101 (bits 3 to 5) = 5.
      M = 2*M'+2 = 12. The authentication tag is 12-byte long */     
    Display_AuthenticationTag((uint8_t *)aExpectedAuthTAG_CMAC, CMAC_TAG_SIZE);
  
    /****************************************************************************/
    /* De-initialize then initialize the AES peripheral                         */
    /****************************************************************************/
    if (HAL_CRYP_DeInit(&CrypHandle) != HAL_OK)
    {
        Error_Handler();
    } 
    /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
    {
        Error_Handler();
    } 
    
    
    /*----------------------------------------------------------------------------------------------*/     
    /* CMAC header phase */
    if (HAL_CRYPEx_AES_Auth(&CrypHandle, (uint8_t *)BBlocks, CMAC_B_SIZE, NULL, TIMEOUT_VALUE)!= HAL_OK)
    {
      Error_Handler();
    }
    /*----------------------------------------------------------------------------------------------*/       
    /* CMAC final phase */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCMCMAC_FINAL_PHASE;
    if (HAL_CRYPEx_AES_Auth(&CrypHandle, (uint8_t *)CBlock, CMAC_C_SIZE, (uint8_t *)aAuthTAG, TIMEOUT_VALUE)!= HAL_OK)
    {
      Error_Handler();
    } 
    /*----------------------------------------------------------------------------------------------*/       
  
  
    /* Display Computed Authentication tag */        
    Display_ComputedTag(CMAC, CMAC_TAG_SIZE);  
  
     
    /* Compare the GMAC authentication tag with the expected one *******************/ 
    data_cmp(aAuthTAG, (uint8_t *)aExpectedAuthTAG_CMAC, CMAC_TAG_SIZE);     

  printf("\n\r ==============================================\n ");
  printf("\n\r CMAC authentication tag computation done.\n ");
  printf("\n\r No issue detected.\n\n ");
  
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

  printf("\n\r ===============================================================\n\r");
  printf(" ================= CRYP Using HW AES block  ====================\n\r");
  printf(" ===============================================================\n\r");
  printf(" ---------------------------------------------------\n\r");
  printf(" Plain Data (Input data for GCM AES 128 encryption):\n\r");
  printf(" ---------------------------------------------------\n\r");

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
static void Display_Header(uint8_t *Tag, uint8_t mode, uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;

  if (mode == GCM)
  {
    printf("\n\r ===============================================================\n\r");
    printf(" -----------------------------------------------\n\r");
    printf(" Header (Input data for GCM AES 128 encryption):\n\r");
    printf(" -----------------------------------------------\n\r");
  }
  else if (mode == GMAC)
  {
    printf("\n\r ================================================================\n\r");
    printf(" ------------------------------------------------\n\r");
    printf(" Header (Input data for GMAC AES 256 encryption):\n\r");
    printf(" ------------------------------------------------\n\r");  
  } 
  else if (mode == CMAC)
  {
    printf("\n\r ================================================================\n\r");
    printf(" ------------------------------------------------\n\r");
    printf(" Header (Input data for CMAC AES 256 encryption):\n\r");
    printf(" ------------------------------------------------\n\r");  
  }   

  for (BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
  {
    printf("[0x%02X]", aHeaderMessage[BufferCounter]);
    count++;

    if (count == 16)
    {
      count = 0;
      printf("  Block %lu \n\r", BufferCounter / 16);
    }
  }
}



/**
  * @brief  Display Expected Ciphered data
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_CipheredData(uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;

  printf("\n\r =================================================================\n\r");
  printf(" -----------------------------------------------------------------\n\r");
  printf(" Expected Ciphered Data (Output data from GCM AES 128 decryption):\n\r");
  printf(" -----------------------------------------------------------------\n\r");

  for (BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
  {
    printf("[0x%02X]", aExpectedCypherText[BufferCounter]);
    count++;

    if (count == 16)
    {
      count = 0;
      printf("  Block %lu \n\r", BufferCounter / 16);
    }
  }
}

/**
  * @brief  Display Expected Authentication Tag
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_AuthenticationTag(uint8_t *Tag, uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;

  printf("\n\r ===================================================\n\r");
  printf(" ---------------------------------------------------\n\r");
  printf(" Expected Authentication tag :\n\r");
  printf(" ---------------------------------------------------\n\r");

  for (BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
  {
    printf("[0x%02X]", Tag[BufferCounter]);
    count++;

    if (count == 16)
    {
      count = 0;
      printf("  Block %lu \n\r", BufferCounter / 16);
    }
  }
}


/**
  * @brief  Display Computed Authentication Tag
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_ComputedTag(uint8_t mode, uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;

  if (mode == GCM)
  {
    printf("\n\r =======================================================\n\r");
    printf(" -------------------------------------------------------\n\r");
    printf(" Computed Authentication tag (derived from GCM 128 AES):\n\r");
    printf(" -------------------------------------------------------\n\r");
  }
  else if (mode == GMAC)
  {
    printf("\n\r =======================================================\n\r");
    printf(" --------------------------------------------------------\n\r");
    printf(" Computed Authentication tag (derived from GMAC 256 AES):\n\r");
    printf(" --------------------------------------------------------\n\r");
  }
  else if (mode == CMAC)
  {
    printf("\n\r =======================================================\n\r");
    printf(" --------------------------------------------------------\n\r");
    printf(" Computed Authentication tag (derived from CMAC 256 AES):\n\r");
    printf(" --------------------------------------------------------\n\r");
  }  
  

  for (BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
  {
    printf("[0x%02X]", aAuthTAG[BufferCounter]);
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
  else if (mode == CTR)
  {
    printf("CTR\n\r");
  }
  else if (mode == GCM)
  {
    printf("GCM\n\r");
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
  else if (mode == CTR)
  {
    printf("CTR\n\r");
  }
  else if (mode == GCM)
  {
    printf("GCM\n\r");
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
    /* Correct encryption */
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

