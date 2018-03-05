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

//=================================================
// 002_CLKGEN
//=================================================


//<<CLKGEN_PM>>

    WREG ((REG_ADDR_BASE + (0xE40<<1)), 0x1008),    // clk_mcu = 172M (step 1/2)
                             //@ clk_spi = 54M (step_1/2)
    WREG ((REG_ADDR_BASE + (0xE40<<1)), 0x5088),    //  clk_mcu = 172M (step 2/2)
                             //@ clk_spi = 54M (step_2/2)

    #if defined (VANCLEEF_MCP_ENABLE)

    #if (VANCLEEF_MCP_ENABLE == TRUE)
    WREG ((REG_ADDR_BASE + (0xE30<<1)), 0x0841),    //  wriu 0x0e60 0x40
    WREG ((REG_ADDR_BASE + (0x1426<<1)),0x0040),    //  SAR
    #else

    #endif

    #endif
//--------------------------
//<<CLKGEN0>>
//--------------------------
    WREG ((REG_ADDR_BASE + (0x100B02<<1)), 0x0007),    //  SRAM bist test clk 7:xtal, 6:62M, 5:86M 4:108M 3:123M 2:144M 1:172M 0:216M

//MCU
    WREG ((REG_ADDR_BASE + (0x100B20<<1)), 0x0400),    //  clk_mcu = 172M (step 1/2)
    WREG ((REG_ADDR_BASE + (0x100B20<<1)), 0x0401),    //  clk_mcu = 172M (step 2/2)

    WREG ((REG_ADDR_BASE + (0x100B22<<1)), 0x0000),    //  clk_ocp on
                            //@ clk_mips = 500M (step 1/2) (MIPSPLL_VCO)
    WREG ((REG_ADDR_BASE + (0x100B22<<1)), 0x8000),    //  clk_mips = 500M (step 2/2) (MIPSPLL_VCO)

//HK_R2
    WREG ((REG_ADDR_BASE + (0x100B24<<1)), 0x001c),    //  clk_aeon_p1 = 240M (step 1/2)
    WREG ((REG_ADDR_BASE + (0x100B24<<1)), 0x009c),    //  clk_aeon select clk_aeon_p1 (step 2/2), default clk_xtal


    WREG ((REG_ADDR_BASE + (0x100B28<<1)), 0x0000),    //  clk_uart1 = 172M (step_1/2)
                            //@ clk_uart2 = 172M (step_1/2)
    WREG ((REG_ADDR_BASE + (0x100B26<<1)), 0x0c06),    //  clk_uart0 = 123M
                            //@ clk_uart1 = 172M (step_2/2)
                            //@ clk_uart2 = 172M (step_2/2)

    WREG ((REG_ADDR_BASE + (0x100B2C<<1)), 0x0010),    //  clk_spi = 54M (step_1/2)
    WREG ((REG_ADDR_BASE + (0x100B2C<<1)), 0x0030),    //  clk_spi = 54M (step_2/2)

    WREG ((REG_ADDR_BASE + (0x100B2E<<1)), 0x0100),    //  clk_fuart = 54M
                            //@ uart synthesizer enable

//PCM & TCK
    WREG ((REG_ADDR_BASE + (0x101E08<<1)), 0x0020),    //  CLK_TCK_IN from PAD_TS1_SYNC
    WREG ((REG_ADDR_BASE + (0x100B34<<1)), 0x0000),    //  clk_pcm = 27M
                            //@ clk_tck = CLK_TCK_IN

//MIU
    WREG ((REG_ADDR_BASE + (0x100B3C<<1)), 0x0100),    // clk_mpll_syn = 432M (for VEPLL  0: 216M, 1:432M)
    //WREG ((REG_ADDR_BASE + (0x1F20167C 0x001C   @ clk_miu = 240M (MIUPLL)
                            // clk_miu_rec = 93K (clk_xtal/128)
    //WREG 0x1F20166c 0x8000   @ ckg_miu_256b = 240M (MIUPLL)

    //WREG 0x1F20167C 0x0010   @ clk_miu = 240M (from UTMI)
                            // clk_miu_rec = 93K (clk_xtal/128)
    //WREG 0x1F20166c 0x8004   @ ckg_miu_256b = 240M (from UTMI)

    WREG ((REG_ADDR_BASE + (0x100B3E<<1)), 0x0018),    //  clk_miu = 288M
                            //@ clk_miu_rec = 93K (clk_xtal/128)
    WREG ((REG_ADDR_BASE + (0x100B36<<1)), 0x8000),    //  ckg_miu_256b = 300M (MIUPLL)

    WREG ((REG_ADDR_BASE + (0x100BFC<<1)), 0x0001),    //  clk_ddr_syn = 432M (for MEMPLL  0: 216M, 1:432M)

