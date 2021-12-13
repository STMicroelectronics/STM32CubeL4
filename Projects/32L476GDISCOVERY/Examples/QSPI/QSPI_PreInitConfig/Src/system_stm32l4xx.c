/**
  ******************************************************************************
  * @file    system_stm32l4xx.c
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M4 Device Peripheral Access Layer System Source File
  *
  *   This file provides four functions and one global variable to be called from
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32l4xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick
  *                                  timer or configure other parameters.
  *
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  *      - SystemInit_ExtMemCtl(): Configures the GPIO and the QSPI in order to
  *                                access the external QSPI memory at the init.
  *                                This function is called when the switch DATA_IN_QSPI
  *                                is activated.
  *
  *      - SetSysClk(): This function is called when the switch DATA_IN_QSPI is 
  *                     activated, it configures the clock at 80 MHz with the PLL 
  *                     activated. It allows to access QSPI memory with high speed.
  *
  *   After each device reset the MSI (4 MHz) is used as system clock source.
  *   Then SystemInit() function is called, in "startup_stm32l4xx.s" file, to
  *   configure the system clock before to branch to main program.
  *
  *   This file configures the system clock as follows:
  *=============================================================================
  *-----------------------------------------------------------------------------
  *        System Clock source                    | MSI (DATA_IN_QSPI deactivated)
  *                                               | PLL (DATA_IN_QSPI activated)
  *-----------------------------------------------------------------------------
  *        SYSCLK(Hz)                             | 4000000 (DATA_IN_QSPI deactivated)
  *                                               | 80000000 (DATA_IN_QSPI activated)
  *-----------------------------------------------------------------------------
  *        HCLK(Hz)                               | 4000000 (DATA_IN_QSPI deactivated)
  *                                               | 80000000 (DATA_IN_QSPI activated)
  *-----------------------------------------------------------------------------
  *        AHB Prescaler                          | 1
  *-----------------------------------------------------------------------------
  *        APB1 Prescaler                         | 1
  *-----------------------------------------------------------------------------
  *        APB2 Prescaler                         | 1
  *-----------------------------------------------------------------------------
  *        PLL_M                                  | 1
  *-----------------------------------------------------------------------------
  *        PLL_N                                  | 8 (DATA_IN_QSPI deactivated)
  *                                               | 40 (DATA_IN_QSPI activated)
  *-----------------------------------------------------------------------------
  *        PLL_P                                  | 7
  *-----------------------------------------------------------------------------
  *        PLL_Q                                  | 2
  *-----------------------------------------------------------------------------
  *        PLL_R                                  | 2
  *-----------------------------------------------------------------------------
  *        PLLSAI1_P                              | NA
  *-----------------------------------------------------------------------------
  *        PLLSAI1_Q                              | NA
  *-----------------------------------------------------------------------------
  *        PLLSAI1_R                              | NA
  *-----------------------------------------------------------------------------
  *        PLLSAI2_P                              | NA
  *-----------------------------------------------------------------------------
  *        PLLSAI2_Q                              | NA
  *-----------------------------------------------------------------------------
  *        PLLSAI2_R                              | NA
  *-----------------------------------------------------------------------------
  *        Require 48MHz for USB OTG FS,          | Disabled
  *        SDIO and RNG clock                     |
  *-----------------------------------------------------------------------------
  *=============================================================================
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

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32l4xx_system
  * @{
  */

/** @addtogroup STM32L4xx_System_Private_Includes
  * @{
  */

#include "stm32l4xx.h"

