/*
*****************************************************************************
**

**  File        : stm32_flash.ld
**
**  Abstract    : Linker script for STM32L476ZG Device with
**                1024KByte FLASH, 96KByte RAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**  Environment : Atollic TrueSTUDIO(R)
**
**  Distribution: The file is distributed as is, without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. This file may only be built (assembled or compiled and linked)
**  using the Atollic TrueSTUDIO(R) product. The use of this file together
**  with other tools than Atollic TrueSTUDIO(R) is not permitted.
**
*****************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = 0x20017FFF;    /* end of RAM */

/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x400;      /* required amount of heap  */
_Min_Stack_Size = 0x400;     /* required amount of stack */

/* Specify the memory areas */
/*
** Note! MEMORY_B1 is only needed if system requires some code/data to be 
** placed at special memory location. MEMORY_B1 specification can be removed if 
** not needed. In that case also remove MEMORY_B1 section at the end of this 
** file. Additional memory sections can also be added here.   
*/
MEMORY
{
  FLASH (rx)      : ORIGIN = 0x8000000, LENGTH = 1024K
  RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 96K
/****************************************************************************/
/*                                                                          */
/*   VDATA_PROTECTED_CODE: used to locate code in protected code segment    */
/*                                                                          */
/****************************************************************************/
/* Note that the protected code segment doesn't exactly start at the 
   starting address used to configure de Firewall.
   This allows to locate the function used to open the Firewall at the
   correct address to obey the call gate sequence */
  VDATA_PROTECTED_CODE (rx)   : ORIGIN = 0x2000F104, LENGTH = 3324
/***************************************************************************/
/*                                                                         */
/*    VDATA_PROTECTED_DATA: used to locate data in volatile data segment   */
/*                                                                         */
/***************************************************************************/    
  VDATA_PROTECTED_DATA (xrw)  : ORIGIN = 0x2000FE00, LENGTH = 512
}

/* Define output sections */
SECTIONS
{
  /* The startup code goes first into RAM */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >RAM

  /* The program code and other data goes into RAM */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >RAM

  /* Constant data goes into RAM */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >RAM

   .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >RAM
    .ARM : {
    __exidx_start = .;
      *(.ARM.exidx*)
      __exidx_end = .;
    } >RAM

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >RAM
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >RAM
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >RAM

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);
  
  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >RAM

  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM

  PROVIDE ( end = _ebss );
  PROVIDE ( _end = _ebss );

  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack :
  {
    . = ALIGN(4);
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(4);
  } >RAM

  /*************************************************************************/   
  /*                                                                       */
  /* VDATA_PROTECTED_CODE section, code is located here explicitly         */
  /* refer to protected_vdata_code.c:                                      */
  /*                                                                       */    
  /* uint32_t __attribute__((section(".vdata_protected_code"))) vdata_protected_function(uint32_t * array) */
  /*                                                                       */
  /*************************************************************************/      
  .memory_vdata_protected_code :
  {
    *(.vdata_protected_code)        /* .vdata_protected_code sections (code) */
    *(.vdata_protected_code*)       /* .vdata_protected_code* sections (code)  */
  } >VDATA_PROTECTED_CODE


  /*************************************************************************/   
  /*                                                                       */
  /* VDATA_PROTECTED_DATA section, data is located here explicitly         */
  /* refer to protected_volatile_data.c:                                   */
  /* uint32_t __attribute__((section(".vdata_protected_data")))  protected_volatile_array[N] = {50, 51, 52, 53, 54, 55, 56, 57, 58, 59}; */
  /*                                                                       */
  /*************************************************************************/      
  .memory_vdata_protected_data :
  {
    *(.vdata_protected_data)        /* .vdata_protected_data sections (code) */
    *(.vdata_protected_data*)       /* .vdata_protected_data* sections (code)  */
  } >VDATA_PROTECTED_DATA


  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }
  
  .ARM.attributes 0 : { *(.ARM.attributes) }
}
