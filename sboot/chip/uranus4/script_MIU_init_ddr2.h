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
#define _RV32_2(a,b)    {a<<1,b}

//U4_MIU0_Reset:
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x121E, 0x0000), //hold miu_0
_RV32_2(0x121E, 0x0C01),
_RV32_2(0x121E, 0x0E00),
_RV32_2(0x1202, 0x0000),
//MaskMiu0AllGroup:
_RV32_2(0x12F8, 0x8000),
_RV32_2(0x1246, 0xFFFE), //Mask MIU0_group_0
_RV32_2(0x1266, 0xFFFF), //Mask MIU0_group_1
_RV32_2(0x1286, 0xFFFF), //Mask MIU0_group_2

//U4_MIU1_Reset:
_RV32_2(0x061E, 0x0000), //hold miu_1
_RV32_2(0x061E, 0x0C01),
_RV32_2(0x061E, 0x0E00),
_RV32_2(0x0602, 0x0000),
//MaskMiu1AllGroup:
_RV32_2(0x06F8, 0x8000),
_RV32_2(0x0646, 0xFFFE), //Mask MIU1_group_0
_RV32_2(0x0666, 0xFFFF), //Mask MIU1_group_1
_RV32_2(0x0686, 0xFFFF), //Mask MIU1_group_2

//MIU0 init
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d22, 0x0C00 ),  // ddrpll auto protection adviced by Miles.Lai 101202
_RV32_2(0x0da2, 0x0C00 ),  // ddrpll auto protection adviced by Miles.Lai 101202
//===MIU0_MEMPLL===================
_RV32_2(0x0d3a, 0x0500 ),  // mclk1x_skew cmd phase /dqsmask0 /mclk /dqsmask1 phase select
_RV32_2(0x0d36, 0x4003 ),  // mclk1x_clkph_ck0 / input div_1st/loop_div_1st
_RV32_2(0x0d34, 0x0300 ),  // loop_div/input_div

#if (ENABLE_MEM1066MHz)
_RV32_2(0x0d30, 0xE780 ),  // ddfset
_RV32_2(0x0d32, 0x0026 ),  // ddrpll_reset/ddrpll_porst/ddrpl_pd
#endif

#if (ENABLE_MEM800MHz)
_RV32_2(0x0d30, 0xD70A ),  // ddfset
_RV32_2(0x0d32, 0x0033 ),  // ddrpll_reset/ddrpll_porst/ddrpl_pd
#endif

#if (ENABLE_MEM667MHz)
_RV32_2(0x0d30, 0xc8a6 ),  // ddfset
_RV32_2(0x0d32, 0x0067 ),  // ddrpll_reset/ddrpll_porst/ddrpl_pd
#endif

#if (ENABLE_MEM400MHz)
_RV32_2(0x0d30, 0xAE14 ),  // ddfset
_RV32_2(0x0d32, 0x0067 ),  // ddrpll_reset/ddrpll_porst/ddrpl_pd
#endif

//===Miu_0 initial=================
{CHIPCFG_CMD_BASE, 0xbf20},
#if (MIU0_LEN == 0x4000000)
_RV32_2(0x1202, 0x0292 ),  //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+4banks+10col+4x
#elif (MIU0_LEN == 0x8000000)
_RV32_2(0x1202, 0x02a2 ),  //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+8banks+10col+4x
#else
#error mmap not correct
#endif

