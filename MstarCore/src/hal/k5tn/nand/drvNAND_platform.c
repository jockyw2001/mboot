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

#include "drvNAND.h"

#if (defined(NAND_DRV_TV_MBOOT)&&NAND_DRV_TV_MBOOT)
#include "MsOS.h"


#if IF_IP_VERIFY
U8  au8_nand_clk_max;
#endif

NAND_DRIVER sg_NandDrv;
static UNFD_ALIGN0 U32 gau32_PartInfo[NAND_PARTITAION_BYTE_CNT/4]UNFD_ALIGN1;

void nand_SysPwrOff(void)
{
    nand_debug(UNFD_DEBUG_LEVEL, 1, " NO power off \n");
    nand_debug(UNFD_DEBUG_LEVEL, 1, " NO power off \n");
}

U32 nand_hw_timer_delay(U32 u32usTick)
{
    #if 0 // Use PIU timer

    U32 u32HWTimer = 0;
    volatile U16 u16TimerLow = 0;
    volatile U16 u16TimerHigh = 0;

    // reset HW timer
    REG_WRITE_UINT16(TIMER0_MAX_LOW, 0xFFFF);
    REG_WRITE_UINT16(TIMER0_MAX_HIGH, 0xFFFF);
    REG_WRITE_UINT16(TIMER0_ENABLE, 0);

    // start HW timer
    REG_SET_BITS_UINT16(TIMER0_ENABLE, 0x0001);

    while( u32HWTimer < 12*u32usTick ) // wait for u32usTick micro seconds
    {
        REG_READ_UINT16(TIMER0_CAP_LOW, u16TimerLow);
        REG_READ_UINT16(TIMER0_CAP_HIGH, u16TimerHigh);

        u32HWTimer = (u16TimerHigh<<16) | u16TimerLow;
    }

    REG_WRITE_UINT16(TIMER0_ENABLE, 0);

    #else // Use kernel udelay
    extern void udelay(unsigned long usec);
    U32 u32_us = u32usTick;

    while(u32_us>1000*20)
    {
        udelay(1000*20);
        u32_us -= 1000*20;
    }

    udelay(u32_us);
    #endif

    return u32usTick+1;
}

U32 nand_pads_release(void)
{
    return UNFD_ST_SUCCESS;
}

U32 nand_pads_init(void)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    pNandDrv->u16_Reg40_Signal =
            (BIT_NC_WP_AUTO | BIT_NC_WP_H | BIT_NC_CE_AUTO | BIT_NC_CE_H) &
            ~(BIT_NC_CHK_RB_EDGEn | BIT_NC_CE_SEL_MASK);

    return UNFD_ST_SUCCESS;
}

#if defined(FCIE4_DDR) && FCIE4_DDR
U32 nand_check_DDR_pad(void)
{
    return UNFD_ST_SUCCESS;
}
#endif

