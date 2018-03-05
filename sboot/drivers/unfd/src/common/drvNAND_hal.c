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
#include "drvNAND_utl.h"

//========================================================
// HAL misc. function definitions
//========================================================
#if defined(NC_SEL_FCIE3) && NC_SEL_FCIE3
U32 NC_WaitComplete(U32 u32_EventReg, U16 u16_WaitEvent, U32 u32_MicroSec);

// ============================================================
#define WAIT_FIFOCLK_READY_CNT  0x10000
U32 NC_waitFifoClkReady(NAND_DRIVER *pNandDrv)
{
    volatile U32 u32_i;
    volatile U16 u16_Reg;

    for(u32_i=0; u32_i<WAIT_FIFOCLK_READY_CNT; u32_i++)
    {
        REG_READ_UINT16(NC_MMA_PRI_REG, u16_Reg);
        if(u16_Reg & BIT_NC_FIFO_CLKRDY)
            break;
        nand_hw_timer_delay(HW_TIMER_DELAY_1us);
    }

    if(WAIT_FIFOCLK_READY_CNT == u32_i)
    {
        NC_ResetFCIE(pNandDrv);

        return UNFD_ST_ERR_R_TIMEOUT;
    }

    return UNFD_ST_SUCCESS;

}

//========================================================
// HAL function definitions
//========================================================
U32 NC_ResetFCIE(NAND_DRIVER *pNandDrv)
{
    volatile U16 u16Reg, u16Cnt, u16Reg41h;

    NC_PlatformResetPre(pNandDrv);

    REG_WRITE_UINT16(NC_SIGNAL, ~DEF_REG0x40_VAL);

    // soft reset
    REG_CLR_BITS_UINT16(NC_TEST_MODE, BIT_FCIE_SOFT_RST); /* active low */
    nand_hw_timer_delay(HW_TIMER_DELAY_1us);
    REG_SET_BITS_UINT16(NC_TEST_MODE, BIT_FCIE_SOFT_RST);
    nand_hw_timer_delay(HW_TIMER_DELAY_1us);
    u16Cnt=0;
    do
    {
        nand_hw_timer_delay(HW_TIMER_DELAY_1us);
        if(0x1000 == u16Cnt++)
        {
            return UNFD_ST_ERR_NO_NFIE;
        }

        REG_READ_UINT16(NC_SIGNAL, u16Reg);
        REG_WRITE_UINT16(NC_WIDTH, ~FCIE_REG41_VAL);
        REG_READ_UINT16(NC_WIDTH, u16Reg41h);

    }while (DEF_REG0x40_VAL != u16Reg || (U16)(~FCIE_REG41_VAL) != u16Reg41h);

    // miu request reset
    REG_SET_BITS_UINT16(NC_MMA_PRI_REG, BIT_MIU_REQUEST_RST);
    nand_hw_timer_delay(HW_TIMER_DELAY_1us);
    REG_CLR_BITS_UINT16(NC_MMA_PRI_REG, BIT_MIU_REQUEST_RST);

    NC_PlatformResetPost(pNandDrv);

    return UNFD_ST_SUCCESS; // ok
}


U32 NC_ConfigNandFlashContext(NAND_DRIVER *pNandDrv)
{
    if (pNandDrv->u16_ECCType < ECC_TYPE_24BIT1KB)
        pNandDrv->u16_SectorByteCnt = 0x200;
    else
        pNandDrv->u16_SectorByteCnt = 0x400;

    pNandDrv->u8_BlkPageCntBits = drvNAND_CountBits(pNandDrv->u16_BlkPageCnt);
    pNandDrv->u8_PageByteCntBits = drvNAND_CountBits(pNandDrv->u16_PageByteCnt);
    //pNandDrv->u8_SpareByteCntBits = drvNAND_CountBits(pNandDrv->u16_SpareByteCnt);
    pNandDrv->u8_SectorByteCntBits = drvNAND_CountBits(pNandDrv->u16_SectorByteCnt);

    pNandDrv->u16_BlkPageCntMask = (1<<pNandDrv->u8_BlkPageCntBits)-1;
    pNandDrv->u16_PageByteCntMask = (1<<pNandDrv->u8_PageByteCntBits)-1;
    pNandDrv->u16_SectorByteCntMask = (1<<pNandDrv->u8_SectorByteCntBits)-1;
    //pNandDrv->u16_SpareByteCntMask = (1<<pNandDrv->u8_SpareByteCntBits)-1;

    pNandDrv->u16_PageSectorCnt = pNandDrv->u16_PageByteCnt >> pNandDrv->u8_SectorByteCntBits;
    pNandDrv->u8_PageSectorCntBits = drvNAND_CountBits(pNandDrv->u16_PageSectorCnt);
    pNandDrv->u16_PageSectorCntMask = (1<<pNandDrv->u8_PageSectorCntBits)-1;
    pNandDrv->u16_SectorSpareByteCnt = pNandDrv->u16_SpareByteCnt >> pNandDrv->u8_PageSectorCntBits;
    pNandDrv->u16_SectorSpareByteCnt &= ~1;
    pNandDrv->u16_SpareByteCnt = pNandDrv->u16_SectorSpareByteCnt << pNandDrv->u8_PageSectorCntBits;
    //pNandDrv->u8_SectorSpareByteCntBits = drvNAND_CountBits(pNandDrv->u16_SectorSpareByteCnt);

    pNandDrv->u8_BlkSectorCntBits = pNandDrv->u8_BlkPageCntBits + pNandDrv->u8_PageSectorCntBits;
    pNandDrv->u16_BlkSectorCnt = 1<<pNandDrv->u8_BlkSectorCntBits;
    pNandDrv->u16_BlkSectorCntMask = pNandDrv->u16_BlkSectorCnt-1;

    return UNFD_ST_SUCCESS;
}


