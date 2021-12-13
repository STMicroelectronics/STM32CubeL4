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
#include "rtc.h"
#include "GUIDEMO.h"
#include "WM.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t GUI_Initialized = 0;

extern TS_StateTypeDef TS_State;
extern uint8_t ts_calibration_done;
extern int     _Next ;

/* Private function prototypes -----------------------------------------------*/
static void BSP_Config(void);
static void IO_Init(void);
void SystemClock_Config(void);
void BSP_Pointer_Update(void);
void BSP_Background(void);
extern void MainTask(void);


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
  
  /* Initialize LCD and LEDs */
  BSP_Config();
  
  /* Initialize RTC and Backup */
  RTC_Init();
  
  /* Init the STemWin GUI Library */
  GUI_Init();
  GUI_Initialized = 1;
  
  /* Activate the use of memory device feature */
  WM_SetCreateFlags(WM_CF_MEMDEV);

  /* Do the calibration if needed */
  Touchscreen_Calibration();
  
  /* Start Demo */
  GUIDEMO_Main();
}

/**
  * @brief  Initializes the STM32L496G-Discovery's LCD and LEDs resources.
  * @param  None
  * @retval None
  */
static void BSP_Config(void)
{
  /* Initialize STM32L496G-Discovery's LEDs */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  
  /*## LCD Configuration ##################################################*/
  /* I/O initialization, required before LCD initialization */
  IO_Init();
  
  /* LCD initialization */
  BSP_LCD_Init();
  
  /* TouchScreen initialization */
   BSP_TS_Init(240, 240);
 
  /* Enable the CRC Module */
  __HAL_RCC_CRC_CLK_ENABLE();
}

/**
* @brief  BSP_Background.
* @param  None
* @retval None
*/ 
void BSP_Background(void)
{
  static uint32_t LED_ticks = 0;
  static uint32_t POINTER_ticks = 0;
  
  if ( LED_ticks++ > 100 )
  {
    LED_ticks = 0;
    
    /* Toggle LED1, LED2 each 100ms */
    BSP_LED_Toggle(LED1);
    BSP_LED_Toggle(LED2);
  }
  
  if ( POINTER_ticks++ > 50 )
  {
    POINTER_ticks = 0;
    
    /* Capture input event and updade cursor */
    if(GUI_Initialized == 1)
    {      
      BSP_Pointer_Update();    
    }
  }
}

/**
  * @brief  Provide the GUI with current state of the touch screen
  * @param  None
  * @retval None
  */
void BSP_Pointer_Update(void)
{
  static GUI_PID_STATE TS_State = {0, 0, 0, 0};
  __IO TS_StateTypeDef  ts;

  BSP_TS_GetState((TS_StateTypeDef *)&ts);
  
  ts.touchX[0] = TouchScreen_Get_Calibrated_X(ts.touchX[0]);
  ts.touchY[0] = TouchScreen_Get_Calibrated_Y(ts.touchY[0]);
  
  if((ts.touchX[0] >= LCD_GetXSize()) ||(ts.touchY[0] >= LCD_GetYSize()) ) 
  {
    ts.touchX[0] = 0;
    ts.touchY[0] = 0;
  }
  
  if((TS_State.Pressed != ts.touchDetected )||
    (TS_State.x != ts.touchX[0]) ||
      (TS_State.y != ts.touchY[0]))
{
  TS_State.Pressed = ts.touchDetected;
  if(ts.touchDetected) 
  {
    TS_State.x = ts.touchX[0];
    TS_State.y = ts.touchY[0];
    GUI_TOUCH_StoreStateEx(&TS_State);
    if (ts_calibration_done == 1)
    {
      /* Skip current demo and jump to next one */    
      _Next = 1;
    }
  }
  else
  {
    GUI_TOUCH_StoreStateEx(&TS_State);
    TS_State.x = 0;
    TS_State.y = 0;      
  }
}
}

/**
  * @brief IO initialization.
  * @note  GPIO PH.00 setting to activate STM32L496 Discovery I/Os
  *        and I/O initialization.  
  * @retval None
  */
static void IO_Init(void)
{   
  GPIO_InitTypeDef GPIO_InitStruct;
  

  __HAL_RCC_GPIOH_CLK_ENABLE();
 
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull =   GPIO_NOPULL;
  GPIO_InitStruct.Alternate = 0;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    
  HAL_GPIO_Init( GPIOH, &GPIO_InitStruct ); 
 
  HAL_GPIO_WritePin( GPIOH, GPIO_PIN_0, GPIO_PIN_RESET); 
  
  /* Initialize the IO functionalities */
  BSP_IO_Init();  
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

