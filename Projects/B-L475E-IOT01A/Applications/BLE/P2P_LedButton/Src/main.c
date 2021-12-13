/**
 ******************************************************************************
 * @file    main.c
 * @author  MCD Application Team
 * @brief   BLE application with BlueNRG
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
#include "lb_demo.h"
#include "scheduler.h"

#include "tl_types.h"
#include "tl_ble_reassembly.h"
#include "tl_ble_hci.h"
#include "lb_server_app.h"
#include "stm32l475e_iot01.h"


/**
 * In order to support the Standby Mode, we need to store some variables in a retention memory.
 * In the STM32L4, the SRAM2 is used for that purpose
 * The section below identified the variables to be stored in the retention memory
 * This section is mapped in the SRAM2 in the scatter file.
 * All variables which content shall be retained while in standby mode shall be located in that section.
 * The same section could be defined in other files to store variables. It is as well possible to define other sections
 * and map them in the SRAM2 in the scatter file.
 */
/**
 * START of Section APP_CONTEXT
 */
static RTC_HandleTypeDef hrtc;  /**< RTC handler declaration */

/**
 * END of Section APP_CONTEXT
 */

static void Init_RTC(void);
static void SystemPower_Config(void);
static void SystemLowPowerMode_Config(void);
void SystemClock_Config(void);

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
  HAL_Init();

#if (CFG_DEBUGGER_SUPPORTED == 1)
  /**
   * Keep debugger enabled while in any low power mode
   */
  HAL_DBGMCU_EnableDBGSleepMode();
  HAL_DBGMCU_EnableDBGStopMode();
  HAL_DBGMCU_EnableDBGStandbyMode();

#endif /* (CFG_DEBUGGER_SUPPORTED == 1) */

  /**
   * The PWR Peripheral is used in most of the drivers. It should be kept enable to make sure all accesses are granted.
   */
  __HAL_RCC_PWR_CLK_ENABLE();

  /**
   *********************************************************************************************************************
   * START OF SECTION - DEBUG FACILITY
   *
   * This section should be removed in production.
   * The Firmware used the RTC IP to implement the TimerServer. During development, it is common to trigger the nreset
   * to restart the application as this would be done on power ON. However, in that case, the RTC domain is not reseted
   * The following implementation resets the RTC domain so that the device behaves in a similar way on nreset to power ON.
   */

  if(LL_RCC_IsActiveFlag_PINRST() != RESET)
  {
    LL_PWR_ClearFlag_SB();

    HAL_PWR_EnableBkUpAccess(); /**< Enable access to the RTC registers */

    /**
     *  Write twice the value to flush the APB-AHB bridge
     *  This bit shall be written in the register before writing the next one
     */
    HAL_PWR_EnableBkUpAccess();

    LL_RCC_ForceBackupDomainReset();
    __HAL_RCC_CLEAR_RESET_FLAGS();
    LL_RCC_ReleaseBackupDomainReset();
  }

  /**
   * END OF SECTION - DEBUG FACILITY
   *********************************************************************************************************************
   */

  /**
   * The Standby mode should not be entered before the initialization is over
   * The default state of the Low Power Manager is to allow the Standby Mode so an request is needed here
   */
  LPM_SetOffMode(CFG_LPM_App, LPM_OffMode_Dis);

  SystemPower_Config();         /**< Configure the system Power */

  SystemLowPowerMode_Config();  /**< Configure the low power mode */

  /**
   * Configure the RTC
   */
  Init_RTC();                             /**< Initialize the RTC IP */
  HW_TS_Init(hw_ts_InitMode_Full, &hrtc);  /**< Initialize the TimerServer */

  /**
   * The system clock is updated from this point as the code below may depend on the system clock selected
   * The code above can be running at any speed.
   * When the application is expected to run at higher speed, it should be better to set the correct system clock
   * in system_stm32yyxx.c so that the initialization phase is running at max speed.
   */
  SystemClock_Config();         /**< Configure the system clock */


#if(CFG_DEBUG_TRACE != 0)
  DbgTraceInit();
#endif


  /**
   * It shall be registered before calling LBR_Init()
   */
  /* SCH_RegTask( CFG_IdleTask_MeasReq, HRSAPP_Measurement ); */
  
  SCH_RegTask( CFG_IdleTask_HciAsynchEvt, TL_BLE_HCI_UserEvtProc );
  SCH_RegTask( CFG_IdleTask_TlEvt, TL_BLE_R_EvtProc );
  SCH_RegTask( CFG_IdleTask_Button, LB_App_Button_Trigger_Received );