U32 nand_pads_switch(U32 u32EnableFCIE)
{
    U16 u16_NandMode;
    // Disable all pad in
    REG_CLR_BITS_UINT16(reg_allpad_in, BIT15);

    /*Check NAND Mode for PE setting*/
    REG_READ_UINT16(reg_nf_en, u16_NandMode);

    u16_NandMode &= REG_NAND_MODE_MASK;

    if(NAND_MODE0 == u16_NandMode )
    {
        REG_SET_BITS_UINT16(reg_hv_sync_out, BIT10|BIT11|BIT14|BIT15);
        REG_SET_BITS_UINT16(reg_s_gpio, BIT6|BIT7);
    }
    else if(NAND_MODE1 == u16_NandMode )
    {
        REG_SET_BITS_UINT16(reg_gpio_pe, BIT4|BIT12);
        REG_SET_BITS_UINT16(reg_ci_d_pe, BIT14|BIT15);

        REG_SET_BITS_UINT16(reg_ci_a_pe, 0x7FFF);
        REG_SET_BITS_UINT16(reg_ci_a_ps, 0x7FFF);
        REG_SET_BITS_UINT16(reg_ci_c_pe, 0x03FF);
        REG_SET_BITS_UINT16(reg_ci_c_ps, 0x03FF);
        REG_SET_BITS_UINT16(reg_ci_d_pe, 0x00FF);
        REG_SET_BITS_UINT16(reg_ci_d_ps, 0x00FF);

        REG_CLR_BITS_UINT16(NC_REG_2Fh, 0x1F00);
        REG_CLR_BITS_UINT16(FCIE_NC_REORDER, BIT14|BIT15);
    }
    else if(NAND_MODE2 == u16_NandMode)
    {
        REG_SET_BITS_UINT16(reg_gpio_pe, BIT4|BIT12);
        REG_SET_BITS_UINT16(reg_ci_d_pe, BIT14|BIT15);

        REG_SET_BITS_UINT16(reg_ci_a_pe, 0x7FFF);
        REG_SET_BITS_UINT16(reg_ci_a_ps, 0x7FFF);
        REG_CLR_BITS_UINT16(reg_ci_c_pe, 0x03FF);
        REG_CLR_BITS_UINT16(reg_ci_c_ps, 0x03FF);
        REG_CLR_BITS_UINT16(reg_ci_d_pe, 0x00FF);
        REG_CLR_BITS_UINT16(reg_ci_d_ps, 0x00FF);

        REG_CLR_BITS_UINT16(NC_REG_2Fh, 0x1F00);
        REG_CLR_BITS_UINT16(FCIE_NC_REORDER, BIT14|BIT15);
    }
    else if(NAND_MODE3 == u16_NandMode)
    {
        REG_SET_BITS_UINT16(reg_gpio_pe, BIT4|BIT12);
        REG_SET_BITS_UINT16(reg_ci_d_pe, BIT14|BIT15);

        REG_CLR_BITS_UINT16(reg_ci_a_pe, 0x7FFF);
        REG_CLR_BITS_UINT16(reg_ci_a_ps, 0x7FFF);
        REG_CLR_BITS_UINT16(reg_ci_c_pe, 0x03FF);
        REG_CLR_BITS_UINT16(reg_ci_c_ps, 0x03FF);
        REG_CLR_BITS_UINT16(reg_ci_d_pe, 0x00FF);
        REG_CLR_BITS_UINT16(reg_ci_d_ps, 0x00FF);

        REG_CLR_BITS_UINT16(NC_REG_2Fh, 0x1F00);
        REG_CLR_BITS_UINT16(FCIE_NC_REORDER, BIT14|BIT15);
    }
    else if(NAND_MODE4 == u16_NandMode)
    {
        REG_SET_BITS_UINT16(reg_gpio_pe, BIT4|BIT12);
        REG_SET_BITS_UINT16(reg_ci_d_pe, BIT14|BIT15);

        REG_SET_BITS_UINT16(reg_ci_a_pe, 0x7FFF);
        REG_SET_BITS_UINT16(reg_ci_a_ps, 0x7FFF);
        REG_SET_BITS_UINT16(reg_ci_c_pe, 0x03FF);
        REG_SET_BITS_UINT16(reg_ci_c_ps, 0x03FF);
        REG_SET_BITS_UINT16(reg_ci_d_pe, 0x00FF);
        REG_SET_BITS_UINT16(reg_ci_d_ps, 0x00FF);

        REG_CLR_BITS_UINT16(NC_REG_2Fh, 0x1F00);
        REG_CLR_BITS_UINT16(FCIE_NC_REORDER, BIT14|BIT15);
    }

    nand_debug(UNFD_DEBUG_LEVEL_LOW, 0,"reg_nf_en(%08X)=%04X\n", reg_nf_en, REG(reg_nf_en));
    nand_debug(UNFD_DEBUG_LEVEL_LOW, 0,"reg_allpad_in(%08X)=%04X\n", reg_allpad_in, REG(reg_allpad_in));

    return UNFD_ST_SUCCESS;
}

U32 nand_clock_setting(U32 u32ClkParam)
{
    /*div4 enable*/
    REG_SET_BITS_UINT16(reg_ckg_fcie, BIT_CLK_ENABLE);

    /*set FCIE 4x clock*/
    REG_CLR_BITS_UINT16(reg_ckg_fcie, BIT1|BIT0);
    REG_CLR_BITS_UINT16(reg_ckg_fcie, EMMC_CLK_MASK);
    REG_SET_BITS_UINT16(reg_ckg_fcie, u32ClkParam << 0);

    return UNFD_ST_SUCCESS;
}


