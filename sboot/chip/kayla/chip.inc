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

CHIP_KAYLA = 1
#-------------------------------------------------------------------------------------------------
# Flash registers
#-------------------------------------------------------------------------------------------------


#-------------------------------------------------------------------------------------------------
# Other registers
#-------------------------------------------------------------------------------------------------


#-------------------------------------------------------------------------------------------------
# UART registers
#-------------------------------------------------------------------------------------------------
#if defined(CONFIG_MSTAR_KAYLA_FPGA)
    UART_CLK_FREQ           = 12000000                         #Clock source
    UART_BAUDRATE           = 38400                            #Baud rate
#else
    UART_CLK_FREQ           = 123000000                         #Clock source
    UART_BAUDRATE           = 115200                            #Baud rate
#endif
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
# Macro CHIP_SWITCH_SPI_FAST_MODE
# @note Switch SPI to fast mode
#-------------------------------------------------------------------------------------------------

	.macro CHIP_SWITCH_SPI_FAST_MODE

        #<<CLKGEN_PM>>
        WREG (REG_ADDR_BASE + (0x0017E2 <<1 )) , 0x0FFF     # CSZ clock adjust
        WREG (REG_ADDR_BASE + (0x0017E4 <<1 )) , 0x0001     # enable fast Mode
	.endm

#-------------------------------------------------------------------------------------------------
# Macro CHIP_MIU_ODT_SETTING
# @note MIU ODT value setting
#-------------------------------------------------------------------------------------------------

	.macro CHIP_MIU_ODT_SETTING
        RREG    $11,                    0xBF221A1C          
        and     $11, $11,               0x001E
        sll     $11, $11,               3       
        RREG    $12,                    0xBF202450
        and     $12, $12,               0xFF0F
        or      $12, $12, $11
        sw      $12,                    0xBF202450          # 0x101228[7:4] = 0x110D0E[4:1]
        
        RREG    $11,                    0xBF221A78
        and     $11, $11,               0x0007
        RREG    $12,                    0xBF221A9C
        and     $12, $12,               0xFFF8
        or      $12, $12, $11
        sw      $12,                    0xBF221A9C          # 0x110D4E[2:0] = 0x110D3C[2:0]
	.endm

#-------------------------------------------------------------------------------------------------
# Macro CHIP_PowerOnChip
# @note Power On Sequence for Chip
#-------------------------------------------------------------------------------------------------

        .macro  CHIP_PowerOnChip
