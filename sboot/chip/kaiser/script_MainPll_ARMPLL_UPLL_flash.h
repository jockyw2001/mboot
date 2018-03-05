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

// turn on other PLL
// swch 4
//=======================================================================================================
// DSPPLL
//=======================================================================================================
WREG    ((REG_ADDR_BASE + (0x110C80<<1)), 0x0003),      // Enable PLL, 360 MHz
                            // RIU_W((`RIUBASE_ANA_MISC_DSP>>1)+7'h01, 2'b11, 16'h3c00);     @[15:8] loop_div_second(LDS), default 0x3c = 360 MHz
                                                                    //clock = 24*LDS/4

WREG    ((REG_ADDR_BASE + (0x110C82<<1)), 0x3a00),      //clock = 24*58/4 = 348 MHz

WREG ((REG_ADDR_BASE + (0x103380<<1)), 0x0017),
//=======================================================================================================
// HDMIPLL (432 MHz)
//=======================================================================================================
WREG    ((REG_ADDR_BASE + (0x110CA0<<1)), 0x001a),      //MPLL_PD=0 ; MPLL_CLK_ADC_VCO_DIV2_PD=0

// TXPLL (216Mhz for 480p)
WREG ((REG_ADDR_BASE + (0x218A <<1 )) , 0x0001),      // power on synth pll
WREG ((REG_ADDR_BASE + (0x2166 <<1 )) , 0x0080),      // power on tx pll
WREG ((REG_ADDR_BASE + (0x2154 <<1 )) , 0x0000),      // power on tm atop
WREG ((REG_ADDR_BASE + (0x215C <<1 )) , 0x000f),      // enable tm atop

WREG ((REG_ADDR_BASE + (0x2156 <<1 )) , 0x0000),      // power on tm atop on tm_reg
WREG ((REG_ADDR_BASE + (0x2158 <<1 )) , 0x0000),      // power on tm atop on tm_reg

WREG ((REG_ADDR_BASE + (0x21B0 <<1 )) , 0x0002),      // Enable TM FIFO

// hdmitx synthesizer

WREG_B ((REG_ADDR_BASE + (0x11303C <<1 )), 0xff),  // enable_clk_432mhz_hdmitx_synth
                    //`RIU_W  ((`RIUBASE_HDMITX_MISC>>1)+7'h10, 2'b11, Nf_reg[15:0]);
                    //`RIU_W  ((`RIUBASE_HDMITX_MISC>>1)+7'h11, 2'b11, Nf_reg[31:16]);

WREG ((REG_ADDR_BASE + (0x113020 <<1 )) , 0x0a21),      

WREG ((REG_ADDR_BASE + (0x113022 <<1 )) , 0x2546), 

{CHIPCFG_CMD_WAIT, 2},                  // delay 2ms

//  software reset synthesizer

WREG ((REG_ADDR_BASE + (0x113000 <<1 )) , 0x0080),      

WREG ((REG_ADDR_BASE + (0x113000<<1 )) , 0x0000), 

//  DR/DC/DAC divider
//  RIU_W  ((`RIUBASE_HDMITX_PM>>1)+7'h38, 2'b11, {dac_div[2:0], dr_div[4:0], loop_div[3:0], tx_div[1:0], regsel[1:0]});
WREG ((REG_ADDR_BASE + (0x2170<<1 )) , 0x6060), 

//  RIU_W  ((`RIUBASE_HDMITX_PM>>1)+7'h34, 2'b11, test[15:0]);  @  power on synth pll
WREG ((REG_ADDR_BASE + (0x2168<<1 )) , 0x0010), 
WREG ((REG_ADDR_BASE + (0x1151F2<<1 )) , 0x0000), 
WREG ((REG_ADDR_BASE + (0x2166<<1 )) , 0x0030), 

WREG ((REG_ADDR_BASE + (0x103380<<1 )) , 0x0018), 

// DAC PLL
WREG_B ((REG_ADDR_BASE + (0x110C20 <<1 )) , 0x04),      //MIPSPLL_OUTPUT_DIV_FIRST[1:0]=2'b01, [4]PD=0
                                                        //RIU_W( (`RIUBASE_ANA_MISC_DAC>>1) + 7'h01, 2'b11, 16'h4000);  @ clock = input*0x40/8
