################################################################################
#
# Copyright (c) 2006-2013 MStar Semiconductor, Inc.
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

#ifndef _BOOT_INC_
#include "boot.inc"
#endif

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

    #// Enable 8051 to access MIU for MStar TV Tool
    bal         BootRomEnableAccessMiu
    nop

    DBGREG      0x0002    

    #// Pre-Trimming Sequence
    bal         BootRomPreTrimSequence
    nop

    DBGREG      0x0003

    #// Power-On Sequence
    bal         BootRomPowerOnSequence
    nop

    DBGREG      0x0004

    #//Configure PAD top for MUX and GPIO (e.g. DDR power)
    lui         a0, %hi(padInitTbl + 10)
    bal         MDrv_WriteRegMaskTbl
    ori         a0, %lo(padInitTbl + 10)

    DBGREG      0x0005

    #// Post-Trimming Sequence
    bal         BootRomPostTrimSequence
    nop

    DBGREG      0x0006
	
#ifdef CONFIG_MSTAR_STR_ENABLE
    RREG        t3, 0xBF001CDC
    and         t3, t3, 0xFFFF
    add         t4, zero, 0x0100
    bne         t3, t4, BootRom_NORMAL
    nop
BootRom_STR_RESUME:
    bal         Boot_StrEntry
    nop
BootRom_NORMAL: 
#endif
    #//Copy image of bootram to DRAM
    bal         BOOT_CopyBootRam
    nop

    DBGREG      0x0007

    #//Set TSP Sram be used by TSP
    #bal         BOOT_SramToTsp
    #nop

    DBGREG      0x0008

    #//Jump to the entry of BOOTRAM
    la          ra, BOOTRAM_Entry
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

    #//Set clock ratio of OCP
    #bal     	BootRomOcpClockRatio
    #nop

    #//Initial related settings of Clock
    bal         BootRomInitClock
    nop

    DBGREG      0x0031
    
    DELAYUS     100

    #//Initial related settings of UART
    bal         BootInitUart
    nop

    DBGREG      0x0032

    #//Initial related settings of cache
    bal         BootRomInitCache
    nop

    DBGREG      0x0033

    #//Disable ADV before MIU initiation
    bal     	BootRomDisableAvd
    nop

    DBGREG      0x0034

#ifdef CONFIG_MSTAR_STR_ENABLE
    DBGREG      0xDDDD
	#//IS_AC_FLOW_CONTROL
    RREG        t3, 0xBF001CDC
    and         t3, t3, 0xFFFF
    add         t4, zero, 0x0100
    bne         t3, t4, BootRom_AcOn_Flow
    nop
BootRom_DcOn_Flow:

    PUTK        'D'
    PUTK        'C'
    PUTK        '_'
    PUTK        'F'
    PUTK        'L'
    PUTK        'O'
    PUTK        'W'
    PUTK        '\n'
    PUTK        '\r'
	

    bal         BootInitMiu_STR
    nop
    DBGREG      0x0135
	
    j           BootRom_Exit_Refresh
    nop
    
#endif

BootRom_AcOn_Flow:

    PUTK        'A'
    PUTK        'C'
    PUTK        '_'
    PUTK        'F'
    PUTK        'L'
    PUTK        'O'
    PUTK        'W'
    PUTK        '\n'
    PUTK        '\r'
    #//Initial related settings of MIU
    bal     	BootInitMiu
    nop

BootRom_Exit_Refresh:
    DBGREG      0x0035

#ifdef CONFIG_MSTAR_32BIT_RIU_MASK_ENABLE
    bal     	BootInitRiuBitMask
    nop
#endif

    bal         BootRomInitFpu
    nop

    DBGREG      0x0036

    #//Initial related settings of SPI flash clock
    #bal         BootRomInitSpiFlashClock
    #nop

    DBGREG      0x0037

    #bal         BootRomChangeSpiFlashClock
    #nop

    DBGREG      0x0038

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

    RREG        t9, 0xBF2226BC #//Select tsp sram to MIPS
    and         t9, t9, 0xffef
    REG_C2M     t9, 0xBF2226BC

    RREG        t9, 0xBF2226BC #//Enable nand boot operation
    or          t9, t9, 0x0008
    REG_C2M     t9, 0xBF2226BC

    RREG        t9, 0xBF2226BC #//Select tsp sram to FCIE4
    and         t9, t9, 0xfff7
    REG_C2M     t9, 0xBF2226BC

    RREG        t9, 0xBF2226BC #//Disable nand boot operation
    or          t9, t9, 0x0010
    REG_C2M     t9, 0xBF2226BC

    jr          ra
    nop

