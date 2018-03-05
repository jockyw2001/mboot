#include "macro.inc"

#if defined(CONFIG_ARMv8_64BIT_KERNEL) || defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
    .globl      vectors
    .globl      sboot_str_jump_64bit_kernel

vectors:
    ventry      el1_sync_invalid                // Synchronous EL1t
    ventry      el1_irq_invalid                 // IRQ EL1t
    ventry      el1_fiq_invalid                 // FIQ EL1t
    ventry      el1_error_invalid               // Error EL1t

    ventry      el1_sync                        // Synchronous el1h
    ventry      el1_irq                         // IRQ EL1h
    ventry      el1_fiq_invalid                 // FIQ EL1h
    ventry      el1_error_invalid               // Error EL1h

    ventry      el0_sync                        // Synchronous 64-bit EL0
    ventry      el0_irq                         // IRQ 64-bit EL0
    ventry      el0_fiq_invalid                 // FIQ 64-bit EL0
    ventry      el0_error_invalid               // Error 64-bit EL0

    ventry      el0_sync_invalid                // Synchronous 32-bit EL0
    ventry      el0_irq_invalid                 // IRQ 32-bit EL0
    ventry      el0_fiq_invalid                 // FIQ 32-bit EL0
    ventry      el0_error_invalid               // Error 32-bit EL0

el1_sync_invalid:
    //b  .
el1_irq_invalid:
    b  .
el1_fiq_invalid:
    b  .
el1_error_invalid:
    b  .
el1_sync:
    b  .
el1_irq:
    b  .
el0_sync:
    b  .
el0_irq:
    b  .
el0_fiq_invalid:
    b  .
el0_error_invalid:
    b  .
el0_sync_invalid:
    b CH_EXEC_MODE
    //b  .
el0_irq_invalid:
    b  .
CH_EXEC_MODE:

#if defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
    mov      x20, x0
    msr  elr_el3, x1

    mrs  x1, spsr_el3
    and  x1, x1, #0xffffffffffffff00
    orr  x1, x1, #0x8
    orr  x1, x1, #0x4
    orr  x1, x1, #0x1
    msr  spsr_el3, x1
    mov  x1, #0x440                 >// #1088
    msr  scr_el3, x1
    eret
#else

sboot_str_jump_64bit_kernel:
    mov      x0, #0x441
    msr      SCR_EL3, x0
    mov      x30,0x20280000
    msr      ELR_EL3, x30

    //Let cpu stay in EL2H mode for 64bit kernel boot up
    mrs      x1,spsr_el3

    bic      x1,x1,#0x1F

    orr      x1,x1,#0x1
    orr      x1,x1,#(1 << 3)

    msr      spsr_el3, x1

    eret
#endif
#endif