WREG ((REG_ADDR_BASE + (0x103380 <<1 )) , 0x0019),      //disable reset & power down



//=======================================================================================================
// GMACPLL (250 MHz)
//=======================================================================================================

//Enable GMACPLL, output freuqency is 250MHz
WREG ((REG_ADDR_BASE + (0x100BC6<<1 )) , 0x0000),      //reg_gmac_gate = 0
WREG ((REG_ADDR_BASE + (0x110C68<<1 )) , 0x8008),      //[15] 1 ssc_en, [14] 0 ssc_mode, [13:12] 00 ddft, [9:0] step 10'h008
                                                       //1000 0000 0000 1000
WREG   ((REG_ADDR_BASE + (0x110C6A<<1)), 0x0683),      // [13:0] span 14'h0683
WREG   ((REG_ADDR_BASE + (0x110C70<<1)), 0xbc6a),      // [15:0] ddfset 16'hbc6a
WREG   ((REG_ADDR_BASE + (0x110C72<<1)), 0x0014),      // [23:16] ddfset 8'h14
WREG   ((REG_ADDR_BASE + (0x110C60<<1)), 0x0019),
WREG   ((REG_ADDR_BASE + (0x110C66<<1)), 0x0007),      // turn on GMACPLL

//=======================================================================================================
// Enable UTMI PLL clock
//=======================================================================================================
 
//Module: usb0 (0x1007)
WREG_B  ((REG_ADDR_BASE + (0x100700<<1)), 0x0a),      //Disable MAC initial suspend, Reset UHC
WREG_B  ((REG_ADDR_BASE + (0x100700<<1)), 0x28),      //elease UHC reset, enable UHC and OTG XIU function

//Module: utmi (0x103A)
WREG    ((REG_ADDR_BASE + (0x103AA2<<1)), 0x2088),      // PLL_TEST[30:28]: 3'b010 for IBIAS current select
                             // PLL_TEST[23] CLK480 to digital output source selection
WREG    ((REG_ADDR_BASE + (0x103AA0<<1)), 0x8051),      //  PLL_TEST[15]: Bypass 480MHz clock divider
                             // PLL_TEST[7:4]: 5'b0101_0 for 1.0x
                             // PLL_TEST[0]: 1: synthesizer clock, 2'b11, 0: XIN_C

WREG     ((REG_ADDR_BASE + (0x103A82<<1)), 0x2084),      // Enable CLK12_SEL bit <2> for select low voltage crystal clock

//     WREG    0x1F207510, 0x0000      @ #7=0 Enable band-gap current  #3=0 Disable force_pll_on
//     0x0007 for Edison

WREG     ((REG_ADDR_BASE + (0x103A88<<1)), 0x0007),      //  bit <0:1> defulat 2'b11, bit<2> XTAL enable
WREG     ((REG_ADDR_BASE + (0x103A80<<1)), 0x6bc3),      // reg_pdn: bit<15>, bit <2> ref_pdn # Turn on reference voltage and regulator
{CHIPCFG_CMD_WAIT, 1},                  // delay 1ms


WREG     ((REG_ADDR_BASE + (0x103A80<<1)), 0x69c3),      //  Turn on UPLL, reg_pdn: bit<9>

{CHIPCFG_CMD_WAIT, 2},                  // delay 2ms


WREG     ((REG_ADDR_BASE + (0x103A80<<1)), 0x0001),      //  Turn all (including hs_current) use override mode

//Module: usb1 (0x1007)
WREG_B  ((REG_ADDR_BASE + (0x100780<<1)), 0x0a),      // Disable MAC initial suspend, Reset UHC
WREG_B  ((REG_ADDR_BASE + (0x100780<<1)), 0x28),      //  Release UHC reset, enable UHC and OTG XIU function

//Module: utmi1 (0x103A)
WREG     ((REG_ADDR_BASE + (0x103A22<<1)), 0x2088),      //  PLL_TEST[30:28]: 3'b010 for IBIAS current select
                             // PLL_TEST[23] CLK480 to digital output source selection
WREG     ((REG_ADDR_BASE + (0x103A20<<1)), 0x8051),      //   PLL_TEST[15]: Bypass 480MHz clock divider
                             // PLL_TEST[7:4]: 5'b0101_0 for 1.0x
                             // PLL_TEST[0]: 1: synthesizer clock, 2'b11, 0: XIN_C

