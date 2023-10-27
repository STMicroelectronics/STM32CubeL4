/**
  ******************************************************************************
  * @file    LTDC/LTDC_Display/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L4xx LTDC HAL API to
  *          display an image
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#include "mire.h"
#include "Lena.h"

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#if defined(DIMMING)
#define PERIOD_VALUE                   (uint32_t)(99)     /* Period Value  */
#define PULSE_VALUE                    (uint32_t)(95)     /* 5% cycle */
#define PULSE_DECREASE_START           (uint32_t)(35)     /* cycle start level when starting to dim the screen */
#define PULSE_DECREASE_DELAY           (uint32_t)(40)     /* gradually decrease cycle level every 40 milliseconds */
#endif /* DIMMING */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef RtcHandle;
LTDC_HandleTypeDef LtdcHandle;

#if defined(USE_CLUT)
/* b24..b31: table index, filled up by HAL_LTDC_ConfigCLUT() API */
/* b16..b23: RED[7:0]    */
/* b8..b15 : GREEN[7:0]  */
/* b0..b7  : BLUE[7:0]   */
/* RGB222, so only 64 indices needed,
   Build CLUT so that RGB components of four consecutive table entries are same
   RGB of CLUT[0] = RGB of CLUT[1] = RGB of CLUT[2] = RGB of CLUT[3]
*/
const uint32_t CLUT[256] = {
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
0x00000080, 0x00000080, 0x00000080, 0x00000080, 0x000000C0, 0x000000C0, 0x000000C0, 0x000000C0,
0x00004000, 0x00004000, 0x00004000, 0x00004000, 0x00004040, 0x00004040, 0x00004040, 0x00004040,
0x00004080, 0x00004080, 0x00004080, 0x00004080, 0x000040C0, 0x000040C0, 0x000040C0, 0x000040C0,
0x00008000, 0x00008000, 0x00008000, 0x00008000, 0x00008040, 0x00008040, 0x00008040, 0x00008040,
0x00008080, 0x00008080, 0x00008080, 0x00008080, 0x000080C0, 0x000080C0, 0x000080C0, 0x000080C0,
0x0000C000, 0x0000C000, 0x0000C000, 0x0000C000, 0x0000C040, 0x0000C040, 0x0000C040, 0x0000C040,
0x0000C080, 0x0000C080, 0x0000C080, 0x0000C080, 0x0000C0C0, 0x0000C0C0, 0x0000C0C0, 0x0000C0C0,
0x00400000, 0x00400000, 0x00400000, 0x00400000, 0x00400040, 0x00400040, 0x00400040, 0x00400040,
0x00400080, 0x00400080, 0x00400080, 0x00400080, 0x004000C0, 0x004000C0, 0x004000C0, 0x004000C0,
0x00404000, 0x00404000, 0x00404000, 0x00404000, 0x00404040, 0x00404040, 0x00404040, 0x00404040,
0x00404080, 0x00404080, 0x00404080, 0x00404080, 0x004040C0, 0x004040C0, 0x004040C0, 0x004040C0,
0x00408000, 0x00408000, 0x00408000, 0x00408000, 0x00408040, 0x00408040, 0x00408040, 0x00408040,
0x00408080, 0x00408080, 0x00408080, 0x00408080, 0x004080C0, 0x004080C0, 0x004080C0, 0x004080C0,
0x0040C000, 0x0040C000, 0x0040C000, 0x0040C000, 0x0040C040, 0x0040C040, 0x0040C040, 0x0040C040,
0x0040C080, 0x0040C080, 0x0040C080, 0x0040C080, 0x0040C0C0, 0x0040C0C0, 0x0040C0C0, 0x0040C0C0,
0x00800000, 0x00800000, 0x00800000, 0x00800000, 0x00800040, 0x00800040, 0x00800040, 0x00800040,
0x00800080, 0x00800080, 0x00800080, 0x00800080, 0x008000C0, 0x008000C0, 0x008000C0, 0x008000C0,
0x00804000, 0x00804000, 0x00804000, 0x00804000, 0x00804040, 0x00804040, 0x00804040, 0x00804040,
0x00804080, 0x00804080, 0x00804080, 0x00804080, 0x008040C0, 0x008040C0, 0x008040C0, 0x008040C0,
0x00808000, 0x00808000, 0x00808000, 0x00808000, 0x00808040, 0x00808040, 0x00808040, 0x00808040,
0x00808080, 0x00808080, 0x00808080, 0x00808080, 0x008080C0, 0x008080C0, 0x008080C0, 0x008080C0,
0x0080C000, 0x0080C000, 0x0080C000, 0x0080C000, 0x0080C040, 0x0080C040, 0x0080C040, 0x0080C040,
0x0080C080, 0x0080C080, 0x0080C080, 0x0080C080, 0x0080C0C0, 0x0080C0C0, 0x0080C0C0, 0x0080C0C0,
0x00C00000, 0x00C00000, 0x00C00000, 0x00C00000, 0x00C00040, 0x00C00040, 0x00C00040, 0x00C00040,
0x00C00080, 0x00C00080, 0x00C00080, 0x00C00080, 0x00C000C0, 0x00C000C0, 0x00C000C0, 0x00C000C0,
0x00C04000, 0x00C04000, 0x00C04000, 0x00C04000, 0x00C04040, 0x00C04040, 0x00C04040, 0x00C04040,
0x00C04080, 0x00C04080, 0x00C04080, 0x00C04080, 0x00C040C0, 0x00C040C0, 0x00C040C0, 0x00C040C0,
0x00C08000, 0x00C08000, 0x00C08000, 0x00C08000, 0x00C08040, 0x00C08040, 0x00C08040, 0x00C08040,
0x00C08080, 0x00C08080, 0x00C08080, 0x00C08080, 0x00C080C0, 0x00C080C0, 0x00C080C0, 0x00C080C0,
0x00C0C000, 0x00C0C000, 0x00C0C000, 0x00C0C000, 0x00C0C040, 0x00C0C040, 0x00C0C040, 0x00C0C040,
0x00C0C080, 0x00C0C080, 0x00C0C080, 0x00C0C080, 0x00C0C0C0, 0x00C0C0C0, 0x00C0C0C0, 0x00C0C0C0};

