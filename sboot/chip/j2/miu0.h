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
//MEM_PLL relative
// reg_0x101bFC       :   0:fmpll=216Mhz, 1:fmpll=432Mhz
// reg_0x110d32~110d30:   DDFSET
// reg_0x110d36[15,14]:   DDRPLL_LOOP_DIV_1st
// reg_0x110d36[13,12]:   DDRPLL_input_DIV_1st
// reg_0x110d34[7~0]  :   DDRPLL_LOOP_DIV_2nd
// reg_0x110d34[15,8] :   DDRPLL_input_DIV_2nd
//mclk1x = ( fmpll *524288 * DDRPLL_LOOP_DIV_1st * DDRPLL_LOOP_DIV_2nd ) /
//         (ddfset * DDRPLL_input_DIV_1st *DDRPLL_input_DIV_2nd )

//const MS_REG_INIT MIU0_DDR_Init[] =
//{
    WREG16( 0x110d3a, 0x0000 ),  // mclk1x_skew cmd phase /dqsmask0 /mclk /dqsmask1 phase select
    WREG16( 0x110d36, 0x8000 ),  // mclk1x_clkph_ck0 / input div_1st/loop_div_1st
    WREG16( 0x110d34, 0x0300 ),  // loop_div/input_div

    WREG16( 0x110d30, 0x2b52 ),  // ddfset
    WREG16( 0x110d32, 0x0035 ),  // ddrpll_reset/ddrpll_porst/ddrpl_pd

    WREG16( 0x101202, 0x03a3 ),  //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+8banks+10col+4x
    WREG16( 0x101204, 0x000C ),  //if I64Mode =0x8b else =0x0b

    WREG16( 0x101206, 0x0450 ),  //refresh cycle=0x50 ,TURN_ON_SCRAMBLE=0x8450
    WREG16( 0x101208, 0x1cbb ),  //reg_tRCD
    WREG16( 0x10120a, 0x27b6 ),  //reg_tRRD
    WREG16( 0x10120c, 0x95c8 ),  //reg_tWL
    WREG16( 0x10120e, 0x205a ),  //tRFC
    WREG16( 0x101210, 0x1c70 ),  //MR0

#if (ENABLE_MEM1600MHz)
    WREG16( 0x101206, 0x0434 ),  //refresh cycle=0x50 ,TURN_ON_SCRAMBLE=0x8450
    WREG16( 0x101208, 0x1cbb ),  //reg_tRCD
    WREG16( 0x10120a, 0x2766 ),  //reg_tRRD
    WREG16( 0x10120c, 0x96ca ),  //reg_tWL
    WREG16( 0x10120e, 0x4080 ),  //tRFC
    WREG16( 0x101210, 0x1c70 ),  //MR0
#endif

#if (ENABLE_MEM1300MHz)
    WREG16( 0x101206, 0x0434 ),  //refresh cycle=0x50 ,TURN_ON_SCRAMBLE=0x8450
    WREG16( 0x101208, 0x1899 ),  //reg_tRCD
    WREG16( 0x10120a, 0x2155 ),  //reg_tRRD
    WREG16( 0x10120c, 0x95a8 ),  //reg_tWL
    WREG16( 0x10120e, 0x406b ),  //tRFC
    WREG16( 0x101210, 0x1a50 ),  //MR0
#endif

    WREG16( 0x101212, 0x4004 ),  //MR1
    WREG16( 0x101214, 0x8018 ),  //MR2
    WREG16( 0x101216, 0xC000 ),  //MR3

    WREG16( 0x101228, 0x00B0 ),
    WREG16( 0x10122c, 0x6199 ),  // Address Balance Mode

    WREG16( 0x110d00, 0x0401 ),  //miu_0_atop  start
    WREG16( 0x110d02, 0xAAAA ),
    WREG16( 0x110d04, 0x0080 ),
    WREG16( 0x110d06, 0x0000 ),
    WREG16( 0x110d08, 0x003F ),
    WREG16( 0x110d0a, 0x0000 ),
    WREG16( 0x110d0c, 0x0000 ),
    WREG16( 0x110d0e, 0x00e5 ),

    WREG16( 0x110d10, 0x0600 ),
    WREG16( 0x110d12, 0x0000 ),
    WREG16( 0x110d14, 0x0000 ),
    WREG16( 0x110d16, 0x0200 ),
    WREG16( 0x110d18, 0x0000 ),
    WREG16( 0x110d1a, 0x0000 ),
    WREG16( 0x110d1c, 0x0000 ),
    WREG16( 0x110d1e, 0xffff ),
