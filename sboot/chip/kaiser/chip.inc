################################################################################
#
# Copyright (c) 2008-2011 MStar Semiconductor, Inc.
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
# BIULD
#-------------------------------------------------------------------------------------------------
#-------------------------------------------------------------------------------------------------
# Init setup
#-------------------------------------------------------------------------------------------------
# UART 0 = 1: enable UART 0  Disable UART 2
# UART 0 = 0: enable UART 2  Disable UART 0
#-------------------------------------------------------------------------------------------------
UART0           = 1
#-------------------------------------------------------------------------------------------------
# CHIP registers
#-------------------------------------------------------------------------------------------------

REG_ADDR_BASE           = 0x1F000000
CLK_GEN0_BASE           = 0x1F201600

R_REG_WDT_MAX_LO        = 0x1F006010
R_REG_WDT_MAX_HI        = 0x1F006014
R_REG_WDT_CLR           = 0x1F006000

REG_CHIP_CONFIG_OVERWRITE              = 0x1F001C7C

REG_CHIP_CONFIG_SBUS0 = 0x9
REG_CHIP_CONFIG_SBUS1 = 0xa
REG_CHIP_CONFIG_DBUS0 = 0xb
REG_CHIP_CONFIG_DBUS1 = 0xc

REG_CHIP_CONFIG_VALUE_SHIFT = 10
REG_CHIP_CONFIG_VALUE_LEN   = 5


#-------------------------------------------------------------------------------------------------
# Flash registers
#-------------------------------------------------------------------------------------------------


#-------------------------------------------------------------------------------------------------
# Other registers
#-------------------------------------------------------------------------------------------------


#-------------------------------------------------------------------------------------------------
# UART registers
#-------------------------------------------------------------------------------------------------

#-------------------------------------------------------------------------------------------------
# SecureChip registers
#-------------------------------------------------------------------------------------------------

R_CA_BASE               = 0x1F227400
R_CA_SBOOT              = (R_CA_BASE + (0x17 << 2))

R_CA_POSTMASK           = (R_CA_BASE + 0x28 << 2)
R_CA_DBGMODE            = (R_CA_BASE + 0x42 << 2)

CA_SBOOT_ACTIVE         = 0xF000
CA_POSTMASK_ENABLE      = 0x0004
CA_DBGMODE_ACTIVE       = 0x8000


#-------------------------------------------------------------------------------------------------
# Macros
#-------------------------------------------------------------------------------------------------

#-------------------------------------------------
#Macro: WREG_B
#-------------------------------------------------
.macro WREG_B reg, value
LDR     r12, =\reg
LDR     r11, =\value
STRB    r11, [r12]
.endm

#-------------------------------------------------
#Macro: WREG
#-------------------------------------------------
.macro WREG reg, value
LDR     r12, =\reg
LDR     r11, =\value
STR     r11, [r12]
    b 1f
    .ltorg
1:
.endm

#-------------------------------------------------
#Macro: RREG
#-------------------------------------------------
.macro RREG cpu_reg, mstar_reg
LDR     r12, =\mstar_reg
LDR     \cpu_reg, [r12]
.endm

#-------------------------------------------------
#Macro: REG_C2M //write cpu register to mstar register
#-------------------------------------------------
.macro REG_C2M cpu_reg, mstar_reg
LDR     r12, =\mstar_reg
STR     \cpu_reg, [r12]
.endm

#-------------------------------------------------
#Macro: DBGREG
#-------------------------------------------------
.macro WREG_DBG value
    WREG    0x1F206700, \value
.endm

#-------------------------------------------------
#Macro: MEMCPY32
#src(r10), dst(r11), len(r12) should be 32bit align
#-------------------------------------------------
.macro MEMCPY32
MEMCPY32_\@:
    LDR     r9, [r10], #4
    STR     r9, [r11], #4
    SUBS    r12, r12,  #4
    BNE     MEMCPY32_\@
