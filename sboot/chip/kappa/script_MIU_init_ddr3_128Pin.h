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
_RV32_2(0x0bfc, 0x0001),
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

{CHIPCFG_CMD_BASE, 0xbf22},
#if ENABLE_MEM1300MHz
_RV32_2(0x0d5c, 0x7777),
_RV32_2(0x0d5e, 0x7777),
#else
_RV32_2(0x0d5c, 0xAAAA),    //Driver strength
_RV32_2(0x0d5e, 0xAAAA),    //Driver strength
#endif

_RV32_2(0x0d74, 0x0044),    //Trigger level

//--------------------
//set DDRPLL0 1333/1600MHz
//--------------------
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d36, 0x4000),    //write CLK phase
_RV32_2(0x0d34, 0x0400),
#if ENABLE_MEM1300MHz
_RV32_2(0x0d30, 0x7b7c),
_RV32_2(0x0d32, 0x0029),
#else
_RV32_2(0x0d30, 0xe8ba),    //Clock generator frequency set
_RV32_2(0x0d32, 0x0022),    //Clock generator frequency set
#endif
//SSC setting (1%)
_RV32_2(0x0d28, 0x0017),    //SSC enable (step)
_RV32_2(0x0d2a, 0x03de),    //SSC  period (span)

_RV32_2(0x0d22, 0x2000),
_RV32_2(0x0d24, 0x2000),
_RV32_2(0x0d24, 0x3000),
_RV32_2(0x0d24, 0x2000),

//------------------
// Wait PLL Stable
//------------------
{CHIPCFG_CMD_WAIT, 10}, //wait 10
#if ENABLE_MEM1300MHz
//---------------------------
// set DDR3_16_8X_CL9_1333
//---------------------------
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x1202, 0x03a3),   //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+8banks+10col+4x
_RV32_2(0x1204, 0x000a),
_RV32_2(0x1206, 0x0430),   //jc_db: should be 0x3420? //refresh cycle=0x20 ,TURN_ON_SCRAMBLE=0x8420
_RV32_2(0x1208, 0x1899),   //jc_db: should be 0x1877? //reg_tRCD
_RV32_2(0x120a, 0x2155),   //jc_db: should be 0x1F46? //reg_tRRD
_RV32_2(0x120c, 0x85a7),   //jc_db: should be 0x5486? //reg_tWL
_RV32_2(0x120e, 0x406b),   //tRF
_RV32_2(0x1210, 0x1b50),   //jc_db: should be 0x0e72? //MR0
_RV32_2(0x1212, 0x4004),   //MR1
_RV32_2(0x1214, 0x8010),   //MR2
_RV32_2(0x1216, 0xc000),   //MR3
_RV32_2(0x122c, 0x0000),

//======MIU_ATOP initial======
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d02, 0xaaaa),
_RV32_2(0x0d04, 0x0080),
_RV32_2(0x0d3c, 0x0744),
_RV32_2(0x0d3e, 0x1077),
_RV32_2(0x0d4c, 0x7733),
_RV32_2(0x0d50, 0x0000),
_RV32_2(0x0d38, 0x0033),
_RV32_2(0x0d3a, 0x0006),
_RV32_2(0x0d48, 0x0033),
_RV32_2(0x0d4a, 0x0002),
_RV32_2(0x0d52, 0x0066),
_RV32_2(0x0d6c, 0x5555),
_RV32_2(0x0d6e, 0x7777),
_RV32_2(0x0d0e, 0x00a5),
_RV32_2(0x0d0e, 0x00e5),
_RV32_2(0x0d0e, 0x00a5),
#else
//---------------------------
// set DDR3_16_8X_CL11_1600
//---------------------------
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x1202, 0x03a3),   //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+8banks+10col+4x
_RV32_2(0x1204, 0x000b),    //RD timing (miu0)
_RV32_2(0x1206, 0x0430),    //CSZ Always ON
_RV32_2(0x1208, 0x1cbb),    //tRAS/tRP/tRCD
_RV32_2(0x120a, 0x2766),    //tRC/tRTP/tRRD
_RV32_2(0x120c, 0x96c8),    //tRTW/tWTR/tWR/tWL
_RV32_2(0x120e, 0x4080),    //tCCD/tRFC
_RV32_2(0x1210, 0x1d70),    //MRS
_RV32_2(0x1212, 0x4004),    //EMRS 1
_RV32_2(0x1214, 0x8018),    //EMRS 2
_RV32_2(0x1216, 0xc000),    //MR3
_RV32_2(0x122c, 0x0000),

//======MIU_ATOP initial======
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d02, 0xaaaa),
_RV32_2(0x0d04, 0x0080),
_RV32_2(0x0d3c, 0x0133),    //Read DQSM 0/1 skew (miu0)
_RV32_2(0x0d3e, 0x1011),    //Write CLK/CMD/DQ skew(miu0)
_RV32_2(0x0d4c, 0x1133),
_RV32_2(0x0d50, 0x0000),
_RV32_2(0x0d38, 0x0045),    //write DQ phase
_RV32_2(0x0d3a, 0x6075),    //write CMD phase
_RV32_2(0x0d48, 0x0044),
_RV32_2(0x0d4a, 0x0002),
_RV32_2(0x0d52, 0x0066),
_RV32_2(0x0d6c, 0x5555),
_RV32_2(0x0d6e, 0x755A),    //Read DQS 0/1 phase (miu0)

