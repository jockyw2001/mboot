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
WREG(REG_ADDR_BASE + (0x100b3e << 1), 0x0008), 

//switch miu256 clk_miu to miupll
WREG(REG_ADDR_BASE + (0x100a40 << 1), 0x0008), 

/////////////////////////////
//reg_CLKGEN0
/////////////////////////////

//BIST Clock = XTAL
//clk_bist change to 216MHz but gated
WREG(REG_ADDR_BASE + (0x100b02 << 1), 0x0001), 

//UART1,2 = xtali ; UART0 = 123MHZ
WREG(REG_ADDR_BASE + (0x100b26 << 1), 0x0C01), 

//UART1,2 = 170MHZ ??(MPLL_DIV_BUF)
WREG(REG_ADDR_BASE + (0x100b28 << 1), 0x0000), 

//clk_spi = 54M
//mspi0 = disable
//mspi1 = disable
//reg_ckg_spi[6:0] / reg_ckg_spi_b0[7] / reg_ckg_mspi0[11:8] / reg_ckg_mspi1[15:12]
WREG(REG_ADDR_BASE + (0x100b2c << 1), 0x1150), 

//FUART clock = 432MHz
//reg_ckg_fuart[3:0]/reg_ckg_fuart0_syn[7:4]/reg_uart_stnthesizer_enable[8]/reg_uart_stnthesizer_sw_rstz[9]
WREG(REG_ADDR_BASE + (0x100b2e << 1), 0x0308),

//PCM Clock = disable
//Default = 0x0001
//reg_ckg_pcm[3:0] / reg_ckg_tck[9:8]
//WREG(REG_ADDR_BASE + (0x100b34 << 1), 0x0000), 

//clk_mpll_syn clock = 432MHZ
//clk_mpll_syn2 clock = 432MHZ
WREG(REG_ADDR_BASE + (0x100b3a << 1), 0x0404), 

//ve clock = disable
//Default = 0x0111
//WREG(REG_ADDR_BASE + (0x100b48 << 1), 0x0300), 

//ve_in clock = disable
//Default = 0x000D
//WREG(REG_ADDR_BASE + (0x100b4a << 1), 0x000c), 

//tsp track clk = disable
//Default = 0x0101
//WREG(REG_ADDR_BASE + (0x100b4e << 1), 0x0000), 

//ts0 clk = disable
//ts1 clk = disable
//Default = 0x0D0D
//WREG(REG_ADDR_BASE + (0x100b50 << 1), 0x000c), 

//ts2 clk = disable
//ts3 clk = disable
//Default = 0x0D0D
//WREG(REG_ADDR_BASE + (0x100b52 << 1), 0x0c0c), 

//clk_tsp = 192M
//syn_stc0 = 432M
WREG(REG_ADDR_BASE + (0x100b54 << 1), 0x0020), 

//tso0_out = disable
//clk_stamp = disable
//Default = 0x010D
//WREG(REG_ADDR_BASE + (0x100b5e << 1), 0x0000), 

//vd_mheg5 = disable
//Default = 0x0301
//WREG(REG_ADDR_BASE + (0x100b60 << 1), 0x0301), 

//[4:0]hvd = disable
//Default = 0x0005
//WREG(REG_ADDR_BASE + (0x100b68 << 1), 0x0001), 

//[3:0]jpd = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100b6a << 1), 0x0000), 

//[3:0]RF = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100b6c << 1), 0x0000), 

//[4:0]mvd = disable
//[11:8]parser = disable
WREG(REG_ADDR_BASE + (0x100b72 << 1), 0x0101), 

//[4:0]evd = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100b7a << 1), 0x0001), 

//[4:0]gpd = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100b7c << 1), 0x0000), 

//[4:0]gopg0 = disable
//[12:8]gopg1 = disable
//Default = 0x0101
//WREG(REG_ADDR_BASE + (0x100b80 << 1), 0x0000), 

//[4:0]gopmix = disable
//[11:8]gopd = disable
//Default = 0x0101
//WREG(REG_ADDR_BASE + (0x100b82 << 1), 0x0000), 

//[4:0]gopg2 = disable
//[12:8]gopg3 = disable
//Default = 0x0101
//WREG(REG_ADDR_BASE + (0x100b84 << 1), 0x0000), 

//[1:0]psram0 = disable
//[3:2]psram1 = disable
//Default = 0x0005
//WREG(REG_ADDR_BASE + (0x100b86 << 1), 0x0000), 

//[3:0]ge = disable
WREG(REG_ADDR_BASE + (0x100b90 << 1), 0x0001), 

//[4:0]dc0 = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100b98 << 1), 0x0000), 

//[3:0]gop mix hd = disable
//[11:8]gop mix sd = disable
//Default = 0x0101
//WREG(REG_ADDR_BASE + (0x100ba0 << 1), 0x0000), 

