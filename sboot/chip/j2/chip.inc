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
# CHIP registers
#-------------------------------------------------------------------------------------------------
    .if (CPU==1)
REG_ADDR_BASE           = 0xbf000000
CLK_GEN0_BASE           = 0xBF201600

R_REG_WDT_MAX_LO        = 0xbf006010
R_REG_WDT_MAX_HI        = 0xbf006014
R_REG_WDT_CLR           = 0xbf006000
    .endif

    .if (CPU==0) # aeon
    .endif

#if defined(MSTAR_MIPS_OCP_CLOCK_RATIO_1_1)
MIPS_OCP_CLOCK_RATIO            = 0x00


#-------------------------------------------------------------------------------------------------
# Flash registers
#-------------------------------------------------------------------------------------------------


#-------------------------------------------------------------------------------------------------
# Other registers
#-------------------------------------------------------------------------------------------------

#-------------------------------------------------------------------------------------------------
# UART registers
#-------------------------------------------------------------------------------------------------
CPU_CLOCK               = 240
CPU_CLK_FREQ            = (CPU_CLOCK * 1000000)
UART_CLK_FREQ           = 123000000                         #Clock source
UART_BAUDRATE           = 115200                            #Baud rate
UART_DIVISOR            = (UART_CLK_FREQ/(16*UART_BAUDRATE))#Divisor


#-------------------------------------------------------------------------------------------------
# SecureChip registers
#-------------------------------------------------------------------------------------------------

R_CA_BASE               = 0xbf245400
R_CA_SBOOT              = (R_CA_BASE + (0x17 << 2))
R_CA_JTAG_PWD0          = (R_CA_BASE + 0x24 << 2)
R_CA_JTAG_PWD1          = (R_CA_BASE + 0x25 << 2)
R_CA_JTAG_PWD2          = (R_CA_BASE + 0x26 << 2)
R_CA_JTAG_PWD3          = (R_CA_BASE + 0x27 << 2)

R_CA_POSTMASK           = (R_CA_BASE + 0x28 << 2)
R_CA_DBGMODE            = (R_CA_BASE + 0x42 << 2)

CA_SBOOT_ACTIVE         = 0xF000
CA_POSTMASK_ENABLE      = 0x0004
CA_DBGMODE_ACTIVE       = 0x8000


#-------------------------------------------------------------------------------------------------
# Macros
#-------------------------------------------------------------------------------------------------

    .macro  WREG_INIT
    _REGBASE = 0x0
    .endm

    .macro  WREG    reg, value
    .if (\reg >> 16) <> _REGBASE
        lui     $8,     (\reg >> 16)
        _REGBASE = (\reg >> 16)
    .endif
    li      $9,     (\value)
    sw      $9,     (\reg & 0xFFFF)($8)
    .endm

    .macro  WREG_B    reg, value
    .if (\reg >> 16) <> _REGBASE
        lui     $8,     (\reg >> 16)
        _REGBASE = (\reg >> 16)
    .endif
    li      $9,     (\value)
    sb      $9,     (\reg & 0xFFFF)($8)
    .endm

    .macro  RREG    var, reg
    .if (\reg >> 16) <> _REGBASE
        lui     $8,     (\reg >> 16)
        _REGBASE = (\reg >> 16)
    .endif
    lw      \var,     (\reg & 0xFFFF)($8)
    .endm

    .macro  WREG_DBG  value
    WREG_INIT
    WREG    0xBF206700, (\value) # mailbox 0
    .endm


#-------------------------------------------------------------------------------------------------
# Macro CHIP_PowerOnChip
# @note Power On Sequence for Chip
#-------------------------------------------------------------------------------------------------

    .include "chip_pll.inc"
    .include "chip_clock.inc"

    .macro  CHIP_PowerOnChip
    WREG_INIT

    BootRomEnableAccessMiu

    #//Initial releated settings of PLL
    BootRomInitPll

    #//Set clock ratio of OCP
    BootRomOcpClockRatio

    #//Initial related settings of Clock
    BootRomInitClock

    .endm


    .macro BootRomEnableAccessMiu
    WREG 0xBF005788, 0x0004      #//Set XDATA2MIU read Priority
    WREG 0xBF00578C, 0x3c11      #//Set the low bound address of MCU XDATA mapping to MIU (Unit: 1K-Byte)
    WREG 0xBF005790, 0x0010      #//Set the low byte address to access xdata from MIU. The granularity is 64k bytes
    WREG 0xBF005794, 0x403c      #//Set the low bound address of MCU XDATA mapping to MIU (Unit: 1K-Byte)
    WREG 0xBF005798, 0x0101      #//Set the low byte address to access xdata from MIU. The granularity is 4k bytes.
    .endm

    .macro BootRomOcpClockRatio

    WREG_B      0xBF203A3C, (MIPS_OCP_CLOCK_RATIO | 0x00)   #//Set clock ratio to 2:1
    DELAYUS     20000                    #//Delay 2ms
    WREG_B      0xBF203A3C, (MIPS_OCP_CLOCK_RATIO | 0x01)   #//Set clock ratio change pulse
    DELAYUS     20000                    #//Delay 2ms

