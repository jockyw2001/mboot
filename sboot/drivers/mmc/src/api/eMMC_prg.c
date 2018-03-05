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
#include <stdlib.h>

#if defined(UNIFIED_eMMC_DRIVER) && UNIFIED_eMMC_DRIVER

//========================================================
U32 eMMC_LoadImages(U32 *pu32_Addr, U32 u32_ByteCnt,U32 u32_HashStage)
{
    #ifndef IP_FCIE_VERSION_5
    *pu32_Addr=0;
    u32_ByteCnt=0;
    u32_HashStage=0;

    return eMMC_ST_SUCCESS;
    #else
    U32 u32_err,u32_dma_addr;
    U16 u16_reg;


    eMMC_FCIE_ClearEvents();
    if(u32_HashStage == 0)
    {
        eMMC_clock_setting(BIT_CLK_XTAL_12M);
        eMMC_pads_switch(FCIE_eMMC_BYPASS);
        REG_FCIE_CLRBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH);

        REG_FCIE_SETBIT(FCIE_BOOT, BIT_NAND_BOOT_EN);
        REG_FCIE_CLRBIT(FCIE_BOOT, BIT_BOOTSRAM_ACCESS_SEL);
        REG_FCIE_CLRBIT(FCIE_BOOT_CONFIG, BIT_BOOT_MODE_EN); // DMA to DRAM
        eMMC_FCIE_Init();
        eMMC_RST_L();
        eMMC_hw_timer_delay(HW_TIMER_DELAY_5ms);
        eMMC_RST_H();
        eMMC_hw_timer_delay(HW_TIMER_DELAY_5ms);
        u32_err = eMMC_ROM_BOOT_CMD0(0xFFFFFFFA, (U32)pu32_Addr, (U16)(u32_ByteCnt>>eMMC_SECTOR_512BYTE_BITS));
        if(eMMC_ST_SUCCESS != u32_err)
            goto  LABEL_LOAD_IMAGE_END;
    }
    else
    {
        REG_FCIE_W(FCIE_JOB_BL_CNT, u32_ByteCnt>>eMMC_SECTOR_512BYTE_BITS);
        u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu32_Addr, u32_ByteCnt);

        REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0, u32_dma_addr & 0xFFFF);
        REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, u32_dma_addr >> 16);
        REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, u32_ByteCnt & 0xFFFF);
        REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16,u32_ByteCnt >> 16);
        REG_FCIE_CLRBIT(FCIE_BOOT, BIT_BOOTSRAM_ACCESS_SEL);
        REG_FCIE_CLRBIT(FCIE_BOOT_CONFIG, BIT_BOOT_MODE_EN); // DMA to DRAM

        REG_FCIE_W(FCIE_SD_CTRL, BIT_SD_DTRX_EN|BIT_JOB_START);
        u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT,BIT_DMA_END, TIME_WAIT_n_BLK_END*(1+(u32_ByteCnt>>20)));

        if(eMMC_ST_SUCCESS != u32_err)
            goto LABEL_LOAD_IMAGE_END;
    }
    // check status
    REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
    if( u16_reg & BIT_SD_FCIE_ERR_FLAGS )
    {
        u32_err = eMMC_ST_ERR_FCIE_STS_ERR;
        if(eMMC_ST_SUCCESS != u32_err)
            goto LABEL_LOAD_IMAGE_END;
    }
    if(u32_HashStage == 2)
    {
        u32_err = eMMC_CMD0(0);
        if(eMMC_ST_SUCCESS != u32_err)
            goto LABEL_LOAD_IMAGE_END;

        REG_FCIE_CLRBIT(FCIE_BOOT, BIT_NAND_BOOT_EN);
    }
    return eMMC_ST_SUCCESS;
    LABEL_LOAD_IMAGE_END:
    REG_FCIE_CLRBIT(FCIE_BOOT, BIT_NAND_BOOT_EN);
    return u32_err;
    #endif
}



