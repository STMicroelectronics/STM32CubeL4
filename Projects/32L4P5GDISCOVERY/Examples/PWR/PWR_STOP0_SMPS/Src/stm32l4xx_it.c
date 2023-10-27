/**
  ******************************************************************************
  * @file    PWR/PWR_STOP0_SMPS/Src/stm32l4xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
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
#include "stm32l4xx_it.h"

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup PWR_STOP0_SMPS
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LOOP_PLACEMENT 0x080FE800
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

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
  HAL_SYSTICK_IRQHandler();
}

/******************************************************************************/
/*                  STM32L4xx Peripherals Interrupt Handlers                  */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles External line 10 to 15 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
#if defined(__CC_ARM)
  register uint32_t StackPointer __ASM("msp");	
#else	
  register uint32_t StackPointer;
#endif
  register uint32_t JumpAddress;
  register uint32_t StackContent;
  static uint32_t counter = 0;
  uint32_t read_counter;

  /* Every other interruption, exit the infinite loop done in Run mode.
     The other interruption allows to exit Stop0 mode. */
  if (counter == 0)
  {
#if !defined(__CC_ARM)
    __ASM volatile ("MOV %0, SP\n" : "=r" (StackPointer) );
#endif

     /* xPSR, PC, LR, R12, R3-R0 are all pushed onto the interrupt-stack.
        First retrieve the position of PC.
        It contains the address to return to out of the ISR handling.
        It is therefore the infinite loop location, hard-coded in the icf file.
        Depending upon the compiler and/or optimization used, need to read from
        the stack pointer position to find it.
        Arbitrarily look for 26 positions at max. */
    read_counter = 0;		
    StackContent = *(uint32_t *)(StackPointer);
#if defined(__CC_ARM)
    while (StackContent != LOOP_PLACEMENT)
    {
      read_counter++;
      StackContent = *(uint32_t *)(StackPointer + 4*read_counter);
			if (read_counter == 26)
			{
				Error_Handler();
			}
    }
#else		
    while ((StackContent != LOOP_PLACEMENT) && (read_counter < 26))
    {
      read_counter++;
      StackContent = *(uint32_t *)(StackPointer + 4*read_counter);
    }
#endif
/* At this point, if found, PC is at position (StackPointer+4*read_counter) */
    if ((read_counter != 26) && (read_counter > 0))
    {
      /* LR contains the return address out of whileloop1() function.
         To exit the infinite loop run in thread mode,
         retrieve LR value to jump there after the ISR execution. */
      JumpAddress = *(uint32_t *)(StackPointer+4*(read_counter-1));

      /* Overwrite PC to exit the infinite loop upon ISR execution */
       *(uint32_t *)(StackPointer+4*read_counter) = JumpAddress;
    }
    else
    {
      Error_Handler();
    }

    counter = 1;	
  }
  else
  {
    counter = 0;
  }

  HAL_GPIO_EXTI_IRQHandler(SEL_JOY_PIN);

}


/**
  * @}
  */

/**
  * @}
  */

