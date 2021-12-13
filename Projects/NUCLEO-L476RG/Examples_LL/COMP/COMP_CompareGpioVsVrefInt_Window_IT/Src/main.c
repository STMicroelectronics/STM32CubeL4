/**
  ******************************************************************************
  * @file    Examples_LL/COMP/COMP_CompareGpioVsVrefInt_Window_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to use a pair of comparator peripherals
  *          to compare a voltage level applied on a GPIO pin
  *          versus 2 thresholds: the internal voltage reference (VrefInt)
  *          and a fraction the internal voltage reference (VrefInt/2),
  *          in interrupt mode.
  *          This example is based on the STM32L4xx COMP LL API;
  *          Peripheral initialization done using LL unitary services functions.
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

/** @addtogroup STM32L4xx_LL_Examples
  * @{
  */

/** @addtogroup COMP_CompareGpioVsVrefInt_Window_IT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Constant voltage generation for this example purpose,                      */
/* using peripherals: DAC.                                                    */
/* Goal is to provide to user an intermediate voltage level to check          */
/* comparator window thresholds.                                              */
#define CONSTANT_VOLTAGE_GENERATION

#if defined(CONSTANT_VOLTAGE_GENERATION)
/* Definitions of environment analog values */
  /* Value of analog reference voltage (Vref+), connected to analog voltage   */
  /* supply Vdda (unit: mV).                                                  */
  #define VDDA_APPLI                       ((uint32_t)3300)
  
  /* Value of comparator thresholds based on VrefInt */
  /* (VrefInt voltage is approximatively 1.22V, refer to device datasheet     */
  /* for min/typ/max values).                                                 */
  /* (unit: mV).                                                              */
  #define VREFINT                          ((uint32_t)1220)
  #define COMP_WINDOW_THRESHOLD_HIGH       (VREFINT)        /* COMP1 input plus is selected to VrefInt in this example */
  #define COMP_WINDOW_THRESHOLD_LOW        (VREFINT /2)     /* COMP2 input plus is selected to 1/2 VrefInt in this example */
  
/* Definitions of data related to this example */
  /* Full-scale digital value with a resolution of 12 bits (voltage range     */
  /* determined by analog voltage references Vref+ and Vref-,                 */
  /* refer to reference manual).                                              */
  #define DIGITAL_SCALE_12BITS             (__LL_DAC_DIGITAL_SCALE(LL_DAC_RESOLUTION_12B))

/* Definitions of constant voltage generation values */
  /* Waveform generation: parameters of constant voltage */
  /* Constant voltage amplitude                                               */
  /* (unit: mV)                                                               */
  #define CONSTANT_VOLTAGE_AMPLITUDE       ((COMP_WINDOW_THRESHOLD_HIGH + COMP_WINDOW_THRESHOLD_LOW) / 2)

#endif /* CONSTANT_VOLTAGE_GENERATION */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_COMP_master(void);
void     Configure_COMP_slave(void);
void     Activate_COMP_master_slave(void);
void     LED_Init(void);
void     LED_On(void);
void     LED_Off(void);
#if defined(CONSTANT_VOLTAGE_GENERATION)
void     Generate_constant_voltage(void);
#endif /* CONSTANT_VOLTAGE_GENERATION */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 80 MHz */
  SystemClock_Config();
  
  /* Initialize LED2 */
  LED_Init();
  
#if defined(CONSTANT_VOLTAGE_GENERATION)
  /* Generate constant voltage to provide to user an intermediate voltage     */
  /* level to check comparator window thresholds.                             */
  Generate_constant_voltage();
#endif /* CONSTANT_VOLTAGE_GENERATION */
  
  /* Configure pair of comparators for window mode */
  /* Configure comparator "master": comparator which input plus is used.      */
  Configure_COMP_master();
  
  /* Configure comparator "slave": comparator which input plus is not used    */
  /* (connected internally to the comparator master input plus).              */
  Configure_COMP_slave();
  
  /* Activate pair of comparators */
  Activate_COMP_master_slave();
  
  /* Lock comparator instance */
  //LL_COMP_Lock(COMP1);
  
  /* Infinite loop */
  while (1)
  {
    /* Note: LED state depending on COMP status is set into COMP IRQ handler, */
    /*        refer to function "ComparatorTrigger_Callback()".               */
    
  }
}

