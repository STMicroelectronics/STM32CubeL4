/**
******************************************************************************
  * @file    spsgrf.h
  * @author  MCD Application Team
  * @brief   This file provides the API to the actual driver of SPSGRF module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPSGRF_H
#define __SPSGRF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Include all integer types definitions */
#include <stdint.h>
#include <stdio.h>


/**
 * @addtogroup SPSGRF
 * @{
 */

/* +++++++++++++++++++++ BEGIN SPSGRF registers +++++++++++++++++++++++++++++++*/
 
/**
 * @addtogroup SPIRIT_Registers         SPIRIT Registers
 * @brief Header file containing all the SPIRIT registers address and masks.
 * @details See the file <i>@ref SPIRIT_Regs.h</i> for more details.
 * @{
 */

/** @defgroup General_Configuration_Registers
  * @{
  */

/** @defgroup ANA_FUNC_CONF_1_Register
  * @{
  */

/**
 *  \brief ANA_FUNC_CONF register 1
 *  \code
 *   Read Write
 *   Default value: 0x0C
 *   7:5 NUM_EN_PIPES: Number of enabled pipes (starting from Data Pipe 0).
 *   4:2 GM_CONF[2:0]: Sets the driver gm of the XO at start-up:
 *       GM_CONF2 | GM_CONF1 | GM_CONF0 | GM [mS]
 *       ------------------------------------------
 *           0    |    0     |     0    |  13.2
 *           0    |    0     |     1    |  18.2
 *           0    |    1     |     0    |  21.5
 *           0    |    1     |     1    |  25.6
 *           1    |    0     |     0    |  28.8
 *           1    |    0     |     1    |  33.9
 *           1    |    1     |     0    |  38.5
 *           1    |    1     |     1    |  43.0
 *   1:0 SET_BLD_LVL[1:0]: Sets the Battery Level Detector threshold:
 *       SET_BLD_LVL1 | SET_BLD_LVL0 | Threshold [V]
 *       ------------------------------------------
 *             0      |      0       |     2.7
 *             0      |      1       |     2.5
 *             1      |      0       |     2.3
 *             1      |      1       |     2.1
 *   \endcode
 */

#define ANA_FUNC_CONF1_BASE                    ((uint8_t)0x00) /*!< ANA_FUNC_CONF1 Address (R/W) */

#define ANA_FUNC_CONF1_NUM_PIPES_MASK                           ((uint8_t)0xE0) /*!< Mask for number of enabled pipes*/

#define ANA_FUNC_CONF1_GMCONF_MASK                    ((uint8_t)0x1C) /*!< Mask of the GmConf field of ANA_FUNC_CONF1 register (R/W) */

#define GM_13_2                                        ((uint8_t)0x00) /*!< Transconducatance Gm at start-up 13.2 mS */
#define GM_18_2                                        ((uint8_t)0x04) /*!< Transconducatance Gm at start-up 18.2 mS */
#define GM_21_5                                        ((uint8_t)0x08) /*!< Transconducatance Gm at start-up 21.5 mS */
#define GM_25_6                                        ((uint8_t)0x0C) /*!< Transconducatance Gm at start-up 25.6 mS */
#define GM_28_8                                        ((uint8_t)0x10) /*!< Transconducatance Gm at start-up 28.8 mS */
#define GM_33_9                                        ((uint8_t)0x14) /*!< Transconducatance Gm at start-up 33.9 mS */
#define GM_38_5                                        ((uint8_t)0x18) /*!< Transconducatance Gm at start-up 38.5 mS */
#define GM_43_0                                        ((uint8_t)0x1C) /*!< Transconducatance Gm at start-up 43.0 mS */

#define ANA_FUNC_CONF1_SET_BLD_LVL_MASK                    ((uint8_t)0x03) /*!< Mask of the SET_BLD_LV field of ANA_FUNC_CONF1 register (R/W) */

#define BLD_LVL_2_7                                ((uint8_t)0x00) /*!< Sets the Battery Level Detector threshold to 2.7V */
#define BLD_LVL_2_5                                ((uint8_t)0x01) /*!< Sets the Battery Level Detector threshold to 2.5V */
#define BLD_LVL_2_3                                ((uint8_t)0x02) /*!< Sets the Battery Level Detector threshold to 2.3V */
#define BLD_LVL_2_1                                ((uint8_t)0x03) /*!< Sets the Battery Level Detector threshold to 2.1V */

/**
 * @}
 */


/** @defgroup ANA_FUNC_CONF_0_Register
 * @{
 */

/**
 *  \brief ANA_FUNC_CONF register 0
 *  \code
 *   Read Write
 *   Default value: 0xC0
 *   7 Reserved.
 *   6 24_26_MHz_SELECT: 1 - 26 MHz configuration
 *                       0 - 24 MHz configuration
 *   5 AES_ON:           1 - AES engine enabled
 *                       0 - AES engine disabled
 *   4 EXT_REF:          1 - Reference signal from XIN pin
 *                       0 - Reference signal from XO circuit
 *   3 HIGH_POWER_MODE:  1 - SET_SMPS_LEVEL word will be set to the value to
 *                           PM_TEST register in RX state, while in TX state it
 *                           will be fixed to 111 (which programs the SMPS output
 *                           at max value 1.8V)
 *                       0 - SET_SMPS_LEVEL word will hold the value written in the
 *                           PM_TEST register both in RX and TX state
 *   2 BROWN_OUT:        1 - Brown_Out Detection enabled
 *                       0 - Brown_Out Detection disabled
 *   1 BATTERY_LEVEL:    1 - Battery level detector enabled
 *                       0 - Battery level detector disabled
 *   0 TS:               1 - Enable the "Temperature Sensor" function
 *                       0 - Disable the "Temperature Sensor" function
 *   \endcode
 */


#define ANA_FUNC_CONF0_BASE                    ((uint8_t)0x01) /*!< ANA_FUNC_CONF0 Address (R/W) */

#define SELECT_24_26_MHZ_MASK                  ((uint8_t)0x40) /*!< Configure the RCO if using 26 MHz or 24 MHz master clock/reference signal */
#define AES_MASK                               ((uint8_t)0x20) /*!< AES engine on/off */
#define EXT_REF_MASK                           ((uint8_t)0x10) /*!< Reference signal from XIN pin (oscillator external) or from XO circuit (oscillator internal)*/
#define HIGH_POWER_MODE_MASK                   ((uint8_t)0x08) /*!< SET_SMPS_LEVEL word will be set to the value to PM_TEST register
                                                                                     in RX state, while in TX state it will be fixed to 111
                                                                                     (which programs the SMPS output at max value, 1.8V) */
#define BROWN_OUT_MASK                         ((uint8_t)0x04) /*!< Accurate Brown-Out detection on/off */
#define BATTERY_LEVEL_MASK                     ((uint8_t)0x02) /*!< Battery level detector circuit on/off */
#define TEMPERATURE_SENSOR_MASK                ((uint8_t)0x01) /*!< The Temperature Sensor (available on GPIO0) on/off */

/**
 * @}
 */

/** @defgroup ANT_SELECT_CONF_Register
 * @{
 */

/**
 *  \brief ANT_SELECT_CONF register
 *  \code
 *   Read Write
 *   Default value: 0x05
 *
 *   7:5   Reserved.
 *
 *   4   CS_BLANKING: Blank received data if signal is below the CS threshold
 *
 *   3   AS_ENABLE: Enable antenna switching
 *                       1 - Enable
 *                       0 - Disable
 *
 *   2:0 AS_MEAS_TIME[2:0]: Measurement time according to the formula Tmeas = 24*2^(EchFlt)*2^AS_MEAS_TIME/fxo
 *   \endcode
 */
#define    ANT_SELECT_CONF_BASE                    ((uint8_t)0x27) /*!< Antenna diversity (works only in static carrier sense mode) */
#define ANT_SELECT_CS_BLANKING_MASK                ((uint8_t)0x10) /*!< CS data blanking on/off */
#define ANT_SELECT_CONF_AS_MASK                    ((uint8_t)0x08) /*!< Antenna diversity on/off */

/**
 * @}
 */

/** @defgroup DEVICE_INFO1_Register
 * @{
 */

/**
 *  \brief DEVICE_INFO1[7:0]  registers
 *  \code
 *   Default value: 0x01
 *   Read
 *
 *   7:0       PARTNUM[7:0]:   Device part number
 *   \endcode
 */
#define    DEVICE_INFO1_PARTNUM                    ((uint8_t)(0xF0)) /*!< Device part number [7:0] */

/**
 * @}
 */

/** @defgroup DEVICE_INFO0_Register
 * @{
 */

/**
 *  \brief DEVICE_INFO0[7:0]  registers
 *  \code
 *   Read
 *
 *   7:0       VERSION[7:0]:  Device version number
 *   \endcode
 */
#define    DEVICE_INFO0_VERSION                    ((uint8_t)(0xF1)) /*!< Device version [7:0]; (0x55 in CUT1.0) */

/**
 * @}
 */


/**
 * @}
 */


/** @defgroup GPIO_Registers
 * @{
 */

/** @defgroup GPIOx_CONF_Registers
 * @{
 */

/**
 *  \brief GPIOx registers
 *  \code
 *   Read Write
 *   Default value: 0x03
 *   7:3 GPIO_SELECT[4:0]: Specify the I/O signal.
 *       GPIO_SELECT[4:0] |    I/O    |      Signal
 *       ------------------------------------------------
 *               0        |  Output   |  nIRQ
 *               0        |  Input    |  TX command
 *               1        |  Output   |  POR inverted
 *               1        |  Input    |  RX command
 *               2        |  Output   |  Wake-Up timer expiration
 *               2        |  Input    |  TX data for direct modulation
 *               3        |  Output   |  Low Battery Detection
 *               3        |  Input    |  Wake-up from external input
 *               4        |  Output   |  TX clock output
 *               5        |  Output   |  TX state
 *               6        |  Output   |  TX FIFO Almost Empty Flag
 *               7        |  Output   |  TX FIFO ALmost Full Flag
 *               8        |  Output   |  RX data output
 *               9        |  Output   |  RX clock output
 *               10       |  Output   |  RX state
 *               11       |  Output   |  RX FIFO Almost Full Flag
 *               12       |  Output   |  RX FIFO Almost Empty Flag
 *               13       |  Output   |  Antenna switch
 *               14       |  Output   |  Valid preamble detected
 *               15       |  Output   |  Sync word detected
 *               16       |  Output   |  RSSI above threshold
 *               17       |  Output   |  MCU clock
 *               18       |  Output   |  TX or RX mode indicator
 *               19       |  Output   |  VDD
 *               20       |  Output   |  GND
 *               21       |  Output   |  External SMPS enable signal
 *               22-31    |  Not Used |  Not Used
 *   2 Reserved
 *   1:0 GpioMode[1:0]: Specify the mode:
 *        GPIO_MODE1  |  GPIO_MODE0  |               MODE
 *       ------------------------------------------------------------
 *             0      |      0       |    Analog (valid only for GPIO_0)
 *             0      |      1       |    Digital Input
 *             1      |      0       |    Digital Output Low Power
 *             1      |      1       |    Digital Output High Power
 *
 *   Note: The Analog mode is used only for temperature sensor indication. This is available only
 *         on GPIO_0 by setting the TS bit in the ANA_FUNC_CONF_0_Register.
 *   \endcode
 */


#define GPIO3_CONF_BASE                        ((uint8_t)0x02) /*!< GPIO_3 register address */
#define GPIO2_CONF_BASE                        ((uint8_t)0x03) /*!< GPIO_3 register address */
#define GPIO1_CONF_BASE                        ((uint8_t)0x04) /*!< GPIO_3 register address */
#define GPIO0_CONF_BASE                        ((uint8_t)0x05) /*!< GPIO_3 register address */

#define CONF_GPIO_IN_TX_Command                ((uint8_t)0x00) /*!< TX command direct from PIN (rising edge, width min=50ns) */
#define CONF_GPIO_IN_RX_Command                ((uint8_t)0x08) /*!< RX command direct from PIN (rising edge, width min=50ns)*/
#define CONF_GPIO_IN_TX_Data                   ((uint8_t)0x10) /*!< TX data input for direct modulation */
#define CONF_GPIO_IN_WKUP_Ext                  ((uint8_t)0x18) /*!< Wake up from external input */

#define CONF_GPIO_OUT_nIRQ                     ((uint8_t)0x00) /*!< nIRQ (Interrupt Request, active low) , default configuration after POR */
#define CONF_GPIO_OUT_POR_Inv                  ((uint8_t)0x08) /*!< POR inverted (active low) */
#define CONF_GPIO_OUT_WUT_Exp                  ((uint8_t)0x10) /*!< Wake-Up Timer expiration: ‘1’ when WUT has expired */
#define CONF_GPIO_OUT_LBD                      ((uint8_t)0x18) /*!< Low battery detection: ‘1’ when battery is below threshold setting */
#define CONF_GPIO_OUT_TX_Data                  ((uint8_t)0x20) /*!< TX data internal clock output (TX data are sampled on the rising edge of it) */
#define CONF_GPIO_OUT_TX_State                 ((uint8_t)0x28) /*!< TX state indication: ‘1’ when Spirit1 is transiting in the TX state */
#define CONF_GPIO_OUT_TX_FIFO_Almost_Empty     ((uint8_t)0x30) /*!< TX FIFO Almost Empty Flag */
#define CONF_GPIO_OUT_TX_FIFO_Amost_Full       ((uint8_t)0x38) /*!< TX FIFO Almost Full Flag */
#define CONF_GPIO_OUT_RX_Data                  ((uint8_t)0x40) /*!< RX data output */
#define CONF_GPIO_OUT_RX_Clock                 ((uint8_t)0x48) /*!< RX clock output (recovered from received data) */
#define CONF_GPIO_OUT_RX_State                 ((uint8_t)0x50) /*!< RX state indication: ‘1’ when Spirit1 is transiting in the RX state */
#define CONF_GPIO_OUT_RX_FIFO_Almost_Full      ((uint8_t)0x58) /*!< RX FIFO Almost Full Flag */
#define CONF_GPIO_OUT_RX_FIFO_Almost_Empty     ((uint8_t)0x60) /*!< RX FIFO Almost Empty Flag */
#define CONF_GPIO_OUT_Antenna_Switch           ((uint8_t)0x68) /*!< Antenna switch used for antenna diversity */
#define CONF_GPIO_OUT_Valid_Preamble           ((uint8_t)0x70) /*!< Valid Preamble Detected Flag */
#define CONF_GPIO_OUT_Sync_Detected            ((uint8_t)0x78) /*!< Sync WordSync Word Detected Flag */
#define CONF_GPIO_OUT_RSSI_Threshold           ((uint8_t)0x80) /*!< CCA Assessment Flag */
#define CONF_GPIO_OUT_MCU_Clock                ((uint8_t)0x88) /*!< MCU Clock */
#define CONF_GPIO_OUT_TX_RX_Mode               ((uint8_t)0x90) /*!< TX or RX mode indicator (to enable an external range extender) */
#define CONF_GPIO_OUT_VDD                      ((uint8_t)0x98) /*!< VDD (to emulate an additional GPIO of the MCU, programmable by SPI) */
#define CONF_GPIO_OUT_GND                      ((uint8_t)0xA0) /*!< GND (to emulate an additional GPIO of the MCU, programmable by SPI) */
#define CONF_GPIO_OUT_SMPS_Ext                 ((uint8_t)0xA8) /*!< External SMPS enable signal (active high) */

#define CONF_GPIO_MODE_ANALOG                  ((uint8_t)0x00) /*!< Analog test BUS on GPIO; used only in test mode (except for temperature sensor) */
#define CONF_GPIO_MODE_DIG_IN                  ((uint8_t)0x01) /*!< Digital Input on GPIO */
#define CONF_GPIO_MODE_DIG_OUTL                ((uint8_t)0x02) /*!< Digital Output on GPIO (low current) */
#define CONF_GPIO_MODE_DIG_OUTH                ((uint8_t)0x03) /*!< Digital Output on GPIO (high current) */

/**
 * @}
 */


/** @defgroup MCU_CK_CONF_Register
 * @{
 */

/**
 *  \brief MCU_CK_CONF  register
 *  \code
 *   Read Write
 *   Default value: 0x00
 *   7   Reserved.
 *   6:5 CLOCK_TAIL[1:0]: Specifies the number of extra cylces provided before entering in STANDBY state.
 *       CLOCK_TAIL1   |   CLOCK_TAIL0    |  Number of Extra Cycles
 *       ------------------------------------------------------------
 *            0        |        0         |           0
 *            0        |        1         |           64
 *            1        |        0         |           256
 *            1        |        1         |           512
 *   4:1 XO_RATIO[3:0]: Specifies the division ratio when XO oscillator is the clock source
 *       XO_RATIO[3:0] |   Division Ratio
 *       -----------------------------------
 *               0     |        1
 *               1     |       2/3
 *               2     |       1/2
 *               3     |       1/3
 *               4     |       1/4
 *               5     |       1/6
 *               6     |       1/8
 *               7     |       1/12
 *               8     |       1/16
 *               9     |       1/24
 *               10    |       1/36
 *               11    |       1/48
 *               12    |       1/64
 *               13    |       1/96
 *               14    |       1/128
 *               15    |       1/256
 *   0   RCO_RATIO: Specifies the divsion ratio when RC oscillator is the clock source
 *                0 - Division Ratio equal to 0
 *                1 - Division Ratio equal to 1/128
 *   \endcode
 */


#define MCU_CK_CONF_BASE                    ((uint8_t)0x06) /*!< MCU Clock Config register address */

#define MCU_CK_ENABLE                                           ((uint8_t)0x80) /*!< MCU clock enable bit */

#define MCU_CK_CONF_CLOCK_TAIL_0                ((uint8_t)0x00) /*!< 0   extra clock cycles provided to the MCU before switching to STANDBY state */
#define MCU_CK_CONF_CLOCK_TAIL_64               ((uint8_t)0x20) /*!< 64  extra clock cycles provided to the MCU before switching to STANDBY state */
#define MCU_CK_CONF_CLOCK_TAIL_256              ((uint8_t)0x40) /*!< 256 extra clock cycles provided to the MCU before switching to STANDBY state */
#define MCU_CK_CONF_CLOCK_TAIL_512              ((uint8_t)0x60) /*!< 512 extra clock cycles provided to the MCU before switching to STANDBY state */
#define MCU_CK_CONF_XO_RATIO_1                  ((uint8_t)0x00) /*!< XO Clock signal available on the GPIO divided by 1     */
#define MCU_CK_CONF_XO_RATIO_2_3                ((uint8_t)0x02) /*!< XO Clock signal available on the GPIO divided by 2/3   */
#define MCU_CK_CONF_XO_RATIO_1_2                ((uint8_t)0x04) /*!< XO Clock signal available on the GPIO divided by 1/2   */
#define MCU_CK_CONF_XO_RATIO_1_3                ((uint8_t)0x06) /*!< XO Clock signal available on the GPIO divided by 1/3   */
#define MCU_CK_CONF_XO_RATIO_1_4                ((uint8_t)0x08) /*!< XO Clock signal available on the GPIO divided by 1/4   */
#define MCU_CK_CONF_XO_RATIO_1_6                ((uint8_t)0x0A) /*!< XO Clock signal available on the GPIO divided by 1/6   */
#define MCU_CK_CONF_XO_RATIO_1_8                ((uint8_t)0x0C) /*!< XO Clock signal available on the GPIO divided by 1/8   */
#define MCU_CK_CONF_XO_RATIO_1_12               ((uint8_t)0x0E) /*!< XO Clock signal available on the GPIO divided by 1/12  */
#define MCU_CK_CONF_XO_RATIO_1_16               ((uint8_t)0x10) /*!< XO Clock signal available on the GPIO divided by 1/16  */
#define MCU_CK_CONF_XO_RATIO_1_24               ((uint8_t)0x12) /*!< XO Clock signal available on the GPIO divided by 1/24  */
#define MCU_CK_CONF_XO_RATIO_1_36               ((uint8_t)0x14) /*!< XO Clock signal available on the GPIO divided by 1/36  */
#define MCU_CK_CONF_XO_RATIO_1_48               ((uint8_t)0x16) /*!< XO Clock signal available on the GPIO divided by 1/48  */
#define MCU_CK_CONF_XO_RATIO_1_64               ((uint8_t)0x18) /*!< XO Clock signal available on the GPIO divided by 1/64  */
#define MCU_CK_CONF_XO_RATIO_1_96               ((uint8_t)0x1A) /*!< XO Clock signal available on the GPIO divided by 1/96  */
#define MCU_CK_CONF_XO_RATIO_1_128              ((uint8_t)0x1C) /*!< XO Clock signal available on the GPIO divided by 1/128 */
#define MCU_CK_CONF_XO_RATIO_1_192              ((uint8_t)0x1E) /*!< XO Clock signal available on the GPIO divided by 1/196 */
#define MCU_CK_CONF_RCO_RATIO_1                 ((uint8_t)0x00) /*!< RCO Clock signal available on the GPIO divided by 1    */
#define MCU_CK_CONF_RCO_RATIO_1_128             ((uint8_t)0x01) /*!< RCO Clock signal available on the GPIO divided by 1/128*/

/**
 * @}
 */

/**
 * @}
 */


/** @defgroup Radio_Configuration_Registers
 * @{
 */



/** @defgroup SYNT3_Register
 * @{
 */

/**
 *  \brief SYNT3  register
 *  \code
 *   Read Write
 *   Default value: 0x0C
 *
 *   7:5 WCP[2:0]: Set the charge pump current according to the VCO frequency in RX mode.
 *
 *         VCO Frequency    |        WCP2      |       WCP1       |       WCP0        |    Charge Pump Current (uA)
 *       ------------------------------------------------------------------------------------------------------------
 *          4644-4678       |         0        |        0         |        0          |             378.4
 *          4708-4772       |         0        |        0         |        1          |             368.9
 *          4772-4836       |         0        |        1         |        0          |             359.5
 *          4836-4902       |         0        |        1         |        1          |             350
 *          4902-4966       |         1        |        0         |        0          |             340.5
 *          4966-5030       |         1        |        0         |        1          |             331.1
 *          5030-5095       |         1        |        1         |        0          |             321.6
 *          5095-5161       |         1        |        1         |        1          |             312.2
 *          5161-5232       |         0        |        0         |        0          |             378.4
 *          5232-5303       |         0        |        0         |        1          |             368.9
 *          5303-5375       |         0        |        1         |        0          |             359.5
 *          5375-5448       |         0        |        1         |        1          |             350
 *          5448-5519       |         1        |        0         |        0          |             340.5
 *          5519-5592       |         1        |        0         |        1          |             331.1
 *          5592-5663       |         1        |        1         |        0          |             321.6
 *          5663-5736       |         1        |        1         |        1          |             312.2
 *
 *
 *   4:0  SYNT[25:21]: highest 5 bits of the PLL programmable divider
 *                     The valid range depends on fXO and REFDIV settings; for
 *                     fXO=26MHz
 *                     REFDIV = 0 - SYNT[25:21] = 11...13
 *                     REFDIV = 1 - SYNT[25:21] = 22…27
 *
 *
 *   \endcode
 */
#define    SYNT3_BASE                        ((uint8_t)0x08) /*!< [4:0] -> SYNT[25:21], highest 5 bits of the PLL programmable divider */

#define WCP_CONF_WCP_378UA                                      ((uint8_t)0x00) /*!< Charge pump current nominal value = 378uA [VCO 4644-4708]&[VCO 5161-5232] */
#define WCP_CONF_WCP_369UA                                      ((uint8_t)0x01) /*!< Charge pump current nominal value = 369uA [VCO 4708-4772]&[VCO 5232-5303] */
#define WCP_CONF_WCP_359UA                                      ((uint8_t)0x02) /*!< Charge pump current nominal value = 359uA [VCO 4772-4836]&[VCO 5303-5375] */
#define WCP_CONF_WCP_350UA                                      ((uint8_t)0x03) /*!< Charge pump current nominal value = 350uA [VCO 4836-4902]&[VCO 5375-5448] */
#define WCP_CONF_WCP_340UA                                      ((uint8_t)0x04) /*!< Charge pump current nominal value = 340uA [VCO 4902-4966]&[VCO 5448-5519] */
#define WCP_CONF_WCP_331UA                                      ((uint8_t)0x05) /*!< Charge pump current nominal value = 331uA [VCO 4966-5030]&[VCO 5519-5592] */
#define WCP_CONF_WCP_321UA                                      ((uint8_t)0x06) /*!< Charge pump current nominal value = 321uA [VCO 5030-5095]&[VCO 5592-5563] */
#define WCP_CONF_WCP_312UA                                      ((uint8_t)0x07) /*!< Charge pump current nominal value = 312uA [VCO 5095-5160]&[VCO 5563-5736] */


/**
 * @}
 */


/** @defgroup SYNT2_Register
 * @{
 */

/**
 *  \brief SYNT2  register
 *  \code
 *   Read Write
 *   Default value: 0x84
 *   7:0  SYNT[20:13]: intermediate bits of the PLL programmable divider.
 *
 *   \endcode
 */

#define    SYNT2_BASE                        ((uint8_t)0x09) /*!< SYNT[20:13], intermediate bits of the PLL programmable divider */

/**
 * @}
 */

/** @defgroup SYNT1_Register
 * @{
 */

/**
 *  \brief SYNT1  register
 *  \code
 *   Read Write
 *   Default value: 0xEC
 *   7:0  SYNT[12:5]: intermediate bits of the PLL programmable divider.
 *
 *   \endcode
 */

#define    SYNT1_BASE                        ((uint8_t)0x0A) /*!< SYNT[12:5], intermediate bits of the PLL programmable divider */

/**
 * @}
 */

/** @defgroup SYNT0_Register
 * @{
 */

/**
 *  \brief SYNT0  register
 *  \code
 *   Read Write
 *   Default value: 0x51
 *   7:3  SYNT[4:0]: lowest bits of the PLL programmable divider.
 *   2:0  BS[2:0]:   Synthesizer band select. This parameter selects the out-of-loop divide factor of the synthesizer
 *                   according to the formula fxo/(B/2)/D*SYNT/2^18
 *
 *              BS2        |       BS1       |       BS0        |     value of B
 *       ---------------------------------------------------------------------------
 *               0         |        0        |        1         |       6
 *               0         |        1        |        0         |       8
 *               0         |        1        |        1         |       12
 *               1         |        0        |        0         |       16
 *               1         |        0        |        1         |       32
 *
 *   \endcode
 */
#define    SYNT0_BASE                        ((uint8_t)0x0B) /*!< [7:3] -> SYNT[4:0], lowest bits of the PLL programmable divider */

#define    SYNT0_BS_6                        ((uint8_t)0x01) /*!< Synthesizer band select (out-of-loop divide factor of the synthesizer)=6  (779-956MHz) */
#define    SYNT0_BS_8                        ((uint8_t)0x02) /*!< Synthesizer band select (out-of-loop divide factor of the synthesizer)=8  (387-470MHz)*/
#define    SYNT0_BS_12                       ((uint8_t)0x03) /*!< Synthesizer band select (out-of-loop divide factor of the synthesizer)=12 (387-470MHz)*/
#define    SYNT0_BS_16                       ((uint8_t)0x04) /*!< Synthesizer band select (out-of-loop divide factor of the synthesizer)=16 (300-348MHz)*/
#define    SYNT0_BS_32                       ((uint8_t)0x05) /*!< Synthesizer band select (out-of-loop divide factor of the synthesizer)=32 (150-174MHz)*/

/**
 * @}
 */

/** @defgroup CHSPACE_Register
 * @{
 */

/**
 *  \brief CHSPACE  register
 *  \code
 *   Read Write
 *   Default value: 0xFC
 *   7:0  CH_SPACING[7:0]: Channel spacing. From ~793Hz to ~200KHz in 793Hz steps
 *                         (in general, frequency step is fXO/215=26MHz/215~793Hz).
 *
 *   \endcode
 */

#define    CHSPACE_BASE                        ((uint8_t)0x0C) /*!< Channel spacing. From ~0.8KHz to ~200KHz in (fXO/2^15)Hz (793Hz for 26MHz XO) steps */

/**
 * @}
 */



/** @defgroup IF_OFFSET_DIG_Register
 * @{
 */

/**
 *  \brief IF_OFFSET_DIG  register
 *  \code
 *   Read Write
 *   Default value: 0xA3
 *   7:0  IF_OFFSET_DIG[7:0]: Intermediate frequency setting for the digital shift-to-baseband circuits. According to the formula: fIF=fXO*(IF_OFFSET_ANA+64)/(12*2^10)=fCLK*(IF_OFFSET_DIG+64)/(12*2^10) Hz.
 *
 *   \endcode
 */
#define    IF_OFFSET_DIG_BASE                        ((uint8_t)0x0D) /*!< Intermediate frequency fIF=fXO*(IF_OFFSET_ANA+64)/(12*2^10)=fCLK*(IF_OFFSET_DIG+64)/(12*2^10) Hz */

/**
 * @}
 */

/** @defgroup IF_OFFSET_ANA_Register
 * @{
 */

/**
 *  \brief IF_OFFSET_ANA  register
 *  \code
 *   Read Write
 *   Default value: 0xA3
 *   7:0  IF_OFFSET_ANA[7:0]: Intermediate frequency setting for the digital shift-to-baseband circuits. According to the formula: fIF=fXO*(IF_OFFSET_ANA+64)/(12*2^10)=fCLK*(IF_OFFSET_DIG+64)/(12*2^10) Hz.
 *
 *   \endcode
 */
#define    IF_OFFSET_ANA_BASE                        ((uint8_t)0x07) /*!< Intermediate frequency fIF=fXO*(IF_OFFSET_ANA+64)/(12*2^10)=fCLK*(IF_OFFSET_DIG+64)/(12*2^10) Hz */


/**
 * @}
 */

/** @defgroup FC_OFFSET1_Register
 * @{
 */

/**
 *  \brief FC_OFFSET1  registers
 *  \code
 *   Read Write
 *   Default value: 0xA3
 *   7:4  Reserved.
 *   3:0  FC_OFFSET[11:8]: Carrier offset. This value is the higher part of a 12-bit 2’s complement integer
 *                         representing an offset in 99Hz(2) units added/subtracted to the
 *                         carrier frequency set by registers SYNT3…SYNT0.
 *                         This register can be used to set a fixed correction value
 *                         obtained e.g. from crystal measurements.
 *
 *   \endcode
 */
#define    FC_OFFSET1_BASE                        ((uint8_t)0x0E) /*!< [3:0] -> [11:8] Carrier offset (upper part) */

/**
 * @}
 */


/** @defgroup FC_OFFSET0_Register
 * @{
 */

/**
 *  \brief FC_OFFSET0  registers
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:0  FC_OFFSET[7:0]: Carrier offset. This value is the lower part of a 12-bit 2’s complement integer
 *                         representing an offset in 99Hz(2) units added/subtracted to the
 *                         carrier frequency set by registers SYNT3…SYNT0.
 *                         This register can be used to set a fixed correction value
 *                         obtained e.g. from crystal measurements.
 *
 *   \endcode
 */
#define    FC_OFFSET0_BASE                        ((uint8_t)0x0F) /*!< [7:0] -> [7:0] Carrier offset (lower part). This value is a 12-bit 2’s complement integer
                                                                    representing an offset in fXO/2^18 (99Hz for 26 MHz XO) units added/subtracted to the carrier frequency
                                                            set by registers SYNT3…SYNT0. Range is +/-200kHz with 26 MHz XO */
/**
 * @}
 */


/** @defgroup PA_LEVEL_x_Registers
 * @{
 */

/**
 *  \brief PA_POWER_x[8:1]  registers
 *  \code
 *   Default values from 8 to 1: [0x03, 0x0E, 0x1A, 0x25, 0x35, 0x40, 0x4E, 0x00]
 *   Read Write
 *
 *   7    Reserved.
 *   6:0  PA_LEVEL_(x-1)[6:0]: Output power level for x-th slot.
 *   \endcode
 */

#define    PA_POWER8_BASE                        ((uint8_t)0x10) /*!< PA Power level for 8th slot of PA ramping or ASK modulation */
#define    PA_POWER7_BASE                        ((uint8_t)0x11) /*!< PA Power level for 7th slot of PA ramping or ASK modulation */
#define    PA_POWER6_BASE                        ((uint8_t)0x12) /*!< PA Power level for 6th slot of PA ramping or ASK modulation */
#define    PA_POWER5_BASE                        ((uint8_t)0x13) /*!< PA Power level for 5th slot of PA ramping or ASK modulation */
#define    PA_POWER4_BASE                        ((uint8_t)0x14) /*!< PA Power level for 4th slot of PA ramping or ASK modulation */
#define    PA_POWER3_BASE                        ((uint8_t)0x15) /*!< PA Power level for 3rd slot of PA ramping or ASK modulation */
#define    PA_POWER2_BASE                        ((uint8_t)0x16) /*!< PA Power level for 2nd slot of PA ramping or ASK modulation */
#define    PA_POWER1_BASE                        ((uint8_t)0x17) /*!< PA Power level for 1st slot of PA ramping or ASK modulation */

/**
 * @}
 */

/** @defgroup PA_POWER_CONF_Registers
 * @{
 */

/**
 *  \brief PA_POWER_CONF_Registers
 *  \code
 *   Default value:0x07
 *   Read Write
 *
 *   7:6  CWC[1:0]: Output stage additional load capacitors bank (to be used to
 *                      optimize the PA for different sub-bands).
 *
 *         CWC1       |        CWC0      |     Total capacity in pF
 *       ---------------------------------------------------------
 *          0         |         0        |        0
 *          0         |         1        |        1.2
 *          1         |         0        |        2.4
 *          1         |         1        |        3.6
 *
 *   5   PA_RAMP_ENABLE:
 *                       1 - Enable the power ramping
 *                       0 - Disable the power ramping
 *   4:3 PA_RAMP_STEP_WIDTH[1:0]: Step width in bit period
 *
 *         PA_RAMP_STEP_WIDTH1       |        PA_RAMP_STEP_WIDTH0       |     PA ramping time step
 *       -------------------------------------------------------------------------------------------
 *                 0                 |                0                 |        1/8 Bit period
 *                 0                 |                1                 |        2/8 Bit period
 *                 1                 |                0                 |        3/8 Bit period
 *                 1                 |                1                 |        4/8 Bit period
 *
 *   2:0 PA_LEVEL_MAX_INDEX[2:0]: Fixes the MAX PA LEVEL in PA ramping or ASK modulation
 *
 *   \endcode
 */
#define    PA_POWER0_BASE                       ((uint8_t)0x18) /*!< PA ramping settings and additional load capacitor banks used
                                                                    for PA optimization in different sub bands*/
#define PA_POWER0_CWC_MASK                                      ((uint8_t)0x20) /*!< Output stage additional load capacitors bank */
#define PA_POWER0_CWC_0                         ((uint8_t)0x00) /*!< No additional PA load capacitor */
#define PA_POWER0_CWC_1_2P                      ((uint8_t)0x40) /*!< 1.2pF additional PA load capacitor */
#define PA_POWER0_CWC_2_4P                      ((uint8_t)0x80) /*!< 2.4pF additional PA load capacitor */
#define PA_POWER0_CWC_3_6P                      ((uint8_t)0xC0) /*!< 3.6pF additional PA load capacitor */
#define PA_POWER0_PA_RAMP_MASK                  ((uint8_t)0x20) /*!< The PA power ramping */
#define PA_POWER0_PA_RAMP_STEP_WIDTH_MASK                       ((uint8_t)0x20) /*!< The step width */
#define PA_POWER0_PA_RAMP_STEP_WIDTH_TB_8            ((uint8_t)0x00) /*!< PA ramping time step = 1/8 Bit period*/
#define PA_POWER0_PA_RAMP_STEP_WIDTH_TB_4            ((uint8_t)0x08) /*!< PA ramping time step = 2/8 Bit period*/
#define PA_POWER0_PA_RAMP_STEP_WIDTH_3TB_8           ((uint8_t)0x10) /*!< PA ramping time step = 3/8 Bit period*/
#define PA_POWER0_PA_RAMP_STEP_WIDTH_TB_2            ((uint8_t)0x18) /*!< PA ramping time step = 4/8 Bit period*/
#define PA_POWER0_PA_LEVEL_MAX_INDEX                            ((uint8_t)0x20) /*!< Final level for power ramping */
#define PA_POWER0_PA_LEVEL_MAX_INDEX_0                ((uint8_t)0x00) /*!<                                                           */
#define PA_POWER0_PA_LEVEL_MAX_INDEX_1                ((uint8_t)0x01) /*!<  Fixes the MAX PA LEVEL in PA ramping or ASK modulation   */
#define PA_POWER0_PA_LEVEL_MAX_INDEX_2                ((uint8_t)0x02) /*!<                                                           */
#define PA_POWER0_PA_LEVEL_MAX_INDEX_3                ((uint8_t)0x03) /*!<                     _________                             */
#define PA_POWER0_PA_LEVEL_MAX_INDEX_4                ((uint8_t)0x04) /*!<     PA_LVL2       _|                 <--|                 */
#define PA_POWER0_PA_LEVEL_MAX_INDEX_5                ((uint8_t)0x05) /*!<                 _|                      |                 */
#define PA_POWER0_PA_LEVEL_MAX_INDEX_6                ((uint8_t)0x06) /*!<     PA_LVL1   _|                        |                 */
#define PA_POWER0_PA_LEVEL_MAX_INDEX_7                ((uint8_t)0x07) /*!<     PA_LVL0 _|                 MAX_INDEX-                 */



/**
 * @}
 */


/** @defgroup MOD1_Register
 * @{
 */

/**
 *  \brief MOD1 register
 *  \code
 *   Read Write
 *   Default value: 0x83
 *   7:0  DATARATE_M[7:0]: The Mantissa of the specified data rate
 *
 *   \endcode
 */
#define    MOD1_BASE                            ((uint8_t)0x1A) /*!< The Mantissa of the specified data rate */

/**
 * @}
 */

/** @defgroup MOD0_Register
 * @{
 */

