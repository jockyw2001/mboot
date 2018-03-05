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
#if (ENABLE_MEM1300MHz)
#if (ENABLE_MSTAR_DDR_256MB_256MB)
#elif (ENABLE_MSTAR_DDR_128MB_128MB)
#elif (ENABLE_MSTAR_DDR_512MB_512MB)
#else
#error "Invalid MIU DRAM Size Selection"
#endif
#else
#error "Invalid MIU Memory Frequency Selection"
#endif

#define _RV32_2(a,b)    {a<<1,b}
//
//swch 4
//
//MIU0 Software reset
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x121e, 0x0000),
_RV32_2(0x121e, 0x0c01),
_RV32_2(0x121e, 0x0e00),
_RV32_2(0x1202, 0x0000),
//
//MIU0 Request mask
_RV32_2(0x12f8, 0x8000),
_RV32_2(0x1246, 0xFFFE),
_RV32_2(0x1266, 0xFFFF),
_RV32_2(0x1286, 0xFFFF),
_RV32_2(0x12A6, 0xFFFF),
//
#if((!(ENABLE_MSTAR_DDR_256MB))&(!(ENABLE_MSTAR_DDR_512MB)))
//MIU1 Software reset
_RV32_2(0x061e, 0x0000),
_RV32_2(0x061e, 0x0c01),
_RV32_2(0x061e, 0x0e00),
_RV32_2(0x0602, 0x0000),
//
//MIU1 Request mask
_RV32_2(0x06f8, 0x8000),
_RV32_2(0x0646, 0xFFFE),
_RV32_2(0x0666, 0xFFFF),
_RV32_2(0x0686, 0xFFFF),
_RV32_2(0x06A6, 0xFFFF),
//
#endif
//=================================================================================================
// MIU0: 128MB DDR3-1600
//
{CHIPCFG_CMD_BASE, 0xbf22},
#if (ENABLE_MEM1300MHz)
_RV32_2(0x0d3a, 0x0000),      // mclk1x_skew cmd phase /dqsmask0 /mclk /dqsmask1 phase select
#if (ENABLE_MSTAR_DDR_512MB_512MB)
_RV32_2(0x0d36, 0x4004),      // mclk1x_clkph_ck0 / input div_1st/loop_div_1st
#else
_RV32_2(0x0d36, 0x4002),      // mclk1x_clkph_ck0 / input div_1st/loop_div_1st
#endif
_RV32_2(0x0d34, 0x0300),      // loop_div/input_div
_RV32_2(0x0d30, 0x2e8b),      // ddfset
_RV32_2(0x0d32, 0x001f),      // ddrpll_reset/ddrpll_porst/ddrpl_pd
_RV32_2(0x0d24, 0x0020),
_RV32_2(0x0d24, 0x0000),
#endif
{CHIPCFG_CMD_WAIT, 4},            //wait  4                         // delay 4ms
//
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x1202, 0x03a3),      //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+8banks+10col+4x