/**
  * @brief  Configure comparator "master" for window mode: comparator 
  *         which input plus is used (COMP instance: COMP1),
  *         GPIO and EXTI line used by comparator.
  * @note   This function configures the comparator but does not enable it,
  *         in order to optimize power consumption (comparator enabled only
  *         when needed).
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_COMP_master(void)
{
  __IO uint32_t wait_loop_index = 0;
  
  /*## Configuration of GPIO used by comparator ##############################*/
  
  /* Configure GPIO connected to the selected comparator input plus */
  /* Enable GPIO clock */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  
  /* Configure GPIO in analog mode to be used as COMP input */
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_5, LL_GPIO_MODE_ANALOG);
  
  /*## Configuration of NVIC #################################################*/
  /* Configure NVIC to enable COMP1 interruptions */
  NVIC_SetPriority(COMP_IRQn, 0);
  NVIC_EnableIRQ(COMP_IRQn);
  
  /*## Configuration of comparator ###########################################*/
  
  /* Enable COMP clock */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  
  /*## Configuration of comparator common instance ###########################*/
  
  /* Set comparator common instance window mode */
  /* Note: Window mode is common to 2 COMP instances, and therefore must      */
  /*       with a COMP common instance (COMP12_COMMON, ...)                   */
  LL_COMP_SetCommonWindowMode(__LL_COMP_COMMON_INSTANCE(COMP1), LL_COMP_WINDOWMODE_COMP1_INPUT_PLUS_COMMON);
  
  /*## Configuration of comparator instance ##################################*/
  
  /* Set comparator instance operating mode to adjust power and speed */
  LL_COMP_SetPowerMode(COMP1, LL_COMP_POWERMODE_ULTRALOWPOWER);
  
  /* Set comparator inputs */
  LL_COMP_ConfigInputs(COMP1, LL_COMP_INPUT_MINUS_VREFINT, LL_COMP_INPUT_PLUS_IO1);
  
  /* Note: Call of the functions below are commented because they are         */
  /*       useless in this example:                                           */
  /*       settings corresponding to default configuration from reset state.  */
  
  /* Set comparator instance hysteresis mode of the input minus */
  //LL_COMP_SetInputHysteresis(COMP1, LL_COMP_HYSTERESIS_NONE);
  
  /* Set comparator instance output polarity */
  //LL_COMP_SetOutputPolarity(COMP1, LL_COMP_OUTPUTPOL_NONINVERTED);
  
  /* Set comparator instance blanking source */
  //LL_COMP_SetOutputBlankingSource(COMP1, LL_COMP_BLANKINGSRC_NONE);
  
  /* Delay for COMP voltage scaler stabilization time.                        */
  /* Compute number of CPU cycles to wait for, from delay in us.              */
  /* Note: Variable divided by 2 to compensate partially                      */
  /*       CPU processing cycles (depends on compilation optimization).       */
  /* Note: If system core clock frequency is below 200kHz, wait time          */
  /*       is only a few CPU processing cycles.                               */
  wait_loop_index = ((LL_COMP_DELAY_VOLTAGE_SCALER_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }
  
  /*## Configuration of EXTI line used by comparator #########################*/
  /* Set EXTI line trigger edge */
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_21);
  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_21);
  
  /* Set EXTI line interruption */
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_21);
  
}

/**
  * @brief  Configure comparator "slave" for window mode: comparator 
  *         which input plus is used (connected internally to the
  *         comparator master input plus) (COMP instance: COMP1),
  *         GPIO and EXTI line used by comparator.
  * @note   This function configures the comparator but does not enable it,
  *         in order to optimize power consumption (comparator enabled only
  *         when needed).
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_COMP_slave(void)
{
  __IO uint32_t wait_loop_index = 0;
  
  /*## Configuration of GPIO used by comparator ##############################*/
  
  /* Configure GPIO connected to the selected comparator input plus */
  /* Note: Comparator instance slave input plus is not used (connected        */
  /*       internally to comparator instance master input plus).              */
  /*       Input minus s connected to 1/2 VrefInt, therefore these is no GPIO */
  /*       to configure for comparator instance slave.                        */
  
  /*## Configuration of NVIC #################################################*/
  /* Note: Comparator instance slave is using the same IRQn as comparator     */
  /*       instance master.                                                   */
  
  /*## Configuration of comparator ###########################################*/
  
  /* Enable COMP clock */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  
  /*## Configuration of comparator common instance ###########################*/
  /* Note: Setting of comparator common instance is already done during       */
  /*       configuration of the comparator instance master.                   */
  
  /*## Configuration of comparator instance ##################################*/
  
  /* Set comparator instance operating mode to adjust power and speed */
  LL_COMP_SetPowerMode(COMP2, LL_COMP_POWERMODE_ULTRALOWPOWER);
  
  /* Set comparator input minus */
  /* Note: Comparator instance slave input plus is not used (connected        */
  /*       internally to comparator instance master input plus).              */
  LL_COMP_SetInputMinus(COMP2, LL_COMP_INPUT_MINUS_1_2VREFINT);
  
  /* Note: Call of the functions below are commented because they are         */
  /*       useless in this example:                                           */
  /*       settings corresponding to default configuration from reset state.  */
  
  /* Set comparator instance hysteresis mode of the input minus */
  //LL_COMP_SetInputHysteresis(COMP2, LL_COMP_HYSTERESIS_NONE);
  
  /* Set comparator instance output polarity */
  //LL_COMP_SetOutputPolarity(COMP2, LL_COMP_OUTPUTPOL_NONINVERTED);
  
  /* Set comparator instance blanking source */
  //LL_COMP_SetOutputBlankingSource(COMP2, LL_COMP_BLANKINGSRC_NONE);
  
  /* Delay for COMP voltage scaler stabilization time.                        */
  /* Compute number of CPU cycles to wait for, from delay in us.              */
  /* Note: Variable divided by 2 to compensate partially                      */
  /*       CPU processing cycles (depends on compilation optimization).       */
  /* Note: If system core clock frequency is below 200kHz, wait time          */
  /*       is only a few CPU processing cycles.                               */
  wait_loop_index = ((LL_COMP_DELAY_VOLTAGE_SCALER_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }
  
  /*## Configuration of EXTI line used by comparator #########################*/
  /* Set EXTI line trigger edge */
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_22);
  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_22);
  
  /* Set EXTI line interruption */
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_22);
  
}