//[11:8]ficlk f2 = disable
//Default = 0x0100
//WREG(REG_ADDR_BASE + (0x100ba2 << 1), 0x0000), 

//[5:0]od2ve = disable
//[13:8]fclk = disable
//Default = 0x0101
//WREG(REG_ADDR_BASE + (0x100ba4 << 1), 0x0000), 

//[5:0]idclk f1 = disable
//[13:8]idclk f2 = disable
//Default = 0x2121
//WREG(REG_ADDR_BASE + (0x100baa << 1), 0x2020), 

//[5:0]idclk f2 = disable
//Default = 0x0021
//WREG(REG_ADDR_BASE + (0x100bac << 1), 0x0000), 

//[3:0]dc_sram0
//[7:4]dc_sram1
//[12:8]usb_dc0
//[13]idclk1_usr_en
//[14]idclk2_usr_en
//[15]idclk3_usr_en
//Default = 0x0100
//WREG(REG_ADDR_BASE + (0x100bae << 1), 0x0000), 

//[4:0]hdgen = disable
//[12:8]hdgen_filter = disable
//Default = 0x0101
//WREG(REG_ADDR_BASE + (0x100bb0 << 1), 0x0000), 

//reg_ckg_dac1@59[0] => for CVBS HotPlug Detect, ON & select XTAL clock
//reg_ckg_dac2@59[8] => YPbPr HotPlug Detect, ON & select XTAL clock
WREG(REG_ADDR_BASE + (0x100bb2 << 1), 0x0808), 

//[4:0]hdgen in = disable
//[11:8]dcs27 = disable
//Default = 0x0101
//WREG(REG_ADDR_BASE + (0x100bb4 << 1), 0x0000), 

//[5:0]ckg_pdw0
//[13:8]ckg_edclk_f2
//[14]de_only f1
//[15]de_only f2
//Default = 0x0100
//WREG(REG_ADDR_BASE + (0x100bb6 << 1), 0x0000), 

//[3:0]sc1 sidclk f0 = disable
//[11:8]sc1 edclk = disable
//[15:12]sc1 vclk = disable
//Default = 0x1101
//WREG(REG_ADDR_BASE + (0x100bba << 1), 0x0000), 

//[4:0]sc1 sidclk f2 = disable
//[11:8]odclk = disable
//Default = 0x0101
//WREG(REG_ADDR_BASE + (0x100bbc << 1), 0x0000), 

//[10:8]emac rx = clk_emac_rx_in(25MHz)
WREG(REG_ADDR_BASE + (0x100bc0 << 1), 0x0000), 

//[2:0]emac tx = clk_emac_tx_in(25MHz)
//[10:8]emac_rx_ref = ???
WREG(REG_ADDR_BASE + (0x100bc2 << 1), 0x0000), 

//[2:0]emac_tx_ref = ???
WREG(REG_ADDR_BASE + (0x100bc4 << 1), 0x0000), 

//[3:0]sync gmac = 216M
WREG(REG_ADDR_BASE + (0x100bc6 << 1), 0x0000), 

WREG(REG_ADDR_BASE + (0x100bfa << 1), 0x0004), 

WREG(REG_ADDR_BASE + (0x100bcc << 1), 0x0004), 
/////////////////////////////
//reg_CLKGEN1
/////////////////////////////

//[11:8]dvbtc ts = clk_atsc_dvb_div
//WREG(REG_ADDR_BASE + (0x103308 << 1), 0x0000), 

//[3:0]dvbtc adc = clk_demod_adcout
//WREG(REG_ADDR_BASE + (0x103314 << 1), 0x0000), 

//[3:0]vifdbb dac = clk_dmplldiv10
//[11:8]vifdbb vdac = clk_dmplldiv10
//WREG(REG_ADDR_BASE + (0x10331a << 1), 0x0000), 

//[3:0]nsk clk = xtali/2
//[4]clk cftop no disable
WREG(REG_ADDR_BASE + (0x103348 << 1), 0x000c), 

//[3:0]sm ca = 27M/6
//[7:4]sm ca2 = 27M/6
WREG(REG_ADDR_BASE + (0x10334a << 1), 0x0000), 

//[3:0]hdmi tx = clk_hdgen
//[13:8]hdmi tx sw = 0
WREG(REG_ADDR_BASE + (0x103350 << 1), 0x0000), 

//[3:0]miic clk = disable
//[7:4]miic2 clk = disable
//[11:8]miic3 clk = disable
//Default = 0x0111
//WREG(REG_ADDR_BASE + (0x103360 << 1), 0x0444),

//[4:0]secgamc ahb = 123M
WREG(REG_ADDR_BASE + (0x103364 << 1), 0x0000), 

/////////////////////////////
//reg_CLKGEN2
/////////////////////////////

