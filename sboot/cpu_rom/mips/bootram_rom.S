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

#ifndef _ArchDefs_h_
#include "ArchDefs.h"
#endif

#define RIUBASE_PM              0xBF000000
#define RIUBASE_NONPM           0xBF200000
#define RIUBASE_NONPM_EXT12     0xBF240000  #//Support Bank 0x12XX

    .text
    .set        noreorder

#-------------------------------------------------------------------------------------------------
# BOOTRAM_Entry:
# @param  None
# @return None
# @note   Entry point of BootRam code
#-------------------------------------------------------------------------------------------------
    .globl      BOOTRAM_ROM_Entry
BOOTRAM_ROM_Entry:

    DBGREG       0x1000
	
    #// Enable 8051 to access MIU for MStar TV Tool
    bal         BootRomEnableAccessMiu
    nop
	
    #//Configure PAD top for MUX and GPIO (e.g. DDR power)
	la           a3, MDrv_WriteRegMaskTbl
    lui          a0, %hi(padInitTbl + 10)
    ori          a0, %lo(padInitTbl + 10)
    jal          a3
    nop
	
	
    # Copy data section from sram to dram
    bal          BOOTRAM_CopyData
    nop

	DBGREG       0x0005

    # Clear bss section
    bal          BOOTRAM_InitBss
    nop

    #//Load chunker header
    la           a3, drvNAND_LoaduBoot
    la           a0, CONFIG_UBOOT_LOADADDR
    la           a1, 0x400
    jal          a3
    nop
	

#ifdef CONFIG_MSTAR_AUTHEN_ENABLE
    #//Authenticate chunk header
    la           a0, CONFIG_UBOOT_LOADADDR
    la           a1, 0x2f0                 
    la           a2, CONFIG_UBOOT_LOADADDR + 0x300
    la           a3, 0x0

    la           t3, MDrv_AESDMA_SecureMain
    jal          t3
    nop
    
    #//If authentication fail, jump to reset whole system
    bne         v0, 0x0001, BootRam_AuthFailLoop
    nop
#endif

    #//Load Uboot
    la           a0,CONFIG_UBOOT_LOADADDR
    lw           a1, (a0)               #// UBOOT_ROM_START in MBOOT PARTITION
    lw           a2, 4(a0)              #// UBOOT_RAM_START
    lw           a3, 8(a0)              #// UBOOT_RAM_END
    sub          t0, a3, a2             #// UBOOT_RAM_END - UBOOT_RAM_START

    la           a3, drvNAND_LoaduBoot
    sub          a0, a2, a1
    add          a1, t0, a1
	
#ifdef CONFIG_MSTAR_AUTHEN_ENABLE
    #//Increment load code size to get secure info.
    addi         a1, a1, 0x200                    #// LOAD SIZE
#endif

    jal          a3
    nop

    PUTC         'U'

#ifdef CONFIG_MSTAR_AUTHEN_ENABLE
    #//Authenticate U-BOOT
    la           a0, CONFIG_UBOOT_LOADADDR
    lw           a2, 4(a0)                         #// UBOOT_RAM_START
    lw           a3, 8(a0)                         #// UBOOT_RAM_END
    sub          a1, a3, a2                        #// UBOOT_RAM_END - UBOOT_RAM_START
    la           a2, CONFIG_UBOOT_LOADADDR + 0x10
    add          a2, a2, a1
    la           a3, 0x0


    la           t3, MDrv_AESDMA_SecureMain
    jal          t3
    nop
    
    #//If authentication fail, jump to reset whole system
    bne         v0, 0x0001, BootRam_AuthFailLoop
    nop
#endif
    #//Jump to The Entry Point of UBoot
    la           a0, CONFIG_UBOOT_LOADADDR - 0x400 #// Chunker header
    lw           a1, 16(a0)                        #// UBOOT_RAM_ENTRY

	jr           a1
    nop
#-------------------------------------------------------------------------------------------------
# BootRom_AuthFailLoop
# @param  None
# @return None
# @note   Fail of authentication
#-------------------------------------------------------------------------------------------------	
#ifdef CONFIG_MSTAR_AUTHEN_ENABLE
BootRam_AuthFailLoop:
    PUTC        'F'
    DELAYUS     100000
    b           BootRam_AuthFailLoop
    nop
#endif

BOOTRAM_CopyUboot:
#//-------------------------------------------------------------------------------------------------
#// Copy Data Section to DRAM
#//-------------------------------------------------------------------------------------------------
    la	        t3, 0xa75f0180
    la	        t4, 0x875f0180
    la	        t5, 0x200000

_CopyData:
    lw          t6, 0(t3)
    sw          t6, 0(t4)

    add         t3, t3, 4
    add         t4, t4, 4
    add         t5, t5, -4

    bnez        t5, _CopyData
    nop

    jr          ra
    nop
#-------------------------------------------------------------------------------------------------
# BOOTRAM_CopyData
# @param  None
# @return None
# @note   Power On Sequence for Chip
#-------------------------------------------------------------------------------------------------
BOOTRAM_CopyData:
#//-------------------------------------------------------------------------------------------------
#// Copy Data Section to DRAM
#//-------------------------------------------------------------------------------------------------
    la	        t3, _end_address_text_section
    la	        t4, _start_address_data_section
    la	        t5, _size_data_section
    beqz        t5, _CopyDataToSramEnd

_CopyDataToSram:
    lw          t6, 0(t3)
    sw          t6, 0(t4)

    add         t3, t3, 4
    add         t4, t4, 4
    add         t5, t5, -4

    bnez        t5, _CopyDataToSram
    nop

_CopyDataToSramEnd:
    jr          ra
    nop

LEAF(BootRomEnableAccessMiu)

        WREG        0xBF005788, 0x0004      #//Set XDATA2MIU read Priority
        WREG        0xBF00578C, 0x3c11      #//Set the low bound address of MCU XDATA mapping to MIU (Unit: 1K-Byte)
        WREG        0xBF005790, 0x0010      #//Set the low byte address to access xdata from MIU. The granularity is 64k bytes
        WREG        0xBF005794, 0x403c      #//Set the low bound address of MCU XDATA mapping to MIU (Unit: 1K-Byte)
        WREG        0xBF005798, 0x0101      #//Set the low byte address to access xdata from MIU. The granularity is 4k bytes.

        jr          ra
        nop

END(BootRomEnableAccessMiu)
#-------------------------------------------------------------------------------------------------
# BOOTRAM_InitBss
# @param  None
# @return None
# @note   Power On Sequence for Chip
#-------------------------------------------------------------------------------------------------
BOOTRAM_InitBss:
#//-------------------------------------------------------------------------------------------------
#// Clear BSS Section in DRAM
#//-------------------------------------------------------------------------------------------------
    la	        t3, _start_address_bss_section
    la	        t4, _size_bss_section
    li	        t5, 0
    beqz        t4, _InitBssLoopEnd

_InitBssLoop:
    sw          t5, 0(t3)

    add         t3, t3, 4
    add         t4, t4, -4

    bnez        t5, _InitBssLoop
    nop

_InitBssLoopEnd:
    jr          ra
    nop

#-------------------------------------------------------------------------------------------------
# BOOTRAM_Pad
# @param  None
# @return None
# @note   Pad with zeros (Reserve SIZE_BOOTRAM_RESERVED Bytes for bootram)
#-------------------------------------------------------------------------------------------------
    .org        (SIZE_BOOTRAM_RESERVED-4)
    .word       0
