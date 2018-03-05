################################################################################
#
# Copyright (c) 2006-2010 MStar Semiconductor, Inc.
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

    .text
    .set        noreorder

#-------------------------------------------------------------------------------------------------
# BOOT_Entry:
# @param  None
# @return None
# @note   Entry point of Boot code
#-------------------------------------------------------------------------------------------------
    .globl      BOOTROM_Entry
BOOTROM_Entry:

# Switch fast mode
    bal         CHIP_SWITCH_SPI_FAST_MODE
    nop    

# Dummy jump
    bal         BOOTROM_JUMP_DUMMY
    nop

    DBGREG      0x0003

    #// Pre-Trimming Sequence
    bal         BootRomPreTrimSequence
    nop

    DBGREG      0x0004

    #// Power-On Sequence
    bal         BootRomPowerOnSequence
    nop

    DBGREG      0x0005

    #// Post-Trimming Sequence
    bal         BootRomPostTrimSequence
    nop

    DBGREG      0x0006

    # set stack pointer
    la          sp,  _stack_pointer     #//Set stack pointer

    DBGREG      0x0007

    la			a0, CONFIG_BOOTRAM_LOADADDR
    la			a1, 0x8000
    bal			drvNAND_Load2ndBL
    nop

    DBGREG      0x0008

#ifdef CONFIG_MSTAR_AUTHEN_ENABLE
    la           a0, CONFIG_BOOTRAM_LOADADDR
    la           a1, 0x2000                 
    la           a2, CONFIG_BOOTRAM_LOADADDR + 0x2300
    la           a3, 0x0

    bal          MDrv_AESDMA_SecureMain
    nop
    
    #//If authentication fail, jump to reset whole system
    bne         v0, 0x0001, BootRom_AuthFailLoop
    nop
#endif

    #PUTK        'M'
    #PUTK        'S'
    #PUTK        't'
    #PUTK        'a'
    #PUTK        'r'
    #PUTK        '\r'
    #PUTK        '\n'

    #//Jump to the entry of BOOTRAM
    la          ra, BOOTRAM_ROM_Entry
    nop


#-------------------------------------------------------------------------------------------------
# BOOTROM_CopyData
# @param  None
# @return None
# @note   Power On Sequence for Chip
#-------------------------------------------------------------------------------------------------
BOOTROM_CopyData:
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

#-------------------------------------------------------------------------------------------------
# BootRom_AuthFailLoop
# @param  None
# @return None
# @note   Fail of authentication
#-------------------------------------------------------------------------------------------------	
#ifdef CONFIG_MSTAR_AUTHEN_ENABLE
BootRom_AuthFailLoop:
    #PUTK        'F'
    #DELAYUS     100000
    b           BootRom_AuthFailLoop
    nop
#endif


#-------------------------------------------------------------------------------------------------
# BOOTROM_InitBss
# @param  None
# @return None
# @note   Power On Sequence for Chip
#-------------------------------------------------------------------------------------------------
BOOTROM_InitBss:
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
# BootRomPowerOnSequence
# @param  None
# @return None
# @note   Power On Sequence for Chip
#-------------------------------------------------------------------------------------------------
BootRomPowerOnSequence:

    move        k1, ra

    #//Initial releated settings of PLL
    bal         BootRomInitPll
    nop

    DBGREG      0x0030

    #//Initial related settings of Clock
    bal         BootRomInitClock
    nop

    DBGREG      0x0031

    DELAYUS     100

    #//Initial related settings of UART
    bal         BootRom_InitPeripheral
    nop

    DBGREG      0x0032

    #//Initial related settings of cache
    bal         BootRomInitCache
    nop

    DBGREG      0x0033


#ifdef CONFIG_MSTAR_STR_ENABLE
    DBGREG      0xDDDD
    #//IS_AC_FLOW_CONTROL
    RREG        t3, 0xBF001CDC
    and         t3, t3, 0xFFFF
    add         t4, zero, 0x0100
    bne         t3, t4, BootRom_AcOn_Flow
    nop
