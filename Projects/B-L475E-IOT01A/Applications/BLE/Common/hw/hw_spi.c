/**
 ******************************************************************************
 * @file    hw_spi.c
 * @author  MCD Application Team
 * @brief   BlueNRG MS Driver
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
#include "common.h"
#include "hw.h"

#include "lpm.h"

/* Private defines -----------------------------------------------------------*/
#undef TRUE
#undef FALSE
#define FALSE (0)
#define TRUE  !FALSE

#define BLUENRG_READY_STATE 0x02

#define HEADER_SIZE 5

#define CS_PULSE_LENGTH (CS_PULSE_625NS_NBR_CYCLES_REQ/4)
#define DUMMY_RAM_ADDRESS_TO_READ (0x20000000)

  /* SPI Private defines-----------------------------------------------------------*/
  // SPI Instance
#define BNRG_SPI_INSTANCE               SPI3
#define BNRG_SPI_CLK_ENABLE()           __HAL_RCC_SPI3_CLK_ENABLE()

  // SPI Configuration
#define BNRG_SPI_MODE                   SPI_MODE_MASTER
#define BNRG_SPI_DIRECTION              SPI_DIRECTION_2LINES
#define BNRG_SPI_DATASIZE               SPI_DATASIZE_8BIT
#define BNRG_SPI_CLKPOLARITY            SPI_POLARITY_LOW
#define BNRG_SPI_CLKPHASE               SPI_PHASE_1EDGE
#define BNRG_SPI_NSS                    SPI_NSS_SOFT
#define BNRG_SPI_FIRSTBIT               SPI_FIRSTBIT_MSB
#define BNRG_SPI_TIMODE                 SPI_TIMODE_DISABLE
#define BNRG_SPI_CRCPOLYNOMIAL          7
#define BNRG_SPI_BAUDRATEPRESCALER      SPI_BAUDRATEPRESCALER_2 /**< Keep clock running at 8Mhz */
#define BNRG_SPI_CRCCALCULATION         SPI_CRCCALCULATION_DISABLE

  // SPI Reset Pin: PA.8
#define BNRG_SPI_RESET_PIN              GPIO_PIN_8
#define BNRG_SPI_RESET_MODE             GPIO_MODE_OUTPUT_PP
#define BNRG_SPI_RESET_PULL             GPIO_NOPULL
#define BNRG_SPI_RESET_SPEED            GPIO_SPEED_FREQ_LOW
#define BNRG_SPI_RESET_ALTERNATE        0
#define BNRG_SPI_RESET_PORT             GPIOA
#define BNRG_SPI_RESET_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define BNRG_SPI_RESET_STDBY_PIN        LL_PWR_GPIO_BIT_8
#define BNRG_SPI_RESET_STDBY_PORT       LL_PWR_GPIO_A

  // SCLK: 
#define BNRG_SPI_SCLK_PIN               GPIO_PIN_10
#define BNRG_SPI_SCLK_MODE              GPIO_MODE_AF_PP
#define BNRG_SPI_SCLK_PULL              GPIO_NOPULL
#define BNRG_SPI_SCLK_SPEED             GPIO_SPEED_FREQ_VERY_HIGH
#define BNRG_SPI_SCLK_ALTERNATE         GPIO_AF6_SPI3
#define BNRG_SPI_SCLK_PORT              GPIOC
#define BNRG_SPI_SCLK_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

  // MISO (Master Input Slave Output): 
#define BNRG_SPI_MISO_PIN               GPIO_PIN_11
#define BNRG_SPI_MISO_MODE              GPIO_MODE_AF_PP
#define BNRG_SPI_MISO_PULL              GPIO_NOPULL
#define BNRG_SPI_MISO_SPEED             GPIO_SPEED_FREQ_VERY_HIGH
#define BNRG_SPI_MISO_ALTERNATE         GPIO_AF6_SPI3
#define BNRG_SPI_MISO_PORT              GPIOC
#define BNRG_SPI_MISO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

  // MOSI (Master Output Slave Input): 
#define BNRG_SPI_MOSI_PIN               GPIO_PIN_12
#define BNRG_SPI_MOSI_MODE              GPIO_MODE_AF_PP
#define BNRG_SPI_MOSI_PULL              GPIO_NOPULL
#define BNRG_SPI_MOSI_SPEED             GPIO_SPEED_FREQ_VERY_HIGH
#define BNRG_SPI_MOSI_ALTERNATE         GPIO_AF6_SPI3
#define BNRG_SPI_MOSI_PORT              GPIOC
#define BNRG_SPI_MOSI_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define BNRG_SPI_MOSI_STDBY_PIN         LL_PWR_GPIO_BIT_12
#define BNRG_SPI_MOSI_STDBY_PORT        LL_PWR_GPIO_C

  // NSS/CSN/CS: PA.1
#define BNRG_SPI_CS_PIN                 GPIO_PIN_13
#define BNRG_SPI_CS_MODE                GPIO_MODE_OUTPUT_PP
#define BNRG_SPI_CS_PULL                GPIO_NOPULL
#define BNRG_SPI_CS_SPEED               GPIO_SPEED_FREQ_VERY_HIGH
#define BNRG_SPI_CS_ALTERNATE           0
#define BNRG_SPI_CS_PORT                GPIOD
#define BNRG_SPI_CS_CLK_ENABLE()        __HAL_RCC_GPIOD_CLK_ENABLE()
#define BNRG_SPI_CS_STDBY_PIN           LL_PWR_GPIO_BIT_13
#define BNRG_SPI_CS_STDBY_PORT          LL_PWR_GPIO_D

