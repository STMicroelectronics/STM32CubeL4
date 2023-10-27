/**
******************************************************************************
  * @file    spsgrf_io.h
  * @author  MCD Application Team
  * @brief   This file contains the transport layer functions (SPI instance). 
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
#include "spsgrf_io.h"

/**
 * @addtogroup SPSGRF_IO
 * @{
 */


/**
 * @defgroup SPSGR_IO_GPIO_Private_Defines                SPSGR_GPIO Private Defines
 * @{
 */
#define POR_TIME ((uint16_t)0x1E00)

/**
 * @}
 */


/**
 * @defgroup SPSGR_IO_GPIO_Private_Types             SPSGR_GPIO Private Types
 * @{
 */

/**
* @brief  Radio_Gpio Port array
*/
GPIO_TypeDef* aRADIO_GPIO_PORT[RADIO_GPIO_NUMBER] = {
  RADIO_GPIO_0_PORT,
  RADIO_GPIO_1_PORT,
  RADIO_GPIO_2_PORT,
  RADIO_GPIO_3_PORT,
  RADIO_GPIO_SDN_PORT
};


/**
* @brief  Radio_Gpio Pin array
*/
static const uint16_t aRADIO_GPIO_PIN[RADIO_GPIO_NUMBER] = {
  RADIO_GPIO_0_PIN,
  RADIO_GPIO_1_PIN,
  RADIO_GPIO_2_PIN,
  RADIO_GPIO_3_PIN,
  RADIO_GPIO_SDN_PIN   
};


/**
* @brief  Radio_Gpio Speed array
*/
static const uint32_t aRADIO_GPIO_SPEED[RADIO_GPIO_NUMBER] = { 
  RADIO_GPIO_0_SPEED,
  RADIO_GPIO_1_SPEED,
  RADIO_GPIO_2_SPEED,
  RADIO_GPIO_3_SPEED,
  RADIO_GPIO_SDN_SPEED    
};


/**
* @brief  Radio_Gpio PuPd array
*/
static const uint32_t aRADIO_GPIO_PUPD[RADIO_GPIO_NUMBER] = {
  RADIO_GPIO_0_PUPD,
  RADIO_GPIO_1_PUPD,
  RADIO_GPIO_2_PUPD,
  RADIO_GPIO_3_PUPD,
  RADIO_GPIO_SDN_PUPD   
};


/**
* @brief  Exti Mode array
*/
static const uint32_t aRADIO_GPIO_EXTI_MODE[RADIO_GPIO_NUMBER-1] = {
  RADIO_GPIO_0_EXTI_MODE,
  RADIO_GPIO_1_EXTI_MODE,
  RADIO_GPIO_2_EXTI_MODE,
  RADIO_GPIO_3_EXTI_MODE     
};


/**
* @brief  Exti IRQn array
*/
static const uint8_t aRADIO_GPIO_IRQn[RADIO_GPIO_NUMBER-1] = {
  RADIO_GPIO_0_EXTI_IRQN,
  RADIO_GPIO_1_EXTI_IRQN,
  RADIO_GPIO_2_EXTI_IRQN,
  RADIO_GPIO_3_EXTI_IRQN  
};

/**
 * @}
 */


/**
 * @defgroup SPSGRF_IO_SPI_Private_Constants             SPSGRF_IO_SPI Private Constants
 * @{
 */ 

  /* Define for SPIRIT1 board  */  
 #if !defined (USE_SPIRIT1_DEFAULT)
 #define USE_SPIRIT1_DEFAULT
#endif
  
/* SPIRIT1_Spi_config */ 
/* SPI1 */  
#define RADIO_SPI                                 SPI3
#define RADIO_SPI_CLK_ENABLE()                  __HAL_RCC_SPI3_CLK_ENABLE()
#define RADIO_SPI_CLK_DISABLE()                 __HAL_RCC_SPI3_CLK_DISABLE()

#define RADIO_SPI_MISO_PORT                      GPIOC
#define RADIO_SPI_MISO_PIN                       GPIO_PIN_11
#define RADIO_SPI_MISO_CLOCK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define RADIO_SPI_MISO_CLOCK_DISABLE()           __HAL_RCC_GPIOC_CLK_DISABLE() 
#define RADIO_SPI_MISO_AF                        GPIO_AF6_SPI3
  
