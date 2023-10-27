/**
  ******************************************************************************
  * @file    Examples_LL/RTC/RTC_Tamper/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure Time Stamp feature through
  *          the STM32L4xx RTC LL API.
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

/** @addtogroup RTC_Tamper
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BACKUP_COUNT RTC_BKP_NUMBER

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO FlagStatus TamperStatus;

/* Backup registers table */
uint32_t aBKPDataReg[BACKUP_COUNT] =
{
  LL_RTC_BKP_DR0,  LL_RTC_BKP_DR1,  LL_RTC_BKP_DR2,
  LL_RTC_BKP_DR3,  LL_RTC_BKP_DR4,  LL_RTC_BKP_DR5,
  LL_RTC_BKP_DR6,  LL_RTC_BKP_DR7,  LL_RTC_BKP_DR8,
  LL_RTC_BKP_DR9,  LL_RTC_BKP_DR10, LL_RTC_BKP_DR11,
  LL_RTC_BKP_DR12, LL_RTC_BKP_DR13, LL_RTC_BKP_DR14,
  LL_RTC_BKP_DR15, LL_RTC_BKP_DR16, LL_RTC_BKP_DR17,
  LL_RTC_BKP_DR18, LL_RTC_BKP_DR19, LL_RTC_BKP_DR20,
  LL_RTC_BKP_DR21, LL_RTC_BKP_DR22, LL_RTC_BKP_DR23,
  LL_RTC_BKP_DR24, LL_RTC_BKP_DR25, LL_RTC_BKP_DR26,
  LL_RTC_BKP_DR27, LL_RTC_BKP_DR28, LL_RTC_BKP_DR29,
  LL_RTC_BKP_DR30, LL_RTC_BKP_DR31
};

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_RTC(void);
void     Configure_RTC_Tamper(void);
void     LED_Init(void);
void     LED_On(void);
void     LED_Blinking(uint32_t Period);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  register uint32_t index = 0;

  /* Configure the system clock to 80 MHz */
  SystemClock_Config();

  /* Initialize LED2 */
  LED_Init();

  /* Configure the RTC peripheral */
  Configure_RTC();

  /* Configure RTC Tamper */
  Configure_RTC_Tamper();

  /* Write Data on the Back Up registers  */
  for (index = 0; index < BACKUP_COUNT; index++)
  {
    LL_RTC_BAK_SetRegister(RTC, aBKPDataReg[index], 0xDF59 + (index * 0x5A));
  }

  /* Check Data is stored on the Back Up registers */
  for (index = 0; index < BACKUP_COUNT; index++)
  {
    if (LL_RTC_BAK_GetRegister(RTC, aBKPDataReg[index]) != (0xDF59 + (index * 0x5A)))
    {
      LED_Blinking(LED_BLINK_ERROR);
    }
  }

  /* Reset flag after writing of backup register in order to wait for new button press */
  TamperStatus = RESET;
  
  /* Wait for Tamper detection */
  while(TamperStatus != SET)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);  
    LL_mDelay(LED_BLINK_FAST);
  }
  
  /* LED2 On: Tamper button pressed */
  LED_On();
    
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Configure RTC.
  * @param  None
  * @retval None
  */
void Configure_RTC(void)
{
  /* Enables the PWR Clock and Enables access to the backup domain */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_EnableBkUpAccess();

  /* Enable RTC Clock */ 
  LL_RCC_EnableRTC();
}

/**
  * @brief  Configure the current time and date and activate Tamper.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_RTC_Tamper(void)
{
  /* Sampling frequency */
  /* Reset value is LL_RTC_TAMPER_SAMPLFREQDIV_32768 */
  //LL_RTC_TAMPER_SetSamplingFreq(RTC, LL_RTC_TAMPER_SAMPLFREQDIV_32768);
  /* Precharge duration */
  /* Reset value is LL_RTC_TAMPER_DURATION_1RTCCLK */
  //LL_RTC_TAMPER_SetPrecharge(RTC, LL_RTC_TAMPER_DURATION_1RTCCLK);
  /* Enable Pull up */
  /* Reset value is pull-up enabled */
  //LL_RTC_TAMPER_EnablePullUp(RTC);
  /* Disable timestamp on tamper detection */
  /* Reset value is timestamp on tamper disabled */
  //LL_RTC_TS_DisableOnTamper(RTC);
  /* Enable backup register after tamper event detection */
  /* Reset value is LL_RTC_TAMPER_NOERASE_TAMPER1 */
  //LL_RTC_TAMPER_EnableEraseBKP(RTC, LL_RTC_TAMPER_NOERASE_TAMPER1);
  /* Disable tamper mask flag */
  /* Reset value is LL_RTC_TAMPER_MASK_TAMPER1 */
  //LL_RTC_TAMPER_DisableMask(RTC, LL_RTC_TAMPER_MASK_TAMPER1);
  
  /* Set Tamper trigger to falling edge */
  /* Reset value is LL_RTC_TAMPER_FILTER_DISABLE */
  //LL_RTC_TAMPER_SetFilterCount(RTC, LL_RTC_TAMPER_FILTER_DISABLE);
  LL_RTC_TAMPER_EnableActiveLevel(RTC, LL_RTC_TAMPER_ACTIVELEVEL_TAMP1);
  
  /* Enable tamper detection */
  LL_RTC_TAMPER_Enable(RTC, LL_RTC_TAMPER_1);
  
  /* Enable IT TAMPER */
  LL_RTC_EnableIT_TAMP1(RTC);

  /* Configure the NVIC for RTC Tamper */
  NVIC_SetPriority(TAMP_STAMP_IRQn, 0x0F);
  NVIC_EnableIRQ(TAMP_STAMP_IRQn);
  
  /* RTC Tamper Interrupt Configuration: EXTI configuration */
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_19);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_19);

  /* Clear the Tamper interrupt pending bit */
  LL_RTC_ClearFlag_TAMP1(RTC);
}

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
  * @brief  Set LED2 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);  
    LL_mDelay(Period);
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
  * @brief  Tamper event callback function
  * @param  None
  * @retval None
  */
void Tamper_Callback(void)
{
  register uint32_t index = 0;
  
  /* Deactivate the tamper */
  LL_RTC_TAMPER_Disable(RTC, LL_RTC_TAMPER_1);
    
  /* Check Data is cleared on the Back Up registers */
  for (index = 0; index < BACKUP_COUNT; index++)
  {
    if (LL_RTC_BAK_GetRegister(RTC, aBKPDataReg[index]) != 0x00)
    {
      LED_Blinking(LED_BLINK_ERROR);
    }
  }
  TamperStatus = SET;
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

