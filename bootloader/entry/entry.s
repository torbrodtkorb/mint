@ Starup routine for the bootloader

@use both arm and thumb
.syntax unified
.cpu cortex-a5
.arm

.section .arm_vectors, "ax"
arm_vectors:
    ldr pc, =reset
    ldr pc, =undef
    ldr pc, =supervisor
    ldr pc, =prefetch_abort
    ldr pc, =data_abort
    .word 0
    ldr pc, =irq
    ldr pc, =fiq

// This is defined in the linker script
.extern _user_stack_e
.extern _fig_stack_e
.extern _irq_stack_e
.extern _abort_stack_e
.extern _undef_stack_e
.extern _svc_stack_e

//This defines the start and stop adress for the .bss segment
.extern _bss_s
.extern _bss_e

// Define for the different modes
USER   = 0b10011
FIQ    = 0b10001
IRQ    = 0b10010
ABORT  = 0b10111
UNDEF  = 0b11011
SYSTEM = 0b11111

.section .text

// This is the entry point of the bootloader. Posessor always boots in SVC mode
// with all interrupts disabled.
reset:
    // Set up stacks for all the operating mode. We know that the processor
    // starts in the SVC mode
    ldr sp, =_svc_stack_e
    cps #UNDEF
    ldr sp, =_undef_stack_e
    cps #ABORT
    ldr sp, =_abort_stack_e
    cps #IRQ
    ldr sp, =_irq_stack_e
    cps #FIQ
    ldr sp, =_fiq_stack_e
    cps #SYSTEM
    ldr sp, =_user_stack_e

    // Clearing the .bss segment
    ldr r0, = _bss_e
    ldr r1, = _bss_s
    mov r2, #0
1:  cmp r0, r1
    strcc r2, [r0], #4
    bcc 1b

    // This is for initializing the c library
    bl __libc_init_array
    dsb
    isb

    bl main
    b .

undef:
    b .

supervisor:
    b .

prefetch_abort:
    b .

data_abort:
    b .

irq:
    b .

fiq:
    b .