BootRom_DcOn_Flow:

    #//add the BL for miusetting
    la          a0, _ld_program_reload_addr
    la          a1, _ld_miu_str_flow_flash_start
    la          a2, 0x1400
    bal         drvNAND_LoadBLOffset
    nop


#ifdef CONFIG_MSTAR_AUTHEN_ENABLE
    la           a0, _ld_program_reload_addr
    la           a1, 0x1000                 
    la           a2, _ld_program_reload_addr + 0x1300
    la           a3, 0x1

    bal          MDrv_AESDMA_SecureMain
    nop
    
    #//If authentication fail, jump to reset whole system
    bne         v0, 0x0001, BootRom_AuthFailLoop
    nop
#endif


    bal         BootInitMiu_STR
    nop
    DBGREG      0x0135

    bal         Boot_StrEntry
    nop
	
    j           BootRom_Exit_Refresh
    nop
    


BootRom_AcOn_Flow:

#endif

    #//add the BL for miusetting
    la          a0, _ld_program_reload_addr
    la          a1, _ld_miu_flow_flash_start
    la          a2, 0x1400
    bal         drvNAND_LoadBLOffset
    nop

#ifdef CONFIG_MSTAR_AUTHEN_ENABLE	

    la           a0, _ld_program_reload_addr
    la           a1, 0x1000                 
    la           a2, _ld_program_reload_addr + 0x1300
    la           a3, 0x1

    bal          MDrv_AESDMA_SecureMain
    nop
    
    #//If authentication fail, jump to reset whole system
    bne         v0, 0x0001, BootRom_AuthFailLoop
    nop
#endif

    #//Initial related settings of MIU0
    bal     	BootInitMiu
    nop
    DBGREG      0x0035   


BootRom_Exit_Refresh:

    #//Set clock ratio of OCP
    #bal     	BootRomOcpClockRatio
    #nop

    move        ra, k1
    jr          ra
    nop

#-------------------------------------------------------------------------------------------------
# BOOT_SramToTsp  Set TSP Sram to TSP
# @param  None
# @return None
# @note   do TSP Sram init
#-------------------------------------------------------------------------------------------------
BOOT_SramToTsp:

    RREG        t9, 0xBF2039C0 #//Select tsp sram to TSP
    or          t9, t9, 0x0001
    REG_C2M     t9, 0xBF2039C0

    jr          ra
    nop


#-------------------------------------------------------------------------------------------------
# BootRomOcpClockRatio
# @param  None
# @return None
# @note   do MIPS OCP clock ratio setting
#-------------------------------------------------------------------------------------------------
#    BootRomOcpClockRatio:

#        WREG_B      0xBF203A3C, (MIPS_OCP_CLOCK_RATIO | 0x00)   #//Set clock ratio to 2:1
#        WREG_B      0xBF203A3C, (MIPS_OCP_CLOCK_RATIO | 0x01)   #//Set clock ratio change pulse

#        WREG_B      0xBF201645, 0x80      #//Switch to CLK_MIPS to mipspll_vco

#        jr          ra
#        nop

#-------------------------------------------------------------------------------------------------
# Boot_StrEntry
# @param  None
# @return None
# @note   Wake up from PM51 STR setting
#-------------------------------------------------------------------------------------------------
#ifdef CONFIG_MSTAR_STR_ENABLE
Boot_StrEntry:


    move        k0, ra
.if (MSTAR_STR_SBOOT_RESUME == 1)
    #check slept status
    bal         read_wkup_pmu
    nop
    and         t0, t0, WAKEUP_FLAG_MASK
    li          t1, WAKEUP_FLAG_SLEPT
    bne         t0, t1, step1
    nop
	
    #set wakeup    
    not         t1, t0
    and         t0, t0, t1
    or          t0, t0, WAKEUP_FLAG_WKUP	
    bal         write_wkup_pmu
    nop

    li          t3, PMU_WAKEUP_ADDR_REGL
    lw          t0, 0x0(t3)

    bal         step2
    nop
	
