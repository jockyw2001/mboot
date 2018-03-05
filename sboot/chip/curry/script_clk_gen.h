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
//swch 4

//switch miu128 clk_miu to miupll
WREG_B((REG_ADDR_BASE + (0x100b3e <<1)) , 0x08),
WREG_B((REG_ADDR_BASE + ((0x100b3e <<1) + 1)) , 0x00),

/////////////////////////////
//reg_CLKGEN0
/////////////////////////////
//clk_ge_p
WREG_B((REG_ADDR_BASE + (0x100b90 <<1)) , 0x01),

//clk_mvd_p
WREG_B((REG_ADDR_BASE + (0x100b72 <<1)) , 0x01),

//clk_parser_p
WREG_B((REG_ADDR_BASE + ((0x100b72 <<1) + 1)) , 0x04),

//clk_stamp_p
WREG_B((REG_ADDR_BASE + ((0x100b5e <<1) + 1)) , 0x00),

//clk_27m_p
WREG_B((REG_ADDR_BASE + ((0x100b54 <<1) + 1)) , 0x00),

//clk_spi_p1
WREG_B((REG_ADDR_BASE + (0x100b2c <<1)) , 0x18),

//clk_spi_p
WREG_B((REG_ADDR_BASE + (0x100b2c <<1)) , 0x58),

//clk_faurt
WREG_B((REG_ADDR_BASE + (0x100b2e <<1)) , 0x10),

//clk_uart0
WREG_B((REG_ADDR_BASE + ((0x100b26 <<1) + 1)) , 0x0c),

//clk_uart1
WREG_B((REG_ADDR_BASE + (0x100b28 <<1)) , 0x0c),

//clk_mspi0
WREG_B((REG_ADDR_BASE + ((0x100b2c <<1) + 1)) , 0x11),

//clk_mspi1
WREG_B((REG_ADDR_BASE + ((0x100b2c <<1) + 1)) , 0x11),

//clk_bdma
WREG_B((REG_ADDR_BASE + (0x100bcc <<1)) , 0x04),

//clk_mcu_p1
WREG_B((REG_ADDR_BASE + ((0x100b20 <<1) + 1)) , 0x00),

//clk_mcu_p
WREG_B((REG_ADDR_BASE + (0x100b20 <<1)) , 0x01),

//clk_miu_p
WREG_B((REG_ADDR_BASE + (0x100b3e <<1)) , 0x08),

//clk_ddr_syn
WREG_B((REG_ADDR_BASE + (0x100bfa <<1)) , 0x04),

//clk_mpll_syn
WREG_B((REG_ADDR_BASE + (0x100b3a <<1)) , 0x04),

//clk_mpll_syn2
WREG_B((REG_ADDR_BASE + ((0x100b3a <<1) + 1)) , 0x04),

//clk_fuart0_synth_in
WREG_B((REG_ADDR_BASE + (0x100b2e <<1)) , 0x00),

//clk_syn_gmac
WREG_B((REG_ADDR_BASE + (0x100bc6 <<1)) , 0x00),

//clk_r2_secure_p1
WREG_B((REG_ADDR_BASE + (0x100b62 <<1)) , 0x0c),
WREG_B((REG_ADDR_BASE + (0x100b62 <<1)) , 0x20),

//clk_dc0_p
//WREG_B((REG_ADDR_BASE + (0x100b98 <<1)) , 0x00),

//clk_sub_dc0_p
WREG_B((REG_ADDR_BASE + ((0x100bae <<1) + 1)) , 0x00),

//clk_fclk_p
//WREG_B((REG_ADDR_BASE + ((0x100ba4 <<1) + 1)) , 0x00),

//clk_dip_fclk_p
//WREG_B((REG_ADDR_BASE + ((0x100ba8 <<1) + 1)) , 0x00),

//clk_od2ve_p
WREG_B((REG_ADDR_BASE + (0x100ba4 <<1)) , 0x00),

//clk_edclk_f2_p
//WREG_B((REG_ADDR_BASE + ((0x100bb6 <<1) + 1)) , 0x00),

//clk_sc1_edclk_p & clk_sc1_fclk_p
//WREG_B((REG_ADDR_BASE + ((0x100bba <<1) + 1)) , 0x00),