//========================================================
U32 eMMC_GetID(U8 *pu8IDByteCnt, U8 *pu8ID)
{
    eMMC_debug(eMMC_DEBUG_LEVEL_HIGH, 1, "\n");

    *pu8IDByteCnt = g_eMMCDrv.u8_IDByteCnt;
    //memcpy(pu8ID, g_eMMCDrv.au8_ID, eMMC_ID_BYTE_CNT);
    pu8ID[0]=g_eMMCDrv.au8_ID[0];
    pu8ID[1]=g_eMMCDrv.au8_ID[1];
    pu8ID[2]=g_eMMCDrv.au8_ID[2];
    pu8ID[3]=g_eMMCDrv.au8_ID[3];
    pu8ID[4]=g_eMMCDrv.au8_ID[4];
    pu8ID[5]=g_eMMCDrv.au8_ID[5];
    pu8ID[6]=g_eMMCDrv.au8_ID[6];
    pu8ID[7]=g_eMMCDrv.au8_ID[7];
    pu8ID[8]=g_eMMCDrv.au8_ID[8];
    pu8ID[9]=g_eMMCDrv.au8_ID[9];
    pu8ID[10]=g_eMMCDrv.au8_ID[10];
    pu8ID[11]=g_eMMCDrv.au8_ID[11];
    pu8ID[12]=g_eMMCDrv.au8_ID[12];
    pu8ID[13]=g_eMMCDrv.au8_ID[13];
    pu8ID[14]=g_eMMCDrv.au8_ID[14];

    return eMMC_ST_SUCCESS;
}

// ==============================================================
void eMMC_DumpSpeedStatus(void)
{
    eMMC_debug(eMMC_DEBUG_LEVEL,0,"  SDR Mode\n");

    eMMC_debug(eMMC_DEBUG_LEVEL,0,"  FCIE Clk: %u.%u MHz, %Xh\n",
        g_eMMCDrv.u32_ClkKHz/1000, (g_eMMCDrv.u32_ClkKHz/100)%10, g_eMMCDrv.u16_ClkRegVal);

    switch(g_eMMCDrv.u8_BUS_WIDTH)
    {
        case BIT_SD_DATA_WIDTH_1:
            eMMC_debug(eMMC_DEBUG_LEVEL,0,"  1-bit ");
            break;
        case BIT_SD_DATA_WIDTH_4:
            eMMC_debug(eMMC_DEBUG_LEVEL,0,"  4-bits ");
            break;
        case BIT_SD_DATA_WIDTH_8:
            eMMC_debug(eMMC_DEBUG_LEVEL,0,"  8-bits ");
            break;
    }
    eMMC_debug(eMMC_DEBUG_LEVEL,0,"width\n");

    eMMC_debug(eMMC_DEBUG_LEVEL,0,"  Bus Speed:");
    switch(g_eMMCDrv.u32_DrvFlag & DRV_FLAG_SPEED_MASK)
    {
        case DRV_FLAG_SPEED_HIGH:
            eMMC_debug(eMMC_DEBUG_LEVEL,0," HIGH\n");
            break;
        default:
            eMMC_debug(eMMC_DEBUG_LEVEL,0," LOW\n");
    }
}


void eMMC_DumpDriverStatus(void)
{
    eMMC_debug(0,1,"\n  eMMCDrvExtFlag: %Xh \n\n", gu32_eMMCDrvExtFlag);

    eMMC_debug(eMMC_DEBUG_LEVEL,0,"eMMC Status: 2014/03/25 \n");

    // ------------------------------------------------------
    // helpful debug info
    // ------------------------------------------------------
    #if defined(ENABLE_eMMC_RIU_MODE)&&ENABLE_eMMC_RIU_MODE
    eMMC_debug(eMMC_DEBUG_LEVEL,0,"  RIU Mode\n");
    #else
    eMMC_debug(eMMC_DEBUG_LEVEL,0,"  MIU Mode\n");
    #endif

    #if defined(ENABLE_eMMC_INTERRUPT_MODE)&&ENABLE_eMMC_INTERRUPT_MODE
    eMMC_debug(eMMC_DEBUG_LEVEL,0,"  Interrupt Mode\n");
    #else
    eMMC_debug(eMMC_DEBUG_LEVEL,0,"  Polling Mode\n");
    #endif

    #if defined(FICE_BYTE_MODE_ENABLE)&&FICE_BYTE_MODE_ENABLE
    eMMC_debug(eMMC_DEBUG_LEVEL,0,"  FCIE Byte Mode\n");
    #else
    eMMC_debug(eMMC_DEBUG_LEVEL,0,"  FCIE Block Mode\n");
    #endif

    eMMC_DumpSpeedStatus();

    eMMC_debug(eMMC_DEBUG_LEVEL,0,"  PwrOff Notification: ");
    switch(g_eMMCDrv.u32_DrvFlag&DRV_FLAG_PwrOffNotif_MASK)
    {
        case DRV_FLAG_PwrOffNotif_OFF:
            eMMC_debug(eMMC_DEBUG_LEVEL,0,"OFF\n");  break;
        case DRV_FLAG_PwrOffNotif_ON:
            eMMC_debug(eMMC_DEBUG_LEVEL,0,"ON\n");  break;
        case DRV_FLAG_PwrOffNotif_SHORT:
            eMMC_debug(eMMC_DEBUG_LEVEL,0,"SHORT\n");  break;
        case DRV_FLAG_PwrOffNotif_LONG:
            eMMC_debug(eMMC_DEBUG_LEVEL,0,"LONG\n");  break;
        default:
            eMMC_debug(eMMC_DEBUG_LEVEL,0," eMMC Err: unknown: %Xh\n",
                g_eMMCDrv.u32_DrvFlag&DRV_FLAG_PwrOffNotif_MASK);
    }

}