//VE
    WREG ((REG_ADDR_BASE + (0x100B48<<1)), 0x0800),    //  clk_ve = 27M
                            //@ clk_vedac = 108M
    WREG ((REG_ADDR_BASE + (0x100B4A<<1)), 0x001c),    //  clk_ve_in = 27M (clk_odclk)

//TSP
    WREG ((REG_ADDR_BASE + (0x101E04<<1)), 0x0001),    //  CLK_TS0_IN from PAD_TS0_CLK
    WREG ((REG_ADDR_BASE + (0x100B50<<1)), 0x0000),    //  clk_ts0 = CLK_TS0_IN
                            // clk_ts1 = CLK_TS0_IN
    WREG ((REG_ADDR_BASE + (0x100B54<<1)), 0x000c),    //  clk_tsp = 172M
                            // clk_stc0 = 27M (STC0_SYNTH_OUT)

//VD_R2
    WREG ((REG_ADDR_BASE + (0x100B60<<1)), 0x001c),    //  ckg_vd_mheg5 = 240M

//HVD
    WREG ((REG_ADDR_BASE + (0x100B62<<1)), 0x0000),    //  ckg_hvd = 216M

//MFE
    WREG ((REG_ADDR_BASE + (0x100B68<<1)), 0x0008),    //  ckg_mfe = 144M (0:123m 4:108m 8:144m c:192m)

//JPD
    WREG ((REG_ADDR_BASE + (0x100B6A<<1)), 0x0008),    //  ckg_jpd = 144M (0:72m 4:108m 8:144m c:192m)

//GPD
    WREG ((REG_ADDR_BASE + (0x100B6C<<1)), 0x0000),    //  ckg_gpd = 216M

//MVD
    WREG ((REG_ADDR_BASE + (0x100B70<<1)), 0x0000),    //  mvd_syn off

    WREG ((REG_ADDR_BASE + (0x100B72<<1)), 0x0000),    //  clk_mvd = 144M
                            // clk_parser = 172M

    WREG ((REG_ADDR_BASE + (0x100B74<<1)), 0x0000),    //  clk_mvd_chroma on
                            // clk_mvd_luma_a on
    WREG ((REG_ADDR_BASE + (0x100B76<<1)), 0x0000),    //  clk_mvd_luma_b on
                            // clk_mvd_luma_c on
    WREG ((REG_ADDR_BASE + (0x100B78<<1)), 0x0000),    //  clk_mvd_rmem on
                            // clk_mvd_rmem1 on

//GOP
    WREG ((REG_ADDR_BASE + (0x100B80<<1)), 0x0000),    //  clk_gopg0 = 27M (clk_odclk)
                            // clk_gopg1 = 27M (clk_odclk)
    WREG ((REG_ADDR_BASE + (0x100B82<<1)), 0x0018),    //  clk_gopmix = 27M (clk_ve)
                            // clk_gopd = 27M (clk_odclk)
    WREG ((REG_ADDR_BASE + (0x100B84<<1)), 0x0004),    //  clk_gopg2 = 192M (clk_idclk)
                            // clk_gopg3 = 27M (clk_odclk)

    WREG ((REG_ADDR_BASE + (0x100B86<<1)), 0x0000),    //  clk_psram0,1,2,3 on

    WREG ((REG_ADDR_BASE + (0x100B88<<1)), 0x1414),    //  clk_gopg4 = (clk_sc1_odclk)
                            // clk_gopg5 = (clk_sc1_odclk)

//GE
    WREG ((REG_ADDR_BASE + (0x100B90<<1)), 0x000c),    //  clk_ge = 240MHz
    WREG ((REG_ADDR_BASE + (0x100B90<<1)), 0x0008),    //  clk_ge = MIUPLL

//MVOP
    WREG ((REG_ADDR_BASE + (0x100B98<<1)), 0x0018),    //  clk_dc0 = 160M

