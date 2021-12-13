/**
  ******************************************************************************
  * @file    DMA/DMAMUX_SYNC/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to use the DMA with the DMAMUX to
  *          synchronize a transfer with LPTIM1 output period using the STM32L4xx HAL API.
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

/** @addtogroup DMAMUX_SYNC
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t TxSyncMessage[] = "\n\r10\n\r09\n\r08\n\r07\n\r06\n\r05\n\r04\n\r03\n\r02\n\r01\n\r00";
/* Size of Transmission buffer */
#define TX_SYNC_MESSAGE_SIZE               (COUNTOF(TxSyncMessage) - 1)

uint8_t BriefMessage[] = "This example shows how to use the DMA with the DMAMUX to synchronize a transfer with LPTIM1 output signal.\n\rThe LPUART1 is used in DMA synchronized mode to send a countdown from 10 to 00 with a period of 2sec \n\r\n\rStart countdown :\n\r";
#define BRIEF_MESSAGE_SIZE               (COUNTOF(BriefMessage) - 1)

uint8_t TxEndMessage[] = "\n\r\n\rExample Finished\n\r";
#define TX_END_MESSAGE_SIZE               (COUNTOF(TxEndMessage) - 1)

UART_HandleTypeDef UartHandle;
LPTIM_HandleTypeDef  LptimHandle;
DMA_HandleTypeDef DMA_Handle;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
void LPTIM_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  HAL_DMA_MuxSyncConfigTypeDef dmamux_syncParams;


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

  /*##-1- Configure LED1 and LED3 ##*/
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /*##-2- Configure the DMA ##################################################*/

  /* Enable DMA clock */
  DMAx_CLK_ENABLE();
  DMAMUX1_CLK_ENABLE();

  /* Configure the DMA handler for Transmission process */
  DMA_Handle.Instance                 = LPUART_TX_DMA_INSTANCE;
  DMA_Handle.Init.Request             = LPUART_TX_DMA_REQUEST;
  DMA_Handle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  DMA_Handle.Init.PeriphInc           = DMA_PINC_DISABLE;
  DMA_Handle.Init.MemInc              = DMA_MINC_ENABLE;
  DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  DMA_Handle.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  DMA_Handle.Init.Mode                = DMA_NORMAL;
  DMA_Handle.Init.Priority            = DMA_PRIORITY_LOW;

  /* Initialize the DMA with for Transmission process */
  HAL_DMA_Init(&DMA_Handle);

  /* Configure the DMAMUX with the Synchronization parameters */
  dmamux_syncParams.EventEnable   = ENABLE;                           /* Enable DMAMUX event generation each time RequestNumber are passed from DMAMUX to the DMA */
  dmamux_syncParams.SyncPolarity  = HAL_DMAMUX_SYNC_RISING;           /* Synchronization edge is Rising  */
  dmamux_syncParams.RequestNumber = 4;                                /* 4 requests are autorized after each edge of the sync signal */
  dmamux_syncParams.SyncSignalID  = HAL_DMAMUX1_SYNC_LPTIM1_OUT;    /* Sync signal is LPTIM1_OUT */
  dmamux_syncParams.SyncEnable    = ENABLE;                           /* Synchronization is enabled */

  HAL_DMAEx_ConfigMuxSync(&DMA_Handle, &dmamux_syncParams);

  /*##-3- Configure and start the LPTIM1 used for DMA transfer Synchronization#*/
  /* LPTIM1 is configured with 2sec period */
  LPTIM_Config();

  /*##-4- Configure the UART ##################################################*/
  /* USART configuration */
  UartHandle.Instance = LPUART;

  /* Configure the serial params */
  UartHandle.Init.BaudRate   = 38400;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  UartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;

  if (HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-5-Send Brief Message with the UART in Polling mode ######################*/
  /* Start transmission data through "BriefMessage" buffer */
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)BriefMessage, BRIEF_MESSAGE_SIZE, HAL_MAX_DELAY )!= HAL_OK)
  {
    /* Transfer error in transmission process */
    Error_Handler();
  }

  /*##-6- Start the synchronized transmission process #####################################*/
  /* Start transmission of the countdown data through "TxSyncMessage" buffer */
  if(HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)TxSyncMessage, TX_SYNC_MESSAGE_SIZE)!= HAL_OK)
  {
    /* Transfer error in transmission process */
    Error_Handler();
  }

  /*##-7- Wait for the end of the synchronized transfer ###################################*/
  while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
  {
  }

  /*##-8-Send example ending  Message with the UART in Polling mode #####################################*/
  /* Start transmission data through "TxEndMessage" buffer */
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)TxEndMessage, TX_END_MESSAGE_SIZE, HAL_MAX_DELAY )!= HAL_OK)
  {
    /* Transfer error in transmission process */
    Error_Handler();
  }

  BSP_LED_On(LED1);

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Configure and start the LPTIM1 with 2sec period and 50% duty cycle.
  * @param  None
  * @retval None
  */
void LPTIM_Config(void)
{

  uint32_t periodValue;
  uint32_t pulseValue ;

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /* Enable the LSE clock source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* LPTIM1 clock source set to LSE*/
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
  PeriphClkInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSE;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  periodValue = (2 * LSE_VALUE)/4;    /* Calculate the Timer  Autoreload value for 2sec period */
  pulseValue  = periodValue/2;        /* Set the Timer  pulse value for 50% duty cycle         */

  /* TIM1 Peripheral clock enable */
  __HAL_RCC_LPTIM1_CLK_ENABLE();

  LptimHandle.Instance                           = LPTIM1;

  LptimHandle.Init.CounterSource                 = LPTIM_COUNTERSOURCE_INTERNAL;
  LptimHandle.Init.UpdateMode                    = LPTIM_UPDATE_ENDOFPERIOD;
  LptimHandle.Init.OutputPolarity                = LPTIM_OUTPUTPOLARITY_LOW;
  LptimHandle.Init.Clock.Source                  = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  LptimHandle.Init.Clock.Prescaler               = LPTIM_PRESCALER_DIV4;
  LptimHandle.Init.UltraLowPowerClock.Polarity   = LPTIM_CLOCKPOLARITY_RISING;
  LptimHandle.Init.UltraLowPowerClock.SampleTime = LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION;
  LptimHandle.Init.Trigger.Source                = LPTIM_TRIGSOURCE_SOFTWARE;
  LptimHandle.Init.Trigger.ActiveEdge            = LPTIM_ACTIVEEDGE_RISING;
  LptimHandle.Init.Trigger.SampleTime            = LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION;

  if(HAL_LPTIM_Init(&LptimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Start the timer */
  if (HAL_LPTIM_PWM_Start(&LptimHandle, periodValue, pulseValue) != HAL_OK)
  {
    Error_Handler();
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

  while(1)
  {
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
    Error_Handler();
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
    Error_Handler();
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

