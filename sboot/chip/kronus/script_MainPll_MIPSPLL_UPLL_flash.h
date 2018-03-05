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

// turn on other analog PLLs
// swch 4


// TXPLL (216Mhz for 480p)
WREG((REG_ADDR_BASE + (0x103350 <<1 )) , 0x0000   ),// enable clk_hdmi_tx_p
WREG((REG_ADDR_BASE + (0x11303c <<1 )) , 0xffff   ),// clkgen enable
WREG((REG_ADDR_BASE + (0x11308a <<1 )) , 0x0000   ),// power on synth pll
#if ((ENABLE_MSTAR_KRONUS_BD_MST124F_D01A_S == 1) || (ENABLE_MSTAR_KRONUS_BD_MST124F_SZ_Demo == 1)|| (ENABLE__MSTAR_KRONUS_BD_MST124F_D02A == 1))
WREG((REG_ADDR_BASE + (0x113066 <<1 )) , 0x01F0   ),// power on tx pll
WREG((REG_ADDR_BASE + (0x113054 <<1 )) , 0x000F   ),// power on tm atop
#else
WREG((REG_ADDR_BASE + (0x113066 <<1 )) , 0x0000   ),// power on tx pll
WREG((REG_ADDR_BASE + (0x113054 <<1 )) , 0x0000   ),// power on tm atop
#endif
WREG((REG_ADDR_BASE + (0x11305c <<1 )) , 0x000f   ),// enable tm atop
#if ((ENABLE_MSTAR_KRONUS_BD_MST124F_D01A_S == 1) || (ENABLE_MSTAR_KRONUS_BD_MST124F_SZ_Demo == 1)|| (ENABLE__MSTAR_KRONUS_BD_MST124F_D02A == 1))
WREG((REG_ADDR_BASE + (0x113056 <<1 )) , 0x0007   ),// power on tm atop on tm_reg
#else
WREG((REG_ADDR_BASE + (0x113056 <<1 )) , 0x0000   ),// power on tm atop on tm_reg
#endif
WREG((REG_ADDR_BASE + (0x113058 <<1 )) , 0x0000   ),// power on tm atop on tm_reg

WREG((REG_ADDR_BASE + (0x113000 <<1 )) , 0x0080   ),// software reset synthesizer
WREG((REG_ADDR_BASE + (0x113000 <<1 )) , 0x0000),
WREG((REG_ADDR_BASE + (0x113100 <<1 )) , 0x0804   ),// [12]: BYPASS_DC_FIFO, [11]: REG_HPLL_LOCK_CHK, [10]: REG_DC_FIFO_RST, [9:8]: START_DE_PH
                          // [7:6]: REG_DC_MODE, [5]: REG_EN_CLAMP, [4]: PKT_MANUAL_MODE, [3]: REG_USER_ENABLE, [2]: REG_EN_PKT, [1]: REG_PHYMODE, [0]: REG_DVIMODE

// VEPLL
WREG((REG_ADDR_BASE + (0x103100 <<1 )) , 0x0000   ),//disable power reset
WREG((REG_ADDR_BASE + (0x103106 <<1 )) , 0x0001   ),//disable reset & power down

// UPLL (turn 480Mhz in USB)
// USBC0 <--> UTMI0
WREG_B((REG_ADDR_BASE + (0x100700 <<1 )) , 0x0a    ),// Disable MAC initial suspend, Reset UHC
WREG_B((REG_ADDR_BASE + (0x100700 <<1 )) , 0x28    ),// Release UHC reset, enable UHC and OTG XIU function

//WREG_B((REG_ADDR_BASE + (0x103aa2 <<1 )) , 0xa0    ),// Set PLL_TEST[23:22] for enable 480MHz clock
//WREG_B((REG_ADDR_BASE + (0x103aa0 <<1 )) , 0x03    ),// UTMI offset 0x20 bit1=1//Set PLL_TEST[1] for PLL multiplier 20X
WREG((REG_ADDR_BASE + (0x103aa2 <<1 )) , 0x00E0    ),
WREG((REG_ADDR_BASE + (0x103aa0 <<1 )) , 0x8003    ),
WREG_B((REG_ADDR_BASE + (0x103aa4 <<1 )) , 0x01    ),

WREG_B((REG_ADDR_BASE + (0x103a82 <<1 )) , 0x84    ),// Enable CLK12_SEL bit <2> for select low voltage crystal clock
WREG_B((REG_ADDR_BASE + (0x103a83 <<1 )-1) , 0x20    ),// Enable CLK12_SEL bit <2> for select low voltage crystal clock
WREG_B((REG_ADDR_BASE + (0x103a89 <<1 )-1) , 0x00    ),// #3=0 Disable force_pll_on
WREG_B((REG_ADDR_BASE + (0x103a88 <<1 )) , 0x00    ),// #7=0 Enable band-gap current
WREG_B((REG_ADDR_BASE + (0x103a81 <<1 )-1) , 0x6b    ),// Turn on reference voltage and regulator
WREG_B((REG_ADDR_BASE + (0x103a80 <<1 )) , 0xc3    ),// reg_pdn: bit<15>, bit <2> ref_pdn
{CHIPCFG_CMD_WAIT, 1},                   // delay 1ms

