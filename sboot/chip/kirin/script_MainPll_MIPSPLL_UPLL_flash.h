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

// SMC PAD
WREG((REG_ADDR_BASE + (0x102958 <<1 )) , 0x0040   ),//set smc pad power down


// turn on other analog PLLs
// swch 4

// VEPLL
WREG((REG_ADDR_BASE + (0x103100 <<1 )) , 0x0000   ),//disable power reset
WREG((REG_ADDR_BASE + (0x103106 <<1 )) , 0x0001   ),//disable reset & power down

//-------------------------------------------------------------------
// initialize UPLL
//-------------------------------------------------------------------
// UPLL (turn 480Mhz in USB)
// USBC0 <--> UTMI0
WREG_B((REG_ADDR_BASE + (0x100700 <<1 )) , 0x0a    ),// Disable MAC initial suspend, Reset UHC
WREG_B((REG_ADDR_BASE + (0x100700 <<1 )) , 0x28    ),// Release UHC reset, enable UHC and OTG XIU function

WREG((REG_ADDR_BASE + (0x103a88 <<1 )) , 0x040f   ), // Enable bandgap current in usb_xcvr"
WREG((REG_ADDR_BASE + (0x103aa0 <<1 )) , 0x8051   ), // PLL_TEST[15]: Bypass 480MHz clock divider"
                                                   // PLL_TEST[7:4]: 5'b0101_0 for 1.0x"
                                                   // PLL_TEST[0]: 1: synthesizer clock, 0: XIN_C"

WREG((REG_ADDR_BASE + (0x103aa2 <<1 )) , 0x2088   ), // PLL_TEST[30:28]: 3'b101 for IBIAS current select"
                                                   // PLL_TEST[23] CLK480 to digital output source selection"
WREG((REG_ADDR_BASE + (0x103a80 <<1 )) , 0x6BC3   ), // Turn on reference voltage and regulator"
                                                   // reg_pdn: bit<15>, bit <2> ref_pdn"
                                                   
WREG((REG_ADDR_BASE + (0x103a80 <<1 )) , 0x69C3   ), // Turn on UPLL, reg_pdn: bit<9>"
WREG((REG_ADDR_BASE + (0x103a80 <<1 )) , 0x0001   ), // Turn all (including hs_current) use override mode"

// UPLL (turn 480Mhz in USB)
// USBC1 <--> UTMI1
WREG_B((REG_ADDR_BASE + (0x100780 <<1 )) , 0x0a    ),// Disable MAC initial suspend, Reset UHC
WREG_B((REG_ADDR_BASE + (0x100780 <<1 )) , 0x28    ),// Release UHC reset, enable UHC and OTG XIU function

WREG((REG_ADDR_BASE + (0x103a08 <<1 )) , 0x040f   ), // Enable bandgap current in usb_xcvr"
WREG((REG_ADDR_BASE + (0x103a20 <<1 )) , 0x8051   ), // PLL_TEST[15]: Bypass 480MHz clock divider"
                                                   // PLL_TEST[7:4]: 5'b0101_0 for 1.0x"
                                                   // PLL_TEST[0]: 1: synthesizer clock, 0: XIN_C"

WREG((REG_ADDR_BASE + (0x103a22 <<1 )) , 0x2088   ), // PLL_TEST[30:28]: 3'b101 for IBIAS current select"
                                                   // PLL_TEST[23] CLK480 to digital output source selection"
WREG((REG_ADDR_BASE + (0x103a00 <<1 )) , 0x6BC3   ), // Turn on reference voltage and regulator"
                                                   // reg_pdn: bit<15>, bit <2> ref_pdn"
                                                   
WREG((REG_ADDR_BASE + (0x103a00 <<1 )) , 0x69C3   ), // Turn on UPLL, reg_pdn: bit<9>"
WREG((REG_ADDR_BASE + (0x103a00 <<1 )) , 0x0001   ), // Turn all (including hs_current) use override mode"


//--------------------------------------
//         Initialize DACPLL
//--------------------------------------

WREG((REG_ADDR_BASE + (0x110b02 <<1 )) , 0x0001   ),// Set MPLL_432

WREG((REG_ADDR_BASE + (0x12112a <<1 )) , 0x0000   ),//reg_dacpll_pd[0],
WREG((REG_ADDR_BASE + (0x12112c <<1 )) , 0x0616   ),//reg_dacpll_loop_div[11:8], reg_dacpll_dac_mode_en[4]
WREG((REG_ADDR_BASE + (0x12112e <<1 )) , 0x0807   ),//reg_dacpll_dr_div  [4:0], reg_dacpll_pdreg[8], reg_dacpll_in_select[11]

// dac synth
WREG((REG_ADDR_BASE + (0x121162 <<1 )) , 0x9999   ),// [15:0] reg_synth_set_15_0
WREG((REG_ADDR_BASE + (0x121164 <<1 )) , 0x0019   ),// [15:0] reg_synth_set_31_16
WREG((REG_ADDR_BASE + (0x12116a <<1 )) , 0x0002   ),// [0] enable ssc 1: enable, 0:disable
                                                    // [1] ssc_sync
WREG((REG_ADDR_BASE + (0x121166 <<1 )) , 0x0007   ),// step
WREG((REG_ADDR_BASE + (0x121168 <<1 )) , 0x0445   ),// span