U32 eMMC_Init(void)
{
    U32 u32_err;

    #if defined(ENABLE_eMMC_INTERRUPT_MODE)&&ENABLE_eMMC_INTERRUPT_MODE
    eMMC_enable_intr_mode();
    #endif

    u32_err = eMMC_CheckIfReady();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC_Init_Ex fail: %Xh\n", u32_err);
    }
    return u32_err;
}

#define eMMC_NOT_READY_MARK    ~(('e'<<24)|('M'<<16)|('M'<<8)|'C')
static U32 sgu32_IfReadyGuard = eMMC_NOT_READY_MARK;

static U32 eMMC_Init_Ex(void)
{
    U32 u32_err;
    U16 u16_i;
    U8 *pgeMMCDrv = (U8*)&g_eMMCDrv;

    for(u16_i=0; u16_i<sizeof(eMMC_DRIVER); u16_i++)
    {
        pgeMMCDrv[u16_i] = 0x00;
    }

    g_eMMCDrv.u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;

    // ---------------------------------
    // init platform & FCIE
    eMMC_PlatformInit();
    u32_err = eMMC_FCIE_Init();

    if(u32_err)
        goto  LABEL_INIT_END;

    // ---------------------------------
    u32_err = eMMC_Init_Device();

    eMMC_debug(eMMC_DEBUG_LEVEL, 0, "eMMC ");
    if(u32_err)
        goto LABEL_INIT_END;

    sgu32_IfReadyGuard = ~eMMC_NOT_READY_MARK;

    LABEL_INIT_END:
    g_eMMCDrv.u32_LastErrCode = u32_err;

    // ---------------------------------
    // setup ID
    // use first 10 bytes of CID

    // arm tool chain using memcpy not aligned => exception
    //memcpy(g_eMMCDrv.au8_ID, &g_eMMCDrv.au8_CID[1], eMMC_ID_FROM_CID_BYTE_CNT);

    g_eMMCDrv.au8_ID[0]= g_eMMCDrv.au8_CID[1];
    g_eMMCDrv.au8_ID[1]= g_eMMCDrv.au8_CID[2];
    g_eMMCDrv.au8_ID[2]= g_eMMCDrv.au8_CID[3];
    g_eMMCDrv.au8_ID[3]= g_eMMCDrv.au8_CID[4];
    g_eMMCDrv.au8_ID[4]= g_eMMCDrv.au8_CID[5];
    g_eMMCDrv.au8_ID[5]= g_eMMCDrv.au8_CID[6];
    g_eMMCDrv.au8_ID[6]= g_eMMCDrv.au8_CID[7];
    g_eMMCDrv.au8_ID[7]= g_eMMCDrv.au8_CID[8];
    g_eMMCDrv.au8_ID[8]= g_eMMCDrv.au8_CID[9];
    g_eMMCDrv.au8_ID[9]= g_eMMCDrv.au8_CID[10];

    g_eMMCDrv.u8_IDByteCnt = eMMC_ID_DEFAULT_BYTE_CNT;
    // add a 11-th byte for number of GB
    g_eMMCDrv.au8_ID[eMMC_ID_FROM_CID_BYTE_CNT] =
        (g_eMMCDrv.u32_SEC_COUNT >> (1+10+10)) + 1;
    eMMC_debug(eMMC_DEBUG_LEVEL, 0, "%u.%u GB [%Xh]\n",
        g_eMMCDrv.u32_SEC_COUNT>>11>>10,
        (g_eMMCDrv.u32_SEC_COUNT>>11)*100/1024%100,
        g_eMMCDrv.u32_SEC_COUNT);
    //eMMC_dump_mem(g_eMMCDrv.au8_ID, 0x10);
    #if 0
    u32_err = eMMC_ReadPartitionInfo_Ex();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "eMMC Err: eMMC Init, no pni, :%Xh\n", u32_err);
        return eMMC_ST_ERR_NO_CIS;
    }
    //eMMC_DumpDriverStatus();
    #endif

    return u32_err;
}

