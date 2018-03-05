//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
#define _RV32_2(a, b)    { (a) << 1, b }



#if defined(MIU1_LEN) && (MIU1_LEN != 0)
#define ENABLE_MIU1 1
#else
#define ENABLE_MIU1 0
#endif
    
    //swch 4

    // After OTP disable TCF engine, it gererated a level triggered IRQ
    // And we need to clear this irq status to make RASP1 work normally
	{CHIPCFG_CMD_BASE, 0x1f2C},   //Bank 0x1628
    _RV32_2(0x2800, 0x0040),   //clear CFB's(TCF) interrupt who share IRQ60 with RASP1

    //MIU0 Software reset
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x0b3a, 0x0808),   //turn on miu clock
    
    //MIU0 Software reset
    _RV32_2(0x121e, 0x0000),   //hold miu_0
    _RV32_2(0x121e, 0x0c01),   //miu_sw_rst
    _RV32_2(0x121e, 0x0c00),
    _RV32_2(0x1202, 0x0000),   //disable all ddr bus out
    
    //MIU0 Request mask
    _RV32_2(0x1246, 0x7FFE),   //mask group0
    _RV32_2(0x1266, 0xFFFF),   //mask group1
    _RV32_2(0x1286, 0xFFFF),   //mask group2
    _RV32_2(0x12A6, 0xFFFF),   //mask group3
    
#if (ENABLE_MIU1)
    //MIU1 Software reset
    _RV32_2(0x061e, 0x0000),   //hold miu_1
    _RV32_2(0x061e, 0x0c01),   //miu_sw_rst
    _RV32_2(0x061e, 0x0c00),
    _RV32_2(0x0602, 0x0000),   //disable all ddr bus out
    
    //MIU1 Request mask
    _RV32_2(0x0646, 0x7FFE),   //mask group0
    _RV32_2(0x0666, 0xFFFF),   //mask group1
    _RV32_2(0x0686, 0xFFFF),   //mask group2
    _RV32_2(0x06A6, 0xFFFF),   //mask group3
#endif
    
#if CONFIG_MSTAR_STR_ENABLE    
    {CHIPCFG_CMD_AVD_RESET, 0x00},
#endif

{CHIPCFG_CMD_BASE, 0x1f22},
#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    _RV32_2(0x0d36, 0x4000),   //reg_DDRPLL_LOOP_DIV_FIRST  : [15:14]   divide by 4
    _RV32_2(0x0d34, 0x0300),   //reg_DDRPLL_LOOP_DIV_SECOND : [15:14]   divide by 3
    _RV32_2(0x0d30, 0x3705),   //ddfset
    _RV32_2(0x0d32, 0x001a),   //ddrpll_reset/ddrpll_porst/ddrpl_pd
    _RV32_2(0x0d2a, 0x0526),   //SSC enable (1%)
    _RV32_2(0x0d28, 0xc00d),   //SSC enable (1%)
#else
#if (ENABLE_MEM1866MHz)
    _RV32_2(0x0d36, 0x4000),   //reg_DDRPLL_LOOP_DIV_FIRST  : [15:14]   divide by 4
    _RV32_2(0x0d34, 0x0300),   //reg_DDRPLL_LOOP_DIV_SECOND : [15:14]   divide by 3
    _RV32_2(0x0d30, 0xc96d),   //ddfset
    _RV32_2(0x0d32, 0x0016),   //ddrpll_reset/ddrpll_porst/ddrpl_pd
    _RV32_2(0x0d2a, 0x05e4),   //SSC enable (1%)
    _RV32_2(0x0d28, 0xc00a),   //SSC enable (1%)
#elif (ENABLE_MEM1600MHz)
    _RV32_2(0x0d36, 0x8000),   //reg_DDRPLL_LOOP_DIV_FIRST  : [15:14]   divide by 4
    _RV32_2(0x0d34, 0x0300),   //reg_DDRPLL_LOOP_DIV_SECOND : [15:14]   divide by 3
    _RV32_2(0x0d30, 0x2b52),   //ddfset
    _RV32_2(0x0d32, 0x0035),   //ddrpll_reset/ddrpll_porst/ddrpl_pd
    _RV32_2(0x0d2a, 0x0528),   //SSC enable (1%)
    _RV32_2(0x0d28, 0xc00d),   //SSC enable (1%)
#else
    #error "Unsupport DDR freq!!!"
#endif
#endif

//Avoid DDRPLL unknown(MCLK05X & MCLK025X) by resset ddrat[28]
//_RV32_2(0x0d24, 0x1000),

#if (ENABLE_MIU1)
#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    _RV32_2(0x0db6, 0x4000),   //reg_DDRPLL_LOOP_DIV_FIRST  : [15:14]   divide by 4
    _RV32_2(0x0db4, 0x0300),   //reg_DDRPLL_LOOP_DIV_SECOND : [15:14]   divide by 3
    _RV32_2(0x0db0, 0x3705),   //ddfset
    _RV32_2(0x0db2, 0x001a),   //ddrpll_reset/ddrpll_porst/ddrpl_pd
    _RV32_2(0x0daa, 0x0526),   //SSC enable (1%)
    _RV32_2(0x0da8, 0xc00d),   //SSC enable (1%)
