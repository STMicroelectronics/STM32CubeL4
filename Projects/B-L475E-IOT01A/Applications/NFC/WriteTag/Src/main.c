/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   This file describe the main program.
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
  
/* 
 * This program is designed for a L475 IoT Dk board using HAL library. 
 * The program initialize the NFC component, then program the tags 
 * according to some NDEF classes available
 */
 
/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "stm32l475e_iot01.h"
#include "lib_TT4_interface.h"
#include "m24sr.h"
#include "VcardCSL1.h"

/** @addtogroup NFC_TagType4_NDEF_Applications
  * @{
  */

/** @addtogroup M24SR_Applications
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[20]; 
  uint32_t DemoIndex;
}BSP_DemoTypedef;

/* Private define ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void AARwrite_demo(void);
static void URIwrite_demo(void);
static void Vcardwrite_demo(void);
static void Vcardwrite_demo2(void);
static void SMSwrite_demo(void);
static void EMAILwrite_demo(void);
static void Error_Handler(void);
static void Toggle_Led2(uint32_t freq);
static void SystemClock_Config(void);

/* Private variables ---------------------------------------------------------*/
uint8_t UserPressButton = 0;
uint8_t DemoIndex = 0;
BSP_DemoTypedef  NFC_examples[]={
  {URIwrite_demo, "URI", 0}, 
  {SMSwrite_demo, "SMS", 1},
  {EMAILwrite_demo, "EMAIL", 2},
  {Vcardwrite_demo, "vCARD", 3},
  {Vcardwrite_demo2, "vCARD2", 4},
  {AARwrite_demo, "AAR", 5}, 
};

/* Field to fill for the SMS demonstration */
char PhoneNumber[] = {"+33612345678\0"};
char Message[] = {"This SMS was generated automatically by tapping your phone near BM1297\0"};
char Instruction[] = {"This is an example of NFC generated SMS using BM1297 board\0"};
/* Field to fill for the Vcard demonstration */
char FirstName[] = {"Help Desk\0"};
char Title[] = {"Customer support\0"};
char Org[] = {"STMicroelectronics\0"};
char HomeAddress[] = {"\0"};
char WorkAddress[] = {"190 avenue Celestin Coq 13106 ROUSSET FRANCE\0"};
char HomeTel[] = {"\0"};
char WorkTel[] = {"\0"};
char CellTel[] = {"+33 6 11 22 33 44\0"};
char HomeEmail[] = {"\0"};
char WorkEmail[] = {"customer.service@st.com \0"}; 

/* Field to fill for the Email demonstration */
char EmailAdd[] = {"customer.service@st.com\0"};
char Subject[] = {"M24SR S/N 754FHFGJF46G329 WARRANTY\0"};
char EmailMessage[] = {"this is a demonstration to illustrate an automatic warranty activation email\0"};
char Information[] = {"automatic warranty\0"};



/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{    
  GPIO_InitTypeDef  gpioinitstruct = {0};

  /* Reset of all peripherals, Initializes the Flash interface and the systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize User LED2*/
  BSP_LED_Init(LED2);
  
  /* Initialize Applic LED1*/
  __HAL_RCC_GPIOA_CLK_ENABLE();
  gpioinitstruct.Pin   = GPIO_PIN_5;
  gpioinitstruct.Mode  = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull  = GPIO_NOPULL;
  gpioinitstruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gpioinitstruct);

  /* Configure the User Button in GPIO Mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  
  /* Init of the Type Tag 4 component (M24SR)
     Thanks to a call to KillSession command during init no issue can occurs
     If customer modify the code to avoid Kill session command call,
     he must retry Init until succes (session can be lock by RF ) */
  while (TT4_Init() != SUCCESS);
  
  /* Set the LED2 on to indicate Init done */
  BSP_LED_On(LED2);

  while (1)
  {
    UserPressButton = 0;
    NFC_examples[DemoIndex++].DemoFunc();
    
    /* Toggle LEDs (different speed between each Test) */
    while (!UserPressButton) Toggle_Led2(80*(DemoIndex+1));
    /* If all Demo has been already executed, Reset DemoIndex to restart BSP example*/
    if(DemoIndex >= COUNT_OF_EXAMPLE(NFC_examples))
    {
      DemoIndex = 0;
    }
  }
}


static void URIwrite_demo(void)
{
  sURI_Info URI;

  /* Prepare URI NDEF message content */
  strcpy(URI.protocol,URI_ID_0x01_STRING);
  strcpy(URI.URI_Message,"st.com");
  strcpy(URI.Information,"\0");
  
  /* First write NDEF */
  while (TT4_WriteURI(&URI) != SUCCESS);
}  

