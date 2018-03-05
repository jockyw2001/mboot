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

typedef struct
{
	const char *msg;
	U8  u8_OpCode_RW_AdrCycle;
	U8  u8_OpCode_Erase_AdrCycle;
	U16 u16_Reg48_Spare;
} drvNAND_ReadSeq;

static const drvNAND_ReadSeq seq[] = {
	{"R_SEQUENCE_003A", ADR_C3TFS0, ADR_C2TRS0, BIT_NC_ONE_COL_ADDR},
	{"R_SEQUENCE_004A", ADR_C4TFS0,	ADR_C3TRS0,	BIT_NC_ONE_COL_ADDR},
	{"R_SEQUENCE_004A30", ADR_C4TFS0, ADR_C2TRS0, 0},
	{"R_SEQUENCE_005A30", ADR_C5TFS0, ADR_C3TRS0, 0},
	{"R_SEQUENCE_006A30", ADR_C6TFS0, ADR_C3TRS0, 0}, // FIXME
	{NULL, 0, 0, 0},
};

void  nand_memset(void * s,char c,int count)
{
    char *xs = (char *) s;

    while (count--)
        *xs++ = c;
}

U32 drvNAND_ProbeReadSeq(NAND_DRIVER *pNandDrv)
{
	U8 u8_seq_idx = 0;
	U32 u32_Err;
	drvNAND_ReadSeq const *pSeq = seq;

	pNandDrv->u16_BlkCnt = 0x400;
	pNandDrv->u16_BlkPageCnt = 0x20;
	pNandDrv->u16_PageByteCnt = 0x800;
	pNandDrv->u16_SpareByteCnt = 0x100;
	pNandDrv->u16_ECCType = NANDINFO_ECC_TYPE;

	NC_ConfigNandFlashContext(pNandDrv);
	u32_Err = NC_Init(pNandDrv);
	if (u32_Err != UNFD_ST_SUCCESS)
		return u32_Err;

	if(pNandDrv->u8_AddrCycleIdx)
	{
		pSeq = &seq[pNandDrv->u8_AddrCycleIdx-1];
		pNandDrv->u8_OpCode_RW_AdrCycle = pSeq->u8_OpCode_RW_AdrCycle;
		pNandDrv->u8_OpCode_Erase_AdrCycle = pSeq->u8_OpCode_Erase_AdrCycle;
		pNandDrv->u16_Reg48_Spare &= ~BIT_NC_ONE_COL_ADDR;
		pNandDrv->u16_Reg48_Spare |= pSeq->u16_Reg48_Spare;

		NC_Config(pNandDrv);
		return UNFD_ST_SUCCESS;
	}
	else
	{
		/* Probe read sequence */
		while (pSeq->msg) 
		{
			pNandDrv->u8_OpCode_RW_AdrCycle = pSeq->u8_OpCode_RW_AdrCycle;
			pNandDrv->u8_OpCode_Erase_AdrCycle = pSeq->u8_OpCode_Erase_AdrCycle;
			pNandDrv->u16_Reg48_Spare &= ~BIT_NC_ONE_COL_ADDR;
			pNandDrv->u16_Reg48_Spare |= pSeq->u16_Reg48_Spare;

			NC_Config(pNandDrv);

			u32_Err = NC_ProbeReadSeq(pNandDrv);
			if (u32_Err == UNFD_ST_SUCCESS)
			{
				pNandDrv->u8_AddrCycleIdx = u8_seq_idx;
				return UNFD_ST_SUCCESS;
			}
			NC_ResetFCIE(pNandDrv);
			pSeq++;
			u8_seq_idx ++;
		}
	}

	return UNFD_ST_ERR_UNKNOWN_RSEQ;
}

U8 drvNAND_CountBits(U32 u32_x)
{
	U8 u8_i = 0;

    if(u32_x==0) return u8_i;
    
	while (u32_x) {
		u8_i++;
		u32_x >>= 1;
	}

	return u8_i-1;
}

#if defined(CONFIG_MSTAR_NS_UBOOT)

static UNFD_ALIGN0 TEST_ALIGN_PACK_t g_TestAlignPack_t UNFD_ALIGN1;

U32 drvNAND_CheckAlignPack(U8 u8_AlignByteCnt)
{
	// check alignment
	if((U32)&(g_TestAlignPack_t.u8_0) & (u8_AlignByteCnt-1))
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, not aliged. expect %X but %lX \n",
			u8_AlignByteCnt, (U32)&(g_TestAlignPack_t.u8_0));
		return UNFD_ST_ERR_NOT_ALIGN;
	}
	// check packed - 0
	if((U32)&(g_TestAlignPack_t.u16_0)-(U32)&(g_TestAlignPack_t.u8_0) != 1 ||
	   (U32)&(g_TestAlignPack_t.u32_0)-(U32)&(g_TestAlignPack_t.u8_0) != 3 ||
	   (U32)&(g_TestAlignPack_t.u32_1)-(U32)&(g_TestAlignPack_t.u8_0) != 7)
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, not packed. check err.0 \n");
		return UNFD_ST_ERR_NOT_PACKED;
	}
	// check packed - 1
	if((U32)&(g_TestAlignPack_t.u16_0)-(U32)&(g_TestAlignPack_t.u8_0) != 1 ||
	   (U32)&(g_TestAlignPack_t.u32_0)-(U32)&(g_TestAlignPack_t.u16_0)!= 2 ||
	   (U32)&(g_TestAlignPack_t.u32_1)-(U32)&(g_TestAlignPack_t.u32_0)!= 4)
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, not packed. check err.1 \n");
		return UNFD_ST_ERR_NOT_PACKED;
	}

	nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "ok\n");
	return UNFD_ST_SUCCESS;
}