_RV32_2(0x1204, 0x008b ),  //if I64Mode =0x8b else =0x0b
_RV32_2(0x1206, 0x8420 ),  //refresh cycle=0x20 ,TURN_ON_SCRAMBLE=0x8420
#if (ENABLE_MEM800MHz)
_RV32_2(0x1208, 0x1666 ),  //reg_tRCD
_RV32_2(0x120a, 0x1C56 ),  //reg_tRRD
_RV32_2(0x120c, 0x5485 ),  //reg_tWL
_RV32_2(0x120e, 0x2044 ),  //tRFC
_RV32_2(0x1210, 0x0A62 ),  //MR0
#else
_RV32_2(0x1208, 0x1877 ),  //reg_tRCD
_RV32_2(0x120a, 0x1F46 ),  //reg_tRRD
_RV32_2(0x120c, 0x5486 ),  //reg_tWL
_RV32_2(0x120e, 0x2044 ),  //tRFC
_RV32_2(0x1210, 0x0A72 ),  //MR0
#endif
_RV32_2(0x1212, 0x4004 ),  //MR1
_RV32_2(0x1214, 0x8000 ),  //MR2
_RV32_2(0x1216, 0xC000 ),  //MR3

_RV32_2(0x1240, 0xC014 ),  //MIU ->cut Group 0 and 1 up, set EMAC high priority
_RV32_2(0x1242, 0x0004 ),  //MIU ->cut Group 0 and 1 up, set EMAC high priority
_RV32_2(0x1248, 0xFDFF ),  //MIU ->cut Group 0 and 1 up, set EMAC high priority
_RV32_2(0x1260, 0xC014 ),  //MIU ->cut Group 0 and 1 up, set EMAC high priority
_RV32_2(0x1262, 0x0015 ),  //MIU ->cut Group 0 and 1 up, set EMAC high priority

//======MIU0_ATOP initial======
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d00, 0x0011 ), //miu_0_atop  start
_RV32_2(0x0d38, 0x0033 ),  // DQ0/DQ1 phase / dqs0/dqs1 phase
_RV32_2(0x0d3a, 0x0040 ),  // mclk1x_skew cmd phase /dqsmask0 /mclk /dqsmask1 phase select
_RV32_2(0x0d02, 0x0000 ),
#if (ENABLE_MEM800MHz)
_RV32_2(0x0d3c, 0x2000 ),
_RV32_2(0x0d3e, 0x0022 ),
#else
_RV32_2(0x0d3c, 0x0200 ),
_RV32_2(0x0d3e, 0x0000 ),
#endif
_RV32_2(0x0d0e, 0x00e7 ),  // 1/2 reg_en_mask
_RV32_2(0x0d0e, 0x00a7 ),  // 2/2 reg_en_mask clr bit6 eg_dqsm_rst_sel
_RV32_2(0x0d6e, 0x0088 ),  //reg_dll0_ph //
_RV32_2(0x0d6a, 0x00c2 ),  //reg_dll1_code	//refine when DDR_CLK change
_RV32_2(0x0d68, 0x00c2 ),  //reg_dll0_code	//refine when DDR_CLK change
_RV32_2(0x0d72, 0x3803 ),  //
#if (MIU_INTERFACE == DDR2_INTERFACE_BGA)
_RV32_2(0x0d74, 0x0888 ),  //trigger level
#else //DDR2_INTERFACE_QFP
_RV32_2(0x0d74, 0x0666 ),  //trigger level
#endif
_RV32_2(0x0d5c, 0x5553 ),  //pad driver
_RV32_2(0x0d5e, 0x5553 ),  //pad driver
_RV32_2(0x0d60, 0x2338 ),  //reg_auto calibration

//EnableMiu0:
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x121E, 0x0C01 ),
_RV32_2(0x121E, 0x0C08 ),
{CHIPCFG_CMD_WAIT, 100}, // delay 100us
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0D00, 0x0001 ),
_RV32_2(0x0D02, 0xAAAA ),

//===MIU1_MEMPLL===================
_RV32_2(0x0dba, 0x0500 ),  // mclk1x_skew cmd phase /dqsmask0 /mclk /dqsmask1 phase select
_RV32_2(0x0db6, 0x4003 ),  // mclk1x_clkph_ck0 / input div_1st/loop_div_1st
_RV32_2(0x0db4, 0x0300 ),  // loop_div/input_div