uint8_t compressed_mire[57600]; /* pixels written in RGB222 format, left-justified as follows R7,R6,G7,G6,B7,B6,0,0 */
uint8_t compressed_lena[57600]; /* pixels written in RGB222 format, left-justified as follows R7,R6,G7,G6,B7,B6,0,0 */
#endif

#if defined(DIMMING)
LPTIM_HandleTypeDef         LCD_LpTimHandle;
#endif /* DIMMING */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void LTDC_Config(void);
static void RTC_Config(void);

void DISP_Config(void);
static void DISP_Enable(void);
void DISP_Disable(void);

void PCLK_Enable(void);
void PCLK_Disable(void);

void Display_Refresh(uint32_t address);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
#if defined(USE_CLUT)
  uint32_t j;
#endif
#if defined(DIMMING)
  uint32_t i;
  uint32_t dimming_init = 0;
#endif

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

  /* Configure the system clock to 40 MHz */
  SystemClock_Config();

#if defined(USE_CLUT)

  /* CLUT construction for RGB222 */
  /* index = R7 R6 G7 G6 B7 B6  0 0 (index is 8-bit long) */
  /* Create CLUT[0], CLUT[4] and finish by CLUT[252]
     In between, build intermediate values */
  /*
  for (i=0; i< 64; i++)
  {
    CLUT[i*4]  = ((i*4) & 0xC0) << 16;  // Write R7 R6
    CLUT[i*4] |= ((i*4) & 0x30) << 10;  // Write G7 G6
    CLUT[i*4] |= ((i*4) & 0x0C) << 4;   // Write B7 B6

    CLUT[i*4+1] = CLUT[i*4] ;
    CLUT[i*4+2] = CLUT[i*4] ;
    CLUT[i*4+3] = CLUT[i*4] ;
  }
  */

  /* Compressed mire and lena images construction */
  /* Move from RGB888 to RGB222 */
  for (j=0; j < (240*240); j++)
  {

    /* Image is originally compressed over 24 bits as
      R7..R0 G7..G0 B7..B0 stored as a sequence of three bytes
      The two MSB of each color component are retrieved to build the CLUT index
     index = R7 R6 G7 G6 B7 B6  0 0 (index is 8-bit long) */
    compressed_mire[j] = (mire_data[3*j+2] & 0xC0) ;         /* Red R7 R6   */
    compressed_mire[j] |= ((mire_data[3*j+1] & 0xC0) >> 2);  /* Green G7 G6 */
    compressed_mire[j] |= ((mire_data[3*j+0] & 0xC0) >> 4);  /* Blue B7 B6  */


    compressed_lena[j] = (Lena_data[3*j+2] & 0xC0) ;
    compressed_lena[j] |= ((Lena_data[3*j+1] & 0xC0) >> 2);
    compressed_lena[j] |= ((Lena_data[3*j+0] & 0xC0) >> 4);
  }