.endm
#-------------------------------------------------------------------------------------------------
# Macro CHIP_SWITCH_SPI_FAST_MODE
# @note Switch SPI to fast mode
#-------------------------------------------------------------------------------------------------

	.macro CHIP_SWITCH_SPI_FAST_MODE

        #<<CLKGEN_PM>>
        WREG (REG_ADDR_BASE + (0x0008E2 <<1 )) , 0x0FFF     # CSZ clock adjust
        WREG (REG_ADDR_BASE + (0x0008E4 <<1 )) , 0x0001     # enable fast Mode
	.endm
#-------------------------------------------------------------------------------------------------
# Macro CHIP_InitDSPRAM
# @note Initial Kaiser SPRAM
#-------------------------------------------------------------------------------------------------

.macro CHIP_InitDSPRAM
      WREG (REG_ADDR_BASE + (0x100b3E  <<1 )), 0x001C        // Enable MIUPLL, 300MHz
.endm
#if 1
#-------------------------------------------------------------------------------------------------
# Macro CHIP_PowerOnChip
# @note Power On Sequence for Chip
#-------------------------------------------------------------------------------------------------

        .macro  CHIP_PowerOnChip
CHIP_PowerOnchip:
#include "script_MainPll_ARMPLL_UPLL_otp.inc"
@=======================================================================================================
@ MIUPLL
@=======================================================================================================
    WREG    0x1F221880 0x0003	@ Enable PLL
                                @`RIU_W((`RIUBASE_ANA_MISC_MIU>>1)+7'h01, 2'b11, 16'h3200);
                                @ [15:8] loop_div_second(LDS), default 0x32 = 300 MHz
                                @ clock = 24*LDS/4

    @WREG    0x1F221884 0x2800	@ Clock = 24*40/4 = 240 MHz
    WREG    0x1F221884 0x3200	@ Clock = 24*50/4 = 300 MHz

@NAND
    WREG 0x1F201790 0x0024   @ clk_fice = 54M

@CA
    WREG 0x1F206690 0x0000   @ clk_nsk = 123M, clk_cftop = 48M
                            @ clk_sm_ca = 27M, clk_sm_ca2 = 27M
        .endm
#endif
#-------------------------------------------------------------------------------------------------
# Macro CHIP_SET_MIU_CLOCK
#-------------------------------------------------------------------------------------------------

        .macro CHIP_SET_MIU_CLOCK
        
        .endm

#-------------------------------------------------------------------------------------------------
# Macro CHIP_ResetCHIP
# @note MIU initialization
#-------------------------------------------------------------------------------------------------

        .macro CHIP_ResetCHIP
#CHIP_ResetCHIP:


        # watchdog chip reset
        WREG    R_REG_WDT_MAX_LO,       1
        WREG    R_REG_WDT_MAX_HI,       0
        WREG    R_REG_WDT_CLR,          1
        DELAYMS 100
        .endm


#-------------------------------------------------------------------------------------------------
# Macro CHIP_InitUART
# Macro PUTC_Char
# @note UART output function
#-------------------------------------------------------------------------------------------------

        .macro   CHIP_InitUART
#CHIP_InitUART:

        WREG        0x1F20164C  0x0C01
.if (UART0==1)
    #enable reg_uart_rx_enable
    RREG        r0,     0x1F001c24
    orr         r0,     r0,#0x0800
    REG_C2M     r0,     0x1F001c24

    #Select to PIU_UART0
    RREG        r0,     0x1F203d4c
    orr         r0,     r0,         #0x0004
    REG_C2M     r0,     0x1F203d4c
.endif

