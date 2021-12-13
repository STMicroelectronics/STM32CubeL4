/**
  ******************************************************************************
  * @file    FatFs/FatFs_eMMC_Standalone/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FATFS_MKFS_ALLOWED 1
#define N_SIZES            7
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS MMCFatFs;  /* File system object for eMMC logical drive */
FIL MyFile;     /* File object */
char MMCPath[4]; /* MMC card logical drive path */
TIM_HandleTypeDef htim = {0};
uint32_t SystemClock;
uint32_t data_sizes[N_SIZES] = {0x00100,  /* 256 bytes   */
                                0x00400,  /* 1 K-byte    */
                                0x01000,  /* 4 K-bytes   */
                                0x10000,  /* 64 K-bytes  */
                                0x20000,  /* 128 K-bytes */
                                0x30000,  /* 192 K-bytes */
                                0x40000}; /* 256 K-bytes */

typedef enum {
  APPLICATION_IDLE = 0,
  APPLICATION_RUNNING,
}FS_FileOperationsTypeDef;

FS_FileOperationsTypeDef Appli_state = APPLICATION_IDLE;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void FS_FileOperations(void);
static void Error_Handler(void);
static void Timer_Config(void);
#if (defined (__GNUC__) && !defined(__ARMCC_VERSION))
extern void initialise_monitor_handles(void);
#endif

uint8_t workBuffer[_MAX_SS];


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /* STM32L4xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user
             can eventually implement his proper time base source (a general purpose
             timer for example or other time source), keeping in mind that Time base
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
             handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the System clock to have a frequency of 120 MHz */
  SystemClock_Config();
#if (defined (__GNUC__) && !defined(__ARMCC_VERSION))
  initialise_monitor_handles();
  printf("Semihosting Test...\n\r");
#endif  


  /* Configure LED1 and LED2 */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_BLUE);

  SystemClock = HAL_RCC_GetSysClockFreq() / 1000000; /* Number of cycles / ns */

  Timer_Config();

  /* 1- Link the eMMC disk I/O driver */
  if(FATFS_LinkDriver(&MMC_Driver, MMCPath) == 0)
  {
    /*##-2- Init the eMMC Card #################################################*/

#if FATFS_MKFS_ALLOWED
      FRESULT res;

      res = f_mkfs(MMCPath, FM_ANY, 0, workBuffer, sizeof(workBuffer));

      if (res != FR_OK)
      {
        Error_Handler();
      }
#endif
      Appli_state = APPLICATION_RUNNING;
  }
  else
  {
    Error_Handler();
    while(1) {};
  }


  /* Infinite loop */
  while (1)
  {
    /* Mass Storage Application State Machine */
    switch(Appli_state)
    {
    case APPLICATION_RUNNING:
      BSP_LED_Off(LED_BLUE);
      FS_FileOperations();
      Appli_state = APPLICATION_IDLE;
      break;

    case APPLICATION_IDLE:
      break;

    default:
      break;
    }

  }
}

static void FS_FileOperations(void)
{
  FRESULT res;                                          /* FatFs function common result code */
  uint32_t byteswritten, bytesread;                     /* File write/read counts */
  uint8_t wtext[] = "stm32l4p5g-discovery: this is STM32 working with FatFs"; /* File write buffer */
  uint8_t rtext[100];                                   /* File read buffer */
  uint32_t Access_time_us;
  uint32_t * copy_start;
  uint32_t counter;

  /* Register the file system object to the FatFs module */
  if(f_mount(&MMCFatFs, (TCHAR const*)MMCPath, 0) == FR_OK)
  {
    /* Create and Open a new text file object with write access */
    if(f_open(&MyFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
    {
      /* Write data to the text file */
      res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);

      if((byteswritten > 0) && (res == FR_OK))
      {
        /* Close the open text file */
        f_close(&MyFile);

        /* Open the text file object with read access */
        if(f_open(&MyFile, "STM32.TXT", FA_READ) == FR_OK)
        {
          /* Read data from the text file */
          res = f_read(&MyFile, rtext, sizeof(rtext), (void *)&bytesread);

          if((bytesread > 0) && (res == FR_OK))
          {
            /* Close the open text file */
            f_close(&MyFile);

            /* Compare read data with the expected data */
            if((bytesread != byteswritten))
            {
              Error_Handler();
            }
          }
        }
      }
    }


    copy_start = (uint32_t *)0x20002000;
    /* Create and Open test files object with write access to measure eMMC read/write accesses */
    for (counter = 0; counter< N_SIZES; counter++)
    {
      if(f_open(&MyFile, "TEST_ACCESS_TIME.TXT", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
      {

        /* Start timer counter */
        HAL_TIM_Base_Start(&htim);
        /* Write data to the text file */
         res = f_write(&MyFile, copy_start, data_sizes[counter], (void *)&byteswritten);
        /* Stop timer counter */
        HAL_TIM_Base_Stop(&htim);
        Access_time_us = __HAL_TIM_GET_COUNTER(&htim);
        Access_time_us /= SystemClock;
        __HAL_TIM_SET_COUNTER(&htim, 0x0);
        printf("%ld\t%s\t%ld\t%s\n", Access_time_us,"us to write",byteswritten,"bytes");

        if((byteswritten > 0) && (res == FR_OK))
        {
          /* Close the open text file */
          f_close(&MyFile);

          /* Open the text file object with read access */
          if(f_open(&MyFile, "TEST_ACCESS_TIME.TXT", FA_READ) == FR_OK)
          {
            /* Start timer counter */
            HAL_TIM_Base_Start(&htim);
            /* Read data from the text file */
            res = f_read(&MyFile, copy_start, data_sizes[counter], (void *)&bytesread);
            /* Stop timer counter */
            HAL_TIM_Base_Stop(&htim);
            Access_time_us = __HAL_TIM_GET_COUNTER(&htim);
            Access_time_us /= SystemClock;
            __HAL_TIM_SET_COUNTER(&htim, 0x0);
            printf("%ld\t%s\t%ld\t%s\n\n", Access_time_us,"us to read",byteswritten,"bytes");

            if((bytesread > 0) && (res == FR_OK))
            {
              /* Close the open text file */
              f_close(&MyFile);

              /* Compare read data with the expected data */
              if((bytesread != byteswritten))
              {
                Error_Handler();
              }
            }
          }
        }
      }
      else
      {
        Error_Handler();
      }
    }
    /* Success of the demo: no error occurrence */
    BSP_LED_On(LED_GREEN);
    return;
  }
  /* Error */
  Error_Handler();
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
  RCC_OscInitStruct.OscillatorType = (RCC_OSCILLATORTYPE_MSI | RCC_OSCILLATORTYPE_HSI48);
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 55;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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

static void Timer_Config(void)
{
    htim.Instance               = TIM2;
    htim.Init.Prescaler         = 0x0000;
    htim.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim.Init.Period            = 0xFFFFFFFF;
    htim.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim.Init.RepetitionCounter = 0x0;

    /* Auto-reload register preload is disabled */
    htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    /* Configure timer time base */
    HAL_TIM_Base_Init(&htim);
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



static void Error_Handler(void)
{
  BSP_LED_Off(LED_GREEN);
  BSP_LED_On(LED_BLUE);
}


/**
  * @}
  */

/**
  * @}
  */

