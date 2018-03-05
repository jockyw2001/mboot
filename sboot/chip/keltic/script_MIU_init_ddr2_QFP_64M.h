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
//swch 4
{CHIPCFG_CMD_BASE, 0xbf20},
    // =====MIU0===
_RV32_2(0x121e, 0x0c00),
_RV32_2(0x121e, 0x0c00),
_RV32_2(0x121e, 0x0c00),
_RV32_2(0x121e, 0x0c01),
_RV32_2(0x121e, 0x0c00),
_RV32_2(0x1202, 0x0000),
_RV32_2(0x1246, 0xfffe),
_RV32_2(0x1266, 0xffff),
_RV32_2(0x1286, 0xffff),
_RV32_2(0x12a6, 0xffff),
//DDRPLL0 800MHz setting
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d36,	0x0002),
_RV32_2(0x0d34,	0x0200),
#if (ENABLE_MEM1066MHz)
_RV32_2(0x0d30,	0x6e1f),
_RV32_2(0x0d32,	0x0034),
#elif (ENABLE_MEM800MHz)
_RV32_2(0x0d30, 0xdcf9),
_RV32_2(0x0d32, 0x0045),
#elif (ENABLE_MEM667MHz)
_RV32_2(0x0d30, 0x5759),
_RV32_2(0x0d32, 0x0053),
#else
#error Unknow DDR Frequency
#endif

_RV32_2(0x0d2A,	0x01EE),  // SSC  period (span) 
_RV32_2(0x0d28,	0x805D),  // SSC enable (step)


_RV32_2(0x0d24,	0x1000), //reg_ddrat[28], 0=>1=>0 
_RV32_2(0x0d24,	0x0000),
{CHIPCFG_CMD_WAIT, 1}, //WAIT 0.001s	// delay 1ms

{CHIPCFG_CMD_BASE, 0xbf20},

_RV32_2(0x1202,	0x0292),
_RV32_2(0x1204,	0x008b), //rd_timing			  b 	  : [4:0]
_RV32_2(0x1206,	0x0434),
#if (ENABLE_MEM1066MHz)
_RV32_2(0x1208,	0x1877),
_RV32_2(0x120a,	0x1f46),
_RV32_2(0x120c,	0x7486),
_RV32_2(0x120e,	0x206a),
_RV32_2(0x1210,	0x0e72),
#else
_RV32_2(0x1208,	0x1466),
_RV32_2(0x120a,	0x1844),
_RV32_2(0x120c,	0x7465),
_RV32_2(0x120e,	0x204f),
_RV32_2(0x1210,	0x0a62),
#endif
_RV32_2(0x1212,	0x4004),
_RV32_2(0x1214,	0x8000),
_RV32_2(0x1216,	0xc000),

{CHIPCFG_CMD_BASE, 0xbf22},

_RV32_2(0x0d00,  0x001a),
_RV32_2(0x0d38,  0x0055), //reg_clkph_dq0         3       : [3:0]
               //reg_clkph_dq1         3       : [7:4]
               //reg_clkph_dqs0        0       : [11:8]
               //reg_clkph_dqs1        0       : [15:12]
_RV32_2(0x0d3a,  0x4040), //reg_clkph_cmd         0       : [3:0]
               //reg_clkph_dqsm0       0       : [7:4]
               //reg_clkph_clk_a       0       : [11:8]
               //reg_clkph_dqsm1       0       : [15:12]
_RV32_2(0x0d02,  0x0000), //reg_cko_state         00      : [7:0]
                           //reg_dqs_state         00      : [15:8]
_RV32_2(0x0d04,  0x0000), //reg_dqs_pre_state : [7:0]
#if (ENABLE_MEM1066MHz)
_RV32_2(0x0d3c,  0x0022), //reg_dqsm0_skew 0 [2:0]
                           //reg_dqsm1_skew 0 [6:4]
                           //reg_dqs_skew   2 [10:8]
                           //reg_cmd1_skew  0 [14:12]
_RV32_2(0x0d3e,  0x0000), //[2:0]  reg_oen_skew
               //[6:4]  reg_dq_skew
               //[10:8] reg_cmd_skew
               //[14:12]    reg_cko_skew
#else
_RV32_2(0x0d3c,  0x0200), //reg_dqsm0_skew 0 [2:0]
                           //reg_dqsm1_skew 0 [6:4]
                           //reg_dqs_skew   2 [10:8]
                           //reg_cmd1_skew  0 [14:12]
