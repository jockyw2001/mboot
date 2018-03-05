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

U32 nand_unfd_init(NAND_DRIVER *pNandDrv)
{
	U32 u32_Err = 0;

	NC_PlatformInit(pNandDrv);

	u32_Err = drvNAND_ProbeReadSeq(pNandDrv);

	return u32_Err;
}

U32  nand_set_param (NAND_DRIVER *pNandDrv, U8 *pu8_DataBuf)
{
	U32 u32_Err = 0;

	//PACK FAILURE
	pNandDrv->u16_SpareByteCnt = *((U16*)(pu8_DataBuf+0x24));
	pNandDrv->u16_PageByteCnt  = *((U16*)(pu8_DataBuf+0x26));
	pNandDrv->u16_BlkPageCnt   = *((U16*)(pu8_DataBuf+0x28));
	pNandDrv->u16_BlkCnt	   = *((U16*)(pu8_DataBuf+0x2A));
	pNandDrv->u32_Config	   = *((U32*)(pu8_DataBuf+0x2C));
	pNandDrv->u16_ECCType      = *((U16*)(pu8_DataBuf+0x30));
	pNandDrv->u16_tRC 	       = *((U16*)(pu8_DataBuf+0x32));
	pNandDrv->u8_tRP		   = pu8_DataBuf[0x34];
	pNandDrv->u8_tREH 	       = pu8_DataBuf[0x35];
	pNandDrv->u8_tREA 	       = pu8_DataBuf[0x36];
	pNandDrv->u8_tRR		   = pu8_DataBuf[0x37];
	pNandDrv->u16_tADL		   = *((U16*)(pu8_DataBuf+0x38));
	pNandDrv->u16_tRHW		   = *((U16*)(pu8_DataBuf+0x3A));
	pNandDrv->u16_tWHR		   = *((U16*)(pu8_DataBuf+0x3C));
	pNandDrv->u8_PairPageMapLoc  = pu8_DataBuf[0x60];
	pNandDrv->u16_tCCS		   = *((U16*)(pu8_DataBuf+0x62));
	pNandDrv->u8_tCS		   = pu8_DataBuf[0x64];
	pNandDrv->u16_tWC 	       = *((U16*)(pu8_DataBuf+0x68));
	pNandDrv->u8_tWP		   = pu8_DataBuf[0x6A];
	pNandDrv->u8_tWH		   = pu8_DataBuf[0x6B];
	pNandDrv->u16_tCWAW	       = *((U16*)(pu8_DataBuf+0x6C));
	pNandDrv->u8_tCLHZ		   = pu8_DataBuf[0x6E];
	pNandDrv->u8_AddrCycleIdx  = pu8_DataBuf[0x6F];
	pNandDrv->u16_tWW 		   = *((U16*)(pu8_DataBuf+0x82));
	pNandDrv->u8_CellType      = pNandDrv->u32_Config&0x1;
	pNandDrv->u8_RequireRandomizer = (pNandDrv->u32_Config>>8) & 0x1;


	NC_ConfigNandFlashContext(pNandDrv);
	u32_Err = NC_RegInit(pNandDrv);
	if(u32_Err != UNFD_ST_SUCCESS)
		return u32_Err;

	pNandDrv->u16_Reg48_Spare &= (~BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE);	
	NC_Config(pNandDrv);


	nand_config_clock(pNandDrv);


	return UNFD_ST_SUCCESS;
}

#if defined(CONFIG_MSTAR_NS_UBOOT)
U32  nand_set_partinfo (NAND_DRIVER *pNandDrv, U8 *pu8_PartInfo)
{
    memcpy(pNandDrv->au8_PartInfo, pu8_PartInfo, NAND_PARTITAION_BYTE_CNT);

    return UNFD_ST_SUCCESS;
}

U32 drvNAND_ReadPartition(NAND_DRIVER *pNandDrv, U16 u16_PartType, U32 u32_SrcPhyAddr, U32 u32_SectorCnt)
{
	PARTITION_INFO_t *pPartInfo = (PARTITION_INFO_t *)pNandDrv->au8_PartInfo;
	PARTITION_RECORD_t *pRecord = pPartInfo->records;

	U32 u32_Err = UNFD_ST_SUCCESS;
	U16 u16_BlkIdx;
	U16 u16_LBA;
	U32 u32_SectorCntRead;
	U16 u16_PageSectorCnt;
	U8 u8_PageSectorCntBits;
	U8 u8_BlkSectorCntBits;
	U16 u16_BlkSectorCnt;

	pRecord = drvNAND_SearchPartition(pNandDrv, pRecord, u16_PartType);
	if (!pRecord)
	{
		u32_Err = UNFD_ST_ERR_INVALID_PART;
		goto READPARTITION_END;
	}

	if(u32_SectorCnt > (((((U32)pRecord->u16_BlkCnt+(U32)pRecord->u16_BackupBlkCnt)<<(U32)pNandDrv->u8_BlkPageCntBits)<<(U32)pNandDrv->u8_PageByteCntBits)>>9))
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "out of range\n");
        u32_Err = UNFD_ST_ERR_INVALID_PARAM;
		goto READPARTITION_END;
	}

	/* sector size from AP is 512 Bytes, not NAND sector size */
	u16_PageSectorCnt = pNandDrv->u16_PageByteCnt >> 9;
	u8_PageSectorCntBits = drvNAND_CountBits(u16_PageSectorCnt);
	//u16_PageSectorCntMask = (1<<u8_PageSectorCntBits)-1;
	u8_BlkSectorCntBits = pNandDrv->u8_BlkPageCntBits + u8_PageSectorCntBits;
	u16_BlkSectorCnt = 1<<u8_BlkSectorCntBits;

	u16_LBA = 0;
	u16_BlkIdx = pRecord->u16_StartBlk;

	while(u32_SectorCnt>0)
	{
		while (!drvNAND_IsGoodBlk(pNandDrv, u16_BlkIdx) )
		{
			nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "Skip bad blk: 0x%04x\n", u16_BlkIdx);
			if((++u16_BlkIdx)>=(pRecord->u16_StartBlk + pRecord->u16_BlkCnt + pRecord->u16_BackupBlkCnt))
			{
				nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "too many bad block\n");
				u32_Err = UNFD_ST_ERR_LACK_BLK;
				goto READPARTITION_END;
			}
		}

		u32_SectorCntRead = u32_SectorCnt > u16_BlkSectorCnt ? u16_BlkSectorCnt : u32_SectorCnt;

		u32_Err = drvNAND_ReadBlk(pNandDrv, (U8*)u32_SrcPhyAddr, pRecord->u16_PartType,
								   u16_BlkIdx, u16_LBA, u32_SectorCntRead);


		if (u32_Err != UNFD_ST_SUCCESS)
		{
			goto READPARTITION_END;
		}

		u32_SectorCnt -= u32_SectorCntRead;
		u32_SrcPhyAddr += (u32_SectorCntRead<<9);
		u16_BlkIdx++;
		u16_LBA++;
	}

READPARTITION_END:
	return u32_Err;
}
#endif

#endif
