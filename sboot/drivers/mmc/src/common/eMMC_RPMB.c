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
//=======================================================================
//  MStar Semiconductor - Unified eMMC Driver
//
//  eMMC_hal.c - Storage Team, 2011/10/03
//
//  Design Notes:
//    2011/10/03 - FCIE3 physical layer functions (hill.sung)
//
//=======================================================================
#include "eMMC.h"

#if defined(CONFIG_PROGRAM_EMMC_RPMB_KEY)
#include "drvCIPHER.h"
#include "drvDSCMB.h"
#endif

#if defined(UNIFIED_eMMC_DRIVER) && UNIFIED_eMMC_DRIVER
extern U8 g_eMMCDrv_array[32];
U8	*gu8_eMMC_SectorBuf = (U8*)(CONFIG_BOOTRAM_LOADADDR+0x20200);
U8	*gu8_eMMC_MAC_CalBuf= (U8*)(CONFIG_BOOTRAM_LOADADDR+0x20600);
U8	*gu8_digest_golden=   (U8*)(CONFIG_BOOTRAM_LOADADDR+0x20A00);
U8	*gu8_digest_from_eMMC=(U8*)(CONFIG_BOOTRAM_LOADADDR+0x20A20);
U8	*gu8_auto_key=(U8*)(CONFIG_BOOTRAM_LOADADDR+0x20A40);
U8	*gu8_input_key=(U8*)(CONFIG_BOOTRAM_LOADADDR+0x20A60);
U8	*gu8_TestBuf=(U8*)(CONFIG_BOOTRAM_LOADADDR+0x20B00);

#define MIU0_PA_BASE               (0x00000000UL)
#define MIU1_PA_BASE               (0x80000000UL)

 static unsigned long long __BA2PA(unsigned long long u64BusAddr)
 {
     unsigned long long u64PhyAddr = 0x0;

 #if defined(CONFIG_MSTAR_K6) || defined(CONFIG_MSTAR_KANO) || defined(CONFIG_MSTAR_K6LITE)
     if( (u64BusAddr >= CONFIG_MIU0_BUSADDR) && (u64BusAddr < CONFIG_MIU1_BUSADDR) ) // MIU0
         u64PhyAddr = u64BusAddr - CONFIG_MIU0_BUSADDR + MIU0_PA_BASE;
     else if ( (u64BusAddr >= CONFIG_MIU1_BUSADDR) )
         u64PhyAddr = u64BusAddr - CONFIG_MIU1_BUSADDR + MIU1_PA_BASE;
 #else
     u64PhyAddr = u64BusAddr - CONFIG_MIU0_BUSADDR + MIU0_PA_BASE;
 #endif

     return u64PhyAddr;
}


// enable Reliable Write
U32  eMMC_CMD23(U16 u16_BlkCnt, U8 u8_Is_Reliable)
{
    U32 u32_err, u32_arg;
    U16 u16_ctrl, u16_reg;

    u32_arg = u16_BlkCnt&0xFFFF;// don't set BIT24
    if(u8_Is_Reliable)
        u32_arg |= BIT31;

    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;


    eMMC_FCIE_ClearEvents();
    u32_err = eMMC_FCIE_SendCmd(
        g_eMMCDrv.u16_Reg10_Mode, u16_ctrl, u32_arg, 23, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        u32_err = eMMC_ST_ERR_CMD23;
        goto LABEL_END;
    }
    else
    {   // check status
        REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

        if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
        {
            u32_err = eMMC_ST_ERR_CMD23;
            goto LABEL_END;
        }
        else
        {   // CMD13 ok, do things here
            u32_err = eMMC_CheckR1Error();
            if(eMMC_ST_SUCCESS != u32_err)
            {
                u32_err = eMMC_ST_ERR_CMD23;
                goto LABEL_END;
            }
        }
    }

    LABEL_END:
    return u32_err;
}

