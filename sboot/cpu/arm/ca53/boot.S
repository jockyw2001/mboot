@###############################################################################
@
@ Copyright (c) 2006-2009 MStar Semiconductor, Inc.
@ All rights reserved.
@
@ Unless otherwise stipulated in writing, any and all information contained
@ herein regardless in any format shall remain the sole proprietary of
@ MStar Semiconductor Inc. and be kept in strict confidence
@ ("MStar Confidential Information") by the recipient.
@ Any unauthorized act including without limitation unauthorized disclosure,
@ copying, use, reproduction, sale, distribution, modification, disassembling,
@ reverse engineering and compiling of the contents of MStar Confidential
@ Information is unlawful and strictly prohibited. MStar hereby reserves the
@ rights to any and all damages, losses, costs and expenses resulting therefrom.
@
@###############################################################################

#include "boot.inc"
#include "macro.inc"
#include "drv_arm.inc"
#include "chip.inc"

    .text
#-------------------------------------------------------------------------------------------------
# BOOT_Entry:
# @param  None
# @return None
# @note   Entry point of Boot code
#-------------------------------------------------------------------------------------------------
    .globl  Boot_Entry
Boot_Entry:
    .word 0xd53800a7 // mrs x7, MPIDR_EL1
    .word 0xf24004e7 // ands x7, x7, #0x03
    .word 0x540000c0 // b.eq Boot_PrimaryCore
    .word 0x5800007e // ldr x30, 0x0c
    .word 0xd61f03c0 // br x30
    .word 0x00000000
    .word __secondary_a_core+0x14000000
    .word 0x00000000
Boot_PrimaryCore:
#if defined(CONFIG_ARMv8_64BIT_KERNEL) || defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
    .word 0x5800069e  // ldr  x30, 0xd0
    .word 0xd51ec01e  // msr  vbar_el3, x30
#endif
    @//init cpu register
    .word 0xd53e1007  // mrs  x7, sctlr_el3
    .word 0xd2820008  // mov  x8, #0x1000
    .word 0xb27e0108  // orr  x8, x8, #0x4
    .word 0xb2400108  // orr  x8, x8, #0x1
    .word 0xb2750108  // orr  x8, x8, #0x800
    .word 0x8a2800e7  // bic  x7, x7, x8
    .word 0xb27b00e7  // orr  x7, x7, #0x20
    .word 0xb27d00e7  // orr  x7, x7, #0x8
    .word 0xd51e1007  // msr  sctlr_el3, x7
    .word 0xd539f227  // mrs  x7, s3_1_c15_c2_1
    .word 0xb27a00e7  // orr  x7, x7, #0x40
    .word 0xd519f227  // msr  s3_1_c15_c2_1, x7

    @//switch from aarch64 to aarch32
    .word 0x5800049e  // ldr  x30, 0x90
    .word 0xd503201f  // nop
    .word 0xd51cc01f  // msr  vbar_el2, xzr
    .word 0xd51c101f  // msr  sctlr_el2, xzr
    .word 0xd518101f  // msr  sctlr_el1, xzr
    .word 0xd51e115f  // msr  cptr_el3, xzr
    .word 0xd51c115f  // msr  cptr_el2, xzr
    .word 0xd518105f  // msr  cpacr_el1, xzr
    .word 0xd51c11ff  // msr  hacr_el2, xzr
    .word 0xd51c117f  // msr  hstr_el2, xzr
    .word 0xd51ce07f  // msr  cntvoff_el2, xzr
    .word 0xd53800a0  // mrs  x0, mpidr_el1
    .word 0xd51c00a0  // msr  vmpidr_el2, x0
    .word 0xd5380000  // mrs  x0, midr_el1
    .word 0xd51c0000  // msr  vpidr_el2, x0
    .word 0xd53e1021  // mrs  x1, actlr_el3
    .word 0xb2400021  // orr  x1, x1, #0x1
    .word 0xb27f0021  // orr  x1, x1, #0x2
    .word 0xb27c0021  // orr  x1, x1, #0x10
    .word 0xb27b0021  // orr  x1, x1, #0x20
    .word 0xb27a0021  // orr  x1, x1, #0x40
    .word 0xd51e1021  // msr  actlr_el3, x1
    .word 0xd51c111f  // msr  hcr_el2, xzr
    .word 0xd51e403e  // msr  elr_el3, x30
    .word 0xd51c101f  // msr  sctlr_el2, xzr
    .word 0xd2807920  // msr  elr_el3, x30
    .word 0xd51e4000  // msr  sctlr_el2, xzr
    .word 0xd51e403e  // msr  elr_el3, x30
    .word 0xd53e4001  // mrs  x1, spsr_el3
    .word 0x928043e2  // mov  x2, #0xfffffffffffffde0
    .word 0x8a020021  // and  x1, x1, x2
    .word 0xb2400421  // orr  x1, x1, #0x3
    .word 0xb27c0021  // orr  x1, x1, #0x10
    .word 0x9276f821  // and  x1, x1, #0xfffffffffffffdff
    .word 0xd51e4001  // msr  spsr_el3, x1
    .word 0xd69f03e0  // eret

    @//data section
    .word aarch32_entry+0x14000000
    .word 0x00000000
