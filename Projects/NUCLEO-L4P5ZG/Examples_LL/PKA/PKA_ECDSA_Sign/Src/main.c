/**
  ******************************************************************************
  * @file    Examples_LL/PKA/PKA_ECDSA_Sign/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use PKA peripheral to generate an
  *          ECDSA signature using the STM32L4xx PKA LL API.
  *          Peripheral initialization done using LL unitary services functions.
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

/** @addtogroup STM32L4xx_LL_Examples
  * @{
  */

/** @addtogroup PKA_ECDSA_Sign
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t endOfProcess = 0;
uint8_t RBuffer[32] = {0};
uint8_t SBuffer[32] = {0};
/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_PKA(void);
void     LED_Init(void);
void     LED_On(void);
void     LED_Blinking(uint32_t Period);
static uint32_t Buffercmp(const uint8_t* pBuffer1,const uint8_t* pBuffer2, uint32_t BufferLength);
__IO uint32_t *PKA_Memcpy_u8_to_u32(__IO uint32_t dst[], const uint8_t src[], uint32_t n);
uint8_t *PKA_Memcpy_u32_to_u8(uint8_t dst[], __IO const uint32_t src[], uint32_t n);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t result = 0;

  /* Configure the system clock to 120 MHz */
  SystemClock_Config();

  /* Initialize LED1 */
  LED_Init();

  /* Configure PKA */
  Configure_PKA();

  /* Enable PKA */
  LL_PKA_Enable(PKA);

  /* Set mode to ECDSA signature generation in interrupt mode */
  LL_PKA_SetMode(PKA, LL_PKA_MODE_ECDSA_SIGNATURE);
  LL_PKA_EnableIT_ADDRERR(PKA);
  LL_PKA_EnableIT_RAMERR(PKA);
  LL_PKA_EnableIT_PROCEND(PKA);

  /* Loads the input buffers to PKA RAM */
  PKA->RAM[PKA_ECDSA_SIGN_IN_ORDER_NB_BITS] = prime256v1_Order_len * 8;
  PKA->RAM[PKA_ECDSA_SIGN_IN_MOD_NB_BITS] = prime256v1_Prime_len * 8;
  PKA->RAM[PKA_ECDSA_SIGN_IN_A_COEFF_SIGN] =  prime256v1_A_sign;

  /* Move the input parameters coefficient |a| to PKA RAM */
  PKA_Memcpy_u8_to_u32(&PKA->RAM[PKA_ECDSA_SIGN_IN_A_COEFF], prime256v1_absA, prime256v1_Prime_len);
  PKA->RAM[PKA_ECDSA_SIGN_IN_A_COEFF + prime256v1_Prime_len / 4] = 0;

  /* Move the input parameters modulus value p to PKA RAM */
  PKA_Memcpy_u8_to_u32(&PKA->RAM[PKA_ECDSA_SIGN_IN_MOD_GF], prime256v1_Prime, prime256v1_Prime_len);
  PKA->RAM[PKA_ECDSA_SIGN_IN_MOD_GF + prime256v1_Prime_len / 4] = 0;

  /* Move the input parameters integer k to PKA RAM */
  PKA_Memcpy_u8_to_u32(&PKA->RAM[PKA_ECDSA_SIGN_IN_K], SigGen_k, prime256v1_Prime_len);
  PKA->RAM[PKA_ECDSA_SIGN_IN_K + prime256v1_Prime_len / 4] = 0;

  /* Move the input parameters base point G coordinate x to PKA RAM */
  PKA_Memcpy_u8_to_u32(&PKA->RAM[PKA_ECDSA_SIGN_IN_INITIAL_POINT_X], prime256v1_GeneratorX, prime256v1_Prime_len);
  PKA->RAM[PKA_ECDSA_SIGN_IN_INITIAL_POINT_X + prime256v1_Prime_len / 4] = 0;

  /* Move the input parameters base point G coordinate y to PKA RAM */
  PKA_Memcpy_u8_to_u32(&PKA->RAM[PKA_ECDSA_SIGN_IN_INITIAL_POINT_Y], prime256v1_GeneratorY, prime256v1_Prime_len);
  PKA->RAM[PKA_ECDSA_SIGN_IN_INITIAL_POINT_Y + prime256v1_Prime_len / 4] = 0;

  /* Move the input parameters hash of message z to PKA RAM */
  PKA_Memcpy_u8_to_u32(&PKA->RAM[PKA_ECDSA_SIGN_IN_HASH_E], SigGen_Hash_Msg, prime256v1_Prime_len);
  PKA->RAM[PKA_ECDSA_SIGN_IN_HASH_E + prime256v1_Prime_len / 4] = 0;

  /* Move the input parameters private key d to PKA RAM */
  PKA_Memcpy_u8_to_u32(&PKA->RAM[PKA_ECDSA_SIGN_IN_PRIVATE_KEY_D], SigGen_d, prime256v1_Prime_len);
  PKA->RAM[PKA_ECDSA_SIGN_IN_PRIVATE_KEY_D + prime256v1_Prime_len / 4] = 0;

  /* Move the input parameters prime order n to PKA RAM */
  PKA_Memcpy_u8_to_u32(&PKA->RAM[PKA_ECDSA_SIGN_IN_ORDER_N], prime256v1_Order, prime256v1_Prime_len);
  PKA->RAM[PKA_ECDSA_SIGN_IN_ORDER_N + prime256v1_Prime_len / 4] = 0;

  /* Launch the computation in interrupt mode */
  LL_PKA_Start(PKA);

  /* Wait for the interrupt callback */
  while(endOfProcess != 1);

  /* Retrieve the result and output buffer */
  result = PKA->RAM[PKA_ECDSA_SIGN_OUT_ERROR];
  PKA_Memcpy_u32_to_u8(RBuffer, &PKA->RAM[PKA_ECDSA_SIGN_OUT_SIGNATURE_R], prime256v1_Prime_len / 4);
  PKA_Memcpy_u32_to_u8(SBuffer, &PKA->RAM[PKA_ECDSA_SIGN_OUT_SIGNATURE_S], prime256v1_Prime_len / 4);

  /* Compare to expected results */
  if (result != 0)
  {
    LED_Blinking(LED_BLINK_ERROR);
  }

  result = Buffercmp(RBuffer, SigGen_R, SigGen_R_len);
  if (result != 0)
  {
    LED_Blinking(LED_BLINK_ERROR);
  }

  result = Buffercmp(SBuffer, SigGen_S, SigGen_S_len);
  if (result != 0)
  {
    LED_Blinking(LED_BLINK_ERROR);
  }

  LED_On();

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function configures PKA Instance.
  * @note   This function is used to :
  *         - Enable peripheral clock for PKA.
  * @param  None
  * @retval None
  */