#else
#if (ENABLE_MEM1866MHz)
    _RV32_2(0x0db6, 0x4000),   //reg_DDRPLL_LOOP_DIV_FIRST  : [15:14]   divide by 4
    _RV32_2(0x0db4, 0x0300),   //reg_DDRPLL_LOOP_DIV_SECOND : [15:14]   divide by 3
    _RV32_2(0x0db0, 0xc96d),   //ddfset
    _RV32_2(0x0db2, 0x0016),   //ddrpll_reset/ddrpll_porst/ddrpl_pd
    _RV32_2(0x0daa, 0x05e4),   //SSC enable (1%)
    _RV32_2(0x0da8, 0xc00a),   //SSC enable (1%)
#elif (ENABLE_MEM1600MHz)
    _RV32_2(0x0db6, 0x8000),   //reg_DDRPLL_LOOP_DIV_FIRST  : [15:14]   divide by 4
    _RV32_2(0x0db4, 0x0300),   //reg_DDRPLL_LOOP_DIV_SECOND : [15:14]   divide by 3
    _RV32_2(0x0db0, 0x2b52),   //ddfset
    _RV32_2(0x0db2, 0x0035),   //ddrpll_reset/ddrpll_porst/ddrpl_pd
    _RV32_2(0x0daa, 0x0528),   //SSC enable (1%)
    _RV32_2(0x0da8, 0xc00d),   //SSC enable (1%)
#else
    #error "Unsupport DDR freq!!!"
#endif
#endif

//Avoid DDRPLL unknown(MCLK05X & MCLK025X) by resset ddrat[28]
//_RV32_2(0x0da4, 0x1000),
#endif

    {CHIPCFG_CMD_WAIT, 1},       // delay 1ms

#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    /* Empty */
#else
    _RV32_2(0x0d24, 0x0000),
#if (ENABLE_MIU1)
    _RV32_2(0x0da4, 0x0000),
#endif
#endif

#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    // SEL MAPPING FOR 2LB
    _RV32_2(0x0d7e, 0x0004),  //20130115 select 2LB
    _RV32_2(0x0dfe, 0x000c),  //20130115 select 2LB
#endif

    {CHIPCFG_CMD_BASE, 0x1f20},

#if(ENABLE_MEM1866MHz) //ENABLE_MEM1866MHz
    _RV32_2(0x1202, 0xf3a7),  //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+8banks+10col+4x
    _RV32_2(0x1204, 0x004b),  //if I64Mode =0x8b else =0x0b
#if CONFIG_MSTAR_STR_ENABLE    
    _RV32_2(0x1206, 0x1d38),  //refresh cycle=0x50 ,TURN_ON_SCRAMBLE=0x8450     //0x0538
#else
    _RV32_2( 0x1206, 0x0d38  ),  //refresh cycle=0x50 ,TURN_ON_SCRAMBLE=0x8450     //0x0538
#endif
    _RV32_2(0x1208, 0x20dd),  //reg_tRCD
    _RV32_2(0x120a, 0x2e76),  //reg_tRRD
    _RV32_2(0x120c, 0xc7e9),  //reg_tWL
#if(MIU_CONFIG == DDR3_MIU0_32_MIU1_32_4GB)
    _RV32_2(0x120e, 0x4120),  //tRFC
#else
    _RV32_2(0x120e, 0x4096),  //tRFC
#endif
    _RV32_2(0x1210, 0x1f14),  //MR0
    _RV32_2(0x1212, 0x4004),  //MR1
    _RV32_2(0x1214, 0x8020),  //MR2
    _RV32_2(0x1216, 0xC000),  //MR3
#elif (ENABLE_MEM1600MHz) //ENABLE_MEM1600MHz
#if(MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    _RV32_2(0x1202, 0xf3a3),  //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+8banks+10col+4x
#else
    _RV32_2(0x1202, 0xf3a7),  //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+8banks+10col+4x
#endif
#if(MS_BOARD_TYPE_SEL == BD_MP3050_SR2)
    _RV32_2(0x1204, 0x004b),  //if I64Mode =0x8b else =0x0b
#else
#if (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x1204, 0x004b),  //if I64Mode =0x8b else =0x0b
#else
    _RV32_2(0x1204, 0x004b),  //if I64Mode =0x8b else =0x0b
#endif
#endif
#if (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2(0x1206, 0x0430),  //MIU0_32_MIU1_32_1600_T_MODE_1003
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x1206, 0x0430),  //MIU0_32_MIU1_32_1600_T_MODE_1003
#else
    _RV32_2(0x1206, 0x0c30),  //refresh cycle=0x50 ,TURN_ON_SCRAMBLE=0x8450