#define RADIO_SPI_MOSI_PORT                      GPIOC
#define RADIO_SPI_MOSI_PIN                       GPIO_PIN_12
#define RADIO_SPI_MOSI_CLOCK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define RADIO_SPI_MOSI_CLOCK_DISABLE()           __HAL_RCC_GPIOC_CLK_DISABLE()   
#define RADIO_SPI_MOSI_AF                        GPIO_AF6_SPI3  
  
#define RADIO_SPI_SCK_PORT                      GPIOC
#define RADIO_SPI_SCK_PIN                       GPIO_PIN_10
#define RADIO_SPI_SCK_CLOCK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define RADIO_SPI_SCK_CLOCK_DISABLE()           __HAL_RCC_GPIOC_CLK_DISABLE()
#define RADIO_SPI_SCK_AF                        GPIO_AF6_SPI3

  
#define RADIO_SPI_CS_PORT                        GPIOB
#define RADIO_SPI_CS_PIN                         GPIO_PIN_5
#define RADIO_SPI_CS_CLOCK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define RADIO_SPI_CS_CLOCK_DISABLE()           __HAL_RCC_GPIOB_CLK_DISABLE()
#define RADIO_SPI_CS_AF                        0  

  
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...) */    
#define RADIO_SPI_TIMEOUT_MAX                   ((uint32_t)10000)

/* SPIRIT1_Spi_config_Private_Defines */  
#define CS_TO_SCLK_DELAY     0x0100
#define CLK_TO_CS_DELAY      0x0100

  
/* SPIRIT1_Spi_config_Headers */
#define HEADER_WRITE_MASK     0x00                                /*!< Write mask for header byte*/
#define HEADER_READ_MASK      0x01                                /*!< Read mask for header byte*/
#define HEADER_ADDRESS_MASK   0x00                                /*!< Address mask for header byte*/
#define HEADER_COMMAND_MASK   0x80                                /*!< Command mask for header byte*/
  
#define LINEAR_FIFO_ADDRESS   0xFF                                  /*!< Linear FIFO address*/

#define SPIRIT1_IRQ_ENABLE()   RadioGpioInterruptCmd(RADIO_GPIO_IRQ,0x04,0x04,ENABLE);
#define SPIRIT1_IRQ_DISABLE()  RadioGpioInterruptCmd(RADIO_GPIO_IRQ,0x04,0x04,DISABLE); 

#define ALL_IRQ_ENABLE()  __enable_irq()
#define ALL_IRQ_DISABLE() __disable_irq()
  
/* SPIRIT1_Spi_config_Private_FunctionPrototypes */
#define SPI_ENTER_CRITICAL()  SPIRIT1_IRQ_DISABLE()   
#define SPI_EXIT_CRITICAL()   SPIRIT1_IRQ_ENABLE()    
  
/* SPIRIT1_Spi_config_Private_Functions */
#define RadioSpiCSLow()        HAL_GPIO_WritePin(RADIO_SPI_CS_PORT, RADIO_SPI_CS_PIN, GPIO_PIN_RESET)
#define RadioSpiCSHigh()       HAL_GPIO_WritePin(RADIO_SPI_CS_PORT, RADIO_SPI_CS_PIN, GPIO_PIN_SET)
  
/* SPIRIT1_Spi_config_Private_Macros */
#define BUILT_HEADER(add_comm, w_r) (add_comm | w_r)                             /*!< macro to build the header byte*/
#define WRITE_HEADER        BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_WRITE_MASK) /*!< macro to build the write 
                                                                                                         header byte*/
#define READ_HEADER         BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_READ_MASK)  /*!< macro to build the read 
                                                                                                         header byte*/
#define COMMAND_HEADER      BUILT_HEADER(HEADER_COMMAND_MASK, HEADER_WRITE_MASK) /*!< macro to build the command 
                                                                                                         header byte*/
/**
 * @}
 */ 


/**
 * @defgroup SPSGRF_IO_SPI_Private_Variables              SPSGRF_IO_SPI Private Variables
 * @{
 */