/* IRQ PIN and PORT defined in the hw.h */
#define BNRG_SPI_IRQ_MODE               GPIO_MODE_IT_RISING
#define BNRG_SPI_IRQ_PULL               GPIO_PULLDOWN
#define BNRG_SPI_IRQ_SPEED              GPIO_SPEED_FREQ_VERY_HIGH
#define BNRG_SPI_IRQ_ALTERNATE          0
#define BNRG_SPI_IRQ_CLK_ENABLE()       __HAL_RCC_GPIOE_CLK_ENABLE()

/* EXTI PIN and PORT and Handler defined in the hw.h */
#define BNRG_SPI_EXTI_NVIC_PREEMPT_PRIO 2
#define BNRG_SPI_EXTI_NVIC_SUB_PRIO     0 

#define BNRG_SPI_FORCE_RESET()          __SPI3_FORCE_RESET()
#define BNRG_SPI_RELEASE_RESET()        __SPI3_RELEASE_RESET()

#define BNRG_DMA_CLK_ENABLE()           __HAL_RCC_DMA2_CLK_ENABLE()
  /* Definition for SPIx's DMA */
#define BNRG_SPI_TX_DMA_ID                DMA2
#define BNRG_SPI_TX_DMA_CHANNEL           LL_DMA_CHANNEL_2
#define BNRG_SPI_TX_DMA_CHANNEL_INSTANCE  DMA2_Channel2
#define BNRG_SPI_TX_DMA_REQUEST           DMA_REQUEST_3
#define BNRG_SPI_TX_DMA_TC_FLAG           DMA_FLAG_TC2
#define BNRG_SPI_TX_DMA_IRQHandler        DMA2_Channel2_IRQHandler
#define BNRG_SPI_TX_DMA_NVIC_PREEMPT_PRIO 1
#define BNRG_SPI_TX_DMA_NVIC_SUB_PRIO     0

#define BNRG_SPI_RX_DMA_ID                DMA2
#define BNRG_SPI_RX_DMA_CHANNEL           LL_DMA_CHANNEL_1
#define BNRG_SPI_RX_DMA_CHANNEL_INSTANCE  DMA2_Channel1
#define BNRG_SPI_RX_DMA_REQUEST           DMA_REQUEST_3
#define BNRG_SPI_RX_DMA_TC_FLAG           DMA_FLAG_TC1
#define BNRG_SPI_RX_DMA_IRQHandler        DMA2_Channel1_IRQHandler
#define BNRG_SPI_RX_DMA_NVIC_PREEMPT_PRIO 1
#define BNRG_SPI_RX_DMA_NVIC_SUB_PRIO     0

  /* Definition for SPIx's NVIC */
#define BNRG_SPI_DMA_TX_IRQn            DMA2_Channel2_IRQn
#define BNRG_SPI_DMA_RX_IRQn            DMA2_Channel1_IRQn


/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  SPI_REQUEST_VALID_HEADER_FOR_RX,
  SPI_REQUEST_VALID_HEADER_FOR_TX,
  SPI_REQUEST_PAYLOAD
} SPI_RECEIVE_REQUEST_t;

typedef enum
{
  SPI_CHECK_RECEIVED_HEADER_FOR_RX,
  SPI_CHECK_RECEIVED_HEADER_FOR_TX,
  SPI_RECEIVE_PAYLOAD_CONT,
  SPI_RECEIVE_END
} SPI_RECEIVE_EVENT_t;

typedef enum
{
  SPI_AVAILABLE,
  SPI_BUSY
} SPI_PERIPHERAL_STATUS_t;

typedef enum
{
  BUFFER_AVAILABLE,
  NO_BUFFER
} EVENT_BUFFER_STATUS_t;
typedef struct
{
  uint8_t* pPayloadData;
  uint16_t PayloadSizeLeftToTransmit;
  uint16_t PayloadSizeToTransmit;
  uint8_t PacketCont;
  uint8_t RequestPending;
} SPI_Transmit_Context_t;

typedef struct
{
  SPI_RECEIVE_EVENT_t SpiReceiveEvent;
  uint16_t PayloadLength;
  uint8_t* pBuffer;
  uint16_t BufferSize;
  uint16_t PayloadSizeLeftToReceive;
  uint8_t PacketCont;
  EVENT_BUFFER_STATUS_t     BufferStatus;
} SPI_Receive_Context_t;

typedef struct
{
  SPI_PERIPHERAL_STATUS_t SpiPeripheralState;
  SPI_Receive_Context_t SpiReceiveContext;
  SPI_Transmit_Context_t SpiTransmitContext;
} SPI_Context_t;



/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
 * START of Section BLE_SPI_CONTEXT
 */
static uint8_t TxRxTimerId;
/**
 * END of Section BLE_SPI_CONTEXT
 */

static SPI_Context_t SpiContext;
static HW_TS_pTimerCb_t pTimerTxRxCallback;
static volatile uint8_t RfResetTimerLock;
static uint8_t ReceivedHeader[HEADER_SIZE];
static uint32_t TimeoutTicks;

