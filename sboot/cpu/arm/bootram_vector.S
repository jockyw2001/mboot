#include "boot.inc"

#if defined(CONFIG_ARMv8_64BIT_KERNEL) || defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
    .globl      vectors

vectors:
    @//Synchronous EL1t
    .align  7
    .word 0x14000000
    @//IRQ EL3t
    .align  7
    .word 0x14000000
    @//FIQ EL3t
    .align  7
    .word 0x14000000
    @//Error EL3t
    .align  7
    .word 0x14000000

    @//Synchronous EL3h
    .align  7
    .word 0x14000000
    @//IRQ EL3h
    .align  7
    .word 0x14000000
    @//FIQ EL3h
    .align  7
    .word 0x14000000
    @//Error EL3h
    .align  7
    .word 0x14000000

    @//Synchronous 64-bit EL2
    .align  7
    .word 0x14000000
    @//IRQ 64-bit EL2
    .align  7
    .word 0x14000000
    @//FIQ 64-bit EL2
    .align  7
    .word 0x14000000
    @//Error 64-bit EL2
    .align  7
    .word 0x14000000

    @//Synchronous 32-bit EL2
    .align  7
    .word 0x5800005e
    .word 0xd61f03c0
    .word CH_EXEC_MODE
    .word 0x00000000
    @//switch to 64 bit mode
    @//IRQ 32-bit EL2
    .align  7
    .word 0x14000000
    @//FIQ 32-bit EL2
    .align  7
    .word 0x14000000
    @//Error 32-bit EL2
    .align  7
    .word 0x14000000

CH_EXEC_MODE:
#if defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
    .word 0xaa0003f4 // mov  x20, x0
    .word 0xd51e4021 // msr  elr_el3, x1
    .word 0xd53e4001 // mrs  x1, spsr_el3
    .word 0x9278dc21 // and  x1, x1, #0xffffffffffffff00
    .word 0xb27d0021 // orr  x1, x1, #0x8
    .word 0xb27e0021 // orr  x1, x1, #0x4
    .word 0xb2400021 // orr  x1, x1, #0x1
    .word 0xd51e4001 // msr  spsr_el3, x1
    .word 0x92800001 // mov  x1, #0xffffffffffffffff       
    .word 0xd51e1101 // msr  scr_el3, x1
    .word 0xd69f03e0 // eret
#else
    .word 0xd2808820 // mov x0, #0x441
    .word 0xd51e1100 // msr scr_el3, x0
    .word 0xd2a4051e // mov x30, #0x20280000
    .word 0xd51e403e // msr elr_el3, x30
    .word 0xd53e4001 // mrs x1, spsr_el3
    .word 0x927be821 // and x1, x1, #0xffffffffffffffe0
    .word 0xb2400021 // orr x1, x1, #0x1
    .word 0xb27d0021 // orr x1, x1, #0x8
    .word 0xd51e4001 // msr spsr_el3, x1
    .word 0xd69f03e0 // eret
#endif
#endif