SPI_HandleTypeDef pSpiHandle;
uint32_t SpiTimeout = RADIO_SPI_TIMEOUT_MAX;                         /*<! Value of Timeout when SPI communication fails */
uint16_t tmpstatus;

/**
 * @}
 */

/**
 * @defgroup SPSGRF_IO_SPI_Private_FunctionPrototypes     SPSGRF_IO_SPI Private Function Prototypes
 * @{
 */
static void SPI_Write(uint8_t Value);
static void SPI_Error(void);

/**
 * @}
 */

/**
 * @defgroup SPSGRF_IO_SPI_Private_Functions              SPSGRF_IO_SPI Private Functions
 * @{
 */

/**
  * @brief  SPI Write a byte to device
  * @param  Value: value to be written
  * @retval None
  */
static void SPI_Write(uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  while (__HAL_SPI_GET_FLAG(&pSpiHandle, SPI_FLAG_TXE) == RESET);
  status = HAL_SPI_Transmit(&pSpiHandle, (uint8_t*) &Value, 1, SpiTimeout);
    
  /* Check the communication status */
  if (status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPI_Error();
  }
}


/**
  * @brief  SPI error treatment function
  * @param  None
  * @retval None
  */
static void SPI_Error(void)
{
  /* De-initialize the SPI communication BUS */
  HAL_SPI_DeInit(&pSpiHandle);
  
  /* Re-Initiaize the SPI communication BUS */
  RadioSpiInit();
}

/**
 * @}
 */

/**
 * @defgroup SPSGRF_IO_SPI_Exported_Functions              SPSGRF_IO_SPI Exported Functions
 * @{
 */

/**
  * @brief  Initializes SPI HAL.
  * @param  None
  * @retval None
  */
void RadioSpiInit(void)
{
  if (HAL_SPI_GetState(&pSpiHandle) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
    pSpiHandle.Instance               = RADIO_SPI;
    pSpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    pSpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
    pSpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
    pSpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
    pSpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    pSpiHandle.Init.CRCPolynomial     = 7;
    pSpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
    pSpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    pSpiHandle.Init.NSS               = SPI_NSS_SOFT;       
    pSpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
    pSpiHandle.Init.Mode              = SPI_MODE_MASTER;

    //HAL_SPI_MspInit(&pSpiHandle);
    if (HAL_SPI_Init(&pSpiHandle) != HAL_OK)
    {
      while(1){};
    }
  }
}


/**
  * @brief  Initializes SPI MSP.
  * @param  SPI_HandleTypeDef* pSpiHandle
  * @retval None
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef* pSpiHandle)
{
  
  GPIO_InitTypeDef GPIO_InitStruct;
  if (pSpiHandle->Instance==RADIO_SPI)
  {
  /*** Configure the GPIOs ***/  
  /* Enable GPIO clock */
    RADIO_SPI_SCK_CLOCK_ENABLE();
    RADIO_SPI_MISO_CLOCK_ENABLE();
    RADIO_SPI_MOSI_CLOCK_ENABLE();
         
    /**SPI1 GPIO Configuration */   

    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct.Alternate = RADIO_SPI_SCK_AF;
    GPIO_InitStruct.Pin = RADIO_SPI_SCK_PIN;
    HAL_GPIO_Init(RADIO_SPI_SCK_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct.Alternate = RADIO_SPI_MISO_AF;
    GPIO_InitStruct.Pin = RADIO_SPI_MISO_PIN;
    HAL_GPIO_Init(RADIO_SPI_MISO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = RADIO_SPI_MOSI_AF;
    GPIO_InitStruct.Pin = RADIO_SPI_MOSI_PIN;
    HAL_GPIO_Init(RADIO_SPI_MOSI_PORT, &GPIO_InitStruct); 
 
    RADIO_SPI_CS_CLOCK_ENABLE();
   
    /* Configure SPI pin: CS */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = RADIO_SPI_CS_AF;
    GPIO_InitStruct.Pin = RADIO_SPI_CS_PIN;
    HAL_GPIO_Init(RADIO_SPI_CS_PORT, &GPIO_InitStruct);
    
    RADIO_SPI_CLK_ENABLE();
  }
}
 