U32  eMMC_CMD18_RPMB_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U32 u32_dma_addr;

    u32_err = eMMC_CMD23(u16_BlkCnt, 0);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;
	// -------------------------------
	// send cmd
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_DTRX_EN;

	u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv.u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);
	u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;

	eMMC_FCIE_ClearEvents();
	REG_FCIE_W(FCIE_JOB_BL_CNT, u16_BlkCnt);
    u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE*u16_BlkCnt);

	REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0, u32_dma_addr & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, u32_dma_addr >> 16);

	REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, (eMMC_SECTOR_512BYTE*u16_BlkCnt) & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16,( eMMC_SECTOR_512BYTE*u16_BlkCnt) >> 16);


	u32_err = eMMC_FCIE_SendCmd(
		u16_mode, u16_ctrl, u32_arg, 18, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
		goto LABEL_END;
    }

	// -------------------------------
	// check FCIE

	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_DMA_END, TIME_WAIT_n_BLK_END*(1+(u16_BlkCnt>>9)));

	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(eMMC_ST_SUCCESS != u32_err || (u16_reg & BIT_SD_FCIE_ERR_FLAGS))
	{
		u32_err = eMMC_ST_ERR_CMD18;
		goto LABEL_END;
	}
	// -------------------------------
	// check device
	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		u32_err = eMMC_ST_ERR_CMD18;
	}

	LABEL_END:
	return u32_err;
}

U32  eMMC_CMD25_RPMB_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt, U8 u8_Is_Reliable)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U32 u32_dma_addr;

    u32_err = eMMC_CMD23(u16_BlkCnt, u8_Is_Reliable);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;
	// -------------------------------
	// send cmd
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;

	u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv.u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);
	u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;

	eMMC_FCIE_ClearEvents();
	REG_FCIE_W(FCIE_JOB_BL_CNT, u16_BlkCnt);
    u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE*u16_BlkCnt);

	REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0, u32_dma_addr & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, u32_dma_addr >> 16);

	REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, (eMMC_SECTOR_512BYTE*u16_BlkCnt) & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16,( eMMC_SECTOR_512BYTE*u16_BlkCnt) >> 16);

	u32_err = eMMC_FCIE_SendCmd(
		u16_mode, u16_ctrl, u32_arg, 25, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
		goto LABEL_END;
    }
    // -------------------------------
    // check device
    REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
    if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR) )
    {
        u32_err = eMMC_ST_ERR_CMD25;
        goto LABEL_END;
    }

	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		u32_err = eMMC_ST_ERR_CMD25_CHK_R1;
		goto LABEL_END;
	}
    // -------------------------------
    // send data
    u32_err = eMMC_FCIE_WaitD0High(TIME_WAIT_DAT0_HIGH);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    REG_FCIE_W(FCIE_SD_CTRL, BIT_SD_DTRX_EN|BIT_SD_DAT_DIR_W|BIT_JOB_START);

	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_DMA_END, TIME_WAIT_n_BLK_END*(1+(u16_BlkCnt>>9)));


	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(eMMC_ST_SUCCESS != u32_err || (u16_reg & (BIT_SD_W_FAIL|BIT_SD_W_CRC_ERR)))
	{
		u32_err = eMMC_ST_ERR_CMD25;
	}


	LABEL_END:
	return u32_err;
}

void  eMMC_SetError(U16 ErrCode, U16 Pos)
{
    REG_FCIE_W(NC_CIFD_ADDR(0), 0xaaaa);
    REG_FCIE_W(NC_CIFD_ADDR(Pos), ErrCode);
    REG_FCIE_W(NC_CIFD_ADDR(2), 0xcccc);
}

U32  eMMC_RPMB_CheckResultError(void)
{
	eMMC_RPMB_DATA *g_eMMCRPMB = (eMMC_RPMB_DATA*)eMMC_get_RPMBContext_address();

    if(g_eMMCRPMB->u16_result & 0x7F)
    {
        eMMC_SetError(g_eMMCRPMB->u16_result, 1);
        if(g_eMMCRPMB->u16_result != 0x0007)
            return ((U32)g_eMMCRPMB->u16_result);
    }

    return eMMC_ST_SUCCESS;
}

void  eMMC_RPMB_Copy_RESULT(U8 *u8_dest,U8 *u8_src, U32 u32_cnt)
{
	U32 u32_i;
    for(u32_i=0;u32_i< u32_cnt;u32_i++)
    {
        u8_dest[u32_i] = u8_src[u32_cnt-u32_i-1];
    }

}

void  eMMC_RPMB_SWAP_memcpy(U8 *u8_dest,U8 *u8_src, U32 u32_cnt)
{
	U32 u32_i;
    for(u32_i=0;u32_i< u32_cnt;u32_i++)
    {
        u8_dest[u32_cnt-u32_i-1] = u8_src[u32_i];
    }
}