#-------------------------------------------------------------------------------------------------
# BOOT_InitEdinburgh  Init Demod Edinburgh
# @param  None
# @return None
# @note   do enable Xtal for Edinburgh
#-------------------------------------------------------------------------------------------------
BOOT_InitEdinburgh:

    RREG        t9, 0xBF001C24
    and         t9, t9, 0xfbff
    REG_C2M     t9, 0xBF001C24

    WREG        0xBF203C70, 0x0000

    RREG        t9, 0xBF224004
    and         t9, t9, 0xdfff
    REG_C2M     t9, 0xBF224004

    RREG        t9, 0xBF2250F0
    and         t9, t9, 0xefff
    REG_C2M     t9, 0xBF2250F0

    RREG        t9, 0xBF2250D4
    and         t9, t9, 0xbfff
    REG_C2M     t9, 0xBF2250D4

    RREG        t9, 0xBF2250F4
    or          t9, t9, 0x0001
    REG_C2M     t9, 0xBF2250F4

    RREG        t9, 0xBF22501C
    or          t9, t9, 0x0010
    REG_C2M     t9, 0xBF22501C

    jr          ra
    nop

#-------------------------------------------------------------------------------------------------
# BootRomInitSpiFlashClock SPI Clock init
# @param  None
# @return None
# @note   do SPI Clock init
#-------------------------------------------------------------------------------------------------
BootRomInitSpiFlashClock:

    WREG_B      (0xBF000000 + (0x000AE2 << 1)), 0xFF        #//Set CSZ deselect time to 16 SPI clock cycles
                                        #//Set CSZ setup time to 16 SPI clock cycles
    WREG_B      (0xBF000000 + (0x000AE2 << 1) + 1), 0x0F    #//Set CSZ hold time to 16 SPI clock cycles
    WREG_B      (0xBF000000 + (0x000AE4 << 1)), 0x01        #//Enable fast read mode

    jr          ra
    nop
#-------------------------------------------------------------------------------------------------
# BootRomChangeSpiFlashClock
# @param  None
# @return None
# @note   change SPI clock
#-------------------------------------------------------------------------------------------------
BootRomChangeSpiFlashClock:

    WREG_B      (0xBF000000 + (0x000E40 << 1) + 1), 0x04    #//Set clock of clk_spi to 86MHz
    WREG_B      (0xBF000000 + (0x000E40 << 1) + 1), 0x44    #//Switch clk_spi between clk_mcu_p and clk_ext_xtali_buf
    WREG_B      (0xBF000000 + (0x000E40 << 1)), 0x80        #//Set clock of MCU to 170MHz
    jr          ra
    nop

#-------------------------------------------------------------------------------------------------
# BootRomOcpClockRatio
# @param  None
# @return None
# @note   do MIPS OCP clock ratio setting
#-------------------------------------------------------------------------------------------------
BootRomOcpClockRatio:

    WREG_B      0xBF203A3C, (MIPS_OCP_CLOCK_RATIO | 0x00)   #//Set clock ratio to 2:1
    WREG_B      0xBF203A3C, (MIPS_OCP_CLOCK_RATIO | 0x01)   #//Set clock ratio change pulse

    WREG_B      0xBF201645, 0x80      #//Switch to CLK_MIPS to mipspll_vco

    jr          ra
    nop

#-------------------------------------------------------------------------------------------------
# BOOT_CopyBootRam
# @param  None
# @return None
# @note   Copy bootram image to DRAM
#-------------------------------------------------------------------------------------------------
BOOT_CopyBootRam:

    la          t0, _ld_bootram_ROM_start
    la          t1, _ld_bootram_RAM_start

    and         t1, t1, ~0xE0000000
    or          t1, t1, 0xA0000000

    la          t2, _ld_bootram_RAM_end
    and         t2, t2, ~0xE0000000
    or          t2, t2, 0xA0000000

1:
    ld          t3, 0(t0)               #//Read 64-bit data to reduce processing time
    ld          t5, 8(t0)

    sd          t3, 0(t1)
    sd          t5, 8(t1)

    addu        t0, t0, 16
    addu        t1, t1, 16

    bltu        t1, t2, 1b
    nop

    jr          ra
    nop


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
    PUTK        'K'
    PUTK        'P'
	
    andi        t0, t0, WAKEUP_ADDR_MASK
    sll         t0, t0, 16             #// wake up info addr
    lw          t1, 0x0(t0)
    lw          t2, 0x4(t0)
    li          t0, MSTAR_SLEEP_MAGIC
    bne         t1, t0, step3
    nop
	
    li          t0, MIU0_VIRTBASE1
    add         t2, t2, t0

    PUTK        'J'
    beqz        t2, step3
    nop

    PUTK        'M'
    jr          t2                      #// jump to wakup addr
    nop
	