/**
 *  \brief MOD0 register
 *  \code
 *   Read Write
 *   Default value: 0x1A
 *   7  CW:              1 - CW Mode enabled - enables the generation of a continous wave carrier without any modulation
 *                       0 - CW Mode disabled
 *
 *   6 BT_SEL:  Select BT value for GFSK
 *                       1 - BT=0.5
 *                       0 - BT=1
 *
 *   5:4 MOD_TYPE[1:0]: Modulation type
 *
 *
 *         MOD_TYPE1       |        MOD_TYPE0      |     Modulation
 *       ---------------------------------------------------------
 *             0           |           0           |        2-FSK,MSK
 *             0           |           1           |        GFSK,GMSK
 *             1           |           0           |        ASK/OOK
 *
 *   3:0  DATARATE_E[3:0]: The Exponent of the specified data rate
 *
 *   \endcode
 */
#define    MOD0_BASE                        ((uint8_t)0x1B) /*!< Modulation Settings, Exponent of the specified data rate, CW mode*/

#define MOD0_MOD_TYPE_2_FSK                 ((uint8_t)0x00) /*!< Modulation type 2-FSK (MSK if the frequency deviation is identical to a quarter of the data rate) */
#define MOD0_MOD_TYPE_GFSK                  ((uint8_t)0x10) /*!< Modulation type GFSK (GMSK if the frequency deviation is identical to a quarter of the data rate) */
#define MOD0_MOD_TYPE_ASK                   ((uint8_t)0x20) /*!< Modulation type ASK (OOK the PA is switched off for symbol "0") */
#define MOD0_MOD_TYPE_MSK                   ((uint8_t)0x00) /*!< Modulation type MSK (the frequency deviation must be identical to a quarter of the data rate) */
#define MOD0_MOD_TYPE_GMSK                  ((uint8_t)0x10) /*!< Modulation type GMSK (the frequency deviation must be identical to a quarter of the data rate) */
#define MOD0_BT_SEL_BT_MASK                 ((uint8_t)0x00) /*!< Select the BT = 1 or BT = 0.5 valid only for GFSK or GMSK modulation*/
#define MOD0_CW                             ((uint8_t)0x80) /*!< Set the Continous Wave (no modulation) transmit mode */

/**
 * @}
 */


/** @defgroup FDEV0_Register
 * @{
 */

/**
 *  \brief FDEV0 register
 *  \code
 *   Read Write
 *   Default value: 0x45
 *   7:4  FDEV_E[3:0]:       Exponent of the frequency deviation (allowed values from 0 to 9)
 *
 *   3 CLOCK_REC_ALGO_SEL:  Select PLL or DLL mode for clock recovery
 *                       1 - DLL mode
 *                       0 - PLL mode
 *
 *   2:0 FDEV_M[1:0]: Mantissa of the frequency deviation (allowed values from 0 to 7)
 *
 *
 *   \endcode
 */
#define    FDEV0_BASE                        ((uint8_t)0x1C) /*!< Sets the Mantissa and exponent of frequency deviation (frequency separation/2)
                                                                and PLL or DLL alogrithm from clock recovery in RX digital demod*/
#define FDEV0_CLOCK_REG_ALGO_SEL_MASK        ((uint8_t)0x08) /*!< Can be DLL or PLL algorithm for clock recovery in RX digital demod (see CLOCKREC reg) */
#define FDEV0_CLOCK_REG_ALGO_SEL_PLL         ((uint8_t)0x08) /*!< Sets PLL alogrithm for clock recovery in RX digital demod (see CLOCKREC reg) */
#define FDEV0_CLOCK_REG_ALGO_SEL_DLL         ((uint8_t)0x00) /*!< Sets DLL alogrithm for clock recovery in RX digital demod (see CLOCKREC reg) */
   
/**
 * @}
 */

/** @defgroup CHFLT_Register
 * @{
 */

/**
 *  \brief CHFLT register
 *  \code
 *   Read Write
 *   Default value: 0x23
 *   7:4  CHFLT_M[3:0]:       Mantissa of the channel filter BW (allowed values from 0 to 8)
 *
 *   3:0  CHFLT_E[3:0]:       Exponent of the channel filter BW (allowed values from 0 to 9)
 *
 *         M\E |   0   |   1   |   2   |   3   |   4  |   5  |  6   |   7 |  8  |  9  |
 *        -----+-------+-------+-------+-------+------+------+------+-----+-----+-----+
 *         0   | 800.1 | 450.9 | 224.7 | 112.3 | 56.1 | 28.0 | 14.0 | 7.0 | 3.5 | 1.8 |
 *         1   | 795.1 | 425.9 | 212.4 | 106.2 | 53.0 | 26.5 | 13.3 | 6.6 | 3.3 | 1.7 |
 *         2   | 768.4 | 403.2 | 201.1 | 100.5 | 50.2 | 25.1 | 12.6 | 6.3 | 3.1 | 1.6 |
 *         3   | 736.8 | 380.8 | 190.0 | 95.0  | 47.4 | 23.7 | 11.9 | 5.9 | 3.0 | 1.5 |
 *         4   | 705.1 | 362.1 | 180.7 | 90.3  | 45.1 | 22.6 | 11.3 | 5.6 | 2.8 | 1.4 |
 *         5   | 670.9 | 341.7 | 170.6 | 85.3  | 42.6 | 21.3 | 10.6 | 5.3 | 2.7 | 1.3 |
 *         6   | 642.3 | 325.4 | 162.4 | 81.2  | 40.6 | 20.3 | 10.1 | 5.1 | 2.5 | 1.3 |
 *         7   | 586.7 | 294.5 | 147.1 | 73.5  | 36.7 | 18.4 | 9.2  | 4.6 | 2.3 | 1.2 |
 *         8   | 541.4 | 270.3 | 135.0 | 67.5  | 33.7 | 16.9 | 8.4  | 4.2 | 2.1 | 1.1 |
 *
 *   \endcode
 */
#define    CHFLT_BASE                        ((uint8_t)0x1D) /*!< RX Channel Filter Bandwidth */

#define CHFLT_800_1                        ((uint8_t)0x00) /*!< RX Channel Filter Bandwidth = 800.1 kHz */
#define CHFLT_795_1                        ((uint8_t)0x10) /*!< RX Channel Filter Bandwidth = 795.1 kHz */
#define CHFLT_768_4                        ((uint8_t)0x20) /*!< RX Channel Filter Bandwidth = 768.4 kHz */
#define CHFLT_736_8                        ((uint8_t)0x30) /*!< RX Channel Filter Bandwidth = 736.8 kHz */
#define CHFLT_705_1                        ((uint8_t)0x40) /*!< RX Channel Filter Bandwidth = 705.1 kHz */
#define CHFLT_670_9                        ((uint8_t)0x50) /*!< RX Channel Filter Bandwidth = 670.9 kHz */
#define CHFLT_642_3                        ((uint8_t)0x60) /*!< RX Channel Filter Bandwidth = 642.3 kHz */
#define CHFLT_586_7                        ((uint8_t)0x70) /*!< RX Channel Filter Bandwidth = 586.7 kHz */
#define CHFLT_541_4                        ((uint8_t)0x80) /*!< RX Channel Filter Bandwidth = 541.4 kHz */
#define CHFLT_450_9                        ((uint8_t)0x01) /*!< RX Channel Filter Bandwidth = 450.9 kHz */
#define CHFLT_425_9                        ((uint8_t)0x11) /*!< RX Channel Filter Bandwidth = 425.9 kHz */
#define CHFLT_403_2                        ((uint8_t)0x21) /*!< RX Channel Filter Bandwidth = 403.2 kHz */
#define CHFLT_380_8                        ((uint8_t)0x31) /*!< RX Channel Filter Bandwidth = 380.8 kHz */
#define CHFLT_362_1                        ((uint8_t)0x41) /*!< RX Channel Filter Bandwidth = 362.1 kHz */
#define CHFLT_341_7                        ((uint8_t)0x51) /*!< RX Channel Filter Bandwidth = 341.7 kHz */
#define CHFLT_325_4                        ((uint8_t)0x61) /*!< RX Channel Filter Bandwidth = 325.4 kHz */
#define CHFLT_294_5                        ((uint8_t)0x71) /*!< RX Channel Filter Bandwidth = 294.5 kHz */
#define CHFLT_270_3                        ((uint8_t)0x81) /*!< RX Channel Filter Bandwidth = 270.3 kHz */
#define CHFLT_224_7                        ((uint8_t)0x02) /*!< RX Channel Filter Bandwidth = 224.7 kHz */
#define CHFLT_212_4                        ((uint8_t)0x12) /*!< RX Channel Filter Bandwidth = 212.4 kHz */
#define CHFLT_201_1                        ((uint8_t)0x22) /*!< RX Channel Filter Bandwidth = 201.1 kHz */
#define CHFLT_190                          ((uint8_t)0x32) /*!< RX Channel Filter Bandwidth = 190.0 kHz */
#define CHFLT_180_7                        ((uint8_t)0x42) /*!< RX Channel Filter Bandwidth = 180.7 kHz */
#define CHFLT_170_6                        ((uint8_t)0x52) /*!< RX Channel Filter Bandwidth = 170.6 kHz */
#define CHFLT_162_4                        ((uint8_t)0x62) /*!< RX Channel Filter Bandwidth = 162.4 kHz */
#define CHFLT_147_1                        ((uint8_t)0x72) /*!< RX Channel Filter Bandwidth = 147.1 kHz */
#define CHFLT_135                          ((uint8_t)0x82) /*!< RX Channel Filter Bandwidth = 135.0 kHz */
#define CHFLT_112_3                        ((uint8_t)0x03) /*!< RX Channel Filter Bandwidth = 112.3 kHz */
#define CHFLT_106_2                        ((uint8_t)0x13) /*!< RX Channel Filter Bandwidth = 106.2 kHz */
#define CHFLT_100_5                        ((uint8_t)0x23) /*!< RX Channel Filter Bandwidth = 100.5 kHz */
#define CHFLT_95                           ((uint8_t)0x33) /*!< RX Channel Filter Bandwidth = 95.0 kHz */
#define CHFLT_90_3                         ((uint8_t)0x43) /*!< RX Channel Filter Bandwidth = 90.3 kHz */
#define CHFLT_85_3                         ((uint8_t)0x53) /*!< RX Channel Filter Bandwidth = 85.3 kHz */
#define CHFLT_81_2                         ((uint8_t)0x63) /*!< RX Channel Filter Bandwidth = 81.2 kHz */
#define CHFLT_73_5                         ((uint8_t)0x73) /*!< RX Channel Filter Bandwidth = 73.5 kHz */
#define CHFLT_67_5                        ((uint8_t)0x83) /*!< RX Channel Filter Bandwidth = 67.5 kHz */
#define CHFLT_56_1                        ((uint8_t)0x04) /*!< RX Channel Filter Bandwidth = 56.1 kHz */
#define CHFLT_53                          ((uint8_t)0x14) /*!< RX Channel Filter Bandwidth = 53.0 kHz */
#define CHFLT_50_2                        ((uint8_t)0x24) /*!< RX Channel Filter Bandwidth = 50.2 kHz */
#define CHFLT_47_4                        ((uint8_t)0x34) /*!< RX Channel Filter Bandwidth = 47.4 kHz */
#define CHFLT_45_1                        ((uint8_t)0x44) /*!< RX Channel Filter Bandwidth = 45.1 kHz */
#define CHFLT_42_6                        ((uint8_t)0x54) /*!< RX Channel Filter Bandwidth = 42.6 kHz */
#define CHFLT_40_6                        ((uint8_t)0x64) /*!< RX Channel Filter Bandwidth = 40.6 kHz */
#define CHFLT_36_7                        ((uint8_t)0x74) /*!< RX Channel Filter Bandwidth = 36.7 kHz */
#define CHFLT_33_7                        ((uint8_t)0x84) /*!< RX Channel Filter Bandwidth = 33.7 kHz */
#define CHFLT_28                          ((uint8_t)0x05) /*!< RX Channel Filter Bandwidth = 28.0 kHz */
#define CHFLT_26_5                        ((uint8_t)0x15) /*!< RX Channel Filter Bandwidth = 26.5 kHz */
#define CHFLT_25_1                        ((uint8_t)0x25) /*!< RX Channel Filter Bandwidth = 25.1 kHz */
#define CHFLT_23_7                        ((uint8_t)0x35) /*!< RX Channel Filter Bandwidth = 23.7 kHz */
#define CHFLT_22_6                        ((uint8_t)0x45) /*!< RX Channel Filter Bandwidth = 22.6 kHz */
#define CHFLT_21_3                        ((uint8_t)0x55) /*!< RX Channel Filter Bandwidth = 21.3 kHz */
#define CHFLT_20_3                        ((uint8_t)0x65) /*!< RX Channel Filter Bandwidth = 20.3 kHz */
#define CHFLT_18_4                        ((uint8_t)0x75) /*!< RX Channel Filter Bandwidth = 18.4 kHz */
#define CHFLT_16_9                        ((uint8_t)0x85) /*!< RX Channel Filter Bandwidth = 16.9 kHz */
#define CHFLT_14                          ((uint8_t)0x06) /*!< RX Channel Filter Bandwidth = 14.0 kHz */
#define CHFLT_13_3                        ((uint8_t)0x16) /*!< RX Channel Filter Bandwidth = 13.3 kHz */
#define CHFLT_12_6                        ((uint8_t)0x26) /*!< RX Channel Filter Bandwidth = 12.6 kHz */
#define CHFLT_11_9                        ((uint8_t)0x36) /*!< RX Channel Filter Bandwidth = 11.9 kHz */
#define CHFLT_11_3                        ((uint8_t)0x46) /*!< RX Channel Filter Bandwidth = 11.3 kHz */
#define CHFLT_10_6                        ((uint8_t)0x56) /*!< RX Channel Filter Bandwidth = 10.6 kHz */
#define CHFLT_10_1                        ((uint8_t)0x66) /*!< RX Channel Filter Bandwidth = 10.1 kHz */
#define CHFLT_9_2                         ((uint8_t)0x76) /*!< RX Channel Filter Bandwidth = 9.2 kHz */
#define CHFLT_8_4                         ((uint8_t)0x86) /*!< RX Channel Filter Bandwidth = 8.4 kHz */
#define CHFLT_7                           ((uint8_t)0x07) /*!< RX Channel Filter Bandwidth = 7.0 kHz */
#define CHFLT_6_6                         ((uint8_t)0x17) /*!< RX Channel Filter Bandwidth = 6.6 kHz */
#define CHFLT_6_3                         ((uint8_t)0x27) /*!< RX Channel Filter Bandwidth = 6.3 kHz */
#define CHFLT_5_9                         ((uint8_t)0x37) /*!< RX Channel Filter Bandwidth = 5.9 kHz */
#define CHFLT_5_6                         ((uint8_t)0x47) /*!< RX Channel Filter Bandwidth = 5.6 kHz */
#define CHFLT_5_3                         ((uint8_t)0x57) /*!< RX Channel Filter Bandwidth = 5.3 kHz */
#define CHFLT_5_1                        ((uint8_t)0x67) /*!< RX Channel Filter Bandwidth = 5.1 kHz */
#define CHFLT_4_6                        ((uint8_t)0x77) /*!< RX Channel Filter Bandwidth = 4.6 kHz */
#define CHFLT_4_2                        ((uint8_t)0x87) /*!< RX Channel Filter Bandwidth = 4.2 kHz */
#define CHFLT_3_5                        ((uint8_t)0x08) /*!< RX Channel Filter Bandwidth = 3.5 kHz */
#define CHFLT_3_3                        ((uint8_t)0x18) /*!< RX Channel Filter Bandwidth = 3.3 kHz */
#define CHFLT_3_1                        ((uint8_t)0x28) /*!< RX Channel Filter Bandwidth = 3.1 kHz */
#define CHFLT_3                          ((uint8_t)0x38) /*!< RX Channel Filter Bandwidth = 3.0 kHz */
#define CHFLT_2_8                        ((uint8_t)0x48) /*!< RX Channel Filter Bandwidth = 2.8 kHz */
#define CHFLT_2_7                        ((uint8_t)0x58) /*!< RX Channel Filter Bandwidth = 2.7 kHz */
#define CHFLT_2_5                        ((uint8_t)0x68) /*!< RX Channel Filter Bandwidth = 2.5 kHz */
#define CHFLT_2_3                        ((uint8_t)0x78) /*!< RX Channel Filter Bandwidth = 2.3 kHz */
#define CHFLT_2_1                        ((uint8_t)0x88) /*!< RX Channel Filter Bandwidth = 2.1 kHz */
#define CHFLT_1_8                        ((uint8_t)0x09) /*!< RX Channel Filter Bandwidth = 1.8 kHz */
#define CHFLT_1_7                        ((uint8_t)0x19) /*!< RX Channel Filter Bandwidth = 1.7 kHz */
#define CHFLT_1_6                        ((uint8_t)0x29) /*!< RX Channel Filter Bandwidth = 1.6 kHz */
#define CHFLT_1_5                        ((uint8_t)0x39) /*!< RX Channel Filter Bandwidth = 1.5 kHz */
#define CHFLT_1_4                        ((uint8_t)0x49) /*!< RX Channel Filter Bandwidth = 1.4 kHz */
#define CHFLT_1_3a                       ((uint8_t)0x59) /*!< RX Channel Filter Bandwidth = 1.3 kHz */
#define CHFLT_1_3                        ((uint8_t)0x69) /*!< RX Channel Filter Bandwidth = 1.3 kHz */
#define CHFLT_1_2                        ((uint8_t)0x79) /*!< RX Channel Filter Bandwidth = 1.2 kHz */
#define CHFLT_1_1                        ((uint8_t)0x89) /*!< RX Channel Filter Bandwidth = 1.1 kHz */

/**
 * @}
 */

/** @defgroup AFC2_Register
 * @{
 */

/**
 *  \brief AFC2 register
 *  \code
 *   Read Write
 *   Default value: 0x48
 *   7  AFC Freeze on Sync: Freeze AFC correction upon sync word detection.
 *                       1 - AFC Freeze enabled
 *                       0 - AFC Freeze disabled
 *
 *   6  AFC Enabled:  Enable AFC
 *                       1 - AFC enabled
 *                       0 - AFC disabled
 *
 *   5  AFC Mode:    Select AFC mode
 *                       1 - AFC Loop closed on 2nd conversion stage.
 *                       0 - AFC Loop closed on slicer
 *
 *   4:0  AFC PD leakage[4:0]: Peak detector leakage. This parameter sets the decay speed of the min/max frequency peak detector (AFC2 register),
 *                             the range allowed is 0..31 (0 - no leakage, 31 - high leakage). The recommended value for this parameter is 4.
 *
 *   \endcode
 */
#define    AFC2_BASE                        ((uint8_t)0x1E) /*!< Automatic frequency compensation algorithm parameters (FSK/GFSK/MSK)*/

#define AFC2_AFC_FREEZE_ON_SYNC_MASK        ((uint8_t)0x80) /*!< The frequency correction value is frozen when SYNC word is detected */
#define AFC2_AFC_MASK                       ((uint8_t)0x40) /*!< Mask of Automatic Frequency Correction */
#define AFC2_AFC_MODE_MASK                  ((uint8_t)0x20) /*!< Automatic Frequency Correction can be in Main MODE or Auxiliary MODE*/
#define AFC2_AFC_MODE_SLICER                ((uint8_t)0x00) /*!< Automatic Frequency Correction Main MODE */
#define AFC2_AFC_MODE_MIXER                 ((uint8_t)0x20) /*!< Automatic Frequency Correction Auxiliary MODE */
   
/**
 * @}
 */

/** @defgroup AFC1_Register
 * @{
 */

/**
 *  \brief AFC1 register
 *  \code
 *   Read Write
 *   Default value: 0x18
 *   7:0  AFC_FAST_PERIOD: Length of the AFC fast period. this parameter sets the length of the fast period in number of samples (AFC1 register), the range allowed
 *                         is 0..255. The recommended setting for this parameter is such that the fast period equals the preamble length. Since the
 *                         algorithm operates typically on 2 samples per symbol, the programmed value should be twice the number of preamble
 *                         symbols.
 *
 *   \endcode
 */
#define    AFC1_BASE                        ((uint8_t)0x1F) /*!< Length of the AFC fast period */

/**
 * @}
 */

/** @defgroup AFC0_Register
 * @{
 */

/**
 *  \brief AFC0 register
 *  \code
 *   Read Write
 *   Default value: 0x25
 *   7:4  AFC_FAST_GAIN_LOG2[3:0]: AFC loop gain in fast mode (2's log)
 *
 *   3:0  AFC_SLOW_GAIN_LOG2[3:0]: AFC loop gain in slow mode (2's log)
 *
 *   \endcode
 */
#define    AFC0_BASE                        ((uint8_t)0x20) /*!< AFC loop gain in fast and slow modes (2's log) */

/**
 * @}
 */

/** @defgroup CLOCKREC_Register
 * @{
 */

/**
 *  \brief CLOCKREC register
 *  \code
 *   Read Write
 *   Default value: 0x58
 *
 *   7:5 CLK_REC_P_GAIN [2:0]: Clock recovery loop gain (log2)
 *
 *   4   PSTFLT_LEN: Set Postfilter length
 *                       1 - 16 symbols
 *                       0 - 8 symbols
 *
 *   3:0 CLK_REC_I_GAIN[3:0]: Integral gain for the clock recovery loop
 *   \endcode
 */

#define    CLOCKREC_BASE                        ((uint8_t)0x23) /*!< Gain of clock recovery loop - Postfilter length 0-8 symbols, 1-16 symbols */

/**
 * @}
 */

/** @defgroup AGCCTRL2_Register
 * @{
 */

/**
 *  \brief AGCCTRL2 register
 *  \code
 *   Read Write
 *   Default value: 0x22
 *
 *   7   Reserved
 *
 *   6   FREEZE_ON_STEADY: Enable freezing on steady state
 *                       1 - Enable
 *                       0 - Disable
 *
 *   5   FREEZE_ON_SYNC: Enable freezing on sync detection
 *                       1 - Enable
 *                       0 - Disable
 *
 *   4   START_MAX_ATTENUATION: Start with max attenuation
 *                       1 - Enable
 *                       0 - Disable
 *
 *   3:0  MEAS_TIME[3:0]: Measure time during which the signal peak is detected (according to the formula 12/fxo*2^MEAS_TIME)
 *   \endcode
 */
#define    AGCCTRL2_BASE                        ((uint8_t)0x24) /*!< AGC freeze strategy, AGC attenuation  strategy, AGC measure time */

#define AGCCTRL2_FREEZE_ON_STEADY_MASK          ((uint8_t)0x40) /*!< The attenuation settings will be frozen as soon as signal level
                                             is betweeen min and max treshold (see AGCCTRL1) */
#define AGCCTRL2_FREEZE_ON_SYNC_MASK            ((uint8_t)0x20) /*!< The attenuation settings will be frozen as soon sync word is detected */
#define AGCCTRL2_START_MAX_ATTENUATION_MASK     ((uint8_t)0x10) /*!< The AGC algorithm can start with MAX attenuation or MIN attenuation */

/**
 * @}
 */

/** @defgroup AGCCTRL1_Register
 * @{
 */

/**
 *  \brief AGCCTRL1 register
 *  \code
 *   Read Write
 *   Default value: 0x65
 *
 *   7:4   THRESHOLD_HIGH[3:0]: High threshold for the AGC
 *
 *   3:0   THRESHOLD_LOW[3:0]: Low threshold for the AGC
 *   \endcode
 */
#define    AGCCTRL1_BASE                        ((uint8_t)0x25) /*!< Sets low and high threshold for AGC */

/**
 * @}
 */

/** @defgroup AGCCTRL0_Register
 * @{
 */

/**
 *  \brief AGCCTRL0 register
 *  \code
 *   Read Write
 *   Default value: 0x8A
 *
 *   7   AGC S_ENABLE: Enable AGC
 *                       1 - Enable
 *                       0 - Disable
 *
 *   6   AGC_MODE: Set linear-Binary AGC mode
 *                       1 - Enable
 *                       0 - Disable
 *
 *   5:0 HOLD_TIME[5:0]: Hold time after gain adjustment according to formula 12/fxo*HOLD_TIME
 *   \endcode
 */
#define    AGCCTRL0_BASE                        ((uint8_t)0x26) /*!< Enables AGC, set AGC algo between linear/binary mode, set hold time
                                                                    to account signal propagation through RX chain */
#define AGCCTRL0_AGC_MASK                       ((uint8_t)0x80) /*!< AGC on/off */
#define AGCCTRL0_AGC_MODE_MASK                  ((uint8_t)0x40) /*!< AGC search correct attenuation in binary mode or sequential mode */
#define AGCCTRL0_AGC_MODE_LINEAR                ((uint8_t)0x00) /*!< AGC search correct attenuation in sequential mode (recommended) */
#define AGCCTRL0_AGC_MODE_BINARY                ((uint8_t)0x40) /*!< AGC search correct attenuation in binary mode */

/**
 * @}
 */

/** @defgroup CHNUM_Register
 * @{
 */

/**
 *  \brief CHNUM  registers
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:0     CH_NUM[7:0]: Channel number. This value is multiplied by the channel spacing and added to the
 *                        synthesizer base frequency to generate the actual RF carrier frequency.
 *   \endcode
 */
#define    CHNUM_BASE                        ((uint8_t)0x6C) /*!< Channel number. This value is multiplied by the channel
                                        spacing and added to the synthesizer base frequency to generate the actual RF carrier frequency */
/**
 * @}
 */

/** @defgroup AFC_CORR_Register
 * @{
 */

/**
 *  \brief AFC_CORR  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7:0   AFC_CORR[7:0]: AFC word of the received packet
 *   \endcode
 */
#define    AFC_CORR_BASE                        ((uint8_t)(0xC4)) /*!< AFC word of the received packet */

/**
 * @}
 */

/**
 * @}
 */


/** @defgroup Packet_Configuration_Registers
 * @{
 */

/** @defgroup PCKTCTRL4_Register
 * @{
 */

/**
 *  \brief PCKTCTRL4 register
 *  \code
 *   Read Write
 *   Default value: 0x00
 *
 *   7:5   NOT_USED.
 *
 *   4:3   ADDRESS_LEN[1:0]: length of address field in bytes
 *
 *   2:0   control_len[2:0]: length of control field in bytes
 *   \endcode
 */
#define    PCKTCTRL4_BASE                        ((uint8_t)0x30) /*!< lenghts of address and control field */

#define PCKTCTRL4_ADDRESS_LEN_MASK               ((uint8_t)0x18)
#define PCKTCTRL4_CONTROL_LEN_MASK               ((uint8_t)0x07)

/**
 * @}
 */

/** @defgroup PCKTCTRL3_Register
 * @{
 */

/**
 *  \brief PCKTCTRL3 register
 *  \code
 *   Read Write
 *   Default value: 0x07
 *
 *   7:6   PCKT_FRMT[1:0]: format of packet
 *
 *          PCKT_FRMT1       |        PCKT_FRMT0       |        Format
 *       ----------------------------------------------------------------------
 *             0             |           0             |        BASIC
 *             1             |           0             |        MBUS
 *             1             |           1             |        STACK
 *
 *   5:4   RX_MODE[1:0]: length of address 0x30 field in bytes
 *
 *          RX_MODE1       |        RX_MODE0       |            Rx Mode
 *       --------------------------------------------------------------------
 *             0           |           0           |              normal
 *             0           |           1           |       direct through FIFO
 *             1           |           0           |       direct through GPIO
 *
 *   3:0   LEN_WID[3:0]: length of length field in bits
 *   \endcode
 */
#define    PCKTCTRL3_BASE                        ((uint8_t)0x31) /*!< packet format, RX mode, lenght of length field */

#define    PCKTCTRL3_PCKT_FRMT_BASIC             ((uint8_t)0x00) /*!< Basic Packet Format */
#define    PCKTCTRL3_PCKT_FRMT_MBUS              ((uint8_t)0x80) /*!< Wireless M-BUS Packet Format */
#define    PCKTCTRL3_PCKT_FRMT_STACK             ((uint8_t)0xC0) /*!< STack Packet Format */

#define    PCKTCTRL3_RX_MODE_NORMAL              ((uint8_t)0x00) /*!< Normal RX Mode */
#define    PCKTCTRL3_RX_MODE_DIRECT_FIFO         ((uint8_t)0x10) /*!< RX Direct Mode; data available through FIFO */
#define    PCKTCTRL3_RX_MODE_DIRECT_GPIO         ((uint8_t)0x20) /*!< RX Direct Mode; data available through selected GPIO */

#define PCKTCTRL3_PKT_FRMT_MASK                  ((uint8_t)0xC0)
#define PCKTCTRL3_RX_MODE_MASK                   ((uint8_t)0x30)
#define PCKTCTRL3_LEN_WID_MASK                   ((uint8_t)0x0F)

/**
 * @}
 */

/** @defgroup PCKTCTRL2_Register
 * @{
 */

/**
 *  \brief PCKTCTRL2 register
 *  \code
 *   Read Write
 *   Default value: 0x1E
 *
 *   7:3   PREAMBLE_LENGTH[4:0]: length of preamble field in bytes (0..31)
 *
 *
 *   2:1   SYNC_LENGTH[1:0]: length of sync field in bytes
 *
 *
 *   0     FIX_VAR_LEN: fixed/variable packet length
 *                       1 - Variable
 *                       0 - Fixed
 *   \endcode
 */
#define    PCKTCTRL2_BASE                        ((uint8_t)0x32) /*!< length of preamble and sync fields (in bytes), fix or variable packet length */
   
#define    PCKTCTRL2_FIX_VAR_LEN_MASK            ((uint8_t)0x01) /*!< Enable/disable the length mode */
#define PCKTCTRL2_PREAMBLE_LENGTH_MASK           ((uint8_t)0xF8)
#define PCKTCTRL2_SYNC_LENGTH_MASK               ((uint8_t)0x06)

/**
 * @}
 */

/** @defgroup PCKTCTRL1_Register
 * @{
 */

/**
 *  \brief PCKTCTRL1 register
 *  \code
 *   Read Write
 *   Default value: 0x20
 *
 *   7:5   CRC_MODE[2:0]: CRC type (0, 8, 16, 24 bits)
 *
 *          CRC_MODE2     |       CRC_MODE1     |        CRC_MODE0     |       CRC Mode  (n. bits - poly)
 *       -------------------------------------------------------------------------------------------------
 *             0           |         0          |          1           |       8 - 0x07
 *             0           |         1          |          0           |       16 -  0x8005
 *             0           |         1          |          1           |       16 - 0x1021
 *             1           |         0          |          0           |       24 - 0x864CBF
 *
 *   4     WHIT_EN[0]: Enable Whitening
 *                       1 - Enable
 *                       0 - Disable
 *
 *   3:2   TX_SOURCE[1:0]: length of sync field in bytes
 *
 *          TX_SOURCE1     |        TX_SOURCE0     |        Tx Mode
 *       --------------------------------------------------------------------
 *             0           |           0           |       normal
 *             0           |           1           |       direct through FIFO
 *             1           |           0           |       direct through GPIO
 *             1           |           1           |       pn9
 *
 *   1   NOT_USED
 *
 *   0   FEC_EN: enable FEC
 *                       1 - FEC in TX , Viterbi decoding in RX
 *                       0 - Disabled
 *   \endcode
 */
#define    PCKTCTRL1_BASE                        ((uint8_t)0x33) /*!< CRC type, whitening enable, TX mode */

#define    PCKTCTRL1_FEC_MASK                    ((uint8_t)0x01) /*!< Enable/disable the Forward Error Correction */
#define PCKTCTRL1_TX_SOURCE_MASK                 ((uint8_t)0x0C) /*!< TX source mode */
#define PCKTCTRL1_CRC_MODE_MASK                  ((uint8_t)0xE0) /*!< CRC type */
#define PCKTCTRL1_WHIT_MASK                      ((uint8_t)0x10) /*!< Enable/disable the Whitening */

/**
 * @}
 */



/** @defgroup PCKTLEN1_Register
 * @{
 */

/**
 *  \brief PCKTLEN1 register
 *  \code
 *   Read Write
 *   Default value: 0x00
 *
 *   7:0   pktlen1[7:0]: lenght of packet in bytes (upper field) LENGHT/256
 *   \endcode
 */
#define    PCKTLEN1_BASE                        ((uint8_t)0x34) /*!< lenght of packet in bytes (upper field) */

/**
 * @}
 */

/** @defgroup PCKTLEN0_Register
 * @{
 */

/**
 *  \brief PCKTLEN0 register
 *  \code
 *   Read Write
 *   Default value: 0x14
 *
 *   7:0   pktlen0[7:0]: lenght of packet in bytes (lower field) LENGHT%256
 *   \endcode
 */
#define    PCKTLEN0_BASE                        ((uint8_t)0x35) /*!< lenght of packet in bytes (lower field) [PCKTLEN=PCKTLEN1x256+PCKTLEN0]*/

/**
 * @}
 */

/** @defgroup SYNCx_Registers
 * @{
 */
/**
 *  \brief SYNCx[4:1] Registers
 *  \code
 *   Read Write
 *   Default value: 0x88
 *
 *   7:0   SYNCx[7:0]: xth sync word
 *   \endcode
 */
#define    SYNC4_BASE                        ((uint8_t)0x36) /*!< Sync word 4 */
#define    SYNC3_BASE                        ((uint8_t)0x37) /*!< Sync word 3 */
#define    SYNC2_BASE                        ((uint8_t)0x38) /*!< Sync word 2 */
#define    SYNC1_BASE                        ((uint8_t)0x39) /*!< Sync word 1 */

/**
 * @}
 */


/** @defgroup MBUS_PRMBL_Register
 * @{
 */

/**
 *  \brief MBUS_PRMBL register
 *  \code
 *   Read Write
 *   Default value: 0x20
 *
 *   7:0 MBUS_PRMBL[7:0]: MBUS preamble control
 *   \endcode
 */
#define    MBUS_PRMBL_BASE                    ((uint8_t)0x3B) /*!< MBUS preamble lenght (in 01 bit pairs) */

/**
 * @}
 */


/** @defgroup MBUS_PSTMBL_Register
 * @{
 */

/**
 *  \brief MBUS_PSTMBL register
 *  \code
 *   Read Write
 *   Default value: 0x20
 *
 *   7:0 MBUS_PSTMBL[7:0]: MBUS postamble control
 *   \endcode
 */
#define    MBUS_PSTMBL_BASE                    ((uint8_t)0x3C) /*!< MBUS postamble length (in 01 bit pairs) */

/**
 * @}
 */

/** @defgroup MBUS_CTRL_Register
 * @{
 */

/**
 *  \brief MBUS_CTRL register
 *  \code
 *   Read Write
 *   Default value: 0x00
 *
 *   7:4   NOT_USED
 *
 *   3:1   MBUS_SUBMODE[2:0]: MBUS submode (allowed values are 0,1,3,5)
 *
 *   0     NOT_USED
 *   \endcode
 */
#define    MBUS_CTRL_BASE                          ((uint8_t)0x3D) /*!< MBUS sub-modes (S1, S2 short/long header, T1, T2, R2) */

#define    MBUS_CTRL_MBUS_SUBMODE_S1_S2L           ((uint8_t)0x00) /*!< MBUS sub-modes S1 & S2L, header lenght min 279, sync 0x7696, Manchester */
#define    MBUS_CTRL_MBUS_SUBMODE_S2_S1M_T2_OTHER  ((uint8_t)0x02) /*!< MBUS sub-modes S2, S1-m, T2 (only other to meter) short header, header lenght min 15, sync 0x7696, Manchester */
#define    MBUS_CTRL_MBUS_SUBMODE_T1_T2_METER      ((uint8_t)0x06) /*!< MBUS sub-modes T1, T2 (only meter to other), header lenght min 19, sync 0x3D, 3 out of 6 */
#define    MBUS_CTRL_MBUS_SUBMODE_R2               ((uint8_t)0x0A) /*!< MBUS sub-mode R2, header lenght min 39, sync 0x7696, Manchester */

/**
 * @}
 */



/** @defgroup PCKT_FLT_GOALS_CONTROLx_MASK_Registers
 * @{
 */

/**
 *  \brief PCKT_FLT_GOALS_CONTROLx_MASK  registers
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:0    CONTROLx_MASK[7:0]:   All 0s - no filtering
 *
 *   \endcode
 */
#define    PCKT_FLT_GOALS_CONTROL0_MASK_BASE            ((uint8_t)0x42) /*!< Packet control field #3 mask, all 0s -> no filtering */

#define    PCKT_FLT_GOALS_CONTROL1_MASK_BASE            ((uint8_t)0x43) /*!< Packet control field #2 mask, all 0s -> no filtering */

#define    PCKT_FLT_GOALS_CONTROL2_MASK_BASE            ((uint8_t)0x44) /*!< Packet control field #1 mask, all 0s -> no filtering */

#define    PCKT_FLT_GOALS_CONTROL3_MASK_BASE            ((uint8_t)0x45) /*!< Packet control field #0 mask, all 0s -> no filtering */

/**
 * @}
 */

/** @defgroup PCKT_FLT_GOALS_CONTROLx_FIELD_Registers
 * @{
 */

/**
 *  \brief PCKT_FLT_GOALS_CONTROLx_FIELD  registers
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:0    CONTROLx_FIELD[7:0]:   Control field (byte x) to be used as reference
 *
 *   \endcode
 */
#define    PCKT_FLT_GOALS_CONTROL0_FIELD_BASE            ((uint8_t)0x46) /*!< Control field (byte #3) */

#define    PCKT_FLT_GOALS_CONTROL1_FIELD_BASE            ((uint8_t)0x47) /*!< Control field (byte #2) */

#define    PCKT_FLT_GOALS_CONTROL2_FIELD_BASE            ((uint8_t)0x48) /*!< Control field (byte #1) */

#define    PCKT_FLT_GOALS_CONTROL3_FIELD_BASE            ((uint8_t)0x49) /*!< Control field (byte #0) */

/**
 * @}
 */

/** @defgroup PCKT_FLT_GOALS_SOURCE_MASK_Register
 * @{
 */

/**
 *  \brief PCKT_FLT_GOALS_SOURCE_MASK  register
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:0    RX_SOURCE_MASK[7:0]:   For received packet only: all 0s - no filtering
 *
 *   \endcode
 */