CHIP_PowerOnchip:
        WREG_INIT

        #if (CONFIG_CAE_CPUCLK_MANUAL_CHANGE_TEST == 1)
        li      $12, 0xABCD
        LOOP_CAE_CPUCLK:
        RREG    $11,                    0xbf206720  #dummy reg
        nop
        bne     $11, $12, LOOP_CAE_CPUCLK
        nop
        #endif
        
        #include "script_MainPll_MIPSPLL_UPLL_otp.inc"

        RREG    $11,                      0xBF2C1FA8
        and     $11, $11,                 0xFC00
        srl     $11, $11,                 2
        RREG    $12,                      0xBF001C64
        and     $12, $12,                 0xC0FF
        or      $12, $12, $11
        sw      $12,                      0xBF001C64        # 0x0E32[13:8] = 0x160FD4[15:10] trimming (TR[5:0])
        WREG (REG_ADDR_BASE + (0x000e40 <<1 )) , 0x1008     # clk_mcu = xtal (step 1/2)
                                                            # clk_spi = xtal (step_1/2)
        #<<CLKGEN0>>
        WREG (REG_ADDR_BASE + (0x100b02 <<1 )) , 0x0007   # SRAM bist test clk 7:xtal, 6:62M, 5:86M 4:108M 3:123M 2:144M 1:172M 0:216M

        #MCU
        WREG (REG_ADDR_BASE + (0x100b20 <<1 )) , 0x0000   # clk_mcu = 144M (step 1/2)
        WREG (REG_ADDR_BASE + (0x100b20 <<1 )) , 0x0001   # clk_mcu = 144M (step 2/2)

        #WREG (REG_ADDR_BASE + (0x101d1e <<1 )) , 0x0021   # clk_ocp ratio and change
        #WREG (REG_ADDR_BASE + (0x100b22 <<1 )) , 0x0000   # clk_ocp on
                                                          # clk_mips = 500M (step 1/2) (MIPSPLL_VCO)
        #WREG (REG_ADDR_BASE + (0x100b22 <<1 )) , 0x8000   # clk_mips = 500M (step 2/2) (MIPSPLL_VCO)

        #MIU
        WREG (REG_ADDR_BASE + (0x100b3e <<1 )) , 0x0000   # clk_miu = MEMPLL out, DDR2 1/4 mode -> 1066/4 = 266

        #NAND
        #WREG (REG_ADDR_BASE + (0x100bc8 <<1 )) , 0x0024   # clk_fice = 54M

        #CA
        #WREG (REG_ADDR_BASE + (0x103348 <<1 )) , 0x0000   # clk_nsk = 123M
                                                          # clk_sm_ca = 27M, clk_sm_ca2 = 27M

        WREG (REG_ADDR_BASE + (0x000e40 <<1 )) , 0x5098   # clk_mcu = 108M (step 2/2)  set spi clock to high speed for fast mode ,must after awhile for spi command address change, or the command with speed will chaos
                                                          # clk_spi = 54M (step_2/2)
        WREG (REG_ADDR_BASE + (0x100bfa <<1 )) , 0x0001

        #TSP
        WREG (REG_ADDR_BASE + (0x100b54 <<1 )) , 0x0024   # clk_tsp = 172Mhz
                                                          # clk_syn_stc0 = 432Mhz

        // switch TSP sram to MIPS
        WREG_B (REG_ADDR_BASE + (0x11135e <<1 )) , 0x18
        WREG_B (REG_ADDR_BASE + (0x11135e <<1 )) , 0x08
        WREG_B (REG_ADDR_BASE + (0x11135e <<1 )) , 0x00
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
#if (CONFIG_CONFIG_Second_UART_Mode_1 || CONFIG_Second_UART_Mode_2 || CONFIG_Second_UART_Mode_3)
        RREG    $11,                    (CLK_GEN0_BASE+0x14*4)
        andi    $11, $11,               0xfff0
        ori     $11, $11,               0x000C
        sw      $11,                     (CLK_GEN0_BASE+0x14*4)
#endif

#if (CONFIG_CONFIG_Third_UART_Mode_1 || CONFIG_Third_UART_Mode_2 || CONFIG_Third_UART_Mode_3)
        RREG    $11,                    (CLK_GEN0_BASE+0x14*4)
        andi    $11, $11,               0xf0ff
        ori     $11, $11,               0x0C00
        sw      $11,                     (CLK_GEN0_BASE+0x14*4)
#endif
### Switch PAD ISP###################################################
#if (CONFIG_First_UART_Mode_1)
        #enable reg_uart_rx_enable
        RREG    $11,                    0xbf001c24
        andi    $11, $11,               0xefff
        ori     $11, $11,               0x0800
        sw      $11,                    0xbf001c24

        #Select to PIU_UART0
        RREG    $11,                    0xbf203d4c
        andi    $11, $11,               0xFFF0
        ori     $11, $11,               0x0004
        sw      $11,                    0xbf203d4c
#endif
#if (CONFIG_Second_UART_Mode_1)
        RREG     $11,                        0xbf203d4c
        andi    $11, $11,                    0xff0f
        ori     $11, $11,                     0x0040
        sw      $11,                           0xbf203d4c
        RREG     $11,                        0xbf203c0c
        andi    $11, $11,                    0xfff0
        ori     $11, $11,                     0x0001
        sw      $11,                           0xbf203c0c
        WREG    0xbf203d40               0x0000
#endif
#if (CONFIG_Second_UART_Mode_2)
        RREG     $11,                        0xbf203d4c
        andi    $11, $11,                    0xff0f
        ori     $11, $11,                     0x0040
        sw      $11,                           0xbf203d4c
        RREG     $11,                        0xbf203c0c
        andi    $11, $11,                    0xfff0
        ori     $11, $11,                     0x0002
        sw      $11,                           0xbf203c0c
        WREG    0xbf203d40               0x0000
#endif
#if (CONFIG_Second_UART_Mode_3)
        RREG     $11,                        0xbf203d4c
        andi    $11, $11,                    0xff0f
        ori     $11, $11,                     0x0040
        sw      $11,                           0xbf203d4c
        RREG     $11,                        0xbf203c0c
        andi    $11, $11,                    0xfff0
        ori     $11, $11,                     0x0003
        sw      $11,                           0xbf203c0c
        WREG    0xbf203d40               0x0000
