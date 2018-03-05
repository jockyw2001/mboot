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
################################################################################


#-------------------------------------------------------------------------------------------------
# Initialize Regs & Initialize Cache
#-------------------------------------------------------------------------------------------------

M_DebugCOUNTDM          = (0x1  << 25)

M_StatusCU              = (0xf  << 28)
M_StatusRP              = (0x1  << 27)
M_StatusRE              = (0x1  << 25)
M_StatusBEV             = (0x1  << 22)
M_StatusTS              = (0x1  << 21)
M_StatusSR              = (0x1  << 20)
M_StatusNMI             = (0x1  << 19)
M_StatusIM7             = (0x1  << 15)
M_StatusIM2             = (0x1  << 10)
M_StatusIM              = (0xff << 8)
M_StatusUM              = (0x1  << 4)
M_StatusERL             = (0x1  << 2)
M_StatusEXL             = (0x1  << 1)
M_StatusIE              = (0x1  << 0)

M_CauseIV               = (0x1  << 23)
M_CauseWP               = (0x1  << 22)
M_CauseIP1              = (0x1  << 9)
M_CauseIP0              = (0x1  << 8)

M_ConfigK23             = (0x7  << 28)
M_ConfigKU              = (0x7  << 25)
M_ConfigMT              = (0x7  << 7)
M_ConfigK0              = (0x7  << 0)
K_CacheAttrCN           = (0x3  << 0)   #Cacheable, noncoherent, write-back, write allocate.
S_Config1IS             = 22
M_Config1IS             = (0x7  << S_Config1IS)
S_Config1IL             = 19
M_Config1IL             = (0x7  << S_Config1IL)
S_Config1IA             = 16
M_Config1IA             = (0x7  << S_Config1IA)
S_Config1DS             = 13
M_Config1DS             = (0x7  << S_Config1DS)
S_Config1DL             = 10
M_Config1DL             = (0x7  << S_Config1DL)
S_Config1DA             = 7
M_Config1DA             = (0x7  << S_Config1DA)

INITIAL_STATUS          = 0x10400000
INITIAL_CONFIG0         = 0x00000002    #Uncached

Idx_WB_IV_D             = 0x1
Idx_L_Tag_D             = 0x5
Idx_S_Tag_I             = 0x8
Idx_S_Tag_D             = 0x9

TAG_SPRAM_EN            = 0x00000080
ERRCTL_SPR              = 0x10000000

A_DSPRAM                = 0x08000000    # 0x88000000
A_KSEG0BASE             = 0x80000000

#-------------------------------------------------------------------------------------------------
# General register usage macro
#-------------------------------------------------------------------------------------------------

        .macro  DELAYMS ms              # ms < 0x7FFFFFFF/CLK*2*1000, up to 19000ms for 216Mhz, 8500ms for 504Mhz
        li      $26,    (\ms * (CPU_CLK_FREQ/2/1000)) #ticks
        mtc0    $0,     $9              #tick1 = 0
        ehb
1:
        mfc0    $10,    $9              #tick2
        bltu    $10,    $26, 1b
        nop
        .endm

        .macro  DELAYUS us              # us < 0x7FFFFFFF/CLK*2*1000000, up to 19000ms for 216Mhz, 8500ms for 504Mhz
        li      $26,    (\us * (CPU_CLK_FREQ/2/1000000)) #ticks
        mtc0    $0,     $9              #tick1 = 0
1:
        mfc0    $10,    $9              #tick2
        bltu    $10,    $26, 1b
        nop
        .endm

        .macro  DELAYUS_12M us              # us < 0x7FFFFFFF/CLK*2*1000000, up to 19000000us
        li      $26,    (\us * (12000000/2/1000000)) #ticks
        mtc0    $0,     $9              #tick1 = 0
1:
        mfc0    $10,    $9              #tick2
        bltu    $10,    $26, 1b
        nop
        .endm

        .macro  DELAYS  second
        li      $25,    (\second * 1000)
2:        
        DELAYMS 1
        sub     $25,    1
        bnez    $25,    2b
        nop
        .endm
#-------------------------------------------------------------------------------------------------
# CPU_Init
# @param  None
# @return None
# @note   Init CPU Registers
#-------------------------------------------------------------------------------------------------
        .macro  CPU_Init
#CPU_Init:

        # CLEAR COUNT
        mtc0    $0,     $9

        # INIT CAUSE
        mtc0    $0,     $13

        # INIT STATUS
        li      $8,     INITIAL_STATUS
        mtc0    $8,     $12

        # INIT CONFIG
        li      $8,     INITIAL_CONFIG0
        mtc0    $8,     $16

        .endm


#-------------------------------------------------------------------------------------------------
# CPU_InitIC
# @param  None
# @return None
# @note   Invalide I-cache
#-------------------------------------------------------------------------------------------------
        .macro  CPU_InitIC
#CPU_InitIC:

        # Clear TagLo & TagHi
        mtc0    $0,     $28, 0                # TagLo / ITagLo
        mtc0    $0,     $28, 2                # DTagLo
        mtc0    $0,     $29, 0                # TagHi / ITagHi
        mtc0    $0,     $29, 2                # DTagHi


        # Config1
        mfc0    $8,     $16, 1

        ### INIT I-CACHE ###

        # Line Size $10
        li      $9,     M_Config1IL
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1IL
        beqz    $9,     2f                                  # no Icache
        nop
        li      $10,    2
        sll     $10,    $10, $9

        # Sets per way $11
        li      $9,     M_Config1IS
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1IS
        li      $11,    64
        sll     $11,    $11, $9

        # Associativity $12
        li      $9,     M_Config1IA
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1IA
        add     $12,    $9, 1

        li      $14,    A_KSEG0BASE
        move    $15,    $11