void nand_DumpPadClk(void)
{
    U16 u16_i;

    nand_debug(0, 1, "clk setting: \n");
    nand_debug(0, 1, "  reg_ckg_fcie(0x%X):0x%x\n", reg_ckg_fcie, REG(reg_ckg_fcie));

    nand_debug(0, 1, "pad setting: \n");
    //fcie pad register
    nand_debug(0, 1, "  NC_REG_2Fh(0x%X):0x%x\n", NC_REG_2Fh, REG(NC_REG_2Fh));
    nand_debug(0, 1, "  NC_DDR_CTRL(0x%X):0x%x\n", NC_DDR_CTRL, REG(NC_DDR_CTRL));

    //chiptop pad register
    nand_debug(0, 1, "  reg_all_pad_in(0x%X):0x%x\n", reg_allpad_in, REG(reg_allpad_in));

    nand_debug(0, 1, "  reg_ci_a_pe (0x%08X):0x%04X\n", reg_ci_a_pe, REG(reg_ci_a_pe));
    nand_debug(0, 1, "  reg_ci_c_pe (0x%08X):0x%04X\n", reg_ci_c_pe, REG(reg_ci_c_pe));
    nand_debug(0, 1, "  reg_ci_d_pe (0x%08X):0x%04X\n", reg_ci_d_pe, REG(reg_ci_d_pe));
    nand_debug(0, 1, "  reg_gpio_pe (0x%08X):0x%04X\n", reg_gpio_pe, REG(reg_gpio_pe));
    nand_debug(0, 1, "  reg_ci_a_ps (0x%08X):0x%04X\n", reg_ci_a_ps, REG(reg_ci_a_ps));
    nand_debug(0, 1, "  reg_ci_c_ps (0x%08X):0x%04X\n", reg_ci_c_ps, REG(reg_ci_c_ps));
    nand_debug(0, 1, "  reg_ci_d_ps (0x%08X):0x%04X\n", reg_ci_d_ps, REG(reg_ci_d_ps));
    nand_debug(0, 1, "  reg_gpio_ps (0x%08X):0x%04X\n", reg_s_gpio, REG(reg_s_gpio));
	nand_debug(0, 1, "	reg_hv_sync_out (0x%08X):0x%04X\n", reg_hv_sync_out, REG(reg_hv_sync_out));

    nand_debug(0, 1, "  reg_nf_en(0x%X):0x%x\n\n", reg_nf_en, REG(reg_nf_en));

    // -------------------------------------------
    nand_debug(0, 1, "chiptop:");
    for(u16_i=0 ; u16_i<0x60 ; u16_i++)
    {
        if(0 == u16_i%8)
            nand_debug(0,0, "\n%02Xh:| ", u16_i);

        nand_debug(0, 0, "%04Xh ", REG(GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, u16_i)));
        if((u16_i & 0x7) == 0x7)
            nand_debug(0,0,"| %02Xh ", u16_i);
    }
    nand_debug(0, 0,"\n\n");

}


#if defined(DECIDE_CLOCK_BY_NAND) && DECIDE_CLOCK_BY_NAND
#define MAX_UNFD(a,b) ((a) > (b) ? (a) : (b))