#define    PCKT_FLT_GOALS_SOURCE_MASK_BASE                ((uint8_t)0x4A) /*!< Source address mask, valid in RX mode */

/**
 * @}
 */

/** @defgroup PCKT_FLT_GOALS_SOURCE_ADDR_Register
 * @{
 */
/**
 *  \brief PCKT_FLT_GOALS_SOURCE_ADDR  register
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:0    RX_SOURCE_ADDR[7:0]:  RX packet source / TX packet destination fields
 *
 *   \endcode
 */
#define    PCKT_FLT_GOALS_SOURCE_ADDR_BASE                ((uint8_t)0x4B) /*!< Source address */

/**
 * @}
 */

/** @defgroup PCKT_FLT_GOALS_BROADCAST_Register
 * @{
 */

/**
 *  \brief PCKT_FLT_GOALS_BROADCAST  register
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:0    BROADCAST[7:0]:  Address shared for broadcast communication link
 *
 *   \endcode
 */
#define    PCKT_FLT_GOALS_BROADCAST_BASE                ((uint8_t)0x4C) /*!< Address shared for broadcast communication links */

/**
 * @}
 */

/** @defgroup PCKT_FLT_GOALS_MULTICAST_Register
 * @{
 */

/**
 *  \brief PCKT_FLT_GOALS_MULTICAST  register
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:0    MULTICAST[7:0]:  Address shared for multicast communication links
 *
 *   \endcode
 */
#define    PCKT_FLT_GOALS_MULTICAST_BASE                ((uint8_t)0x4D) /*!< Address shared for multicast communication links */

/**
 * @}
 */

/** @defgroup PCKT_FLT_GOALS_TX_SOURCE_ADDR_Register
 * @{
 */

/**
 *  \brief PCKT_FLT_GOALS_TX_SOURCE_ADDR  register
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:0    TX_SOURCE_ADDR[7:0]:  TX packet source / RX packet destination fields
 *
 *   \endcode
 */
#define    PCKT_FLT_GOALS_TX_ADDR_BASE                ((uint8_t)0x4E) /*!< Address of the destination (also device own address) */

/**
 * @}
 */

/** @defgroup PCKT_FLT_OPTIONS_Register
 * @{
 */

/**
 *  \brief PCKT_FLT_OPTIONS  register
 *  \code
 *   Default value: 0x70
 *   Read Write
 *   7   Reserved.
 *
 *   6   RX_TIMEOUT_AND_OR_SELECT[0]:  1 - ‘OR’ logical function applied to CS/SQI/PQI
 *                                     values (masked by 7:5 bits in PROTOCOL register)
 *   5   CONTROL_FILTERING[0]:         1 - RX packet accepted if its control fields matches
 *                                     with masked CONTROLx_FIELD registers.
 *   4   SOURCE_FILTERING[0]:          1 - RX packet accepted if its source field
 *                                     matches w/ masked RX_SOURCE_ADDR register.
 *   3   DEST_VS_ SOURCE _ADDR[0]:   1 - RX packet accepted if its destination
 *                                   address matches with TX_SOURCE_ADDR reg.
 *   2   DEST_VS_MULTICAST_ADDR[0]:  1 - RX packet accepted if its destination
 *                                   address matches with MULTICAST register
 *   1   DEST_VS_BROADCAST_ADDR[0]:  1 - RX packet accepted if its destination
 *                                   address matches with BROADCAST register.
 *   0   CRC_CHECK[0]:               1 - packet discarded if CRC not valid.
 *
 *   \endcode
 */
#define    PCKT_FLT_OPTIONS_BASE                          ((uint8_t)0x4F) /*!< Options relative to packet filtering */

#define    PCKT_FLT_OPTIONS_CRC_CHECK_MASK                ((uint8_t)0x01) /*!< Enable/disable of CRC check: packet is discarded if CRC is not valid [RX] */
#define    PCKT_FLT_OPTIONS_DEST_VS_BROADCAST_ADDR_MASK   ((uint8_t)0x02) /*!< Packet discarded if destination address differs from BROADCAST register [RX] */
#define    PCKT_FLT_OPTIONS_DEST_VS_MULTICAST_ADDR_MASK   ((uint8_t)0x04) /*!< Packet discarded if destination address differs from MULTICAST register [RX] */
#define    PCKT_FLT_OPTIONS_DEST_VS_TX_ADDR_MASK          ((uint8_t)0x08) /*!< Packet discarded if destination address differs from TX_ADDR register [RX] */
#define    PCKT_FLT_OPTIONS_SOURCE_FILTERING_MASK         ((uint8_t)0x10) /*!< Packet discarded if source address (masked by the SOURCE_MASK register)
                                                                                     differs from SOURCE_ADDR register [RX] */
#define    PCKT_FLT_OPTIONS_CONTROL_FILTERING_MASK        ((uint8_t)0x20) /*!< Packet discarded if the x-byte (x=1¸4) control field  (masked by the CONTROLx_MASK register)
                                                                                     differs from CONTROLx_FIELD register [RX] */
#define    PCKT_FLT_OPTIONS_RX_TIMEOUT_AND_OR_SELECT      ((uint8_t)0x40) /*!< Logical function applied to CS/SQI/PQI values (masked by [7:5] bits in PROTOCOL[2]
                                                                                     register) */

/**
 * @}
 */

/** @defgroup TX_CTRL_FIELD_Registers
 * @{
 */

/**
 *  \brief TX_CTRL_FIELDx  registers
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:0     TX_CTRLx[7:0]: Control field value to be used in TX packet as byte n.x
 *   \endcode
 */
#define    TX_CTRL_FIELD3_BASE                    ((uint8_t)0x68) /*!< Control field value to be used in TX packet as byte n.3 */

#define    TX_CTRL_FIELD2_BASE                    ((uint8_t)0x69) /*!< Control field value to be used in TX packet as byte n.2 */

#define    TX_CTRL_FIELD1_BASE                    ((uint8_t)0x6A) /*!< Control field value to be used in TX packet as byte n.1 */

#define    TX_CTRL_FIELD0_BASE                    ((uint8_t)0x6B) /*!< Control field value to be used in TX packet as byte n.0 */

/**
 * @}
 */


/** @defgroup TX_PCKT_INFO_Register
 * @{
 */

/**
 *  \brief TX_PCKT_INFO  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7:6     Not used.
 *
 *   5:4     TX_SEQ_NUM:   Current TX packet sequence number
 *
 *   0       N_RETX[3:0]:  Number of retransmissions done on the
 *                        last TX packet
 *   \endcode
 */
#define    TX_PCKT_INFO_BASE                    ((uint8_t)(0xC2)) /*!< Current TX packet sequence number [5:4];
                                                                                        Number of retransmissions done on the last TX packet [3:0]*/
/**
 * @}
 */

/** @defgroup RX_PCKT_INFO_Register
 * @{
 */

/**
 *  \brief RX_PCKT_INFO  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7:3     Not used.
 *
 *   2       NACK_RX:   NACK field of the received packet
 *
 *   1:0     RX_SEQ_NUM[1:0]:  Sequence number of the received packet
 *   \endcode
 */
#define    RX_PCKT_INFO_BASE                    ((uint8_t)(0xC3)) /*!< NO_ACK field of the received packet [2];
                                                                                       sequence number of the received packet [1:0]*/

#define    TX_PCKT_INFO_NACK_RX                 ((uint8_t)(0x04)) /*!< NACK field of the received packet */

/**
 * @}
 */

/** @defgroup RX_PCKT_LEN1
 * @{
 */

/**
 *  \brief RX_PCKT_LEN1  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7:0   RX_PCKT_LEN1[7:0]:  Length (number of bytes) of the received packet: RX_PCKT_LEN=RX_PCKT_LEN1 × 256 + RX_PCKT_LEN0
 *                             This value is packet_length/256
 *   \endcode
 */
#define    RX_PCKT_LEN1_BASE                    ((uint8_t)(0xC9)) /*!< Length (number of  bytes) of the received packet: */

/**
 * @}
 */

/** @defgroup RX_PCKT_LEN0
 * @{
 */

/**
 *  \brief RX_PCKT_LEN0  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7:0   RX_PCKT_LEN0[7:0]:  Length (number of bytes) of the received packet: RX_PCKT_LEN=RX_PCKT_LEN1 × 256 + RX_PCKT_LEN0
 *                             This value is packet_length%256
 *   \endcode
 */
#define    RX_PCKT_LEN0_BASE                    ((uint8_t)(0xCA)) /*!< RX_PCKT_LEN=RX_PCKT_LEN1 × 256 + RX_PCKT_LEN0 */

/**
 * @}
 */


/** @defgroup CRC_FIELD_Register
 * @{
 */

/**
 *  \brief CRC_FIELD[2:0]  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7:0   CRC_FIELDx[7:0]:    upper(x=2), middle(x=1) and lower(x=0) part of the crc field of the received packet
 *  \endcode
 */
#define    CRC_FIELD2_BASE                        ((uint8_t)(0xCB)) /*!< CRC2 field of the received packet */

#define    CRC_FIELD1_BASE                        ((uint8_t)(0xCC)) /*!< CRC1 field of the received packet */

#define    CRC_FIELD0_BASE                        ((uint8_t)(0xCD)) /*!< CRC0 field of the received packet */

/**
 * @}
 */

/** @defgroup RX_CTRL_FIELD_Register
 * @{
 */

/**
 *  \brief RX_CTRL_FIELD[3:0]  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7:0   RX_CTRL_FIELDx[7:0]:    upper(x=3), middle(x=2), middle(x=1) and lower(x=0) part of the control field of the received packet
 *   \endcode
 */
#define    RX_CTRL_FIELD0_BASE                    ((uint8_t)(0xCE)) /*!< CRTL3 Control field of the received packet */

#define    RX_CTRL_FIELD1_BASE                    ((uint8_t)(0xCF)) /*!< CRTL2 Control field of the received packet */

#define    RX_CTRL_FIELD2_BASE                    ((uint8_t)(0xD0)) /*!< CRTL1 Control field of the received packet */

#define    RX_CTRL_FIELD3_BASE                    ((uint8_t)(0xD1)) /*!< CRTL0 Control field of the received packet */

/**
 * @}
 */

/** @defgroup RX_ADDR_FIELD_Register
 * @{
 */

/**
 *  \brief RX_ADDR_FIELD[1:0]  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7:0   RX_ADDR_FIELDx[7:0]:    source(x=1) and destination(x=0) address field of the received packet
 *   \endcode
 */
#define    RX_ADDR_FIELD1_BASE                    ((uint8_t)(0xD2)) /*!< ADDR1 Address field of the received packet */

#define    RX_ADDR_FIELD0_BASE                    ((uint8_t)(0xD3)) /*!< ADDR0 Address field of the received packet */

/**
 * @}
 */

/**
 * @}
 */


/** @defgroup Protocol_Registers
 * @{
 */

/** @defgroup PROTOCOL2_Register
 * @{
 */

/**
 *  \brief PROTOCOL2  register
 *  \code
 *   Default value: 0x06
 *   Read Write
 *   7   CS_TIMEOUT_MASK:              1 - CS value contributes to timeout disabling
 *
 *   6   SQI_TIMEOUT_MASK:             1 - SQI value contributes to timeout disabling
 *
 *   5   PQI_TIMEOUT_MASK:             1 - PQI value contributes to timeout disabling
 *
 *   4:3  TX_SEQ_NUM_RELOAD[1:0]:      TX sequence number to be used when counting reset is required using the related command.
 *
 *   2   RCO_CALIBRATION[0]:           1 - Enables the automatic RCO calibration
 *
 *   1   VCO_CALIBRATION[0]:           1 - Enables the automatic VCO calibration
 *
 *   0   LDCR_MODE[0]:                 1 - LDCR mode enabled
 *
 *   \endcode
 */
#define    PROTOCOL2_BASE                        ((uint8_t)0x50) /*!< Protocol2 regisetr address */

#define    PROTOCOL2_LDC_MODE_MASK               ((uint8_t)0x01) /*!< Enable/disable Low duty Cycle mode */
#define    PROTOCOL2_VCO_CALIBRATION_MASK        ((uint8_t)0x02) /*!< Enable/disable VCO automatic calibration */
#define    PROTOCOL2_RCO_CALIBRATION_MASK        ((uint8_t)0x04) /*!< Enable/disable RCO automatic calibration */
#define    PROTOCOL2_PQI_TIMEOUT_MASK            ((uint8_t)0x20) /*!< PQI value contributes to timeout disabling */
#define    PROTOCOL2_SQI_TIMEOUT_MASK            ((uint8_t)0x40) /*!< SQI value contributes to timeout disabling */
#define    PROTOCOL2_CS_TIMEOUT_MASK             ((uint8_t)0x80) /*!< CS value contributes to timeout disabling */
  
/**
 * @}
 */

/** @defgroup PROTOCOL1_Register
 * @{
 */

/**
 *  \brief PROTOCOL1  register
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7   LDCR_RELOAD_ON_SYNC:          1 - LDCR timer will be reloaded with the value stored in the LDCR_RELOAD registers
 *
 *   6   PIGGYBACKING:                 1 - PIGGYBACKING enabled
 *
 *   5:4   Reserved.
 *
 *   3   SEED_RELOAD[0]:               1 - Reload the back-off random generator
 *                                        seed using the value written in the
 *                                        BU_COUNTER_SEED_MSByte / LSByte registers
 *
 *   2   CSMA_ON   [0]:                1 - CSMA channel access mode enabled
 *
 *   1   CSMA_PERS_ON[0]:              1 - CSMA persistent (no back-off) enabled
 *
 *   0   AUTO_PCKT_FLT[0]:             1 - automatic packet filtering mode enabled
 *
 *   \endcode
 */
#define    PROTOCOL1_BASE                        ((uint8_t)0x51) /*!< Protocol1 regisetr address */

#define    PROTOCOL1_AUTO_PCKT_FLT_MASK          ((uint8_t)0x01) /*!< Enable/disable automatic packet filtering mode */
#define    PROTOCOL1_CSMA_PERS_ON_MASK           ((uint8_t)0x02) /*!< Enable/disable CSMA persistent (no back-off)  */
#define    PROTOCOL1_CSMA_ON_MASK                ((uint8_t)0x04) /*!< Enable/disable CSMA channel access mode */
#define    PROTOCOL1_SEED_RELOAD_MASK            ((uint8_t)0x08) /*!< Reloads the seed of the PN generator for CSMA procedure */
#define    PROTOCOL1_PIGGYBACKING_MASK           ((uint8_t)0x40) /*!< Enable/disable Piggybacking */
#define    PROTOCOL1_LDC_RELOAD_ON_SYNC_MASK     ((uint8_t)0x80) /*!< LDC timer will be reloaded with the value stored in the LDC_RELOAD registers */

/**
 * @}
 */

/** @defgroup PROTOCOL0_Register
 * @{
 */

/**
 *  \brief PROTOCOL0  register
 *  \code
 *   Default value: 0x08
 *   Read Write
 *   7:4   NMAX_RETX[3:0]:             Max number of re-TX.  0 - re-transmission is not performed
 *
 *   3     NACK_TX[0]:                 1 - field NO_ACK=1 on transmitted packet
 *
 *   2     AUTO_ACK[0]:                1 - automatic ack after RX
 *
 *   1     PERS_RX[0]:                 1 - persistent reception enabled
 *
 *   0     PERS_TX[0]:                 1 - persistent transmission enabled
 *
 *   \endcode
 */
#define    PROTOCOL0_BASE                        ((uint8_t)0x52) /*!< Persistent RX/TX, autoack, Max number of retransmissions */

#define    PROTOCOL0_PERS_TX_MASK                        ((uint8_t)0x01) /*!< Enables persistent transmission */
#define    PROTOCOL0_PERS_RX_MASK                        ((uint8_t)0x02) /*!< Enables persistent reception */
#define    PROTOCOL0_AUTO_ACK_MASK                       ((uint8_t)0x04) /*!< Enables auto acknowlegment */
#define    PROTOCOL0_NACK_TX_MASK                        ((uint8_t)0x08) /*!< Writes field NO_ACK=1 on transmitted packet */
#define    PROTOCOL0_NMAX_RETX_MASK                      ((uint8_t)0xF0) /*!< Retransmission mask */

/**
 * @}
 */

/** @defgroup TIMERS5_Register
 * @{
 */

/**
 *  \brief TIMERS5  register
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:0   RX_TIMEOUT_PRESCALER[7:0] :             RX operation timeout: prescaler value
 *   \endcode
 */
#define    TIMERS5_RX_TIMEOUT_PRESCALER_BASE            ((uint8_t)0x53) /*!< RX operation timeout: prescaler value */

/**
 * @}
 */

/** @defgroup TIMERS4_Register
 * @{
 */

/**
 *  \brief TIMERS4  register
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:0   RX_TIMEOUT_COUNTER[7:0] :               RX operation timeout: counter value
 *   \endcode
 */
#define    TIMERS4_RX_TIMEOUT_COUNTER_BASE                ((uint8_t)0x54) /*!< RX operation timeout: counter value */

/**
 * @}
 */

/** @defgroup TIMERS3_Register
 * @{
 */

/**
 *  \brief TIMERS3  register
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:0   LDCR_PRESCALER[7:0] :                   LDC Mode: Prescaler part of the wake-up value
 *   \endcode
 */
#define    TIMERS3_LDC_PRESCALER_BASE                ((uint8_t)0x55) /*!< LDC Mode: Prescaler of the wake-up timer */

/**
 * @}
 */

/** @defgroup TIMERS2_Register
 * @{
 */

/**
 *  \brief TIMERS2  register
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:0   LDCR_COUNTER[7:0] :                    LDC Mode: counter part of the wake-up value
 *   \endcode
 */
#define    TIMERS2_LDC_COUNTER_BASE                ((uint8_t)0x56) /*!< LDC Mode: counter of the wake-up timer */

/**
 * @}
 */

/** @defgroup TIMERS1_Register
 * @{
 */

/**
 *  \brief TIMERS1  register
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:0   LDCR_RELOAD_PRESCALER[7:0] :           LDC Mode: Prescaler part of the reload value
 *   \endcode
 */
#define    TIMERS1_LDC_RELOAD_PRESCALER_BASE            ((uint8_t)0x57) /*!< LDC Mode: Prescaler part of the reload value */

/**
 * @}
 */

/** @defgroup TIMERS0_Register
 * @{
 */

/**
 *  \brief TIMERS0  register
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:0   LDCR_RELOAD_COUNTER[7:0] :           LDC Mode: Counter part of the reload value
 *   \endcode
 */
#define    TIMERS0_LDC_RELOAD_COUNTER_BASE                ((uint8_t)0x58) /*!< LDC Mode: Counter part of the reload value */

/**
 * @}
 */


/** @defgroup CSMA_CONFIG3_Register
 * @{
 */

/**
 *  \brief CSMA_CONFIG3  registers
 *  \code
 *   Default value: 0xFF
 *   Read Write
 *   7:0     BU_COUNTER_SEED_MSByte: Seed of the random number generator used to apply the BEB (Binary Exponential Backoff) algorithm (MSB)
 *   \endcode
 */
#define    CSMA_CONFIG3_BASE                              ((uint8_t)0x64) /*!< CSMA/CA: Seed of the random number generator used to apply the BEB (Binary Exponential Backoff) algorithm (MSB) */

/**
 * @}
 */

/** @defgroup CSMA_CONFIG2_Register
 * @{
 */

/**
 *  \brief CSMA_CONFIG2  registers
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:0     BU_COUNTER_SEED_LSByte: Seed of the random number generator used to apply the BEB (Binary Exponential Backoff) algorithm (LSB)
 *   \endcode
 */
#define    CSMA_CONFIG2_BASE                              ((uint8_t)0x65) /*!< CSMA/CA: Seed of the random number generator used to apply the BEB (Binary Exponential Backoff) algorithm (LSB) */

/**
 * @}
 */

/** @defgroup CSMA_CONFIG1_Register
 * @{
 */

/**
 *  \brief CSMA_CONFIG1  registers
 *  \code
 *   Default value: 0x04
 *   Read Write
 *   7:2     BU_PRESCALER[5:0]: Used to program the back-off unit BU
 *
 *   1:0     CCA_PERIOD[1:0]: Used to program the Tcca time (64 / 128 /256 / 512 × Tbit.
 *   \endcode
 */
#define    CSMA_CONFIG1_BASE                      ((uint8_t)0x66) /*!< CSMA/CA: Prescaler of the back-off time unit (BU); CCA period */

#define    CSMA_CCA_PERIOD_64TBIT                       ((uint8_t)0x00) /*!< CSMA/CA: Sets CCA period to 64*TBIT */
#define    CSMA_CCA_PERIOD_128TBIT                      ((uint8_t)0x01) /*!< CSMA/CA: Sets CCA period to 128*TBIT */
#define    CSMA_CCA_PERIOD_256TBIT                      ((uint8_t)0x02) /*!< CSMA/CA: Sets CCA period to 256*TBIT */
#define    CSMA_CCA_PERIOD_512TBIT                      ((uint8_t)0x03) /*!< CSMA/CA: Sets CCA period to 512*TBIT */

/**
 * @}
 */

/** @defgroup CSMA_CONFIG0_Register
 * @{
 */

/**
 *  \brief CSMA_CONFIG0  registers
 *  \code
 *   Default value: 0x00
 *   Read Write
 *   7:4     CCA_LENGTH[3:0]: Used to program the Tlisten time
 *
 *   3       Reserved.
 *
 *   2:0     NBACKOFF_MAX[2:0]: Max number of back-off cycles.
 *   \endcode
 */
#define    CSMA_CONFIG0_BASE                      ((uint8_t)0x67) /*!< CSMA/CA: CCA lenght; Max number of backoff cycles */

/**
 * @}
 */

/**
 * @}
 */


/** @defgroup Link_Quality_Registers
 * @{
 */

/** @defgroup QI_Register
 * @{
 */

/**
 *  \brief QI register
 *  \code
 *   Read Write
 *   Default value: 0x02
 *
 *   7:6   SQI_TH[1:0]: SQI threshold according to the formula: 8*SYNC_LEN - 2*SQI_TH
 *
 *   5:2   PQI_TH[3:0]: PQI threshold according to the formula: 4*PQI_THR
 *
 *
 *   1     SQI_EN[0]:  SQI enable
 *                 1 - Enable
 *                 0 - Disable
 *
 *   0     PQI_EN[0]: PQI enable
 *                 1 - Enable
 *                 0 - Disable
 *   \endcode
 */
#define    QI_BASE                            ((uint8_t)0x3A) /*!< QI register */

#define    QI_PQI_MASK                            ((uint8_t)0x01) /*!< PQI enable/disable  */
#define    QI_SQI_MASK                            ((uint8_t)0x02) /*!< SQI enable/disable  */

/**
 * @}
 */

/** @defgroup LINK_QUALIF2
 * @{
 */

/**
 *  \brief LINK_QUALIF2  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7:0   PQI[7:0]: PQI value of the received packet
 *   \endcode
 */
#define    LINK_QUALIF2_BASE                    ((uint8_t)(0xC5)) /*!< PQI value of the received packet */

/**
 * @}
 */

/** @defgroup LINK_QUALIF1
 * @{
 */

/**
 *  \brief LINK_QUALIF1  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7 CS:  Carrier Sense indication
 *
 *   6:0   SQI[6:0]: SQI value of the received packet
 *   \endcode
 */
#define    LINK_QUALIF1_BASE                    ((uint8_t)(0xC6)) /*!< Carrier sense indication [7]; SQI value of the received packet */

#define    LINK_QUALIF1_CS                      ((uint8_t)(0x80)) /*!< Carrier sense indication [7] */

/**
 * @}
 */

/** @defgroup LINK_QUALIF0
 * @{
 */

/**
 *  \brief LINK_QUALIF0  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7:4   LQI [3:0]:  LQI value of the received packet
 *
 *   3:0   AGC_WORD[3:0]: AGC word of the received packet
 *   \endcode
 */
#define    LINK_QUALIF0_BASE                    ((uint8_t)(0xC7)) /*!< LQI value of the received packet [7:4]; AGC word of the received packet [3:0] */

/**
 * @}
 */

/** @defgroup RSSI_LEVEL
 * @{
 */

/**
 *  \brief RSSI_LEVEL  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7:0   RSSI_LEVEL[7:0]:  RSSI level of the received packet
 *   \endcode
 */
#define    RSSI_LEVEL_BASE                        ((uint8_t)(0xC8)) /*!< RSSI level of the received packet */

/**
 * @}
 */

/** @defgroup RSSI_FLT_Register
 * @{
 */

/**
 *  \brief RSSI register
 *  \code
 *   Read Write
 *   Default value: 0xF3
 *   7:4  RSSI_FLT[3:0]: Gain of the RSSI filter
 *
 *   3:2  CS_MODE[1:0]: AFC loop gain in slow mode (2's log)
 *
 *          CS_MODE1       |        CS_MODE0       |                     CS Mode
 *       -----------------------------------------------------------------------------------------
 *             0           |           0           |        Static CS
 *             0           |           1           |        Dynamic CS with 6dB dynamic threshold
 *             1           |           0           |        Dynamic CS with 12dB dynamic threshold
 *             1           |           1           |        Dynamic CS with 18dB dynamic threshold
 *
 *   1:0   OOK_PEAK_DECAY[1:0]: Peak decay control for OOK: 3 slow decay; 0 fast decay
 *
 *   \endcode
 */
#define    RSSI_FLT_BASE                        ((uint8_t)0x21) /*!< Gain of the RSSI filter; lower value is fast but inaccurate,
                                                                    higher value is slow and more accurate */
#define RSSI_FLT_CS_MODE_MASK                    ((uint8_t)0x0C) /*!< Carrier sense mode mask */
#define RSSI_FLT_CS_MODE_STATIC                  ((uint8_t)0x00) /*!< Carrier sense mode;  static carrier sensing */
#define RSSI_FLT_CS_MODE_DYNAMIC_6               ((uint8_t)0x04) /*!< Carrier sense mode;  dynamic carrier sensing with 6dB threshold */
#define RSSI_FLT_CS_MODE_DYNAMIC_12              ((uint8_t)0x08) /*!< Carrier sense mode;  dynamic carrier sensing with 12dB threshold */
#define RSSI_FLT_CS_MODE_DYNAMIC_18              ((uint8_t)0x0C) /*!< Carrier sense mode;  dynamic carrier sensing with 18dB threshold */
#define RSSI_FLT_OOK_PEAK_DECAY_MASK             ((uint8_t)0x03) /*!< Peak decay control for OOK mask */
#define RSSI_FLT_OOK_PEAK_DECAY_FAST             ((uint8_t)0x00) /*!< Peak decay control for OOK: fast decay */
#define RSSI_FLT_OOK_PEAK_DECAY_MEDIUM_FAST      ((uint8_t)0x01) /*!< Peak decay control for OOK: medium_fast decay */
#define RSSI_FLT_OOK_PEAK_DECAY_MEDIUM_SLOW      ((uint8_t)0x02) /*!< Peak decay control for OOK: medium_fast decay */
#define RSSI_FLT_OOK_PEAK_DECAY_SLOW             ((uint8_t)0x03) /*!< Peak decay control for OOK: slow decay */

/**
 * @}
 */

/** @defgroup RSSI_TH_Register
 * @{
 */

/**
 *  \brief RSSI_TH register
 *  \code
 *   Read Write
 *   Default value: 0x24
 *
 *   7:0 RSSI_THRESHOLD [7:0]:    Signal detect threshold in 0.5dB.  -120dBm corresponds to 20
 *   \endcode
 */
#define    RSSI_TH_BASE                        ((uint8_t)0x22) /*!< Signal detect threshold in 0.5dB stp. 20 correspond to -120 dBm */

/**
 * @}
 */

/**
 * @}
 */


/** @defgroup FIFO_Registers
 * @{
 */

/** @defgroup FIFO_CONFIG3_Register
 * @{
 */

/**
 *  \brief FIFO_CONFIG3  registers
 *  \code
 *   Default value: 0x30
 *   Read Write
 *   7    Reserved.
 *
 *   6:0  rxafthr [6:0]: FIFO Almost Full threshold for rx fifo.
 *
 *   \endcode
 */
#define    FIFO_CONFIG3_RXAFTHR_BASE                ((uint8_t)0x3E) /*!< FIFO Almost Full threshold for rx fifo [6:0] */

/**
 * @}
 */

/** @defgroup FIFO_CONFIG2_Register
 * @{
 */

/**
 *  \brief FIFO_CONFIG2  registers
 *  \code
 *   Default value: 0x30
 *   Read Write
 *   7    Reserved.
 *
 *   6:0  rxaethr [6:0]: FIFO Almost Empty threshold for rx fifo.
 *
 *   \endcode
 */
#define    FIFO_CONFIG2_RXAETHR_BASE                ((uint8_t)0x3F) /*!< FIFO Almost Empty threshold for rx fifo [6:0] */

/**
 * @}
 */

/** @defgroup FIFO_CONFIG1_Register
 * @{
 */

/**
 *  \brief FIFO_CONFIG1  registers
 *  \code
 *   Default value: 0x30
 *   Read Write
 *   7    Reserved.
 *
 *   6:0  txafthr [6:0]: FIFO Almost Full threshold for tx fifo.
 *
 *   \endcode
 */
#define    FIFO_CONFIG1_TXAFTHR_BASE                ((uint8_t)0x40) /*!< FIFO Almost Full threshold for tx fifo [6:0] */

/**
 * @}
 */

/** @defgroup FIFO_CONFIG0_Register
 * @{
 */

/**
 *  \brief FIFO_CONFIG0  registers
 *  \code
 *   Default value: 0x30
 *   Read Write
 *   7    Reserved.
 *
 *   6:0  txaethr [6:0]: FIFO Almost Empty threshold for tx fifo.
 *
 *   \endcode
 */
#define    FIFO_CONFIG0_TXAETHR_BASE                ((uint8_t)0x41) /*!< FIFO Almost Empty threshold for tx fifo [6:0] */

/**
 * @}
 */

/** @defgroup LINEAR_FIFO_STATUS1_Register
 * @{
 */

/**
 *  \brief LINEAR_FIFO_STATUS1  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7     Reserved.
 *
 *   6:0   elem_txfifo[6:0]:     Number of elements in the linear TXFIFO (<=96)
 *   \endcode
 */
#define    LINEAR_FIFO_STATUS1_BASE                ((uint8_t)(0xE6)) /*!< Number of elements in the linear TX FIFO [6:0] (<=96) */

/**
 * @}
 */

/** @defgroup LINEAR_FIFO_STATUS0_Register
 * @{
 */

/**
 *  \brief LINEAR_FIFO_STATUS0  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7     Reserved.
 *
 *   6:0   elem_rxfifo[6:0]:     Number of elements in the linear RXFIFO (<=96)
 *   \endcode
 */
#define    LINEAR_FIFO_STATUS0_BASE                ((uint8_t)(0xE7)) /*!< Number of elements in the linear RX FIFO [6:0] (<=96) */

/**
 * @}
 */


/**
 * @}
 */


/** @defgroup Calibration_Registers
 * @{
 */

/** @defgroup RCO_VCO_CALIBR_IN2_Register
 * @{
 */

/**
 *  \brief RCO_VCO_CALIBR_IN2  registers
 *  \code
 *   Default value: 0x70
 *   Read Write
 *   7:4     RWT_IN[3:0]: RaWThermometric word value for the RCO [7:4]
 *
 *   3:0     RFB_IN[4:1]: ResistorFineBit word value for the RCO (first 4 bits)
 *   \endcode
 */
#define    RCO_VCO_CALIBR_IN2_BASE                    ((uint8_t)0x6D) /*!< RaWThermometric word value for the RCO [7:4]; ResistorFineBit word value for the RCO [3:0] */

/**
 * @}
 */

/** @defgroup RCO_VCO_CALIBR_IN1_Register
 * @{
 */

/**
 *  \brief RCO_VCO_CALIBR_IN1  registers
 *  \code
 *   Default value: 0x48
 *   Read Write
 *
 *   7     RFB_IN[0]: ResistorFineBit word value for the RCO (LSb)
 *
 *   6:0   VCO_CALIBR_TX[6:0]:  Word value for the VCO to be used in TX mode
 *   \endcode
 */
#define    RCO_VCO_CALIBR_IN1_BASE                    ((uint8_t)0x6E) /*!< ResistorFineBit word value for the RCO [7]; Word value for the VCO to be used in TX mode  [6:0]*/

/**
 * @}
 */

/** @defgroup RCO_VCO_CALIBR_IN0_Register
 * @{
 */

/**
 *  \brief RCO_VCO_CALIBR_IN0  registers
 *  \code
 *   Default value: 0x48
 *   Read Write
 *
 *   7     Reserved.
 *
 *   6:0   VCO_CALIBR_RX[6:0]:  Word value for the VCO to be used in RX mode
 *   \endcode
 */
#define    RCO_VCO_CALIBR_IN0_BASE                    ((uint8_t)0x6F) /*!< Word value for the VCO to be used in RX mode [6:0] */

/**
 * @}
 */

/** @defgroup RCO_VCO_CALIBR_OUT1_Register
 * @{
 */

/**
 *  \brief RCO_VCO_CALIBR_OUT1  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7:4   RWT_OUT[3:0]:    RWT word from internal RCO calibrator
 *
 *   3:0   RFB_OUT[4:1]:     RFB word from internal RCO calibrator (upper part)
 *   \endcode
 */
#define    RCO_VCO_CALIBR_OUT1_BASE                ((uint8_t)(0xE4)) /*!< RaWThermometric RWT word from internal RCO calibrator [7];
                                                                                       ResistorFineBit RFB word from internal RCO oscillator [6:0] */
/**
 * @}
 */

/** @defgroup RCO_VCO_CALIBR_OUT0_Register
 * @{
 */

/**
 *  \brief RCO_VCO_CALIBR_OUT0  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7     RFB_OUT[0]:    RFB word from internal RCO calibrator (last bit LSB)
 *
 *   6:0   VCO_CALIBR_DATA[6:0]:     Output word from internal VCO calibrator
 *   \endcode
 */
#define    RCO_VCO_CALIBR_OUT0_BASE                ((uint8_t)(0xE5)) /*!< ResistorFineBit RFB word from internal RCO oscillator [0];
                                                                                       Output word from internal calibrator [6:0]; */
/**
 * @}
 */

/**
 * @}
 */


/** @defgroup AES_Registers
 * @{
 */

/** @defgroup AES_KEY_IN_Register
 * @{
 */

/**
 *  \brief AES_KEY_INx  registers
 *  \code
 *   Default value: 0x00
 *   Read Write
 *
 *   7:0   AES_KEY_INx[7:0]:  AES engine key input (total - 128 bits)
 *   \endcode
 */
#define    AES_KEY_IN_15_BASE                    ((uint8_t)0x70) /*!< AES engine key input 15 */
   
#define    AES_KEY_IN_14_BASE                    ((uint8_t)0x71) /*!< AES engine key input 14 */

#define    AES_KEY_IN_13_BASE                    ((uint8_t)0x72) /*!< AES engine key input 13 */

#define    AES_KEY_IN_12_BASE                    ((uint8_t)0x73) /*!< AES engine key input 12 */

#define    AES_KEY_IN_11_BASE                    ((uint8_t)0x74) /*!< AES engine key input 11 */

#define    AES_KEY_IN_10_BASE                    ((uint8_t)0x75) /*!< AES engine key input 10 */

#define    AES_KEY_IN_9_BASE                    ((uint8_t)0x76) /*!< AES engine key input 9 */

#define    AES_KEY_IN_8_BASE                    ((uint8_t)0x77) /*!< AES engine key input 8 */

#define    AES_KEY_IN_7_BASE                    ((uint8_t)0x78) /*!< AES engine key input 7 */

#define    AES_KEY_IN_6_BASE                    ((uint8_t)0x79) /*!< AES engine key input 6 */

#define    AES_KEY_IN_5_BASE                    ((uint8_t)0x7A) /*!< AES engine key input 5 */

#define    AES_KEY_IN_4_BASE                    ((uint8_t)0x7B) /*!< AES engine key input 4 */

#define    AES_KEY_IN_3_BASE                    ((uint8_t)0x7C) /*!< AES engine key input 3 */

#define    AES_KEY_IN_2_BASE                    ((uint8_t)0x7D) /*!< AES engine key input 2 */

#define    AES_KEY_IN_1_BASE                    ((uint8_t)0x7E) /*!< AES engine key input 1 */

#define    AES_KEY_IN_0_BASE                    ((uint8_t)0x7F) /*!< AES engine key input 0 */

/**
 * @}
 */

/** @defgroup AES_DATA_IN_Register
 * @{
 */

/**
 *  \brief AES_DATA_INx  registers
 *  \code
 *   Default value: 0x00
 *   Read Write
 *
 *   7:0   AES_DATA_INx[7:0]:  AES engine data input (total - 128 bits)
 *   \endcode
 */
#define    AES_DATA_IN_15_BASE                    ((uint8_t)0x80) /*!< AES engine data input 15
                                                                                     Take care: Address is in reverse order respect data numbering; eg.: 0x81 -> AES_data14[7:0] */
#define    AES_DATA_IN_14_BASE                    ((uint8_t)0x81) /*!< AES engine data input 14 */

#define    AES_DATA_IN_13_BASE                    ((uint8_t)0x82) /*!< AES engine data input 13 */

#define    AES_DATA_IN_12_BASE                    ((uint8_t)0x83) /*!< AES engine data input 12 */

#define    AES_DATA_IN_11_BASE                    ((uint8_t)0x84) /*!< AES engine data input 11 */

#define    AES_DATA_IN_10_BASE                    ((uint8_t)0x85) /*!< AES engine data input 10 */

#define    AES_DATA_IN_9_BASE                    ((uint8_t)0x86) /*!< AES engine data input 9 */

#define    AES_DATA_IN_8_BASE                    ((uint8_t)0x87) /*!< AES engine data input 8 */

#define    AES_DATA_IN_7_BASE                    ((uint8_t)0x88) /*!< AES engine data input 7 */

#define    AES_DATA_IN_6_BASE                    ((uint8_t)0x89) /*!< AES engine data input 6 */

