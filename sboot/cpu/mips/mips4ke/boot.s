
################################################################################
#
# Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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
################################################################################


        .include "boot.inc"

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
        j       BOOT_Start
        nop

        # Revision ID
        .org    0x0014
        .word   BOOT_VERSION

        .org    0x0018

BOOT_Start:
        WREG_DBG  0x0000

        # Initialize Regs
        CPU_Init

        WREG_DBG  0x0001

        # Initialize Cache
        #CPU_InitIC

        #CPU_InitDC

        # Enable cache
        CPU_EnableCache

        WREG_DBG  0x0002

        # Intialize DSPRAM
        CPU_InitDSPRAM                                      # Sync to Secure Boot
        nop

        WREG_DBG  0x0003

        # Jump to BOOTROM_Entry_Main
        la      $31,    BOOTROM_Entry_Main
        jr      $31
        nop




#-------------------------------------------------------------------------------------------------
# BOOT_Exception
# @param  None
# @return None
# @note   Exception handler for general exception or decompression failure at boot code
#-------------------------------------------------------------------------------------------------
        .global BOOT_Fail
        .global CPU_Exception
        .org    0x0380 #0xBFC00380 if StatusBEV=1 StatusEXL=0 CauseIV=0
BOOT_Fail:
CPU_Exception:

        WREG_DBG  0x000F

    .if 1 #DEBUG
1:
        b       1b                                          # trap here
        nop

    .else

        la      $31, CHIP_ResetCHIP
#        b       CHIP_ResetCPU
        jr      $31
        nop

    .endif

        .set    at


    .if 0 #(SUPPORT_POWER_SAVING)
        .set    noat

#-------------------------------------------------------------------------------------------------
# BOOT_CPU_DRAM_Standby
# @param  None
# @return None
# @note   Let DRAM & CPU enter standby mode
#-------------------------------------------------------------------------------------------------
        .org    0xa80      # Cannot change it, _PWR_EnterPowerSavingMode will call it.
CPU_Standby:

    .if (0)
        #
        # Power down the DRAM
        #
    .endif

        # Enable Interrupt
        mfc0    $2,     $12
        nop
        or      $2,     $2, M_StatusIE
        mtc0    $2,     $12
        nop
        nop
        nop

        # Enter MIPS power saving mode.
        wait
        nop

        jr      $31
        nop

        .set    at
    .endif



#        .org    (SIZE_BOOT_RESERVED-4)
#        .word   0

