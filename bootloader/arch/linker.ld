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
    .arm_vectors : {
        KEEP(*(.arm_vectors))
    } > ram

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
        . = . + USER_STACK;
        . = ALIGN(8);
        _user_stack_e = .;
        
        . = . + FIQ_STACK;
        . = ALIGN(8);
        _fiq_stack_e = .;

        . = . + IRQ_STACK;
        . = ALIGN(8);
        _irq_stack_e = .;

        . = . + ABORT_STACK;
        . = ALIGN(8);
        _abort_stack_e = .;

        . = . + SVC_STACK;
        . = ALIGN(8);
        _svc_stack_e = .;
       
        . = . + UNDEF_STACK;
        . = ALIGN(8);
        _undef_stack_e = .;

    } > ram
}

end = .;