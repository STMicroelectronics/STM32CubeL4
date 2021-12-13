/**
  ******************************************************************************
  * @file    Examples_MIX/DMA2D/DMA2D_MemToMemWithRBSwap/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a description of how to configure 
  *          DMA2D peripheral in Memory to Memory transfer mode while applying 
  *          Red and Blue Swap and display the result on LCD, in resorting to 
  *          DMA2D LL APIs.
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
#include "ARGB8888_240x160.h"

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup DMA2D_MemToMemWithLCD
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Buffers filled up by DMA2D IP */
uint32_t aBuffer0[(LAYER_SIZE_X * LAYER_SIZE_Y * LAYER_BYTE_PER_PIXEL) / 4];
uint32_t aBuffer1[(LAYER_SIZE_X * LAYER_SIZE_Y * LAYER_BYTE_PER_PIXEL) / 4];
/* Blended image Ready flag */
__IO uint32_t   blended_image_ready = 0;

/* Private function prototypes -----------------------------------------------*/
static void IO_Init(void);
static void LCD_ImagePreparation(uint16_t x0, uint16_t y0, uint16_t xSize, uint16_t ySize);
static void SystemClock_Config(void);
/* Private functions ---------------------------------------------------------*/


/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t i = 0;  
  
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
  
  /* Configure the system clock */
  SystemClock_Config(); 
    
  /* Configure LED1 and LED2 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  
  /*## LCD Initialization ##################################################*/
  /* I/O initialization, required before LCD initialization */
  IO_Init();
  
  /* LCD initialization */
  BSP_LCD_Init();

  /* Pick position in LCD */
  LCD_ImagePreparation((ST7789H2_LCD_PIXEL_WIDTH - LAYER_SIZE_X)/2, (ST7789H2_LCD_PIXEL_HEIGHT - LAYER_SIZE_Y)/2, LAYER_SIZE_X, LAYER_SIZE_Y);
  
  /*## DMA2D Clock enable ################################################*/
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2D);
  NVIC_SetPriority(DMA2D_IRQn, 0);  
  NVIC_EnableIRQ(DMA2D_IRQn);
  
  /* Infinite loop start */
  while (1)
  {
    /*## 1 - Prepare first two images to blend ###############################*/ 
    blended_image_ready = 0;  
  
    /*** 1a - First image preparation *****************************************/
    /* The first image is created from the ARGB8888 input image */
    /* Configure the DMA2D Output Mode to Pixel Format Conversion */ 
    LL_DMA2D_SetMode(DMA2D, LL_DMA2D_MODE_M2M_PFC);

    /* Configure the DMA2D output Color Mode */  
    /* For the first image, force ARGB1555 setting not supported by LCD in order 
       to artificially create a distorted image */
    LL_DMA2D_SetOutputColorMode(DMA2D, LL_DMA2D_OUTPUT_MODE_ARGB1555); 
    
    /* Foreground (Input) Configuration */
    /* Set Alpha value to full opaque */
    LL_DMA2D_FGND_SetAlpha(DMA2D, 0xFF);
    /* Foreground (Input) layer format is ARGB8888 (32 bpp) */
    LL_DMA2D_FGND_SetColorMode(DMA2D, LL_DMA2D_INPUT_MODE_ARGB8888);    
    /* Set Foreground (Input) memory address */
    LL_DMA2D_FGND_SetMemAddr(DMA2D, (uint32_t)&ARGB8888_240x160);
  
    /* Set output buffer address */ 
    LL_DMA2D_SetOutputMemAddr(DMA2D, (uint32_t)&aBuffer0);
    
    /* Config DMA2D size transfer */
    LL_DMA2D_ConfigSize(DMA2D, LAYER_SIZE_Y, LAYER_SIZE_X); 
   
    /* Enable the transfer complete, transfer error and configuration error interrupts */
    LL_DMA2D_EnableIT_TC(DMA2D);
    LL_DMA2D_EnableIT_TE(DMA2D);
    LL_DMA2D_EnableIT_CE(DMA2D);  
  
    /* Enable the Peripheral */
    LL_DMA2D_Start(DMA2D);
  
    /* Wait until first output buffer is ready */
    while(blended_image_ready == 0) {;}
    
    /*** 1b - Second image preparation ****************************************/
    /* Likewise, the second image is created from the ARGB8888 input image */
    /* Update only needed parameters */
    blended_image_ready = 0;  

    /* Configure the DMA2D output Color Mode */  
    /* Set proper output mode, supported by the LCD */
    LL_DMA2D_SetOutputColorMode(DMA2D, LL_DMA2D_OUTPUT_MODE_RGB565); 
    /* Force Red and Blue swap for illustration purposes */
    LL_DMA2D_SetOutputRBSwapMode(DMA2D, LL_DMA2D_RB_MODE_SWAP);
  
    /* Set output buffer address */ 
    LL_DMA2D_SetOutputMemAddr(DMA2D, (uint32_t)&aBuffer1);    
   
    /* Enable the transfer complete, transfer error and configuration error interrupts */
    LL_DMA2D_EnableIT_TC(DMA2D);
    LL_DMA2D_EnableIT_TE(DMA2D);
    LL_DMA2D_EnableIT_CE(DMA2D);  
  
    /* Enable the Peripheral */
    LL_DMA2D_Start(DMA2D);
  
    /* Wait until second output buffer is ready */
    while(blended_image_ready == 0) {;}    
    
  
    /*## 2 - Blend first two images  #########################################*/
    /* The first image (distorted image) gradually and smoothly gives way to the 
       second one (image with R&B swapped for illustration purposes) */
       
    /* Update only needed parameters */
    /* Set alpha mode for foreground (first image) and background (second image) buffers */ 
    LL_DMA2D_FGND_SetAlphaMode(DMA2D, LL_DMA2D_ALPHA_MODE_REPLACE);
    LL_DMA2D_BGND_SetAlphaMode(DMA2D, LL_DMA2D_ALPHA_MODE_REPLACE);
    /* Set input color mode for foreground (first image) and background (second image) buffers */ 
    LL_DMA2D_FGND_SetColorMode(DMA2D, LL_DMA2D_INPUT_MODE_RGB565);
    LL_DMA2D_BGND_SetColorMode(DMA2D, LL_DMA2D_INPUT_MODE_RGB565);    
    
    /* Set blending mode */
    LL_DMA2D_SetMode(DMA2D, LL_DMA2D_MODE_M2M_BLEND);
    /* No R&B swap for the blended output */
    LL_DMA2D_SetOutputRBSwapMode(DMA2D, LL_DMA2D_RB_MODE_REGULAR);
  
    /* Set foreground, background and output address */
    LL_DMA2D_FGND_SetMemAddr(DMA2D, (uint32_t)&aBuffer0);
    LL_DMA2D_BGND_SetMemAddr(DMA2D, (uint32_t)&aBuffer1);     
    LL_DMA2D_SetOutputMemAddr(DMA2D, (uint32_t)&(LCD_ADDR->REG));      
    
    /* Set size of transfer to LCD  */
    /* Force 1 pixel per line and width in pixels x height in pixels   */
    /* as number of lines to align DMA2D transfer to LCD configuration */
    LL_DMA2D_ConfigSize(DMA2D, LAYER_SIZE_Y * LAYER_SIZE_X, 1);       
    
    /* Gradual images blending process */
    for (i=0; i< 256; i++)
    {
      HAL_Delay(10);
    
      /* 2a - Blending process ************************************************/
      blended_image_ready = 0;

      /* While decreasing alpha value for foreground image, increase alpha value
        of background image */
      LL_DMA2D_FGND_SetAlpha(DMA2D, 0xFF - (i%256));
      LL_DMA2D_BGND_SetAlpha(DMA2D, (i%256));
                              
      /* Enable the transfer complete, transfer error and configuration error interrupts */
      LL_DMA2D_EnableIT_TC(DMA2D);
      LL_DMA2D_EnableIT_TE(DMA2D);
      LL_DMA2D_EnableIT_CE(DMA2D);  
  
      /* Enable the Peripheral */
      LL_DMA2D_Start(DMA2D);
  
      /* 2b- Copy blended image in center of LCD frame buffer *****************/
      /* Wait until blended image is ready to be displayed */
      while(blended_image_ready == 0) {;}
    }
  
    
    /*## 3 - Third image preparation #########################################*/
    /* Create third image from same ARGB8888 input image.
     DMA2D transfer parameters are set to get the correct format at the output.
     Again, update only necessary parameters  */
       
    blended_image_ready = 0;  
  
    /* Configure the DMA2D Output Mode to Pixel Format Conversion */ 
    LL_DMA2D_SetMode(DMA2D, LL_DMA2D_MODE_M2M_PFC);
  
    /* Set third image buffer address */ 
    LL_DMA2D_SetOutputMemAddr(DMA2D, (uint32_t)&aBuffer0);    
    /* Set Foreground (Input) memory address */
    LL_DMA2D_FGND_SetMemAddr(DMA2D, (uint32_t)&ARGB8888_240x160); 
    /* Foreground layer format is ARGB8888 (32 bpp) */
    LL_DMA2D_FGND_SetColorMode(DMA2D, LL_DMA2D_INPUT_MODE_ARGB8888);  
    
    /* Config DMA2D size transfer */
    LL_DMA2D_ConfigSize(DMA2D, LAYER_SIZE_Y, LAYER_SIZE_X);     
    
    /* Enable the transfer complete, transfer error and configuration error interrupts */
    LL_DMA2D_EnableIT_TC(DMA2D);
    LL_DMA2D_EnableIT_TE(DMA2D);
    LL_DMA2D_EnableIT_CE(DMA2D);  
  
    /* Enable the Peripheral */
    LL_DMA2D_Start(DMA2D);
  
    /* Wait until output buffer is ready */
    while(blended_image_ready == 0) {;}    
     
         
    /*## 4 - Blend next two images  ##########################################*/
    /* The second image (R&B swapped) now gradually and smoothly gives way to the 
       third one (that with the correct format) */
         
    /* Update only needed parameters */
    
    LL_DMA2D_ConfigSize(DMA2D, LAYER_SIZE_Y * LAYER_SIZE_X, 1);    
                           
    LL_DMA2D_FGND_SetColorMode(DMA2D, LL_DMA2D_INPUT_MODE_RGB565);
    LL_DMA2D_SetMode(DMA2D, LL_DMA2D_MODE_M2M_BLEND);
      
    LL_DMA2D_FGND_SetMemAddr(DMA2D, (uint32_t)&aBuffer1);
    LL_DMA2D_BGND_SetMemAddr(DMA2D, (uint32_t)&aBuffer0);     
    LL_DMA2D_SetOutputMemAddr(DMA2D, (uint32_t)&(LCD_ADDR->REG));  
     
    /* Gradual images blending process */
    for (i=0; i< 256; i++)
    {
      HAL_Delay(10);
    
      /* 4a - Blending process ************************************************/    
      blended_image_ready = 0;
  
      /* While decreasing alpha value for foreground image, increase alpha value
        of background image */
      LL_DMA2D_FGND_SetAlpha(DMA2D, 0xFF - (i%256));
      LL_DMA2D_BGND_SetAlpha(DMA2D, (i%256));
   
      /* Enable the transfer complete, transfer error and configuration error interrupts */
      LL_DMA2D_EnableIT_TC(DMA2D);
      LL_DMA2D_EnableIT_TE(DMA2D);
      LL_DMA2D_EnableIT_CE(DMA2D);  
  
      /* Enable the Peripheral */
      LL_DMA2D_Start(DMA2D);
  
      /* 4b- Copy blended image in center of LCD frame buffer *****************/
      /* Wait until blended image is ready to be displayed */
      while(blended_image_ready == 0) {;}
    }
  
    /* One-second delay */
    HAL_Delay(1000);
    
    
    /*## 5 - Fourth image preparation ########################################*/    
    /* Create a fully white image, stored in aBuffer1 */
    /* This image will be used to ensure a smooth fading of the last image,
       giving way to a blank screen */
    for (i=0; i< (LAYER_SIZE_X * LAYER_SIZE_Y)/2 ; i++)
    {
      *(aBuffer1+i) = 0xFFFFFFFF;
    }
  
  
    /*## 6 - Blend next two images  ##########################################*/
    /* The third image (correct format one) now gradually and smoothly fades away 
       to give way to a blank screen (the last image) */  
       
    /* The only parameters to update is that of the foreground and background addresses */       
     LL_DMA2D_FGND_SetMemAddr(DMA2D, (uint32_t)&aBuffer0);
     LL_DMA2D_BGND_SetMemAddr(DMA2D, (uint32_t)&aBuffer1);     
  
    /* Gradual images blending process */
    for (i=0; i< 256; i++)
    {
      HAL_Delay(10);
    
      /* 6a - Blending process ************************************************/    
      blended_image_ready = 0;
  
      LL_DMA2D_FGND_SetAlpha(DMA2D, 0xFF - (i%256));
      LL_DMA2D_BGND_SetAlpha(DMA2D, (i%256));
                             
      /* Enable the transfer complete, transfer error and configuration error interrupts */
      LL_DMA2D_EnableIT_TC(DMA2D);
      LL_DMA2D_EnableIT_TE(DMA2D);
      LL_DMA2D_EnableIT_CE(DMA2D);  
  
      /* Enable the Peripheral */
      LL_DMA2D_Start(DMA2D);
  
      /* 6b- Copy blended image in center of LCD frame buffer *****************/
      /* Wait until blended image is ready to be displayed */
      while(blended_image_ready == 0) {;}
    } 
  
    /* One-second delay before re-starting the complete loop */
    HAL_Delay(1000);
  
  } /*  while (1) */

}


/**
  * @brief  On Error Handler on condition TRUE.
  * @param  condition : Can be TRUE or FALSE
  * @retval None
  */
void OnError_Handler(uint32_t condition)
{
  if(condition)
  {
    BSP_LED_On(LED1);
    while(1) { ; } /* Blocking on error */
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
  HAL_Delay(200);
  
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
/**
  * @}
  */

/**
  * @}
  */