U32 NC_Init(NAND_DRIVER *pNandDrv)
{
    U32 u32_RetVal;

    // disable NC
    REG_CLR_BITS_UINT16(NC_PATH_CTL, BIT_NC_EN);
    REG_WRITE_UINT16(NC_CTRL , 0);
    // reset NC
    u32_RetVal = NC_ResetFCIE(pNandDrv);
    if (UNFD_ST_SUCCESS != u32_RetVal)
    {
        return u32_RetVal;
    }
    // disable interupts
    REG_CLR_BITS_UINT16(NC_MIE_INT_EN, BIT_MMA_DATA_END | BIT_NC_JOB_END);
    // clean int events
    REG_W1C_BITS_UINT16(NC_MIE_EVENT, BIT_MMA_DATA_END | BIT_NC_JOB_END);
    // enable NC
    REG_SET_BITS_UINT16(NC_PATH_CTL, BIT_NC_EN);

    // config NC registers context
    NC_RegInit(pNandDrv);

    #if defined(NC_HWCMD_DELAY) && NC_HWCMD_DELAY
	REG_READ_UINT16(NC_RAND_W_CMD, pNandDrv->u16_Reg56_Rand_W_Cmd);
    #endif

    #if defined(NC_TRR_TCS) && NC_TRR_TCS
	REG_READ_UINT16(NC_LFSR_CTRL, pNandDrv->u16_Reg59_LFSRCtrl);
    #endif

    NC_Config(pNandDrv);

    // ===================================================	
    // reset NAND flash

    u32_RetVal = NC_ResetNandFlash(pNandDrv);
    if(UNFD_ST_SUCCESS != u32_RetVal)
    {
        return u32_RetVal;
    }

    return UNFD_ST_SUCCESS;
}



U32 NC_RegInit(NAND_DRIVER *pNandDrv)
{
    // config NC
    pNandDrv->u16_Reg1B_SdioCtrl = (pNandDrv->u16_SectorByteCnt +
            (pNandDrv->u16_SpareByteCnt >> pNandDrv->u8_PageSectorCntBits)) &~ 0x1UL;

    if (NC_MAX_SECTOR_BYTE_CNT < pNandDrv->u16_Reg1B_SdioCtrl)
    {
        nand_die();
    }
    pNandDrv->u16_Reg1B_SdioCtrl |= BIT_SDIO_BLK_MODE;

    pNandDrv->u16_Reg48_Spare = (pNandDrv->u16_SpareByteCnt >> pNandDrv->u8_PageSectorCntBits);
    if (NC_MAX_SECTOR_SPARE_BYTE_CNT < pNandDrv->u16_Reg48_Spare || (pNandDrv->u16_Reg48_Spare & 1))
    {
        nand_die();
    }

    if(pNandDrv->u16_SpareByteCnt >= 512)
    {
        pNandDrv->u16_Reg48_Spare |= BIT_NC_SPARE_DEST_MIU;
    }

    pNandDrv->u16_Reg49_SpareSize = pNandDrv->u16_SpareByteCnt;
    if (NC_MAX_TOTAL_SPARE_BYTE_CNT < pNandDrv->u16_Reg49_SpareSize || (pNandDrv->u16_Reg49_SpareSize & 1))
    {
        nand_die();
    }

    pNandDrv->u16_Reg50_EccCtrl = REG50_ECC_CTRL_INIT_VALUE;

    switch (pNandDrv->u16_PageByteCnt)
    {
        case 0x0800:  pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_2KB;  break;
        case 0x1000:  pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_4KB;  break;
        case 0x2000:  pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_8KB;  break;
        case 0x4000:  pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_16KB;  break;
        case 0x8000:  pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_32KB;  break;
        default:
                      nand_die();
    }

    switch (pNandDrv->u16_ECCType)
    {
        case ECC_TYPE_4BIT:
            pNandDrv->u16_Reg50_EccCtrl &= ~BIT_NC_ECC_TYPE_4b512Bn;
            pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_4BIT;
			pNandDrv->u16_ECCCorretableBit = 4;
            break;
        case ECC_TYPE_8BIT:
            pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_8b512B;
            pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_8BIT;
			pNandDrv->u16_ECCCorretableBit = 8;
            break;
        case ECC_TYPE_12BIT:
            pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_12b512B;
            pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_12BIT;
			pNandDrv->u16_ECCCorretableBit = 12;
            break;
        case ECC_TYPE_16BIT:
            pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_16b512B;
            pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_16BIT;
			pNandDrv->u16_ECCCorretableBit = 16;
            break;
        case ECC_TYPE_20BIT:
            pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_20b512B;
            pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_20BIT;
			pNandDrv->u16_ECCCorretableBit = 20;
            break;
        case ECC_TYPE_24BIT:
            pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_24b512B;
            pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_24BIT;
			pNandDrv->u16_ECCCorretableBit = 24;
            break;
        case ECC_TYPE_24BIT1KB:
            pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_24b1KB;
            pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_24BIT1KB;
			pNandDrv->u16_ECCCorretableBit = 24;
            break;
        case ECC_TYPE_32BIT1KB:
            pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_32b1KB;
            pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_32BIT1KB;
			pNandDrv->u16_ECCCorretableBit = 32;
            break;
        case ECC_TYPE_40BIT1KB:
            pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_40b1KB;
            pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_40BIT1KB;
			pNandDrv->u16_ECCCorretableBit = 40;
            break;
        default:
            nand_die();
    }
    pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECCERR_NSTOP;
    return UNFD_ST_SUCCESS;
}

