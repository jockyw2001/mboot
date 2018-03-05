/*
 *#############################################################################
 *
 * Copyright (c) 2006-2014 MStar Semiconductor, Inc.
 * All rights reserved.
 *
 * Unless otherwise stipulated in writing, any and all information contained
 * herein regardless in any format shall remain the sole proprietary of
 * MStar Semiconductor Inc. and be kept in strict confidence
 * (¡§MStar Confidential Information¡¨) by the recipient.
 * Any unauthorized act including without limitation unauthorized disclosure,
 * copying, use, reproduction, sale, distribution, modification, disassembling,
 * reverse engineering and compiling of the contents of MStar Confidential
 * Information is unlawful and strictly prohibited. MStar hereby reserves the
 * rights to any and all damages, losses, costs and expenses resulting therefrom.
 *
 *#############################################################################
 */

#include "boot.inc"
#include "macro.inc"

#define CPUECTLR_EL1 S3_1_C15_C2_1

    .text
    .globl  Boot_Entry
Boot_Entry:

    # inital PIU timers to measure boot time
    # refine from bring up process
    WREG  0x1f006040, 0x0000
    WREG  0x1f006040, 0x0001
    WREG  0x1f006088, 0xFFFF
    WREG  0x1f00608C, 0xFFFF
    WREG  0x1f006080, 0x0000
    WREG  0x1f006080, 0x0001
    RREG   w0, (REG_GIC_CPU_BASE_ADDRESS + 0x0c)

    ldr         x26, =(REG_GIC_CPU_BASE_ADDRESS + 0x10)
    str         w0, [x26]

    # disable watch dog
    WREG  0x1f006010, 0x0000
    WREG  0x1f006014, 0x0000

    WREG_DBG  0x0001

    #//Initialize CPU Registers
    bl    Boot_InitCpuRegister

    WREG_DBG  0x0002

    #//Only CPU0 continues to execute and the secondary CPUs execute Boot_SecondaryCore
    mrs         x7, MPIDR_EL1               //Read Multiprocessor Affinity Register
    ands        x7, x7, #0x03               //Mask off, leaving the CPU ID field
    b.ne        Boot_SecondaryCore

Boot_PrimaryCore:
    ldr         x7, =BOOTROM_Entry          //Jump to Entry Point of BootRom
    br          x7

Boot_SecondaryCore:
    #//Enable IRQ on EL3
    mrs         x7, SCR_EL3
    orr         x7, x7, #0x02
    msr         SCR_EL3, x7

    #//Set Priority Mask Register in CPU Interface
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x04), 0xf0

    #//Set Control Register in CPU Interface to Enable Software Interrupt
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x00), 0x01

    WREG        (MS_BUS_RIU_ADDR + (0x1033A2 << 1)), 0xff

    #enable kernel to access cpuactlr smpen bit
    mrs x0, ACTLR_EL3
    orr x0, x0, #(1 << 1)
    msr ACTLR_EL3, x0

    mrs x0, ACTLR_EL2
    orr x0, x0, #(1 << 1)
    msr ACTLR_EL2, x0

 #store mstar addr in bank 0x101e reg:0x6c,0x6d
     ldr x7, =mstar_secondary_reset
     lsr x7, x7, #10
     ldr x8, =0x1f203db0
     uxth w7, w7
     strh w7 ,[x8]
     ldr x7, =mstar_secondary_reset
     lsr x7, x7, #16
     and	x7, x7, #0xffff
     uxth w7,w7
     ldr x8, =0x1f203db4
     ldrh w9, [x8]
     and	w9, w9, #0xffc0
     orr	w9, w7, w9
     orr	w9, w9, #0x8000
     strh w9, [x8]


    wfi

    #//Core 1/2/3 must jump to DRAM space to avoid SPI flash hang issue
    ldr         x7, =Bootram_SecondaryCore
    br          x7

Boot_SecondaryCoreLoop:
    b           Boot_SecondaryCoreLoop

#-------------------------------------------------------------------------------------------------
# Boot_InitCpuRegister
# @param  None
# @return None
# @note   ARM internal register init setting
#-------------------------------------------------------------------------------------------------
Boot_InitCpuRegister:
    mrs     x7, SCTLR_EL3               //System Control Register (EL3)
    mov     x8, #(0x1 << 12)            //Clear I bit 12 to disable I Cache
    orr     x8, x8, #(0x1 << 2)         //Clear C bit  2 to disable D Cache
    orr     x8, x8, #(0x1 << 0)         //Clear M bit  0 to disable MMU
    orr     x8, x8, #(0x1 << 11)        //Clear Z bit 11 to disable branch prediction
    bic     x7, x7, x8
    msr     SCTLR_EL3, x7               //Write System Control Register

    mrs     x7, CPUECTLR_EL1            //CPU extended control register, EL1
    orr     x7, x7, #(0x1 << 6)         //Enable hardware management of data coherency with other processors in SMP system
    msr     CPUECTLR_EL1, x7

    # invalidate_icache_all
    ic      ialluis
    isb     sy

    # Enable I-Cache
    mrs     x7, SCTLR_EL3               //System Control Register (EL3)
    orr     x7, x7, #(0x1 << 12)
    msr     SCTLR_EL3, x7               //Write System Control Register

    ret
