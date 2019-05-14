/**
 ******************************************************************************
 * @file    ble_config_template.h
 * @author  MCD Application Team
 * @brief   BLE configuration file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license SLA0044,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        http://www.st.com/SLA0044
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BLE_CONFIG_H
#define __BLE_CONFIG_H


/******************************************************************************
 *
 * BLE SERVICES CONFIGURATION
 * blesvc
 *
 ******************************************************************************/
/**
 * This setting shall be set to '1' if the device needs to support the Peripheral Role
 * In the MS configuration, both BLE_CFG_PERIPHERAL and BLE_CFG_CENTRAL shall be set to '1'
 */
#define BLE_CFG_PERIPHERAL                                                             1

/**
 * This setting shall be set to '1' if the device needs to support the Central Role
 * In the MS configuration, both BLE_CFG_PERIPHERAL and BLE_CFG_CENTRAL shall be set to '1'
 */
#define BLE_CFG_CENTRAL                                                                1


#define BLE_CFG_DATA_ROLE_MODE                    2

#define BLE_CFG_MENU_DEVICE_INFORMATION           0
/**
 * There is one handler per service enabled
 * Note: There is no handler for the Device Information Service
 *
 * This shall take into account all registered handlers
 * (from either the provided services or the custom services)
 */
#define BLE_CFG_SVC_MAX_NBR_CB      5

#define BLE_CFG_CLT_MAX_NBR_CB      0

/******************************************************************************
 * Device Information Service (DIS)
 ******************************************************************************/
/**< Number of characteristic added by the user */
#define BLE_CFG_DIS_NUMBER_OF_CUSTOM_CHAR                      0

/**< Options: Supported(1) or Not Supported(0) */
#define BLE_CFG_DIS_MANUFACTURER_NAME_STRING                                       1
#define BLE_CFG_DIS_MODEL_NUMBER_STRING                                            0
#define BLE_CFG_DIS_SERIAL_NUMBER_STRING                                           0
#define BLE_CFG_DIS_HARDWARE_REVISION_STRING                                       0
#define BLE_CFG_DIS_FIRMWARE_REVISION_STRING                                       0
#define BLE_CFG_DIS_SOFTWARE_REVISION_STRING                                       0
#define BLE_CFG_DIS_SYSTEM_ID                                                      0
#define BLE_CFG_DIS_IEEE_CERTIFICATION                                             0
#define BLE_CFG_DIS_PNP_ID                                                         0

/**
 * device information service characteristic lengths
 */
#define BLE_CFG_DIS_SYSTEM_ID_LEN_MAX                                                   (8)
#define BLE_CFG_DIS_MODEL_NUMBER_STRING_LEN_MAX                                         (32)
#define BLE_CFG_DIS_SERIAL_NUMBER_STRING_LEN_MAX                                        (32)
#define BLE_CFG_DIS_FIRMWARE_REVISION_STRING_LEN_MAX                                    (32)
#define BLE_CFG_DIS_HARDWARE_REVISION_STRING_LEN_MAX                                    (32)
#define BLE_CFG_DIS_SOFTWARE_REVISION_STRING_LEN_MAX                                    (32)
#define BLE_CFG_DIS_MANUFACTURER_NAME_STRING_LEN_MAX                                    (32)
#define BLE_CFG_DIS_IEEE_CERTIFICATION_LEN_MAX                                          (32)
#define BLE_CFG_DIS_PNP_ID_LEN_MAX                                                      (7)

/******************************************************************************
 * Heart Rate Service (HRS)
 ******************************************************************************/
#define BLE_CFG_HRS_NUMBER_OF_CUSTOM_CHAR    0 /**< Number of characteristic added by the user */

#define BLE_CFG_HRS_BODY_SENSOR_LOCATION_CHAR           1 /**< BODY SENSOR LOCATION CHARACTERISTIC */
#define BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG           1 /**< ENERGY EXTENDED INFO FLAG */
#define BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG             1 /**< Max number of RR interval values - Shall not be greater than 9 */


#endif /*__BLE_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