void NC_Config(NAND_DRIVER *pNandDrv)
{
    REG_WRITE_UINT16(NC_SDIO_CTL, pNandDrv->u16_Reg1B_SdioCtrl);
    REG_WRITE_UINT16(NC_SIGNAL, pNandDrv->u16_Reg40_Signal);
    /*sector spare size*/
    REG_WRITE_UINT16(NC_SPARE, pNandDrv->u16_Reg48_Spare);
    /* page spare size*/
    REG_WRITE_UINT16(NC_SPARE_SIZE, (U16)pNandDrv->u16_Reg49_SpareSize);
    /* page size and ECC type*/
    REG_WRITE_UINT16(NC_ECC_CTRL, pNandDrv->u16_Reg50_EccCtrl);

    REG_WRITE_UINT16(NC_LATCH_DATA, pNandDrv->u16_Reg57_RELatch);

    #if defined(NC_TWHR_TCLHZ) && NC_TWHR_TCLHZ
    /*set timing for tCLHZ*/
    REG_WRITE_UINT16(NC_NAND_TIMING, pNandDrv->u16_Reg5A_tWHR_tCLHZ);
    #endif

    #if defined(NC_TCWAW_TADL) && NC_TCWAW_TADL
    /*set timing for tADL and tCWAW*/
    REG_WRITE_UINT16(NC_NAND_TIMING1,pNandDrv->u16_Reg5D_tCWAW_tADL);
    #endif

    #if (defined(FCIE_LFSR) && FCIE_LFSR) || (defined(NC_TRR_TCS) && NC_TRR_TCS)
    REG_WRITE_UINT16(NC_LFSR_CTRL, pNandDrv->u16_Reg59_LFSRCtrl);
    #endif

    #if defined(NC_HWCMD_DELAY) && NC_HWCMD_DELAY
    REG_WRITE_UINT16(NC_RAND_W_CMD, pNandDrv->u16_Reg56_Rand_W_Cmd);
    #endif

	
}

void NC_ReConfig(NAND_DRIVER *pNandDrv)
{
	NC_ResetFCIE(pNandDrv);
	NC_Config(pNandDrv);
}

static U32 ErrorBitCountforEmptyPage(U8 u8_Byte)
{
    int i;
    int sum = 0;
    if(u8_Byte ^ 0xFF)
    {
        for(i = 0; i < 8; i ++)
            sum += ((u8_Byte >> i)  & 0x1) ^ 1;
    }
    return sum;
}

/*
* Check whether the cause of ECC fail is reading a non-all0xff empty page.
* @main: Main Data of a NAND Page
* @spare: Spare Data of a NAND Page
* @return 1: if False Alarm 0: Real ECC Fail
*/
int nand_CheckEmptyPageFalseAlarm(NAND_DRIVER *pNandDrv, U8 *main, U8 *spare)
{
	 int ECCErrBitCnt = 0;
     int i,j;
	 
     for(j = 0; j < pNandDrv->u16_PageSectorCnt; j++)
     {
         ECCErrBitCnt = 0;
		
	     for(i=0;i< pNandDrv->u16_SectorByteCnt;i++)
	     {
		     ECCErrBitCnt += ErrorBitCountforEmptyPage(main[j*pNandDrv->u16_SectorByteCnt+i]);
		     if(ECCErrBitCnt > pNandDrv->u16_ECCCorretableBit)
		         return 0;		 
	     }
		 for(i=0;i< pNandDrv->u16_SectorSpareByteCnt;i++)
		 {
			 ECCErrBitCnt += ErrorBitCountforEmptyPage(spare[j*pNandDrv->u16_SectorSpareByteCnt+i]);
			 if(ECCErrBitCnt > pNandDrv->u16_ECCCorretableBit)
				 return 0;		 
		 }
     }
	 //not read ecc fail
     nand_memset(main, 0xff, pNandDrv->u16_PageByteCnt);
     nand_memset(spare,0xff, pNandDrv->u16_SpareByteCnt);


     return 1; 

}

int nand_CheckEmptySectorFalseAlarm(NAND_DRIVER *pNandDrv, U8 *main, U8 *spare)
{
	 int ECCErrBitCnt = 0;
     int i,j;
	 

     ECCErrBitCnt = 0;
	
     for(i=0;i< pNandDrv->u16_SectorByteCnt;i++)
     {
	     ECCErrBitCnt += ErrorBitCountforEmptyPage(main[i]);
	     if(ECCErrBitCnt > pNandDrv->u16_ECCCorretableBit)
	         return 0;		 
     }
	 for(i=0;i< pNandDrv->u16_SectorSpareByteCnt;i++)
	 {
		 ECCErrBitCnt += ErrorBitCountforEmptyPage(spare[i]);
		 if(ECCErrBitCnt > pNandDrv->u16_ECCCorretableBit)
			 return 0;		 
	 }

	 //not read ecc fail
     nand_memset(main, 0xff, pNandDrv->u16_SectorByteCnt);
     nand_memset(spare,0xff, pNandDrv->u16_SectorSpareByteCnt);


     return 1; 

}


