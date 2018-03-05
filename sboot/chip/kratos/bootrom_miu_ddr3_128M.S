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

#if ENABLE_MEM1600MHz

#include "boot.inc"

#ifndef _ArchDefs_h_
#include "ArchDefs.h"
#endif

#ifndef _BOARD_H_
#include "Board.h"
#endif

#ifndef _DRV_MIU_INIT_H_
#include "drv_miu_init.h"
#endif

#ifndef _MIUSEL_KRATOS_2MIU_H_
#include "MIUSEL_KRATOS_2MIU.h"
#endif

#define MIU_AUTO_DRV_N_TRG_LVL              0
#define MIU_TRIG_LVL                        0
#define MIU_AUTO_DRV                        0
#define MIU_AUTO_SKEW                       0
#define MIU_AUTO_DDL                        0
#define MIU_AUTO_DQS                        0

    .text
	.set    	noreorder

    .global     BootInitMiu
    .ent        BootInitMiu

BootInitMiu:

    move        t7, ra

    DBGREG      0x0020

    #//================================================
    #// Start MIU init !!!
    #//================================================
    WREG        (REG_ADDR_BASE + (0x101220 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x10121e << 1)), 0x0c00
    WREG        (REG_ADDR_BASE + (0x10121e << 1)), 0x0c00
    WREG        (REG_ADDR_BASE + (0x10121e << 1)), 0x0c00
    WREG        (REG_ADDR_BASE + (0x10121e << 1)), 0x0c01
    WREG        (REG_ADDR_BASE + (0x10121e << 1)), 0x0c00
    WREG        (REG_ADDR_BASE + (0x101202 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x101246 << 1)), 0xfffe
    WREG        (REG_ADDR_BASE + (0x101266 << 1)), 0xffff
    WREG        (REG_ADDR_BASE + (0x101286 << 1)), 0xffff
    WREG        (REG_ADDR_BASE + (0x1012a6 << 1)), 0xffff
#//--------------------
#//set DDRPLL0 1600MHz
#//--------------------
    WREG        (REG_ADDR_BASE + (0x110d36 << 1)), 0x4000
    WREG        (REG_ADDR_BASE + (0x110d34 << 1)), 0x0400
#//1600
    WREG        (REG_ADDR_BASE + (0x110D30 << 1)), 0xE8BA  #//792MHz
    WREG        (REG_ADDR_BASE + (0x110D32 << 1)), 0x0022

    WREG        (REG_ADDR_BASE + (0x110d22 << 1)), 0x2000
#//Avoid DDRPLL unknown(MCLK05X & MCLK025X) by reset ddrat[28]
    WREG        (REG_ADDR_BASE + (0x110d24 << 1)), 0x2000
    WREG        (REG_ADDR_BASE + (0x110d24 << 1)), 0x3000
    WREG        (REG_ADDR_BASE + (0x110d24 << 1)), 0x2000
#//------------------
#// Wait PLL Stable
#//------------------
    DELAYUS     10000
