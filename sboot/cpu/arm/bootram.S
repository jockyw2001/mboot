################################################################################
#
# Copyright (c) 2006-2007 MStar Semiconductor, Inc.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# MStar Semiconductor Inc. and be kept in strict confidence
# (¡§MStar Confidential Information¡¨) by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of MStar Confidential
# Information is unlawful and strictly prohibited. MStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#
################################################################################



#include "boot.inc"
#include "macro.inc"

#if defined(CONFIG_MSTAR_K6LITE) || defined(CONFIG_MSTAR_K7U)
#if defined(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT)
    .sect ".ldr.teepostloader.text"
#endif
    .global      Bootram_SecondaryCore
#ifdef CONFIG_ARMv7_OPTEE
Bootram_SecondaryCore:
       wfi

       RREG            r1, (CONFIG_KERNEL_START_ADDRESS+0x8000 - 0x8)
       ldr             r2, =0xfac0             @//check magic number
       mrc             p15, 0, r0, c0, c0, 5
       and             r0, r0, #0x03
       orr             r2, r0, r2
       cmp             r1, r2
       beq             GetKey
       b		Bootram_SecondaryCore

       GetKey:
       WREG            (CONFIG_KERNEL_START_ADDRESS+0x8000 - 0x8), 0
       RREG            r1, (CONFIG_KERNEL_START_ADDRESS+0x8000 - 0x4)
       blx             r1
       nop
       nop
	b		.    //never reach here
#else
Bootram_SecondaryCore:
    RREG        r1, (CONFIG_KERNEL_START_ADDRESS+0x8000)
    ldr         r2, =0xbabe               //check magic number
    cmp         r1, r2
    bne         Bootram_SecondaryCore

Bootram_MpCoreLoop:
    RREG        r0,(CONFIG_KERNEL_START_ADDRESS+0x8004)
    mov         pc, r0
#endif
#else
    .globl      Bootram_SecondaryCore
Bootram_SecondaryCore:
    # Get data base address
    .word 0x58000058 // ldr x24, 0x8
    .word 0x14000003 // b addr+0xc
    .word DATA_Bootram_SecondaryCore
    .word 0x00000000

#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
    .word 0xd53e1007 // mrs x7, sctlr_el3
    .word 0xd2820008 // mov x8, #0x1000    
    .word 0xb27e0108 // orr x8, x8, #0x4
    .word 0xb2400108 // orr x8, x8, #0x1
    .word 0xb2750108 // orr x8, x8, #0x800
    .word 0x8a2800e7 // bic x7, x7, x8
    .word 0xb27b00e7 // orr x7, x7, #0x20
    .word 0xb27d00e7 // orr x7, x7, #0x8
    .word 0xd51e1007 // msr sctlr_el3, x7
    .word 0xd539f227 // mrs x7, s3_1_c15_c2_1
    .word 0xb27a00e7 // orr x7, x7, #0x40
    .word 0xd519f227 // msr s3_1_c15_c2_1, x7

FirmwarePath:
    .word 0xd503205f // wfe
    .word 0xf940031a // ldr  x26, [x24] ; Read D1
    .word 0xb9400341 // ldr  w1, [x26]
    .word 0xf9401b02 // ldr  x2, [x24, 0x30] ; Read D7
    .word 0xd53800a0 // mrs  x0, mpidr_el1
    .word 0x12001c07 // and  w7, w0, #0xff
    .word 0x12181c00 // and  w0, w0, #0xff00
    .word 0x0b401ce0 // add  w0, w7, w0, lsr #7
    .word 0x2a000042 // orr  w2, w2, w0
    .word 0x6b01005f // cmp  w2, w1
    .word 0x54000181 // b.ne NormalPath
    .word 0x52800000 // mov  w0, #0x0                        
    .word 0xb9000340 // str  w0, [x26]
    .word 0xf940071a // ldr  x26, [x24, 0x8] ; Read D2
    .word 0xb9400347 // ldr  w7, [x26]
    .word 0xf9400b1a // ldr  x26, [x24, 0x10] ; Read D3
    .word 0xb9400348 // ldr  w8, [x26]
    .word 0x0b074109 // add  w9, w8, w7, lsl #16
    .word 0xd65f0120 // ret  x9
    .word 0xd503201f // nop
    .word 0xd503201f // nop
    .word 0x17ffffeb // b  FirmwarePath

