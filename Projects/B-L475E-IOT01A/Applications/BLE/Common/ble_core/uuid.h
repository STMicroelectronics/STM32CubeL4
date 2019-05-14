/**
 ******************************************************************************
 * @file    uuid.h
 * @author  MCD Application Team
 * @brief   Header containing the UUIDs of all the services and caharcteristics
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


#ifndef _UUID_H_
#define _UUID_H_

/* Descriptor UUIDs */
#define CHAR_EXTENDED_PROPERTIES_DESCRIPTOR_UUID                       (0x2900)
#define CHAR_USER_DESCRIPTION_DESCRIPTOR_UUID                          (0x2901)
#define CLIENT_CHAR_CONFIG_DESCRIPTOR_UUID                             (0x2902)
#define SERVER_CHAR_CONFIG_DESCRIPTOR_UUID                             (0x2903)
#define CHAR_PRESENTATION_FORMAT_DESCRIPTOR_UUID                       (0x2904)
#define CHAR_AGGREGATE_FORMAT_DESCRIPTOR_UUID                          (0x2905)
#define VALID_RANGE_DESCRIPTOR_UUID                                    (0x2906)
#define EXTERNAL_REPORT_REFERENCE_DESCRIPTOR_UUID                      (0x2907)
#define REPORT_REFERENCE_DESCRIPTOR_UUID                               (0x2908)
#define ES_CONFIGURATION_DESCRIPTOR_UUID                               (0x290B)
#define ES_MEASUREMENT_DESCRIPTOR_UUID                                 (0x290C)
#define ES_TRIGGER_SETTING_DESCRIPTOR_UUID                             (0x290D)

/* UUIDs of immediate alert service */
#define IMMEDIATE_ALERT_SERVICE_UUID                                   (0x1802)
#define ALERT_LEVEL_CHARACTERISTIC_UUID                                (0x2A06)

/* UUIDs for Link Loss service */
#define LINK_LOSS_SERVICE_UUID                                         (0x1803)
#define LINK_LOSS_ALERT_LEVEL_CHARACTERISTIC_UUID                      (0x2A06)

/* UUIDs for TX Power service */
#define TX_POWER_SERVICE_UUID                                          (0x1804)
#define TX_POWER_LEVEL_CHARACTERISTIC_UUID                             (0x2A07)

/* UUIDs for Time service */
#define CURRENT_TIME_SERVICE_UUID                                      (0x1805)
#define CURRENT_TIME_CHAR_UUID                                         (0x2A2B)
#define LOCAL_TIME_INFORMATION_CHAR_UUID                               (0x2A0F)
#define REFERENCE_TIME_INFORMATION_CHAR_UUID                           (0x2A14)

/* UUIDs for Reference Time Update service */
#define REFERENCE_UPDATE_TIME_SERVICE_UUID                             (0x1806)
#define TIME_UPDATE_CONTROL_POINT_CHAR_UUID                            (0x2A16)
#define TIME_UPDATE_STATE_CHAR_UUID                                    (0x2A17)

/* UUIDs for Next DST Change service */
#define NEXT_DST_CHANGE_SERVICE_UUID                                   (0x1807)
#define TIME_WITH_DST_CHAR_UUID                                        (0x2A11)

/* UUIDs for glucose profile */
#define GLUCOSE_SERVICE_UUID                                           (0x1808)
#define GLUCOSE_MEASUREMENT_CHAR_UUID                                  (0x2A18)
#define GLUCOSE_MEASUREMENT_CONTEXT_CHAR_UUID                          (0x2A34)
#define GLUCOSE_FEATURE_CHAR_UUID                                      (0x2A51)
/* Record Access Control Point (RACP) */
#define GLUCOSE_RACP_CHAR_UUID                                         (0x2A52)

/* UUIDs for health thermometer profile */
#define HEALTH_THERMOMETER_SERVICE_UUID                                (0x1809)
#define TEMPERATURE_MEASUREMENT_CHAR_UUID                              (0x2A1C)
#define TEMPERATURE_TYPE_CHAR_UUID                                     (0x2A1D)
#define INTERMEDIATE_TEMPERATURE_CHAR_UUID                             (0x2A1E)
#define MEASUREMENT_INTERVAL_CHAR_UUID                                 (0x2A21)