#endif
    _RV32_2(0x1208, 0x1cbb),  //reg_tRCD
    _RV32_2(0x120a, 0x2766),  //reg_tRRD
    _RV32_2(0x120c, 0x96c8),  //reg_tWL
#if(MIU_CONFIG == DDR3_MIU0_32_MIU1_32_4GB)
    _RV32_2(0x120e, 0x4114),  //20130218 tRFC for 4G
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2(0x120e, 0x4114),  //20130218 tRFC for 4G
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x120e, 0x4114),  //20130218 tRFC for 4G
#else
    _RV32_2(0x120e, 0x4082),  //tRFC
#endif

    _RV32_2(0x1210, 0x1c70),  //MR0
    _RV32_2(0x1212, 0x4040),  //MR1
    _RV32_2(0x1214, 0x8018),  //MR2
    _RV32_2(0x1216, 0xC000),  //MR3
#else
    #error "Unsupport DDR freq!!!"
#endif //ENABLE_MEM1866MHz

    _RV32_2(0x1226, 0x0000),

#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    _RV32_2(0x1228, 0x4000),
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2(0x1228, 0x4000), // reg_pack_en
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x1228, 0x4000), // reg_pack_en
#else
    _RV32_2(0x1228, 0x40B0), // enable dynamic ODT (20130515)
#endif

    _RV32_2(0x122c, 0x0000),
#if CONFIG_MSTAR_STR_ENABLE    
    _RV32_2(0x12d8, 0x0200),
#endif
    _RV32_2(0x12d4, 0x0000),   // 20130108 turn off read_last_done_z_bypass
    _RV32_2(0x12b2, 0xffff),   // 20130108 change last_done_z position
    _RV32_2(0x1292, 0xffff),   // 20130108 change last_done_z position
    _RV32_2(0x1272, 0xffff),   // 20130108 change last_done_z position
    _RV32_2(0x1252, 0xffff),   // 20130108 change last_done_z position

    {CHIPCFG_CMD_BASE, 0x1f22},

#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
#if CONFIG_MSTAR_STR_ENABLE    
    _RV32_2(0x0d00, 0x0a0a),  //20130109 PAD MUX and IC side odt enable
#else
    _RV32_2(0x0d00, 0x0a03),  //20130109 PAD MUX and IC side odt enable
#endif
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
#if CONFIG_MSTAR_STR_ENABLE   
    _RV32_2(0x0d00, 0x0a0c),  //IC side odt enable (20130131), reg_pkg_sel
#else
    _RV32_2(0x0d00, 0x0a04),  //IC side odt enable (20130131), reg_pkg_sel
#endif
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
#if CONFIG_MSTAR_STR_ENABLE   
    _RV32_2(0x0d00, 0x0a0c),  //IC side odt enable (20130131), reg_pkg_sel
#else
    _RV32_2(0x0d00, 0x0a04),  //IC side odt enable (20130131), reg_pkg_sel
#endif
#else
#if CONFIG_MSTAR_STR_ENABLE   
    _RV32_2(0x0d00, 0x0a08),  //reg_pkg_sel      1    [0] //IC side odt enable (20130104)
#else
    _RV32_2( 0x0d00, 0x0a00  ),  //reg_pkg_sel      1    [0] //IC side odt enable (20130104)
#endif
#endif

    _RV32_2(0x0d04, 0x0080),  //reg_dqs_pre_state
    _RV32_2(0x0d10, 0x0200),

#if (ENABLE_MEM1866MHz)
    _RV32_2(0x0d38, 0x0044),
    _RV32_2(0x0d48, 0x0044),
#if CONFIG_MSTAR_STR_ENABLE   
    _RV32_2(0x0d3a, 0x7070),
    _RV32_2(0x0d4a, 0x0000),
#else
    _RV32_2( 0x0d3a, 0x7072  ),
    _RV32_2( 0x0d4a, 0x0002  ),
#endif
    _RV32_2(0x0d02, 0x0000),
#elif (ENABLE_MEM1600MHz)
#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    _RV32_2(0x0d38, 0x0055),
    _RV32_2(0x0d48, 0x0055),
    _RV32_2(0x0d3a, 0x5052),
    _RV32_2(0x0d4a, 0x5052),
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2(0x0d38, 0x0044),
    _RV32_2(0x0d48, 0x0044),
    _RV32_2(0x0d3a, 0x9091),  /* refine 20140411 */
    _RV32_2(0x0d4a, 0x9092),
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0d38, 0x2266),
    _RV32_2(0x0d48, 0x2266),
    _RV32_2(0x0d3a, 0x9090),
    _RV32_2(0x0d4a, 0x9090),
#else
    _RV32_2(0x0d38, 0x0044),
    _RV32_2(0x0d48, 0x0044),
#if(MS_BOARD_TYPE_SEL == BD_MP3050_SR2)
    _RV32_2(0x0d3a, 0x7077),
#else
    _RV32_2(0x0d3a, 0x5052),