1:
        # Invalidate each tag entry
        cache   Idx_S_Tag_I, 0($14)
        add     $14,    $10
        sub     $15,    1
        bnez    $15,    1b
        nop
        move    $15,    $11
        sub     $12,    1
        bnez    $12,    1b
        nop

2:
        .endm


#-------------------------------------------------------------------------------------------------
# CPU_InitDC
# @param  None
# @return None
# @note   Invalidate D-cache
#-------------------------------------------------------------------------------------------------
        .macro  CPU_InitDC
#CPU_InitDC:

        # Clear TagLo & TagHi
        mtc0    $0,     $28, 0                # TagLo / ITagLo
        mtc0    $0,     $28, 2                # DTagLo
        mtc0    $0,     $29, 0                # TagHi / ITagHi
        mtc0    $0,     $29, 2                # DTagHi

        # Config1
        mfc0    $8,     $16, 1

        ### INIT D-CACHE ###

        # Line Size $10
        li      $9,     M_Config1DL
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1DL
        beqz    $9,     2f                                  # no Dcache
        nop
        li      $10,    2
        sll     $10,    $10, $9

        # Sets per way $11
        li      $9,     M_Config1DS
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1DS
        li      $11,    64
        sll     $11,    $11, $9

        # Associativity $12
        li      $9,     M_Config1DA
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1DA
        add     $12,    $9, 1

        li      $14,    A_KSEG0BASE
        move    $15,    $11

1:
        # Invalidate each tag entry
        cache   Idx_S_Tag_D, 0($14)
        add     $14,    $10
        sub     $15,    1
        bnez    $15,    1b
        nop
        move    $15,    $11
        sub     $12,    1
        bnez    $12,    1b
        nop

2:
        .endm


#-------------------------------------------------------------------------------------------------
# CPU_FlushDC                                               # ( $2 should not be touch )
# @param  None
# @return None
# @note   Flush D-cache
#-------------------------------------------------------------------------------------------------
        .macro  CPU_FlushDC
#CPU_FlushDC:

        # Config1
        mfc0    $8,     $16, 1

        ### FLUSH D-CACHE ###

        # Line Size $10
        li      $9,     M_Config1DL
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1DL
        beqz    $9,     2f                                  # no Dcache
        nop
        li      $10,    2
        sll     $10,    $10, $9

        # Sets per way $11
        li      $9,     M_Config1DS
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1DS
        li      $11,    64
        sll     $11,    $11, $9

        # Associativity $12
        li      $9,     M_Config1DA
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1DA
        add     $12,    $9, 1

        li      $14,    A_KSEG0BASE
        move    $15,    $11

1:
        # Invalidate each tag entry
        cache   Idx_WB_IV_D, 0($14)
        add     $14,    $10
        sub     $15,    1
        bnez    $15,    1b
        nop
        move    $15,    $11
        sub     $12,    1
        bnez    $12,    1b
        nop

2:
        .endm


#-------------------------------------------------------------------------------------------------
# CPU_InitDSPRAM
# @param  None
# @return None
# @note   Init Data ScratchPad RAM
#-------------------------------------------------------------------------------------------------
        .macro CPU_InitDSPRAM
        .set push
        .set at
#CPU_InitDSPRAM:

        # SRAM fixed at address 0xB0006000
        # for i from $16 to $17
        li      $15, 0xaaaabbbb
        li      $16, 0xB0006000
        li      $17, 0x2000
        addu    $17, $17, $16
1:
        sw      $15,  0($16)
        add     $16, 4
        bltu    $16, $17, 1b
        nop
        .set pop
        .endm

#-------------------------------------------------------------------------------------------------
# CPU_RemapDSPRAM
# @param  None
# @return None
# @note   Remap Data ScratchPad RAM
#-------------------------------------------------------------------------------------------------
        .macro CPU_RemapDSPRAM
        .set push
        .set at
CPU_RemapDSPRAM:
        WREG_INIT

        # Map SPRAM to 0x16000000
        WREG    (0xbf200000 + 4 * 0xC4B), 0x6000
        WREG    (0xbf200000 + 4 * 0xC4C), 0x1000
        li      $11, (0xbf200000 + 4 * (0xC49))
        lw      $12, 0($11)
        or      $12, 0x2
        sw      $12, 0($11)

        # Fire L2 Flush pipe data for remap SPRAM address
        WREG    (0xbf200000 + 4 * 0xC41), (0 << 4) | 2
        WREG    (0xbf200000 + 4 * 0xC41), (1 << 4) | 2
        WREG    (0xbf200000 + 4 * 0xC41), 0

        .set pop
				.endm
				
#-------------------------------------------------------------------------------------------------
# CPU_EnableCache
# @param  None
# @return None
# @note   Enable Cache
#-------------------------------------------------------------------------------------------------
        .macro CPU_EnableCache
#CPU_EnableCache:

        # Enable cache
        mfc0    $8,     $16
        and     $8,     (~M_ConfigK0)
        li      $8,     (K_CacheAttrCN)
        mtc0    $8,     $16
        .endm


#-------------------------------------------------------------------------------------------------
# CPU_DisCache
# @param  None
# @return None
# @note   Disable Cache and Scratchpad
#-------------------------------------------------------------------------------------------------
        .macro CPU_DisCache
#CPU_DisCache:

        # Disble cache
        li      $8,     INITIAL_CONFIG0
        ehb
        mtc0    $8,     $16
        ehb

        # Disble Scratchpad
        li      $16, A_DSPRAM
        ehb
        mtc0    $16, $28, 2          # move to DTagLo
        ehb
        .endm