/**
* @brief  Write single or multiple RF Transceivers register
* @param  cRegAddress: base register's address to be write
* @param  cNbBytes: number of registers and bytes to be write
* @param  pcBuffer: pointer to the buffer of values have to be written into registers
* @retval StatusBytes
*/
uint8_t * RadioSpiWriteRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint8_t aHeader[2] = {0};
  uint8_t *pStatus=(uint8_t *)&tmpstatus;
  
  tmpstatus = 0x0000;
  /* Built the aHeader bytes */
  aHeader[0] = WRITE_HEADER;
  aHeader[1] = cRegAddress;
  
  SPI_ENTER_CRITICAL();
  
  /* Puts the SPI chip select low to start the transaction */
  RadioSpiCSLow();
  
  for (volatile uint16_t Index = 0; Index < CS_TO_SCLK_DELAY; Index++);
    
  /* Write the aHeader bytes and read the SPIRIT1 status bytes */
  HAL_SPI_TransmitReceive(&pSpiHandle, (uint8_t *)&aHeader[0], (uint8_t *)&(tmpstatus), 1, SpiTimeout);
  tmpstatus = tmpstatus << 8;  
  
  /* Write the aHeader bytes and read the SPIRIT1 status bytes */
  HAL_SPI_TransmitReceive(&pSpiHandle, (uint8_t *)&aHeader[1], (uint8_t *)&tmpstatus, 1, SpiTimeout);
        
  /* Writes the registers according to the number of bytes */
  for (int index = 0; index < cNbBytes; index++)
  {
    SPI_Write(pcBuffer[index]);
  }
  
  /* To be sure to don't rise the Chip Select before the end of last sending */
   while (__HAL_SPI_GET_FLAG(&pSpiHandle, SPI_FLAG_TXE) == RESET);
  /* Puts the SPI chip select high to end the transaction */
  RadioSpiCSHigh();
  
  SPI_EXIT_CRITICAL();
  
  return pStatus;

}


/**
* @brief  Read single or multiple SPIRIT1 register
* @param  cRegAddress: base register's address to be read
* @param  cNbBytes: number of registers and bytes to be read
* @param  pcBuffer: pointer to the buffer of registers' values read
* @retval StatusBytes
*/
uint8_t * RadioSpiReadRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint8_t *pStatus = (uint8_t *)&tmpstatus;
  uint8_t aHeader[2] = {0};
  uint8_t dummy = 0xFF;
 
  tmpstatus = 0x0000;
    
  /* Built the aHeader bytes */
  aHeader[0] = READ_HEADER;
  aHeader[1] = cRegAddress;
  
  SPI_ENTER_CRITICAL();
  
  /* Put the SPI chip select low to start the transaction */
  RadioSpiCSLow();
  
  for (volatile uint16_t Index = 0; Index < CS_TO_SCLK_DELAY; Index++);

  /* Write the aHeader bytes and read the SPIRIT1 status bytes */
  HAL_SPI_TransmitReceive(&pSpiHandle, (uint8_t *)&aHeader[0], (uint8_t *)&(tmpstatus), 1, SpiTimeout);
  tmpstatus = tmpstatus << 8;  
  
  /* Write the aHeader bytes and read the SPIRIT1 status bytes */
  HAL_SPI_TransmitReceive(&pSpiHandle, (uint8_t *)&aHeader[1], (uint8_t *)&tmpstatus, 1, SpiTimeout);
 
  for (int index = 0; index < cNbBytes; index++)
  { 
    HAL_SPI_TransmitReceive(&pSpiHandle, (uint8_t *)&dummy, (uint8_t *)&(pcBuffer)[index], 1, SpiTimeout);
  } 

  /* To be sure to don't rise the Chip Select before the end of last sending */
  while (__HAL_SPI_GET_FLAG(&pSpiHandle, SPI_FLAG_TXE) == RESET);
  
  /* Put the SPI chip select high to end the transaction */
  RadioSpiCSHigh();
  
  SPI_EXIT_CRITICAL();
 
  return pStatus;
  
}