U32 nand_config_timing(U16 u16_1T)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U16 u16_DefaultTRR;
    U16 u16_DefaultTCS;
    U16 u16_DefaultTWW;
    U16 u16_DefaultRX40Cmd;
    U16 u16_DefaultRX40Adr;
    U16 u16_DefaultRX56;
    U16 u16_DefaultTADL;
    U16 u16_DefaultTCWAW;
    #if defined(NC_TWHR_TCLHZ) && NC_TWHR_TCLHZ
    U16 u16_DefaultTCLHZ = 4;
    #endif
    U16 u16_DefaultTWHR;
    U16 u16_Tmp, u16_Cnt;
    U16 u16_Tmp2, u16_Cnt2;


    #if defined(FCIE4_DDR) && FCIE4_DDR
    if(pNandDrv->u16_Reg58_DDRCtrl&BIT_DDR_ONFI)
    {
        u16_DefaultTRR = NC_ONFI_DEFAULT_TRR;
        u16_DefaultTCS = NC_ONFI_DEFAULT_TCS;
        u16_DefaultTWW = NC_ONFI_DEFAULT_TWW;
        u16_DefaultRX40Cmd = NC_ONFI_DEFAULT_RX40CMD;
        u16_DefaultRX40Adr = NC_ONFI_DEFAULT_RX40ADR;
        u16_DefaultRX56 = NC_ONFI_DEFAULT_RX56;
        u16_DefaultTADL = NC_ONFI_DEFAULT_TADL;
        u16_DefaultTCWAW = NC_ONFI_DEFAULT_TCWAW;
        u16_DefaultTWHR = NC_ONFI_DEFAULT_TWHR;
    }
    else if(pNandDrv->u16_Reg58_DDRCtrl&BIT_DDR_TOGGLE)
    {
        u16_DefaultTRR = NC_TOGGLE_DEFAULT_TRR;
        u16_DefaultTCS = NC_TOGGLE_DEFAULT_TCS;
        u16_DefaultTWW = NC_TOGGLE_DEFAULT_TWW;
        u16_DefaultRX40Cmd = NC_TOGGLE_DEFAULT_RX40CMD;
        u16_DefaultRX40Adr = NC_TOGGLE_DEFAULT_RX40ADR;
        u16_DefaultRX56 = NC_TOGGLE_DEFAULT_RX56;
        u16_DefaultTADL = NC_TOGGLE_DEFAULT_TADL;
        u16_DefaultTCWAW = NC_TOGGLE_DEFAULT_TCWAW;
        u16_DefaultTWHR = NC_TOGGLE_DEFAULT_TWHR;
    }
    else
    #endif
    {
        u16_DefaultTRR = NC_SDR_DEFAULT_TRR;
        u16_DefaultTCS = NC_SDR_DEFAULT_TCS;
        u16_DefaultTWW = NC_SDR_DEFAULT_TWW;
        u16_DefaultRX40Cmd = NC_SDR_DEFAULT_RX40CMD;
        u16_DefaultRX40Adr = NC_SDR_DEFAULT_RX40ADR;
        u16_DefaultRX56 = NC_SDR_DEFAULT_RX56;
        u16_DefaultTADL = NC_SDR_DEFAULT_TADL;
        u16_DefaultTCWAW = NC_SDR_DEFAULT_TCWAW;
        u16_DefaultTWHR = NC_SDR_DEFAULT_TWHR;
    }

    #if defined(NC_INST_DELAY) && NC_INST_DELAY
    // Check CMD_END
    u16_Tmp = MAX_UNFD(pNandDrv->u16_tWHR, pNandDrv->u16_tCWAW);
    u16_Cnt = (u16_Tmp+u16_1T-1)/u16_1T;

    if(u16_DefaultRX40Cmd >= u16_Cnt)
        u16_Cnt = 0;
    else if(u16_Cnt-u16_DefaultRX40Cmd > 0xFF)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Cnt -= u16_DefaultRX40Cmd;

    // Check ADR_END
    u16_Tmp2 = MAX_UNFD(MAX_UNFD(pNandDrv->u16_tWHR, pNandDrv->u16_tADL), pNandDrv->u16_tCCS);
    u16_Cnt2 = (u16_Tmp2+u16_1T-1)/u16_1T;

    if(u16_DefaultRX40Adr >= u16_Cnt2)
        u16_Cnt2 = 0;
    else if(u16_Cnt2-u16_DefaultRX40Adr > 0xFF)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Cnt2 -= u16_DefaultRX40Adr;

    // get the max cnt
    u16_Cnt = MAX_UNFD(u16_Cnt, u16_Cnt2);

    pNandDrv->u16_Reg40_Signal &= ~(0x00FF<<8);
    pNandDrv->u16_Reg40_Signal |= (u16_Cnt<<8);
    nand_debug(UNFD_DEBUG_LEVEL_HIGH,1, "u16_Reg40_Signal =  %X\n",pNandDrv->u16_Reg40_Signal);
    #endif

    #if defined(NC_HWCMD_DELAY) && NC_HWCMD_DELAY
    u16_Cnt = (pNandDrv->u16_tRHW+u16_1T-1)/u16_1T + 2;

    if(u16_DefaultRX56 >= u16_Cnt)
        u16_Cnt = 0;
    else if(u16_Cnt-u16_DefaultRX56 > 0xFF)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Cnt -= u16_DefaultRX56;

    pNandDrv->u16_Reg56_Rand_W_Cmd &= ~(0x00FF<<8);
    pNandDrv->u16_Reg56_Rand_W_Cmd |= (u16_Cnt<<8);
    nand_debug(UNFD_DEBUG_LEVEL_HIGH,1, "u16_Reg56_Rand_W_Cmd =  %X\n",pNandDrv->u16_Reg56_Rand_W_Cmd);
    #endif

    #if defined(NC_TRR_TCS) && NC_TRR_TCS
    u16_Cnt = (pNandDrv->u8_tRR+u16_1T-1)/u16_1T + 2;

    if(u16_DefaultTRR >= u16_Cnt)
        u16_Cnt = 0;
    else if(u16_Cnt-u16_DefaultTRR > 0x0F)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Cnt -= u16_DefaultTRR;

    u16_Tmp = (pNandDrv->u8_tCS+u16_1T-1)/u16_1T + 2;

    if(u16_DefaultTCS >= u16_Tmp)
        u16_Tmp = 0;
    else if(u16_Tmp-u16_DefaultTCS > 0x0F)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Tmp -= u16_DefaultTCS;

    u16_Tmp2 = (pNandDrv->u16_tWW+u16_1T-1)/u16_1T + 2;

    if(u16_DefaultTWW >= u16_Tmp2)
        u16_Tmp2 = 0;
    else if(u16_Tmp2-u16_DefaultTWW > 0x0F)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Tmp2 -= u16_DefaultTWW;

    u16_Cnt2 = MAX_UNFD(u16_Tmp, u16_Tmp2);

    pNandDrv->u16_Reg59_LFSRCtrl &= ~(0x00FF);
    pNandDrv->u16_Reg59_LFSRCtrl |= (u16_Cnt|(u16_Cnt2<<4));
    nand_debug(UNFD_DEBUG_LEVEL_HIGH,1, "u16_Reg59_LFSRCtrl =  %X\n",pNandDrv->u16_Reg59_LFSRCtrl);
    #endif

    #if defined(NC_TCWAW_TADL) && NC_TCWAW_TADL
    u16_Cnt = (pNandDrv->u16_tADL + u16_1T - 1) / u16_1T + 2;

    if(u16_DefaultTADL > u16_Cnt)
        u16_Cnt = 0;
    else if(u16_Cnt-u16_DefaultTADL > 0xFF)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Cnt -= u16_DefaultTADL;

    u16_Cnt2 = (pNandDrv->u16_tCWAW + u16_1T - 1) / u16_1T + 2;

    if(u16_DefaultTADL > u16_Cnt2)
        u16_Cnt2 = 0;
    else if(u16_Cnt2-u16_DefaultTCWAW > 0xFF)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Cnt2 -= u16_DefaultTCWAW;

    pNandDrv->u16_Reg5D_tCWAW_tADL &= ~(0xFFFF);
    pNandDrv->u16_Reg5D_tCWAW_tADL |= (u16_Cnt|(u16_Cnt2<<8));
    nand_debug(UNFD_DEBUG_LEVEL_HIGH,1, "u16_Reg5D_tCWAW_tADL =  %X\n",pNandDrv->u16_Reg5D_tCWAW_tADL);
    #endif

    #if defined(NC_TWHR_TCLHZ) && NC_TWHR_TCLHZ
    u16_Cnt = (pNandDrv->u8_tCLHZ + u16_1T - 1) / u16_1T + 2;

    if(u16_DefaultTCLHZ > u16_Cnt)
        u16_Cnt = 0;
    else if(u16_Cnt-u16_DefaultTCLHZ > 0xF)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Cnt -= u16_DefaultTCLHZ;

    u16_Cnt2 = (pNandDrv->u16_tWHR + u16_1T - 1) / u16_1T + 2;

    if(u16_DefaultTWHR > u16_Cnt2)
        u16_Cnt2 = 0;
    else if(u16_Cnt2-u16_DefaultTWHR > 0xFF)
        return UNFD_ST_ERR_INVALID_PARAM;
    else
        u16_Cnt2 -= u16_DefaultTWHR;

    pNandDrv->u16_Reg5A_tWHR_tCLHZ &= ~(0xFFFF);
    pNandDrv->u16_Reg5A_tWHR_tCLHZ |= ((u16_Cnt&0xF)|(u16_Cnt2<<8));
    nand_debug(UNFD_DEBUG_LEVEL_HIGH,1, "u16_Reg5A_tWHR_tCLHZ =  %X\n",pNandDrv->u16_Reg5A_tWHR_tCLHZ);
    #endif

    NC_Config();
    return UNFD_ST_SUCCESS;
}

