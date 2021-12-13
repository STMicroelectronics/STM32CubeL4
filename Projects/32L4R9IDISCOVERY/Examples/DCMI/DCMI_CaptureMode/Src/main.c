/**
  ******************************************************************************
  * @file    DCMI/DCMI_CaptureMode/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use DCMI peripheral and
  *          LCD DSI to capture and display an RGB image (390x390) using the
  *          STM32L4xx HAL API and BSP.
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
#include <string.h>
#include <stdio.h>
#include "stm32l4r9i_discovery_camera.h"
#include "stm32l4r9i_discovery_psram.h"

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup DCMI/DCMI_CaptureMode
  * @{
  */

/* Physical frame buffer for background and foreground layers */
/* 390*390 pixels with 24bpp - 20% (with GFXMMU) */
#if defined ( __ICCARM__ )  /* IAR Compiler */
  #pragma data_alignment = 16
uint32_t              PhysFrameBuffer[390*390/2];
#elif defined (__GNUC__)    /* GNU Compiler */
uint32_t              PhysFrameBuffer[91260] __attribute__ ((aligned (16)));
#else                       /* ARM Compiler */
__align(16) uint32_t  PhysFrameBuffer[91260];
#endif

/* LTDC and DSI handles */
DSI_HandleTypeDef    DsiHandle;
LTDC_HandleTypeDef   LtdcHandle;
extern DCMI_HandleTypeDef  hDcmiHandler;
DMA2D_HandleTypeDef Dma2dHandle;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define VSYNC               1
#define VBP                 1
#define VFP                 1
#define VACT                390
#define HSYNC               1
#define HBP                 1
#define HFP                 1
#define HACT                390

#define BRIGHTNESS_MIN      50
#define BRIGHTNESS_NORMAL   200

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMA2D_HandleTypeDef   Dma2dHandle;
static __IO int32_t pending_buffer = -1;


/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static uint8_t LCD_Config(void);
static void LCD_PowerOn(void);
static void IO_Init(void);
extern void show_to_lcd(void);

HAL_StatusTypeDef hal_status = HAL_OK;

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Error Handler.
  * @retval None
  */
static void Error_Handler(void)
{
  BSP_LED_On(LED1);
  while(1) { ; } /* Blocking on error */
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32L4xx HAL library initialization:
       - Configure the Flash prefetch, Instruction cache, Data cache
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the System clock to have a frequency of 120 MHz */
  SystemClock_Config();

  /* Initialize used Leds */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  IO_Init();

  /* Initialize the LCD   */
  if(LCD_Config() != LCD_OK)
  {
    Error_Handler();
  }


  /* Initialize the Camera in QVGA mode */
  BSP_CAMERA_Init(CAMERA_R640x480);

  HAL_Delay(500);

  /* Disable unwanted HSYNC (IT_LINE)/VSYNC interrupts */
  __HAL_DCMI_DISABLE_IT(&hDcmiHandler, DCMI_IT_LINE | DCMI_IT_VSYNC);

  hal_status = HAL_DCMI_Start_DMA(&hDcmiHandler, DCMI_MODE_CONTINUOUS,  (uint32_t)PhysFrameBuffer , 76050);

  /* Infinite loop */
  while(1)
  {
  }
}


void show_to_lcd(void)
{
  BSP_LED_Toggle(LED1);
}

/**
  * @brief  Camera Frame Event callback.
  */
void BSP_CAMERA_FrameEventCallback(void)
{
  BSP_LED_Toggle(LED2);
  HAL_DSI_Refresh(&DsiHandle);
}

/**
  * @brief IO initialization.
  * @note  GPIO PH.00 setting to activate STM32L4R9 Discovery I/Os
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
  * @brief  End of Refresh DSI callback.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_EndOfRefreshCallback(DSI_HandleTypeDef *hdsi)
{
  if(pending_buffer > 0)
  {
    pending_buffer = 0;
  }
}

/** @defgroup Private_Functions Private Functions
  * @{
  */

/**
  * @brief  Initializes the DSI LCD.
  * The initialization is done as below:
  *     - DSI PLL initialization
  *     - DSI initialization
  *     - GFXMMU initialization
  *     - LTDC initialization
  *     - RM67162 LCD Display IC Driver initialization
  * @param  None
  * @retval LCD state
  */
