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

#include "eMMC.h"

#if defined(UNIFIED_eMMC_DRIVER) && UNIFIED_eMMC_DRIVER

#if defined(ENABLE_EMMC_POWER_SAVING_MODE) && ENABLE_EMMC_POWER_SAVING_MODE

void eMMC_Prepare_Power_Saving_Mode_Queue(void)
{
    REG_FCIE_SETBIT(reg_pwrgd_int_glirm, BIT_PWRGD_INT_GLIRM_EN);

    REG_FCIE_CLRBIT(reg_pwrgd_int_glirm, BIT_PWEGD_INT_GLIRM_MASK);
    REG_FCIE_SETBIT(reg_pwrgd_int_glirm, (0x2<<10));
    REG_FCIE_CLRBIT(FCIE_PWR_SAVE_CTL, BIT_BAT_SD_POWER_SAVE_MASK);

    /* (1) Clear HW Enable */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x00), 0x0000);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x01),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x07);

    /* (2) Clear All Interrupt */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x02), 0xffff);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x03),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x00);

    /* (3) Clear SD MODE Enable */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x04), 0x0000);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x05),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0B);

    /* (4) Clear SD CTL Enable */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x06), 0x0000);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x07),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0C);

    /* (5) Reset Start */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x08), 0x0000);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x09),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x3F);

    /* (6) Reset End */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x0A), 0x0001);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x0B),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x3F);

    /* (7) Set "FCIE_DDR_MODE" */
    #if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x0C), 0x1100);
    #elif defined(ENABLE_eMMC_HS200) && ENABLE_eMMC_HS200
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x0C), 0x1000);
    #elif defined(ENABLE_eMMC_ATOP) && ENABLE_eMMC_ATOP
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x0C), 0x0180);
    #endif
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x0D),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0F);

    /* (8) Set "SD_MOD" */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x0E), 0x0021);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x0F),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0B);

    /* (9) Enable "reg_sd_en" */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x10), 0x0001);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x11),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x07);

    /* (10) Command Content, IDLE */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x12), 0x0040);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x13),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x20);

    /* (11) Command Content, STOP */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x14), 0x0000);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x15),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x21);

    /* (12) Command Content, STOP */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x16), 0x0000);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x17),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x22);

    /* (13) Command & Response Size */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x18), 0x0500);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x19),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0A);

    /* (14) Enable Interrupt, SD_CMD_END */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x1A), 0x0002);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x1B),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x01);

    /* (15) Command Enable + job Start */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x1C), 0x0044);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x1D),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0C);

    /* (16) Wait Interrupt */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x1E), 0x0000);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x1F),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WINT);

    /* (17) STOP */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x20), 0x0000);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x21),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_STOP);

    REG_FCIE_CLRBIT(FCIE_PWR_SAVE_CTL, BIT_SD_POWER_SAVE_RST);
    REG_FCIE_SETBIT(FCIE_PWR_SAVE_CTL, BIT_SD_POWER_SAVE_RST);

    REG_FCIE_SETBIT(FCIE_PWR_SAVE_CTL, BIT_POWER_SAVE_MODE|BIT_POWER_SAVE_MODE_INT_EN);
}

#endif

void eMMC_DumpPadClk(void)
{
    //---------------------------------------------------------------------
    eMMC_debug(0, 0, "[pad setting]: ");
    switch(g_eMMCDrv.u8_PadType)
    {
        case FCIE_eMMC_BYPASS:      eMMC_debug(0, 0, "FCIE_eMMC_BYPASS\n");     break;
        case FCIE_eMMC_SDR:         eMMC_debug(0, 0, "FCIE_eMMC_SDR\n");        break;
        case FCIE_eMMC_DDR:         eMMC_debug(0, 0, "FCIE_eMMC_DDR\n");        break;
        default:
            eMMC_debug(0, 0, "eMMC Err: Pad unknown, %d\n", g_eMMCDrv.u8_PadType); eMMC_die("\n");
            break;
    }

    eMMC_debug(0, 0, "FCIE_DDR_MODE = %04Xh\n", REG_FCIE(FCIE_DDR_MODE));

    eMMC_debug(0, 0, "reg_ckg_fcie = %04Xh \n", REG_FCIE(reg_ckg_fcie));
}

