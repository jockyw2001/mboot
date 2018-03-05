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

#ifndef _BOARD_H_
#include "Board.h"
#endif

#ifndef _DRV_MIU_INIT_H_
#include "drv_miu_str_init.h"
#endif

#ifndef _MIUSEL_KIWI_2MIU_H_
#include "MIUSEL_KIWI_2MIU.h"
#endif

    .text
    .set    	noreorder
    .global     BootInitMiu_STR
    .ent        BootInitMiu_STR
BootInitMiu_STR:
    move        t7, ra

    PUTK        'D'
    PUTK        'C'
    PUTK        '_'
    PUTK        'F'
    PUTK        'L'
    PUTK        'O'
    PUTK        'W'
    PUTK        '\n'
    PUTK        '\r'
#//with mcu3 rst
#if 1
    lui         a0, %hi(MIU_STR_PreInit)
    bal         MDrv_WriteRegTbl32
    ori         a0, %lo(MIU_STR_PreInit)
    
#//------------------
#// Wait PLL Stable
#//------------------
    DELAYUS     10000
	
    lui         a0, %hi(MIU_STR_DDR_Init)
    bal         MDrv_WriteRegTbl32
    ori         a0, %lo(MIU_STR_DDR_Init)

#//-------------------------
#// fifo stage setting  
#//-------------------------


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

    #========================
    #//Enter MIU Self-refresh
    #========================
#if (ENABLE_MEM1866MHz||ENABLE_MEM1600MHz)
    // [DDR3][enter]
    //<MIU0>
    WREG        (REG_ADDR_BASE + (0x101206 << 1)), 0x3d38
	WREG        (REG_ADDR_BASE + (0x10121e << 1)), 0x0000
	DELAYUS     2000
    WREG        (REG_ADDR_BASE + (0x101246 << 1)), 0xfffe
    WREG        (REG_ADDR_BASE + (0x101266 << 1)), 0xffff
    WREG        (REG_ADDR_BASE + (0x101286 << 1)), 0xffff
    WREG        (REG_ADDR_BASE + (0x1012a6 << 1)), 0xffff
    DELAYUS     1000
    WREG        (REG_ADDR_BASE + (0x101218 << 1)), 0x0400
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x002f
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x052e
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x002e
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x032e
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x002e
    WREG        (REG_ADDR_BASE + (0x101206 << 1)), 0x1d38
	DELAYUS     1000
    //<MIU0>
    WREG        (REG_ADDR_BASE + (0x101246 << 1)), 0xFFFF
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x202f
	DELAYUS     1000
	
	#//----------------------Down-----------------ddr3 anpower down
	#/*
    #WREG        (REG_ADDR_BASE + (0x101202 << 1)), 0xe3a3
    #WREG        (REG_ADDR_BASE + (0x101204 << 1)), 0x0c0c
    #DELAYUS     1000
    #WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00a8
    #WREG        (REG_ADDR_BASE + (0x110d2e << 1)), 0x00ff
    #WREG        (REG_ADDR_BASE + (0x110d00 << 1)), 0x0029 //rayray,reg_odt_en off
    #WREG        (REG_ADDR_BASE + (0x110d00 << 1)), 0x0039 //rayray,reg_odt_en off
    #WREG        (REG_ADDR_BASE + (0x110d7c << 1)), 0x000f
    #WREG        (REG_ADDR_BASE + (0x110d54 << 1)), 0xB070
    #WREG        (REG_ADDR_BASE + (0x110d08 << 1)), 0x0000
	#*/
    #// [DDR3][enter self refresh]