#if !defined  (HSE_VALUE)
  #define HSE_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (MSI_VALUE)
  #define MSI_VALUE    ((uint32_t)4000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* MSI_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_Defines
  * @{
  */

/************************* Miscellaneous Configuration ************************/
/*!< Uncomment the following line if you need to use QSPI memory mounted
     on DK as data memory  */
#define DATA_IN_QSPI

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field.
                                   This value must be a multiple of 0x200. */
/******************************************************************************/
/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_Variables
  * @{
  */
  /* The SystemCoreClock variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetHCLKFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
  */
#if defined (DATA_IN_QSPI)
  uint32_t SystemCoreClock = 80000000;
#else
  uint32_t SystemCoreClock = 4000000;
#endif

  const uint8_t  AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
  const uint8_t  APBPrescTable[8] =  {0, 0, 0, 0, 1, 2, 3, 4};
  const uint32_t MSIRangeTable[12] = {100000, 200000, 400000, 800000, 1000000, 2000000, \
                                      4000000, 8000000, 16000000, 24000000, 32000000, 48000000};
/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_FunctionPrototypes
  * @{
  */
#if defined (DATA_IN_QSPI)
  static void SetSysClk(void);
  static void SystemInit_ExtMemCtl(void); 
#endif /* DATA_IN_QSPI */

/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system.
  * @param  None
  * @retval None
  */

void SystemInit(void)
{
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif
  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set MSION bit */
  RCC->CR |= RCC_CR_MSION;

  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;

  /* Reset HSEON, CSSON , HSION, and PLLON bits */
  RCC->CR &= (uint32_t)0xEAF6FFFF;

  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x00001000;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Disable all interrupts */
  RCC->CIER = 0x00000000;

#if defined (DATA_IN_QSPI)
  SetSysClk();
  SystemInit_ExtMemCtl();
#endif /* DATA_IN_QSPI */

  /* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif
}

/**
  * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.
  *
  * @note   - The system frequency computed by this function is not the real
  *           frequency in the chip. It is calculated based on the predefined
  *           constant and the selected clock source:
  *
  *           - If SYSCLK source is MSI, SystemCoreClock will contain the MSI_VALUE(*)
  *
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(**)
  *
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(***)
  *
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(***)
  *             or HSI_VALUE(*) or MSI_VALUE(*) multiplied/divided by the PLL factors.
  *
  *         (*) MSI_VALUE is a constant defined in stm32l4xx_hal.h file (default value
  *             4 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.
  *
  *         (**) HSI_VALUE is a constant defined in stm32l4xx_hal.h file (default value
  *              16 MHz) but the real value may vary depending on the variations
  *              in voltage and temperature.
  *
  *         (***) HSE_VALUE is a constant defined in stm32l4xx_hal.h file (default value
  *              8 MHz), user has to ensure that HSE_VALUE is same as the real
  *              frequency of the crystal used. Otherwise, this function may
  *              have wrong result.
  *
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  *
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate(void)
{
  uint32_t tmp = 0, msirange = 0, pllvco = 0, pllr = 2, pllsource = 0, pllm = 2;

  /* Get MSI Range frequency--------------------------------------------------*/
  if((RCC->CR & RCC_CR_MSIRGSEL) == RESET)
  { /* MSISRANGE from RCC_CSR applies */
    msirange = (RCC->CSR & RCC_CSR_MSISRANGE) >> 8;
  }
  else
  { /* MSIRANGE from RCC_CR applies */
    msirange = (RCC->CR & RCC_CR_MSIRANGE) >> 4;
  }
  /*MSI frequency range in HZ*/
  msirange = MSIRangeTable[msirange];

  /* Get SYSCLK source -------------------------------------------------------*/
  switch (RCC->CFGR & RCC_CFGR_SWS)
  {
    case 0x00:  /* MSI used as system clock source */
      SystemCoreClock = msirange;
      break;

    case 0x04:  /* HSI used as system clock source */
      SystemCoreClock = HSI_VALUE;
      break;

    case 0x08:  /* HSE used as system clock source */
      SystemCoreClock = HSE_VALUE;
      break;

    case 0x0C:  /* PLL used as system clock  source */
      /* PLL_VCO = (HSE_VALUE or HSI_VALUE or MSI_VALUE/ PLLM) * PLLN
         SYSCLK = PLL_VCO / PLLR
         */
      pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC);
      pllm = ((RCC->PLLCFGR & RCC_PLLCFGR_PLLM) >> 4) + 1 ;

      switch (pllsource)
      {
        case 0x02:  /* HSI used as PLL clock source */
          pllvco = (HSI_VALUE / pllm);
          break;

        case 0x03:  /* HSE used as PLL clock source */
          pllvco = (HSE_VALUE / pllm);
          break;

        default:    /* MSI used as PLL clock source */
          pllvco = (msirange / pllm);
          break;
      }
      pllvco = pllvco * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 8);
      pllr = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> 25) + 1) * 2;
      SystemCoreClock = pllvco/pllr;
      break;

    default:
      SystemCoreClock = msirange;
      break;
  }
  /* Compute HCLK clock frequency --------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK clock frequency */
  SystemCoreClock >>= tmp;
}

