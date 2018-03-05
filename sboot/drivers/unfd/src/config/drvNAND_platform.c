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
#if (!defined(CONFIG_NAND_FLASH)) || ((defined(CONFIG_MSTAR_KANO)||defined(CONFIG_MSTAR_CURRY)||defined(CONFIG_MSTAR_K6)||defined(CONFIG_MSTAR_K6LITE)) \
    &&(!defined(CONFIG_MSTAR_BFN_EN))&&(!defined(CONFIG_MSTAR_NS_UBOOT)))

#else

#include "drvNAND.h"

#if defined(CONFIG_MSTAR_NS_UBOOT)
static UNFD_ALIGN0 U8 drvNandPageBuf[1024*8] UNFD_ALIGN1;
static UNFD_ALIGN0 U8 drvNandSpareBuf[2048] UNFD_ALIGN1;
#endif

#if (defined(NAND_DRV_TV_MBOOT)&&NAND_DRV_TV_MBOOT)
U32 nand_hw_timer_delay(U32 u32usTick)
{
    U32 u32HWTimer = 0;
    volatile U16 u16TimerLow = 0;
    volatile U16 u16TimerHigh = 0;

    // reset HW timer
    REG_WRITE_UINT16(TIMER1_MAX_LOW, 0xFFFF);
    REG_WRITE_UINT16(TIMER1_MAX_HIGH, 0xFFFF);
    REG_WRITE_UINT16(TIMER1_ENABLE, 0);

    // start HW timer
    REG_SET_BITS_UINT16(TIMER1_ENABLE, 0x0001);

    while( u32HWTimer < 12*u32usTick ) // wait for u32usTick micro seconds
    {
        REG_READ_UINT16(TIMER1_CAP_LOW, u16TimerLow);
        REG_READ_UINT16(TIMER1_CAP_HIGH, u16TimerHigh);

        u32HWTimer = (u16TimerHigh<<16) | u16TimerLow;
    }

    REG_WRITE_UINT16(TIMER1_ENABLE, 0);

    return u32usTick+1;
}