#if (ENABLE_MEM1300MHz)
_RV32_2(0x1204, 0x000B),      //if I64Mode =0x8b else =0x0b
#endif
_RV32_2(0x1206, 0x8434),      //refresh cycle=0x50 ,TURN_ON_SCRAMBLE=0x8450
#if (ENABLE_MEM1300MHz)
#if (ENABLE_MSTAR_DDR_512MB_512MB)
_RV32_2(0x1208, 0x1CBB),      //reg_tRCD
_RV32_2(0x120a, 0x2766),      //reg_tRRD
_RV32_2(0x120c, 0xB6CA),      //reg_tWL
_RV32_2(0x120e, 0x40C0),      //tRFC
#else
_RV32_2(0x1208, 0x1899),      //reg_tRCD
_RV32_2(0x120a, 0x2165),      //reg_tRRD
_RV32_2(0x120c, 0x96BA),      //reg_tWL
_RV32_2(0x120e, 0x406B),      //tRFC
#endif
#endif
_RV32_2(0x1210, 0x1c70),      //MR0
_RV32_2(0x1212, 0x4004),      //MR1
_RV32_2(0x1214, 0x8018),      //MR2
_RV32_2(0x1216, 0xC000),      //MR3
_RV32_2(0x1228, 0xb270),      //pack_en enable
#if (ENABLE_MEM1300MHz)
_RV32_2(0x122c, 0x71c1),      // Address Balance Mode
#endif
_RV32_2(0x1230, 0x0000),
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d54, 0x0000),      //miu_0_atop  start
#if (ENABLE_MEM1300MHz)
_RV32_2(0x0d00, 0x0411),
#endif
_RV32_2(0x0d02, 0x0000),
_RV32_2(0x0d04, 0x0080),
_RV32_2(0x0d06, 0x0000),
_RV32_2(0x0d08, 0x003F),
_RV32_2(0x0d0a, 0x0000),
_RV32_2(0x0d0c, 0x0000),
_RV32_2(0x0d0e, 0x00e5),
_RV32_2(0x0d0e, 0x00a5),
_RV32_2(0x0d10, 0x0600),
_RV32_2(0x0d12, 0x0000),
_RV32_2(0x0d14, 0x0000),
_RV32_2(0x0d16, 0x0200),
_RV32_2(0x0d18, 0x0000),
_RV32_2(0x0d1a, 0x0000),
_RV32_2(0x0d1c, 0x0000),
//wriu -w   0x110d1e  0xffff
_RV32_2(0x0d20, 0x0020),
_RV32_2(0x0d22, 0x0000),
_RV32_2(0x0d24, 0x0000),
_RV32_2(0x0d24, 0x0000),
_RV32_2(0x0d26, 0x0000),
_RV32_2(0x0d28, 0x0012),    //set MIU0 ssc
_RV32_2(0x0d2a, 0x0458),    //set MIU0 ssc
_RV32_2(0x0d2c, 0x0014),
_RV32_2(0x0d2e, 0x0000),
#if (ENABLE_MEM1300MHz)
_RV32_2(0x0d38, 0x0043),
_RV32_2(0x0d3a, 0x0000),
_RV32_2(0x0d3c, 0x0077),
#if (ENABLE_MSTAR_DDR_512MB_512MB)
_RV32_2(0x0d38, 0x3376),
#else
_RV32_2(0x0d38, 0x0043),
#endif
_RV32_2(0x0d3a, 0x0000),
#endif
_RV32_2(0x0d3c, 0x0077),
_RV32_2(0x0d3e, 0x0000),
_RV32_2(0x0d40, 0x0000),
_RV32_2(0x0d42, 0x0000),
_RV32_2(0x0d44, 0x0000),
_RV32_2(0x0d46, 0x0000),
_RV32_2(0x0d48, 0x0000),
_RV32_2(0x0d4a, 0x0000),
_RV32_2(0x0d4c, 0x0000),
_RV32_2(0x0d4e, 0x0000),
_RV32_2(0x0d50, 0x0000),
_RV32_2(0x0d52, 0x0000),
_RV32_2(0x0d54, 0x0000),
_RV32_2(0x0d56, 0x0000),
_RV32_2(0x0d58, 0x0000),
_RV32_2(0x0d5a, 0x0000),
_RV32_2(0x0d5c, 0x4444),
_RV32_2(0x0d5e, 0x4444),
_RV32_2(0x0d60, 0x2370),
_RV32_2(0x0d62, 0x0032),
_RV32_2(0x0d64, 0xf200),
_RV32_2(0x0d66, 0x0000),
_RV32_2(0x0d68, 0x0020),
_RV32_2(0x0d6a, 0x0020),
_RV32_2(0x0d6c, 0x0000),
#if (ENABLE_MEM1300MHz)
_RV32_2(0x0d6e, 0x0089),
_RV32_2(0x0d72, 0x8804),
#endif
_RV32_2(0x0d0e, 0x00a5),
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x121e, 0x0c01),
_RV32_2(0x121e, 0x0c00),
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d7e, 0x0000),
#if (ENABLE_MEM1300MHz)
_RV32_2(0x0d00, 0x0401),
#endif
_RV32_2(0x0d02, 0xaaaa),
//
//=================================================================================================
// MIU1: 128MB DDR3-1600
//
#if((!(ENABLE_MSTAR_DDR_256MB))&(!(ENABLE_MSTAR_DDR_512MB)))
#if (ENABLE_MEM1300MHz)
_RV32_2(0x0dba, 0x0000),      // mclk1x_skew cmd phase /dqsmask0 /mclk /dqsmask1 phase select
_RV32_2(0x0db6, 0x4003),      // mclk1x_clkph_ck0 / input div_1st/loop_div_1st
_RV32_2(0x0db4, 0x0300),      // loop_div/input_div
_RV32_2(0x0db0, 0x2e8b),      // ddfset
_RV32_2(0x0db2, 0x001f),      // ddrpll_reset/ddrpll_porst/ddrpl_pd
_RV32_2(0x0da4, 0x0020),
_RV32_2(0x0da4, 0x0000),
#endif
//
{CHIPCFG_CMD_WAIT, 4},            //wait  4                         // delay 4ms
//
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x0602, 0x03a3),      //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+8banks+10col+4x