#endif

  /* Disp pin config */
  DISP_Config();
  /* Configure the RTC */
  RTC_Config();

  DISP_Enable();

  /* Configure the LTDC */
  LTDC_Config();




  /* Infinite loop */
  while (1)
  {
#if defined(USE_CLUT)
    Display_Refresh((uint32_t)&compressed_mire);

#if defined(DIMMING)
  /* PWM En basic test */
   if (dimming_init == 0)
  {
    dimming_init = 1;
    i = PULSE_DECREASE_START;
    /* Always redo the full initialization as there is no apriori knowledge
      of IO or timer settings at this point (may have been modified by application) */

    __HAL_RCC_LSI_ENABLE();
    /* Select LSI as LPTIM2 clock source */
    __HAL_RCC_LPTIM2_CONFIG(RCC_LPTIM2CLKSOURCE_LSI);


    LCD_LpTimHandle.Instance = LPTIM2;
    __HAL_LPTIM_RESET_HANDLE_STATE(&LCD_LpTimHandle); /* to force MSP call */

    LCD_LpTimHandle.Init.Clock.Source    = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
    LCD_LpTimHandle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
    LCD_LpTimHandle.Init.CounterSource   = LPTIM_COUNTERSOURCE_INTERNAL;
    LCD_LpTimHandle.Init.Trigger.Source  = LPTIM_TRIGSOURCE_SOFTWARE;
    LCD_LpTimHandle.Init.OutputPolarity  = LPTIM_OUTPUTPOLARITY_HIGH;
    LCD_LpTimHandle.Init.UpdateMode      = LPTIM_UPDATE_IMMEDIATE;
    LCD_LpTimHandle.Init.Input1Source    = LPTIM_INPUT1SOURCE_COMP1;

    /* Initialize LPTIM peripheral according to the passed parameters */
    if (HAL_LPTIM_Init(&LCD_LpTimHandle) != HAL_OK)
    {
      /* Initialization Error */
      while(1);
    }

    if (HAL_LPTIM_PWM_Start(&LCD_LpTimHandle, PERIOD_VALUE, i) != HAL_OK)
    {
      while(1);
    }
  }

    for (i=PULSE_DECREASE_START; i<PULSE_VALUE;i++)
    {
      /* Optional delay to slow down the dimming transition. */
      HAL_Delay(PULSE_DECREASE_DELAY);
      if (HAL_LPTIM_PWM_Start(&LCD_LpTimHandle, PERIOD_VALUE, i) != HAL_OK)
      {
       while(1);
      }
    }
#endif /* DIMMING */

    HAL_Delay(3000);
    Display_Refresh((uint32_t)&compressed_lena);
#if defined(DIMMING)
    for (i=PULSE_VALUE ; i> PULSE_DECREASE_START;i--)
    {
      /* Optional delay to slow down the dimming transition. */
    HAL_Delay(PULSE_DECREASE_DELAY);
    if (HAL_LPTIM_PWM_Start(&LCD_LpTimHandle, PERIOD_VALUE, i) != HAL_OK)
    {
     while(1);
    }
    }
#endif /* DIMMING */
    HAL_Delay(3000);
#else
    Display_Refresh((uint32_t)&mire_data);
    HAL_Delay(3000);
    Display_Refresh((uint32_t)&Lena_data);
    HAL_Delay(3000);
#endif
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 40000000
  *            HCLK(Hz)                       = 40000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 30
  *            PLL_Q                          = 6
  *            PLL_R                          = 6
  *            PLL_P                          = 7
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable MSI Oscillator and activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_7;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 30;
  RCC_OscInitStruct.PLL.PLLR = 6;
  RCC_OscInitStruct.PLL.PLLQ = 6;
  RCC_OscInitStruct.PLL.PLLP = 7;
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
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  /*##-1- Configure the RTC peripheral #######################################*/
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follows:
      - Hour Format    = Format 24
      - Asynch Prediv  = Value according to source clock
      - Synch Prediv   = Value according to source clock
      - OutPut         = Output Disable
      - OutPutRemap    = Enable to get CALIB on PB2
      - OutPutPolarity = High Polarity
      - OutPutType     = Open Drain */
  /* Enable the Calibration Output */

  RtcHandle.Instance = RTC;
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV; /* 0x7F */
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV; /* (32 kHz / 128) - 1 = 0xF9 */
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE; // As CALIB has to be output
  RtcHandle.Init.OutPutRemap = RTC_OUTPUT_REMAP_POS1 ; // Remap set to PB2 (but need to use PC13 in final application)
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_PUSHPULL;

  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Enable the calibration output */
  /* 1Hz = Backlight OFF */
//  HAL_RTCEx_SetCalibrationOutPut(&RtcHandle, RTC_CALIBOUTPUT_1HZ);
  HAL_RTCEx_SetCalibrationOutPut(&RtcHandle, RTC_CALIBOUTPUT_512HZ);

}

