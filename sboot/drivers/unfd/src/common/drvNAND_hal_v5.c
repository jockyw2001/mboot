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

//#include <linux/string.h>
#include "drvNAND.h"
#include "drvNAND_utl.h"

#if (defined(NC_SEL_FCIE5) && (NC_SEL_FCIE5))

#define CHECK_HAL_PARAM      1 // normal can be 0

//========================================================
// HAL pre-processors
//========================================================
#if IF_FCIE_SHARE_PINS
  #define NC_PAD_SWITCH(enable)    nand_pads_switch(enable);
#else
    // NULL to save CPU a JMP/RET time
  #define NC_PAD_SWITCH(enable)
#endif

#if IF_FCIE_SHARE_IP
  // re-config FCIE3 for NFIE mode
  #define NC_RECONFIG()   {NC_ReConfig(); nand_clock_setting(pNandDrv->u32_Clk);}
#else
    // NULL to save CPU a JMP/RET time
  #define NC_RECONFIG()
#endif

#define NAND_TIMEOUT_RETRY_CNT     5

//========================================================
// HAL misc. function definitions
//========================================================

//void NC_DumpRegisters(void);
//void NC_DumpDebugBus(void);
//U32  NC_CheckEWStatus(U8 u8_OpType);
U32  NC_WaitComplete(U16 u16_WaitEvent, U32 u32_MicroSec);
U32 NC_WaitCIFD_Event(U16 u16_WaitEvent, U32	u32_MicroSec);

U32 NC_WaitGetCIFD(U8 * pu8_DataBuf, U32 u32_ByteCnt, U32 u32_MicroSec);
U32 NC_WaitSetCIFD(U8 * pu8_DataBuf, U32 u32_ByteCnt);

//  #define OPTYPE_ERASE        1
//  #define OPTYPE_WRITE        2

//U32 NC_SendReadRetryCmd(U8 u8_RetryIndex, U8 u8_SetToDefault);
// ============================================================
int Disable_Err_log = 0;
//static U8 gu8_ReadRetryHappens = 0;

//========================================================
// HAL function definitions
//========================================================
U32 NC_ResetFCIE(NAND_DRIVER *pNandDrv)
{
	//volatile U16 u16_i=0;
	volatile U16 u16Reg, u16Cnt;

	NC_PlatformResetPre(pNandDrv);

	// soft reset
	REG_CLR_BITS_UINT16(NC_FCIE_RST, BIT_FCIE_SOFT_RST); /* active low */

	//As reset is active, Check Reset Status from 0 -> 1
	#if 1
	u16Cnt=0;
	do
	{
		nand_hw_timer_delay(HW_TIMER_DELAY_1us);
	  	if(0x1000 == u16Cnt++)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR: NC_ResetFCIE Err1, ErrCode: 0x%03X\n", UNFD_ST_ERR_NO_NFIE);
			return UNFD_ST_ERR_NO_NFIE;
		}

		REG_READ_UINT16(NC_FCIE_RST, u16Reg);

	}while (BIT_RST_STS_MASK  != (u16Reg  & BIT_RST_STS_MASK));
	#endif

	//nand_hw_timer_delay(HW_TIMER_DELAY_1ms);
	REG_SET_BITS_UINT16(NC_FCIE_RST, BIT_FCIE_SOFT_RST);
	//nand_hw_timer_delay(HW_TIMER_DELAY_1ms);

	//Restore reset bit, check reset status from 1 -> 0
	#if 1
	u16Cnt=0;
	do
	{
		nand_hw_timer_delay(HW_TIMER_DELAY_1us);
	  	if(0x1000 == u16Cnt++)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR: NC_ResetFCIE Err2, ErrCode: 0x%03X\n", UNFD_ST_ERR_NO_NFIE);
			return UNFD_ST_ERR_NO_NFIE;
		}

		REG_READ_UINT16(NC_FCIE_RST, u16Reg);

	}while (0  != (u16Reg  & BIT_RST_STS_MASK));
	#endif

	NC_PlatformResetPost(pNandDrv);

    //nand_debug(1, 1, "2: %Xh\n", REG(NC_RESERVED_FOR_SW));

	return UNFD_ST_SUCCESS; // ok
}


U32 NC_ConfigNandFlashContext(NAND_DRIVER *pNandDrv)
{
	if (pNandDrv->u16_ECCType < ECC_TYPE_24BIT1KB ||
		pNandDrv->u16_ECCType == ECC_TYPE_32BIT || pNandDrv->u16_ECCType == ECC_TYPE_40BIT || pNandDrv->u16_ECCType == ECC_TYPE_60BIT)
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
	REG_CLR_BITS_UINT16(NC_FUN_CTL, BIT_NC_EN);
	REG_WRITE_UINT16(NC_CTRL , 0);
	// reset NC
	u32_RetVal = NC_ResetFCIE(pNandDrv);
	if (UNFD_ST_SUCCESS != u32_RetVal)
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR: NC_Init, ErrCode:%Xh \r\n", UNFD_ST_ERR_NO_NFIE);
		return u32_RetVal;
	}

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


	u32_RetVal = NC_ReadID(pNandDrv);
	if (u32_RetVal != UNFD_ST_SUCCESS) 
	{
		u32_RetVal = UNFD_ST_ERR_UNKNOWN_ID;
		return u32_RetVal;
	}

	
	return UNFD_ST_SUCCESS;
}

