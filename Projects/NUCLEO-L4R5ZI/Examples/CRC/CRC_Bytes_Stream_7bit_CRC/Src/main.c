/**
  ******************************************************************************
  * @file    CRC/CRC_Bytes_Stream_7bit_CRC/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the STM32L4xx CRC HAL API
  *          to compute 7-bit CRC codes from buffers of data bytes (8-bit data),
  *          based on a user-defined generating polynomial.
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

/** @addtogroup CRC_Bytes_Stream_7bit_CRC
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE_5  5  /* CRC7_DATA8_TEST5[] is 5-byte long   */ 
#define BUFFER_SIZE_17 17 /* CRC7_DATA8_TEST17[] is 17-byte long */ 
#define BUFFER_SIZE_1  1  /* CRC7_DATA8_TEST1[] is 1-byte long   */ 
#define BUFFER_SIZE_2  2  /* CRC7_DATA8_TEST2[] is 2-byte long   */ 

/* User-defined polynomial */
#define CRC_POLYNOMIAL_7B  0x65  /* X^7 + X^6 + X^5 + X^2 + 1,
                                   used in Train Communication Network, IEC 60870-5[17] */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* CRC handler declaration */
CRC_HandleTypeDef   CrcHandle;

/* Used for storing CRC Value */
__IO uint32_t uwCRCValue = 0;

/* Bytes buffers that will consecutively yield CRCs */
static const uint8_t CRC7_DATA8_TEST5[5]   = {0x12,0x34,0xBA,0x71,0xAD};
static const uint8_t CRC7_DATA8_TEST17[17] = {0x12,0x34,0xBA,0x71,0xAD,
                                              0x11,0x56,0xDC,0x88,0x1B,
                                              0xEE,0x4D,0x82, 0x93,0xA6,
                                              0x7F,0xC3};
static const uint8_t CRC7_DATA8_TEST1[1]   = {0x19};                                                
static const uint8_t CRC7_DATA8_TEST2[2]   = {0xAB,0xCD};

       

/* Expected CRC Values */
/* The 7 LSB bits are the 7-bit long CRC */
uint32_t uwExpectedCRCValue_1 = 0x00000057;    /* First byte stream CRC  */
uint32_t uwExpectedCRCValue_2 = 0x0000006E;    /* Second byte stream CRC */
uint32_t uwExpectedCRCValue_3 = 0x0000004B;    /* Third byte stream CRC  */
uint32_t uwExpectedCRCValue_4 = 0x00000026;    /* Fourth byte stream CRC */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  
  /* Configure the system clock to 120 MHz */
  SystemClock_Config();

  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);


  /****************************************************************************/
  /*                                                                          */
  /*                     CRC peripheral initialization                        */
  /*                                                                          */    
  /****************************************************************************/
    
  CrcHandle.Instance = CRC;

  /* The default polynomial is not used. The one to be used must be defined 
     in CrcHandle.Init.GeneratingPolynomial */  
  CrcHandle.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_DISABLE;
  
  /* Set the value of the generating polynomial. 
    The one used in that example is the 7-bit long CRC generating
    polynomial X^7 + X^6 + X^5 + X^2 + 1 */
  CrcHandle.Init.GeneratingPolynomial    = CRC_POLYNOMIAL_7B;
  
  /* The user-defined generating polynomial yields a 7-bit long CRC */
  CrcHandle.Init.CRCLength               = CRC_POLYLENGTH_7B;

  /* The default init value is used */
  CrcHandle.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_ENABLE;

  /* The input data are not inverted */
  CrcHandle.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE;

  /* The output data are not inverted */
  CrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;

  /* The input data are bytes (8-bit long data) */
  CrcHandle.InputDataFormat              = CRC_INPUTDATA_FORMAT_BYTES;

  /* De-initialize the CRC peripheral */
  if (HAL_CRC_DeInit(&CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }  

  /* Then, initialize the CRC handle */
  if (HAL_CRC_Init(&CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }


  /****************************************************************************/
  /*                                                                          */
  /*         CRC computation of a first bytes stream                          */
  /*                                                                          */    
  /****************************************************************************/

  /* The 7-bit long CRC of a 5-byte buffer is computed. After peripheral initialization, 
     the CRC calculator is initialized with the default value that is 0x7F for 
     a 7-bit CRC.
    
    The computed CRC is stored in uint32_t uwCRCValue. The 7-bit long CRC is made of
    uwCRCValue 7 LSB bits. */

  uwCRCValue = HAL_CRC_Accumulate(&CrcHandle, (uint32_t *)&CRC7_DATA8_TEST5, BUFFER_SIZE_5);

  /* Compare the CRC value to the expected one */
  if (uwCRCValue != uwExpectedCRCValue_1)
  {
    /* Wrong CRC value: Turn LED3 on */
    Error_Handler();
  }

  
  /****************************************************************************/
  /*                                                                          */
  /*         CRC computation of a second bytes stream                         */
  /*                                                                          */    
  /****************************************************************************/

  /* The 7-bit long CRC of a 17-byte buffer is computed. The CRC calculator
    is not re-initialized, instead the previously computed CRC is used
    as initial value. */

  uwCRCValue = HAL_CRC_Accumulate(&CrcHandle, (uint32_t *)&CRC7_DATA8_TEST17, BUFFER_SIZE_17);

  /* Compare the CRC value to the expected one */
  if (uwCRCValue != uwExpectedCRCValue_2)
  {
    /* Wrong CRC value: Turn LED3 on */
    Error_Handler();
  }


  /****************************************************************************/
  /*                                                                          */
  /*         CRC computation of a single byte                                 */
  /*                                                                          */    
  /****************************************************************************/

  /* The 7-bit long CRC of a 1-byte buffer is computed. The CRC calculator
    is not re-initialized, instead the previously computed CRC is used
    as initial value. */

  uwCRCValue = HAL_CRC_Accumulate(&CrcHandle, (uint32_t *)&CRC7_DATA8_TEST1, BUFFER_SIZE_1);

  /* Compare the CRC value to the expected one */
  if (uwCRCValue != uwExpectedCRCValue_3)
  {
    /* Wrong CRC value: Turn LED3 on */
    Error_Handler();
  }


  /****************************************************************************/
  /*                                                                          */
  /*         CRC computation of the last bytes stream                         */
  /*                                                                          */    
  /****************************************************************************/

  /* The 7-bit long CRC of a 2-byte buffer is computed. The CRC calculator
    is re-initialized with the default value that is 0x7F for a 7-bit CRC.
    This is done with a call to HAL_CRC_Calculate() instead of 
    HAL_CRC_Accumulate(). */

  uwCRCValue = HAL_CRC_Calculate(&CrcHandle, (uint32_t *)&CRC7_DATA8_TEST2, BUFFER_SIZE_2);

  /* Compare the CRC value to the expected one */
  if (uwCRCValue != uwExpectedCRCValue_4)
  {
    /* Wrong CRC value: Turn LED3 on */
    Error_Handler();
  }
  else
  {
    /* Right CRC value: Turn LED1 on */
    BSP_LED_On(LED1);
  }  


  /* Infinite loop */
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

