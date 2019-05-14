/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
* File Name          : compiler.h
* Author             : AMS - HEA&RF BU
* Version            : V1.0.0
* Date               : 19-July-2012
* Description        : Compiler-dependent macros.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
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