#if (ENABLE_MEM1600MHz)
    WREG16( 0x110d20, 0x0020 ),
    WREG16( 0x110d22, 0x0000 ),
#else
    WREG16( 0x110d20, 0x4020 ),
    WREG16( 0x110d22, 0x0c00 ),
#endif
    WREG16( 0x110d24, 0x0000 ),
    WREG16( 0x110d24, 0x0000 ),
    WREG16( 0x110d26, 0x0000 ),
    WREG16( 0x110d28, 0x0000 ),
    WREG16( 0x110d2a, 0x0000 ),
    WREG16( 0x110d2c, 0x0014 ),
    WREG16( 0x110d2e, 0x0000 ),

    WREG16( 0x110d30, 0x2B52 ),
    WREG16( 0x110d32, 0x0035 ),
    WREG16( 0x110d34, 0x0400 ),
    WREG16( 0x110d36, 0x4003 ),
    WREG16( 0x110d38, 0x3377 ),
    WREG16( 0x110d3a, 0x0000 ),
    WREG16( 0x110d3c, 0x0033 ),

#if (ENABLE_MEM1600MHz)
    WREG16( 0x110d30, 0xEB85 ),
    WREG16( 0x110d32, 0x0019 ),
    WREG16( 0x110d34, 0x0300 ),
    WREG16( 0x110d36, 0x4003 ),
    WREG16( 0x110d38, 0x3377 ),
    WREG16( 0x110d3a, 0x0000 ),
    WREG16( 0x110d3c, 0x0077 ),
#endif

#if (ENABLE_MEM1300MHz)
    WREG16( 0x110d30, 0xe6cb ),
    WREG16( 0x110d32, 0x001f ),
    WREG16( 0x110d34, 0x0300 ),
    WREG16( 0x110d3c, 0x0033 ),
#endif

    WREG16( 0x110d3e, 0x0000 ),
    WREG16( 0x110d40, 0x0000 ),
    WREG16( 0x110d42, 0x0000 ),
    WREG16( 0x110d44, 0x0000 ),
    WREG16( 0x110d46, 0x0000 ),
    WREG16( 0x110d48, 0x0000 ),
    WREG16( 0x110d4a, 0x0000 ),
    WREG16( 0x110d4c, 0x0000 ),
    WREG16( 0x110d4e, 0x0000 ),
    WREG16( 0x110d50, 0x0000 ),
    WREG16( 0x110d52, 0x0000 ),
    WREG16( 0x110d54, 0x0000 ),
    WREG16( 0x110d56, 0x0000 ),
    WREG16( 0x110d58, 0x0000 ),
    WREG16( 0x110d5a, 0x0000 ),
    WREG16( 0x110d5c, 0x3333 ),
    WREG16( 0x110d5e, 0x3333 ),
    WREG16( 0x110d60, 0x0374 ),
#if (ENABLE_MEM1600MHz)
    WREG16( 0x110d62, 0x0032 ),
#else
    WREG16( 0x110d62, 0x0032 ),
#endif
    WREG16( 0x110d64, 0xf200 ),
    WREG16( 0x110d66, 0x0000 ),
    WREG16( 0x110d68, 0x0020 ),
    WREG16( 0x110d6a, 0x0020 ),
    WREG16( 0x110d6c, 0x0000 ),
    WREG16( 0x110d6e, 0x0088 ),
    WREG16( 0x110d72, 0x0008 ),

#if (ENABLE_MEM1600MHz)
    WREG16( 0x110d72, 0x5805 ),
#endif

#if (ENABLE_MEM1300MHz)
    WREG16( 0x110d72, 0x6806 ),
#endif

    WREG16( 0x110d0e, 0x00a5 ),  // 2/2 reg_en_mask clr bit6 eg_dqsm_rst_sel
    WREG16( 0x110d60, 0x0370 ),

//};