U32 nand_find_timing(U8 *pu8_ClkIdx, U8 u8_find_DDR_timg)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 au32_1TTable[NFIE_CLK_TABLE_CNT] = NFIE_1T_TABLE;
    U32 au32_ClkValueTable[NFIE_CLK_TABLE_CNT] = NFIE_CLK_VALUE_TABLE;

    #if defined(FCIE4_DDR) && FCIE4_DDR
    U32 au32_4Clk1TTable[NFIE_4CLK_TABLE_CNT] = NFIE_4CLK_1T_TABLE;
    U32 au32_4ClkValueTable[NFIE_4CLK_TABLE_CNT] = NFIE_4CLK_VALUE_TABLE;
    #endif
    U32 u32_Clk;
    U16 u16_SeqAccessTime, u16_Tmp, u16_Tmp2, u16_1T, u16_RE_LATCH_DELAY;
    S8 s8_ClkIdx;

    s8_ClkIdx = 0;
    u16_1T = 0;

    if(pNandDrv->u16_Reg58_DDRCtrl&BIT_DDR_ONFI)
    {
        u16_SeqAccessTime = 10;
    }
    else if(pNandDrv->u16_Reg58_DDRCtrl&BIT_DDR_TOGGLE)
    {
        u16_Tmp = MAX_UNFD(MAX_UNFD(2*pNandDrv->u8_tRP, 2*pNandDrv->u8_tREH), pNandDrv->u16_tRC);
        u16_Tmp2 = MAX_UNFD(MAX_UNFD(pNandDrv->u8_tWP, pNandDrv->u8_tWH), (pNandDrv->u16_tWC+1)/2);
        u16_SeqAccessTime = MAX_UNFD(u16_Tmp, u16_Tmp2);
    }
    else
    {
        u16_Tmp = MAX_UNFD(MAX_UNFD(pNandDrv->u8_tRP, pNandDrv->u8_tREH), (pNandDrv->u16_tRC+1)/2);
        u16_Tmp2 = MAX_UNFD(MAX_UNFD(pNandDrv->u8_tWP, pNandDrv->u8_tWH), (pNandDrv->u16_tWC+1)/2);
        u16_SeqAccessTime = MAX_UNFD(u16_Tmp, u16_Tmp2);

        u16_Tmp = (pNandDrv->u8_tREA + NAND_SEQ_ACC_TIME_TOL)/2;
        u16_Tmp2 = u16_SeqAccessTime;
        u16_SeqAccessTime = MAX_UNFD(u16_Tmp, u16_Tmp2);
    }

    u32_Clk = 1000000000/((U32)u16_SeqAccessTime); //FCIE5 needs to be update .....

    if(!u8_find_DDR_timg)
    {
        for(s8_ClkIdx =  0; s8_ClkIdx <= NFIE_CLK_TABLE_CNT - 1; s8_ClkIdx ++)
        {
            if(u32_Clk <= au32_ClkValueTable[s8_ClkIdx])
            {
                break;
            }
        }
    }
    else
    {
        #if defined(FCIE4_DDR) && FCIE4_DDR
        for(s8_ClkIdx =  0; s8_ClkIdx <= NFIE_4CLK_TABLE_CNT - 1; s8_ClkIdx ++)
        {
            if(u32_Clk < au32_4ClkValueTable[s8_ClkIdx])
            {
                break;
            }
        }
        #endif
    }
    s8_ClkIdx --;


