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
#if defined(CONFIG_MSTAR_BFE_EN) || (defined(CONFIG_MSTAR_NS_UBOOT) && defined(CONFIG_MMC))

#include "eMMC.h"
#if defined(UNIFIED_eMMC_DRIVER) && UNIFIED_eMMC_DRIVER

U32 gu32_eMMCDrvExtFlag = 0;
void *eMMC_get_RPMBContext_address(void)
{
	return (void*)(CONFIG_BOOTRAM_LOADADDR+0x20000);
}

void *eMMC_memset (void *str, int c, unsigned int len)
{
	register char *st = str;

	while (len-- > 0)
		*st++ = c;
	return str;
}

void *eMMC_memcpy (void *destaddr, void const *srcaddr, unsigned int len)
{
	char *dest = destaddr;
	char const *src = srcaddr;

	while (len-- > 0)
		*dest++ = *src++;
	return destaddr;
}


U32 eMMC_hw_timer_delay(U32 u32us)
{
    U32 u32HWTimer = 0;
    volatile U16 u16TimerLow = 0;
    volatile U16 u16TimerHigh = 0;

    // reset HW timer
    REG_FCIE_W(TIMER1_MAX_LOW, 0xFFFF);
    REG_FCIE_W(TIMER1_MAX_HIGH, 0xFFFF);
    REG_FCIE_W(TIMER1_ENABLE, 0);

    // start HW timer
    REG_FCIE_SETBIT(TIMER1_ENABLE, 0x0001);

    while( u32HWTimer < 12*u32us ) // wait for u32usTick micro seconds
    {
        REG_FCIE_R(TIMER1_CAP_LOW, u16TimerLow);
        REG_FCIE_R(TIMER1_CAP_HIGH, u16TimerHigh);

        u32HWTimer = (u16TimerHigh<<16) | u16TimerLow;
    }

    REG_FCIE_W(TIMER1_ENABLE, 0);

    return u32us;
}

U32 eMMC_hw_timer_start(void)
{
    // Reset PIU Timer1
    REG_FCIE_W(TIMER1_MAX_LOW, 0xFFFF);
    REG_FCIE_W(TIMER1_MAX_HIGH, 0xFFFF);
    REG_FCIE_W(TIMER1_ENABLE, 0);

    // Start PIU Timer1
    REG_FCIE_SETBIT(TIMER1_ENABLE, 0x0001);
    return 0;
}

//--------------------------------
U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
    flush_cache();
    flush_L2();

    if( u32_DMAAddr < 0x20000000 )
        return u32_DMAAddr - 0x10008000;
    else
        #ifdef IP_FCIE_VERSION_5
        return  (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
        #else
        return  (u32_DMAAddr & 0x1fffffff);
        #endif
}

U32 eMMC_pads_switch(U32 u32Mode)
{
    g_eMMCDrv.u8_PadType = u32Mode;

    // chiptop
    REG_FCIE_CLRBIT(reg_chiptop_0x08, BIT_SD_CONFIG);
    REG_FCIE_CLRBIT(reg_chiptop_0x08, BIT_NAND_MODE);
    REG_FCIE_SETBIT(reg_chiptop_0x08, BIT_EMMC_CONFIG_EMMC_MODE_1);
    REG_FCIE_CLRBIT(reg_chiptop_0x50, BIT_ALL_PAD_IN);

    // fcie
    #if defined(CONFIG_MSTAR_KANO)
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_8BIT_MACRO_EN);
    #elif defined(CONFIG_MSTAR_CURRY)
    REG_FCIE_CLRBIT(FCIE_DDR_MODE,
        BIT_PAD_IN_MASK|BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_CLK2_SEL|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    REG_FCIE_CLRBIT(reg_emmcpll_0x1a, BIT10);
    #elif defined(CONFIG_MSTAR_K6) || defined(CONFIG_MSTAR_K6LITE)
    REG_FCIE_CLRBIT(FCIE_DDR_MODE,
        BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_CLK2_SEL|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    #endif

    // emmcpll
    REG_FCIE_CLRBIT(reg_emmcpll_0x1c, BIT9);
    REG_FCIE_CLRBIT(reg_emmcpll_0x68, BIT0|BIT1);

    switch(u32Mode)
    {
        case FCIE_eMMC_BYPASS:
            #if defined(CONFIG_MSTAR_KANO)
            REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH);
            #elif defined(CONFIG_MSTAR_CURRY)
            REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_PAD_IN_MASK|BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL);
            REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT10);
            #elif defined(CONFIG_MSTAR_K6) || defined(CONFIG_MSTAR_K6LITE)
            REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL);
            #endif
            break;

        case FCIE_eMMC_SDR:
            // fcie
            REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_8BIT_MACRO_EN);
            REG_FCIE_SETBIT(reg_emmcpll_0x1c, BIT9);
            REG_FCIE_SETBIT(reg_emmcpll_0x68, BIT0);

            break;

        default:
            eMMC_debug(1, 1, "eMMC Err: wrong parameter for switch pad func\n");
            return eMMC_ST_ERR_PARAMETER;
            break;
    }

    return eMMC_ST_SUCCESS;
}


