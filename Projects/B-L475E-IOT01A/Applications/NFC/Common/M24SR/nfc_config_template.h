/**
  ******************************************************************************
  * @file    nfc_config_template.h
  * @author  MCD Application Team
  * @brief   This file for basic configuration settings
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
#ifndef __NFC_CONFIG_H
#define __NFC_CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup NFC_CONFIG
  * @{
  */

/** @defgroup NFC_CONFIG_Exported_Defines  NFC_CONFIG Exported Defines
  * @{
  */


/**
  * @brief  M24SR_EEPROM_Size_Version
  */
#define M24SR04_NDEF_MAX_SIZE                0x200
#define M24SR16_NDEF_MAX_SIZE                0x800
#define M24SR64_NDEF_MAX_SIZE                0x2000
/* The maximum size of a NDEF will be 64kBits with M24SR64 */
/* if smaller memory used update this define to save space */
#define NFC_TT4_NDEF_MAX_SIZE          M24SR16_NDEF_MAX_SIZE

/**
  * @brief  M24SR_GPO_Config_Mode
  */
#define M24SR_I2C_GPO_POLLING   0 /* Normal  I²C polling */
#define M24SR_I2C_GPO_SYNCHRO   1 /* allow to use GPO polling as I2C synchronization */ 
#define M24SR_I2C_GPO_INTERRUPT 2 /* allow to use GPO interrupt as I2C synchronization */ 
/* The maximum size of a NDEF will be 64kBits with M24SR64 */
/* if smaller memory used update this define to save space */
#define M24SR_I2C_GPO_MODE            M24SR_I2C_GPO_INTERRUPT
  
/**
  * @}
  */
  
/**
  * @}
  */  
  
#ifdef __cplusplus
}
#endif


#endif /* __NFC_CONFIG_H */
 
/******************* (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