U32 nand_pads_init(NAND_DRIVER *pNandDrv, U16 u16_NandMode)
{
    #if defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS)
    REG_CLR_BITS_UINT16(reg_nf_en, REG_NAND_MODE_MASK);
    REG_SET_BITS_UINT16(reg_nf_en, u16_NandMode);

    if(NAND_MODE1 == u16_NandMode )
    {
        REG_SET_BITS_UINT16(reg_ci_a_pe, 0x7FFF);
        REG_SET_BITS_UINT16(reg_ci_c_pe, 0x03FF);
        REG_SET_BITS_UINT16(reg_ci_d_pe, 0x00FF);
    }
    else if(NAND_MODE2 == u16_NandMode)
    {
        REG_SET_BITS_UINT16(reg_ci_a_pe, 0x7FFF);
        REG_CLR_BITS_UINT16(reg_ci_c_pe, 0x03FF);
        REG_CLR_BITS_UINT16(reg_ci_d_pe, 0x00FF);
    }
    else if(NAND_MODE3 == u16_NandMode)
    {
        REG_CLR_BITS_UINT16(reg_ci_a_pe, 0x7FFF);
        REG_CLR_BITS_UINT16(reg_ci_c_pe, 0x03FF);
        REG_CLR_BITS_UINT16(reg_ci_d_pe, 0x00FF);
    }

    REG_SET_BITS_UINT16(reg_gpio_pe, BIT11|BIT3);

    #elif defined(CONFIG_MSTAR_KAYLA)
    REG_CLR_BITS_UINT16(reg_nf_en, REG_NAND_MODE_MASK);
    REG_SET_BITS_UINT16(reg_nf_en, u16_NandMode);

    if(NAND_MODE1 == u16_NandMode )
    {
        REG_SET_BITS_UINT16(reg_ci_a_pe, 0x7FFF);
        REG_SET_BITS_UINT16(reg_ci_a_ps, 0x7FFF);
        REG_SET_BITS_UINT16(reg_ci_c_pe, 0x03FF);
        REG_SET_BITS_UINT16(reg_ci_c_ps, 0x03FF);
        REG_SET_BITS_UINT16(reg_ci_d_pe, 0x00FF);
        REG_SET_BITS_UINT16(reg_ci_d_ps, 0x00FF);
    }
    else if(NAND_MODE2 == u16_NandMode)
    {
        REG_SET_BITS_UINT16(reg_ci_a_pe, 0x7FFF);
        REG_SET_BITS_UINT16(reg_ci_a_ps, 0x7FFF);
        REG_CLR_BITS_UINT16(reg_ci_c_pe, 0x03FF);
        REG_CLR_BITS_UINT16(reg_ci_c_ps, 0x03FF);
        REG_CLR_BITS_UINT16(reg_ci_d_pe, 0x00FF);
        REG_CLR_BITS_UINT16(reg_ci_d_ps, 0x00FF);
    }
    else if(NAND_MODE3 == u16_NandMode)
    {
        REG_CLR_BITS_UINT16(reg_ci_a_pe, 0x7FFF);
        REG_CLR_BITS_UINT16(reg_ci_a_ps, 0x7FFF);
        REG_CLR_BITS_UINT16(reg_ci_c_pe, 0x03FF);
        REG_CLR_BITS_UINT16(reg_ci_c_ps, 0x03FF);
        REG_CLR_BITS_UINT16(reg_ci_d_pe, 0x00FF);
        REG_CLR_BITS_UINT16(reg_ci_d_ps, 0x00FF);
    }
    else if(NAND_MODE4 == u16_NandMode)
    {
        REG_SET_BITS_UINT16(reg_ci_a_pe, 0x7FFF);
        REG_SET_BITS_UINT16(reg_ci_a_ps, 0x7FFF);
        REG_SET_BITS_UINT16(reg_ci_c_pe, 0x03FF);
        REG_SET_BITS_UINT16(reg_ci_c_ps, 0x03FF);
        REG_SET_BITS_UINT16(reg_ci_d_pe, 0x00FF);
        REG_SET_BITS_UINT16(reg_ci_d_ps, 0x00FF);
    }

    REG_SET_BITS_UINT16(reg_gpio_pe, BIT11|BIT3);
    REG_SET_BITS_UINT16(reg_gpio_ps, BIT15|BIT14);

    #elif defined(CONFIG_MSTAR_K6)||defined(CONFIG_MSTAR_K6LITE)
    //We only use mode1 on K6
    REG_CLR_BITS_UINT16(reg_nf_en, REG_NAND_MODE_MASK);
    REG_SET_BITS_UINT16(reg_nf_en, u16_NandMode);

    REG_WRITE_UINT16(reg_nand_pe_0_15, 0xffff);
    REG_WRITE_UINT16(reg_nand_ps_0_15, 0x7aff);
    REG_SET_BITS_UINT16(reg_nand_pe_16_17, BIT0|BIT1);
    REG_SET_BITS_UINT16(reg_nand_ps_16_17, BIT4|BIT5);
	#elif defined(CONFIG_MSTAR_K5TN)
    REG_CLR_BITS_UINT16(reg_nf_en, REG_NAND_MODE_MASK);
    REG_SET_BITS_UINT16(reg_nf_en, u16_NandMode);
    
    
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
    else if(NAND_MODE4 == u16_NandMode )
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
    #endif
    pNandDrv->u16_Reg40_Signal =
            (BIT_NC_WP_AUTO | BIT_NC_WP_H | BIT_NC_CE_AUTO | BIT_NC_CE_H) &
            ~(BIT_NC_CHK_RB_EDGEn | BIT_NC_CE_SEL_MASK);

    return UNFD_ST_SUCCESS;
}


U32 nand_pads_switch(NAND_DRIVER *pNandDrv, U32 u32EnableFCIE)
{
    pNandDrv = pNandDrv;
    u32EnableFCIE = u32EnableFCIE;

    // Disable all pad in
    REG_CLR_BITS_UINT16(reg_allpad_in, BIT15);

    #if defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA) || defined(CONFIG_MSTAR_K6) || defined(CONFIG_MSTAR_K6LITE) || defined(CONFIG_MSTAR_K5TN)
    // Only support SDR mode, do nothing
    #endif

    #if defined(CONFIG_MSTAR_KANO) || defined(CONFIG_MSTAR_CURRY)

    // Bypass mode
    REG_SET_BITS_UINT16(reg_nf_en, BIT4);

    #endif

    return UNFD_ST_SUCCESS;
}