/**
  * @brief  Configure the LTDC peripheral for JDI display
  * @param  None
  * @retval None
  */
void LTDC_Config()
{
  RCC_PeriphCLKInitTypeDef  PeriphClkInit;

  /* LCD clock configuration */
  /* PLLSAI2_VCO Input = MSI_VALUE/PLLSAI2_M = 8 /2 = 4 Mhz */
  /* PLLSAI2_VCO Output = PLLSAI2_VCO Input * PLLSAI2N =  4 * 40 = 160 Mhz */
  /* PLLLCDCLK = PLLSAI2_VCO Output/PLLSAIR = 160/6 = 26.6 Mhz */
  /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_4 = 26.6/16 =1.67Mhz */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInit.PLLSAI2.PLLSAI2Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI2.PLLSAI2M = 2;
  PeriphClkInit.PLLSAI2.PLLSAI2N = 40;
  PeriphClkInit.PLLSAI2.PLLSAI2R = 6;
  PeriphClkInit.PLLSAI2.PLLSAI2ClockOut = RCC_PLLSAI2_LTDCCLK;
  PeriphClkInit.LtdcClockSelection = RCC_LTDCCLKSOURCE_PLLSAI2_DIV16;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    while(1); // PLF to change
  }

  /* Timing Configuration */
  LtdcHandle.Init.HorizontalSync = 1;                // HSYNC width = 2 cycles
  LtdcHandle.Init.VerticalSync = 0;                  // VSYNC width = 1 cycle
  LtdcHandle.Init.AccumulatedHBP = 1+ 2;             // HBP = 2 cycles
  LtdcHandle.Init.AccumulatedVBP = 0+ 1;             // VBP = 1 cycle
  LtdcHandle.Init.AccumulatedActiveW = 1+2 + 240;    // HA  = 240  cycles
  LtdcHandle.Init.AccumulatedActiveH = 0+1 + 240;    // VA  = 240  cycles
  LtdcHandle.Init.TotalWidth = 1+2+240 + 2;          // HFP = 2 cycles
  LtdcHandle.Init.TotalHeigh = 0+1+240 + 2 +1;       // VFP = 2 cycles + 1 Extra cycle for synchronous stop
  /* Initialize the LCD pixel width and pixel height */
  LtdcHandle.LayerCfg->ImageWidth = 240;
  LtdcHandle.LayerCfg->ImageHeight = 240;
  /* Background value */
  LtdcHandle.Init.Backcolor.Blue = 0;
  LtdcHandle.Init.Backcolor.Green = 0x30; // 0XC0 : Blue /
  LtdcHandle.Init.Backcolor.Red = 0;
  /* Polarity */
  LtdcHandle.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  LtdcHandle.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  LtdcHandle.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  LtdcHandle.Init.PCPolarity = LTDC_PCPOLARITY_IIPC;
  LtdcHandle.Instance = LTDC;
  HAL_LTDC_Init(&LtdcHandle);
}

