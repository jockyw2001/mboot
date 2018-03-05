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

void eMMC_DumpPadClk(void)
{
    //----------------------------------------------
    eMMC_debug(0, 0, "\n[clk setting]: %uKHz \n", g_eMMCDrv.u32_ClkKHz);
    eMMC_debug(0, 0, "FCIE CLK(0x%X):0x%04X\n", reg_ckg_fcie, REG_FCIE_U16(reg_ckg_fcie));

    //----------------------------------------------
    eMMC_debug(0, 0, "\n[pad setting]: ");
    switch(g_eMMCDrv.u8_PadType)
    {
        case FCIE_eMMC_BYPASS:  eMMC_debug(0,0,"BYPASS\n"); break;
        case FCIE_eMMC_SDR:     eMMC_debug(0,0,"SDR\n");    break;
        default:
            eMMC_debug(0,0,"eMMC Err: Pad unknown, %Xh\n", g_eMMCDrv.u8_PadType);
            eMMC_die("\n");
    }

    eMMC_debug(0, 0, "reg_chiptop_0x50 (0x%08X):0x%04X\n", reg_chiptop_0x50, REG_FCIE_U16(reg_chiptop_0x50));

    eMMC_debug(0, 0, "reg_chiptop_0x08 (0x%08X):0x%04X\n", reg_chiptop_0x08, REG_FCIE_U16(reg_chiptop_0x08));

    eMMC_debug(0, 0, "reg_chiptop_0x24 (0x%08X):0x%04X\n", reg_chiptop_0x24, REG_FCIE_U16(reg_chiptop_0x24));
    eMMC_debug(0, 0, "reg_chiptop_0x25 (0x%08X):0x%04X\n", reg_chiptop_0x25, REG_FCIE_U16(reg_chiptop_0x25));
    eMMC_debug(0, 0, "reg_chiptop_0x26 (0x%08X):0x%04X\n", reg_chiptop_0x26, REG_FCIE_U16(reg_chiptop_0x26));
    eMMC_debug(0, 0, "reg_chiptop_0x27 (0x%08X):0x%04X\n", reg_chiptop_0x27, REG_FCIE_U16(reg_chiptop_0x27));
    eMMC_debug(0, 0, "reg_chiptop_0x2a (0x%08X):0x%04X\n", reg_chiptop_0x2a, REG_FCIE_U16(reg_chiptop_0x2a));
    eMMC_debug(0, 0, "reg_chiptop_0x2b (0x%08X):0x%04X\n", reg_chiptop_0x2b, REG_FCIE_U16(reg_chiptop_0x2b));
    eMMC_debug(0, 0, "reg_chiptop_0x2c (0x%08X):0x%04X\n", reg_chiptop_0x2c, REG_FCIE_U16(reg_chiptop_0x2c));

    eMMC_debug(0, 0, "reg_chiptop_0x36 (0x%08X):0x%04X\n", reg_chiptop_0x36, REG_FCIE_U16(reg_chiptop_0x36));
    eMMC_debug(0, 0, "\n");
}


U32 eMMC_pads_switch(U32 u32_FCIE_IF_Type)
{
    U16 u16_eMMCMode;

    REG_FCIE_R(reg_chiptop_0x08, u16_eMMCMode);

    u16_eMMCMode &= BIT_EMMC_CFG_MASK;

    REG_FCIE_SETBIT(reg_chiptop_0x27, BIT_GPIO4_PE|BIT_GPIO2_PE);

    REG_FCIE_CLRBIT(reg_chiptop_0x26, BIT_GPIO8_PS);
    REG_FCIE_SETBIT(reg_chiptop_0x26, BIT_GPIO8_PE);

    if(u16_eMMCMode == BIT_EMMC_MODE1)
    {
        REG_FCIE_SETBIT(reg_chiptop_0x24, 0x7FFF);
        REG_FCIE_SETBIT(reg_chiptop_0x2a, 0x7FFF);
        REG_FCIE_SETBIT(reg_chiptop_0x25, 0x03FF);
        REG_FCIE_SETBIT(reg_chiptop_0x2b, 0x03FF);
        REG_FCIE_SETBIT(reg_chiptop_0x26, 0x00FF);
        REG_FCIE_SETBIT(reg_chiptop_0x2c, 0x00FF);
    }
    else if(u16_eMMCMode == BIT_EMMC_MODE2)
    {
        REG_FCIE_SETBIT(reg_chiptop_0x24, 0x7FFF);
        REG_FCIE_SETBIT(reg_chiptop_0x2a, 0x7FFF);
        REG_FCIE_CLRBIT(reg_chiptop_0x25, 0x03FF);
        REG_FCIE_CLRBIT(reg_chiptop_0x2b, 0x03FF);
        REG_FCIE_CLRBIT(reg_chiptop_0x26, 0x00FF);
        REG_FCIE_CLRBIT(reg_chiptop_0x2c, 0x00FF);
    }
    else if(u16_eMMCMode == BIT_EMMC_MODE3)
    {
        REG_FCIE_CLRBIT(reg_chiptop_0x24, 0x7FFF);
        REG_FCIE_CLRBIT(reg_chiptop_0x2a, 0x7FFF);
        REG_FCIE_CLRBIT(reg_chiptop_0x25, 0x03FF);
        REG_FCIE_CLRBIT(reg_chiptop_0x2b, 0x03FF);
        REG_FCIE_CLRBIT(reg_chiptop_0x26, 0x00FF);
        REG_FCIE_CLRBIT(reg_chiptop_0x2c, 0x00FF);
    }

    REG_FCIE_CLRBIT(FCIE_BOOT_CONFIG, BIT8|BIT9|BIT10|BIT11|BIT12);
    REG_FCIE_CLRBIT(reg_chiptop_0x50, BIT_ALL_PAD_IN);
    REG_FCIE_CLRBIT(FCIE_REG_2Dh, BIT_csreg_miu_wp_last2_sel|BIT_reg_pad_in_sel_sd);

    switch(u32_FCIE_IF_Type)
    {
        case FCIE_eMMC_BYPASS:
            eMMC_debug(eMMC_DEBUG_LEVEL_MEDIUM, 0,"eMMC pads: BYPASS\n");
            g_eMMCDrv.u8_PadType = FCIE_eMMC_BYPASS;
            break;

        case FCIE_eMMC_SDR:
            eMMC_debug(eMMC_DEBUG_LEVEL_MEDIUM, 0,"eMMC pads: SDR\n");
            REG_FCIE_SETBIT(FCIE_REG_2Dh,BIT_reg_pad_in_sel_sd);
            g_eMMCDrv.u8_PadType = FCIE_eMMC_SDR;
            break;

        default:
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: unknown interface: %X\n", u32_FCIE_IF_Type);
            return eMMC_ST_ERR_INVALID_PARAM;
    }

    return eMMC_ST_SUCCESS;
}