// MIU_1 1066Mhz
#if (ENABLE_MEM1066MHz)
_RV32_2(0x0db0, 0xE781 ),  // ddfset
_RV32_2(0x0db2, 0x0026 ),  // ddrpll_reset/ddrpll_porst/ddrpl_pd
#endif

// MIU_1 800Mhz
#if (ENABLE_MEM800MHz)
_RV32_2(0x0db0, 0xD70A ),  // ddfset
_RV32_2(0x0db2, 0x0033 ),  // ddrpll_reset/ddrpll_porst/ddrpl_pd
#endif

#if (ENABLE_MEM667MHz)
_RV32_2(0x0db0, 0xc8a6 ),  // ddfset
_RV32_2(0x0db2, 0x0067 ),  // ddrpll_reset/ddrpll_porst/ddrpl_pd
#endif

#if (ENABLE_MEM400MHz)
_RV32_2(0x0db0, 0xAE14 ),  // ddfset
_RV32_2(0x0db2, 0x0067 ),  // ddrpll_reset/ddrpll_porst/ddrpl_pd
#endif

//===Miu_1 initial=================
{CHIPCFG_CMD_BASE, 0xbf20},
#if (MIU1_LEN == 0x4000000)
_RV32_2(0x0602, 0x0292 ),  //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+4banks+10col+4x
#else
_RV32_2(0x0602, 0x02a2 ),  //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+8banks+10col+4x
#endif

_RV32_2(0x0604, 0x008b ),  //if I64Mode =0x8b else =0x0b
_RV32_2(0x0606, 0x8420 ),  //refresh cycle=0x20 ,TURN_ON_SCRAMBLE=0x8420
#if (ENABLE_MEM800MHz)
_RV32_2(0x0608, 0x1666 ),  //reg_tRCD
_RV32_2(0x060a, 0x1C56 ),  //reg_tRRD
_RV32_2(0x060c, 0x5485 ),  //reg_tWL
_RV32_2(0x060e, 0x2044 ),  //tRFC
_RV32_2(0x0610, 0x0A62 ),  //MR0
#else
_RV32_2(0x0608, 0x1877 ),  //reg_tRCD
_RV32_2(0x060a, 0x1F46 ),  //reg_tRRD
_RV32_2(0x060c, 0x5486 ),  //reg_tWL
_RV32_2(0x060e, 0x2044 ),  //tRFC
_RV32_2(0x0610, 0x0A72 ),  //MR0
#endif
_RV32_2(0x0612, 0x4004 ),  //MR1
_RV32_2(0x0614, 0x8000 ),  //MR2
_RV32_2(0x0616, 0xC000 ),  //MR3

_RV32_2(0x0660, 0xC004 ),  //MIU2->allow GROUP 1 been CUT-IN,cut GROUP 1 up,GOP SC VDEC high priority
_RV32_2(0x0662, 0x0015 ),  //MIU2->allow GROUP 1 been CUT-IN,cut GROUP 1 up,GOP SC VDEC high priority
_RV32_2(0x0666, 0x0020 ),  //MIU2->allow GROUP 1 been CUT-IN,cut GROUP 1 up,GOP SC VDEC high priority
_RV32_2(0x0668, 0xFF5F ),  //MIU2->allow GROUP 1 been CUT-IN,cut GROUP 1 up,GOP SC VDEC high priority
_RV32_2(0x0688, 0xFFE7 ),  //MIU2->allow GROUP 1 been CUT-IN,cut GROUP 1 up,GOP SC VDEC high priority

