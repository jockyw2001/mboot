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

#ifndef NAND_DRV_V3
#define NAND_DRV_V3

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

#ifndef U64
#define U64  unsigned long long
#endif
#ifndef U32
#define U32  unsigned int
#endif
#ifndef U16
#define U16  unsigned short
#endif
#ifndef U8
#define U8   unsigned char
#endif
#ifndef S64
#define S64  signed long long
#endif
#ifndef S32
#define S32  signed long
#endif
#ifndef S16
#define S16  signed short
#endif
#ifndef S8
#define S8   signed char
#endif
#ifndef NULL
#define NULL (void*)0
#endif

#ifndef UINT16
#define UINT16 U16
#endif
#ifndef UINT32
#define UINT32 U32
#endif
#ifndef UINT64
#define UINT64 U64
#endif

//=====================================================================================
#include "drvNAND_config.h" // [CAUTION]: edit drvNAND_config.h for your platform
//=====================================================================================
#include "drvNAND_err_codes.h"

#define NAND_BBT_PAGE_COUNT 8

//===========================================================
// Partition Info parameters
//===========================================================
#define UNFD_LOGI_PART    0x8000 // bit-or if the partition needs Wear-Leveling
#define UNFD_HIDDEN_PART  0x4000 // bit-or if this partition is hidden, normally it is set for the LOGI PARTs.

#define UNFD_PART_HWCONFIG		1
#define UNFD_PART_BOOTLOGO      2
#define UNFD_PART_BL            3
#define UNFD_PART_OS            4
#define UNFD_PART_CUS           5
#define UNFD_PART_UBOOT         6
#define UNFD_PART_SECINFO	    7
#define UNFD_PART_OTP           8
#define UNFD_PART_RECOVERY      9
#define UNFD_PART_E2PBAK        10
#define UNFD_PART_NVRAMBAK      11
#define UNFD_PART_APANIC        12
#define UNFD_PART_ENV           13
#define UNFD_PART_MISC          14
#define UNFD_PART_TBL           15
#define UNFD_PART_CTRL          16
#define UNFD_PART_FDD           17
#define UNFD_PART_TDD           18
#define UNFD_PART_UBIRO         19
#define UNFD_PART_OPTEE         20
#define UNFD_PART_ARMFW         21

typedef UNFD_PACK0 struct _PARTITION_RECORD {

	U16 u16_StartBlk;     // the start block index, reserved for UNFD internal use.
	U16 u16_BlkCnt;       // project team defined
	U16 u16_PartType;     // project team defined, e.g. UNFD_PART_XXX_0
	U16 u16_BackupBlkCnt; // reserved good blocks count for backup, UNFD internal use.
                          // e.g. u16BackupBlkCnt  = u16BlkCnt * 0.03 + 2
} UNFD_PACK1 PARTITION_RECORD_t, *P_PARTITION_RECORD_t;

typedef UNFD_PACK0 struct _PARTITION_INFO {

	U32 u32_ChkSum;
	U16	u16_SpareByteCnt;
	U16	u16_PageByteCnt;
	U16	u16_BlkPageCnt;
	U16	u16_BlkCnt;
	U16 u16_PartCnt;
	U16 u16_UnitByteCnt;
	PARTITION_RECORD_t records[];

} UNFD_PACK1 PARTITION_INFO_t;

typedef UNFD_PACK0 struct _DDR_TIMING_GROUP
{
	U8	u8_ClkIdx;
	U8	u8_DqsMode;
	U8	u8_DelayCell;
	U8	u8_DdrTiming;
} UNFD_PACK1 DDR_TIMING_GROUP_t;