#endif
    _RV32_2(0x0d4a, 0x8082),
#endif
    _RV32_2(0x0d02, 0x0000),
#else
    #error "Unsupport DDR freq!!!"
#endif

#if (ENABLE_MEM1866MHz)
    _RV32_2(0x0d3c, 0x1333),
    _RV32_2(0x0d4c, 0x0044),
#elif (ENABLE_MEM1600MHz)
#if (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0d3c, 0x1244),
    _RV32_2(0x0d4c, 0x1144),
#else
    _RV32_2(0x0d3c, 0x1133),
    _RV32_2(0x0d4c, 0x0033),
#endif
#else
    #error "Unsupport DDR freq!!!"
#endif

#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    _RV32_2(0x0d5a, 0x5555),
    _RV32_2(0x0d5c, 0x5555),
    _RV32_2(0x0d5e, 0x5555),
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2(0x0d5a, 0x7777),
    _RV32_2(0x0d5c, 0x7777),
    _RV32_2(0x0d5e, 0x7777),
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0d5a, 0x5555),
    _RV32_2(0x0d5c, 0x5555),
    _RV32_2(0x0d5e, 0x5555),
#else
    _RV32_2(0x0d5a, 0x3333),
#if (ENABLE_MEM1866MHz)
    _RV32_2(0x0d5c, 0x4444),
#elif (ENABLE_MEM1600MHz)
    _RV32_2(0x0d5c, 0x3333),
#else
    #error "Unsupport DDR freq!!!"
#endif
    _RV32_2(0x0d5e, 0x3333),
#endif
#if (ENABLE_MEM1866MHz)
    _RV32_2(0x0d3e, 0x1033),
#elif (ENABLE_MEM1600MHz)
#if (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0d3e, 0x2022),
#else
    _RV32_2(0x0d3e, 0x1011),
#endif
#else
    #error "Unsupport DDR freq!!!"
#endif

#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    _RV32_2(0x0d50, 0x1110),
    _RV32_2(0x0d74, 0x4444),
    _RV32_2(0x0d52, 0x0044),
    _RV32_2(0x0d6c, 0xbbbb),
    _RV32_2(0x0d6e, 0xbbbb),
    _RV32_2(0x0d6a, 0x0020),
    _RV32_2(0x0d68, 0x0020),
#else
#if (ENABLE_MEM1866MHz)
    _RV32_2(0x0d50, 0x1100),
    _RV32_2(0x0d52, 0x0075),
    _RV32_2(0x0d0e, 0x00e9),
    _RV32_2(0x0d0e, 0x00a9),
    _RV32_2(0x0d6c, 0x6688),
    _RV32_2(0x0d6e, 0x6677),
#elif (ENABLE_MEM1600MHz)

#if (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2(0x0d50, 0x0000),
    _RV32_2(0x0d0e, 0x00e7),
    _RV32_2(0x0d0e, 0x00a7),
    _RV32_2(0x0d52, 0x7777),
    _RV32_2(0x0d5a, 0x7777),
    _RV32_2(0x0d5c, 0x7777),
    _RV32_2(0x0d5e, 0x7777),
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0d50, 0x1111),
    _RV32_2(0x0d0e, 0x00e7),
    _RV32_2(0x0d0e, 0x00a7),
    _RV32_2(0x0d52, 0x0033),
#else
    _RV32_2(0x0d50, 0x1111),
#endif

    _RV32_2(0x0d0e, 0x00e7),
    _RV32_2(0x0d0e, 0x00a7),

#if (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0d6c, 0x7788),
    _RV32_2(0x0d6e, 0x7788),
#else
    _RV32_2(0x0d6c, 0x6677),
    _RV32_2(0x0d6e, 0x6666),
#endif

#else
    #error "Unsupport DDR freq!!!"
#endif
    _RV32_2(0x0d6a, 0x0020),
    _RV32_2(0x0d68, 0x0020),
#if (ENABLE_MEM1866MHz)
    _RV32_2(0x0d74, 0x2222), ////reg_trg_lvl
#elif (ENABLE_MEM1600MHz)

#if (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
                            ////reg_trg_lvl
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
                            ////reg_trg_lvl
#else
    _RV32_2( 0x0d74, 0x4444  ), ////reg_trg_lvl
#endif

#else
    #error "Unsupport DDR freq!!!"
#endif
#endif

#if (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0d72, 0x3803),
    _RV32_2(0x0d74, 0x4444),
#else
#if CONFIG_MSTAR_STR_ENABLE   
    _RV32_2(0x0d72, 0x8803), //eason
#else
    _RV32_2( 0x0d72, 0x0003  ),
#endif
#endif

    //DLL calibration
    _RV32_2(0x0d60, 0x000c),
    _RV32_2(0x0d60, 0x0008),
    _RV32_2(0x0d62, 0x007f),
    _RV32_2(0x0d64, 0xf200),
    _RV32_2(0x0d60, 0x2370),

    {CHIPCFG_CMD_WAIT, 5},       // delay 5ms

    {CHIPCFG_CMD_BASE, 0x1f20},

    _RV32_2(0x121e, 0x0c01), //miu_sw_rst