#define    AES_DATA_IN_5_BASE                    ((uint8_t)0x8A) /*!< AES engine data input 5 */

#define    AES_DATA_IN_4_BASE                    ((uint8_t)0x8B) /*!< AES engine data input 4 */

#define    AES_DATA_IN_3_BASE                    ((uint8_t)0x8C) /*!< AES engine data input 3 */

#define    AES_DATA_IN_2_BASE                    ((uint8_t)0x8D) /*!< AES engine data input 2 */

#define    AES_DATA_IN_1_BASE                    ((uint8_t)0x8E) /*!< AES engine data input 1 */

#define    AES_DATA_IN_0_BASE                    ((uint8_t)0x8F) /*!< AES engine data input 0 */

/**
 * @}
 */

/** @defgroup AES_DATA_OUT_Register
 * @{
 */

/**
 *  \brief AES_DATA_OUT[15:0]  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7:0   AES_DATA_OUTx[7:0]:    AES engine data output (128 bits)
 *   \endcode
 */
#define    AES_DATA_OUT_15_BASE                    ((uint8_t)(0xD4)) /*!< AES engine data output 15 */

#define    AES_DATA_OUT_14_BASE                    ((uint8_t)(0xD5)) /*!< AES engine data output 14 */

#define    AES_DATA_OUT_13_BASE                    ((uint8_t)(0xD6)) /*!< AES engine data output 13 */

#define    AES_DATA_OUT_12_BASE                    ((uint8_t)(0xD7)) /*!< AES engine data output 12 */

#define    AES_DATA_OUT_11_BASE                    ((uint8_t)(0xD8)) /*!< AES engine data output 11 */

#define    AES_DATA_OUT_10_BASE                    ((uint8_t)(0xD9)) /*!< AES engine data output 10 */

#define    AES_DATA_OUT_9_BASE                    ((uint8_t)(0xDA)) /*!< AES engine data output 9 */

#define    AES_DATA_OUT_8_BASE                    ((uint8_t)(0xDB)) /*!< AES engine data output 8 */

#define    AES_DATA_OUT_7_BASE                    ((uint8_t)(0xDC)) /*!< AES engine data output 7 */

#define    AES_DATA_OUT_6_BASE                    ((uint8_t)(0xDD)) /*!< AES engine data output 6 */

#define    AES_DATA_OUT_5_BASE                    ((uint8_t)(0xDE)) /*!< AES engine data output 5 */

#define    AES_DATA_OUT_4_BASE                    ((uint8_t)(0xDF)) /*!< AES engine data output 4 */

#define    AES_DATA_OUT_3_BASE                    ((uint8_t)(0xE0)) /*!< AES engine data output 3 */

#define    AES_DATA_OUT_2_BASE                    ((uint8_t)(0xE1)) /*!< AES engine data output 2 */

#define    AES_DATA_OUT_1_BASE                    ((uint8_t)(0xE2)) /*!< AES engine data output 1 */

#define    AES_DATA_OUT_0_BASE                    ((uint8_t)(0xE3)) /*!< AES engine data output 0 */

/**
 * @}
 */

/**
 * @}
 */

/** @defgroup IRQ_Registers
 * @{
 */

/** @defgroup IRQ_MASK0_Register
 * @{
 */

/**
 *  \brief IRQ_MASK0  registers
 *  \code
 *   Default value: 0x00
 *   Read Write
 *
 *   7:0   INT_MASK0:  IRQ mask, if the correspondent bit is set and IRQ can be generated (according to the next table)
 *
 *            Bit |     Events Group Interrupt Event
 *           -------------------------------------------------------
 *            0   |       RX data ready
 *            1   |       RX data discarded (upon filtering)
 *            2   |       TX data sent
 *            3   |       Max re-TX reached
 *            4   |       CRC error
 *            5   |       TX FIFO underflow/overflow error
 *            6   |       RX FIFO underflow/overflow error
 *            7   |       TX FIFO almost full
 *    \endcode
 */


#define    IRQ_MASK0_BASE                        ((uint8_t)0x93) /*!< IRQ_MASK is split into 4 registers*/

#define IRQ_MASK0_RX_DATA_READY                  ((uint8_t)0x01) /*!< IRQ: RX data ready */
#define IRQ_MASK0_RX_DATA_DISC                   ((uint8_t)0x02) /*!< IRQ: RX data discarded (upon filtering) */
#define IRQ_MASK0_TX_DATA_SENT                   ((uint8_t)0x04) /*!< IRQ: TX data sent */
#define IRQ_MASK0_MAX_RE_TX_REACH                ((uint8_t)0x08) /*!< IRQ: Max re-TX reached */
#define IRQ_MASK0_CRC_ERROR                      ((uint8_t)0x10) /*!< IRQ: CRC error */
#define IRQ_MASK0_TX_FIFO_ERROR                  ((uint8_t)0x20) /*!< IRQ: TX FIFO underflow/overflow error */
#define IRQ_MASK0_RX_FIFO_ERROR                  ((uint8_t)0x40) /*!< IRQ: RX FIFO underflow/overflow error */
#define IRQ_MASK0_TX_FIFO_ALMOST_FULL            ((uint8_t)0x80) /*!< IRQ: TX FIFO almost full */

/**
 * @}
 */

/** @defgroup IRQ_MASK1_Register
 * @{
 */

/**
 *  \brief IRQ_MASK1  registers
 *  \code
 *   Default value: 0x00
 *   Read Write
 *
 *   7:0   INT_MASK1:  IRQ mask, if the correspondent bit is set and IRQ can be generated (according to the next table)
 *
 *            Bit |     Events Group Interrupt Event
 *           -------------------------------------------------------
 *            8   |       TX FIFO almost empty
 *            9   |       RX FIFO almost full
 *            10  |       RX FIFO almost empty
 *            11  |       Max number of back-off during CCA
 *            12  |       Valid preamble detected
 *            13  |       Sync word detected
 *            14  |       RSSI above threshold (Carrier Sense)
 *            15  |       Wake-up timeout in LDCR mode13
 *     \endcode
 */

#define    IRQ_MASK1_BASE                        ((uint8_t)0x92) /*!< IRQ_MASK is split into 4 registers*/

#define IRQ_MASK1_TX_FIFO_ALMOST_EMPTY           ((uint8_t)0x01) /*!< IRQ: TX FIFO almost empty */
#define IRQ_MASK1_RX_FIFO_ALMOST_FULL            ((uint8_t)0x02) /*!< IRQ: RX FIFO almost full */
#define IRQ_MASK1_RX_FIFO_ALMOST_EMPTY           ((uint8_t)0x04) /*!< IRQ: RX FIFO almost empty  */
#define IRQ_MASK1_MAX_BO_CCA_REACH               ((uint8_t)0x08) /*!< IRQ: Max number of back-off during CCA */
#define IRQ_MASK1_VALID_PREAMBLE                 ((uint8_t)0x10) /*!< IRQ: Valid preamble detected */
#define IRQ_MASK1_VALID_SYNC                     ((uint8_t)0x20) /*!< IRQ: Sync word detected */
#define IRQ_MASK1_RSSI_ABOVE_TH                  ((uint8_t)0x40) /*!< IRQ: RSSI above threshold */
#define IRQ_MASK1_WKUP_TOUT_LDC                  ((uint8_t)0x80) /*!< IRQ: Wake-up timeout in LDC mode */

/**
 * @}
 */

/** @defgroup IRQ_MASK2_Register
 * @{
 */

/**
 *  \brief IRQ_MASK2  registers
 *  \code
 *   Default value: 0x00
 *   Read Write
 *
 *   7:0   INT_MASK2:  IRQ mask, if the correspondent bit is set and IRQ can be generated (according to the next table)
 *
 *            Bit |     Events Group Interrupt Event
 *           -------------------------------------------------------
 *            16  |       READY state in steady condition14
 *            17  |       STANDBY state switching in progress
 *            18  |       Low battery level
 *            19  |       Power-On reset
 *            20  |       Brown-Out event
 *            21  |       LOCK state in steady condition
 *            22  |       PM start-up timer expiration
 *            23  |       XO settling timeout
 *   \endcode
 */
#define    IRQ_MASK2_BASE                      ((uint8_t)0x91) /*!< IRQ_MASK is split into 4 registers*/

#define IRQ_MASK2_READY                        ((uint8_t)0x01) /*!< IRQ: READY state */
#define IRQ_MASK2_STANDBY_DELAYED              ((uint8_t)0x02) /*!< IRQ: STANDBY state after MCU_CK_CONF_CLOCK_TAIL_X clock cycles */
#define IRQ_MASK2_LOW_BATT_LVL                 ((uint8_t)0x04) /*!< IRQ: Battery level below threshold*/
#define IRQ_MASK2_POR                          ((uint8_t)0x08) /*!< IRQ: Power On Reset */
#define IRQ_MASK2_BOR                          ((uint8_t)0x10) /*!< IRQ: Brown out event (both accurate and inaccurate)*/
#define IRQ_MASK2_LOCK                         ((uint8_t)0x20) /*!< IRQ: LOCK state */
#define IRQ_MASK2_PM_COUNT_EXPIRED             ((uint8_t)0x40) /*!< IRQ: only for debug; Power Management startup timer expiration (see reg PM_START_COUNTER, 0xB5) */
#define IRQ_MASK2_XO_COUNT_EXPIRED             ((uint8_t)0x80) /*!< IRQ: only for debug; Crystal oscillator settling time counter expired */

/**
 * @}
 */

/** @defgroup IRQ_MASK3_Register
 * @{
 */

/**
 *  \brief IRQ_MASK3  registers
 *  \code
 *   Default value: 0x00
 *   Read Write
 *
 *   7:0   INT_MASK3:  IRQ mask, if the correspondent bit is set and IRQ can be generated (according to the next table)
 *
 *            Bit |     Events Group Interrupt Event
 *           -------------------------------------------------------
 *            24  |       SYNTH locking timeout
 *            25  |       SYNTH calibration start-up time
 *            26  |       SYNTH calibration timeout
 *            27  |       TX circuitry start-up time
 *            28  |       RX circuitry start-up time
 *            29  |       RX operation timeout
 *            30  |       Others AES End–of –Operation
 *            31  |       Reserved
 *   \endcode
 */
#define    IRQ_MASK3_BASE                        ((uint8_t)0x90) /*!< IRQ_MASK is split into 4 registers*/

#define IRQ_MASK3_SYNTH_LOCK_TIMEOUT             ((uint8_t)0x01) /*!< IRQ: only for debug; LOCK state timeout */
#define IRQ_MASK3_SYNTH_LOCK_STARTUP             ((uint8_t)0x02) /*!< IRQ: only for debug; see CALIBR_START_COUNTER */
#define IRQ_MASK3_SYNTH_CAL_TIMEOUT              ((uint8_t)0x04) /*!< IRQ: only for debug; SYNTH calibration timeout */
#define IRQ_MASK3_TX_START_TIME                  ((uint8_t)0x08) /*!< IRQ: only for debug; TX circuitry startup time; see TX_START_COUNTER */
#define IRQ_MASK3_RX_START_TIME                  ((uint8_t)0x10) /*!< IRQ: only for debug; RX circuitry startup time; see TX_START_COUNTER */
#define IRQ_MASK3_RX_TIMEOUT                     ((uint8_t)0x20) /*!< IRQ: RX operation timeout */
#define IRQ_MASK3_AES_END                        ((uint8_t)0x40) /*!< IRQ: AES End of operation */

/**
 * @}
 */


/** @defgroup IRQ_STATUS0_Register
 * @{
 */

/**
 *  \brief IRQ_STATUS0  registers
 *  \code
 *   Default value: 0x00
 *   Read Write
 *
 *   7:0   INT_STATUS0:  IRQ status, if the correspondent bit is set and IRQ has been generated (according to the next table)
 *
 *            Bit |     Events Group Interrupt Event
 *           -------------------------------------------------------
 *            0   |       RX data ready
 *            1   |       RX data discarded (upon filtering)
 *            2   |       TX data sent
 *            3   |       Max re-TX reached
 *            4   |       CRC error
 *            5   |       TX FIFO underflow/overflow error
 *            6   |       RX FIFO underflow/overflow error
 *            7   |       TX FIFO almost full
 *   \endcode
 */

#define IRQ_STATUS0_BASE                    ((uint8_t)(0xFD)) /*!< IRQ Events(RR, split into 4 registers) */

#define IRQ_STATUS0_SYNTH_LOCK_TIMEOUT                ((uint8_t)(0x01)) /*!< IRQ: LOCK state timeout */
#define IRQ_STATUS0_SYNTH_LOCK_STARTUP                ((uint8_t)(0x02)) /*!< IRQ: only for debug; see CALIBR_START_COUNTER */
#define IRQ_STATUS0_SYNTH_CAL_TIMEOUT                ((uint8_t)(0x04)) /*!< IRQ: SYNTH locking timeout */
#define IRQ_STATUS0_TX_START_TIME                ((uint8_t)(0x08)) /*!< IRQ: only for debug; TX circuitry startup time; see TX_START_COUNTER */
#define IRQ_STATUS0_RX_START_TIME                ((uint8_t)(0x10)) /*!< IRQ: only for debug; RX circuitry startup time; see TX_START_COUNTER */
#define IRQ_STATUS0_RX_TIMEOUT                    ((uint8_t)(0x20)) /*!< IRQ: RX operation timeout expiration */
#define IRQ_STATUS0_AES_END                    ((uint8_t)(0x40)) /*!< IRQ: AES End of operation */

/**
 * @}
 */

/** @defgroup IRQ_STATUS1_Register
 * @{
 */

/**
 *  \brief IRQ_STATUS1  registers
 *  \code
 *   Default value: 0x00
 *   Read Write
 *
 *   7:0   INT_STATUS1:  IRQ status, if the correspondent bit is set and IRQ has been generated (according to the next table)
 *
 *            Bit |     Events Group Interrupt Event
 *           -------------------------------------------------------
 *            8   |       TX FIFO almost empty
 *            9   |       RX FIFO almost full
 *            10  |       RX FIFO almost empty
 *            11  |       Max number of back-off during CCA
 *            12  |       Valid preamble detected
 *            13  |       Sync word detected
 *            14  |       RSSI above threshold (Carrier Sense)
 *            15  |       Wake-up timeout in LDCR mode13
 *   \endcode
 */

#define IRQ_STATUS1_BASE                    ((uint8_t)(0xFC)) /*!< IRQ Events(RR, split into 4 registers) */

#define IRQ_STATUS1_READY                    ((uint8_t)(0x01)) /*!< IRQ: READY state in steady condition*/
#define IRQ_STATUS1_STANDBY_DELAYED                ((uint8_t)(0x02)) /*!< IRQ: STANDBY state after MCU_CK_CONF_CLOCK_TAIL_X clock cycles */
#define IRQ_STATUS1_LOW_BATT_LVL                ((uint8_t)(0x04)) /*!< IRQ: Battery level below threshold*/
#define IRQ_STATUS1_POR                        ((uint8_t)(0x08)) /*!< IRQ: Power On Reset */
#define IRQ_STATUS1_BOR                        ((uint8_t)(0x10)) /*!< IRQ: Brown out event (both accurate and inaccurate)*/
#define IRQ_STATUS1_LOCK                    ((uint8_t)(0x20)) /*!< IRQ: LOCK state in steady condition */
#define IRQ_STATUS1_PM_COUNT_EXPIRED                ((uint8_t)(0x40)) /*!< IRQ: Power Management startup timer expiration (see reg PM_START_COUNTER, 0xB5) */
#define IRQ_STATUS1_XO_COUNT_EXPIRED                ((uint8_t)(0x80)) /*!< IRQ: Crystal oscillator settling time counter expired */

/**
 * @}
 */

/** @defgroup IRQ_STATUS2_Register
 * @{
 */

/**
 *  \brief IRQ_STATUS2  registers
 *  \code
 *   Default value: 0x00
 *   Read Write
 *
 *   7:0   INT_STATUS2:  IRQ status, if the correspondent bit is set and IRQ has been generated (according to the next table)
 *
 *            Bit |     Events Group Interrupt Event
 *           -------------------------------------------------------
 *            16  |       READY state in steady condition14
 *            17  |       STANDBY state switching in progress
 *            18  |       Low battery level
 *            19  |       Power-On reset
 *            20  |       Brown-Out event
 *            21  |       LOCK state in steady condition
 *            22  |       PM start-up timer expiration
 *            23  |       XO settling timeout
 *   \endcode
 */

#define IRQ_STATUS2_BASE                    ((uint8_t)0xFB) /*!< IRQ Events(RR, split into 4 registers) */

#define IRQ_STATUS2_TX_FIFO_ALMOST_EMPTY            ((uint8_t)0x01) /*!< IRQ: TX FIFO almost empty */
#define IRQ_STATUS2_RX_FIFO_ALMOST_FULL                ((uint8_t)0x02) /*!< IRQ: RX FIFO almost full */
#define IRQ_STATUS2_RX_FIFO_ALMOST_EMPTY            ((uint8_t)0x04) /*!< IRQ: RX FIFO almost empty */
#define IRQ_STATUS2_MAX_BO_CCA_REACH                ((uint8_t)0x08) /*!< IRQ: Max number of back-off during CCA */
#define IRQ_STATUS2_VALID_PREAMBLE                ((uint8_t)0x10) /*!< IRQ: Valid preamble detected */
#define IRQ_STATUS2_VALID_SYNC                    ((uint8_t)0x20) /*!< IRQ: Sync word detected */
#define IRQ_STATUS2_RSSI_ABOVE_TH                ((uint8_t)(0x40)) /*!< IRQ: RSSI above threshold */
#define IRQ_STATUS2_WKUP_TOUT_LDC                ((uint8_t)(0x80)) /*!< IRQ: Wake-up timeout in LDC mode */

/**
 * @}
 */

/** @defgroup IRQ_STATUS3_Register
 * @{
 */

/**
 *  \brief IRQ_STATUS3  registers
 *  \code
 *   Default value: 0x00
 *   Read Write
 *
 *   7:0   INT_STATUS3:  IRQ status, if the correspondent bit is set and IRQ has been generated (according to the next table)
 *
 *            Bit |     Events Group Interrupt Event
 *           -------------------------------------------------------
 *            24  |       SYNTH locking timeout
 *            25  |       SYNTH calibration start-up time
 *            26  |       SYNTH calibration timeout
 *            27  |       TX circuitry start-up time
 *            28  |       RX circuitry start-up time
 *            29  |       RX operation timeout
 *            30  |       Others AES End–of –Operation
 *            31  |       Reserved
 *    \endcode
 */
#define IRQ_STATUS3_BASE                    ((uint8_t)0xFA) /*!< IRQ Events(RR, split into 4 registers) */

#define IRQ_STATUS3_RX_DATA_READY                ((uint8_t)0x01) /*!< IRQ: RX data ready */
#define IRQ_STATUS3_RX_DATA_DISC                ((uint8_t)0x02) /*!< IRQ: RX data discarded (upon filtering) */
#define IRQ_STATUS3_TX_DATA_SENT                ((uint8_t)0x04) /*!< IRQ: TX data sent */
#define IRQ_STATUS3_MAX_RE_TX_REACH                ((uint8_t)0x08) /*!< IRQ: Max re-TX reached */
#define IRQ_STATUS3_CRC_ERROR                    ((uint8_t)0x10) /*!< IRQ: CRC error */
#define IRQ_STATUS3_TX_FIFO_ERROR                ((uint8_t)0x20) /*!< IRQ: TX FIFO underflow/overflow error */
#define IRQ_STATUS3_RX_FIFO_ERROR                ((uint8_t)0x40) /*!< IRQ: RX FIFO underflow/overflow error */
#define IRQ_STATUS3_TX_FIFO_ALMOST_FULL                ((uint8_t)0x80) /*!< IRQ: TX FIFO almost full */

/**
 * @}
 */

/**
 * @}
 */


/** @defgroup MC_STATE_Registers
 * @{
 */

/** @defgroup MC_STATE1_Register
 * @{
 */

/**
 *  \brief MC_STATE1  registers
 *  \code
 *   Default value: 0x50
 *   Read
 *
 *   7:4     Reserved.
 *
 *   3       ANT_SELECT:  Currently selected antenna
 *
 *   2       TX_FIFO_Full:     1 - TX FIFO is full
 *
 *   1       RX_FIFO_Empty:    1 - RX FIFO is empty
 *
 *   0       ERROR_LOCK:       1 - RCO calibrator error
 *   \endcode
 */
#define    MC_STATE1_BASE                        ((uint8_t)(0xC0)) /*!< MC_STATE1 register address (see the SpiritStatus struct */


/**
 * @}
 */


/** @defgroup MC_STATE0_Register
 * @{
 */

/**
 *  \brief MC_STATE0  registers
 *  \code
 *   Default value: 0x00
 *   Read
 *
 *   7:1     STATE[6:0]: Current MC state.
 *
 *      REGISTER VALUE |         STATE
 *   --------------------------------------------
 *            0x40     |         STANDBY
 *            0x36     |         SLEEP
 *            0x03     |         READY
 *            0x3B     |         PM setup
 *            0x23     |         XO settling
 *            0x53     |         SYNTH setup
 *            0x1F     |         PROTOCOL
 *            0x4F     |         SYNTH calibration
 *            0x0F     |         LOCK
 *            0x33     |         RX
 *            0x5F     |         TX
 *
 *   0       XO_ON:       1 - XO is operating
 *   \endcode
 */
#define    MC_STATE0_BASE                        ((uint8_t)(0xC1)) /*!< MC_STATE0 register address. In this version ALL existing states have been inserted
                                                                                       and are still to be verified */
/**
 * @}
 */

/**
 * @}
 */
   

/* +++++++++++++++++++++ END SPSGRF registers +++++++++++++++++++++++++++++++++*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/






/** @defgroup Engineering-Test_Registers
 * @{
 */

#define    SYNTH_CONFIG1_BASE                    ((uint8_t)(0x9E)) /*!< Synthesizier registers: M, A, K data sync on positive/negative clock edges [4],
                                                                                       Enable Linearization of the charge pump [3], split time 1.75/3.45ns [2], VCO calibration window 16,32,64,128 clock cycles [1:0]*/
#define    SYNTH_CONFIG0_BASE                    ((uint8_t)(0x9F)) /*!< Enable DSM randomizer [7], Window width 1.2-7.5ns (Down-up) of lock detector*/
#define    VCOTH_BASE                        ((uint8_t)(0xA0)) /*!< Controls the threshold frequency between VCO low and VCO high [7:0]
                                                                                       VCOth frequency=2*fXO*(96+VCO_TH/16), fmin=4992 MHz, fmax=5820 MHz*/
#define    PM_CONFIG2_BASE                        ((uint8_t)(0xA4)) /*!< Enables high current buffer on Temperature sensor, sets SMPS options */
#define    PM_CONFIG1_BASE                        ((uint8_t)(0xA5)) /*!< Set SMPS options */
#define    PM_CONFIG0_BASE                        ((uint8_t)(0xA6)) /*!< Set SMPS options */
#define    VCO_CONFIG_BASE                        ((uint8_t)(0xA1)) /*!< Set VCO current [5:2]part and [1:0] part */
#define    XO_CONFIG_BASE                        ((uint8_t)(0xA7)) /*!< Clock management options from XO to digital part */

#define    XO_RCO_TEST_BASE                    ((uint8_t)(0xB4)) /*!< Test of XO and RCO */

/**
 * @}
 */


/** @addtogroup Commands
 * @{
 */

#define    COMMAND_TX                        ((uint8_t)(0x60)) /*!< Start to transmit; valid only from READY */
#define    COMMAND_RX                        ((uint8_t)(0x61)) /*!< Start to receive; valid only from READY */
#define    COMMAND_READY                            ((uint8_t)(0x62)) /*!< Go to READY; valid only from STANDBY or SLEEP or LOCK */
#define    COMMAND_STANDBY                            ((uint8_t)(0x63)) /*!< Go to STANDBY; valid only from READY */
#define    COMMAND_SLEEP                            ((uint8_t)(0x64)) /*!< Go to SLEEP; valid only from READY */
#define    COMMAND_LOCKRX                            ((uint8_t)(0x65)) /*!< Go to LOCK state by using the RX configuration of the synth; valid only from READY */
#define    COMMAND_LOCKTX                            ((uint8_t)(0x66)) /*!< Go to LOCK state by using the TX configuration of the synth; valid only from READY */
#define    COMMAND_SABORT                            ((uint8_t)(0x67)) /*!< Force exit form TX or RX states and go to READY state; valid only from TX or RX */
#define    COMMAND_LDC_RELOAD                        ((uint8_t)(0x68)) /*!< LDC Mode: Reload the LDC timer with the value stored in the  LDC_PRESCALER / COUNTER
                                                                                       registers; valid from all states  */
#define    COMMAND_SEQUENCE_UPDATE                            ((uint8_t)(0x69)) /*!< Autoretransmission: Reload the Packet sequence counter with the value stored in the PROTOCOL[2] register
                                                                                       valid from all states */
#define    COMMAND_AES_ENC                            ((uint8_t)(0x6A)) /*!< AES: Start the encryption routine; valid from all states; valid from all states */
#define    COMMAND_AES_KEY                            ((uint8_t)(0x6B)) /*!< AES: Start the procedure to compute the key for the decryption; valid from all states */
#define    COMMAND_AES_DEC                            ((uint8_t)(0x6C)) /*!< AES: Start the decryption routine using the current key; valid from all states */
#define    COMMAND_AES_KEY_DEC                        ((uint8_t)(0x6D)) /*!< AES: Compute the key and start the decryption; valid from all states */
#define    COMMAND_SRES                            ((uint8_t)(0x70)) /*!< Reset of all digital part, except SPI registers */
#define    COMMAND_FLUSHRXFIFO                        ((uint8_t)(0x71)) /*!< Clean the RX FIFO; valid from all states */
#define    COMMAND_FLUSHTXFIFO                        ((uint8_t)(0x72)) /*!< Clean the TX FIFO; valid from all states */

/**
 * @}
 */

/**
 * @}
 */
 
 
 /**
 * @addtogroup SPIRIT_Types_Constants_Macros         SPIRIT Types Constants Macros
 * @brief Header file containing all the SPIRIT Types Constants Macros.
 * @{
 */
 
/* --------------------------------   COMMON-------------------------------- */ 

/**
 * @defgroup Calibration_Exported_Types Calibration Exported Types
 * @{
 */


/**
 * @brief  Spirit Functional state. Used to enable or disable a specific option.
 */
typedef enum
{
  S_DISABLE = 0,
  S_ENABLE = !S_DISABLE

} SpiritFunctionalState;

#define IS_SPIRIT_FUNCTIONAL_STATE(STATE)   (STATE == S_DISABLE || STATE == S_ENABLE)

/**
 * @brief  Spirit Flag status. Used to control the state of a flag.
 */
typedef enum
{
  S_RESET = 0,
  S_SET = !S_RESET

} SpiritFlagStatus;

#define IS_SPIRIT_FLAG_STATUS(STATUS)   (STATUS == S_RESET || STATUS == S_SET)


/**
 * @brief  boolean type enumeration.
 */
typedef enum
{
  S_FALSE = 0,
  S_TRUE  = !S_FALSE

} SpiritBool;

/**
 * @brief  SPIRIT States enumeration.
 */
typedef enum
{
  MC_STATE_STANDBY           =0x40,    /*!< STANDBY */
  MC_STATE_SLEEP             =0x36,    /*!< SLEEP */
  MC_STATE_READY             =0x03,    /*!< READY */
  MC_STATE_PM_SETUP          =0x3D,    /*!< PM_SETUP */
  MC_STATE_XO_SETTLING       =0x23,    /*!< XO_SETTLING */
  MC_STATE_SYNTH_SETUP       =0x53,    /*!< SYNT_SETUP */
  MC_STATE_PROTOCOL          =0x1F,    /*!< PROTOCOL */
  MC_STATE_SYNTH_CALIBRATION =0x4F,    /*!< SYNTH */
  MC_STATE_LOCK              =0x0F,    /*!< LOCK */
  MC_STATE_RX                =0x33,    /*!< RX */
  MC_STATE_TX                =0x5F,    /*!< TX */
  MC_STATE_LOCKWON           =0x13    /*!< LOCKWON */  

} SpiritState;



/**
 * @brief SPIRIT Status. This definition represents the single field of the SPIRIT
 *        status returned on each SPI transaction, equal also to the MC_STATE registers.
 *        This field-oriented structure allows user to address in simple way the single
 *        field of the SPIRIT status.
 *        The user shall define a variable of SpiritStatus type to access on SPIRIT status fields.
 * @note  The fields order in the structure depends on used endianness (little or big
 *        endian). The actual definition is valid ONLY for LITTLE ENDIAN mode. Be sure to
 *        change opportunely the fields order when use a different endianness.
 */

typedef struct
{
  uint8_t XO_ON:1;           /*!< This one bit field notifies if XO is operating
                                 (XO_ON is 1) or not (XO_On is 0) */
  SpiritState MC_STATE: 7;   /*!< This 7 bits field indicates the state of the
                                 Main Controller of SPIRIT. The possible states
                                 and their corresponding values are defined in
                                 @ref SpiritState */
  uint8_t ERROR_LOCK: 1;     /*!< This one bit field notifies if there is an
                                 error on RCO calibration (ERROR_LOCK is 1) or
                                 not (ERROR_LOCK is 0) */
  uint8_t RX_FIFO_EMPTY: 1;  /*!< This one bit field notifies if RX FIFO is empty
                                 (RX_FIFO_EMPTY is 1) or not (RX_FIFO_EMPTY is 0) */
  uint8_t TX_FIFO_FULL: 1;   /*!< This one bit field notifies if TX FIFO is full
                                (TX_FIFO_FULL is 1) or not (TX_FIFO_FULL is 0) */
  uint8_t ANT_SELECT: 1;     /*!< This one bit field notifies the currently selected
                                  antenna */
  uint8_t : 4;               /*!< This 4 bits field are reserved and equal to 5 */

}SpiritStatus;


/**
 * @}
 */

/* --------------------------------   CALIBRATION  ----------------------------------- */ 

/**
 * @defgroup Calibration_Exported_Types Calibration Exported Types
 * @{
 */
 
/**
 * @brief  VCO_H / VCO_L selection.
 */
typedef enum
{

  VCO_L = 0x00,   /*!< VCO lower */
  VCO_H,          /*!< VCO higher */
} VcoSel;


#define IS_VCO_SEL(REF)   (REF == VCO_L  ||\
                           REF == VCO_H \
                           )

/**
 * @}
 */

/* --------------------------------   COMMANDS  ----------------------------------- */ 
 
/**
 * @defgroup Commands_Exported_Types    Commands Exported Types
 * @{
 */

/**
  * @brief  SPIRIT Commands codes enumeration
  */
typedef enum
{
  CMD_TX =  COMMAND_TX,                           /*!< Start to transmit; valid only from READY */
  CMD_RX =  COMMAND_RX,                           /*!< Start to receive; valid only from READY */
  CMD_READY =  COMMAND_READY,                     /*!< Go to READY; valid only from STANDBY or SLEEP or LOCK */
  CMD_STANDBY =  COMMAND_STANDBY,                 /*!< Go to STANDBY; valid only from READY */
  CMD_SLEEP = COMMAND_SLEEP,                      /*!< Go to SLEEP; valid only from READY */
  CMD_LOCKRX = COMMAND_LOCKRX,                    /*!< Go to LOCK state by using the RX configuration of the synth; valid only from READY */
  CMD_LOCKTX = COMMAND_LOCKTX,                    /*!< Go to LOCK state by using the TX configuration of the synth; valid only from READY */
  CMD_SABORT = COMMAND_SABORT,                    /*!< Force exit form TX or RX states and go to READY state; valid only from TX or RX */
  CMD_LDC_RELOAD = COMMAND_LDC_RELOAD,            /*!< LDC Mode: Reload the LDC timer with the value stored in the  LDC_PRESCALER / COUNTER  registers; valid from all states  */
  CMD_SEQUENCE_UPDATE =  COMMAND_SEQUENCE_UPDATE, /*!< Autoretransmission: Reload the Packet sequence counter with the value stored in the PROTOCOL[2] register valid from all states */
  CMD_AES_ENC = COMMAND_AES_ENC,                  /*!< Commands: Start the encryption routine; valid from all states; valid from all states */
  CMD_AES_KEY = COMMAND_AES_KEY,                  /*!< Commands: Start the procedure to compute the key for the decryption; valid from all states */
  CMD_AES_DEC = COMMAND_AES_DEC,                  /*!< Commands: Start the decryption routine using the current key; valid from all states */
  CMD_AES_KEY_DEC = COMMAND_AES_KEY_DEC,          /*!< Commands: Compute the key and start the decryption; valid from all states */
  CMD_SRES = COMMAND_SRES,                        /*!< Reset of all digital part, except SPI registers */
  CMD_FLUSHRXFIFO = COMMAND_FLUSHRXFIFO,          /*!< Clean the RX FIFO; valid from all states */
  CMD_FLUSHTXFIFO = COMMAND_FLUSHTXFIFO,          /*!< Clean the TX FIFO; valid from all states */
} SpiritCmd;

#define IS_SPIRIT_CMD(CMD)  (CMD == CMD_TX || \
                             CMD == CMD_RX || \
                             CMD == CMD_READY || \
                             CMD == CMD_STANDBY || \
                             CMD == CMD_SLEEP || \
                             CMD == CMD_LOCKRX || \
                             CMD == CMD_LOCKTX || \
                             CMD == CMD_SABORT || \
                             CMD == CMD_LDC_RELOAD || \
                             CMD == CMD_SEQUENCE_UPDATE || \
                             CMD == CMD_AES_ENC || \
                             CMD == CMD_AES_KEY || \
                             CMD == CMD_AES_DEC || \
                             CMD == CMD_AES_KEY_DEC || \
                             CMD == CMD_SRES || \
                             CMD == CMD_FLUSHRXFIFO || \
                             CMD == CMD_FLUSHTXFIFO \
                            )

/**
 * @}
 */

/**
 * @defgroup Commands_Exported_Macros   Commands Exported Macros
 * @{
 */

/**
 * @brief  Sends the TX command to SPIRIT. Start to transmit.
 * @param  None.
 * @retval None.
 */
#define SpiritCmdStrobeTx()      {SpiritManagementWaCmdStrobeTx(); \
                                  SpiritCmdStrobeCommand(CMD_TX);} 

/**
 * @brief  Sends the RX command to SPIRIT. Start to receive.
 * @param  None.
 * @retval None.
 */
#define SpiritCmdStrobeRx()      {SpiritManagementWaCmdStrobeRx(); \
                                  SpiritCmdStrobeCommand(CMD_RX); \
                                  } 

/**
 * @brief  Sends the Ready state command to SPIRIT. Go to READY.
 * @param  None.
 * @retval None.
 */
#define SpiritCmdStrobeReady()          SpiritCmdStrobeCommand(CMD_READY)

/**
 * @brief  Sends the Standby command to SPIRIT. Go to STANDBY.
 * @param  None.
 * @retval None.
 */
#define SpiritCmdStrobeStandby()        SpiritCmdStrobeCommand(CMD_STANDBY)

/**
 * @brief  Sends the Sleep command to SPIRIT. Go to SLEEP.
 * @param  None.
 * @retval None.
 */
#define SpiritCmdStrobeSleep()          SpiritCmdStrobeCommand(CMD_SLEEP)

/**
 * @brief  Sends the LOCK_RX command to SPIRIT. Go to the LOCK state by using the RX configuration of the synthesizer.
 * @param  None.
 * @retval None.
 */
#define SpiritCmdStrobeLockRx()         SpiritCmdStrobeCommand(CMD_LOCKRX)

/**
 * @brief  Sends the LOCK_TX command to SPIRIT. Go to the LOCK state by using the TX configuration of the synthesizer.
 * @param  None.
 * @retval None.
 */
#define SpiritCmdStrobeLockTx()         SpiritCmdStrobeCommand(CMD_LOCKTX)

/**
 * @brief  Sends the SABORT command to SPIRIT. Exit from TX or RX states and go to READY state.
 * @param  None.
 * @retval None.
 */
#define SpiritCmdStrobeSabort()         SpiritCmdStrobeCommand(CMD_SABORT)

/**
 * @brief  Sends the LDC_RELOAD command to SPIRIT. Reload the LDC timer with the value stored in the LDC_PRESCALER / COUNTER registers.
 * @param  None.
 * @retval None.
 */
#define SpiritCmdStrobeLdcReload()      SpiritCmdStrobeCommand(CMD_LDC_RELOAD)

/**
 * @brief  Sends the SEQUENCE_UPDATE command to SPIRIT. Reload the Packet sequence counter with the value stored in the PROTOCOL[2] register.
 * @param  None.
 * @retval None.
 */
#define SpiritCmdStrobeSequenceUpdate() SpiritCmdStrobeCommand(CMD_SEQUENCE_UPDATE)

/**
 * @brief  Sends the AES_ENC command to SPIRIT. Starts the encryption routine.
 * @param  None.
 * @retval None.
 */
#define SpiritCmdStrobeAesEnc()         SpiritCmdStrobeCommand(CMD_AES_ENC)

/**
 * @brief  Sends the AES_KEY command to SPIRIT. Starts the procedure to compute the key for the decryption.
 * @param  None.
 * @retval None.
 */
#define SpiritCmdStrobeAesKey()         SpiritCmdStrobeCommand(CMD_AES_KEY)

/**
 * @brief  Sends the AES_DEC command to SPIRIT. Starts the decryption using the current key.
 * @param  None.
 * @retval None.
 */
#define SpiritCmdStrobeAesDec()         SpiritCmdStrobeCommand(CMD_AES_DEC)

/**
 * @brief  Sends the KEY_DEC command to SPIRIT. Computes the key derivation and start the decryption.
 * @param  None.
 * @retval None.
 */
#define SpiritCmdStrobeAesKeyDec()     SpiritCmdStrobeCommand(CMD_AES_KEY_DEC)

/**
 * @brief  Sends the SRES command to SPIRIT. Partial reset: all digital circuit will be reset (exception for SPI only).
 * @param  None.
 * @retval None.
 */
#define SpiritCmdStrobeSres()          SpiritCmdStrobeCommand(CMD_SRES)  
  
