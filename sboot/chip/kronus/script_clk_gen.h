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
// swch 4

WREG(REG_ADDR_BASE + (0x100b28 << 1), 0x0000), // clk_uart1 = 172M (step_1/2)
                                               // clk_uart2 = 172M (step_1/2)
WREG(REG_ADDR_BASE + (0x100b26 << 1), 0x0c06), // clk_uart0 = 123M
                                               // clk_uart1 = 172M (step_2/2)
                                               // clk_uart2 = 172M (step_2/2)

WREG(REG_ADDR_BASE + (0x100b2c << 1), 0x0010), // clk_spi = 54M (step_1/2)
WREG(REG_ADDR_BASE + (0x100b2c << 1), 0x0030), // clk_spi = 54M (step_2/2)

WREG(REG_ADDR_BASE + (0x100b2e << 1), 0x0100), // clk_fuart = 54M
                                               // uart synthesizer enable

// PCM & TCK
WREG(REG_ADDR_BASE + (0x100b34 << 1), 0x0000), // clk_pcm = 27M
                                               // clk_tck = CLK_TCK_IN

// MIU
#if ((ENABLE_MEM1066MHz == 1)||(ENABLE_MEM400MHz == 1)||(ENABLE_MEM333MHz == 1))
WREG(REG_ADDR_BASE + (0x100b3c << 1), 0x0102), // clk_mpll_syn = 432M (for VEPLL  0: 216M, 1:432M                                    // reg_ckg_mempll => select mempll0_clk05x
WREG(REG_ADDR_BASE + (0x100b3e << 1), 0x0000), // clk_miu = MEMPLL out, DDR2 1/4 mode -> 1066/4 = 266
#else
WREG(REG_ADDR_BASE + (0x100b3c << 1), 0x0100), // clk_mpll_syn = 432M (for VEPLL  0: 216M, 1:432M)
#if (ENABLE_KRONUS_U03 == 1)
WREG(REG_ADDR_BASE + (0x100b3e << 1), 0x0010), // clk_miu = 240M
#elif (ENABLE_KRONUS_U04 == 1)
WREG(REG_ADDR_BASE + (0x100b3e << 1), 0x0018), // clk_miu = 288M
#endif
#endif
WREG(REG_ADDR_BASE + (0x100bfc << 1), 0x0001), // clk_ddr_syn = 432M (for MEMPLL  0: 216M, 1:432M)

// VE
WREG(REG_ADDR_BASE + (0x100b48 << 1), 0x0800), // clk_ve = 27M
                                               // clk_vedac = 108M
WREG(REG_ADDR_BASE + (0x100b4a << 1), 0x001c), // clk_ve_in = 27M (clk_odclk)

// TSP
WREG(REG_ADDR_BASE + (0x101e04 << 1), 0x0001), // CLK_TS0_IN from PAD_TS0_CLK
WREG(REG_ADDR_BASE + (0x100b50 << 1), 0x0000), // clk_ts0 = CLK_TS0_IN
                                               // clk_ts1 = CLK_TS0_IN
WREG(REG_ADDR_BASE + (0x100b54 << 1), 0x0000), // clk_tsp = 144M
                                               // clk_stc0 = 27M (STC0_SYNTH_OUT)

// VD_MHEG5
WREG(REG_ADDR_BASE + (0x100b60 << 1), 0x0000), // ckg_vd_mheg5 = 160M


#if ((ENABLE_MSTAR_KRONUS_BD_MST124F_D01A_S == 1) || (ENABLE__MSTAR_KRONUS_BD_MST124F_SZ_Demo == 1)|| (ENABLE__MSTAR_KRONUS_BD_MST124F_D02A == 1)) // NO HVD / NO JPD
WREG(REG_ADDR_BASE + (0x100b62 << 1), 0x0001),
WREG(REG_ADDR_BASE + (0x100b6a << 1), 0x0001),
#else
// HVD
WREG(REG_ADDR_BASE + (0x100b62 << 1), 0x0004), // ckg_HVD = 160M
// JPD
WREG(REG_ADDR_BASE + (0x100b6a << 1), 0x0008), // ckg_jpg = 144M (0:72m 4:108m 8:144m c:192m)
#endif

// MVD
WREG(REG_ADDR_BASE + (0x100b70 << 1), 0x0000), // mvd_syn off

WREG(REG_ADDR_BASE + (0x100b72 << 1), 0x0400), // clk_mvd = 144M
                                               // clk_mvd2 = 144M

WREG(REG_ADDR_BASE + (0x100b74 << 1), 0x0000), // clk_mvd_chroma on
                                               // clk_mvd_luma_a on
WREG(REG_ADDR_BASE + (0x100b76 << 1), 0x0000), // clk_mvd_luma_b on
                                               // clk_mvd_luma_c on
WREG(REG_ADDR_BASE + (0x100b78 << 1), 0x0000), // clk_mvd_rmem on
                                               // clk_mvd_rmem1 on

// GOP
WREG(REG_ADDR_BASE + (0x100b80 << 1), 0x0000), // clk_gopg0 = 27M (clk_odclk)
                                               // clk_gopg1 = 27M (clk_odclk)