#if CONFIG_MSTAR_STR_ENABLE 
    _RV32_2(0x121e, 0x0c00), 
#endif
    {CHIPCFG_CMD_WAIT, 1},       // delay 1ms

#if CONFIG_MSTAR_STR_ENABLE
#else
    _RV32_2( 0x121e, 0x0c08  ),
#endif
    {CHIPCFG_CMD_BASE, 0x1f22},
#if CONFIG_MSTAR_STR_ENABLE
#else
#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    _RV32_2( 0x0d00, 0x0a03  ), //PAD MUX and IC side odt enable
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2( 0x0d00, 0x0a04  ), //IC side odt enable (20130131), reg_pkg_sel [0]
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2( 0x0d00, 0x0a04  ), //IC side odt enable (20130131), reg_pkg_sel [0]
#else
//_RV32_2( 0x0d00, 0x0a00  ), //reg_pkg_sel      1   : [0], //IC side odt enable (20130104)
#endif
#endif
    _RV32_2(0x0d02, 0xaaaa), // @reg_cko_state        cc  : [7:0]
    _RV32_2(0x0d18, 0x0000), 
#if CONFIG_MSTAR_STR_ENABLE
#else
    {CHIPCFG_CMD_BASE, 0x1f20},
#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    _RV32_2(0x1202, 0x03a3),
#else
    _RV32_2( 0x1202, 0x03a7  ),
#endif
#endif

    {CHIPCFG_CMD_BASE, 0x1f22},
#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x0d54, 0xc070),  //20130329  reg_cmd_mode_sel
#else
    _RV32_2(0x0d54, 0xc000),  //20130329  reg_cmd_mode_sel
#endif
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x0d54, 0xc070),
#else
    _RV32_2(0x0d54, 0xc000),
#endif
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x0d54, 0xc070),
#else
    _RV32_2(0x0d54, 0xc000),
#endif
#else
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x0d54, 0x8070),  //20130329  reg_cmd_mode_sel
#else
    _RV32_2(0x0d54, 0x8000),  //20130329  reg_cmd_mode_sel
#endif
#endif

    {CHIPCFG_CMD_BASE, 0x1f20},
#if ( ENABLE_MIU1 == 1 )

#if(MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    _RV32_2(0x0602, 0xf3a3),  //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+8banks+10col+4x
#else
    _RV32_2(0x0602, 0xf3a7),  //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+8banks+10col+4x
#endif

#if (ENABLE_MEM1866MHz)
    _RV32_2(0x0604, 0x004b),  //if I64Mode =0x8b else =0x0b
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x0606, 0x1d38),  //refresh cycle=0x50 ,TURN_ON_SCRAMBLE=0x8450
#else
    _RV32_2(0x0606, 0x0d38),  //refresh cycle=0x50 ,TURN_ON_SCRAMBLE=0x8450
#endif
    _RV32_2(0x0608, 0x20dd),  //reg_tRCD
    _RV32_2(0x060a, 0x2e76),  //reg_tRRD
    _RV32_2(0x060c, 0xc7e9),  //reg_tWL
#if(MIU_CONFIG == DDR3_MIU0_32_MIU1_32_4GB)
    _RV32_2(0x060e, 0x4120),  //tRFC
#else
    _RV32_2(0x060e, 0x4096),  //tRFC
#endif
    _RV32_2(0x0610, 0x1f14),  //MR0
    _RV32_2(0x0612, 0x4004),  //MR1
    _RV32_2(0x0614, 0x8020),  //MR2
    _RV32_2(0x0616, 0xC000),  //MR3
#elif (ENABLE_MEM1600MHz)
#if(MS_BOARD_TYPE_SEL == BD_MP3050_SR2)
    _RV32_2(0x0604, 0x004b),  //if I64Mode =0x8b else =0x0b
#else
#if(MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0604, 0x004b),  //if I64Mode =0x8b else =0x0b
#else
    _RV32_2(0x0604, 0x004b),  //if I64Mode =0x8b else =0x0b
#endif
#endif
#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    _RV32_2(0x0606, 0x0430),  //refresh cycle=0x50 ,TURN_ON_SCRAMBLE=0x8450
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2(0x0606, 0x0430),  //tREFRESH [7:0] * 16
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0606, 0x0430),  //tREFRESH [7:0] * 16
#else
    _RV32_2(0x0606, 0x0c30),  //refresh cycle=0x50 ,TURN_ON_SCRAMBLE=0x8450
#endif
    _RV32_2(0x0608, 0x1cbb),  //reg_tRCD
    _RV32_2(0x060a, 0x2766),  //reg_tRRD
    _RV32_2(0x060c, 0x96c8),  //reg_tWL