#if(LB_CLIENT!=0) 
  SCH_RegTask( CFG_IdleTask_ConnDev1, LBR_AppConnReq1 );
  SCH_RegTask( CFG_IdleTask_SearchService, LBR_Client_Update_Service );
#endif
#if(LB_SERVER!=0) 
  SCH_RegTask( CFG_IdleTask_StartAdv, LBR_Adv_Request );
#endif  

  /**
   * BLE application start
   */
  LBR_Init(LBR_Full);
  
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  BSP_LED_Init(LED2);
  
  /* Start the main processes */
  while(1)
  {
    SCH_Run();
  } /* end while(1) */
}


/* ********************  Private functions ***********************************/

  /**
   * @brief This function initializes the RTC IP
   * @param  None
   * @retval None
   */
static void Init_RTC(void)
{
  __HAL_RCC_LSI_ENABLE();     /**< Enable the LSI clock */

  HAL_PWR_EnableBkUpAccess(); /**< Enable access to the RTC registers */

  /**
   *  Write twice the value to flush the APB-AHB bridge
   *  This bit shall be written in the register before writing the next one
   */
  HAL_PWR_EnableBkUpAccess();

  __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSI); /**< Select LSI as RTC Input */

  __HAL_RCC_RTC_ENABLE();                     /**< Enable RTC */

  hrtc.Instance = RTC;                        /**< Define instance */

  /**
   * Bypass the shadow register
   */
  HAL_RTCEx_EnableBypassShadow(&hrtc);

  /**
   * Set the Asynchronous prescaler
   */
  hrtc.Init.AsynchPrediv = CFG_RTC_ASYNCH_PRESCALER;
  hrtc.Init.SynchPrediv = CFG_RTC_SYNCH_PRESCALER;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;             /**< need to be initialized to not corrupt the RTC_CR register */
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;  /**< need to be initialized to not corrupt the RTC_CR register */
  HAL_RTC_Init(&hrtc);

  __HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc) ;  /**< Disable Write Protection */

  LL_RTC_WAKEUP_SetClock(hrtc.Instance, CFG_RTC_WUCKSEL_DIVIDER);

  while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == 0);  /**< Wait for LSI to be stable */

  return;
}

/**
 * @brief  Configure the system for power optimization
 *
 * @note  This API configures the system to be ready for low power mode
 *    - Unused GPIO in Analog mode
 *    - FLASH in low power mode
 *
 * @param  None
 * @retval None
 */
static void SystemPower_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure = {0};

#if (CFG_DEBUGGER_SUPPORTED == 0)
  /**
   * Disable FLASH in SLEEP Mode
   */
  __HAL_FLASH_SLEEP_POWERDOWN_ENABLE();
#endif

#if (MSI_WITHOUT_PLL == 0)
  /**
   * Select HSI as system clock source after Wake Up from Stop mode
   */
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_StopWakeUpClock_HSI);
#endif

  /**
   * Disable all GPIOs clock in SLEEP mode
   */
#if (CFG_DEBUGGER_SUPPORTED == 0)
  __HAL_RCC_GPIOA_CLK_SLEEP_DISABLE();
#endif
  __HAL_RCC_GPIOB_CLK_SLEEP_DISABLE();
  __HAL_RCC_GPIOC_CLK_SLEEP_DISABLE();
  __HAL_RCC_GPIOD_CLK_SLEEP_DISABLE();
  __HAL_RCC_GPIOE_CLK_SLEEP_DISABLE();
  __HAL_RCC_GPIOF_CLK_SLEEP_DISABLE();
  __HAL_RCC_GPIOG_CLK_SLEEP_DISABLE();
  __HAL_RCC_GPIOH_CLK_SLEEP_DISABLE();

  /**
   * Port CDEFGH are properly configured on reset
   */

  /**
   * Enable GPIOs clock
   */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /**
   * Configure all GPIO port pins in Analog mode with no pull
   */
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

#if (CFG_DEBUGGER_SUPPORTED == 1)
  GPIO_InitStructure.Pin = (GPIO_PIN_All ^ (GPIO_PIN_13 | GPIO_PIN_14));
