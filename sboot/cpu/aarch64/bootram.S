################################################################################
#
# Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
#include "chip.inc"
#include "macro.inc"
    .extern UBOOT_ENTRY

    .globl JUMP_UBOOT

/*
 * PSR bits
 */
#define PSR_MODE_EL0t   0x00000000
#define PSR_MODE_EL1t   0x00000004
#define PSR_MODE_EL1h   0x00000005
#define PSR_MODE_EL2t   0x00000008
#define PSR_MODE_EL2h   0x00000009
#define PSR_MODE_EL3t   0x0000000c
#define PSR_MODE_EL3h   0x0000000d
#define PSR_MODE_MASK   0x0000000f
#define CPUECTLR_EL1 S3_1_C15_C2_1

#//================================================================================================
#//SecondaryCore loop
#//================================================================================================
#if defined(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT)
    .sect ".ldr.teepostloader.text"
#endif
    .globl      Bootram_SecondaryCore
Bootram_SecondaryCore:
#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
    mrs  x7, sctlr_el3
    mov  x8, #0x1000
    orr  x8, x8, #0x4
    orr  x8, x8, #0x1
    orr  x8, x8, #0x800
    bic  x7, x7, x8
    orr  x7, x7, #0x20
    orr  x7, x7, #0x8
    msr  sctlr_el3, x7
    mrs  x7, s3_1_c15_c2_1
    orr  x7, x7, #0x40
    msr  s3_1_c15_c2_1, x7

FirmwarePath:
    wfe
    ldr x26, =0x1f200a28
    ldr		w1, [x26]
    ldr		x2, =0xfac0
    mrs		x0, mpidr_el1
#if 0
    and		w0, w0, #0x3
#else
    and		w7, w0, #0xff
    and		w0, w0, #0xff << 8
    add		w0, w7, w0, LSR #7
#endif
    orr		w2, w2, w0 //get cpu jump number
    cmp		w2, w1
    b.ne	NormalPath
    mov		w0, #0x0
    str		w0, [x26]
    ldr x26, =0x1f200a24
    ldr		w7, [x26]
    ldr x26, =0x1f200a20
    ldr		w8, [x26]
    add		w9, w8, w7, lsl #16
    ret		x9
    nop
    nop
    b	FirmwarePath
NormalPath:
    ldr x26, =0x1f200a28
    ldr w1, [x26]
    ldr w2, =0xbabe
    cmp w1, w2
    b.ne FirmwarePath
    mov x9, #0x0
    ldr x26, =0x1f200a24
    ldr w7, [x26]
    ldr x26, =0x1f200a20
    ldr w8, [x26]
    add w9, w8, w7, lsl #16
    mrs x0, currentel
    msr elr_el3, x9
    msr sctlr_el2, xzr
    mov x0, #0x3c9
    msr spsr_el3, x0
    mov x0, #0x401
    msr scr_el3, x0
    eret
#else
#if defined(CONFIG_ARMv8_32BIT_KERNEL)
    RREG        w1, (CONFIG_KERNEL_START_ADDRESS+0x8000)
#else
    RREG        w1, BOOT_INFO_REGISTER_MAGIC
#endif

    ldr         w2, =0xbabe               //check magic number
    cmp         w1, w2
    b.ne         Bootram_SecondaryCore

#endif
#if defined(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT)
    .text
#endif
Bootram_MpCoreLoop:

#if defined(CONFIG_ARMv8_32BIT_KERNEL)


    b aarch32_setup_ex
    //bl aarch32_setup_ex
    .word 0xe320f000  //nop
    .word 0xe320f000  //nop
    .word 0xeafffffe  //b .

#else
#if defined(CONFIG_MSTAR_KANO) || defined(CONFIG_MSTAR_K6) || defined(CONFIG_MSTAR_CURRY)
    #//Initial Global Interrupt Interface
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x0004), 0x000000f0
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0180), 0xffff0000
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0100), 0x0000ffff

    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x0000), 0x00000001
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0000), 0x00000003

    ldr         x1, =REG_GIC_DIST_BASE_ADDRESS + 0x80       // GICD_IGROUPR
    ldr         w0, =~0                                     // Grp1 interrupts
    str         w0, [x1]

    ldr         x1, =REG_GIC_CPU_BASE_ADDRESS               // GICC_CTLR
    ldr         w0, =0x3                                    // EnableGrp0 | EnableGrp1
    str         w0, [x1]
