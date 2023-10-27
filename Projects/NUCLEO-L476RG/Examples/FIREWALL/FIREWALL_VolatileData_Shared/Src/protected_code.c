/**
  ******************************************************************************
  * @file    FIREWALL/FIREWALL_VolatileData_Shared/Src/protected_code.c
  * @author  MCD Application Team
  * @brief   Definition of functions to be located in Firewall protected code area (in FLASH)
  *******************************************************************************  
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
#include "main.h"


extern uint32_t protected_non_volatile_array[N];
extern uint32_t protected_volatile_array[N];




/* function in protected segment, direct access to this
   function follows call gate sequence since *.icf is written
   to locate it at the proper address: protected code segment start + 4:
   
   Firewall is configured as follows:
   fw_init.CodeSegmentStartAddress      = 0x08010000;
   
   The lines below are extracted from IAR scatter file stm32l476xx_flash.icf.  
   The scatter file places protected_function() at = 0x08010004:
      ...
      define symbol __ICFEDIT_protected_region_ROM_start__    = 0x08010004;
      define symbol __ICFEDIT_protected_region_ROM_end__      = 0x080101FF;    
      ...
      define region protected_ROM_region      = mem:[from __ICFEDIT_protected_region_ROM_start__   to __ICFEDIT_protected_region_ROM_end__];
      ...
      place in  protected_ROM_region {readonly object protected_code.o};
     */

#if defined(__GNUC__)
void __attribute__((section(".nvdata_protected_code"))) protected_function(void)
#else
void protected_function(void)
#endif  
{
  uint32_t i = 1;
  uint32_t j = 0;
                                   

  /* In case of protected_function() second call, during main() second run(),
     VDS bit is reset: protected volatile data are no more shared with 
     non-protected code */
  if (READ_REG(RTC->BKP4R) == 2)
  {
    __HAL_FIREWALL_VOLATILEDATA_SHARED_DISABLE();
  }


  /* Application processing filling up the array located
    in the protected volatile data segment */
   
   for (j=0; j<N; j++)
   {
     protected_volatile_array[j] += (2*i+3*j + protected_non_volatile_array[j]) ;
   }
   
   /* Set FPA bit for a proper closure of the Firewall when exiting the
      protected function */
   __HAL_FIREWALL_PREARM_ENABLE();
   
   return;

}