U32 NC_RegInit(NAND_DRIVER *pNandDrv)
{
	// config NC

	pNandDrv->u16_Reg48_Spare = (pNandDrv->u16_SpareByteCnt >> pNandDrv->u8_PageSectorCntBits);
	if (NC_MAX_SECTOR_SPARE_BYTE_CNT < pNandDrv->u16_Reg48_Spare || (pNandDrv->u16_Reg48_Spare & 3))
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR: invalid Sector Spare Size: %Xh bytes!\r\n", pNandDrv->u16_Reg48_Spare);
		nand_die();
	}

	#if defined(SPARE_DMA_ADDR_AUTO_INC) && SPARE_DMA_ADDR_AUTO_INC
	pNandDrv->u16_Reg48_Spare |= BIT_SPARE_ADR_AUTO_INC;
	#endif

	pNandDrv->u16_Reg49_SpareSize = pNandDrv->u16_SpareByteCnt;
	if (NC_MAX_TOTAL_SPARE_BYTE_CNT < pNandDrv->u16_Reg49_SpareSize || (pNandDrv->u16_Reg49_SpareSize & 3))
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR: invalid Total Spare Size: %Xh bytes!\r\n", pNandDrv->u16_Reg49_SpareSize);
		nand_die();
	}

	pNandDrv->u16_Reg50_EccCtrl = REG50_ECC_CTRL_INIT_VALUE;

	switch (pNandDrv->u16_PageByteCnt)
	{
		case 0x0200:
			pNandDrv->u16_Reg50_EccCtrl &= ~BIT_NC_PAGE_SIZE_512Bn;
			pNandDrv->u16_Reg48_Spare |= BIT_NC_ONE_COL_ADDR;
			break;
		case 0x0800:  pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_2KB;  break;
		case 0x1000:  pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_4KB;  break;
		case 0x2000:  pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_8KB;  break;
		case 0x4000:  pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_16KB;  break;
		case 0x8000:  pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_32KB;  break;
		default:
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR: invalid Page Size: %Xh bytes!\r\n", pNandDrv->u16_PageByteCnt);
			nand_die();
	}

	switch (pNandDrv->u16_ECCType)
	{
		case ECC_TYPE_4BIT:
			//FCIE5 not support 4bit ecc -> using 8 bit ecc instead. for NNI compatible.
		case ECC_TYPE_8BIT:
			pNandDrv->u16_Reg50_EccCtrl &= ~BIT_NC_ECC_TYPE_8b512Bn;
			pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_8BIT;
			pNandDrv->u16_ECCCorretableBit = 8;
			break;
		case ECC_TYPE_16BIT:
			pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_16b512B;
			pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_16BIT;
			pNandDrv->u16_ECCCorretableBit = 16;
			break;
		case ECC_TYPE_24BIT:
			pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_24b512B;
			pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_24BIT;
			pNandDrv->u16_ECCCorretableBit = 24;
			break;
		case ECC_TYPE_32BIT:
			pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_32b512B;
			pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_32BIT;
			pNandDrv->u16_ECCCorretableBit = 32;
			break;
		case ECC_TYPE_40BIT:
			pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_40b512B;
			pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_40BIT;
			pNandDrv->u16_ECCCorretableBit = 40;
			break;
		case ECC_TYPE_60BIT:
			pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_60b512B;
			pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_60BIT;
			pNandDrv->u16_ECCCorretableBit = 60;
			break;
		case ECC_TYPE_8BIT1KB:
			pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_8b1KB;
			pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_8BIT1KB;
			pNandDrv->u16_ECCCorretableBit = 8;
			break;
		case ECC_TYPE_16BIT1KB:
			pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_16b1KB;
			pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_16BIT1KB;
			pNandDrv->u16_ECCCorretableBit = 16;
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
		case ECC_TYPE_60BIT1KB:
			pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_60b1KB;
			pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_60BIT1KB;
			pNandDrv->u16_ECCCorretableBit = 60;
			break;
		default:
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR: invalid ECC Type: %Xh \r\n", pNandDrv->u16_ECCType);
			nand_die();
	}
	pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECCERR_NSTOP;
	return UNFD_ST_SUCCESS;
}

void NC_Config(NAND_DRIVER *pNandDrv)
{
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
	// clean int events
	REG_W1C_BITS_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END);

	REG_WRITE_UINT16(NC_FUN_CTL, BIT_NC_EN);

	NC_ResetFCIE(pNandDrv);
    NC_Config(pNandDrv);
}


#if defined(ENABLE_NAND_RIU_MODE) && ENABLE_NAND_RIU_MODE

