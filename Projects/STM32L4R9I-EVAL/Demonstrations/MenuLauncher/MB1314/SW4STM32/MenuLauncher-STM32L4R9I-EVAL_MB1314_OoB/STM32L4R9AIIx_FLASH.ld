/*
*****************************************************************************
**

**  File        : LinkerScript.ld
**
**  Abstract    : Linker script for STM32L4R9AIIx Device with
**                34816KByte FLASH, 10880KByte RAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**
**  Distribution: The file is distributed as is, without any warranty
**                of any kind.
**
**  (c)Copyright Ac6.
**  You may use this file as-is or modify it according to the needs of your
**  project. Distribution of this file (unmodified or modified) is not
**  permitted. Ac6 permit registered System Workbench for MCU users the
**  rights to distribute the assembled, compiled & linked contents of this
**  file as part of an application binary file, provided that it is built
**  using the System Workbench for MCU toolchain.
**
*****************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = 0x200A0000;    /* end of RAM */
/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x2000;    
_Min_Stack_Size = 0x1000; 

/* Specify the memory areas */
MEMORY
{
/* FLASH_DL (rx)        : ORIGIN = 0x08000000, LENGTH = 24K */
FLASH_ML1 (rx)       : ORIGIN = 0x08008000, LENGTH = 236K
/* while_loop (rx)      : ORIGIN = 0x08007000, LENGTH = 19K */
/* FLASH_ML2 (rx)       : ORIGIN = 0x08043000, LENGTH = 244K */
/* FLASH_STEW (rx)      : ORIGIN = 0x08080000, LENGTH = 512K */
/* FLASH_TGFX (rx)      : ORIGIN = 0x08100000, LENGTH = 512K */
/* FLASH_EWIZ (rx)      : ORIGIN = 0x08180000, LENGTH = 512K */
/* ExtFLASH (rx)        : ORIGIN = 0x80000000, LENGTH = 32768K */
RAM (xrw)            : ORIGIN = 0x20000000, LENGTH = 640K
/* PSRAM (xrw)          : ORIGIN = 0x60000000, LENGTH = 8192K */
/* ExtSRAM (xrw)        : ORIGIN = 0x70000000, LENGTH = 2048K */
/* OSPI_ML1 (rx)        : ORIGIN = 0x90000000, LENGTH = 2240K */
/* OSPI_ML2 (rx)        : ORIGIN = 0x90230000, LENGTH = 1856K */
OSPI_STEW (rx)       : ORIGIN = 0x90400000, LENGTH = 20480K
/* OSPI_TGFX (rx)       : ORIGIN = 0x91800000, LENGTH = 20480K */
/* OSPI_EWIZ (rx)       : ORIGIN = 0x92C00000, LENGTH = 167936K */
}

/* Define output sections */
SECTIONS
{
  /* The startup code goes first into FLASH */
  .isr_vector 0x08000000:
  {
    . = ALIGN(8);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(8);
  }

  .while_loop 0x08007000: 
  {
  	. = ALIGN(8);
  	*(While1Section)
  	. = ALIGN(8);
  } 
  
  /* The program code and other data goes into FLASH */
  .text :
  {
    . = ALIGN(8);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(8);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH_ML1

  /* Constant data goes into FLASH */
  .rodata :
  {
    . = ALIGN(8);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(8);
  } >FLASH_ML1

  .ARM.extab   : 
  { 
  . = ALIGN(8);
  *(.ARM.extab* .gnu.linkonce.armextab.*)
  . = ALIGN(8);
  } >FLASH_ML1
  
  .ARM : {
	. = ALIGN(8);
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
	. = ALIGN(8);
  } >FLASH_ML1

  .preinit_array     :
  {
	. = ALIGN(8);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
	. = ALIGN(8);
  } >FLASH_ML1
  
  .init_array :
  {
	. = ALIGN(8);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
	. = ALIGN(8);
  } >FLASH_ML1
  
  .fini_array :
  {
	. = ALIGN(8);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
	. = ALIGN(8);
  } >FLASH_ML1

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Uninitialized data section */
  . = ALIGN(4);
  
  .bss (NOLOAD):
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(.FrameBufferSection*)
    *(.STemWinMemPool*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data : 
  {
    . = ALIGN(8);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(8);
    _edata = .;        /* define a global symbol at data end */
  } >RAM AT> FLASH_ML1

 .FreeRTOSHeapSection : {
    *(.FreeRTOSHeapSection)
  } >RAM 
    
  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM

  

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

    .ARM.attributes 0 : { *(.ARM.attributes) }
    .ExtFlashSection : {*(.ExtFlashSection)} >OSPI_STEW
}