//======MIU1_ATOP initial======
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d80, 0x0011 ), //miu_0_atop  start
#if (MIU_INTERFACE == DDR2_INTERFACE_BGA)
_RV32_2(0x0db8, 0x0033 ),  // DQ0/DQ1 phase / dqs0/dqs1 phase
#else //DDR2_INTERFACE_QFP
_RV32_2(0x0db8, 0x0044 ),  // DQ0/DQ1 phase / dqs0/dqs1 phase
#endif
_RV32_2(0x0dba, 0x0040 ),  // mclk1x_skew cmd phase /dqsmask0 /mclk /dqsmask1 phase select
_RV32_2(0x0d82, 0x0000 ),
#if (ENABLE_MEM800MHz)
_RV32_2(0x0dbc, 0x2000 ),
_RV32_2(0x0dbe, 0x0022 ),
#else
_RV32_2(0x0dbc, 0x0200 ),
_RV32_2(0x0dbe, 0x0000 ),
#endif
_RV32_2(0x0d8e, 0x00e7 ),  // 1/2 reg_en_mask
_RV32_2(0x0d8e, 0x00a7 ),  // 2/2 reg_en_mask clr bit6 eg_dqsm_rst_sel
_RV32_2(0x0dee, 0x0088 ),  //reg_dll0_ph //
_RV32_2(0x0dea, 0x00c2 ),  //reg_dll1_code	//refine when DDR_CLK change
_RV32_2(0x0de8, 0x00c2 ),  //reg_dll0_code	//refine when DDR_CLK change

_RV32_2(0x0dDC, 0x5553 ),  //pad driver
_RV32_2(0x0dDE, 0x5553 ),  //pad driver
_RV32_2(0x0dE0, 0x2338 ),  //reg_auto calibration
_RV32_2(0x0dF2, 0x3803 ),  //
#if (MIU_INTERFACE == DDR2_INTERFACE_BGA)
_RV32_2(0x0dF4, 0x0888 ),  //trigger level
#else //DDR2_INTERFACE_QFP
_RV32_2(0x0dF4, 0x0666 ),  //trigger level
#endif

//EnableMiu0:
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x061E, 0x0C01),
_RV32_2(0x061E, 0x0C08),
{CHIPCFG_CMD_WAIT, 100},    // delay 100us
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0D80, 0x0001),
_RV32_2(0x0D82, 0xAAAA),

_RV32_2(0x0D08, 0xffff),   //rxenable
_RV32_2(0x0D88, 0xffff),   //rxenable

//AutoInitialMiuCycle:
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x1200, 0x0000),   //hold miu_0
_RV32_2(0x0600, 0x0000),   //hold miu_1
{CHIPCFG_CMD_WAIT, 200}, //delay 200us

//set ODT
_RV32_2(0x1200, 0x0010),   //hold miu_0
_RV32_2(0x0600, 0x0010),   //hold miu_1
_RV32_2(0x1200, 0x0018),   // miu_0 #DRAM reset
_RV32_2(0x0600, 0x0018),   // miu_1 DRAM reset
_RV32_2(0x1200, 0x001C),   //DRAmreset+DRAM chip select
_RV32_2(0x0600, 0x001C),   // miu_1 DRAM reset
{CHIPCFG_CMD_WAIT, 500}, // delay 500us
_RV32_2(0x1200, 0x001E),   //DRAmreset+DRAM chip select+enable CKE
_RV32_2(0x0600, 0x001E),
{CHIPCFG_CMD_WAIT, 400}, // delay 400us
_RV32_2(0x1200, 0x001F),   //0x001F #DRAmreset+DRAM chip select+enable CKE +auto initial dram cycle
_RV32_2(0x0600, 0x001F),   //0x001F #DRAmreset+DRAM chip select+enable CKE +auto initial dram cycle

//=================================================================================================
//UnMaskMiu0AllGroup:
_RV32_2(0x1246, 0x0000),   //Mask MIU0_group_0
_RV32_2(0x1266, 0x0000),   //Mask MIU0_group_1
_RV32_2(0x1286, 0x0000),   //Mask MIU0_group_2

//UnMaskMiu1AllGroup:
_RV32_2(0x0646, 0x0000),   //Mask MIU0_group_0
_RV32_2(0x0666, 0x0000),   //Mask MIU0_group_1
_RV32_2(0x0686, 0x0000),   //Mask MIU0_group_2