U32  NC_ReadSector_RIUMode(NAND_DRIVER *pNandDrv, U32 u32_PhyRowIdx, U8 u8_SectorInPage, U16 *pu16_DataInBuf, U8 *pu8_SpareBuf, U8 u8_RandCmdDis)
{
	volatile U16 u16_Tmp, u16_i, u16_j, u16_k, u16_Cnt, u16_ByteLoc;
	U16 u16_Col;
	U8 *pu8_DataBuf = (U8*)pu16_DataInBuf;
    U16 *pu16_DataBuf = pu16_DataInBuf;
	U32 u32_Tmp, u32_Err;
	U16 u16_PageByteCnt = 0, u16_SpareByteCnt = 0, u16_ECCType = 0;

    #if 0
    LDR_PUTS("NC_ReadSector_RIUMode\n");
    LDR_PUTS("Page: ");
    LDR_PUTDW(u32_PhyRowIdx);
    LDR_PUTS("\n");
    LDR_PUTS("Sector: ");
    LDR_PUTX(u8_SectorInPage);
    LDR_PUTS("\n");
    #endif

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

	REG_SET_BITS_UINT16(NC_FUN_CTL, BIT_R2N_MODE_EN); // enable RIU Mode
	REG_WRITE_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END);

	// if Partial Mode (if not 512B/page, set Partial Mode)
	REG_READ_UINT16(NC_ECC_CTRL, u16_Tmp);
	if (u16_Tmp & BIT_NC_PAGE_SIZE_MASK) {
        REG_WRITE_UINT16(NC_PART_MODE, BIT_PARTIAL_MODE_EN|(u8_SectorInPage<<BIT_START_SECTOR_IDX_SHIFT));
	}

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_ADRSET);
	u16_Col = u8_SectorInPage << pNandDrv->u8_SectorByteCntBits;
	REG_WRITE_UINT16(NC_AUXREG_DAT, u16_Col);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx & 0xFFFF);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx >> 16);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (pNandDrv->u8_OpCode_RW_AdrCycle<< 8) | (CMD_0x00));
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_WAITRB << 8) | CMD_0x30);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_BREAK << 8) | ACT_SER_DIN);

	REG_WRITE_UINT16(NC_CTRL, BIT_NC_JOB_START);

	//read for data
	u16_Cnt = pNandDrv->u16_SectorByteCnt >> 6; // how many 64B per sector
	u16_Cnt = (pNandDrv->u16_SectorByteCnt & (0x40 -1)) ? u16_Cnt + 1 : u16_Cnt;
	for (u16_j=0; u16_j<u16_Cnt; u16_j++)
	{   // read data
		u16_Tmp =( (pNandDrv->u16_SectorByteCnt - (u16_j << 6)) >= 0x40) ?
			0x40 : (pNandDrv->u16_SectorByteCnt - (u16_j << 6) );
		u32_Err = NC_WaitGetCIFD((U8*)((U32)pu8_DataBuf + (u16_j << 6)), u16_Tmp, WAIT_READ_TIME);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
		    LDR_PUTS("NC_ReadSectors_RIU: Data NC_WaitGetCIFD fail \n");
			u32_Err = UNFD_ST_ERR_R_TIMEOUT_RM; // timeout
            goto EXIT;
		}
	}

	//read for spare
	u16_Cnt = pNandDrv->u16_SectorSpareByteCnt >> 6; // how many 64B per sector
	u16_Cnt = (pNandDrv->u16_SectorSpareByteCnt & (0x40 -1)) ? u16_Cnt + 1 : u16_Cnt;
	for (u16_j=0; u16_j<u16_Cnt; u16_j++)
	{   // read data
		u16_Tmp =( (pNandDrv->u16_SectorSpareByteCnt - (u16_j << 6)) >= 0x40) ?
			0x40: (pNandDrv->u16_SectorSpareByteCnt - (u16_j << 6));
		u32_Err = NC_WaitGetCIFD( (U8*)((U32)pu8_SpareBuf + (u16_j << 6)), u16_Tmp, WAIT_READ_TIME);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			LDR_PUTS("NC_ReadSectors_RIU: Spare NC_WaitGetCIFD fail \n");
            u32_Err = UNFD_ST_ERR_R_TIMEOUT_RM; // timeout
            goto EXIT;
		}
	}

	// wait for events
	u32_Tmp = 0;
	do{
		u16_Tmp = 0; // do not remove this line
		REG_READ_UINT16(NC_MIE_EVENT, u16_Tmp);
		nand_hw_timer_delay(HW_TIMER_DELAY_1us);
		u32_Tmp++;
		if(u32_Tmp > WAIT_READ_TIME)
  		{
			LDR_PUTS("NC_ReadSectors_RIU: Timeout \n");
			u32_Err = UNFD_ST_ERR_R_TIMEOUT_RM; // timeout
            goto EXIT;
		}
	}while(((u16_Tmp & BIT_NC_R2N_ECCCOR)==0) && ((u16_Tmp & (BIT_NC_JOB_END )) != (BIT_NC_JOB_END )));

    #if 0
    LDR_PUTS("pu8_DataBuf: ");
    LDR_PUTDW(pu8_DataBuf);
    LDR_PUTS("\n");
    for(u16_i=0; u16_i<(pNandDrv->u16_SectorByteCnt>>2); u16_i++)
    {
        LDR_PUTW(pu16_DataBuf[u16_i]);
        if( (u16_i & 0x7) == 0x7 ) LDR_PUTS("\n");
        else LDR_PUTS(" ");
    }
    LDR_PUTS("\n");
    #endif

    #if 0
    LDR_PUTS("pu8_SpareBuf: ");
    LDR_PUTDW(pu8_SpareBuf);
    LDR_PUTS("\n");
    for(u16_i=0; u16_i<(pNandDrv->u16_SectorByteCnt>>2); u16_i++)
    {
        LDR_PUTW(pu8_SpareBuf[u16_i]);
        if( (u16_i & 0x7) == 0x7 ) LDR_PUTS("\n");
        else LDR_PUTS(" ");
    }
    LDR_PUTS("\n");
    #endif

    // if ECC Fail
	REG_READ_UINT16(NC_ECC_STAT2, u16_Tmp);
	if ((u16_Tmp & BIT_NC_ECC_FLGA_MASK) == BIT_NC_ECC_FLGA_FAIL)
	{
		LDR_PUTS("NC_ReadSectors_RIU: ECC Error \n");
        LDR_PUTS("NC_ECC_STAT2: ");
        LDR_PUTW(u16_Tmp);
        LDR_PUTS("\n");
        u32_Err = UNFD_ST_ERR_ECC_FAIL_RM;
        goto EXIT;
	}

	REG_READ_UINT16(NC_MIE_EVENT, u16_Tmp);
	if (u16_Tmp & BIT_NC_R2N_ECCCOR)
	{
//		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "RIU Mode, ECC happens: ");
		// make CIFD available for SW
		// [CAUTION]: before clear BIT_NC_R2N_ECC, JOB_END would not set.
		REG_W1C_BITS_UINT16(NC_MIE_EVENT, BIT_NC_R2N_ECCCOR);
		if(NC_WaitComplete(BIT_NC_JOB_END, WAIT_READ_TIME) == WAIT_READ_TIME)
		{
		    LDR_PUTS("NC_ReadSectors_RIU: ECC correct NC_WaitComplete Timeout 1\n");
			u32_Err = UNFD_ST_ERR_R_TIMEOUT_RM;
            goto EXIT;
		}

		REG_READ_UINT16(NC_ECC_STAT2, u16_Tmp);
		u16_Cnt = (u16_Tmp & BIT_NC_ECC_CNT_MASK) >> BIT_NC_ECC_CNT_SHIFT;
//		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "%u bits\r\n", u16_Cnt);
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
//			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "bit loc: %02Xh \r\n", u16_j);
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

    	u16_Tmp = BIT_NC_R2N_ECCCOR;
  	}

	else if(0 == (u16_Tmp & BIT_NC_R2N_ECCCOR)){
		if (NC_WaitComplete(BIT_NC_JOB_END, WAIT_READ_TIME) == WAIT_READ_TIME)
		{
		    LDR_PUTS("NC_ReadSectors_RIU: ECC correct NC_WaitComplete Timeout 2\n");
			u32_Err = UNFD_ST_ERR_R_TIMEOUT_RM;
            goto EXIT;
		}
	}

    EXIT:

  	REG_WRITE_UINT16(NC_PART_MODE, 0);
	REG_CLR_BITS_UINT16(NC_FUN_CTL, BIT_R2N_MODE_EN);

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
	return UNFD_ST_SUCCESS;
}