U32 nand_clock_setting(U32 u32ClkParam)
{
    #if defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA) || defined(CONFIG_MSTAR_K5TN)
    /*div4 enable*/
    REG_SET_BITS_UINT16(reg_ckg_fcie, BIT_CLK_ENABLE);

    /*set FCIE 4x clock*/
    REG_CLR_BITS_UINT16(REG_CLK_EMMC, BIT1|BIT0);
    REG_CLR_BITS_UINT16(REG_CLK_EMMC, EMMC_CLK_MASK);
    REG_SET_BITS_UINT16(REG_CLK_EMMC, u32ClkParam << 0);

    #endif

    #if defined(CONFIG_MSTAR_KANO) || defined(CONFIG_MSTAR_CURRY)

    REG_CLR_BITS_UINT16(reg_ckg_fcie, BIT2|BIT3|BIT4|BIT5);
    REG_SET_BITS_UINT16(reg_ckg_fcie, BIT_CLK_ENABLE);
    REG_CLR_BITS_UINT16(reg_ckg_fcie, BIT1|BIT0);

    REG_SET_BITS_UINT16(reg_ckg_fcie, u32ClkParam);


    #endif

    #if defined(CONFIG_MSTAR_K6) || defined(CONFIG_MSTAR_K6LITE)
    REG_SET_BITS_UINT16(reg_ckg_fcie, BIT_CLK_SELECT);
    REG_SET_BITS_UINT16(reg_clk_nfie_div4_en, BIT_CLK_DIV4_ENABLE);

    REG_CLR_BITS_UINT16(reg_ckg_fcie, BIT9|BIT8);
    REG_CLR_BITS_UINT16(reg_ckg_fcie, NFIE_CLK_MASK);
    REG_SET_BITS_UINT16(reg_ckg_fcie, u32ClkParam << 0);


    /*** ECC Clock ***/
    REG_CLR_BITS_UINT16(reg_ckg_ecc, (ECC_CLK_MASK|ECC_CLK_GATING|ECC_CLK_INVERSE));
    REG_SET_BITS_UINT16(reg_ckg_ecc, ECC_SW_DEFAULT_CLK);

    #endif

    return UNFD_ST_SUCCESS;
}


U32 NC_PlatformResetPre(NAND_DRIVER *pNandDrv)
{
    pNandDrv = pNandDrv;
    return UNFD_ST_SUCCESS;
}

U32 NC_PlatformResetPost(NAND_DRIVER *pNandDrv)
{
    U16 u16_Reg;

    #if defined(NC_SEL_FCIE3) && NC_SEL_FCIE3

    REG_WRITE_UINT16(NC_PATH_CTL, BIT_NC_EN);

    REG_READ_UINT16(FCIE_NC_CIFD_BASE, u16_Reg); // dummy read for CIFD clock

    REG_READ_UINT16(FCIE_NC_CIFD_BASE, u16_Reg); // dummy read for CIFD clock   twice said by designer

    REG_WRITE_UINT16(NC_WIDTH, FCIE_REG41_VAL);  // duty cycle 3:1 in 86Mhz (depends on chip config)

    #if defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA) || defined(CONFIG_MSTAR_K5TN)

    /* HW bug
       In order to fix the front data overlapped by tail data of spare in CIFD when spare byte > 512
       But when ecc correctable Not in first sector, the following sector spare data will not be copied to CIFD.

       when spare > 512 using spare dma
    */
    REG_SET_BITS_UINT16(NC_DDR_CTRL, BIT7);
    #endif

    #elif defined(NC_SEL_FCIE5) && NC_SEL_FCIE5

    REG_WRITE_UINT16(NC_FUN_CTL, BIT_NC_EN);

    REG_READ_UINT16(FCIE_NC_WBUF_CIFD_BASE, u16_Reg); // dummy read for WCIFD clock
    REG_READ_UINT16(FCIE_NC_WBUF_CIFD_BASE, u16_Reg); // dummy read for WCIFD clock

    REG_READ_UINT16(FCIE_NC_RBUF_CIFD_BASE, u16_Reg); // dummy read for RCIFD clock twice said by designer
    REG_READ_UINT16(FCIE_NC_RBUF_CIFD_BASE, u16_Reg); // dummy read for RCIFD clock twice said by designer

    #endif

    return UNFD_ST_SUCCESS;
}

#if !(defined(ENABLE_NAND_RIU_MODE) && ENABLE_NAND_RIU_MODE)
U32 nand_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
    flush_cache();
    flush_L2();

    return (u32_DMAAddr-CONFIG_MIU0_BUSADDR);
}
#endif