/**
 * @brief  Sends the FLUSHRXFIFO command to SPIRIT. Clean the RX FIFO.
 * @param  None.
 * @retval None.
 */
#define SpiritCmdStrobeFlushRxFifo()    SpiritCmdStrobeCommand(CMD_FLUSHRXFIFO)

/**
 * @brief  Sends the FLUSHTXFIFO command to SPIRIT. Clean the TX FIFO.
 * @param  None.
 * @retval None.
 */
#define SpiritCmdStrobeFlushTxFifo()    SpiritCmdStrobeCommand(CMD_FLUSHTXFIFO)

/**
 * @}
 */

 /** @defgroup Configuration_Exported_Constants  Configuration Exported Constants
 * @{
 */
#define DOUBLE_XTAL_THR                         30000000

/**
 * @}
 */
 
/* --------------------------------   CSMA  -------------------------------------- */ 
 
 /**
 * @defgroup Csma_Exported_Types        CSMA Exported Types
 * @{
 */

/**
 * @brief  Multiplier for Tcca time enumeration (Tcca = Multiplier*Tbit).
 */
typedef enum
{
  TBIT_TIME_64 = CSMA_CCA_PERIOD_64TBIT,      /*!< CSMA/CA: Sets CCA period to 64*TBIT */
  TBIT_TIME_128 = CSMA_CCA_PERIOD_128TBIT,    /*!< CSMA/CA: Sets CCA period to 128*TBIT */
  TBIT_TIME_256 = CSMA_CCA_PERIOD_256TBIT,    /*!< CSMA/CA: Sets CCA period to 256*TBIT */
  TBIT_TIME_512 = CSMA_CCA_PERIOD_512TBIT,    /*!< CSMA/CA: Sets CCA period to 512*TBIT */
}CcaPeriod;

#define IS_CCA_PERIOD(PERIOD)   (PERIOD == TBIT_TIME_64 || \
                                 PERIOD == TBIT_TIME_128 || \
                                 PERIOD == TBIT_TIME_256 || \
                                 PERIOD == TBIT_TIME_512)


/**
 * @brief  Multiplier of Tcca time enumeration to obtain Tlisten (Tlisten = [1...15]*Tcca).
 */
typedef enum
{
  TCCA_TIME_0   = 0x00,     /*!< CSMA/CA: Sets CCA length to 0 */
  TCCA_TIME_1   = 0x10,     /*!< CSMA/CA: Sets CCA length to 1*TLISTEN */
  TCCA_TIME_2   = 0x20,     /*!< CSMA/CA: Sets CCA length to 2*TLISTEN */
  TCCA_TIME_3   = 0x30,     /*!< CSMA/CA: Sets CCA length to 3*TLISTEN */
  TCCA_TIME_4   = 0x40,     /*!< CSMA/CA: Sets CCA length to 4*TLISTEN */
  TCCA_TIME_5   = 0x50,     /*!< CSMA/CA: Sets CCA length to 5*TLISTEN */
  TCCA_TIME_6   = 0x60,     /*!< CSMA/CA: Sets CCA length to 6*TLISTEN */
  TCCA_TIME_7   = 0x70,     /*!< CSMA/CA: Sets CCA length to 7*TLISTEN */
  TCCA_TIME_8   = 0x80,     /*!< CSMA/CA: Sets CCA length to 8*TLISTEN */
  TCCA_TIME_9   = 0x90,     /*!< CSMA/CA: Sets CCA length to 9*TLISTEN */
  TCCA_TIME_10  = 0xA0,     /*!< CSMA/CA: Sets CCA length to 10*TLISTEN */
  TCCA_TIME_11  = 0xB0,     /*!< CSMA/CA: Sets CCA length to 11*TLISTEN */
  TCCA_TIME_12  = 0xC0,     /*!< CSMA/CA: Sets CCA length to 12*TLISTEN */
  TCCA_TIME_13  = 0xD0,     /*!< CSMA/CA: Sets CCA length to 13*TLISTEN */
  TCCA_TIME_14  = 0xE0,     /*!< CSMA/CA: Sets CCA length to 14*TLISTEN */
  TCCA_TIME_15  = 0xF0,     /*!< CSMA/CA: Sets CCA length to 15*TLISTEN */
}CsmaLength;

#define IS_CSMA_LENGTH(LENGTH)   (LENGTH == TCCA_TIME_0 || \
                                  LENGTH == TCCA_TIME_1 || \
                                  LENGTH == TCCA_TIME_2 || \
                                  LENGTH == TCCA_TIME_3 || \
                                  LENGTH == TCCA_TIME_4 || \
                                  LENGTH == TCCA_TIME_5 || \
                                  LENGTH == TCCA_TIME_6 || \
                                  LENGTH == TCCA_TIME_7 || \
                                  LENGTH == TCCA_TIME_8 || \
                                  LENGTH == TCCA_TIME_9 || \
                                  LENGTH == TCCA_TIME_10 || \
                                  LENGTH == TCCA_TIME_11 || \
                                  LENGTH == TCCA_TIME_12 || \
                                  LENGTH == TCCA_TIME_13 || \
                                  LENGTH == TCCA_TIME_14 || \
                                  LENGTH == TCCA_TIME_15)


/**
  * @brief  SPIRIT CSMA Init structure definition
  */
typedef struct
{
  SpiritFunctionalState     xCsmaPersistentMode;          /*!< Specifies if the CSMA persistent mode has to be on or off.
                                                               This parameter can be S_ENABLE or S_DISABLE */
  CcaPeriod                 xMultiplierTbit;              /*!< Specifies the Tbit multiplier to obtain the Tcca.
                                                               This parameter can be a value of @ref CcaPeriod */
  CsmaLength                xCcaLength;                   /*!< Specifies the Tcca multiplier to determinate the Tlisten.
                                                               This parameter can be a value of @ref CsmaLength. */
  uint8_t                   cMaxNb;                       /*!< Specifies the max number of backoff cycles. Not used in persistent mode.
                                                               This parameter is an uint8_t. */
  uint16_t                  nBuCounterSeed;               /*!< Specifies the BU counter seed. Not used in persistent mode.
                                                               This parameter can be a value of 16 bits. */
  uint8_t                   cBuPrescaler;                 /*!< Specifies the BU prescaler. Not used in persistent mode.
                                                               This parameter can be a value of 6 bits. */
}CsmaInit;

/**
 *@}
 */


/**
 * @defgroup Csma_Exported_Constants    CSMA Exported Constants
 * @{
 */

#define IS_BU_COUNTER_SEED(SEED)    (SEED!=0)
#define IS_BU_PRESCALER(PRESCALER)  (PRESCALER<64)
#define IS_CMAX_NB(NB)              (NB<8)

/**
 *@}
 */

/* --------------------------------   DIRECT_RF  ----------------------------------- */ 
 
 /**
 * @defgroup DirectRf_Exported_Types    Direct RF Exported Types
 * @{
 */

/**
 * @brief  Direct transmission mode enumeration for SPIRIT.
 */
typedef enum
{
  NORMAL_TX_MODE = 0x00,          /*!< Normal mode, no direct transmission is used */
  DIRECT_TX_FIFO_MODE = 0x04,     /*!< Source is FIFO: payload bits are continuously read from the TX FIFO */
  DIRECT_TX_GPIO_MODE = 0x08,     /*!< Source is GPIO: payload bits are continuously read from one of the GPIO ports and transmitted without any processing */
  PN9_TX_MODE = 0x0C              /*!< A pseudorandom binary sequence is generated internally */
}DirectTx;

#define IS_DIRECT_TX(MODE)  (((MODE) == NORMAL_TX_MODE) || \
                             ((MODE) == DIRECT_TX_FIFO_MODE) || \
                             ((MODE) == DIRECT_TX_GPIO_MODE)  || \
                             ((MODE) == PN9_TX_MODE))

/**
 * @brief  Direct receive mode enumeration for SPIRIT.
 */
typedef enum
{
  NORMAL_RX_MODE = 0x00,          /*!< Normal mode, no direct reception is used */
  DIRECT_RX_FIFO_MODE = 0x10,     /*!< Destination is FIFO: payload bits are continuously written to the RX FIFO and not subjected to any processing*/
  DIRECT_RX_GPIO_MODE = 0x20      /*!< Destination is GPIO: payload bits are continuously written to one of the GPIO ports and not subjected to any processing*/
}DirectRx;

#define IS_DIRECT_RX(MODE)  (((MODE) == NORMAL_RX_MODE) || \
                             ((MODE) == DIRECT_RX_FIFO_MODE) || \
                             ((MODE) == DIRECT_RX_GPIO_MODE))

/**
 *@}
 */

/* --------------------------------   GENERAL  ----------------------------------- */ 
 
 /**
 * @defgroup General_Exported_Types     General Exported Types
 * @{
 */

/**
 * @brief  SPIRIT ModeExtRef enumeration
 */

typedef enum
{
  MODE_EXT_XO = 0,
  MODE_EXT_XIN = !MODE_EXT_XO
} ModeExtRef;

#define IS_MODE_EXT(MODE)   (MODE == MODE_EXT_XO || \
                             MODE == MODE_EXT_XIN)


/**
 * @brief  SPIRIT BatteryLevel enumeration
 */

typedef enum
{
  BLD_LVL_2_7_V = 0,
  BLD_LVL_2_5_V = 1,
  BLD_LVL_2_3_V = 2,
  BLD_LVL_2_1_V = 3
} BatteryLevel;

#define IS_BLD_LVL(MODE)  (MODE == BLD_LVL_2_7_V || \
                           MODE == BLD_LVL_2_5_V || \
                           MODE == BLD_LVL_2_3_V || \
                           MODE == BLD_LVL_2_1_V)


/**
 * @brief  SPIRIT GmConf enumeration
 */

typedef enum
{
  GM_SU_13_2 = 0,
  GM_SU_18_2,
  GM_SU_21_5,
  GM_SU_25_6,
  GM_SU_28_8,
  GM_SU_33_9,
  GM_SU_38_5,
  GM_SU_43_0
} GmConf;

#define IS_GM_CONF(MODE)    (MODE == GM_SU_13_2 || \
                             MODE == GM_SU_18_2 || \
                             MODE == GM_SU_21_5 || \
                             MODE == GM_SU_25_6 || \
                             MODE == GM_SU_28_8 || \
                             MODE == GM_SU_33_9 || \
                             MODE == GM_SU_38_5 || \
                             MODE == GM_SU_43_0)


/**
 * @brief  SPIRIT packet type enumeration
 */

typedef enum
{
  PKT_BASIC = 0x00,
  PKT_MBUS = 0x02,
  PKT_STACK

} PacketType;

#define IS_PKT_TYPE(TYPE)    (TYPE == PKT_BASIC || \
                             TYPE == PKT_MBUS || \
                             TYPE == PKT_STACK || \
                             )


/**
 * @brief  SPIRIT version type enumeration
 */

typedef enum
{
  SPIRIT_VERSION_2_1 = 0x01,    /* Deprecated */
  SPIRIT_VERSION_3_0,           /* The only version of SPIRIT1 */
} SpiritVersion;

/**
 * @}
 */

/**
 * @defgroup General_Exported_Macros            General Exported Macros
 * @{
 */
#define SpiritGeneralLibraryVersion() "Spirit1_Libraries_v.3.2.4"

/**
 * @}
 */

/* --------------------------------   GPIO  ----------------------------------- */ 
 
/** @defgroup Gpio_Exported_Types       GPIO Exported Types
 * @{
 */

/**
 * @brief  SPIRIT GPIO pin enumeration.
 */
typedef enum
{
  SPIRIT_GPIO_0  = GPIO0_CONF_BASE, /*!< GPIO_0 selected */
  SPIRIT_GPIO_1  = GPIO1_CONF_BASE, /*!< GPIO_1 selected */
  SPIRIT_GPIO_2  = GPIO2_CONF_BASE, /*!< GPIO_2 selected */
  SPIRIT_GPIO_3  = GPIO3_CONF_BASE  /*!< GPIO_3 selected */
}SpiritGpioPin;


#define IS_SPIRIT_GPIO(PIN)  ((PIN == SPIRIT_GPIO_0) || \
                              (PIN == SPIRIT_GPIO_1) || \
                              (PIN == SPIRIT_GPIO_2) || \
                              (PIN == SPIRIT_GPIO_3))


/**
 * @brief  SPIRIT GPIO mode enumeration.
 */
typedef enum
{
  SPIRIT_GPIO_MODE_DIGITAL_INPUT      = 0x01, /*!< Digital Input on GPIO */
  SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP  = 0x02, /*!< Digital Output on GPIO (low current) */
  SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP  = 0x03  /*!< Digital Output on GPIO (high current) */
}SpiritGpioMode;

#define IS_SPIRIT_GPIO_MODE(MODE)   ((MODE == SPIRIT_GPIO_MODE_DIGITAL_INPUT) || \
                                     (MODE == SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP) || \
                                     (MODE == SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP))



/**
 * @brief  SPIRIT I/O selection enumeration.
 */
typedef enum
{
  SPIRIT_GPIO_DIG_OUT_IRQ                               = 0x00, /*!< nIRQ (Interrupt Request, active low) , default configuration after POR */
  SPIRIT_GPIO_DIG_OUT_POR_INV                           = 0x08, /*!< POR inverted (active low) */
  SPIRIT_GPIO_DIG_OUT_WUT_EXP                           = 0x10, /*!< Wake-Up Timer expiration: "1" when WUT has expired */
  SPIRIT_GPIO_DIG_OUT_LBD                               = 0x18, /*!< Low battery detection: "1" when battery is below threshold setting */
  SPIRIT_GPIO_DIG_OUT_TX_DATA                           = 0x20, /*!< TX data internal clock output (TX data are sampled on the rising edge of it) */
  SPIRIT_GPIO_DIG_OUT_TX_STATE                          = 0x28, /*!< TX state indication: "1" when Spirit1 is passing in the TX state */
  SPIRIT_GPIO_DIG_OUT_TX_FIFO_ALMOST_EMPTY              = 0x30, /*!< TX FIFO Almost Empty Flag */
  SPIRIT_GPIO_DIG_OUT_TX_FIFO_ALMOST_FULL               = 0x38, /*!< TX FIFO Almost Full Flag */
  SPIRIT_GPIO_DIG_OUT_RX_DATA                           = 0x40, /*!< RX data output */
  SPIRIT_GPIO_DIG_OUT_RX_CLOCK                          = 0x48, /*!< RX clock output (recovered from received data) */
  SPIRIT_GPIO_DIG_OUT_RX_STATE                          = 0x50, /*!< RX state indication: "1" when Spirit1 is passing in the RX state */
  SPIRIT_GPIO_DIG_OUT_RX_FIFO_ALMOST_FULL               = 0x58, /*!< RX FIFO Almost Full Flag */
  SPIRIT_GPIO_DIG_OUT_RX_FIFO_ALMOST_EMPTY              = 0x60, /*!< RX FIFO Almost Empty Flag */
  SPIRIT_GPIO_DIG_OUT_ANTENNA_SWITCH                    = 0x68, /*!< Antenna switch used for antenna diversity  */
  SPIRIT_GPIO_DIG_OUT_VALID_PREAMBLE                    = 0x70, /*!< Valid Preamble Detected Flag */
  SPIRIT_GPIO_DIG_OUT_SYNC_DETECTED                     = 0x78, /*!< Sync WordSync Word Detected Flag */
  SPIRIT_GPIO_DIG_OUT_RSSI_THRESHOLD                    = 0x80, /*!< RSSI above threshold */
  SPIRIT_GPIO_DIG_OUT_MCU_CLOCK                         = 0x88, /*!< MCU Clock */
  SPIRIT_GPIO_DIG_OUT_TX_RX_MODE                        = 0x90, /*!< TX or RX mode indicator (to enable an external range extender) */
  SPIRIT_GPIO_DIG_OUT_VDD                               = 0x98, /*!< VDD (to emulate an additional GPIO of the MCU, programmable by SPI) */
  SPIRIT_GPIO_DIG_OUT_GND                               = 0xA0, /*!< GND (to emulate an additional GPIO of the MCU, programmable by SPI) */
  SPIRIT_GPIO_DIG_OUT_SMPS_EXT                          = 0xA8, /*!< External SMPS enable signal (active high) */
  SPIRIT_GPIO_DIG_OUT_SLEEP_OR_STANDBY                  = 0xB0,
  SPIRIT_GPIO_DIG_OUT_READY                             = 0xB8,
  SPIRIT_GPIO_DIG_OUT_LOCK                              = 0xC0,
  SPIRIT_GPIO_DIG_OUT_WAIT_FOR_LOCK_SIG                 = 0xC8,
  SPIRIT_GPIO_DIG_OUT_WAIT_FOR_TIMER_FOR_LOCK           = 0xD0,
  SPIRIT_GPIO_DIG_OUT_WAIT_FOR_READY2_SIG               = 0xD8,
  SPIRIT_GPIO_DIG_OUT_WAIT_FOR_TIMER_FOR_PM_SET         = 0xE0,
  SPIRIT_GPIO_DIG_OUT_WAIT_VCO_CALIBRATION              = 0xE8,
  SPIRIT_GPIO_DIG_OUT_ENABLE_SYNTH_FULL_CIRCUIT         = 0xF0,
  SPIRIT_GPIO_DIG_OUT_WAIT_FOR_RCCAL_OK_SIG             = 0xFF,

  SPIRIT_GPIO_DIG_IN_TX_COMMAND                         = 0x00,
  SPIRIT_GPIO_DIG_IN_RX_COMMAND                         = 0x08,
  SPIRIT_GPIO_DIG_IN_TX_DATA_INPUT_FOR_DIRECTRF         = 0x10,
  SPIRIT_GPIO_DIG_IN_DATA_WAKEUP                        = 0x18,
  SPIRIT_GPIO_DIG_IN_EXT_CLOCK_AT_34_7KHZ               = 0x20

}SpiritGpioIO;

#define IS_SPIRIT_GPIO_IO(IO_SEL)        ((IO_SEL == SPIRIT_GPIO_DIG_OUT_IRQ) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_POR_INV) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_WUT_EXP) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_LBD) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_TX_DATA) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_TX_STATE) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_TX_FIFO_ALMOST_EMPTY) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_TX_FIFO_ALMOST_FULL) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_RX_DATA) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_RX_CLOCK) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_RX_STATE) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_RX_FIFO_ALMOST_FULL) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_RX_FIFO_ALMOST_EMPTY) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_ANTENNA_SWITCH) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_VALID_PREAMBLE) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_SYNC_DETECTED) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_RSSI_THRESHOLD) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_MCU_CLOCK) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_TX_RX_MODE) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_VDD) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_GND) || \
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_SMPS_EXT) ||\
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_SLEEP_OR_STANDBY) ||\
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_READY) ||\
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_LOCK) ||\
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_WAIT_FOR_LOCK_SIG) ||\
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_WAIT_FOR_TIMER_FOR_LOCK) ||\
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_WAIT_FOR_READY2_SIG) ||\
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_WAIT_FOR_TIMER_FOR_PM_SET) ||\
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_WAIT_VCO_CALIBRATION) ||\
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_ENABLE_SYNTH_FULL_CIRCUIT) ||\
                                          (IO_SEL == SPIRIT_GPIO_DIG_OUT_WAIT_FOR_RCCAL_OK_SIG) ||\
                                          (IO_SEL == SPIRIT_GPIO_DIG_IN_TX_COMMAND) ||\
                                          (IO_SEL == SPIRIT_GPIO_DIG_IN_RX_COMMAND) ||\
                                          (IO_SEL == SPIRIT_GPIO_DIG_IN_TX_DATA_INPUT_FOR_DIRECTRF) ||\
                                          (IO_SEL == SPIRIT_GPIO_DIG_IN_DATA_WAKEUP) ||\
                                          (IO_SEL == SPIRIT_GPIO_DIG_IN_EXT_CLOCK_AT_34_7KHZ))

/**
 * @brief  SPIRIT OutputLevel enumeration.
 */

typedef enum
{
  LOW = 0,
  HIGH = !LOW
}OutputLevel;

#define IS_SPIRIT_GPIO_LEVEL(LEVEL)      ((LEVEL == LOW) || \
                                          (LEVEL == HIGH))

/**
 * @brief  SPIRIT GPIO Init structure definition.
 */
typedef struct
{
  SpiritGpioPin xSpiritGpioPin;    /*!< Specifies the GPIO pins to be configured.
                                        This parameter can be any value of @ref SpiritGpioPin */

  SpiritGpioMode xSpiritGpioMode;  /*!< Specifies the operating mode for the selected pins.
                                        This parameter can be a value of @ref SpiritGpioMode */

  SpiritGpioIO xSpiritGpioIO;      /*!< Specifies the I/O selection for the selected pins.
                                        This parameter can be a value of @ref SpiritGpioIO */

}SGpioInit;

/**
 * @brief  SPIRIT clock output XO prescaler enumeration.
 */

typedef enum
{
 XO_RATIO_1       = 0x00, /*!< XO Clock signal available on the GPIO divided by 1     */
 XO_RATIO_2_3     = 0x02, /*!< XO Clock signal available on the GPIO divided by 2/3   */
 XO_RATIO_1_2     = 0x04, /*!< XO Clock signal available on the GPIO divided by 1/2   */
 XO_RATIO_1_3     = 0x06, /*!< XO Clock signal available on the GPIO divided by 1/3   */
 XO_RATIO_1_4     = 0x08, /*!< XO Clock signal available on the GPIO divided by 1/4   */
 XO_RATIO_1_6     = 0x0A, /*!< XO Clock signal available on the GPIO divided by 1/6   */
 XO_RATIO_1_8     = 0x0C, /*!< XO Clock signal available on the GPIO divided by 1/8   */
 XO_RATIO_1_12    = 0x0E, /*!< XO Clock signal available on the GPIO divided by 1/12  */
 XO_RATIO_1_16    = 0x10, /*!< XO Clock signal available on the GPIO divided by 1/16  */
 XO_RATIO_1_24    = 0x12, /*!< XO Clock signal available on the GPIO divided by 1/24  */
 XO_RATIO_1_36    = 0x14, /*!< XO Clock signal available on the GPIO divided by 1/36  */
 XO_RATIO_1_48    = 0x16, /*!< XO Clock signal available on the GPIO divided by 1/48  */
 XO_RATIO_1_64    = 0x18, /*!< XO Clock signal available on the GPIO divided by 1/64  */
 XO_RATIO_1_96    = 0x1A, /*!< XO Clock signal available on the GPIO divided by 1/96  */
 XO_RATIO_1_128   = 0x1C, /*!< XO Clock signal available on the GPIO divided by 1/128 */
 XO_RATIO_1_192   = 0x1E  /*!< XO Clock signal available on the GPIO divided by 1/196 */
}ClockOutputXOPrescaler;

#define IS_SPIRIT_CLOCK_OUTPUT_XO(RATIO) ((RATIO == XO_RATIO_1) || \
                                           (RATIO == XO_RATIO_2_3) || \
                                           (RATIO == XO_RATIO_1_2) || \
                                           (RATIO == XO_RATIO_1_3) || \
                                           (RATIO == XO_RATIO_1_4) || \
                                           (RATIO == XO_RATIO_1_6) || \
                                           (RATIO == XO_RATIO_1_8) || \
                                           (RATIO == XO_RATIO_1_12) || \
                                           (RATIO == XO_RATIO_1_16) || \
                                           (RATIO == XO_RATIO_1_24) || \
                                           (RATIO == XO_RATIO_1_36) || \
                                           (RATIO == XO_RATIO_1_48) || \
                                           (RATIO == XO_RATIO_1_64) || \
                                           (RATIO == XO_RATIO_1_96) || \
                                           (RATIO == XO_RATIO_1_128) || \
                                           (RATIO == XO_RATIO_1_192))

/**
 * @brief  SPIRIT Clock Output RCO prescaler enumeration.
 */

typedef enum
{
 RCO_RATIO_1          = 0x00, /*!< RCO Clock signal available on the GPIO divided by 1     */
 RCO_RATIO_1_128      = 0x01  /*!< RCO Clock signal available on the GPIO divided by 1/128   */
}ClockOutputRCOPrescaler;

#define IS_SPIRIT_CLOCK_OUTPUT_RCO(RATIO) ((RATIO == RCO_RATIO_1) || \
                                           (RATIO == RCO_RATIO_1_128))

/**
 * @brief  SPIRIT ExtraClockCycles enumeration.
 */

typedef enum
{
EXTRA_CLOCK_CYCLES_0     = 0x00, /*!< 0   extra clock cycles provided to the MCU before switching to STANDBY state */
EXTRA_CLOCK_CYCLES_64    = 0x20, /*!< 64  extra clock cycles provided to the MCU before switching to STANDBY state */
EXTRA_CLOCK_CYCLES_256   = 0x40, /*!< 256 extra clock cycles provided to the MCU before switching to STANDBY state */
EXTRA_CLOCK_CYCLES_512   = 0x60  /*!< 512 extra clock cycles provided to the MCU before switching to STANDBY state */
}ExtraClockCycles;

#define IS_SPIRIT_CLOCK_OUTPUT_EXTRA_CYCLES(CYCLES) ((CYCLES == EXTRA_CLOCK_CYCLES_0) || \
                                                      (CYCLES == EXTRA_CLOCK_CYCLES_64) || \
                                                      (CYCLES == EXTRA_CLOCK_CYCLES_256) || \
                                                      (CYCLES == EXTRA_CLOCK_CYCLES_512))

/**
 * @brief  SPIRIT Clock Output initialization structure definition.
 */
typedef struct
{
  ClockOutputXOPrescaler   xClockOutputXOPrescaler;  /*!< Specifies the XO Ratio as clock output.
                                                          This parameter can be any value of @ref ClockOutputXOPrescaler */

  ClockOutputRCOPrescaler  xClockOutputRCOPrescaler; /*!< Specifies the RCO Ratio as clock output.
                                                          This parameter can be a value of @ref ClockOutputRCOPrescaler */

  ExtraClockCycles         xExtraClockCycles;       /*!< Specifies the Extra Clock Cycles provided before entering in Standby State.
                                                          This parameter can be a value of @ref ExtraClockCycles */

}ClockOutputInit;

/**
 * @}
 */

/* --------------------------------   IRQ  ----------------------------------- */ 
 
/**
 * @defgroup Irq_Exported_Types IRQ Exported Types
 * @{
 */

/**
 * @brief IRQ bitfield structure for SPIRIT. This structure is used to read or write the single IRQ bit.
 *        During the initialization the user has to fill this structure setting to one the single field related
 *        to the IRQ he wants to enable, and to zero the single field related to all the IRQs he wants to disable.
 *        The same structure can be used to retrieve all the IRQ events from the IRQ registers IRQ_STATUS[3:0],
 *        and read if one or more specific IRQ raised.
 * @note  The fields order in the structure depends on used endianness (little or big
 *        endian). The actual definition is valid ONLY for LITTLE ENDIAN mode. Be sure to
 *        change opportunely the fields order when use a different endianness.
 */
typedef struct
{
  SpiritFlagStatus  IRQ_RX_DATA_READY:1;            /*!< IRQ: RX data ready */
  SpiritFlagStatus  IRQ_RX_DATA_DISC:1;             /*!< IRQ: RX data discarded (upon filtering) */
  SpiritFlagStatus  IRQ_TX_DATA_SENT:1;             /*!< IRQ: TX data sent */
  SpiritFlagStatus  IRQ_MAX_RE_TX_REACH:1;          /*!< IRQ: Max re-TX reached */
  SpiritFlagStatus  IRQ_CRC_ERROR:1;                /*!< IRQ: CRC error */
  SpiritFlagStatus  IRQ_TX_FIFO_ERROR:1;            /*!< IRQ: TX FIFO underflow/overflow error */
  SpiritFlagStatus  IRQ_RX_FIFO_ERROR:1;            /*!< IRQ: RX FIFO underflow/overflow error */
  SpiritFlagStatus  IRQ_TX_FIFO_ALMOST_FULL:1;      /*!< IRQ: TX FIFO almost full */

  SpiritFlagStatus  IRQ_TX_FIFO_ALMOST_EMPTY:1;     /*!< IRQ: TX FIFO almost empty */
  SpiritFlagStatus  IRQ_RX_FIFO_ALMOST_FULL:1;      /*!< IRQ: RX FIFO almost full */
  SpiritFlagStatus  IRQ_RX_FIFO_ALMOST_EMPTY:1;     /*!< IRQ: RX FIFO almost empty  */
  SpiritFlagStatus  IRQ_MAX_BO_CCA_REACH:1;         /*!< IRQ: Max number of back-off during CCA */
  SpiritFlagStatus  IRQ_VALID_PREAMBLE:1;           /*!< IRQ: Valid preamble detected */
  SpiritFlagStatus  IRQ_VALID_SYNC:1;               /*!< IRQ: Sync word detected */
  SpiritFlagStatus  IRQ_RSSI_ABOVE_TH:1;            /*!< IRQ: RSSI above threshold */
  SpiritFlagStatus  IRQ_WKUP_TOUT_LDC:1;            /*!< IRQ: Wake-up timeout in LDC mode */

  SpiritFlagStatus  IRQ_READY:1;                    /*!< IRQ: READY state */
  SpiritFlagStatus  IRQ_STANDBY_DELAYED:1;          /*!< IRQ: STANDBY state after MCU_CK_CONF_CLOCK_TAIL_X clock cycles */
  SpiritFlagStatus  IRQ_LOW_BATT_LVL:1;             /*!< IRQ: Battery level below threshold*/
  SpiritFlagStatus  IRQ_POR:1;                      /*!< IRQ: Power On Reset */
  SpiritFlagStatus  IRQ_BOR:1;                      /*!< IRQ: Brown out event (both accurate and inaccurate)*/
  SpiritFlagStatus  IRQ_LOCK:1;                     /*!< IRQ: LOCK state */
  SpiritFlagStatus  IRQ_PM_COUNT_EXPIRED:1;         /*!< IRQ: only for debug; Power Management startup timer expiration (see reg PM_START_COUNTER, 0xB5) */
  SpiritFlagStatus  IRQ_XO_COUNT_EXPIRED:1;         /*!< IRQ: only for debug; Crystal oscillator settling time counter expired */

  SpiritFlagStatus  IRQ_SYNTH_LOCK_TIMEOUT:1;       /*!< IRQ: only for debug; LOCK state timeout */
  SpiritFlagStatus  IRQ_SYNTH_LOCK_STARTUP:1;       /*!< IRQ: only for debug; see CALIBR_START_COUNTER */
  SpiritFlagStatus  IRQ_SYNTH_CAL_TIMEOUT:1;        /*!< IRQ: only for debug; SYNTH calibration timeout */
  SpiritFlagStatus  IRQ_TX_START_TIME:1;            /*!< IRQ: only for debug; TX circuitry startup time; see TX_START_COUNTER */
  SpiritFlagStatus  IRQ_RX_START_TIME:1;            /*!< IRQ: only for debug; RX circuitry startup time; see TX_START_COUNTER */
  SpiritFlagStatus  IRQ_RX_TIMEOUT:1;               /*!< IRQ: RX operation timeout */
  SpiritFlagStatus  IRQ_AES_END:1;                  /*!< IRQ: AES End of operation */
  SpiritFlagStatus  :1;                             /*!< Reserved bit */

} SpiritIrqs;

/**
 * @brief  IRQ list enumeration for SPIRIT. This enumeration type can be used to address a
 *         specific IRQ.
 */
typedef enum
{
  RX_DATA_READY = 0x00000001,           /*!< IRQ: RX data ready */
  RX_DATA_DISC = 0x00000002,            /*!< IRQ: RX data discarded (upon filtering) */
  TX_DATA_SENT = 0x00000004,            /*!< IRQ: TX data sent */
  MAX_RE_TX_REACH = 0x00000008,         /*!< IRQ: Max re-TX reached */
  CRC_ERROR = 0x00000010,               /*!< IRQ: CRC error */
  TX_FIFO_ERROR = 0x00000020,           /*!< IRQ: TX FIFO underflow/overflow error */
  RX_FIFO_ERROR = 0x00000040,           /*!< IRQ: RX FIFO underflow/overflow error */
  TX_FIFO_ALMOST_FULL = 0x00000080,     /*!< IRQ: TX FIFO almost full */
  TX_FIFO_ALMOST_EMPTY = 0x00000100,    /*!< IRQ: TX FIFO almost empty */
  RX_FIFO_ALMOST_FULL = 0x00000200,     /*!< IRQ: RX FIFO almost full */
  RX_FIFO_ALMOST_EMPTY = 0x00000400,    /*!< IRQ: RX FIFO almost empty  */
  MAX_BO_CCA_REACH = 0x00000800,        /*!< IRQ: Max number of back-off during CCA */
  VALID_PREAMBLE = 0x00001000,          /*!< IRQ: Valid preamble detected */
  VALID_SYNC = 0x00002000,              /*!< IRQ: Sync word detected */
  RSSI_ABOVE_TH = 0x00004000,           /*!< IRQ: RSSI above threshold */
  WKUP_TOUT_LDC = 0x00008000,           /*!< IRQ: Wake-up timeout in LDC mode */
  READY = 0x00010000,                   /*!< IRQ: READY state */
  STANDBY_DELAYED = 0x00020000,         /*!< IRQ: STANDBY state after MCU_CK_CONF_CLOCK_TAIL_X clock cycles */
  LOW_BATT_LVL = 0x00040000,            /*!< IRQ: Battery level below threshold*/
  POR = 0x00080000,                     /*!< IRQ: Power On Reset */
  BOR = 0x00100000,                     /*!< IRQ: Brown out event (both accurate and inaccurate)*/
  LOCK = 0x00200000,                    /*!< IRQ: LOCK state */
  PM_COUNT_EXPIRED = 0x00400000,        /*!< IRQ: only for debug; Power Management startup timer expiration (see reg PM_START_COUNTER, 0xB5) */
  XO_COUNT_EXPIRED = 0x00800000,        /*!< IRQ: only for debug; Crystal oscillator settling time counter expired */
  SYNTH_LOCK_TIMEOUT = 0x01000000,      /*!< IRQ: only for debug; LOCK state timeout */
  SYNTH_LOCK_STARTUP = 0x02000000,      /*!< IRQ: only for debug; see CALIBR_START_COUNTER */
  SYNTH_CAL_TIMEOUT = 0x04000000,       /*!< IRQ: only for debug; SYNTH calibration timeout */
  TX_START_TIME = 0x08000000,           /*!< IRQ: only for debug; TX circuitry startup time; see TX_START_COUNTER */
  RX_START_TIME = 0x10000000,           /*!< IRQ: only for debug; RX circuitry startup time; see TX_START_COUNTER */
  RX_TIMEOUT = 0x20000000,              /*!< IRQ: RX operation timeout */
  AES_END = 0x40000000,                 /*!< IRQ: AES End of operation */
  ALL_IRQ = 0x7FFFFFFF                  /*!< All the above mentioned IRQs */

} IrqList;

#define IS_SPIRIT_IRQ_LIST(VALUE)   ((VALUE == RX_DATA_READY) || \
                                     (VALUE == RX_DATA_DISC)  || \
                                     (VALUE == TX_DATA_SENT)  || \
                                     (VALUE == MAX_RE_TX_REACH)  || \
                                     (VALUE == CRC_ERROR)  || \
                                     (VALUE == TX_FIFO_ERROR)  || \
                                     (VALUE == RX_FIFO_ERROR)  || \
                                     (VALUE == TX_FIFO_ALMOST_FULL)  || \
                                     (VALUE == TX_FIFO_ALMOST_EMPTY)  || \
                                     (VALUE == RX_FIFO_ALMOST_FULL)  || \
                                     (VALUE == RX_FIFO_ALMOST_EMPTY)  || \
                                     (VALUE == MAX_BO_CCA_REACH)  || \
                                     (VALUE == VALID_PREAMBLE)  || \
                                     (VALUE == VALID_SYNC)  || \
                                     (VALUE == RSSI_ABOVE_TH)  || \
                                     (VALUE == WKUP_TOUT_LDC)  || \
                                     (VALUE == READY)  || \
                                     (VALUE == STANDBY_DELAYED)  || \
                                     (VALUE == LOW_BATT_LVL)  || \
                                     (VALUE == POR)  || \
                                     (VALUE == BOR)  || \
                                     (VALUE == LOCK)  || \
                                     (VALUE == PM_COUNT_EXPIRED)  || \
                                     (VALUE == XO_COUNT_EXPIRED)  || \
                                     (VALUE == SYNTH_LOCK_TIMEOUT)  || \
                                     (VALUE == SYNTH_LOCK_STARTUP)  || \
                                     (VALUE == SYNTH_CAL_TIMEOUT)  || \
                                     (VALUE == TX_START_TIME)  || \
                                     (VALUE == RX_START_TIME)  || \
                                     (VALUE == RX_TIMEOUT)  || \
                                     (VALUE == AES_END)   || \
                                     (VALUE == ALL_IRQ ))

/**
 * @}
 */

 /* --------------------------------   LINEAR FIFO  ----------------------------------- */ 

/**
 * @defgroup LinearFifo_Exported_Constants      Linear FIFO Exported Constants
 * @{
 */
#define IS_FIFO_THR(VAL)  (VAL<=96)

/**
 * @}
 */

/* --------------------------------   PKT_COMMON  ----------------------------------- */  

/**
 * @defgroup PktCommon_Exported_Types   Pkt Common Exported Types
 * @{
 */

/**
 * @brief  Preamble length in bytes enumeration.
 */
