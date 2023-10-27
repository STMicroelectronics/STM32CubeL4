/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   This file provides main program functions
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

/* Private typedef -----------------------------------------------------------*/
typedef  void (*pFunc)(void);

/* Private define ------------------------------------------------------------*/
#define STBL_DESCRITION                 "ST Demo Loader V1.0.0"
#define BOARD_NAME                      "STM32L4R9I-EVAL"

#define MB1314_ML_ADDRESS               0x08006000
#define MB1315_ML_ADDRESS               0x08043000
#define DEFAULT_ML_ADDRESS              MB1315_ML_ADDRESS

#ifdef USE_UART_TRACES
#define USARTx                           USART3
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_10
#define USARTx_TX_GPIO_PORT              GPIOB
#define USARTx_TX_AF                     GPIO_AF7_USART3
#define USARTx_RX_PIN                    GPIO_PIN_11
#define USARTx_RX_GPIO_PORT              GPIOB
#define USARTx_RX_AF                     GPIO_AF7_USART3

#define TS_RGB_I2C_ADDRESS               0x70
#define TS_DSI_I2C_ADDRESS               0x70
#endif /* USE_UART_TRACES */

/* Private macro -------------------------------------------------------------*/
#ifdef USE_UART_TRACES
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
#endif /* USE_UART_TRACES */

/* Private variables ---------------------------------------------------------*/
#ifdef USE_UART_TRACES
static UART_HandleTypeDef UART_Handle;
#endif /* USE_UART_TRACES */

/* Private function prototypes -----------------------------------------------*/
static void BSP_Config(void);
static void SystemClock_Config(void);
static void BSP_ConfigSerial( void );

/* Private functions ---------------------------------------------------------*/
#ifdef USE_UART_TRACES
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();

  /* Enable USART1 clock */
  USARTx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
}
#endif /* USE_UART_TRACES */

/**
  * @brief LTDC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param  None
  * @retval None
  */
static void MB1315_LTDC_MspInit( void )
{
  /* IO expander init */
  BSP_IO_Init();

  /* LCD_DISP & LCD_RST pins configuration (over MFX IO Expander) */
  BSP_IO_ConfigPin(LCD_RGB_DISP_PIN | LCD_RGB_RST_PIN, IO_MODE_OUTPUT);
  
  /* Screen initialization */
  BSP_IO_WritePin(LCD_RGB_RST_PIN, IO_PIN_RESET );
  HAL_Delay(2);
  BSP_IO_WritePin(LCD_RGB_RST_PIN, IO_PIN_SET );

  /* Assert display enable LCD_DISP pin via MFX expander */
  BSP_IO_WritePin(LCD_RGB_DISP_PIN, IO_PIN_SET);
}

/**
  * @brief LTDC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param  None
  * @retval None
  */
static void MB1315_LTDC_MspDeInit( void )
{
  BSP_IO_WritePin(LCD_RGB_DISP_PIN, IO_PIN_RESET);
  BSP_IO_WritePin(LCD_RGB_RST_PIN, IO_PIN_RESET );
}

static void MB1314_LTDC_MspInit( void )
{

}
static void MB1314_LTDC_MspDeInit( void )
{
  
}

/**
  * @brief  Jump to Sub demo application
  * @param  Address  : Address where the sub demonstration code is located
  * @retval None
  */