U32 NC_PlatformInit(NAND_DRIVER *pNandDrv)
{
    #if defined(NC_SEL_FCIE3) && NC_SEL_FCIE3

    REG_WRITE_UINT16(NC_PATH_CTL, 0x20);

    #endif

    nand_pads_switch(pNandDrv, NAND_PAD_BYPASS_MODE);

    #if defined (DECIDE_CLOCK_BY_NAND) && DECIDE_CLOCK_BY_NAND
    pNandDrv->u32_Clk = FCIE3_SW_SLOWEST_CLK;
    nand_clock_setting(FCIE3_SW_SLOWEST_CLK);
	  #if defined(NC_SEL_FCIE3) && NC_SEL_FCIE3
    pNandDrv->u16_Reg57_RELatch = 0;
	  #elif defined(NC_SEL_FCIE5) && NC_SEL_FCIE5
    pNandDrv->u16_Reg57_RELatch = BIT_NC_LATCH_DATA_2_0_T|BIT_NC_LATCH_STS_2_0_T;
	  #endif
    REG_WRITE_UINT16(NC_LATCH_DATA, pNandDrv->u16_Reg57_RELatch);
    #elif defined(CONFIG_MSTAR_TITANIA_BD_FPGA) && CONFIG_MSTAR_TITANIA_BD_FPGA
    pNandDrv->u32_Clk = NFIE_CLK_12M; 
	nand_clock_setting(NFIE_CLK_12M);
      #if defined(NC_SEL_FCIE3) && NC_SEL_FCIE3
    pNandDrv->u16_Reg57_RELatch = 0;
      #elif defined(NC_SEL_FCIE5) && NC_SEL_FCIE5
    pNandDrv->u16_Reg57_RELatch = BIT_NC_LATCH_DATA_2_0_T|BIT_NC_LATCH_STS_2_0_T;
      #endif
    REG_WRITE_UINT16(NC_LATCH_DATA, pNandDrv->u16_Reg57_RELatch);	
	#else
	pNandDrv->u32_Clk = FCIE3_SW_DEFAULT_CLK; 
    nand_clock_setting(FCIE3_SW_DEFAULT_CLK);
    pNandDrv->u16_Reg57_RELatch = REG57_ECO_FIX_INIT_VALUE;
    REG_WRITE_UINT16(NC_LATCH_DATA, pNandDrv->u16_Reg57_RELatch);
    #endif

    #if defined(CONFIG_MSTAR_NS_UBOOT)
    pNandDrv->pu8_PageDataBuf= (U8*)drvNandPageBuf;
    pNandDrv->pu8_PageSpareBuf = (U8*)drvNandSpareBuf;
    #endif

    return UNFD_ST_SUCCESS;
}

#if defined(DECIDE_CLOCK_BY_NAND) && DECIDE_CLOCK_BY_NAND
#define MAX_UNFD(a,b) ((a) > (b) ? (a) : (b))

U32 nand_config_timing(NAND_DRIVER *pNandDrv,U16 u16_1T)
{
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


    u16_DefaultTRR = NC_SDR_DEFAULT_TRR;
    u16_DefaultTCS = NC_SDR_DEFAULT_TCS;
    u16_DefaultTWW = NC_SDR_DEFAULT_TWW;
    u16_DefaultRX40Cmd = NC_SDR_DEFAULT_RX40CMD;
    u16_DefaultRX40Adr = NC_SDR_DEFAULT_RX40ADR;
    u16_DefaultRX56 = NC_SDR_DEFAULT_RX56;
    u16_DefaultTADL = NC_SDR_DEFAULT_TADL;
    u16_DefaultTCWAW = NC_SDR_DEFAULT_TCWAW;
    u16_DefaultTWHR = NC_SDR_DEFAULT_TWHR;


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
    #endif

    NC_Config(pNandDrv);
    return UNFD_ST_SUCCESS;
}