#else

U32 NC_ReadSector_DMAMode(NAND_DRIVER *pNandDrv, U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U8 u8_RandCmdDis)
{
	U32 u32_Tmp, u32_SectorCnt_Tmp;
    U32 u32_Err = UNFD_ST_SUCCESS;
	U16 u16_PageCnt, u16_Tmp, u16_Val;
	U32 u32_DataDMAAddr;
	U32 u32_SpareDMAAddr;
    U16 u16_PageByteCnt = 0, u16_SpareByteCnt = 0, u16_ECCType = 0;

    #if 0
    LDR_PUTS("NC_ReadSector_DMAMode\n");
    LDR_PUTS("Page: ");
    LDR_PUTDW(u32_PhyRowIdx);
    LDR_PUTS("\n");
    LDR_PUTS("Sector: ");
    LDR_PUTX(u8_SectorInPage);
    LDR_PUTS("\n");
    #endif

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

	REG_WRITE_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END);
	REG_CLR_BITS_UINT16(NC_FUN_CTL, BIT_R2N_MODE_EN);

	//-----------------------------------------------
	u32_DataDMAAddr = nand_translate_DMA_address_Ex((U32)pu8_DataBuf, pNandDrv->u16_SectorByteCnt);
	u32_SpareDMAAddr = nand_translate_DMA_address_Ex((U32)pu8_SpareBuf, pNandDrv->u16_SectorSpareByteCnt);

	REG_WRITE_UINT16(NC_RSPARE_DMA_ADR0, u32_SpareDMAAddr & 0xFFFF);
	REG_WRITE_UINT16(NC_RSPARE_DMA_ADR1, u32_SpareDMAAddr >>16);

	// if not 512B/page, set Partial Mode
	REG_READ_UINT16(NC_ECC_CTRL, u16_Tmp);

    REG_WRITE_UINT16(NC_PART_MODE, BIT_PARTIAL_MODE_EN|(u8_SectorInPage<<BIT_START_SECTOR_IDX_SHIFT));
	REG_WRITE_UINT16(NC_RDATA_DMA_ADR0, u32_DataDMAAddr & 0xFFFF);
	REG_WRITE_UINT16(NC_RDATA_DMA_ADR1, u32_DataDMAAddr >> 16);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_ADRSET);
    REG_WRITE_UINT16(NC_AUXREG_DAT, u8_SectorInPage << pNandDrv->u8_SectorByteCntBits);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx & 0xFFFF);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx >> 16);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (pNandDrv->u8_OpCode_RW_AdrCycle<< 8) | (CMD_0x00));
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_WAITRB << 8) | CMD_0x30);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_BREAK << 8) | ACT_SER_DIN);

    if( (U32)pu8_DataBuf < 0x20000000 ) // DMA to SRAM
    {
        REG_SET_BITS_UINT16(NC_FUN_CTL, BIT_BOOT_MODE);
        REG_SET_BITS_UINT16(NC_BOOT_MODE, BIT_BOOT_SRAM_SEL);
        REG_SET_BITS_UINT16(NC_BOOT_CONFIG, BIT_BOOT_MODE_EN);
    }

	REG_WRITE_UINT16(NC_CTRL, BIT_NC_JOB_START);

	if (NC_WaitComplete(BIT_NC_JOB_END, WAIT_READ_TIME) == (WAIT_READ_TIME))
	{
        LDR_PUTS("NC_ReadSector_DMAMode: Timeout\n");
		u32_Err = UNFD_ST_ERR_R_TIMEOUT;
        goto EXIT;
	}

    if( (U32)pu8_DataBuf < 0x20000000 ) // DMA to SRAM
    {
        REG_CLR_BITS_UINT16(NC_BOOT_MODE, BIT_BOOT_SRAM_SEL);
        REG_CLR_BITS_UINT16(NC_FUN_CTL, BIT_BOOT_MODE);
        REG_CLR_BITS_UINT16(NC_BOOT_CONFIG, BIT_BOOT_MODE_EN);
    }

	// Check ECC
	REG_READ_UINT16(NC_ECC_STAT0, u16_Tmp);
	if (u16_Tmp & BIT_NC_ECC_FAIL)
	{
	    if(pNandDrv->u8_RequireRandomizer && IF_LFSR_ENABLED())
			REG_READ_UINT16(NC_NAND_ZERO_BITCNT,u16_Val);
		else
			REG_READ_UINT16(NC_FCIE_ZERO_BITCNT,u16_Val);
		if(u16_Val <= pNandDrv->u16_ECCCorretableBit)
		{
			nand_memset(pu8_DataBuf, 0xFF, pNandDrv->u16_SectorByteCnt);
            nand_memset(pu8_SpareBuf, 0xFF, pNandDrv->u16_SectorSpareByteCnt);

			// No need to reset read retry status
			// Clean False Alarm Status
			NC_ReConfig(pNandDrv);
			u32_Err = UNFD_ST_SUCCESS;
		}
        else
        {
			LDR_PUTS("NC_ReadSector_DMAMode: ECC fail\n");
			u32_Err = UNFD_ST_ERR_ECC_FAIL;
        }
    }

    EXIT:
	REG_WRITE_UINT16(NC_PART_MODE, 0);

    #if 0
    {
        U16 u16_i;
        U16 *pu16_DataBuf = (U16*)pu8_DataBuf;
        LDR_PUTS("pu8_DataBuf: ");
        LDR_PUTDW(pu8_DataBuf);
        LDR_PUTS("\n");
        for(u16_i=0; u16_i<(pNandDrv->u16_SectorByteCnt>>2); u16_i++)
        {
            LDR_PUTW(pu16_DataBuf[u16_i]);
            if( (u16_i & 0x7) == 0x7 ) LDR_PUTS("\n");
            else LDR_PUTS(" ");
        }
        LDR_PUTS("\n");
    }
    #endif

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

