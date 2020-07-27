/**
  ******************************************************************************
  * @file    stm32l4r9i_eval_ospi_ram.c
  * @author  MCD Application Team
  * @brief   This file includes a standard driver for the ISS66WVH8M8 HyperRAM
  *          memory mounted on STM32L4R9I-EVAL board.
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
  [..]
   (#) This driver is used to drive the ISS66WVH8M8 HyperRAM external
       memory mounted on STM32L4R9I-EVAL evaluation board.

   (#) This driver need a specific component driver (ISS66WVH8M8) to be included with.

   (#) Initialization steps:
       (++) Initialize the HyperRAM external memory using the BSP_OSPI_RAM_Init() function. This
            function includes the MSP layer hardware resources initialization and the
            OSPI interface with the external memory.

   (#) HyperRAM memory operations
       (++) HyperRAM memory can be accessed with read/write operations once it is
            initialized.
            Read/write operation can be performed with AHB access using the functions
            BSP_OSPI_RAM_Read()/BSP_OSPI_RAM_Write().
       (++) The function BSP_OSPI_RAM_GetInfo() returns the configuration of the HyperRAM memory.
            (see the HyperRAM memory data sheet)
       (++) Perform erase block operation using the function BSP_OSPI_RAM_Erase_Block() and by
            specifying the block address. You can perform an erase operation of the whole
            chip by calling the function BSP_OSPI_RAM_Erase_Chip().
       (++) The function BSP_OSPI_RAM_GetStatus() returns the current status of the HyperRAM memory.
            (see the HyperRAM memory data sheet)
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4r9i_eval_ospi_ram.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L4R9I_EVAL
  * @{
  */

/** @defgroup STM32L4R9I_EVAL_OSPI_RAM STM32L4R9I_EVAL OSPI RAM
  * @{
  */

/* Private constants --------------------------------------------------------*/
/** @defgroup STM32L4R9I_EVAL_OSPI_RAM_Private_Constants Private Constants
  * @{
  */
#define OSPI_RAM_QUAD_DISABLE       0x0
#define OSPI_RAM_QUAD_ENABLE        0x1

#define OSPI_RAM_HIGH_PERF_DISABLE  0x0
#define OSPI_RAM_HIGH_PERF_ENABLE   0x1
/**
  * @}
  */
/* Private variables ---------------------------------------------------------*/

/** @defgroup STM32L4R9I_EVAL_OSPI_RAM_Private_Variables Private Variables
  * @{
  */
OSPI_HandleTypeDef OspiRamHandle;
DMA_HandleTypeDef DmaOspiRamHandle;

/**
  * @}
  */


/* Private functions ---------------------------------------------------------*/

/** @defgroup STM32L4R9I_EVAL_OSPI_RAM_Private_Functions Private Functions
  * @{
  */
static void OSPI_RAM_MspInit  (void);
static void OSPI_RAM_MspDeInit(void);

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/

/** @addtogroup STM32L4R9I_EVAL_OSPI_RAM_Exported_Functions
  * @{
  */

/**
  * @brief  Initializes the OSPI interface.
  * @retval HyperRAM memory status
  */