/* Search for partition of type @u16_PartType, begin from @pRecord */
PARTITION_RECORD_t *drvNAND_SearchPartition(NAND_DRIVER *pNandDrv, PARTITION_RECORD_t *pRecord,
											U16 u16_PartType)
{
	PARTITION_INFO_t *pPartInfo = (PARTITION_INFO_t *)pNandDrv->au8_PartInfo;;

	while (pRecord - pPartInfo->records < pPartInfo->u16_PartCnt) {

		if (pRecord->u16_PartType == u16_PartType)
			return pRecord;

		pRecord++;
	}

	return (void*)0;
}

/* return 1: Good block, 0: Bad block */
U32 drvNAND_IsGoodBlk(NAND_DRIVER *pNandDrv, U16 u16_PBA)
{
	U8 *au8_DataBuf = (U8*)pNandDrv->pu8_PageDataBuf;
	U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;
	U32 u32_PageIdx;
	U32 u32_Err;
	U32 i;

	// old way checking
	for(i=0; i<1; i++)
	{
		u32_PageIdx = u16_PBA << pNandDrv->u8_BlkPageCntBits;
		
	    switch(i)
	    {
			case 0:
			    break;
			case 1: u32_PageIdx++;
			    break;
			case 2: u32_PageIdx += pNandDrv->u16_BlkPageCnt-1;
			    break;
			case 3: u32_PageIdx += pNandDrv->u16_BlkPageCnt-2;
			    break;
	    }
	    u32_Err = NC_ReadSector(pNandDrv, u32_PageIdx, 0, au8_DataBuf, au8_SpareBuf, 0);
        
	    if (u32_Err != UNFD_ST_SUCCESS)
            nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "NC_ReadSectors(0x%X)=0x%X\n", (unsigned int)u32_PageIdx, (unsigned int)u32_Err);
	
	    if (au8_SpareBuf[0] != 0xFF)
		    return 0; // bad blk
	}

	return 1; // good blk
}

U32 drvNAND_ReadBlk(NAND_DRIVER *pNandDrv, U8 *pu8_DestAddr, U8 u8_PartType, U16 u16_PBA,
							U16 u16_LBA, U32 u32_SectorCnt)
{
	U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
	U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;

	U32 u32_Err;
	//BLK_INFO_t *pBlkInfo = (BLK_INFO_t*)au8_SpareBuf;
	U16 u16_PageSectorCnt;
	U8 u8_PageSectorCntBits;
	U32 u32_PageIdx;
	U16 u16_PageCnt;
	U16 u16_SectorCntRead;
	U8 u8_SectorIdxInPage;
    
	nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "BlkIdx:%d, BlkNo:%d, SectorCnt:%d\n", (unsigned int)u16_PBA, (unsigned int)u16_LBA, (unsigned int)u32_SectorCnt);

	/* sector size from AP is 512 Bytes, not NAND sector size */
	u16_PageSectorCnt = pNandDrv->u16_PageByteCnt >> 9;
	u8_PageSectorCntBits = drvNAND_CountBits(u16_PageSectorCnt);

	/* Read aligned sectors then */
	u32_PageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits);
	u16_PageCnt = (u32_SectorCnt>>u8_PageSectorCntBits);

	if(u16_PageCnt>0)
	{
		u32_Err = NC_ReadPages(pNandDrv, u32_PageIdx, pu8_DestAddr, (U8*)au8_SpareBuf, u16_PageCnt);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04x failed with EC: 0x%04x\n",
				(unsigned int)u32_PageIdx, (unsigned int)u32_Err);

			return u32_Err;
		}

		pu8_DestAddr += (pNandDrv->u16_PageByteCnt*u16_PageCnt);
		u32_PageIdx += u16_PageCnt;
		u32_SectorCnt -= (u16_PageSectorCnt*u16_PageCnt);
	}

	/* Read remaining unaligned sectors finally */
	u8_SectorIdxInPage = 0;
	while( u32_SectorCnt > 0 )
	{
		u32_Err = NC_ReadSector(pNandDrv, u32_PageIdx, u8_SectorIdxInPage, au8_PageBuf, au8_SpareBuf, 0);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04x failed with EC: 0x%04x\n",
					   (unsigned int)u32_PageIdx, (unsigned int)u32_Err);

			return u32_Err;
		}

        	u16_SectorCntRead = u32_SectorCnt > (pNandDrv->u16_SectorByteCnt>>9) ? (pNandDrv->u16_SectorByteCnt>>9) : u32_SectorCnt;
		memcpy(pu8_DestAddr, au8_PageBuf, (u16_SectorCntRead<<9));
        	pu8_DestAddr += (u16_SectorCntRead<<9);
        	u32_SectorCnt -= u16_SectorCntRead;
        	u8_SectorIdxInPage++;
	}

	return UNFD_ST_SUCCESS;
}
#endif

#endif //(defined(CONFIG_MSTAR_KANO)||defined(CONFIG_MSTAR_K6)||defined(CONFIG_MSTAR_K6LITE)||defined(CONFIG_MSTAR_CURRY))&&(!defined(CONFIG_MSTAR_BFN_EN))