U32 eMMC_pads_switch(U32 u32Mode)
{
    g_eMMCDrv.u8_PadType = u32Mode;

    // chiptop
    REG_FCIE_CLRBIT(reg_chiptop_0x08, BIT_SD_CONFIG);
    REG_FCIE_CLRBIT(reg_chiptop_0x08, BIT_NAND_MODE);
    REG_FCIE_SETBIT(reg_chiptop_0x08, BIT_EMMC_CONFIG);
    REG_FCIE_CLRBIT(reg_chiptop_0x50, BIT_ALL_PAD_IN);

    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE,
        BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL|BIT_DDR_EN|BIT_8BIT_MACRO_EN);

    // emmc_pll
    REG_FCIE_CLRBIT(reg_emmcpll_0x1c, BIT9);

    REG_FCIE_CLRBIT(reg_emmcpll_0x68, BIT0);

    REG_FCIE_CLRBIT(reg_emmcpll_0x6a, BIT0|BIT1);

    REG_FCIE_W(reg_emmcpll_0x6b, 0x0000);

    REG_FCIE_CLRBIT(reg_emmcpll_0x6d, BIT0);

    switch(u32Mode)
    {
        case FCIE_eMMC_BYPASS:
            // Actually, small macro
            eMMC_debug(eMMC_DEBUG_LEVEL_LOW, 0, "FCIE_eMMC_BYPASS\n");

            // fcie
            REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL);

            break;

        case FCIE_eMMC_SDR:
            eMMC_debug(eMMC_DEBUG_LEVEL_LOW, 0, "FCIE_eMMC_SDR\n");

            // fcie
            REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_8BIT_MACRO_EN);

            // emmc_pll
            REG_FCIE_SETBIT(reg_emmcpll_0x1c, BIT9);

            REG_FCIE_SETBIT(reg_emmcpll_0x68, BIT0);

            break;

        case FCIE_eMMC_DDR:
            eMMC_debug(eMMC_DEBUG_LEVEL_LOW, 0, "FCIE_eMMC_DDR\n");

            // fcie
            REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_DDR_EN|BIT_8BIT_MACRO_EN);

            // emmc_pll
            REG_FCIE_SETBIT(reg_emmcpll_0x1c, BIT9);

            REG_FCIE_SETBIT(reg_emmcpll_0x68, BIT0);

            REG_FCIE_SETBIT(reg_emmcpll_0x6d, BIT0);

            if(g_eMMCDrv.u8_BUS_WIDTH == BIT_SD_DATA_WIDTH_4)
            {
                REG_FCIE_SETBIT(reg_emmcpll_0x6a, 1<<0);
                REG_FCIE_W(reg_emmcpll_0x6b, 0x0213);
            }
            else if(g_eMMCDrv.u8_BUS_WIDTH == BIT_SD_DATA_WIDTH_8)
            {
                REG_FCIE_SETBIT(reg_emmcpll_0x6a, 2<<0);
                REG_FCIE_W(reg_emmcpll_0x6b, 0x0113);
            }
            else
            {
                eMMC_debug(1, 1, "eMMC Err: set bus width before pad switch\n");
                return eMMC_ST_ERR_INVALID_PARAM;
            }

            REG_FCIE_SETBIT(reg_emmcpll_0x6f, BIT0);

            /* reset 8bit macro start*/
            REG_FCIE_CLRBIT(reg_emmcpll_0x6f, BIT1);
            REG_FCIE_SETBIT(reg_emmcpll_0x6f, BIT1);
            /* reset 8bit macro end */

            break;

        default:
            eMMC_debug(1, 1, "eMMC Err: wrong parameter for switch pad func\n");
            return eMMC_ST_ERR_PARAMETER;
            break;
    }

    return eMMC_ST_SUCCESS;
}

U32 eMMC_clock_setting(U16 u16_ClkParam)
{
    U16 u16_Tmp = 0;

    eMMC_PlatformResetPre();

    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

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

    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
    REG_FCIE_R(reg_ckg_fcie, u16_Tmp);
    udelay(7);
    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_GATING|BIT_FCIE_CLK_INVERSE|BIT_CLKGEN_FCIE_MASK|BIT_CKG_NFIE_DVI4_EN);

    if(u16_ClkParam & eMMC_4X_FLAG) {
        //eMMC_debug(0,0,"eMMC PLL: %Xh\n", u16_ClkParam);
        if( g_eMMCDrv.u32_DrvFlag & DRV_FLAG_DDR_MODE ) // DDR52
        {
            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_CKG_NFIE_DVI4_EN);
            g_eMMCDrv.u32_ClkKHz >>= 2;
        }
        else if( g_eMMCDrv.u32_DrvFlag & DRV_FLAG_SPEED_HIGH ) // HS
        {
            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_CKG_NFIE_DVI4_EN);
            g_eMMCDrv.u32_ClkKHz >>= 2;
        }
        else
            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_CKG_NFIE_DVI4_EN);
    }

    REG_FCIE_SETBIT(reg_ckg_fcie, (u16_ClkParam & 0xF)<<10);
    REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);

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
    BIT_FCIE_CLK_4X_192M,
    BIT_FCIE_CLK_4X_160M,
    BIT_FCIE_CLK_4X_144M,
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

    if((REG_FCIE(reg_emmc_test)&BIT0) == 0)
    {
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

    eMMC_pads_switch(FCIE_eMMC_BYPASS);

    eMMC_clock_setting(FCIE_SLOWEST_CLK);

    return eMMC_ST_SUCCESS;
}

#endif