#if defined(ENABLE_NAND_RIU_MODE) && ENABLE_NAND_RIU_MODE
U32  NC_ReadSector_RIUMode(NAND_DRIVER *pNandDrv, U32 u32_PhyRowIdx, U8 u8_SectorInPage, U16 *pu16_DataInBuf, U8 *pu8_SpareBuf, U8 u8_RandCmdDis)
{
    volatile U16 u16_Tmp, u16_i, u16_j, u16_k, u16_Cnt, u16_ByteLoc;
    U16 u16_Col;
    U8 *pu8_DataBuf = (U8 *)pu16_DataInBuf;
    U16 *pu16_DataBuf = pu16_DataInBuf;
    U32 u32_Err = UNFD_ST_SUCCESS, u32_Tmp;
    U16 u16_PageByteCnt = 0, u16_SpareByteCnt = 0, u16_ECCType = 0;

    if(u8_RandCmdDis)
    {
        u16_PageByteCnt = pNandDrv->u16_PageByteCnt;
        u16_SpareByteCnt = pNandDrv->u16_SpareByteCnt;
        u16_ECCType = pNandDrv->u16_ECCType;

        pNandDrv->u16_PageByteCnt = 0x800;
        pNandDrv->u16_SpareByteCnt = 0x100;
        pNandDrv->u16_ECCType = NANDINFO_ECC_TYPE;

        NC_ConfigNandFlashContext(pNandDrv);
        NC_RegInit(pNandDrv);
        pNandDrv->u16_Reg48_Spare |= BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;		
        NC_Config(pNandDrv);
    }

    REG_SET_BITS_UINT16(NC_R2N_CTRL, BIT_R2N_MODE_EN); // enable RIU Mode

    // if Partial Mode (if not 512B/page, set Partial Mode)
#if 0
    REG_WRITE_UINT16(NC_PART_MODE, BIT_PARTIAL_MODE_EN);
    REG_CLR_BITS_UINT16(NC_PART_MODE, BIT_START_SECTOR_IDX_MASK);
    REG_SET_BITS_UINT16(NC_PART_MODE, u8_SectorInPage<<BIT_START_SECTOR_IDX_SHIFT);
#else
    REG_WRITE_UINT16(NC_PART_MODE, BIT_PARTIAL_MODE_EN|(u8_SectorInPage<<BIT_START_SECTOR_IDX_SHIFT));
#endif
    REG_WRITE_UINT16(NC_JOB_BL_CNT, 1); /* read 1 sector only */
    //REG_WRITE_UINT16(NC_SDIO_ADDR0, 0);
    //REG_WRITE_UINT16(NC_SDIO_ADDR1, 0);
    u32_Err = NC_waitFifoClkReady(pNandDrv);
    if( u32_Err != UNFD_ST_SUCCESS)
        goto EXIT;

    REG_SET_BITS_UINT16(NC_PATH_CTL, BIT_MMA_EN);

    REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_ADRSET);
    u16_Col = u8_SectorInPage << pNandDrv->u8_SectorByteCntBits;	
    REG_WRITE_UINT16(NC_AUXREG_DAT, u16_Col);
    REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx & 0xFFFF);
    REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx >> 16);

    REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
    REG_WRITE_UINT16(NC_AUXREG_DAT, (pNandDrv->u8_OpCode_RW_AdrCycle<< 8) | (CMD_0x00));

    REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_WAITRB << 8) | CMD_0x30);
    REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_BREAK << 8) | ACT_SER_DIN);

    REG_WRITE_UINT16(NC_CTRL,
            (BIT_NC_CIFD_ACCESS|BIT_NC_JOB_START));

    u16_Cnt = pNandDrv->u16_SectorByteCnt >> 9; // how many 512B per sector
    for (u16_j=0; u16_j<u16_Cnt; u16_j++)
    {   // read data
        if (NC_WaitComplete(NC_R2N_STAT,BIT_RIU_RDY_MMA, WAIT_READ_TIME)==  WAIT_READ_TIME)
        {
            u32_Err = UNFD_ST_ERR_R_TIMEOUT_RM; // timeout
            goto EXIT;
        }

        if (0 == u16_j)
            REG_SET_BITS_UINT16(NC_R2N_CTRL, BIT_R2N_DI_START);

        u16_k = u16_j << 8;

        for (u16_i=0; u16_i<256; u16_i++)
        {
            if (NC_WaitComplete(NC_MIE_EVENT, BIT_NC_R2N_RDY, WAIT_READ_TIME) == WAIT_READ_TIME)
            {
                u32_Err = UNFD_ST_ERR_R_TIMEOUT_RM; // timeout
                goto EXIT;
            }
            REG_READ_UINT16(NC_R2N_DATA_RD, pu16_DataBuf[u16_k + u16_i]);
            REG_SET_BITS_UINT16(NC_R2N_CTRL, BIT_R2N_DI_EN);
        }
    }
    REG_WRITE_UINT16(NC_R2N_CTRL, BIT_R2N_MODE_EN|BIT_R2N_DI_END);

    // wait for events
    u32_Tmp = 0;
    do{
        u16_Tmp = 0; // do not remove this line
        REG_READ_UINT16(NC_MIE_EVENT, u16_Tmp);
        nand_hw_timer_delay(HW_TIMER_DELAY_1us);
        u32_Tmp++;
        if(u32_Tmp > WAIT_READ_TIME)
        {
            u32_Err = UNFD_ST_ERR_R_TIMEOUT_RM;
            goto EXIT;
        }
    }while(((u16_Tmp & BIT_NC_R2N_ECC)==0) && ((u16_Tmp & (BIT_NC_JOB_END | BIT_MMA_DATA_END)) != (BIT_NC_JOB_END | BIT_MMA_DATA_END)));

    // if ECC Fail
    REG_READ_UINT16(NC_ECC_STAT2, u16_Tmp);
    if ((u16_Tmp & BIT_NC_ECC_FLGA_MASK) == BIT_NC_ECC_FLGA_FAIL)
    {
        u32_Err = UNFD_ST_ERR_ECC_FAIL_RM;
        goto EXIT;
    }

    REG_READ_UINT16(NC_MIE_EVENT, u16_Tmp);
    if (u16_Tmp & BIT_NC_R2N_ECC)
    {
        // make CIFD available for SW
        // [CAUTION]: before clear BIT_NC_R2N_ECC, JOB_END would not set.
        REG_W1C_BITS_UINT16(NC_MIE_EVENT, BIT_NC_R2N_ECC);
        if(NC_WaitComplete(NC_MIE_EVENT, BIT_NC_JOB_END|BIT_MMA_DATA_END, WAIT_READ_TIME) == WAIT_READ_TIME)
        {
            REG_CLR_BITS_UINT16(NC_PATH_CTL, BIT_MMA_EN);
            REG_CLR_BITS_UINT16(NC_R2N_CTRL, BIT_R2N_MODE_EN);
        }

        // SW correct ECC bits
        if(pu8_SpareBuf)
            NC_GetCIFD(pu8_SpareBuf, 0, pNandDrv->u16_SectorSpareByteCnt-pNandDrv->u16_ECCCodeByteCnt);

        REG_READ_UINT16(NC_ECC_STAT2, u16_Tmp);
        u16_Cnt = (u16_Tmp & BIT_NC_ECC_CNT_MASK) >> BIT_NC_ECC_CNT_SHIFT;
        for (u16_i=0; u16_i<u16_Cnt; u16_i++)
        {
            REG_CLR_BITS_UINT16(NC_ECC_STAT2, BIT_NC_ECC_SEL_LOC_MASK);
            REG_SET_BITS_UINT16(NC_ECC_STAT2, u16_i<<BIT_NC_ECC_SEL_LOC_SHIFT);
LABEL_ECC_LOC_READ:
            //REG_READ_UINT16(NC_ECC_LOC, u16_Tmp);
            REG_READ_UINT16(NC_ECC_LOC, u16_j);
            REG_READ_UINT16(NC_ECC_LOC, u16_k);
            if(u16_j != u16_k)
                goto LABEL_ECC_LOC_READ;
            u16_ByteLoc = u16_j >> 3;
            if (u16_ByteLoc < pNandDrv->u16_SectorByteCnt)
            {
                pu8_DataBuf[u16_ByteLoc] ^= 1<<(u16_j%8);
            }
            else if (u16_ByteLoc < pNandDrv->u16_SectorByteCnt +
                    pNandDrv->u16_SectorSpareByteCnt - pNandDrv->u16_ECCCodeByteCnt)
            {
                pu8_SpareBuf[u16_ByteLoc-pNandDrv->u16_SectorByteCnt] ^= 1<<(u16_j%8);
            }
        }

        u16_Tmp = BIT_NC_R2N_ECC;
    }
    else{
        if (NC_WaitComplete(NC_MIE_EVENT, BIT_NC_JOB_END|BIT_MMA_DATA_END, WAIT_READ_TIME) == WAIT_READ_TIME)
        {
            u32_Err = UNFD_ST_ERR_R_TIMEOUT_RM;
            goto EXIT;
        }

        NC_GetCIFD(pu8_SpareBuf, 0, pNandDrv->u16_SectorSpareByteCnt-pNandDrv->u16_ECCCodeByteCnt);
    }

