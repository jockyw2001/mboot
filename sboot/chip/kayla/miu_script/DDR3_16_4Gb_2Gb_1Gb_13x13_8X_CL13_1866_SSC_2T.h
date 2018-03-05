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
#define _RV32_1(addr, value)    {addr, (((value) & 0xff) << 8) | 0xff}
#define _RV32_1_MASK(addr, mask, value)    {addr, (((value) & 0xff) << 8) | mask}
#define _RV32_2(a,b)    {a<<1,b}
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x1220,0x0000),
_RV32_2(0x121e,0x0c00),
_RV32_2(0x121e,0x0c00),
_RV32_2(0x121e,0x0c00),
_RV32_2(0x121e,0x0c00),
_RV32_2(0x1202,0x0000),
_RV32_2(0x1246,0xfffe),
_RV32_2(0x1266,0xffff),
_RV32_2(0x1286,0xffff),
_RV32_2(0x12a6,0xffff),
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d36,0x4000),
_RV32_2(0x0d34,0x0400),
_RV32_2(0x0d30,0xF45A),
_RV32_2(0x0d32,0x001d),
_RV32_2(0x0d22,0x2000),
_RV32_2(0x0d24,0x2000),
_RV32_2(0x0d24,0x3000),
_RV32_2(0x0d24,0x2000),
{CHIPCFG_CMD_WAIT,10},
_RV32_2(0x0d2a,0x0482),
_RV32_2(0x0d28,0xC011),
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x1202,0x03a3),
_RV32_2(0x1204,0x000b),
_RV32_2(0x1206,0x1538),
_RV32_2(0x1208,0x20dd),
_RV32_2(0x120a,0x2e76),
_RV32_2(0x120c,0xb8e9),
_RV32_2(0x120e,0x40F3),
_RV32_2(0x1210,0x1f14),
_RV32_2(0x1212,0x4040),
_RV32_2(0x1214,0x8020),
_RV32_2(0x1216,0xc000),
_RV32_2(0x1228,0x0400),
_RV32_2(0x12d2,0x8000),
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d02,0xaaaa),
_RV32_2(0x0d04,0x0080),
_RV32_2(0x0d0a,0x1133),
_RV32_2(0x0d38,0x0054),
_RV32_2(0x0d3a,0x6066),
_RV32_2(0x0d3c,0x9444),
_RV32_2(0x0d3e,0x2044),
_RV32_2(0x0d48,0x0055),
_RV32_2(0x0d4a,0x0002),
_RV32_2(0x0d4c,0x3333),
_RV32_2(0x0d4e,0x0033),
_RV32_2(0x0d50,0x1111),
_RV32_2(0x0d52,0x0003),
_RV32_2(0x0d6c,0x5555),
_RV32_2(0x0d6e,0x8787),
_RV32_2(0x0d54,0xc000),
_RV32_2(0x0d08,0x003f),
_RV32_2(0x0d0e,0xc0a9),
_RV32_2(0x0d0e,0xc0e9),
_RV32_2(0x0d0e,0xc0a9),
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x121e,0x0c01),
_RV32_2(0x121e,0x0c00),
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d00,0x2010),
_RV32_2(0x0d00,0x0000),
_RV32_2(0x0d7c,0x0000),
_RV32_2(0x0d00,0x0605),
_RV32_2(0x0d0e,0xc0a9),
_RV32_2(0x0d0e,0xc0e9),
_RV32_2(0x0d0e,0xc0a9),
_RV32_2(0x0d58,0x0404),
_RV32_2(0x0d5a,0x4444),
_RV32_2(0x0d5c,0x4444),
_RV32_2(0x0d5e,0x4444),
_RV32_2(0x0d60,0x000c),
_RV32_2(0x0d60,0x0008),
_RV32_2(0x0d60,0x2370),
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x1200,0x0000),
_RV32_2(0x1200,0x0008),
_RV32_2(0x1200,0x000c),
{CHIPCFG_CMD_WAIT,1},
_RV32_2(0x1200,0x000e),
{CHIPCFG_CMD_WAIT,1},
_RV32_2(0x1200,0x001f),
{CHIPCFG_CMD_WAIT,10},
_RV32_2(0x121e,0x1a08),
_RV32_2(0x1240,0x8015),
_RV32_2(0x1260,0x8015),
_RV32_2(0x1280,0x8015),
_RV32_2(0x12a0,0x8015),
_RV32_2(0x1246,0x0000),
_RV32_2(0x1266,0x0000),
_RV32_2(0x1286,0x0000),
_RV32_2(0x12a6,0x0000),
_RV32_2(0x12fe,0x0087),
_RV32_2(0x1252,0xffff),
_RV32_2(0x1272,0xffff),
_RV32_2(0x1292,0xffff),
_RV32_2(0x12b2,0xffff),
