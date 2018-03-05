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

    .text

.globl _vector
_vector:
    b BOOT_Entry
_undefined_instruction:
    b .
_software_interrupt:
    b .
_prefetch_abort:
    b .
_data_abort:
    b .
_not_used:
    b .
_irq:
    b .
_fiq:
    b .

#include "chip.inc"
#-------------------------------------------------------------------------------------------------
# BOOT_Entry:
# @param  None
# @return None
# @note   Entry point of Boot code
#-------------------------------------------------------------------------------------------------
    .globl  BOOT_Entry
BOOT_Entry:

    # inital PIU timers to measure boot time
    # refine from bring up process
    WREG  0x1f006040, 0x0000
    WREG  0x1f006040, 0x0001
    WREG  0x1f006088, 0xFFFF
    WREG  0x1f00608C, 0xFFFF
    WREG  0x1f006080, 0x0000
    WREG  0x1f006080, 0x0001

    # Process GIC interrupt end
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



     @
     @ If Cortex A Class handle secondary cores
     @
    mrc     p15, 0, r0, c0, c0, 5
    and r0, #0x3
    cmp r0, #0
    beq Core0_Next

    BL      __secondary_a_core                @ Will not return

 Core0_Next :
    # Jump to BOOTROM_Entry
    ldr   pc, =BOOTROM_Entry

__secondary_a_core:

    #//Set Priority Mask Register in CPU Interface
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x04), 0xf0

    #//Set Control Register in CPU Interface to Enable Software Interrupt
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x00), 0x01

    WREG        (MS_BUS_RIU_ADDR + (0x1033A2 << 1)), 0xff

    wfi

    @//Core 1 must jump to DRAM space to avoid SPI flash hang issue
    ldr         pc, =Bootram_SecondaryCore

#-------------------------------------------------------------------------------------------------
# BOOT_Pad
# @param  None
# @return None
# @note   Pad with zeros (Reserve SIZE_BOOT_RESERVED Bytes for boot)
#-------------------------------------------------------------------------------------------------
    @.org    (SIZE_BOOT_RESERVED-4)
    @.word   0