U32 nand_find_timing(NAND_DRIVER *pNandDrv, U8 *pu8_ClkIdx)
{
    U32 au32_1TTable[NFIE_CLK_TABLE_CNT] = NFIE_1T_TABLE;
    U32 au32_ClkValueTable[NFIE_CLK_TABLE_CNT] = NFIE_CLK_VALUE_TABLE;
    U16 u16_i, u16_j, u16_pass_begin = 0xFF, u16_pass_cnt = 0;
	U16 u16_nandstatus;
    U32 u32_Err;
    U8  au8_ID[NAND_ID_BYTE_CNT];
    U32 au32_ClkTable[NFIE_CLK_TABLE_CNT] = NFIE_CLK_TABLE;	
    U32 u32_Clk;
    U16 u16_SeqAccessTime, u16_Tmp, u16_Tmp2, u16_1T, u16_RE_LATCH_DELAY;
    S8 s8_ClkIdx;

    s8_ClkIdx = 0;
    u16_1T = 0;
    
	u16_Tmp = MAX_UNFD(MAX_UNFD(pNandDrv->u8_tRP, pNandDrv->u8_tREH), (pNandDrv->u16_tRC+1)/2);
	u16_Tmp2 = MAX_UNFD(MAX_UNFD(pNandDrv->u8_tWP, pNandDrv->u8_tWH), (pNandDrv->u16_tWC+1)/2);
	u16_SeqAccessTime = MAX_UNFD(u16_Tmp, u16_Tmp2);

	u16_Tmp = (pNandDrv->u8_tREA + NAND_SEQ_ACC_TIME_TOL)/2;
	u16_Tmp2 = u16_SeqAccessTime;
	u16_SeqAccessTime = MAX_UNFD(u16_Tmp, u16_Tmp2);

    u32_Clk = 1000000000/((U32)u16_SeqAccessTime); //FCIE5 needs to be update .....


    for(s8_ClkIdx =  0; s8_ClkIdx <= NFIE_CLK_TABLE_CNT - 1; s8_ClkIdx ++)
    {
        if(u32_Clk <= au32_ClkValueTable[s8_ClkIdx])
        {
            break;
        }
    }


    s8_ClkIdx --;


RETRY:
    if(s8_ClkIdx<0)
        return UNFD_ST_ERR_INVALID_PARAM;

        u16_1T = au32_1TTable[s8_ClkIdx];

    if(nand_config_timing(pNandDrv, u16_1T) != UNFD_ST_SUCCESS)
    {
        s8_ClkIdx--;
        goto RETRY;
    }
    #if defined(NC_SEL_FCIE3) && NC_SEL_FCIE3

    u16_RE_LATCH_DELAY = BIT_NC_LATCH_DATA_1_0_T;

    pNandDrv->u16_Reg57_RELatch &= ~BIT_NC_LATCH_DATA_MASK;
    pNandDrv->u16_Reg57_RELatch |= u16_RE_LATCH_DELAY;
    #elif defined(NC_SEL_FCIE5) && NC_SEL_FCIE5

	NC_ReadStatus(pNandDrv);
	REG_READ_UINT16(NC_ST_READ, u16_nandstatus);
	
	pNandDrv->u32_Clk = au32_ClkTable[s8_ClkIdx];
	nand_clock_setting(pNandDrv->u32_Clk);
	
	//using read id to detect relatch
    for(u16_i = 0 ; u16_i < NAND_ID_BYTE_CNT ; u16_i ++)
    {
        au8_ID[u16_i] = pNandDrv->au8_ID[u16_i];
    }
	for(u16_i = 0 ; u16_i < 16 ; u16_i ++)
	{
        pNandDrv->u16_Reg57_RELatch &= ~(BIT_NC_LATCH_DATA_MASK|BIT_NC_PAD_SEL_FAILLING);
        if(u16_i &1)	 //select falling edge otherwise rising edge is selected
       	    pNandDrv->u16_Reg57_RELatch|=BIT_NC_PAD_SEL_FAILLING;
        pNandDrv->u16_Reg57_RELatch |= ((u16_i/2) << 1) &BIT_NC_LATCH_DATA_MASK;
       
        NC_Config(pNandDrv);
        u32_Err = NC_ReadID(pNandDrv);
        if(u32_Err != UNFD_ST_SUCCESS)
			return u32_Err;

        for(u16_j = 0; u16_j < NAND_ID_BYTE_CNT; u16_j++)
        {
       	   if(pNandDrv->au8_ID[u16_j] != au8_ID[u16_j])
       	   {
       	       break;
       	   }
        }
        if(u16_j == NAND_ID_BYTE_CNT)
        {
       	     if(u16_pass_begin == 0xFF)
       		     u16_pass_begin = u16_i;
       	     u16_pass_cnt ++;
        }
	}
	
    if(u16_pass_cnt == 0)
    {
    	 pNandDrv->u16_Reg57_RELatch = BIT_NC_LATCH_DATA_2_0_T|BIT_NC_LATCH_STS_2_0_T;
    	 NC_Config(pNandDrv);
    	 s8_ClkIdx = 0;
    	 *pu8_ClkIdx = (U8)s8_ClkIdx;

         for(u16_i = 0 ; u16_i < NAND_ID_BYTE_CNT ; u16_i ++)
         {
             pNandDrv->au8_ID[u16_i]=au8_ID[u16_i];
         }
    	 return UNFD_ST_SUCCESS;
    }
    else
    {
    	 u16_i = u16_pass_begin + u16_pass_cnt/2;
    	 pNandDrv->u16_Reg57_RELatch &= ~(BIT_NC_LATCH_DATA_MASK|BIT0);
    	 if(u16_i &1)	 //select falling edge otherwise rising edge is selected
    		 pNandDrv->u16_Reg57_RELatch|=BIT0;
    	 pNandDrv->u16_Reg57_RELatch |= ((u16_i/2) << 1) &BIT_NC_LATCH_DATA_MASK;
         for(u16_i = 0 ; u16_i < NAND_ID_BYTE_CNT ; u16_i ++)
         {
             pNandDrv->au8_ID[u16_i]=au8_ID[u16_i];
         }
    }
   
    //detect read status
   
    u16_pass_begin = 0xFF;
    u16_pass_cnt = 0;
   
    for(u16_i = 0 ; u16_i < 8 ; u16_i ++)
    {
    	 U16 u16_tmpStatus;
    	 pNandDrv->u16_Reg57_RELatch &= ~(BIT_NC_LATCH_STS_MASK);
    	 pNandDrv->u16_Reg57_RELatch |= ((u16_i) << 5) & BIT_NC_LATCH_STS_MASK;
    
    	 NC_Config(pNandDrv);
    	 NC_ReadStatus(pNandDrv);
    	 REG_READ_UINT16(NC_ST_READ, u16_tmpStatus);
    
    	 if(u16_tmpStatus == u16_nandstatus)
    	 {
    		 if(u16_pass_begin == 0xFF)
    			 u16_pass_begin = u16_i;
    		 u16_pass_cnt ++;
    	 }
    }
    if(u16_pass_cnt == 0)
    {
    	 pNandDrv->u16_Reg57_RELatch = BIT_NC_LATCH_DATA_2_0_T|BIT_NC_LATCH_STS_2_0_T;
    	 NC_Config(pNandDrv);
    	 s8_ClkIdx = 0;
    	 *pu8_ClkIdx = (U8)s8_ClkIdx;
    
    	 return UNFD_ST_SUCCESS;
    }
    else
    {
    	 u16_i = u16_pass_begin + u16_pass_cnt/2;
    	 pNandDrv->u16_Reg57_RELatch &= ~(BIT_NC_LATCH_STS_MASK);
    	 pNandDrv->u16_Reg57_RELatch |= ((u16_i) << 5) & BIT_NC_LATCH_STS_MASK;
    }
    u16_RE_LATCH_DELAY = pNandDrv->u16_Reg57_RELatch;

    #endif
    *pu8_ClkIdx = (U8)s8_ClkIdx;

    return UNFD_ST_SUCCESS;

}
#endif

U32 nand_config_clock(NAND_DRIVER *pNandDrv)
{
    #if defined(DECIDE_CLOCK_BY_NAND) && DECIDE_CLOCK_BY_NAND
    U32 u32_Err = 0;
    U32 au32_ClkTable[NFIE_CLK_TABLE_CNT] = NFIE_CLK_TABLE;
    char *ClkStrTable[NFIE_CLK_TABLE_CNT] = NFIE_CLK_TABLE_STR;

    U8 u8_ClkIdx = 0;



    u32_Err = nand_find_timing(pNandDrv, &u8_ClkIdx);
    if(u32_Err != UNFD_ST_SUCCESS)
    {
        LDR_PUTS("Err, NAND, Cannot config nand timing\\n");
        return(u32_Err);
    }
    
	pNandDrv->u32_Clk = au32_ClkTable[u8_ClkIdx];
    nand_clock_setting(pNandDrv->u32_Clk);

    REG_WRITE_UINT16(NC_LATCH_DATA, pNandDrv->u16_Reg57_RELatch);


	#endif

    return UNFD_ST_SUCCESS;
}

#else
  #error "Error! no platform functions."
#endif
#endif

