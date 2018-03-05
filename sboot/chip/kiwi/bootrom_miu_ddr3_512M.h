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

#if ENABLE_MEM1866MHz

const MS_REG_INIT MIU_PreInit[] =
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
    _RV32_2(0x101202, 0x0000),
    _RV32_2(0x101246, 0xfffe),
    _RV32_2(0x101266, 0xffff),
    _RV32_2(0x101286, 0xffff),
    _RV32_2(0x1012a6, 0xffff),
	//--------------------
	//set DDRPLL0 1866MHz
	//--------------------
    _RV32_2(0x110d36, 0x4000),
    _RV32_2(0x110d34, 0x0400),
	//1866
    _RV32_2(0x110D30, 0xf45a),  //923MHz
    _RV32_2(0x110D32, 0x001d),

	//Avoid DDRPLL unknown(MCLK05X & MCLK025X) by reset ddrat[28]
    _RV32_2(0x110d22, 0x2000),
    _RV32_2(0x110d24, 0x2000),
    _RV32_2(0x110d24, 0x3000),
    _RV32_2(0x110d24, 0x2000),
	
	_END_OF_TBL32_,
    MIU_VER
};
	
	
	
const MS_REG_INIT MIU_DDR_Init[] =
{	
	//---------------------------
	//set DDR3_16_8X_CL11_1866
	//---------------------------
    _RV32_2(0x101202, 0x03a3),
    _RV32_2(0x101204, 0x000c),  //RD Timing 
    _RV32_2(0x101206, 0x1d38),  //[12:12] reg_cke_always_on	
    _RV32_2(0x101208, 0x20dd),
    _RV32_2(0x10120a, 0x2e86),
    _RV32_2(0x10120c, 0xb8f9),
    _RV32_2(0x10120e, 0x40f3),
    _RV32_2(0x101210, 0x1f14),
    _RV32_2(0x101212, 0x4000),
    _RV32_2(0x101214, 0x8020),
    _RV32_2(0x101216, 0xc000),
    _RV32_2(0x1012d2, 0x9000),  //reg_dram_size
    _RV32_2(0x110d02, 0xaaaa),
    _RV32_2(0x110d04, 0x0080),
    _RV32_2(0x110d3c, 0x1333),  //odtm_dly & odtm_skew align to dqsm enable
    _RV32_2(0x110d4e, 0x0033),
    _RV32_2(0x110d3e, 0x1133),  //CLK skew
    _RV32_2(0x110d4c, 0x3333),
    _RV32_2(0x110d0a, 0x1022),
    _RV32_2(0x110d50, 0x1111),
    _RV32_2(0x110d38, 0x0066),  //DQSDQ
    _RV32_2(0x110d3a, 0x3030),  //DQSM,CMD phase
    _RV32_2(0x110d48, 0x0033),
    _RV32_2(0x110d4a, 0x0002),
    _RV32_2(0x110d52, 0x0000),
    _RV32_2(0x110d74, 0x0066),
    _RV32_2(0x110d78, 0x0002),  //[ 7: 0] reg_sel_vref  02
    _RV32_2(0x110d7a, 0x3800),  //[15: 8] reg_sel_zq  38 //CS phase
    _RV32_2(0x110d6c, 0x5555),
    _RV32_2(0x110d6e, 0x5766),  //Read phase
    _RV32_2(0x110d0e, 0x00a9),
    _RV32_2(0x110d0e, 0x00e9),
    _RV32_2(0x110d0e, 0x00a9),
    _RV32_2(0x10121e, 0x0c01),  //miu software reset
    _RV32_2(0x10121e, 0x0c00),
    _RV32_2(0x110d00, 0x0039),
    _RV32_2(0x110d00, 0x0031),
    _RV32_2(0x110d00, 0x0021),  //[    9] reg_odt0_en
                                                           //[   10] reg_odt1_en
    _RV32_2(0x110d7c, 0x0007),  //[    5] reg_ddr_tree_ldo_pd 0
    _RV32_2(0x110d7e, 0x0000),
    _RV32_2(0x110d54, 0xb000),  //[15:14] reg_cmd_mode_sel
                                                           //00:default mode 01:2cmd mode 10:2cmd mode 11:2cmd mode(cs)
    _RV32_2(0x110d08, 0x003f),  //[ 5: 0] reg_rx_en

	//-----------
	// DQSM RST  
	//-----------
    _RV32_2(0x110d0e, 0x00a9),
    _RV32_2(0x110d0e, 0x00e9),
    _RV32_2(0x110d0e, 0x00a9),

    _RV32_2(0x110d74, 0x0066),  //trig lvl
    //using drvn default value in kiwi
    //_RV32_2(0x110d58, 0x0033),  //cs drvn/drvp
    _RV32_2(0x110d5c, 0x5555),  //MIU CLK/CMD/DQ Driving
    _RV32_2(0x110d5e, 0x5555),  //MIU CLK/CMD/DQ Driving

    _RV32_2(0x110D28, 0x0011),  //SSC 1%
    _RV32_2(0x110D2A, 0x0482),

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

const MS_REG_INIT Samsung_MIU_DDR_Init[] =
{	
	//---------------------------
	//set DDR3_16_8X_CL11_1866
	//---------------------------
    _RV32_2(0x101202, 0x03a3),
    _RV32_2(0x101204, 0x000c),  //RD Timing 
    _RV32_2(0x101206, 0x1d38),  //[12:12] reg_cke_always_on	
    _RV32_2(0x101208, 0x20dd),
    _RV32_2(0x10120a, 0x2e86),
    _RV32_2(0x10120c, 0xb8f9),
    _RV32_2(0x10120e, 0x40f3),
    _RV32_2(0x101210, 0x1f14),
    _RV32_2(0x101212, 0x4000),
    _RV32_2(0x101214, 0x8020),
    _RV32_2(0x101216, 0xc000),
    _RV32_2(0x1012d2, 0x9000),  //reg_dram_size
    _RV32_2(0x110d02, 0xaaaa),
    _RV32_2(0x110d04, 0x0080),
    _RV32_2(0x110d3c, 0x1333),  //odtm_dly & odtm_skew align to dqsm enable
    _RV32_2(0x110d4e, 0x0033),
    _RV32_2(0x110d3e, 0x1133),  //CLK skew
    _RV32_2(0x110d4c, 0x3333),
    _RV32_2(0x110d0a, 0x1022),
    _RV32_2(0x110d50, 0x1111),
    _RV32_2(0x110d38, 0x0066),  //DQSDQ
    _RV32_2(0x110d3a, 0x2020),  //DQSM,CMD phase
    _RV32_2(0x110d48, 0x0033),
    _RV32_2(0x110d4a, 0x0002),
    _RV32_2(0x110d52, 0x0000),
    _RV32_2(0x110d74, 0x0066),
    _RV32_2(0x110d78, 0x0002),  //[ 7: 0] reg_sel_vref  02
    _RV32_2(0x110d7a, 0x3800),  //[15: 8] reg_sel_zq  38 //CS phase
    _RV32_2(0x110d6c, 0x5555),
    _RV32_2(0x110d6e, 0x6677),  //Read phase
    _RV32_2(0x110d0e, 0x00a9),
    _RV32_2(0x110d0e, 0x00e9),
    _RV32_2(0x110d0e, 0x00a9),
    _RV32_2(0x10121e, 0x0c01),  //miu software reset
    _RV32_2(0x10121e, 0x0c00),
    _RV32_2(0x110d00, 0x0039),
    _RV32_2(0x110d00, 0x0031),
    _RV32_2(0x110d00, 0x0021),  //turn off asic ODT 
    _RV32_2(0x110d7c, 0x0007),  //[    5] reg_ddr_tree_ldo_pd 0
    _RV32_2(0x110d7e, 0x0000),
    _RV32_2(0x110d54, 0xb000),  //[15:14] reg_cmd_mode_sel
                                                           //00:default mode 01:2cmd mode 10:2cmd mode 11:2cmd mode(cs)
    _RV32_2(0x110d08, 0x003f),  //[ 5: 0] reg_rx_en

	//-----------
	// DQSM RST  
	//-----------
    _RV32_2(0x110d0e, 0x00a9),
    _RV32_2(0x110d0e, 0x00e9),
    _RV32_2(0x110d0e, 0x00a9),

    _RV32_2(0x110d74, 0x0066),  //trig lvl
    //using drvn default value in kiwi
    //_RV32_2(0x110d58, 0x0033),  //cs drvn/drvp
    _RV32_2(0x110d5c, 0x5555),  //MIU CLK/CMD/DQ Driving
    _RV32_2(0x110d5e, 0x5555),  //MIU CLK/CMD/DQ Driving

    _RV32_2(0x110D28, 0x0011),  //SSC 1%
    _RV32_2(0x110D2A, 0x0482),

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

#if 0 //if mcu3 reset
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
    _RV32_2(0x101202, 0x0000),
    _RV32_2(0x101246, 0xfffe),
    _RV32_2(0x101266, 0xffff),
    _RV32_2(0x101286, 0xffff),
    _RV32_2(0x1012a6, 0xffff),
	//--------------------
	//set DDRPLL0 1866MHz
	//--------------------
    _RV32_2(0x110d36, 0x4000),
    _RV32_2(0x110d34, 0x0400),
	//1866
    _RV32_2(0x110D30, 0xf45a),  //923MHz
    _RV32_2(0x110D32, 0x001d),

	//Avoid DDRPLL unknown(MCLK05X & MCLK025X) by reset ddrat[28]
    _RV32_2(0x110d22, 0x2000),
    _RV32_2(0x110d24, 0x2000),
    _RV32_2(0x110d24, 0x3000),
    _RV32_2(0x110d24, 0x2000),
	
    //_RV32_2(0x101252, 0xffff), //group 0 last done z check in select
    //_RV32_2(0x101272, 0xffff), //group 1 last done z check in select
    //_RV32_2(0x101292, 0xffff), //group 2 last done z check in select
    //_RV32_2(0x1012b2, 0xffff), //group 3 last done z check in select

	_END_OF_TBL32_,
};

const MS_REG_INIT MIU_STR_DDR_Init[] =
{	
	//---------------------------
	//set DDR3_16_8X_CL11_1866
	//---------------------------
    _RV32_2(0x101202, 0xf3a3),
    _RV32_2(0x101204, 0x000c),  //RD Timing 
    _RV32_2(0x101206, 0x1d38),  //[12:12] reg_cke_always_on	
    _RV32_2(0x101208, 0x20dd),
    _RV32_2(0x10120a, 0x2e86),
    _RV32_2(0x10120c, 0xb8f9),
    _RV32_2(0x10120e, 0x40f3),
    _RV32_2(0x101210, 0x1f14),
    _RV32_2(0x101212, 0x4004),
    _RV32_2(0x101214, 0x8020),
    _RV32_2(0x101216, 0xc000),
    _RV32_2(0x1012d2, 0x9000),  //reg_dram_size
    _RV32_2(0x110d02, 0xaaaa),
    _RV32_2(0x110d04, 0x0080),
    _RV32_2(0x110d3c, 0x9333),  //odtm_dly & odtm_skew align to dqsm enable
    _RV32_2(0x110d4e, 0x0033),
    _RV32_2(0x110d3e, 0x1133),  //CLK skew
    _RV32_2(0x110d4c, 0x3333),
    _RV32_2(0x110d0a, 0x1100),
    _RV32_2(0x110d50, 0x1111),
    _RV32_2(0x110d38, 0x0066),  //DQSDQ
    _RV32_2(0x110d3a, 0x3030),  //DQSM,CMD phase
    _RV32_2(0x110d48, 0x0033),
    _RV32_2(0x110d4a, 0x0002),
    _RV32_2(0x110d52, 0x0000),
    _RV32_2(0x110d74, 0x0066),
    _RV32_2(0x110d78, 0x0002),  //[ 7: 0] reg_sel_vref  02
    _RV32_2(0x110d7a, 0x3800),  //[15: 8] reg_sel_zq  38 //CS phase
    _RV32_2(0x110d6c, 0x5555),
    _RV32_2(0x110d6e, 0x5566),  //Read phase
    _RV32_2(0x110d0e, 0x00a9),
    _RV32_2(0x110d0e, 0x00e9),
    _RV32_2(0x110d0e, 0x00a9),
    _RV32_2(0x10121e, 0x0c01),  //miu software reset
    _RV32_2(0x10121e, 0x0c00),
    _RV32_2(0x110d00, 0x0039),
    _RV32_2(0x110d00, 0x0039),
    _RV32_2(0x110d00, 0x0229),  //[    9] reg_odt0_en
                                                           //[   10] reg_odt1_en
    _RV32_2(0x110d7c, 0x0007),  //[    5] reg_ddr_tree_ldo_pd 0
    _RV32_2(0x110d7e, 0x0000),
    _RV32_2(0x110d54, 0xb070),  //[15:14] reg_cmd_mode_sel
                                                           //00:default mode 01:2cmd mode 10:2cmd mode 11:2cmd mode(cs)
    _RV32_2(0x110d08, 0x0000),  //[ 5: 0] reg_rx_en

	//-----------
	// DQSM RST  
	//-----------
    _RV32_2(0x110d0e, 0x00a9),
    _RV32_2(0x110d0e, 0x00e9),
    _RV32_2(0x110d0e, 0x00a9),

    _RV32_2(0x110d74, 0x0066),  //trig lvl
    //using drvn default value in kiwi
    //_RV32_2(0x110d58, 0x0033),  //cs drvn/drvp
    _RV32_2(0x110d5c, 0xd555),  //MIU CLK/CMD/DQ Driving
    _RV32_2(0x110d5e, 0xd555),  //MIU CLK/CMD/DQ Driving

    _RV32_2(0x110D28, 0x0011),  //SSC 1%
    _RV32_2(0x110D2A, 0x0482),

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

#endif