//[4:0] vp9 = 432M
WREG(REG_ADDR_BASE + (0x100a00 << 1), 0x0001), 

//[4:0]ecc = xtal
//[12:8]gmac ahb = 123M
WREG(REG_ADDR_BASE + (0x100a02 << 1), 0x0000), 

//[5:0]rasp = disable
//Default = 0x000D
//WREG(REG_ADDR_BASE + (0x100a04 << 1), 0x0001), 

//[5:0]rasp1 = disable
//Default = 0x000D
//WREG(REG_ADDR_BASE + (0x100a06 << 1), 0x0001), 

//[5:0]emm0 = disable
//Default = 0x000D
//WREG(REG_ADDR_BASE + (0x100a08 << 1), 0x0001), 

//[5:0]emm1 = disable
//Default = 0x000D
//WREG(REG_ADDR_BASE + (0x100a0a << 1), 0x0001), 

//[4:0]tso3 in = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100a10 << 1), 0x0000), 

//[4:0]tso4 in = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100a12 << 1), 0x0000), 

//[4:0]tso5 in = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100a14 << 1), 0x0000), 

//[3:0]mfe = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100a20 << 1), 0x0001), 

//[3:0]njpd = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100a22 << 1), 0x0000), 

//[4:0]smart = 172M
WREG(REG_ADDR_BASE + (0x100a24 << 1), 0x0000), 

//[3:0]zdec vld = disable
//Default = 0x0000
//WREG(REG_ADDR_BASE + (0x100a26 << 1), 0x0001), 

//[3:0]zdec lzd = disable
//Default = 0x0000
//WREG(REG_ADDR_BASE + (0x100a28 << 1), 0x0001), 

//[4:0]evd lite = disable
//[12:8]evd ppu lite = disable
//Default = 0x0101
//WREG(REG_ADDR_BASE + (0x100a2e << 1), 0x0101), 

//[4:0]hvd aec = disable
//[12:8]bist hi codec lite = disable
//Default = 0x1D01
//WREG(REG_ADDR_BASE + (0x100a30 << 1), 0x1c01), 

//[6:0]vd mheg5 = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100a32 << 1), 0x0000), 

//[2:0]hvd idb = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100a34 << 1), 0x0000), 

//[4:0]hvd aec = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100a36 << 1), 0x0001), 

//[4:0]evd ppu = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100a38 << 1), 0x0001), 

//[3:0]vp8 = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100a3a << 1), 0x0001), 

//[3:0]hdr lut = disable
//[7:4]hdr lut sram = disable
//[10:8]xvycc sram = disable
//Default = 0x0111
//WREG(REG_ADDR_BASE + (0x100a4a << 1), 0x0000), 

// Bist Clock for each module
WREG(REG_ADDR_BASE + (0x100a60 << 1), 0x0001), 
WREG(REG_ADDR_BASE + (0x100a62 << 1), 0x0001), 
WREG(REG_ADDR_BASE + (0x100a68 << 1), 0x0001), 
WREG(REG_ADDR_BASE + (0x100a6a << 1), 0x0001), 
WREG(REG_ADDR_BASE + (0x100a6c << 1), 0x0001), 
WREG(REG_ADDR_BASE + (0x100a6e << 1), 0x0001), 
WREG(REG_ADDR_BASE + (0x100a70 << 1), 0x0001), 
WREG(REG_ADDR_BASE + (0x100a72 << 1), 0x0001), 

//[3:0]ts_sampe = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100a76 << 1), 0x0000), 

//[3:0]fcie sync = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100a78 << 1), 0x0000), 

//[4:0]gpu = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100a7a << 1), 0x0000), 

//[6:0]sdio = disable
//Default = 0x0001
//WREG(REG_ADDR_BASE + (0x100a7c << 1), 0x0000), 

//Enable HDMI_RX clock -> for DC_OFF operation, clock will disable at PM code when DC OFF, it need open at here
WREG(REG_ADDR_BASE + (0x000e96 << 1), 0x0000), 

// GOP0/1/2/3 HW auto clock gate enable
WREG_B_MASK(REG_ADDR_BASE + (0x120276 <<1), 0x03, 0x03),
WREG_B_MASK(REG_ADDR_BASE + (0x120576 <<1), 0x03, 0x03),
WREG_B_MASK(REG_ADDR_BASE + (0x120876 <<1), 0x03, 0x03),
WREG_B_MASK(REG_ADDR_BASE + (0x120b76 <<1), 0x03, 0x03),

// dip0_gate_en, dip1_gate_en, more like HW auto clock gate enable.
WREG_B_MASK(REG_ADDR_BASE + (0x136c80 <<1), 0x80, 0x80),
WREG_B_MASK(REG_ADDR_BASE + (0x136cc0 <<1), 0x80, 0x80),