typedef UNFD_PACK0 struct _NAND_FLASH_INFO
{
	U8  au8_Tag[16];
	U8  u8_IDByteCnt;
	U8  au8_ID[15];
	U32 u32_ChkSum;
	U16 u16_SpareByteCnt;
	U16 u16_PageByteCnt;
	U16 u16_BlkPageCnt;
	U16 u16_BlkCnt;
	U32 u32_Config;
	U16 u16_ECCType;
	U16	u16_tRC;
	U8  u8_tRP;
	U8  u8_tREH;
	U8  u8_tREA;
	U8  u8_tRR;
	U16 u16_tADL;
	U16 u16_tRHW;
	U16 u16_tWHR;
	U16 u16_InitBadBlkCnt;
	U8  u8_Vendor[16];
	U8  u8_PartNumber[16];
	U8  u8_PairPageMapLoc;
	U8  u8_PairPageMapType;
	U16 u16_tCCS;
	U8  u8_tCS;
	//for TV ROM Code
	U8  u8_BL0PBA;
	U8  u8_BL1PBA;
	U8  u8_UBOOTPBA;
	U16	u16_tWC;
	U8  u8_tWP;
	U8  u8_tWH;
	U16 u16_tCWAW;
	U8 u8_tCLHZ;
	U8 u8_AddrCycleIdx;
	DDR_TIMING_GROUP_t tDefaultDDR;
	DDR_TIMING_GROUP_t tMaxDDR;
	DDR_TIMING_GROUP_t tMinDDR;
	U8  u8_HashPBA[3][2];
	U16  u16_tWW;
	U8	u8_ReadRetryType;
	U8  u8_BitflipThreshold;
	U8	u8_Hash0PageIdx;
	U8	u8_Hash1PageIdx;
	U32	 u32_BootSize;
	U16	u16_BBtPageCheckCount;
	U16	u16_BBtPageIdx[NAND_BBT_PAGE_COUNT];
	U16	u16_BBtMarker[NAND_BBT_PAGE_COUNT];
} UNFD_PACK1 NAND_FLASH_INFO_t;

typedef struct _PAIRED_PAGE_MAP {
    U16 u16_LSB;
} PAIRED_PAGE_MAP_t;

//===========================================================
// driver parameters
//===========================================================
#define NAND_ID_BYTE_CNT     15

#define R_SEQUENCE_003A      1
#define R_SEQUENCE_004A      2
#define R_SEQUENCE_004A30    3
#define R_SEQUENCE_005A30    4
#define R_SEQUENCE_006A30    5

typedef struct _NAND_DRIVER
{
	U8  au8_ID[NAND_ID_BYTE_CNT];
	U8  u8_IDByteCnt;
	//-----------------------------
	// HAL parameters
	//-----------------------------
	#if (defined(NC_SEL_FCIE3) && NC_SEL_FCIE3) || (defined(NC_SEL_FCIE5) && NC_SEL_FCIE5)
	volatile U16 u16_Reg1B_SdioCtrl;
	volatile U16 u16_Reg40_Signal;
	volatile U16 u16_Reg48_Spare;
	volatile U16 u16_Reg49_SpareSize;
	volatile U16 u16_Reg50_EccCtrl;
	volatile U16 u16_Reg57_RELatch;

	volatile U16 u16_Reg2C_SMStatus;
	volatile U16 u16_Reg58_DDRCtrl;
	volatile U16 u16_Reg5A_tWHR_tCLHZ;
	volatile U16 u16_Reg5D_tCWAW_tADL;	
	#endif
    #if defined(NC_HWCMD_DELAY) && NC_HWCMD_DELAY
	volatile U16 u16_Reg56_Rand_W_Cmd;
    #endif

    #if defined(FCIE_LFSR) && FCIE_LFSR|| (defined(NC_TRR_TCS) && NC_TRR_TCS)
	volatile U16 u16_Reg59_LFSRCtrl;
    #endif
	U32 u32_Clk;

	//-----------------------------
	// NAND Info (listed by their init_sequence)
	//-----------------------------
	U8  u8_OpCode_RW_AdrCycle;
	U8  u8_OpCode_Erase_AdrCycle;
	U16 u16_One_Col_Addr;

	U16 u16_BlkCnt;
	U16 u16_BlkPageCnt;
	U16 u16_PageByteCnt;
	U16 u16_SpareByteCnt;
	U16 u16_ECCCorretableBit;
	U16 u16_ECCType;
	U32 u32_Config;
	U16	u16_tRC;
	U8  u8_tRP;
	U8  u8_tREH;
	U8  u8_tREA;
	U8  u8_tRR;
	U16 u16_tADL;
	U16 u16_tRHW;
	U16 u16_tWHR;
	U16 u16_tCCS;
	U8  u8_tCS;
	U16	u16_tWC;
	U8  u8_tWP;
	U8  u8_tWH;
	U16 u16_tCWAW;
	U8	u8_tCLHZ;
	U8 	u8_AddrCycleIdx;
	U16  u16_tWW;
    U8	u8_CellType;
	U8  u8_RequireRandomizer;
    U8	u8_PairPageMapLoc;

	U16 u16_BlkSectorCnt;
	U16 u16_PageSectorCnt;
	U16 u16_SectorByteCnt;
	U16 u16_SectorSpareByteCnt;
	U16 u16_ECCCodeByteCnt;

	U8  u8_BlkPageCntBits;
	U8  u8_BlkSectorCntBits;
	U8  u8_PageByteCntBits;
	//U8  u8_SpareByteCntBits;
	U8  u8_PageSectorCntBits;
	U8  u8_SectorByteCntBits;
	//U8  u8_SectorSpareByteCntBits;

	U16 u16_BlkPageCntMask;
	U16 u16_BlkSectorCntMask;
	U16 u16_PageByteCntMask;
	  //U16 u16_SpareByteCntMask;
	U16 u16_PageSectorCntMask;
	U16 u16_SectorByteCntMask;

#if defined(CONFIG_MSTAR_NS_UBOOT)
	U8 *pu8_PageSpareBuf;
	U8 *pu8_PageDataBuf;

	U8 au8_PartInfo[NAND_PARTITAION_BYTE_CNT];
#endif


} NAND_DRIVER, *P_NAND_DRIVER;

