/**
  ******************************************************************************
  * @file    k_module.h
  * @author  MCD Application Team
  * @brief   Header for k_module.c file
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
#ifndef __K_MODULE_H
#define __K_MODULE_H

#ifdef __cplusplus
 extern "C" {
#endif

#undef GLOBAL
#ifndef KMODULE_C
#define GLOBAL extern
#else
#define GLOBAL
#endif   
   
/* Includes ------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
typedef enum {
  KMODULE_OK, 
  /* Module exec */
  KMODULE_ERROR_PRE,
  KMODULE_ERROR_EXEC,
  KMODULE_ERROR_POST,
  KMODULE_ERROR_ICON,
  KMODULE_RESMISSIG
} KMODULE_RETURN;

/* Exported types ------------------------------------------------------------*/
typedef struct
{
 uint8_t        kModuleId;
 KMODULE_RETURN (*kModulePreExec)(void);  
 KMODULE_RETURN (*kModuleExec)(void);
 KMODULE_RETURN (*kModulePostExec)(void);
 KMODULE_RETURN (*kModuleResourceCheck)(void);
} K_ModuleItem_Typedef;

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern K_ModuleItem_Typedef kmodule_info[MODULE_MAX];

/* Exported functions ------------------------------------------------------- */
GLOBAL void kModule_Init(void);
GLOBAL void kModule_Add(uint8_t kModuleId, K_ModuleItem_Typedef kModuleInfo);
GLOBAL KMODULE_RETURN kModule_CheckResource(void);
GLOBAL KMODULE_RETURN kModule_Execute(uint8_t kModuleId);
GLOBAL KMODULE_RETURN kModule_EnterLowPower(uint8_t kModuleId);
GLOBAL KMODULE_RETURN kModule_ExitLowPower(uint8_t kModuleId);

#ifdef __cplusplus
}
#endif

#endif /*__K_MODULE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