static const uint8_t WriteHeaderCmd[HEADER_SIZE] = {0x0a, 0x00, 0x00, 0x00, 0x00};
static const uint8_t ReadHeaderCmd[HEADER_SIZE] = {0x0b, 0x00, 0x00, 0x00, 0x00};
static const uint8_t DummyBytes = 0xFF;
static SPI_HandleTypeDef SpiHandle;

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SPI_Transmit_Manager(void);
static void SPI_Receive_Manager(SPI_RECEIVE_REQUEST_t ReceiveRequest);
static void Disable_SPI_Receiving_Path(void);
static void Enable_SPI_Receiving_Path(void);
static void Enable_SPI_CS(void);
static void Disable_SPI_CS(void);
static void DisableEnable_SPI_CS(void);
static void TransmitClosure(void);
static void ReceiveClosure(void);
static void ReceiveHeader(SPI_RECEIVE_EVENT_t ReceiveEvent, uint8_t * DataHeader);
static void WakeupBlueNRG(void);
static void TimerTransmitCallback(void);
static void pf_nRFResetTimerCallBack(void);
static void TimerTxRxCallback(void);
static void ProcessEndOfReceive(void);
static void SPI_Init(SPI_HandleTypeDef * hspi);
static void ResetBlueNRG(void);

/* Private Functions Definition -----------------------------------------------*/

/**
 * @brief  This function notify when then BlueNRG nRESET may be released
 * @param  None
 * @retval None
 */
static void pf_nRFResetTimerCallBack(void)
{
  RfResetTimerLock = 0;

  return;
}

/**
 * @brief  Timer callback to handle RxTx Timers
 * @param  None
 * @retval None
 */
static void TimerTxRxCallback(void)
{
  pTimerTxRxCallback();

  return;
}

/**
 * @brief  Close the receiver path
 * @param  None
 * @retval None
 */
static void ReceiveClosure(void)
{
  /*
   *  Disable both DMA
   */
  __HAL_DMA_DISABLE(SpiHandle.hdmatx);
  __HAL_DMA_DISABLE(SpiHandle.hdmarx);

  /*
   * Check if a command is pending
   */
  __disable_irq();
  Disable_SPI_Receiving_Path();
  if(SpiContext.SpiTransmitContext.RequestPending == TRUE)
  {
    SpiContext.SpiTransmitContext.RequestPending = FALSE;
    SpiContext.SpiPeripheralState = SPI_BUSY;
    __enable_irq();
    WakeupBlueNRG();
  }
  else
  {
    SpiContext.SpiPeripheralState = SPI_AVAILABLE;
    __enable_irq();
  }

  return;
}

/**
 * @brief  Delay Notification to the App to prevent dummy event read
 * @param  None
 * @retval None
 */
static void ProcessEndOfReceive(void)
{
  SpiContext.SpiReceiveContext.BufferStatus = NO_BUFFER;
  ReceiveClosure();

  HW_BNRG_Isr(SpiContext.SpiReceiveContext.PayloadLength);

  return;
}

/**
 * @brief  Timer callback to apply timeout SPI FIX
 * @param  None
 * @retval None
 */
static void TimerTransmitCallback(void)
{
  SPI_Receive_Manager(SPI_REQUEST_VALID_HEADER_FOR_TX);  /**< BlueNRG not ready for writing */
  LPM_SetStopMode(LPM_SPI_TX_Id, LPM_StopMode_En);

  return;
}