#//---------------------------
#//set DDR3_16_8X_CL11_1600
#//---------------------------
    WREG        (REG_ADDR_BASE + (0x101202 << 1)), 0x03a3
    WREG        (REG_ADDR_BASE + (0x101204 << 1)), 0x004b  #//RD Timing 
    WREG        (REG_ADDR_BASE + (0x101206 << 1)), 0x1d30  #//[12:12] reg_cke_always_on	
    WREG        (REG_ADDR_BASE + (0x101208 << 1)), 0x1cbb
    WREG        (REG_ADDR_BASE + (0x10120a << 1)), 0x2767
    WREG        (REG_ADDR_BASE + (0x10120c << 1)), 0xb6c8
    WREG        (REG_ADDR_BASE + (0x10120e << 1)), 0x4080
    WREG        (REG_ADDR_BASE + (0x101210 << 1)), 0x1d70
    WREG        (REG_ADDR_BASE + (0x101212 << 1)), 0x4004
    WREG        (REG_ADDR_BASE + (0x101214 << 1)), 0x8018
    WREG        (REG_ADDR_BASE + (0x101216 << 1)), 0xc000
    WREG        (REG_ADDR_BASE + (0x10122c << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x110d02 << 1)), 0xaaaa
    WREG        (REG_ADDR_BASE + (0x110d04 << 1)), 0x0080
    WREG        (REG_ADDR_BASE + (0x110d3c << 1)), 0x1133
    WREG        (REG_ADDR_BASE + (0x110d4e << 1)), 0x0033
    WREG        (REG_ADDR_BASE + (0x110d3e << 1)), 0x1011
    WREG        (REG_ADDR_BASE + (0x110d4c << 1)), 0x1133
    WREG        (REG_ADDR_BASE + (0x110d0a << 1)), 0x1022
    WREG        (REG_ADDR_BASE + (0x110d50 << 1)), 0x1111
    WREG        (REG_ADDR_BASE + (0x110d38 << 1)), 0x0056  #//DQSDQ
    WREG        (REG_ADDR_BASE + (0x110d3a << 1)), 0x3031  #//DQSM
    WREG        (REG_ADDR_BASE + (0x110d48 << 1)), 0x0033
    WREG        (REG_ADDR_BASE + (0x110d4a << 1)), 0x0002
    WREG        (REG_ADDR_BASE + (0x110d52 << 1)), 0x0002
    WREG        (REG_ADDR_BASE + (0x110d74 << 1)), 0x2077
    WREG        (REG_ADDR_BASE + (0x110d78 << 1)), 0x0002  #//[ 7: 0] reg_sel_vref  02
    WREG        (REG_ADDR_BASE + (0x110d7a << 1)), 0x3801  #//[15: 8] reg_sel_zq  38 //CS phase
    WREG        (REG_ADDR_BASE + (0x110d6c << 1)), 0x5555
    WREG        (REG_ADDR_BASE + (0x110d6e << 1)), 0x6666
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00a7
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00e7
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00a7
    WREG        (REG_ADDR_BASE + (0x10121e << 1)), 0x0c01  #//miu software reset
    WREG        (REG_ADDR_BASE + (0x10121e << 1)), 0x0c00
    WREG        (REG_ADDR_BASE + (0x110d00 << 1)), 0x0039
    WREG        (REG_ADDR_BASE + (0x110d00 << 1)), 0x0031
    WREG        (REG_ADDR_BASE + (0x110d00 << 1)), 0x0021  #//[    9] reg_odt0_en
                                                           #//[   10] reg_odt1_en
    WREG        (REG_ADDR_BASE + (0x110d7c << 1)), 0x0007  #//[    5] reg_ddr_tree_ldo_pd 0
    WREG        (REG_ADDR_BASE + (0x110d7e << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x110d54 << 1)), 0x8000  #//[15:14] reg_cmd_mode_sel
                                                           #//00:default mode 01:2cmd mode 10:2cmd mode 11:2cmd mode(cs)
    WREG        (REG_ADDR_BASE + (0x110d08 << 1)), 0x00ff  #//[ 5: 0] reg_rx_en

#//-----------
#// DQSM RST  
#//-----------
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00a7
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00e7
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00a7

    WREG        (REG_ADDR_BASE + (0x110d74 << 1)), 0x0006  #//trig lvl
    WREG        (REG_ADDR_BASE + (0x110d58 << 1)), 0x0033  #//cs drvn/drvp
    WREG        (REG_ADDR_BASE + (0x110d5c << 1)), 0x3333  #//drvn
    WREG        (REG_ADDR_BASE + (0x110d5e << 1)), 0x3333  #//drvp

    WREG        (REG_ADDR_BASE + (0x110D28 << 1)), 0x0017  #//SSC 1%
    WREG        (REG_ADDR_BASE + (0x110D2A << 1)), 0x03DE

#//-------------
#// BW setting  
#//-------------
    WREG        (REG_ADDR_BASE + (0x101240 << 1)), 0x8015
    WREG        (REG_ADDR_BASE + (0x101260 << 1)), 0x8015
    WREG        (REG_ADDR_BASE + (0x101280 << 1)), 0x8015
    WREG        (REG_ADDR_BASE + (0x1012a0 << 1)), 0x8015