//U02 DDR3 write phase setting
_RV32_2(0x0d48, 0x0042),
_RV32_2(0x0d7e, 0x8000),

_RV32_2(0x0d0e, 0x00a7),
_RV32_2(0x0d0e, 0x00e7),
_RV32_2(0x0d0e, 0x00a7),
#endif

{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x12fc, 0x0000),    //Sync in fifo stage

/* vivaldi block sw reset*/

{CHIPCFG_CMD_BASE_8BITS, 0xBF20},

_RV32_2(0x1EFE, 0x0010),
_RV32_2(0x1EFE, 0x1010),

{CHIPCFG_CMD_BASE, 0xbf20},
/* vivaldi block sw reset end*/

_RV32_2(0x121e, 0x0c01),
_RV32_2(0x121e, 0x0c00),

{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d00, 0x0038),
_RV32_2(0x0d00, 0x0030),
_RV32_2(0x0d00, 0x0220),    //A-top ODT (miu0)
_RV32_2(0x0d54, 0x0000),

//-----------------------
//start to program DLL0
//-----------------------
_RV32_2(0x0d62, 0x007f),
_RV32_2(0x0d60, 0x000c),
_RV32_2(0x0d60, 0x0008),
_RV32_2(0x0d60, 0x2370),
//start inivec
_RV32_2(0x0d08, 0x00ff),

//enable miu_0
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x1200, 0x000),
{CHIPCFG_CMD_WAIT, 1}, // Wait 200us, (keep DRAM RESET = 0)

_RV32_2(0x1200, 0x0008),
_RV32_2(0x1200, 0x000c),
{CHIPCFG_CMD_WAIT, 1}, // Wait 500us, (keep DRAM RESET = 1 , CKE = 0)

_RV32_2(0x1200, 0x000e),
{CHIPCFG_CMD_WAIT, 1}, // Wait tXPR=400ns

//--------Initial DRAM start here!!!
_RV32_2(0x1200, 0x001f),
{CHIPCFG_CMD_WAIT, 100}, // Wait init done

//--------Initial Done
//--------DDR2 wait 400ns for tXPR(?), DDR3 wait 512T for tZQinit
{CHIPCFG_CMD_WAIT, 1}, //wait  1
//-------------
// BW setting
//-------------
_RV32_2(0x1240, 0x8115),
_RV32_2(0x1260, 0x8115),
_RV32_2(0x1280, 0x8115),
_RV32_2(0x12a0, 0x8115),
_RV32_2(0x122a, 0x2423),    //act 2 bank function for HVD(id = 'h23) & MVD(id = 'h24)

//--------------------
// MIU select setting
//----------- --------
//---------------------------------
// Pack setting
//---------------------------------
_RV32_2(0x1228, 0x40bc),    //ODT mask/delay (miu0)

//-------------------------
// Reduce latency setting
//-------------------------
_RV32_2(0x12d4, 0x0000),

//== add de-glitch path ===============================================
_RV32_2(0x1246, 0x7ffe),
_RV32_2(0x121e, 0x0c08),
_RV32_2(0x12e0, 0x0000),    //Reset BIST
_RV32_2(0x12e2, 0x0000),    //Set base address to 0
_RV32_2(0x12e4, 0x0001),    //set BIST length for low word
_RV32_2(0x12e6, 0x0000),    //set BIST length for high word
_RV32_2(0x12e8, 0x5aa5),    //set BIST pattern

#if ENABLE_MEM1300MHz
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d0e, 0x00e5),    //dqsm_rst = 1s
#else
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d0e, 0x00e7),    //dqsm_rst = 1
#endif

{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x12e0, 0x0200),    //Set BIST to write only mode
_RV32_2(0x12e0, 0x0201),    //Fire BIST
_RV32_2(0x12e8, 0xffff),    //Dummy setting
_RV32_2(0x12e8, 0xffff),    //Dummy setting

#if ENABLE_MEM1300MHz
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d0e, 0x00a5),    //dqsm_rst = 0
#else
{CHIPCFG_CMD_BASE, 0xbf22},
_RV32_2(0x0d0e, 0x00a7),    //Read DQSM 0/1 delay (miu0)
#endif
{CHIPCFG_CMD_WAIT, 10}, //wait  1

//------------------------------
// unmask all
//------------------------------
{CHIPCFG_CMD_BASE, 0xbf20},
_RV32_2(0x121a, 0x0001),
_RV32_2(0x1246, 0x0000),
_RV32_2(0x1266, 0x0000),
_RV32_2(0x1286, 0x0000),
_RV32_2(0x12a6, 0x0000),
_RV32_2(0x121e, 0x0c08),
//=================================================================
// End of MIU init !!!
//=================================================================