U32 eMMC_Init_Device_Ex(void)
{
    U32 u32_err;

    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_INIT_DONE;

    // init eMMC device
    u32_err = eMMC_Identify();
    if(u32_err)
        return u32_err;

    eMMC_clock_setting(FCIE_SLOW_CLK);

    // determine device parameters, from CSD
    u32_err = eMMC_CSD_Config();
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

    // setup eMMC device
    // CMD7
    u32_err = eMMC_CMD3_CMD7(g_eMMCDrv.u16_RCA, 7);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

    // determine device parameters, from Ext_CSD
    u32_err = eMMC_ExtCSD_Config();
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

    //eMMC_debug(0,0,"set bus witdth 8 bits\n");
    u32_err = eMMC_SetBusWidth(8, 0);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

    //eMMC_debug(0,0,"set to high speed\n");
    u32_err = eMMC_SetBusSpeed(eMMC_SPEED_HIGH);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

    eMMC_pads_switch(FCIE_eMMC_BYPASS);
    eMMC_clock_setting(FCIE_SLOW_CLK);

    g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_INIT_DONE;

    return u32_err;
}


U32 eMMC_Init_Device(void)
{
    U32 u32_err;

    u32_err = eMMC_Init_Device_Ex();
    if(eMMC_ST_SUCCESS != u32_err)
        goto  LABEL_INIT_END;

    //eMMC_dump_mem(g_eMMCDrv.au8_AllRsp, 0x100);
    LABEL_INIT_END:

    return u32_err;

}

//#else

/*void mstar_dump_fcie_register(void)
{
    U32 i;
    for(i=0; i<0x60; i++) {
        eMMC_debug(0,0,"%04X ", REG_FCIE(GET_REG_ADDR(FCIE_REG_BASE_ADDR, i)));
        if(i%8==7) eMMC_debug(0,0,"\n");
    }
}*/

U32 eMMC_CheckIfReady(void)
{
    #if 0   // For STR, force reinit
    if(eMMC_NOT_READY_MARK != sgu32_IfReadyGuard)
        return eMMC_ST_SUCCESS;
    else
    #endif
        return eMMC_Init_Ex();
}

void eMMC_ResetReadyFlag(void)
{
    sgu32_IfReadyGuard = eMMC_NOT_READY_MARK;
}


// pu8_CID needs to hold 16 bytes
U32 eMMC_GetCID(U8 *pu8_CID)
{
    if(NULL==pu8_CID)
        return eMMC_ST_ERR_INVALID_PARAM;

    //memcpy(pu8_CID, &g_eMMCDrv.au8_CID[1], 15);
    pu8_CID[0]=g_eMMCDrv.au8_CID[1];
    pu8_CID[1]=g_eMMCDrv.au8_CID[2];
    pu8_CID[2]=g_eMMCDrv.au8_CID[3];
    pu8_CID[3]=g_eMMCDrv.au8_CID[4];
    pu8_CID[4]=g_eMMCDrv.au8_CID[5];
    pu8_CID[5]=g_eMMCDrv.au8_CID[6];
    pu8_CID[6]=g_eMMCDrv.au8_CID[7];
    pu8_CID[7]=g_eMMCDrv.au8_CID[8];
    pu8_CID[8]=g_eMMCDrv.au8_CID[9];
    pu8_CID[9]=g_eMMCDrv.au8_CID[10];
    pu8_CID[10]=g_eMMCDrv.au8_CID[11];
    pu8_CID[11]=g_eMMCDrv.au8_CID[12];
    pu8_CID[12]=g_eMMCDrv.au8_CID[13];
    pu8_CID[13]=g_eMMCDrv.au8_CID[14];
    pu8_CID[14]=g_eMMCDrv.au8_CID[15];
    pu8_CID[15]=g_eMMCDrv.au8_CID[16];

    pu8_CID[15] = 0; // no CRC
    return eMMC_ST_SUCCESS;
}

