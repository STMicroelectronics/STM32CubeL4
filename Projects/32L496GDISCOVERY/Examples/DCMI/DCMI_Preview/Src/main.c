/**
  ******************************************************************************
  * @file    DCMI/DCMI_CaptureMode/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describe how to configure the camera in continuous mode
  *          and QVGA resolution and to freeze/unfreeze the capture.
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

/** @addtogroup DCMI_CaptureMode
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern DCMI_HandleTypeDef hDcmiHandler;
DMA2D_HandleTypeDef Dma2dHandle;
uint16_t pBuffer[ST7789H2_LCD_PIXEL_WIDTH * ST7789H2_LCD_PIXEL_WIDTH];
HAL_StatusTypeDef hal_status = HAL_OK;
__IO uint32_t capture_suspended = 0;
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void IO_Init(void);
static void LCD_ImagePreparation(uint16_t x0, uint16_t y0, uint16_t xSize, uint16_t ySize);
static HAL_StatusTypeDef LCD_Write(uint32_t OrigAddress, uint32_t DestAddress, uint32_t TransferSize);
static void OnError_Handler(uint32_t condition);
static void BSP_JOY_Sel_Init(void);
static void DMA2D_Config(void);
static void TransferError(DMA2D_HandleTypeDef* Dma2dHandle);
static void TransferComplete(DMA2D_HandleTypeDef* Dma2dHandle);

/* Exported functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  HAL_StatusTypeDef hal_status = HAL_OK;  										  

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

  /*##-2- Initialize the LCD #################################################*/
  /*##-1- LEDs and joystick initialization  #################################################*/
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED1);
  BSP_JOY_Sel_Init();
  
  /*##-2- LCD configuration  #################################################*/
  /* I/O initialization, required before LCD initialization */
  IO_Init();
  /* LCD initialization */
  BSP_LCD_Init();  

  /* DMA2D initialization */   
  DMA2D_Config(); 


  /*##-3- Camera Initialization ############################*/
  /* Initialize the Camera in QVGA mode */
  BSP_CAMERA_Init(CAMERA_R320x240);

  /* Wait 1s to let auto-loops in the camera module converge and lead to correct exposure */
  HAL_Delay(1000);
  
  /*##-4- Camera Continuous capture start in QVGA resolution ############################*/
  /* Disable unwanted HSYNC (IT_LINE)/VSYNC interrupts */
  __HAL_DCMI_DISABLE_IT(&hDcmiHandler, DCMI_IT_LINE | DCMI_IT_VSYNC);
 
  /* LCD size is 240 x 240 and format is RGB565 i.e. 16 bpp or 2 bytes/pixel. 
     The LCD frame size is therefore 240 * 240 half-words of (240*240)/2 32-bit long words . 
     Since the DMA associated to DCMI IP is configured in  BSP_CAMERA_MspInit() of stm32l496g_discovery_camera.c file
     with words alignment, the last parameter of HAL_DCMI_Start_DMA is set to:
      (ST7789H2_LCD_PIXEL_WIDTH*ST7789H2_LCD_PIXEL_HEIGHT)/2, that is 240 * 240 / 2
   */   
  hal_status = HAL_DCMI_Start_DMA(&hDcmiHandler, DCMI_MODE_CONTINUOUS,  (uint32_t)pBuffer , (ST7789H2_LCD_PIXEL_WIDTH*ST7789H2_LCD_PIXEL_HEIGHT)/2 );
  OnError_Handler(hal_status != HAL_OK); 



  while (1)
  {
  }
}

/**
  * @brief  Camera Frame Event callback.
  */