#if defined (DATA_IN_QSPI)
/**
  * @brief  Configures the clock at 80MHz.
  *         Called in startup_stm32l4xx.s before jump to main.
  *         This function configures the clock for fast access to external memories
  * @param  None
  * @retval None
  */
void SetSysClk(void)
{
  register uint32_t tmpreg = 0, timeout = 0xFFFF;

  /*--------------------------------------------------------------------------*/
  /*------------ Configuration of the clock at 80 MHz with PLL on ------------*/
  /*--------------------------------------------------------------------------*/
  /* Configuration of the PLL */
  /* - PLLR = 2
     - PLLN = 40
     - PLLSRC = MSI */
  RCC->PLLCFGR = 0x00002801;
  /* Enable PLL */
  RCC->CR |= RCC_CR_PLLON;
  
  /* Wait that PLL is ready */
  timeout = 0xFFFF;
  do
  {
    tmpreg = (RCC->CR & RCC_CR_PLLRDY); 
  } while((tmpreg != RCC_CR_PLLRDY) && (timeout-- > 0));
  
  if (timeout != 0)
  {
    /* Enable PLL output */
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;
    /* Configure the flash latency */
    FLASH->ACR = 0x00000604;
    /* Select PLL as system clock */
    RCC->CFGR = RCC_CFGR_SW_PLL;
    
    /* Wait that the PLL selection is effective */
    timeout = 0xFFFF;
    do
    {
      tmpreg = (RCC->CFGR & RCC_CFGR_SWS); 
    } while((tmpreg != RCC_CFGR_SWS) && (timeout-- > 0));
  }
}

/**
  * @brief  Setup the external memory controller.
  *         Configures the GPIO and the QSPI in order to access the external 
  *         QSPI memory at the init.
  *         This function is called when the switch DATA_IN_QSPI is activated in 
  *         SystemInit() before jump to main.
  * @param  None
  * @retval None
  */