WREG     ((REG_ADDR_BASE + (0x103A02<<1)), 0x2084),      //   Enable CLK12_SEL bit <2> for select low voltage crystal clock
//     WREG    0x1F207410, 0x0000      @ Enable band-gap current
// modify to 0x0007 for Edison
WREG     ((REG_ADDR_BASE + (0x103A08<<1)), 0x0007),      //   bit <0:1> defulat 2'b11, bit<2> XTAL enable
WREG     ((REG_ADDR_BASE + (0x103A00<<1)), 0x6bc3),      //  reg_pdn: bit<15>, bit <2> ref_pdn  # Turn on reference voltage and regulator
{CHIPCFG_CMD_WAIT, 1},                  // delay 1ms


WREG     ((REG_ADDR_BASE + (0x103A00<<1)), 0x69c3),      //   Turn on UPLL, reg_pdn: bit<9>
{CHIPCFG_CMD_WAIT, 2},                  // delay 2ms


WREG     ((REG_ADDR_BASE + (0x103A00<<1)), 0x0001),      //   Turn all (including hs_current) use override mode

//USBC2 <--> UTMI2
WREG_B  ((REG_ADDR_BASE + (0x110200<<1)), 0x0a),      //   Disable MAC initial suspend, Reset UHC
WREG_B  ((REG_ADDR_BASE + (0x110200<<1)), 0x28),      //   Release UHC reset, enable UHC and OTG XIU function

WREG     ((REG_ADDR_BASE + (0x102A22<<1)), 0x2088),      //   Set PLL_TEST[23:21] for enable 480MHz clock
WREG     ((REG_ADDR_BASE + (0x102A20<<1)), 0x8051),      //  
WREG     ((REG_ADDR_BASE + (0x102A02<<1)), 0x2084),      //   Enable CLK12_SEL bit <2> for select low voltage crystal clock
//     WREG    0x1F205410, 0x0000       @ Enable band-gap current
// modify 0x0007 for Edison
WREG     ((REG_ADDR_BASE + (0x102A08<<1)), 0x0007),      //   bit <0:1> defulat 2'b11, bit<2> XTAL enable
WREG     ((REG_ADDR_BASE + (0x102A00<<1)), 0x6bc3),      //   reg_pdn: bit<15>, bit <2> ref_pdn  # Turn on reference voltage and regulator
{CHIPCFG_CMD_WAIT, 1},                  // delay 1ms


WREG     ((REG_ADDR_BASE + (0x102A00<<1)), 0x69c3),      //   Turn on UPLL, reg_pdn: bit<9>
{CHIPCFG_CMD_WAIT, 2},                  // delay 2ms


WREG     ((REG_ADDR_BASE + (0x102A00<<1)), 0x0001),      //   Turn all (including hs_current) use override mode


//Module: USB30 UTMI
WREG     ((REG_ADDR_BASE + (0x122B22<<1)), 0x2088),      //   Set PLL_TEST[23:21] for enable 480MHz clock
WREG     ((REG_ADDR_BASE + (0x122B20<<1)), 0x8051),
WREG     ((REG_ADDR_BASE + (0x122B02<<1)), 0x2084),      //   Enable CLK12_SEL bit <2> for select low voltage crystal clock
//     WREG    0x1F245610, 0x0000       @ Enable band-gap current
WREG     ((REG_ADDR_BASE + (0x122B08<<1)), 0x0007),      //   bit <0:1> defulat 2'b11, bit<2> XTAL enable
WREG     ((REG_ADDR_BASE + (0x122B00<<1)), 0x6bc3),      //   reg_pdn: bit<15>, bit <2> ref_pdn  # Turn on reference voltage and regulator
{CHIPCFG_CMD_WAIT, 1},                  // delay 1ms


WREG     ((REG_ADDR_BASE + (0x122B00<<1)), 0x69c3),      //  Turn on UPLL, reg_pdn: bit<9>
{CHIPCFG_CMD_WAIT, 2},                  // delay 2ms


WREG     ((REG_ADDR_BASE + (0x122B00<<1)), 0x0001),      //  Turn all (including hs_current) use override mode

 //