/* UUIDs for Device Information Service */
#define DEVICE_INFORMATION_SERVICE_UUID                                (0x180A)
#define SYSTEM_ID_UUID                                                 (0x2A23)
#define MODEL_NUMBER_UUID                                              (0x2A24)
#define SERIAL_NUMBER_UUID                                             (0x2A25)
#define FIRMWARE_REVISION_UUID                                         (0x2A26)
#define HARDWARE_REVISION_UUID                                         (0x2A27)
#define SOFTWARE_REVISION_UUID                                         (0x2A28)
#define MANUFACTURER_NAME_UUID                                         (0x2A29)
#define IEEE_CERTIFICATION_UUID                                        (0x2A2A)
#define PNP_ID_UUID                                                    (0x2A50)

/* UUIDs for Heart Rate Service */
#define HEART_RATE_SERVICE_UUID                                        (0x180D)
#define CLIENT_CHARACTERISTIC_CONFIG_DESCRIPTOR_UUID                   (0x2902)
#define HEART_RATE_MEASURMENT_UUID                                     (0x2A37)
#define SENSOR_LOCATION_UUID                                           (0x2A38)
#define CONTROL_POINT_UUID                                             (0x2A39)

/* UUIDs for Phone Alert status profile */
#define PHONE_ALERT_SERVICE_UUID                                       (0x180E)
#define PHONE_ALERT_STATUS_CHARAC_UUID                                 (0x2A3F)
#define RINGER_CNTRL_POINT_CHARAC_UUID                                 (0x2A40)
#define RINGER_SETTING_CHARAC_UUID                                     (0x2A41)

/* UUIDs for battery service */ 
#define BATTERY_SERVICE_UUID                                           (0x180F)
#define BATTERY_LEVEL_CHAR_UUID                                        (0x2A19)

/* UUIDs for Blood Pressure profile */
#define BLOOD_PRESSURE_SERVICE_UUID                                    (0x1810)
#define BLOOD_PRESSURE_MEASUREMENT_CHAR_UUID                           (0x2A35)
#define INTERMEDIATE_CUFF_PRESSURE_CHARAC_UUID                         (0x2A36)
#define BLOOD_PRESSURE_FEATURE_CHARAC                                  (0x2A49)

/* UUIDs for alert notification profile */
#define ALERT_NOTIFICATION_SERVICE_UUID                                (0x1811)
#define SUPPORTED_NEW_ALERT_CATEGORY_CHAR_UUID                         (0x2A47)
#define NEW_ALERT_CHAR_UUID                                            (0x2A46)
#define SUPPORTED_UNREAD_ALERT_CATEGORY_CHAR_UUID                      (0x2A48)
#define UNREAD_ALERT_STATUS_CHAR_UUID                                  (0x2A45)
#define ALERT_NOTIFICATION_CONTROL_POINT_CHAR_UUID                     (0x2A44)

/* UUIDs for human interface device */
#define HUMAN_INTERFACE_DEVICE_SERVICE_UUID                            (0x1812)
#define PROTOCOL_MODE_CHAR_UUID                                        (0x2A4E)
#define REPORT_CHAR_UUID                                               (0x2A4D)
#define REPORT_MAP_CHAR_UUID                                           (0x2A4B)
#define BOOT_KEYBOARD_INPUT_REPORT_CHAR_UUID                           (0x2A22)
#define BOOT_KEYBOARD_OUTPUT_REPORT_CHAR_UUID                          (0x2A32)
#define BOOT_MOUSE_INPUT_REPORT_CHAR_UUID                              (0x2A33)
#define HID_INFORMATION_CHAR_UUID                                      (0x2A4A)
#define HID_CONTROL_POINT_CHAR_UUID                                    (0x2A4C)

/* UUIDs for scan parameter service */
#define SCAN_PARAMETER_SERVICE_UUID                                    (0x1813)
#define SCAN_INTERVAL_WINDOW_CHAR_UUID                                 (0x2A4F)
#define SCAN_REFRESH_CHAR_UUID                                         (0x2A31)

/* UUIDs for location and navigation device */
#define LOCATION_NAVIGATION_SERVICE_UUID                               (0x1819)
#define LN_FEATURE_UUID                                                (0x2A6A)
#define LOCATION_SPEED_UUID                                            (0x2A67)
#define POSITION_QUALITY_UUID                                          (0x2A69)
#define LN_CONTROL_POINT_UUID                                          (0x2A6B)
#define NAVIGATION_UUID                                                (0x2A68)