#if 0
static eMMC_ALIGN0 eMMC_TEST_ALIGN_PACK_t g_eMMCTestAlignPack_t eMMC_ALIGN1;

U32 eMMC_CheckAlignPack(U8 u8_AlignByteCnt)
{
    eMMC_SetError(0x0, 3);

	// check alignment
	if((U32)&(g_eMMCTestAlignPack_t.u8_0) & (u8_AlignByteCnt-1))
	{
		eMMC_SetError(0xa0, 3);
		return eMMC_ST_ERR_NOT_ALIGN;
	}
	// check packed - 0
	if((U32)&(g_eMMCTestAlignPack_t.u16_0)-(U32)&(g_eMMCTestAlignPack_t.u8_0) != 1 ||
	   (U32)&(g_eMMCTestAlignPack_t.u32_0)-(U32)&(g_eMMCTestAlignPack_t.u8_0) != 3 ||
	   (U32)&(g_eMMCTestAlignPack_t.u32_1)-(U32)&(g_eMMCTestAlignPack_t.u8_0) != 7)
	{
		eMMC_SetError(0xa1, 3);
		return eMMC_ST_ERR_NOT_PACKED;
	}
	// check packed - 1
	if((U32)&(g_eMMCTestAlignPack_t.u16_0)-(U32)&(g_eMMCTestAlignPack_t.u8_0) != 1 ||
	   (U32)&(g_eMMCTestAlignPack_t.u32_0)-(U32)&(g_eMMCTestAlignPack_t.u16_0)!= 2 ||
	   (U32)&(g_eMMCTestAlignPack_t.u32_1)-(U32)&(g_eMMCTestAlignPack_t.u32_0)!= 4)
	{
		eMMC_SetError(0xa2, 3);
		return eMMC_ST_ERR_NOT_PACKED;
	}

	//eMMC_debug(eMMC_DEBUG_LEVEL_HIGH, 1, "ok\n");
	return eMMC_ST_SUCCESS;
}
#endif
U32  eMMC_RPMB_Test(void)
{
    //U16 u16_i, u16_j;

    eMMC_SetError(0, 1);
    REG_FCIE_W(FCIE_DBG1, 0xcafe);
    REG_FCIE_W(FCIE_DBG2, 0xfeed);


    #if 0 // check key
    REG_FCIE_W(FCIE_DBG1, 0xabcd);
    REG_FCIE_W(FCIE_DBG2, 0xab00);
	GetHmacKey((U32)gu8_auto_key, 32);

    for(u16_i=0; u16_i<16; u16_i++)
    {
		REG_FCIE_W(NC_CIFD_ADDR(u16_i),
		gu8_auto_key[u16_i*2]<<8|gu8_auto_key[u16_i*2+1]);
    }

    REG_FCIE_W(FCIE_DBG2, 0xab22);
    while(1);
    #endif

    #if 0 // check if need to program RPMB key into eMMC
    REG_FCIE_W(FCIE_DBG2, 0xa000|eMMC_RPMB_IfKeyWritten());
    while(1);
    #endif

    #if 0 // program key
    eMMC_RPMB_program_auth_key();
    REG_FCIE_W(FCIE_DBG2, 0x0002);
    while(1);
    #endif

    #if 0 // w/r test

    for(u16_i=0; u16_i<0x20000/0x100; u16_i++)
    {
        REG_FCIE_W(FCIE_DBG1, 0x8000+u16_i);

        for(u16_j=0; u16_j<0x100; u16_j++)
        {
            gu8_TestBuf[u16_j] = 0xaa;
            gu8_TestBuf[u16_j+0x100] = ~gu8_TestBuf[u16_j];
        }

        eMMC_RPMB_Write_data(gu8_TestBuf, 0x100, u16_i);
        eMMC_RPMB_Read_data(&gu8_TestBuf[0x100], 0x100, u16_i);
        if(eMMC_ComapreData(gu8_TestBuf, &gu8_TestBuf[0x100], 0x100))
        {
            LDR_PUTS("eMMC_ComapreData error\n");
            eMMC_SetError(0xb001, 3);
            while(1);
        }

	LDR_PUTS("eMMC_ComapreData ok\n");
    }

    eMMC_SetError(0xcccc, 3);
    eMMC_SetError(0xdddd, 4);
    eMMC_SetError(0xeeee, 5);
    while(1);

    #endif
	return 0;
}
#if defined(CONFIG_PROGRAM_EMMC_RPMB_KEY)
static U32 CIPHER(U32 AddrIn, U32 LenIn, U32 AddrOut, U32 LenOut, BOOL bDecrypt)
{
    BOOL ret = FALSE;
    U8 *pu8Buf = NULL;
    U8 *pu8DataIn = NULL;
    U8 *pu8DataOut = NULL;
    DRV_CIPHER_DMACFG stCfg;
    U32 u32Exception = 0;
    U32 u32CmdId = 0;

    memset(&stCfg, 0, sizeof(DRV_CIPHER_DMACFG));
    // Init
   // if( DRV_CIPHER_OK != MDrv_CIPHER_Init())
   // {
   //     return FALSE;
   // }
    // Algo
    stCfg.stAlgo.eMainAlgo = E_CIPHER_MAIN_AES;
    stCfg.stAlgo.eSubAlgo =  E_CIPHER_SUB_ECB;
    stCfg.stAlgo.eResAlgo = E_CIPHER_RES_CLR;
    stCfg.stAlgo.eSBAlgo = E_CIPHER_SB_CLR;

    //Key
    stCfg.stKey.eKeySrc = E_CIPHER_KSRC_KL;
    stCfg.stKey.u8KeyIdx = 0;
    stCfg.stKey.u8KeyLen = 16;
    stCfg.stKey.pu8KeyData = NULL;
    stCfg.u32CAVid = 0xF;

    //Encrypt
    stCfg.bDecrypt = bDecrypt;

    //InData
    stCfg.stInput.u32Addr = AddrIn;

    stCfg.stInput.u32Size = LenIn;

    //OutData
    stCfg.stOutput.u32Addr = AddrOut;

    stCfg.stOutput.u32Size = LenOut;
    //MsOS_FlushMemory();

     if(DRV_CIPHER_OK != MDrv_CIPHER_DMACalc(stCfg, &u32CmdId))
    {
         return FALSE;
    }

    do{
        ret = MDrv_CIPHER_IsDMADone(u32CmdId, &u32Exception);
    }while(ret == FALSE);

    if(u32Exception != 0)
    {
        LDR_PUTDW(u32Exception);
        return FALSE;
    }
    return TRUE;
}