static uint8_t LCD_Config(void)
{
  LTDC_LayerCfgTypeDef    LayerCfg;
  DSI_PLLInitTypeDef      dsiPllInit;
  DSI_PHY_TimerTypeDef    PhyTimings;
  DSI_HOST_TimeoutTypeDef HostTimeouts;
  DSI_LPCmdTypeDef        LPCmd;
  DSI_CmdCfgTypeDef       CmdCfg;

  /* Power on LCD */
  LCD_PowerOn();

  /**********************/
  /* LTDC CONFIGURATION */
  /**********************/

  /* LTDC initialization */
  __HAL_LTDC_RESET_HANDLE_STATE(&LtdcHandle);
  LtdcHandle.Instance = LTDC;
  LtdcHandle.Init.HSPolarity         = LTDC_HSPOLARITY_AL;
  LtdcHandle.Init.VSPolarity         = LTDC_VSPOLARITY_AL;
  LtdcHandle.Init.DEPolarity         = LTDC_DEPOLARITY_AL;
  LtdcHandle.Init.PCPolarity         = LTDC_PCPOLARITY_IPC;
  LtdcHandle.Init.HorizontalSync     = 0;   /* HSYNC width - 1 */
  LtdcHandle.Init.VerticalSync       = 0;   /* VSYNC width - 1 */
  LtdcHandle.Init.AccumulatedHBP     = 1;   /* HSYNC width + HBP - 1 */
  LtdcHandle.Init.AccumulatedVBP     = 1;   /* VSYNC width + VBP - 1 */
  LtdcHandle.Init.AccumulatedActiveW = 391; /* HSYNC width + HBP + Active width - 1 */
  LtdcHandle.Init.AccumulatedActiveH = 391; /* VSYNC width + VBP + Active height - 1 */
  LtdcHandle.Init.TotalWidth         = 392; /* HSYNC width + HBP + Active width + HFP - 1 */
  LtdcHandle.Init.TotalHeigh         = 392; /* VSYNC width + VBP + Active height + VFP - 1 */
  LtdcHandle.Init.Backcolor.Red      = 0;   /* Not used default value */
  LtdcHandle.Init.Backcolor.Green    = 0;   /* Not used default value */
  LtdcHandle.Init.Backcolor.Blue     = 0;   /* Not used default value */
  if(HAL_LTDC_Init(&LtdcHandle) != HAL_OK)
  {
    return(LCD_ERROR);
  }

  /* LTDC layer 1 configuration */
  LayerCfg.WindowX0        = 0;
  LayerCfg.WindowX1        = 390;
  LayerCfg.WindowY0        = 0;
  LayerCfg.WindowY1        = 390;
  LayerCfg.PixelFormat     = LTDC_PIXEL_FORMAT_RGB565;
  LayerCfg.Alpha           = 0xFF; /* NU default value */
  LayerCfg.Alpha0          = 0; /* NU default value */
  LayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA; /* Not Used: default value */
  LayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA; /* Not Used: default value */
  LayerCfg.FBStartAdress   = (uint32_t)PhysFrameBuffer;
  LayerCfg.ImageWidth      = 390;
  LayerCfg.ImageHeight     = 390;
  LayerCfg.Backcolor.Red   = 0; /* Not Used: default value */
  LayerCfg.Backcolor.Green = 0; /* Not Used: default value */
  LayerCfg.Backcolor.Blue  = 0; /* Not Used: default value */
  if(HAL_LTDC_ConfigLayer(&LtdcHandle, &LayerCfg, LTDC_LAYER_1) != HAL_OK)
  {
    return(LCD_ERROR);
  }

  /*********************/
  /* DSI CONFIGURATION */
  /*********************/

  /* DSI initialization */
  __HAL_DSI_RESET_HANDLE_STATE(&DsiHandle);
  DsiHandle.Instance = DSI;
  DsiHandle.Init.AutomaticClockLaneControl = DSI_AUTO_CLK_LANE_CTRL_DISABLE;
  /* We have 1 data lane at 500Mbps => lane byte clock at 500/8 = 62,5 MHZ */
  /* We want TX escape clock at around 20MHz and under 20MHz so clock division is set to 4 */
  DsiHandle.Init.TXEscapeCkdiv             = 4;
  DsiHandle.Init.NumberOfLanes             = DSI_ONE_DATA_LANE;
  /* We have HSE value at 16 Mhz and we want data lane at 500Mbps */
  dsiPllInit.PLLNDIV = 125;
  dsiPllInit.PLLIDF  = DSI_PLL_IN_DIV4;
  dsiPllInit.PLLODF  = DSI_PLL_OUT_DIV1;
  if(HAL_DSI_Init(&DsiHandle, &dsiPllInit) != HAL_OK)
  {
    return(LCD_ERROR);
  }

  PhyTimings.ClockLaneHS2LPTime  = 33; /* Tclk-post + Tclk-trail + Ths-exit = [(60ns + 52xUI) + (60ns) + (300ns)]/16ns */
  PhyTimings.ClockLaneLP2HSTime  = 30; /* Tlpx + (Tclk-prepare + Tclk-zero) + Tclk-pre = [150ns + 300ns + 8xUI]/16ns */
  PhyTimings.DataLaneHS2LPTime   = 11; /* Ths-trail + Ths-exit = [(60ns + 4xUI) + 100ns]/16ns */
  PhyTimings.DataLaneLP2HSTime   = 21; /* Tlpx + (Ths-prepare + Ths-zero) + Ths-sync = [150ns + (145ns + 10xUI) + 8xUI]/16ns */
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime        = 7;
  if(HAL_DSI_ConfigPhyTimer(&DsiHandle, &PhyTimings) != HAL_OK)
  {
    return(LCD_ERROR);
  }



  if (HAL_DSI_SetLowPowerRXFilter(&DsiHandle, 10000) != HAL_OK)
  {
    return(LCD_ERROR);
  }
  if (HAL_DSI_ConfigErrorMonitor(&DsiHandle, HAL_DSI_ERROR_NONE) != HAL_OK)
  {
    return(LCD_ERROR);
  }


  HostTimeouts.TimeoutCkdiv                 = 1;
  HostTimeouts.HighSpeedTransmissionTimeout = 0;
  HostTimeouts.LowPowerReceptionTimeout     = 0;
  HostTimeouts.HighSpeedReadTimeout         = 0;
  HostTimeouts.LowPowerReadTimeout          = 0;
  HostTimeouts.HighSpeedWriteTimeout        = 0;
  HostTimeouts.HighSpeedWritePrespMode      = 0;
  HostTimeouts.LowPowerWriteTimeout         = 0;
  HostTimeouts.BTATimeout                   = 0;
  if(HAL_DSI_ConfigHostTimeouts(&DsiHandle, &HostTimeouts) != HAL_OK)
  {
    return(LCD_ERROR);
  }

  LPCmd.LPGenShortWriteNoP  = DSI_LP_GSW0P_ENABLE;
  LPCmd.LPGenShortWriteOneP = DSI_LP_GSW1P_ENABLE;
  LPCmd.LPGenShortWriteTwoP = DSI_LP_GSW2P_ENABLE;
  LPCmd.LPGenShortReadNoP   = DSI_LP_GSR0P_ENABLE;
  LPCmd.LPGenShortReadOneP  = DSI_LP_GSR1P_ENABLE;
  LPCmd.LPGenShortReadTwoP  = DSI_LP_GSR2P_ENABLE;
  LPCmd.LPGenLongWrite      = DSI_LP_GLW_DISABLE;
  LPCmd.LPDcsShortWriteNoP  = DSI_LP_DSW0P_ENABLE;
  LPCmd.LPDcsShortWriteOneP = DSI_LP_DSW1P_ENABLE;
  LPCmd.LPDcsShortReadNoP   = DSI_LP_DSR0P_ENABLE;
  LPCmd.LPDcsLongWrite      = DSI_LP_DLW_DISABLE;
  LPCmd.LPMaxReadPacket     = DSI_LP_MRDP_DISABLE;
  LPCmd.AcknowledgeRequest  = DSI_ACKNOWLEDGE_DISABLE;
  if(HAL_DSI_ConfigCommand(&DsiHandle, &LPCmd) != HAL_OK)
  {
    return(LCD_ERROR);
  }

  CmdCfg.VirtualChannelID      = 0;
  CmdCfg.ColorCoding           = DSI_RGB888;
  CmdCfg.CommandSize           = 390;
  CmdCfg.TearingEffectSource   = DSI_TE_DSILINK;
  CmdCfg.TearingEffectPolarity = DSI_TE_FALLING_EDGE;
  CmdCfg.HSPolarity            = DSI_HSYNC_ACTIVE_LOW;
  CmdCfg.VSPolarity            = DSI_VSYNC_ACTIVE_LOW;
  CmdCfg.DEPolarity            = DSI_DATA_ENABLE_ACTIVE_HIGH;
  CmdCfg.VSyncPol              = DSI_VSYNC_FALLING;
  CmdCfg.AutomaticRefresh      = DSI_AR_ENABLE;
  CmdCfg.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_ENABLE;
  if(HAL_DSI_ConfigAdaptedCommandMode(&DsiHandle, &CmdCfg) != HAL_OK)
  {
    return(LCD_ERROR);
  }

  /* Disable the Tearing Effect interrupt activated by default on previous function */
  __HAL_DSI_DISABLE_IT(&DsiHandle, DSI_IT_TE);

  if(HAL_DSI_ConfigFlowControl(&DsiHandle, DSI_FLOW_CONTROL_BTA) != HAL_OK)
  {
    return(LCD_ERROR);
  }

  /* Enable DSI */
  __HAL_DSI_ENABLE(&DsiHandle);

  /*************************/
  /* LCD POWER ON SEQUENCE */
  /*************************/
  /* Step 1 */
  /* Go to command 2 */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x01);
  /* IC Frame rate control, set power, sw mapping, mux switch timing command */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x06, 0x62);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x0E, 0x80);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x0F, 0x80);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x10, 0x71);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x13, 0x81);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x14, 0x81);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x15, 0x82);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x16, 0x82);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x18, 0x88);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x19, 0x55);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1A, 0x10);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1C, 0x99);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1D, 0x03);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1E, 0x03);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1F, 0x03);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x20, 0x03);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x25, 0x03);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x26, 0x8D);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x2A, 0x03);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x2B, 0x8D);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x36, 0x00);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x37, 0x10);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x3A, 0x00);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x3B, 0x00);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x3D, 0x20);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x3F, 0x3A);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x40, 0x30);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x41, 0x1A);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x42, 0x33);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x43, 0x22);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x44, 0x11);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x45, 0x66);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x46, 0x55);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x47, 0x44);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x4C, 0x33);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x4D, 0x22);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x4E, 0x11);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x4F, 0x66);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x50, 0x55);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x51, 0x44);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x57, 0x33);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x6B, 0x1B);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x70, 0x55);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x74, 0x0C);

  /* Go to command 3 */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x02);
  /* Set the VGMP/VGSP coltage control */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x9B, 0x40);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x9C, 0x00);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x9D, 0x20);

  /* Go to command 4 */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x03);
  /* Set the VGMP/VGSP coltage control */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x9B, 0x40);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x9C, 0x00);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x9D, 0x20);

  /* Go to command 5 */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x04);
  /* VSR command */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x5D, 0x10);
  /* VSR1 timing set */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x00, 0x8D);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x01, 0x00);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x02, 0x01);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x03, 0x01);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x04, 0x10);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x05, 0x01);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x06, 0xA7);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x07, 0x20);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x08, 0x00);
  /* VSR2 timing set */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x09, 0xC2);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x0A, 0x00);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x0B, 0x02);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x0C, 0x01);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x0D, 0x40);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x0E, 0x06);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x0F, 0x01);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x10, 0xA7);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x11, 0x00);
  /* VSR3 timing set */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x12, 0xC2);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x13, 0x00);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x14, 0x02);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x15, 0x01);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x16, 0x40);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x17, 0x07);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x18, 0x01);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x19, 0xA7);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1A, 0x00);
  /* VSR4 timing set */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1B, 0x82);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1C, 0x00);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1D, 0xFF);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1E, 0x05);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x1F, 0x60);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x20, 0x02);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x21, 0x01);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x22, 0x7C);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x23, 0x00);
  /* VSR5 timing set */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x24, 0xC2);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x25, 0x00);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x26, 0x04);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x27, 0x02);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x28, 0x70);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x29, 0x05);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x2A, 0x74);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x2B, 0x8D);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x2D, 0x00);
  /* VSR6 timing set */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x2F, 0xC2);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x30, 0x00);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x31, 0x04);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x32, 0x02);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x33, 0x70);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x34, 0x07);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x35, 0x74);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x36, 0x8D);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x37, 0x00);
  /* VSR marping command */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x5E, 0x20);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x5F, 0x31);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x60, 0x54);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x61, 0x76);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x62, 0x98);

  /* Go to command 6 */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x05);
  /* Set the ELVSS voltage */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x05, 0x17);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x2A, 0x04);
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x91, 0x00);

  /* Go back in standard commands */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0xFE, 0x00);

  /* Set the Pixel format */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x3A, 0x07);

  /* Set tear off */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, DSI_SET_TEAR_OFF, 0x0);

  /* Set DSI mode to internal timing added vs ORIGINAL for Command mode */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0xC2, 0x0);

  /* Set memory address MODIFIED vs ORIGINAL */
  {
    uint8_t InitParam1[4]= {0x00, 0x04, 0x01, 0x89};
    uint8_t InitParam2[4]= {0x00, 0x00, 0x01, 0x85};

    HAL_DSI_LongWrite(&DsiHandle, 0, DSI_DCS_LONG_PKT_WRITE, 4, DSI_SET_COLUMN_ADDRESS, InitParam1);
    HAL_DSI_LongWrite(&DsiHandle, 0, DSI_DCS_LONG_PKT_WRITE, 4, DSI_SET_PAGE_ADDRESS, InitParam2);
  }

  /* Sleep out */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P0, DSI_EXIT_SLEEP_MODE, 0x0);

  HAL_Delay(120);

  /* Set default Brightness */
  HAL_DSI_ShortWrite(&DsiHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x51, BRIGHTNESS_NORMAL);

  /* Set display on */
  if(HAL_DSI_ShortWrite(&DsiHandle,
                        0,
                        DSI_DCS_SHORT_PKT_WRITE_P0,
                        DSI_SET_DISPLAY_ON,
                        0x0) != HAL_OK)
  {
    return(LCD_ERROR);
  }

  /* Enable DSI Wrapper */
  __HAL_DSI_WRAPPER_ENABLE(&DsiHandle);

  return LCD_OK;
}

