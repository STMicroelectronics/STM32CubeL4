SUbGhz SPSGRF Common files description:

 - spsgrf_app.c/h   Provide to the application some abstraction of the component (spsgrf.c) functions 
 - spsgrf_io.c/h    is the abstraction between component (spsgrf.c) and board pins for SPI and GPIO
                    GPIO
                      (MCU configure and control the SPSGRF gpio (gpio3 and SDN) from MCU side)
                      note: the GPIO functions in spsgrf.c/h are to tell via SPI to SPSGRF module 
                      how those pins have to be set on module side.
                    SPI 
                      SPI transport layer specific to spsgrf because it enters "critical session"
                      by setting/clearing the spsgrf_gpio3 in irq disable/enable each SPI operation
 - spsgrf.c/h       SPSGRF component driver itself (will be moved in BSP when officilized):
                     - AES:          Configuration and management of SPIRIT AES Engine.
                     - CALIBRATION:  Configuration and management of SPIRIT VCO-RCO calibration.
                     - COMMANDS:     Management of SPIRIT Commands.
                     - CSMA:         Configuration and management of SPIRIT CSMA.
                     - DIRECT_RF:    Configuration and management of SPIRIT direct transmission / receive modes.
                     - GENERAL:      Configuration and management of SPIRIT General functionalities.
                     - GPIO:         This file provides all the low level API to manage SPIRIT GPIO (from SPSGRF side)
                     - IRQ:          Configuration and management of SPIRIT IRQs.
                     - LINEAR_FIFO:  Configuration and management of SPIRIT Fifo.
                     - MANAGEMENT:   The management layer for SPIRIT1 library.
                     - PKT_BASIC:    Configuration and management of SPIRIT Basic packets.
                     - PKT_COMMON:   Configuration and management of the common features of SPIRIT packets.
                     - PKT_MBUS:     Configuration and management of SPIRIT MBUS packets.
                     - PKT_STACK:    Configuration and management of SPIRIT STack packets.
                     - QI:           Configuration and management of SPIRIT QI.
                     - RADIO:        This file provides all the low level API to manage Analog and Digital radio part of SPIRIT.
                     - TIMER:        Configuration and management of SPIRIT timers.
                     - Additionally at the beginning of spsgrf.h there is a section for SPSGRF registers mapping.
 - spsgrf_types.h   is also part of the "component driver" but currently separated 
                    because sometime the application just needs these few definitions

@par See more details about each spsgrf.c/h sub-modules, here below:

/* --------------------------------------------------------------------------------------

  * @brief   AES: Configuration and management of SPIRIT AES Engine.
  *
  * @details
  *
  * In order to encrypt data, the user must manage the AES_END IRQ.
  * The data have to be split in blocks of 16 bytes and written
  * into the <i>AES DATA IN registers</i>. Then, after the key is written
  * into the <i>AES KEY registers</i>, a command of <i>Execute encryption</i>
  * has to be sent.
  *
  * <b>Example:</b>
  * @code
  *
  *  SpiritAesWriteDataIn(data_buff , N_BYTES);
  *  SpiritAesExecuteEncryption();
  *
  *  while(!aes_end_flag);       // the flag is set by the ISR routine which manages the AES_END irq
  *  aes_end_flag=RESET;
  *
  *  SpiritAesReadDataOut(enc_data_buff , N_BYTES);
  *
  * @endcode
  *
  * In order to decrypt data, the user must manage the AES_END IRQ and have a decryption key.
  * There are two operative modes to make the data decryption:
  * <ul>
  * <li> Derive the decryption key from the encryption key and decrypt data directly
  * using the <i>SpiritAesDeriveDecKeyExecuteDec()</i> function
  *
  * <b>Example:</b>
  * @code
  *
  *  SpiritAesWriteDataIn(enc_data_buff , N_BYTES);
  *  SpiritAesDeriveDecKeyExecuteDec();
  *
  *  while(!aes_end_flag);       // the flag is set by the ISR routine which manages the AES_END irq
  *  aes_end_flag=RESET;
  *
  *  SpiritAesReadDataOut(data_buff , N_BYTES);
  *
  * @endcode
  * </li>
  *
  * <li> Derive the decryption key from the encryption key using the <i>SpiritAesDeriveDecKeyFromEnc()</i>
  * function, store it into the <i>AES KEY registers</i> and then decrypt data using the
  * <i>SpiritAesExecuteDecryption()</i> function
  *
  * <b>Example:</b>
  * @code
  *
  *  SpiritAesWriteDataIn(key_enc , 16);
  *  SpiritAesDeriveDecKeyFromEnc();
  *
  *  while(!aes_end_flag);       // the flag is set by the ISR routine which manages the AES_END irq
  *  aes_end_flag=RESET;
  *
  *  SpiritAesReadDataOut(key_dec , 16);
  *
  *  SpiritAesWriteKey(key_dec);
  *  SpiritAesWriteDataIn(enc_data_buff , 16);
  *  SpiritAesExecuteDecryption();
  *
  *  while(!aes_end_flag);       // the flag is set by the ISR routine which manages the AES_END irq
  *  aes_end_flag=RESET;
  *
  *  SpiritAesReadDataOut(data_buff , N_BYTES);
  *
  * @endcode
  * </li>
  * </ul>

-------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------

  * @brief   CALIBRATION: Configuration and management of SPIRIT VCO-RCO calibration.
  *
  * @details
  *
  * This module allows the user to set some parameters which deal
  * with the oscillators calibration.
  * The state machine of Spirit contemplates some optional calibrating operations
  * in the transition between the READY and the LOCK state.
  * The user is allowed to enable or disable the automatic RCO/VCO calibration
  * by calling the functions <i>@ref SpiritCalibrationVco()</i> and <i>@ref SpiritCalibrationRco()</i>.
  * The following example shows how to do an initial calibration of VCO.
  *
  * <b>Example:</b>
  * @code
  *  uint8_t calData;
  *
  *  SpiritCalibrationVco(S_ENABLE);
  *  SpiritCmdStrobeLockTx();
  *
  *  while(g_xStatus.MC_STATE != MC_STATE_LOCK){
  *      SpiritRefreshStatus();
  *  }
  *
  *  calData = SpiritCalibrationGetVcoCalDataTx();
  *  SpiritCalibrationSetVcoCalDataTx(calData);
  *
  *  SpiritCmdStrobeReady();
  *  SpiritCalibrationVco(S_DISABLE);
  *
  * @endcode
  *
  * Similar operations can be done for the RCO calibrator.

-------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------

  * @brief  COMMANDS: Management of SPIRIT Commands.
  *
  * @details
  *
  * In this module can be found all the API used to strobe commands to
  * Spirit.
  * Every command strobe is an SPI transaction with a specific command code.
  *
  * <b>Example:</b>
  * @code
  *   ...
  *
  *   SpiritCmdStrobeRx();
  *
  *   ...
  * @endcode
  *

-------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------

  * @brief CONFIG:  Spirit Configuration and useful defines 
  * 
  * @details
  *
  * This file is used to include all or a part of the Spirit
  * libraries into the application program which will be used.
  * Moreover some important parameters are defined here and the
  * user is allowed to edit them.

-------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------

  * @brief  CSMA Configuration and management of SPIRIT CSMA.
  * @details
  *
  * The Spirit CSMA feature, when configured and enabled, is transparent
  * for the user. It means the user has only to call the <i>@ref SpiritCsmaInit()</i>
  * function on a filled structure and then enable the CSMA policy using the <i>@ref SpiritCsma()</i>
  * function.
  *
  * <b>Example:</b>
  * @code
  *
  * CsmaInit csmaInit={
  *   S_DISABLE,         // persistent mode
  *   TBIT_TIME_64,      // Tbit time
  *   TCCA_TIME_3,       // Tcca time
  *   5,                 // max number of backoffs
  *   0xFA21,            // BY counter seed
  *   32                 // CU prescaler
  * };
  *
  * ...
  *
  * SpiritCsmaInit(&csmaInit);
  * SpiritCsma(S_ENABLE);
  *
  *
  * @endcode
  *
  * @note The CS status depends of the RSSI threshold set. Please see the Spirit_Qi
  * module for details.

-------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------

  * @brief   DIRECT_RF: Configuration and management of SPIRIT direct transmission / receive modes.
  * @details
  *
  * This module contains functions to manage the direct Tx/Rx mode.
  * The user can choose the way to send data to Spirit through the
  * enumerative types <i>@ref DirectTx</i>/<i>@ref DirectRx</i>.
  *
-------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------

  * @brief  GPIO: This file provides all the low level API to manage SPIRIT GPIO (from SPSGRF side).
  * 
  * @details
  *
  * This module can be used to configure the Spirit GPIO pins to perform
  * specific functions.
  * The structure <i>@ref gpioIRQ</i> can be used to specify these features for
  * one of the four Spirit Gpio pin.
  * The following example shows how to configure a pin (GPIO 3) to be used as an IRQ source
  * for a microcontroller using the <i>@ref SpiritGpioInit()</i> function.
  *
  * <b>Example:</b>
  * @code
  *
  * SGpioInit gpioIRQ={
  *   SPIRIT_GPIO_3,
  *   SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP,
  *   SPIRIT_GPIO_DIG_OUT_IRQ
  * };
  *
  * ...
  *
  * SpiritGpioInit(&gpioIRQ);
  *
  * @endcode
  *
  * @note Please read the functions documentation for the other GPIO features.
  *
-------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------

  * @brief  IRQ: Configuration and management of SPIRIT IRQs.
  * 
  * @details
  *
  * On the Spirit side specific IRQs can be enabled by setting a specific bitmask.
  * The Spirit libraries allow the user to do this in two different ways:
  * <ul>
  *
  * <li>The first enables the IRQs one by one, i.e. using an SPI transaction for each
  * IRQ to enable.
  *
  * <b>Example:</b>
  * @code
  *
  *  SpiritIrqDeInit(NULL);                // this call is used to reset the IRQ mask registers
  *  SpiritIrq(RX_DATA_READY , S_ENABLE);
  *  SpiritIrq(VALID_SYNC , S_ENABLE);
  *  SpiritIrq(RX_TIMEOUT , S_ENABLE);
  *
  * @endcode
  *
  * </li>
  *
  * <li>The second strategy is to set the IRQ bitfields structure. So, during the initialization the user
  * has to fill the @ref SpiritIrqs structure setting to one the single field related to the IRQ he
  * wants to enable, and to zero the single field related to all the IRQs he wants to disable.
  *
  * <b>Example:</b>
  * @code
  *
  *  SpiritIrqs irqMask;
  *
  *  ...
  *
  *  SpiritIrqDeInit(&irqMask);                // this call is used to reset the IRQ mask registers
  *                                            // and to set to 0x00000000 the irq mask in order to disable
  *                                            // all IRQs (disabled by default on startup)
  *  irqMask.IRQ_RX_DATA_READY = 1;
  *  irqMask.IRQ_VALID_SYNC = 1;
  *  irqMask.IRQ_RX_TIMEOUT = 1;
  *
  *  ...
  * @endcode
  * </li>
  * </ul>
  *
  * The most applications will require a Spirit IRQ notification on an microcontroller EXTI line.
  * Then, the user can check which IRQ has been raised using two different ways.
  *
  * On the ISR of the EXTI line physically linked to the Spirit pin configured for IRQ:
  *
  * <ul>
  * <li> Check <b>only one</b> Spirit IRQ (because the Spirit IRQ status register automatically blanks itself
  * after an SPI reading) into the ISR.
  *
  * <b>Example:</b>
  * @code
  *
  *  if(SpiritIrqCheckFlag(RX_DATA_READY))
  *  {
  *          // do something...
  *  }
  *
  * @endcode
  * </li>
  *
  * <li> Check more than one Spirit IRQ status by storing the entire IRQ status registers into a bitfields <i>@ref SpiritIrqs</i> structure
  * and then check the interested bits.
  *
  * <b>Example:</b>
  * @code
  *
  *  SpiritIrqGetStatus(&irqStatus);
  *
  *  if(irqStatus.IRQ_RX_DATA_READY)
  *  {
  *          // do something...
  *  }
  *  if(irqStatus.IRQ_VALID_SYNC)
  *  {
  *         // do something...
  *  }
  *  if(irqStatus.RX_TIMEOUT)
  *  {
  *         // do something...
  *  }
  *
  * @endcode
  * </li>
  * </ul>
  *

-------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------

  * @brief  PKT_BASIC: Configuration and management of SPIRIT Basic packets.
  * 
  * @details
  *
  * This module can be used to manage the configuration of Spirit Basic
  * packets.
  * The user can obtain a packet configuration filling the structure
  * <i>@ref PktBasicInit</i>, defining in it some general parameters
  * for the Spirit Basic packet format.
  * Another structure the user can fill is <i>@ref PktBasicAddressesInit</i>
  * to define the addresses which will be used during the communication.
  * Moreover, functions to set the payload length and the destination address
  * are provided.
  *
  * <b>Example:</b>
  * @code
  *
  * PktBasicInit basicInit={
  *   PKT_PREAMBLE_LENGTH_08BYTES,       // preamble length in bytes
  *   PKT_SYNC_LENGTH_4BYTES,            // sync word length in bytes
  *   0x1A2635A8,                        // sync word
  *   PKT_LENGTH_VAR,                    // variable or fixed payload length
  *   7,                                 // length field width in bits (used only for variable length)
  *   PKT_NO_CRC,                        // CRC mode
  *   PKT_CONTROL_LENGTH_0BYTES,         // control field length
  *   S_ENABLE,                          // address field
  *   S_DISABLE,                         // FEC
  *   S_ENABLE                           // whitening
  * };
  *
  * PktBasicAddressesInit addressInit={
  *   S_ENABLE,                          // enable/disable filtering on my address
  *   0x34,                              // my address (address of the current node)
  *   S_DISABLE,                         // enable/disable filtering on multicast address
  *   0xEE,                              // multicast address
  *   S_DISABLE,                         // enable/disable filtering on broadcast address
  *   0xFF                               // broadcast address
  * };
  *
  * ...
  *
  * SpiritPktBasicInit(&basicInit);
  * SpiritPktBasicAddressesInit(&addressInit);
  *
  * ...
  *
  * SpiritPktBasicSetPayloadLength(20);
  * SpiritPktBasicSetDestinationAddress(0x44);
  *
  * ...
  *
  * @endcode
  *
  * The module provides some other functions that can be used to modify
  * or read only some configuration parameters.
  *

-------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------

  * @brief PKT_COMMON:  Configuration and management of the common features of SPIRIT packets.
  * 
  * @details
  *
  * This module provides all the common functions and definitions used by the
  * packets modules.
  * Here are also defined all the generic enumeration types that are redefined
  * in the specific packets modules, but every enumeration value is referred
  * to this module. So the user who wants to configure the preamble of a Basic,
  * or a STack packet has to use the enumeration values defined here.
  *
  * <b>Example:</b>
  * @code
  *
  *   ...
  *
  *   SpiritPktBasicSetPreambleLength(PKT_PREAMBLE_LENGTH_18BYTES);
  *
  *   ...
  *
  * @endcode
  *
  * @note Is recommended for the user to not use these API directly
  * importing this module in his application.
  *

-------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------

  * @brief  PKT_MBUS: Configuration and management of SPIRIT MBUS packets.
  * 
  * @details
  *
  * This module can be used to manage the configuration of Spirit MBUS
  * packets.
  * The user can obtain a packet configuration filling the structure
  * <i>@ref PktMbusInit</i>, defining in it some general parameters
  * for the Spirit MBUS packet format.
  * Since the MBUS protocol is a standard, the configuration of a MBUS
  * packet is very simple to do.
  *
  * <b>Example:</b>
  * @code
  *
  * PktMbusInit mbusInit={
  *   MBUS_SUBMODE_S1_S2_LONG_HEADER,    // MBUS submode selection
  *   36,                                // added "01" chips on preamble
  *   16                                 // postamble length in "01" chips
  * };
  *
  * ...
  *
  * SpiritPktMbusInit(&mbusInit);
  *
  * ...
  *
  * @endcode
  *
  * The module provides some other functions that can be used to modify
  * or read only some configuration parameters.
  *

-------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------

  * @brief  PKT_STACK: Configuration and management of SPIRIT STack packets.
  * 
  * @details
  *
  * This module can be used to manage the configuration of Spirit STack
  * packets, and it is quite similar to the Basic packets one since the
  * STack packets can be considered an extension of Basic.
  * The user can obtain a packet configuration filling the structure
  * <i>@ref PktStackInit</i>, defining in it some general parameters
  * for the Spirit STack packet format.
  * Another structure the user can fill is <i>@ref PktStackAddressesInit</i>
  * to define the addresses which will be used during the communication.
  * The structure <i>@ref PktStackLlpInit</i> is provided in order to configure
  * the link layer protocol features like autoack, autoretransmission
  * or piggybacking.
  * Moreover, functions to set the payload length and the destination address
  * are provided.
  *
  * <b>Example:</b>
  * @code
  *
  * PktStackInit stackInit={
  *   PKT_PREAMBLE_LENGTH_08BYTES,       // preamble length in bytes
  *   PKT_SYNC_LENGTH_4BYTES,            // sync word length in bytes
  *   0x1A2635A8,                        // sync word
  *   PKT_LENGTH_VAR,                    // variable or fixed payload length
  *   7,                                 // length field width in bits (used only for variable length)
  *   PKT_NO_CRC,                        // CRC mode
  *   PKT_CONTROL_LENGTH_0BYTES,         // control field length
  *   S_DISABLE,                         // FEC
  *   S_ENABLE                           // whitening
  * };
  *
  * PktStackAddressesInit addressInit={
  *   S_ENABLE,                          // enable/disable filtering on my address
  *   0x34,                              // my address (address of the current node)
  *   S_DISABLE,                         // enable/disable filtering on multicast address
  *   0xEE,                              // multicast address
  *   S_DISABLE,                         // enable/disable filtering on broadcast address
  *   0xFF                               // broadcast address
  * };
  *
  * PktStackLlpInit stackLLPInit ={
  *   S_DISABLE,                         // enable/disable the autoack feature
  *   S_DISABLE,                         // enable/disable the piggybacking feature
  *   PKT_DISABLE_RETX                   // set the max number of retransmissions or disable them
  * };
  * ...
  *
  * SpiritPktStackInit(&stackInit);
  * SpiritPktStackAddressesInit(&addressInit);
  * SpiritPktStackLlpInit(&stackLLPInit);
  *
  * ...
  *
  * SpiritPktStackSetPayloadLength(20);
  * SpiritPktStackSetDestinationAddress(0x44);
  *
  * ...
  *
  * @endcode
  *
  * The module provides some other functions that can be used to modify
  * or read only some configuration parameters.
  *

-------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------

  * @brief QI: Configuration and management of SPIRIT QI.
  * @details
  *
  * This module can be used to configure and read some quality indicators
  * used by Spirit.
  * API to set thresholds and to read values in raw mode or in dBm are
  * provided.
  *
  * <b>Example:</b>
  * @code
  *
  *   float rssiValuedBm;
  *   uint8_t pqiValue, sqiValue;
  *
  *   SpiritQiPqiCheck(S_ENABLE);
  *   SpiritQiSqiCheck(S_ENABLE);
  *
  *   ...
  *
  *   rssiValueDbm = SpiritQiGetRssidBm();
  *   pqiValue = SpiritQiGetPqi();
  *   sqiValue = SpiritQiGetSqi();
  *
  *   ...
  *
  * @endcode

-------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------

  * @brief  RADIO: This file provides all the low level API to manage Analog and Digital
  *          radio part of SPIRIT.
  * @details
  *
  * In order to configure the Radio main parameters, the user can
  * fit <i>SRadioInit</i> structure the and call the <i>SpiritRadioInit()</i>
  * function passing its pointer as an argument.
  *
  * <b>Example:</b>
  * @code
  *
  * SRadioInit radioInit = {
  *     0,                       // Xtal offset in ppm
  *     433.4e6,                 // base frequency
  *     20e3,                    // channel space
  *     0,                       // channel number
  *     FSK,                     // modulation select
  *     38400,                   // datarate
  *     20e3,                    // frequency deviation
  *     100.5e3                  // channel filter bandwidth
  * };
  *
  * ...
  *
  * SpiritRadioInit(&radioInit);
  * @endcode
  *
  * Another important parameter for the radio configuration is the
  * transmission power.
  * The user is allowed to configure it using the function <i>SpiritRadioSetPALeveldBm()</i>
  * which sets the PA LEVEL specified by the first argument to the
  * power expressed in dBm by the second parameter.
  *
  * <b>Example:</b>
  * @code
  *
  *  SpiritRadioSetPALeveldBm(0 , 10.0);
  *
  * @endcode
  *
  *
  * @note The effective power that is set can be a little different from the
  * passed argument in dBm because the function performs an approximation.
  *

-------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------

  * @brief  TIMER: Configuration and management of SPIRIT timers.
  * @details
  *
  * This module provides API to configure the Spirit timing mechanisms.
  * They allow the user to set the timer registers using raw values or
  * compute them since the desired timer value is expressed in ms.
  * Moreover the management of the Spirit LDCR mode can be done using
  * these API.
  *
  * <b>Example:</b>
  * @code
  *   ...
  *
  *   SpiritTimerSetRxTimeoutMs(50.0);
  *   SpiritTimerSetWakeUpTimerMs(150.0);
  *
  *   // IRQ configuration for RX_TIMEOUT and WAKEUP_TIMEOUT
  *   ...
  *
  *   SpiritTimerLdcrMode(S_ENABLE);
  *
  *   ...
  *
  * @endcode
  *
-------------------------------------------------------------------------------------- */



