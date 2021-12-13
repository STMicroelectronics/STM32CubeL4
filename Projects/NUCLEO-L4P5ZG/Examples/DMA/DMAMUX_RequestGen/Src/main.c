/**
  ******************************************************************************
  * @file    DMA/DMAMUX_RequestGen/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to use the DMA with the DMAMUX to
  *          request generator using the STM32L4xx HAL API.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
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

/** @defgroup DMAMUX_RequestGen
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO   uint32_t DMA_TransferErrorFlag = 0;

uint32_t SRC_Buffer_LED1_Toggle[2]  =
                                      {  LED1_PIN,   /*Value for LED1 ON  */
                                         0           /*Value for LED1 OFF */
                                      };

DMA_HandleTypeDef DMA_Handle;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void HAL_TransferError(DMA_HandleTypeDef *hdma);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  HAL_DMA_MuxRequestGeneratorConfigTypeDef dmamux_ReqGenParams;


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

  /* -1- Initialize LEDs mounted on STM32L4P5ZG-Nucleo (144 pins) board */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /*##-2- Configure the DMA ##################################################*/
  /* Enable DMA1 clock */
  DMAx_CLK_ENABLE();
  DMAMUX1_CLK_ENABLE();

  /* Configure the DMA handler for Transmission process     */
  /* DMA mode is set to circular for an infinite DMA transfer */
  DMA_Handle.Instance                 = DMA1_Channel1;

  DMA_Handle.Init.Request             = DMA_REQUEST_GENERATOR0;
  DMA_Handle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  DMA_Handle.Init.PeriphInc           = DMA_PINC_DISABLE;
  DMA_Handle.Init.MemInc              = DMA_MINC_ENABLE;
  DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  DMA_Handle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  DMA_Handle.Init.Mode                = DMA_CIRCULAR;
  DMA_Handle.Init.Priority            = DMA_PRIORITY_LOW;

  /* Initialize the DMA with for Transmission process */
  HAL_DMA_Init(&DMA_Handle);

  /* Register Error Callback */
  HAL_DMA_RegisterCallback(&DMA_Handle, HAL_DMA_XFER_ERROR_CB_ID, &HAL_TransferError);

  /* NVIC configuration for DMA transfer complete interrupt*/
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

  /*##-3- Configure and enable the DMAMUX Request generator  ####################*/
  dmamux_ReqGenParams.SignalID  = HAL_DMAMUX1_REQ_GEN_EXTI13;   /* External request signal is EXTI13 signal */
  dmamux_ReqGenParams.Polarity  = HAL_DMAMUX_REQ_GEN_RISING;    /* External request signal edge is Rising  */
  dmamux_ReqGenParams.RequestNumber = 1;                        /* 1 requests on each edge of the external request signal  */

  HAL_DMAEx_ConfigMuxRequestGenerator(&DMA_Handle, &dmamux_ReqGenParams);

  /* NVIC configuration for DMAMUX request generator overrun errors*/
  HAL_NVIC_SetPriority(DMAMUX1_OVR_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMAMUX1_OVR_IRQn);

  HAL_DMAEx_EnableMuxRequestGenerator (&DMA_Handle);

  /*##-4- Configure and enable the User push-button in EXTI mode used as DMA external request signal #####*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /*##-5- Start the DMA transfer ################################################*/
  /* DMA source buffer is SRC_BUFFER_LED1_TOGGLE containing values to be written
  to LED1 GPIO ODR register in order to turn LED1 On/Off each time comes a request from the DMAMUX request generator */
  HAL_DMA_Start_IT(&DMA_Handle, (uint32_t)SRC_Buffer_LED1_Toggle, (uint32_t)&LED1_GPIO_PORT->ODR, 2);

  /* Infinite loop */
  while (1)
  {
    if(DMA_TransferErrorFlag != 0)
    {
      Error_Handler();
    }
  }
}

/**
  * @brief  This function is executed in case of DMA error occurrence.
  * @param  None
  * @retval None
  */
static void HAL_TransferError(DMA_HandleTypeDef *hdma)
{
  DMA_TransferErrorFlag = 1;
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