#if !(defined(CONFIG_MSTAR_K6)|| defined(CONFIG_MSTAR_K6LITE))
U32 eMMC_pll_setting(U16 u16_ClkParam)
{
    static U16 u16_OldClkParam=0xFFFF;
    U32 u32_value_reg_emmc_pll_pdiv;

    if(u16_ClkParam == u16_OldClkParam)
        return eMMC_ST_SUCCESS;
    else
        u16_OldClkParam = u16_ClkParam;

    // 1. reset emmc pll
    REG_FCIE_SETBIT(reg_emmc_pll_reset,BIT0);
    REG_FCIE_CLRBIT(reg_emmc_pll_reset,BIT0);

    // 2. synth clock
    switch(u16_ClkParam)
    {
        case eMMC_PLL_CLK_200M: // 200M
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
        #if 0
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x22); // 20xMHz
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x8F5C);
        #else
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x24); // 195MHz
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x03D8);
        #endif
            u32_value_reg_emmc_pll_pdiv =1;// PostDIV: 2
            break;

        case eMMC_PLL_CLK_160M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv =1;// PostDIV: 2
            break;

        case eMMC_PLL_CLK_140M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x31);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x5F15);
            u32_value_reg_emmc_pll_pdiv = 1;// PostDIV: 2
            break;

        case eMMC_PLL_CLK_120M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x39);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x9999);
            u32_value_reg_emmc_pll_pdiv = 1;// PostDIV: 2
            break;

        case eMMC_PLL_CLK_100M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x45);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x1EB8);
            u32_value_reg_emmc_pll_pdiv = 1;// PostDIV: 2
            break;

        case eMMC_PLL_CLK__86M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x28);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x2FA0);
            u32_value_reg_emmc_pll_pdiv = 2;// PostDIV: 4
            break;

        case eMMC_PLL_CLK__80M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv = 2;// PostDIV: 4
            break;

        case eMMC_PLL_CLK__72M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x30);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 2;// PostDIV: 4
            break;

        case eMMC_PLL_CLK__62M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x37);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0xBDEF);
            u32_value_reg_emmc_pll_pdiv = 2;// PostDIV: 4
            break;

        case eMMC_PLL_CLK__52M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x42);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x7627);
            u32_value_reg_emmc_pll_pdiv = 2;// PostDIV: 4
            break;

        case eMMC_PLL_CLK__48M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x48);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 2;// PostDIV: 4
            break;

        case eMMC_PLL_CLK__40M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv = 4;// PostDIV: 8
            break;

        case eMMC_PLL_CLK__36M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x30);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 4;// PostDIV: 8
            break;

        case eMMC_PLL_CLK__32M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x36);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 4;// PostDIV: 8
            break;

        case eMMC_PLL_CLK__27M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x40);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 4;// PostDIV: 8
            break;

        case eMMC_PLL_CLK__20M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv = 7;// PostDIV: 16
            break;

        default:
            eMMC_debug(0,0,"eMMC Err: emmc PLL not configed %Xh\n", u16_ClkParam);
            eMMC_die(" ");
            return eMMC_ST_ERR_UNKNOWN_CLK;
            break;
    }

    // 3. VCO clock ( loop N = 4 )
    REG_FCIE_CLRBIT(reg_emmcpll_fbdiv,0xffff);
    REG_FCIE_SETBIT(reg_emmcpll_fbdiv,0x6);// PostDIV: 8

    // 4. 1X clock
    REG_FCIE_CLRBIT(reg_emmcpll_pdiv,BIT2|BIT1|BIT0);
    REG_FCIE_SETBIT(reg_emmcpll_pdiv,u32_value_reg_emmc_pll_pdiv);// PostDIV: 8

    if(u16_ClkParam==eMMC_PLL_CLK__20M)
    {
        REG_FCIE_SETBIT(reg_emmc_pll_test, BIT10);
    }
    else
    {
        REG_FCIE_CLRBIT(reg_emmc_pll_test, BIT10);
    }

    eMMC_hw_timer_delay(HW_TIMER_DELAY_100us); // asked by Irwin

    return eMMC_ST_SUCCESS;
}