EXIT:
    REG_WRITE_UINT16(NC_PART_MODE, 0);
    REG_CLR_BITS_UINT16(NC_PATH_CTL, BIT_MMA_EN);
    REG_CLR_BITS_UINT16(NC_R2N_CTRL, BIT_R2N_MODE_EN);
    if(u8_RandCmdDis)
    {
        pNandDrv->u16_PageByteCnt = u16_PageByteCnt;
        pNandDrv->u16_SpareByteCnt = u16_SpareByteCnt;
        pNandDrv->u16_ECCType = u16_ECCType;

        NC_ConfigNandFlashContext(pNandDrv);
        NC_RegInit(pNandDrv);
        pNandDrv->u16_Reg48_Spare &= (~BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE);		
        NC_Config(pNandDrv);
    }	
    return u32_Err;
}
#else
#if (defined(MIU_CHECK_LAST_DONE)&&MIU_CHECK_LAST_DONE)
U32 NC_wait_MIULastDone(NAND_DRIVER *pNandDrv)
{
    U32 u32_retry=0;
    U16 u16_Reg;
    while(u32_retry < DELAY_1s_in_us)
    {
        //REG_READ_UINT16(NC_PATH_CTL, u16_Reg);
        REG_READ_UINT16(NC_REG_MIU_LAST_DONE, u16_Reg);
        if((u16_Reg & BIT_MIU_LAST_DONE) == BIT_MIU_LAST_DONE)
        {
            REG_W1C_BITS_UINT16(NC_REG_MIU_LAST_DONE,BIT_MIU_LAST_DONE); //Must  Clear LAST DONE @@
            break;
        }
        u32_retry++;
        nand_hw_timer_delay(1);
    }
    if(u32_retry == DELAY_1s_in_us)
    {
        NC_ResetFCIE(pNandDrv);
        return UNFD_ST_ERR_R_TIMEOUT;
    }
    return UNFD_ST_SUCCESS;
}
#endif
U32 NC_ReadSector_DMAMode(NAND_DRIVER *pNandDrv, U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U8 u8_RandCmdDis)
{
    U32 u32_DataDMAAddr;
    U32 u32_SpareDMAAddr;
    U32 u32_Err = UNFD_ST_SUCCESS;
    U16 u16_Tmp;
    U16 u16_PageByteCnt = 0, u16_SpareByteCnt = 0, u16_ECCType = 0;
	int falsealarm;

    if(u8_RandCmdDis)
    {
        u16_PageByteCnt = pNandDrv->u16_PageByteCnt;
        u16_SpareByteCnt = pNandDrv->u16_SpareByteCnt;
        u16_ECCType = pNandDrv->u16_ECCType;
        pNandDrv->u16_PageByteCnt = 0x800;
        pNandDrv->u16_SpareByteCnt = 0x100;
        pNandDrv->u16_ECCType = NANDINFO_ECC_TYPE;
        NC_ConfigNandFlashContext(pNandDrv);
        NC_RegInit(pNandDrv);
        pNandDrv->u16_Reg48_Spare |= BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;		
        NC_Config(pNandDrv);
    }
    REG_WRITE_UINT16(NC_MIE_EVENT, (BIT_NC_JOB_END | BIT_MMA_DATA_END));
    u32_DataDMAAddr = nand_translate_DMA_address_Ex((U32)pu8_DataBuf, pNandDrv->u16_SectorByteCnt);
    if(pNandDrv->u16_Reg48_Spare & BIT_NC_SPARE_DEST_MIU)
    {
        u32_SpareDMAAddr = nand_translate_DMA_address_Ex((U32)pu8_SpareBuf, pNandDrv->u16_SectorSpareByteCnt);
        REG_WRITE_UINT16(NC_SPARE_DMA_ADR0, u32_SpareDMAAddr & 0xFFFF);
        REG_WRITE_UINT16(NC_SPARE_DMA_ADR1, u32_SpareDMAAddr >>16);
    }
    REG_WRITE_UINT16(NC_JOB_BL_CNT, 1);
    REG_WRITE_UINT16(NC_PART_MODE, BIT_PARTIAL_MODE_EN|(u8_SectorInPage<<BIT_START_SECTOR_IDX_SHIFT));
    REG_WRITE_UINT16(NC_SDIO_ADDR0, u32_DataDMAAddr & 0xFFFF);
    REG_WRITE_UINT16(NC_SDIO_ADDR1, u32_DataDMAAddr >> 16);
    REG_CLR_BITS_UINT16(NC_MMA_PRI_REG, BIT_NC_DMA_DIR_W);
    u32_Err=NC_waitFifoClkReady(pNandDrv);
    if(u32_Err != UNFD_ST_SUCCESS)
    {
       goto  EXIT;
    }
    REG_SET_BITS_UINT16(NC_PATH_CTL, BIT_MMA_EN);
    REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_ADRSET);
    REG_WRITE_UINT16(NC_AUXREG_DAT, u8_SectorInPage << pNandDrv->u8_SectorByteCntBits);
    REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx & 0xFFFF);
    REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx >> 16);
    REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
    REG_WRITE_UINT16(NC_AUXREG_DAT, (pNandDrv->u8_OpCode_RW_AdrCycle<< 8) | (CMD_0x00));
    REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_WAITRB << 8) | CMD_0x30);
    REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_BREAK << 8) | ACT_SER_DIN);
    REG_WRITE_UINT16(NC_CTRL, (BIT_NC_CIFD_ACCESS|BIT_NC_JOB_START) & ~(BIT_NC_IF_DIR_W));
    if (NC_WaitComplete(NC_MIE_EVENT, BIT_NC_JOB_END|BIT_MMA_DATA_END, WAIT_READ_TIME) == (WAIT_READ_TIME))
    {
        u32_Err = UNFD_ST_ERR_R_TIMEOUT;
        goto  EXIT;
    }


    // check until MIU is done
    #if (defined(MIU_CHECK_LAST_DONE)&&MIU_CHECK_LAST_DONE)
    u32_Err=NC_wait_MIULastDone(pNandDrv);
    if( u32_Err != UNFD_ST_SUCCESS)
	{
        goto  EXIT;
    }
    #endif
    if (pu8_SpareBuf && (pNandDrv->u16_Reg48_Spare & BIT_NC_SPARE_DEST_MIU) == 0)
    {
        #if SPARE640B_CIFD512B_PATCH
        if(pNandDrv->u16_SectorSpareByteCnt > 0x200)
            NC_GetCIFD(pu8_SpareBuf, 0, 0x200);
        else
        #endif
            NC_GetCIFD(pu8_SpareBuf, 0, pNandDrv->u16_SectorSpareByteCnt);
    }

    // Check ECC
    REG_READ_UINT16(NC_ECC_STAT0, u16_Tmp);
    if (u16_Tmp & BIT_NC_ECC_FAIL)
    {  
	    falsealarm = nand_CheckEmptySectorFalseAlarm(pNandDrv, pu8_DataBuf, pu8_SpareBuf);
	    if(falsealarm == 1)
	    {
		    NC_ReConfig(pNandDrv);
		    u32_Err = UNFD_ST_SUCCESS;
	    }
	    else
		    u32_Err = UNFD_ST_ERR_ECC_FAIL;
    }
