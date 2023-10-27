/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
* File Name          : compiler.h
* Author             : AMS - HEA&RF BU
* Version            : V1.0.0
* Date               : 19-July-2012
* Description        : Compiler-dependent macros.
********************************************************************************
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*******************************************************************************/

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifdef __ICCARM__
#define PACKED
#else
#ifdef __GNUC__
#define __packed __attribute__((__packed__))
#define PACKED __attribute__((packed))
#else
#ifdef __CC_ARM
#define PACKED
#else
#error Neither ICCARM nor GNUC C detected. Define your "packed" macro.
#define PACKED
#define __packed
#endif
#endif
#endif

/* Change this define to 1 if zero-length arrays are not supported by your compiler. */
#ifndef __CC_ARM
#define VARIABLE_SIZE 0
#else
#define VARIABLE_SIZE 1
#endif

#endif /* DOXYGEN_SHOULD_SKIP_THIS */