.if (UART0==0)
##   Select UART2 source
    WREG        0x1F203d4c  0x0400
    WREG        0x1F203d50  0x0000
    #10: RX2/TX2 uses PAD_DDCR_DA2/PAD_DDCR_CK2
    RREG        r0,         0x1F203c08
    orr         r0,         r0,     #0x0800
    REG_C2M     r0,         0x1F203c08

    RREG        r0,         0x1F203c08
    and         r0,         r0,     #0xFBFF
    REG_C2M     r0,         0x1F203c08
.endif

    RREG        r0,         0x1F201318   @ Divisor Latch Access;
    orr         r0,         r0,     #0x0080       @ 1: The divisor latches can be accessed
    REG_C2M     r0,         0x1F201318

    WREG        0x1F201300  UART_DIVISOR_L
    WREG        0x1F201308  UART_DIVISOR_H

    RREG        r0,         0x1F201318    @ Kernel need

    bic         r0,         r0,     #0x0080

    REG_C2M     r0,         0x1F201318

    WREG        0x1F201310  0x0000        @ Kernel need
    WREG        0x1F201310  0x0007

    WREG        0x1F201318  0x0000        @ Kernel need
    WREG        0x1F201318  0x0003
    WREG        0x1F201320  0x0000        @ Kernel need
    WREG        0x1F201308  0x0000        @ Kernel need


    PUTK 'U'
    PUTK 'A'
    PUTK 'R'
    PUTK 'T'
    PUTK '_'
    PUTK '1'
    PUTK '1'
    PUTK '5'
    PUTK '2'
    PUTK '0'
    PUTK '0'
        .endm
#-------------------------------------------------------------------------------------------------
# Macro CHIP_WDTDisable
# @note Disable WDT
#-------------------------------------------------------------------------------------------------
        .macro  CHIP_WDTDisable
#CHIP_WDTDisable:
        # Disable Watchdog Timer
        WREG    R_REG_WDT_MAX_LO, 0
        WREG    R_REG_WDT_MAX_HI, 0
        .endm


#-------------------------------------------------------------------------------------------------
# Macro CHIP_DbgActive
# @note Activate DBG
#-------------------------------------------------------------------------------------------------
        .macro  CHIP_DbgActive
#CHIP_DbgActive:


        .endm
#-------------------------------------------------------------------------------------------------
# Macro CHIP_IsL2Enable
# @param  None
# @return 0 L2 is disabled
# @return 1 L2 is enabled
#-------------------------------------------------------------------------------------------------
        .macro CHIP_IsL2Enable reg
        # L2 Enabled in HASH1, always return TRUE
        li      \reg, 1
        .endm


#-------------------------------------------------------------------------------------------------
# Macro CHIP_EnableL2Cache
# @param  None
# @return None
# @note   Enable L2 Cache
#-------------------------------------------------------------------------------------------------
        .macro CHIP_EnableL2Cache
#CHIP_EnableL2Cache:
        # Move to HASH1 (in the param.c)
        .endm

#-------------------------------------------------------------------------------------------------
# CHIP_EnableFPU
# @param  None
# @return None
# @note   Enable FPU
#-------------------------------------------------------------------------------------------------
        .macro CHIP_EnableFPU
#CHIP_EnableFPU:

        .endm
        
@-------------------------------------------------
@Macro: DELAYUS
@-------------------------------------------------
.macro DELAYUS us
#if 0
    LDR     r12, =\us
DELAYUS_\@:
    SUBS    r12, #1
    BNE     DELAYUS_\@
#else
    LDR     r10, =(\us*12) & 0x0000FFFF
    LDR     r11, =0x1F006048
    STR     r10, [r11]
    LDR     r10, =(\us*12) >> 16
    LDR     r11, =0x1F00604C
    STR     r10, [r11]
    LDR     r10, =0x1F006040
    LDR     r11, [r10]
    LDR     r11, =0x00000002
    STR     r11, [r10]
DELAYUS_\@:
    LDR     r10, =0x1F006044
    LDR     r11, [r10]
    SUBS    r11, r11, #0x00000001
    BNE     DELAYUS_\@
#endif
.endm