//===========================================================
// internal used functions
//===========================================================
#include "drvNAND_utl.h"

//===========================================================
// public used functions
//===========================================================
extern U32 nand_unfd_init(NAND_DRIVER *pNandDrv);
extern U32 nand_set_param (NAND_DRIVER *pNandDrv, U8 *pu8_DataBuf);
extern U32 nand_pads_switch(NAND_DRIVER *pNandDrv, U32 u32EnableFCIE);
extern U32 nand_pads_init(NAND_DRIVER *pNandDrv, U16 u16_NandMode);
extern U32 nand_clock_setting(U32 u32ClkParam);
extern U32 nand_set_partinfo (NAND_DRIVER *pNandDrv, U8 *pu8_PartInfo);
extern U32 drvNAND_ReadPartition(NAND_DRIVER *pNandDrv, U16 u16_PartType, U32 u32_SrcPhyAddr, U32 u32_SectorCnt );
//---------------------------------------
// HAL functions
//---------------------------------------
extern U32  nand_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt);
extern U32  NC_Init(NAND_DRIVER *pNandDrv);
extern U32  NC_RegInit(NAND_DRIVER *pNandDrv);
extern U32  NC_PlatformResetPre(NAND_DRIVER *pNandDrv);
extern U32  NC_PlatformResetPost(NAND_DRIVER *pNandDrv);
extern U32  NC_wait_MIULastDone(NAND_DRIVER *pNandDrv);
extern U32  NC_PlatformInit(NAND_DRIVER *pNandDrv);
extern U32  NC_ConfigNandFlashContext(NAND_DRIVER *pNandDrv);
extern U32  NC_ResetFCIE(NAND_DRIVER *pNandDrv);
extern U32  NC_ProbeReadSeq(NAND_DRIVER *pNandDrv);
extern U32  NC_ResetNandFlash(NAND_DRIVER *pNandDrv);
extern void NC_Config(NAND_DRIVER *pNandDrv);
extern void NC_GetCIFD(U8 *pu8_Buf, U32 u32_CIFDPos, U32 u32_ByteCnt);
extern U32  NC_ReadSector_RIUMode(NAND_DRIVER *pNandDrv, U32 u32_PhyRowIdx, U8 u8_SectorInPage, U16 *pu16_DataInBuf, U8 *pu8_SpareBuf, U8 u8_RandCmdDis);
extern U32  NC_ReadSector_DMAMode(NAND_DRIVER *pNandDrv, U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U8 u8_RandCmdDis);
extern void NC_SetCIFD(U8 *pu8_Buf, U32 u32_CIFDPos, U32 u32_ByteCnt);
extern U32 NC_ReadID(NAND_DRIVER *pNandDrv);
extern U32 NC_ReadStatus(NAND_DRIVER *pNandDrv);


#if defined(ENABLE_NAND_RIU_MODE) && ENABLE_NAND_RIU_MODE
    #define NC_ReadSector NC_ReadSector_RIUMode
#else
    #define NC_ReadSector NC_ReadSector_DMAMode
#endif
#endif // NAND_DRV_V3

