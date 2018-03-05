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

//EVDPLL Power on
WREG_B((REG_ADDR_BASE + (0x110988 <<1)) , 0x11),
WREG_B((REG_ADDR_BASE + ((0x110982 <<1) + 1)) , 0x00),

//MIU128PLL Power on
WREG_B((REG_ADDR_BASE + (0x110968 <<1)) , 0x11),
WREG_B((REG_ADDR_BASE + ((0x110962 <<1) + 1)) , 0x00),

//MIU256PLL Power on
WREG_B((REG_ADDR_BASE + (0x110976 <<1)) , 0x19),
WREG_B((REG_ADDR_BASE + (0x110978 <<1)) , 0x11),
WREG_B((REG_ADDR_BASE + ((0x110972 <<1) + 1)) , 0x00),

//DACPLL
WREG ((REG_ADDR_BASE + (0x12112c <<1 )) , 0x5101),
WREG_B ((REG_ADDR_BASE + ((0x12112e <<1) + 1)) , 0x00),

//dac synth
WREG((REG_ADDR_BASE + (0x121162 <<1 )) , 0x0000),
WREG((REG_ADDR_BASE + (0x121164 <<1 )) , 0x0040),
WREG((REG_ADDR_BASE + (0x12116a <<1 )) , 0x0000),
WREG((REG_ADDR_BASE + (0x121166 <<1 )) , 0x0000),
WREG((REG_ADDR_BASE + (0x121168 <<1 )) , 0x0000),
//WREG((REG_ADDR_BASE + (0x101a00 <<1 )) , 0x0020),

// GMACPLL
WREG_B((REG_ADDR_BASE + (0x100bc6 <<1)) , 0x00),
WREG_B((REG_ADDR_BASE + (0x110cd2 <<1)) , 0x1b),
WREG_B((REG_ADDR_BASE + (0x110cc6 <<1)) , 0x00),

//UPLL0
WREG_B ((REG_ADDR_BASE + (0x100800 <<1)) , 0xc0),
WREG_B ((REG_ADDR_BASE + (0x10080e <<1)) , 0x01),
WREG ((REG_ADDR_BASE + (0x100804 <<1)) , 0x0110),

//UPLL1
WREG_B ((REG_ADDR_BASE + (0x100820 <<1)) , 0xc0),
WREG_B ((REG_ADDR_BASE + (0x10082e <<1)) , 0x01),
WREG ((REG_ADDR_BASE + (0x100824 <<1)) , 0x0110),

//UTMI0
WREG ((REG_ADDR_BASE + (0x103a88 <<1)) , 0x040f),
WREG ((REG_ADDR_BASE + (0x103a80 <<1)) , 0x0001),

//UTMI1
WREG ((REG_ADDR_BASE + (0x103a08 <<1)) , 0x040f),
WREG ((REG_ADDR_BASE + (0x103a00 <<1)) , 0x0001),

//UTMI2
WREG ((REG_ADDR_BASE + (0x103908 <<1)) , 0x040f),
WREG ((REG_ADDR_BASE + (0x103900 <<1)) , 0x0001),

//[GPUPLL Synth setting as 550MHz]
WREG ((REG_ADDR_BASE + (0x1604c0 <<1)) , 0x147a),
WREG ((REG_ADDR_BASE + (0x1604c2 <<1)) , 0x002e),
WREG_B ((REG_ADDR_BASE + (0x1604c4 <<1)) , 0x01),

// power on mpll
WREG_B ((REG_ADDR_BASE + ((0x110b02 <<1) + 1)) , 0x00),

// power on g3d pll
WREG_B ((REG_ADDR_BASE + ((0x160422 <<1) + 1)) , 0x00),

//=======================================================================================================
// Enable UTMI PLL clock
//=======================================================================================================
WREG        ((REG_ADDR_BASE + (0x100800 << 1)), 0x00c0),  //enable UPLL0 clock
WREG_B      ((REG_ADDR_BASE + (0x10080E << 1)), 0x01),    //enable 384MHz clk from UPLL0
WREG        ((REG_ADDR_BASE + (0x100804 << 1)), 0x0110),  //UPLL0

//================================================================================================
                                                          //initial UPLL1 & UTMISS0 & UTMISS1
WREG        ((REG_ADDR_BASE + (0x100820 << 1)), 0x00c0),  //enable UPLL1 clock
//WREG_B      ((REG_ADDR_BASE + (0x10082E << 1)), 0x01),  //enable 384MHz clk from UPLL1
WREG        ((REG_ADDR_BASE + (0x100824 << 1)), 0x0110),  //UPLL1

WREG        ((REG_ADDR_BASE + (0x103A88 << 1)), 0x040F),  // UTMI0
WREG        ((REG_ADDR_BASE + (0x103A80 << 1)), 0x7F05),
WREG        ((REG_ADDR_BASE + (0x103A08 << 1)), 0x040F),  // UTMI1
WREG        ((REG_ADDR_BASE + (0x103A00 << 1)), 0x7F05),
WREG        ((REG_ADDR_BASE + (0x103908 << 1)), 0x040F),  // UTMI2
WREG        ((REG_ADDR_BASE + (0x103900 << 1)), 0x7F05),