/**
  * @brief  Perform comparator activation procedure of the pair of comparators
  *         for window mode (COMP instances: COMP1, COMP2).
  * @note   Operations:
  *         - Enable comparator instance master
  *         - Enable comparator instance slave
  *         - Wait for comparator startup time
  *           (required to reach propagation delay specification)
  * @param  None
  * @retval None
  */
void Activate_COMP_master_slave(void)
{
  __IO uint32_t wait_loop_index = 0;
  
  /* Enable comparator instance master */
  LL_COMP_Enable(COMP1);
  
  /* Enable comparator instance slave */
  LL_COMP_Enable(COMP2);
  
  /* Delay for comparator startup time.                                       */
  /* Compute number of CPU cycles to wait for, from delay in us.              */
  /* Note: Variable divided by 2 to compensate partially                      */
  /*       CPU processing cycles (depends on compilation optimization).       */
  /* Note: If system core clock frequency is below 200kHz, wait time          */
  /*       is only a few CPU processing cycles.                               */
  wait_loop_index = ((LL_COMP_DELAY_STARTUP_US * (SystemCoreClock / (100000 * 2))) / 10);
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }
  
}

#if defined(CONSTANT_VOLTAGE_GENERATION)
/**
  * @brief  Generate constant voltage for this example purpose,
  *         to provide to user a voltage available on board connectors.
  *         Generation using peripherals:
  *         - GPIO (analog output): pin PA.04
  *         - DAC: DAC1 channel1
  * @param  None
  * @retval None
  */