#endif
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /**
   * Disable GPIOs clock
   */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();

  return;
}

/**
 * @brief  Execute the low power mode configuration.
 * @note
 * @param  None
 * @retval None
 */
static void SystemLowPowerMode_Config(void)
{
  LPM_Conf_t LowPowerModeConfiguration;

  /**
   * Keep SRAM2 supplied when in standby mode
   */
  HAL_PWREx_EnableSRAM2ContentRetention();

  LowPowerModeConfiguration.Stop_Mode_Config = CFG_StopMode2;
  LowPowerModeConfiguration.OFF_Mode_Config = CFG_Standby;
  LPM_SetConf(&LowPowerModeConfiguration);

  return;
}

/**
 * @brief  Configure the system clock
 *
 * @note   This API configures
 *         - The system clock source
 *           - The AHBCLK, APBCLK dividers
 *           - The flash latency
 *           - The PLL settings (when required)
 *
 * @param  None
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

#if (HSI_WITH_PLL == 1)
  /**
   * Set to Range1 has the reset value has been changed in system_stm32yyxx.c to Range2
   */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /**
   *  Enable HSI oscillator and configure the PLL to reach the max system frequency
   *  (80MHz) when using HSI oscillator as PLL clock source.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV17;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV8;

  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /**
   *  Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers.
   *  The SysTick 1 msec interrupt is required for the HAL process (Timeout management); by default
   *  the configuration is done using the HAL_Init() API, and when the system clock configuration
   *  is updated the SysTick configuration will be adjusted by the HAL_RCC_ClockConfig() API.
   */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV16;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV8;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);

#elif (HSI_WITHOUT_PLL == 1)
  /**
   * The device has been already set to Range2 in system_stm32yyxx.c
   */

  /**
   *  Enable HSI oscillator and configure the system at 16MHz
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;

  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /**
   *  Configure the HCLK, PCLK1 and PCLK2 clocks dividers  to get 8Mhz.
   *  The SysTick 1 msec interrupt is required for the HAL process (Timeout management); by default
   *  the configuration is done using the HAL_Init() API, and when the system clock configuration
   *  is updated the SysTick configuration will be adjusted by the HAL_RCC_ClockConfig() API.
   */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
#elif (MSI_WITHOUT_PLL == 1)
  /**
   * The device has been already set to Range2 in system_stm32yyxx.c
   */

  /**
   *  Enable MSI oscillator and configure the system at 16MHz
   *  The best power consumption are achieved with 8Mhz on the SPI to access the BlueNRG device
   */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_8;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_OFF;

  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /**
   *  Configure the HCLK, PCLK1 and PCLK2 clocks dividers  to a maximum of 16Mhz on SPI.
   */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
#else
#error Missing System Clock Configuration
#endif

  return;
}


/* ******************************************************************************/
/* ******  Exported functions to OVERLOAD __weak defined in /Common modules *****/
/* ******************************************************************************/

/**
  * @brief  This function implement the application action when a EXTI IRQ is raised
  *         It OVERLOADS the __WEAK function defined in stm32l4xx_hal_gpio.h/c
  * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
  * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch(GPIO_Pin)
  {
    case BNRG_SPI_EXTI_PIN:
      HW_BNRG_SpiIrqCb();
      break;
    case USER_BUTTON_PIN:
      LBR_App_Key_Button_Action();
      break; 
    default:
      break;
  }

  return;
}

/**
  * @brief  This function is empty as the SysTick Timer is not used
  *         It OVERLOADS the __WEAK function defined in stm32l4xx_hal.c
  * @param Delay: specifies the delay time length, in milliseconds.
  * @retval None
 */
void HAL_Delay(__IO uint32_t Delay)
{
  return;
}

/**
  * @brief  This function OVERLOADEDs the one defined as __WEAK in the LPM (LowPowerManager)
  *         The application should implement dedicated code before getting out from Stop Mode. 
  *         This is where the application should reconfigure the clock tree when needed
  * @param  None
  * @retval None
  */
void LPM_ExitStopMode(void)
{
#if (HSI_WITH_PLL == 1)
  __HAL_RCC_PLL_ENABLE();                                     /**< Enable PLL */
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET);        /**< Wait till PLL is ready */
  LL_RCC_SetSysClkSource(RCC_CFGR_SW_PLL);              /**< Select PLL as system clock source */
  while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_CFGR_SWS_PLL);  /**< Wait till PLL is used as system clock source */