// initial USBPLL ===================================================================================
                                                          //Module: usb0 (0x100700)
WREG_B      ((REG_ADDR_BASE + (0x100700 << 1)), 0x0a),    //Disable MAC initial suspend, Reset UHC
WREG_B      ((REG_ADDR_BASE + (0x100700 << 1)), 0x28),    //Release UHC reset, enable UHC and OTG XIU function

                                                          //Module: utmi0 (0x103A80)
WREG        ((REG_ADDR_BASE + (0x103AA2 << 1)), 0x2088),  //PLL_TEST[30:28]: 3'b101 for IBIAS current select
WREG        ((REG_ADDR_BASE + (0x103AA0 << 1)), 0x8051),  //PLL_TEST[15]: Bypass 480MHz clock divider
WREG        ((REG_ADDR_BASE + (0x103A82 << 1)), 0x2084),  //Enable CLK12_SEL bit <2> for select low voltage crystal clock
WREG        ((REG_ADDR_BASE + (0x103A88 << 1)), 0x0426),  //bit<7>: Power down UTMI port-0 bandgap current

WREG        ((REG_ADDR_BASE + (0x103A80 << 1)), 0x6bc3),  //reg_pdn: bit<15>, bit <2> ref_pdn # Turn on reference voltage and regulator
{CHIPCFG_CMD_WAIT, 1},

WREG        ((REG_ADDR_BASE + (0x103A80 << 1)), 0x69c3),  //Turn on UPLL, reg_pdn: bit<9>
{CHIPCFG_CMD_WAIT, 2},

WREG        ((REG_ADDR_BASE + (0x103A80 << 1)), 0x0001),  //Turn all (including hs_current) use override mode

//================================================================================================
                                                          //Module: usb1 (0x100780)
WREG_B      ((REG_ADDR_BASE + (0x100780 << 1)), 0x0a),    //Disable MAC initial suspend, Reset UHC
WREG_B      ((REG_ADDR_BASE + (0x100780 << 1)), 0x28),    //Release UHC reset, enable UHC and OTG XIU function

                                                          //Module: utmi1 (0x103A00)
WREG        ((REG_ADDR_BASE + (0x103A22 << 1)), 0x2088),  //PLL_TEST[30:28]: 3'b101 for IBIAS current select
WREG        ((REG_ADDR_BASE + (0x103A20 << 1)), 0x8051),  //PLL_TEST[15]: Bypass 480MHz clock divider
WREG        ((REG_ADDR_BASE + (0x103A02 << 1)), 0x2084),  //Enable CLK12_SEL bit <2> for select low voltage crystal clock
WREG        ((REG_ADDR_BASE + (0x103A08 << 1)), 0x0426),  //bit<7>: Power down UTMI port-0 bandgap current

WREG        ((REG_ADDR_BASE + (0x103A00 << 1)), 0x6bc3),  //reg_pdn: bit<15>, bit <2> ref_pdn  # Turn on reference voltage and regulator
{CHIPCFG_CMD_WAIT, 1},

WREG        ((REG_ADDR_BASE + (0x103A00 << 1)), 0x69c3),  //Turn on UPLL, reg_pdn: bit<9>
{CHIPCFG_CMD_WAIT, 2},

WREG        ((REG_ADDR_BASE + (0x103A00 << 1)), 0x0001),  //Turn all (including hs_current) use override mode

//================================================================================================
                                                          //Module: usb2 (0x110200)
WREG_B      ((REG_ADDR_BASE + (0x110200 << 1)), 0x0a),    //Disable MAC initial suspend, Reset UHC
WREG_B      ((REG_ADDR_BASE + (0x110200 << 1)), 0x28),    //Release UHC reset, enable UHC and OTG XIU function

                                                          //Module: utmi2 (0x103900)
WREG        ((REG_ADDR_BASE + (0x103922 << 1)), 0x2088),  //PLL_TEST[30:28]: 3'b101 for IBIAS current select
WREG        ((REG_ADDR_BASE + (0x103920 << 1)), 0x8051),  //PLL_TEST[15]: Bypass 480MHz clock divider
WREG        ((REG_ADDR_BASE + (0x103902 << 1)), 0x2084),  //Enable CLK12_SEL bit <2> for select low voltage crystal clock
WREG        ((REG_ADDR_BASE + (0x103908 << 1)), 0x0426),  //bit<7>: Power down UTMI port-0 bandgap current

WREG        ((REG_ADDR_BASE + (0x103900 << 1)), 0x6bc3),  //reg_pdn: bit<15>, bit <2> ref_pdn  # Turn on reference voltage and regulator
{CHIPCFG_CMD_WAIT, 1},

WREG        ((REG_ADDR_BASE + (0x103900 << 1)), 0x69c3),  //Turn on UPLL, reg_pdn: bit<9>
{CHIPCFG_CMD_WAIT, 2},

WREG        ((REG_ADDR_BASE + (0x103900 << 1)), 0x0001),  //Turn all (including hs_current) use override mode