EXIT:
    REG_WRITE_UINT16(NC_PART_MODE, 0);


	
    if(u8_RandCmdDis)
    {
        pNandDrv->u16_PageByteCnt = u16_PageByteCnt;
        pNandDrv->u16_SpareByteCnt = u16_SpareByteCnt;
        pNandDrv->u16_ECCType = u16_ECCType;
        NC_ConfigNandFlashContext(pNandDrv);
        NC_RegInit(pNandDrv);
        pNandDrv->u16_Reg48_Spare &= (~BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE);		
        NC_Config(pNandDrv);
    }
    return u32_Err;
}
#endif

static U32 NC_ReadPages_Ex(NAND_DRIVER *pNandDrv, U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt)
{
	 U32 u32_DataDMAAddr;
	 U32 u32_SpareDMAAddr;
	 U32 u32_Err=UNFD_ST_SUCCESS;
	 U16 u16_Tmp;
	 int falsealarm;
	
	 REG_WRITE_UINT16(NC_MIE_EVENT, (BIT_NC_JOB_END | BIT_MMA_DATA_END));
	
	 u32_DataDMAAddr = nand_translate_DMA_address_Ex((U32)pu8_DataBuf, pNandDrv->u8_PageSectorCntBits*u32_PageCnt);
	 if(pNandDrv->u16_Reg48_Spare & BIT_NC_SPARE_DEST_MIU)
	 {
		 u32_SpareDMAAddr = nand_translate_DMA_address_Ex((U32)pu8_SpareBuf, pNandDrv->u16_SpareByteCnt);
		 REG_WRITE_UINT16(NC_SPARE_DMA_ADR0, u32_SpareDMAAddr & 0xFFFF);
		 REG_WRITE_UINT16(NC_SPARE_DMA_ADR1, u32_SpareDMAAddr >>16);
	 }
	 REG_WRITE_UINT16(NC_SER_DIN_BYTECNT_LW, pNandDrv->u16_PageByteCnt & 0xFFFF);
	 REG_WRITE_UINT16(NC_SER_DIN_BYTECNT_HW, pNandDrv->u16_PageByteCnt >> 16);
	
	 REG_WRITE_UINT16(NC_JOB_BL_CNT, u32_PageCnt << pNandDrv->u8_PageSectorCntBits);
	 REG_WRITE_UINT16(NC_SDIO_ADDR0, u32_DataDMAAddr & 0xFFFF);
	 REG_WRITE_UINT16(NC_SDIO_ADDR1, u32_DataDMAAddr >> 16);
	 REG_CLR_BITS_UINT16(NC_MMA_PRI_REG, BIT_NC_DMA_DIR_W);
	 u32_Err = NC_waitFifoClkReady(pNandDrv);
	 if( u32_Err != UNFD_ST_SUCCESS)
		 return u32_Err;
	
	 REG_SET_BITS_UINT16(NC_PATH_CTL, BIT_MMA_EN);
	
	 REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_ADRSET);
	 REG_WRITE_UINT16(NC_AUXREG_DAT, 0);
	 REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx & 0xFFFF);
	 REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx >> 16);
	
	 REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
	 REG_WRITE_UINT16(NC_AUXREG_DAT, (pNandDrv->u8_OpCode_RW_AdrCycle<< 8) | (CMD_0x00));
	
	 REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_WAITRB << 8) | CMD_0x30);
	 REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_REPEAT << 8) | ACT_SER_DIN);
	 REG_WRITE_UINT16(NC_AUXREG_DAT, ACT_BREAK);
	 REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_RPTCNT);
	 REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PageCnt - 1);
	
	 REG_WRITE_UINT16(NC_CTRL, (BIT_NC_CIFD_ACCESS|BIT_NC_JOB_START) & ~(BIT_NC_IF_DIR_W));
	
	if(NC_WaitComplete(NC_MIE_EVENT, BIT_NC_JOB_END|BIT_MMA_DATA_END, WAIT_READ_TIME * u32_PageCnt) ==
	   (WAIT_READ_TIME * u32_PageCnt ))
	{
		return UNFD_ST_ERR_R_TIMEOUT;
	}

	
    #if defined(MIU_CHECK_LAST_DONE) && MIU_CHECK_LAST_DONE
	// check until MIU is done
	u32_Err = NC_wait_MIULastDone(pNandDrv);
	if(u32_Err != UNFD_ST_SUCCESS)
		return u32_Err;
    #endif
	
	if((pNandDrv->u16_Reg48_Spare & BIT_NC_SPARE_DEST_MIU) == 0)
	{
		if(pNandDrv->u16_SpareByteCnt > 0x200)
			NC_GetCIFD(pu8_SpareBuf, 0, 0x200);
		else
			NC_GetCIFD(pu8_SpareBuf, 0, pNandDrv->u16_SpareByteCnt);
	}

	// Check ECC
	REG_READ_UINT16(NC_ECC_STAT0, u16_Tmp);
	if (u16_Tmp & BIT_NC_ECC_FAIL)
	{
		falsealarm = nand_CheckEmptyPageFalseAlarm(pNandDrv, pu8_DataBuf, pu8_SpareBuf);
		if(falsealarm == 1)
		{
			NC_ReConfig(pNandDrv);
		    return UNFD_ST_SUCCESS;
		}
		else
			return UNFD_ST_ERR_ECC_FAIL;
	}

	return UNFD_ST_SUCCESS;

}