U32 eMMC_WriteData(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr)
{
    U32 u32_err=eMMC_ST_SUCCESS;
    volatile U16 u16_BlkCnt;

    // check if eMMC Init
    if(eMMC_NOT_READY_MARK == sgu32_IfReadyGuard)
    {
        u32_err = eMMC_Init();
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC not ready (init) \n");
            return eMMC_ST_ERR_NOT_INIT;
        }
    }

    if(NULL == pu8_DataBuf)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: w data buf is NULL: %Xh \n", (U32)pu8_DataBuf);
        return eMMC_ST_ERR_INVALID_PARAM;
    }
    // check if u32_DataByteCnt is 512B boundary
    if(u32_DataByteCnt & (eMMC_SECTOR_512BYTE-1))
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: data not 512B boundary \n");
        return eMMC_ST_ERR_INVALID_PARAM;
    }

    if( (u32_BlkAddr > g_eMMCDrv.u32_SEC_COUNT) || /* Prevent start block overflow */
        ((u32_DataByteCnt>>9) > g_eMMCDrv.u32_SEC_COUNT) || /* Prevent block length overflow */
        (u32_BlkAddr + (u32_DataByteCnt>>9) > g_eMMCDrv.u32_SEC_COUNT) )
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: invalid data range, %Xh > %Xh \n",
            u32_BlkAddr + (u32_DataByteCnt>>9), g_eMMCDrv.u32_SEC_COUNT);
        return eMMC_ST_ERR_INVALID_PARAM;
    }

    // write data
    while(u32_DataByteCnt)
    {
        if(u32_DataByteCnt > eMMC_SECTOR_512BYTE)
        {
            if((u32_DataByteCnt>>eMMC_SECTOR_512BYTE_BITS) < BIT_SD_JOB_BLK_CNT_MASK)
                u16_BlkCnt = (u32_DataByteCnt>>eMMC_SECTOR_512BYTE_BITS);
            else
                u16_BlkCnt = BIT_SD_JOB_BLK_CNT_MASK;

            u32_err = eMMC_CMD25(u32_BlkAddr, pu8_DataBuf, u16_BlkCnt);
        }
        else
        {
            u16_BlkCnt = 1;
            u32_err = eMMC_CMD24(u32_BlkAddr, pu8_DataBuf);
        }

        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: W fail: %Xh\n", u32_err);
            g_eMMCDrv.u32_LastErrCode = u32_err;
            break;
        }

        u32_BlkAddr += u16_BlkCnt;
        pu8_DataBuf += u16_BlkCnt << eMMC_SECTOR_512BYTE_BITS;
        u32_DataByteCnt -= u16_BlkCnt << eMMC_SECTOR_512BYTE_BITS;
    }

    return u32_err;

}