/**
 * @brief  This function is used for low level initialization of the SPI 
 *         communication with the BlueNRG Expansion Board.
 * @param  Pointer to the handle of the STM32Cube HAL SPI interface.
 * @retval None
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  uint32_t peripheral_address;
  static DMA_HandleTypeDef hdma_tx;
  static DMA_HandleTypeDef hdma_rx;

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hspi->Instance==BNRG_SPI_INSTANCE)
  {
    /* Enable peripherals clock */

    /* Enable GPIO Ports Clock */  
    BNRG_SPI_SCLK_CLK_ENABLE();
    BNRG_SPI_MISO_CLK_ENABLE();
    BNRG_SPI_MOSI_CLK_ENABLE();
    BNRG_SPI_CS_CLK_ENABLE();
    BNRG_SPI_IRQ_CLK_ENABLE();

    /* Enable SPI clock */
    BNRG_SPI_CLK_ENABLE();

    /* SCLK */
    GPIO_InitStruct.Pin = BNRG_SPI_SCLK_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_SCLK_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_SCLK_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_SCLK_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_SCLK_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_SCLK_PORT, &GPIO_InitStruct); 

    /* MISO */
    GPIO_InitStruct.Pin = BNRG_SPI_MISO_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_MISO_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_MISO_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_MISO_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_MISO_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_MISO_PORT, &GPIO_InitStruct);

    /* MOSI */
    GPIO_InitStruct.Pin = BNRG_SPI_MOSI_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_MOSI_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_MOSI_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_MOSI_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_MOSI_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_MOSI_PORT, &GPIO_InitStruct);

    /* NSS/CSN/CS */
    HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = BNRG_SPI_CS_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_CS_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_CS_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_CS_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_CS_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_CS_PORT, &GPIO_InitStruct);

    /* IRQ -- INPUT */
    GPIO_InitStruct.Pin = BNRG_SPI_IRQ_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_IRQ_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_IRQ_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_IRQ_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_IRQ_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_IRQ_PORT, &GPIO_InitStruct);

    /*##-3- Configure the DMA channel ##########################################*/ 

    /* Enable DMA1 clock */
    BNRG_DMA_CLK_ENABLE();   

    /* Configure the DMA handler for Transmission process */
    hdma_tx.Init.Request             = BNRG_SPI_TX_DMA_REQUEST;
    hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_tx.Init.Mode                = DMA_NORMAL;
    hdma_tx.Init.Priority            = DMA_PRIORITY_HIGH;
    hdma_tx.Instance                 = BNRG_SPI_TX_DMA_CHANNEL_INSTANCE;

    HAL_DMA_Init(&hdma_tx);   
    peripheral_address = LL_SPI_DMA_GetRegAddr(BNRG_SPI_INSTANCE);
    LL_DMA_SetPeriphAddress(BNRG_SPI_TX_DMA_ID, BNRG_SPI_TX_DMA_CHANNEL, peripheral_address);

    /* Associate the initialized DMA handle to the SPI handle */
    __HAL_LINKDMA(hspi, hdmatx, hdma_tx);

    /* Configure the DMA handler for Transmission process */
    hdma_rx.Init.Request             = BNRG_SPI_RX_DMA_REQUEST;
    hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_rx.Init.Mode                = DMA_NORMAL;
    hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
    hdma_rx.Instance                 = BNRG_SPI_RX_DMA_CHANNEL_INSTANCE;

    HAL_DMA_Init(&hdma_rx);
    peripheral_address = LL_SPI_DMA_GetRegAddr(BNRG_SPI_INSTANCE);
    LL_DMA_SetPeriphAddress(BNRG_SPI_RX_DMA_ID, BNRG_SPI_RX_DMA_CHANNEL, peripheral_address);

    /* Associate the initialized DMA handle to the SPI handle */
    __HAL_LINKDMA(hspi, hdmarx, hdma_rx); 

    /* Configure the NVIC for SPI */  
    HAL_NVIC_SetPriority(BNRG_SPI_DMA_TX_IRQn, 
                         BNRG_SPI_TX_DMA_NVIC_PREEMPT_PRIO, 
                         BNRG_SPI_TX_DMA_NVIC_SUB_PRIO);
    HAL_NVIC_EnableIRQ(BNRG_SPI_DMA_TX_IRQn);

    HAL_NVIC_SetPriority(BNRG_SPI_DMA_RX_IRQn, 
                         BNRG_SPI_RX_DMA_NVIC_PREEMPT_PRIO, 
                         BNRG_SPI_RX_DMA_NVIC_SUB_PRIO);
    HAL_NVIC_EnableIRQ(BNRG_SPI_DMA_RX_IRQn);

    /* Configure the NVIC for SPI */  
    HAL_NVIC_SetPriority(BNRG_SPI_EXTI_IRQn, 
                         BNRG_SPI_EXTI_NVIC_PREEMPT_PRIO, 
                         BNRG_SPI_EXTI_NVIC_SUB_PRIO);    
  }

  return;
}


/**
 * @brief  Initializes the SPI communication with the BlueNRG Shield
 * @param  hspi: SPI Handle
 * @retval None
 */
void SPI_Init(SPI_HandleTypeDef * hspi)
{
  hspi->Instance = BNRG_SPI_INSTANCE;
  hspi->Init.Mode = BNRG_SPI_MODE;
  hspi->Init.Direction = BNRG_SPI_DIRECTION;
  hspi->Init.DataSize = BNRG_SPI_DATASIZE;
  hspi->Init.CLKPolarity = BNRG_SPI_CLKPOLARITY;
  hspi->Init.CLKPhase = BNRG_SPI_CLKPHASE;
  hspi->Init.NSS = BNRG_SPI_NSS;
  hspi->Init.FirstBit = BNRG_SPI_FIRSTBIT;
  hspi->Init.TIMode = BNRG_SPI_TIMODE;
  hspi->Init.CRCPolynomial = BNRG_SPI_CRCPOLYNOMIAL;
  hspi->Init.BaudRatePrescaler = BNRG_SPI_BAUDRATEPRESCALER;
  hspi->Init.CRCCalculation = BNRG_SPI_CRCCALCULATION;

  HAL_SPI_Init(hspi);

  return;
}

/**
 * @brief  Resets the BlueNRG.
 * @param  None
 * @retval None
 */
void ResetBlueNRG(void)
{
  uint8_t ubnRFResetTimerID;

  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = BNRG_SPI_RESET_PIN;
  GPIO_InitStruct.Speed = BNRG_SPI_RESET_SPEED;
  GPIO_InitStruct.Mode = BNRG_SPI_RESET_MODE;
  GPIO_InitStruct.Pull = BNRG_SPI_RESET_PULL;
  GPIO_InitStruct.Alternate = BNRG_SPI_RESET_ALTERNATE;
  HW_TS_Create(CFG_TimProcID_isr, &ubnRFResetTimerID, hw_ts_SingleShot, pf_nRFResetTimerCallBack);

  BNRG_SPI_RESET_CLK_ENABLE();

  HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_RESET);
  HAL_GPIO_Init(BNRG_SPI_RESET_PORT, &GPIO_InitStruct);

  HW_TS_Start(ubnRFResetTimerID, BLUENRG_HOLD_TIME_IN_RESET);
  RfResetTimerLock = 1;
  while(RfResetTimerLock == 1)
  {
    HW_SleepMode();
  }

  HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_SET);

  HW_TS_Delete(ubnRFResetTimerID);

  return;
}