typedef enum
{
  PKT_PREAMBLE_LENGTH_01BYTE            = 0x00, /*!< Preamble length 1 byte*/
  PKT_PREAMBLE_LENGTH_02BYTES           = 0x08, /*!< Preamble length 2 bytes */
  PKT_PREAMBLE_LENGTH_03BYTES           = 0x10, /*!< Preamble length 3 bytes */
  PKT_PREAMBLE_LENGTH_04BYTES           = 0x18, /*!< Preamble length 4 bytes */
  PKT_PREAMBLE_LENGTH_05BYTES           = 0x20, /*!< Preamble length 5 bytes */
  PKT_PREAMBLE_LENGTH_06BYTES           = 0x28, /*!< Preamble length 6 bytes */
  PKT_PREAMBLE_LENGTH_07BYTES           = 0x30, /*!< Preamble length 7 bytes */
  PKT_PREAMBLE_LENGTH_08BYTES           = 0x38, /*!< Preamble length 8 bytes */
  PKT_PREAMBLE_LENGTH_09BYTES           = 0x40, /*!< Preamble length 9 bytes */
  PKT_PREAMBLE_LENGTH_10BYTES           = 0x48, /*!< Preamble length 10 bytes */
  PKT_PREAMBLE_LENGTH_11BYTES           = 0x50, /*!< Preamble length 11 bytes */
  PKT_PREAMBLE_LENGTH_12BYTES           = 0x58, /*!< Preamble length 12 bytes */
  PKT_PREAMBLE_LENGTH_13BYTES           = 0x60, /*!< Preamble length 13 bytes */
  PKT_PREAMBLE_LENGTH_14BYTES           = 0x68, /*!< Preamble length 14 bytes */
  PKT_PREAMBLE_LENGTH_15BYTES           = 0x70, /*!< Preamble length 15 bytes */
  PKT_PREAMBLE_LENGTH_16BYTES           = 0x78, /*!< Preamble length 16 bytes */
  PKT_PREAMBLE_LENGTH_17BYTES           = 0x80, /*!< Preamble length 17 bytes */
  PKT_PREAMBLE_LENGTH_18BYTES           = 0x88, /*!< Preamble length 18 bytes */
  PKT_PREAMBLE_LENGTH_19BYTES           = 0x90, /*!< Preamble length 19 bytes */
  PKT_PREAMBLE_LENGTH_20BYTES           = 0x98, /*!< Preamble length 20 bytes */
  PKT_PREAMBLE_LENGTH_21BYTES           = 0xA0, /*!< Preamble length 21 bytes */
  PKT_PREAMBLE_LENGTH_22BYTES           = 0xA8, /*!< Preamble length 22 bytes */
  PKT_PREAMBLE_LENGTH_23BYTES           = 0xB0, /*!< Preamble length 23 bytes */
  PKT_PREAMBLE_LENGTH_24BYTES           = 0xB8, /*!< Preamble length 24 bytes */
  PKT_PREAMBLE_LENGTH_25BYTES           = 0xC0, /*!< Preamble length 25 bytes */
  PKT_PREAMBLE_LENGTH_26BYTES           = 0xC8, /*!< Preamble length 26 bytes */
  PKT_PREAMBLE_LENGTH_27BYTES           = 0xD0, /*!< Preamble length 27 bytes */
  PKT_PREAMBLE_LENGTH_28BYTES           = 0xD8, /*!< Preamble length 28 bytes */
  PKT_PREAMBLE_LENGTH_29BYTES           = 0xE0, /*!< Preamble length 29 bytes */
  PKT_PREAMBLE_LENGTH_30BYTES           = 0xE8, /*!< Preamble length 30 bytes */
  PKT_PREAMBLE_LENGTH_31BYTES           = 0xF0, /*!< Preamble length 31 bytes */
  PKT_PREAMBLE_LENGTH_32BYTES           = 0xF8  /*!< Preamble length 32 bytes */

}PktPreambleLength;

#define IS_PKT_PREAMBLE_LENGTH(LENGTH)  ((LENGTH == PKT_PREAMBLE_LENGTH_01BYTE) || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_02BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_03BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_04BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_05BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_06BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_07BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_08BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_09BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_10BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_11BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_12BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_13BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_14BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_15BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_16BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_17BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_18BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_19BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_20BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_21BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_22BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_23BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_24BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_25BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_26BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_27BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_28BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_29BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_30BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_31BYTES)  || \
                                           (LENGTH == PKT_PREAMBLE_LENGTH_32BYTES))

/**
 * @brief  Sync length in bytes enumeration.
 */
typedef enum
{
  PKT_SYNC_LENGTH_1BYTE            = 0x00, /*!< Sync length 1 byte*/
  PKT_SYNC_LENGTH_2BYTES           = 0x02, /*!< Sync length 2 bytes*/
  PKT_SYNC_LENGTH_3BYTES           = 0x04, /*!< Sync length 3 bytes */
  PKT_SYNC_LENGTH_4BYTES           = 0x06 , /*!< Sync length 4 bytes */

}PktSyncLength;

#define IS_PKT_SYNC_LENGTH(LENGTH)     ((LENGTH == PKT_SYNC_LENGTH_1BYTE) || \
                                          (LENGTH == PKT_SYNC_LENGTH_2BYTES)|| \
                                          (LENGTH == PKT_SYNC_LENGTH_3BYTES)|| \
                                          (LENGTH == PKT_SYNC_LENGTH_4BYTES))

/**
 * @brief  CRC length in bytes enumeration.
 */
typedef enum
{
  PKT_NO_CRC               = 0x00, /*!< No CRC                              */
  PKT_CRC_MODE_8BITS       = 0x20, /*!< CRC length 8 bits  - poly: 0x07     */
  PKT_CRC_MODE_16BITS_1    = 0x40, /*!< CRC length 16 bits - poly: 0x8005   */
  PKT_CRC_MODE_16BITS_2    = 0x60, /*!< CRC length 16 bits - poly: 0x1021   */
  PKT_CRC_MODE_24BITS      = 0x80, /*!< CRC length 24 bits - poly: 0x864CFB */

}PktCrcMode;

#define IS_PKT_CRC_MODE(MODE)   ((MODE == PKT_NO_CRC) || \
                                   (MODE == PKT_CRC_MODE_8BITS)  || \
                                   (MODE == PKT_CRC_MODE_16BITS_1)  || \
                                   (MODE == PKT_CRC_MODE_16BITS_2) || \
                                   (MODE == PKT_CRC_MODE_24BITS))

/**
 * @brief  Fixed or variable payload length enumeration.
 */
typedef enum
{
  PKT_LENGTH_FIX  = 0x00,    /*!< Fixed payload length     */
  PKT_LENGTH_VAR  = 0x01     /*!< Variable payload length  */

}PktFixVarLength;

#define IS_PKT_FIX_VAR_LENGTH(LENGTH)   ((LENGTH == PKT_LENGTH_FIX) || \
                                           (LENGTH == PKT_LENGTH_VAR))

/**
 * @brief  Control length in bytes enumeration for SPIRIT packets.
 */
typedef enum
{
  PKT_CONTROL_LENGTH_0BYTES = 0x00,     /*!< Control length 0 byte*/
  PKT_CONTROL_LENGTH_1BYTE,             /*!< Control length 1 byte*/
  PKT_CONTROL_LENGTH_2BYTES,            /*!< Control length 2 bytes*/
  PKT_CONTROL_LENGTH_3BYTES,            /*!< Control length 3 bytes*/
  PKT_CONTROL_LENGTH_4BYTES             /*!< Control length 4 bytes*/

}PktControlLength;

#define IS_PKT_CONTROL_LENGTH(LENGTH) ((LENGTH == PKT_CONTROL_LENGTH_0BYTES) || \
                                         (LENGTH == PKT_CONTROL_LENGTH_1BYTE)   || \
                                         (LENGTH == PKT_CONTROL_LENGTH_2BYTES)  || \
                                         (LENGTH == PKT_CONTROL_LENGTH_3BYTES)  || \
                                         (LENGTH == PKT_CONTROL_LENGTH_4BYTES))

/**
 * @brief  Sync words enumeration for SPIRIT packets.
 */
typedef enum
{
  PKT_SYNC_WORD_1=0x01,  /*!< Index of the 1st sync word*/
  PKT_SYNC_WORD_2,       /*!< Index of the 2nd sync word*/
  PKT_SYNC_WORD_3,       /*!< Index of the 3rd sync word*/
  PKT_SYNC_WORD_4        /*!< Index of the 4th sync word*/

}PktSyncX;

#define IS_PKT_SYNCx(WORD)    ((WORD == PKT_SYNC_WORD_1) || \
                                 (WORD == PKT_SYNC_WORD_2) || \
                                 (WORD == PKT_SYNC_WORD_3) || \
                                 (WORD == PKT_SYNC_WORD_4))

/**
 * @brief  Max retransmissions number enumeration for SPIRIT packets.
 */
typedef enum
{
  PKT_DISABLE_RETX    = 0x00,   /*!< No retrasmissions*/
  PKT_N_RETX_1        = 0x10,   /*!< Max retrasmissions 1*/
  PKT_N_RETX_2        = 0x20,   /*!< Max retrasmissions 2*/
  PKT_N_RETX_3        = 0x30,   /*!< Max retrasmissions 3*/
  PKT_N_RETX_4        = 0x40,   /*!< Max retrasmissions 4*/
  PKT_N_RETX_5        = 0x50,   /*!< Max retrasmissions 5*/
  PKT_N_RETX_6        = 0x60,   /*!< Max retrasmissions 6*/
  PKT_N_RETX_7        = 0x70,   /*!< Max retrasmissions 7*/
  PKT_N_RETX_8        = 0x80,   /*!< Max retrasmissions 8*/
  PKT_N_RETX_9        = 0x90,   /*!< Max retrasmissions 9*/
  PKT_N_RETX_10       = 0xA0,   /*!< Max retrasmissions 10*/
  PKT_N_RETX_11       = 0xB0,   /*!< Max retrasmissions 11*/
  PKT_N_RETX_12       = 0xC0,   /*!< Max retrasmissions 12*/
  PKT_N_RETX_13       = 0xD0,   /*!< Max retrasmissions 13*/
  PKT_N_RETX_14       = 0xE0,   /*!< Max retrasmissions 14*/
  PKT_N_RETX_15       = 0xF0    /*!< Max retrasmissions 15*/

}PktNMaxReTx;

#define IS_PKT_NMAX_RETX(N_RETX)    ((N_RETX == PKT_DISABLE_RETX) || \
                                       (N_RETX == PKT_N_RETX_1) || \
                                       (N_RETX == PKT_N_RETX_2) || \
                                       (N_RETX == PKT_N_RETX_3) || \
                                       (N_RETX == PKT_N_RETX_4) || \
                                       (N_RETX == PKT_N_RETX_5) || \
                                       (N_RETX == PKT_N_RETX_6) || \
                                       (N_RETX == PKT_N_RETX_7) || \
                                       (N_RETX == PKT_N_RETX_8) || \
                                       (N_RETX == PKT_N_RETX_9) || \
                                       (N_RETX == PKT_N_RETX_10) || \
                                       (N_RETX == PKT_N_RETX_11) || \
                                       (N_RETX == PKT_N_RETX_12) || \
                                       (N_RETX == PKT_N_RETX_13) || \
                                       (N_RETX == PKT_N_RETX_14) || \
                                       (N_RETX == PKT_N_RETX_15))

/**
 *@}
 */

/**
 * @defgroup PktCommon_Exported_Constants               Pkt Common Exported Constants
 * @{
 */
#define IS_PKT_LENGTH_WIDTH_BITS(BITS)                (BITS<=16)
#define IS_PKT_SEQ_NUMBER_RELOAD(SEQN)                (SEQN<=3)

/**
 *@}
 */

/**
 * @defgroup PktCommon_Exported_Macros                  Pkt Common Exported Macros
 * @{
 */


/**
 * @brief  Macro used to compute the lower part of the packet length, to write in the PCKTLEN0 register
 * @param  nLength Length of the packet payload.
 *         This parameter is an uint16_t.
 * @retval None.
 */
#define BUILD_PCKTLEN0(nLength) ((nLength) & 0xFF)

/**
 * @brief  Macro used to compute the upper part of the packet length, to write the PCKTLEN1 register
 * @param  nLength Length of the packet payload.
 *         This parameter is an uint16_t.
 * @retval None.
 */
#define BUILD_PCKTLEN1(nLength) ((nLength) >> 8)

/**
 *@}
 */

/**
 * @defgroup PktBasic_Exported_Types    Pkt Basic Exported Types
 * @{
 */

/**
 * @brief  Preamble length in bytes enumeration.
 */
typedef PktPreambleLength                  BasicPreambleLength;
#define IS_BASIC_PREAMBLE_LENGTH           IS_PKT_PREAMBLE_LENGTH

/**
 * @brief  Sync length in bytes enumeration.
 */
typedef PktSyncLength                      BasicSyncLength;
#define IS_BASIC_SYNC_LENGTH               IS_PKT_SYNC_LENGTH

/**
 * @brief  CRC length in bytes enumeration.
 */
typedef PktCrcMode                         BasicCrcMode;
#define IS_BASIC_CRC_MODE                  IS_PKT_CRC_MODE

/**
 * @brief  Fixed or variable payload length enumeration.
 */
typedef PktFixVarLength                    BasicFixVarLength;
#define IS_BASIC_FIX_VAR_LENGTH            IS_PKT_FIX_VAR_LENGTH

/**
 * @brief  Control length in bytes enumeration.
 */
typedef PktControlLength                   BasicControlLength;
#define IS_BASIC_CONTROL_LENGTH            IS_PKT_CONTROL_LENGTH

/**
 * @brief  Sync words enumeration.
 */
typedef PktSyncX                           BasicSyncX;
#define IS_BASIC_SYNCx                     IS_PKT_SYNCx

/**
 * @brief  SPIRIT Basic Packet Init structure definition. This structure allows users to set the main options
 *         for the Basic packet.
 */
typedef struct
{

  BasicPreambleLength           xPreambleLength;        /*!< Specifies the preamble length.
                                                             This parameter can be any value of @ref BasicPreambleLength */
  BasicSyncLength               xSyncLength;            /*!< Specifies the sync word length. The 32bit word passed (lSyncWords) will be stored in the SYNCx registers from the MSb
                                                             until the number of bytes in xSyncLength has been stored.
                                                             This parameter can be any value of @ref BasicSyncLength */
  uint32_t                      lSyncWords;             /*!< Specifies the sync words.
                                                             This parameter is a uint32_t word with format: 0x|SYNC1|SYNC2|SYNC3|SYNC4| */
  BasicFixVarLength             xFixVarLength;          /*!< Specifies if a fixed length of packet has to be used.
                                                             This parameter can be any value of @ref BasicFixVarLength */
  uint8_t                       cPktLengthWidth;        /*!< Specifies the size of the length of packet in bits. This field is useful only if
                                                             the field xFixVarLength is set to BASIC_LENGTH_VAR. For Basic packets the length width
                                                             is log2( max payload length + control length (0 to 4) + address length (0 or 1)).
                                                             This parameter is an uint8_t */
  BasicCrcMode                  xCrcMode;               /*!< Specifies the CRC word length of packet.
                                                             This parameter can be any value of @ref BasicCrcMode */
  BasicControlLength            xControlLength;         /*!< Specifies the length of a control field to be sent.
                                                             This parameter can be any value of @ref BasicControlLength */
  SpiritFunctionalState         xAddressField;          /*!< Specifies if the destination address has to be sent.
                                                             This parameter can be S_ENABLE or S_DISABLE */
  SpiritFunctionalState         xFec;                   /*!< Specifies if FEC has to be enabled.
                                                             This parameter can be S_ENABLE or S_DISABLE */
  SpiritFunctionalState         xDataWhitening;         /*!< Specifies if data whitening has to be enabled.
                                                             This parameter can be S_ENABLE or S_DISABLE */
}PktBasicInit;


/**
 * @brief  SPIRIT Basic Packet address structure definition. This structure allows users to specify
 *         the node/multicast/broadcast addresses and the correspondent filtering options.
 */
typedef struct
{

  SpiritFunctionalState         xFilterOnMyAddress;             /*!< If set RX packet is accepted if its destination address matches with cMyAddress.
                                                                     This parameter can be S_ENABLE or S_DISABLE */
  uint8_t                       cMyAddress;                     /*!< Specifies the TX packet source address (address of this node).
                                                                     This parameter is an uint8_t */
  SpiritFunctionalState         xFilterOnMulticastAddress;      /*!< If set RX packet is accepted if its destination address matches with cMulticastAddress.
                                                                     This parameter can be S_ENABLE or S_DISABLE */
  uint8_t                       cMulticastAddress;              /*!< Specifies the Multicast group address for this node.
                                                                     This parameter is an uint8_t */
  SpiritFunctionalState         xFilterOnBroadcastAddress;      /*!< If set RX packet is accepted if its destination address matches with cBroadcastAddress.
                                                                     This parameter can be S_ENABLE or S_DISABLE */
  uint8_t                       cBroadcastAddress;              /*!< Specifies the Broadcast address for this node.
                                                                     This parameter is an uint8_t */
}PktBasicAddressesInit;

/**
 *@}
 */

/* --------------------------------   PKT_BASIC  ----------------------------------- */  

/**
 * @defgroup PktBasic_Exported_Constants        Pkt Basic Exported Constants
 * @{
 */

#define IS_BASIC_LENGTH_WIDTH_BITS                      IS_PKT_LENGTH_WIDTH_BITS

/**
 *@}
 */

/**
 * @defgroup PktBasic_Exported_Macros   Pkt Basic Exported Macros
 * @{
 */

/**
 * @brief  Macro used to compute per lower part of the packet length
 *         for Spirit Basic packets, to write in the PCKTLEN0 register.
 * @param  nLength Length of the packet payload.
 *         This parameter is an uint16_t.
 * @retval None.
 */
#define BASIC_BUILD_PCKTLEN0(nLength)                                            BUILD_PCKTLEN0(nLength)

/**
 * @brief  Macro used to compute per upper part of the packet length
 *         for Spirit Basic packets, to write the PCKTLEN1 register.
 * @param  nLengthLength of the packet payload.
 *         This parameter is an uint16_t.
 * @retval None.
 */
#define BASIC_BUILD_PCKTLEN1(nLength)                                            BUILD_PCKTLEN1(nLength)

/**
 * @brief  Sets the CONTROL field length for SPIRIT Basic packets.
 * @param  xControlLength length of CONTROL field in bytes.
 *         This parameter can be any value of @ref PktControlLength.
 * @retval None.
 */
#define SpiritPktBasicSetControlLength(xControlLength)                  SpiritPktCommonSetControlLength(xControlLength)

/**
 * @brief  Returns the CONTROL field length for SPIRIT Basic packets.
 * @param  None.
 * @retval uint8_t Control field length.
 */
#define SpiritPktBasicGetControlLength()                                SpiritPktCommonGetControlLength()

/**
 * @brief  Sets the PREAMBLE field length for SPIRIT Basic packets.
 * @param  xPreambleLength length of PREAMBLE field in bytes.
 *         This parameter can be any value of @ref BasicPreambleLength.
 * @retval None.
 */
#define SpiritPktBasicSetPreambleLength(xPreambleLength)                SpiritPktCommonSetPreambleLength((PktPreambleLength)xPreambleLength)

/**
 * @brief  Returns the PREAMBLE field length mode for SPIRIT Basic packets.
 * @param  None.
 * @retval uint8_t Preamble field length in bytes.
 */
#define SpiritPktBasicGetPreambleLength()                               SpiritPktCommonGetPreambleLength()

/**
 * @brief  Sets the SYNC field length for SPIRIT Basic packets.
 * @param  xSyncLength length of SYNC field in bytes.
 *         This parameter can be any value of @ref BasicSyncLength.
 * @retval None.
 */
#define SpiritPktBasicSetSyncLength(xSyncLength)                         SpiritPktCommonSetSyncLength((PktSyncLength)xSyncLength)

/**
 * @brief  Returns the SYNC field length for SPIRIT Basic packets.
 * @param  None.
 * @retval uint8_t SYNC field length in bytes.
 */
#define SpiritPktBasicGetSyncLength()                                   SpiritPktCommonGetSyncLength()

/**
 * @brief  Sets fixed or variable payload length mode for SPIRIT packets.
 * @param  xFixVarLength variable or fixed length.
 *         BASIC_FIXED_LENGTH_VAR -> variable (the length is extracted from the received packet).
 *         BASIC_FIXED_LENGTH_FIX -> fix (the length is set by PCKTLEN0 and PCKTLEN1).
 * @retval None.
 */
#define SpiritPktBasicSetFixVarLength(xFixVarLength)                    SpiritPktCommonSetFixVarLength((PktFixVarLength)xFixVarLength)

/**
 * @brief  Enables or Disables the CRC filtering.
 * @param  xNewState new state for CRC_CHECK.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define SpiritPktBasicFilterOnCrc(xNewState)                            SpiritPktCommonFilterOnCrc(xNewState)

/**
 * @brief  Returns the CRC filtering bit.
 * @param  None.
 * @retval SpiritFunctionalState This parameter can be S_ENABLE or S_DISABLE.
 */
#define SpiritPktBasicGetFilterOnCrc()                                  SpiritPktCommonGetFilterOnCrc()

/**
 * @brief  Sets the CRC mode for SPIRIT Basic packets.
 * @param  xCrcMode CRC mode.
 *         This parameter can be any value of @ref BasicCrcMode.
 * @retval None.
 */
#define SpiritPktBasicSetCrcMode(xCrcMode)                              SpiritPktCommonSetCrcMode((PktCrcMode)xCrcMode)

/**
 * @brief  Returns the CRC mode for SPIRIT Basic packets.
 * @param  None.
 * @retval BasicCrcMode Crc mode.
 */
#define SpiritPktBasicGetCrcMode()                                      (BasicCrcMode)SpiritPktCommonGetCrcMode()

/**
 * @brief  Enables or Disables WHITENING for SPIRIT packets.
 * @param  xNewState new state for WHITENING mode.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define SpiritPktBasicWhitening(xNewState)                               SpiritPktCommonWhitening(xNewState)

/**
 * @brief  Enables or Disables FEC for SPIRIT Basic packets.
 * @param  xNewState new state for FEC mode.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define SpiritPktBasicFec(xNewState)                                     SpiritPktCommonFec(xNewState)

/**
 * @brief  Sets a specific SYNC word for SPIRIT Basic packets.
 * @param  xSyncX SYNC word number to be set.
 *         This parameter can be any value of @ref BasicSyncX.
 * @param  cSyncWord SYNC word.
 *         This parameter is an uint8_t.
 * @retval None.
 */
#define SpiritPktBasicSetSyncxWord(xSyncX, cSyncWord)                   SpiritPktCommonSetSyncxWord((PktSyncX)xSyncX, cSyncWord)

/**
 * @brief  Returns a specific SYNC words for SPIRIT Basic packets.
 * @param  xSyncX SYNC word number to be get.
 *         This parameter can be any value of @ref BasicSyncX.
 * @retval uint8_t Sync word x.
 */
#define SpiritPktBasicGetSyncxWord(xSyncX)                              SpiritPktCommonGetSyncxWord(xSyncX)

/**
 * @brief  Sets multiple SYNC words for SPIRIT Basic packets.
 * @param  lSyncWords SYNC words to be set with format: 0x|SYNC1|SYNC2|SYNC3|SYNC4|.
 *         This parameter is a uint32_t.
 * @param  xSyncLength SYNC length in bytes. The 32bit word passed will be stored in the SYNCx registers from the MSb
 *         until the number of bytes in xSyncLength has been stored.
 *         This parameter is a @ref BasicSyncLength.
 * @retval None.
 */
#define SpiritPktBasicSetSyncWords(lSyncWords, xSyncLength)              SpiritPktCommonSetSyncWords(lSyncWords, (PktSyncLength)xSyncLength)

/**
 * @brief  Returns multiple SYNC words for SPIRIT Basic packets.
 * @param  xSyncLength SYNC length in bytes. The 32bit word passed will be stored in the SYNCx registers from the MSb
 *         until the number of bytes in xSyncLength has been stored.
 *         This parameter is a pointer to @ref BasicSyncLength.
 * @retval uint32_t Sync words. The format of the read 32 bit word is 0x|SYNC1|SYNC2|SYNC3|SYNC4|.
 */
#define SpiritPktBasicGetSyncWords(xSyncLength)                         SpiritPktCommonGetSyncWords((PktSyncLength)xSyncLength)

/**
 * @brief  Returns the SPIRIT variable length width (in number of bits).
 * @param  None.
 * @retval Variable length width in bits.
 */
#define SpiritPktBasicGetVarLengthWidth()                               SpiritPktCommonGetVarLengthWidth()

/**
 * @brief  Sets the destination address for the Tx packet.
 * @param  cAddress destination address.
 *         This parameter is an uint8_t.
 * @retval None.
 */
#define SpiritPktBasicSetDestinationAddress(cAddress)                   SpiritPktCommonSetDestinationAddress(cAddress)

/**
 * @brief  Returns the settled destination address.
 * @param  None.
 * @retval uint8_t Transmitted destination address.
 */
#define SpiritPktBasicGetTransmittedDestAddress()                       SpiritPktCommonGetTransmittedDestAddress()

/**
 * @brief  Sets the node address. When the filtering on my address is on, if the destination address extracted from the received packet is equal to the content of the
 *         my address, then the packet is accepted (this is the address of the node).
 * @param  cAddress Address of the present node.
 *         This parameter is an uint8_t.
 * @retval None.
 */
#define SpiritPktBasicSetMyAddress(cAddress)                            SpiritPktCommonSetMyAddress(cAddress)

/**
 * @brief  Returns the address of the present node.
 * @param  None.
 * @retval uint8_t My address (address of this node).
 */
#define SpiritPktBasicGetMyAddress()                                    SpiritPktCommonGetMyAddress()

/**
 * @brief  Sets the broadcast address. When the broadcast filtering is on, if the destination address extracted from the received packet is equal to the content of the
 *         BROADCAST_ADDR register, then the packet is accepted.
 * @param  cAddress Broadcast address.
 *         This parameter is an uint8_t.
 * @retval None.
 */
#define SpiritPktBasicSetBroadcastAddress(cAddress)                     SpiritPktCommonSetBroadcastAddress(cAddress)

/**
 * @brief  Returns the broadcast address.
 * @param  None.
 * @retval uint8_t Broadcast address.
 */
#define SpiritPktBasicGetBroadcastAddress()                             SpiritPktCommonGetBroadcastAddress()

/**
 * @brief  Sets the multicast address. When the multicast filtering is on, if the destination address extracted from the received packet is equal to the content of the
 *         MULTICAST_ADDR register, then the packet is accepted.
 * @param  cAddress Multicast address.
 *         This parameter is an uint8_t.
 * @retval None.
 */
#define SpiritPktBasicSetMulticastAddress(cAddress)                     SpiritPktCommonSetMulticastAddress(cAddress)

/**
 * @brief  Returns the multicast address.
 * @param  None.
 * @retval uint8_t Multicast address.
 */
#define SpiritPktBasicGetMulticastAddress()                             SpiritPktCommonGetMulticastAddress()

/**
 * @brief  Sets the control mask. The 1 bits of the CONTROL_MASK indicate the
 *         bits to be used in filtering. (All 0s no filtering)
 * @param  lMask Control mask.
 *         This parameter is an uint32_t.
 * @retval None.
 */
#define SpiritPktBasicSetCtrlMask(lMask)                                SpiritPktCommonSetCtrlMask(lMask)

/**
 * @brief  Returns the control mask. The 1 bits of the CONTROL_MASK indicate the
 *         bits to be used in filtering. (All 0s no filtering)
 * @param  None.
 * @retval uint32_t Control mask.
 */
#define SpiritPktBasicGetCtrlMask()                                     SpiritPktCommonGetCtrlMask()

/**
 * @brief  Sets the control field reference. If the bits enabled by the
 *         CONTROL_MASK match the ones of the control fields extracted from the received packet
 *         then the packet is accepted.
 * @param  lReference Control reference.
 *         This parameter is an uint32_t.
 * @retval None.
 */
#define SpiritPktBasicSetCtrlReference(lReference)                      SpiritPktCommonSetCtrlReference(lReference)

/**
 * @brief  Returns the control field reference.
 * @param  None.
 * @retval uint32_t Control reference.
 */
#define SpiritPktBasicGetCtrlReference()                                SpiritPktCommonGetCtrlReference()

/**
 * @brief  Sets the TX control field.
 * @param  lField Tx control field.
 *         This parameter is an uint32_t.
 * @retval None.
 */
#define SpiritPktBasicSetTransmittedCtrlField(lField)                   SpiritPktCommonSetTransmittedCtrlField(lField)

/**
 * @brief  Returns the TX control field.
 * @param  None.
 * @retval uint32_t Control field of the transmitted packet.
 */
#define SpiritPktBasicGetTransmittedCtrlField()                         SpiritPktCommonGetTransmittedCtrlField()

/**
 * @brief  If enabled RX packet is accepted if its destination address matches with My address.
 * @param  xNewState new state for DEST_VS_SOURCE_ADDRESS.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define SpiritPktBasicFilterOnMyAddress(xNewState)                      SpiritPktCommonFilterOnMyAddress(xNewState)

/**
 * @brief  If enabled RX packet is accepted if its destination address matches with multicast address.
 * @param  xNewState new state for DEST_VS_MULTICAST_ADDRESS.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define SpiritPktBasicFilterOnMulticastAddress(xNewState)               SpiritPktCommonFilterOnMulticastAddress(xNewState)

/**
 * @brief  If enabled RX packet is accepted if its destination address matches with broadcast address.
 * @param  xNewState new state for DEST_VS_BROADCAST_ADDRESS.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define SpiritPktBasicFilterOnBroadcastAddress(xNewState)               SpiritPktCommonFilterOnBroadcastAddress(xNewState)

/**
 * @brief  Returns the enable bit of the my address filtering.
 * @param  None.
 * @retval SpiritFunctionalState This parameter can be S_ENABLE or S_DISABLE.
 */
#define SpiritPktBasicGetFilterOnMyAddress()                            SpiritPktCommonGetFilterOnMyAddress();

/**
 * @brief  Returns the enable bit of the multicast address filtering.
 * @param  None.
 * @retval SpiritFunctionalState This parameter can be S_ENABLE or S_DISABLE.
 */
#define SpiritPktBasicGetFilterOnMulticastAddress()                     SpiritPktCommonGetFilterOnMulticastAddress();

/**
 * @brief  Returns the enable bit of the broadcast address filtering.
 * @param  None.
 * @retval SpiritFunctionalState This parameter can be S_ENABLE or S_DISABLE.
 */
#define SpiritPktBasicGetFilterOnBroadcastAddress()                     SpiritPktCommonGetFilterOnBroadcastAddress();

/**
 * @brief  Returns the destination address of the received packet.
 * @param  None.
 * @retval uint8_t Destination address of the received packet.
 */
#define SpiritPktBasicGetReceivedDestAddress()                          SpiritPktCommonGetReceivedDestAddress()

/**
 * @brief  Returns the control field of the received packet.
 * @param  None.
 * @retval uint32_t Received control field.
 */
#define SpiritPktBasicGetReceivedCtrlField()                            SpiritPktCommonGetReceivedCtrlField()

/**
 * @brief  Returns the CRC field of the received packet.
 * @param  cCrcFieldVect array in which the CRC field has to be stored.
 *         This parameter is an uint8_t array of 3 elements.
 * @retval None.
 */
#define SpiritPktBasicGetReceivedCrcField(cCrcFieldVect)                SpiritPktCommonGetReceivedCrcField(cCrcFieldVect)

/**
 * @brief  If enabled RX packet is accepted only if the masked control field matches the
 *         masked control field reference (CONTROL_MASK & CONTROL_FIELD_REF == CONTROL_MASK & RX_CONTROL_FIELD).
 * @param  xNewState new state for Control filtering enable bit.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 * @note   This filtering control is enabled by default but the control mask is by default set to 0.
 *         As a matter of fact the user has to enable the control filtering bit after the packet initialization
 *         because the PktInit routine disables it.
 */
#define SpiritPktBasicFilterOnControlField(xNewState)                           SpiritPktCommonFilterOnControlField(xNewState)

/**
 * @brief  Returns the enable bit of the control field filtering.
 * @param  None.
 * @retval SpiritFunctionalState This parameter can be S_ENABLE or S_DISABLE.
 */
#define SpiritPktBasicGetFilterOnControlField()                                 SpiritPktCommonGetFilterOnControlField();

/**
 *@}
 */

 /* --------------------------------   PKT_MBUS  ----------------------------------- */  
 
/**
 * @defgroup PktMbus_Exported_Types     Pkt MBUS Exported Types
 * @{
 */

/**
 * @brief  MBUS submode enumeration.
 */

typedef enum
{
  MBUS_SUBMODE_S1_S2_LONG_HEADER            = MBUS_CTRL_MBUS_SUBMODE_S1_S2L,                /*!< MBUS submode S1, S2 (long header) - Header length = mbus_prmbl_ctrl + 279 (in "01" bit pairs) , Sync word = 0x7696 (length 18 bits) */
  MBUS_SUBMODE_S1_M_S2_T2_OTHER_TO_METER    = MBUS_CTRL_MBUS_SUBMODE_S2_S1M_T2_OTHER,       /*!< MBUS submode S1-m, S2, T2 (other to meter) - Header length = mbus_prmbl_ctrl + 15 (in "01" bit pairs) , Sync word = 0x7696 (length 18 bits)*/
  MBUS_SUBMODE_T1_T2_METER_TO_OTHER         = MBUS_CTRL_MBUS_SUBMODE_T1_T2_METER,           /*!< MBUS submode T1, T2 (meter to other) - Header length = mbus_prmbl_ctrl + 19 (in "01" bit pairs) ,  Sync word = 0x3D (length 10 bits)*/
  MBUS_SUBMODE_R2_SHORT_HEADER              = MBUS_CTRL_MBUS_SUBMODE_R2,                    /*!< MBUS submode R2, short header - Header length = mbus_prmbl_ctrl + 39 (in "01" bit pairs) , Sync word = 0x7696 (length 18 bits)*/

}MbusSubmode;

#define IS_MBUS_SUBMODE(MODE)   (((MODE) == MBUS_SUBMODE_S1_S2_LONG_HEADER) || \
                                 ((MODE) == MBUS_SUBMODE_S1_M_S2_T2_OTHER_TO_METER) || \
                                 ((MODE) == MBUS_SUBMODE_T1_T2_METER_TO_OTHER) || \
                                 ((MODE) == MBUS_SUBMODE_R2_SHORT_HEADER))

/**
 * @brief  SPIRIT MBUS Packet Init structure definition
 */
typedef struct
{
  MbusSubmode       xMbusSubmode;              /*!< Specifies the SUBMODE to be configured.
                                                    This parameter can be a value of @ref MbusSubmode */

  uint8_t           cPreambleLength;           /*!< Specifies the PREAMBLE length.
                                                    This parameter can be any value between 0 and 255 chip sequence '01' */

  uint8_t           cPostambleLength;          /*!< Specifies the POSTAMBLE length.
                                                    This parameter can be any value between 0 and 255 chip sequence '01' */
}PktMbusInit;

/**
 *@}
 */

 /* --------------------------------   PKT_STACK  ----------------------------------- */  

/**
 * @defgroup PktStack_Exported_Types    Pkt STack Exported Types
 * @{
 */

/**
 * @brief  Preamble length in bytes enumeration.
 */
typedef PktPreambleLength                  StackPreambleLength;
#define IS_STACK_PREAMBLE_LENGTH           IS_PKT_PREAMBLE_LENGTH

/**
 * @brief  Sync length in bytes enumeration.
 */
typedef PktSyncLength                      StackSyncLength;
#define IS_STACK_SYNC_LENGTH               IS_PKT_SYNC_LENGTH

/**
 * @brief  CRC length in bytes enumeration.
 */
typedef PktCrcMode                         StackCrcMode;
#define IS_STACK_CRC_MODE                  IS_PKT_CRC_MODE

/**
 * @brief  Fixed or variable payload length enumeration.
 */
typedef PktFixVarLength                    StackFixVarLength;
#define IS_STACK_FIX_VAR_LENGTH            IS_PKT_FIX_VAR_LENGTH

/**
 * @brief  Control length in bytes enumeration for SPIRIT.
 */
typedef PktControlLength                   StackControlLength;
#define IS_STACK_CONTROL_LENGTH            IS_PKT_CONTROL_LENGTH

/**
 * @brief  Sync words enumeration for SPIRIT.
 */
typedef PktSyncX                           StackSyncX;
#define IS_STACK_SYNCx                     IS_PKT_SYNCx

/**
 * @brief  Max retransmission number enumeration for SPIRIT.
 */
typedef PktNMaxReTx                        StackNMaxReTx;
#define IS_STACK_NMAX_RETX                 IS_PKT_NMAX_RETX

/**
 * @brief  SPIRIT STack Packet Init structure definition. This structure allows users to set the main options
 *         for the STack packet.
 */
typedef struct
{

  StackPreambleLength           xPreambleLength;         /*!< Specifies the preamble length of packet.
                                                              This parameter can be any value of @ref StackPreambleLength */
  StackSyncLength               xSyncLength;             /*!< Specifies the sync word length of packet.
                                                              This parameter can be any value of @ref StackSyncLength */
  uint32_t                      lSyncWords;             /*!< Specifies the sync words.
                                                              This parameter is a uint32_t word with format: 0x|SYNC1|SYNC2|SYNC3|SYNC4| */
  StackFixVarLength             xFixVarLength;           /*!< Specifies if a fixed length of packet has to be used.
                                                              This parameter can be any value of @ref StackFixVarLength */
  uint8_t                       cPktLengthWidth;         /*!< Specifies the size of the length of packet in bits. This field is useful only if
                                                              the field xFixVarLength is set to STACK_LENGTH_VAR. For STack packets the length width
                                                               is log2( max payload length + control length (0 to 4) + address length (always 2)).
                                                              This parameter is an uint8_t */
  StackCrcMode                  xCrcMode;                /*!< Specifies the CRC word length of packet.
                                                              This parameter can be any value of @ref StackCrcMode */
  StackControlLength            xControlLength;          /*!< Specifies the length of a control field to be sent.
                                                              This parameter can be any value of @ref StackControlLength */
  SpiritFunctionalState         xFec;                    /*!< Specifies if FEC has to be enabled.
                                                              This parameter can be any value of @ref SpiritFunctionalState */
  SpiritFunctionalState         xDataWhitening;          /*!< Specifies if data whitening has to be enabled.
                                                              This parameter can be any value of @ref SpiritFunctionalState */
}PktStackInit;

/**
 * @brief  SPIRIT STack packet address structure definition. This structure allows users to specify
 *         the node/multicast/broadcast addresses and the correspondent filtering options.
 */