NormalPath:
    .word 0xf9400f1a // ldr  x26, [x24, 0x18] ; Read D4
    .word 0xb9400341 // ldr  w1, [x26]
    .word 0xb9403b02 // ldr  w2, [x24, 0x38] ; Read D8
    .word 0x6b02003f // cmp  w1, w2
    .word 0x54fffcc1 // b.ne FirmwarePath
    .word 0xd2800009 // mov  x9, #0x0                 
    .word 0xf940131a // ldr  x26, [x24, 0x20] ; Read D5
    .word 0xb9400347 // ldr  w7, [x26]
    .word 0xf940171a // ldr  x26, [x24, 0x28] ; Read D6
    .word 0xb9400348 // ldr  w8, [x26]
    .word 0x0b074109 // add  w9, w8, w7, lsl #16
    .word 0xd5384240 // mrs  x0, currentel
    .word 0xd51e4029 // msr  elr_el3, x9
    .word 0xd51c101f // msr  sctlr_el2, xzr
    .word 0xd2807920 // mov  x0, #0x3c9           
    .word 0xd51e4000 // msr  spsr_el3, x0
    .word 0xd2808020 // mov  x0, #0x401        
    .word 0xd51e1100 // msr  scr_el3, x0
    .word 0xd69f03e0 // eret

    #Data
    .align 3
DATA_Bootram_SecondaryCore:
    .word 0x1f200a28 // D1
    .word 0x00000000
    .word 0x1f200a24 // D2
    .word 0x00000000
    .word 0x1f200a20 // D3 
    .word 0x00000000
    .word 0x1f200a28 // D4
    .word 0x00000000
    .word 0x1f200a24 // D5 
    .word 0x00000000
    .word 0x1f200a20 // D6
    .word 0x00000000
    .word 0x0000fac0 // D7 
    .word 0x00000000
    .word 0x0000babe // D8

#else
#if defined(CONFIG_ARMv8_32BIT_KERNEL)
    .word 0xf940031a // ldr x26, [x24] ; Read D0
    .word 0xb9400341 // ldr w1, [x26]
    .word 0xb9400b02 // ldr w2, [x24, 0x8] ; Read D1
    .word 0x6b02003f // cmp w1, w2
    .word 0x54ffff81 // b.ne Bootram_SecondaryCore

    # aarch32_setup
    .word 0xf9400b1a // ldr  x26, [x24, 0x10] ; Read D2
    .word 0xb9400347 // ldr  w7, [x26]
    .word 0xd51ec01f // msr  vbar_el3, xzr
    .word 0xd51cc01f // msr  vbar_el2, xzr

    # Put system registers to known values
    .word 0xd51c101f // msr  sctlr_el2, xzr
    .word 0xd518101f // msr  sctlr_el1, xzr
    .word 0xd51e115f // msr  cptr_el3, xzr
    .word 0xd51c115f // msr  cptr_el2, xzr
    .word 0xd518105f // msr  cpacr_el1, xzr

    # msr      FPEXC32_EL2, xzr
    .word 0xd51c11ff // msr  hacr_el2, xzr
    .word 0xd51c117f // msr  hstr_el2, xzr
    .word 0xd51ce07f // msr  cntvoff_el2, xzr

    # Set ID register view for NS.EL1
    .word 0xd53800a0 // mrs  x0, mpidr_el1
    .word 0xd51c00a0 // msr  vmpidr_el2, x0
    .word 0xd5380000 // mrs  x0, midr_el1
    .word 0xd51c0000 // msr  vpidr_el2, x0
 
    # CORTEX-A53 SPECIFIC CORE
    .word 0xd53e1021 // mrs  x1, actlr_el3
    .word 0xb2400021 // orr  x1, x1, #0x1
    .word 0xb27f0021 // orr  x1, x1, #0x2
    .word 0xb27c0021 // orr  x1, x1, #0x10
    .word 0xb27b0021 // orr  x1, x1, #0x20
    .word 0xb27a0021 // orr  x1, x1, #0x40
    .word 0xd51e1021 // msr  actlr_el3, x1

    .word 0xd51c111f // msr  hcr_el2, xzr
    .word 0xd51e403e // msr  elr_el3, x30
    .word 0xd51c101f // msr  sctlr_el2, xzr
    .word 0xd2807920 // mov  x0, #0x3c9        
    .word 0xd51e4000 // msr  spsr_el3, x0

    # core 1/2/3 must set their SCR RW/NS bit to 0x0
    .word 0xd2800000 // mov  x0, #0x0               
    .word 0xd51e1100 // msr  scr_el3, x0

    # set ELR to vexpress_secondary_startup
    .word 0xd51e4027 // msr  elr_el3, x7

    .word 0xd53e4001 // mrs  x1, spsr_el3
    .word 0x928043e2 // mov  x2, #0xfffffffffffffde0        
    .word 0x8a020021 // and  x1, x1, x2
    .word 0xb2400421 // orr  x1, x1, #0x3  ; SVC mode
    .word 0xb27c0021 // orr  x1, x1, #0x10 ; AARCH32
    .word 0x9276f821 // and  x1, x1, #0xfffffffffffffdff
    .word 0xd51e4001 // msr  spsr_el3, x1

    # Jump to kernel entry and change state to aarch32
    .word 0xd69f03e0 // eret

    #Data
    .align 3
