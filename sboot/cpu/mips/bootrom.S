################################################################################
#
# Copyright (c) 2008-2012 MStar Semiconductor, Inc.
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


#include "boot.inc"
#include "chip.inc"

        .text
        .set    noreorder

        .global BOOTROM_Entry_Main

        .extern _ld_STACK_addr

#-------------------------------------------------------------------------------------------------
# BOOTROM_Main:
# @param  None
# @return None
# @note   Entry point of Boot code
#-------------------------------------------------------------------------------------------------
        .org    0x0000                                      # Entry 0
BOOTROM_Entry_Main:
BOOTROM_Main:

        WREG_INIT

        WREG_DBG 0x0010
#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
        # Switch fast mode
        CHIP_SWITCH_SPI_FAST_MODE

        # Dummy jump
        bal     BOOTROM_JUMP_DUMMY
        nop

        # Power On Sequence
        CHIP_PowerOnChip

        WREG_DBG 0x0011
        
#endif
        # Initialize Cache
        CPU_InitIC

        CPU_InitDC

        # Enable cache
         CPU_EnableCache

        # Enable L2 cache
         CHIP_EnableL2Cache

        # Enable FPU
        # CHIP_EnableFPU #it will be init later in the flow
        # Init UART
        CHIP_InitUART

        PUTC '\r'
        PUTC '\n'
        PUTC 'B'
        WREG_DBG 0x0012

        nop

BOOTROM_RAMLoader:
        # HW configuration
        # Do STACK initilization here (coz. SCS_Disable)

        WREG_DBG 0x0013

        PUTC 'O'
        PUTC 'O'
        PUTC 'T'

        # InitMIU
        la      $sp,    _ld_STACK_end

#if defined(CONFIG_MSTAR_VID_ENABLE)
#if defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN)
        jal     BOOTROM_C_InitVID
        nop
#endif
#endif        
        # check for SPI existance
        jal     HAL_SERFLASH_ReadID
        nop

#if  ((!defined(CONFIG_MSTAR_KELTIC)) && (!defined(CONFIG_MSTAR_KAPPA)) && (!defined(CONFIG_MSTAR_KRITI)))
        # check return value
        beqz    $2, BOOTROM_TO_NAND
#endif
        PUTC 'S'
        PUTC 'P'
        PUTC 'I'

        jal     BOOTROM_C_InitMIU_SPI;
        nop


#if defined(CONFIG_NO_RELOAED_HASH2)
        RREG    $t3, 0xBF001CDC
        and     $t3, $t3, 0xFFFF
        add     $t4, $zero, 0x0100
        beq     $t3, $t4, NO_RELOAD_HASH2    //check AC on or DC on , if DC on run NO_RELOAD_HASH2
        nop
#endif
        bal      BOOTROM_CopyBin
        nop

NO_RELOAD_HASH2:
        # goto exit position
        j BOOTROM_JUMP_TO_EXIT
        nop
#if  ((!defined(CONFIG_MSTAR_KELTIC)) && (!defined(CONFIG_MSTAR_KAPPA) ) && (!defined(CONFIG_MSTAR_KRITI)))
BOOTROM_TO_NAND:

        PUTC 'N'
        PUTC 'A'
        PUTC 'N'
        PUTC 'D'

        CPU_DisCache

        jal     BOOTROM_C_InitMIU_NAND;
        nop
        
#endif

BOOTROM_JUMP_TO_EXIT:

        #Set miu clock
        CHIP_SET_MIU_CLOCK

        WREG_DBG 0x0017

        PUTC '_'
        PUTC 'O'
        PUTC 'K'

        # check for SPI existance
        jal     HAL_SERFLASH_ReadID
        nop

        # check return value
        beqz    $2, _Exit_NAND


        WREG_DBG 0x0018

        jal     BOOTROM_C_GetHashTwoInfo
        nop

        WREG_DBG 0x0019

        # Clean intermediate data
        jal     _Exit                                       # ( $2 should not be touched )
        nop


#-------------------------------------------------------------------------------------------------
# BOOTROM_JUMP_DUMMY
# @param  None
# @return None
# @note   CPU jump to dummy code to let SPI controller re-send command & address (SPI clk changed)
#-------------------------------------------------------------------------------------------------
BOOTROM_JUMP_DUMMY:
        nop
        jr      $31
        nop

#-------------------------------------------------------------------------------------------------
# BOOTROM_CopyBin
# @param  None
# @return None
# @note   Copy LDR & APP from ROM to DRAM (DMA or CPU)
#-------------------------------------------------------------------------------------------------
BOOTROM_CopyBin:

        la      $8,     _ld_LDR_load_start
        lw      $9,     _var_LDR_run_base
        lw      $10,    _var_LDR_size
        add     $10,    $8, $10

1:
        ld      $11,    0($8)                               # ld=(lw lw) to save time
        ld      $13,    8($8)
        sd      $11,    0($9)
        sd      $13,    8($9)
        addu    $8,     $8, 16
        addu    $9,     $9, 16
        bltu    $8,     $10, 1b
        nop

        jr      $31
        nop

#-------------------------------------------------------------------------------------------------
# _Exit
# @param  None
# @return None
# @note   Clear intermediate data
#-------------------------------------------------------------------------------------------------
_Exit:
        # LDR stack pointer is trusted and set
        lw      $8,     _var_LDR_run_base
        lw      $sp,    LDR_HDR_STACK($8)

        # Load HASH2 entry point to $2
        lw      $2,     _var_LDR_entry_point

        # Clear SPRAM and cache
        la      $31,    BOOT_Clear
        jal     $31
        nop

        PUTC '!'

        # Load HASH2 entry point from $2
        move    $31, $2

        # jump to HASH2
        jr      $31
        nop


#-------------------------------------------------------------------------------------------------

#-------------------------------------------------------------------------------------------------
# _Exit_NAND
# @param  None
# @return None
# @note   Clear intermediate data
#-------------------------------------------------------------------------------------------------
_Exit_NAND:
        # LDR stack pointer is trusted and set
        la       $sp,    _ld_LDR_stack_end

        # Load HASH2 entry point to $2
        la      $2,     _ld_LDR_run_start

        # Clear SPRAM and cache
        la      $31,    BOOT_Clear
        jal     $31
        nop

        PUTC '!'

        # Prepare post-mask range
        lui     $8,     R_CA_POSTMASK >> 16
        ori     $8,     R_CA_POSTMASK & 0xFFFF
        lw      $9,     0($8)
        ori     $9,     CA_POSTMASK_ENABLE

        # Prepare enable debug mode
        li      $10, R_CA_DBGMODE
        lw      $11, 0($10)
        ori     $11, CA_DBGMODE_ACTIVE

        # Prepare disble cache
        li      $12,     INITIAL_CONFIG0

        # Load HASH2 entry point from $2
        move    $31, $2

        # Assume cache line size > 16bytes
        .balign 32
        # Enable post-mask
        sw      $9, 0($8)
        # Enable debug mode
        sw      $11, 0($10)

        # jump to HASH2
        jr      $31
        # Disble cache
        mtc0    $12,     $16

#-------------------------------------------------------------------------------------------------


# Local variable
#-------------------------------------------------------------------------------------------------
        .bss                                                # All data will be lost after BOOT_Clear
        .global _var_LDR_run_base
        .global _var_LDR_size
        .global _var_LDR_entry_point
_var_LDR_run_base:
        .word   0xDEADBEEF
_var_LDR_size:
        .word   0xDEADBEEF
_var_LDR_entry_point:
        .word   0xDEADBEEF