void HalEmmcPll_dll_setting(void)
{
    volatile U16 u16_reg;
    REG_FCIE_CLRBIT(reg_emmcpll_0x09, BIT0); // ­«½Æ reg_emmc_rxdll_dline_en

    // Reset eMMC_DLL
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX30, BIT2);
    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX30, BIT2);

    //DLL pulse width and phase
    REG_FCIE_W(REG_EMMC_PLL_RX01, 0x7F72);

    // DLL code
    REG_FCIE_W(REG_EMMC_PLL_RX32, 0xF200);

    // DLL calibration
    REG_FCIE_W(REG_EMMC_PLL_RX30, 0x3378);
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX33, BIT15);

    // Wait 100us
    eMMC_hw_timer_delay(250);

    // Get hw dll0 code
    REG_FCIE_R(REG_EMMC_PLL_RX33, u16_reg);

    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX34, (BIT10 - 1));
    // Set dw dll0 code
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX34, u16_reg & 0x03FF);

    // Disable reg_hw_upcode_en
    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX30, BIT9);

    // Clear reg_emmc_dll_test[7]
    REG_FCIE_CLRBIT(reg_emmcpll_0x02, BIT15); // switch pad ¾Þ§@ bit13, excel undefine ??

    // Enable reg_rxdll_dline_en
    REG_FCIE_SETBIT(reg_emmcpll_0x09, BIT0); // ­«½Æ

}
#endif

