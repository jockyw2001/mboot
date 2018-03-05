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
# CHIP registers
#-------------------------------------------------------------------------------------------------

REG_ADDR_BASE           = 0xbf000000
CLK_GEN0_BASE           = 0xBF201600

R_REG_WDT_MAX_LO        = 0xbf006010
R_REG_WDT_MAX_HI        = 0xbf006014
R_REG_WDT_CLR           = 0xbf006000

REG_CHIP_CONFIG_OVERWRITE              = 0xbf001c7c

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
CPU_CLK_FREQ            = 600000000                         #CPU clock
UART_CLK_FREQ           = 123000000                         #Clock source
UART_BAUDRATE           = 115200                            #Baud rate
UART_DIVISOR            = (UART_CLK_FREQ/(16*UART_BAUDRATE))#Divisor


#-------------------------------------------------------------------------------------------------
# SecureChip registers
#-------------------------------------------------------------------------------------------------

R_CA_BASE               = 0xbf227400
R_CA_SBOOT              = (R_CA_BASE + (0x17 << 2))

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
# Macro CHIP_CONFIG_CHECK
# @note config check for production
#-------------------------------------------------------------------------------------------------

	.macro CHIP_CONFIG_CHECK

# ==================== this code should not be modified for production============ tj
        # Get REG_CHIP_CONFIG_VALUE into $17
        li      $19, REG_CHIP_CONFIG_OVERWRITE
        lw      $18, 0($19)
        ext     $17, $18, REG_CHIP_CONFIG_VALUE_SHIFT, REG_CHIP_CONFIG_VALUE_LEN

        # IF REG_CHIP_CONFIG_VALUE == SBUS or DBUS
        beq     $17, REG_CHIP_CONFIG_SBUS0, BOOT_Halt4tester
        nop
        beq     $17, REG_CHIP_CONFIG_SBUS1, BOOT_Halt4tester
        nop
        beq     $17, REG_CHIP_CONFIG_DBUS0, BOOT_Halt4tester
        nop
        beq     $17, REG_CHIP_CONFIG_DBUS1, BOOT_Halt4tester
        nop
# =============================== end ============================== tj
	.endm
#-------------------------------------------------------------------------------------------------
# Macro CHIP_SWITCH_SPI_FAST_MODE
# @note Switch SPI to fast mode
#-------------------------------------------------------------------------------------------------

	.macro CHIP_SWITCH_SPI_FAST_MODE

        #<<CLKGEN_PM>>
        WREG (REG_ADDR_BASE + (0x0017E2 <<1 )) , 0x0FFF     # CSZ clock adjust
        WREG (REG_ADDR_BASE + (0x0017E4 <<1 )) , 0x0001     # enable fast Mode
	.endm

#-------------------------------------------------------------------------------------------------
# Macro CHIP_PowerOnChip
# @note Power On Sequence for Chip
#-------------------------------------------------------------------------------------------------

        .macro  CHIP_PowerOnChip
CHIP_PowerOnchip:
        WREG_INIT
#include "script_MainPll_MIPSPLL_UPLL_otp.inc"

        WREG (REG_ADDR_BASE + (0x000e40 <<1 )) , 0x1008     # clk_mcu = xtal (step 1/2)
                                                            # clk_spi = xtal (step_1/2)
        #<<CLKGEN0>>
        WREG (REG_ADDR_BASE + (0x100b02 <<1 )) , 0x0007   # SRAM bist test clk 7:xtal, 6:62M, 5:86M 4:108M 3:123M 2:144M 1:172M 0:216M

        #MCU
        WREG (REG_ADDR_BASE + (0x100b20 <<1 )) , 0x0400   # clk_mcu = 172M (step 1/2)
        WREG (REG_ADDR_BASE + (0x100b20 <<1 )) , 0x0401   # clk_mcu = 172M (step 2/2)

        WREG (REG_ADDR_BASE + (0x101d1e <<1 )) , 0x0021   # clk_ocp ratio and change
        WREG (REG_ADDR_BASE + (0x100b22 <<1 )) , 0x0000   # clk_ocp on
                                                          # clk_mips = 500M (step 1/2) (MIPSPLL_VCO)
        WREG (REG_ADDR_BASE + (0x100b22 <<1 )) , 0x8000   # clk_mips = 500M (step 2/2) (MIPSPLL_VCO)

        #MIU
        WREG (REG_ADDR_BASE + (0x100b3e <<1 )) , 0x000C   # clk_miu = MEMPLL out, DDR2 1/4 mode -> 1066/4 = 266

        #NAND
        WREG (REG_ADDR_BASE + (0x100bc8 <<1 )) , 0x0024   # clk_fice = 54M

        #CA
        WREG (REG_ADDR_BASE + (0x103348 <<1 )) , 0x0000   # clk_nsk = 123M
                                                          # clk_sm_ca = 27M, clk_sm_ca2 = 27M
        WREG (REG_ADDR_BASE + (0x000e40 <<1 )) , 0x5098   # clk_mcu = 108M (step 2/2)  set spi clock to high speed for fast mode ,must after awhile for spi command address change, or the command with speed will chaos
                                                          # clk_spi = 54M (step_2/2)
        .endm