U32 eMMC_ReadData(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr)
{
    U32 u32_err;
    volatile U16 u16_BlkCnt;
    U8  u8_IfNotCacheLineAligned=0;

    //eMMC_debug(eMMC_DEBUG_LEVEL,1,"\n");

    // check if eMMC Init
    if(eMMC_NOT_READY_MARK == sgu32_IfReadyGuard)
    {
        u32_err = eMMC_Init();
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC not ready (init) \n");
            return eMMC_ST_ERR_NOT_INIT;
        }
    }
    // check if u32_DataByteCnt is 512B boundary
    if(u32_DataByteCnt & (eMMC_SECTOR_512BYTE-1))
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: data not 512B boundary \n");
        return eMMC_ST_ERR_INVALID_PARAM;
    }

    if( (u32_BlkAddr > g_eMMCDrv.u32_SEC_COUNT) || /* Prevent start block overflow */
        ((u32_DataByteCnt>>9) > g_eMMCDrv.u32_SEC_COUNT) || /* Prevent block length overflow */
        (u32_BlkAddr + (u32_DataByteCnt>>9) > g_eMMCDrv.u32_SEC_COUNT) )
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: invalid data range, %Xh > %Xh \n",
            u32_BlkAddr + (u32_DataByteCnt>>9), g_eMMCDrv.u32_SEC_COUNT);
        return eMMC_ST_ERR_INVALID_PARAM;
    }

    // read data
    // first 512 bytes, special handle if not cache line aligned
    if((U32)pu8_DataBuf & (eMMC_CACHE_LINE-1))
    {
        #if 0
        eMMC_debug(eMMC_DEBUG_LEVEL_WARNING,1,
            "eMMC Warn: R, buffer not Cache Line aligned: %Xh \n",
            (U32)pu8_DataBuf);
        #endif

        u32_err = eMMC_CMD17(u32_BlkAddr, gau8_eMMC_SectorBuf);
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: R fail.0: %Xh\n", u32_err);
            g_eMMCDrv.u32_LastErrCode = u32_err;
            return u32_err;
        }
        memcpy(pu8_DataBuf, gau8_eMMC_SectorBuf, eMMC_SECTOR_512BYTE);
        u32_BlkAddr += 1;
        pu8_DataBuf += eMMC_SECTOR_512BYTE;
        u32_DataByteCnt -= eMMC_SECTOR_512BYTE;

        // last 512B must be not cache line aligned,
        // reserved for last eMMC_CMD17_CIFD
        if(u32_DataByteCnt){
            u8_IfNotCacheLineAligned = 1;
            u32_DataByteCnt -= eMMC_SECTOR_512BYTE;
        }
    }

    while(u32_DataByteCnt)
    {
        if(u32_DataByteCnt > eMMC_SECTOR_512BYTE)
        {
            if((u32_DataByteCnt>>eMMC_SECTOR_512BYTE_BITS) < BIT_SD_JOB_BLK_CNT_MASK)
                u16_BlkCnt = (u32_DataByteCnt>>eMMC_SECTOR_512BYTE_BITS);
            else
                u16_BlkCnt = BIT_SD_JOB_BLK_CNT_MASK;

            u32_err = eMMC_CMD18(u32_BlkAddr, pu8_DataBuf, u16_BlkCnt);
        }
        else
        {
            u16_BlkCnt = 1;
            u32_err = eMMC_CMD17(u32_BlkAddr, pu8_DataBuf);
        }

        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: R fail.1: %Xh\n", u32_err);
            g_eMMCDrv.u32_LastErrCode = u32_err;
            break;
        }

        u32_BlkAddr += u16_BlkCnt;
        pu8_DataBuf += u16_BlkCnt << eMMC_SECTOR_512BYTE_BITS;
        u32_DataByteCnt -= u16_BlkCnt << eMMC_SECTOR_512BYTE_BITS;
    }

    // last 512 bytes, special handle if not cache line aligned
    if(u8_IfNotCacheLineAligned)
    {
        u32_err = eMMC_CMD17(u32_BlkAddr, gau8_eMMC_SectorBuf);
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: R fail.2: %Xh\n", u32_err);
            g_eMMCDrv.u32_LastErrCode = u32_err;
        }
        memcpy(pu8_DataBuf, gau8_eMMC_SectorBuf, eMMC_SECTOR_512BYTE);
    }

    return u32_err;

}

static U32  eMMC_strcmp(U8* u8_cs,U8* u8_ct, U8 u8_cnt)
{
    U32 u32_err=eMMC_ST_SUCCESS;
    U8  u8_i;

    for(u8_i=0;u8_i< u8_cnt;u8_i++)
    {
       if(u8_cs[u8_i]!=u8_ct[u8_i])
           break;
    }
    if(u8_i < u8_cnt)
        return eMMC_ST_ERR_DATA_MISMATCH;

    return u32_err;
}
U32 emmc_write_pdb (U32 part, emmc_partition_t *pdb_p)
{
    U32 u32_err;

    memcpy(gau8_eMMC_SectorBuf,(U8*)pdb_p, sizeof(emmc_partition_t));
    u32_err = eMMC_CMD24_MIU(part, gau8_eMMC_SectorBuf);

    return u32_err;
}

U32 emmc_read_pdb (U32 part, emmc_partition_t *pdb_p)
{
    U32 u32_err;

    u32_err = eMMC_CMD17_MIU(part, gau8_eMMC_SectorBuf);

    if(u32_err)
        return u32_err;

    memcpy((U8*)pdb_p, gau8_eMMC_SectorBuf, sizeof(emmc_partition_t));

    return u32_err;
}

