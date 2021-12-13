/**
  ******************************************************************************
  * @file    RCC/RCC_CRS_Synchronization_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use the RCC HAL API to configure the
  *          system clock (SYSCLK) and modify the clock settings on run time.
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

/** @addtogroup RCC_CRS_Synchronization_IT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CRS_STATUS_INIT       0
#define CRS_STATUS_SYNCOK     1
#define CRS_STATUS_SYNCWARN   2
#define CRS_STATUS_SYNCMISS   3
#define CRS_STATUS_SYNCERR    4


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RCC_CRSInitTypeDef CRSInitStruct = {0};

/* Variable to save SYNC status*/
__IO uint32_t CRSSyncStatus = CRS_STATUS_INIT;

/* Private function prototypes -----------------------------------------------*/
static void CRS_Init(void);
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
  RCC_CRSSynchroInfoTypeDef syncinfo = {0};
  
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

  /* Configure the PLL to reach the max system frequency (80 MHz) */
  SystemClock_Config();

  /* Configure LED3 */
  BSP_LED_Init(LED3);

  /* CRS initialization (enable HSI48 and LSE oscillators and then enable CRS clock */
  /* Enable CRS interrupt */
  CRS_Init();
  
  /* HSI48 Synchronization without synchronization frequency (source set to GPIO */
  /* which is not configured)*/
  CRSInitStruct.Prescaler             = RCC_CRS_SYNC_DIV1;
  CRSInitStruct.Source                = RCC_CRS_SYNC_SOURCE_GPIO;
  CRSInitStruct.Polarity              = RCC_CRS_SYNC_POLARITY_FALLING;
  /* F(LSE)=32.768kHz then Reload=(f(Target)/f(LSE))-1= 0x5B7*/
  CRSInitStruct.ReloadValue           = __HAL_RCC_CRS_RELOADVALUE_CALCULATE(HSI48_VALUE, LSE_VALUE);
  /* Felim value calculated like this FELIM = (fTARGET / fSYNC) * STEP[%] / 100% / 2 with STEP=0.14% then FELIM = 2 */
  CRSInitStruct.ErrorLimitValue       = 2;
  /* Change the HSI trimming value to see the automatic calibration performed by CRS */
  CRSInitStruct.HSI48CalibrationValue = 0x00;

  /* Start automatic synchronization*/
  HAL_RCCEx_CRSConfig(&CRSInitStruct);

  /* Enable CRS interrupts */
  __HAL_RCC_CRS_ENABLE_IT(RCC_CRS_IT_SYNCOK | RCC_CRS_IT_SYNCWARN | RCC_CRS_IT_ERR | RCC_CRS_IT_ESYNC);

  /* Simulate a SYNC EVENT which will raise a SYNC_MISS EVENT */ 
  HAL_RCCEx_CRSSoftwareSynchronizationGenerate();

  /* Infinite loop */
  while (1)
  {
    switch(CRSSyncStatus)
    {
    case CRS_STATUS_SYNCOK:
      /* Power on LED3 */
      BSP_LED_On(LED3);
      break;

    case CRS_STATUS_SYNCMISS:
      /* Power off LED3 */
      BSP_LED_Off(LED3);
      /* Perform a new configuration to LSE when SYNC MISS is detected. */
      /* For visual comfort on LED3 apply delay before new configuration */
      HAL_Delay(1000);
      
      /* Retrieve synchronization information */
      HAL_RCCEx_CRSGetSynchronizationInfo(&syncinfo);
    
      /* Change source to LSE and restart synchronization if necessary */
      if(CRSInitStruct.Source != RCC_CRS_SYNC_SOURCE_LSE)
      {
        CRSInitStruct.Source = RCC_CRS_SYNC_SOURCE_LSE;
        HAL_RCCEx_CRSConfig(&CRSInitStruct);

        /* Enable CRS interrupts after new CRS configuration */
        __HAL_RCC_CRS_ENABLE_IT(RCC_CRS_IT_SYNCOK | RCC_CRS_IT_SYNCWARN | RCC_CRS_IT_ERR | RCC_CRS_IT_ESYNC);
      }
      else
      {
        /* Enable CRS SYNCOK interrupt */
        __HAL_RCC_CRS_ENABLE_IT(RCC_CRS_IT_SYNCOK);
      }
      break;
      
    case CRS_STATUS_SYNCWARN:
      /* Toggle LED3 when SYNC WARN is detected. */
      BSP_LED_On(LED3);
      HAL_Delay(100);
      BSP_LED_Off(LED3);
      HAL_Delay(100);
      BSP_LED_On(LED3);
      HAL_Delay(100);
      BSP_LED_Off(LED3);
      HAL_Delay(100);
      BSP_LED_On(LED3);
      HAL_Delay(100);
      BSP_LED_Off(LED3);
      /* Increase tolerance (FELIM value) until no more warning */
      CRSInitStruct.ErrorLimitValue += 1;
      HAL_RCCEx_CRSConfig(&CRSInitStruct);
      /* Enable CRS interrupts after new CRS configuration */
      __HAL_RCC_CRS_ENABLE_IT(RCC_CRS_IT_SYNCOK | RCC_CRS_IT_SYNCWARN | RCC_CRS_IT_ERR | RCC_CRS_IT_ESYNC);
      break;

    case CRS_STATUS_SYNCERR:
      /* Power off LED3 */
      BSP_LED_Off(LED3);
      break;

    default:
      break;
    }
  }
}