#if(MIU_CONFIG == DDR3_MIU0_32_MIU1_32_4GB)
    _RV32_2(0x060e, 0x4114),  //20130218 tRFC for 4G
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2(0x060e, 0x4114),  //20130218 tRFC for 4G
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x060e, 0x4114),  //20130218 tRFC for 4G
#else
    _RV32_2(0x060e, 0x4082),  //tRFC
#endif
    _RV32_2(0x0610, 0x1c70),  //MR0
    _RV32_2(0x0612, 0x4040),  //MR1
    _RV32_2(0x0614, 0x8018),  //MR2
    _RV32_2(0x0616, 0xC000),  //MR3
#else
    #error "Unsupport DDR freq!!!"
#endif

#if(MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    _RV32_2(0x0628, 0x4000),
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2(0x0628, 0x4000), //reg_pack_en
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0628, 0x4000), //reg_pack_en
#else
    _RV32_2(0x0628, 0x40b0), // enable dynamic ODT (20130515)
#endif

    _RV32_2(0x062c, 0x0000),
    //_RV32_2( 0x065a, 0x4040  ), //	  reduce 2t latency en	  //
    //_RV32_2( 0x065a, 0x0040  ), //reduce 2t latency en
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x06d8, 0x0200),
#endif
    _RV32_2(0x06d4, 0x0000), // 20130108 turn off read_last_done_z_bypass
    _RV32_2(0x06b2, 0xffff), // 20130108 change last_done_z position
    _RV32_2(0x0692, 0xffff),   // 20130108 change last_done_z position
    _RV32_2(0x0672, 0xffff),   // 20130108 change last_done_z position
    _RV32_2(0x0652, 0xffff),  // 20130108 change last_done_z position

    {CHIPCFG_CMD_BASE, 0x1f22},
#if(MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x0d80, 0x0a0a),
#else
    _RV32_2(0x0d80, 0x0a03),
#endif
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x0d80, 0x0a0c),
#else
    _RV32_2(0x0d80, 0x0a04),
#endif
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x0d80, 0x0a0c),
#else
    _RV32_2(0x0d80, 0x0a04),
#endif
#else
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x0d80, 0x0a08),
#else
    _RV32_2(0x0d80, 0x0a00),
#endif
#endif

    _RV32_2(0x0d84, 0x0080),
    _RV32_2(0x0d90, 0x0200),

#if(ENABLE_MEM1866MHz)
    _RV32_2(0x0db8, 0x0044),
    _RV32_2(0x0dc8, 0x0044),
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x0dba, 0x5050),
    _RV32_2(0x0dca, 0x0000),
#else
    _RV32_2(0x0dba, 0x5052),
    _RV32_2(0x0dca, 0x0002),
#endif
    _RV32_2(0x0d82, 0x0000),
    _RV32_2(0x0dbc, 0x1333),
    _RV32_2(0x0dcc, 0x0044),
    _RV32_2(0x0dbe, 0x1033),
    _RV32_2(0x0dd0, 0x1100),
    _RV32_2(0x0dd2, 0x0076),
#elif(ENABLE_MEM1600MHz)
#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    _RV32_2(0x0db8, 0x0055),
    _RV32_2(0x0dc8, 0x0055),
    _RV32_2(0x0dba, 0x5052),
    _RV32_2(0x0dca, 0x5052),
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2(0x0db8, 0x0044),
    _RV32_2(0x0dc8, 0x0044),
    _RV32_2(0x0dba, 0x9091),  /* refine 20140411 */
    _RV32_2(0x0dca, 0x9092),
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0db8, 0x2266),
    _RV32_2(0x0dc8, 0x2266),
    _RV32_2(0x0dba, 0x8010),
    _RV32_2(0x0dca, 0x8010),
#else
    _RV32_2(0x0db8, 0x0044),
    _RV32_2(0x0dc8, 0x0044),
#if(MS_BOARD_TYPE_SEL == BD_MP3050_SR2)
    //#error _RV32_2( 0x0dba, 0x7077  )
    _RV32_2(0x0dba, 0x7077),
#else
    //#error _RV32_2( 0x0dba, 0x7072  )
    _RV32_2(0x0dba, 0x5052),
#endif
    _RV32_2(0x0dca, 0x8082),
#endif
    _RV32_2(0x0d82, 0x0000),
#if (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0dbc, 0x1245),
    _RV32_2(0x0dcc, 0x1145),
    _RV32_2(0x0dbe, 0x2022),
    _RV32_2(0x0dd0, 0x1111),
    _RV32_2(0x0dd2, 0x3333),
#else
    _RV32_2(0x0dbc, 0x1133),
    _RV32_2(0x0dcc, 0x0033),
    _RV32_2(0x0dbe, 0x1011),
#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    _RV32_2(0x0dd0, 0x1110),
    _RV32_2(0x0df4, 0x4444),
    _RV32_2(0x0dd2, 0x0044),
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2(0x0dd0, 0x0000),
    _RV32_2(0x0dd2, 0x7777),
#else
    _RV32_2(0x0dd0, 0x1111),