U32 NC_ReadID(NAND_DRIVER *pNandDrv)
{
	volatile U16 u16_Reg;
	U16 u16_i;
	U16 u16_IDByteCnt;


	// data go through CIFD
	REG_SET_BITS_UINT16(NC_FUN_CTL,BIT_R2N_MODE_EN);

	REG_WRITE_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_ADRSET);
	REG_WRITE_UINT16(NC_AUXREG_DAT, 0);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ADR_C2T1S0 << 8) | CMD_0x90);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_BREAK << 8) | ACT_RAN_DIN);

	u16_IDByteCnt = NAND_ID_BYTE_CNT+1;

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_RAN_CNT);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u16_IDByteCnt);
	REG_WRITE_UINT16(NC_AUXREG_DAT, 0); /*offset 0*/

	REG_WRITE_UINT16(NC_CTRL, BIT_NC_JOB_START);

	//polling for wbuf is full, means wbuffer is full or random data is done
	if(NC_WaitCIFD_Event(BIT_WBUF_FULL, DELAY_100ms_in_us) == DELAY_100ms_in_us)
	{
		REG_CLR_BITS_UINT16(NC_FUN_CTL,BIT_R2N_MODE_EN);
		return UNFD_ST_ERR_R_TIMEOUT;
	}

	NC_GetCIFD(pNandDrv->au8_ID, 0, NAND_ID_BYTE_CNT);


	REG_W1C_BITS_UINT16(NC_CIFD_EVENT, BIT_WBUF_FULL);
	REG_SET_BITS_UINT16(NC_CIFD_EVENT, BIT_WBUF_EMPTY_TRI);
	
	if (NC_WaitComplete(BIT_NC_JOB_END, DELAY_100ms_in_us) == DELAY_100ms_in_us)
	{
		REG_CLR_BITS_UINT16(NC_FUN_CTL,BIT_R2N_MODE_EN);
		return UNFD_ST_ERR_R_TIMEOUT; // timeout
	}
	

    if(0 == pNandDrv->u8_IDByteCnt)
    {
        for (u16_i=2; u16_i<(NAND_ID_BYTE_CNT-1); u16_i++)
            if(pNandDrv->au8_ID[u16_i] == pNandDrv->au8_ID[0] && 
                pNandDrv->au8_ID[u16_i+1] == pNandDrv->au8_ID[1])
                break;
        pNandDrv->u8_IDByteCnt = u16_i;
    }

	REG_CLR_BITS_UINT16(NC_FUN_CTL,BIT_R2N_MODE_EN);
	return UNFD_ST_SUCCESS; // ok
}

