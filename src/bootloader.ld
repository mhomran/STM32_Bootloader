/**
 ******************************************************************************
 * @file      LinkerScript.ld
 * @author    Auto-generated by STM32CubeIDE
 * @brief     Linker script for STM32F407VGTx Device from STM32F4 series
 *                      1024Kbytes FLASH
 *                      64Kbytes CCMRAM
 *                      128Kbytes RAM
 *
 *            Set heap size, stack size and stack location according
 *            to application requirements.
 *
 *            Set memory bank area and size if external memory is used
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(BOOT_RAM) + LENGTH(BOOT_RAM);	/* end of "BOOT_RAM" Ram type memory */

_SRAM_Origin = 0x20000000;
_SRAM_End = 0x20000000 + 128K;
_Flash_Origin = 0x8000000;
_Flash_End = 0x8000000 + 1024K;

_Min_Heap_Size = 0;	/* required amount of heap  */
_Min_Stack_Size = 0x400;	/* required amount of stack */

/* Bootloader */
_Bootloader_Flash_Origin = _Flash_Origin; 
_Bootloader_Flash_Size = 48K; /* sector 0:2 */
_Bootloader_SRAM_Origin = _SRAM_Origin; 
_Bootloader_SRAM_Size = 16K;

/* STM32CubeHAL */
_HAL_Flash_Origin = 0x0800C000; 
_HAL_Flash_Size = 16K; /* sector 3 */

/* Application */
_App_Flash_Start = 0x08010000;
_App_Flash_End = 0x080FFFFF;

/* Image 0 */
_Image_0_Flash_Origin = 0x08010000;
_Image_0_Flash_Size = 192K; /* sector 4, 5 */
_Image_0_SRAM_Origin = _SRAM_Origin + 16K;
_Image_0_SRAM_Size = 16K; 

/* Image 1 */
_Image_1_Flash_Origin = 0x08040000;
_Image_1_Flash_Size = 256K; /* sector 6, 7 */
_Image_1_SRAM_Origin = _Image_0_SRAM_Origin + 16K;
_Image_1_SRAM_Size = 16K; 

/* Image 2 */
_Image_2_Flash_Origin = 0x08080000;
_Image_2_Flash_Size = 256K; /* sector 8, 9 */
_Image_2_SRAM_Origin = _Image_1_SRAM_Origin + 16K;
_Image_2_SRAM_Size = 16K; 

/* Image 3 */
_Image_3_Flash_Origin = 0x080C0000;
_Image_3_Flash_Size = 256K; /* sector 10, 11 */
_Image_3_SRAM_Origin = _Image_2_SRAM_Origin + 16K;
_Image_3_SRAM_Size = 16K; 

/* Memories definition */
MEMORY
{
  BOOT_RAM    (xrw)    : ORIGIN = _Bootloader_SRAM_Origin,   LENGTH = _Bootloader_SRAM_Size
  BOOT_FLASH    (rx)    : ORIGIN = _Bootloader_Flash_Origin,   LENGTH = _Bootloader_Flash_Size
  HAL_FLASH (rx)        : ORIGIN = _HAL_Flash_Origin, LENGTH = _HAL_Flash_Size
}

/* Sections */
SECTIONS
{
  /* The startup code into "BOOT_FLASH" Rom type memory */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >BOOT_FLASH

  /* The program code and other data into "BOOT_FLASH" Rom type memory */
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
  } >BOOT_FLASH
  
  
  /* Constant data into "BOOT_FLASH" Rom type memory */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >BOOT_FLASH

  .ARM.extab   : {
    . = ALIGN(4);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
  } >BOOT_FLASH

  .ARM : {
    . = ALIGN(4);
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(4);
  } >BOOT_FLASH

  .preinit_array     :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);
  } >BOOT_FLASH

  .init_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(4);
  } >BOOT_FLASH

  .fini_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(4);
  } >BOOT_FLASH

  /* Used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections into "BOOT_RAM" Ram type memory */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    *(.RamFunc)        /* .RamFunc sections */
    *(.RamFunc*)       /* .RamFunc* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */

  } >BOOT_RAM AT> BOOT_FLASH

  .hal :
  {
    . = ALIGN(4);
    KEEP(*(.hal))
    . = ALIGN(4);
  } > HAL_FLASH AT> HAL_FLASH

  /* Uninitialized data section into "BOOT_RAM" Ram type memory */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.igot.plt)       /* The added section */
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >BOOT_RAM AT> BOOT_RAM

  /* User_heap_stack section, used to check that there is enough "BOOT_RAM" Ram  type memory left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >BOOT_RAM AT> BOOT_RAM

  /* Remove information from the compiler libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