/**
  * @brief  This function initializes the clock configuration for CRS.
  * @param  None
  * @retval None
  */
static void CRS_Init(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

  if (RCC_OscInitStruct.HSI48State != RCC_HSI48_ON)
  {
    /* Enable HSI48 and LSE Oscillator*/
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSI48State     = RCC_HSI48_ON;
    RCC_OscInitStruct.LSEState       = RCC_LSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
  }
  
  /* Enable CRS clock*/
  __HAL_RCC_CRS_CLK_ENABLE();

  /* Enable and set CRS interrupt*/
  HAL_NVIC_SetPriority(CRS_IRQn, 0x03, 0x00);
  HAL_NVIC_EnableIRQ(CRS_IRQn);

  /* Output HSI48 MCO pin(PA8) */
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI48, RCC_MCODIV_1);
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
  * @brief  RCCEx Clock Recovery System SYNCOK interrupt callback.
  * @retval none
  */
void HAL_RCCEx_CRS_SyncOkCallback(void)
{
  CRSSyncStatus = CRS_STATUS_SYNCOK;

  /* Disable CRS SYNCOK interrupt since synchronization succeeded */
  __HAL_RCC_CRS_DISABLE_IT(RCC_CRS_IT_SYNCOK);
}

/**
  * @brief  RCCEx Clock Recovery System SYNCWARN interrupt callback.
  * @retval none
  */
void HAL_RCCEx_CRS_SyncWarnCallback(void)
{
  CRSSyncStatus = CRS_STATUS_SYNCWARN;
}

/**
  * @brief  RCCEx Clock Recovery System Expected SYNC interrupt callback.
  * @retval none
  */
void HAL_RCCEx_CRS_ExpectedSyncCallback(void)
{
}

/**
  * @brief  RCCEx Clock Recovery System Error interrupt callback.
  * @param  Combination of Error status. 
  *         This parameter can be a combination of the following values:
  *           @arg @ref RCC_CRS_SYNCERR
  *           @arg @ref RCC_CRS_SYNCMISS
  *           @arg @ref RCC_CRS_TRIMOVF
  * @retval none
  */
void HAL_RCCEx_CRS_ErrorCallback(uint32_t Error)
{
  if((Error & RCC_CRS_SYNCMISS) != RESET)
  {
    CRSSyncStatus = CRS_STATUS_SYNCMISS;
  }
  else if((Error & RCC_CRS_SYNCERR) != RESET)
  {
    CRSSyncStatus = CRS_STATUS_SYNCERR;
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* LED3 is slowly blinking (1 sec. period) */
  while(1)
  {
    BSP_LED_Toggle(LED3);
    HAL_Delay(1000);
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