step3:
.endif

    move        ra, k0
    jr          ra
    nop	
#endif
#-------------------------------------------------------------------------------------------------
# BootRomEnableAccessMiu
# @param  None
# @return None
# @note   Enable to access MIU
#-------------------------------------------------------------------------------------------------
LEAF(BootRomEnableAccessMiu)

    WREG        (0xBF000000 + (0x002BC4 << 1)), 0x0004      #//Set XDATA2MIU read Priority
    WREG        (0xBF000000 + (0x002BC6 << 1)), 0x3c11      #//Set the low bound address of MCU XDATA mapping to MIU (Unit: 1K-Byte)
    WREG        (0xBF000000 + (0x002BC8 << 1)), 0x0010      #//Set the low byte address to access xdata from MIU. The granularity is 64k bytes
    WREG        (0xBF000000 + (0x002BCA << 1)), 0x403c      #//Set the low bound address of MCU XDATA mapping to MIU (Unit: 1K-Byte)
    WREG        (0xBF000000 + (0x002BCC << 1)), 0x0101      #//Set the low byte address to access xdata from MIU. The granularity is 4k bytes.

    jr          ra
    nop

END(BootRomEnableAccessMiu)

#-------------------------------------------------------------------------------------------------
# BootInitUart
# @param  None
# @return None
# @note   Init UART
#-------------------------------------------------------------------------------------------------
LEAF(BootInitUart)

#ifdef MSTAR_BD_FPGA

    WREG        0xbf001c24 0x0800
    WREG        0xbf203d4c 0x2104
    WREG        0xbf201318 0x0083
    WREG        0xbf201300 0x0013
    WREG        0xbf201308 0x0000
    WREG        0xbf201310 0x0000
    WREG        0xbf201318 0x0003
    WREG        0xbf201310 0x0000
    WREG        0xbf201310 0x0007
    WREG        0xbf006010 0x0000
    WREG        0xbf006014 0x0000

    PUTK        'U'
    PUTK        'A'
    PUTK        'R'
    PUTK        'T'
    PUTK        '_'
    PUTK        '3'
    PUTK        '8'
    PUTK        '4'
    PUTK        '0'
    PUTK        '0'

    PUTK        '\n'
    PUTK        '\r'

#else
    WREG        (0xBF000000 + (0x100B26 << 1)), 0x0C01      #//Set CLK_UART0 to 123Mhz

.if (BOOTINFO_UART0_ENABLE == 1)
    RREG        t3, (0xBF000000 + (0x000E12 << 1))          #//Enable UART Rx via PAD_DDCA for dig_mux
    ori         t3, t3, 0x0800
    sw          t3, (0xBF000000 + (0x000E12 << 1))

    RREG        t3, (0xBF000000 + (0x101EA6 << 1))          #//Select UART0 source to PIU UART0
    or          t3, t3, 0x0004
    sw          t3, (0xBF000000 + (0x101EA6 << 1))

.endif

.if (BOOTINFO_UART1_ENABLE == 1)
    WREG        (0xBF000000 + (0x101EA6 << 1)), 0x0504      #//Set UART1 select

    WREG        (0xBF000000 + (0x101EA8 << 1)), 0x0000      #//Set UART4 source to None

    RREG        t3, (0xBF000000 + (0x101EA4 << 1))          #//Set third UART mode: RX2/TX2 uses PAD_UART_RX2/PAD_UART_TX2
    ori         t3, t3, 0x0C00
    sw          t3, (0xBF000000 + (0x101EA4 << 1))

    #uart1 init
    RREG        t3, (0xBF000000 + (0x11060C << 1))          #//Enable Divisor Latch Access
    ori         t3, t3, 0x0080
    sw          t3, (0xBF000000 + (0x11060C << 1))

    WREG        (0xBF000000 + (0x110600 << 1)), UART_DIVISOR_L  #//Set Divisor Latch LSB
    WREG        (0xBF000000 + (0x110604 << 1)), UART_DIVISOR_H  #//Set Divisor Latch MSB

    RREG        t3, (0xBF000000 + (0x11060C << 1))          #//Disable Divisor Latch Access
    and         t3, t3, 0xFF7F
    sw          t3, (0xBF000000 + (0x11060C << 1))

    WREG        (0xBF000000 + (0x110608 << 1)), 0x0000
    WREG        (0xBF000000 + (0x110608 << 1)), 0x0007      #//Enable FIFO and Clear TX/RX FIFO

    WREG        (0xBF000000 + (0x11060C << 1)), 0x0000
    WREG        (0xBF000000 + (0x11060C << 1)), 0x0003      #//8-bit Data / 1-bit Stop / No Parity Bit
    WREG        (0xBF000000 + (0x110610 << 1)), 0x0000      #//Set DTS/RTS to 1
    WREG        (0xBF000000 + (0x110604 << 1)), 0x0000      #//Disable Interrupt