U8 u8ACPU_RootKey[16] =
    {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

static BOOL KeyLadder(DSCMB_KLEng eKLMode, DSCMB_KL_SelEng eKLSel, U32 u32Level, U8 *pu8KeyACPU, U8 *pu8KeyKLIn, U8* pu8KeyOut, DSCMB_KLSrc eSrc, DSCMB_KLDst eDst)
{
    DSCMB_KL_Status KL_Status = 0;
    U8 ACPU_Out[16] = {0x00};
    BOOL ret = FALSE;

    DSCMB_KLCfg_All KLConfigAll =
    {
        .eAlgo = eKLMode,
        .eSrc = eSrc,
        .eDst = eDst,
        .eOutsize = E_DSCMB_KL_128_BITS,
        .eKeyType = 0,  // Don't care when dst is Crypto DMA
        .u32Level = u32Level,
        .u32EngID = 0,                  //Set to zero in STB chip, Don't care when dst is Crypto DMA
        .u32DscID = 0,// Don't care when dst is Crypto DMA
        .u8KeyACPU = pu8KeyACPU,
        .pu8KeyKLIn = pu8KeyKLIn,
        .bDecrypt = TRUE,
        .bInverse = FALSE,
        .eKLSel   = eKLSel,
        .u32CAVid = 0xF,
    };

         //Init
         //if(TRUE != MDrv_DSCMB2_Init())
         //{
         //    LDR_PUTS("Init Fail\n");
         //    return FALSE;
         // }

    //MDrv_DSCMB2_SetDBGLevel (8)
    // Key Ladder
    ret = MDrv_DSCMB2_KLadder_AtomicExec(&KLConfigAll, ACPU_Out, &KL_Status);
    if(ret == FALSE)
    {
        LDR_PUTS("KL Fail\n");
        LDR_PUTDW(KL_Status);
        ret = FALSE;
    }
    else
    {
       /* U32 i=0;
        U32* pu32ACPU_Out = (MS_U32*)ACPU_Out;
        for(i = 0; i< 4; i++)
        {
            LDR_PUTDW(*(pu32ACPU_Out + i));
        }
        LDR_PUTS("\n");*/

        memcpy(pu8KeyOut, ACPU_Out, 16);
        ret = TRUE;
    }

        return ret;
}
int GetHmacKey(U32 u32KeyBufPA ,U32 u32Key_len)
{
    U32 i;
    U8 ACPU_Out[16] = {0x00};
    for ( i = 0; i < u32Key_len; i++ ) {
        *((U8 *)(u32KeyBufPA+i)) = 0x0;
        }
    // Get eMMC CID
    eMMC_GetCID((U8 *)u32KeyBufPA);
    //Init
    if( DRV_CIPHER_OK != MDrv_CIPHER_Init())
    {
        LDR_PUTS("MDrv_CIPHER_Init fail\n");
        return false ;
    }
    //verify keyladder + AES ECB, use keyladder generate key to session key slot and run AES encrypt
    KeyLadder(E_DSCMB_KL_AES, E_DSCMB_KL_SEL_CW, 1, u8ACPU_RootKey, u32KeyBufPA, ACPU_Out, E_DSCMB_KL_SRC_ACPU, E_DSCMB_KL_DST_DMA_SK0);
    CIPHER(__BA2PA(u32KeyBufPA), u32Key_len, __BA2PA(u32KeyBufPA), u32Key_len, FALSE);
    return true;
}
#else
int GetHmacKey(U32 u32KeyBufPA ,U32 u32Key_len)
{
     return true;
}
#endif
U32  eMMC_RPMB_program_auth_key(void)
{
    U32 u32_err=0;
    eMMC_RPMB_DATA *g_eMMCRPMB = (eMMC_RPMB_DATA*)eMMC_get_RPMBContext_address();

    GetHmacKey((U32)gu8_auto_key,32);

    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    //==============write auth key========================
    memset((void*)g_eMMCRPMB,0,sizeof(eMMC_RPMB_DATA));
    memcpy(g_eMMCRPMB->u8_auth_key,gu8_auto_key,32);

    g_eMMCRPMB->u16_req_rsp = eMMC_RPMB_REQ_AUTH_KEY;
    eMMC_RPMB_SWAP_memcpy(gu8_eMMC_SectorBuf,(U8*)g_eMMCRPMB,sizeof(eMMC_RPMB_DATA));

    u32_err = eMMC_CMD25_RPMB_MIU(0,gu8_eMMC_SectorBuf, 1, 1);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    //=============read result==========================
    memset((void*)g_eMMCRPMB,0,sizeof(eMMC_RPMB_DATA));
    g_eMMCRPMB->u16_req_rsp = eMMC_RPMB_REQ_RESULT_R;

    u32_err = eMMC_RPMB_NoDataReq();

    LABEL_END:
    eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3);

    return u32_err;
}