#endif

#endif

#else
    #error "Unsupport DDR freq!!!"
#endif

#if(MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    _RV32_2(0x0dda, 0x5555),
    _RV32_2(0x0ddc, 0x5555),
    _RV32_2(0x0dde, 0x5555),
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2(0x0dda, 0x7777),
    _RV32_2(0x0ddc, 0x7777),
    _RV32_2(0x0dde, 0x7777),
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0dda, 0x5555),
    _RV32_2(0x0ddc, 0x5555),
    _RV32_2(0x0dde, 0x5555),
#else
    _RV32_2(0x0dda, 0x3333),
    _RV32_2(0x0ddc, 0x3333),
    _RV32_2(0x0dde, 0x3333),
#endif

#if(ENABLE_MEM1866MHz)
    _RV32_2(0x0d8e, 0x00e9),
    _RV32_2(0x0d8e, 0x00a9),
#elif(ENABLE_MEM1600MHz)
#if(MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2(0x0d8e, 0x00e7), //reg_en_mask
    _RV32_2(0x0d8e, 0x00a7), //release rst sel
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0d8e, 0x00e7), //reg_en_mask
    _RV32_2(0x0d8e, 0x00a7), //release rst sel
    #endif
    _RV32_2(0x0d8e, 0x00e7), //reg_en_mask
    _RV32_2(0x0d8e, 0x00a7), //release rst sel
#endif

#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    _RV32_2(0x0dec, 0xbbbb), //reg_dll1_ph
    _RV32_2(0x0dee, 0xbbbb), //reg_dll1_ph
#else
#if(ENABLE_MEM1866MHz)
    _RV32_2(0x0dec, 0x6677),
    _RV32_2(0x0dee, 0x6677),
#elif(ENABLE_MEM1600MHz)
#if(MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2(0x0dec, 0x7777),
    _RV32_2(0x0dee, 0x7777),
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0dec, 0x7788),
    _RV32_2(0x0dee, 0x7788),
#else
    _RV32_2(0x0dec, 0x6677),
    _RV32_2(0x0dee, 0x7777),
#endif
#else
    #error "Unsupport DDR freq!!!"
#endif
#endif

    {CHIPCFG_CMD_BASE, 0x1f22},
    _RV32_2(0x0dea, 0x0020),
    _RV32_2(0x0de8, 0x0020),
#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    /* Empty */
#else
#if(ENABLE_MEM1866MHz)
    _RV32_2(0x0df4, 0x2222),
#elif(ENABLE_MEM1600MHz)
    _RV32_2(0x0df4, 0x4444),
#else
    #error "Unsupport DDR freq!!!"
#endif
#endif

#if (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0df2, 0x3803),
    _RV32_2(0x0df4, 0x4444),
#else
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x0df2, 0x8803), //eason
#else
    _RV32_2( 0x0df2, 0x0003  ),
#endif
#endif

    //DLL calibration
    _RV32_2(0x0de0, 0x000c),
    _RV32_2(0x0de0, 0x0008),
    _RV32_2(0x0de2, 0x007f),
    _RV32_2(0x0de4, 0xf200),
    _RV32_2(0x0de0, 0x2370),
    
    {CHIPCFG_CMD_WAIT, 5},       // delay 5ms
    
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x061e, 0x0c01), //0x0c01
    
    {CHIPCFG_CMD_WAIT, 1},       // delay 1ms
    
    _RV32_2(0x061e, 0x0c08), //0x0c01
    
    {CHIPCFG_CMD_BASE, 0x1f22},
#if CONFIG_MSTAR_STR_ENABLE
#else
#if(MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
#if 20130430
    _RV32_2(0x0d80, 0x0a03), //20130109 PAD MUX and IC side odt enable
#else
    _RV32_2(0x0d80, 0x1e03), //20130109 PAD MUX and IC side odt enable
#endif

#elif(MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2(0x0d80, 0x0a04), //IC side odt enable (20130131)
#elif(MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0d80, 0x0a04), //IC side odt enable (20130131)
#else
    //_RV32_2( 0x0d80, 0x0a00  ), //reg_pkg_sel      1   : [0]
#endif
#endif
    _RV32_2(0x0d82, 0xaaaa), //reg_cko_state        cc  : [7:0]
    _RV32_2(0x0d98, 0x0000), //
#if CONFIG_MSTAR_STR_ENABLE
#else
    {CHIPCFG_CMD_BASE, 0x1f20},

#if(MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
    _RV32_2(0x0602, 0x03a3),  //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+8banks+10col+4x
#else
    _RV32_2(0x0602, 0x03a7),  //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+8banks+10col+4x
#endif
#endif
    {CHIPCFG_CMD_BASE, 0x1f22},

#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x0dd4, 0xc070), //20130329  reg_cmd_mode_sel
#else
    _RV32_2( 0x0dd4, 0xc000  ), //20130329  reg_cmd_mode_sel
