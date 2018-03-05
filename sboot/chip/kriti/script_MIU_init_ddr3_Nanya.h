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
//================================================
// Start MIU init !!!             
//================================================
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x1220, 0x0000),   //miu initial
_RV32_2(0x121e, 0x0c00),   //miu initial
_RV32_2(0x121e, 0x0c00),
_RV32_2(0x121e, 0x0c00),
_RV32_2(0x121e, 0x0c01),   //miu software reset
_RV32_2(0x121e, 0x0c00),
_RV32_2(0x1202, 0x0000),   //dram type set as '0'
_RV32_2(0x1246, 0xFFFE),   //mask group0
_RV32_2(0x1266, 0xFFFF),   //mask group1
_RV32_2(0x1286, 0xFFFF),   //mask group2
_RV32_2(0x12A6, 0xFFFF),   //mask group3
//--------------------
//set DDRPLL0 1600MHz
//--------------------
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d36, 0x4000),
_RV32_2(0x0d34, 0x0400),
#if ENABLE_MEM1600MHz
_RV32_2(0x0d30, 0xE8BA),		//Clock generator frequency set
_RV32_2(0x0d32, 0x0022),		//MCLK Freq (miu0)
#elif ENABLE_MEM1333MHz
_RV32_2(0x0d30, 0x7b7c),		//Clock generator frequency set
_RV32_2(0x0d32, 0x0029),		//MCLK Freq (miu0)
#endif
_RV32_2(0x0d22, 0x2000),
_RV32_2(0x0d24, 0x2000),
_RV32_2(0x0d24, 0x3000),
_RV32_2(0x0d24, 0x2000),
//------------------
// Wait PLL Stable
//------------------
{CHIPCFG_CMD_WAIT, 10}, //wait 10
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x1202, 0x03a3),
_RV32_2(0x1204, 0x004a),
_RV32_2(0x1206, 0x1430),//[12:12] reg_cke_always_on	
_RV32_2(0x1208, 0x1cbb),
_RV32_2(0x120a, 0x2767),
_RV32_2(0x120c, 0xb6c8),
_RV32_2(0x120e, 0x4080),
_RV32_2(0x1210, 0x1d70),
_RV32_2(0x1212, 0x4004),
_RV32_2(0x1214, 0x8018),
_RV32_2(0x1216, 0xc000),
_RV32_2(0x122c, 0x0000),
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d02, 0xaaaa),
_RV32_2(0x0d04, 0x0080),
_RV32_2(0x0d3c, 0x1133),
_RV32_2(0x0d4e, 0x0033),
_RV32_2(0x0d3e, 0x1011),
_RV32_2(0x0d4c, 0x1133),
_RV32_2(0x0d0a, 0x1022),
_RV32_2(0x0d50, 0x1111),
_RV32_2(0x0d38, 0x0065),
_RV32_2(0x0d3a, 0x4042),
_RV32_2(0x0d48, 0x0033),
_RV32_2(0x0d4a, 0x0002),
_RV32_2(0x0d52, 0x0002),
_RV32_2(0x0d74, 0x2077),
_RV32_2(0x0d78, 0x0002),
_RV32_2(0x0d7a, 0x3800),
_RV32_2(0x0d6c, 0x5555),
_RV32_2(0x0d6e, 0x3344),
_RV32_2(0x0d0e, 0x00a7),
_RV32_2(0x0d0e, 0x00e7),
_RV32_2(0x0d0e, 0x00a7),
{CHIPCFG_CMD_BASE, 0xbf20},

/* vivaldi block sw reset*/

{CHIPCFG_CMD_BASE_8BITS, 0xBF20},

_RV32_2(0x1EFE, 0x0010),
_RV32_2(0x1EFE, 0x1010),

{CHIPCFG_CMD_BASE, 0xbf20},
/* vivaldi block sw reset end*/

_RV32_2(0x121e, 0x0c01),
_RV32_2(0x121e, 0x0c00),
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d00, 0x0039),
_RV32_2(0x0d00, 0x0031),
_RV32_2(0x0d00, 0x0021),
_RV32_2(0x0d7c, 0x000f),
_RV32_2(0x0d7e, 0x0000),
_RV32_2(0x0d54, 0xc000),
_RV32_2(0x0d08, 0x00ff),

_RV32_2(0x0d0e, 0x00a7),
_RV32_2(0x0d0e, 0x00e7),
_RV32_2(0x0d0e, 0x00a7),

_RV32_2(0x0d74, 0x0007),
_RV32_2(0x0d58, 0x5c5c),
_RV32_2(0x0d5c, 0x5555),
_RV32_2(0x0d5e, 0x5555),

_RV32_2(0x0d28, 0x0017),
_RV32_2(0x0d2a, 0x03DE),

{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x1200, 0x0000),
//-----Wait 200us, (keep DRAM RESET = 0)-----
{CHIPCFG_CMD_WAIT, 1}, //wait 1
_RV32_2(0x1200, 0x0008),
_RV32_2(0x1200, 0x000c),
//-----Wait 500us, (keep DRAM RESET = 1 , CKE = 0)-----
{CHIPCFG_CMD_WAIT, 1}, //wait 1
_RV32_2(0x1200, 0x000e),
//-----Wait tXPR=400ns-----
{CHIPCFG_CMD_WAIT, 1}, //wait 1
//--------Initial DRAM start here!!!
_RV32_2(0x1200, 0x001f),
//-----Wait init done-----
{CHIPCFG_CMD_WAIT, 100}, //wait 100
//--------Initial Done
//--------DDR2 wait 400ns for tXPR(?), DDR3 wait 512T for tZQinit
{CHIPCFG_CMD_WAIT, 1}, //wait 1
//-------------
// BW setting  
//-------------
_RV32_2(0x1240, 0x8015),
_RV32_2(0x1260, 0x8015),
_RV32_2(0x1280, 0x8015),
_RV32_2(0x12a0, 0x8015),

{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d62, 0x0032),
_RV32_2(0x0d64, 0xf200),
_RV32_2(0x0d68, 0x0020),
_RV32_2(0x0d6a, 0x0020),
_RV32_2(0x0d60, 0x000c),
_RV32_2(0x0d60, 0x0008),
_RV32_2(0x0d60, 0x2370),
//--------------------
// MIU select setting 
//----------- --------
//---------------------------------
// Pack setting                    
//---------------------------------
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x1228, 0x4090),
//-------------------------
// Reduce latency setting  
//-------------------------
_RV32_2(0x122a, 0x0000),
_RV32_2(0x12d4, 0x0000),
//------------------------------
// unmask all                   
//------------------------------
_RV32_2(0x121a, 0x0001),
_RV32_2(0x12fc, 0x0400),
{CHIPCFG_CMD_WAIT, 100}, //wait 100
_RV32_2(0x1246, 0x0000),
_RV32_2(0x1266, 0x0000),
_RV32_2(0x1286, 0x0000),
_RV32_2(0x12a6, 0x0000),
_RV32_2(0x121e, 0x5a08),
_RV32_2(0x12fe, 0x00e1),
_RV32_2(0x12fe, 0x01e1),
_RV32_2(0x12fe, 0x00e1),
//=================================================================
// End of MIU init !!!             
//=================================================================
