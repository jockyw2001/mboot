//################################################################################
//#
//# Copyright (c) 2006-2010 MStar Semiconductor, Inc.
//# All rights reserved.
//#
//# Unless otherwise stipulated in writing, any and all information contained
//# herein regardless in any format shall remain the sole proprietary of
//# MStar Semiconductor Inc. and be kept in strict confidence
//# (!¡±MStar Confidential Information!¡L) by the recipient.
//# Any unauthorized act including without limitation unauthorized disclosure,
//# copying, use, reproduction, sale, distribution, modification, disassembling,
//# reverse engineering and compiling of the contents of MStar Confidential
//# Information is unlawful and strictly prohibited. MStar hereby reserves the
//# rights to any and all damages, losses, costs and expenses resulting therefrom.
//#
//################################################################################
#ifdef CONFIG_DRAM_MOBF 
//#-------------------------------------------------------------------------------------------------
//# InitMobf
//# @param
//# @return None
//# @note
//#-------------------------------------------------------------------------------------------------
const MS_REG_INIT DRAMOBF_TABLE_Init[] =
{	
    _RV32_1(0x123906, 0x4F),   //0x1239 bank, 0x03[7:0] = 0x4F //[3:0]: reg_tzpc2nonpm_rng_source_enable[3:0]= 4'hF, enable analog rc clk 
    _RV32_1(0x100be6, 0x10),   //0x100b bank, 0x73[7:0] = 0x10 //[3:0]: release clk_aesdma_gated
    _RV32_2(0x1708ec, 0xc001),
    _RV32_2(0x1708e8, 0xb5dc),
    _RV32_2(0x1708ea, 0x4632),
    _RV32_2(0x1708ec, 0x0002),
    _RV32_2(0x1708e8, 0x7552),
    _RV32_2(0x1708ea, 0x9389),
    _RV32_2(0x1708ec, 0x0003),
    _RV32_2(0x1708e8, 0x7334),
    _RV32_2(0x1708ea, 0x3498),
    _RV32_2(0x1708ec, 0x0004),
    _RV32_2(0x1708e8, 0x2856),
    _RV32_2(0x1708ea, 0x4948),
    _RV32_2(0x1708ec, 0x0005),
    _RV32_2(0x1708e8, 0x4712),
    _RV32_2(0x1708ea, 0x2928),
    _RV32_2(0x1708ec, 0x0006),
    _RV32_2(0x1708e8, 0x3639),
    _RV32_2(0x1708ea, 0x4529),
    _RV32_2(0x1708ec, 0x0007),
    _RV32_2(0x1708e8, 0x3421),
    _RV32_2(0x1708ea, 0x8555),
    _RV32_2(0x1708ec, 0x0008),
    _RV32_2(0x1708e8, 0x6915),
    _RV32_2(0x1708ea, 0xfeba),
    _RV32_2(0x1708ec, 0x0009),
    _RV32_2(0x1708e8, 0xedf1),
    _RV32_2(0x1708ea, 0x13dc),
    _RV32_2(0x1708ec, 0x000a),
    _RV32_2(0x1708e8, 0x78e4),
    _RV32_2(0x1708ea, 0xff69),
    _RV32_2(0x1708ec, 0x000b),
    _RV32_2(0x1708e8, 0xcbed),
    _RV32_2(0x1708ea, 0x3266),
    _RV32_2(0x1708ec, 0x000c),
    _RV32_2(0x1708e8, 0xcebc),
    _RV32_2(0x1708ea, 0x2643),
    _RV32_2(0x1708ec, 0x000d),
    _RV32_2(0x1708e8, 0x4523),
    _RV32_2(0x1708ea, 0x7867),
    _RV32_2(0x1708ec, 0x000e),
    _RV32_2(0x1708e8, 0x6587),
    _RV32_2(0x1708ea, 0x2143),
    _RV32_2(0x1708ec, 0x000f),
    _RV32_2(0x1708e8, 0x5397),
    _RV32_2(0x1708ea, 0x231f),
    _RV32_2(0x1708ec, 0x0010),
    _RV32_2(0x1708e8, 0xeecc),
    _RV32_2(0x1708ea, 0xffdd),
    _RV32_2(0x1708ec, 0x0011),
    _RV32_2(0x1708e8, 0x4623),
    _RV32_2(0x1708ea, 0x138c),
    _RV32_2(0x1708ec, 0x0012),
    _RV32_2(0x1708e8, 0x3245),
    _RV32_2(0x1708ea, 0x421f),
    _END_OF_TBL32_
};
#endif //dramobf