#endif
#elif(MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x0dd4, 0xc070), //reg_cmd_mode_sel
#else
    _RV32_2( 0x0dd4, 0xc000  ), //reg_cmd_mode_sel
#endif
#elif(MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x0dd4, 0xc070), //reg_cmd_mode_sel
#else
    _RV32_2( 0x0dd4, 0xc000  ), //reg_cmd_mode_sel
#endif
#else
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x0dd4, 0x8070), //20130329  reg_cmd_mode_sel
#else
    _RV32_2( 0x0dd4, 0x8000  ), //20130329  reg_cmd_mode_sel
#endif
#endif

#endif // end enable MIU!

#if CONFIG_MSTAR_STR_ENABLE
#else
    {CHIPCFG_CMD_WAIT, 2},       // delay 2ms
    {CHIPCFG_CMD_BASE, 0x1f22},
   
    _RV32_2(0x0d08, 0x003f), //reg_rx_en
#if (ENABLE_MIU1)
    _RV32_2(0x0d88, 0x003f), //reg_rx_en
#endif

    //Add Kaiser/KaiserS SW Patch here
    {CHIPCFG_CMD_BASE, 0x1f22},
#if (ENABLE_MEM1866MHz)
    _RV32_2(0x0d0e, 0x00e9  ),
{CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x12f0, 0x0000), // SW Patch for BIST, 20130108 select MIU0
    _RV32_2(0x12e4, 0x0001), // 20130108 test one data
    _RV32_2(0x12e0, 0x0200), // 20130108 write only
    _RV32_2(0x12e0, 0x0201), // 20130108 write onlyd
    _RV32_2(0x12e0, 0x0201), // 20130108 dummy write only
    _RV32_2(0x12e0, 0x0000), // 20130108 return to original state
    {CHIPCFG_CMD_BASE, 0x1f22},
    _RV32_2(0x0d0e, 0x00a9),
#elif (ENABLE_MEM1600MHz)
    _RV32_2(0x0d0e, 0x00e7),
#if (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2(0x0d0e, 0x00e7), //release rst sel  2013.08.26
    _RV32_2(0x0d0e, 0x00a7), //release rst sel  2013.08.26
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0d0e, 0x00e7), //release rst sel  2013.08.26
    _RV32_2(0x0d0e, 0x00a7), //release rst sel  2013.08.26
#endif

    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x12f0, 0x0000), // SW Patch for BIST, 20130108 select MIU0
    _RV32_2(0x12e4, 0x0001), // 20130108 test one data
    _RV32_2(0x12e0, 0x0200), // 20130108 write only
    _RV32_2(0x12e0, 0x0201), // 20130108 write onlyd
    _RV32_2(0x12e0, 0x0201), // 20130108 dummy write only
    _RV32_2(0x12e0, 0x0000), // 20130108 return to original state
    {CHIPCFG_CMD_BASE, 0x1f22},
    _RV32_2(0x0d0e, 0x00a7),
#endif

    {CHIPCFG_CMD_BASE, 0x1f22},
#if(ENABLE_MEM1866MHz)
    _RV32_2(0x0d8e, 0x00e9),
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x12f0, 0x8000), // SW Patch for BIST, 20130108 select MIU1
    _RV32_2(0x12e4, 0x0001), // 20130108 test one data
    _RV32_2(0x12e0, 0x0200), // 20130108 write only
    _RV32_2(0x12e0, 0x0201), // 20130108 write onlyd
    _RV32_2(0x12e0, 0x0201), // 20130108 dummy write only
    _RV32_2(0x12f0, 0x0000), // 20130108 select MIU0
    {CHIPCFG_CMD_BASE, 0x1f22},
    _RV32_2(0x0d8e, 0x00a9),
#elif(ENABLE_MEM1600MHz)
    _RV32_2(0x0d8e, 0x00e7),
#if (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE)
    _RV32_2(0x0d8e, 0x00e7), //release rst sel  2013.08.26
    _RV32_2(0x0d8e, 0x00a7), //release rst sel  2013.08.26
#elif (MIU_CONFIG == DDR3_MIU0_32_MIU1_32_TMODE_2LAYER)
    _RV32_2(0x0d8e, 0x00e7), //release rst sel  2013.08.26
    _RV32_2(0x0d8e, 0x00a7), //release rst sel  2013.08.26
#endif
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x12f0, 0x8000), // SW Patch for BIST, 20130108 select MIU1
    _RV32_2(0x12e4, 0x0001), // 20130108 test one data
    _RV32_2(0x12e0, 0x0200), // 20130108 write only
    _RV32_2(0x12e0, 0x0201), // 20130108 write only
    _RV32_2(0x12e0, 0x0201), // 20130108 dummy write only
    _RV32_2(0x12e0, 0x0000), // 20130108 return to original state
    _RV32_2(0x12f0, 0x0000), // 20130108 select MIU0
    {CHIPCFG_CMD_BASE, 0x1f22},
    _RV32_2(0x0d8e, 0x00a7),
#endif
#endif