U32 Search_partition(U8* u8_name, U32* u32_BlkAddr, U32* u32_BlkCnt)
{
    U32 u32_i, u32_err;
    U8 u8_mboot_name[32];
    emmc_partition_t eMMC_partition;
    //auto write mboot partition table
	for(u32_i=0; u32_i< 32;u32_i++)
	{
        u8_mboot_name[u32_i]=0;
	}
    u8_mboot_name[0]=0x4D;
	u8_mboot_name[1]=0x42;
    u8_mboot_name[2]=0x4F;
	u8_mboot_name[3]=0x4F;
	u8_mboot_name[4]=0x54;

    u32_err = emmc_read_pdb(1, &eMMC_partition);
    if(u32_err == eMMC_ST_SUCCESS)
    {
       //Search MBoot partition table
       if(eMMC_strcmp(u8_mboot_name,eMMC_partition.name,32)==eMMC_ST_ERR_DATA_MISMATCH)
       {
           for(u32_i=0;u32_i< sizeof(emmc_partition_t);u32_i++)
               ((U8*)(&eMMC_partition))[u32_i] =0;

           eMMC_partition.signature = EMMC_PARTITION_MAGIC;
           eMMC_partition.start_block = EMMC_CHUNK_HEAD_START;
           eMMC_partition.block_count = EMMC_MBOOT_PATITION_SIZE;
           for(u32_i=0;u32_i< 32;u32_i++)
               eMMC_partition.name[u32_i]= u8_mboot_name[u32_i];

           u32_err = emmc_write_pdb(1,&eMMC_partition);
           if(u32_err)
               return u32_err;
        }
        *u32_BlkAddr = eMMC_partition.start_block;
        *u32_BlkCnt  = eMMC_partition.block_count;
     }
     else
         return u32_err;

     if(eMMC_strcmp(u8_name,u8_mboot_name,32)==eMMC_ST_SUCCESS)
         return eMMC_ST_SUCCESS;
     //auto write mboot partition table
	 for(u32_i=0; u32_i< 32;u32_i++)
	 {
        u8_mboot_name[u32_i]=0;
	 }
     u8_mboot_name[0]=0x4D;
	 u8_mboot_name[1]=0x50;
     u8_mboot_name[2]=0x4F;
	 u8_mboot_name[3]=0x4F;
	 u8_mboot_name[4]=0x4C;

    u32_err = emmc_read_pdb(2, &eMMC_partition);
    if(u32_err == eMMC_ST_SUCCESS)
    {
       //Search MBoot partition table
       if(eMMC_strcmp(u8_mboot_name,eMMC_partition.name,32)==eMMC_ST_ERR_DATA_MISMATCH)
       {
           for(u32_i=0;u32_i< sizeof(emmc_partition_t);u32_i++)
               ((U8*)(&eMMC_partition))[u32_i] =0;

           eMMC_partition.signature = EMMC_PARTITION_MAGIC;
           eMMC_partition.start_block = EMMC_CHUNK_HEAD_START+EMMC_MBOOT_PATITION_SIZE;
           eMMC_partition.block_count = EMMC_MPOOL_PATITION_SIZE;
           for(u32_i=0;u32_i< 32;u32_i++)
               eMMC_partition.name[u32_i]= u8_mboot_name[u32_i];

           u32_err = emmc_write_pdb(2,&eMMC_partition);
           if(u32_err)
               return u32_err;
        }
        *u32_BlkAddr = eMMC_partition.start_block;
        *u32_BlkCnt  = eMMC_partition.block_count;
     }
     else
         return u32_err;

     if(eMMC_strcmp(u8_name,u8_mboot_name,32)==eMMC_ST_SUCCESS)
         return eMMC_ST_SUCCESS;

	for(u32_i=3; u32_i< EMMC_RESERVED_FOR_MAP_V2;u32_i++)
    {
        u32_err = emmc_read_pdb(u32_i, &eMMC_partition);

        if(u32_err==eMMC_ST_SUCCESS)
        {
           if(eMMC_strcmp(u8_name,eMMC_partition.name,32)==eMMC_ST_SUCCESS)
               break;
        }
        else
           return u32_err;
    }

	if(u32_i == EMMC_RESERVED_FOR_MAP_V2)
          return eMMC_ST_ERR_DATA_MISMATCH;

    *u32_BlkAddr = eMMC_partition.start_block;
    *u32_BlkCnt  = eMMC_partition.block_count;

    return u32_err;
}