/**
 * @brief  Enable SPI IRQ.
 * @param  None
 * @retval None
 */
static void Enable_SPI_Receiving_Path(void)
{  
  __HAL_GPIO_EXTI_CLEAR_IT(BNRG_SPI_EXTI_PIN);
  HAL_NVIC_ClearPendingIRQ(BNRG_SPI_EXTI_IRQn);
  HAL_NVIC_EnableIRQ(BNRG_SPI_EXTI_IRQn);

  if (HAL_GPIO_ReadPin(BNRG_SPI_IRQ_PORT, BNRG_SPI_IRQ_PIN) == GPIO_PIN_SET)
  {
    __HAL_GPIO_EXTI_GENERATE_SWIT(BNRG_SPI_IRQ_PIN);
  }
}

/**
 * @brief  Disable SPI IRQ.
 * @param  None
 * @retval None
 */
static void Disable_SPI_Receiving_Path(void)
{  
  HAL_NVIC_DisableIRQ(BNRG_SPI_EXTI_IRQn);
}

/**
 * @brief  Enable SPI CS.
 * @param  None
 * @retval None
 */
static void Enable_SPI_CS(void)
{
  /* CS reset */
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_RESET);
}

/**
 * @brief  Disable SPI CS.
 * @param  None
 * @retval None
 */
static void Disable_SPI_CS(void)
{
  while (__HAL_SPI_GET_FLAG(&SpiHandle,SPI_FLAG_BSY) == SET);

  /* CS set */
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_SET);
}

/**
 * @brief  Disable and Enable SPI CS.
 * @param  None
 * @retval None
 */
static void DisableEnable_SPI_CS(void)
{
  uint8_t localloop;
  Disable_SPI_CS(); /**< CS Set */

  /* CS set */
  for (localloop = 0 ; localloop < CS_PULSE_LENGTH ; localloop++)
  {
    *(volatile uint32_t*)DUMMY_RAM_ADDRESS_TO_READ;
  }

  Enable_SPI_CS(); /**< CS Reset */

  return;
}



/**
 * @brief  Wakeup BlueNRG
 * @param  None
 * @retval None
 */
static void WakeupBlueNRG(void)
{
  pTimerTxRxCallback = TimerTransmitCallback;
  Enable_SPI_CS();
  TimeoutTicks = SPI_TX_TIMEOUT;
  HW_BNRG_TimStartReq();
  LPM_SetStopMode(LPM_SPI_TX_Id, LPM_StopMode_Dis);

  return;
}



/**
 * @brief  Close the transmit mechanism after packet has been sent
 *         Wait for the event to come back
 * @param  None
 * @retval None
 */
static void TransmitClosure(void)
{ 
  LPM_SetStopMode(LPM_SPI_TX_Id, LPM_StopMode_Dis);
  SpiContext.SpiPeripheralState = SPI_AVAILABLE;
  Disable_SPI_CS();
  /*
   *  Disable both DMA
   */
  __HAL_DMA_DISABLE(SpiHandle.hdmatx);
  __HAL_DMA_DISABLE(SpiHandle.hdmarx);
  if(SpiContext.SpiReceiveContext.BufferStatus == BUFFER_AVAILABLE)
  {
    Enable_SPI_Receiving_Path();
  }

  return;
}

/**
 * @brief  Manage the SPI transmit
 * @param  None
 * @retval None
 */
static void SPI_Transmit_Manager(void)
{
  /*
   *  Disable both DMA
   */
  __HAL_DMA_DISABLE(SpiHandle.hdmatx);
  __HAL_DMA_DISABLE(SpiHandle.hdmarx);

  __HAL_DMA_DISABLE_IT(SpiHandle.hdmarx, DMA_IT_TC); /**< Disable Receive packet notification */

  __HAL_DMA_CLEAR_FLAG(SpiHandle.hdmatx, BNRG_SPI_TX_DMA_TC_FLAG); /**< Clear flag in DMA */
  HAL_NVIC_ClearPendingIRQ(BNRG_SPI_DMA_TX_IRQn); /**< Clear DMA pending bit in NVIC */
  __HAL_DMA_ENABLE_IT(SpiHandle.hdmatx, DMA_IT_TC);  /**< Enable Transmit packet notification */

  LL_DMA_SetMemoryIncMode(BNRG_SPI_TX_DMA_ID, BNRG_SPI_TX_DMA_CHANNEL, LL_DMA_MEMORY_INCREMENT); /**< Configure DMA to send Tx packet */

  LL_DMA_SetDataLength(BNRG_SPI_TX_DMA_ID, BNRG_SPI_TX_DMA_CHANNEL, SpiContext.SpiTransmitContext.PayloadSizeToTransmit); /**< Set counter in DMA TX */

  LL_DMA_SetMemoryAddress(BNRG_SPI_TX_DMA_ID, BNRG_SPI_TX_DMA_CHANNEL, (uint32_t)SpiContext.SpiTransmitContext.pPayloadData);  /**< Set memory address in DMA TX */

  __HAL_DMA_ENABLE(SpiHandle.hdmatx); /**< Enable DMA TX */

}

/**
 * @brief  Manage the SPI receive
 * @param  ReceiveRequest: the receive request
 * @retval None
 */