/**
* @brief  Send a command
* @param  cCommandCode: command code to be sent
* @retval StatusBytes
*/
uint8_t * RadioSpiCommandStrobes(uint8_t cCommandCode)
{
  uint8_t aHeader[2] = {0};
  uint8_t *pStatus = (uint8_t *)&tmpstatus;

  tmpstatus = 0x0000;  
  /* Built the aHeader bytes */
  aHeader[0] = COMMAND_HEADER;
  aHeader[1] = cCommandCode;
  
  SPI_ENTER_CRITICAL();
  
  /* Puts the SPI chip select low to start the transaction */
  RadioSpiCSLow();
  
  for (volatile uint16_t Index = 0; Index < CS_TO_SCLK_DELAY; Index++);
  /* Write the aHeader bytes and read the SPIRIT1 status bytes */
  HAL_SPI_TransmitReceive(&pSpiHandle, (uint8_t *)&aHeader[0], (uint8_t *)&tmpstatus, 1, SpiTimeout);
  tmpstatus = tmpstatus<<8;  

  /* Write the aHeader bytes and read the SPIRIT1 status bytes */
  HAL_SPI_TransmitReceive(&pSpiHandle, (uint8_t *)&aHeader[1], (uint8_t *)&tmpstatus, 1, SpiTimeout);

  /* To be sure to don't rise the Chip Select before the end of last sending */
  while (__HAL_SPI_GET_FLAG(&pSpiHandle, SPI_FLAG_TXE) == RESET);

  /* Puts the SPI chip select high to end the transaction */
  RadioSpiCSHigh();
  
  SPI_EXIT_CRITICAL();
  
  return pStatus;
  
}


/**
* @brief  Write data into TX FIFO
* @param  cNbBytes: number of bytes to be written into TX FIFO
* @param  pcBuffer: pointer to data to write
* @retval StatusBytes
*/
uint8_t * RadioSpiWriteFifo(uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint8_t *pStatus = (uint8_t *)&tmpstatus;
  uint8_t aHeader[2] = {0};
  
  tmpstatus = 0x0000;
  /* Built the aHeader bytes */
  aHeader[0] = WRITE_HEADER;
  aHeader[1] = LINEAR_FIFO_ADDRESS;
  
  SPI_ENTER_CRITICAL();
  
  /* Put the SPI chip select low to start the transaction */
  RadioSpiCSLow();
  
  for (volatile uint16_t Index = 0; Index < CS_TO_SCLK_DELAY; Index++);

  /* Write the aHeader bytes and read the SPIRIT1 status bytes */
  HAL_SPI_TransmitReceive(&pSpiHandle, (uint8_t *)&aHeader[0], (uint8_t *)&tmpstatus, 1, SpiTimeout);
  tmpstatus = tmpstatus<<8;  
  
    /* Write the aHeader bytes and read the SPIRIT1 status bytes */
  HAL_SPI_TransmitReceive(&pSpiHandle, (uint8_t *)&aHeader[1], (uint8_t *)&tmpstatus, 1, SpiTimeout);

    /* Writes the registers according to the number of bytes */
  for (int index = 0; index < cNbBytes; index++)
  {
    SPI_Write(pcBuffer[index]);
  }
 
  /* To be sure to don't rise the Chip Select before the end of last sending */
  while (__HAL_SPI_GET_FLAG(&pSpiHandle, SPI_FLAG_TXE) == RESET); 
  
  /* Put the SPI chip select high to end the transaction */
  RadioSpiCSHigh();
  
  SPI_EXIT_CRITICAL();
  
  return pStatus; 
}