static void JumpToMenuLauncher(uint32_t MenuLauncherAddress)
{
  /* Store the address of the Sub Demo binary */
  WRITE_REG(RTC->BKP30R, (uint32_t) MenuLauncherAddress);
  NVIC_SystemReset();
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */ 
int main(void)
{
  static __IO uint32_t MenuLauncherAddress = 0;

  /* Enable RTC back-up registers access */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();

  /* LED used by Error_Handler() */
  BSP_LED_Init(LED_RED);

  /* Init Wakeup/Tamper push-button in GPIO Mode */
  BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_GPIO);
  if(BSP_PB_GetState(BUTTON_WAKEUP) == GPIO_PIN_SET)
  {
    HAL_Init();
    WRITE_REG(RTC->BKP30R, 0);
    /* Now you must release the button */
    while (BSP_PB_GetState(BUTTON_WAKEUP) == GPIO_PIN_SET)
    {
      BSP_LED_Toggle(LED_RED);
      HAL_Delay(100);
    }
  }
  BSP_PB_DeInit(BUTTON_WAKEUP);

  MenuLauncherAddress = READ_REG(RTC->BKP30R);
  if (MenuLauncherAddress == 0)
  {
    /* STM32F4xx HAL library initialization:
         - Configure the Flash prefetch, instruction and Data caches
         - Configure the Systick to generate an interrupt each 1 msec
         - Set NVIC Group Priority to 4
         - Global MSP (MCU Support Package) initialization
       */
    HAL_Init();
  }  
  else
  {
    /* Reinitialize the Stack pointer*/
    __set_MSP(*(__IO uint32_t*) MenuLauncherAddress);
    /* Jump to application address */  
    ((pFunc) (*(__IO uint32_t*) (MenuLauncherAddress + 4)))();
  }

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();

  BSP_Config();

  /* Check for TS id */
  /* Read ID and verify if the touch screen driver is ready */
  MB1315_LTDC_MspInit();
  {
    ft5336_ts_drv.Init(TS_RGB_I2C_ADDRESS);
    if(ft5336_ts_drv.ReadID(TS_RGB_I2C_ADDRESS) == FT5336_ID_VALUE)
    {
      MB1315_LTDC_MspDeInit();
      #ifdef USE_UART_TRACES
      printf("Jump to MB1315 MenuLauncher\n");
      #endif /* USE_UART_TRACES */

      BSP_LED_On(LED_BLUE);
      BSP_LED_Off(LED_RED);
      /*
       * Jump to MB1315 MenuLauncher 
       */
      JumpToMenuLauncher(MB1315_ML_ADDRESS);
    }
  }
  MB1315_LTDC_MspDeInit();

  MB1314_LTDC_MspInit();
  {
    /* Scan FT3X67 TouchScreen IC controller ID register by I2C Read */
    ft3x67_ts_drv.Init(TS_DSI_I2C_ADDRESS);
    if(ft3x67_ts_drv.ReadID(TS_DSI_I2C_ADDRESS) == FT3X67_ID_VALUE)
    {
      MB1314_LTDC_MspDeInit();
      #ifdef USE_UART_TRACES
      printf("Jump to MB1314 MenuLauncher\n");
      #endif /* USE_UART_TRACES */

      BSP_LED_On(LED_GREEN);
      BSP_LED_Off(LED_RED);
      /* 
       * Jump to MB1314 MenuLauncher
       */
      JumpToMenuLauncher(MB1314_ML_ADDRESS);
    }
  }
  MB1314_LTDC_MspDeInit();

  /* 
   * We should never get here as control is now taken by the scheduler
   */
  printf("Error Unknown LCD\n");
  BSP_LED_On(LED_ORANGE);
  while(1);
}

/**
  * @brief  Initializes the STM324x9I-EVAL's LCD and LEDs resources.
  * @param  None
  * @retval None
  */
static void BSP_Config(void)
{
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_ORANGE);
  
#ifdef USE_UART_TRACES
  BSP_ConfigSerial();
  
  printf( "\n\n%s: %s (Build %s at %s)\n", BOARD_NAME, STBL_DESCRITION, __DATE__, __TIME__ );
  printf( "CPU running at %dMHz, Peripherals at %dMHz/%dMHz\n", (HAL_RCC_GetHCLKFreq()/1000000U), (HAL_RCC_GetPCLK1Freq()/1000000U), (HAL_RCC_GetPCLK2Freq()/1000000U) );
#endif /* USE_UART_TRACES */
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 110000000
  *            HCLK(Hz)                       = 110000000
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
  //HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  __HAL_RCC_PWR_CLK_DISABLE();

  /* Enable MSI Oscillator and activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6; /*!< MSI = 8 MHz. Do not modify this value used as SAI Source, MMC  */
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* To avoid overshoot due to maximum frequency, select PLL as system clock source */
  /* with prescaler dividers 2 as first step */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Pprescaler dividers at 1 as second step */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
  /* Turn LED RED on */
  BSP_LED_On(LED_RED);

  __disable_irq();
  while(1);
}

void BSP_ErrorHandler(void)
{
  // FIXME : We just ignore the BSP errors because we got many I2C timeout errors
  // Error_Handler();

  BSP_LED_On(LED_RED);
}

#ifdef USE_UART_TRACES
static void BSP_ConfigSerial( void )
{
  UART_Handle.Instance            = USARTx;
  UART_Handle.Init.BaudRate       = 115200;
  UART_Handle.Init.WordLength     = UART_WORDLENGTH_8B;
  UART_Handle.Init.StopBits       = UART_STOPBITS_1;
  UART_Handle.Init.Parity         = UART_PARITY_NONE;
  UART_Handle.Init.Mode           = UART_MODE_TX_RX;
  UART_Handle.Init.HwFlowCtl      = UART_HWCONTROL_NONE;
  UART_Handle.Init.OverSampling   = UART_OVERSAMPLING_16;

  HAL_UART_Init( &UART_Handle );
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UART_Handle, (uint8_t *)&ch, 1, 0xFFFF); 

  return ch;
}
#endif /* USE_UART_TRACES */

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