void Configure_PKA(void)
{
  /* Enable peripheral clock for RNG */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_PKA);

  /* Configure NVIC for PKA interrupts */
  /*   Set priority for PKA_IRQn */
  /*   Enable PKA_IRQn */
  NVIC_SetPriority(PKA_IRQn, 0);
  NVIC_EnableIRQ(PKA_IRQn);
}

void PKA_ERROR_callback(void)
{
  LED_Blinking(LED_BLINK_ERROR);
}

void PKA_PROCEND_callback(void)
{
  endOfProcess = 1;
}
/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint32_t Buffercmp(const uint8_t* pBuffer1,const uint8_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  Copy uint8_t array to uint32_t array to fit PKA number representation.
  * @param  dst Pointer to destination
  * @param  src Pointer to source
  * @param  n Number of u32 to be handled
  * @retval dst
  */
__IO uint32_t *PKA_Memcpy_u8_to_u32(__IO uint32_t dst[], const uint8_t src[], uint32_t n)
{
  const uint32_t *ptrSrc = (const uint32_t *) src;

  if (dst != 0)
  {
    for (uint32_t index = 0; index < n / 4; index++)
    {
      dst[index] = __REV(ptrSrc[n / 4 - index - 1]);
    }
  }
  return dst;
}

/**
  * @brief  Copy uint32_t array to uint8_t array to fit PKA number representation.
  * @param  dst Pointer to destination
  * @param  src Pointer to source
  * @param  n Number of u8 to be handled (must be multiple of 4)
  * @retval dst
  */
uint8_t *PKA_Memcpy_u32_to_u8(uint8_t dst[], __IO const uint32_t src[], uint32_t n)
{
  uint32_t *ptrDst = (uint32_t *) dst;
  if (dst != 0)
  {
    for (uint32_t index = 0; index < n; index++)
    {
      ptrDst[n - index - 1] = __REV(src[index]);
    }
  }
  return dst;
}
/**
  * @brief  Initialize LED1.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED1 Clock */
  LED1_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED1 */
  LL_GPIO_SetPinMode(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Turn-on LED1.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED1 on */
  LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

/**
  * @brief  Set LED1 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle LED1 in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
    LL_mDelay(Period);
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
  *            PLL_R                          = 2
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Enable voltage range 1 boost mode for frequency above 80 Mhz */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_EnableRange1BoostMode();
  LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);

  /* MSI already enabled at reset */

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 60, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  /* Intermediate AHB prescaler 2 when target frequency clock is higher than 80 MHz */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Insure 1µs transition state at intermediate medium speed clock based on DWT*/
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  DWT->CYCCNT = 0;
  while(DWT->CYCCNT < 100);

  /* AHB prescaler 1 */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 120MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ(MSI_VALUE,
                                  LL_RCC_PLLM_DIV_1, 60, LL_RCC_PLLR_DIV_2)*/
  LL_Init1msTick(120000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(120000000);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
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