#-------------------------------------------------------------------------------------------------
# Macro CHIP_SET_MIU_CLOCK
# @note MIU CLOCK TO 288MHz
#-------------------------------------------------------------------------------------------------

        .macro CHIP_SET_MIU_CLOCK
#CHIP_SET_MIU_CLOCK:

        WREG_INIT
        #for K1 U04 and following version to set MIU clock to 288MHz
        lw      $9, 0xBF203D9C
        and     $9, $9, 0x00000F00
        li      $10, 0x00000400
        blt     $9, $10, end_if
        nop
#if ((ENABLE_MEM400MHz)||(ENABLE_MEM333MHz))
        WREG(REG_ADDR_BASE + (0x100b3e << 1)), 0x0000 # less than 192 or 1/4 MEMPLL
        WREG(REG_ADDR_BASE + (0x100b3c << 1)), 0x0102
#else
        WREG(REG_ADDR_BASE + (0x100b3e << 1)), 0x0018
#endif
end_if:
        .endm

#-------------------------------------------------------------------------------------------------
# Macro CHIP_ResetCHIP
# @note MIU initialization
#-------------------------------------------------------------------------------------------------

        .macro CHIP_ResetCHIP
#CHIP_ResetCHIP:

        WREG_INIT
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

        WREG_INIT

        WREG (CLK_GEN0_BASE+0x13*4),    0x0C01

### Switch PAD ISP###################################################
        #enable reg_uart_rx_enable
        RREG    $11,                    0xbf001c24
        ori     $11, $11,               0x0800
        sw      $11,                    0xbf001c24

        #Select to PIU_UART0
        RREG    $11,                    0xbf203d4c
        ori     $11, $11,               0x0004
        sw      $11,                    0xbf203d4c
### Switch PAD ISP end###################################################

        RREG    $11,                    0xbf201318  # Kernel need
        ori     $11, $11,               0x0080
        sw      $11,                    0xbf201318

        WREG    0xbf201300,             UART_DIVISOR & 0x00ff
        WREG    0xbf201308,             ((UART_DIVISOR >> 8) & 0x00ff)

        RREG    $11,                    0xbf201318  # Kernel need
        and     $11, $11,               0xFF7F
        sw      $11,                    0xbf201318

        WREG    0xbf201310              0x0000      # Kernel need
        WREG    0xbf201310              0x0007

        WREG    0xbf201318              0x0000      # Kernel need
        WREG    0xbf201318              0x0003
        WREG    0xbf201320              0x0000      # Kernel need
        WREG    0xbf201308              0x0000      # Kernel need

        .endm


        .macro  PUTC char
        WREG_INIT
        WREG    0xBF201300,     (\char)
        .endm


#-------------------------------------------------------------------------------------------------
# Macro CHIP_WDTDisable
# @note Disable WDT
#-------------------------------------------------------------------------------------------------
        .macro  CHIP_WDTDisable
#CHIP_WDTDisable:
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
        # check FP = 1
        mfc0    $9, $16,1
        and     $9, (1<<0)
        beq     $9, $0, 1f

        # set CU1 = 1 and FR = 1
        mfc0    $8, $12
        or      $8, (1<<29) #CU1 = 1
        or      $8, (1<<26) #FR = 1
        #MTC0(   $8, $12  )
        nop
        mtc0    $8, $12
        sll     $0,$0,1
        sll     $0,$0,1
        sll     $0,$0,1
        sll     $0,$0,3


        # set FS = 1
        li      $8, (1<<24) #set FS (flush to zero) bit
        #CTC1(   $8, $31    )
        nop
        ctc1    $8, $31
        sll     $0,$0,1
        sll     $0,$0,1
        sll     $0,$0,1
        sll     $0,$0,3

        # check FIR != 0
        #CFC1(   $8, $0     )
        nop
        cfc1    $8, $0
        sll     $0,$0,1
        sll     $0,$0,1
        sll     $0,$0,1
        sll     $0,$0,3
        beq     $8, $0, 1f
        nop
        nop
        nop
1:
        .endm