DATA_Bootram_SecondaryCore:
    .word 0x20208000 // D0
    .word 0x00000000
    .word 0x0000babe // D1
    .word 0x00000000
    .word 0x20208004 // D2
    .word 0x00000000
#else
    .word 0xf940031a // ldr x26, [x24] ; Read D0
    .word 0xb9400341 // ldr w1, [x26]
    .word 0xb9400b02 // ldr w2, [x24, 0x8] ; Read D1
    .word 0x6b02003f // cmp w1, w2
    .word 0x54ffff81 // b.ne Bootram_SecondaryCore

    .word 0xf9400b1a // ldr x26, [x24, 0x10] ; Read D2
    .word 0xb9406319 // ldr w25, [x24, 0x60] ; Read D12
    .word 0xb9000359 // str w25, [x26]
    .word 0xf9400f1a // ldr x26, [x24, 0x18] ; Read D3
    .word 0xb9406719 // ldr w25, [x24, 0x64] ; Read D13
    .word 0xb9000359 // str w25, [x26]
    .word 0xf940131a // ldr x26, [x24, 0x20] ; Read D4
    .word 0xb9406b19 // ldr w25, [x24, 0x68] ; Read D14
    .word 0xb9000359 // str w25, [x26]
    .word 0xf940171a // ldr x26, [x24, 0x28] ; Read D5
    .word 0xb9406f19 // ldr w25, [x24, 0x6c] ; Read D15
    .word 0xb9000359 // str w25, [x26]
    .word 0xf9401b1a // ldr x26, [x24, 0x30] ; Read D6
    .word 0xb9407319 // ldr w25, [x24, 0x70] ; Read D16
    .word 0xb9000359 // str w25, [x26]
    
    .word 0xf9401f01 // ldr x1, [x24, 0x38] ; Read D7
    .word 0xb9407700 // ldr w0, [x24, 0x74] ; Read D17
    .word 0xb9000020 // str w0, [x1]
    .word 0xf9402301 // ldr x1, [x24, 0x40] ; Read D8
    .word 0xb9407b00 // ldr w0, [x24, 0x78] ; Read D18
    .word 0xb9000020 // str w0, [x1]

    #el3_setup
    .word 0x9400000c // bl el3_setup

CHECK_CORE1_START_ADDR:
    .word 0xf940271a // ldr x26, [x24, 0x48] ; Read D9
    .word 0xb9400347 // ldr w7, [x26]
    .word 0x120004e7 // and w7, w7, #0x3
    .word 0x710000ff // cmp w7, #0x0
    .word 0x54ffff81 // b.ne CHECK_CORE1_START_ADDR

    .word 0xf9402b1a // ldr x26, [x24, 0x50] ; Read D10
    .word 0xb9400347 // ldr w7, [x26]
    .word 0xf9402f1a // ldr x26, [x24, 0x58] ; Read D11
    .word 0xb9400348 // ldr w8, [x26]
    .word 0x0b074109 // add w9, w8, w7, lsl #16
    .word 0xd61f0120 // br x9