typedef struct
{
  SpiritFunctionalState         xFilterOnMyAddress;             /*!< If set RX packet is accepted if its destination address matches with cMyAddress.
                                                                     This parameter can be S_ENABLE or S_DISABLE */
  uint8_t                       cMyAddress;                     /*!< Specifies the TX packet source address (address of this node).
                                                                     This parameter is an uint8_t */
  SpiritFunctionalState         xFilterOnMulticastAddress;      /*!< If set RX packet is accepted if its destination address matches with cMulticastAddress.
                                                                     This parameter can be S_ENABLE or S_DISABLE */
  uint8_t                       cMulticastAddress;              /*!< Specifies the Multicast group address for this node.
                                                                     This parameter is an uint8_t */
  SpiritFunctionalState         xFilterOnBroadcastAddress;      /*!< If set RX packet is accepted if its destination address matches with cBroadcastAddress.
                                                                     This parameter can be S_ENABLE or S_DISABLE */
  uint8_t                       cBroadcastAddress;              /*!< Specifies the Broadcast address for this node.
                                                                     This parameter is an uint8_t */
}PktStackAddressesInit;

/**
 * @brief  SPIRIT STack packet LLP structure definition. This structure allows users to configure
 *         all the LLP options for STack packets.
 */
typedef struct
{
  SpiritFunctionalState         xAutoAck;                /*!< Specifies if the auto ACK feature is used or not.
                                                              This parameter can be a value of @ref SpiritFunctionalState */
  SpiritFunctionalState         xPiggybacking;           /*!< Specifies if the piggybacking feature is used or not.
                                                              This parameter can be a value of @ref SpiritFunctionalState */
  StackNMaxReTx                 xNMaxRetx;               /*!< Specifies the number of MAX-Retransmissions.
                                                              This parameter can be a value of @ref StackNMaxReTx */
}PktStackLlpInit;

/**
 *@}
 */

/**
 * @defgroup PktStack_Exported_Constants        Pkt STack Exported Constants
 * @{
 */
#define IS_STACK_LENGTH_WIDTH_BITS                      IS_PKT_LENGTH_WIDTH_BITS

/**
 *@}
 */

/**
 * @defgroup PktStack_Exported_Macros   Pkt STack Exported Macros
 * @{
 */

/**
 * @brief  Macro used to compute the lower part of the packet length
 *         for Spirit STack packets, to write in the PCKTLEN0 register.
 * @param  nLength length of the packet payload.
 *         This parameter is an uint16_t.
 * @retval None.
 */
#define STACK_BUILD_PCKTLEN0(nLength)                                    BUILD_PCKTLEN0(nLength)

/**
 * @brief  Macro used to compute the upper part of the packet length
 *         for Spirit STack packets, to write the PCKTLEN1 register.
 * @param  nLength length of the packet payload.
 *         This parameter is an uint16_t.
 * @retval None.
 */
#define STACK_BUILD_PCKTLEN1(nLength)                                    BUILD_PCKTLEN1(nLength)

/**
 * @brief  Sets the CONTROL length for SPIRIT STack packets.
 * @param  xControlLength length of CONTROL field in bytes.
 *         This parameter can be any value of @ref StackControlLength.
 * @retval None.
 */
#define SpiritPktStackSetControlLength(xControlLength)                          SpiritPktCommonSetControlLength(xControlLength)

/**
 * @brief  Returns the CONTROL length for SPIRIT STack packets.
 * @param  None.
 * @retval Control length.
 */
#define SpiritPktStackGetControlLength()                                       SpiritPktCommonGetControlLength()

/**
 * @brief  Sets the PREAMBLE Length mode for SPIRIT STack packets.
 * @param  xPreambleLength length of PREAMBLE field in bytes.
 *         This parameter can be any value of @ref StackPreambleLength.
 * @retval None.
 */
#define SpiritPktStackSetPreambleLength(xPreambleLength)                        SpiritPktCommonSetPreambleLength((PktPreambleLength)xPreambleLength)

/**
 * @brief  Returns the PREAMBLE Length mode for SPIRIT STack packets.
 * @param  None.
 * @retval uint8_t Preamble length in bytes.
 */
#define SpiritPktStackGetPreambleLength()                                      SpiritPktCommonGetPreambleLength()

/**
 * @brief  Sets the SYNC Length for SPIRIT STack packets.
 * @param  xSyncLength length of SYNC field in bytes.
 *         This parameter can be any value of @ref StackSyncLength.
 * @retval None.
 */
#define SpiritPktStackSetSyncLength(xSyncLength)                                SpiritPktCommonSetSyncLength((PktSyncLength)xSyncLength)

/**
 * @brief  Returns the SYNC Length for SPIRIT STack packets.
 * @param  None.
 * @retval uint8_t Sync length in bytes.
 */
#define SpiritPktStackGetSyncLength()                                           SpiritPktCommonGetSyncLength()

/**
 * @brief  Sets fixed or variable payload length mode for SPIRIT STack packets.
 * @param  xFixVarLength variable or fixed length.
 *         PKT_FIXED_LENGTH_VAR -> variable (the length is extracted from the received packet).
 *         PKT_FIXED_LENGTH_FIX -> fix (the length is set by PCKTLEN0 and PCKTLEN1).
 * @retval None.
 */
#define SpiritPktStackSetFixVarLength(xFixVarLength)                            SpiritPktCommonSetFixVarLength((PktFixVarLength)xFixVarLength)

/**
 * @brief  Enables or Disables the CRC filtering.
 * @param  xNewState new state for CRC_CHECK.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define SpiritPktStackFilterOnCrc(xNewState)                                    SpiritPktCommonFilterOnCrc(xNewState)

/**
 * @brief  Returns the CRC filtering bit.
 * @param  None.
 * @retval SpiritFunctionalState This parameter can be S_ENABLE or S_DISABLE.
 */
#define SpiritPktStackGetFilterOnCrc()                                          SpiritPktCommonGetFilterOnCrc()

/**
 * @brief  Sets the CRC mode for SPIRIT STack packets.
 * @param  xCrcMode CRC mode.
 *         This parameter can be any value of @ref StackCrcMode.
 * @retval None.
 */
#define SpiritPktStackSetCrcMode(xCrcMode)                                      SpiritPktCommonSetCrcMode((PktCrcMode)xCrcMode)

/**
 * @brief  Returns the CRC mode for SPIRIT packets.
 * @param  None.
 * @retval StackCrcMode Crc mode.
 */
#define SpiritPktStackGetCrcMode()                                             (StackCrcMode)SpiritPktCommonGetCrcMode()

/**
 * @brief  Enables or Disables WHITENING for SPIRIT STack packets.
 * @param  xNewState new state for WHITENING mode.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define SpiritPktStackWhitening(xNewState)                                     SpiritPktCommonWhitening(xNewState)

/**
 * @brief  Enables or Disables FEC for SPIRIT STack packets.
 * @param  xNewState new state for FEC mode.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define SpiritPktStackFec(xNewState)                                            SpiritPktCommonFec(xNewState)

/**
 * @brief  Sets a specific SYNC word for SPIRIT STack packets.
 * @param  xSyncX SYNC word number to be set.
 *         This parameter can be any value of @ref StackSyncX.
 * @param  cSyncWord SYNC word.
 *         This parameter is an uint8_t.
 * @retval None.
 */
#define SpiritPktStackSetSyncxWord(xSyncX, cSyncWord)                          SpiritPktCommonSetSyncxWord((PktSyncX)xSyncX,cSyncWord)

/**
 * @brief  Returns a specific SYNC word for SPIRIT STack packets.
 * @param  xSyncX SYNC word number to be get.
 *         This parameter can be any value of @ref StackSyncX.
 * @retval uint8_t Sync word x.
 */
#define SpiritPktStackGetSyncxWord(xSyncX)                                     SpiritPktCommonGetSyncxWord(xSyncX)

/**
 * @brief  Sets multiple SYNC words for SPIRIT STack packets.
 * @param  lSyncWords SYNC words to be set with format: 0x|SYNC1|SYNC2|SYNC3|SYNC4|.
 *         This parameter is a uint32_t.
 * @param  xSyncLength SYNC length in bytes. The 32bit word passed will be stored in the SYNCx registers from the MSb
 *         until the number of bytes in xSyncLength has been stored.
 *         This parameter is a @ref StackSyncLength.
 * @retval None.
 */
#define SpiritPktStackSetSyncWords(lSyncWords, xSyncLength)                    SpiritPktCommonSetSyncWords(lSyncWords,(PktSyncLength)xSyncLength)

/**
 * @brief  Returns multiple SYNC words for SPIRIT packets.
 * @param  xSyncLength SYNC length in bytes. The 32bit word passed will be stored in the SYNCx registers from the MSb
 *         until the number of bytes in xSyncLength has been stored.
 *         This parameter is a pointer to @ref StackSyncLength.
 * @retval uint32_t Sync words. The format of the read 32 bit word is 0x|SYNC1|SYNC2|SYNC3|SYNC4|.
 */
#define SpiritPktStackGetSyncWords(xSyncLength)                                 SpiritPktCommonGetSyncWords((PktSyncLength)xSyncLength)

/**
 * @brief  Returns the SPIRIT variable length width (in number of bits).
 * @param  None.
 * @retval uint8_t Variable length width in bits.
 */
#define SpiritPktStackGetVarLengthWidth()                                       SpiritPktCommonGetVarLengthWidth()

/**
 * @brief  Sets the destination address for the Tx packet.
 * @param  cAddress destination address.
 *         This parameter is an uint8_t.
 * @retval None.
 */
#define SpiritPktStackSetDestinationAddress(cAddress)                           SpiritPktCommonSetDestinationAddress(cAddress)

/**
 * @brief  Sets the Rx packet reference source address. The source address extracted from the received packet is masked
 *         with the source reference mask and then compared to the masked reference value.
 * @param  cAddress Reference source address.
 *         This parameter is an uint8_t.
 * @retval None.
 */
#define SpiritPktStackSetSourceReferenceAddress(cAddress)                       SpiritPktCommonSetDestinationAddress(cAddress)

/**
 * @brief  Returns the Rx packet reference source address. The source address extracted from the received packet is masked
 *         with the source reference mask and then compared to the masked reference value.
 * @param  cAddress Reference source address.
 *         This parameter is an uint8_t.
 * @retval None.
 */
#define SpiritPktStackGetSourceReferenceAddress()                               SpiritPktCommonGetTransmittedDestAddress()

/**
 * @brief  Returns the settled destination address.
 * @param  None.
 * @retval uint8_t Transmitted destination address.
 */
#define SpiritPktStackGetTransmittedDestAddress()                               SpiritPktCommonGetTransmittedDestAddress()

/**
 * @brief  Sets the node address. When the filtering on my address is on, if the destination address extracted from the received packet is equal to the content of the
 *         my address, then the packet is accepted (this is the address of the node).
 * @param  cAddress Address of the present node.
 *         This parameter is an uint8_t.
 * @retval None.
 */
#define SpiritPktStackSetMyAddress(cAddress)                                    SpiritPktCommonSetMyAddress(cAddress)

/**
 * @brief  Returns the address of the present node.
 * @param  None.
 * @retval uint8_t My address (address of this node).
 */
#define SpiritPktStackGetMyAddress()                                            SpiritPktCommonGetMyAddress()

/**
 * @brief  Sets the broadcast address. When the broadcast filtering is on, if the destination address extracted from the received packet is equal to the content of the
 *         BROADCAST_ADDR register, then the packet is accepted.
 * @param  cAddress Broadcast address.
 *         This parameter is an uint8_t.
 * @retval None.
 */
#define SpiritPktStackSetBroadcastAddress(cAddress)                             SpiritPktCommonSetBroadcastAddress(cAddress)

/**
 * @brief  Returns the broadcast address.
 * @param  None.
 * @retval uint8_t Broadcast address.
 */
#define SpiritPktStackGetBroadcastAddress()                                     SpiritPktCommonGetBroadcastAddress()

/**
 * @brief  Sets the multicast address. When the multicast filtering is on, if the destination address extracted from the received packet is equal to the content of the
 *         MULTICAST_ADDR register, then the packet is accepted.
 * @param  cAddress Multicast address.
 *         This parameter is an uint8_t.
 * @retval None.
 */
#define SpiritPktStackSetMulticastAddress(cAddress)                             SpiritPktCommonSetMulticastAddress(cAddress)

/**
 * @brief  Returns the multicast address.
 * @param  None.
 * @retval uint8_t Multicast address.
 */
#define SpiritPktStackGetMulticastAddress()                                     SpiritPktCommonGetMulticastAddress()

/**
 * @brief  Sets the control mask. The 1 bits of the CONTROL_MASK indicate the
 *         bits to be used in filtering. (All 0s no filtering)
 * @param  lMask Control mask.
 *         This parameter is an uint32_t.
 * @retval None.
 */
#define SpiritPktStackSetCtrlMask(lMask)                                        SpiritPktCommonSetCtrlMask(lMask)

/**
 * @brief  Returns the control mask. The 1 bits of the CONTROL_MASK indicate the
 *         bits to be used in filtering. (All 0s no filtering)
 * @param  None.
 * @retval uint32_t Control mask.
 */
#define SpiritPktStackGetCtrlMask()                                             SpiritPktCommonGetCtrlMask()

/**
 * @brief  Sets the control field reference. If the bits enabled by the
 *         CONTROL_MASK match the ones of the control fields extracted from the received packet
 *         then the packet is accepted.
 * @param  lReference Control reference.
 *         This parameter is an uint32_t.
 * @retval None.
 */
#define SpiritPktStackSetCtrlReference(lReference)                              SpiritPktCommonSetCtrlReference(lReference)

/**
 * @brief  Returns the control field reference.
 * @param  None.
 * @retval uint32_t Control reference.
 */
#define SpiritPktStackGetCtrlReference()                                        SpiritPktCommonGetCtrlReference()

/**
 * @brief  Sets the TX control field.
 * @param  lField TX CONTROL FIELD.
 *         This parameter is an uint32_t.
 * @retval None.
 */
#define SpiritPktStackSetTransmittedCtrlField(lField)                           SpiritPktCommonSetTransmittedCtrlField(lField)

/**
 * @brief  Returns the TX control field.
 * @param  None.
 * @retval uint32_t Control field of the transmitted packet.
 */
#define SpiritPktStackGetTransmittedCtrlField()                                 SpiritPktCommonGetTransmittedCtrlField()

/**
 * @brief  If enabled RX packet is accepted if its destination address matches with TX_SOURCE_ADDRESS.
 * @param  xNewState new state for DEST_VS_SOURCE_ADDRESS.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define SpiritPktStackFilterOnMyAddress(xNewState)                               SpiritPktCommonFilterOnMyAddress(xNewState)

/**
 * @brief  If enabled RX packet is accepted if its destination address matches with MULTICAST_ADDRESS.
 * @param  xNewState new state for DEST_VS_MULTICAST_ADDRESS.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define SpiritPktStackFilterOnMulticastAddress(xNewState)                        SpiritPktCommonFilterOnMulticastAddress(xNewState)

/**
 * @brief  If enabled RX packet is accepted if its destination address matches with BROADCAST_ADDRESS.
 * @param  xNewState new state for DEST_VS_BROADCAST_ADDRESS.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define SpiritPktStackFilterOnBroadcastAddress(xNewState)                        SpiritPktCommonFilterOnBroadcastAddress(xNewState)

/**
 * @brief  Returns the enable bit of the my address filtering.
 * @param  None.
 * @retval SpiritFunctionalStateThis parameter can be S_ENABLE or S_DISABLE.
 */
#define SpiritPktStackGetFilterOnMyAddress()                                    SpiritPktCommonGetFilterOnMyAddress();

/**
 * @brief  Returns the enable bit of the multicast address filtering.
 * @param  None.
 * @retval SpiritFunctionalState This parameter can be S_ENABLE or S_DISABLE.
 */
#define SpiritPktStackGetFilterOnMulticastAddress()                             SpiritPktCommonGetFilterOnMulticastAddress();

/**
 * @brief  Returns the enable bit of the broadcast address filtering.
 * @param  None.
 * @retval SpiritFunctionalState This parameter can be S_ENABLE or S_DISABLE.
 */
#define SpiritPktStackGetFilterOnBroadcastAddress()                             SpiritPktCommonGetFilterOnBroadcastAddress();

/**
 * @brief  Returns the control field of the received packet.
 * @param  None.
 * @retval uint32_t Received control field.
 */
#define SpiritPktStackGetReceivedCtrlField()                                     SpiritPktCommonGetReceivedCtrlField()

/**
 * @brief  Returns the CRC field of the received packet.
 * @param  cCrcFieldVect array in which the CRC field has to be stored.
 *         This parameter is an uint8_t array of 3 elements.
 * @retval None.
 */
#define SpiritPktStackGetReceivedCrcField(cCrcFieldVect)                         SpiritPktCommonGetReceivedCrcField(cCrcFieldVect)

/**
 * @brief  Sets the AUTO ACKNOLEDGEMENT mechanism on the receiver. When the feature is enabled and
 *         a data packet has been correctly received, then an acknowledgement packet is sent back to the originator of the received
 *         packet. If the PIGGYBACKING bit is also set, payload data will be read from the FIFO; otherwise an empty packet is sent
 *         only containing the source and destination addresses and the sequence number of the packet being acknowledged.
 * @param  xAutoAck new state for autoack.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @param  xPiggybacking new state for autoack.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define SpiritPktStackAutoAck(xAutoAck, xPiggybacking)                          SpiritPktCommonAutoAck(xAutoAck, xPiggybacking)

/**
 * @brief  Sets the AUTO ACKNOLEDGEMENT mechanism on the transmitter. On the transmitter side, the NACK_TX field can be used to require or not an acknowledgment for each individual packet: if
 *         NACK_TX is set to "1" then acknowledgment will not be required; if NACK_TX is set to "0" then acknowledgment will be
 *         required.
 * @param  xNewState new state for TX_AUTOACK.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define SpiritPktStackRequireAck(xNewState)                                     SpiritPktCommonRequireAck(xNewState)

/**
 * @brief  Sets the TX sequence number to be used to start counting.
 * @param  cSeqNumberReload new value for Tx seq number reload.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define SpiritPktStackSetTransmittedSeqNumberReload(cSeqNumberReload)           SpiritPktCommonSetTransmittedSeqNumberReload(cSeqNumberReload)

/**
 * @brief  Sets the max number of automatic retransmission.
 * @param  xNMaxReTx max number of retransmission.
 *         This parameter can be any value of @ref PktNMaxReTx.
 * @retval None.
 */
#define SpiritPktStackSetNMaxReTx(xNMaxReTx)                                    SpiritPktCommonSetNMaxReTx((PktNMaxReTx)xNMaxReTx)

/**
 * @brief  Returns the max number of automatic retransmission.
 * @param  None.
 * @retval uint8_t Max number of retransmissions.
 */
#define SpiritPktStackGetNMaxReTx()                                             SpiritPktCommonGetNMaxReTx()

/**
 * @brief  Returns the TX ACK request.
 * @param  None.
 * @retval SpiritFunctionalState.
 */
#define SpiritPktStackGetGetTxAckRequest()                                       SpiritPktCommonGetTxAckRequest()

/**
 * @brief  Returns the destination address of the received packet.
 * @param  None.
 * @retval uint8_t Destination address of the received packet.
 */
#define SpiritPktStackGetReceivedDestAddress()                                  SpiritPktCommonGetReceivedDestAddress()

/**
 * @brief  Returns the source address of the received packet.
 * @param  None.
 * @retval uint8_t Source address of the received packet.
 */
#define SpiritPktStackGetReceivedSourceAddress()                                SpiritPktCommonGetReceivedSourceAddress()

/**
 * @brief  Returns the sequence number of the received packet.
 * @param  None.
 * @retval uint8_t Received Sequence number.
 */
#define SpiritPktStackGetReceivedSeqNumber()                                    SpiritPktCommonGetReceivedSeqNumber()

/**
 * @brief  Returns the Nack bit of the received packet
 * @param  None.
 * @retval uint8_t Value of the NAck bit.
 */
#define SpiritPktStackGetReceivedNackRx()                                       SpiritPktCommonGetReceivedNackRx()

/**
 * @brief  Returns the sequence number of the transmitted packet.
 * @param  None.
 * @retval uint8_t Sequence number of the transmitted packet.
 */
#define SpiritPktStackGetTransmittedSeqNumber()                                 SpiritPktCommonGetTransmittedSeqNumber()

/**
 * @brief  Returns the number of retransmission done on the transmitted packet.
 * @param  None.
 * @retval uint8_t Number of retransmissions done until now.
 */
#define SpiritPktStackGetNReTx()                                                SpiritPktCommonGetNReTx()

/**
 * @brief  If enabled RX packet is accepted only if the masked control field matches the
 *         masked control field reference (CONTROL_MASK & CONTROL_FIELD_REF == CONTROL_MASK & RX_CONTROL_FIELD).
 * @param  xNewState new state for Control filtering enable bit.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 * @note   This filtering control is enabled by default but the control mask is by default set to 0.
 *         As a matter of fact the user has to enable the control filtering bit after the packet initialization
 *         because the PktInit routine disables it.
 */
#define SpiritPktStackFilterOnControlField(xNewState)                           SpiritPktCommonFilterOnControlField(xNewState)

/**
 * @brief  Returns the enable bit of the control field filtering.
 * @param  None.
 * @retval SpiritFunctionalState This parameter can be S_ENABLE or S_DISABLE.
 */
#define SpiritPktStackGetFilterOnControlField()                                 SpiritPktCommonGetFilterOnControlField();

/**
 *@}
 */
 
 /* --------------------------------   Qi  ----------------------------------- */  
 
/**
 * @defgroup Qi_Exported_Types  QI Exported Types
 * @{
 */

/**
 * @brief  PQI threshold value enumeration.
 */
typedef enum
{
   PQI_TH_0=0x00,
   PQI_TH_1=0x04,
   PQI_TH_2=0x08,
   PQI_TH_3=0x0C,
   PQI_TH_4=0x10,
   PQI_TH_5=0x14,
   PQI_TH_6=0x18,
   PQI_TH_7=0x1C,
   PQI_TH_8=0x20,
   PQI_TH_9=0x24,
   PQI_TH_10=0x28,
   PQI_TH_11=0x2C,
   PQI_TH_12=0x30,
   PQI_TH_13=0x34,
   PQI_TH_14=0x38,
   PQI_TH_15=0x3C

} PqiThreshold;

#define IS_PQI_THR(VALUE)   (VALUE==PQI_TH_0 ||\
                             VALUE==PQI_TH_1 ||\
                             VALUE==PQI_TH_2 ||\
                             VALUE==PQI_TH_3 ||\
                             VALUE==PQI_TH_4 ||\
                             VALUE==PQI_TH_5 ||\
                             VALUE==PQI_TH_6 ||\
                             VALUE==PQI_TH_7 ||\
                             VALUE==PQI_TH_8 ||\
                             VALUE==PQI_TH_9 ||\
                             VALUE==PQI_TH_10 ||\
                             VALUE==PQI_TH_11 ||\
                             VALUE==PQI_TH_12 ||\
                             VALUE==PQI_TH_13 ||\
                             VALUE==PQI_TH_14 ||\
                             VALUE==PQI_TH_15)

/**
 * @brief  SQI threshold value enumeration.
 */
typedef enum
{
   SQI_TH_0=0x00,
   SQI_TH_1=0x40,
   SQI_TH_2=0x80,
   SQI_TH_3=0xC0

} SqiThreshold;

#define IS_SQI_THR(VALUE)   (VALUE==SQI_TH_0 ||\
                             VALUE==SQI_TH_1 ||\
                             VALUE==SQI_TH_2 ||\
                             VALUE==SQI_TH_3)


/**
 * @brief  RSSI filter gain value enumeration.
 */
typedef enum
{
   RSSI_FG_0=0x00,
   RSSI_FG_1=0x10,
   RSSI_FG_2=0x20,
   RSSI_FG_3=0x30,
   RSSI_FG_4=0x40,
   RSSI_FG_5=0x50,
   RSSI_FG_6=0x60,
   RSSI_FG_7=0x70,
   RSSI_FG_8=0x80,
   RSSI_FG_9=0x90,
   RSSI_FG_10=0xA0,
   RSSI_FG_11=0xB0,
   RSSI_FG_12=0xC0,
   RSSI_FG_13=0xD0,
   RSSI_FG_14=0xE0,     /*<! recommended value */
   RSSI_FG_15=0xF0

} RssiFilterGain;

#define IS_RSSI_FILTER_GAIN(VALUE)  (VALUE==RSSI_FG_0 ||\
                                     VALUE==RSSI_FG_1 ||\
                                     VALUE==RSSI_FG_2 ||\
                                     VALUE==RSSI_FG_3 ||\
                                     VALUE==RSSI_FG_4 ||\
                                     VALUE==RSSI_FG_5 ||\
                                     VALUE==RSSI_FG_6 ||\
                                     VALUE==RSSI_FG_7 ||\
                                     VALUE==RSSI_FG_8 ||\
                                     VALUE==RSSI_FG_9 ||\
                                     VALUE==RSSI_FG_10 ||\
                                     VALUE==RSSI_FG_11 ||\
                                     VALUE==RSSI_FG_12 ||\
                                     VALUE==RSSI_FG_13 ||\
                                     VALUE==RSSI_FG_14 ||\
                                     VALUE==RSSI_FG_15)

/**
 * @brief  CS mode enumeration.
 */
typedef enum
{
   CS_MODE_STATIC_3DB=0x00,
   CS_MODE_DYNAMIC_6DB=0x04,
   CS_MODE_DYNAMIC_12DB=0x08,
   CS_MODE_DYNAMIC_18DB=0x0C

} CSMode;

#define IS_CS_MODE(MODE)    (MODE==CS_MODE_STATIC_3DB ||\
                             MODE==CS_MODE_DYNAMIC_6DB ||\
                             MODE==CS_MODE_DYNAMIC_12DB ||\
                             MODE==CS_MODE_DYNAMIC_18DB)

/**
  *@}
  */


/**
 * @defgroup Qi_Exported_Constants      QI Exported Constants
 * @{
 */

/*  range for the RSSI Threshold in dBm  */
#define IS_RSSI_THR_DBM(VALUE)  (VALUE>=-130 && VALUE<=-2)

/**
  *@}
  */


/**
 * @defgroup Qi_Exported_Macros         QI Exported Macros
 * @{
 */

/**
 * @brief  Macro to obtain the RSSI value in dBm
 * @param  None.
 * @retval RSSI in dBm.
 *         This parameter is a float.
 */
#define SpiritQiGetRssidBm()            (-120.0+((float)(SpiritQiGetRssi()-20))/2)

/**
 *@}
 */
 
/* --------------------------------   RADIO  ----------------------------------- */  

/** @defgroup Radio_Exported_Types      Radio Exported Types
 * @{
 */

/**
 * @brief  SPIRIT XTAL frequency enumeration
 */
typedef enum
{
  XTAL_FLAG_24_MHz     = 0x00, /*!< 24 MHz Xtal selected */
  XTAL_FLAG_26_MHz     = 0x01  /*!< 26 MHz Xtal selected */

}XtalFlag;

#define IS_XTAL_FLAG(FLAG) (((FLAG) == XTAL_FLAG_24_MHz) || \
                            ((FLAG) == XTAL_FLAG_26_MHz))

/**
 * @brief  SPIRIT Band enumeration
 */
typedef enum
{
  HIGH_BAND     = 0x00, /*!< High_Band selected: from 779 MHz to 915 MHz */
  MIDDLE_BAND   = 0x01, /*!< Middle Band selected: from 387 MHz to 470 MHz */
  LOW_BAND      = 0x02,  /*!< Low Band selected: from 300 MHz to 348 MHz */
  VERY_LOW_BAND = 0x03  /*!< Vary low Band selected: from 150 MHz to 174 MHz */
}BandSelect;


#define IS_BAND_SELECTED(BAND) ((BAND == HIGH_BAND) || \
                                (BAND == MIDDLE_BAND) || \
                                (BAND == LOW_BAND) || \
                                (BAND == VERY_LOW_BAND))

/**
 * @brief  SPIRIT Modulation enumeration
 */
typedef enum
{
  FSK         = 0x00, /*!< 2-FSK modulation selected */
  GFSK_BT05   = 0x50, /*!< GFSK modulation selected with BT=0.5 */
  GFSK_BT1    = 0x10, /*!< GFSK modulation selected with BT=1 */
  ASK_OOK     = 0x20, /*!< ASK or OOK modulation selected. ASK will use power ramping */
  MSK         = 0x30  /*!< MSK modulation selected */

}ModulationSelect;


#define IS_MODULATION_SELECTED(MOD) (((MOD) == FSK) || \
                                     ((MOD) == GFSK_BT05) || \
                                     ((MOD) == GFSK_BT1) || \
                                     ((MOD) == ASK_OOK) || \
                                     ((MOD) == MSK))

/**
 * @brief  SPIRIT PA additional load capacitors bank enumeration
 */
typedef enum
{
  LOAD_0_PF    = PA_POWER0_CWC_0,    /*!< No additional PA load capacitor */
  LOAD_1_2_PF  = PA_POWER0_CWC_1_2P, /*!< 1.2pF additional PA load capacitor */
  LOAD_2_4_PF  = PA_POWER0_CWC_2_4P, /*!< 2.4pF additional PA load capacitor */
  LOAD_3_6_PF  = PA_POWER0_CWC_3_6P  /*!< 3.6pF additional PA load capacitor */

}PALoadCapacitor;

#define IS_PA_LOAD_CAP(CWC) (((CWC) == LOAD_0_PF) || \
                             ((CWC) == LOAD_1_2_PF) || \
                             ((CWC) == LOAD_2_4_PF) || \
                             ((CWC) == LOAD_3_6_PF))

/**
 * @brief  SPIRIT AFC Mode selection
 */
typedef enum
{
  AFC_SLICER_CORRECTION  = AFC2_AFC_MODE_SLICER,    /*!< AFC loop closed on slicer */
  AFC_2ND_IF_CORRECTION  = AFC2_AFC_MODE_MIXER      /*!< AFC loop closed on 2nd conversion stage */

}AFCMode;

#define IS_AFC_MODE(MODE)   ((MODE) == AFC_SLICER_CORRECTION || (MODE) == AFC_2ND_IF_CORRECTION)

/**
 * @brief  SPIRIT AGC Mode selection
 */
typedef enum
{
  AGC_LINEAR_MODE  = AGCCTRL0_AGC_MODE_LINEAR,    /*!< AGC works in linear mode */
  AGC_BINARY_MODE  = AGCCTRL0_AGC_MODE_BINARY     /*!< AGC works in binary mode */

}AGCMode;

#define IS_AGC_MODE(MODE)   ((MODE) == AGC_LINEAR_MODE || (MODE) == AGC_BINARY_MODE)

/**
 * @brief  SPIRIT Clock Recovery Mode selection
 */
typedef enum
{
  CLK_REC_DLL  = FDEV0_CLOCK_REG_ALGO_SEL_DLL,    /*!< DLL alogrithm for clock recovery */
  CLK_REC_PLL  = FDEV0_CLOCK_REG_ALGO_SEL_PLL     /*!< PLL alogrithm for clock recovery */
}ClkRecMode;

#define IS_CLK_REC_MODE(MODE)   ((MODE) == CLK_REC_PLL || (MODE) == CLK_REC_DLL)

/**
 * @brief  SPIRIT Postfilter length
 */
typedef enum
{
  PSTFLT_LENGTH_8   = 0x00,    /*!< Postfilter length is 8 symbols */
  PSTFLT_LENGTH_16  = 0x10     /*!< Postfilter length is 16 symbols */

}PstFltLength;

#define IS_PST_FLT_LENGTH(LENGTH)   ((LENGTH) == PSTFLT_LENGTH_8 || (LENGTH) == PSTFLT_LENGTH_16)

/**
 * @brief  SPIRIT OOK Peak Decay
 */
typedef enum
{
  FAST_DECAY   = 0x00,        /*!< Peak decay control for OOK: fast decay */
  MEDIUM_FAST_DECAY  = 0x01,  /*!< Peak decay control for OOK: medium_fast decay */
  MEDIUM_SLOW_DECAY = 0x02,   /*!< Peak decay control for OOK: medium_fast decay */
  SLOW_DECAY = 0x03           /*!< Peak decay control for OOK: slow decay */

}OokPeakDecay;

#define IS_OOK_PEAK_DECAY(DECAY)   (((DECAY) == FAST_DECAY) ||\
                                    ((DECAY) == MEDIUM_FAST_DECAY) ||\
                                    ((DECAY) == MEDIUM_SLOW_DECAY) ||\
                                    ((DECAY) == SLOW_DECAY))

/**
 * @brief  SPIRIT Radio Init structure definition
 */
typedef struct
{
  int16_t           nXtalOffsetPpm;     /*!< Specifies the offset frequency (in ppm)
                                             to compensate crystal inaccuracy expressed
                                             as signed value.*/

  uint32_t          lFrequencyBase;     /*!< Specifies the base carrier frequency (in Hz),
                                             i.e. the carrier frequency of channel #0.
                                             This parameter can be in one of the following ranges:
                                             High_Band: from 779 MHz to 915 MHz
                                             Middle Band: from 387 MHz to 470 MHz
                                             Low Band: from 300 MHz to 348 MHz */
  uint32_t          nChannelSpace;      /*!< Specifies the channel spacing expressed in Hz.
                                             The channel spacing is expressed as:
                                             NxFREQUENCY_STEPS, where FREQUENCY STEPS
                                             is F_Xo/2^15.
                                             This parameter can be in the range: [0, F_Xo/2^15*255] Hz */
  uint8_t           cChannelNumber;      /*!< Specifies the channel number. This value
                                             is multiplied by the channel spacing and
                                             added to synthesizer base frequency to
                                             generate the actual RF carrier frequency */
  ModulationSelect  xModulationSelect;   /*!< Specifies the modulation. This
                                             parameter can be any value of
                                             @ref ModulationSelect */
  uint32_t          lDatarate;          /*!< Specifies the datarate expressed in bps.
                                             This parameter can be in the range between
                                             100 bps and 500 kbps */
  uint32_t          lFreqDev;           /*!< Specifies the frequency deviation expressed in Hz.
                                             This parameter can be in the range: [F_Xo*8/2^18, F_Xo*7680/2^18] Hz */
  uint32_t          lBandwidth;          /*!< Specifies the channel filter bandwidth
                                             expressed in Hz. This parameter can be
                                             in the range between 1100 and 800100 Hz */
}SRadioInit;

/**
 * @}
 */


/** @defgroup Radio_Exported_Constants       Radio Exported Constants
 * @{
 */

/** @defgroup Radio_Band
 * @{
 */

#define FBASE_DIVIDER           262144           /*!< 2^18 factor dividing fxo in fbase formula */

#define HIGH_BAND_FACTOR      6       /*!< Band select factor for high band. Factor B in the equation 2 */
#define MIDDLE_BAND_FACTOR    12      /*!< Band select factor for middle band. Factor B in the equation 2 */
#define LOW_BAND_FACTOR       16      /*!< Band select factor for low band. Factor B in the equation 2 */
#define VERY_LOW_BAND_FACTOR  32      /*!< Band select factor for very low band. Factor B in the equation 2 */

#define HIGH_BAND_LOWER_LIMIT         778000000   /*!< Lower limit of the high band: 779 MHz */
#define HIGH_BAND_UPPER_LIMIT         957100000   /*!< Upper limit of the high band: 956 MHz */
#define MIDDLE_BAND_LOWER_LIMIT       386000000   /*!< Lower limit of the middle band: 387 MHz */
#define MIDDLE_BAND_UPPER_LIMIT       471100000   /*!< Upper limit of the middle band: 470 MHz */
#define LOW_BAND_LOWER_LIMIT          299000000   /*!< Lower limit of the low band: 300 MHz */
#define LOW_BAND_UPPER_LIMIT          349100000   /*!< Upper limit of the low band: 348 MHz */
#define VERY_LOW_BAND_LOWER_LIMIT     149000000   /*!< Lower limit of the very low band: 150 MHz */
#define VERY_LOW_BAND_UPPER_LIMIT     175100000   /*!< Upper limit of the very low band: 174 MHz */

#define IS_FREQUENCY_BAND_HIGH(FREQUENCY) ((FREQUENCY)>=HIGH_BAND_LOWER_LIMIT && \
                                           (FREQUENCY)<=HIGH_BAND_UPPER_LIMIT)

#define IS_FREQUENCY_BAND_MIDDLE(FREQUENCY) ((FREQUENCY)>=MIDDLE_BAND_LOWER_LIMIT && \
                                             (FREQUENCY)<=MIDDLE_BAND_UPPER_LIMIT)

#define IS_FREQUENCY_BAND_LOW(FREQUENCY) ((FREQUENCY)>=LOW_BAND_LOWER_LIMIT && \
                                          (FREQUENCY)<=LOW_BAND_UPPER_LIMIT)

#define IS_FREQUENCY_BAND_VERY_LOW(FREQUENCY) ((FREQUENCY)>=VERY_LOW_BAND_LOWER_LIMIT && \
                                          (FREQUENCY)<=VERY_LOW_BAND_UPPER_LIMIT)

#define IS_FREQUENCY_BAND(FREQUENCY) (IS_FREQUENCY_BAND_HIGH(FREQUENCY)|| \
                                      IS_FREQUENCY_BAND_MIDDLE(FREQUENCY)|| \
                                      IS_FREQUENCY_BAND_LOW(FREQUENCY)|| \
                                      IS_FREQUENCY_BAND_VERY_LOW(FREQUENCY))

/**
 * @}
 */


/** @defgroup Radio_IF_Offset           Radio IF Offset
 * @{
 */
#define IF_OFFSET_ANA(F_Xo) (lroundf(480140.0f/(F_Xo)*12288-64.0f))      /*!< It represents the IF_OFFSET_ANA in order
                                                                               to have an intermediate frequency of 480 kHz */
                                                                               /* #1035-D */
/**
 * @}
 */


/** @defgroup Radio_FC_Offset                   Radio FC Offset
 * @{
 */
#define F_OFFSET_DIVIDER           262144             /*!< 2^18 factor dividing fxo in foffset formula */
#define PPM_FACTOR                 1000000            /*!< 10^6 factor to use with Xtal_offset_ppm */


