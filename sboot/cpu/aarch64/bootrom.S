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

#-------------------------------------------------------------------------------------------------
# BOOTROM_CopyBin
# @param  None
# @return None
# @note   Copy LDR & APP from ROM to DRAM (DMA or CPU)
# @src(r10), dst(r11), len(r12) should be 32bit align
#-------------------------------------------------------------------------------------------------
.macro BOOTROM_CopyBin
    LDR     x25, =_ld_LDR_load_start
    LDR     x28, =_var_LDR_run_base
    LDR     x26, [x28]
    LDR     x28, =_var_LDR_size
    LDR     x27, [x28]
    MEMCPY32
.endm

#-------------------------------------------------------------------------------------------------
# BOOTROM_Entry:
# @param  None
# @return None
# @note   Entry point of Boot code
#-------------------------------------------------------------------------------------------------
    .globl  BOOTROM_Entry
BOOTROM_Entry:

#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
    # Switch fast mode
    CHIP_SWITCH_SPI_FAST_MODE

    # Dummy jump
    bl     BOOTROM_JUMP_DUMMY
    nop

    # Power on Sequence
    CHIP_PowerOnChip

    WREG_DBG  0x0011

    # Intialize DSPRAM
    CHIP_InitDSPRAM

    WREG_DBG  0x0012
#endif
    CHIP_InitUART

    PUTC '\r'
    PUTC '\n'
    PUTC 'B'

    WREG_DBG  0x00013

BOOTROM_RAMLoader:

    PUTC 'O'
    PUTC 'O'
    PUTC 'T'

    # Initial Stack Pointer
    ldr    x7, =_ld_STACK_end
    mov    sp, x7

    bl  BOOTROM_C_InitMIU_ENTRY

#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
    CPU_GPU_ACCESS_MIU_SETUP
#endif

#ifdef CONFIG_MSTAR_NS_UBOOT
    ldr x0, =_ld_TEE_LDR_run_base
    ldr x1, =_ld_TEE_LDR_run_end
    bl BOOT_SEAL_SetSecureRange
#endif

    .extern _boot_flow_mode
    ldr     x25, =_boot_flow_mode
    ldr     w26, [x25]
    and     w26, w26 , #0xff

    cmp     w26, #1
    b.ne     _Exit

#if defined(CONFIG_NO_RELOAED_HASH2)
    RREG     w0, 0x1F001CDC
    cmp      w0, #0x100
    beq      NO_RELOAD_HASH2    //check AC on or DC on , if DC on run NO_RELOAD_HASH2
    cmp      w0, #0x200
    beq      NO_RELOAD_HASH2    //check AC on or DC on , if DC on run NO_RELOAD_HASH2
#endif
    BOOTROM_CopyBin
    nop

NO_RELOAD_HASH2:
    bl    BOOTROM_C_GetHashTwoInfo
    nop

    # Clean intermediate data
    bl     _Exit
    nop

#-------------------------------------------------------------------------------------------------
# _Exit
# @param  None
# @return None
# @note   Clear intermediate data
#-------------------------------------------------------------------------------------------------
_Exit:
    # LDR stack pointer is trusted and set
    ldr     x25,     =_var_LDR_stack_end
    ldr     x26,    [x25]
    mov     sp,     x26

    # Load HASH2 entry point to $2
    ldr     x25,     =_var_LDR_entry_point
    ldr     x27,     [x25]

    # jump to HASH2
    br  x27
    nop

#-------------------------------------------------------------------------------------------------
# BOOTROM_JUMP_DUMMY
# @param  None
# @return None
# @note   CPU jump to dummy code to let SPI controller re-send command & address (SPI clk changed)
#-------------------------------------------------------------------------------------------------
BOOTROM_JUMP_DUMMY:
    nop
    ret
