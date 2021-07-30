/**
  * Image 0 linker script file
  */

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(IMAGE_0_RAM) + LENGTH(IMAGE_0_RAM);	/* end of "IMAGE_0_RAM" Ram type memory */

_SRAM_Origin = 0x20000000;
_SRAM_End = 0x20000000 + 128K;
_Flash_Origin = 0x8000000;
_Flash_End = 0x8000000 + 1024K;

_Min_Heap_Size = 0;	/* required amount of heap  */
_Min_Stack_Size = 0x400;	/* required amount of stack */

/* Image 0 */
_Image_0_Flash_Origin = 0x08010000;
_Image_0_Flash_Size = 192K; /* sector 4, 5 */
_Image_0_SRAM_Origin = _SRAM_Origin + 16K;
_Image_0_SRAM_Size = 16K; 

/* Memories definition */
MEMORY
{
  IMAGE_0_RAM    (xrw)    : ORIGIN = _Image_0_SRAM_Origin,   LENGTH = _Image_0_SRAM_Size
  IMAGE_0_FLASH    (rx)    : ORIGIN = _Image_0_Flash_Origin,   LENGTH = _Image_0_Flash_Size
}

/* Sections */
SECTIONS
{
  .img_header :
  {
    . = ALIGN(4);
    KEEP(*(.img_header))
    . = ALIGN(4);
  } >IMAGE_0_FLASH

  /* The startup code into "IMAGE_0_FLASH" Rom type memory */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >IMAGE_0_FLASH

  /* The program code and other data into "IMAGE_0_FLASH" Rom type memory */
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
  } >IMAGE_0_FLASH
  
  
  /* Constant data into "IMAGE_0_FLASH" Rom type memory */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >IMAGE_0_FLASH

  .ARM.extab   : {
    . = ALIGN(4);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
  } >IMAGE_0_FLASH

  .ARM : {
    . = ALIGN(4);
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(4);
  } >IMAGE_0_FLASH

  .preinit_array     :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);
  } >IMAGE_0_FLASH

  .init_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(4);
  } >IMAGE_0_FLASH

  .fini_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(4);
  } >IMAGE_0_FLASH

  /* Used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections into "IMAGE_0_RAM" Ram type memory */
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

  } >IMAGE_0_RAM AT> IMAGE_0_FLASH

  .eimage :
  {
    . = ALIGN(4);
    _Image_0_Flash_End = .;
    . = ALIGN(4);
  } >IMAGE_0_FLASH AT> IMAGE_0_FLASH

  /* Uninitialized data section into "IMAGE_0_RAM" Ram type memory */
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
  } >IMAGE_0_RAM AT> IMAGE_0_RAM

  /* User_heap_stack section, used to check that there is enough "IMAGE_0_RAM" Ram  type memory left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >IMAGE_0_RAM AT> IMAGE_0_RAM

  /* Remove information from the compiler libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
