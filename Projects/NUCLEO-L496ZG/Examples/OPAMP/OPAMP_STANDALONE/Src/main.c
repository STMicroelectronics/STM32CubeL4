/**
  ******************************************************************************
  * @file    OPAMP/OPAMP_STANDALONE/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the DAC
  *          peripheral to generate several signals.
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

/** @addtogroup OPAMP_STANDALONE
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef    DacHandle;
OPAMP_HandleTypeDef  OpampHandle;
TIM_HandleTypeDef    htim;

/* set to 1 after User push-button interrupt  */
__IO uint32_t UserButtonStatus = 0;  

static DAC_ChannelConfTypeDef sConfig;

const uint16_t Sine12bit[32] = {511, 611, 707, 796, 847, 874, 984, 1014, 1023,
                                1014, 984, 937, 874, 796, 707, 611, 511, 411,
                                315, 227, 149, 149, 38, 9, 0, 9, 38, 86, 149, 227,
                                315, 411};

/* Private function prototypes -----------------------------------------------*/
void DAC_Config(void);
void TIM_Config(void);
void OPAMP_Config_Power(uint32_t powermode);
void SystemClock_Config(void);
void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t examplestate = 0; 
    
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

  /* Configure LED3 */
  BSP_LED_Init(LED3);
  /* Initialize the User push-button.
     It is used for changing the gain */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  /* Configure the TIM to control the DAC */
  TIM_Config();

  /* Configure the DAC to generator sine wave */
  DAC_Config();

  /* Configure the OPAMP1 in STANDALONE mode : gain = 2 */

  while (1)
  {
    /* Test if user an action occurred on the User push-button */
    if (UserButtonStatus == 1)
    {
      UserButtonStatus = 0;
      /* Change the mode */
      if (examplestate == 0)
      {
        /* normal mode */
        OPAMP_Config_Power(OPAMP_POWERMODE_NORMAL);
        examplestate= 1;
      }
      else /* normal mode */
      {
        /* Low power mode */
        OPAMP_Config_Power(OPAMP_POWERMODE_LOWPOWER);
        examplestate= 0;
      }
    }
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
void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);

  while (1)
  {
  }
}

void DAC_Config(void)
{
  /* Configure the DAC peripheral instance */
  DacHandle.Instance = DAC1;
    
  /*##-1- Initialize the DAC peripheral ######################################*/
  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-1- DAC channel Configuration ##########################################*/
  /* Select normal power mode */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  /* Select T2 as the external trigger */
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  /* Select DAC channel output buffer enabled */
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  /* Connect DAC output to on chip peripheral */
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_ENABLE;
  /* Select factory trimming */
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if(HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }

  if(HAL_DAC_Start_DMA(&DacHandle, DAC_CHANNEL_2, (uint32_t*)Sine12bit, 
                       sizeof (Sine12bit) / sizeof (uint32_t), 
                       DAC_ALIGN_12B_R) != HAL_OK)
  {
    /* Start DMA Error */
    Error_Handler();
  }
}

/**
  * @brief  TIM Configuration
  * @note   TIM configuration is based on APB1 frequency
  * @note   TIM Update event occurs each TIMxCLK/65535   
  * @param  None
  * @retval None
  */
void TIM_Config(void)
{
  TIM_MasterConfigTypeDef sMasterConfig;
  
  /*##-1- Configure the TIM peripheral #######################################*/
  /* Time base configuration */
  htim.Instance = TIM2;
  
  htim.Init.Period = 0xFFFF;          
  htim.Init.Prescaler = 0;       
  htim.Init.ClockDivision = 0;    
  htim.Init.CounterMode = TIM_COUNTERMODE_UP; 
  HAL_TIM_Base_Init(&htim);

  /* TIM2 TRGO selection */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

  /*##-2- Enable TIM peripheral counter ######################################*/
  HAL_TIM_Base_Start(&htim);
  }


/**             
  * @brief  OPAMP Configuration
  * @note   
  * @note   
  * @param  None
  * @retval None
  */
void OPAMP_Config_Power(uint32_t powermode)
  {
  /* Set OPAMP instance */
  OpampHandle.Instance = OPAMP2; 
    
  /*##-1- Deinit  OPAMP    ##################################################*/
  HAL_OPAMP_DeInit(&OpampHandle);

  /*##-1- Configure OPAMP    ##################################################*/
  /* Select power supply range */
  OpampHandle.Init.PowerSupplyRange = OPAMP_POWERSUPPLY_HIGH; 
  
  /* Select power mode */
  
  if (powermode == OPAMP_POWERMODE_NORMAL)
  {
    OpampHandle.Init.PowerMode = OPAMP_POWERMODE_NORMAL; 
  }
  else
  {
    OpampHandle.Init.PowerMode = OPAMP_POWERMODE_LOWPOWER; 
  }

  /* Select STANDALONE Mode */
  OpampHandle.Init.Mode = OPAMP_STANDALONE_MODE;  

  /* Select VPx as input for OPAMP */
  OpampHandle.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_DAC_CH;

  /* Select VM0 as input for OPAMP */
  OpampHandle.Init.InvertingInput = OPAMP_INVERTINGINPUT_IO0; 
   
  /* Select the factory trimming */
  OpampHandle.Init.UserTrimming = OPAMP_TRIMMING_FACTORY;

  /* Init */
  if(HAL_OK != HAL_OPAMP_Init(&OpampHandle))
  {
    Error_Handler();
  }

  /*##-2- Start OPAMP    #####################################################*/
  /* Enable OPAMP */
  if(HAL_OK != HAL_OPAMP_Start(&OpampHandle))
  {
    Error_Handler();
  }  
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t first_transition_timing = 0; 
  uint32_t current_transition_timing = 0;
  
  current_transition_timing = HAL_GetTick();
  
  /* 500 ms */
  if ((current_transition_timing - first_transition_timing) > 500)
  {
    /* Wait that user release the User push-button */
    while(BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_SET)
    {
    }
    
    /* A genuine transition is detected */
    UserButtonStatus = 1;

    /* current transition timing becomes first transition timing */   
    first_transition_timing = current_transition_timing;   
  }
  else
  /* Next transitions within 500 ms window time span are ignored */
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

