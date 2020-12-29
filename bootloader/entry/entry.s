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

reset:
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