#if defined(CONFIG_ARMv8_64BIT_KERNEL) || defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
    .word vectors
    .word 0x00000000
#endif

aarch32_entry:

   # inital PIU timers to measure boot time
    # refine from bring up process
    WREG  0x1f006040, 0x0000
    WREG  0x1f006040, 0x0001
    WREG  0x1f006088, 0xFFFF
    WREG  0x1f00608C, 0xFFFF
    WREG  0x1f006080, 0x0000
    WREG  0x1f006080, 0x0001

    RREG   r0, (REG_GIC_CPU_BASE_ADDRESS + 0x0c)
    ldr    r1, =(REG_GIC_CPU_BASE_ADDRESS + 0x10)
    str    r0, [r1]

    # disable watch dog
    WREG  0x1f006010, 0x0000
    WREG  0x1f006014, 0x0000

    WREG_DBG  0x0001

    @
    @ First ensure all interrupts are disabled.
    @
    MRS     r0, CPSR
    ORR     r0, r0, #0xc0  @ Disable IRQ & FIQ
    ORR     r0, r0, #0xd3      @ARM mode,Supervisor mode
    MSR     CPSR_c, r0

    @
    @ Ensure that the MMU and caches are off
    @
    MRC     p15, 0, r0, c1, c0, 0           @ Get control register
    BIC     r0, r0, #0x1                  @ Disable MMU
    BIC     r0, r0, #0x4                  @ Disable D Cache
    BIC     r0, r0, #0x1000                  @ Disable I Cache
    MCR     p15, 0, r0, c1, c0, 0           @ Write control register

    ldr   pc, =BOOTROM_Entry

    .align 3