U32 eMMC_clock_setting(U16 u16_ClkParam)
{
    eMMC_PlatformResetPre();

    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

    #if defined(CONFIG_MSTAR_K6) || defined(CONFIG_MSTAR_K6LITE)

    switch(u16_ClkParam) {
        case BIT_FCIE_CLK_1X_XTAL:  g_eMMCDrv.u32_ClkKHz =   3000;  break;
        case BIT_FCIE_CLK_1X_12M:   g_eMMCDrv.u32_ClkKHz =  12000;  break;
        case BIT_FCIE_CLK_1X_15_5M: g_eMMCDrv.u32_ClkKHz =  15500;  break;
        case BIT_FCIE_CLK_1X_18M:   g_eMMCDrv.u32_ClkKHz =  18000;  break;
        case BIT_FCIE_CLK_1X_21_5M: g_eMMCDrv.u32_ClkKHz =  21500;  break;
        case BIT_FCIE_CLK_1X_27M:   g_eMMCDrv.u32_ClkKHz =  27000;  break;
        case BIT_FCIE_CLK_1X_36M:   g_eMMCDrv.u32_ClkKHz =  36000;  break;
        case BIT_FCIE_CLK_1X_40M:   g_eMMCDrv.u32_ClkKHz =  40000;  break;
        case BIT_FCIE_CLK_1X_48M:   g_eMMCDrv.u32_ClkKHz =  48000;  break;
        case BIT_FCIE_CLK_1X_54M:   g_eMMCDrv.u32_ClkKHz =  54000;  break;
        case BIT_FCIE_CLK_1X_60M:   g_eMMCDrv.u32_ClkKHz =  60000;  break;
        case BIT_FCIE_CLK_1X_88_5M: g_eMMCDrv.u32_ClkKHz =  88500;  break;
        case BIT_FCIE_CLK_1X_75K:   g_eMMCDrv.u32_ClkKHz =     75;  break;
        case BIT_FCIE_CLK_1X_80M:   g_eMMCDrv.u32_ClkKHz =  80000;  break;

        case BIT_FCIE_CLK_4X_XTAL:  g_eMMCDrv.u32_ClkKHz =  12000;  break;
        case BIT_FCIE_CLK_4X_48M:   g_eMMCDrv.u32_ClkKHz =  48000;  break;
        case BIT_FCIE_CLK_4X_62M:   g_eMMCDrv.u32_ClkKHz =  62000;  break;
        case BIT_FCIE_CLK_4X_72M:   g_eMMCDrv.u32_ClkKHz =  72000;  break;
        case BIT_FCIE_CLK_4X_86M:   g_eMMCDrv.u32_ClkKHz =  86000;  break;
        case BIT_FCIE_CLK_4X_108M:  g_eMMCDrv.u32_ClkKHz = 108000;  break;
        case BIT_FCIE_CLK_4X_144M:  g_eMMCDrv.u32_ClkKHz = 144000;  break;
        case BIT_FCIE_CLK_4X_160M:  g_eMMCDrv.u32_ClkKHz = 160000;  break;
        case BIT_FCIE_CLK_4X_192M:  g_eMMCDrv.u32_ClkKHz = 192000;  break;
        case BIT_FCIE_CLK_4X_216M:  g_eMMCDrv.u32_ClkKHz = 216000;  break;
        case BIT_FCIE_CLK_4X_240M:  g_eMMCDrv.u32_ClkKHz = 240000;  break;
        case BIT_FCIE_CLK_4X_354M:  g_eMMCDrv.u32_ClkKHz = 354000;  break;
        case BIT_FCIE_CLK_4X_300K:  g_eMMCDrv.u32_ClkKHz =    300;  break;
        case BIT_FCIE_CLK_4X_320M:  g_eMMCDrv.u32_ClkKHz = 320000;  break;

        default:
            eMMC_debug(1, 1, "eMMC Err: clkgen %Xh\n", eMMC_ST_ERR_INVALID_PARAM);
            eMMC_die(" ");
            return eMMC_ST_ERR_INVALID_PARAM; break;
    }

    //eMMC_debug(0,0,"eMMC CLKGEN: %Xh\n", u16_ClkParam);
    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_GATING|BIT_FCIE_CLK_INVERSE|BIT_CLKGEN_FCIE_MASK|BIT_CKG_NFIE_DVI4_EN);

    if( REG_FCIE(FCIE_DDR_MODE) & BIT_8BIT_MACRO_EN )
        REG_FCIE_SETBIT(reg_ckg_fcie, BIT_CKG_NFIE_DVI4_EN);
    else
        REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_CKG_NFIE_DVI4_EN);

    REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL|((u16_ClkParam & 0xF)<<10));

    #else

    switch(u16_ClkParam)
    {
        // emmc_pll clock
        case eMMC_PLL_CLK__20M  : g_eMMCDrv.u32_ClkKHz =  20000; break;
        case eMMC_PLL_CLK__27M  : g_eMMCDrv.u32_ClkKHz =  27000; break;
        case eMMC_PLL_CLK__32M  : g_eMMCDrv.u32_ClkKHz =  32000; break;
        case eMMC_PLL_CLK__36M  : g_eMMCDrv.u32_ClkKHz =  36000; break;
        case eMMC_PLL_CLK__40M  : g_eMMCDrv.u32_ClkKHz =  40000; break;
        case eMMC_PLL_CLK__48M  : g_eMMCDrv.u32_ClkKHz =  48000; break;
        case eMMC_PLL_CLK__52M  : g_eMMCDrv.u32_ClkKHz =  52000; break;
        case eMMC_PLL_CLK__62M  : g_eMMCDrv.u32_ClkKHz =  62000; break;
        case eMMC_PLL_CLK__72M  : g_eMMCDrv.u32_ClkKHz =  72000; break;
        case eMMC_PLL_CLK__80M  : g_eMMCDrv.u32_ClkKHz =  80000; break;
        case eMMC_PLL_CLK__86M  : g_eMMCDrv.u32_ClkKHz =  86000; break;
        case eMMC_PLL_CLK_100M  : g_eMMCDrv.u32_ClkKHz = 100000; break;
        case eMMC_PLL_CLK_120M  : g_eMMCDrv.u32_ClkKHz = 120000; break;
        case eMMC_PLL_CLK_140M  : g_eMMCDrv.u32_ClkKHz = 140000; break;
        case eMMC_PLL_CLK_160M  : g_eMMCDrv.u32_ClkKHz = 160000; break;
        case eMMC_PLL_CLK_200M  : g_eMMCDrv.u32_ClkKHz = 200000; break;

        // clock_gen fcie clock
        case BIT_CLK_XTAL_12M   : g_eMMCDrv.u32_ClkKHz =  12000; break;
        case BIT_FCIE_CLK_20M   : g_eMMCDrv.u32_ClkKHz =  20000; break;
        case BIT_FCIE_CLK_32M   : g_eMMCDrv.u32_ClkKHz =  32000; break;
        case BIT_FCIE_CLK_36M   : g_eMMCDrv.u32_ClkKHz =  36000; break;
        case BIT_FCIE_CLK_40M   : g_eMMCDrv.u32_ClkKHz =  40000; break;
        case BIT_FCIE_CLK_43_2M : g_eMMCDrv.u32_ClkKHz =  43200; break;
        case BIT_FCIE_CLK_300K  : g_eMMCDrv.u32_ClkKHz =    300; break;
        case BIT_CLK_XTAL_24M   : g_eMMCDrv.u32_ClkKHz =  24000; break;
        case BIT_FCIE_CLK_48M   : g_eMMCDrv.u32_ClkKHz =  48000; break;

        default:
            eMMC_die(" ");
            break;
    }

    if(u16_ClkParam & eMMC_PLL_FLAG)
    {
        REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_GATING|BIT_FCIE_CLK_INVERSE|BIT_CLKGEN_FCIE_MASK);

        REG_FCIE_CLRBIT(reg_ckg_mpll, BIT_CKG_MPLL_GATING|BIT_CKG_MPLL_INVERSE|BIT_CLKGEN2_CLK_SEL_MASK);
        REG_FCIE_SETBIT(reg_ckg_mpll, BIT_CLKGEN2_CLK_SEL);

        eMMC_pll_setting(u16_ClkParam);

        if( g_eMMCDrv.u32_DrvFlag & DRV_FLAG_SPEED_HIGH ) // HS
        {
            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL|BIT_FCIE_CLK_EMMC_PLL_1X<<2);
            g_eMMCDrv.u32_ClkKHz >>=2;
        }
    }
    else
    {
        REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_GATING|BIT_FCIE_CLK_INVERSE|BIT_CLKGEN_FCIE_MASK);
        REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL|u16_ClkParam<<2);
    }

    #endif

    g_eMMCDrv.u16_ClkRegVal = (U16)u16_ClkParam;

    return eMMC_ST_SUCCESS;
}