static void SPI_Receive_Manager(SPI_RECEIVE_REQUEST_t ReceiveRequest)
{
  uint16_t byte_count;
  uint8_t localloop;

  /*
   *  Disable both DMA
   */
  __HAL_DMA_DISABLE(SpiHandle.hdmatx);
  __HAL_DMA_DISABLE(SpiHandle.hdmarx);

  /**
   * Flush the Rx register or FIFO
   */
  for (localloop = 0 ; localloop < SPI_FIFO_RX_DEPTH ; localloop++)
  {
    *(volatile uint8_t*)LL_SPI_DMA_GetRegAddr(BNRG_SPI_INSTANCE);
  }

  __HAL_DMA_ENABLE_IT(SpiHandle.hdmarx, DMA_IT_TC);  /**< Enable Receive packet notification */
  __HAL_DMA_DISABLE_IT(SpiHandle.hdmatx, DMA_IT_TC); /**< Disable Transmit packet notification */

  switch (ReceiveRequest)
  {
    case SPI_REQUEST_VALID_HEADER_FOR_RX:
      ReceiveHeader(SPI_CHECK_RECEIVED_HEADER_FOR_RX, (uint8_t *)ReadHeaderCmd);
      break;

    case SPI_REQUEST_VALID_HEADER_FOR_TX:
      ReceiveHeader(SPI_CHECK_RECEIVED_HEADER_FOR_TX, (uint8_t *)WriteHeaderCmd);
      break;

    case SPI_REQUEST_PAYLOAD:
      if (SpiContext.SpiReceiveContext.PacketCont == TRUE)
      {
        byte_count = SpiContext.SpiReceiveContext.PayloadSizeLeftToReceive;
      }
      else
      {
        byte_count = (ReceivedHeader[4]<<8)|ReceivedHeader[3];
        SpiContext.SpiReceiveContext.PayloadSizeLeftToReceive = byte_count;
      }

      if (byte_count > SpiContext.SpiReceiveContext.BufferSize)
      {
        byte_count = SpiContext.SpiReceiveContext.BufferSize;
        SpiContext.SpiReceiveContext.PayloadSizeLeftToReceive -= byte_count;
        SpiContext.SpiReceiveContext.SpiReceiveEvent = SPI_RECEIVE_PAYLOAD_CONT;
        SpiContext.SpiReceiveContext.PacketCont = TRUE;
      }
      else
      {
        SpiContext.SpiReceiveContext.SpiReceiveEvent = SPI_RECEIVE_END;
        SpiContext.SpiReceiveContext.PacketCont = FALSE;
      }

      SpiContext.SpiReceiveContext.PayloadLength = byte_count;

      LL_DMA_SetMemoryIncMode(BNRG_SPI_TX_DMA_ID, BNRG_SPI_TX_DMA_CHANNEL, LL_DMA_MEMORY_NOINCREMENT); /**< Configure DMA to send same Byte */

      /*
       *  Set counter in both DMA
       */
      LL_DMA_SetDataLength(BNRG_SPI_RX_DMA_ID, BNRG_SPI_RX_DMA_CHANNEL, byte_count);
      LL_DMA_SetDataLength(BNRG_SPI_TX_DMA_ID, BNRG_SPI_TX_DMA_CHANNEL, byte_count);

      /*
       *  Set memory address in both DMA
       */
      LL_DMA_SetMemoryAddress(BNRG_SPI_RX_DMA_ID, BNRG_SPI_RX_DMA_CHANNEL, (uint32_t)SpiContext.SpiReceiveContext.pBuffer);
      LL_DMA_SetMemoryAddress(BNRG_SPI_TX_DMA_ID, BNRG_SPI_TX_DMA_CHANNEL, (uint32_t)&DummyBytes);
      break;

    default:
      break;
  }

  /*
   *  Enable both DMA - Rx First
   */
  __HAL_DMA_ENABLE(SpiHandle.hdmarx);
  __HAL_DMA_ENABLE(SpiHandle.hdmatx);

  return;
}

/**
 * @brief Receive header
 * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 * @param  DataHeader: buffer holding the header to receive
 * @retval None
 */
static void ReceiveHeader(SPI_RECEIVE_EVENT_t ReceiveEvent, uint8_t * DataHeader)
{
  SpiContext.SpiReceiveContext.SpiReceiveEvent = ReceiveEvent;

  LL_DMA_SetMemoryIncMode(BNRG_SPI_TX_DMA_ID, BNRG_SPI_TX_DMA_CHANNEL, LL_DMA_MEMORY_INCREMENT);  /**< Configure DMA to send Tx packet */

  /*
   *  Set counter in both DMA
   */
  LL_DMA_SetDataLength(BNRG_SPI_TX_DMA_ID, BNRG_SPI_TX_DMA_CHANNEL, HEADER_SIZE);
  LL_DMA_SetDataLength(BNRG_SPI_RX_DMA_ID, BNRG_SPI_RX_DMA_CHANNEL, HEADER_SIZE);

  /*
   *  Set memory address in both DMA
   */
  LL_DMA_SetMemoryAddress(BNRG_SPI_RX_DMA_ID, BNRG_SPI_RX_DMA_CHANNEL, (uint32_t)ReceivedHeader);
  LL_DMA_SetMemoryAddress(BNRG_SPI_TX_DMA_ID, BNRG_SPI_TX_DMA_CHANNEL, (uint32_t)DataHeader);

  return;
}


/* Exported Functions Definition -----------------------------------------------*/


void HW_BNRG_Close(void)
{
  HW_TS_Delete(TxRxTimerId);

  return;
}