static void SMSwrite_demo(void)
{
  uint16_t status = ERROR;
  sSMSInfo SMSStruct;
  sSMSInfo *pSMSStruct;
  
  pSMSStruct = &SMSStruct;
  
  memcpy(pSMSStruct->PhoneNumber, PhoneNumber, strlen(PhoneNumber)+1);
  memcpy(pSMSStruct->Message, Message, strlen(Message)+1);
  memcpy(pSMSStruct->Information, Instruction, strlen(Instruction)+1);  
  
  status = TT4_WriteSMS ( pSMSStruct );

  if(status != SUCCESS)
  {
    Error_Handler();    
  }
}

static void EMAILwrite_demo(void)
{
  uint16_t status = ERROR;
  sEmailInfo EmailStruct;
  sEmailInfo *pEmailStruct;
  
  pEmailStruct = &EmailStruct;
  
  memcpy(pEmailStruct->EmailAdd, EmailAdd, strlen(EmailAdd)+1);
  memcpy(pEmailStruct->Subject, Subject, strlen(Subject)+1);
  memcpy(pEmailStruct->Message, EmailMessage, strlen(EmailMessage)+1);
  memcpy(pEmailStruct->Information, Information, strlen(Information)+1);
  
  status = TT4_WriteEmail ( pEmailStruct );

  if(status != SUCCESS)
  {
    Error_Handler();    
  }
}

static void Vcardwrite_demo(void)
{
  uint16_t status = ERROR;

  /* USee a NDEF file Vcard with a default Jpeg embedded */
  status = TT4_WriteNDEF((uint8_t*)uVcardCSL1);
  if(status != SUCCESS)
  {
    Error_Handler();    
  }
}

static void Vcardwrite_demo2(void)
{
  uint16_t status = ERROR;
  sVcardInfo VcardStruct;
  sVcardInfo *pVcardStruct;
  
  pVcardStruct = &VcardStruct;
  
  memcpy(pVcardStruct->FirstName, FirstName, strlen(FirstName)+1);
  memcpy(pVcardStruct->Title, Title, strlen(Title)+1);
  memcpy(pVcardStruct->Org, Org, strlen(Org)+1);
  memcpy(pVcardStruct->HomeAddress, HomeAddress, strlen(HomeAddress)+1);
  memcpy(pVcardStruct->WorkAddress, WorkAddress, strlen(WorkAddress)+1);
  memcpy(pVcardStruct->HomeTel, HomeTel, strlen(HomeTel)+1);
  memcpy(pVcardStruct->WorkTel, WorkTel, strlen(WorkTel)+1);
  memcpy(pVcardStruct->CellTel, CellTel, strlen(CellTel)+1);
  memcpy(pVcardStruct->HomeEmail, HomeEmail, strlen(HomeEmail)+1);
  memcpy(pVcardStruct->WorkEmail, WorkEmail, strlen(WorkEmail)+1);
 
  status = TT4_WriteVcard ( &VcardStruct );
  if(status != SUCCESS)
  {
    Error_Handler();    
  }
}

static void AARwrite_demo(void)
{
  uint16_t status = ERROR;
  uint8_t NULL_NDEF[2] = {0,0};
  sAARInfo AAR_struct;
  sAARInfo *pAAR;
  
  pAAR = &AAR_struct;

  /* to set only AAR force NDEF size to 0 before */
  TT4_WriteNDEF(NULL_NDEF);
  memcpy(pAAR->PakageName, "com.stm.bluetoothlevalidation", strlen("com.stm.bluetoothlevalidation")+1);
    

  /* Add AAR record */
  status = TT4_AddAAR(pAAR);
  if(status != SUCCESS)
  {
    Error_Handler();    
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
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
static void SystemClock_Config(void)
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
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  return;
}


/**
  * @brief  Toggle LEDs
  * @param  None
  * @retval None
  */
static void Toggle_Led2(uint32_t freq)
{
  BSP_LED_Toggle(LED2);
  HAL_Delay(freq);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);  /* LED1 */
  while (1)
  {
  }
}

/* Public functions ---------------------------------------------------------*/

/**
  * @brief EXTI line detection callback.
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == NFC_GPIO_GPO_PIN)
  {
    M24SR_GPO_Callback();
  }

  if (USER_BUTTON_PIN == GPIO_Pin)
  {
    while (BSP_PB_GetState(BUTTON_USER) == RESET);
    UserPressButton = 1;
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
void assert_failed(uint8_t* file, uint32_t line)
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

