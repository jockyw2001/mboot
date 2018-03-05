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

//<<CLKGEN0>>
WREG ((REG_ADDR_BASE + (0x100b28 <<1 )) , 0x0000),   // clk_uart1 = 172M (step_1/2)
                                                  // clk_uart2 = 172M (step_1/2)
WREG ((REG_ADDR_BASE + (0x100b26 <<1 )) , 0x0c06),   // clk_uart0 = 123M
                                                  // clk_uart1 = 172M (step_2/2)
                                                  // clk_uart2 = 172M (step_2/2)

WREG ((REG_ADDR_BASE + (0x100b2c <<1 )) , 0x0010),   // clk_spi = 54M (step_1/2)
WREG ((REG_ADDR_BASE + (0x100b2c <<1 )) , 0x0030),   // clk_spi = 54M (step_2/2)

WREG ((REG_ADDR_BASE + (0x100b2e <<1 )) , 0x0100),   // clk_fuart = 54M
                                                  // uart synthesizer enable

//PCM & TCK
WREG ((REG_ADDR_BASE + (0x100b34 <<1 )) , 0x0000),   // clk_pcm = 27M
                                                  // clk_tck = CLK_TCK_IN

//MIU
WREG ((REG_ADDR_BASE + (0x100b3c <<1 )) , 0x0100),   // clk_mpll_syn = 432M (for VEPLL  0: 216M, 1:432M)

WREG ((REG_ADDR_BASE + (0x100b3e <<1 )) , 0x0018),   // clk_miu = 288M
                                                  // clk_miu_rec = 93K (clk_xtal/128)
WREG ((REG_ADDR_BASE + (0x100bfc <<1 )) , 0x0001),   // clk_ddr_syn = 432M (for MEMPLL  0: 216M, 1:432M)

//VE
WREG ((REG_ADDR_BASE + (0x100ba6 <<1 )) , 0x000c),   // clk_odclk
WREG ((REG_ADDR_BASE + (0x100b48 <<1 )) , 0x0800),   // clk_ve = 27M
                                                  // clk_vedac = 108M
WREG ((REG_ADDR_BASE + (0x100b4a <<1 )) , 0x001c),   // clk_ve_in = 27M (clk_odclk)

//TSP
WREG ((REG_ADDR_BASE + (0x101e04 <<1 )) , 0x0001),   // CLK_TS0_IN from PAD_TS0_CLK
WREG ((REG_ADDR_BASE + (0x100b50 <<1 )) , 0x0000),   // clk_ts0 = CLK_TS0_IN
                                                  // clk_ts1 = CLK_TS0_IN
WREG ((REG_ADDR_BASE + (0x100b54 <<1 )) , 0x0000),   // clk_tsp = 144M
                                                  // clk_stc0 = 27M (STC0_SYNTH_OUT)
WREG ((REG_ADDR_BASE + (0x100b58 <<1 )) , 0x0101),   // reg_ts1_clk_sample_en = 1
                                                  // reg_ts1_clk_sample_cnt = 1
                                                  // enable TS1 clock delay

//VD_MHEG5
WREG ((REG_ADDR_BASE + (0x100b60 <<1 )) , 0x0000),   // ckg_vd_mheg5 = 160M

//HVD
WREG ((REG_ADDR_BASE + (0x100b62 <<1 )) , 0x0000),   // ckg_HVD = 216M

//JPD
WREG ((REG_ADDR_BASE + (0x100b6a <<1 )) , 0x000c),   // ckg_jpd = 192M (0:72m 4:108m 8:144m c:192m)

//MVD
WREG ((REG_ADDR_BASE + (0x100b70 <<1 )) , 0x0000),   // mvd_syn off

WREG ((REG_ADDR_BASE + (0x100b72 <<1 )) , 0x0400),   // clk_mvd = 144M
                                                  // clk_mvd2 = 175M

WREG ((REG_ADDR_BASE + (0x100b74 <<1 )) , 0x0000),   // clk_mvd_chroma on
                                                  // clk_mvd_luma_a on
WREG ((REG_ADDR_BASE + (0x100b76 <<1 )) , 0x0000),   // clk_mvd_luma_b on
                                                  // clk_mvd_luma_c on
WREG ((REG_ADDR_BASE + (0x100b78 <<1 )) , 0x0000),   // clk_mvd_rmem on
                                                  // clk_mvd_rmem1 on

//GOP
WREG ((REG_ADDR_BASE + (0x100baa <<1 )) , 0x0010),   // clk_idclk_f2
WREG ((REG_ADDR_BASE + (0x100b80 <<1 )) , 0x0000),   // clk_gopg0 = 27M (clk_odclk)
                                                  // clk_gopg1 = 27M (clk_odclk)