//MVOP SUB
    WREG ((REG_ADDR_BASE + (0x100B9A<<1)), 0x0018),    //  clk_sub_dc0 = 160M

//SC
    WREG ((REG_ADDR_BASE + (0x100BA2<<1)), 0x0400),    //  clk_ficlk = 192M (clk_fclk)
    WREG ((REG_ADDR_BASE + (0x100BA4<<1)), 0x1000),    //  clk_flck = 192M
    WREG ((REG_ADDR_BASE + (0x100BA6<<1)), 0x000c),    //  clk_odclk = 27M (CLK_OUT_DAC/8) (output pixel clock)
    WREG ((REG_ADDR_BASE + (0x100BAA<<1)), 0x000c),    //  clk_idclk = 192M (clk_dc0)
    WREG ((REG_ADDR_BASE + (0x100BB0<<1)), 0x0c0c),    //  clk_hdgen = 27M (CLK_OUT_DAC/8)
                            // clk_hdgen_fir = 27M (CLK_OUT_DAC/8)
    WREG ((REG_ADDR_BASE + (0x100BB2<<1)), 0x000c),    //  clk_dac1 = 108M (clk_vedac) (SD)
                            // clk_dac2 = 27M (clk_hdgen_fir) (HD)
    WREG ((REG_ADDR_BASE + (0x100BB4<<1)), 0x000c),    //  clk_dcs = 27M (CLK_OUT_DAC/8)
                            // clk_dcs27 = 27M

//SC1
    WREG ((REG_ADDR_BASE + (0x100BD4<<1)), 0x0000),    //  clk_sc1_idclk0, clk_sc1_idclk2 on
    WREG ((REG_ADDR_BASE + (0x100BD6<<1)), 0x0000),    //  clk_sc1_sidckl0, clk_sc1_sidclk2, clk_sc1_edclk, clk_sc1_fclk on
    WREG ((REG_ADDR_BASE + (0x100BD8<<1)), 0x0000),    //  clk_sc1_feclk_f2, clk_sc1_feclk2_f2, clk_sc1_odclk on

//EMAC
    WREG ((REG_ADDR_BASE + (0x100BC0<<1)), 0x0000),    //  clk_emac_ahb = 123M
                            // clk_emac_rx = CLK_EMAC_RX_IN
    WREG ((REG_ADDR_BASE + (0x100BC2<<1)), 0x0000),    //  clk_emac_tx = CLK_EMAC_TX_IN
                            // clk_emac_rx_ref = CLK_EMAC_RX_IN
    WREG ((REG_ADDR_BASE + (0x100BC4<<1)), 0x0000),    //  clk_emac_tx_ref = CLK_EMAC_TX_IN

//NAND
    //WREG ((REG_ADDR_BASE + (0x100BC8<<1)), 0x0024),    //  clk_fice = 54M

//MPIF
    WREG ((REG_ADDR_BASE + (0x100BD0<<1)), 0x0000),    //  clk_mpif = 123M

//SDIO
    WREG ((REG_ADDR_BASE + (0x100BD2<<1)), 0x0028),    //  clk_sdio = 48M

//--------------------------
//<<CLKGEN1>>
//--------------------------

//CA
    WREG ((REG_ADDR_BASE + (0x103348<<1)), 0x0000),    //  clk_nsk = 123M, clk_cftop = 48M
                            //@ clk_sm_ca = 27M, clk_sm_ca2 = 27M

//HDMITX
    WREG ((REG_ADDR_BASE + (0x103350<<1)), 0x0000),    //  clk_hdmitx = 27M (clk_odclk)

//MIIC
    WREG ((REG_ADDR_BASE + (0x103360<<1)), 0x0000),    //  clk_miic = 24M

//GPU
    //@WREG ((REG_ADDR_BASE + (0x103362)), 0x0000),    //  clk_gpu = 240M
    WREG ((REG_ADDR_BASE + (0x103362<<1)), 0x000C),   // clk_gpu = MIUPLL

// BT (disalbe)
    WREG ((REG_ADDR_BASE + (0x103374<<1)), 0x0001), // @ disable clk_bt_au_src

// SATA_AHB
    WREG ((REG_ADDR_BASE + (0x103376<<1)), 0x0004),    //  clk_sata_ahb = 216M

// CH34
    WREG ((REG_ADDR_BASE + (0x103378<<1)), 0x0000),    //  clk_rf_src = 432M