step1:
    xor         t0, t0, t0
    bal         write_wkup_pmu
    nop
	
step2:
    #check wakeup flag
    bal         read_wkup_pmu
    nop

    and         t2, t0, WAKEUP_FLAG_MASK 
	
    li          t1, WAKEUP_FLAG_WKUP
    bne         t2, t1, step3
    nop
    PUTK        'W'
    #PUTK        'K'
    #PUTK        'P'
	
    andi        t0, t0, WAKEUP_ADDR_MASK
    sll         t0, t0, 16             #// wake up info addr
    lw          t1, 0x0(t0)
    lw          t2, 0x4(t0)
    li          t0, MSTAR_SLEEP_MAGIC
    bne         t1, t0, step3
    nop
	
    li          t0, MIU0_VIRTBASE1
    add         t2, t2, t0

    #PUTK        'J'
    beqz        t2, step3
    nop

    #PUTK        'M'
    jr          t2                      #// jump to wakup addr
    nop
	
step3:
.endif
    move        ra, k0
    jr          ra
    nop
	
#endif

#-------------------------------------------------------------------------------------------------
# CHIP_SWITCH_SPI_FAST_MODE
# @param  None
# @return None
# @note   CHIP_SWITCH_SPI_FAST_MODE
#-------------------------------------------------------------------------------------------------
LEAF(CHIP_SWITCH_SPI_FAST_MODE)

    WREG        (0xBF000000 + (0x0017E2 << 1)), 0x0FFF      # CSZ clock adjust
    WREG        (0xBF000000 + (0x0017E4 << 1)), 0x0001      # enable fast Mode
    jr          ra
    nop

END(CHIP_SWITCH_SPI_FAST_MODE)

#-------------------------------------------------------------------------------------------------
# BOOTROM_JUMP_DUMMY
# @param  None
# @return None
# @note   BOOTROM_JUMP_DUMMY
#-------------------------------------------------------------------------------------------------
LEAF(BOOTROM_JUMP_DUMMY)
    nop
    jr          ra
    nop

END(BOOTROM_JUMP_DUMMY)

#-------------------------------------------------------------------------------------------------
# Check resume status
# @param  None
# @return None
# @note   read_wkup_pmu/write_wkup_pmus
#-------------------------------------------------------------------------------------------------
#ifdef CONFIG_MSTAR_STR_ENABLE
.if (MSTAR_STR_SBOOT_RESUME == 1)
read_wkup_pmu:
    li          t2, PMU_WAKEUP_ADDR_REGL
    lw          t0, 0x0(t2)

    li          t2, PMU_WAKEUP_ADDR_REGH
    lw          t1, 0x0(t2)

    andi        t0, t0, PMU_WAKEUP_ADDR_LMASK
    andi        t1, t1, PMU_WAKEUP_ADDR_HMASK
    or          t0, t0, t1
    jr          ra
    nop


write_wkup_pmu:
    li          t3, PMU_WAKEUP_ADDR_REGL
    lw          t1, 0x0(t3)
    andi        t1, t1, PMU_WAKEUP_ADDR_HMASK
    andi        t2, t0, PMU_WAKEUP_ADDR_LMASK
    or          t1, t1, t2
    sw          t1, 0x0(t3)

    li          t3, PMU_WAKEUP_ADDR_REGH
    lw          t1, 0x0(t3)
    andi        t1, t1, PMU_WAKEUP_ADDR_LMASK
    andi        t2, t0, PMU_WAKEUP_ADDR_HMASK
    or          t1, t1, t2
    sw          t1, 0x0(t3)
    jr          ra
    nop

.endif
#endif