el3_setup:
    .word 0xd5384240 // mrs   x0, currentel
    .word 0xf100301f // cmp   x0, #PSR_MODE_EL3t
    .word 0xfa4d1804 // ccmp  x0, #PSR_MODE_EL3h, #0x4, ne
    .word 0x54000040 // b.eq  1f
    .word 0xd65f03c0 // ret
1:  .word 0xd51e403e // msr  elr_el3, x30
    .word 0xd51c101f // msr  sctlr_el2, xzr
    .word 0xd2807920 // mov  x0, #0x3c9 
    .word 0xd51e4000 // msr  spsr_el3, x0
    .word 0xd2808020 // mov  x0, #0x401
    .word 0xd51e1100 // msr  scr_el3, x0
    .word 0xd69f03e0 // eret

    # Data Section
    # Addr
    .align 3
DATA_Bootram_SecondaryCore:
    .word BOOT_INFO_REGISTER_MAGIC // D0 0x0
    .word 0x00000000
    .word 0x0000babe // D1
    .word 0x00000000
    .word (REG_GIC_CPU_BASE_ADDRESS + 0x0004) // D2 0x10
    .word 0x00000000
    .word (REG_GIC_DIST_BASE_ADDRESS + 0x0180) // D3
    .word 0x00000000
    .word (REG_GIC_DIST_BASE_ADDRESS + 0x0100) // D4 0x20
    .word 0x00000000
    .word (REG_GIC_CPU_BASE_ADDRESS + 0x0000) // D5
    .word 0x00000000
    .word (REG_GIC_DIST_BASE_ADDRESS + 0x0000) // D6 0x30
    .word 0x00000000
    .word (REG_GIC_DIST_BASE_ADDRESS + 0x80) // D7
    .word 0x00000000
    .word REG_GIC_CPU_BASE_ADDRESS // D8 0x40
    .word 0x00000000
    .word BOOT_INFO_REGISTER_CORE1_START_ADDR_LOW // D9
    .word 0x00000000
    .word BOOT_INFO_REGISTER_CORE1_START_ADDR_HIGH // D10 0x50
    .word 0x00000000
    .word BOOT_INFO_REGISTER_CORE1_START_ADDR_LOW // D11
    .word 0x00000000
    # Data
    .word 0x000000f0 // D12 0x60
    .word 0xffff0000 // D13
    .word 0x0000ffff // D14
    .word 0x00000001 // D15
    .word 0x00000003 // D16 0x70
    .word 0xffffffff // D17
    .word 0x00000003 // D18
#endif
#endif //CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
#endif //NOT CONFIG_MSTAR_K6LITE

#if defined(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT)
    .text
#endif
    .globl GIC_Init
GIC_Init:
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x0004), 0x000000f0
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0180), 0xffff0000
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0100), 0x0000ffff

    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x0000), 0x00000001
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0000), 0x00000003

    ldr         r1, =REG_GIC_DIST_BASE_ADDRESS + 0x80      // GICD_IGROUPR
    ldr         r0, =0                                     // Grp0 interrupts
    str         r0, [r1]

    ldr         r1, =REG_GIC_CPU_BASE_ADDRESS               // GICC_CTLR
    ldr         r0, =0x3                                    // EnableGrp0 | EnableGrp1
    str         r0, [r1]

    WREG        (0x16001000+ 0x0F00), 0x000e0001

    #//Switch FIQ/IRQ merge bit
    RREG        r0, (0x1f000000 + (0x123964 << 1))
    ldr         r1, =0x0020
    bic         r0, r0, r1
    ldr         r1, =0x0050
    orr         r0, r0, r1
    REG_C2M     r0, (0x1f000000 + (0x123964 << 1))
    
    bx lr
    
    .globl Poke_Other_Core
Poke_Other_Core:

    WREG        (0x16001000+ 0x0F00), 0x000e0001

    bx lr