WREG ((REG_ADDR_BASE + (0x100b82 <<1 )) , 0x0018),   // clk_gopmix = 27M (clk_odclk)
                                                  // clk_gopd = 27M (clk_odclk)
WREG ((REG_ADDR_BASE + (0x100b84 <<1 )) , 0x0004),   // clk_gopg2 = 27M (clk_idclk)
                                                  // clk_gopg3 = 27M (clk_odclk)
WREG ((REG_ADDR_BASE + (0x100b86 <<1 )) , 0x0000),   // clk_psram0 on

//GE
WREG ((REG_ADDR_BASE + (0x100b90 <<1 )) , 0x0000),   // clk_ge = 216M

//STB_DC
WREG ((REG_ADDR_BASE + (0x100b98 <<1 )) , 0x0018),   // clk_dc0 = 192M

//SC
WREG ((REG_ADDR_BASE + (0x100ba2 <<1 )) , 0x0400),   // clk_ficlk = 192M (clk_fclk)
WREG ((REG_ADDR_BASE + (0x100ba4 <<1 )) , 0x1000),   // clk_flck = 192M
WREG ((REG_ADDR_BASE + (0x100ba6 <<1 )) , 0x000c),   // clk_odclk = 27M (CLK_OUT_DAC/8) (output pixel clock)
WREG ((REG_ADDR_BASE + (0x100baa <<1 )) , 0x0010),   // clk_idclk = 27M (odclk)
WREG ((REG_ADDR_BASE + (0x100bb0 <<1 )) , 0x0c00),   // clk_hdgen = 27M (clk_odlck)
                                                  // clk_hdgen_fir = 27M (CLK_OUT_DAC/8)
WREG ((REG_ADDR_BASE + (0x100bb2 <<1 )) , 0x000c),   // clk_dac1 = 108M (clk_vedac) (SD)
                                                  // clk_dac2 = 27M (clk_hdgen_fir) (HD)
WREG ((REG_ADDR_BASE + (0x100bb4 <<1 )) , 0x000c),   // clk_dcs27 = 27M

//EMAC
WREG ((REG_ADDR_BASE + (0x100bc0 <<1 )) , 0x0000),   // clk_emac_ahb = 123M
                                                  // clk_emac_rx = CLK_EMAC_RX_IN
WREG ((REG_ADDR_BASE + (0x100bc2 <<1 )) , 0x0000),   // clk_emac_tx = CLK_EMAC_TX_IN
                                                  // clk_emac_rx_ref = CLK_EMAC_RX_IN
WREG ((REG_ADDR_BASE + (0x100bc4 <<1 )) , 0x0000),   // clk_emac_tx_ref = CLK_EMAC_TX_IN

//MPIF
WREG ((REG_ADDR_BASE + (0x100bd0 <<1 )) , 0x0024),   // clk_mpif = 160M

//<<CLK_GEN1>>
//DEMOD
//WREG ((REG_ADDR_BASE + (0x103300 <<1 )) , 0x0511),   # parallel mode: 0x0511 / serial mode 0x0400
                          // reg_ckg_dvbtmk_ts_out_mode@0x00

//WREG ((REG_ADDR_BASE + (0x103308 <<1 )) , 0x0000),   # reg_ckg_dvbtc_ts@0x04[11:8] : 8KHz

//WREG ((REG_ADDR_BASE + (0x10330c <<1 )) , 0x0000),   # reg_ckg_dvbtc_inner1x@0x06[0:0] : DMPLLDIV14/2=31MHz
                          // reg_ckg_dvbtc_inner2x@0x06[8:8] : DMPLLDIV14=62MHz

//WREG ((REG_ADDR_BASE + (0x103314 <<1 )) , 0x0000),   # reg_ckg_dvbtc_adc@0x0a[3:0] : ADC_CLK=28.8MHz
                          // reg_ckg_demod_dma@0x0a[11:8] : ADC_CLK

//WREG ((REG_ADDR_BASE + (0x103316 <<1 )) , 0x0000),   # reg_ckg_dvbtc_eq8x@0x0b[0:0] : DMPLLDIV6=144MHz
                          // reg_ckg_dvbtc_eq@0x0b[8:8] : DMPLLDIV6/8=18MHz

//CA
WREG ((REG_ADDR_BASE + (0x10334a <<1 )) , 0x0000),
WREG ((REG_ADDR_BASE + (0x10334a <<1 )) , 0x1100),

//HDMITX
WREG ((REG_ADDR_BASE + (0x103350 <<1 )) , 0x0000),   // clk_hdmitx = 27M (clk_odclk)

//MIIC
WREG ((REG_ADDR_BASE + (0x103360 <<1 )) , 0x0000),   // clk_miic = 24M