RETRY:
    if(s8_ClkIdx<0)
        return UNFD_ST_ERR_INVALID_PARAM;
    if(!u8_find_DDR_timg)
        u16_1T = au32_1TTable[s8_ClkIdx];
    #if defined(FCIE4_DDR) && FCIE4_DDR
    else
        u16_1T = au32_4Clk1TTable[s8_ClkIdx];
    #endif

    if(nand_config_timing(u16_1T) != UNFD_ST_SUCCESS)
    {
        s8_ClkIdx--;
        goto RETRY;
    }

    u16_RE_LATCH_DELAY = BIT_NC_LATCH_DATA_1_0_T;

    pNandDrv->u16_Reg57_RELatch &= ~BIT_NC_LATCH_DATA_MASK;
    pNandDrv->u16_Reg57_RELatch |= u16_RE_LATCH_DELAY;

    *pu8_ClkIdx = (U8)s8_ClkIdx;

    return UNFD_ST_SUCCESS;

}


#endif


// [nand_config_clock] the entry for clock auto-config (by DECIDE_CLOCK_BY_NAND)
// [nand_find_timing]   find a FCIE clock
// [nand_config_timing] set other auxiliary parameters (cycle count)
//                                if timing parameters in nni are 0, would use default cycle count.
U32 nand_config_clock(U16 u16_SeqAccessTime)
{
    #if defined(DECIDE_CLOCK_BY_NAND) && DECIDE_CLOCK_BY_NAND
    NAND_DRIVER * pNandDrv = drvNAND_get_DrvContext_address();
    U32 u32_Err = 0;
    U32 au32_ClkTable[NFIE_CLK_TABLE_CNT] = NFIE_CLK_TABLE;
    char *ClkStrTable[NFIE_CLK_TABLE_CNT] = NFIE_CLK_TABLE_STR;

    #if defined(FCIE4_DDR) && FCIE4_DDR
    U32 au32_4ClkTable[NFIE_4CLK_TABLE_CNT] = NFIE_4CLK_TABLE;
    char *Clk4StrTable[NFIE_4CLK_TABLE_CNT] = NFIE_4CLK_TABLE_STR;
    U32 au32_1TTable[NFIE_4CLK_TABLE_CNT] = NFIE_4CLK_1T_TABLE;
    #endif

    U8 u8_ClkIdx = 0;

    #if defined(FCIE4_DDR) && FCIE4_DDR           //DDR enable of FCIE4
    if(pNandDrv->u16_Reg58_DDRCtrl&BIT_DDR_MASM)
    {
        if(pNandDrv->tDefaultDDR.u8_DdrTiming == 0)
        {
            nand_detect_ddr_timing();
        }
        else
        {
            pNandDrv->u32_minClk = au32_4ClkTable[pNandDrv->tMinDDR.u8_ClkIdx];
            pNandDrv->u32_Clk = au32_4ClkTable[pNandDrv->tDefaultDDR.u8_ClkIdx];
            #if defined(FCIE4_DDR_EMMC_PLL) && FCIE4_DDR_EMMC_PLL
            NC_FCIE4SetInterface_EMMC_PLL(1, pNandDrv->tDefaultDDR.u8_DqsMode,
                 pNandDrv->tDefaultDDR.u8_DdrTiming);
            #else
            NC_FCIE4SetInterface(1, pNandDrv->tDefaultDDR.u8_DqsMode,
                pNandDrv->tDefaultDDR.u8_DelayCell, pNandDrv->tDefaultDDR.u8_DdrTiming);
            #endif

            if(nand_config_timing(au32_1TTable[pNandDrv->tDefaultDDR.u8_ClkIdx]) != UNFD_ST_SUCCESS)
            {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Err, NAND, Cannot config nand timing\n");
                nand_die();
                return(u32_Err);
            }
        }
        nand_debug(UNFD_DEBUG_LEVEL,1,"ok, get default DDR timing: 2Ch:%X, 57h:%X\n",
                    pNandDrv->u16_Reg2C_SMStatus, pNandDrv->u16_Reg57_RELatch);
        u8_ClkIdx = pNandDrv->tDefaultDDR.u8_ClkIdx;
        printf("FCIE is set to %sHz\n", Clk4StrTable[u8_ClkIdx]);
    }
    else
    #endif
    {
        u32_Err = nand_find_timing(&u8_ClkIdx, 0);
        if(u32_Err != UNFD_ST_SUCCESS)
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Err, NAND, Cannot config nand timing\n");
            nand_die();
            return(u32_Err);
        }
        pNandDrv->u32_Clk = au32_ClkTable[u8_ClkIdx];
        printf("FCIE is set to %sHz\n", ClkStrTable[u8_ClkIdx]);
    }

    nand_clock_setting(pNandDrv->u32_Clk);
    REG_WRITE_UINT16(NC_LATCH_DATA, pNandDrv->u16_Reg57_RELatch);
    //nand_debug(UNFD_DEBUG_LEVEL, 0,"reg_ckg_fcie(%08X)=%08X\n", reg_ckg_fcie, REG(reg_ckg_fcie));
    //printf("RE LATCH is set to %X\n", pNandDrv->u16_Reg57_RELatch);

  // timing defined by
  //  - nand_clock_setting
  //  - default auxiliary timing parameters
  #endif

  return UNFD_ST_SUCCESS;
}