#define F_OFFSET_LOWER_LIMIT(F_Xo)            ((-(int32_t)F_Xo)/F_OFFSET_DIVIDER*2048)
#define F_OFFSET_UPPER_LIMIT(F_Xo)            ((int32_t)(F_Xo/F_OFFSET_DIVIDER*2047))

#define IS_FREQUENCY_OFFSET(OFFSET, F_Xo) (OFFSET>=F_OFFSET_LOWER_LIMIT(F_Xo) && OFFSET<=F_OFFSET_UPPER_LIMIT(F_Xo))


/**
 * @}
 */


/** @defgroup Radio_Channel_Space          Radio Channel Space
 * @{
 */


#define CHSPACE_DIVIDER         32768              /*!< 2^15 factor dividing fxo in channel space formula */

#define IS_CHANNEL_SPACE(CHANNELSPACE, F_Xo)    (CHANNELSPACE<=(F_Xo/32768*255))

/**
 * @}
 */


/** @defgroup Radio_Datarate                    Radio Datarate
 * @{
 */
#define MINIMUM_DATARATE                 100  /*!< Minimum datarate supported by SPIRIT1 100 bps */
#define MAXIMUM_DATARATE                 510000  /*!< Maximum datarate supported by SPIRIT1 500 kbps */

#define IS_DATARATE(DATARATE)           (DATARATE>=MINIMUM_DATARATE && DATARATE<=MAXIMUM_DATARATE)

/**
 * @}
 */


/** @defgroup Radio_Frequency_Deviation         Radio Frequency Deviation
 * @{
 */
#define F_DEV_MANTISSA_UPPER_LIMIT      7  /*!< Maximum value for the mantissa in frequency deviation formula */
#define F_DEV_EXPONENT_UPPER_LIMIT      9  /*!< Maximum value for the exponent in frequency deviation formula */

#define F_DEV_LOWER_LIMIT(F_Xo)        (F_Xo>>16)
#define F_DEV_UPPER_LIMIT(F_Xo)        ((F_Xo*15)>>10)

#define IS_F_DEV(FDEV,F_Xo)             (FDEV>=F_DEV_LOWER_LIMIT(F_Xo) && FDEV<=F_DEV_UPPER_LIMIT(F_Xo))

/**
 * @}
 */

/** @defgroup Radio_Channel_Bandwidth           Radio Channel Bandwidth
 * @{
 */
#define CH_BW_LOWER_LIMIT(F_Xo)      1100*(F_Xo/1000000)/26  /*!< Minimum value of the channel filter bandwidth */
#define CH_BW_UPPER_LIMIT(F_Xo)    800100*(F_Xo/1000000)/26  /*!< Maximum value of the channel filter bandwidth */

#define IS_CH_BW(BW,F_Xo)         ((BW)>=CH_BW_LOWER_LIMIT(F_Xo) && (BW)<=CH_BW_UPPER_LIMIT(F_Xo))

/**
 * @}
 */


/** @defgroup Radio_Power_Amplifier                     Radio Power Amplifier
 * @{
 */

#define IS_PA_MAX_INDEX(INDEX)       ((INDEX)<=7)
#define IS_PAPOWER_DBM(PATABLE)      ((PATABLE)>= (-31) && (PATABLE)<=(12))
#define IS_PAPOWER(PATABLE)          ((PATABLE)<=90)
#define IS_PA_STEP_WIDTH(WIDTH)      ((WIDTH)>=1 && (WIDTH)<=4)

/**
 * @}
 */


/** @defgroup Radio_Automatic_Frequency_Correction              Radio Automatic Frequency Correction
 * @{
 */

#define IS_AFC_FAST_GAIN(GAIN)          ((GAIN)<=15)
#define IS_AFC_SLOW_GAIN(GAIN)          ((GAIN)<=15)
#define IS_AFC_PD_LEAKAGE(LEAKAGE)      ((LEAKAGE)<=31)

/**
 * @}
 */

/** @defgroup Radio_Automatic_Gain_Control                      Radio Automatic Gain Control
 * @{
 */

#define AGC_MEASURE_TIME_UPPER_LIMIT_US(F_Xo)           (393216.0/F_Xo)

#define IS_AGC_MEASURE_TIME_US(TIME, F_Xo)              (TIME<=AGC_MEASURE_TIME_UPPER_LIMIT_US(F_Xo))

#define IS_AGC_MEASURE_TIME(TIME)                       (TIME<=15)

#define AGC_HOLD_TIME_UPPER_LIMIT_US(F_Xo)              (756.0/F_Xo)

#define IS_AGC_HOLD_TIME_US(TIME,F_Xo)                  (TIME<=AGC_HOLD_TIME_UPPER_LIMIT_US(F_Xo))


#define IS_AGC_HOLD_TIME(TIME)                          (TIME<=63)

#define IS_AGC_THRESHOLD(THRESHOLD)                     (THRESHOLD<=15)

/**
 * @}
 */


/** @defgroup Radio_Clock_Recovery                      Radio Clock Recovery
 * @{
 */

#define IS_CLK_REC_P_GAIN(GAIN)       ((GAIN)<=7)
#define IS_CLK_REC_I_GAIN(GAIN)       ((GAIN)<=15)

/**
 * @}
 */ 

/**
 * @}
 */  
 
/* --------------------------------   TIMER  ----------------------------------- */  
 
 
/**
 * @defgroup Timer_Exported_Types       Timer Exported Types
 * @{
 */

/**
 * @brief  All the possible RX timeout stop conditions enumeration.
 */
typedef enum{

     NO_TIMEOUT_STOP = 0x00,                /*!< Timeout never stopped */
     TIMEOUT_ALWAYS_STOPPED = 0x08,         /*!< Timeout always stopped (default) */
     RSSI_ABOVE_THRESHOLD = 0x04,           /*!< Timeout stopped on RSSI above threshold */
     SQI_ABOVE_THRESHOLD = 0x02,            /*!< Timeout stopped on SQI above threshold */
     PQI_ABOVE_THRESHOLD = 0x01,            /*!< Timeout stopped on PQI above threshold */
     RSSI_AND_SQI_ABOVE_THRESHOLD = 0x06,   /*!< Timeout stopped on both RSSI and SQI above threshold */
     RSSI_AND_PQI_ABOVE_THRESHOLD = 0x05,   /*!< Timeout stopped on both RSSI and PQI above threshold */
     SQI_AND_PQI_ABOVE_THRESHOLD = 0x03,    /*!< Timeout stopped on both SQI and PQI above threshold */
     ALL_ABOVE_THRESHOLD = 0x07,            /*!< Timeout stopped only if RSSI, SQI and PQI are above threshold */
     RSSI_OR_SQI_ABOVE_THRESHOLD = 0x0E,    /*!< Timeout stopped if one between RSSI or SQI are above threshold */
     RSSI_OR_PQI_ABOVE_THRESHOLD = 0x0D,    /*!< Timeout stopped if one between RSSI or PQI are above threshold */
     SQI_OR_PQI_ABOVE_THRESHOLD = 0x0B,     /*!< Timeout stopped if one between SQI or PQI are above threshold */
     ANY_ABOVE_THRESHOLD = 0x0F             /*!< Timeout stopped if one among RSSI, SQI or SQI are above threshold */

} RxTimeoutStopCondition;


#define IS_RX_TIMEOUT_STOP_CONDITION(COND)  ( COND == NO_TIMEOUT_STOP || \
                                                COND == TIMEOUT_ALWAYS_STOPPED || \
                                                COND == RSSI_ABOVE_THRESHOLD || \
                                                COND == SQI_ABOVE_THRESHOLD || \
                                                COND == PQI_ABOVE_THRESHOLD || \
                                                COND == RSSI_AND_SQI_ABOVE_THRESHOLD || \
                                                COND == RSSI_AND_PQI_ABOVE_THRESHOLD || \
                                                COND == SQI_AND_PQI_ABOVE_THRESHOLD || \
                                                COND == ALL_ABOVE_THRESHOLD || \
                                                COND == RSSI_OR_SQI_ABOVE_THRESHOLD || \
                                                COND == RSSI_OR_PQI_ABOVE_THRESHOLD || \
                                                COND == SQI_OR_PQI_ABOVE_THRESHOLD || \
                                                COND == ANY_ABOVE_THRESHOLD )

/**
 * @}
 */


/**
 * @defgroup Timer_Exported_Constants   Timer Exported Constants
 * @{
 */

/**
 * @brief  It represents the Time Step for RX_Timeout timer in case of 24 MHz Crystal, expressed in us.
 *         It is equal to 1210/(24*10^6). With this time step it is possible to fix the RX_Timeout to
 *         a minimum value of 50.417us to a maximum value of about 3.278 s.
 *         Remember that it is possible to have infinite RX_Timeout writing 0 in the RX_Timeout_Counter and/or RX_Timeout_Prescaler registers.
 */
#define      RX_TCLK_24MHz           50.417f
#define      IS_RX_TIMEOUT_24MHz(TIMEOUT)        (TIMEOUT*1000)>=RX_TCLK_24MHz

/**
 * @brief  It represents the Time Step for RX_Timeout timer in case of 26 MHz Crystal, expressed in us.
 *         It is equal to 1210/(26*10^6). With this time step it is possible to fix the RX_Timeout to
 *         a minimum value of 46.538us to a maximum value of about 3.026 s.
 *         Remember that it is possible to have infinite RX_Timeout writing 0 in the RX_Timeout_Counter register.
 */
#define      RX_TCLK_26MHz           46.538f
#define      IS_RX_TIMEOUT_26MHz(TIMEOUT)        (TIMEOUT*1000)>=RX_TCLK_26MHz

/**
 * @brief  It represents the Time Step for RX_Wakeup timer expressed in us. This timer is based on RCO (about 34.7 kHZ).
 *         With this time step it is possible to fix the Wakeup_Timeout to a minimum value of 28.818us to a maximum
 *         value of about 1.888 s.
 */
#define      WAKEUP_TCLK            28.818f
#define      IS_WKUP_TIMEOUT(TIMEOUT)        (TIMEOUT*1000)>=WAKEUP_TCLK

/**
 * @}
 */

/**
 * @defgroup Timer_Exported_Macros              Timer Exported Macros
 * @{
 */

#define SET_INFINITE_RX_TIMEOUT()     SpiritTimerSetRxTimeoutCounter(0)

/**
 * @}
 */
 
 /**
 * @}
 */
 
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* ++++++++++++++++++++++   Exported global variable    +++++++++++++++++++++++ */
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

 /**
 * @addtogroup SPIRIT_Exported_variables         SPIRIT Exported Variables
 * @brief SPIRIT Global variable accessible by the application.
 * @{
 */

/**
 * @defgroup Common_Exported_Varaibles     Common Exported Varaibles
 * @{
 */
extern volatile SpiritStatus g_xStatus;

/**
 * @}
 */

/**
 * @}
 */

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* ++++++++++++++++++++++   API Functions prototypes    +++++++++++++++++++++++ */
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

 /**
 * @addtogroup SPIRIT_FunctionsPrototypes         SPIRIT FunctionsPrototypes
 * @brief Header file containing all the SPIRIT FunctionsPrototypes.
 * @{
 */
 
/**
 * @defgroup Aes_Exported_Functions     AES Exported Functions
 * @{
 */
void SpiritAesMode(SpiritFunctionalState xNewState);
void SpiritAesWriteDataIn(uint8_t* pcBufferDataIn, uint8_t cDataLength);
void SpiritAesReadDataOut(uint8_t* pcBufferDataOut, uint8_t cDataLength);
void SpiritAesWriteKey(uint8_t* pcKey);
void SpiritAesReadKey(uint8_t* pcKey);
void SpiritAesDeriveDecKeyFromEnc(void);
void SpiritAesExecuteEncryption(void);
void SpiritAesExecuteDecryption(void);
void SpiritAesDeriveDecKeyExecuteDec(void);
/**
 * @}
 */
 
 /**
 * @defgroup Calibration_Exported_Functions     Calibration Exported Functions
 * @{
 */
void SpiritCalibrationRco(SpiritFunctionalState xNewState);
void SpiritCalibrationVco(SpiritFunctionalState xNewState);
void SpiritCalibrationSetRcoCalWords(uint8_t cRwt, uint8_t cRfb);
void SpiritCalibrationGetRcoCalWords(uint8_t* pcRwt, uint8_t* pcRfb);
uint8_t SpiritCalibrationGetVcoCalData(void);
void SpiritCalibrationSetVcoCalDataTx(uint8_t cVcoCalData);
uint8_t SpiritCalibrationGetVcoCalDataTx(void);
void SpiritCalibrationSetVcoCalDataRx(uint8_t cVcoCalData);
uint8_t SpiritCalibrationGetVcoCalDataRx(void);
VcoSel SpiritCalibrationGetVcoSelecttion(void);
void SpiritCalibrationSelectVco(VcoSel xVco);
/**
 * @}
 */

 /**
 * @defgroup Commands_Exported_Functions    Commands Exported Functions
 * @{
 */
void SpiritCmdStrobeCommand(SpiritCmd xCommandCode);
/**
 * @}
 */
 
/**
 * @defgroup Csma_Exported_Functions    CSMA Exported Functions
 * @{
 */
void SpiritCsmaInit(CsmaInit* pxCsmaInit);
void SpiritCsmaGetInfo(CsmaInit* pxCsmaInit);
void SpiritCsma(SpiritFunctionalState xNewState);
SpiritFunctionalState SpiritCsmaGetCsma(void);
void SpiritCsmaPersistentMode(SpiritFunctionalState xNewState);
SpiritFunctionalState SpiritCsmaGetPersistentMode(void);
void SpiritCsmaSeedReloadMode(SpiritFunctionalState xNewState);
SpiritFunctionalState SpiritCsmaGetSeedReloadMode(void);
void SpiritCsmaSetBuCounterSeed(uint16_t nBuCounterSeed);
uint16_t SpiritCsmaGetBuCounterSeed(void);
void SpiritCsmaSetBuPrescaler(uint8_t cBuPrescaler);
uint8_t SpiritCsmaGetBuPrescaler(void);
void SpiritCsmaSetCcaPeriod(CcaPeriod xMultiplierTbit);
CcaPeriod SpiritCsmaGetCcaPeriod(void);
void SpiritCsmaSetCcaLength(CsmaLength xCcaLength);
uint8_t SpiritCsmaGetCcaLength(void);
void SpiritCsmaSetMaxNumberBackoff(uint8_t cMaxNb);
uint8_t SpiritCsmaGetMaxNumberBackoff(void);
/**
 *@}
 */
 
 /**
 * @defgroup DirectRf_Exported_Functions        Direct RF Exported Functions
 * @{
 */
void SpiritDirectRfSetRxMode(DirectRx xDirectRx);
DirectRx SpiritDirectRfGetRxMode(void);
void SpiritDirectRfSetTxMode(DirectTx xDirectTx);
DirectTx SpiritDirectRfGetTxMode(void);
/**
 *@}
 */
 
 /**
 * @defgroup General_Exported_Functions         General Exported Functions
 * @{
 */
void SpiritGeneralBatteryLevel(SpiritFunctionalState xNewState);
void SpiritGeneralSetBatteryLevel(BatteryLevel xBatteryLevel);
BatteryLevel SpiritGeneralGetBatteryLevel(void);
void SpiritGeneralBrownOut(SpiritFunctionalState xNewState);
void SpiritGeneralHighPwr(SpiritFunctionalState xNewState);
void SpiritGeneralSetExtRef(ModeExtRef xExtMode);
ModeExtRef SpiritGeneralGetExtRef(void);
void SpiritGeneralSetXoGm(GmConf xGm);
GmConf SpiritGeneralGetXoGm(void);
PacketType SpiritGeneralGetPktType(void);
uint16_t SpiritGeneralGetDevicePartNumber(void);
uint8_t SpiritGeneralGetSpiritVersion(void);
/**
 * @}
 */
 
 /** @defgroup Gpio_Exported_Functions   GPIO Exported Functions
 * @{
 */
void SpiritGpioInit(SGpioInit* pxGpioInitStruct);
void SpiritGpioTemperatureSensor(SpiritFunctionalState xNewState);
void SpiritGpioSetLevel(SpiritGpioPin xGpioX, OutputLevel xLevel);
OutputLevel SpiritGpioGetLevel(SpiritGpioPin xGpioX);
void SpiritGpioClockOutput(SpiritFunctionalState xNewState);
void SpiritGpioClockOutputInit(ClockOutputInit* pxClockOutputInitStruct);
void SpiritGpioSetXOPrescaler(ClockOutputXOPrescaler xXOPrescaler);
ClockOutputXOPrescaler SpiritGpioGetXOPrescaler(void);
void SpiritGpioSetRCOPrescaler(ClockOutputRCOPrescaler xRCOPrescaler);
ClockOutputRCOPrescaler SpiritGpioGetRCOPrescaler(void);
void SpiritGpioSetExtraClockCycles(ExtraClockCycles xExtraCycles);
ExtraClockCycles SpiritGpioGetExtraClockCycles(void);
/**
 * @}
 */
 
 /**
 * @defgroup Irq_Exported_Functions     IRQ Exported Functions
 * @{
 */
void SpiritIrqDeInit(SpiritIrqs* pxIrqInit);
void SpiritIrqInit(SpiritIrqs* pxIrqInit);
void SpiritIrq(IrqList xIrq, SpiritFunctionalState xNewState);
void SpiritIrqGetMask(SpiritIrqs* pxIrqMask);
void SpiritIrqGetStatus(SpiritIrqs* pxIrqStatus);
void SpiritIrqClearStatus(void);
SpiritBool SpiritIrqCheckFlag(IrqList xFlag);
/**
 * @}
 */
 
 /**
 * @defgroup LinearFifo_Exported_Functions                      Linear FIFO Exported Functions
 * @{
 */
uint8_t SpiritLinearFifoReadNumElementsRxFifo(void);
uint8_t SpiritLinearFifoReadNumElementsTxFifo(void);
void SpiritLinearFifoSetAlmostFullThresholdRx(uint8_t cThrRxFifo);
uint8_t SpiritLinearFifoGetAlmostFullThresholdRx(void);
void SpiritLinearFifoSetAlmostEmptyThresholdRx(uint8_t cThrRxFifo);
uint8_t SpiritLinearFifoGetAlmostEmptyThresholdRx(void);
void SpiritLinearFifoSetAlmostFullThresholdTx(uint8_t cThrTxFifo);
uint8_t SpiritLinearFifoGetAlmostFullThresholdTx(void);
void SpiritLinearFifoSetAlmostEmptyThresholdTx(uint8_t cThrTxFifo);
uint8_t SpiritLinearFifoGetAlmostEmptyThresholdTx(void);
/**
 * @}
 */
 
/**
 * @addgroup SPIRIT_MANAGEMENT_FUNCTIONS
 * @{
 */
uint8_t SpiritManagementWaVcoCalibration(void);
void SpiritManagementWaCmdStrobeTx(void);
void SpiritManagementWaCmdStrobeRx(void);
void SpiritManagementWaTRxFcMem(uint32_t nDesiredFreq);
void SpiritManagementWaExtraCurrent(void);
/**
* @}
*/
 
/**
 * @defgroup PktBasic_Exported_Functions        Pkt Basic Exported Functions
 * @{
 */
void SpiritPktBasicInit(PktBasicInit* pxPktBasicInit);
void SpiritPktBasicGetInfo(PktBasicInit* pxPktBasicInit);
void SpiritPktBasicAddressesInit(PktBasicAddressesInit* pxPktBasicAddresses);
void SpiritPktBasicGetAddressesInfo(PktBasicAddressesInit* pxPktBasicAddresses);
void SpiritPktBasicSetFormat(void);
void SpiritPktBasicAddressField(SpiritFunctionalState xAddressField);
SpiritFunctionalState SpiritPktBasicGetAddressField(void);
void SpiritPktBasicSetPayloadLength(uint16_t nPayloadLength);
uint16_t SpiritPktBasicGetPayloadLength(void);
uint16_t SpiritPktBasicGetReceivedPktLength(void);
void SpiritPktBasicSetVarLengthWidth(uint16_t nMaxPayloadLength,SpiritFunctionalState xAddressField, BasicControlLength xControlLength);
/**
 *@}
 */ 
 
/**
 * @defgroup PktCommon_Exported_Functions               Pkt Common Exported Functions
 * @{
 */
void SpiritPktCommonSetControlLength(PktControlLength xControlLength);
uint8_t SpiritPktCommonGetControlLength(void);
void SpiritPktCommonSetPreambleLength(PktPreambleLength xPreambleLength);
uint8_t SpiritPktCommonGetPreambleLength(void);
void SpiritPktCommonSetSyncLength(PktSyncLength xSyncLength);
uint8_t SpiritPktCommonGetSyncLength(void);
void SpiritPktCommonSetFixVarLength(PktFixVarLength xFixVarLength);
void SpiritPktCommonFilterOnCrc(SpiritFunctionalState xNewState);
SpiritFunctionalState SpiritPktCommonGetFilterOnCrc(void);
void SpiritPktCommonSetCrcMode(PktCrcMode xCrcLength);
PktCrcMode SpiritPktCommonGetCrcMode(void);
void SpiritPktCommonWhitening(SpiritFunctionalState xNewState);
void SpiritPktCommonFec(SpiritFunctionalState xNewState);
void SpiritPktCommonSetSyncxWord(PktSyncX xSyncX,  uint8_t cSyncWord);
uint8_t SpiritPktCommonGetSyncxWord(PktSyncX xSyncX);
void SpiritPktCommonSetSyncWords(uint32_t lSyncWords, PktSyncLength xSyncLength);
uint32_t SpiritPktCommonGetSyncWords(PktSyncLength xSyncLength);
uint8_t SpiritPktCommonGetVarLengthWidth(void);
void SpiritPktCommonSetDestinationAddress(uint8_t cAddress);
uint8_t SpiritPktCommonGetTransmittedDestAddress(void);
void SpiritPktCommonSetMyAddress(uint8_t cAddress);
uint8_t SpiritPktCommonGetMyAddress(void);
void SpiritPktCommonSetBroadcastAddress(uint8_t cAddress);
uint8_t SpiritPktCommonGetBroadcastAddress(void);
SpiritFunctionalState SpiritPktCommonGetTxAckRequest(void);
void SpiritPktCommonSetMulticastAddress(uint8_t cAddress);
uint8_t SpiritPktCommonGetMulticastAddress(void);
void SpiritPktCommonSetCtrlMask(uint32_t lMask);
uint32_t SpiritPktCommonGetCtrlMask(void);
void SpiritPktCommonSetCtrlReference(uint32_t lReference);
uint32_t SpiritPktCommonGetCtrlReference(void);
void SpiritPktCommonSetTransmittedCtrlField(uint32_t lField);
uint32_t SpiritPktCommonGetTransmittedCtrlField(void);
void SpiritPktCommonFilterOnMyAddress(SpiritFunctionalState xNewState);
void SpiritPktCommonFilterOnMulticastAddress(SpiritFunctionalState xNewState);
void SpiritPktCommonFilterOnBroadcastAddress(SpiritFunctionalState xNewState);
SpiritFunctionalState SpiritPktCommonGetFilterOnMyAddress(void);
SpiritFunctionalState SpiritPktCommonGetFilterOnMulticastAddress(void);
SpiritFunctionalState SpiritPktCommonGetFilterOnBroadcastAddress(void);
uint8_t SpiritPktCommonGetReceivedDestAddress(void);
uint32_t SpiritPktCommonGetReceivedCtrlField(void);
void SpiritPktCommonGetReceivedCrcField(uint8_t* cCrcFieldVect);
void SpiritPktCommonAutoAck(SpiritFunctionalState xAutoAck,SpiritFunctionalState xPiggybacking);
void SpiritPktCommonRequireAck(SpiritFunctionalState xRequireAck);
void SpiritPktCommonSetTransmittedSeqNumberReload(uint8_t cSeqNumberReload);
void SpiritPktCommonSetNMaxReTx(PktNMaxReTx xNMaxReTx);
uint8_t SpiritPktCommonGetNMaxReTx(void);
uint8_t SpiritPktCommonGetReceivedDestAddress(void);
uint8_t SpiritPktCommonGetReceivedSourceAddress(void);
uint8_t SpiritPktCommonGetReceivedSeqNumber(void);
uint8_t SpiritPktCommonGetReceivedNackRx(void);
uint8_t SpiritPktCommonGetTransmittedSeqNumber(void);
uint8_t SpiritPktCommonGetNReTx(void);
void SpiritPktCommonFilterOnControlField(SpiritFunctionalState xNewState);
SpiritFunctionalState SpiritPktCommonGetFilterOnControlField(void);
/**
 *@}
 */ 
 
/**
 * @defgroup PktMbus_Exported_Functions                 Pkt MBUS Exported Functions
 * @{
 */
void SpiritPktMbusInit(PktMbusInit* pxPktMbusInit);
void SpiritPktMbusGetInfo(PktMbusInit* pxPktMbusInit);
void SpiritPktMbusSetFormat(void);
void SpiritPktMbusSetPreamble(uint8_t cPreamble);
uint8_t SpiritPktMbusGetPreamble(void);
void SpiritPktMbusSetPostamble(uint8_t cPostamble);
uint8_t SpiritPktMbusGetPostamble(void);
void SpiritPktMbusSetSubmode(MbusSubmode xMbusSubmode);
MbusSubmode SpiritPktMbusGetSubmode(void);
void SpiritPktMbusSetPayloadLength(uint16_t nPayloadLength);
uint16_t SpiritPktMbusGetPayloadLength(void);
/**
 *@}
 */
 
/**
 * @defgroup PktStack_Exported_Functions        Pkt STack Exported Functions
 * @{
 */
void SpiritPktStackInit(PktStackInit* pxPktStackInit);
void SpiritPktStackGetInfo(PktStackInit* pxPktStackInit);
void SpiritPktStackAddressesInit(PktStackAddressesInit* pxPktStackAddresses);
void SpiritPktStackGetAddressesInfo(PktStackAddressesInit* pxPktStackAddresses);
void SpiritPktStackLlpInit(PktStackLlpInit* pxPktStackLlpInit);
void SpiritPktStackLlpGetInfo(PktStackLlpInit* pxPktStackLlpInit);
void SpiritPktStackSetFormat(void);
void SpiritPktStackSetPayloadLength(uint16_t nPayloadLength);
uint16_t SpiritPktStackGetPayloadLength(void);
void SpiritPktStackSetVarLengthWidth(uint16_t nMaxPayloadLength, StackControlLength xControlLength);
void SpiritPktStackSetRxSourceMask(uint8_t cMask);
uint8_t SpiritPktStackGetRxSourceMask(void);
uint16_t SpiritPktStackGetReceivedPktLength(void);
void SpiritPktStackFilterOnSourceAddress(SpiritFunctionalState xNewState);
void SpiritPktStackSetAddressLength(void);
/**
 *@}
 */ 
 
/**
 * @defgroup Qi_Exported_Functions       QI Exported Functions
 * @{
 */
void SpiritQiPqiCheck(SpiritFunctionalState xNewState);
void SpiritQiSqiCheck(SpiritFunctionalState xNewState);
void SpiritQiSetPqiThreshold(PqiThreshold xPqiThr);
PqiThreshold SpiritQiGetPqiThreshold(void);
void SpiritQiSetSqiThreshold(SqiThreshold xSqiThr);
SqiThreshold SpiritQiGetSqiThreshold(void);
void SpiritQiSetRssiThreshold(uint8_t cRssiThr);
uint8_t SpiritQiGetRssiThreshold(void);
uint8_t SpiritQiComputeRssiThreshold(int32_t cDbmValue);
void SpiritQiSetRssiThresholddBm(int32_t nDbmValue);
uint8_t SpiritQiGetPqi(void);
uint8_t SpiritQiGetSqi(void);
uint8_t SpiritQiGetLqi(void);
SpiritFlagStatus SpiritQiGetCs(void);
uint8_t SpiritQiGetRssi(void);
void SpiritQiSetRssiFilterGain(RssiFilterGain xRssiFg);
RssiFilterGain SpiritQiGetRssiFilterGain(void);
void SpiritQiSetCsMode(CSMode xCsMode);
CSMode SpiritQiGetCsMode(void);
void SpiritQiCsTimeoutMask(SpiritFunctionalState xNewState);
void SpiritQiPqiTimeoutMask(SpiritFunctionalState xNewState);
void SpiritQiSqiTimeoutMask(SpiritFunctionalState xNewState);
/**
 *@}
 */
 
 /** @defgroup Radio_Exported_Functions                          Radio Exported Functions
 * @{
 */
uint8_t SpiritRadioInit(SRadioInit* pxSRadioInitStruct);
void SpiritRadioGetInfo(SRadioInit* pxSRadioInitStruct);
void SpiritRadioSetXtalFlag(XtalFlag xXtal);
XtalFlag SpiritRadioGetXtalFlag(void);
uint8_t SpiritRadioSearchWCP(uint32_t lFc);
void SpiritRadioSetSynthWord(uint32_t lSynthWord);
uint32_t SpiritRadioGetSynthWord(void);
void SpiritRadioSetBand(BandSelect xBand);
BandSelect SpiritRadioGetBand(void);
void SpiritRadioSetChannel(uint8_t cChannel);
uint8_t SpiritRadioGetChannel(void);
void SpiritRadioSetChannelSpace(uint32_t lChannelSpace);
uint32_t SpiritRadioGetChannelSpace(void);
void SpiritRadioSetFrequencyOffsetPpm(int16_t nXtalPpm);
void SpiritRadioSetFrequencyOffset(int32_t lFOffset);
int32_t SpiritRadioGetFrequencyOffset(void);
void SpiritRadioVcoCalibrationWAFB(SpiritFunctionalState xNewstate);
uint8_t SpiritRadioSetFrequencyBase(uint32_t lFBase);
uint32_t SpiritRadioGetFrequencyBase(void);
uint32_t SpiritRadioGetCenterFrequency(void);
void SpiritRadioSearchDatarateME(uint32_t lDatarate, uint8_t* pcM, uint8_t* pcE);
void SpiritRadioSearchFreqDevME(uint32_t lFDev, uint8_t* pcM, uint8_t* pcE);
void SpiritRadioSearchChannelBwME(uint32_t lBandwidth, uint8_t* pcM, uint8_t* pcE);
void SpiritRadioSetDatarate(uint32_t lDatarate);
uint32_t SpiritRadioGetDatarate(void);
void SpiritRadioSetFrequencyDev(uint32_t lFDev);
uint32_t SpiritRadioGetFrequencyDev(void);
void SpiritRadioSetChannelBW(uint32_t lBandwidth);
uint32_t SpiritRadioGetChannelBW(void);
void SpiritRadioSetModulation(ModulationSelect xModulation);
ModulationSelect SpiritRadioGetModulation(void);
void SpiritRadioCWTransmitMode(SpiritFunctionalState xNewState);
void SpiritRadioSetOokPeakDecay(OokPeakDecay xOokDecay);
OokPeakDecay SpiritRadioGetOokPeakDecay(void);
uint8_t SpiritRadioGetdBm2Reg(uint32_t lFBase, float fPowerdBm);
float SpiritRadioGetReg2dBm(uint32_t lFBase, uint8_t cPowerReg);
void SpiritRadioSetPATabledBm(uint8_t cPALevelMaxIndex, uint8_t cWidth, PALoadCapacitor xCLoad, float* pfPAtabledBm);
void SpiritRadioGetPATabledBm(uint8_t* pcPALevelMaxIndex, float* pfPAtabledBm);
void SpiritRadioSetPATable(uint8_t cPALevelMaxIndex, uint8_t cWidth, PALoadCapacitor xCLoad, uint8_t* pcPAtable);
void SpiritRadioGetPATable(uint8_t* pcPALevelMaxIndex, uint8_t* pcPAtable);
void SpiritRadioSetPALeveldBm(uint8_t cIndex, float fPowerdBm);
float SpiritRadioGetPALeveldBm(uint8_t cIndex);
void SpiritRadioSetPALevel(uint8_t cIndex, uint8_t cPower);
uint8_t SpiritRadioGetPALevel(uint8_t cIndex);
void SpiritRadioSetPACwc(PALoadCapacitor xCLoad);
PALoadCapacitor SpiritRadioGetPACwc(void);
void SpiritRadioSetPALevelMaxIndex(uint8_t cIndex);
uint8_t SpiritRadioGetPALevelMaxIndex(void);
void SpiritRadioSetPAStepWidth(uint8_t cWidth);
uint8_t SpiritRadioGetPAStepWidth(void);
void SpiritRadioPARamping(SpiritFunctionalState xNewState);
SpiritFunctionalState SpiritRadioGetPARamping(void);
void SpiritRadioAFC(SpiritFunctionalState xNewState);
void SpiritRadioAFCFreezeOnSync(SpiritFunctionalState xNewState);
void SpiritRadioSetAFCMode(AFCMode xMode);
AFCMode SpiritRadioGetAFCMode(void);
void SpiritRadioSetAFCPDLeakage(uint8_t cLeakage);
uint8_t SpiritRadioGetAFCPDLeakage(void);
void SpiritRadioSetAFCFastPeriod(uint8_t cLength);
uint8_t SpiritRadioGetAFCFastPeriod(void);
void SpiritRadioSetAFCFastGain(uint8_t cGain);
uint8_t SpiritRadioGetAFCFastGain(void);
void SpiritRadioSetAFCSlowGain(uint8_t cGain);
uint8_t SpiritRadioGetAFCSlowGain(void);
int8_t SpiritRadioGetAFCCorrectionReg(void);
int32_t SpiritRadioGetAFCCorrectionHz(void);
void SpiritRadioAGC(SpiritFunctionalState xNewState);
void SpiritRadioSetAGCMode(AGCMode xMode);
AGCMode SpiritRadioGetAGCMode(void);
void SpiritRadioSetAGCMeasureTimeUs(uint16_t nTime);
uint16_t SpiritRadioGetAGCMeasureTimeUs(void);
void SpiritRadioSetAGCMeasureTime(uint8_t cTime);
uint8_t SpiritRadioGetAGCMeasureTime(void);
void SpiritRadioSetAGCHighThreshold(uint8_t cHighThreshold);
uint8_t SpiritRadioGetAGCHighThreshold(void);
void SpiritRadioSetAGCLowThreshold(uint8_t cLowThreshold);
uint8_t SpiritRadioGetAGCLowThreshold(void);
void SpiritRadioSetClkRecMode(ClkRecMode xMode);
ClkRecMode SpiritRadioGetClkRecMode(void);
void SpiritRadioSetClkRecPGain(uint8_t cPGain);
uint8_t SpiritRadioGetClkRecPGain(void);
void SpiritRadioSetClkRecIGain(uint8_t cIGain);
uint8_t SpiritRadioGetClkRecIGain(void);
void SpiritRadioSetClkRecPstFltLength(PstFltLength xLength);
PstFltLength SpiritRadioGetClkRecPstFltLength(void);
void SpiritRadioCsBlanking(SpiritFunctionalState xNewState);
void SpiritRadioPersistenRx(SpiritFunctionalState xNewState);
uint32_t SpiritRadioGetXtalFrequency(void);
void SpiritRadioSetXtalFrequency(uint32_t lXtalFrequency);
void SpiritRadioSetRefDiv(SpiritFunctionalState xNewState);
SpiritFunctionalState SpiritRadioGetRefDiv(void);
void SpiritRadioSetDigDiv(SpiritFunctionalState xNewState);
SpiritFunctionalState SpiritRadioGetDigDiv(void);
/**
 * @}
 */

 /**
 * @defgroup Timer_Exported_Functions           Timer Exported Functions
 * @{
 */
void SpiritTimerLdcrMode(SpiritFunctionalState xNewState);
void SpiritTimerLdcrAutoReload(SpiritFunctionalState xNewState);
SpiritFunctionalState SpiritTimerLdcrGetAutoReload(void);
void SpiritTimerSetRxTimeout(uint8_t cCounter , uint8_t cPrescaler);
void SpiritTimerSetRxTimeoutMs(float fDesiredMsec);
void SpiritTimerSetRxTimeoutCounter(uint8_t cCounter);
void SpiritTimerSetRxTimeoutPrescaler(uint8_t cPrescaler);
void SpiritTimerGetRxTimeout(float* pfTimeoutMsec, uint8_t* pcCounter , uint8_t* pcPrescaler);
void SpiritTimerSetWakeUpTimer(uint8_t cCounter , uint8_t cPrescaler);
void SpiritTimerSetWakeUpTimerMs(float fDesiredMsec);
void SpiritTimerSetWakeUpTimerCounter(uint8_t cCounter);
void SpiritTimerSetWakeUpTimerPrescaler(uint8_t cPrescaler);
void SpiritTimerSetWakeUpTimerReloadMs(float fDesiredMsec);
void SpiritTimerGetWakeUpTimer(float* pfWakeUpMsec, uint8_t* pcCounter , uint8_t* pcPrescaler);
void SpiritTimerSetWakeUpTimerReload(uint8_t cCounter , uint8_t cPrescaler);
void SpiritTimerSetWakeUpTimerReloadCounter(uint8_t cCounter);
void SpiritTimerSetWakeUpTimerReloadPrescaler(uint8_t cPrescaler);
void SpiritTimerGetWakeUpTimerReload(float* pfWakeUpReloadMsec, uint8_t* pcCounter , uint8_t* pcPrescaler);
void SpiritTimerComputeWakeUpValues(float fDesiredMsec , uint8_t* pcCounter , uint8_t* pcPrescaler);
void SpiritTimerComputeRxTimeoutValues(float fDesiredMsec , uint8_t* pcCounter , uint8_t* pcPrescaler);
void SpiritTimerSetRxTimeoutStopCondition(RxTimeoutStopCondition xStopCondition);
void SpiritTimerReloadStrobe(void);
uint16_t SpiritTimerGetRcoFrequency(void);
/**
 * @}
 */

/**
 * @defgroup Common_Exported_Functions           Common Exported Functions
 * @{
 */
void SpiritRefreshStatus(void);

/**
 *@}
 */

/**
 * @}
 */
 
/**
 * @}
 */ 
#ifdef __cplusplus
}
#endif

#endif /* __SPSGRF_H */
