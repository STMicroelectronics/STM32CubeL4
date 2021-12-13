/**
  ******************************************************************************
  * @file    Examples_MIX/OPAMP/OPAMP_CALIBRATION/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use a operational amplifier
  *          peripheral with OPAMP calibration and operation.
  *          This example is based on the STM32L4xx OPAMP HAL & LL API
  *          (LL API is used for performance improvement).
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

/** @addtogroup STM32L4xx_HAL_MIX_Examples
  * @{
  */

/** @addtogroup OPAMP_CALIBRATION
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef    DacHandle;
OPAMP_HandleTypeDef  OpampHandle;
TIM_HandleTypeDef    htim;

__IO uint32_t UserButtonStatus = 0;  /* set to 1 after User push-button interrupt  */

uint32_t factorytrimmingvaluep = 0, factorytrimmingvaluen = 0;
uint32_t oldtrimmingvaluep = 0, oldtrimmingvaluen = 0; 
uint32_t newtrimmingvaluep = 0, newtrimmingvaluen = 0; 

uint32_t ledblinkingduration = 0; 

static DAC_ChannelConfTypeDef sConfig;

const uint16_t Sine12bit[32] = {510, 610, 700, 790, 840, 870, 980, 1010, 1020,
                                1010, 980, 930, 870, 790, 700, 610, 510, 410,
                                310, 220, 140, 140, 40, 0, 00, 10, 40, 90, 150, 230,
                                320, 41};

/* Private function prototypes -----------------------------------------------*/
static void DAC_Config(void);
static void TIM_Config(void);
static void OPAMP_Calibrate_Before_Run (void);
static void OPAMP_Config(void);
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
  
  /* Configure the system clock to 80 MHz */
  SystemClock_Config();
  
  /* Initialize LED */
  BSP_LED_Init(LED2);

  /* Configure User push-button in Interrupt mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Configure the TIM to control the DAC */
  TIM_Config();

  /* Configure the DAC to generate sine wave */
  DAC_Config();

  /*##-1- Configure OPAMP    #################################################*/
  /* Set OPAMP instance */
  OpampHandle.Instance = OPAMPx; 
  
  /* Configure the OPAMPx */
  OPAMP_Config();
  
  /*##-2- Calibrate OPAMP    #################################################*/
  
  /* Press user button to launch calibration */ 
  /*  printf("\n\n\r Press user button to launch calibration \n\r"); */
  
  while(UserButtonStatus != 1);

  HAL_Delay(100); 
  UserButtonStatus = 0;
  
  OPAMP_Calibrate_Before_Run();
 
  /*  printf("\n\r The LED blinks rapidly if ");
  printf("\n\r Calibrated trimming are different from Factory Trimming \n\r");
  
  printf("\n\r The LED blinks slowly if ");
  printf("\n\r Calibrated trimming same as Factory Trimming \n\r"); */
  
  /*##-3- Start OPAMP    #####################################################*/

  /* Press user button to launch OPAMP */ 
  /*  printf("\n\n\r Press user button to launch OPAMP, gain = 2\n\r"); */
  while(UserButtonStatus != 1)
  {
    BSP_LED_Toggle (LED2);
    HAL_Delay(ledblinkingduration); 
  }
  BSP_LED_Off(LED2);
  
  UserButtonStatus = 0;

  
  
  /* Customize process using LL interface to improve the performance         */
  /* (exhaustive feature management not handled).                            */
  
  /* ########## Starting from this point HAL API must not be used ########## */
  
  /* Enable OPAMP */
  LL_OPAMP_Enable(OPAMPx);
  
  /* Note: Equivalent command using HAL driver:                               */
  /* HAL_OPAMP_Start(&OpampHandle);                                           */
  
  /*##-3- Modify OPAMP setting while OPAMP ON ################################*/
  
  /* Press user button to change OPAMP setting on the fly (dummy change,      */
  /* Note: This is a dummy change, without application purpose in             */
  /*       this example: change non-inverting input source (no signal         */
  /*       connected to this input, therefore OPAMP output will appear as     */
  /*       floating).                                                         */
  /* printf("\n\n\r Press user button to modify OPAMP non-inverting input source \n\r"); */
  /* printf("       i.e. non-inverting input source is changed on the fly \n\r"); */
  
  while(UserButtonStatus != 1);
  HAL_Delay(500);
  UserButtonStatus = 0;
  
  /* Set OPAMP non-inverting input connection */ 
  LL_OPAMP_SetInputNonInverting(OPAMP2, LL_OPAMP_INPUT_NONINV_DAC1_CH1);
  
  /* Note: Equivalent command using HAL driver:                               */
  /* Change the input */
  /* OpampHandle.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_VP0; */
  /* Update OMAP config */
  /* HAL_OPAMP_Init(&OpampHandle); */

  
  /*##-4- Stop OPAMP #########################################################*/
  
  /* Press user button to stop OPAMP */ 
  /* printf("\n\n\r Press user button to stop OPAMP \n\r"); */
  
  while(UserButtonStatus != 1);
  HAL_Delay(500);
  UserButtonStatus = 0;

  /* Disable OPAMP */
  LL_OPAMP_Disable(OPAMPx);
  
  /* Note: Equivalent command using HAL driver:                               */
  /* HAL_OPAMP_Stop(&OpampHandle);                                            */
  
  /*##-5- End of tests       #################################################*/
  
  /* printf("\n\n\r End of example \n\r"); */
  while (1);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 1
  *            PLL_N                          = 20
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable HSE Oscillator and activate PLL with HSE as source   */
  /* (Default MSI Oscillator enabled at system reset remains ON) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
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
  /* Turn LED on */
  BSP_LED_On(LED2);

  while (1)
  {
  }
}