U32 eMMC_clock_setting(U16 u16_ClkParam)
{
    eMMC_PlatformResetPre();

    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
    REG_FCIE_SETBIT(reg_ckg_fcie, BIT_XTAL_CLK_SEL);

    switch(u16_ClkParam)
    {
        case BIT_FCIE_CLK_300K:     g_eMMCDrv.u32_ClkKHz = 300;     break;
        case BIT_FCIE_CLK_5M:       g_eMMCDrv.u32_ClkKHz = 5400;    break;
        case BIT_FCIE_CLK_12M:      g_eMMCDrv.u32_ClkKHz = 12000;   break;
        case BIT_FCIE_CLK_13M:      g_eMMCDrv.u32_ClkKHz = 13000;   break;
        case BIT_FCIE_CLK_18M:      g_eMMCDrv.u32_ClkKHz = 18000;   break;
        case BIT_FCIE_CLK_20M:      g_eMMCDrv.u32_ClkKHz = 20000;   break;
        case BIT_FCIE_CLK_27M:      g_eMMCDrv.u32_ClkKHz = 27000;   break;
        case BIT_FCIE_CLK_32M:      g_eMMCDrv.u32_ClkKHz = 32000;   break;
        case BIT_FCIE_CLK_36M:      g_eMMCDrv.u32_ClkKHz = 36000;   break;
        case BIT_FCIE_CLK_40M:      g_eMMCDrv.u32_ClkKHz = 40000;   break;
        case BIT_FCIE_CLK_43M:      g_eMMCDrv.u32_ClkKHz = 43200;   break;
        case BIT_FCIE_CLK_48M:      g_eMMCDrv.u32_ClkKHz = 48000;   break;
        default:
            eMMC_debug(eMMC_DEBUG_LEVEL_LOW,1,"eMMC Err: %Xh\n", eMMC_ST_ERR_INVALID_PARAM);
            return eMMC_ST_ERR_INVALID_PARAM;
    }

    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);
    REG_FCIE_SETBIT(reg_ckg_fcie, u16_ClkParam<<BIT_FCIE_CLK_SHIFT);

    eMMC_debug(eMMC_DEBUG_LEVEL_LOW, 1, "clk:%uKHz, Param:%Xh, fcie(%Xh):%Xh\n",
        g_eMMCDrv.u32_ClkKHz, u16_ClkParam, reg_ckg_fcie, REG_FCIE_U16(reg_ckg_fcie));

    g_eMMCDrv.u16_ClkRegVal = (U16)u16_ClkParam;

    eMMC_PlatformResetPost();

    return eMMC_ST_SUCCESS;
}


U32 eMMC_clock_gating(void)
{
    eMMC_PlatformResetPre();
    g_eMMCDrv.u32_ClkKHz = 0;
    REG_FCIE_W(reg_ckg_fcie, BIT_FCIE_CLK_Gate);
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
    eMMC_PlatformResetPost();
    return eMMC_ST_SUCCESS;
}


U8 gau8_FCIEClkSel[eMMC_FCIE_VALID_CLK_CNT]={
    BIT_FCIE_CLK_48M,
    BIT_FCIE_CLK_40M,
    BIT_FCIE_CLK_36M,
    BIT_FCIE_CLK_27M,
    BIT_FCIE_CLK_18M,

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
    eMMC_pads_switch(FCIE_DEFAULT_PAD);
    eMMC_clock_setting(FCIE_SLOWEST_CLK);

    return eMMC_ST_SUCCESS;
}
#endif