#endif
#if (CONFIG_Third_UART_Mode_1)
        RREG     $11,                        0xbf203d4c
        andi    $11, $11,                    0xf0ff
        ori     $11, $11,                     0x0400
        sw      $11,                           0xbf203d4c
        RREG     $11,                        0xbf203c0c
        andi    $11, $11,                    0xff0f
        ori     $11, $11,                     0x0010
        sw      $11,                           0xbf203c0c
        WREG    0xbf203d40               0x0000
#endif
#if (CONFIG_Third_UART_Mode_2)
        RREG     $11,                        0xbf203d4c
        andi    $11, $11,                    0xf0ff
        ori     $11, $11,                     0x0400
        sw      $11,                           0xbf203d4c
        RREG     $11,                        0xbf203c0c
        andi    $11, $11,                    0xff0f
        ori     $11, $11,                     0x0020
        sw      $11,                           0xbf203c0c
        WREG    0xbf203d40               0x0000
#endif
#if (CONFIG_Third_UART_Mode_3)
        RREG     $11,                        0xbf203d4c
        andi    $11, $11,                    0xf0ff
        ori     $11, $11,                     0x0400
        sw      $11,                           0xbf203d4c
        RREG     $11,                        0xbf203c0c
        andi    $11, $11,                    0xff0f
        ori     $11, $11,                     0x0030
        sw      $11,                           0xbf203c0c
        WREG    0xbf203d40               0x0000
#endif
#if (CONFIG_Fourth_UART_Mode_1)
        RREG     $11,                        0xbf203d4c
        andi    $11, $11,                    0x0fff
        ori     $11, $11,                     0x4000
        sw      $11,                           0xbf203d4c
        RREG     $11,                        0xbf203c0c
        andi    $11, $11,                    0xf0ff
        ori     $11, $11,                     0x0100
        sw      $11,                           0xbf203c0c
        WREG    0xbf203d40               0x0000
#endif
#if (CONFIG_Fourth_UART_Mode_2)
        RREG     $11,                        0xbf203d4c
        andi    $11, $11,                    0x0fff
        ori     $11, $11,                     0x4000
        sw      $11,                           0xbf203d4c
        RREG     $11,                        0xbf203c0c
        andi    $11, $11,                    0xf0ff
        ori     $11, $11,                     0x0200
        sw      $11,                           0xbf203c0c
        WREG    0xbf203d40               0x0000
#endif
#if (CONFIG_Fourth_UART_Mode_3)
        RREG     $11,                        0xbf203d4c
        andi    $11, $11,                    0x0fff
        ori     $11, $11,                     0x4000
        sw      $11,                           0xbf203d4c
        RREG     $11,                        0xbf203c0c
        andi    $11, $11,                    0xf0ff
        ori     $11, $11,                     0x0300
        sw      $11,                           0xbf203c0c
        WREG    0xbf203d40               0x0000
#endif
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
# Macro CHIP_CONFIG_CHECK
# @note config check for production
#-------------------------------------------------------------------------------------------------
	.macro CHIP_CONFIG_CHECK
    # do nothing in current stage
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
# Macro CHIP_InitTimer
# @note Initial PIU timer
#-------------------------------------------------------------------------------------------------
        .macro CHIP_InitTimer
        WREG_INIT
        WREG (REG_ADDR_BASE + (0x3024 <<1 )) , 0xFFFF
        WREG (REG_ADDR_BASE + (0x3026 <<1 )) , 0xFFFF
        WREG (REG_ADDR_BASE + (0x3044 <<1 )) , 0xFFFF
        WREG (REG_ADDR_BASE + (0x3046 <<1 )) , 0xFFFF
        WREG (REG_ADDR_BASE + (0x3020 <<1 )) , 0x0000
        WREG (REG_ADDR_BASE + (0x3020 <<1 )) , 0x0001
        WREG (REG_ADDR_BASE + (0x3040 <<1 )) , 0x0000
        WREG (REG_ADDR_BASE + (0x3040 <<1 )) , 0x0001
        .endm        
#-------------------------------------------------------------------------------------------------
# Macro CHIP_SET_MIU_CLOCK
# @note Use Only on Kronus
#-------------------------------------------------------------------------------------------------

        .macro CHIP_SET_MIU_CLOCK

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
