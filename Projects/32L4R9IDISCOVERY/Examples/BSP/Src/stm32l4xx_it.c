/**
  ******************************************************************************
  * @file    BSP/Src/stm32l4xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
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
#include "stm32l4xx_it.h"

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if defined(LCD_DIMMING)
extern RTC_HandleTypeDef RTCHandle;
extern __IO uint32_t display_dimmed;
extern __IO uint32_t maintain_display;
extern DSI_HandleTypeDef    hdsi_discovery;
#if defined(LCD_OFF_AFTER_DIMMING)
extern __IO uint32_t display_turned_off;
#endif /* defined(LCD_OFF_AFTER_DIMMING) */
extern TIM_HandleTypeDef    TimHandle;
#endif /* defined(LCD_DIMMING) */
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32L4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles External line 1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
#if defined(LCD_DIMMING)

  uint32_t screen_back_on = 0;
  
  if (display_dimmed == 1)
  {
    /* Stop screen dimming */
    HAL_TIM_Base_Stop_IT(&TimHandle);
    HAL_DSI_ShortWrite(&hdsi_discovery, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x51, BRIGHTNESS_MAX);
    display_dimmed = 0;
  }
#if defined(LCD_OFF_AFTER_DIMMING)
  else if (display_turned_off == 1)
  {
    /* Turn on LCD */
    HAL_DSI_ShortWrite(&hdsi_discovery, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x51, BRIGHTNESS_MAX);
    BSP_LCD_DisplayOn();
    display_turned_off = 0;
    screen_back_on = 1;
  }
#endif /* defined(LCD_OFF_AFTER_DIMMING) */

  maintain_display = 1; /* Prevent LCD screen dimming for at least one RTC wake-up timer counter duration */
  if (screen_back_on == 1)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(MFX_INT_PIN);
    HAL_NVIC_DisableIRQ(MFX_INT_EXTI_IRQn);
    Mfx_Event();
    HAL_NVIC_EnableIRQ(MFX_INT_EXTI_IRQn);
  }
  else
#endif /* defined(LCD_DIMMING) */

  HAL_GPIO_EXTI_IRQHandler(MFX_INT_PIN);
}

/**
  * @brief  This function handles External line 10 to 15 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
#if defined(LCD_DIMMING)
  if (display_dimmed == 1)
  {
    /* Stop screen dimming */
    HAL_TIM_Base_Stop_IT(&TimHandle);
    HAL_DSI_ShortWrite(&hdsi_discovery, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x51, BRIGHTNESS_MAX);
    display_dimmed = 0;
  }
#if defined(LCD_OFF_AFTER_DIMMING)
  else if (display_turned_off == 1)
  {
    /* Turn on LCD */
    HAL_DSI_ShortWrite(&hdsi_discovery, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x51, BRIGHTNESS_MAX);
    BSP_LCD_DisplayOn();
    display_turned_off = 0;
  }
#endif /* defined(LCD_OFF_AFTER_DIMMING) */

  maintain_display = 1;  /* Prevent LCD screen dimming for at least one RTC wake-up timer counter duration */
#endif /* defined(LCD_DIMMING) */

  HAL_GPIO_EXTI_IRQHandler(SEL_JOY_PIN);
}

/**
  * @brief  This function handles SAI1 interrupt request.
  * @param  None
  * @retval None
  */
void SAI1_IRQHandler(void)
{
  HAL_SAI_IRQHandler(&BSP_AUDIO_hSai_Tx);
  HAL_SAI_IRQHandler(&BSP_AUDIO_hSai_Rx);
}

/**
  * @brief  This function handles SAI DMA interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(BSP_AUDIO_hSai_Tx.hdmatx);
}

/**
  * @brief  This function handles SAI DMA interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Channel2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(BSP_AUDIO_hSai_Rx.hdmarx);
}

/**
  * @brief  This function handles PSRAM DMA interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Channel3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(psramHandle.hdma);
}

/**
  * @brief  This function handles DFSDM Left DMAinterrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel4_IRQHandler(void)
{
  HAL_DMA_IRQHandler(BSP_AUDIO_hDfsdmLeftFilter.hdmaReg);
}

/**
  * @brief  This function handles DFSDM Right DMAinterrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(BSP_AUDIO_hDfsdmRightFilter.hdmaReg);
}

/**
  * @brief  This function handles DMA2D Handler.
  * @param  None
  * @retval None
  */
void DMA2D_IRQHandler(void)
{
  BSP_LCD_DMA2D_IRQHandler();
}

/**
  * @brief  This function handles DSI global interrupt request.
  * @param  None
  * @retval None
  */
void DSI_IRQHandler(void)
{
#if defined(LCD_DIMMING)
  if(__HAL_DSI_GET_FLAG(&hdsi_discovery, DSI_FLAG_ER) != RESET)
  {
    if(__HAL_DSI_GET_IT_SOURCE(&hdsi_discovery, DSI_IT_ER) != RESET)
    {

      if (display_dimmed == 1)
      {
        /* Stop screen dimming */
        HAL_TIM_Base_Stop_IT(&TimHandle);
        HAL_DSI_ShortWrite(&hdsi_discovery, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x51, BRIGHTNESS_MAX);
        display_dimmed = 0;
      }
#if defined(LCD_OFF_AFTER_DIMMING)
      else if (display_turned_off == 1)
      {
        /* Turn on LCD */
        HAL_DSI_ShortWrite(&hdsi_discovery, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x51, BRIGHTNESS_MAX);
        BSP_LCD_DisplayOn();
        display_turned_off = 0;
      }
#endif /* defined(LCD_OFF_AFTER_DIMMING) */
      maintain_display = 1;     /* Flag to prevent LCD dimming or turning off */
    }
  }
#endif /* defined(LCD_DIMMING) */

  BSP_LCD_DSI_IRQHandler();
}

/**
  * @brief  This function handles LTDC global interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_IRQHandler(void)
{
  BSP_LCD_LTDC_IRQHandler();
}

/**
  * @brief  This function handles LTDC error global interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_ER_IRQHandler(void)
{
  BSP_LCD_LTDC_ER_IRQHandler();
}

/**
  * @brief  This function handles SDMMC1 interrupt request.
  * @param  None
  * @retval None
  */
void SDMMC1_IRQHandler(void)
{
  HAL_SD_IRQHandler(&hsd_discovery);
}

/**
  * @brief  DCMI interrupt handler.
  * @param  None
  * @retval None
  */
void DCMI_IRQHandler(void)
{
  HAL_DCMI_IRQHandler(&hDcmiHandler);
}

/**
  * @brief  DMA interrupt handler.
  * @param  None
  * @retval None
  */
void DMA2_Channel6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hDcmiHandler.DMA_Handle);
}

#if defined(LCD_DIMMING)
/**
  * @brief  RTC wake-up timer interrupt handler.
  * @param  None
  * @retval None
  */
void RTC_WKUP_IRQHandler(void)
{
  HAL_RTCEx_WakeUpTimerIRQHandler(&RTCHandle);
}

/**
  * @brief  Timer 3 interrupt handler.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle);
}
#endif /* defined(LCD_DIMMING) */

/**
  * @}
  */

/**
  * @}
  */