/**
* @brief  Read data from RX FIFO
* @param  cNbBytes: number of bytes to read from RX FIFO
* @param  pcBuffer: pointer to data read from RX FIFO
* @retval StatusBytes
*/
uint8_t * RadioSpiReadFifo(uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint8_t *pStatus = (uint8_t *)&tmpstatus;
  uint8_t aHeader[2];
  uint8_t dummy=0xFF;
  
  tmpstatus = 0x0000;
  /* Built the aHeader bytes */
  aHeader[0]=READ_HEADER;
  aHeader[1]=LINEAR_FIFO_ADDRESS;
  
  SPI_ENTER_CRITICAL();
  
  /* Put the SPI chip select low to start the transaction */
  RadioSpiCSLow();
  
  for (volatile uint16_t Index = 0; Index < CS_TO_SCLK_DELAY; Index++);

  /* Write the aHeader bytes and read the SPIRIT1 status bytes */
  HAL_SPI_TransmitReceive(&pSpiHandle, (uint8_t *)&aHeader[0], (uint8_t *)&tmpstatus, 1, SpiTimeout);
  tmpstatus = tmpstatus<<8;  
  
    /* Write the aHeader bytes and read the SPIRIT1 status bytes */
  HAL_SPI_TransmitReceive(&pSpiHandle, (uint8_t *)&aHeader[1], (uint8_t *)&tmpstatus, 1, SpiTimeout);

  for (int index = 0; index < cNbBytes; index++)
  { 
    HAL_SPI_TransmitReceive(&pSpiHandle, (uint8_t *)&dummy, (uint8_t *)&pcBuffer[index], 1, SpiTimeout);
  } 
  
  /* To be sure to don't rise the Chip Select before the end of last sending */
  while(__HAL_SPI_GET_FLAG(&pSpiHandle, SPI_FLAG_TXE) == RESET);
    
  /* Put the SPI chip select high to end the transaction */
  RadioSpiCSHigh();
  
  SPI_EXIT_CRITICAL();
  
  return pStatus;  
}

/**
* @}
*/


/**
 * @defgroup SPSGR_IO_GPIO_Exported_Functions              SPSGR_GPIO Exported Functions
 * @{
 */
 

/**
* @brief  Configures MCU GPIO and EXTI Line for GPIOs.
* @param  xGpio Specifies the GPIO to be configured.
*         This parameter can be one of following parameters:
*         @arg GPIO_0
*         @arg GPIO_1
*         @arg GPIO_2
*         @arg GPIO_3
* @param  xGpioMode Specifies GPIO mode.
*         This parameter can be one of following parameters:
*         @arg RADIO_MODE_GPIO_IN: MCU GPIO will be used as simple input.
*         @argRADIO_MODE_GPIO_OUT: MCU GPIO will be used as simple output.
*         @arg RADIO_MODE_EXTI_IN: MCU GPIO will be connected to EXTI line with interrupt
*         generation capability.
* @retval None.
*/
void RadioGpioInit(RadioGpioPin xGpio, RadioGpioMode xGpioMode)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Check the parameters */
  assert_param(IS_RADIO_GPIO_PIN(xGpio));
  assert_param(IS_RADIO_GPIO_MODE(xGpioMode));
  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();  /* GPIO_3*/
  __HAL_RCC_GPIOB_CLK_ENABLE();  /* GPIO_SDN */
  
  /* Configures MCU GPIO */
  if (xGpioMode == RADIO_MODE_GPIO_OUT)
  {
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  }
  else
  {
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  }
  
  GPIO_InitStruct.Pin = aRADIO_GPIO_PIN[xGpio];
  GPIO_InitStruct.Pull = aRADIO_GPIO_PUPD[xGpio];
  GPIO_InitStruct.Speed = aRADIO_GPIO_SPEED[xGpio];
  HAL_GPIO_Init(aRADIO_GPIO_PORT[xGpio], &GPIO_InitStruct);
  
  if (xGpioMode == RADIO_MODE_EXTI_IN)
  {
    GPIO_InitStruct.Pin = aRADIO_GPIO_PIN[xGpio];
    GPIO_InitStruct.Pull = aRADIO_GPIO_PUPD[xGpio];
    GPIO_InitStruct.Speed = aRADIO_GPIO_SPEED[xGpio];
    GPIO_InitStruct.Mode = aRADIO_GPIO_EXTI_MODE[xGpio];
    HAL_GPIO_Init(aRADIO_GPIO_PORT[xGpio], &GPIO_InitStruct);
    
  /* Enable and set Button EXTI Interrupt to the lowest priority */
  /*  NVIC_SetPriority((IRQn_Type)(aRADIO_GPIO_IRQn[xGpio]), 0x02); */
  /*  HAL_NVIC_EnableIRQ((IRQn_Type)(aRADIO_GPIO_IRQn[xGpio]));     */
  } 
}