void Display_Refresh(uint32_t address)
{
  /* Configure the LTDC layer */
  LTDC_LayerCfgTypeDef  layer_cfg;

  layer_cfg.WindowX0 = 0;
  layer_cfg.WindowX1 = 240;
  layer_cfg.WindowY0 = 0;
  layer_cfg.WindowY1 = 240;
#if defined(USE_CLUT)
  layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_L8;
#else
  layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
#endif
  layer_cfg.FBStartAdress = address;//(uint32_t)&SLe_square;
  layer_cfg.Alpha = 255;
  layer_cfg.Alpha0 = 0;
  layer_cfg.Backcolor.Blue = 0;
  layer_cfg.Backcolor.Green = 0;
  layer_cfg.Backcolor.Red = 0;
  layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  layer_cfg.ImageWidth = 240;
  layer_cfg.ImageHeight = 240;
  HAL_LTDC_ConfigLayer(&LtdcHandle, &layer_cfg, 0);

#if defined(USE_CLUT)
  /* Download CLUT */
  HAL_LTDC_ConfigCLUT(&LtdcHandle, (uint32_t *)CLUT, 256, 0);
  /* Enable CLUT */
  HAL_LTDC_EnableCLUT(&LtdcHandle, 0);
#endif

  /* Enable interrupt */
   HAL_LTDC_ProgramLineEvent(&LtdcHandle, 244);
  __HAL_LTDC_ENABLE_IT(&LtdcHandle, LTDC_IT_LI);

  /* Enable the LTDC */
  __HAL_LTDC_ENABLE(&LtdcHandle);

  /* Enable the Pads */
    GPIO_InitTypeDef GPIO_InitStruct;

   /* the pixel clock */
  /* PA4: PCLK */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF11_LTDC;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* PE2..3 R6..7 */
  /* PE5..6 G6..7 */
  /* PE7..8 B6..7 */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5  | GPIO_PIN_6 | GPIO_PIN_7  | GPIO_PIN_8  ;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF11_LTDC;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);


  /* Disable HSYNC and VSYNC IOs */
  /* PC2: HSYNC */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_2 ;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF11_LTDC;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  /* PE1: VSYNC */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_1 ;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF11_LTDC;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}


void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef *hltdc)
{
  GPIO_InitTypeDef GPIO_InitStruct;

    /* PA4: PCLK */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF11_LTDC;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* PE2..3 R6..7 */
  /* PE5..6 G6..7 */
  /* PE7..8 B6..7 */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5  | GPIO_PIN_6 | GPIO_PIN_7  | GPIO_PIN_8  ;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF11_LTDC;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);


    /* Disable HSYNC and VSYNC IOs */
  /* PC2: HSYNC */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_2 ;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF11_LTDC;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  /* PE1: VSYNC */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_1 ;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF11_LTDC;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);


  /* Disable the LTDC */
  __HAL_LTDC_DISABLE(hltdc);
}





/**
  * @brief  Configure the LTDC peripheral for JDI display
  * @param  None
  * @retval None
  */
void DISP_Config()
{
  GPIO_InitTypeDef GPIO_InitStruct;


      __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWREx_EnableVddIO2();

    /* GPIO Configuration */
  /* PG8: DISP */
  __HAL_RCC_GPIOG_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);


      /* GPIO Configuration */
  /* PC0: LCD_DE */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* GPIO Configuration */
  /* PD13: Backlight */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

static void DISP_Enable()
{
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_SET);

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
}

void DISP_Disable()
{
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_RESET);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED2 on */
  BSP_LED_Init(LED2);
  BSP_LED_On(LED2);

  while (1)
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
void assert_failed(char *file, uint32_t line)
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

