/**
  ******************************************************************************
  * @file    FIREWALL/FIREWALL_VolatileData_Executable/Src/protected_vdata_code.c 
  * @author  MCD Application Team
  * @brief   Definition of function located in Firewall protected volatile data area (in SRAM1)
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
#include "main.h"

/* Data located in protected volatile data segment */
extern uint32_t protected_volatile_array[N];

/**
  * @brief  Protected function located in volatile data segment set as executable
  * @param array: input array of length N
  * @note  The API processes the input data in carrying out some computations
  *        resorting (for demonstration purposes) to data located in the
  *        protected data segment. 
  *        At the same time, the protected data are updated. 
  *          
  *        The Firewall protects the area from 0x2000F100 to  0x2000FFFF
  *        
  *          fw_init.VDataSegmentStartAddress    = 0x2000F100;
  *          fw_init.VDataSegmentLength          = 0xF00;
  *        
  *        The lines below are extracted from IAR scatter file stm32l476xx_sram.icf.          
  *        The scatter file locates vdata_protected_function in a sub-region starting at 0x2000F104. 
  *          ...
  *          define symbol __ICFEDIT_protected_region_SRAM1_a_start__    = 0x2000F104;
  *          define symbol __ICFEDIT_protected_region_SRAM1_a_end__      = 0x2000FDFF;
  *          ...
  *          define region protected_SRAM1_a_region    = mem:[from __ICFEDIT_protected_region_SRAM1_a_start__   to __ICFEDIT_protected_region_SRAM1_a_end__];
  *          ...
  *          place in  protected_SRAM1_a_region {readonly object protected_vdata_code.o};
  *          ...
  *            
  * @retval uint32_t value, result of function internal processing
  */
#if defined(__GNUC__)
uint32_t __attribute__((section(".vdata_protected_code"))) vdata_protected_function(uint32_t * array)
#else
uint32_t vdata_protected_function(uint32_t * array)
#endif  
{
  uint32_t i = 5;
  uint32_t j = 0;


    for (j=0; j<N; j++)
   {
     protected_volatile_array[j] += i+j*j + array[j];
   }
   
   i =   protected_volatile_array[N-1] + 11 * protected_volatile_array[0]; 
   

   /* Ensure no reset occurs when jumping to non-protected code */
   __HAL_FIREWALL_PREARM_ENABLE();
   
   return i;

}



