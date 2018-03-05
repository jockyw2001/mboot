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

// [Notice]
// 1. GE clock is OFF !
// 2. ZDEC clock is OFF !
// 3. 1st_GMAC AHB clock is ON / 216MHz !
// 4. 2nd_GMAC AHB clock is ON / 144MHz !
// 5. clk_usb20_p0/p1/p2_240m is OFF !
// 6. TSP* clock is OFF !
// 7. VE clock is OFF !
// 8. HDGEN(YPbPr) clock is OFF !
// 9. Vivalid9 mcu clock is ON / 216MHz.
// 10. SCx clock is OFF.
// 11. Periperial Ips setting
// => IICM* clock is OFF !
// => fuar0 clock is ON/fuart_syn(432MHz)
// => uart0 clock is ON/123MHz
// => uart1 clock is ON/123MHz
// => mspi* clock is OFF !


//switch miu128 clk_miu to miupll
WREG(REG_ADDR_BASE + (0x100b3e << 1), 0x0008),

//switch clk_ddr_syn
WREG_B((REG_ADDR_BASE + (0x100bfa <<1)) , 0x04),



//Clk_SEL Setting@mboot
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100A02 <<1) + 1)), 0x0C, 0x1C), // reg_ckg_gmac_ahb@0001[12:10]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x103364 <<1)), 0x08, 0x1C), // reg_ckg_secgmac_ahb@0032[4:2]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B2C <<1)), 0x18, 0x1C), // reg_ckg_spi@0016[4:2]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B2E <<1)), 0x08, 0x0C), // reg_ckg_fuart@0017[3:2]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B26 <<1) + 1)), 0x0C, 0x1C), // reg_ckg_uart0@0013[12:10]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B28 <<1)), 0x0C, 0x1C), // reg_ckg_uart1@0014[4:2]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x10336A <<1)), 0x00, 0x1C), // reg_ckg_v9mcu@0035[4:2]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B2E <<1)), 0x00, 0xC0), // reg_ckg_fuart0_syn@0017[7:6]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100BC6 <<1)), 0x00, 0x0C), // reg_ckg_gmac@0063[3:2]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B20 <<1) + 1)), 0x00, 0x1C), // reg_ckg_mcu@0010[12:10]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B20 <<1)), 0x01, 0x01), // reg_sw_mcu_clk@0010[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B3E <<1)), 0x08, 0x0C), // reg_ckg_miu@001f[3:2]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100BFA <<1)), 0x04, 0x0C), // reg_ckg_ddr_syn@007d[3:2]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B3A <<1)), 0x04, 0x1C), // reg_ckg_mpll_syn@001d[4:2]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B3A <<1) + 1)), 0x04, 0x1C), // reg_ckg_mpll_syn2@001d[12:10]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B40 <<1)), 0x04, 0x1C), // reg_ckg_mpll_syn3@0020[4:2]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B40 <<1) + 1)), 0x04, 0x1C), // reg_ckg_mpll_syn4@0020[12:10]


WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B54 <<1)), 0x00, 0x1C), // reg_ckg_tsp@002a[4:2]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B72 <<1) + 1)), 0x04, 0x0C), // reg_ckg_parser@0039[11:10]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B5E <<1) + 1)), 0x00, 0x0C), // reg_ckg_stamp@002f[11:10]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100BCC <<1)), 0x04, 0x0C), // reg_ckg_bdma@0066[3:2]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100A24 <<1)), 0x08, 0x1C), // reg_ckg_smart@0012[4:2]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B62 <<1)), 0x0C, 0x1C), // reg_ckg_r2_secure@0031[4:2]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B62 <<1)), 0x20, 0x20), // reg_ckg_r2_secure@0031[5:5]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100BB2 <<1)), 0x08, 0x0C), // reg_ckg_dac1@0059[3:2]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100BB2 <<1) + 1)), 0x08, 0x0C), // reg_ckg_dac2@0059[11:10]





//Clk_ON/OFF Setting@mboot
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B90 <<1)), 0xff, 0x01), // reg_ckg_ge@0048[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B7C <<1)), 0xff, 0x01), // reg_ckg_gpd@003e[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100A22 <<1)), 0xff, 0x01), // reg_ckg_njpd@0011[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100A20 <<1)), 0xff, 0x01), // reg_ckg_mfe@0010[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B72 <<1)), 0xff, 0x01), // reg_ckg_mvd@0039[0]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100A26 <<1)), 0xff, 0x01), // reg_ckg_zdec_vld@0013[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100A28 <<1)), 0xff, 0x01), // reg_ckg_zdec_lzd@0014[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B60 <<1) + 1)), 0xff, 0x01), // ~reg_clk_vd_mheg5_icg_en@~0030[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100A7C <<1)), 0xff, 0x01), // reg_ckg_sdio@003e[0]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100BC8 <<1)), 0xff, 0x01), // reg_ckg_nfie@0064[0]