WREG_B((REG_ADDR_BASE + (0x103a81 <<1 )-1) , 0x69    ),// Turn on UPLL, reg_pdn: bit<9>
{CHIPCFG_CMD_WAIT, 2},                   // delay 1ms

WREG_B((REG_ADDR_BASE + (0x103a80 <<1 )) , 0x01    ),// Turn all (including hs_current) use override mode
WREG_B((REG_ADDR_BASE + (0x103a81 <<1 )-1) , 0x00),
WREG_B((REG_ADDR_BASE + (0x103a83 <<1 )-1) , 0x00    ),//#7=0

WREG_B((REG_ADDR_BASE + (0x103abc <<1 )) , 0x01    ),// set CA_START as 1
WREG_B((REG_ADDR_BASE + (0x103abc <<1 )) , 0x00    ),// release CA_START

#if (ENABLE_MSTAR_KRONUS_BD_MST124I_D01A_S || ENABLE_MSTAR_KRONUS_BD_MST124H_D01A)
WREG((REG_ADDR_BASE + (0x103AAC<<1 )) , 0x0210   ),
WREG((REG_ADDR_BASE + (0x102A2C <<1 )) , 0x0210   ),
#endif

//WREG_B((REG_ADDR_BASE + (0x103aa1 <<1 )-1) , 0x80    // for port0),

// USBC2 <--> UTMI2
WREG_B((REG_ADDR_BASE + (0x110200 <<1 )) , 0x0a    ),// Disable MAC initial suspend, Reset UHC
WREG_B((REG_ADDR_BASE + (0x110200 <<1 )) , 0x68    ),// Release UHC reset, enable UHC and OTG XIU function

//WREG_B((REG_ADDR_BASE + (0x102a22 <<1 )) , 0xe0    ),// Set PLL_TEST[23:21] for enable 480MHz clock
//WREG((REG_ADDR_BASE + (0x102a20 <<1 )) , 0x8003  ),// Set PLL_TEST[1] for PLL multiplier 20X
WREG((REG_ADDR_BASE + (0x102a22 <<1 )) , 0x00e0  ),
WREG((REG_ADDR_BASE + (0x102a20 <<1 )) , 0x8003  ),
WREG_B((REG_ADDR_BASE + (0x102a24 <<1 )) , 0x01  ),

WREG_B((REG_ADDR_BASE + (0x102a02 <<1 )) , 0x84    ),// Enable CLK12_SEL bit <2> for select low voltage crystal clock
WREG_B((REG_ADDR_BASE + (0x102a03 <<1 )-1) , 0x20),
WREG_B((REG_ADDR_BASE + (0x102a09 <<1 )-1) , 0x00    ),// Disable force_pll_on
WREG_B((REG_ADDR_BASE + (0x102a08 <<1 )) , 0x00    ),// Enable band-gap current
WREG_B((REG_ADDR_BASE + (0x102a01 <<1 )-1) , 0x6b    ),// Turn on reference voltage and regulator
WREG_B((REG_ADDR_BASE + (0x102a00 <<1 )) , 0xc3    ),// reg_pdn: bit<15>, bit <2> ref_pdn
{CHIPCFG_CMD_WAIT, 1},                   // delay 1ms

WREG_B((REG_ADDR_BASE + (0x102a01 <<1 )-1) , 0x69    ),// Turn on UPLL, reg_pdn: bit<9>
{CHIPCFG_CMD_WAIT, 2},                   // delay 1ms

WREG_B((REG_ADDR_BASE + (0x102a00 <<1 )) , 0x01    ),// Turn all (including hs_current) use override mode
WREG_B((REG_ADDR_BASE + (0x102a01 <<1 )-1) , 0x00),
WREG_B((REG_ADDR_BASE + (0x102a03 <<1 )-1) , 0x00    ),// #7=0

WREG_B((REG_ADDR_BASE + (0x102a3c <<1 )) , 0x01    ),// set CA_START as 1
WREG_B((REG_ADDR_BASE + (0x102a3c <<1 )) , 0x00    ),// release CA_START

//WREG_B((REG_ADDR_BASE + (0x102a21 <<1 )-1) , 0x80    // for port2),


// Enable greenwich demod clock in KGB
#ifdef ENABLE_GREENWICH_CLK
WREG  ((REG_ADDR_BASE + (0x112810 <<1 )) , 0x0200  ),  // bit 9 enable Greenwich clock
WREG  ((REG_ADDR_BASE + (0x112818 <<1 )) , 0x0005  ),  // Power off AVDD33_ADC 
#endif