void BSP_CAMERA_FrameEventCallback(void)
{
  /* Fully fill the LCD screen */
  LCD_ImagePreparation(0, 0, ST7789H2_LCD_PIXEL_WIDTH, ST7789H2_LCD_PIXEL_HEIGHT);
  
  /* Write data (through DMA2D) */
  hal_status = LCD_Write((uint32_t) (&pBuffer), (uint32_t)&(LCD_ADDR->REG), ST7789H2_LCD_PIXEL_WIDTH * ST7789H2_LCD_PIXEL_HEIGHT);
  OnError_Handler(hal_status != HAL_OK); 
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
  * @brief LCD image preparation.
  * @note  Configure image position and size for Discovery STM32L496 LCD
  *        and set LCD in pixel writing mode.
  *        This API must be followed by LCD_Write() call. 
  * @param  x0: first pixel x position
  * @param  y0: first pixel y position 
  * @param  xSize: image width (in pixels)
  * @param  ySize: image height (in pixels)           
  * @retval None
  */
static void LCD_ImagePreparation(uint16_t x0, uint16_t y0, uint16_t xSize, uint16_t ySize)
{
    /* CASET: Column Address Set */ 
  LCD_IO_WriteReg(ST7789H2_CASET);
  /* Send commands */
  LCD_IO_WriteData(0x00);
  LCD_IO_WriteData(x0);
  LCD_IO_WriteData(0x00);
  LCD_IO_WriteData(x0 + xSize -1);
    /* RASET: Row Address Set */  
  LCD_IO_WriteReg(ST7789H2_RASET);  
  /* Send commands */
  LCD_IO_WriteData(0x00);
  LCD_IO_WriteData(y0);
  LCD_IO_WriteData(0x00);
  LCD_IO_WriteData(y0 + ySize -1); 
    
    /* Prepare to write to LCD RAM */
  LCD_IO_WriteReg(ST7789H2_WRITE_RAM);  
}            
  
/**
  * @brief LCD image write-up.
  * @note  Resort to DMA to feed image pixels to STM32L496 Discovery LCD.   
  *        LCD is configured in RGB565: one pixel is coded over 16 bits.   
  * @param  OrigAddress: Image address
  * @param  DestAddress: LCD RAM address
  * @param  TransferSize: image size (in pixels)      
  * @retval HAL status
  */          
static HAL_StatusTypeDef LCD_Write(uint32_t OrigAddress, uint32_t DestAddress, uint32_t TransferSize)
{          
  /* Force 1 pixel per line and width in pixels x height in pixels   */
  /* as number of lines to align DMA2D transfer to LCD configuration */            
  if (HAL_OK != HAL_DMA2D_Start_IT(&Dma2dHandle, OrigAddress, DestAddress, 1, TransferSize))
  {
    return HAL_ERROR;
  }           
  
  return HAL_OK;
}   

static void DMA2D_Config(void)
  {
  HAL_StatusTypeDef hal_status = HAL_OK;
  
  /* Enable DMA2D clock */
  __HAL_RCC_DMA2D_CLK_ENABLE();

  /* NVIC configuration for DMA2D transfer complete interrupt */
  HAL_NVIC_SetPriority(DMA2D_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2D_IRQn);   

  /* Configure the DMA2D Mode, Color Mode and output offset */  
  Dma2dHandle.Init.Mode         = DMA2D_M2M;              /* DMA2D Mode memory to memory */
  Dma2dHandle.Init.ColorMode    = DMA2D_OUTPUT_RGB565;    /* Output color mode is RGB565 : 16 bpp */
  Dma2dHandle.Init.OutputOffset = 0x0;                    /* No offset in output */  
  Dma2dHandle.Init.RedBlueSwap   = DMA2D_RB_REGULAR;      /* No R&B swap for the output image */
  Dma2dHandle.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;   /* No alpha inversion for the output image */

  /* DMA2D Callbacks Configuration */
  Dma2dHandle.XferCpltCallback  = TransferComplete;
  Dma2dHandle.XferErrorCallback = TransferError;
  
  /* Foreground Configuration : Layer 1 */
  Dma2dHandle.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  Dma2dHandle.LayerCfg[1].InputAlpha = 0xFF;                    /* Fully opaque */
  Dma2dHandle.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;  /* Foreground layer format is RGB565 : 16 bpp */
  Dma2dHandle.LayerCfg[1].InputOffset = 0x0;                    /* No offset in input */
  Dma2dHandle.LayerCfg[1].RedBlueSwap   = DMA2D_RB_REGULAR;     /* No R&B swap for the input foreground image */
  Dma2dHandle.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA;  /* No alpha inversion for the input foreground image */

  Dma2dHandle.Instance = DMA2D; 
      
  /* DMA2D Initialization */
  hal_status = HAL_DMA2D_Init(&Dma2dHandle);
  OnError_Handler(hal_status != HAL_OK);
    
  hal_status = HAL_DMA2D_ConfigLayer(&Dma2dHandle, 1);
  OnError_Handler(hal_status != HAL_OK);
}   

/**
  * @brief  On Error Handler on condition TRUE.
  * @param  condition : Can be TRUE or FALSE
  * @retval None
  */
static void OnError_Handler(uint32_t condition)
{
  if(condition)
  {
    BSP_LED_On(LED1);
    while(1) { ; } /* Blocking on error */
  }
}

/**
  * @brief  DMA2D Transfer complete callback
  * @param  hdma2d: DMA2D handle. 
  * @note   This example shows a simple way to report end of DMA2D transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
static void TransferComplete(DMA2D_HandleTypeDef *hdma2d)
{
  /* Set LED2 On */
  BSP_LED_On(LED2);   
}

/**
  * @brief  DMA2D error callback
  * @param  hdma2d: DMA2D handle
  * @note   This example shows a simple way to report DMA2D transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
static void TransferError(DMA2D_HandleTypeDef *hdma2d)
{
  /* Set LED1 On */
  BSP_LED_On(LED1);
}

/**
  * @brief  EXTI line detection callback.
  * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == SEL_JOY_PIN)
  {
    if (capture_suspended == 1)
    {
      HAL_DCMI_Resume(&hDcmiHandler);
      capture_suspended = 0;
      /* Set LED2 On */
      BSP_LED_On(LED2);  
    }
    else
    {
      HAL_DCMI_Suspend(&hDcmiHandler);
      capture_suspended = 1;
      /* Set LED2 Off */
      BSP_LED_Off(LED2); 
    }
  }
}


/**
  * @brief  Configure joystick Select button in EXTI mode
  * @retval None
  */
void BSP_JOY_Sel_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Initialize the joystick Select button clock */
  SEL_JOY_GPIO_CLK_ENABLE();
   
  /* GPIO structure initialization */  
  GPIO_InitStruct.Pin = SEL_JOY_PIN;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW ;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
         
  HAL_GPIO_Init(SEL_JOY_GPIO_PORT, &GPIO_InitStruct);
      
  /* Enable and set joystick Select button EXTI Interrupt to the lowest priority */
  HAL_NVIC_SetPriority((IRQn_Type)(SEL_JOY_EXTI_IRQn), 0x0F, 0x00);
  HAL_NVIC_EnableIRQ((IRQn_Type)(SEL_JOY_EXTI_IRQn));  
      
  return;
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
#endif /* USE_FULL_ASSERT */


/**
  * @}
  */

/**
  * @}
  */