#endif

  return;
}

/**
  * @brief  This function OVERLOADEDs the one defined as __WEAK in the LPM (LowPowerManager)
  *         It sets the IO configuration to be applied when in standby mode 
  * @param  None
  * @retval None
  */
void LPM_EnterOffMode(void)
{
  LL_PWR_EnableWakeUpPin(BNRG_SPI_IRQ_WAKEUP_PIN);
  LL_PWR_EnablePUPDCfg();

  return;

}

/**
  * @brief  This function OVERLOADEDs the one defined as __WEAK in the LPM (LowPowerManager)
  *         The application should implement dedicated code before getting out from Off mode 
  * @param  None
  * @retval None
  */
void LPM_ExitOffMode(void)
{
  LL_PWR_DisablePUPDCfg();
  LL_PWR_DisableWakeUpPin(BNRG_SPI_IRQ_WAKEUP_PIN);
  BNRG_SPI_CLEAR_WAKEUP_FLAG();

  return;
}

/**
  * @brief  This function is empty to avoid starting the SysTick Timer
  *         It OVERLOADS the __WEAK function defined in stm32l4xx_hal.c
  * @param TickPriority: Tick interrupt priority.
  * @retval HAL status
 */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  return (HAL_OK);
}

/**
 * @brief This function OVERLOADEDs the one defined as __WEAK in the SCH (scheduler) 
 *        The application should enter low power mode
 * @param  None
 * @retval None
 */
void SCH_Idle ( void )
{
  LPM_EnterModeSelected();

  return;
}

/**
 * @brief This function OVERLOADEDs the one defined as __WEAK in the TL (transport layer), because
 *        only the application knows which TASKs are currently running and should be paused/resumed.
 *        The applicaiton shall not send a new command when TL the status is set to TL_BLE_HCI_CmdBusy.
 *        At least, TL_BLE_HCI_UserEvtProc shall not be executed while the TL remains busy
 * @param status
 * @retval None
 */
void TL_BLE_HCI_StatusNot(TL_BLE_HCI_CmdStatus_t status)
{
  switch(status)
  {
    case TL_BLE_HCI_CmdBusy:
      SCH_PauseTask( CFG_IdleTask_StartAdv );
      SCH_PauseTask( CFG_IdleTask_Button );
      SCH_PauseTask( CFG_IdleTask_HciAsynchEvt );
#if(LB_CLIENT!=0)       
      SCH_PauseTask( CFG_IdleTask_ConnDev1 );
      SCH_PauseTask( CFG_IdleTask_SearchService );
#endif      
      break;

    case TL_BLE_HCI_CmdAvailable:
      SCH_ResumeTask( CFG_IdleTask_StartAdv );
      SCH_ResumeTask( CFG_IdleTask_Button );
      SCH_ResumeTask( CFG_IdleTask_HciAsynchEvt );
#if(LB_CLIENT!=0)       
      SCH_ResumeTask( CFG_IdleTask_ConnDev1 );
      SCH_ResumeTask( CFG_IdleTask_SearchService );
#endif      
      break;

    default:
      break;
  }
  return;
}

/**
  * @brief  This function OVERLOADEDs the one defined as __WEAK in the TS (TimeServer)
  *         This API is running in the RTC Wakeup interrupt context. 
  *         The application may implement an Operating System to change the context priority where the timer
  *         callback may be handled. This API provides the module ID to identify which module is concerned and to allow
  *         sending the information to the correct task
  *
  * @param  TimerProcessID: The TimerProcessId associated with the timer when it has been created
  * @param  TimerID: The TimerID of the expired timer
  * @param  pTimerCallBack: The Callback associated with the timer when it has been created
  * @retval None
  */
void HW_TS_RTC_Int_AppNot(uint32_t eTimerProcessID, uint8_t ubTimerID, HW_TS_pTimerCb_t pfTimerCallBack)
{
  switch (eTimerProcessID)
  {
    case CFG_TimProcID_isr:
      /**
       * This is expected to be called in the Timer Interrupt Handler to limit latency
       */
      pfTimerCallBack();
      break;

    default:
      pfTimerCallBack();
      break;
  }
}



