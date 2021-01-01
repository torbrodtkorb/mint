@ Starup routine for the bootloader

@use both arm and thumb
.syntax unified
.cpu cortex-a5
.arm

.section .arm_vetors, "ax"
arm_vectors:
    ldr pc, =reset
    ldr pc, =undef
    ldr pc, =supervisor
    ldr pc, =prefetch_abort
    ldr pc, =data_abort
    .word 0
    ldr pc, =irq
    ldr pc, =fiq

.section .text

.extern _bss_s
.extern _bss_e
reset:
    ldr r4, =0x01
    mov r5, #0x03
    
    add r6, r5, r4
    ldr r7, =_bss_s
    ldr r8, =_bss_e
    b main

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