/**
  * @brief  LCD power on
  *         Power on LCD.
  */
static void LCD_PowerOn(void)
{
  BSP_IO_Init();

  /* Configure the GPIO connected to DSI_3V3_POWERON signal as output low */
  /* to activate 3V3_LCD. VDD_LCD is also activated if VDD = 3,3V */
  BSP_IO_WritePin(IO_PIN_8, GPIO_PIN_RESET);
  BSP_IO_ConfigPin(IO_PIN_8, IO_MODE_OUTPUT);

  /* Wait at least 1ms before enabling 1V8_LCD */
  HAL_Delay(1);

  /* Configure the GPIO connected to DSI_1V8_POWERON signal as output low */
  /* to activate 1V8_LCD. VDD_LCD is also activated if VDD = 1,8V */
  BSP_IO_WritePin(AGPIO_PIN_2, GPIO_PIN_RESET);
  BSP_IO_ConfigPin(AGPIO_PIN_2, IO_MODE_OUTPUT);

  /* Wait at least 15 ms (minimum reset low width is 10ms and add margin for 1V8_LCD ramp-up) */
  HAL_Delay(15);

  /* Configure the GPIO connected to DSI_RESET signal */
  BSP_IO_ConfigPin(IO_PIN_10, IO_MODE_OUTPUT);

  /* Deactivate DSI_RESET */
  BSP_IO_WritePin(IO_PIN_10, GPIO_PIN_SET);

  /* Wait reset complete time (maximum time is 5ms when LCD in sleep mode and 120ms when LCD is not in sleep mode) */
  HAL_Delay(120);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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

