/**
  ******************************************************************************
  * @file    OPAMP/OPAMP_PGA/Src/main.c
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

/** @addtogroup OPAMP_PGA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef    DacHandle;
OPAMP_HandleTypeDef  OpampHandle;
TIM_HandleTypeDef    htim;
RCC_OscInitTypeDef RCC_OscInitStruct_LSI;

uint32_t            powerstatus = 0;

__IO uint32_t UserButtonStatus = 0;  /* set to 1 after User push-button interrupt  */

static DAC_ChannelConfTypeDef sConfig;

const uint16_t Sine12bit[32] = {511, 611, 707, 796, 847, 874, 984, 1014, 1023,
                                1014, 984, 937, 874, 796, 707, 611, 511, 411,
                                315, 227, 149, 149, 38, 9, 0, 9, 38, 86, 149, 227,
                                315, 411};

/* Private function prototypes -----------------------------------------------*/
static void DAC_Config(void);
static void TIM_Config(void);
void OPAMP_Config_Power(uint32_t powermode);
void DAC_Config_lowPower (void);
void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
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
  
  /* Configure the system clock to 80 MHz */
  SystemClock_Config();

  /* Configure LED2 */
  BSP_LED_Init(LED2);

  /* Initialize the User push-button.
     It is used for changing the gain */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Configure the TIM to control the DAC */
  TIM_Config();

  /* Configure the DAC to generator sine wave */
  DAC_Config();

  /* Configure the OPAMP1 in PGA mode */ 
  OPAMP_Config_Power(OPAMP_POWERMODE_NORMAL);

/*----------------------------------------------------------------------------*/
/* Step 0:                                                                    */
/*  	DAC:     normal power mode                                              */
/*  	OPAMP:   normal power mode - gain = 2                                   */
/*  	DMA:     circular mode - DMA half transfer IT handled by Cortex         */
/*  	Cortex:  run mode                                                       */
/*----------------------------------------------------------------------------*/
  while (UserButtonStatus == 0)
  {
  }
  UserButtonStatus = 0;  
  
  /* Gain change on the fly to 4 */
  OpampHandle.Init.PgaGain = OPAMP_PGA_GAIN_4;
  
  if(HAL_OK != HAL_OPAMP_Init(&OpampHandle))
  {
    Error_Handler();
  }

/*----------------------------------------------------------------------------*/ 
/* Step 1: (1 push on Key button)                                             */
/*  	DAC:     normal power mode                                              */
/*  	OPAMP:   normal power mode - gain = 4                                   */
/*  	DMA:     circular mode - DMA half transfer IT handled by Cortex         */
/*  	Cortex:  run mode                                                       */
/*----------------------------------------------------------------------------*/

  while (UserButtonStatus == 0)
  {
  }
  UserButtonStatus = 0;  
  
  /* Test will be done in low power mode without - no DMA IT running in */
  /* circular mode */
  powerstatus =  1;
  
  /* DAC is in low power mode */
  DAC_Config_lowPower();

/*----------------------------------------------------------------------------*/
/* Step 2: (2 pushes on Key button)                                           */
/*    DAC:     Low power mode                                                 */
/*  	OPAMP:   normal power mode - gain = 4                                   */
/*    DMA:     circular mode - DMA half transfer - no IT handled by Cortex    */
/*    Cortex:  run mode                                                       */
/*----------------------------------------------------------------------------*/
  
  while (UserButtonStatus == 0)
  {
  }
  UserButtonStatus = 0;  
  
  /* OPAMP set in low power mode */
  OPAMP_Config_Power(OPAMP_POWERMODE_LOWPOWER);  

  /* DAC    in low power mode       */
  /* OPAMP  in low power mode       */
 
  /* User push-button (External line 15 to 10) will be used to wakeup the system from SLEEP mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  
  /*Suspend Tick increment to prevent wakeup by Systick interrupt. 
  Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base)*/
  HAL_SuspendTick();
 
  /* Infinite loop */
  /* Push on key button wakes CORTEX up */
  /* than changes gain to 2 or 4 */
  /* then sleep again until next push */

#ifdef CHANGE_GAIN_IN_SLEEP_IN_MAIN  /* switch gain in low power mode using main */  

  while (1)
  {   
      if (OpampHandle.Init.PgaGain == OPAMP_PGA_GAIN_2)
      {
/*----------------------------------------------------------------------------*/
/* Step 3: (another push on Key button)                                       */
/*    DAC:     Low power mode                                                 */
/*  	OPAMP:   Low power mode - gain = 4                                      */
/*    DMA:     circular mode - DMA half transfer - no IT handled by Cortex    */
/*    Cortex:  sleep mode                                                     */ 
/*----------------------------------------------------------------------------*/
        /* Gain change on the fly to 4 */
        OpampHandle.Init.PgaGain = OPAMP_PGA_GAIN_4;
      }
      else
      {
/*----------------------------------------------------------------------------*/
/* Step 4: (another push on Key button)                                       */
/*    DAC:     Low power mode                                                 */
/*  	OPAMP:   Low power mode - gain = 2                                      */
/*    DMA:     circular mode - DMA half transfer - no IT handled by Cortex    */
/*    Cortex:  sleep mode                                                     */ 
/*----------------------------------------------------------------------------*/
        /* Gain change on the fly to 2 */
        OpampHandle.Init.PgaGain = OPAMP_PGA_GAIN_2;
      }
      
      if(HAL_OK != HAL_OPAMP_Init(&OpampHandle))
      {
        Error_Handler();
      }
      
      /* Enter SLEEP Mode, wake up is done once Key push button is pressed */  
      HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
  }  