void SystemInit_ExtMemCtl(void)
{
  /****************************************************************************/
  /*                                                                          */
  /* Configuration of the IOs :                                               */
  /* --------------------------                                               */
  /* GPIOE10 : CLK                                                            */
  /* GPIOE11 : BK1_nCS                                                        */
  /* GPIOE12 : BK1_IO0/SO                                                     */
  /* GPIOE13 : BK1_IO1/SI                                                     */
  /* GPIOE14 : BK1_IO2                                                        */
  /* GPIOE15 : BK1_IO3                                                        */
  /*                                                                          */
  /* Configuration of the QSPI :                                              */
  /* ---------------------------                                              */
  /* - Instruction is on one single line                                      */
  /* - Address is 24-bits on four lines                                       */
  /* - No alternate bytes                                                     */
  /* - Ten dummy cycles                                                       */
  /* - Data is on four lines                                                  */
  /*                                                                          */
  /* If the clock is changed :                                                */
  /* -------------------------                                                */
  /* - Modify the prescaler in the control register                           */
  /* - Update the number of dummy cycles on the memory side and on            */
  /*   communication configuration register                                   */
  /*                                                                          */
  /* If the memory is changed :                                               */
  /* --------------------------                                               */
  /* - Update the device configuration register with the memory configuration */
  /* - Modify the instructions with the instruction set of the memory         */
  /* - Configure the number of dummy cycles as described in memory datasheet  */
  /* - Modify the data size and alternate bytes according memory datasheet    */
  /*                                                                          */
  /****************************************************************************/ 
  
  register uint32_t tmpreg = 0, datareg = 0, timeout = 0xFFFF;

  /*--------------------------------------------------------------------------*/
  /*------------------ Activation of the peripheral clocks -------------------*/
  /*--------------------------------------------------------------------------*/
  /* Enable clock of the QSPI */
  RCC->AHB3ENR |= RCC_AHB3ENR_QSPIEN;
  
  /* Enable clock of the IO pins */
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
  
  /*--------------------------------------------------------------------------*/
  /*--------------------- Configuration of the I/O pins ----------------------*/
  /*--------------------------------------------------------------------------*/
  /* Configure alternate function selection for IO pins */
  /* - GPIOE15, GPIOE14, GPIOE13, GPIOE12, GPIOE11 and GPIOE10 as AF10 */
  GPIOE->AFR[1] = 0xAAAAAA00;

  /* Configure alternate function mode for IO pins */
  /* - GPIOE15, GPIOE14, GPIOE13, GPIOE12, GPIOE11 and GPIOE10 as Alternate function mode */
  GPIOE->MODER = 0xAAAFFFFF;

  /* Configure output speed for IO pins */
  /* - GPIOE15, GPIOE14, GPIOE13, GPIOE12, GPIOE11 and GPIE10 as Very high speed */
  GPIOE->OSPEEDR = 0xFFF00000;

  /* Configure pull-up or pull-down for IO pins */
  /* - GPIOE11 as Pull-up */
  GPIOE->PUPDR = 0x00400000;
  
  /*--------------------------------------------------------------------------*/
  /*----------------------- Initialization of the QSPI -----------------------*/
  /*--------------------------------------------------------------------------*/
  /* Wait that the QSPI is ready */
  timeout = 0xFFFF;
  do
  {
    tmpreg = (QUADSPI->SR & QUADSPI_SR_BUSY); 
  } while((tmpreg != 0) && (timeout-- > 0));
  
  if (timeout != 0)
  {
    /* Configure device configuration register of QSPI */
    /* - FSIZE = 23 */
    QUADSPI->DCR = 0x00170000;
    /* Configure control register of QSPI */
    /* - PRESCALER = 1
       - FTHRES = 3
       - EN = 1 */
    QUADSPI->CR = 0x01000301;
    
    /*------------------------------------------------------------------------*/
    /*----------- Configuration of the dummy cycles on flash side ------------*/
    /*------------------------------------------------------------------------*/
    /* Configure communication register to read volatile configuration register */
    /* - FMODE = Indirect read
       - DMODE = Data on a single line
       - IMODE = Instruction on a single line
       - INSTRUCTION = READ_VOL_CFG_REG_CMD */
    QUADSPI->CCR = 0x05000185;
    
    /* Wait that the transfer is complete */
    timeout = 0xFFFF;
    do
    {
      tmpreg = (QUADSPI->SR & QUADSPI_SR_TCF); 
    } while((tmpreg == 0) && (timeout-- > 0));
    
    if (timeout != 0)
    {
      /* Read received value */
      datareg = QUADSPI->DR;
      /* Clear transfer complete flag */
      QUADSPI->FCR = QUADSPI_FCR_CTCF;
      /* Perform abort (mandatory workaround for this version of QSPI) */
      QUADSPI->CR |= QUADSPI_CR_ABORT;
      
      /* Wait that the abort is complete */
      timeout = 0xFFFF;
      do
      {
        tmpreg = (QUADSPI->SR & QUADSPI_SR_TCF); 
      } while((tmpreg == 0) && (timeout-- > 0));
      
      if (timeout != 0)
      {
        /* Clear transfer complete flag */
        QUADSPI->FCR = QUADSPI_FCR_CTCF;
        
        /* Configure communication register to enable write operations */
        /* - FMODE = Indirect write
           - IMODE = Instruction on a single line
           - INSTRUCTION = WRITE_ENABLE_CMD */
        QUADSPI->CCR = 0x00000106;
        
        /* Wait that the transfer is complete */
        timeout = 0xFFFF;
        do
        {
          tmpreg = (QUADSPI->SR & QUADSPI_SR_TCF); 
        } while((tmpreg == 0) && (timeout-- > 0));
        
        if (timeout != 0)
        {
          /* Clear transfer complete flag */
          QUADSPI->FCR = QUADSPI_FCR_CTCF;
          
          /* Configure the mask for the auto-polling mode on write enable bit of status register */
          QUADSPI->PSMKR = 0x2;
          /* Configure the value for the auto-polling mode on write enable bit of status register */
          QUADSPI->PSMAR = 0x2;
          /* Configure the auto-polling interval */
          QUADSPI->PIR   = 0x10;
          /* Configure control register to automatically stop the auto-polling mode */
          QUADSPI->CR |= QUADSPI_CR_APMS;
          /* Configure communication register to perform auto-polling mode on status register */
          /* - FMODE = Automatic polling
             - DMODE = Data on a single line
             - IMODE = Instruction on a single line
             - INSTRUCTION = READ_STATUS_REG_CMD */
          QUADSPI->CCR = 0x09000105;
          
          /* Wait that the status match occurs */
          timeout = 0xFFFF;
          do
          {
            tmpreg = (QUADSPI->SR & QUADSPI_SR_SMF); 
          } while((tmpreg == 0) && (timeout-- > 0));
          
          if (timeout != 0)
          {
            /* Clear status match flag */
            QUADSPI->FCR = QUADSPI_FCR_CSMF;
            
            /* Configure communication register to write volatile configuration register */
            /* - FMODE = Indirect write
               - DMODE = Data on a single line
               - IMODE = Instruction on a single line
               - INSTRUCTION = WRITE_VOL_CFG_REG_CMD */
            QUADSPI->CCR = 0x01000181;
            /* Write the value to transmit (volatile configuration register with new dummy cycles) */
            QUADSPI->DR = ((datareg & 0x0F) | 0xA0);
            
            /* Wait that the transfer is complete */
            timeout = 0xFFFF;
            do
            {
              tmpreg = (QUADSPI->SR & QUADSPI_SR_TCF); 
            } while((tmpreg == 0) && (timeout-- > 0));
            
            if (timeout != 0)
            {
              /* Clear transfer complete flag */
              QUADSPI->FCR = QUADSPI_FCR_CTCF;
              /* Perform abort (mandatory workaround for this version of QSPI) */
              QUADSPI->CR |= QUADSPI_CR_ABORT;
              
              /* Wait that the abort is complete */
              timeout = 0xFFFF;
              do
              {
                tmpreg = (QUADSPI->SR & QUADSPI_SR_TCF); 
              } while((tmpreg == 0) && (timeout-- > 0));
              
              if (timeout != 0)
              {
                /* Clear transfer complete flag */
                QUADSPI->FCR = QUADSPI_FCR_CTCF;
                
                /*------------------------------------------------------------*/
                /*--------- Configuration of the memory-mapped mode ----------*/
                /*------------------------------------------------------------*/
                /* Configure communication register for reading sequence in memory-mapped mode */
                /* - FMODE = Memory-mapped
                   - DMODE = Data on four lines
                   - DCYC = 10
                   - ADSIZE = 24-bit address
                   - ADMODE = Address on four lines
                   - IMODE = Instruction on a single line
                   - INSTRUCTION = QUAD_INOUT_FAST_READ_CMD */
                QUADSPI->CCR = 0x0F282DEB;
              }
            }
          }
        }
      }
    }
  }
}
#endif /* DATA_IN_QSPI*/

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

