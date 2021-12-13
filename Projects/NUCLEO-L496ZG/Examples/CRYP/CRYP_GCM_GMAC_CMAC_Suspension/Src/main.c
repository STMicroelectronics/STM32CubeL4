/**
  ******************************************************************************
  * @file    CRYP/CRYP_GCM_GMAC_CMAC_Suspension/Src/main.c
  * @author  MCD Application Team   
  * @brief   This example provides a short description of how to use the CRYP
  *          AES peripheral to suspend and resume the AES GCM, GMAC and CMAC
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

/** @addtogroup CRYP_GCM_GMAC_CMAC_Suspension
  * @{
  */
  

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PAYLOAD_SIZE        64 /* in bytes */
#define GCM_HEADER_SIZE     64 /* in bytes */
#define GMAC_HEADER_SIZE    64 /* in bytes */
#define CMAC_HEADER_SIZE    32 /* in bytes */
#define CMAC_B_SIZE         32 /* in bytes */
#define CMAC_C_SIZE         16 /* in bytes */
#define AES_TAG_SIZE        16 /* in bytes */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* CRYP handler declaration */
CRYP_HandleTypeDef     CrypHandle;


/******************************************************************************/
/*                                                                            */
/*                             Encryption keys                                */
/*                                                                            */
/******************************************************************************/

  /* Key used in GCM interruption case */
  uint8_t aAES128key[16] = {0xB8, 0x91, 0x24, 0x8C, 0x3B, 0x7B, 0x59, 0x84, 
                            0x2A, 0x94, 0xE5, 0xD6, 0x7D, 0xCD, 0x5B, 0xCD}; /* key size 128 bits */  
                           
  /* Key used in GCM, GMAC and CMAC interruption cases */
  uint8_t aAES256key[32] = {0xFA,0x18,0x23,0x0F,0xF4,0x88,0xF1,0xF8,
                     0xD5,0x6C,0x1B,0xBE,0x35,0x6F,0xA0,0xB2,
                     0x65,0xA2,0x93,0xD7,0x37,0x38,0x1B,0x41,
                     0x05,0x2C,0x3D,0x41,0x34,0x72,0xF6,0x2F};  /* key size 256 bits */
                     
  /* Key used in GMAC interruption case */ 
  uint8_t aAES256key_GMAC[32] = {0x38, 0xC5, 0x17, 0x12, 0xA8, 0x96, 0xA7, 0xAA,
                     0x0D, 0x69, 0x12, 0x23, 0xDA, 0x57, 0xB2, 0xDB,
                     0x00, 0xFD, 0xCE, 0xCA, 0x2B, 0xC5, 0xFB, 0xE9,
                     0xB2, 0xBB, 0x91, 0xA6, 0xE0, 0xD5, 0xAF, 0x19};  /* key size 256 bits */   
        
  /* Key used in CMAC interruption case */                     
 uint8_t aAES128key_CMAC[16] = {0xE6,0x9C,0xF8,0x9C,0x97,0x5C,0x9A,0x0D,
                      0x9E,0xE9,0x8D,0x50,0x13,0xEC,0xB8,0xA4};  /* key size 128 bits */                    
                                        


/******************************************************************************/
/*                                                                            */
/*                       Initialization Vectors                               */
/*                                                                            */
/******************************************************************************/
                           
  /* Initialization Vectors used in GCM interruption case */
  uint8_t aIV_1[16] = {0x1F, 0x80, 0x72, 0xA2, 0xAF, 0xFC, 0xA8, 0xFD, 
                    0x93, 0x3B, 0x65, 0x61, 0x00, 0x00, 0x00, 0x02};    
  
  /* Initialization Vectors used in GCM, GMAC and CMAC interruption case */                  
  uint8_t aIV_2[16] = {0xB9,0x02,0x76,0x3E,0x3E,0x34,0xA7,0xC6,
                    0x39,0xC8,0xE9,0xB1,0x00,0x00,0x00,0x02}; 
                    
  /* Initialization Vectors used in GMAC case */
  uint8_t aIV_GMAC[16] = {0x3D, 0xC1, 0x23, 0x68, 0x87, 0x1E, 0x52, 0x39,
                    0xD9, 0xE3, 0xDF, 0xBA, 0x00, 0x00, 0x00, 0x02};   
                                                                          