# FIXME: OTP not stable
#    WREG        0xBF201644, 0x0000
#
#.if (CPU_CLOCK == 240)
#    WREG        0xBF201644, 0x8800        #//Switch to CLK_MIPS to 240MHz
#.else
#    WREG        0xBF201644, 0x8000        #//Switch to CLK_MIPS to mipspll_vco
#.endif

    .endm


#-------------------------------------------------------------------------------------------------
# Macro CHIP_SET_MIU_CLOCK
# @note MIU CLOCK TO 288MHz
#-------------------------------------------------------------------------------------------------

        .macro CHIP_SET_MIU_CLOCK
#CHIP_SET_MIU_CLOCK:
        .endm

#-------------------------------------------------------------------------------------------------
# Macro CHIP_ResetCHIP
# @note MIU initialization
#-------------------------------------------------------------------------------------------------

    .macro CHIP_ResetCHIP
#CHIP_ResetCHIP:
    WREG_INIT
    # watchdog chip reset
    WREG    R_REG_WDT_MAX_LO, 1
    WREG    R_REG_WDT_MAX_HI, 0
    WREG    R_REG_WDT_CLR, 1
    DELAYMS 100
    .endm


#-------------------------------------------------------------------------------------------------
# Macro CHIP_InitUART
# Macro PUTC_Char
# @note UART output function
#-------------------------------------------------------------------------------------------------

    .macro   CHIP_InitUART
    WREG_INIT

    WREG        (CLK_GEN0_BASE + 0x13 * 4), 0x0C01     #//Set CLK_UART0 to 123Mhz

    #.if (BOOTINFO_UART0_ENABLE == 1)
    RREG        $11, 0xbf001c24          #//Enable UART Rx via PAD_DDCA for dig_mux
    ori         $11, $11, 0x0800
    sw          $11, 0xbf001c24

    RREG        $11, 0xbf203d4c          #//Select UART0 source to PIU UART0
    or          $11, $11, 0x0004
    sw          $11, 0xbf203d4c
    #.endif


    RREG        $11, 0xbf201318          #//Enable Divisor Latch Access
    ori         $11, $11, 0x0080
    sw          $11, 0xbf201318

    WREG        0xbf201300, UART_DIVISOR & 0x00ff   #//Set Divisor Latch LSB
    WREG        0xbf201308, ((UART_DIVISOR >> 8) & 0x00ff)   #//Set Divisor Latch MSB

    RREG        $11, 0xbf201318          #//Disable Divisor Latch Access
    and         $11, $11, 0xFF7F
    sw          $11, 0xbf201318

    WREG        0xbf201310 0x0000
    WREG        0xbf201310 0x0007       #//Enable FIFO and Clear TX/RX FIFO

    WREG        0xbf201318 0x0000
    WREG        0xbf201318 0x0003       #//8-bit Data / 1-bit Stop / No Parity Bit
    WREG        0xbf201320 0x0000       #//Set DTS/RTS to 1
    WREG        0xbf201308 0x0000       #//Disable Interrupt
    .endm


    .macro  PUTC char
    WREG_INIT
    WREG    0xBF201300, (\char)
    .endm


#-------------------------------------------------------------------------------------------------
# Macro CHIP_WDTDisable
# @note Disable WDT
#-------------------------------------------------------------------------------------------------
        .macro  CHIP_WDTDisable
        WREG_INIT
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
        WREG_INIT
        # Enable debug mode
        li      $8, R_CA_DBGMODE
        lw      $9, 0($8)
        ori     $9, CA_DBGMODE_ACTIVE
        sw      $9, 0($8)
        .endm