static void DAC_Config(void)
{
  /* Configure the DAC peripheral instance */
  DacHandle.Instance = DACx;

  /*##-1- Initialize the DAC peripheral ######################################*/
  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-1- DAC channel Configuration ###########################################*/
  /* Select normal power mode */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  /* Connect DAC output to on chip peripheral */
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_ENABLE;
  /* Select factory trimming */
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;

  if(HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNELa) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }

  /*##-2- Enable DAC Channel and associated DMA ##############################*/
  if(HAL_DAC_Start_DMA(&DacHandle, DACx_CHANNELa, (uint32_t*)Sine12bit, 
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
  * @brief  OPAMP Calibration before OPAMP runs
  * @note   
  * @note   
  * @param  None
  * @retval None
  */
void OPAMP_Calibrate_Before_Run(void)
  {

  /* Retrieve Factory Trimming */
   factorytrimmingvaluep = HAL_OPAMP_GetTrimOffset(&OpampHandle, OPAMP_FACTORYTRIMMING_P);
   factorytrimmingvaluen = HAL_OPAMP_GetTrimOffset(&OpampHandle, OPAMP_FACTORYTRIMMING_N);
    
   /* Run OPAMP calibration */
   HAL_OPAMP_SelfCalibrate(&OpampHandle);
   
   /* Store trimming value */
   oldtrimmingvaluep = OpampHandle.Init.TrimmingValueP;
   oldtrimmingvaluen = OpampHandle.Init.TrimmingValueN;

   /* Are just calibrated trimming same or different from Factory Trimming */
   if  ((oldtrimmingvaluep != factorytrimmingvaluep) || (oldtrimmingvaluen != factorytrimmingvaluen))
   {
     /* Calibrated trimming are different from Factory Trimming */
     /* printf("....... Calibrated trimming are different from Factory Trimming \n\r"); */
     /* LED blinks quickly */
     ledblinkingduration = 250;       
   }
   
   else
   {
     /* Calibrated trimming same as Factory Trimming */  
     /* printf("....... Calibrated trimming same as Factory Trimming \n\r"); */
    /* LED blinks slowly */
     ledblinkingduration = 1500;
   }
  
   /* With use temperature sensor   */
   /* Calibration */
  }

/**             
  * @brief  OPAMP Configuration
  * @note   
  * @note   
  * @param  None
  * @retval None
  */
void OPAMP_Config(void)
{
  /* Select OPAMP mode */
  OpampHandle.Init.Mode = OPAMP_FOLLOWER_MODE;
  
  /* Select power supply range */
  OpampHandle.Init.PowerSupplyRange = OPAMP_POWERSUPPLY_HIGH;
  
  /* Select power mode */
  OpampHandle.Init.PowerMode = OPAMP_POWERMODE_NORMAL;
  
  /* Select VPx as input for OPAMP */
  OpampHandle.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_IO0;
  
  /* Select the inverting input of the OPAMP: no inverting input connection since OPAMP is on follower mode */
  /* OpampHandle.Init.InvertingInput = OPAMP_INVERTINGINPUT_IO0; */
  
  /* Select the user trimming (just calibrated) */
  OpampHandle.Init.UserTrimming = OPAMP_TRIMMING_USER;
  
  /* Init */
  HAL_OPAMP_Init(&OpampHandle);
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