/**
* @brief  Enables or disables the interrupt on GPIO .
* @param  xGpio Specifies the GPIO whose priority shall be changed.
*         This parameter can be one of following parameters:
*         @arg GPIO_0
*         @arg GPIO_1
*         @arg GPIO_2
*         @arg GPIO_3
* @param  nPreemption Specifies Preemption Priority.
* @param  nSubpriority Specifies Subgroup Priority.
* @param  xNewState Specifies the State.
*         This parameter can be one of following parameters:
*         @arg ENABLE: Interrupt is enabled
*         @arg DISABLE: Interrupt is disabled
* @retval None.
*/
void RadioGpioInterruptCmd(RadioGpioPin xGpio, uint8_t nPreemption, uint8_t nSubpriority, FunctionalState xNewState)
{
  HAL_NVIC_SetPriority((IRQn_Type) (aRADIO_GPIO_IRQn[xGpio]), nPreemption, nSubpriority);
  if (!xNewState)
  {
       HAL_NVIC_DisableIRQ((IRQn_Type)(aRADIO_GPIO_IRQn[xGpio])); 
  }
  else
  {
        HAL_NVIC_EnableIRQ((IRQn_Type)(aRADIO_GPIO_IRQn[xGpio]));
  }
}


/**
* @brief  Returns the level of a specified GPIO.
* @param  xGpio Specifies the GPIO to be read.
*         This parameter can be one of following parameters:
*         @arg GPIO_0
*         @arg GPIO_1
*         @arg GPIO_2
*         @arg GPIO_3
* @retval FlagStatus Level of the GPIO. This parameter can be:
*         SET or RESET.
*/
FlagStatus RadioGpioGetLevel(RadioGpioPin xGpio)
{
  /* Gets the GPIO level */
  uint16_t nDataPort = HAL_GPIO_ReadPin(aRADIO_GPIO_PORT[xGpio], aRADIO_GPIO_PIN[xGpio]);
  if (nDataPort & aRADIO_GPIO_PIN[xGpio])
  {
    return SET;
  }
  else
  {
    return RESET; 
  }
}


/**
* @brief  Sets the level of a specified GPIO.
* @param  xGpio Specifies the GPIO to be set.
*         This parameter can be one of following parameters:
*         @arg GPIO_0
*         @arg GPIO_1
*         @arg GPIO_2
*         @arg GPIO_3
* @param  GPIO_PinState Level of the GPIO. This parameter can be:
*         GPIO_PIN_SET or GPIO_PIN_RESET.
* @retval None.
*/
void RadioGpioSetLevel(RadioGpioPin xGpio, GPIO_PinState xState)
{
  /* Sets the GPIO level */
  HAL_GPIO_WritePin(aRADIO_GPIO_PORT[xGpio], aRADIO_GPIO_PIN[xGpio], xState); 
}


/**
* @brief  Puts at logic 1 the SDN pin.
* @param  None.
* @retval None.
*/
void RadioEnterShutdown(void)
{
  /* Puts high the GPIO connected to shutdown pin */
  /* Check the parameters */ 
  RadioGpioSetLevel(RADIO_GPIO_SDN, GPIO_PIN_SET);
}


/**
* @brief  Put at logic 0 the SDN pin.
* @param  None.
* @retval None.
*/
void RadioExitShutdown(void)
{
  /* Puts low the GPIO connected to shutdown pin */
  RadioGpioSetLevel(RADIO_GPIO_SDN, GPIO_PIN_RESET);

  /* Delay to allow the circuit POR, about 700 us */
  for (volatile uint32_t Index = 0; Index < POR_TIME; Index++);
}


/**
* @brief  check the logic(0 or 1) at the SDN pin.
* @param  None.
* @retval FlagStatus.
*/
FlagStatus RadioCheckShutdown(void)
{
  return RadioGpioGetLevel(RADIO_GPIO_SDN);
}

/**
 * @}
 */

/**
* @}
*/

