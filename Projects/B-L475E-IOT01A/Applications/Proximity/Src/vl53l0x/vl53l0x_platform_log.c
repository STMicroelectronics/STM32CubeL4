/**
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
  * Programming Golden Rule: Keep it Simple!
  *
  */

/*!
 * \file   VL53L0X_platform_log.c
 * \brief  Code function definitions for Ewok Platform Layer
 *
 */


#include <stdio.h>    // sprintf(), vsnprintf(), printf()
#include <stdarg.h>   // sprintf(), vsnprintf(), printf()
#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#include "vl53l0x_def.h"
#include "vl53l0x_platform_log.h"


uint32_t _trace_level = TRACE_LEVEL_WARNING;
uint32_t _trace_modules = TRACE_MODULE_NONE;
uint32_t _trace_functions = TRACE_FUNCTION_NONE;

#if defined(VL53L0X_LOG_ENABLE) && (TRACE_UART==1)
int32_t VL53L0X_trace_config(char *filename, uint32_t modules, uint32_t level, uint32_t functions)
{
    int STATUS = 0;

    _trace_functions = functions;
    _trace_level = level;
    _trace_modules = modules;

    return STATUS;
}


void trace_print_module_function(uint32_t module, uint32_t level, uint32_t function, const char *format, ...)
{
    if ( ((level <=_trace_level) && ((module & _trace_modules) > 0))
        || ((function & _trace_functions) > 0) )
    {
    	va_list arg_list;
        va_start(arg_list, format);
        trace_vprintf( format, arg_list);
        va_end(arg_list);
    }
}
#endif
