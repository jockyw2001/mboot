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


WREG_B(REG_ADDR_BASE + (0x101e38 << 1), 0x0000),
WREG_B(REG_ADDR_BASE + (0x112840 << 1), 0x0000),
WREG_B(REG_ADDR_BASE + (0x11287a << 1), 0x0001),

#if defined(CONFIG_MSD5S56) && (CONFIG_MSD5S56==1)
WREG(REG_ADDR_BASE + (0x100b20 << 1), 0x0401), // clk_mcu = 172MHz
#else
WREG(REG_ADDR_BASE + (0x100b20 << 1), 0x0001), // clk_mcu = 216MHz
#endif
WREG(REG_ADDR_BASE + (0x100b28 << 1), 0x0000), // clk_uart1 = 172M (step_1/2)
                                               // clk_uart2 = 172M (step_1/2)
WREG(REG_ADDR_BASE + (0x100b26 << 1), 0x0c06), // clk_uart0 = 123M
                                               // clk_uart1 = 172M (step_2/2)


WREG(REG_ADDR_BASE + (0x100b2c << 1), 0x0010), // clk_spi = 54M (step_1/2)
WREG(REG_ADDR_BASE + (0x100b2c << 1), 0x0030), // clk_spi = 54M (step_2/2)

WREG(REG_ADDR_BASE + (0x100b2e << 1), 0x0100), // clk_fuart = 54M
                                               // uart synthesizer enable

// PCM & TCK
WREG(REG_ADDR_BASE + (0x101e08 << 1), 0x0020), // CLK_TCK_IN from PAD_TS1_SYNC
WREG(REG_ADDR_BASE + (0x100b34 << 1), 0x0000), // clk_pcm = 27M
                                               // clk_tck = CLK_TCK_IN

// MIU
WREG(REG_ADDR_BASE + (0x100b3c << 1), 0x0100), // clk_mpll_syn = 432M (for VEPLL  0: 216M, 1:432M)
                                               // clk_mpll_syn2 = 216M
WREG(REG_ADDR_BASE + (0x100bf8 << 1), 0x0100), // clk_fsync = 432M
#if defined(CONFIG_MSD5S56) && (CONFIG_MSD5S56==1)
WREG(REG_ADDR_BASE + (0x100b3e << 1), 0x000C), // clk_miu = 216M
#else
WREG(REG_ADDR_BASE + (0x100b3e << 1), 0x0010), // clk_miu = 266M
#endif
WREG(REG_ADDR_BASE + (0x100bfa << 1), 0x0001), // clk_ddr_syn = 432M (for MEMPLL  0: 216M, 1:432M)

// VE
WREG(REG_ADDR_BASE + (0x100b48 << 1), 0x0800), // clk_ve = 27M
                                               // clk_vedac = 108M


WREG(REG_ADDR_BASE + (0x100b4a << 1), 0x001c), // clk_ve_in = 27M (clk_odclk)
// BDMA
WREG(REG_ADDR_BASE + (0x100bcc << 1), 0x0004), // clk_bdma = clk_miu


// TSP
WREG(REG_ADDR_BASE + (0x101e04 << 1), 0x0001), // CLK_TS0_IN from PAD_TS0_CLK
WREG(REG_ADDR_BASE + (0x100b50 << 1), 0x0400), // clk_ts0 = CLK_TS0_IN
                                               // clk_ts1 = CLK_TS1_IN
WREG(REG_ADDR_BASE + (0x100b52 << 1), 0x0400), // clk_ts2 = CLK_TS0_IN
                                               // clk_ts3 = CLK_TS1_IN
WREG(REG_ADDR_BASE + (0x100b54 << 1), 0x0000), // clk_tsp = 192M
                                               // clk_stc0 = STC0_SYNTH_OUT

// VD_MHEG5
WREG(REG_ADDR_BASE + (0x100b60 << 1), 0x0000), // ckg_vd_mheg5 = 160M



// HVD
#if defined(CONFIG_MSD5S56) && (CONFIG_MSD5S56==1)
WREG(REG_ADDR_BASE + (0x100b68 << 1), 0x0C04), // CLK_HVD=192Mhz
#else
WREG(REG_ADDR_BASE + (0x100b68 << 1), 0x000C), // CLK_HVD=240Mhz
#endif

// JPD
WREG(REG_ADDR_BASE + (0x100b6a << 1), 0x0008), // ckg_jpg = 144M (0:72m 4:108m 8:144m c:192m)


// MVD
WREG(REG_ADDR_BASE + (0x100b70 << 1), 0x0000), // mvd_syn off

#if defined(CONFIG_MSD5S56) && (CONFIG_MSD5S56==1)
WREG(REG_ADDR_BASE + (0x100b72 << 1), 0x0010),  // clk_mvd = 86 Mhz                                         
#else
WREG(REG_ADDR_BASE + (0x100b72 << 1), 0x0000), // clk_mvd = 160Mhz
                                               // clk_parser = 172Mhz
#endif


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
WREG(REG_ADDR_BASE + (0x100b84 << 1), 0x0000), // clk_gopg2 = 27M (clk_odclk)
                                               // clk_gopg3 = 27M (clk_odclk)


//CLK_PSRAM
WREG(REG_ADDR_BASE + (0x100b86 << 1), 0x0000), // clk_psram0 on
                                               // clk_psram1 on
// GE
WREG(REG_ADDR_BASE + (0x100b90 << 1), 0x0000), // clk_ge = clk_miu @kayla