#else /* switch gain in low power mode using IT */ 
 
/*----------------------------------------------------------------------------*/
/* Step3 <-> Step4 handled by EXTI15_10_IRQHandler                            */
/* Cortex goes to sleep upon exit of EXTI15_10_IRQHandler                     */
/*----------------------------------------------------------------------------*/
  HAL_PWR_EnableSleepOnExit();

  /* Enter SLEEP Mode, wake up is done once Key push button is pressed */  
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
  
  /* Code shall not reach this point as only IT are processed */  
  while (1)
  {
  } 
#endif

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
  /* Turn LED2 on */
  BSP_LED_On(LED2);

  while (1)
  {
  }
}

static void DAC_Config(void)
{
  /* Configure the DAC peripheral instance */
  DacHandle.Instance = DAC1;
  
  /*##-0- DeInitialize the DAC peripheral ####################################*/
  if (HAL_OK != HAL_DAC_DeInit(&DacHandle))
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-1- Initialize the DAC peripheral ######################################*/
  if (HAL_OK != HAL_DAC_Init(&DacHandle))
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
  if(HAL_OK != HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_2))
  {
    /* Channel configuration Error */
    Error_Handler();
  }

  /*##-2- Enable DAC Channel and associated DMA ##############################*/
  if(HAL_OK != HAL_DAC_Start_DMA(&DacHandle, DAC_CHANNEL_2, (uint32_t*)Sine12bit, 
                       sizeof (Sine12bit) / sizeof (uint32_t), 
                       DAC_ALIGN_12B_R))
  {
    /* Start DMA Error */
    Error_Handler();
  }
}

void  DAC_Config_lowPower (void)
{
  /*##########################################################################*/
  /*## LOW POWER MODE ########################################################*/
  
  RCC_OscInitStruct_LSI.OscillatorType = RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct_LSI.LSIState       = RCC_LSI_ON;
  RCC_OscInitStruct_LSI.PLL.PLLState   = RCC_PLL_NONE;
  
  if (HAL_OK != HAL_RCC_OscConfig(&RCC_OscInitStruct_LSI))
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Configure the DAC peripheral instance */
  DacHandle.Instance = DAC1;

  /*##-0- DeInitialize the DAC peripheral ####################################*/
  if (HAL_OK != HAL_DAC_DeInit(&DacHandle))
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-1- Initialize the DAC peripheral ######################################*/
  if (HAL_OK != HAL_DAC_Init(&DacHandle))
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- DAC channel Configuration ##########################################*/
  /* Select low power mode */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_ENABLE;
  /* Select T2 as the external trigger */
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  /* Select DAC channel output buffer enabled */
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  /* Connect DAC output to on chip peripheral */
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_ENABLE;
  /* Select factory trimming */
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;

  /* configure low power mode - sample and hold mode */
  sConfig.DAC_SampleAndHoldConfig.DAC_SampleTime = 20;
  sConfig.DAC_SampleAndHoldConfig.DAC_HoldTime = 10;
  sConfig.DAC_SampleAndHoldConfig.DAC_RefreshTime = 5;
  
  if (HAL_OK != HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_2))
  {
    /* Channel configuration Error */
    Error_Handler();
  }
  
  /*##-3- DAC channel Start ##################################################*/    
  if(HAL_OK != HAL_DAC_Start_DMA(&DacHandle, DAC_CHANNEL_2, (uint32_t*)Sine12bit, 
                       sizeof (Sine12bit) / sizeof (uint32_t), 
                       DAC_ALIGN_12B_R))
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
  if(HAL_OK != HAL_TIM_Base_Init(&htim))
  {
    Error_Handler();
  }
  /* TIM2 TRGO selection */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if(HAL_OK != HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig))
  {
    Error_Handler();
  }
  
  /*##-2- Enable TIM peripheral counter ######################################*/
  if(HAL_OK != HAL_TIM_Base_Start(&htim))
  {
    Error_Handler();
  }
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

  /*##-0- DeInitialize the OPAMP peripheral ##################################*/
  if (HAL_OK != HAL_OPAMP_DeInit(&OpampHandle))
  {
    Error_Handler();
  }  
  
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

  /* Select PGAMode */
  OpampHandle.Init.Mode = OPAMP_PGA_MODE;  

  /* Select VPx as input for OPAMP */
  OpampHandle.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_DAC_CH;

  /* Select no VMx in PGA mode */
  OpampHandle.Init.InvertingInput = OPAMP_INVERTINGINPUT_CONNECT_NO;
   
  /* Configure the gain */
  OpampHandle.Init.PgaGain = OPAMP_PGA_GAIN_2;

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
  if (GPIO_Pin == USER_BUTTON_PIN)
  {
    UserButtonStatus = 1;
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