#endif
    /*enter el1 bypassing el2 */
    bl          el3_setup

    RREG        w7, BOOT_INFO_REGISTER_CORE1_START_ADDR_LOW
    and         w7, w7, #0x03
    cmp         w7, #0
    b.ne        Bootram_MpCoreLoop        //Check the starting address of secondary processor is legal

    RREG        w7, BOOT_INFO_REGISTER_CORE1_START_ADDR_HIGH
    RREG        w8, BOOT_INFO_REGISTER_CORE1_START_ADDR_LOW

    add         w9, w8, w7, lsl #16
    br          x9

#endif

/*************************************************************************
 * If we're fortunate enough to boot at EL3, ensure that the world is
 * sane before dropping to EL2.
 *************************************************************************/
el3_setup:
    mrs         x0, CurrentEL
    cmp         x0, #PSR_MODE_EL3t
    ccmp        x0, #PSR_MODE_EL3h, #0x4, ne
    b.eq        1f
    ret

1:  msr         ELR_EL3, x30
    msr         SCTLR_EL2, XZR
    mov         x0, #0x3c9
    msr         SPSR_EL3, x0
    mov         x0, #0x401
    msr         SCR_EL3, x0
    eret



JUMP_UBOOT:

#ifndef CONFIG_TEE_LOADER
#if defined(CONFIG_ARMv8_64BIT_KERNEL) || defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
    // Set Vector to boot 64bit kernel
    ldr x26, =vectors
    msr  vbar_el3, x26
#endif
#endif

#if (defined(CONFIG_MSTAR_KANO) || defined(CONFIG_MSTAR_CURRY)) && defined(CONFIG_SECURE_HW_IP)
    // For low power, Disable TSP clock
    ldr x26, =(0x1f000000 + (0x100B00 << 1) + (0x54 << 1))
    ldr x25, [x26]
    and x25, x25, #0xffff
    orr x25, x25, #1
    str x25, [x26]
#endif

#ifndef CONFIG_TEE_LOADER
#ifndef CONFIG_MSTAR_NOCS3_ENABLE
#if defined(CONFIG_MSTAR_KANO) || defined(CONFIG_MSTAR_K6) || defined(CONFIG_MSTAR_CURRY)
    #//Initial Global Interrupt Interface
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x0004), 0x000000f0
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0180), 0xffff0000
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0100), 0x0000ffff

    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x0000), 0x00000001
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0000), 0x00000003

#if defined(CONFIG_ARMv8_32BIT_KERNEL)
    ldr         x1, =REG_GIC_DIST_BASE_ADDRESS + 0x80      // GICD_IGROUPR
    ldr         w0, =0                                     // Grp0 interrupts
    str         w0, [x1]

#else
    ldr         x1, =REG_GIC_DIST_BASE_ADDRESS + 0x80       // GICD_IGROUPR
    ldr         w0, =~0                                     // Grp1 interrupts
    ldr         w2, =~(3 << 30)                             // mboot will be secure_group, so bit_30 need to be 0
    and         w0, w0, w2
    str         w0, [x1]

////////////////////////////////////////////////////
// Setup SPI interrupt ID as Grp1 for SPI mode kernel
////////////////////////////////////////////////////
    ldr         x1, =REG_GIC_DIST_BASE_ADDRESS + 0x80      // GICD_IGROUPR
    ldr         w2, =0xffffffff
    ldr         w0, =0xa
    _GIC_LOOP:
    add         x1, x1,#4
    str         w2, [x1]
    sub         w0,w0,#0x1
    cmp         w0,#0
    bne         _GIC_LOOP
////////////////////////////////////////////////////

#endif

    ldr         x1, =REG_GIC_CPU_BASE_ADDRESS               // GICC_CTLR
    ldr         w0, =0x3                                    // EnableGrp0 | EnableGrp1
    str         w0, [x1]
#else
    #//Initial Global Interrupt Interface
    WREG        (0x16001000 + 0x0000), 0x00000001
    WREG        (0x16002000 + 0x0004), 0x000000f0
    WREG        (0x16001000+ 0x0180), 0xffff0000
    WREG        (0x16001000+ 0x0100), 0x0000ffff
    WREG        (0x16002000+ 0x0000), 0x00000001