void Generate_constant_voltage(void)
{
  __IO uint32_t wait_loop_index = 0;
  
  /*## Configuration of GPIO used by DAC channels ############################*/
  
  /* Enable GPIO Clock */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA); 
  
  /* Configure GPIO in analog mode to be used as DAC output */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);
  
  /*## Configuration of NVIC #################################################*/ 
  /* Note: In this example, DAC interruptions are not activated.              */
  /*       If needed to be activated, refer to DAC examples.                  */
  
  /*## Configuration of DAC ##################################################*/
  
  /* Enable DAC clock (core clock) */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DAC1);
  
  /* Set the mode for the selected DAC channel */
  // LL_DAC_SetMode(DAC1, LL_DAC_CHANNEL_1, LL_DAC_MODE_NORMAL_OPERATION);
  
  /* Select trigger source */
  LL_DAC_SetTriggerSource(DAC1, LL_DAC_CHANNEL_1, LL_DAC_TRIG_SOFTWARE);
  
  /* Set the output for the selected DAC channel */
  LL_DAC_ConfigOutput(DAC1, LL_DAC_CHANNEL_1, LL_DAC_OUTPUT_MODE_NORMAL, LL_DAC_OUTPUT_BUFFER_ENABLE, LL_DAC_OUTPUT_CONNECT_GPIO);
  
  /* Note: DAC channel output configuration can also be done using            */
  /*       LL unitary functions:                                              */
  // LL_DAC_SetOutputMode(DAC1, LL_DAC_CHANNEL_1, LL_DAC_OUTPUT_MODE_NORMAL);
  // LL_DAC_SetOutputBuffer(DAC1, LL_DAC_CHANNEL_1, LL_DAC_OUTPUT_BUFFER_ENABLE);
  // LL_DAC_SetOutputConnection(DAC1, LL_DAC_CHANNEL_1, LL_DAC_OUTPUT_CONNECT_GPIO);
  
  /* Set DAC mode sample-and-hold timings */
  // LL_DAC_SetSampleAndHoldSampleTime (DAC1, LL_DAC_CHANNEL_1, 0x3FF);
  // LL_DAC_SetSampleAndHoldHoldTime   (DAC1, LL_DAC_CHANNEL_1, 0x3FF);
  // LL_DAC_SetSampleAndHoldRefreshTime(DAC1, LL_DAC_CHANNEL_1, 0xFF);
  
  /* Disable DAC channel DMA request */
  // LL_DAC_DisableDMAReq(DAC1, LL_DAC_CHANNEL_1);
  
  /* Set the data to be loaded in the data holding register */
  //LL_DAC_ConvertData12RightAligned(DAC1, LL_DAC_CHANNEL_1, 0x000);
  
  /*## Activation of DAC #####################################################*/
  
  /* Enable DAC channel */
  LL_DAC_Enable(DAC1, LL_DAC_CHANNEL_1);
  
  /* Delay for DAC channel voltage settling time from DAC channel startup.    */
  /* Compute number of CPU cycles to wait for, from delay in us.              */
  /* Note: Variable divided by 2 to compensate partially                      */
  /*       CPU processing cycles (depends on compilation optimization).       */
  /* Note: If system core clock frequency is below 200kHz, wait time          */
  /*       is only a few CPU processing cycles.                               */
  wait_loop_index = ((LL_DAC_DELAY_STARTUP_VOLTAGE_SETTLING_US * (SystemCoreClock / (100000 * 2))) / 10);
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }
  
  /* Enable DAC channel trigger */
  LL_DAC_EnableTrigger(DAC1, LL_DAC_CHANNEL_1);
  
  /*## Set DAC output voltage ################################################*/
  
  /* Set the data to be loaded in the data holding register */
  LL_DAC_ConvertData12RightAligned(DAC1, LL_DAC_CHANNEL_1, CONSTANT_VOLTAGE_AMPLITUDE);
  
  /* Trig DAC conversion by software */
  LL_DAC_TrigSWConversion(DAC1, LL_DAC_CHANNEL_1);
  
}
#endif /* CONSTANT_VOLTAGE_GENERATION */

/**
  * @brief  Initialize LED2.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED2 Clock */
  LED2_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED2 */
  LL_GPIO_SetPinMode(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Turn-on LED2.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED2 on */
  LL_GPIO_SetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

/**
  * @brief  Turn-off LED2.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LED2 off */
  LL_GPIO_ResetOutputPin(LED2_GPIO_PORT, LED2_PIN);
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
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* MSI configuration and activation */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
  LL_RCC_MSI_Enable();
  while(LL_RCC_MSI_IsReady() != 1) 
  {
  };
  
  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 40, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1) 
  {
  };
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) 
  {
  };
  
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 80MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ(__LL_RCC_CALC_MSI_FREQ(LL_RCC_MSIRANGESEL_RUN, LL_RCC_MSIRANGE_6), 
                                  LL_RCC_PLLM_DIV_1, 40, LL_RCC_PLLR_DIV_2)*/
  LL_Init1msTick(80000000);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(80000000);
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/

/**
  * @brief  Comparator interruption callback
  *         This function is executed when the comparator threshold
  *         is triggered.
  * @retval None
  */
void ComparatorTrigger_Callback()
{
  __IO uint32_t COMP1OutputLevel = LL_COMP_ReadOutputLevel(COMP1);
  __IO uint32_t COMP2OutputLevel = LL_COMP_ReadOutputLevel(COMP2);
  
  /* Set LED state in function of comparator output level */
  /* Case of voltage above comparators thresholds window */
  if (   (COMP1OutputLevel == LL_COMP_OUTPUT_LEVEL_LOW)
      && (COMP2OutputLevel == LL_COMP_OUTPUT_LEVEL_LOW)
     )
  {
    LED_Off();
  }
  /* Case of voltage below comparators thresholds window */
  else if (   (COMP1OutputLevel == LL_COMP_OUTPUT_LEVEL_HIGH)
           && (COMP2OutputLevel == LL_COMP_OUTPUT_LEVEL_HIGH)
     )
  {
    LED_Off();
  }
  /* Case of voltage inside comparators thresholds window */
  else /* COMP1 output high, COMP2 output low */
  {
    LED_On();
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

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