static U32 NC_ReadPages_Ex(NAND_DRIVER *pNandDrv, U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt)
{
	U32 u32_DataDMAAddr;
	U32 u32_SpareDMAAddr = 0;
    U16 u16_Tmp, u16_Val;

	REG_WRITE_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END);
	REG_CLR_BITS_UINT16(NC_FUN_CTL, BIT_R2N_MODE_EN);
	
	u32_DataDMAAddr = nand_translate_DMA_address_Ex((U32)pu8_DataBuf, pNandDrv->u16_PageByteCnt * u32_PageCnt);
	u32_SpareDMAAddr = nand_translate_DMA_address_Ex((U32)pu8_SpareBuf, pNandDrv->u16_SpareByteCnt);

	
	REG_WRITE_UINT16(NC_RSPARE_DMA_ADR0, u32_SpareDMAAddr & 0xFFFF);
	REG_WRITE_UINT16(NC_RSPARE_DMA_ADR1, u32_SpareDMAAddr >>16);

	REG_WRITE_UINT16(NC_RDATA_DMA_ADR0, u32_DataDMAAddr & 0xFFFF);
	REG_WRITE_UINT16(NC_RDATA_DMA_ADR1, u32_DataDMAAddr >> 16);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_ADRSET);
	REG_WRITE_UINT16(NC_AUXREG_DAT, 0);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx & 0xFFFF);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx >> 16);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (pNandDrv->u8_OpCode_RW_AdrCycle<< 8) | (CMD_0x00));

	if (pNandDrv->u16_Reg48_Spare& BIT_NC_ONE_COL_ADDR)
	{ // if a page 512B
		REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_SER_DIN << 8) | ACT_WAITRB);
		REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_BREAK << 8) | ACT_REPEAT);
	}
	else
	{
		REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_WAITRB << 8) | CMD_0x30);
		REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_REPEAT << 8) | ACT_SER_DIN);
		REG_WRITE_UINT16(NC_AUXREG_DAT, ACT_BREAK);
	}
	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_RPTCNT);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PageCnt - 1);
    if( (U32)pu8_DataBuf < 0x20000000 ) // DMA to SRAM
    {
        REG_SET_BITS_UINT16(NC_FUN_CTL, BIT_BOOT_MODE);
        REG_SET_BITS_UINT16(NC_BOOT_MODE, BIT_BOOT_SRAM_SEL);
        REG_SET_BITS_UINT16(NC_BOOT_CONFIG, BIT_BOOT_MODE_EN);
    }

	REG_WRITE_UINT16(NC_CTRL,  BIT_NC_JOB_START);

	if (NC_WaitComplete(BIT_NC_JOB_END, WAIT_READ_TIME * u32_PageCnt) ==
	  (WAIT_READ_TIME * u32_PageCnt ))
	{
        LDR_PUTS("Error: NC_ReadPages Timeout\n");
		return UNFD_ST_ERR_R_TIMEOUT;	
	}

    if( (U32)pu8_DataBuf < 0x20000000 ) // DMA to SRAM
    {
        REG_CLR_BITS_UINT16(NC_BOOT_MODE, BIT_BOOT_SRAM_SEL);
        REG_CLR_BITS_UINT16(NC_FUN_CTL, BIT_BOOT_MODE);
        REG_CLR_BITS_UINT16(NC_BOOT_CONFIG, BIT_BOOT_MODE_EN);
    }
	// Check ECC
	REG_READ_UINT16(NC_ECC_STAT0, u16_Tmp);
	if (u16_Tmp & BIT_NC_ECC_FAIL)
	{
        // FCIE5 check ZERO_COUNT register which records max zero count of a sector during this DMA
	    if(pNandDrv->u8_RequireRandomizer && IF_LFSR_ENABLED())
			REG_READ_UINT16(NC_NAND_ZERO_BITCNT,u16_Val);
		else
			REG_READ_UINT16(NC_FCIE_ZERO_BITCNT,u16_Val);
		if(u16_Val <= pNandDrv->u16_ECCCorretableBit)
		{
			// when meet 1 empty page, all rest of pages in the same block are empty.
			nand_memset(pu8_DataBuf, 0xFF, pNandDrv->u16_PageByteCnt);
            nand_memset(pu8_SpareBuf, 0xFF, pNandDrv->u16_SpareByteCnt);

			// No need to reset read retry status
			// Clean False Alarm Status
			NC_ReConfig(pNandDrv);
		}
        else
        {
			LDR_PUTS("NC_ReadPages: ECC fail\n");
			return UNFD_ST_ERR_ECC_FAIL;
        }
    }	


    return UNFD_ST_SUCCESS;

}

U32 NC_ReadPages(NAND_DRIVER *pNandDrv, U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt)
{
    U32 u32_i, u32_k;
    U32 u32_Err;
	U32 u32_UECCRow, u32_UECCRowTmp, u32_UECCPageCnt;
    U8  *pu8_UECCDataBuf, *pu8_UECCSpareBuf;

    u32_UECCRow = u32_PhyRowIdx;
    u32_UECCPageCnt = u32_PageCnt;
    pu8_UECCDataBuf = pu8_DataBuf;
    pu8_UECCSpareBuf = pu8_SpareBuf;
	
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

	REG_WRITE_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END);

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
		//nand_reset_WatchDog();
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

  	if (NC_WaitComplete(BIT_NC_JOB_END, WAIT_RESET_TIME) == WAIT_RESET_TIME)
  	{
		NC_ResetFCIE(pNandDrv);
        NC_Config(pNandDrv);
		return UNFD_ST_ERR_RST_TIMEOUT;
  	}

  	return UNFD_ST_SUCCESS;
}