U32 eMMC_clock_gating(void)
{
    eMMC_PlatformResetPre();
    g_eMMCDrv.u32_ClkKHz = 0;
    REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_GATING);// gate clock
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
    eMMC_PlatformResetPost();
    return eMMC_ST_SUCCESS;
}

U8 gau8_FCIEClkSel[eMMC_FCIE_VALID_CLK_CNT] = {
    #if defined(CONFIG_MSTAR_K6) || defined(CONFIG_MSTAR_K6LITE)
    BIT_FCIE_CLK_1X_48M,
    BIT_FCIE_CLK_1X_36M,
    BIT_FCIE_CLK_1X_27M,
    #else
    BIT_FCIE_CLK_48M,
    BIT_FCIE_CLK_36M,
    BIT_FCIE_CLK_20M,
    #endif
};

U32 eMMC_PlatformResetPre(void)
{
    return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{
    return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
    #if 1
    // for eMMC 4.5 HS200 need 1.8V, unify all eMMC IO power to 1.8V
    // works both for eMMC 4.4 & 4.5
    // eMMC_debug(0,0,"1.8V IO power for eMMC\n");
    // Irwin Tyan: set this bit to boost IO performance at low power supply.
    if((REG_FCIE(reg_emmc_test)&BIT0) == 0) {
        //eMMC_debug(0, 0, "eMMC Err: not 1.8V IO setting\n");
        REG_FCIE_SETBIT(reg_emmc_test, BIT0);// 1.8V must set this bit
        REG_FCIE_SETBIT(reg_emmc_test, BIT2); // atop patch
        REG_FCIE_CLRBIT(reg_nand_pad_driving, 0xffff);// 1.8V must set this bit
        REG_FCIE_SETBIT(reg_nand_pad_driving, 0xf);// 1.8V must set this bit
    }
    #else
    eMMC_debug(0,0,"3.3V IO power for eMMC\n");
    REG_FCIE_CLRBIT(reg_emmc_test, BIT0);// 3.3V must clear this bit
    #endif

    eMMC_pads_switch(EMMC_DEFO_SPEED_MODE);
    eMMC_clock_setting(FCIE_SLOWEST_CLK);

    return eMMC_ST_SUCCESS;

}

static U32 sgu32_MemGuard0 = 0xA55A;
eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
static U32 sgu32_MemGuard1 = 0x1289;

eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes

U32 eMMC_CheckIfMemCorrupt(void)
{
    if(0xA55A != sgu32_MemGuard0 || 0x1289 != sgu32_MemGuard1)
        return eMMC_ST_ERR_MEM_CORRUPT;

    return eMMC_ST_SUCCESS;
}

#endif
#else

#endif