// STB_DC
WREG(REG_ADDR_BASE + (0x100b98 << 1), 0x0010), // clk_dc0 = 123M
// SC
WREG(REG_ADDR_BASE + (0x100ba2 << 1), 0x0400), // clk_ficlk = 192M (clk_fclk)
WREG(REG_ADDR_BASE + (0x100ba4 << 1), 0x1000), // clk_flck = 192M
WREG(REG_ADDR_BASE + (0x100ba6 << 1), 0x000C), // clk_odclk = 27M (CLK_OUT_DAC/8) (output pixel clock)
WREG(REG_ADDR_BASE + (0x100baa << 1), 0x000c), // clk_idclk = 192M (clk_dc0)
WREG(REG_ADDR_BASE + (0x100bb0 << 1), 0x1818), // clk_hdgen = 27M (clk_odlck)
                                               // clk_hdgen_fir = 27M (CLK_OUT_DAC/8)
WREG(REG_ADDR_BASE + (0x100bb2 << 1), 0x000c), // clk_dac1 = 108M (clk_vedac) (SD)
                                               // clk_dac2 = 27M (clk_hdgen_fir), (HD)
WREG(REG_ADDR_BASE + (0x100bb4 << 1), 0x0000), // clk_dcs27 = 27M
// MIIC
WREG(REG_ADDR_BASE + (0x103360 << 1), 0x0444), // clk_miic = xtal/2
                                               // clk_miic2 = xtal/2
                                               // clk_miic3 = xtal/2

//TSO
WREG(REG_ADDR_BASE + (0x100b4e << 1), 0x0000), // clk_tso0_in = ts0_clk
                                               // clk_tso_trace = 216Mhz
WREG(REG_ADDR_BASE + (0x100b5e << 1), 0x0000), // clk_stamp

// Gmac
WREG(REG_ADDR_BASE + (0x103364 << 1), 0x0010), // clk_stamp

#if defined (CONFIG_MSTAR_KAYLA_BD_MST247D_D01A_S) || defined (CONFIG_MSTAR_KAYLA_BD_MST247D_D01A) || defined (CONFIG_MSTAR_KAYLA_BD_MST247D_D02A)
    
// disable YPbPr
WREG_B(REG_ADDR_BASE + (0x101a06 << 1), 0x00),

// disable RF
WREG(REG_ADDR_BASE + (0x1128e0 << 1), 0x0000),//70h,rf_clk
WREG(REG_ADDR_BASE + (0x160be0 << 1), 0x0000),
WREG_B(REG_ADDR_BASE + (0x160be0 << 1)+1, 0x00), // down digsyn clk
// disable RGMII
WREG(REG_ADDR_BASE + (0x110cc6 << 1), 0xffff), // down GMACPLL
WREG_B(REG_ADDR_BASE + (0x100bc6 << 1), 0x01), // down digsyn clk

//WREG_B(REG_ADDR_BASE + (0x000e39 << 1), 0x03), //RGMII = GPIO mode

WREG(REG_ADDR_BASE + (0x000f4e << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f50 << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f52 << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f54 << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f56 << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f58 << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f5a << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f5c << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f5e << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f60 << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f62 << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f64 << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f66 << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f68 << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f6a << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f6c << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f6e << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f70 << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f72 << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f74 << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f76 << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f78 << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f7a << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f7c << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f7e << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f80 << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f82 << 1), 0x4011),
WREG(REG_ADDR_BASE + (0x000f84 << 1), 0x4011),

// disable LAN
WREG_B(REG_ADDR_BASE + (0x0032b6 << 1)+1, 0x17  ), // Power-down ADC
WREG_B(REG_ADDR_BASE + (0x0032d4 << 1), 0x20), // Power-down LPF_OP
WREG(REG_ADDR_BASE + (0x00333a << 1), 0x03F3), // down PD_TX_IDAC, PD_TX_LD = 0
//WREG_B(REG_ADDR_BASE + (0x0033a1 << 1), 0x10), // down VBUF

WREG_B(REG_ADDR_BASE + (0x0032cc << 1) +1, 0xd9  ), // Power-down ADCPL
WREG_B(REG_ADDR_BASE + (0x0032ea << 1), 0x60), // down PORST
WREG_B(REG_ADDR_BASE + (0x0032ca << 1)+1, 0x13  ), // Power-on BGAP
WREG_B(REG_ADDR_BASE + (0x0032ba << 1)+1, 0x85  ), // Power-on REF
WREG_B(REG_ADDR_BASE + (0x0033a0 << 1)+1, 0x10  ), // VBUF

WREG(REG_ADDR_BASE + (0x0032fc << 1), 0x0102), // Power-down LDO

WREG_B(REG_ADDR_BASE + (0x0033c4 << 1)+1, 0x40  ), // 100gat =1
WREG_B(REG_ADDR_BASE + (0x003330 << 1), 0x53), // 200gat =1
WREG_B(REG_ADDR_BASE + (0x003204 << 1), 0x11  ), // Release reset analog

// disable ch34 clk  //kayla_ANA_DIG_RF_OFF.txt
WREG_B(REG_ADDR_BASE + (0x1128E0 << 1), 0x0),
WREG_B(REG_ADDR_BASE + (0x160bE0 << 1), 0x0),
WREG_B(REG_ADDR_BASE + (0x160bE1 << 1), 0x0),
WREG_B(REG_ADDR_BASE + (0x160bE2 << 1), 0x10),



#endif