#elif (ENABLE_MEM1333MHz)
    // [DDR2][enter]
    //<MIU0>
    WREG        (REG_ADDR_BASE + (0x1012E0 << 1)), 0x0000
    
    WREG        (REG_ADDR_BASE + (0x101246 << 1)), 0xfffe  //mask group0(except g00 REF)
    WREG        (REG_ADDR_BASE + (0x101266 << 1)), 0xffff  //mask group1
    WREG        (REG_ADDR_BASE + (0x101286 << 1)), 0xffff  //mask group2
    WREG        (REG_ADDR_BASE + (0x1012a6 << 1)), 0xffff  //mask group3
    
    DELAYUS     20000
    WREG        (REG_ADDR_BASE + (0x101206 << 1)), 0x1420  //CKE always on bit[12] on
    
    WREG        (REG_ADDR_BASE + (0x101218 << 1)), 0x0400
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x002f  //off auto_ref
    
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x042f  //trig precharge all
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x052f  //trig precharge all
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x002f  //trig precharge all
    
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x022f  //trig refresh
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x032f  //trig refresh
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x002f  //trig refresh
	
    DELAYUS     20000
    //<MIU0>
    WREG        (REG_ADDR_BASE + (0x101246 << 1)), 0xFFFF  //mask g00
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x202f  //self refresh on
    DELAYUS     100000
    // [DDR2][enter self refresh]
#else
#error "Invalid Memory Frequency Selection"
#endif

    #========================
    #//Exit MIU Self-refresh
    #========================
#if (ENABLE_MEM1866MHz||ENABLE_MEM1600MHz)
    WREG        (REG_ADDR_BASE + (0x110d2e << 1)), 0x0000
    #//rayray,reg_odt_en on
    #//WREG        (REG_ADDR_BASE + (0x110d00 << 1)), 0x0229
    WREG        (REG_ADDR_BASE + (0x110d7c << 1)), 0x0007
    WREG        (REG_ADDR_BASE + (0x110d54 << 1)), 0xB000
    WREG        (REG_ADDR_BASE + (0x101204 << 1)), 0x000c
    WREG        (REG_ADDR_BASE + (0x101202 << 1)), 0x03a3
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x002f
    WREG        (REG_ADDR_BASE + (0x101246 << 1)), 0xfffe
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x002f
    DELAYUS     1000
    WREG        (REG_ADDR_BASE + (0x110d08 << 1)), 0x003f
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00a9
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00a9
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00e9
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00a9
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x032f
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x002f
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x001f
    DELAYUS     1000
    WREG        (REG_ADDR_BASE + (0x101206 << 1)), 0x1d38
    WREG        (REG_ADDR_BASE + (0x101246 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x101266 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x101286 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x1012a6 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x10121e << 1)), 0x0008

#elif (ENABLE_MEM1333MHz)
    WREG        (REG_ADDR_BASE + (0x110d2e << 1)), 0x0000 #//reg_pd_line
    WREG        (REG_ADDR_BASE + (0x110d00 << 1)), 0x0000 #//rayray,reg_odt_en on
    WREG        (REG_ADDR_BASE + (0x110d7c << 1)), 0x0007 #//reg_zq_pd off
    #//Power Up ddr_atop
    WREG        (REG_ADDR_BASE + (0x110d54 << 1)), 0x0000 #//reg_atop_pd off
    WREG        (REG_ADDR_BASE + (0x101204 << 1)), 0x000a #//reg_csz_oenz off
    WREG        (REG_ADDR_BASE + (0x101202 << 1)), 0x0392

    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x002f #//self refresh off
	DELAYUS     1000
    WREG        (REG_ADDR_BASE + (0x110d08 << 1)), 0x003f #//reg_rx_en on
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00a3 #//reg_en_mask on
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00a3
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00e3
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00a3

    WREG        (REG_ADDR_BASE + (0x101246 << 1)), 0xfffe #//unmask group00
	
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x022f #//trig refresh
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x032f #//trig refresh
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x002f #//trig refresh

    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x000f #//on auto_ref
    DELAYUS     50000
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x001f #//on odt
    DELAYUS     50000
    WREG        (REG_ADDR_BASE + (0x101206 << 1)), 0x1420 #//CKE always on bit[12] off
    DELAYUS     20000
    WREG        (REG_ADDR_BASE + (0x101246 << 1)), 0x0000 #//unmask group0
    WREG        (REG_ADDR_BASE + (0x101266 << 1)), 0x0000 #//unmask group1
    WREG        (REG_ADDR_BASE + (0x101286 << 1)), 0x0000 #//unmask group2
    WREG        (REG_ADDR_BASE + (0x1012a6 << 1)), 0x0000 #//unmask group3
    WREG        (REG_ADDR_BASE + (0x10121e << 1)), 0x0008 #//init done add 
    DELAYUS     10000
	
