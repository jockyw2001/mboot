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
    .globl      BootRom_InitPeripheral
BootRom_InitPeripheral:
    move        k0, ra
	
    bal         BOOTRomInitTimer
    nop

    #//Initial related settings of UART
    bal         BootInitUart
    nop

    #//Disable ADV before MIU initiation
    bal     	BootRomDisableAvd
    nop

#ifdef CONFIG_MSTAR_32BIT_RIU_MASK_ENABLE
    bal     	BootInitRiuBitMask
    nop
#endif
    bal         BootRomInitFpu
    nop


#ifndef MSTAR_BD_FPGA
    #//Initial related settings of SPI flash clock
    bal         BootRomInitSpiFlashClock
    nop

    bal         BootRomChangeSpiFlashClock
    nop
#endif
    move        ra, k0
    jr          ra 

 
#-------------------------------------------------------------------------------------------------
# BOOTRomInitTimer  Init Timer
# @param  None
# @return None
# @note   do PIU Timer init
#-------------------------------------------------------------------------------------------------
BOOTRomInitTimer:

# initalize the PIU timer1 to measure boot time
    WREG  0xBF006088, 0xFFFF
    WREG  0xBF00608C, 0xFFFF
    WREG  0xBF006080, 0x0000
    WREG  0xBF006080, 0x0001
# Clear Timer0 for STR to prevent stuck by DELAYUS
    WREG  0xBF006048, 0xFFFF
    WREG  0xBF00604C, 0xFFFF
    WREG  0xBF006040, 0x0000
    WREG  0xBF006040, 0x0000

    jr          ra
    nop

#-------------------------------------------------------------------------------------------------
# BootRomInitSpiFlashClock SPI Clock init
# @param  None
# @return None
# @note   do SPI Clock init
#-------------------------------------------------------------------------------------------------
BootRomInitSpiFlashClock:

        WREG_B      0xBF0015C4, 0xFF        #//Set CSZ deselect time to 16 SPI clock cycles
                                        #//Set CSZ setup time to 16 SPI clock cycles
        WREG_B      0xBF0015C5, 0x0F        #//Set CSZ hold time to 16 SPI clock cycles
        WREG_B      0xBF0015C8, 0x01        #//Enable fast read mode

        jr          ra
        nop

#-------------------------------------------------------------------------------------------------
# BootRomChangeSpiFlashClock
# @param  None
# @return None
# @note   change SPI clock
#-------------------------------------------------------------------------------------------------
BootRomChangeSpiFlashClock:

        WREG_B      0xBF001C81, 0x04        #//Set clock of clk_spi to 86MHz
        WREG_B      0xBF001C81, 0x44        #//Switch clk_spi between clk_mcu_p and clk_ext_xtali_buf
        WREG_B      0xBF001C80, 0x80        #//Set clock of MCU to 170MHz

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

    WREG        0xbf20164c, 0x0C01      #//Set CLK_UART0 to 123Mhz

.if (BOOTINFO_UART0_ENABLE == 1)
    RREG        t3, 0xbf001c24          #//Enable UART Rx via PAD_DDCA for dig_mux
    ori         t3, t3, 0x0800
    sw          t3, 0xbf001c24

    RREG        t3, 0xbf203d4c          #//Select UART0 source to PIU UART0
    or          t3, t3, 0x0004
    sw          t3, 0xbf203d4c

.endif

.if (BOOTINFO_UART2_ENABLE == 1)
    RREG        t3, 0xbf203d4c          #//Select UART2 source to PIU UART0
    ori         t3, t3, 0x0400
    sw          t3, 0xbf203d4c

    WREG        0xbf203d50 0x0000       #//Set UART4 source to None

    RREG        t3, 0xbf203c08          #//Set third UART mode: RX2/TX2 uses PAD_UART_RX2/PAD_UART_TX2
    ori         t3, t3, 0x0800
    sw          t3, 0xbf203c08
.endif

    RREG        t3, 0xbf201318          #//Enable Divisor Latch Access
    ori         t3, t3, 0x0080
    sw          t3, 0xbf201318

    WREG        0xbf201300 UART_DIVISOR_L   #//Set Divisor Latch LSB
    WREG        0xbf201308 UART_DIVISOR_H   #//Set Divisor Latch MSB

    RREG        t3, 0xbf201318          #//Disable Divisor Latch Access
    and         t3, t3, 0xFF7F
    sw          t3, 0xbf201318

    WREG        0xbf201310 0x0000
    WREG        0xbf201310 0x0007       #//Enable FIFO and Clear TX/RX FIFO

    WREG        0xbf201318 0x0000
    WREG        0xbf201318 0x0003       #//8-bit Data / 1-bit Stop / No Parity Bit
    WREG        0xbf201320 0x0000       #//Set DTS/RTS to 1
    WREG        0xbf201308 0x0000       #//Disable Interrupt

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
#endif

    jr          ra
    nop

    END(BootInitUart)

#-------------------------------------------------------------------------------------------------
# BootInitRiuBitMask
# @param  None
# @return None
# @note   RIU bitmask
#-------------------------------------------------------------------------------------------------
    BootInitRiuBitMask:

        lw      t1, 0xBF20310C
        or      t1, t1, (0x01 << 5)
        sw      t1, 0xBF20310C

        lw      t1, 0xBF00025C
        or      t1, t1, (0x01 << 0)
        sw      t1, 0xBF00025C

        lw      t1, 0xBF20025C
        or      t1, t1, (0x01 << 0)
        sw      t1, 0xBF20025C

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

    RREG        t9, 0xBF206C54
    or          t9, t9, 0x0010
    REG_C2M     t9, 0xBF206C54

    jr          ra
    nop

END(BootRomDisableAvd)