void HW_BNRG_Init(HW_BNRG_InitMode_t InitMode)
{
  SPI_Init(&SpiHandle);

  LL_SPI_EnableDMAReq_TX(BNRG_SPI_INSTANCE);
  LL_SPI_EnableDMAReq_RX(BNRG_SPI_INSTANCE);

  __HAL_SPI_ENABLE(&SpiHandle);

  SpiContext.SpiReceiveContext.PacketCont = FALSE;
  SpiContext.SpiTransmitContext.RequestPending = FALSE;
  SpiContext.SpiPeripheralState = SPI_AVAILABLE;
  SpiContext.SpiReceiveContext.BufferStatus = NO_BUFFER;

  if(InitMode != HW_BNRG_Limited)
  {
    LL_PWR_SetWakeUpPinPolarityHigh(BNRG_SPI_IRQ_WAKEUP_PIN);

    HW_TS_Create(CFG_TimProcID_isr, &TxRxTimerId, hw_ts_SingleShot, TimerTxRxCallback);

    /**
     *  Reset BlueNRG hardware
     */
    ResetBlueNRG();
  }
  else
  {
    LL_PWR_DisableGPIOPullDown(BNRG_SPI_MOSI_STDBY_PORT, BNRG_SPI_MOSI_STDBY_PIN);
    LL_PWR_DisableGPIOPullUp(BNRG_SPI_CS_STDBY_PORT, BNRG_SPI_CS_STDBY_PIN);
    LL_PWR_DisableGPIOPullUp(BNRG_SPI_RESET_STDBY_PORT, BNRG_SPI_RESET_STDBY_PIN);
  }

  return;
}


void HW_BNRG_Send(uint8_t* payload_data, uint16_t payload_size)
{  
  SpiContext.SpiTransmitContext.pPayloadData = payload_data;
  SpiContext.SpiTransmitContext.PayloadSizeLeftToTransmit = payload_size;

  __disable_irq();
  if(SpiContext.SpiPeripheralState == SPI_AVAILABLE)
  {
    SpiContext.SpiPeripheralState = SPI_BUSY;
    Disable_SPI_Receiving_Path();
    __enable_irq();
    WakeupBlueNRG();
  }
  else
  {
    SpiContext.SpiTransmitContext.RequestPending = TRUE;
    __enable_irq();
  }

  return;
}


void HW_BNRG_EvtReq(uint8_t *buffer, uint16_t buff_size)
{
  SpiContext.SpiReceiveContext.pBuffer = buffer;
  SpiContext.SpiReceiveContext.BufferSize = buff_size;
  SpiContext.SpiReceiveContext.BufferStatus = BUFFER_AVAILABLE;

  if( SpiContext.SpiReceiveContext.PacketCont == TRUE )
  {
    SPI_Receive_Manager(SPI_REQUEST_PAYLOAD);
  }
  else
  {
    Enable_SPI_Receiving_Path();
  }

  return;
}


void HW_BNRG_SpiIrqCb(void)
{  
  __disable_irq();
  if(SpiContext.SpiPeripheralState == SPI_AVAILABLE)
  {
    SpiContext.SpiPeripheralState = SPI_BUSY;
    __enable_irq();
    Enable_SPI_CS();
    SPI_Receive_Manager(SPI_REQUEST_VALID_HEADER_FOR_RX);
    LPM_SetStopMode(LPM_SPI_RX_Id, LPM_StopMode_En);
  }
  else
  {
    __enable_irq();
  }
}


void HW_BNRG_DMATxCb(void)
{
  if(__HAL_DMA_GET_IT_SOURCE(SpiHandle.hdmatx, DMA_IT_TC)  && __HAL_DMA_GET_FLAG(SpiHandle.hdmatx, BNRG_SPI_TX_DMA_TC_FLAG))
  {
    __HAL_DMA_CLEAR_FLAG(SpiHandle.hdmatx, BNRG_SPI_TX_DMA_TC_FLAG);

    if(SpiContext.SpiTransmitContext.PacketCont == TRUE)
    {
      SpiContext.SpiTransmitContext.pPayloadData += SpiContext.SpiTransmitContext.PayloadSizeToTransmit;
      DisableEnable_SPI_CS();
      SPI_Receive_Manager(SPI_REQUEST_VALID_HEADER_FOR_TX);
    }
    else
    {
      TransmitClosure();
    }
  }

  return;
}