#//$display($time," start to program DLL0 ");
    WREG        (REG_ADDR_BASE + (0x110d62 << 1)), 0x0032  #//pulse width
    WREG        (REG_ADDR_BASE + (0x110d64 << 1)), 0xf200  #//phase, code
    WREG        (REG_ADDR_BASE + (0x110d68 << 1)), 0x0020  #//dll0_code
    WREG        (REG_ADDR_BASE + (0x110d6a << 1)), 0x0020  #//dll1_code
    WREG        (REG_ADDR_BASE + (0x110d60 << 1)), 0x000c  #//sw rst
    WREG        (REG_ADDR_BASE + (0x110d60 << 1)), 0x0008  #//sw rst
    WREG        (REG_ADDR_BASE + (0x110d60 << 1)), 0x2370  #//enable hw, dyn, rd_avg

#//---------------------------------
#// Pack setting                    
#//---------------------------------
    WREG        (REG_ADDR_BASE + (0x101228 << 1)), 0x4090
#//-------------------------
#// Reduce latency setting  
#//-------------------------
    WREG        (REG_ADDR_BASE + (0x10122a << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x1012d4 << 1)), 0x0000

#//-------------------------
#// fifo stage setting  
#//-------------------------
    WREG        (REG_ADDR_BASE + (0x10121e << 1)), 0x1a00  #//[    3] reg_sw_init_done
                                                           #//[12: 8] reg_sync_out_threshold
                                                           #//[   14] reg_cmd_fifo_4_stage ==> 1 
    WREG        (REG_ADDR_BASE + (0x1012fc << 1)), 0x4400  #//[13:12] reg_sync_in_stage_disp
                                                           #//[15:14] reg_sync_in_stage_others ==>0

    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x0000
#//-----Wait 200us, (keep DRAM RESET = 0)-----
    DELAYUS     1000
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x0008
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x000c
#//-----Wait 500us, (keep DRAM RESET = 1 , CKE = 0)-----
    DELAYUS     1000
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x000e
#//-----Wait tXPR=400ns-----
    DELAYUS     1000
#//--------Initial DRAM start here!!!
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x001f
#//-----Wait init done-----
    DELAYUS     100000
#//--------Initial Done
#//--------DDR2 wait 400ns for tXPR(?), DDR3 wait 512T for tZQinit
    DELAYUS     1000
#//------------------------------
#// unmask all                   
#//------------------------------
    WREG        (REG_ADDR_BASE + (0x10121a << 1)), 0x0001
    WREG        (REG_ADDR_BASE + (0x101246 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x101266 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x101286 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x1012a6 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x10121e << 1)), 0x1a08  #//[    3] reg_sw_init_done
                                                           #//[12: 8] reg_sync_out_threshold
                                                           #//[   14] reg_cmd_fifo_4_stage ==> 1 

    WREG        (REG_ADDR_BASE + (0x1012fe << 1)), 0x00e1  #//change post arbiter priority to 1 ==>0 ==>2 ==>3
    WREG        (REG_ADDR_BASE + (0x1012fe << 1)), 0x01e1
    WREG        (REG_ADDR_BASE + (0x1012fe << 1)), 0x00e1
    #//=================================================================
    #// End of MIU init !!!
    #//=================================================================
    WREG        (REG_ADDR_BASE + (0x1012f0 << 1)), 0x0000

#//-------------------------
#// Auto phase
#//-------------------------
    WREG        (REG_ADDR_BASE + (0x101246 <<1 )), 0x7FFE   #//Mask MIU0_group_0
    bal         Miu0_AutoPhase
    nop

#//-------------------------
#// BIST test
#//-------------------------
    WREG        (REG_ADDR_BASE + (0x101246 <<1 )), 0x7FFE   #//Un-mask MIU0_group_0
    bal         Miu0_BistTest
    nop

    WREG        (REG_ADDR_BASE + (0x1012e0 << 1)), 0x0000   #//stop bist test

    WREG        (REG_ADDR_BASE + (0x101246 <<1 )), 0x0000   #//Unmask MIU0_group_0

    move        ra, t7
    jr          ra
    nop

    .size       BootInitMiu,.-BootInitMiu
    .end        BootInitMiu

#endif