__secondary_a_core:
    # Get data base address
    .word 0x58000058 // ldr x24, 0x8
    .word 0x14000003 // b addr+0xc
    .word DATA_secondary_a_core+0x14000000
    .word 0x00000000

    # Complete the interrupt that processed at ROM code
    .word 0xf940031a // ldr x26, [x24] ; Read D1
    .word 0xb9400340 // ldr w0, [x26]
    .word 0xf940071a // ldr x26, [x24, 0x8] ; Read D2
    .word 0xb9000340 // str w0, [x26]

    # disable watch dog
    .word 0xf9400b1a // ldr x26, [x24, 0x10] ; Read D3
    .word 0xb9401719 // ldr w25, [x24, 0x14] ; Read D4
    .word 0xb9000359 // str w25, [x26]
    .word 0xf9400f1a // ldr x26, [x24, 0x18] ; Read D5
    .word 0xb9401f19 // ldr w25, [x24, 0x1c] ; Read D6
    .word 0xb9000359 // str w25, [x26]

    # InitCpuRegister
    .word 0xd53e1007 // mrs x7, sctlr_el3 ; System Control Register (EL3)
    .word 0xd2820008 // mov x8, #0x1000  ; Clear I bit 12 to disable I Cache
    .word 0xb27e0108 // orr x8, x8, #0x4 ; Clear C bit  2 to disable D Cache
    .word 0xb2400108 // orr x8, x8, #0x1 ; Clear M bit  0 to disable MMU
    .word 0xb2750108 // orr x8, x8, #0x80 ; Clear Z bit 11 to disable branch prediction
    .word 0x8a2800e7 // bic x7, x7, x8
    .word 0xd51e1007 // msr sctlr_el3, x7 Write System Control Register

    .word 0xd539f227 // mrs x7, s3_1_c15_ ; CPU extended control register, EL1
    .word 0xb27a00e7 // orr x7, x7, #0x40 ; Enable hardware management of data coherency with other processors in SMP system
    .word 0xd519f227 // msr s3_1_c15_c2_1

    # invalidate_icache_all
    .word 0xd508711f // ic  ialluis
    .word 0xd5033fdf // isb

    #  Enable I-Cache
    .word 0xd53e1007 // mrs x7, sctlr_el3 ; System Control Register (EL3)
    .word 0xb27400e7 // orr x7, x7, #0x10
    .word 0xd51e1007 // msr sctlr_el3, x7 ; Write System Control Register

    # Enable IRQ on EL3
    .word 0xd53e1107 // mrs x7, SCR_EL3
    .word 0xb27f00e7 // orr x7, x7, #0x02
    .word 0xd51e1107 // msr SCR_EL3, x7

    # Set Priority Mask Register in CPU Interface
    .word 0xf940131a // ldr x26, [x24, 0x20] ; Read D7
    .word 0xb9402b19 // ldr w25, [x24, 0x28] ; Read D8
    .word 0xb9000359 // str w25, [x26]
    
    # Set Control Register in CPU Interface to Enable Software Interrupt
    .word 0xf9401b1a // ldr x26, [x24, 0x30] ; Read D9
    .word 0xb9403b19 // ldr w25, [x24, 0x38] ; Read D10
    .word 0xb9000359 // str w25, [x26]

    .word 0xf940231a // ldr x26, [x24, 0x40] ; Read D11
    .word 0xb9404b19 // ldr w25, [x24, 0x48] ; Read D12
    .word 0xb9000359 // str w25, [x26]

    # enable kernel to access cpuactlr smpen bit
    .word 0xd53e1020 // mrs x0, ACTLR_EL3
    .word 0xb27f0000 // orr x0, x0, #(1 << 1)
    .word 0xd51e1020 // msr ACTLR_EL3, x0
    
    .word 0xd53c1020 // mrs x0, ACTLR_EL2
    .word 0xb27f0000 // orr x0, x0, #(1 << 1)
    .word 0xd51c1020 // msr ACTLR_EL2, x0

    .word 0xd503207f // wfi

    # Jump to Bootram Entry
    .word 0xf9402b07 // ldr x7, [x24, 0x50] ; Read D13
    .word 0xd61f00e0 // br x7

    # Data 
    .align 3
DATA_secondary_a_core:
    .word (REG_GIC_CPU_BASE_ADDRESS + 0x0c) // D1
    .word 0x00000000
    .word (REG_GIC_CPU_BASE_ADDRESS + 0x10) // D2
    .word 0x00000000
    .word 0x1f006010 // D3
    .word 0x00000000 // D4
    .word 0x1f006014 // D5
    .word 0x00000000 // D6
    .word (REG_GIC_CPU_BASE_ADDRESS + 0x04) //D7
    .word 0x00000000
    .word 0x000000f0 //D8
    .word 0x00000000
    .word (REG_GIC_CPU_BASE_ADDRESS + 0x00) //D9
    .word 0x00000000
    .word 0x00000001 //D10
    .word 0x00000000
    .word (MS_BUS_RIU_ADDR + (0x1033A2 << 1)) //D11
    .word 0x00000000
    .word 0x000000ff //D12
    .word 0x00000000
    .word Bootram_SecondaryCore //D13
    .word 0x00000000