#if (ENABLE_MEM1300MHz)
_RV32_2(0x0604, 0x000B),      //if I64Mode =0x8b else =0x0b
#endif
_RV32_2(0x0606, 0x8434),      //refresh cycle=0x20 ,TURN_ON_SCRAMBLE=0x8420
#if (ENABLE_MEM1300MHz)
#if (ENABLE_MSTAR_DDR_512MB_512MB)
_RV32_2(0x0608, 0x1CBB),      //reg_tRCD
_RV32_2(0x060a, 0x2766),      //reg_tRRD
_RV32_2(0x060c, 0xB6CA),      //reg_tWL
_RV32_2(0x060e, 0x40C0),      //tRFC
#else
_RV32_2(0x0608, 0x1899),      //reg_tRCD
_RV32_2(0x060a, 0x2165),      //reg_tRRD
_RV32_2(0x060c, 0x96BA),      //reg_tWL
_RV32_2(0x060e, 0x406B),      //tRFC
#endif
#endif
_RV32_2(0x0610, 0x1c70),      //MR0
_RV32_2(0x0612, 0x4004),      //MR1
_RV32_2(0x0614, 0x8018),      //MR2
_RV32_2(0x0616, 0xC000),      //MR3
_RV32_2(0x0628, 0xb270),      //pack_en enable
#if (ENABLE_MEM1300MHz)
_RV32_2(0x062c, 0x71c1),      // Address Balance Mode
#endif
_RV32_2(0x0630, 0x0000),
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0dd4, 0x0000),
_RV32_2(0x0d80, 0x0411),
_RV32_2(0x0d82, 0x0000),
_RV32_2(0x0d84, 0x0080),
_RV32_2(0x0d86, 0x0000),
_RV32_2(0x0d88, 0x003F),
_RV32_2(0x0d8a, 0x0000),
_RV32_2(0x0d8c, 0x0000),
_RV32_2(0x0d8e, 0x00e5),
_RV32_2(0x0d8e, 0x00a5),
_RV32_2(0x0d90, 0x0600),
_RV32_2(0x0d92, 0x0000),
_RV32_2(0x0d94, 0x0000),
_RV32_2(0x0d96, 0x0200),
_RV32_2(0x0d98, 0x0000),
_RV32_2(0x0d9a, 0x0000),
_RV32_2(0x0d9c, 0x0000),
//wriu -w   0x110d9e  0xffff
_RV32_2(0x0da0, 0x0020),
_RV32_2(0x0da2, 0x0000),
_RV32_2(0x0da4, 0x0000),
_RV32_2(0x0da6, 0x0000),
_RV32_2(0x0da8, 0x0012),    //set MIU1 ssc
_RV32_2(0x0daa, 0x0458),    //set MIU1 ssc
_RV32_2(0x0dac, 0x0014),
_RV32_2(0x0dae, 0x0000),
#if (ENABLE_MEM1300MHz)
_RV32_2(0x0db8, 0x0053),
_RV32_2(0x0dba, 0x0000),
_RV32_2(0x0dbc, 0x0077),
#if (ENABLE_MSTAR_DDR_512MB_512MB)
_RV32_2(0x0db8, 0x3386),
#else
_RV32_2(0x0db8, 0x0053),
#endif
#endif
_RV32_2(0x0dba, 0x0000),
_RV32_2(0x0dbc, 0x0077),
_RV32_2(0x0dbe, 0x0000),
_RV32_2(0x0dc0, 0x0000),
_RV32_2(0x0dc2, 0x0000),
_RV32_2(0x0dc4, 0x0000),
_RV32_2(0x0dc6, 0x0000),
_RV32_2(0x0dc8, 0x0000),
_RV32_2(0x0dca, 0x0000),
_RV32_2(0x0dcc, 0x0000),
_RV32_2(0x0dce, 0x0000),
_RV32_2(0x0dd0, 0x0000),
_RV32_2(0x0dd2, 0x0000),
_RV32_2(0x0dd4, 0x0000),
_RV32_2(0x0dd6, 0x0000),
_RV32_2(0x0dd8, 0x0000),
_RV32_2(0x0dda, 0x0000),
#if (ENABLE_MEM1300MHz)
_RV32_2(0x0ddc, 0x4444),
_RV32_2(0x0dde, 0x4444),
#endif
_RV32_2(0x0de0, 0x2370),
_RV32_2(0x0de2, 0x0032),
_RV32_2(0x0de4, 0xf200),
_RV32_2(0x0de6, 0x0000),
_RV32_2(0x0de8, 0x0020),
_RV32_2(0x0dea, 0x0020),
_RV32_2(0x0dec, 0x0000),
#if (ENABLE_MEM1300MHz)
#if (ENABLE_MSTAR_DDR_512MB_512MB)
_RV32_2(0x0dee, 0x0089),
#else
_RV32_2(0x0dee, 0x008a),
#endif
_RV32_2(0x0df2, 0x8f04),
_RV32_2(0x0df2, 0x8f04),
#endif
_RV32_2(0x0d8e, 0x00a5),
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x061e, 0x0c01),
_RV32_2(0x061e, 0x0c00),
//wriu -w   0x1012fe  0x0000
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0dfe, 0x0000),
_RV32_2(0x0d80, 0x0401),
_RV32_2(0x0d82, 0xaaaa),
//
//=================================================================================================
#endif //MIU1

