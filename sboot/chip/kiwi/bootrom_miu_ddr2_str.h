//################################################################################
//#
//# Copyright (c) 2006-2010 MStar Semiconductor, Inc.
//# All rights reserved.
//#
//# Unless otherwise stipulated in writing, any and all information contained
//# herein regardless in any format shall remain the sole proprietary of
//# MStar Semiconductor Inc. and be kept in strict confidence
//# (¡§MStar Confidential Information¡¨) by the recipient.
//# Any unauthorized act including without limitation unauthorized disclosure,
//# copying, use, reproduction, sale, distribution, modification, disassembling,
//# reverse engineering and compiling of the contents of MStar Confidential
//# Information is unlawful and strictly prohibited. MStar hereby reserves the
//# rights to any and all damages, losses, costs and expenses resulting therefrom.
//#
//################################################################################

#if ENABLE_MEM1333MHz

const MS_REG_INIT MIU_STR_PreInit[] =
{
    //================================================
    // Start MIU init !!!
    //================================================
    _RV32_2(0x101220, 0x0000),
    _RV32_2(0x10121e, 0x0c00),
    _RV32_2(0x10121e, 0x0c00),
    _RV32_2(0x10121e, 0x0c00),
    _RV32_2(0x10121e, 0x0c01),
    _RV32_2(0x10121e, 0x0c00),
    _RV32_2(0x101246, 0xfffe),
    _RV32_2(0x101266, 0xffff),
    _RV32_2(0x101286, 0xffff),
    _RV32_2(0x1012a6, 0xffff),
//--------------------
//set DDRPLL0 1333MHz
//--------------------
    _RV32_2(0x110d36, 0x4000),
    _RV32_2(0x110d34, 0x0400),
//1333
    _RV32_2(0x110d30, 0xe6c1),  //659.835MHz
    _RV32_2(0x110d32, 0x0029),

//Avoid DDRPLL unknown(MCLK05X & MCLK025X) by reset ddrat[28]	
    _RV32_2(0x110d22, 0x2000),
    _RV32_2(0x110d24, 0x2000),
    _RV32_2(0x110d24, 0x3000),
    _RV32_2(0x110d24, 0x2000),
	
	_END_OF_TBL32_,
    MIU_VER
};
	
	
	
const MS_REG_INIT MIU_STR_DDR_Init[] =
{	
//---------------------------
// set DDR2-16-8X-CL9 (2CMD)
//---------------------------
    _RV32_2(0x101202, 0xf392),
    _RV32_2(0x101204, 0x000a), //RD Timing
    _RV32_2(0x101206, 0x0420), //[12:12] reg_cke_always_on	
    _RV32_2(0x101208, 0x1e99),
    _RV32_2(0x10120a, 0x2788),
    _RV32_2(0x10120c, 0x75a8),
    _RV32_2(0x10120e, 0x206a),
    _RV32_2(0x101210, 0x0003),
    _RV32_2(0x101212, 0x4004),
    _RV32_2(0x101214, 0x8000),
    _RV32_2(0x101216, 0xc000),
    _RV32_2(0x1012d2, 0x6000), //dram size
    _RV32_2(0x110d54, 0x0070),
    _RV32_2(0x110d00, 0x0018),
    _RV32_2(0x110d38, 0x0055), //DQSDQ
    _RV32_2(0x110d3a, 0x0000), //DQSM,CMD phase
    _RV32_2(0x110d48, 0x0033),
    _RV32_2(0x110d4a, 0x0003),
    _RV32_2(0x110d02, 0x0000),
    _RV32_2(0x110d04, 0x0000),
    _RV32_2(0x110d3c, 0x1177),
    _RV32_2(0x110d4e, 0x0077),
    _RV32_2(0x110d3e, 0x1011),
    _RV32_2(0x110d0a, 0x1100),
    _RV32_2(0x110d50, 0x1111),
    _RV32_2(0x110d52, 0x0000),
    _RV32_2(0x110d6e, 0x6666),  //Read phase
    _RV32_2(0x110d6a, 0x00c2),
    _RV32_2(0x110d68, 0x00c2),
    _RV32_2(0x110d54, 0x0070),  //[15:14] reg_cmd_mode_sel
                                //00:default mode 01:2cmd mode 10:2cmd mode 11:2cmd mode(cs)
    _RV32_2(0x110d0e, 0x00a3),
    _RV32_2(0x110d0e, 0x00e3),
    _RV32_2(0x110d0e, 0x00a3),
    _RV32_2(0x10121e, 0x0c01),  //miu software reset
    _RV32_2(0x10121e, 0x0c00),
    _RV32_2(0x110d7e, 0x0000),
    _RV32_2(0x110d00, 0x0018),
    _RV32_2(0x110d00, 0x0018),
    _RV32_2(0x110d00, 0x0008),  //[    9] reg_odt0_en
                                                           //[   10] reg_odt1_en	
    _RV32_2(0x110d7c, 0x0007),  //[    5] reg_ddr_tree_ldo_pd 0
    _RV32_2(0x110d02, 0xaaaa),
    _RV32_2(0x110d08, 0x003f),  //[ 5: 0] reg_rx_en
//-----------
// DQSM RST
//-----------
    _RV32_2(0x110d0e, 0x00a3),
    _RV32_2(0x110d0e, 0x00e3),
    _RV32_2(0x110d0e, 0x00a3),

    _RV32_2(0x110d74, 0x0044),  //trig lvl
    _RV32_2(0x110d5c, 0x5555),  //MIU CLK/CMD/DQ Driving
    _RV32_2(0x110d5e, 0x5555),  //MIU CLK/CMD/DQ Driving

    _RV32_2(0x110d28, 0x0021),  //SSC 1%
    _RV32_2(0x110d2a, 0x0339),

//-------------
// BW setting
//-------------
    _RV32_2(0x101240, 0x8015),
    _RV32_2(0x101260, 0x8015),
    _RV32_2(0x101280, 0x8015),
    _RV32_2(0x1012a0, 0x8015),

//$display($time," start to program DLL0 ");
    _RV32_2(0x110d62, 0x0032),  //pulse width
    _RV32_2(0x110d64, 0xf200),  //phase, code
    _RV32_2(0x110d68, 0x0020),  //dll0_code
    _RV32_2(0x110d6a, 0x0020),  //dll1_code
    _RV32_2(0x110d60, 0x000c),  //sw rst
    _RV32_2(0x110d60, 0x0008),  //sw rst
    _RV32_2(0x110d60, 0x2370),  //enable hw, dyn, rd_avg

	
    _END_OF_TBL32_,
    MIU_VER
};

#endif