uint8_t BSP_OSPI_RAM_Init(void)
{
  OSPI_HyperbusCfgTypeDef  sHyperbusCfg;

  OspiRamHandle.Instance = OCTOSPI2;

  /* Call the DeInit function to reset the driver */
  if (HAL_OSPI_DeInit(&OspiRamHandle) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  /* System level initialization */
  OSPI_RAM_MspInit();

  /* OctoSPI initialization */
  OspiRamHandle.Init.FifoThreshold         = 4;
  OspiRamHandle.Init.DualQuad              = HAL_OSPI_DUALQUAD_DISABLE;
  OspiRamHandle.Init.MemoryType            = HAL_OSPI_MEMTYPE_HYPERBUS;
  OspiRamHandle.Init.DeviceSize            = POSITION_VAL(ISS66WVH8M8_RAM_SIZE); /* 64 MBits */
  OspiRamHandle.Init.ChipSelectHighTime    = 2;
  OspiRamHandle.Init.FreeRunningClock      = HAL_OSPI_FREERUNCLK_DISABLE;
  OspiRamHandle.Init.ClockMode             = HAL_OSPI_CLOCK_MODE_0;
  OspiRamHandle.Init.ClockPrescaler        = 3; /* OctoSPI clock = 110MHz / ClockPrescaler = 36.67 MHz */
  OspiRamHandle.Init.SampleShifting        = HAL_OSPI_SAMPLE_SHIFTING_NONE;
  OspiRamHandle.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
  OspiRamHandle.Init.ChipSelectBoundary    = 0;
  OspiRamHandle.Init.DelayBlockBypass      = HAL_OSPI_DELAY_BLOCK_USED;

  if (HAL_OSPI_Init(&OspiRamHandle) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  /* OctoSPI Hyperbus configuration to access memory space */
  sHyperbusCfg.RWRecoveryTime   = 3; /* 40 ns @ 60MHz */
  sHyperbusCfg.AccessTime       = ISS66WVH8M8_LATENCY_166M;
  sHyperbusCfg.WriteZeroLatency = HAL_OSPI_LATENCY_ON_WRITE;
  sHyperbusCfg.LatencyMode      = HAL_OSPI_FIXED_LATENCY;

  if (HAL_OSPI_HyperbusCfg(&OspiRamHandle, &sHyperbusCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  return OSPI_RAM_OK;
}

/**
  * @brief  De-Initializes the OSPI interface.
  * @retval HyperRAM memory status
  */
uint8_t BSP_OSPI_RAM_DeInit(void)
{
  OspiRamHandle.Instance = OCTOSPI1;

  /* Call the DeInit function to reset the driver */
  if (HAL_OSPI_DeInit(&OspiRamHandle) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  /* System level De-initialization */
  OSPI_RAM_MspDeInit();

  return OSPI_RAM_OK;
}

/**
  * @brief  Reads an amount of data from the HyperRAM memory.
  * @param  pData: Pointer to data to be read
  * @param  ReadAddr: Read start address
  * @param  Size: Size of data to read
  * @retval HyperRAM memory status
  */
uint8_t BSP_OSPI_RAM_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
  OSPI_HyperbusCmdTypeDef  sCommand;

  /* Initialize the read command */
  sCommand.AddressSpace = HAL_OSPI_MEMORY_ADDRESS_SPACE;
  sCommand.AddressSize  = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.Address      = ReadAddr;
  sCommand.DQSMode      = HAL_OSPI_DQS_ENABLE;
  sCommand.NbData       = Size;

  /* Configure the command */
  if (HAL_OSPI_HyperbusCmd(&OspiRamHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  /* Reception of the data */
  if (HAL_OSPI_Receive(&OspiRamHandle, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  return OSPI_RAM_OK;
}

/**
  * @brief  Reads an amount of data from the HyperRAM memory in DMA mode.
  * @param  pData: Pointer to data to be read
  * @param  ReadAddr: Read start address
  * @param  Size: Size of data to read
  * @retval HyperRAM memory status
  */
uint8_t BSP_OSPI_RAM_Read_DMA(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
  OSPI_HyperbusCmdTypeDef  sCommand;

  /* Initialize the read command */
  sCommand.AddressSpace = HAL_OSPI_MEMORY_ADDRESS_SPACE;
  sCommand.AddressSize  = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.Address      = ReadAddr;
  sCommand.DQSMode      = HAL_OSPI_DQS_ENABLE;
  sCommand.NbData       = Size;

  /* Configure the command */
  if (HAL_OSPI_HyperbusCmd(&OspiRamHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  /* Reception of the data */
  if (HAL_OSPI_Receive_DMA(&OspiRamHandle, pData) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  return OSPI_RAM_OK;
}

/**
  * @brief  Writes an amount of data to the HyperRAM memory.
  * @param  pData: Pointer to data to be written
  * @param  WriteAddr: Write start address
  * @param  Size: Size of data to write
  * @retval HyperRAM memory status
  */
uint8_t BSP_OSPI_RAM_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
  OSPI_HyperbusCmdTypeDef  sCommand;

  /* Initialize the write command */
  sCommand.AddressSpace = HAL_OSPI_MEMORY_ADDRESS_SPACE;
  sCommand.AddressSize  = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.Address      = WriteAddr;
  sCommand.DQSMode      = HAL_OSPI_DQS_ENABLE;
  sCommand.NbData       = Size;

  /* Configure the command */
  if (HAL_OSPI_HyperbusCmd(&OspiRamHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  /* Transmission of the data */
  if (HAL_OSPI_Transmit(&OspiRamHandle, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  return OSPI_RAM_OK;
}

/**
  * @brief  Writes an amount of data to the HyperRAM memory in DMA mode.
  * @param  pData: Pointer to data to be written
  * @param  WriteAddr: Write start address
  * @param  Size: Size of data to write
  * @retval HyperRAM memory status
  */
uint8_t BSP_OSPI_RAM_Write_DMA(uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
  OSPI_HyperbusCmdTypeDef  sCommand;

  /* Initialize the write command */
  sCommand.AddressSpace = HAL_OSPI_MEMORY_ADDRESS_SPACE;
  sCommand.AddressSize  = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.Address      = WriteAddr;
  sCommand.DQSMode      = HAL_OSPI_DQS_ENABLE;
  sCommand.NbData       = Size;

  /* Configure the command */
  if (HAL_OSPI_HyperbusCmd(&OspiRamHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  /* Transmission of the data */
  if (HAL_OSPI_Transmit_DMA(&OspiRamHandle, pData) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  return OSPI_RAM_OK;
}

/**
  * @brief  Configure the OSPI in memory-mapped mode
  * @retval HyperRAM memory status
  */
uint8_t BSP_OSPI_RAM_EnableMemoryMappedMode(void)
{
  OSPI_HyperbusCmdTypeDef  sCommand;
  OSPI_MemoryMappedTypeDef sMemMappedCfg;

  /* OctoSPI Hyperbus command configuration */
  sCommand.AddressSpace = HAL_OSPI_MEMORY_ADDRESS_SPACE;
  sCommand.AddressSize  = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.Address      = 0;
  sCommand.DQSMode      = HAL_OSPI_DQS_ENABLE;
  sCommand.NbData       = 1;

  if (HAL_OSPI_HyperbusCmd(&OspiRamHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  /* OctoSPI activation of memory-mapped mode */
  sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;

  if (HAL_OSPI_MemoryMapped(&OspiRamHandle, &sMemMappedCfg) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  return OSPI_RAM_OK;
}

/**
  * @brief  This function enter the HyperRAM memory in deep power down mode.
  * @retval HyperRAM memory status
  */
uint8_t BSP_OSPI_RAM_EnterDeepPowerDown(void)
{
  OSPI_HyperbusCfgTypeDef  sHyperbusCfg;
  OSPI_HyperbusCmdTypeDef  sCommand;
  uint16_t reg;

  /* To write register, no latency is required. So a reconfiguration is performed */
  sHyperbusCfg.RWRecoveryTime   = 3; /* 40 ns @ 60MHz */
  sHyperbusCfg.AccessTime       = ISS66WVH8M8_LATENCY_166M;
  sHyperbusCfg.WriteZeroLatency = HAL_OSPI_NO_LATENCY_ON_WRITE;
  sHyperbusCfg.LatencyMode      = HAL_OSPI_FIXED_LATENCY;

  if (HAL_OSPI_HyperbusCfg(&OspiRamHandle, &sHyperbusCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  /* Read the value of the CR0 register */
  sCommand.AddressSpace = HAL_OSPI_REGISTER_ADDRESS_SPACE;
  sCommand.AddressSize  = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.Address      = ISS66WVH8M8_CR0_ADDRESS;
  sCommand.DQSMode      = HAL_OSPI_DQS_ENABLE;
  sCommand.NbData       = 2;

  if (HAL_OSPI_HyperbusCmd(&OspiRamHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  if (HAL_OSPI_Receive(&OspiRamHandle, (uint8_t *)(&reg), HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  /* Update the deep power down value of the CR0 register */
  SET_BIT(reg, ISS66WVH8M8_CR0_DPDE);

  if (HAL_OSPI_HyperbusCmd(&OspiRamHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  if (HAL_OSPI_Transmit(&OspiRamHandle, (uint8_t *)(&reg), HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  /* ---          Memory takes 10us min to enter deep power down          --- */
  /* --- At least 30us should be respected before leaving deep power down --- */

  /* A reconfiguration is performed to re-enable the latency on write access */
  sHyperbusCfg.WriteZeroLatency = HAL_OSPI_LATENCY_ON_WRITE;

  if (HAL_OSPI_HyperbusCfg(&OspiRamHandle, &sHyperbusCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  return OSPI_RAM_OK;
}

/**
  * @brief  This function leave the HyperRAM memory from deep power down mode.
  * @retval HyperRAM memory status
  */
uint8_t BSP_OSPI_RAM_LeaveDeepPowerDown(void)
{
  /* --- A dummy command is sent to the memory, as the nCS should be low for at least 200 ns --- */
  /* ---                  Memory takes 150us max to leave deep power down                    --- */

   OSPI_HyperbusCmdTypeDef  sCommand;
   uint32_t data;

  /* Initialize the read command */
  sCommand.AddressSpace = HAL_OSPI_MEMORY_ADDRESS_SPACE;
  sCommand.AddressSize  = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.Address      = 0;
  sCommand.DQSMode      = HAL_OSPI_DQS_ENABLE;
  sCommand.NbData       = 4;

  /* Configure the command */
  if (HAL_OSPI_HyperbusCmd(&OspiRamHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  /* Reception of the data */
  if (HAL_OSPI_Receive(&OspiRamHandle, (uint8_t*)(&data), HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_RAM_ERROR;
  }

  return OSPI_RAM_OK;
}

/**
  * @brief  Handles OctoSPI HyperRAM DMA transfer interrupt request.
  * @retval None
  */
void BSP_OSPI_RAM_DMA_IRQHandler(void)
{
  HAL_DMA_IRQHandler(OspiRamHandle.hdma);
}

/**
  * @brief  Handles OctoSPI HyperRAM interrupt request.
  * @retval None
  */
void BSP_OSPI_RAM_IRQHandler(void)
{
  HAL_OSPI_IRQHandler(&OspiRamHandle);
}
/**
  * @}
  */

/** @addtogroup STM32L4R9I_EVAL_OSPI_RAM_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the OSPI MSP.
  * @retval None
  */
static void OSPI_RAM_MspInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable the OctoSPI memory interface, DMA, DMAMUX and GPIO clocks */
  __HAL_RCC_OSPIM_CLK_ENABLE();
  __HAL_RCC_OSPI2_CLK_ENABLE();
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /* Reset the OctoSPI memory interface */
  __HAL_RCC_OSPI2_FORCE_RESET();
  __HAL_RCC_OSPI2_RELEASE_RESET();

  /* IOSV bit MUST be set to access GPIO port G[2:15] */
  __HAL_RCC_PWR_CLK_ENABLE();
  SET_BIT(PWR->CR2, PWR_CR2_IOSV);

  /* OctoSPI CS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = GPIO_PIN_5;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_OCTOSPIM_P2;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /* OctoSPI DQS GPIO pins configuration  */
  GPIO_InitStruct.Pin  = GPIO_PIN_15;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* OctoSPI D3, D4, D5, D6 and D7 GPIO pins configuration  */
  GPIO_InitStruct.Pin  = (GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /* OctoSPI CK, nCK, D2, D1 and D0 GPIO pins configuration  */
  GPIO_InitStruct.Pin = (GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11);
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /* Configure the OctoSPI DMA */
  DmaOspiRamHandle.Instance = DMA1_Channel2;
  DmaOspiRamHandle.Init.Request             = DMA_REQUEST_OCTOSPI2;
  DmaOspiRamHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
  DmaOspiRamHandle.Init.MemInc              = DMA_MINC_ENABLE;
  DmaOspiRamHandle.Init.Mode                = DMA_NORMAL;
  DmaOspiRamHandle.Init.Priority            = DMA_PRIORITY_LOW;
  DmaOspiRamHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  DmaOspiRamHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;

  __HAL_LINKDMA(&OspiRamHandle, hdma, DmaOspiRamHandle);
  HAL_DMA_Init(&DmaOspiRamHandle);

  /* Enable and set priority of the OctoSPI and DMA interrupts */
  HAL_NVIC_SetPriority(OCTOSPI2_IRQn, 0x0F, 0);
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0x00, 0);

  HAL_NVIC_EnableIRQ(OCTOSPI2_IRQn);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
}

/**
  * @brief  De-Initializes the OSPI MSP.
  * @retval None
  */
static void OSPI_RAM_MspDeInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Disable DMA and OctoSPI interrupts */
  HAL_NVIC_DisableIRQ(DMA1_Channel2_IRQn);
  HAL_NVIC_DisableIRQ(OCTOSPI2_IRQn);

  /* De-configure the OctoSPI DMA */
  DmaOspiRamHandle.Instance = DMA1_Channel2;
  HAL_DMA_DeInit(&DmaOspiRamHandle);

  /* OctoSPI CLK, CS, D0-D7, DQS GPIO pins de-configuration  */
  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_15);
  HAL_GPIO_DeInit(GPIOH, (GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12));
  HAL_GPIO_DeInit(GPIOI, (GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11));

  /* Set GPIOI pin 5 in pull up mode (optimum default setting) */
  GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pin       = GPIO_PIN_5;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /* Set GPIOI pin 6 and 7 in no pull, low state (optimum default setting) */
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Pin       = (GPIO_PIN_6 | GPIO_PIN_7);
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_7, GPIO_PIN_RESET);

  /* Reset the OctoSPI memory interface */
  __HAL_RCC_OSPI2_FORCE_RESET();
  __HAL_RCC_OSPI2_RELEASE_RESET();

  /* Disable the OctoSPI memory interface clock */
  __HAL_RCC_OSPI2_CLK_DISABLE();
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