U32 NC_ReadPages(NAND_DRIVER *pNandDrv, U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt)
{
    U32 u32_i, u32_k;
    U32 u32_Err;
    #if defined(ENABLE_NAND_RIU_MODE) && ENABLE_NAND_RIU_MODE
    for(u32_i=0 ; u32_i<u32_PageCnt ; u32_i++)
    {
        for(u32_k=0 ; u32_k<pNandDrv->u16_PageSectorCnt; u32_k++)
        {
            u32_Err = NC_ReadSector_RIUMode(pNandDrv, u32_PhyRowIdx+u32_i, u32_k,
                pu8_DataBuf+(u32_i<<pNandDrv->u8_PageByteCntBits)+(u32_k<<pNandDrv->u8_SectorByteCntBits), 
                pu8_SpareBuf+u32_k*pNandDrv->u16_SectorSpareByteCnt, 0);
            if(u32_Err != UNFD_ST_SUCCESS)
                return u32_Err;
        }
    }
    #else
	for(u32_i=0 ; u32_i<u32_PageCnt ; u32_i++)
	{
        u32_Err = NC_ReadPages_Ex(pNandDrv, u32_PhyRowIdx+u32_i, 
			pu8_DataBuf+(u32_i<<pNandDrv->u8_PageByteCntBits), 
			pu8_SpareBuf, 1);

		if(u32_Err != UNFD_ST_SUCCESS)
			return u32_Err;

	}
	#endif
    return u32_Err;
}