#if defined(CONFIG_PROGRAM_EMMC_RPMB_KEY)
void MDrv_AESDMA_Hmac_sha256(const U8 *text, U32 text_len, const U8 *key, U32 key_len, void *digest)
{
    DRV_CIPHER_HASHCFG cfg;
    U32 u32CmdId = 0;
    U32 u32Exception = 0;
    U32 i=0;
    U8 i_pad = 0x36;
    U8 o_pad = 0x5c;
    U8 first_digest[32];
    U8 i_key_pad[64 + text_len];
    U8 o_key_pad[96];

    memset(i_key_pad, i_pad, sizeof(i_key_pad));
    memset(o_key_pad, o_pad, sizeof(o_key_pad));
    memcpy(i_key_pad, key, key_len);
    memcpy(o_key_pad, key, key_len);

    for(i = 0; i < key_len; i++)
    {
	*(i_key_pad + i) = *(key + i) ^ i_pad;
	*(o_key_pad + i) = *(key + i) ^ o_pad;
    }

     //==============Calculate first HASH=============
    memset(&cfg, 0, sizeof(DRV_CIPHER_HASHCFG));
    memcpy(i_key_pad + 64, text,  text_len);
    cfg.eAlgo = E_CIPHER_HASH_ALGO_SHA256;
    cfg.stInput.u32Addr = __BA2PA(i_key_pad);
    cfg.stInput.u32Size = sizeof(i_key_pad);
    cfg.pu8Digest_Buf = first_digest;
    cfg.u32Digest_Buf_Size = 32;

    if(DRV_CIPHER_OK != MDrv_CIPHER_HASH(cfg, &u32CmdId))
    {
    	LDR_PUTS("First HASH Fail1\n");
        return ;
    }

    while(FALSE == MDrv_CIPHER_IsHASHDone(u32CmdId, &u32Exception))
    {
        LDR_PUTS("HASH1 Done?\n");
    }

    if(u32Exception != 0)
    {
        LDR_PUTS("First HASH Fail2\n");
        LDR_PUTDW(u32Exception);
	LDR_PUTS("\n");
        return;
    }

//==============Calculate second HASH=============
    memset(&cfg, 0, sizeof(DRV_CIPHER_HASHCFG));
    memcpy(o_key_pad + 64, first_digest,  sizeof(first_digest));

    cfg.eAlgo = E_CIPHER_HASH_ALGO_SHA256;
    cfg.stInput.u32Addr =__BA2PA(o_key_pad);
    cfg.stInput.u32Size = sizeof(o_key_pad);
    cfg.pu8Digest_Buf = (U8 *)digest;
    cfg.u32Digest_Buf_Size = 32;

    if(DRV_CIPHER_OK != MDrv_CIPHER_HASH(cfg, &u32CmdId))
    {
    	LDR_PUTS("Second HASH Fail1\n");
        return ;
    }

    while(FALSE == MDrv_CIPHER_IsHASHDone(u32CmdId, &u32Exception))
    {
        LDR_PUTS("HASH2 Done?\n");
    }

    if(u32Exception != 0)
    {
        LDR_PUTS("Second HASH Fail2\n");
        LDR_PUTDW(u32Exception);
	LDR_PUTS("\n");
        return;
    }

}