U32  eMMC_ComapreData(U8 *pu8_Buf0, U8 *pu8_Buf1, U32 u32_ByteCnt)
{
	U32 u32_i;

	for(u32_i=0; u32_i<u32_ByteCnt; u32_i++)
	{
		if(pu8_Buf0[u32_i] != pu8_Buf1[u32_i])
			break;
	}

    if(u32_i != u32_ByteCnt)
        return eMMC_ST_ERR_DATA_MISMATCH;

    return eMMC_ST_SUCCESS;
}


U32 eMMC_WriteBootPart(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr, U8 u8_PartNo)
{
    U32 u32_err;
    U16 u16_SecCnt, u16_i ,u16_j;

    // check if eMMC Init
    if(eMMC_NOT_READY_MARK == sgu32_IfReadyGuard)
    {
        u32_err = eMMC_Init();
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC not ready (init) \n");
            return eMMC_ST_ERR_NOT_INIT;
        }
    }
    // set Access Boot Partition 1
    if(u8_PartNo == 1)
        u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT0);
    else if(u8_PartNo == 2)
    {
        u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1);
    }
    if(eMMC_ST_SUCCESS != u32_err)
    {
       eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh, eMMC, set Ext_CSD[179]: %Xh fail\n",
           u32_err, BIT3|BIT0);
       return u32_err;
    }
    u16_SecCnt = (u32_DataByteCnt>>9) + ((u32_DataByteCnt&0x1FF)?1:0);

    u32_err = eMMC_WriteData(pu8_DataBuf,
              u16_SecCnt<<eMMC_SECTOR_512BYTE_BITS,
              u32_BlkAddr);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,
           "eMMC Err: write Boot Partition fail, %Xh\n", u32_err);
        return u32_err;
    }
    // verify Boot Code
    u16_j =0;
    for(u16_i=(U16)u32_BlkAddr; u16_i<u16_SecCnt; u16_i++)
    {
        u32_err = eMMC_CMD17(u16_i, gau8_eMMC_SectorBuf);
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,
                "eMMC Err: read Boot Partition Sector %u fail, %Xh\n",
                u16_i, u32_err);
            return u32_err;
        }

        u32_err = eMMC_ComapreData(pu8_DataBuf+(u16_j<<9), gau8_eMMC_SectorBuf, eMMC_SECTOR_512BYTE);
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,
                "eMMC Err: check Boot Partition Sector %u fail, %Xh\n",
                u16_i, u32_err);
            return u32_err;
        }
        u16_j++;
    }

    // set Boot Bus
    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 177, BIT1);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh, eMMC, set Ext_CSD[177]: %Xh fail\n",
            u32_err, BIT1);
        return u32_err;
    }

    // clear Access Boot Partition
    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3);

    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh, eMMC, set Ext_CSD[179]: %Xh fail\n",
            u32_err, BIT3);
        return u32_err;
    }

    return u32_err;
}

U32 eMMC_ReadBootPart(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr, U8 u8_PartNo)
{
    U32 u32_err;
    U16 u16_SecCnt;

    if(eMMC_NOT_READY_MARK == sgu32_IfReadyGuard)
    {
        u32_err = eMMC_Init();
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC not ready (init) \n");
            return eMMC_ST_ERR_NOT_INIT;
        }
    }

    // set Access Boot Partition 1
    if(u8_PartNo == 1)
        u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT0);
    else if(u8_PartNo == 2)
        u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1);

    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh, eMMC, set Ext_CSD[179]: %Xh fail\n",
            u32_err, BIT3|BIT0);
        return u32_err;
    }

    // read Boot Code
    u16_SecCnt = (u32_DataByteCnt>>9) + ((u32_DataByteCnt&0x1FF)?1:0);

    u32_err = eMMC_ReadData(pu8_DataBuf,
                  u16_SecCnt<<eMMC_SECTOR_512BYTE_BITS,
                  u32_BlkAddr);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,
            "eMMC Err: read Boot Partition fail, %Xh\n", u32_err);
        return u32_err;
    }

    // clear Access Boot Partition
    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3);

    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh, eMMC, set Ext_CSD[179]: %Xh fail\n",
            u32_err, BIT3);
        return u32_err;
    }

    return u32_err;
}


#endif
#else

#endif