/******************************************************************************/
/*                                                                            */
/*                    GCM input / expected output data                        */
/*                                                                            */
/******************************************************************************/

  /************************/
  /* low priority-message */
  /************************/

 /* Header message, will be authenticated but not encrypted */
  uint8_t aHeaderMessage_1_GCM[GCM_HEADER_SIZE] =  {0x18, 0x80, 0xBA, 0xBA, 
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
  
  uint8_t aPlaintext_1_GCM[PAYLOAD_SIZE] = 
                             {0x0D, 0xF6, 0xCD, 0xCA, 
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
                              0x95, 0x6E, 0xFA, 0x14}; /* plaintext */
  
  uint8_t aCiphertext_1_GCM[PAYLOAD_SIZE] = 
                                      {0x9A, 0xF6, 0xE0, 0x1F, 
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
                                       0x1C, 0x5E, 0xDA, 0x8A};/* ciphertext */
                         
  /* Expected authentication TAG */
  uint8_t ExpectedAuthTAG_1_GCM[AES_TAG_SIZE] = {0xE9, 0xDB, 0x4B, 0xD3, 
                                 0x4A, 0xAE, 0x16, 0xF7,
                                 0xDB, 0x81, 0x5D, 0x24, 
                                 0xE2, 0x82, 0x0D, 0x3D};                             
                                              
  

  /*************************/
  /* high priority-message */
  /*************************/  
  /* used as well in GMAC and CMAC interruption cases */
  
 /* Header message, will be authenticated but not encrypted */
  uint8_t aHeaderMessage_2_GCM[GCM_HEADER_SIZE] =  {0x80, 0x3F, 0x1A, 0x8B,
                                  0x78, 0x79, 0x30, 0xA3,
                                  0xD2, 0x37, 0x94, 0x9C,
                                  0x6A, 0x84, 0xD7, 0x04,
                                  0xB1, 0x41, 0x17, 0x79,
                                  0x55, 0xF3, 0x5C, 0x4F,
                                  0xBD, 0x97, 0x30, 0x88,
                                  0x2E, 0xED, 0x8E, 0xA3,
                                  0x4B, 0xAE, 0x2C, 0xA8,
                                  0xF9, 0xC3, 0x27, 0x5C,
                                  0xC1, 0xCB, 0xFB, 0xBC,
                                  0x4C, 0x2B, 0x4F, 0xD2,
                                  0xB6, 0xFD, 0x6C, 0x66,
                                  0x52, 0x0C, 0xF1, 0xC9,
                                  0xC4, 0x10, 0xA3, 0x21,
                                  0xDA, 0xF3, 0xFD, 0x31};
    

  uint8_t aPlaintext_2_GCM[PAYLOAD_SIZE] = 
                             {0x87, 0x25, 0xA2, 0x2A,
                              0x86, 0x80, 0xE9, 0xC9,
                              0xB7, 0x47, 0x4B, 0xE4,
                              0x02, 0x03, 0x72, 0x9B,
                              0xA9, 0xB9, 0x00, 0xDF,
                              0x13, 0xFB, 0xC5, 0xF2,
                              0x9B, 0xD8, 0x0C, 0x69,
                              0x34, 0x75, 0xCC, 0x0A,
                              0x38, 0xBB, 0x9B, 0x6E,
                              0x69, 0xBE, 0x3C, 0x84,
                              0x23, 0x20, 0x05, 0x87,
                              0x57, 0x25, 0x23, 0x77,
                              0x16, 0x00, 0xDE, 0x24,
                              0x0D, 0x2A, 0xFC, 0xA4,
                              0x12, 0xA9, 0x03, 0x08,
                              0x3E, 0x47, 0x1E, 0xCF};                  
  

  uint8_t aCiphertext_2_GCM[PAYLOAD_SIZE] = {0xFD, 0xE6, 0x10, 0xFA,
                                       0x91, 0x71, 0xCA, 0x5B,
                                       0x57, 0x8A, 0xDD, 0x93,
                                       0x42, 0xC2, 0xF1, 0x19,
                                       0xB5, 0xBD, 0x33, 0xDF,
                                       0x8D, 0x66, 0xD6, 0x81,
                                       0x49, 0xD7, 0x79, 0x0D,
                                       0xF0, 0x1B, 0x9F, 0x35,
                                       0x60, 0xEA, 0x8A, 0xB8,
                                       0x8B, 0x70, 0x76, 0xE8,
                                       0xEB, 0x1B, 0xEE, 0x7F,
                                       0xFA, 0xB2, 0x98, 0x5E,
                                       0x21, 0x83, 0x06, 0x5B,
                                       0xD8, 0xE5, 0xDF, 0xA0,
                                       0xAC, 0x30, 0xDE, 0xD9,
                                       0xC1, 0x42, 0x0B, 0x0E};
 
                                      
  /* Expected authentication TAG */
  uint8_t ExpectedAuthTAG_2_GCM[AES_TAG_SIZE] = {0x26, 0x9C, 0xDC, 0x8B,
                                 0xD7, 0x34, 0x6D, 0x53,
                                 0xD1, 0x11, 0x37, 0xB2,
                                 0x07, 0x25, 0x06, 0x2D};                                                         



/******************************************************************************/
/*                                                                            */
/*                    GMAC input / expected output data                       */
/*                                                                            */
/******************************************************************************/

  /************************/
  /* low priority-message */
  /************************/

 /* Header message, will be authenticated but not encrypted */
  uint8_t aHeaderMessage_1_GMAC[GMAC_HEADER_SIZE] = {0xC6, 0x9B, 0x43, 0x2C,  
                                    0xD3, 0x5C, 0x6A, 0xEE, 
                                    0x00, 0x3D, 0x17, 0x8E,  
                                    0x40, 0x58, 0x58, 0xF3,                                 
                                    0x8C, 0x4A, 0x3A, 0x4B,  
                                    0xAC, 0xCB, 0x6E, 0x05, 
                                    0x2F, 0xF4, 0xB1, 0xD5,  
                                    0x37, 0xD1, 0x29, 0x99, 
                                    0x9B, 0xC6, 0x2C, 0x43,  
                                    0x5C, 0xD3, 0xEE, 0x6A, 
                                    0x3D, 0x00, 0x8E, 0x17,  
                                    0x58, 0x40, 0xF3, 0x58,                                 
                                    0x4A, 0x8C, 0x4B, 0x3A,  
                                    0xCB, 0xAC, 0x05, 0x6E, 
                                    0xF4, 0x2F, 0xD5, 0xB1,  
                                    0xD1, 0x37, 0x99, 0x29};
  

                         
  /* Expected authentication TAG */
  uint8_t ExpectedAuthTAG_1_GMAC[AES_TAG_SIZE] = {0x6e, 0x1a, 0xe0, 0x1d, 0xfc, 0x23, 0x60, 0xdc, 
                                                 0xa1, 0xa6, 0x73, 0xaa, 0xc0, 0x82, 0xf0, 0xf9};                          
                                              
  

  /*************************/
  /* high priority-message */
  /*************************/  
  /* same as that used in GCM interruption case */
 

/******************************************************************************/
/*                                                                            */
/*                    CMAC input / expected output data                       */
/*                                                                            */
/******************************************************************************/

  /************************/
  /* low priority-message */
  /************************/

 /* Header message, will be authenticated but not encrypted */
  uint8_t aHeaderMessage_1_CMAC[CMAC_HEADER_SIZE] = {0xA5,0xA8,0x10,0x00,
                                 0x17,0xB6,0x64,0xA6,
                                 0xA6,0x40,0xB3,0x33,
                                 0x8B,0x43,0xB1,0x43,
                                 0x00,0x00,0x46,0x6E,
                                 0x00,0x00,0x00,0x00,
                                 0x00,0x00,0x00,0X00,
                                 0x00,0x00,0x00,0X00};  
                                 
  /* B Blocks */
    uint8_t BBlocks[CMAC_B_SIZE] = {0x48,0x9D,0x70,0x77,
                          0x39,0x8D,0x02,0xBC,
                          0x00,0x00,0x00,0x00,
                          0x00,0x00,0x00,0x00, 
                          0x77,0x70,0x9D,0x48,
                          0xBC,0x02,0x8D,0x39,
                          0x00,0x00,0x00,0x00,
                          0x00,0x00,0x00,0x00};   
                          
  /* C Block */
  uint8_t CBlock[CMAC_C_SIZE]  = {0x48,0x9D,0x70,0x07,
                         0x39,0x8D,0x02,0xBC,
                         0x00,0x00,0x00,0x00,
                         0x00,0x00,0x00,0x00};                                                              
  

                         
  /* Expected authentication TAG */
  uint8_t ExpectedAuthTAG_1_CMAC[AES_TAG_SIZE] = {0x5e, 0x14, 0xa5, 0xec, 0x28, 0x48, 0x85, 0x7c, 
                                                  0x9c, 0x38, 0x19, 0xe1, 0xf0, 0x6e, 0x2e, 0xf6};                           
                                              
  

  /*************************/
  /* high priority-message */
  /*************************/  
  /* same as that used in GCM interruption case */
 



/******************************************************************************/
/*                                                                            */
/*                           Output buffers                                   */
/*                                                                            */
/******************************************************************************/  
  
  uint8_t aAESOutput_1[PAYLOAD_SIZE]; /* low priority message AES output data   */
  uint8_t aAESOutput_2[PAYLOAD_SIZE]; /* high priority message AES output data  */ 
  uint8_t ComputedAuthTAG_1[AES_TAG_SIZE]; /* low priority message computed authentication tag  */ 
  uint8_t ComputedAuthTAG_2[AES_TAG_SIZE]; /* high priority message computed authentication tag */      
     
  
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
  uint8_t SavedSUSPxR[8*4] = {0};  
  uint8_t SavedAES_CR[1*4] = {0};
  uint64_t FullHeaderSize;                                          
  /* Remaining number of B blocks in bytes at resumption time 
    in case of CMAC header phase interruption
    (can be 0 if all B blocks have been processed
    before suspension) */
  uint64_t Bsize;
  /* pointer at B blocks at resumption time 
    in case of CMAC header phase interruption
    (can be NULL if all B blocks have been processed
    before suspension) */
  uint8_t * pBlock;                                                                                                                                             

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

  /* Configure the system clock to 80 MHz */
  SystemClock_Config();
  
  /* Configure LEDs */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);



  /*##- Configure the CRYP peripheral ######################################*/
  /* Set the common CRYP parameters */

  CrypHandle.Instance = AES;
  
  
    /**************************************************************************/
    /*                                                                        */
    /*                                                                        */    
    /*           AES 128 GCM low-priority message encryption and tag          */
    /*       computation interrupted by AES 256 GCM high priority message     */
    /*                   encryption and tag computation                       */
    /*                       (interrupt mode)                                 */
    /*                                                                        */
    /*                                                                        */        
    /**************************************************************************/  
  
  
     /*************************************************************************/
    /*                                                                        */     
     /* Start processing low priority message                                 */
    /*                                                                        */     
     /*************************************************************************/    
  
    CrypHandle.Init.DataType      = CRYP_DATATYPE_32B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_128B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_ENCRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_GCM_GMAC;
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCM_INIT_PHASE;     
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey          = aAES128key;
    CrypHandle.Init.pInitVect     = aIV_1;         
    CrypHandle.Init.Header        = aHeaderMessage_1_GCM; 
    CrypHandle.Init.HeaderSize    = GCM_HEADER_SIZE;     
    
    
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
    /**************************************************************************/
    /*                                                                        */
    /*                Low priority message suspension request                 */
    /*                                                                        */    
    /**************************************************************************/      
    /* While processing is on-going, set flag requesting to suspend the 
       on-going process */
    /* Depending on the user application, this flag may be set upon
      detection of a higher priority message to process */
    HAL_CRYPEx_ProcessSuspend(&CrypHandle);
    /* Wait for the IP to be suspended */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_SUSPENDED);
    
    
    /**************************************************************************/
    /*                                                                        */
    /*                Low priority message GCM header phase                   */
    /*                   processing parameters safeguard                      */
    /*                                                                        */    
    /**************************************************************************/     
    /* At that point, CrypHandle processing is suspended.
       Save SUSPxR registers */
    HAL_CRYPEx_Read_SuspendRegisters(&CrypHandle, SavedSUSPxR);    
    /*   Disable AES and save AES configuration */
    __HAL_CRYP_DISABLE(&CrypHandle);
    HAL_CRYPEx_Read_ControlRegister(&CrypHandle, SavedAES_CR);    
    
    
    /* Save low-priority message CRYP handle parameters */
    hcryp_saved = CrypHandle;
    

    /**************************************************************************/
    /*                                                                        */    
    /*               Start processing high priority message                   */
    /*                                                                        */    
    /**************************************************************************/  
        
    /* Configure AES to handle higher priority message */ 
    CrypHandle.Init.DataType      = CRYP_DATATYPE_32B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_256B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_ENCRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_GCM_GMAC;
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCM_INIT_PHASE;     
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey          = aAES256key; 
    CrypHandle.Init.pInitVect     = aIV_2;         
    CrypHandle.Init.Header        = aHeaderMessage_2_GCM; 
    CrypHandle.Init.HeaderSize    = GCM_HEADER_SIZE;        
      
      
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
    /*------------------------------------------------------------*/    
    /* GCM init phase */
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, NULL, 0, NULL)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for GCM init phase to finish */ 
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
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, aPlaintext_2_GCM, PAYLOAD_SIZE, aAESOutput_2)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for GCM payload phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
    /*------------------------------------------------------------*/    
    /* GCM final phase */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCMCMAC_FINAL_PHASE;
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, NULL, PAYLOAD_SIZE, ComputedAuthTAG_2)!= HAL_OK)
    {
      Error_Handler();
    } 
    /* Wait for GCM final phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);      
    /*------------------------------------------------------------*/

    
    
    /* Compare ciphered text to the expected one */
    data_cmp(aAESOutput_2, aCiphertext_2_GCM, PAYLOAD_SIZE);
    
    /* Compare computed authentication tag to the expected one */
    data_cmp(ComputedAuthTAG_2, ExpectedAuthTAG_2_GCM, AES_TAG_SIZE);
    
    
    /**************************************************************************/
    /*                                                                        */       
    /* Restore AES configuration to resume low-priority message header phase  */
    /*                                                                        */     
    /**************************************************************************/ 
    
    CrypHandle = hcryp_saved;   
    /* Save complete header size which will be used for last block
       construction during the final phase */
    FullHeaderSize = CrypHandle.Init.HeaderSize;
    /* Header phase will restart from where it stopped */
    CrypHandle.Init.Header        = CrypHandle.pCrypInBuffPtr; 
    CrypHandle.Init.HeaderSize    = CrypHandle.CrypInCount;
    
    /* Disable AES and write-back SUSPxR registers */;         
    __HAL_CRYP_DISABLE(&CrypHandle);
    /* Restore AES Suspend Registers */
    HAL_CRYPEx_Write_SuspendRegisters(&CrypHandle, SavedSUSPxR);
    /* Restore Control,  Key and IV Registers, then enable AES */
    HAL_CRYPEx_Write_ControlRegister(&CrypHandle, SavedAES_CR);
    HAL_CRYPEx_Write_KeyRegisters(&CrypHandle, aAES128key, CRYP_KEYSIZE_128B);
    HAL_CRYPEx_Write_IVRegisters(&CrypHandle, aIV_1);  
    __HAL_CRYP_ENABLE(&CrypHandle);
    
    
    /* Resume low priority GCM header phase */  
    /*------------------------------------------------------------*/    
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, NULL, 0, NULL)!= HAL_OK)
    {
      Error_Handler();
    } 
    /* Wait for GCM header phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
    /*------------------------------------------------------------*/
    /* GCM payload phase */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCM_PAYLOAD_PHASE;
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, aPlaintext_1_GCM, PAYLOAD_SIZE, aAESOutput_1)!= HAL_OK)
    {
      Error_Handler();
    }
    /**************************************************************************/
    /*                                                                        */
    /*          Low priority message second suspension request                */
    /*                                                                        */    
    /**************************************************************************/     
    /* While processing is on-going, set flag requesting to suspend the 
       on-going process */
    /* Depending on the user application, this flag may be set upon
      detection of a higher priority message to process */
    HAL_CRYPEx_ProcessSuspend(&CrypHandle);
    /* Wait for the IP to be suspended */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_SUSPENDED);    
    /*------------------------------------------------------------*/

    /**************************************************************************/
    /*                                                                        */
    /*               Low priority message GCM payload phase                   */
    /*                  processing parameters safeguard                       */
    /*                                                                        */    
    /**************************************************************************/ 
         
    /* At that point, CrypHandle processing is suspended.
       Save SUSPxR registers */
    HAL_CRYPEx_Read_SuspendRegisters(&CrypHandle, SavedSUSPxR);  
    /* Save IV registers */
    HAL_CRYPEx_Read_IVRegisters(&CrypHandle, SavedIV);       
    /*   Disable AES and save AES configuration */
    __HAL_CRYP_DISABLE(&CrypHandle);
    HAL_CRYPEx_Read_ControlRegister(&CrypHandle, SavedAES_CR);    
    
    
    /* Save low-priority message CRYP handle parameters */
    hcryp_saved = CrypHandle;
    

    /**************************************************************************/
    /*                                                                        */    
    /*     Start processing high priority message after 2nd suspension        */
    /*                   of low priority message                              */
    /*    (same high priority message as above for simplicity's sake)         */
    /*                                                                        */    
    /**************************************************************************/  
        
    /* Configure AES to handle higher priority message */ 
    CrypHandle.Init.DataType      = CRYP_DATATYPE_32B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_256B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_ENCRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_GCM_GMAC;
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCM_INIT_PHASE;     
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey          = aAES256key; 
    CrypHandle.Init.pInitVect     = aIV_2;         
    CrypHandle.Init.Header        = aHeaderMessage_2_GCM; 
    CrypHandle.Init.HeaderSize    = GCM_HEADER_SIZE;        
      
      
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
    /*------------------------------------------------------------*/    
    /* GCM init phase */
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, NULL, 0, NULL)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for GCM init phase to finish */ 
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
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, aPlaintext_2_GCM, PAYLOAD_SIZE, aAESOutput_2)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for GCM payload phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
    /*------------------------------------------------------------*/    
    /* GCM final phase */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCMCMAC_FINAL_PHASE;
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, NULL, PAYLOAD_SIZE, ComputedAuthTAG_2)!= HAL_OK)
    {
      Error_Handler();
    } 
    /* Wait for GCM final phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);      
    /*------------------------------------------------------------*/

    
    
    /* Compare ciphered text to the expected one */
    data_cmp(aAESOutput_2, aCiphertext_2_GCM, PAYLOAD_SIZE);
    
    /* Compare computed authentication tag to the expected one */
    data_cmp(ComputedAuthTAG_2, ExpectedAuthTAG_2_GCM, AES_TAG_SIZE);
    
    
    /**************************************************************************/
    /*                                                                        */         
    /* Restore AES configuration to resume low-priority message payload phase */
    /*                                                                        */       
    /**************************************************************************/ 

    CrypHandle = hcryp_saved;   

    /* payload phase will restart from where it stopped */
    pInput           =  CrypHandle.pCrypInBuffPtr;
    pOutput          =  CrypHandle.pCrypOutBuffPtr; 
    Size             =  CrypHandle.CrypInCount;

    
    /* Disable AES and write-back SUSPxR registers */;         
    __HAL_CRYP_DISABLE(&CrypHandle);
    /* Restore AES Suspend Registers */
    HAL_CRYPEx_Write_SuspendRegisters(&CrypHandle, SavedSUSPxR);
    /* Restore Control,  Key and IV Registers, then enable AES */
    HAL_CRYPEx_Write_ControlRegister(&CrypHandle, SavedAES_CR);
    HAL_CRYPEx_Write_KeyRegisters(&CrypHandle, aAES128key, CRYP_KEYSIZE_128B);
    HAL_CRYPEx_Write_IVRegisters(&CrypHandle, SavedIV);  
    __HAL_CRYP_ENABLE(&CrypHandle);
    
    
    /**************************************************************************/   
    /* Resume low priority GCM payload phase  */
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, pInput, Size, pOutput)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for GCM payload phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
    /*------------------------------------------------------------*/    
    /* GCM final phase */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCMCMAC_FINAL_PHASE;
    /* Restore complete header size for last block construction 
       (CrypHandle.Init.HeaderSize was set to a smaller size when
       low-priority message header phase was resumed).
    */
    CrypHandle.Init.HeaderSize    = FullHeaderSize;      
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, NULL, PAYLOAD_SIZE, ComputedAuthTAG_1)!= HAL_OK)
    {
      Error_Handler();
    } 
    /* Wait for GCM final phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);      
    /*------------------------------------------------------------*/



    /* Compare ciphered text to the expected one */
    data_cmp(aAESOutput_1, aCiphertext_1_GCM, PAYLOAD_SIZE);
    
    /* Compare computed authentication tag to the expected one */
    data_cmp(ComputedAuthTAG_1, ExpectedAuthTAG_1_GCM, AES_TAG_SIZE);


  


  printf("\n======================================\n ");
  printf("\n\r GCM processing suspend/resume done.\n ");
  printf("No issue detected.\n ");
  
  
  
    /**************************************************************************/
    /*                                                                        */      
    /*                                                                        */
    /*            AES 256 GMAC low-priority message tag computation           */
    /*            interrupted by AES 256 GCM high priority message            */
    /*                   encryption and tag computation                       */
    /*                       (interrupt mode)                                 */
    /*                                                                        */
    /*                                                                        */          
    /**************************************************************************/  
  
  
    /**************************************************************************/
    /*                                                                        */       
    /*                Start processing low priority message                   */
    /*                                                                        */       
    /**************************************************************************/    
  
    CrypHandle.Init.DataType      = CRYP_DATATYPE_1B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_256B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_TAG_GENERATION;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_GCM_GMAC;
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCM_INIT_PHASE;     
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey          = aAES256key_GMAC;
    CrypHandle.Init.pInitVect     = aIV_GMAC;         
    CrypHandle.Init.Header        = aHeaderMessage_1_GMAC; 
    CrypHandle.Init.HeaderSize    = GMAC_HEADER_SIZE;     
    
    
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
    
    /*------------------------------------------------------------*/    
    /* GMAC init phase */
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, NULL, 0, NULL)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for GMAC init phase to finish
       For simplicity's sake, this example is just waiting till the end of the 
       process, but application may perform other tasks while phase processing
       is ongoing. */ 
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
    /*------------------------------------------------------------*/
    /* GMAC header phase */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCMCMAC_HEADER_PHASE;
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, NULL, 0, NULL)!= HAL_OK)
    {
      Error_Handler();
    } 
    /**************************************************************************/
    /*                                                                        */
    /*                Low priority message suspension request                 */
    /*                                                                        */    
    /**************************************************************************/        
    /* While processing is on-going, set flag requesting to suspend the 
       on-going process */
    /* Depending on the user application, this flag may be set upon
      detection of a higher priority message to process */
    HAL_CRYPEx_ProcessSuspend(&CrypHandle);
    /* Wait for the IP to be suspended */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_SUSPENDED);
  
    /**************************************************************************/
    /*                                                                        */
    /*               Low priority message GMAC header phase                   */
    /*                  processing parameters safeguard                       */
    /*                                                                        */    
    /**************************************************************************/ 
                
    /* At that point, CrypHandle processing is suspended.
       Save SUSPxR registers */
    HAL_CRYPEx_Read_SuspendRegisters(&CrypHandle, SavedSUSPxR);    
    /*   Disable AES and save AES configuration */
    __HAL_CRYP_DISABLE(&CrypHandle);
    HAL_CRYPEx_Read_ControlRegister(&CrypHandle, SavedAES_CR);    
    
    
    /* Save low-priority message CRYP handle parameters */
    hcryp_saved = CrypHandle;
    

    /**************************************************************************/
    /*                                                                        */      
    /*               Start processing high priority message                   */
    /*                                                                        */      
    /**************************************************************************/  
        
    /* Configure AES to handle higher priority message */ 
    CrypHandle.Init.DataType      = CRYP_DATATYPE_32B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_256B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_ENCRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_GCM_GMAC;
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCM_INIT_PHASE;     
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey          = aAES256key; 
    CrypHandle.Init.pInitVect     = aIV_2;         
    CrypHandle.Init.Header        = aHeaderMessage_2_GCM; 
    CrypHandle.Init.HeaderSize    = GCM_HEADER_SIZE;        
      
      
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
    /*------------------------------------------------------------*/    
    /* GCM init phase */
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, NULL, 0, NULL)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for GCM init phase to finish */ 
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
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, aPlaintext_2_GCM, PAYLOAD_SIZE, aAESOutput_2)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for GCM payload phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
    /*------------------------------------------------------------*/    
    /* GCM final phase */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCMCMAC_FINAL_PHASE;
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, NULL, PAYLOAD_SIZE, ComputedAuthTAG_2)!= HAL_OK)
    {
      Error_Handler();
    } 
    /* Wait for GCM final phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);      
    /*------------------------------------------------------------*/

    
    
    /* Compare ciphered text to the expected one */
    data_cmp(aAESOutput_2, aCiphertext_2_GCM, PAYLOAD_SIZE);
    
    /* Compare computed authentication tag to the expected one */
    data_cmp(ComputedAuthTAG_2, ExpectedAuthTAG_2_GCM, AES_TAG_SIZE);
    
    
    /**************************************************************************/
    /*                                                                        */         
    /* Restore AES configuration to resume low-priority message header phase  */
    /*                                                                        */       
    /**************************************************************************/ 
    
    CrypHandle = hcryp_saved;   
    /* Save complete header size which will be used for last block
       construction during the final phase */
    FullHeaderSize = CrypHandle.Init.HeaderSize;
    /* Header phase will restart from where it stopped */
    CrypHandle.Init.Header        = CrypHandle.pCrypInBuffPtr; 
    CrypHandle.Init.HeaderSize    = CrypHandle.CrypInCount;
    
    /* Disable AES and write-back SUSPxR registers */;         
    __HAL_CRYP_DISABLE(&CrypHandle);
    /* Restore AES Suspend Registers */
    HAL_CRYPEx_Write_SuspendRegisters(&CrypHandle, SavedSUSPxR);
    /* Restore Control, Key and IV Registers, then enable AES */
    HAL_CRYPEx_Write_ControlRegister(&CrypHandle, SavedAES_CR);
    HAL_CRYPEx_Write_KeyRegisters(&CrypHandle, aAES256key_GMAC, CRYP_KEYSIZE_256B);
    HAL_CRYPEx_Write_IVRegisters(&CrypHandle, aIV_GMAC);  
    __HAL_CRYP_ENABLE(&CrypHandle);
    
    
    /* Resume low priority GMAC header phase */  
    /*------------------------------------------------------------*/    
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, NULL, 0, NULL)!= HAL_OK)
    {
      Error_Handler();
    } 
    /* Wait for GMAC header phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
    /*------------------------------------------------------------*/   
    /* GMAC final phase */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCMCMAC_FINAL_PHASE;
    /* Restore complete header size for last block construction 
       (CrypHandle.Init.HeaderSize was set to a smaller size when
       low-priority message header phase was restarted).
    */
    CrypHandle.Init.HeaderSize    = FullHeaderSize;      
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, NULL, 0, ComputedAuthTAG_1)!= HAL_OK)
    {
      Error_Handler();
    } 
    /* Wait for GMAC final phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);      
    /*------------------------------------------------------------*/

    
    /* Compare computed authentication tag to the expected one */
    data_cmp(ComputedAuthTAG_1, ExpectedAuthTAG_1_GMAC, AES_TAG_SIZE);

  
  printf("\n=======================================\n ");
  printf("\n\r GMAC processing suspend/resume done.\n ");
  printf("No issue detected.\n ");
  


    /**************************************************************************/
    /*                                                                        */
    /*                                                                        */      
    /*            AES 128 CMAC low-priority message tag computation           */
    /*            interrupted by AES 256 GCM high priority message            */
    /*                   encryption and tag computation                       */
    /*                       (interrupt mode)                                 */
    /*                                                                        */
    /*                                                                        */          
    /**************************************************************************/  
  
  
    /**************************************************************************/
    /*                                                                        */       
    /*                Start processing low priority message                   */
    /*                                                                        */       
    /**************************************************************************/    
  
    CrypHandle.Init.DataType      = CRYP_DATATYPE_32B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_128B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_TAG_GENERATION;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_CMAC;
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCMCMAC_HEADER_PHASE;     
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey          = aAES128key_CMAC;       
    CrypHandle.Init.Header        = aHeaderMessage_1_CMAC; 
    CrypHandle.Init.HeaderSize    = CMAC_HEADER_SIZE;     
    
    
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
    

    /*------------------------------------------------------------*/
    /* CMAC header phase */
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, (uint8_t *)BBlocks, CMAC_B_SIZE, NULL)!= HAL_OK)
    {
      Error_Handler();
    } 
    /**************************************************************************/
    /*                                                                        */
    /*                Low priority message suspension request                 */
    /*                                                                        */    
    /**************************************************************************/        
    /* While processing is on-going, set flag requesting to suspend the 
       on-going process */
    /* Depending on the user application, this flag may be set upon
      detection of a higher priority message to process */
    HAL_CRYPEx_ProcessSuspend(&CrypHandle);
    /* Wait for the IP to be suspended */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_SUSPENDED);
  
       
    /**************************************************************************/
    /*                                                                        */
    /*               Low priority message CMAC header phase                   */
    /*                  processing parameters safeguard                       */
    /*                                                                        */    
    /**************************************************************************/ 
                
    /* At that point, CrypHandle processing is suspended.
       Save SUSPxR registers */
    HAL_CRYPEx_Read_SuspendRegisters(&CrypHandle, SavedSUSPxR);
    /* Save IV registers */
    HAL_CRYPEx_Read_IVRegisters(&CrypHandle, SavedIV);           
    /*   Disable AES and save AES configuration */
    __HAL_CRYP_DISABLE(&CrypHandle);
    HAL_CRYPEx_Read_ControlRegister(&CrypHandle, SavedAES_CR);    
    
    
    /* Save low-priority message CRYP handle parameters */
    hcryp_saved = CrypHandle;
    

    /**************************************************************************/
    /*                                                                        */      
    /*               Start processing high priority message                   */
    /*                                                                        */      
    /**************************************************************************/  
        
    /* Configure AES to handle higher priority message */ 
    CrypHandle.Init.DataType      = CRYP_DATATYPE_32B;
    CrypHandle.Init.KeySize       = CRYP_KEYSIZE_256B;
    CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_ENCRYPT;
    CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_GCM_GMAC;
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCM_INIT_PHASE;     
    CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE;  
    CrypHandle.Init.pKey          = aAES256key; 
    CrypHandle.Init.pInitVect     = aIV_2;         
    CrypHandle.Init.Header        = aHeaderMessage_2_GCM; 
    CrypHandle.Init.HeaderSize    = GCM_HEADER_SIZE;        
      
      
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
    /*------------------------------------------------------------*/    
    /* GCM init phase */
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, NULL, 0, NULL)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for GCM init phase to finish */ 
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
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, aPlaintext_2_GCM, PAYLOAD_SIZE, aAESOutput_2)!= HAL_OK)
    {
      Error_Handler();
    }
    /* Wait for GCM payload phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
    /*------------------------------------------------------------*/    
    /* GCM final phase */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCMCMAC_FINAL_PHASE;
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, NULL, PAYLOAD_SIZE, ComputedAuthTAG_2)!= HAL_OK)
    {
      Error_Handler();
    } 
    /* Wait for GCM final phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);      
    /*------------------------------------------------------------*/

    
    
    /* Compare ciphered text to the expected one */
    data_cmp(aAESOutput_2, aCiphertext_2_GCM, PAYLOAD_SIZE);
    
    /* Compare computed authentication tag to the expected one */
    data_cmp(ComputedAuthTAG_2, ExpectedAuthTAG_2_GCM, AES_TAG_SIZE);
    
    
    /**************************************************************************/
    /*                                                                        */        
    /* Restore AES configuration to resume low-priority message header phase  */
    /*                                                                        */      
    /**************************************************************************/ 
    
    CrypHandle = hcryp_saved;   

    /* Header phase will restart from where it stopped */
    /* Have B blocks all be processed or are there still some remaining ? */
    if (CrypHandle.CrypInCount > CrypHandle.Init.HeaderSize)
    {
     /* Some B blocks still remain */
      pBlock = CrypHandle.pCrypInBuffPtr;
      Bsize = CrypHandle.CrypInCount - CrypHandle.Init.HeaderSize;
    }
    else
    {
      /* All B blocks have been processed, suspension occurred during header processing */
      pBlock = NULL;
      Bsize = 0;  
      CrypHandle.Init.Header        = CrypHandle.pCrypInBuffPtr; 
      CrypHandle.Init.HeaderSize    = CrypHandle.CrypInCount;
    }
    
    /* Disable AES and write-back SUSPxR registers */;         
    __HAL_CRYP_DISABLE(&CrypHandle);
    /* Restore AES Suspend Registers */
    HAL_CRYPEx_Write_SuspendRegisters(&CrypHandle, SavedSUSPxR);
    /* Restore Control, Key and IV Registers, then enable AES */
    HAL_CRYPEx_Write_ControlRegister(&CrypHandle, SavedAES_CR);
    HAL_CRYPEx_Write_KeyRegisters(&CrypHandle, aAES128key_CMAC, CRYP_KEYSIZE_128B);
    HAL_CRYPEx_Write_IVRegisters(&CrypHandle, SavedIV);  
    __HAL_CRYP_ENABLE(&CrypHandle);
    
    
    /* Resume low priority CMAC header phase */  
    /*------------------------------------------------------------*/    
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, pBlock, Bsize, NULL)!= HAL_OK)
    {
      Error_Handler();
    } 
    /* Wait for CMAC header phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);
    /*------------------------------------------------------------*/   
    /* CMAC final phase */
    CrypHandle.Init.GCMCMACPhase  = CRYP_GCMCMAC_FINAL_PHASE;     
    if (HAL_CRYPEx_AES_Auth_IT(&CrypHandle, CBlock, CMAC_C_SIZE, ComputedAuthTAG_1)!= HAL_OK)
    {
      Error_Handler();
    } 
    /* Wait for CMAC final phase to finish */
    while (HAL_CRYP_GetState(&CrypHandle) != HAL_CRYP_STATE_READY);      
    /*------------------------------------------------------------*/

    
    /* Compare computed authentication tag to the expected one */
    data_cmp(ComputedAuthTAG_1, ExpectedAuthTAG_1_CMAC, AES_TAG_SIZE);

  
  printf("\n=======================================\n ");
  printf("\n\r CMAC processing suspend/resume done.\n ");
  printf("No issue detected.\n ");
  
  /* Turn LED1 on */
  BSP_LED_On(LED1);
  
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

  /* Turn LED3 on */
  BSP_LED_On(LED3);

  while (1)
  {
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