#endif
#endif //CONFIG_MSTAR_NOCS3_ENABLE

    #//Poke Core1/2/3 using software interrupt
    WREG        (0x16001000+ 0x0F00), 0x000e0001

    #//Switch FIQ/IRQ merge bit
    RREG        w0, (0x1f000000 + (0x123964 << 1))
    ldr         w1, =0x0020
    bic         w0, w0, w1
    ldr         w1, =0x0050
    orr         w0, w0, w1
    REG_C2M     w0, (0x1f000000 + (0x123964 << 1))
#endif

#if defined (CONFIG_MSTAR_NS_UBOOT)
#if defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
#if defined (CONFIG_TEE_LOADER)
    ldr        x29, =UBOOT_ENTRY
    ldr        x30, [x29]
    b          aarch32_setup
#else
    ldr        x1, =UBOOT_ENTRY
    ldr        x0, [x1]
    bl         ns_uboot_boot
#endif //CONFIG_TEE_LOADER
#endif //CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
#else
    ldr        x29, =UBOOT_ENTRY
    ldr        x30, [x29]
    b          aarch32_setup
#endif//(CONFIG_MSTAR_NS_UBOOT)

    .globl      sboot_str_jump_kernel
sboot_str_jump_kernel:
    ldr        x2, [x0]
    ldr        w30, [x0, #4]
    b          aarch32_setup

aarch32_setup:
    #msr      vbar_el3, xzr
    msr      vbar_el2, xzr
#if defined (CONFIG_TEE_LOADER)
   /*Put system registers to known values*/
    MSR      SCTLR_EL2, xzr
    msr      SCTLR_EL1, xzr
    msr      CPTR_EL2, xzr
    msr      CPTR_EL2, xzr
    msr      CPACR_EL1, xzr
    /*msr      FPEXC32_EL2, xzr*/
    msr      HACR_EL2, xzr
    msr      HSTR_EL2, xzr
    msr      CNTVOFF_EL2, xzr

    /*Set ID register view for NS.EL1*/
    mrs      x0, MPIDR_EL1
    msr      VMPIDR_EL2, x0
    mrs      x0, MIDR_EL1
    msr      VPIDR_EL2, x0

    /*CORTEX-A53 SPECIFIC CORE*/
    mrs      x1, ACTLR_EL2
    orr      x1, x1, #1
    orr      x1, x1, #(1 << 1)
    orr      x1, x1, #(1 << 4)
    orr      x1, x1, #(1 << 5)
    orr      x1, x1, #(1 << 6)
    msr      ACTLR_EL2, x1

    msr      HCR_EL2, xzr
    msr      ELR_EL2, x30
    msr      SCTLR_EL2, XZR

    mov      x0, #0x3c9
    msr      SPSR_EL2, x0

    //mov      x0, #0x401
    //msr      SCR_EL3, x0

    mrs      x1,spsr_el2
    mov      x2, 0xfffffffffffffde0
    and      x1, x1, x2
    orr      x1, x1,#0x3       //SVC mode
    orr      x1, x1,#(1 << 4)  //AARCH32
    and      x1, x1,~(1 << 9)  //Litle endian
    msr      spsr_el2, x1
#else
   /*Put system registers to known values*/
    MSR      SCTLR_EL2, xzr
    msr      SCTLR_EL1, xzr
    msr      CPTR_EL3, xzr
    msr      CPTR_EL2, xzr
    msr      CPACR_EL1, xzr
    /*msr      FPEXC32_EL2, xzr*/
    msr      HACR_EL2, xzr
    msr      HSTR_EL2, xzr
    msr      CNTVOFF_EL2, xzr

    /*Set ID register view for NS.EL1*/
    mrs      x0, MPIDR_EL1
    msr      VMPIDR_EL2, x0
    mrs      x0, MIDR_EL1
    msr      VPIDR_EL2, x0

    /*CORTEX-A53 SPECIFIC CORE*/
    mrs      x1, ACTLR_EL3
    orr      x1, x1, #1
    orr      x1, x1, #(1 << 1)
    orr      x1, x1, #(1 << 4)
    orr      x1, x1, #(1 << 5)
    orr      x1, x1, #(1 << 6)
    msr      ACTLR_EL3, x1

    msr      HCR_EL2, xzr
    msr      ELR_EL3, x30
    msr      SCTLR_EL2, XZR

    mov      x0, #0x3c9
    msr      SPSR_EL3, x0

    //mov      x0, #0x401
    //msr      SCR_EL3, x0

    mrs      x1,spsr_el3
    mov      x2, 0xfffffffffffffde0
    and      x1, x1, x2
    orr      x1, x1,#0x3       //SVC mode
    orr      x1, x1,#(1 << 4)  //AARCH32
    and      x1, x1,~(1 << 9)  //Litle endian
    msr      spsr_el3, x1
#endif
    eret

aarch32_setup_ex:


    RREG     w7,(CONFIG_KERNEL_START_ADDRESS+0x8004)
    msr      vbar_el3, xzr
    msr      vbar_el2, xzr

   /*Put system registers to known values*/
    MSR      SCTLR_EL2, xzr
    msr      SCTLR_EL1, xzr
    msr      CPTR_EL3, xzr
    msr      CPTR_EL2, xzr
    msr      CPACR_EL1, xzr
    /*msr      FPEXC32_EL2, xzr*/
    msr      HACR_EL2, xzr
    msr      HSTR_EL2, xzr
    msr      CNTVOFF_EL2, xzr

    /*Set ID register view for NS.EL1*/
    mrs      x0, MPIDR_EL1
    msr      VMPIDR_EL2, x0
    mrs      x0, MIDR_EL1
    msr      VPIDR_EL2, x0

    /*CORTEX-A53 SPECIFIC CORE*/
    mrs      x1, ACTLR_EL3
    orr      x1, x1, #1
    orr      x1, x1, #(1 << 1)
    orr      x1, x1, #(1 << 4)
    orr      x1, x1, #(1 << 5)
    orr      x1, x1, #(1 << 6)
    msr      ACTLR_EL3, x1

    mrs      x1, ACTLR_EL2
    orr      x1, x1, #(1<<1)
    msr      ACTLR_EL2, x1


    msr      HCR_EL2, xzr
    msr      ELR_EL3, x30
    msr      SCTLR_EL2, XZR
    mov      x0, #0x3c9
    msr      SPSR_EL3, x0

    //core 1/2/3 must set their SCR RW/NS bit to 0x0

    mov       x0,#0
    //mrs      x0,SCR_EL3
    //and      x0, x0,~(1)  //clear NS bit
    //and      x0, x0,~(1 << 10)  //clear RW bit
    msr      SCR_EL3, x0

    //set ELR to vexpress_secondary_startup
    msr      ELR_EL3, x7

    mrs      x1,spsr_el3
    mov      x2, 0xfffffffffffffde0
    and      x1, x1, x2
    orr      x1, x1,#0x3       //SVC mode
    orr      x1, x1,#(1 << 4)  //AARCH32
    and      x1, x1,~(1 << 9)  //Litle endian
    msr      spsr_el3, x1

    eret



   .align 10
   .globl mstar_secondary_reset

mstar_secondary_reset:

    RREG   w0, (REG_GIC_CPU_BASE_ADDRESS + 0x0c)
    ldr         x26, =(REG_GIC_CPU_BASE_ADDRESS + 0x10)
    str         w0, [x26]    /*disable watch dog*/

    WREG  0x1f006010, 0x0000
    WREG  0x1f006014, 0x0000
    WREG_DBG  0x0001    /*init cpu register */

    mrs     x7, SCTLR_EL3
    mov     x8, #(0x1 << 12)
    orr     x8, x8, #(0x1 << 2)
    orr     x8, x8, #(0x1 << 0)
    orr     x8, x8, #(0x1 << 11)
    bic     x7, x7, x8

    msr     SCTLR_EL3, x7
    mrs     x7, CPUECTLR_EL1
    orr     x7, x7, #(0x1 << 6)
    msr     CPUECTLR_EL1, x7

    /*enable kernel to access CPUACTLR smpen bit*/
    mrs x0, actlr_el3
    orr x0, x0, #(1 << 1)
    msr actlr_el3, x0
    mrs x0, actlr_el2
    orr x0, x0, #(1 << 1)
    msr actlr_el2, x0

    #//Enable IRQ on EL3

    mrs         x7, SCR_EL3
    orr         x7, x7, #0x02
    msr         SCR_EL3, x7
    #//Set Priority Mask Register in CPU Interface
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x04), 0xf0

    #//Set Control Register in CPU Interface to Enable Software Interrupt
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x00), 0x01
    WREG        (MS_BUS_RIU_ADDR + (0x1033A2 << 1)), 0xff

    b Bootram_SecondaryCore