_RV32_2(0x0d3e,  0x0022), //[2:0]  reg_oen_skew
               //[6:4]  reg_dq_skew
               //[10:8] reg_cmd_skew
               //[14:12]    reg_cko_skew
#endif
_RV32_2(0x0d0e,  0x00e7), //[3:1] reg_dqsm_dly
_RV32_2(0x0d0e,  0x00a7),
_RV32_2(0x0d6e,  0x0066), //reg_dll0_ph	: [3:0]
               //reg_dll1_ph    : [7:4]


_RV32_2(0x0d6a,  0x00c2),
_RV32_2(0x0d68,  0x00c2),
_RV32_2(0x0d54,  0x0000), //reg_cmd_mode_sel    [15:14] 00:default mode 01:2cmd mode 10:2cmd mode 11:2cmd mode(cs)
{CHIPCFG_CMD_BASE, 0xbf20},

_RV32_2(0x121e,  0x0c01), //miu software reset
_RV32_2(0x121e,  0x0c00),

{CHIPCFG_CMD_WAIT, 1}, //WAIT 0.001s	// delay 1ms
{CHIPCFG_CMD_BASE, 0xbf22},

_RV32_2(0x0d7e,  0x0000),
_RV32_2(0x0d00,  0x0011), // 
_RV32_2(0x0d00,  0x0001), //reg_pkg_sel 0:DDR3, 1:BGA, 2:QFP

_RV32_2(0x0d02,  0xaaaa),
_RV32_2(0x0d08,  0x00ff),

_RV32_2(0x0d74,  0x0077), //trig lvl
_RV32_2(0x0d5c,  0x3333), //drvn
_RV32_2(0x0d5e,  0x3333), //drvp
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x1200,  0x0000),
_RV32_2(0x1200,  0x0008),
_RV32_2(0x1200,  0x000c),
{CHIPCFG_CMD_WAIT, 1}, //WAIT 0.001s	// delay 1ms

_RV32_2(0x1200,  0x000e),
{CHIPCFG_CMD_WAIT, 1}, //WAIT 0.001s	// delay 1ms

_RV32_2(0x1200,  0x001f),

{CHIPCFG_CMD_WAIT, 10}, ///check reg_101200 #15 for wait initial done

//analog de glitch
_RV32_2(0x1246,  0x7FFE),
_RV32_2(0x121e,  0x0c08),
_RV32_2(0x12e0,  0x0000),
_RV32_2(0x12e2,  0x0000),
_RV32_2(0x12e4,  0x0001),
_RV32_2(0x12e6,  0x0000),
_RV32_2(0x12e8,  0x5aa5),
{CHIPCFG_CMD_BASE, 0xbf22},

_RV32_2(0x0d0e,  0x00e7),// bit6
{CHIPCFG_CMD_BASE, 0xbf20},

_RV32_2(0x12e0,  0x0200),
_RV32_2(0x12e0,  0x0201),
_RV32_2(0x12e8,  0xffff),
_RV32_2(0x12e8,  0xffff),
{CHIPCFG_CMD_BASE, 0xbf22},

_RV32_2(0x0d0e,  0x00a7), //0 bit6 
{CHIPCFG_CMD_WAIT, 1},

{CHIPCFG_CMD_BASE, 0xbf20},

//BW setting
_RV32_2(0x1240,  0x8001),
_RV32_2(0x1260,  0x8001),
_RV32_2(0x1280,  0x8001),
_RV32_2(0x12a0,  0x8001),

{CHIPCFG_CMD_BASE, 0xbf22},

_RV32_2(0x0d60,  0x000c),
_RV32_2(0x0d60,  0x0008),
_RV32_2(0x0d62,  0x007f),
_RV32_2(0x0d64,  0xf200),
_RV32_2(0x0d60,  0x2738),

{CHIPCFG_CMD_BASE, 0xbf20},

//unmask all
_RV32_2(0x1246,  0x0000), //unmask group0
_RV32_2(0x1266,  0x0000), //unmask group1
_RV32_2(0x1286,  0x0000), //unmask group2
_RV32_2(0x12a6,  0x0000), //unmask group3

_RV32_2(0x121e,  0x0c08), //threshold, release ff mask(bit3) (enable sw init done)
_RV32_2(0x12f0,  0x0000), //sel
_RV32_2(0x12f6,  0x0000), //reg_dramobf_pipe_en [0]
                           //reg_dramobf_bypass  [1]
                           //reg_dramobf_legacy  [2]


