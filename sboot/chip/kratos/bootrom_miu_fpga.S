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

#ifdef CONFIG_MSTAR_KRATOS_BD_FPGA

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

    # swch 3
    WREG (REG_ADDR_BASE + (0x002BC6 << 1)), 0x3c11
    WREG (REG_ADDR_BASE + (0x002BCA << 1)), 0x403c
    WREG (REG_ADDR_BASE + (0x002BC4 << 1)), 0x0004

    # swch 4
    WREG (REG_ADDR_BASE + (0x101202 << 1)), 0x0858
    WREG (REG_ADDR_BASE + (0x101204 << 1)), 0x0185
    WREG (REG_ADDR_BASE + (0x101206 << 1)), 0x0120
    WREG (REG_ADDR_BASE + (0x101210 << 1)), 0x0031
    WREG (REG_ADDR_BASE + (0x101212 << 1)), 0x0000
    WREG (REG_ADDR_BASE + (0x10123e << 1)), 0x0001
    WREG (REG_ADDR_BASE + (0x10121e << 1)), 0x0c01
    WREG (REG_ADDR_BASE + (0x10121e << 1)), 0x0c08

    WREG (REG_ADDR_BASE + (0x101200 << 1)), 0x0008
    WREG (REG_ADDR_BASE + (0x101200 << 1)), 0x000c
    WREG (REG_ADDR_BASE + (0x101200 << 1)), 0x000e
    WREG (REG_ADDR_BASE + (0x101200 << 1)), 0x000f

_CheckDramData:
    lui         t0, 0xBF20
    ori         t0, t0,0x2400
    lw          t1, 0(t0)
    and         t1, t1, 0x8000
    beqz        t1, _CheckDramData
    nop

    #//================================================
    #// End of MIU init !!!             
    #//================================================

    move        ra, t7
    jr          ra
    nop

    .size       BootInitMiu,.-BootInitMiu
    .end        BootInitMiu

#endif

