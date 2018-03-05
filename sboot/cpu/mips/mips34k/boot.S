################################################################################
#
# Copyright (c) 2008-2010 MStar Semiconductor, Inc.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# MStar Semiconductor Inc. and be kept in strict confidence
# ("MStar Confidential Information") by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of MStar Confidential
# Information is unlawful and strictly prohibited. MStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#
#
#  scs
#  1. Jump to BOOT right after postmask.
#  2. Only disable cache when before running loader.
#  3. DbgActive at BOOT for CPU exception
#
#  sboot Requirement
#  1. No BOOTROM calls BOOT except fixed address exception.
#
#
################################################################################

#include "boot.inc"
        .include "chip.inc"
        .include "version.inc"

        .text
        .set    noreorder

        .global BOOT_Entry


#-------------------------------------------------------------------------------------------------
# BOOT_Entry:
# @param  None
# @return None
# @note   Entry point of Boot code
#-------------------------------------------------------------------------------------------------
BOOT_Entry:
        b       BOOT_Start
        nop

        .word   BUILD_DATE
        .word   BUILD_TIME
        .word   BUILD_VERSION

BOOT_Start:

        # Chip Config Check , move to chip.inc for different chip
        CHIP_CONFIG_CHECK
        
        WREG_INIT

        CHIP_WDTDisable

        WREG_DBG 0x0000

        # Initialize Regs
        CPU_Init

        WREG_DBG 0x0001

        # Initialize Cache
        #CPU_InitIC

        #CPU_InitDC

        WREG_DBG 0x0002

.if (CHIP_KRIS!=1) && (CHIP_KAYLA!=1)
        # Intialize DSPRAM
        CPU_InitDSPRAM
.endif        
        nop

        WREG_DBG 0x0003

        # Jump to BOOTROM_Entry_Main
        la      $31,    BOOTROM_Entry_Main
        jr      $31
        nop



#-------------------------------------------------------------------------------------------------
# BOOT_Exception & BOOT_Fail
# @param  None
# @return None
# @note   Exception handler for general exception or decompression failure at boot code
#-------------------------------------------------------------------------------------------------
        .global BOOT_Fail
        .global CPU_Exception
        .org    0x0380 #0xBFC00380 if StatusBEV=1 StatusEXL=0 CauseIV=0
BOOT_Fail:
CPU_Exception:

        WREG_INIT

        WREG_DBG 0x000F

        PUTC ' '
        PUTC 'F'
        PUTC 'A'
        PUTC 'I'
        PUTC 'L'

        # Enable debug mode
        CHIP_DbgActive

        # Wait to reset
        ori     $8,     $0, 6
2:
        DELAYMS 1000
        sub     $8,     1
        bnez    $8,     2b
        nop

        # Reset chip
        CHIP_ResetCHIP

BOOT_Fail_NO_RESET:
        b       .
        nop

#-------------------------------------------------------------------------------------------------
# BOOT_Clear
# @param  None
# @return None
# @note   Clear all intermediate data
#-------------------------------------------------------------------------------------------------
        .global BOOT_Clear
BOOT_Clear:

        # Invalidate Cache                                  # ( $2 should not be touch )
        CPU_InitIC

        CPU_FlushDC

        # Sync Data
        sync
        nop
        nop
        nop

        jr      $31
        nop


#-------------------------------------------------------------------------------------------------
# BOOT_Halt4tester
# @param  None
# @return None
# @note
#-------------------------------------------------------------------------------------------------
        .global BOOT_Halt4tester
BOOT_Halt4tester:
        b .
        nop

