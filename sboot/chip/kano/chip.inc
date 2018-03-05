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

#include "Board.h"

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
# UART registers
#-------------------------------------------------------------------------------------------------


#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
#Clock source
UART_CLK_FREQ           = 123000000
#Baud rate
UART_BAUD_RATE          = 115200
#else
#Clock source
UART_CLK_FREQ           = 12000000
#Baud rate
UART_BAUD_RATE          = 38400
#endif

#Divisor
UART_DIVISOR            = (UART_CLK_FREQ/(16 * UART_BAUD_RATE))

UART_DIVISOR_H          = (UART_DIVISOR >> 8)
UART_DIVISOR_L          = (UART_DIVISOR & 0xff)

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

#-------------------------------------------------------------------------------------------------
# Macro CHIP_SWITCH_SPI_FAST_MODE
# @note Switch SPI to fast mode
#-------------------------------------------------------------------------------------------------

	.macro CHIP_SWITCH_SPI_FAST_MODE

        #<<CLKGEN_PM>>
        WREG (REG_ADDR_BASE + (0x0017E2 <<1 )) , 0x0FFF     // CSZ clock adjust
        WREG (REG_ADDR_BASE + (0x0017E4 <<1 )) , 0x0001     // enable fast Mode
	.endm
#-------------------------------------------------------------------------------------------------
# Macro CHIP_InitDSPRAM
# @note Initial Kaiser SPRAM
#-------------------------------------------------------------------------------------------------

.macro CHIP_InitDSPRAM
#jerry_ht_need_prepare
.endm

#-------------------------------------------------------------------------------------------------
# Macro CHIP_PowerOnChip
# @note Power On Sequence for Chip
#-------------------------------------------------------------------------------------------------

.macro  CHIP_PowerOnChip
    #include "script_MainPll_ARMPLL_UPLL_otp.inc"

    DELAYUS     1000

    WREG (REG_ADDR_BASE + (0x101df8 <<1 )) , 0x0001
    

    WREG (REG_ADDR_BASE + (0x000e40 <<1 )) , 0x1008     // clk_mcu = xtal (step 1/2)
                                                       // clk_spi = xtal (step_1/2)

    #<<CLKGEN0>>
    WREG (REG_ADDR_BASE + (0x100b02 <<1 )) , 0x0007   // SRAM bist test clk 7:xtal, 6:62M, 5:86M 4:108M 3:123M 2:144M 1:172M 0:216M

    #MCU
    WREG (REG_ADDR_BASE + (0x100b20 <<1 )) , 0x0000   // clk_mcu = 144M (step 1/2)
    WREG (REG_ADDR_BASE + (0x100b20 <<1 )) , 0x0001   // clk_mcu = 144M (step 2/2)

    WREG (REG_ADDR_BASE + (0x000e40 <<1 )) , 0x5080   // clk_mcu = 216M (step 2/2)  set spi clock to high speed for fast mode ,must after awhile for spi command address change, or the command with speed will chaos

    // HW patch for ARM exclusive instruction
    WREG (REG_ADDR_BASE + (0x10188a << 1)) , 0x0031

    // Control to CPU frequency caculate
    WREG (REG_ADDR_BASE + (0x101ea0 << 1)), 0x0000
    WREG (REG_ADDR_BASE + (0x101eea << 1)), 0x4000
    WREG_B (REG_ADDR_BASE + (0x101896 << 1)), 0x01
    WREG (REG_ADDR_BASE + (0x101eee << 1)), 0x0002
    WREG (REG_ADDR_BASE + (0x101eec << 1)), 0x0001
    WREG_B ((REG_ADDR_BASE + (0x101ee0 << 1)) + 1), 0x80
    WREG_B ((REG_ADDR_BASE + (0x101ee0 << 1)) + 1), 0x00

    //TSP & TSO MCM enable [HW patch to FIQ and MM IF work ok]
    WREG_B ((REG_ADDR_BASE + (0x171500 << 1)) + 1), 0x10
    WREG_B (REG_ADDR_BASE + (0x171502 << 1)), 0x10

    // turn on HDMI LDO power for U02 chip (compatible for U01, this seeting not work for U01)
    WREG_B (REG_ADDR_BASE + (0x173080 << 1)), 0x01
    WREG_B (REG_ADDR_BASE + (0x17307a << 1)+1 ), 0x04
.endm

#-------------------------------------------------------------------------------------------------
# Macro CPU_GPU_ACCESS_MIU_SETUP
# @note Setup CPU and GPU to access MIU ok
#-------------------------------------------------------------------------------------------------

.macro  CPU_GPU_ACCESS_MIU_SETUP
    # ARM access MIU
    WREG (REG_ADDR_BASE + (0x101882 << 1)) , 0x0484
.endm

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
  
    WREG        0x1F20164C  0x0C01
#if defined(CONFIG_ARMv8_32BIT_SBOOT)
    #enable reg_uart_rx_enable
    RREG        r0,  0x1F001c24
    bic         r0,  r0, #0x1000
    orr         r0,  r0, #0x0800
    REG_C2M     r0,  0x1F001c24

    #Select to PIU_UART0
    RREG        r0,  0x1F203d4c
    orr         r0,  r0,  #0x0004
    REG_C2M     r0,  0x1F203d4c

    RREG        r0,  0x1F201318      // Divisor Latch Access;
    orr         r0,  r0,  #0x0080   // 1: The divisor latches can be accessed
    REG_C2M     r0,  0x1F201318

    WREG        0x1F201300  UART_DIVISOR_L
    WREG        0x1F201308  UART_DIVISOR_H

    RREG        r0,  0x1F201318    // Kernel need

    bic         r0,  r0,  #0x0080

    REG_C2M     r0,  0x1F201318
#else
    #enable reg_uart_rx_enable
    RREG        w25,  0x1F001c24
    bic         w25,  w25, #0x1000
    orr         w25,  w25, #0x0800
    REG_C2M     w25,  0x1F001c24

    #Select to PIU_UART0
    RREG        w25,  0x1F203d4c
    orr         w25,  w25,  #0x0004
    REG_C2M     w25,  0x1F203d4c

    RREG        w25,  0x1F201318      // Divisor Latch Access;
    orr         w25,  w25,  #0x0080   // 1: The divisor latches can be accessed
    REG_C2M     w25,  0x1F201318

    WREG        0x1F201300  UART_DIVISOR_L
    WREG        0x1F201308  UART_DIVISOR_H

    RREG        w25,  0x1F201318    // Kernel need

    bic         w25,  w25,  #0x0080

    REG_C2M     w25,  0x1F201318
#endif

    WREG        0x1F201310  0x0000        // Kernel need
    WREG        0x1F201310  0x0007

    WREG        0x1F201318  0x0000        // Kernel need
    WREG        0x1F201318  0x0003
    WREG        0x1F201320  0x0000        // Kernel need
    WREG        0x1F201308  0x0000        // Kernel need

.endm

.macro  PUTC char
    WREG_B 0x1F201300 \char
.endm