#else
#error "Invalid Memory Frequency Selection"
#endif

#endif
#//end of with mcu3


#if 0
    #========================
    #//Exit MIU Self-refresh
    #========================
#if (ENABLE_MEM1866MHz)
    WREG        (REG_ADDR_BASE + (0x110d2e << 1)), 0x0000
    #//rayray,reg_odt_en on
    #//WREG        (REG_ADDR_BASE + (0x110d00 << 1)), 0x0229
    WREG        (REG_ADDR_BASE + (0x110d7c << 1)), 0x0007
    WREG        (REG_ADDR_BASE + (0x110d54 << 1)), 0xB000
    WREG        (REG_ADDR_BASE + (0x101204 << 1)), 0x000c
    WREG        (REG_ADDR_BASE + (0x101202 << 1)), 0x03a3
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x002f
    WREG        (REG_ADDR_BASE + (0x101246 << 1)), 0xfffe
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x002f
    DELAYUS     1000
    WREG        (REG_ADDR_BASE + (0x110d08 << 1)), 0x003f
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00a9
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00a9
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00e9
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00a9
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x032f
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x002f
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x001f
    DELAYUS     1000
    WREG        (REG_ADDR_BASE + (0x101206 << 1)), 0x1d38
    WREG        (REG_ADDR_BASE + (0x101246 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x101266 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x101286 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x1012a6 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x10121e << 1)), 0x0008

#elif (ENABLE_MEM1333MHz)
    WREG        (REG_ADDR_BASE + (0x110d2e << 1)), 0x0000 #//reg_pd_line
    WREG        (REG_ADDR_BASE + (0x110d00 << 1)), 0x0000 #//rayray,reg_odt_en on
    WREG        (REG_ADDR_BASE + (0x110d7c << 1)), 0x0007 #//reg_zq_pd off
    #//Power Up ddr_atop
    WREG        (REG_ADDR_BASE + (0x110d54 << 1)), 0x0000 #//reg_atop_pd off
    WREG        (REG_ADDR_BASE + (0x101204 << 1)), 0x000a #//reg_csz_oenz off
    WREG        (REG_ADDR_BASE + (0x101202 << 1)), 0x0392

    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x002f #//self refresh off
	DELAYUS     1000
    WREG        (REG_ADDR_BASE + (0x110d08 << 1)), 0x003f #//reg_rx_en on
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00a3 #//reg_en_mask on
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00a3
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00e3
    WREG        (REG_ADDR_BASE + (0x110d0e << 1)), 0x00a3

    WREG        (REG_ADDR_BASE + (0x101246 << 1)), 0xfffe #//unmask group00
	
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x022f #//trig refresh
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x032f #//trig refresh
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x002f #//trig refresh

    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x000f #//on auto_ref
    DELAYUS     50000
    WREG        (REG_ADDR_BASE + (0x101200 << 1)), 0x001f #//on odt
    DELAYUS     50000
    WREG        (REG_ADDR_BASE + (0x101206 << 1)), 0x1420 #//CKE always on bit[12] off
    DELAYUS     20000
    WREG        (REG_ADDR_BASE + (0x101246 << 1)), 0x0000 #//unmask group0
    WREG        (REG_ADDR_BASE + (0x101266 << 1)), 0x0000 #//unmask group1
    WREG        (REG_ADDR_BASE + (0x101286 << 1)), 0x0000 #//unmask group2
    WREG        (REG_ADDR_BASE + (0x1012a6 << 1)), 0x0000 #//unmask group3
    WREG        (REG_ADDR_BASE + (0x10121e << 1)), 0x0008 #//init done add 
    DELAYUS     10000
	
#else
#error "Invalid Memory Frequency Selection"
#endif


#endif

#//-------------------------
#// DRAMOBF setting
#//-------------------------	
#ifdef CONFIG_DRAM_MOBF 	
    lui         a0, %hi(DRAMOBF_TABLE_Init)
    bal         MDrv_WriteRegTbl32
    ori         a0, %lo(DRAMOBF_TABLE_Init)
#endif	

    move        ra, t7
    jr          ra
    nop
    
    .size       BootInitMiu_STR,.-BootInitMiu_STR
    .end        BootInitMiu_STR