#else
void MDrv_AESDMA_Hmac_sha256(const U8 *text, U32 text_len, const U8 *key, U32 key_len, void *digest)
{

	return;
}

#endif

U32  eMMC_RPMB_Verify_Hmac(U8* pu8_DataBuf, U16 u16_BlkCnt)
{
	U32 u32_err=0;
	U16 u16_i;

	GetHmacKey((U32)gu8_auto_key,32);
	eMMC_RPMB_SWAP_memcpy(gu8_input_key,gu8_auto_key,32);

	for(u16_i=0;u16_i<u16_BlkCnt;u16_i++)
	{
		memcpy(gu8_eMMC_MAC_CalBuf+(u16_i*284),pu8_DataBuf+(u16_i<<9)+228,284);
	}

	memcpy(gu8_digest_from_eMMC, pu8_DataBuf+((u16_BlkCnt-1)<<9)+196 ,32);
       MDrv_AESDMA_Hmac_sha256(gu8_eMMC_MAC_CalBuf,u16_BlkCnt*284,gu8_input_key,32,gu8_digest_golden);

	if(eMMC_ComapreData(gu8_digest_golden, gu8_digest_from_eMMC,32))
	{
		LDR_PUTS("eMMC_ComapreData Fail\n");
		u32_err = eMMC_ST_ERR_RPMB_HMAC_ERR;
	}

	return u32_err;
}

U32  eMMC_RPMB_Read_data(U8 *pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_BlkAddr)
{
    U16 u16_i,u16_BlkCnt;
	U32 u32_err;
    eMMC_RPMB_DATA *g_eMMCRPMB = (eMMC_RPMB_DATA*)eMMC_get_RPMBContext_address();

	u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
	if(u32_err)
        goto LABEL_END;

    while(u32_DataByteCnt)
    {
        memset((void*)g_eMMCRPMB, 0 ,512);
        //blk addr
        g_eMMCRPMB->u16_addr 	= u16_BlkAddr;
        g_eMMCRPMB->u16_req_rsp	= eMMC_RPMB_REQ_AUTH_DATA_R;

        eMMC_RPMB_SWAP_memcpy(gu8_eMMC_SectorBuf,(U8*)g_eMMCRPMB ,sizeof(eMMC_RPMB_DATA));
        u32_err = eMMC_CMD25_RPMB_MIU(0,gu8_eMMC_SectorBuf, 1, 0);
        if(u32_err)
            break;

        if(u16_BlkAddr & 1)//block address is odd
            u16_BlkCnt = 1;
        else//block address is even
        {
            if(u32_DataByteCnt >= 512)
                u16_BlkCnt = 2;
            else
                u16_BlkCnt = 1;
        }

        u32_err = eMMC_CMD18_RPMB_MIU(0,gu8_eMMC_SectorBuf, u16_BlkCnt);
        if(u32_err)
            break;

        u32_err = eMMC_RPMB_Verify_Hmac(gu8_eMMC_SectorBuf, u16_BlkCnt);
        if(u32_err)
            break;

        for(u16_i=0;u16_i<u16_BlkCnt;u16_i++)
        {
            eMMC_RPMB_SWAP_memcpy(pu8_DataBuf+(u16_i<<8),gu8_eMMC_SectorBuf+(u16_i<<9)+228,256);
            eMMC_RPMB_Copy_RESULT((U8*)g_eMMCRPMB,gu8_eMMC_SectorBuf+(u16_i<<9),sizeof(eMMC_RPMB_DATA));
            u32_err = eMMC_RPMB_CheckResultError();
            if(u32_err)
                break;
        }

        if(u16_i != u16_BlkCnt)
            break;

        u16_BlkAddr += u16_BlkCnt;
        pu8_DataBuf  += u16_BlkCnt << 8;
        u32_DataByteCnt -= u16_BlkCnt << 8;
    }
	LABEL_END:
    eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3);

    return u32_err;
}