U32 NC_WaitCIFD_Event(U16 u16_WaitEvent, U32  u32_MicroSec)
{
	volatile U32 u32_Count;
	volatile U16 u16_Reg;

	for (u32_Count=0; u32_Count < u32_MicroSec; u32_Count++)
	{
		REG_READ_UINT16(NC_CIFD_EVENT, u16_Reg);
		if ((u16_Reg & u16_WaitEvent) == u16_WaitEvent)
			break;

		nand_hw_timer_delay(HW_TIMER_DELAY_1us);
	}
	if (u32_Count >= u32_MicroSec)
	{
		REG_READ_UINT16(NC_CIFD_EVENT, u16_Reg);
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "CIFD Event Timout %X\n", u16_Reg);
		if(Disable_Err_log == 0)
		{
			//NC_DumpDebugBus();
			//NC_DumpRegisters();
		}
	}
	return u32_Count;
}

U32 NC_WaitGetCIFD(U8 * pu8_DataBuf, U32 u32_ByteCnt, U32 u32_MicroSec)
{
	if(u32_ByteCnt > NC_CIFD_WBUF_BYTE_CNT)
	{
		return UNFD_ST_ERR_INVAL_CIFD_CNT;
	}
	if (NC_WaitCIFD_Event(BIT_WBUF_FULL, u32_MicroSec)== (u32_MicroSec ))
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Error: CIFD timeout 0, ErrCode:%Xh\r\n", UNFD_ST_ERR_R_TIMEOUT_RM);
		REG_WRITE_UINT16(NC_PART_MODE, 0);
		REG_CLR_BITS_UINT16(NC_FUN_CTL, BIT_R2N_MODE_EN);
		return UNFD_ST_ERR_R_TIMEOUT_RM; // timeout
	}
	NC_GetCIFD(pu8_DataBuf , 0, u32_ByteCnt);

	REG_WRITE_UINT16(NC_CIFD_EVENT,BIT_WBUF_FULL);

	REG_WRITE_UINT16(NC_CIFD_EVENT, BIT_WBUF_EMPTY_TRI);

	return UNFD_ST_SUCCESS;
}


//extern void NC_Test_Reg717274_Set(void);
//extern void NC_Test_Reg717274_Get(void);
U32 NC_WaitComplete(U16 u16_WaitEvent, U32 u32_MicroSec)
{
	volatile U32 u32_Count;
	#if defined(ENABLE_NAND_INTERRUPT_MODE) && ENABLE_NAND_INTERRUPT_MODE
	volatile U32 u32_Err;
	#endif
	volatile U16 u16_Reg;

	#if defined(ENABLE_NAND_INTERRUPT_MODE) && ENABLE_NAND_INTERRUPT_MODE
	REG_READ_UINT16(NC_MIE_INT_EN, u16_Reg);
	if(u16_Reg & u16_WaitEvent)
	{
		u32_Err = nand_WaitCompleteIntr(u16_WaitEvent, u32_MicroSec);
		if( u32_Err == UNFD_ST_SUCCESS )
			return 0;
		else
		{
			//NC_DumpDebugBus();
			//NC_DumpRegisters();

			nand_die();
			return u32_MicroSec;
		}
	}
	#endif

	for (u32_Count=0; u32_Count < u32_MicroSec; u32_Count++)
	{
		REG_READ_UINT16(NC_MIE_EVENT, u16_Reg);
		if ((u16_Reg & u16_WaitEvent) == u16_WaitEvent)
			break;

		nand_hw_timer_delay(HW_TIMER_DELAY_1us);
		//nand_reset_WatchDog();
	}

	if (u32_Count < u32_MicroSec)
		REG_W1C_BITS_UINT16(NC_MIE_EVENT, u16_WaitEvent); /*clear events*/
	else
	{
		if(Disable_Err_log == 0)
		{
			//NC_DumpDebugBus();
			//NC_DumpRegisters();
		}
	}

	return u32_Count;
}