void HW_BNRG_DMARxCb(void)
{
  uint16_t byte_count;
  uint8_t ready_state;

  if(__HAL_DMA_GET_IT_SOURCE(SpiHandle.hdmarx, DMA_IT_TC)  && __HAL_DMA_GET_FLAG(SpiHandle.hdmarx, BNRG_SPI_RX_DMA_TC_FLAG))
  {
    __HAL_DMA_CLEAR_FLAG(SpiHandle.hdmarx, BNRG_SPI_RX_DMA_TC_FLAG);

    /**
     * The TCIF shall be cleared to be able to start a new DMA transmission later on when required.
     * When receiving data, the TCIE is not set as there is no need to handle the interrupt
     * handler of the DMA_Tx.
     * The TCIF clearing is mandatory on STM32F4 but not on STM32L0.
     * In order to keep code identical across platform, the TCIF clearing may be kept as well on
     * the STM32L0 and all other MCUs.
     */
    __HAL_DMA_CLEAR_FLAG(SpiHandle.hdmatx, BNRG_SPI_TX_DMA_TC_FLAG);
    switch (SpiContext.SpiReceiveContext.SpiReceiveEvent)
    {
      case SPI_CHECK_RECEIVED_HEADER_FOR_RX:
        byte_count = (ReceivedHeader[4]<<8)|ReceivedHeader[3];
        ready_state = ReceivedHeader[0];

        if ((byte_count == 0) || (ready_state != BLUENRG_READY_STATE))
        {
          if (HAL_GPIO_ReadPin(BNRG_SPI_IRQ_PORT, BNRG_SPI_IRQ_PIN) == GPIO_PIN_RESET)
          {
            /**
             * This USE CASE shall never happen as this may break the IRQ/CS specification
             * The IRQ line shall never be low when CS is low to avoid BlueNRG race condition when
             * entering low power mode
             * the SPI_END_RECEIVE_FIX has been implemented to make sure this USE CASE never occurs
             * However, even when the behavior is not compliant to the specification, the BlueNRG
             * may not fail so it is increasing robustness by adding this checking just in case the
             * timeout define in the workaround is too short which will end up to marginally brake
             * the specification.
             * This checking will popping BluenRG for a dummy even
             */

            /* Release CS line */
            Disable_SPI_CS();

            LPM_SetStopMode(LPM_SPI_RX_Id, LPM_StopMode_Dis);

            ReceiveClosure();
          }
          else
          {
            DisableEnable_SPI_CS();
            SPI_Receive_Manager(SPI_REQUEST_VALID_HEADER_FOR_RX); /**< BlueNRG not ready for reading */
          }
        }
        else
        {
          SPI_Receive_Manager(SPI_REQUEST_PAYLOAD);  /**< BlueNRG is ready for reading */
        }
        break;

      case SPI_RECEIVE_PAYLOAD_CONT:
        HW_BNRG_Isr(SpiContext.SpiReceiveContext.PayloadLength);
        break;

      case SPI_RECEIVE_END:
        /* Release CS line */
        Disable_SPI_CS();

        LPM_SetStopMode(LPM_SPI_RX_Id, LPM_StopMode_Dis);


#if (SPI_END_RECEIVE_FIX == 1)
        pTimerTxRxCallback = ProcessEndOfReceive;
        TimeoutTicks = SPI_END_RECEIVE_FIX_TIMEOUT;
        HW_BNRG_TimStartReq();
#else
        ProcessEndOfReceive();
#endif
        break;

      case SPI_CHECK_RECEIVED_HEADER_FOR_TX:
        byte_count = (ReceivedHeader[2]<<8)|ReceivedHeader[1];
        ready_state = ReceivedHeader[0];

        if ((byte_count == 0) || (ready_state != BLUENRG_READY_STATE))
        {
          DisableEnable_SPI_CS();
          SPI_Receive_Manager(SPI_REQUEST_VALID_HEADER_FOR_TX);  /**< BlueNRG not ready for writing */
        }
        else
        {
          if( byte_count < SpiContext.SpiTransmitContext.PayloadSizeLeftToTransmit)
          {
            SpiContext.SpiTransmitContext.PayloadSizeToTransmit = byte_count;
            SpiContext.SpiTransmitContext.PayloadSizeLeftToTransmit -= SpiContext.SpiTransmitContext.PayloadSizeToTransmit;
            SpiContext.SpiTransmitContext.PacketCont = TRUE;
          }
          else
          {
            SpiContext.SpiTransmitContext.PayloadSizeToTransmit = SpiContext.SpiTransmitContext.PayloadSizeLeftToTransmit;
            SpiContext.SpiTransmitContext.PacketCont = FALSE;
          }

          SPI_Transmit_Manager();
        }

        break;

      default:
        break;
    }
  }

  return;
}


void HW_BNRG_SetupStdbyMode(void)
{
  /**
   * Configure IO in StdByMode
   */
  LL_PWR_EnableGPIOPullDown(BNRG_SPI_MOSI_STDBY_PORT, BNRG_SPI_MOSI_STDBY_PIN);
  LL_PWR_EnableGPIOPullUp(BNRG_SPI_CS_STDBY_PORT, BNRG_SPI_CS_STDBY_PIN);
  LL_PWR_EnableGPIOPullUp(BNRG_SPI_RESET_STDBY_PORT, BNRG_SPI_RESET_STDBY_PIN);

  return;
}


void HW_BNRG_TimStartAllowed(void)
{
  HW_TS_Start(TxRxTimerId, TimeoutTicks);

  return;
}

/* __WEAK functions: The application can overload this function  if needed ----*/
/* ----------------------------------------------------------------------------*/

/**
   * @brief  Request a timer to be started
   * @param  None
   * @retval None
   * @Note   If the specific application need to wait before starting the timer 
   *         this function can be overloaded in the specific application itself 
   */
__weak void HW_BNRG_TimStartReq(void)
{
  /**
   * In this file (part of the COMMON code) it is assumed that the TimerServer  
   * implements critical section by default (CFG_HW_TS_USE_PRIMASK_AS_CRITICAL_SECTION = 1)
   * the Timer requested can be started immediately because there is no risk that
   * the timer is already in use.
   * If a specific application needs to handle the timer differently,
   * it is suggested to overload the function without change the COMMON one. 
   */
  HW_BNRG_TimStartAllowed();

  return;
}