U32 NC_ProbeReadSeq(NAND_DRIVER *pNandDrv)
{
    volatile U16 u16_Reg;
    volatile U32 u32_Count;

    REG_WRITE_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END|BIT_MMA_DATA_END);

    REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_ADRSET);
    REG_WRITE_UINT16(NC_AUXREG_DAT, 0);
    REG_WRITE_UINT16(NC_AUXREG_DAT, 0);
    REG_WRITE_UINT16(NC_AUXREG_DAT, 0);

    REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
    REG_WRITE_UINT16(NC_AUXREG_DAT, (pNandDrv->u8_OpCode_RW_AdrCycle<< 8) | (CMD_0x00));

    if (pNandDrv->u16_Reg48_Spare& BIT_NC_ONE_COL_ADDR)  // if a page 512B
    {
        REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_BREAK << 8) | ACT_WAITRB);
    }
    else
    {
        REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_WAITRB << 8) | CMD_0x30);
        REG_WRITE_UINT16(NC_AUXREG_DAT, ACT_BREAK);
    }

    REG_WRITE_UINT16(NC_CTRL, BIT_NC_JOB_START);
    for (u32_Count=0; u32_Count < WAIT_PROBE_TIME; u32_Count++)
    {
        REG_READ_UINT16(NC_MIE_EVENT, u16_Reg);
        if ((u16_Reg & BIT_NC_JOB_END) == BIT_NC_JOB_END)
            break;

        nand_hw_timer_delay(HW_TIMER_DELAY_1us);
    }

    if (u32_Count < WAIT_PROBE_TIME)
        REG_W1C_BITS_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END); /*clear events*/
    else
    {
        NC_ResetFCIE(pNandDrv);
        NC_Config(pNandDrv);
        NC_ResetNandFlash(pNandDrv);
        return UNFD_ST_ERR_R_TIMEOUT;
    }

    return UNFD_ST_SUCCESS;
}

U32 NC_ResetNandFlash(NAND_DRIVER *pNandDrv)
{
    REG_W1C_BITS_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END);

    REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);

    REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_WAITRB << 8) | CMD_0xFF);

    REG_WRITE_UINT16(NC_AUXREG_DAT, ACT_BREAK);

    REG_WRITE_UINT16(NC_CTRL, BIT_NC_JOB_START);

    if(NC_WaitComplete(NC_MIE_EVENT, BIT_NC_JOB_END, WAIT_RESET_TIME) == WAIT_RESET_TIME)		
    {
        NC_ResetFCIE(pNandDrv);
        NC_Config(pNandDrv);

        return UNFD_ST_ERR_RST_TIMEOUT;
    }

    return UNFD_ST_SUCCESS;
}

U32 NC_WaitComplete(U32 u32_EventReg, U16 u16_WaitEvent, U32 u32_MicroSec)
{
    volatile U32 u32_Count;
    volatile U16 u16_Reg;

    for (u32_Count=0; u32_Count < u32_MicroSec; u32_Count++)
    {
        REG_READ_UINT16(u32_EventReg, u16_Reg);
        if ((u16_Reg & u16_WaitEvent) == u16_WaitEvent)
            break;

        nand_hw_timer_delay(HW_TIMER_DELAY_1us);
    }

    if (u32_Count < u32_MicroSec)
        REG_W1C_BITS_UINT16(u32_EventReg, u16_WaitEvent); /*clear events*/

    return u32_Count;

}

void NC_GetCIFD(U8 *pu8_Buf, U32 u32_CIFDPos, U32 u32_ByteCnt)
{
    U32 u32_i, u32_BufPos;
    U16 u16_Tmp;

    if (u32_CIFDPos & 1) {
        REG_READ_UINT16(NC_CIFD_ADDR(u32_CIFDPos>>1), u16_Tmp);
        pu8_Buf[0] = (U8)(u16_Tmp >> 8);
        u32_CIFDPos += 1;
        u32_ByteCnt -= 1;
        u32_BufPos = 1;
    } else
        u32_BufPos = 0;

    for (u32_i=0; u32_i<u32_ByteCnt>>1; u32_i++) {
        REG_READ_UINT16(NC_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
        pu8_Buf[(u32_i<<1)+u32_BufPos] = (U8)u16_Tmp;
        pu8_Buf[(u32_i<<1)+u32_BufPos+1] = (U8)(u16_Tmp>>8);
    }

    if (u32_ByteCnt - (u32_i<<1)) {
        REG_READ_UINT16(NC_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
        pu8_Buf[(u32_i<<1)+u32_BufPos] = (U8)u16_Tmp;
    }

}

#endif // NC_SEL_FCIE3
#endif // (defined(CONFIG_MSTAR_KANO)||defined(CONFIG_MSTAR_CURRY)||defined(CONFIG_MSTAR_K6)||defined(CONFIG_MSTAR_K6LITE))&&(!defined(CONFIG_MSTAR_BFN_EN))