U32  eMMC_RPMB_NoDataReq(void)
{
    U32 u32_err;
    eMMC_RPMB_DATA *g_eMMCRPMB = (eMMC_RPMB_DATA*)eMMC_get_RPMBContext_address();

    eMMC_RPMB_SWAP_memcpy(gu8_eMMC_SectorBuf,(U8*)g_eMMCRPMB,sizeof(eMMC_RPMB_DATA));

    u32_err = eMMC_CMD25_RPMB_MIU(0,gu8_eMMC_SectorBuf, 1, 0);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

    u32_err = eMMC_CMD18_RPMB_MIU(0,gu8_eMMC_SectorBuf, 1);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

    eMMC_RPMB_Copy_RESULT((U8*)g_eMMCRPMB,gu8_eMMC_SectorBuf,sizeof(eMMC_RPMB_DATA));

    u32_err = eMMC_RPMB_CheckResultError();

    return u32_err;
}


U32  eMMC_RPMB_IfKeyWritten(void)
{
	U32 u32_err;
	eMMC_RPMB_DATA *g_eMMCRPMB = (eMMC_RPMB_DATA*)eMMC_get_RPMBContext_address();

    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        goto LABEL_END;
    }
    // read data to check key is written or not
    memset((void*)g_eMMCRPMB,0,sizeof(eMMC_RPMB_DATA));
    g_eMMCRPMB->u16_addr     = 0;
    g_eMMCRPMB->u16_req_rsp  = eMMC_RPMB_REQ_AUTH_DATA_R;

    eMMC_RPMB_SWAP_memcpy(gu8_eMMC_SectorBuf,(void*)g_eMMCRPMB,sizeof(eMMC_RPMB_DATA));
    u32_err = eMMC_CMD25_RPMB_MIU(0,gu8_eMMC_SectorBuf, 1, 0);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;


    u32_err = eMMC_CMD18_RPMB_MIU(0,gu8_eMMC_SectorBuf, 1);

    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

	eMMC_RPMB_Copy_RESULT((void*)g_eMMCRPMB,gu8_eMMC_SectorBuf,sizeof(eMMC_RPMB_DATA));
	u32_err = eMMC_RPMB_CheckResultError();

    LABEL_END:
    eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3);


    if(eMMC_ST_SUCCESS != u32_err)
        return 2;

    if(g_eMMCRPMB->u16_result == 0x0007)
        return 0;
    else
        return 1;
}

U32  eMMC_RPMB_Read_Result(U16 *pu16_Result)
{
	U32 u32_err;
	eMMC_RPMB_DATA *g_eMMCRPMB = (eMMC_RPMB_DATA*)eMMC_get_RPMBContext_address();

    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

    memset((void*)g_eMMCRPMB,0,sizeof(eMMC_RPMB_DATA));
    g_eMMCRPMB->u16_req_rsp = eMMC_RPMB_REQ_RESULT_R;

    u32_err = eMMC_RPMB_NoDataReq();
    if(eMMC_ST_SUCCESS == u32_err)
        *pu16_Result = g_eMMCRPMB->u16_result;

    return eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3);
}