void nand_set_WatchDog(U8 u8_IfEnable)
{
  // do nothing in Linux
}

void nand_reset_WatchDog(void)
{
  // do nothing in Linux
}

U32 nand_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt, int mode)
{
    flush_cache(u32_DMAAddr, u32_ByteCnt);
    Chip_Flush_Memory();
    return  (u32_DMAAddr & 0x1fffffff);
}

U32 nand_translate_DMA_MIUSel(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
    return 0;
}

void nand_flush_miu_pipe(void)
{

}

void nand_check_blank_page_post_read(U8* main, U8* spare)
{

}

void nand_read_dma_post_flush(U32 u32_DMAAddr, U32 u32_ByteCnt)
{

}

void *drvNAND_get_DrvContext_address(void) // exposed API
{
  return &sg_NandDrv;
}

void *drvNAND_get_DrvContext_PartInfo(void)
{
    return (void*)((U32)gau32_PartInfo);
}

static UNFD_ALIGN0 U32 drvNandPageBuf[8192] UNFD_ALIGN1; /* 8192*4 Bytes */
static UNFD_ALIGN0 U32 drvNandSpareBuf[640] UNFD_ALIGN1; /*   640*4 Bytes */

U32 NC_PlatformResetPre(void)
{
  //HalMiuMaskReq(MIU_CLT_FCIE);
  return UNFD_ST_SUCCESS;
}

