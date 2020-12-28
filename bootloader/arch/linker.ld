/* RAM linker script for the Mint bootloader */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)

MEMORY {
    ram (rwx) : ORIGIN = 0x00200000, LENGTH = 0x10000 /* 64 KiB internal SRAM */
}

USER_STACK = 1024;
FIQ_STACK = 64;
IRQ_STACK = 64;
ABORT_STACK = 64;
SVC_STACK = 64;
UNDEF_STACK = 64;

SECTIONS {

    /* We place the code first */
    .text : {
        . = ALIGN(4);
        _text_s = .;

        *(.text)
        *(.text.*)
        *(.glue_7)
        *(.glue_7t)

        . = ALIGN(4);
        KEEP(*(.init))
        KEEP(*(.fini))

        . = ALIGN(4);
        _text_e = .;

    } > ram

    /* Black C magic */
    .ARM.extab : {
        *(.ARM.extab .gnu.linkonce.armextab.*)
    } > ram

    .ARM.exidx : {
        *(.ARM.exidx .gnu.linkonce.armexidx.)
    } > ram

    .preinit_array : {
        KEEP(*(.preinit_array))
    } > ram

    .init_array : {
        KEEP(*(SORT(.init_array.)))
        KEEP(*(.init_array))
    } > ram

    .fini_array : {
        KEEP(*(.fini_array))
        KEEP(*(SORT(.fini_array.)))
    } > ram

    .rodata : {
        . = ALIGN(4);
        _rodata_s = .;
        KEEP(*(.rodata))
        KEEP(*(.rodata.*))
        . = ALIGN(4);
        _rodata_e = .;
    } > ram

    .data : {
        . = ALIGN(4);
        _data_s = .;
        KEEP(*(.data))
        KEEP(*(.data.*))
        . = ALIGN(4);
        KEEP(*(.ramfunc))
        KEEP(*(.ramfunc.*))

        . = ALIGN(4);
        _data_e = .;
    } > ram

    .bss (NOLOAD) : {
        . = ALIGN(4);
        _bss_s = .;
        *(.bss)
        *(.bss.*)
        . = ALIGN(4);
        _bss_e = .;
    } > ram

    .stack (NOLOAD) : {
        . = ALIGN(8);
        . = . + USER_STACK;
        _user_stack_e = .;
        
        . = ALIGN(8);
        . = . + FIQ_STACK;
        _fiq_stack_e = .;

        . = ALIGN(8);
        . = . + IRQ_STACK;
        _irq_stack_e = .;

        . = ALIGN(8);
        . = . + ABORT_STACK;
        _abort_stack_e = .;

        . = ALIGN(8);
        . = . + SVC_STACK;
        _svc_stack_e = .;
       
        . = ALIGN(8);
        . = . + UNDEF_STACK;
        _undef_stack_e = .;

    } > ram
}

end = .;