//clk_afbc_p
//WREG_B((REG_ADDR_BASE + (0x100bbe <<1)) , 0x00),

//clk_gop_mix_hd_eng_p
//WREG_B((REG_ADDR_BASE + (0x100ba0 <<1)) , 0x00),

//clk_gop_mix_sd_eng_p
//WREG_B((REG_ADDR_BASE + ((0x100ba0 <<1) + 1)) , 0x00),

//0x24
//[3:0] ve clock : 27M
//[7:] ve 13 clock : 13.5M
//[12:8] ve dac clock
//WREG(REG_ADDR_BASE + (0x100b48 << 1), 0x0300),

//0x25
//ve_in
//WREG(REG_ADDR_BASE + (0x100b4a << 1), 0x000c),

//0x59
//[3:0]dac1 = 0
//[11:8]dac2 = 0
//WREG(REG_ADDR_BASE + (0x100bb2 << 1), 0x0000),  // reg_ckg_dac1@59[0], for CVBS HotPlug detect, ON & select XTAL clock
WREG(REG_ADDR_BASE + (0x100bb2 << 1), 0x0008),    // reg_ckg_dac2@59[8]

//0x5a
//[4:0]hdgen in = reserve ???
//[11:8]dcs27 = 27M
//WREG(REG_ADDR_BASE + (0x100bb4 << 1), 0x0000),

//0x5d
//[3:0]sc1 sidclk f0 = 0 ???
//[11:8]sc1 edclk = 172M
//[15:12]sc1 vclk = 123M
//WREG(REG_ADDR_BASE + (0x100bba << 1), 0x0000),

//0x5e
//[4:0]sc1 sidclk f2 = 0
//[11:8]odclk = 27M
//WREG(REG_ADDR_BASE + (0x100bbc << 1), 0x0000),

/////////////////////////////
//reg_CLKGEN1
/////////////////////////////

//clk_secgmac_ahb
WREG_B((REG_ADDR_BASE + (0x103364 <<1)) , 0x08),

//clk_miic & clk_miic2 & clk_miic3
WREG_B((REG_ADDR_BASE + (0x103360 <<1)) , 0x11),
WREG_B((REG_ADDR_BASE + (0x103360 <<1)) , 0x11),
WREG_B((REG_ADDR_BASE + ((0x103360 <<1) + 1)) , 0x01),

//clk_v9mcu_p
WREG_B((REG_ADDR_BASE + (0x10336a <<1)) , 0x00),

/////////////////////////////
//reg_CLKGEN2
/////////////////////////////

//switch miu256 clk_miu to miupll
WREG_B((REG_ADDR_BASE + (0x100a40 <<1)) , 0x08),
WREG_B((REG_ADDR_BASE + ((0x100a40 <<1) + 1)) , 0x00),

//clk_njpd_p
WREG_B((REG_ADDR_BASE + (0x100a22 <<1)) , 0x01),

//clk_mfe_p
WREG_B((REG_ADDR_BASE + (0x100a20 <<1)) , 0x01),

//clk_zdec_vld_p
WREG_B((REG_ADDR_BASE + (0x100a26 <<1)) , 0x01),

//clk_zdec_lzd_p
WREG_B((REG_ADDR_BASE + (0x100a28 <<1)) , 0x01),

//clk_gmac_ahb
WREG_B((REG_ADDR_BASE + ((0x100a02 <<1) + 1)) , 0x0c),

//clk_miic4 & clk_miic5 & clk_miic6
WREG_B((REG_ADDR_BASE + (0x100a74 <<1)) , 0x01),
WREG_B((REG_ADDR_BASE + (0x100a74 <<1)) , 0x11),
WREG_B((REG_ADDR_BASE + ((0x100a74 <<1) + 1)) , 0x01),

//clk_smart_p
WREG_B((REG_ADDR_BASE + (0x100a24 <<1)) , 0x00),

//dip_gate_en
WREG_B((REG_ADDR_BASE + (0x136c80 <<1)) , 0x80), // dip0_gate_en, 1¡¦h1 enable HW-auto-clock-gated
WREG_B((REG_ADDR_BASE + (0x136cc0 <<1)) , 0x80), // dip1_gate_en, 1¡¦h1 enable HW-auto-clock-gated