U32 NC_PlatformResetPost(void)
{
    U16 u16_Reg;

    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    REG_WRITE_UINT16(NC_PATH_CTL, BIT_NC_EN);

    REG_READ_UINT16(FCIE_NC_CIFD_BASE, u16_Reg); // dummy read for CIFD clock

    REG_READ_UINT16(FCIE_NC_CIFD_BASE, u16_Reg); // dummy read for CIFD clock   twice said by designer

    REG_WRITE_UINT16(NC_WIDTH, FCIE_REG41_VAL);  // duty cycle 3:1 in 86Mhz (depends on chip config)

    #if defined (DECIDE_CLOCK_BY_NAND) && DECIDE_CLOCK_BY_NAND
    REG_WRITE_UINT16(NC_LATCH_DATA, pNandDrv->u16_Reg57_RELatch);
    #else
    #if defined(REG57_ECO_FIX_INIT_VALUE)
    pNandDrv->u16_Reg57_RELatch = REG57_ECO_FIX_INIT_VALUE;
    REG_WRITE_UINT16(NC_LATCH_DATA, REG57_ECO_FIX_INIT_VALUE);
    #endif

    #endif

    /*HW bug
    In order to fix the front data overlapped by tail data of spare in CIFD when spare byte > 512
    But when ecc correctable Not in first sector, the following sector spare data will not be copied to CIFD.

    when spare > 512 using spare dma
    */
    REG_SET_BITS_UINT16(NC_DDR_CTRL, BIT7);
    pNandDrv->u16_Reg58_DDRCtrl |= BIT7;

    return UNFD_ST_SUCCESS;
}


U32 NC_PlatformInit(void)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    nand_pads_init();
    pNandDrv->u8_WordMode = 0; // TV/Set-Top Box projects did not support x16 NAND flash
    nand_pads_switch(NAND_PAD_BYPASS_MODE);
    pNandDrv->u8_PadMode = NAND_PAD_BYPASS_MODE;

    REG_WRITE_UINT16(NC_PATH_CTL, 0x20);

    #if defined (DECIDE_CLOCK_BY_NAND) && DECIDE_CLOCK_BY_NAND
    pNandDrv->u32_Clk =FCIE3_SW_SLOWEST_CLK;
    nand_clock_setting(FCIE3_SW_SLOWEST_CLK);
    pNandDrv->u16_Reg57_RELatch = 0;
    REG_WRITE_UINT16(NC_LATCH_DATA, pNandDrv->u16_Reg57_RELatch);
    #else
    pNandDrv->u32_Clk =FCIE3_SW_DEFAULT_CLK;
    nand_clock_setting(FCIE3_SW_DEFAULT_CLK);
    #endif

    pNandDrv->pu8_PageDataBuf= (U8*)drvNandPageBuf;
    pNandDrv->pu8_PageSpareBuf = (U8*)drvNandSpareBuf;

    return UNFD_ST_SUCCESS;
}

void HWTimer_Start(void)
{
    // reset HW timer
    REG_WRITE_UINT16(TIMER0_MAX_LOW, 0xFFFF);
    REG_WRITE_UINT16(TIMER0_MAX_HIGH, 0xFFFF);
    REG_WRITE_UINT16(TIMER0_ENABLE, 0);

    // start HW timer
    REG_SET_BITS_UINT16(TIMER0_ENABLE, 0x0001);

}

U32 HWTimer_End(void)
{
    U32 u32HWTimer = 0;
    U16 u16TimerLow = 0;
    U16 u16TimerHigh = 0;

    // Get HW timer
    REG_READ_UINT16(TIMER0_CAP_LOW, u16TimerLow);
    REG_READ_UINT16(TIMER0_CAP_HIGH, u16TimerHigh);

    u32HWTimer = (u16TimerHigh<<16) | u16TimerLow;

    return u32HWTimer;
}

#else
  #error "Error! no platform functions."
#endif