WREG(REG_ADDR_BASE + (0x100b82 << 1), 0x0018), // clk_gopmix = 27M (clk_ve)
                                               // clk_gopd = 27M (clk_odclk)
WREG(REG_ADDR_BASE + (0x100b84 << 1), 0x0004), // clk_gopg2 = 192M (clk_idclk)
                                               // clk_gopg3 = 27M (clk_odclk)

WREG(REG_ADDR_BASE + (0x100b86 << 1), 0x0000), // clk_psram0 on

// GE
WREG(REG_ADDR_BASE + (0x100b90 << 1), 0x0000), // clk_ge = 170M

// STB_DC
#if ((ENABLE_MSTAR_KRONUS_BD_MST124F_D01A_S == 1) || (ENABLE_MSTAR_KRONUS_BD_MST124F_SZ_Demo == 1)|| (ENABLE__MSTAR_KRONUS_BD_MST124F_D02A == 1))
WREG(REG_ADDR_BASE + (0x100b98 << 1), 0x0100), // clk_dc0 = 192M
#else
WREG(REG_ADDR_BASE + (0x100b98 << 1), 0x0018), // clk_dc0 = 192M
#endif
// SC
#if ((ENABLE_MSTAR_KRONUS_BD_MST124F_D01A_S == 1) || (ENABLE_MSTAR_KRONUS_BD_MST124F_SZ_Demo == 1)|| (ENABLE__MSTAR_KRONUS_BD_MST124F_D02A == 1))
WREG(REG_ADDR_BASE + (0x100ba2 << 1), 0x0001),
#else
WREG(REG_ADDR_BASE + (0x100ba2 << 1), 0x0400), // clk_ficlk = 192M (clk_fclk)
#endif
WREG(REG_ADDR_BASE + (0x100ba4 << 1), 0x1000), // clk_flck = 192M
WREG(REG_ADDR_BASE + (0x100ba6 << 1), 0x000c), // clk_odclk = 27M (CLK_OUT_DAC/8) (output pixel clock)
WREG(REG_ADDR_BASE + (0x100baa << 1), 0x000c), // clk_idclk = 192M (clk_dc0)
WREG(REG_ADDR_BASE + (0x100bb0 << 1), 0x0c00), // clk_hdgen = 27M (clk_odlck)
                                               // clk_hdgen_fir = 27M (CLK_OUT_DAC/8)
WREG(REG_ADDR_BASE + (0x100bb2 << 1), 0x000c), // clk_dac1 = 108M (clk_vedac) (SD)
                                               // clk_dac2 = 27M (clk_hdgen_fir), (HD)
WREG(REG_ADDR_BASE + (0x100bb4 << 1), 0x000c), // clk_dcs = 27M (CLK_OUT_DAC/8)
                                               // clk_dcs27 = 27M

// EMAC
#if ((ENABLE_MSTAR_KRONUS_BD_MST124F_D01A_S == 1) || (ENABLE_MSTAR_KRONUS_BD_MST124F_SZ_Demo == 1)|| (ENABLE__MSTAR_KRONUS_BD_MST124F_D02A == 1))
WREG(REG_ADDR_BASE + (0x100bc0 << 1), 0x0101), // Disable clock
WREG(REG_ADDR_BASE + (0x100bc2 << 1), 0x0101),
WREG(REG_ADDR_BASE + (0x100bc4 << 1), 0x0001),
#else
WREG(REG_ADDR_BASE + (0x100bc0 << 1), 0x0000), // clk_emac_ahb = 123M
                                               // clk_emac_rx = CLK_EMAC_RX_IN
WREG(REG_ADDR_BASE + (0x100bc2 << 1), 0x0000), // clk_emac_tx = CLK_EMAC_TX_IN
                                               // clk_emac_rx_ref = CLK_EMAC_RX_IN
WREG(REG_ADDR_BASE + (0x100bc4 << 1), 0x0000), // clk_emac_tx_ref = CLK_EMAC_TX_IN
#endif

#if ((ENABLE_MSTAR_KRONUS_BD_MST124F_D01A_S == 1) || (ENABLE_MSTAR_KRONUS_BD_MST124F_SZ_Demo == 1)|| (ENABLE__MSTAR_KRONUS_BD_MST124F_D02A == 1))
WREG(REG_ADDR_BASE + (0x100bd0 << 1), 0x0001),
WREG(REG_ADDR_BASE + (0x103350 << 1), 0x0001),  //disable HDMI clk
WREG(REG_ADDR_BASE + (0x10331a << 1), 0x0001),  // disable VIF clk even Kronus no vif...
#else
// MPIF
WREG(REG_ADDR_BASE + (0x100bd0 << 1), 0x0000), // clk_mpif = 123M
// HDMITX
WREG(REG_ADDR_BASE + (0x103350 << 1), 0x0000), // clk_hdmitx = 27M (clk_odclk)
#endif
// MIIC
WREG(REG_ADDR_BASE + (0x103360 << 1), 0x0000), // clk_miic = 24M