//////////////////
// DO NOT disable ECC clock here, it will cause NAND ECC function disabled. Please see mantis#1412713 for detail
//////////////////
//WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100A02 <<1)), 0xff, 0x01), // reg_ckg_ecc@0001[0]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100A02 <<1) + 1)), 0x00, 0x01), // reg_ckg_gmac_ahb@0001[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x103364 <<1)), 0x00, 0x01), // reg_ckg_secgmac_ahb@0032[0]
WREG_B_MASK((REG_ADDR_BASE + (0x121F24 <<1)), 0xff, 0x01), // reg_ckg_gmac_rx@0012[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x121F22 <<1) + 1)), 0xff, 0x01), // reg_ckg_gmac_tx@0011[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x121F24 <<1) + 1)), 0xff, 0x01), // reg_ckg_gmac_rx_ref@0012[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x121F24 <<1)), 0xff, 0x10), // reg_ckg_gmac_tx_ref@0012[4]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x122424 <<1)), 0xff, 0x01), // reg_ckg_secgmac_rx@0012[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x122422 <<1) + 1)), 0xff, 0x01), // reg_ckg_secgmac_tx@0011[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x122424 <<1) + 1)), 0xff, 0x01), // reg_ckg_secgmac_rx_ref@0012[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x122424 <<1)), 0xff, 0x10), // reg_ckg_secgmac_tx_ref@0012[4]
WREG_B_MASK((REG_ADDR_BASE + (0x103A88 <<1)), 0xff, 0x02), // ~reg_utmi_clk120_en@~0044[1]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x103A08 <<1)), 0xff, 0x02), // ~reg_utmi_clk120_en@~0004[1]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x103908 <<1)), 0xff, 0x02), // ~reg_utmi_clk120_en@~0004[1]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B7A <<1)), 0xff, 0x01), // reg_ckg_evd@003d[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100A38 <<1)), 0xff, 0x01), // reg_ckg_evd_ppu@001c[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B68 <<1)), 0xff, 0x01), // reg_ckg_hvd@0034[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B68 <<1)), 0xff, 0x01), // reg_ckg_hvd@0034[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100A36 <<1)), 0xff, 0x01), // reg_ckg_hvd_aec@001b[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100A3A <<1)), 0xff, 0x01), // reg_ckg_vp8@001d[0]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B54 <<1)), 0xff, 0x01), // reg_ckg_tsp@002a[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B72 <<1) + 1)), 0xff, 0x01), // reg_ckg_parser@0039[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B5E <<1) + 1)), 0xff, 0x01), // reg_ckg_stamp@002f[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B50 <<1)), 0xff, 0x01), // reg_ckg_ts0@0028[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B50 <<1) + 1)), 0xff, 0x01), // reg_ckg_ts1@0028[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B52 <<1)), 0xff, 0x01), // reg_ckg_ts2@0029[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B52 <<1) + 1)), 0xff, 0x01), // reg_ckg_ts3@0029[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B54 <<1) + 1)), 0xff, 0x01), // reg_ckg_stc0@002a[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B54 <<1) + 1)), 0xff, 0x10), // reg_ckg_stc1@002a[12]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100A9E <<1)), 0xff, 0x01), // reg_ckg_stc2@004f[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100A9E <<1)), 0xff, 0x10), // reg_ckg_stc3@004f[4]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B48 <<1) + 1)), 0xff, 0x01), // reg_ckg_vedac@0024[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B48 <<1)), 0xff, 0x01), // reg_ckg_ve@0024[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B48 <<1)), 0xff, 0x10), // reg_ckg_ve_13@0024[4]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100BB0 <<1) + 1)), 0xff, 0x01), // reg_ckg_hdgen_fir@0058[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100BB0 <<1)), 0xff, 0x01), // reg_ckg_hdgen@0058[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100BB4 <<1)), 0xff, 0x01), // reg_ckg_hdgen_in@005a[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100BB2 <<1)), 0xff, 0x01), // reg_ckg_dac1@0059[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100BB2 <<1)), 0xff, 0x01), // reg_ckg_dac1@0059[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100BB0 <<1) + 1)), 0xff, 0x01), // reg_ckg_hdgen_fir@0058[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B48 <<1) + 1)), 0xff, 0x01), // reg_ckg_vedac@0024[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100BB2 <<1) + 1)), 0xff, 0x01), // reg_ckg_dac2@0059[8]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B98 <<1)), 0xff, 0x01), // reg_ckg_dc0@004c[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100BAE <<1) + 1)), 0xff, 0x01), // reg_ckg_sub_dc0@0057[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100BA4 <<1) + 1)), 0xff, 0x01), // reg_ckg_fclk@0052[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100BA8 <<1) + 1)), 0xff, 0x01), // reg_ckg_dip_fclk@0054[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100BA4 <<1)), 0xff, 0x01), // reg_ckg_od2ve@0052[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100BB6 <<1) + 1)), 0xff, 0x01), // reg_ckg_edclk_f2@005b[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100BBA <<1) + 1)), 0xff, 0x01), // reg_ckg_sc1_edclk@005d[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100BBA <<1) + 1)), 0xff, 0x10), // reg_ckg_sc1_fclk@005d[12]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100BBE <<1)), 0xff, 0x01), // reg_ckg_afbc@005f[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100BA0 <<1)), 0xff, 0x01), // reg_ckg_gop_mix_hd_eng@0050[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100BA0 <<1) + 1)), 0xff, 0x01), // reg_ckg_gop_mix_sd_eng@0050[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100BA6 <<1)), 0xff, 0x01), // reg_ckg_odclk@0053[0]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B2C <<1)), 0x00, 0x01), // reg_ckg_spi@0016[0]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x103360 <<1)), 0xff, 0x01), // reg_ckg_miic@0030[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x103360 <<1)), 0xff, 0x10), // reg_ckg_miic2@0030[4]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x103360 <<1) + 1)), 0xff, 0x01), // reg_ckg_miic3@0030[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100A74 <<1)), 0xff, 0x01), // reg_ckg_miic4@003a[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100A74 <<1)), 0xff, 0x10), // reg_ckg_miic5@003a[4]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100A74 <<1) + 1)), 0xff, 0x01), // reg_ckg_miic6@003a[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B2E <<1)), 0x00, 0x01), // reg_ckg_fuart@0017[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B26 <<1) + 1)), 0x00, 0x01), // reg_ckg_uart0@0013[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B28 <<1)), 0x00, 0x01), // reg_ckg_uart1@0014[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B2C <<1) + 1)), 0xff, 0x01), // reg_ckg_mspi0@0016[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B2C <<1) + 1)), 0xff, 0x10), // reg_ckg_mspi1@0016[12]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x10336A <<1)), 0x00, 0x01), // reg_ckg_v9mcu@0035[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B2E <<1)), 0x00, 0x10), // reg_ckg_fuart0_syn@0017[4]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100BC6 <<1)), 0x00, 0x01), // reg_ckg_gmac@0063[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100A78 <<1)), 0xff, 0x01), // reg_ckg_fcie_syn@003c[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100A24 <<1)), 0xff, 0x01), // reg_ckg_smart@0012[0]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B3E <<1) + 1)), 0x00, 0x10), // reg_miu_acp_clk_en@001f[12]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B20 <<1) + 1)), 0x00, 0x01), // reg_ckg_mcu@0010[8]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B3E <<1)), 0x00, 0x01), // reg_ckg_miu@001f[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100BFA <<1)), 0x00, 0x01), // reg_ckg_ddr_syn@007d[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B3A <<1)), 0x00, 0x01), // reg_ckg_mpll_syn@001d[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B3A <<1) + 1)), 0x00, 0x01), // reg_ckg_mpll_syn2@001d[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B40 <<1)), 0x00, 0x01), // reg_ckg_mpll_syn3@001d[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B40 <<1) + 1)), 0x00, 0x01), // reg_ckg_mpll_syn4@001d[8]


WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B54 <<1)), 0x00, 0x01), // reg_ckg_tsp@002a[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B72 <<1) + 1)), 0x00, 0x01), // reg_ckg_parser@0039[8]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100B5E <<1) + 1)), 0x00, 0x01), // reg_ckg_stamp@002f[8]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100BCC <<1)), 0x00, 0x01), // reg_ckg_bdma@0066[0]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100A24 <<1)), 0x00, 0x01), // reg_ckg_smart@0012[0]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100B62 <<1)), 0x00, 0x01), // reg_ckg_r2_secure@0031[0]

WREG_B_MASK_NO_BASE((REG_ADDR_BASE + (0x100BB2 <<1)), 0x00, 0x01), // reg_ckg_dac1@0059[0]
WREG_B_MASK_NO_BASE((REG_ADDR_BASE + ((0x100BB2 <<1) + 1)), 0x00, 0x01), // reg_ckg_dac2@0059[8]