/* UUIDs for environmental sensing profile */
#define ENVIRONMENTAL_SENSING_SERVICE_UUID                             (0x181A)
#define DESCRIPTOR_VALUE_CHANGED_UUID                                  (0x2A7D)
#define APPARENT_WIND_DIRECTION_UUID                                   (0x2A73)
#define APPARENT_WIND_SPEED_UUID                                       (0x2A72)
#define DEW_POINT_UUID                                                 (0x2A7B)
#define ELEVATION_UUID                                                 (0x2A6C)
#define GUST_FACTOR_UUID                                               (0x2A74)
#define HEAT_INDEX_UUID                                                (0x2A7A)
#define HUMIDITY_UUID                                                  (0x2A6F)
#define IRRADIANCE_UUID                                                (0x2A77)
#define POLLEN_CONCENTRATION_UUID                                      (0x2A75)
#define RAINFALL_UUID                                                  (0x2A78)
#define PRESSURE_UUID                                                  (0x2A6D)
#define TEMPERATURE_UUID                                               (0x2A6E)
#define TRUE_WIND_DIRECTION_UUID                                       (0x2A71)
#define TRUE_WIND_SPEED_UUID                                           (0x2A70)
#define UV_INDEX_UUID                                                  (0x2A76)
#define WIND_CHILL_UUID                                                (0x2A79)
#define BAROMETRIC_PRESSURE_TREND_UUID                                 (0x2AA3)
#define MAGNETIC_DECLINATION_UUID                                      (0x2A2C)
#define MAGNETIC_FLUX_DENSITY_2D_UUID                                  (0x2AA0)
#define MAGNETIC_FLUX_DENSITY_3D_UUID                                  (0x2AA1)
   
/* UUIDs for body composition service */
#define BODY_COMPOSITION_SERVICE_UUID                                  (0x181B)
#define BODY_COMPOSITION_MEASUREMENT_CHAR_UUID                         (0x2A9C)
#define BODY_COMPOSITION_FEATURE_CHARAC                                (0x2A9B)      
   
/* UUIDs for weight scale profile */
#define WEIGHT_SCALE_SERVICE_UUID                                      (0x181D)
#define WEIGHT_SCALE_MEASUREMENT_CHAR_UUID                             (0x2A9D)
#define WEIGHT_SCALE_FEATURE_CHARAC                                    (0x2A9E)  
   
/* UUIDs for Internet Support Service */
#define INTERNET_SUPPORT_SERVICE_UUID                                  (0x1820)
   
/* UUIDs for HTTP proxy Service */
#define HTTP_PROXY_SERVICE_UUID                                        (0x1823)
#define HTTP_URI_CHAR_UUID                                             (0x2AB6)
#define HTTP_HEADERS_CHAR_UUID                                         (0x2AB7)
#define HTTP_STATUS_CODE_CHAR_UUID                                     (0x2AB8)
#define HTTP_ENTITY_BODY_CHAR_UUID                                     (0x2AB9)
#define HTTP_CONTROL_POINT_CHAR_UUID                                   (0x2ABA)
#define HTTP_SECURITY_CHAR_UUID                                        (0x2ABB)
   
/* Custom Services*/
/* UUIDs for data transfer service */
#define DATA_TRANSFER_SERVICE_UUID                                     (0xF2E0)
#define DATA_TRANSFER_TX_CHAR_UUID                                     (0xF2E1)
#define DATA_TRANSFER_RX_CHAR_UUID                                     (0xF2E2)    

/* UUIDs for custom battery service */ 
#define CUSTOM_BATTERY_SERVICE_UUID                                    (0xF2F0)
#define CUSTOM_BATTERY_LEVEL_CHAR_UUID                                 (0xF2F1)

/* Custom Services*/
/* UUIDs for data transfer service */
#define LED_BUTTON_SERVICE_UUID                                         (0x1A30)
#define LED_CHAR_UUID                                                   (0x2B50)
#define BUTTON_CHAR_UUID                                                (0x2B51)    
 
/*UUIDs for End Device Management Service*/
#define END_DEVICE_MGT_SERVICE_UUID                                     (0x1A40)
#define END_DEVICE_STATUS_CHAR_UUID                                     (0x2B60)   


#endif  /* _UUID_H_ */