{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x1200, 0x0000),
_RV32_2(0x1200, 0x0008),
_RV32_2(0x1200, 0x000c),
{CHIPCFG_CMD_WAIT, 1},//wait  1                     // delay 1ms
_RV32_2(0x1200, 0x000e),
{CHIPCFG_CMD_WAIT, 1},//wait  1                     // delay 1ms
_RV32_2(0x1200, 0x001f),
{CHIPCFG_CMD_WAIT, 50},//wait 50                    // delay 50ms
//
#if((!(ENABLE_MSTAR_DDR_256MB))&(!(ENABLE_MSTAR_DDR_512MB)))
_RV32_2(0x0600, 0x0000),
_RV32_2(0x0600, 0x0008),
_RV32_2(0x0600, 0x000c),
{CHIPCFG_CMD_WAIT, 1},//wait  1                     // delay 1ms
_RV32_2(0x0600, 0x000e),
{CHIPCFG_CMD_WAIT, 1},//wait  1                     // delay 1ms
_RV32_2(0x0600, 0x001f),
{CHIPCFG_CMD_WAIT, 50},//wait  50                   // delay 50ms
//
#endif
//=================================================================================================
//MIU_Init_Priority
//MIU_Set_Group_Priority
//
_RV32_2(0x1246, 0x0000),   //unmask group0
_RV32_2(0x1266, 0x0000),   //unmask group1
_RV32_2(0x1286, 0x0000),   //unmask group2
_RV32_2(0x12A6, 0x0000),   //mask group3 //jc_db: should u4 have group3
//
#if((!(ENABLE_MSTAR_DDR_256MB))&(!(ENABLE_MSTAR_DDR_512MB)))
_RV32_2(0x0646, 0x0000),   //unmask group0
_RV32_2(0x0666, 0x0000),   //unmask group1
_RV32_2(0x0686, 0x0000),   //unmask group2
_RV32_2(0x06A6, 0x0000),   //mask group3 //jc_db: should u4 have group3
//
#endif
//=================================================================================================
_RV32_2(0x121e, 0x0c08),     //SW initial done and turn on arbitor
//
//