U32  eMMC_RPMB_Get_Counter(volatile U32 *pu32_WCnt)
{
	U32 u32_err;
	eMMC_RPMB_DATA *g_eMMCRPMB = (eMMC_RPMB_DATA*)eMMC_get_RPMBContext_address();

    memset((void*)g_eMMCRPMB,0,sizeof(eMMC_RPMB_DATA));
    g_eMMCRPMB->u16_req_rsp = eMMC_RPMB_REQ_WRITE_CNT_VAL;

    u32_err = eMMC_RPMB_NoDataReq();
    if(eMMC_ST_SUCCESS == u32_err)
        *pu32_WCnt = g_eMMCRPMB->u32_writecnt;

    return u32_err;
}

U32  eMMC_RPMB_Write_data(U8* pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_blk_addr)
{
    U32 u32_err=0;
    volatile U32 u32_wcnt;
    U16 u16_i,u16_BlkCnt;
    eMMC_RPMB_DATA *g_eMMCRPMB = (eMMC_RPMB_DATA*)eMMC_get_RPMBContext_address();

    GetHmacKey((U32)gu8_auto_key, 32);

    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    while(u32_DataByteCnt)
    {
        u32_wcnt = 0;
        eMMC_RPMB_Get_Counter(&u32_wcnt);
        if(g_eMMCRPMB->u16_result & 0x80)//rpmb write limit times
        {
            u32_err = eMMC_ST_ERR_RPMB_WCNT_MAX;
            break;
        }
        if(u16_blk_addr & 1)//block address is odd
            u16_BlkCnt = 1;
        else//block address is even
        {
            if(u32_DataByteCnt >= 512)
                u16_BlkCnt = 2;
            else
                u16_BlkCnt = 1;
        }

        memset(gu8_eMMC_MAC_CalBuf,0,2*284);
        memset(gu8_eMMC_SectorBuf,0,2<<9);

        for(u16_i=0;u16_i< u16_BlkCnt;u16_i++)
        {
            memset((void*)g_eMMCRPMB,0,sizeof(eMMC_RPMB_DATA));
            g_eMMCRPMB->u32_writecnt = u32_wcnt;
            g_eMMCRPMB->u16_addr     = u16_blk_addr;
            g_eMMCRPMB->u16_blk_cnt  = u16_BlkCnt;
            g_eMMCRPMB->u16_req_rsp  = eMMC_RPMB_REQ_AUTH_DATA_W;
            memcpy(g_eMMCRPMB->u8_data, pu8_DataBuf+(u16_i<<8), 256);
            eMMC_RPMB_SWAP_memcpy(gu8_eMMC_MAC_CalBuf+u16_i*284, (U8*)g_eMMCRPMB, 284);
            eMMC_RPMB_SWAP_memcpy(gu8_eMMC_SectorBuf+(u16_i<<9), (U8*)g_eMMCRPMB, sizeof(eMMC_RPMB_DATA));
        }

        eMMC_RPMB_SWAP_memcpy(gu8_input_key, gu8_auto_key, 32);
        MDrv_AESDMA_Hmac_sha256(gu8_eMMC_MAC_CalBuf, u16_BlkCnt*284, gu8_input_key, 32, gu8_digest_golden);

        memcpy(gu8_eMMC_SectorBuf+(((u16_BlkCnt-1)<<9)+196),gu8_digest_golden,32);

        u32_err = eMMC_CMD25_RPMB_MIU(0,gu8_eMMC_SectorBuf, u16_BlkCnt, 1);
        if(eMMC_ST_SUCCESS != u32_err)
            break;

        //=============read result==========================
        memset((void*)g_eMMCRPMB,0,sizeof(eMMC_RPMB_DATA));
        g_eMMCRPMB->u16_req_rsp = eMMC_RPMB_REQ_RESULT_R;

        u32_err = eMMC_RPMB_NoDataReq();

        if(eMMC_ST_SUCCESS != u32_err)
            break;

        u16_blk_addr += u16_BlkCnt;
        pu8_DataBuf  += u16_BlkCnt<<8;
        u32_DataByteCnt -= u16_BlkCnt<<8;
    }



    LABEL_END:
    eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3);

    return u32_err;
}


U32 eMMC_RPMB_Check_Program_Key(void)
{
     U32 u32_err=0;

     u32_err = eMMC_Init();
     if(u32_err != eMMC_ST_SUCCESS)
         return u32_err;

     if(1 == eMMC_RPMB_IfKeyWritten())
     {
         PUTC('*');
         //eMMC_RPMB_program_auth_key();
         //while(1);
     }
     else
     {
         PUTC('#');
         u32_err = eMMC_RPMB_program_auth_key();
         PUTC('!');
    }
    return u32_err;
}
#endif