.endif

.if (BOOTINFO_UART2_ENABLE == 1)
    RREG        t3, (0xBF000000 + (0x101EA6 << 1))          #//Select UART2 source to PIU UART0
    ori         t3, t3, 0x0400
    sw          t3, (0xBF000000 + (0x101EA6 << 1))

    WREG        (0xBF000000 + (0x101EA8 << 1)), 0x0000      #//Set UART4 source to None

    RREG        t3, (0xBF000000 + (0x101E04 << 1))          #//Set third UART mode: RX2/TX2 uses PAD_UART_RX2/PAD_UART_TX2
    ori         t3, t3, 0x0800
    sw          t3, (0xBF000000 + (0x101E04 << 1))
.endif

    RREG        t3, (0xBF000000 + (0x10098C << 1))           #//Enable Divisor Latch Access
    ori         t3, t3, 0x0080
    sw          t3, (0xBF000000 + (0x10098C << 1))

    WREG        (0xBF000000 + (0x100980 << 1)), UART_DIVISOR_L   #//Set Divisor Latch LSB
    WREG        (0xBF000000 + (0x100984 << 1)), UART_DIVISOR_H   #//Set Divisor Latch MSB

    RREG        t3, (0xBF000000 + (0x10098C << 1))           #//Disable Divisor Latch Access
    and         t3, t3, 0xFF7F
    sw          t3, (0xBF000000 + (0x10098C << 1))

    WREG        (0xBF000000 + (0x100988 << 1)), 0x0000
    WREG        (0xBF000000 + (0x100988 << 1)), 0x0007       #//Enable FIFO and Clear TX/RX FIFO

    WREG        (0xBF000000 + (0x10098C << 1)), 0x0000
    WREG        (0xBF000000 + (0x10098C << 1)), 0x0003       #//8-bit Data / 1-bit Stop / No Parity Bit
    WREG        (0xBF000000 + (0x100990 << 1)), 0x0000       #//Set DTS/RTS to 1
    WREG        (0xBF000000 + (0x100984 << 1)), 0x0000       #//Disable Interrupt

    PUTK        'U'
    PUTK        'A'
    PUTK        'R'
    PUTK        'T'
    PUTK        '_'
    PUTK        '1'
    PUTK        '1'
    PUTK        '5'
    PUTK        '2'
    PUTK        '0'
    PUTK        '0'

    PUTK        '\n'
    PUTK        '\r'

    jr          ra
    nop
#endif
END(BootInitUart)

#-------------------------------------------------------------------------------------------------
# BootInitRiuBitMask
# @param  None
# @return None
# @note   RIU bitmask
#-------------------------------------------------------------------------------------------------
BootInitRiuBitMask:

    lw      t1, (0xBF000000 + (0x0101886 << 1))
    or      t1, t1, (0x01 << 5)
    sw      t1, (0xBF000000 + (0x101886 << 1))

    lw      t1, (0xBF000000 + (0x00012E << 1))
    or      t1, t1, (0x01 << 0)
    sw      t1, (0xBF000000 + (0x00012E << 1))

    lw      t1, (0xBF000000 + (0x10012E << 1))
    or      t1, t1, (0x01 << 0)
    sw      t1, (0xBF000000 + (0x10012E << 1))

    jr      ra
    nop

LEAF(BootRomInitFpu)

    mfc0    t1, C0_Config1
	nop
    and     t1, t1, 0x01            #//Check Status of FPU Implemented
    beq     t1, zero, BootRomInitFpuExit
    nop

    mfc0    t0, C0_Status
	nop
    or      t0, t0, (0x01 << 29)    #//Enable Co-Processor 1
    or      t0, t0, (0x01 << 26)    #//Set floating point registers can contain any datatype
    mtc0    t0, C0_Status
    nop

    move    t0, zero                #//FS = FO = FN = 0: IEEE-compliant mode
    ctc1    t0, C1_FCSR
    nop

BootRomInitFpuExit:
    jr          ra
    nop

END(BootRomInitFpu)

#-------------------------------------------------------------------------------------------------
# BootRomDisableAvd
# @param  None
# @return None
# @note   Disable AVD before MIU init
#-------------------------------------------------------------------------------------------------
LEAF(BootRomDisableAvd)

    #//AVD Reset
    RREG        t0, (0xbf000000 + (0x10362a << 1))
    or          t0, t0, 0x0010
    sw          t0, (0xbf000000 + (0x10362a << 1))

    jr          ra
    nop

END(BootRomDisableAvd)

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