void NC_SetCIFD_Const(U8 u8_Data, U32 u32_CIFDPos, U32 u32_ByteCnt)
{
	U32 u32_i;
	volatile U16 u16_Tmp;

	if (u32_CIFDPos & 1) {
		REG_READ_UINT16(NC_RBUF_CIFD_ADDR(u32_CIFDPos>>1), u16_Tmp);
		u16_Tmp &= 0x00FF;
		u16_Tmp += u8_Data << 8;
		//nand_debug(1, 1, "0: %X \r\n", u16_Tmp);
		REG_WRITE_UINT16(NC_RBUF_CIFD_ADDR(u32_CIFDPos>>1), u16_Tmp);
		u32_CIFDPos += 1;
		u32_ByteCnt -= 1;
	}

	for (u32_i=0; u32_i<u32_ByteCnt>>1; u32_i++) {
		u16_Tmp = u8_Data + (u8_Data << 8);
		//nand_debug(1, 1, "1: %X \r\n", u16_Tmp);
		REG_WRITE_UINT16(NC_RBUF_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
	}

	if (u32_ByteCnt - (u32_i<<1)) {
		REG_READ_UINT16(NC_RBUF_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
		u16_Tmp = (u16_Tmp & 0xFF00) + u8_Data;
		//nand_debug(1, 1, "2: %X \r\n", u16_Tmp);
		REG_WRITE_UINT16(NC_RBUF_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
	}
}

void NC_SetCIFD(U8 *pu8_Buf, U32 u32_CIFDPos, U32 u32_ByteCnt)
{
	U32 u32_i, u32_BufPos;
	volatile U16 u16_Tmp;

	if (u32_CIFDPos & 1) {
		REG_READ_UINT16(NC_RBUF_CIFD_ADDR(u32_CIFDPos>>1), u16_Tmp);
		u16_Tmp &= 0x00FF;
		u16_Tmp += pu8_Buf[0] << 8;
		//nand_debug(1, 1, "0: %X \r\n", u16_Tmp);
		REG_WRITE_UINT16(NC_RBUF_CIFD_ADDR(u32_CIFDPos>>1), u16_Tmp);
		u32_CIFDPos += 1;
		u32_ByteCnt -= 1;
		u32_BufPos = 1;
	} else
		u32_BufPos = 0;

	for (u32_i=0; u32_i<u32_ByteCnt>>1; u32_i++) {
		u16_Tmp = pu8_Buf[(u32_i<<1)+u32_BufPos] +
		      (pu8_Buf[(u32_i<<1)+u32_BufPos+1] << 8);
		//nand_debug(1, 1, "1: %X \r\n", u16_Tmp);
		REG_WRITE_UINT16(NC_RBUF_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
	}

	if (u32_ByteCnt - (u32_i<<1)) {
		REG_READ_UINT16(NC_RBUF_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
		u16_Tmp = (u16_Tmp & 0xFF00) + pu8_Buf[(u32_i<<1)+u32_BufPos];
		//nand_debug(1, 1, "2: %X \r\n", u16_Tmp);
		REG_WRITE_UINT16(NC_RBUF_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
	}
}

void NC_GetRBufCIFD(U8 *pu8_Buf, U32 u32_CIFDPos, U32 u32_ByteCnt)
{
	U32 u32_i, u32_BufPos;
	U16 u16_Tmp;

	if (u32_CIFDPos & 1) {
		REG_READ_UINT16(NC_RBUF_CIFD_ADDR(u32_CIFDPos>>1), u16_Tmp);
		pu8_Buf[0] = (U8)(u16_Tmp >> 8);
		u32_CIFDPos += 1;
		u32_ByteCnt -= 1;
		u32_BufPos = 1;
	} else
		u32_BufPos = 0;

	for (u32_i=0; u32_i<u32_ByteCnt>>1; u32_i++) {
		REG_READ_UINT16(NC_RBUF_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
		pu8_Buf[(u32_i<<1)+u32_BufPos] = (U8)u16_Tmp;
		pu8_Buf[(u32_i<<1)+u32_BufPos+1] = (U8)(u16_Tmp>>8);
	}

	if (u32_ByteCnt - (u32_i<<1)) {
		REG_READ_UINT16(NC_RBUF_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
		pu8_Buf[(u32_i<<1)+u32_BufPos] = (U8)u16_Tmp;
	}

}

void NC_GetCIFD(U8 *pu8_Buf, U32 u32_CIFDPos, U32 u32_ByteCnt)
{
	U32 u32_i, u32_BufPos;
	U16 u16_Tmp;

	if (u32_CIFDPos & 1) {
		REG_READ_UINT16(NC_WBUF_CIFD_ADDR(u32_CIFDPos>>1), u16_Tmp);
		pu8_Buf[0] = (U8)(u16_Tmp >> 8);
		u32_CIFDPos += 1;
		u32_ByteCnt -= 1;
		u32_BufPos = 1;
	} else
		u32_BufPos = 0;

	for (u32_i=0; u32_i<u32_ByteCnt>>1; u32_i++) {
		REG_READ_UINT16(NC_WBUF_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
		pu8_Buf[(u32_i<<1)+u32_BufPos] = (U8)u16_Tmp;
		pu8_Buf[(u32_i<<1)+u32_BufPos+1] = (U8)(u16_Tmp>>8);
	}

	if (u32_ByteCnt - (u32_i<<1)) {
		REG_READ_UINT16(NC_WBUF_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
		pu8_Buf[(u32_i<<1)+u32_BufPos] = (U8)u16_Tmp;
	}

}
/*lint -restore */

U32 NC_ReadStatus(NAND_DRIVER *pNandDrv)
{
	U8 u8Status = 0;

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_CHKSTATUS<<8) | CMD_0x70);
	REG_WRITE_UINT16(NC_AUXREG_DAT, ACT_BREAK);

	REG_WRITE_UINT16(NC_CTRL, BIT_NC_JOB_START);

	if(NC_WaitComplete(BIT_NC_JOB_END, DELAY_100ms_in_us) == DELAY_100ms_in_us)
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Error: NC_ReadStatus Timeout, ErrCode:%Xh \r\n", UNFD_ST_ERR_R_TIMEOUT);

		return UNFD_ST_ERR_R_TIMEOUT;
	}

	u8Status = (U8)REG(NC_ST_READ);
	NC_SetCIFD(&u8Status, 0, 1);


	return UNFD_ST_SUCCESS;
}


#endif // NC_SEL_FCIE5
#endif
